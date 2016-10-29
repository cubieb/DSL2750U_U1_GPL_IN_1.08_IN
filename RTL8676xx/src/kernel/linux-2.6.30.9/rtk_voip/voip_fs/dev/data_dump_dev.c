/*
 * In our coding, 'DD' is short for data dump. 
 * This kind of device is useful for dump arbitrary data especially 
 * PCM data, and it only needs very little cost. 
 * In kernel part, add a *SINGLE* function to tell this module the 
 * data you want to dump. 
 * In application, just use 'cat' command to read data from device. 
 *
 * Note: 
 *  1. This module doesn't know data structure and all are seen as 
 *     byte stream. 
 *  2. Data in single writing operation is seens as an element, so
 *     we write nothing if free buffer size is not enough. 
 */

#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/sched.h>

#include "rtk_voip.h"
#include "voip_init.h"
#include "voip_dev.h"

#define VOIP_DD_DEV_NAME	"DD"

#define NUM_OF_DD_DEV	( VOIP_DD_DEV_END - VOIP_DD_DEV_START + 1 )
#define DD_BUFF_SIZE	( 80 * 2 * 100 )	// 1 sec in narrow band mode 

// ==================================================================
// Data dump fifo 
// ==================================================================

typedef struct {
	// sync. 
	wait_queue_head_t wq;
	// fifo
	int ri, wi;
	unsigned char data[ DD_BUFF_SIZE ];
} ddinst_t;

static ddinst_t *ddinst[ NUM_OF_DD_DEV ];

static int check_and_get_ddinst_idx( unsigned int minor )
{
	unsigned int n = minor - VOIP_DD_DEV_START;
	
	if( n < NUM_OF_DD_DEV )
		return n;
	
	return -1;
}

static int ddinst_write_internal( ddinst_t *pddinst, char *buff, size_t count )
{
	size_t space, stage1_len, stage2_len;
			
	// calculate space 
	if( pddinst ->wi >= pddinst ->ri ) {
		space = DD_BUFF_SIZE - pddinst ->wi + pddinst ->ri;
	} else {
		space = pddinst ->ri - pddinst ->wi;
	}
	
	// check whether space is enough 
	if( space <= count ) {		// '=' because (wi==ri) indicates empty 
		return -1;
	}
	
	// write it! (stage 1, wrap case)
	if( pddinst ->wi + count >= DD_BUFF_SIZE ) {
		stage1_len = DD_BUFF_SIZE - pddinst ->wi;
		memcpy( pddinst ->data + pddinst ->wi, buff, stage1_len );
		
		pddinst ->wi = 0;
		buff += stage1_len;
	} else
		stage1_len = 0;
	
	// copy it! (stage 2)
	stage2_len = count - stage1_len;
	memcpy( pddinst ->data + pddinst ->wi, buff, stage2_len );
	
	pddinst ->wi += stage2_len;
	
	// wake up reading process 
	wake_up_interruptible( &pddinst ->wq );
	
	return 0;
}

void ddinst_write( unsigned int minor, char *buff, size_t count )
{
	int idx;
	
	idx = check_and_get_ddinst_idx( minor );
	
	if( idx < 0 || ddinst[ idx ] == NULL )
		return;
		
	if( ddinst_write_internal( ddinst[ idx ], buff, count ) < 0 )
		printk( "PRF%X ", minor );
}

