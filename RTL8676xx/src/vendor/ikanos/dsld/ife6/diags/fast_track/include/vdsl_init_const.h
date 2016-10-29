#ifndef _VDSL_INIT_CONST_H_
#define _VDSL_INIT_CONST_H_

/* Commented By SACHIN DESAI */
/*typedef  unsigned char  u8; */
/*typedef  unsigned short u16; */

#define  SYNCHRO_11             (11)
#define  SYNCHRO_00             (12)
#define  SYNCHRO_MSG_OFF        (99)
#define  SYNCHRO_MSG_11			0xff
#define  SYNCHRO_MSG_00			0x00

#define  NUM_BYTES				(512)
#define  NUM_ROWS				(110)
#define  NUM_TONES				(2048)
#define  SCR_PERIOD     		(2047)
#define  TWO_SCR_PERIOD    		(2*SCR_PERIOD)
#define  SCR_BYTES_ARRAY_SIZE   (SCR_PERIOD + NUM_BYTES + 1)
#define  SYNCHRO_ARRAY_SIZE     (2048+512)
#define  SHR					(4)

extern char scr_four_bits_array[2*NUM_BYTES];

#endif
