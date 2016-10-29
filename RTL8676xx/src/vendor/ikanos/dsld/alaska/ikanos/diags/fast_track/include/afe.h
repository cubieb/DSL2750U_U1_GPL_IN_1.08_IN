/**************************************************************
***************************************************************
**
** BME Firmware, Copyright (C) 2000-2002
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
$Date: 2008/07/18 11:09:31 $
**************************************************************/

#ifndef _AFE_H_
#define _AFE_H_

#include "bme.h"
#include "bme_app.h"
#include "mem.h"

typedef struct  {
	int address;		// address of port in afe
	int rw;				// read only or read/write. TRUE = read/write
	int data;			// initial value to be written at startup, as well as saved values. when writes are performed
} AFE_DATABASE;

typedef enum {
	IFE_GCH00,			// 0x20 rw (1)
	IFE_GCH01, 			// 0x21 rw (2)	
	IFE_GCH10,			// 0x22 rw (3)
	IFE_GCH11,			// 0x23 rw (4)
	IFE_XFER_STATUS,	// 0x24 rw (5)
	IFE0_RETUNE_CNTL,	// 0x25 rw (6)
	IFE1_RETUNE_CNTL, 	// 0x26 rw (7) 
	VDSLBG,				// 0x29 rw (8)
	VTEST,				// 0x2d	rw (9)
	AFE_VCH0,			// 0x30 rw (10)
	VCH0ST,				// 0x31 r  (11)
	AFE_VCH1,			// 0x32 rw (12)
	VCH1ST, 			// 0x33 rw (13)
	VBIAS,				// 0x34 rw (14)
	DCXO,				// 0x36 rw (15)
	VERSION,			// 0x3f	rw (16)
	CAL_CREG,			// 0x70 rw (17)
	CAL_SREG,			// 0x72 r  (18)
	RFI_CH0,			// 0x7a rw (19)
	RFI_CH1,			// 0x7c rw (20)
	VTXCH0, 			// 0x86 rw (21)
	VTXCH1,				// 0x87 rw (22)
	VRXCH0,				// 0x88 rw (23)
	VRXCH1,				// 0x89 rw (24)
	GBLCTR,				// 0x8a rw (25)
	CADDR,				// 0x90 rw (26)
	TXCOEF,				// 0x91 rw (27)
	RXCOEF,				// 0x92 rw (28)
	RFCOEF,				// 0x93 rw (29)
	PMC1CH0,			// 0xa0 r  (30)
	PMC1CH1,			// 0xa1 r  (31)
	PMC2CH0,			// 0xa2 r  (32)
	PMC2CH1,			// 0xa3 r  (33)
	INTEG0,				// 0xa8 rw (34)
	INTEG1,				// 0xa9 rw (35)
	FIFOCH0,			// 0xF0 rw (36)
	FIFOCH1,			// 0xF1 rw (37)
	NUM_AFE_REGISTERS
} tAFERegs;

typedef enum {TX_FILTER_COEF = 0, RX_FILTER_COEF, HPF_FILTER_COEF, NOISE_SHAPING_COEF, RFI_FILTER_COEF} FILTER_COEF_TYPE;
typedef	enum {VGA_0db = 0, VGA_minus1db, VGA_minus2db, VGA_minus3pt1db, VGA_minus4pt5db, VGA_minus6db, VGA_minus8db, VGA_minus10pt5db} VGA_GAINS;
typedef enum {PGA1_0pt9db = 0, PGA1_1pt9db, PGA1_2pt8db, PGA1_4db, PGA1_5db, PGA1_6db, PGA1_7pt5db, PGA1_8pt5db, PGA1_9pt5db,  
				PGA1_12db, PGA1_13db, PGA1_13pt9db, PGA1_15pt85db, PGA1_15pt9db, PGA1_16pt8db, PGA1_18pt8db, PGA1_19pt8db, PGA1_20pt7db} PGA_1_GAINS;
typedef enum {PGA2_minus1pt26db = 0, PGA2_5db, PGA2_11pt2db }PGA_2_GAINS;

extern void setAFEOn (int which);
extern void setAFEOff (int which);
extern void initAFE (int);

extern void turn_on_afe(int AFE_index, int chan_num);
extern void turn_off_afe(int AFE_index, int chan_num);
extern int 	afeWriteRegister(int which_reg, int which_afe, int data);
extern int  afeReadRegister (int which_reg, int which_afe);
extern int  write_hfe_tx_gain(int hfe_index, int chan_num, int val);
extern int  write_hfe_rx_gain(int hfe_index, int chan_num, int val);
extern int  test_afe_rw(int tot_afes, int *ibuf, int *obuf);
extern int  read_afe_pwr(int afe_index, int chan_num);
extern int 	get_afe_pwr(int afe_index, int chan_num);


int write_ife_vga_gain(int hfe_index, int chan_num, VGA_GAINS gain_val);

#endif

