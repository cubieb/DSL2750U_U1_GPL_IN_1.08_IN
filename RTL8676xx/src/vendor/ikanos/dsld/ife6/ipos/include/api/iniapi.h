/**************************************************************
$Workfile:   iniapi.h  $
***************************************************************
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
$Revision: 1.1 $
$Date: 2008/07/18 11:09:31 $
**************************************************************/

#ifndef INIAPI_H
#define INIAPI_H

/* Updated register defines */
#ifdef CO_4P_BOARD

#define INI_MDData_0	0x05
#define INI_MDData_1	0x06
#define INI_MIIPHYAD	0x07
#define INI_MIIREGAD	0x08
#define INI_MIICTRL_0	0x20
#define INI_MIICTRL_1	0x40
#define INI_MIICTRL_2	0x60
#define INI_MIICTRL_3	0x80
#define INTERRUPT_CSR	0x10
#define INTERRUPT_EN	0x11

#define INI_MDData_0_NEW	0x05
#define INI_MDData_1_NEW	0x06
#define INI_MIIPHYAD_NEW	0x07
#define INI_MIIREGAD_NEW	0x08
#define INI_MIICTRL_0_NEW	0x20
#define INI_MIICTRL_1_NEW	0x40
#define INI_MIICTRL_2_NEW	0x60
#define INI_MIICTRL_3_NEW	0x80
#define INTERRUPT_CSR_NEW	0x10
#define INTERRUPT_EN_NEW	0x11

#else

#define INI_MDData_0	0x24
#define INI_MDData_1	0x25
#define INI_MIIPHYAD	0x26
#define INI_MIIREGAD	0x27
#define INI_MIICTRL_0	0x23
#define INTERRUPT_CSR	0x21
#define INTERRUPT_EN	0x22

#define INI_MDData_0_NEW	0x05
#define INI_MDData_1_NEW	0x06
#define INI_MIIPHYAD_NEW	0x07
#define INI_MIIREGAD_NEW	0x08
#define INI_MIICTRL_0_NEW	0x20
/* for 4p cpe */
#define INI_MIICTRL_1_NEW	0x40
#define INI_MIICTRL_2_NEW	0x60
#define INI_MIICTRL_3_NEW	0x80

#define INTERRUPT_CSR_NEW	0x10
#define INTERRUPT_EN_NEW	0x11

#endif

#define INI_DUPLEX_MODE	0x1  // 1=Half duplex
#define INI_SPEED_MODE	0x2  // 1=10 Mbps

#define NEW_INI_CODE 1
#define OLD_INI_CODE 0

#define INI_EXTRA_CNT		0x12

//Offsets from Per-Port INI Registers
#define TX_BYTE_CNT		0x04
#define DROP_FRAME_CNT		0x05
#define TX_FRAME_CNT		0x06
#define TX_SHORT_FRAME_CNT	0x07
#define TX_LONG_FRAME_CNT	0x08
#define TX_CRC_FRAME_CNT	0x09
#define TX_ALIGN_FRAME_CNT	0x0A
#define RX_BYTE_CNT			0x0B
#define RX_DROP_BYTE_CNT	0x0C
#define RX_FRAME_CNT		0x0D
#define PAUSE_FRAME_CNT		0x0E
#define RX_SHORT_FRAME_CNT	0x0F
#define RX_LONG_FRAME_CNT	0x10
#define RX_CRC_FRAME_CNT	0x11
#define RX_SINGLE_COL_FRAME_CNT 0x12
#define RX_MULTI_COL_FRAME_CNT	0x13
#define RX_DEFER_FRAME_CNT	0x14
#define RX_ABORT_FRAME_CNT	0x15


// Below are out of date, don't use.


/* INI Control Registers */
#define GLOBAL_CSR	0x0
#define INI_MODE	0x1
#define CELL_LENGTH	0x2

/* INI Global ISR */
#define INI_ENABLE 0x01
#define INI_RESET  0x80
/* INI Mode */
#define MID_DIR_PHY 		0x0
#define MID_DIR_MAX 		0x01
#define UL2_TX_MODE_PHY 	0x02
#define UL2_LPBK_ENABLE 	0x04
#define MII_MODE_NORMAL 	0x18
#define MII_MODE_RMII		0x08
#define MII_MODE_SMII		0x10 
#define PAUSE_FR_MODE_DISABLE	0x60
#define PAUSE_FR_MODE_FIXED	0x20
#define PAUSE_FR_MODE_AUTO	0x40

/* Interrupts Enable */
#define CELL_BUF_OVER_EN 	0x01
#define FRAME_BUF_OVER_EN	0x02
#define FRAME_ERR_EN		0x08		
#define PAUSE_FRAME_EN		0x10
#define ALL_INTERRUPT_EN	0x1F

#define RX_FRAME_BUF_OVER	0x01
#define TX_FRAME_BUF_OVER	0x02

/* INI UL2PHYxADDR */
#define UL2PHY_ADDR_BASE	0x03
#define UL2_PHY_ACTIVE 		0x20

/* INI HDLC Address */
#define HDLC_ADDR_BASE		0x0A
#define HDLC_CNTL		0x0E

/* INI Frame Count */
#define TX_FRAME_CNT_ADDR	0x11
#define RX_FRAME_CNT_ADDR	0x1A
#define PAUSE_FRAME_CNT_ADDR	0x22

#define MAX_ETHERNET_PORTS	4


typedef struct tIniCntrs
{
	unsigned long 	TxFrameCnt;
	unsigned long 	RxFrameCnt;
	unsigned long  	TxCrcCnt;
	unsigned long 	RxCrcCnt;
	unsigned long  	DropFrameCnt;

	UINT16 	PrevTxFrameCnt;
	UINT16 	PrevRxFrameCnt;
	UINT16  PrevTxCrcCnt;
	UINT16 	PrevRxCrcCnt;
	UINT16  PrevDropFrameCnt;
} tIniCntrs;


long iniCrcRead (int port);
UINT16 iniRxDropByte (int port);

void iniSetup();
void clearIniCntrs();


extern tIniCntrs IniCntrs[5];
extern tIniCntrs FeIniCntrs[5];

#endif