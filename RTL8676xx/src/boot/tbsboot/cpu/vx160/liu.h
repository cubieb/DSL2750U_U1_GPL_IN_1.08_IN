/* =============================================================================
 *
 * Copyright (C) 2001 Analog Devices Inc. All rights reserved
 *
 * The information and source code contained herein is the exclusive property
 * of Analog Devices and may not be disclosed, examined or reproduced in whole
 * or in part without the explicit written authorization from Analog Devices.
 *
 * ===========================================================================*/

/*======================================================================
 *
 * Copyright (C) 2000 Chiplogic Inc. All rights reserved
 *
 * The information and source code contained herein is the exclusive
 * property of Chiplogic and may not be disclosed, examined, or
 * reproduced in whole or in part without explicit written authorization
 * of the company.
 *
 *======================================================================
 */

/*======================================================================
 * File Name    :       liu.h
 * Description  :       Details of sierra ethernet controller registers
 * CVS ID       :       $$
 *======================================================================
 */

/*======================================================================
 * History
 *
 * Date         Author                  Comment
 * 12/19/2000   Malati Chavva           Initial version
 *======================================================================
 */

  // They rely on the following naming conventions:
  //  XX_BIT            The bit position of the single bit XX
  //  YY_FST            The bit position of the LSB of bitfield YY
  //  YY_LEN            The number of bit in the bitfield YY

  //  UX		register with X bits

#ifndef _LIU_H
#define _LIU_H

