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
$Date: 2008/07/18 11:09:30 $
**************************************************************/

#ifndef _AFE_H_
#define _AFE_H_

typedef struct  {
	int address;		// address of port in afe
	int rw;				// read only or read/write. TRUE = read/write
	int data;			// initial value to be written at startup, as well as saved values. when writes are performed
} AFE_DATABASE;

#ifdef USE_AFE_2
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
	VCH1TST, 			// 0x33 rw (13)
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
	TXCOFF,				// 0x91 rw (27)
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
#else
typedef enum {
	AFEADDR_AFE_ACH1,	// 	0x20 rw	(1)
	AFEADDR_AFE_ACH2,	// 	0x21 rw	(2)
	AFEADDR_AFE_ACH3,	//	0x22 rw	(3)
	AFEADDR_AFE_ACH4,	//	0x23 rw	(4)
	AFEADDR_AFE_ACH5,	// 	0x24 rw	(5)	
	AFEADDR_AFE_ACH6,	//	0x25 rw	(6)
	AFEADDR_AFE_ACH7,	//	0x26 rw	(7)
	AFEADDR_AFE_ACH8,	//	0x27 rw	(8)
	AFEADDR_ADSLPD,		// 	0x28 rw	(9)
	AFEADDR_ADSLBG,		//	0x29 rw (10)
	AFEADDR_ADSLOVFL,	//	0x2B r  (11)
	AFEADDR_ATSTPD,		//	0x2C rw (12)
	AFEADDR_ATEST,		//	0x2D rw (13)
	AFEADDR_AFE_VCH1,	//	0x30 rw (14)
	AFEADDR_VCH1ST,		//  0x31 r	(15)
	AFEADDR_AFE_VCH2,	//	0x32 rw	(16)
	AFEADDR_VCH2ST,		//	0x33 r 	(17)
	AFEADDR_VBIAST,		//	0x34 rw (18)
	AFEADDR_GP,			//  0x38 rw (19)
	AFEADDR_VERSION,		// 	0x3F r	(20)
	AFEADDR_CAL_CREG,	//	0x70 rw (21)
	AFEADDR_CAL_SREG,	//	0x72 r	(22)
	AFEADDR_CAL_SRG2,	//	0x74 r  (23)
	AFEADDR_ERR_REG,	//	0x76 rw (24)
	AFEADDR_CLP_REG,	//	0x78 rw (25)
	AFEADDR_TNR_CREG,	//	0x7A rw (26)
	AFEADDR_TNR_SREG,	//	0x7C r  (27)
	AFEADDR_ADSLCH1,	//	0x80 rw (28)
	AFEADDR_ADSLCH2,	//	0x81 rw (29)
	AFEADDR_ADSLCH3,	//	0x82 rw (30)
	AFEADDR_ADSLCH4,	//	0x83 rw (31)
	AFEADDR_ADSLCH5,	//	0x84 rw (32)
	AFEADDR_ADSLCH6,	//  0x85 rw (33)
	AFEADDR_ADSLCH7,	//	0x86 rw (34)
	AFEADDR_ADSLCH8,	//	0x87 rw (35)
	AFEADDR_VDSLCH1,	//	0x88 rw (36)
	AFEADDR_VDSLCH2,	//	0x89 rw (37)
	AFEADDR_GBLCTR,		//	0x8a rw (38)
	AFEADDR_CADDR,		//	0x90 rw (39)
	AFEADDR_TXCOEF,		//	0x91 rw (40)
	AFEADDR_RXCOEF,		//	0x92 rw (41)
	AFEADDR_HPCOEF,		//  0x93 rw (42)
	AFEADDR_NSCOEF,		//	0x94 rw (43)
	AFEADDR_RAMTST,		//	0x9F rw (44)
	AFEADDR_PMCH1,		//	0xA0 r 	(45)
	AFEADDR_PMCH2,		//	0xA1 r 	(46)
	AFEADDR_PMCH3,		//	0xA2 r 	(47)
	AFEADDR_PMCH4,		//	0xA3 r  (48)
	AFEADDR_PMCH5,		//	0xA4 r  (49)
	AFEADDR_PMCH6,		//	0xA5 r 	(50)
	AFEADDR_PMCH7,		//	0xA6 r 	(51)
	AFEADDR_PMCH8,		//	0xA7 r 	(52)
	AFEADDR_INTEG,		//	0xA8 rw (53)
	AFEADDR_FIFO,		//	0xF0 rw	(54)
	NUM_AFE_REGISTERS,	// total 54 AFE registers
} tAFERegs;
#endif

typedef enum {TX_FILTER_COEF = 0, RX_FILTER_COEF, HPF_FILTER_COEF, NOISE_SHAPING_COEF} FILTER_COEF_TYPE;

extern void setAFEOn (int which);
extern void setAFEOff (int which);
extern void initAFE (int);
extern void initVDSL_AFE(int AFE_index, int chan_num, LOCATION loc, BAND_PLANS bandplan);
extern void turn_on_afe(int AFE_index, int chan_num);
extern void turn_off_afe(int AFE_index, int chan_num);
extern int 	afeWriteRegister(int which_reg, int which_afe, int data);
extern int  afeReadRegister (int which_reg, int which_afe);
extern int  write_hfe_tx_gain(int hfe_index, int chan_num, int val);
extern int  write_hfe_rx_gain(int hfe_index, int chan_num, int val);
extern int  test_afe_rw(int tot_afes, int *ibuf, int *obuf);
extern int  read_afe_pwr(int afe_index, int chan_num);
extern int 	get_afe_pwr(int afe_index, int chan_num);
#endif

