#ifndef __VOIP_PROC_H__
#define __VOIP_PROC_H__
#include <linux/proc_fs.h>

#define PROC_VOIP_DIR			"voip"
#define PROC_VOIP_CH_FORMAT		"ch%u"
#define PROC_VOIP_SS_FORMAT		"ss%u"
#define PROC_VOIP_DECT_DIR		"dect"
#define PROC_VOIP_CH_MAPS_DIR	"maps"
#define PROC_VOIP_PCM_DIR		"pcm"

/* 
 * We use the last parameter of create_proc_read_entry() to encode 
 * proc path. 
 */
/*
 * bit0-6: channel ID or session ID 
 * bit7: channel(1) or session(0)
 */
#define PROC_DATA_BIT_CH			0x80
#define PROC_DATA_FROM_CH( ch )		( PROC_DATA_BIT_CH | ( ch & 0x7F ) )
#define PROC_DATA_FROM_SS( ss )		( 0                | ( ss & 0x7F ) )

#define CH_FROM_PROC_DATA( data )	( ( ( unsigned long )data & PROC_DATA_BIT_CH ) ? \
									  ( ( unsigned long )data & 0x7F ) : 0xFF )
#define SS_FROM_PROC_DATA( data )	( !( ( unsigned long )data & PROC_DATA_BIT_CH ) ? \
									   ( ( unsigned long )data & 0x7F ) : 0xFF )
#define IS_CH_PROC_DATA( data )		( ( unsigned long )data & PROC_DATA_BIT_CH )


/*
 * Create entries for channel or session 
 */
static inline void create_voip_channel_proc_read_entry(
	const char * name,
	read_proc_t * read_proc )
{
	extern int voip_ch_num;

	int i;
	char buf[ 256 ];
		
	for( i = 0; i < voip_ch_num; i ++ ) {	// 'voip/ch%u/'
		sprintf( buf, PROC_VOIP_DIR "/" PROC_VOIP_CH_FORMAT "/%s", i, name );
		create_proc_read_entry( buf, 0, NULL, read_proc, ( void * )PROC_DATA_FROM_CH( i ) );
	}
}

static inline void create_voip_session_proc_read_entry(
	const char * name,
	read_proc_t * read_proc )
{
	extern int sess_num;

	int i;
	char buf[ 256 ];
		
	for( i = 0; i < sess_num; i ++ ) {	// 'voip/ss%u/'
		sprintf( buf, PROC_VOIP_DIR "/" PROC_VOIP_SS_FORMAT "/%s", i, name );
		create_proc_read_entry( buf, 0, NULL, read_proc, ( void * )PROC_DATA_FROM_SS( i ) );
	}
}

/*
 * Remove entries from channel or session 
 */
static inline void remove_voip_channel_proc_entry(
	const char * name )
{
	extern int voip_ch_num;

	int i;
	char buf[ 256 ];
	
	for( i = 0; i < voip_ch_num; i ++ ) {	// 'voip/ch%u/'
		sprintf( buf, PROC_VOIP_DIR "/" PROC_VOIP_CH_FORMAT "/%s", i, name );
		remove_proc_entry( buf, NULL );
	}
}

static inline void remove_voip_session_proc_entry(
	const char * name )
{
	extern int sess_num;

	int i;
	char buf[ 256 ];
	
	for( i = 0; i < sess_num; i ++ ) {	// 'voip/ss%u/'
		sprintf( buf, PROC_VOIP_DIR "/" PROC_VOIP_SS_FORMAT "/%s", i, name );
		remove_proc_entry( buf, NULL );
	}
}


#endif /* __VOIP_PROC_H__ */