#ifdef __cplusplus
extern "C" {
#endif

/* include files */

#define  LIU_BASE_ADDRESS	0xb9110000
#define  LIU2_BASE_ADDRESS      0xb9150000
#define  LIU3_BASE_ADDRESS      0xb9190000

typedef struct _LIU_REGISTERS 
{
  /* 0x0000 */	volatile   uint32_t	dma_ctrl;
    /* DMA burst size is a multilple of AHB cache-line default is 32 bytes */
    #define LIU_DmBurst_FST         2          
    #define LIU_DmBurst_LEN         7
    #define LIU_PowrMgmnt_BIT       12
    #define LIU_TestMode_BIT        13
    #define LIU_TxBigE_BIT          14
    #define LIU_RxBigE_BIT          15
    #define LIU_TxWakeUp_BIT        16
    #define LIU_SwIntReq_BIT        17
    #define LIU_IntMask_BIT         18
    #define LIU_RmTxInit_BIT        20
    #define LIU_RmRxInit_BIT        21
    #define LIU_RxAlign_FST         22
    #define LIU_RxAlign_LEN         2
    #define LIU_RxAlign2_BIT        23
    #define LIU_RMSwRq_BIT         24
    #define LIU_RMemBank_BIT       25

  /* 0x0004 */	volatile   uint32_t	txFrmPtr;  /* align to 16 byte boundary */
    #define LIU_Tx_EOL_BIT         1  /* bit set means valid address */
  
      /* Tx threshold is 11 bits long value. It indicates the no. of bytes 
       * after which the MAC should start transmitting. Must be <1620 */
  /* 0x0008 */	volatile   uint32_t  txThresh;  

    /* TX polling counter . With 33MHx clock each unit = 61.44 usec. 
     * It is 12 bit counter */

    /* TX polling counter . With 33MHx clock each unit = 61.44 usec. 
     * It is 12 bit counter */
  /* 0x000c */	volatile   uint32_t  txPollCtr;	
  
  /* 0x0010 */	volatile   uint32_t	blFrmPtr; /* align to 16 byte boundary */
    #define LIU_Buf_EOL_BIT         1  /* bit set means valid address */
  
    /* ??? The use is not clear */
  /* 0x0014 */	volatile   uint32_t	rxFragSize;	

  /* 0x0018 */	volatile   uint32_t	int_en;	
    #define LIU_FDAExEn_BIT     0       /* Interrupt if the Free Desc. Area
                                         * becomes exhausted */
    #define LIU_BLExEn_BIT      1       /* Interrupt if the Buffer list
                                         * becomes exhausted */
    #define LIU_STargAbtEn_BIT  2
    #define LIU_RTargAbtEn_BIT  3
    #define LIU_RMasAbtEn_BIT   4
    #define LIU_SSysErrEn_BIT   5
    #define LIU_DParErrEn_BIT   6
    #define LIU_EarNotEn_BIT	7
    #define LIU_DParDEn_BIT     8
    #define LIU_DmParErrEn_BIT  9
    #define LIU_TxCltCmpEn_BIT  10      /* Interrupt after transmission of
                                         * MAc control packet is complete */
    #define LIU_NRAbtEn_BIT     11

  /* 0x001c */	volatile   uint32_t	fda_Bas;  /* align to 16 byte boundary */

    /* not clear how to set the value ???? */  
  /* 0x0020 */	volatile   uint32_t	fda_Lim;  /* align to 16 byte boundary */	
  
  /* 0x0024 */  volatile   uint32_t	int_src;	
    #define LIU_EarNot_BIT	6	/* writing 1 clears this bit */
    #define LIU_FDAEx_BIT       11      /* writing 1 clears this bit */
    #define LIU_BLEx_BIT        12      /* writing 1 clears this bit */
    #define LIU_TxCtlCmp_BIT    9       /* writing 1 clears this bit */
    #define LIU_IntBLEx_BIT     4       /* writing 1 clears this bit */
    #define LIU_IntFDAEx_BIT    3       /* writing 1 clears this bit */
    #define LIU_IntMacRx_BIT    1       /* writing 1 clears this bit */
    #define LIU_IntMacTx_BIT    0       /* writing 1 clears this bit */
  
  /* 0x0028 */	volatile   uint32_t	reserved1;	
  /* 0x002c */	volatile   uint32_t	reserved2;	
  
    /* Pause count occured by mac and remote mac while receiving and sending */
  /* 0x0030 */	volatile   uint32_t	pauseCnt;	
  /* 0x0034 */	volatile   uint32_t	remPauCnt;	

    /* Alternate register to check the tx status when txcmp int occurs. 
     * It is same as Tx_stat register */
  /* 0x0038 */	volatile   uint32_t	txConFrmStat;	
  
  /* 0x003c */	volatile   uint32_t	reserved3;
  
  /* 0x0040 */	volatile   uint32_t	mac_ctrl;	
    #define LIU_HaltReq_BIT     0       /* stop tx and rx after completion 
                                         * of any current packets */
    #define LIU_HaltImm_BIT     1       /* stop tx and rx immediately */
    #define LIU_Reset_BIT       2       /* Reset all */
    #define LIU_FullDup_BIT     3       /* If MII_Conn is high, then this bit
                                         * set is Full-duplex and 0 is half.
                                         * If MII_Conn is low, indicates the 
                                         * status of MII_FD depending on 
                                         * Connection mode */
    #define LIU_MacLoop_BIT     4       /* Loopback mode */
    #define LIU_Conn_FST        5       /* 00 = Automatic  */
    #define LIU_Conn_LEN        2       /* 01 = Force 10 Mbit/s endec 
                                         * 10 = Force MII */
    #define LIU_Loop10_BIT      7       /* If set, assert the Loop_10 
                                         * external signal */
    #define LIU_LinkChg         8       /* link change indicator, writing 1
                                         * clears this bit. */
    #define LIU_Link10_BIT      15      /* Link 10 status indicator */
  
  /* 0x0044 */	volatile   uint32_t	arc_ctl;	
    #define LIU_StationAcc_BIT  0       /* accept uncast packets */        
    #define LIU_GroupAcc_BIT    1       /* accept multicast packets */        
    #define LIU_BroadAcc_BIT    2       /* accept broadcast packets */  
    #define LIU_NegARC_BIT      3       /* 0 = Accept packets ARC recognizes 
                                         * 1 = Reject packets ARC recognizes */ 
    #define LIU_CmpEn_BIT       4       /* enable compare mode */

  /* 0x0048 */	volatile   uint32_t	tx_ctrl;	
    #define LIU_TxEn_BIT        0       /* 0 = stop transmission immediately */
    #define LIU_TxHalt_BIT      1       /* Halt after completion of current pkt*/
    #define LIU_NoPad_BIT       2       /* No padding for pkts < 64 bytes */
    #define LIU_NoCrc_BIT       3       /* Do not add CRC at the end of pkt */
    #define LIU_Fback_BIT       4       /* Use faster back-off timers and byte counts */
    #define LIU_NoExDef_BIT     5       /* Surpress the checking of excessive deferal */
    #define LIU_SdPause_BIT     6       /* Send a Pause command or control packet  */
    #define LIU_MII10_BIT       7       /* Checks SQE checking in MII 10 Mb/s mode */
    #define LIU_EnUnder_BIT     8       /* Iterrupt if the MAC trasmit FIFO is empty during transmission */
    #define LIU_EnExdefer_BIT   9       /* Interrupt if the MAC defer for MAX_DEFFERAL time */
    #define LIU_EnLCarr_BIT     10       /* Interrupt if carrier sense is not detected or is dropped*/           
    #define LIU_EnExColl_BIT    11       /* Interrupt if 16 collisions occured at the same packet */
    #define LIU_EnLateColl_BIT  12       /*Interrupt if late collision (after 64 bytes) is occured in the packet */
    #define LIU_EnTxPar_BIT     13       /*Interrupt if the MAC trasmit packet has a parity error */
    #define LIU_EnComp_BIT     14	/* Interrupt when the MAC transmits */

  /* 0x004c */	volatile   uint32_t	tx_stat;	

  /* 0x0050 */	volatile   uint32_t	rx_ctrl;
    #define LIU_RxEn_BIT        0       /* 0 = stop reception immediately */
    #define LIU_RxHalt_BIT      1       /* Halt after reception of current pkt*/
    #define LIU_LongEn_BIT      2       /* Accept long Frames (VLAN) <1522 */
    #define LIU_EnLongErr_BIT   11       /* Accept long Frames >1518 bytes */
    #define LIU_ShortEn_BIT     3       /* Accept short Frames <64 bytes */
    #define LIU_StripCrc_BIT    4       /* srip crc from the message */
    #define LIU_PassCtl_BIT     5       /* srip crc from the message */
    #define LIU_IgnoreCrc_BIT   6       /* ignore Crc */
    #define LIU_EnGood_BIT     14       /* Interrupt on recept of good pckts */ 
    #define LIU_EnAlign_BIT    8       /* Interrupt on recept of misaligned */ 
    #define LIU_EnCRCErr_BIT    9       /* Interrupt on recept of CRC pckts */ 
    #define LIU_EnRxPar_BIT    13       /* Interrupt on recept of parity err */ 
    #define LIU_EnOver_BIT     10       /* Interrupt on recept of overflow */ 
    #define LIU_IgnoreLen_BIT  7        /* Ignore packet len (802.3) */
    #define LIU_EnLenErr_BIT   12       /* Interrupt on length error */
 
  /* 0x0054 */	volatile   uint32_t	rx_stat;	
    #define LIU_LenErr_BIT     4        /* Bytes 12/13 not type and not
                                         * same as pkt length
                                         */
    #define LIU_LongErr_BIT    11       /* Frame longer than 1514 bytes */
    #define LIU_Good_BIT       14       /* Frame good */

  /* ??? related to using the phy device */
  /* 0x0058 */	volatile   uint32_t	md_data;	
  /* 0x005c */	volatile   uint32_t	md_ca;	
    #define LIU_Addr1_FST	0
    #define LIU_Addr1_LEN	5
    #define LIU_PHY_FST		5
    #define LIU_PHY_LEN		5
    #define LIU_Wr_BIT		10
    #define LIU_Busy_BIT	11
    #define LIU_PreSup_BIT	12
    #define LIU_RxMCast_BIT     17
    #define LIU_RxBCast_BIT     18
    #define LIU_RxVLAN_BIT      19
    #define LIU_RxPause_BIT     20
  /* ???? Address recognition circuit */
  /* 0x0060 */	volatile   uint32_t	arc_addr;	
  /* 0x0064 */	volatile   uint32_t	arc_data;	
  /* 0x0068 */	volatile   uint32_t	arc_en;	

  /* PROM control registers  to be used ????*/
  /* 0x006c */	volatile   uint32_t	prom_ctl;	
  /* 0x0070 */	volatile   uint32_t	prom_data;	
  /* 0x0074 */	volatile   uint32_t	reserved4;	
  /* 0x0078 */	volatile   uint32_t	reserved5;	
  /* 0x007c */	volatile   uint32_t	miss_ctl;	

  /* 0x0080 */	volatile   uint32_t	CntData;	

  /* 0x0084 */	volatile   uint32_t	CntAcc;	
    #define  LIU_Addr_FST    0
    #define  LIU_Addr_LEN    8
    #define  LIU_WrRdN_BIT   8 
    #define  LIU_Clear_BIT   9 

  /* 0x0088 */	volatile   uint32_t TxRmIntEn;	
    #define  LIU_TxRmIntEn_FST   0;
    #define  LIU_TxRmIntEn_LEN   30; 
  /* 0x008c */	volatile   uint32_t RxRmIntEn;	
    #define  LIU_RxRmIntEn_FST   0;
    #define  LIU_RxRmIntEn_LEN   24; 
  /* 0x0090 */	volatile   uint32_t	reserved6;	
  /* 0x0094 */	volatile   uint32_t	reserved7;	
  /* 0x0098 */	volatile   uint32_t	reserved8;	
  /* 0x009c */	volatile   uint32_t	reserved9;	
  /* Wake up registers */
  /* 0x00a0 */  volatile   uint32_t WU_Ctl;
    #define  LIU_MPCtl_BIT 	0
    #define  LIU_ONCtl_FST	1
    #define  LIU_ONCtl_LEN	5
    #define  LIU_MPIdx_FST	9
    #define  LIU_MPIdx_LEN	5
    #define  LIU_LinkChgCtl_BIT	14
    #define  LIU_EECtl_FST	15
    #define  LIU_EECtl_LEN	2
    #define  LIU_HostNRCtl_BIT	19
    #define  LIU_EnRamPM_BIT	20

  /* 0x00a4 */  volatile  uint32_t WU_stat;
    #define  LIU_MPStat_BIT	0
    #define  LIU_ONStat_FST	1
    #define  LIU_ONStat_LEN	5
    #define  LIU_LinkChgStat_BIT	14
    #define  LIU_EEStat_FST	15
    #define  LIU_EEStat_LEN	2
    #define  LIU_HNRStat_BIT	19

  /* 0x00a8 */  volatile  uint32_t ON_Res0;
  /* 0x00ac */  volatile  uint32_t ON_Res1;
  /* 0x00b0 */  volatile  uint32_t ON_Res2;
  /* 0x00b4 */  volatile  uint32_t ON_Res3;
  /* 0x00b8 */  volatile  uint32_t ON_Res4;
  /* 0x00bc */  volatile  uint32_t HNR_CNT;
  /* 0x00c0 */  uint32_t reserved10[(0x100 - 0xc0) / sizeof(uint32_t)];

  /* The following registers are not part of the EMAC itself, but
   * they are part of the AHB interface for the EMAC
   */
  /* 0x0100 */  uint32_t     reserved11;
  /* 0x0104 */  volatile  uint16_t ahbCmd;
  /* 0x0106 */  volatile  uint16_t ahbStat;
    #define LIU_AhbSpeed_FST    5       /* 11 = reserved */
    #define LIU_AhbSpeed_LEN    2       /* 01 = AHB clock 33 MHz 
                                         * 10 = 66 MHz
                                         * 00 = 100 MHz */
  /* 0x0108 */  uint8_t reservedXX[3];
                volatile  uint8_t  ahbRevId;

  /* 0x010c */  volatile  uint32_t ahbCtl;
    #define  LIU_BIST_AVAIL_BIT  31     /* R/O 1 = BIST implemented */
    #define  LIU_BIST_RUN_BIT    30     /* set to start self test */
    #define  LIU_BIST_RMON_BIT   29     /* Indicates error in RMON SRAM */
    #define  LIU_BIST_DMAP_BIT   28     /* Indicates error in DMA RAM parity */
    #define  LIU_BIST_DMA_BIT    27     /* Indicates error in DMA RAM content */

    #define  LIU_lat_timer_FST   8      /* latency timer # of clocks */
    #define  LIU_lat_timer_LEN   8      
    #define  LIU_Cache_ln_FST    0      /* Cache line size */
    #define  LIU_Cache_ln_LEN    7      

  /* 0x0110 */  uint32_t reserved12[(0x13c - 0x110) / sizeof(uint32_t)];

  /* 0x013c */  volatile  uint32_t ahbIntLine;
  /* 0x0140 */  volatile  uint32_t ahbPmCap;
  /* 0x0144 */  volatile  uint32_t ahbPmCtl;
    #define  LIU_PM_DataSel_FST	9
    #define  LIU_PM_DataSel_LEN	4
    #define  LIU_PM_Scale_FST	13
    #define  LIU_PM_Scale_LEN	2
    #define  LIU_PMPS_FST	0
    #define  LIU_PMPS_LEN	2
  

} LIU_REGISTERS, *PLIU_REGISTERS;
#ifdef __cplusplus
}
#endif   

#endif /* for the #define _liu.h */					    

	
