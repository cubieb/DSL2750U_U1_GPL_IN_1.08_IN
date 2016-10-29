/**************************************************************
$Workfile:   verilog_test.h  $
***************************************************************
***************************************************************
**							    												
**	Filename    : verilog_test.h.c	  		    									
**	Description : c include file included inside every test file	 					
**	Author	    : Samir Palnitkar   									
**	Date	    : 9/7/2001				 								
**	Notes	    :		  	   		    										
**      References  : 
**	Revision History :
**							    												
****************************************************************/
/***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
***************************************************************
$Revision: 1.1 $
$Date: 2008/07/18 11:08:35 $
**************************************************************/

#define CELL_SIZE	0x35 /* 53 bytes */ 

/* variable types */
#ifndef __VAR_DEFINITIONS__
#define __VAR_DEFINITIONS__
#define UINT64  unsigned long long int
#define LONG	signed long
#define UINT32  unsigned long
#define SHORT	signed short
#define UINT16  unsigned short
#define BYTE	unsigned char
#define WORD  unsigned short
#endif /* __VAR_DEFINITIONS__ */

/*Include Extern Files*/
extern UINT32 pio_read (UINT32 destAddr, UINT32 *data);
extern UINT32 pio_read_return(UINT32 srcAddr);
extern UINT32 pio_read_check(UINT32 srcAddr, UINT32 expectedData);
extern UINT32 PIO_READ (UINT32 destAddr);
extern UINT32 pio_write (UINT32 destAddr, UINT32 data);
extern UINT32 PIO_WRITE (UINT32 destAddr, UINT32 data);
extern void wait_for_fifo_data (UINT32 blockName, /*Identifies block type
						XFI_BLOCK, YFI_BLOCK, ZFI_BLOCK etc*/
			UINT32 subBlockName, /*Name of sub block, eg for YFI, TOFT, CYFT_A, CYFT_B etc*/
			UINT32 regA, UINT32 regB, UINT32 regC,
			UINT32 dataSize, /*number of bytes to wait for*/
			UINT32 computeAvailableData /*1 means check for fullness of FIFO
						      0 means check for emptiness of FIFO*/
			);

extern void write_array_to_file( 
			BYTE *list_of_bytes,
			int list_length,
			char *filename
			);
extern void load_dma_from_file(
			UINT32 destAddr, 
			int size, /*size in units, internally converted to bytes*/
			int conversion_code, /* 0 = byte, 1 = 16 bit, 2 = 32 bit*/
			char *filename,
			int fifo_transaction, /*set to 1 if destination is a fifo*/
			int channel_num /*set only if fifo_transaction bit is set to 1*/
			);
extern void write_bytes_from_host_file(
			UINT32 destAddr, 
			char *filename,
			int size, /*size in bytes*/
			int fifo_transaction, /*set to 1 if destination is a fifo*/
			int channel_num /*set only if fifo_transaction bit is set to 1*/
			);
extern void read_bytes_to_host_file(
			UINT32 srcAddr, 
			char *filename,
			int size, /*size in bytes*/
			int fifo_transaction, /*set to 1 if source is a fifo*/
			int channel_num /*set only if fifo_transaction bit is set to 1*/
			);

extern void poll_register_loop(
                        UINT32 addr,       /*read register from this location*/
                        UINT32 mask,       /*Used to decide which bits to mask*/
                        UINT32 loop_value  /* Continue to loop until
                                           read_value and mask equals loop value*/
                        );