static ssize_t ddinst_read( ddinst_t *pddinst, struct file *filp, char *buff, size_t count )
{
	size_t data_len, stage1_len, stage2_len;
		
	// wait for some data 
	while( pddinst ->ri == pddinst ->wi ) {
		
		if( filp ->f_flags & O_NONBLOCK )
			return -EAGAIN;
		
		if( wait_event_interruptible( pddinst ->wq, pddinst ->ri != pddinst ->wi ) )
			return -ERESTARTSYS;	/* signal: tell the fs layer to handle it */
	}
	
	// calculate data length 
	if( pddinst ->ri > pddinst ->wi )
		data_len = DD_BUFF_SIZE - pddinst ->ri + pddinst ->wi;
	else
		data_len = pddinst ->wi - pddinst ->ri;
	
	// truncate copy size  
	if( data_len > count )
		data_len = count;
	
	// copy it! (stage 1, wrap case)
	if( pddinst ->ri + data_len >= DD_BUFF_SIZE ) {
		stage1_len = DD_BUFF_SIZE - pddinst ->ri;
		copy_to_user( buff, pddinst ->data + pddinst ->ri, stage1_len );
		
		pddinst ->ri = 0;
		buff += stage1_len;
	} else
		stage1_len = 0;
	
	// copy it! (stage 2)
	stage2_len = data_len - stage1_len;
	copy_to_user( buff, pddinst ->data + pddinst ->ri, stage2_len );
	
	pddinst ->ri += stage2_len;
	
	return data_len;
}

static int ddinst_open( unsigned int minor, void **pp_pd )
{
	int idx;
	ddinst_t *pddinst;
	int ret;
	
	idx = check_and_get_ddinst_idx( minor );
	
	if( idx < 0 )
		return -ENODEV;
	
	// check inst exist? 
	if( ( pddinst = ddinst[ idx ] ) )
		return -EBUSY;
	
	ddinst[ idx ] = ( ddinst_t * )1;		// make it non-zero to avoid race condition 
	
	// allocate space 
	pddinst = kmalloc( sizeof( ddinst_t ), GFP_KERNEL );
	
	if( pddinst == NULL ) {
		ret = -ENOMEM;
		goto label_err;
	}
	
	// init wait queue
	init_waitqueue_head( &pddinst ->wq );
	
	// init fifo 
	pddinst ->ri = pddinst ->wi = 0;
	
	// save pointer for reading and writing function 
	ddinst[ idx ] = *pp_pd = pddinst;
	
	return 0;

label_err:
	ddinst[ idx ] = 0;

	return ret;
}

static int ddinst_close( unsigned int minor, ddinst_t *pddinst )
{
	int idx;
		
	idx = check_and_get_ddinst_idx( minor );
	
	if( idx < 0 )
		return -ENODEV;
	
	// clean pointer 
	ddinst[ idx ] = NULL;
	
	// free memory 
	kfree( pddinst );
	
	return 0;
}

// ==================================================================
// dev f_op 
// ==================================================================

static ssize_t dd_read( struct file *filp, char *buff, size_t count, loff_t *offp )
{
	return ddinst_read( ( ddinst_t * )filp ->private_data, filp, buff, count );
}

static int dd_open( struct inode *node, struct file *filp )
{
	int ret;
	void *p_pd;	// private data 
	unsigned int minor;
	
	minor = MINOR( node ->i_rdev );
	
	ret = ddinst_open( minor, &p_pd );
	
	if( ret < 0 )
		return ret;
	
	filp ->private_data = p_pd;
	
	return 0;
}

static int dd_close( struct inode *node, struct file *filp )
{
	int ret;
	unsigned int minor;
	
	minor = MINOR( node ->i_rdev );
	
	ret = ddinst_close( minor, ( ddinst_t * )filp ->private_data );
	
	if( ret < 0 )
		return ret;
	
	return 0;
}

static struct file_operations dd_fops = {
	read:		dd_read,
	open:		dd_open,
	release:	dd_close,
};

static int __init voip_dd_dev_init( void )
{
	int ret;
	
	ret = register_voip_chrdev( VOIP_DD_DEV_START, NUM_OF_DD_DEV, VOIP_DD_DEV_NAME, &dd_fops );
	
	if( ret < 0 )
		printk( "register data dump dev fail\n" );
	
	return 0;
}

static void __exit voip_dd_dev_exit( void )
{
	unregister_voip_chrdev( VOIP_DD_DEV_START, NUM_OF_DD_DEV );
}

voip_initcall( voip_dd_dev_init );
voip_exitcall( voip_dd_dev_exit );

