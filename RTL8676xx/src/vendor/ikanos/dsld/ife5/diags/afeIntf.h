
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
$Date: 2008/07/18 11:08:34 $
**************************************************************/

#ifndef _AFE_H_
#define _AFE_H_

/*#ifndef TRUE
#define TRUE                                    (1)
#endif*/

/*#ifndef FALSE
#define FALSE                                   (0)
#endif*/


  // struct to hold info about afe regs 
typedef struct  {
	int address;	// address of reg in afe
	int rw;		// read only or read/write. TRUE = read/write
	int data;	// initial value to be written at startup, as well 
			//   as saved values, when writes are performed
} AFE_DATABASE;

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



extern int  afeWriteRegister(int which_reg, int which_afe, int data);
extern int  afeReadRegister (int which_reg, int which_afe);
extern void initAFE (int to_afes);

#endif

