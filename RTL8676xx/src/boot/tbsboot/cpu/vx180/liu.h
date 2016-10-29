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

#define IS_VX180    1

/* include files */

#if IS_VX180        // vx180
    #define  LIU_BASE_ADDRESS       0xb9110880
    #define  LIU2_BASE_ADDRESS      0xb9150880

typedef struct _LIU_REGISTERS
{
        /* port configuration register */
        /* 0x0880 */    volatile   uint32_t   port_cfg;
        #define   GIG_ReEntLim_FST   12
        #define   GIG_ReEntLim_LEN    4
        #define   GIG_Pre_FST         9
        #define   GIG_Pre_LEN         3
        #define   GIG_PhyLpBk_BIT     8
        #define   GIG_SusPauFrm_BIT   6
        #define   GIG_Duplex_BIT      5
        #define   GIG_TxFcEn_FST      3
        #define   GIG_TxFcEn_BIT      2
        #define   GIG_Speed_FST       1
        #define   GIG_Speed_LEN       2
        #define   GIG_PadEn_BIT       0

        /* 0x0884 */    volatile   uint32_t   arc_ctl;    /* the bits are
                                        inverted in vx180 compared to vx160 */
        #define   GIG_AddrFltEn_BIT     0
        #define   GIG_Mcast_BIT         3
        #define   GIG_Bcast_BIT         4

        /* 0x0888 */    volatile   uint32_t   arc_st_ctrl;        /* station
                                           enable reg - 1 bit for each addr  in the table*/
        /* 0x088c */    volatile   uint32_t   revlpbk;
        /* 0x0890 */    volatile   uint32_t   reserved3;
        /* 0x0894 */    volatile   uint32_t   reserved4;
        /* 0x0898 */    volatile   uint32_t   reserved5;

        /* 0x089c */    volatile   uint32_t   cgrcr;      /* soft reset cfg */
        #define      GIG_SFTRST_BIT      1
        #define      GIG_PSFTRST         6

        /* 0x08a0 */    volatile   uint32_t   dbgcfg;     /* debug config reg */
        /* 0x08a4 */    volatile   uint32_t   rxpf;       /* rx pause frame ctrl
                                                           reg */
        /* 0x08a8 */    volatile   uint32_t   xpdis_ipg;  /* xmii port disable
                                                           and IPG value reg */
        #define      GIG_RxDis_BIT      15
        #define      GIG_TxDis_BIT      14
        #define      GIG_PrtEn_BIT      13
        #define      GIG_IpgVal_FST     0
        #define      GIG_IpgVal_LEN     6

        /* 0x08ac */    volatile   uint32_t   reserved6;
        /* 0x08b0 */    volatile   uint32_t   reserved7;
        /* 0x08b4 */    volatile   uint32_t   reserved8;
        /* 0x08b8 */    volatile   uint32_t   reserved9;
        /* 0x08bc */    volatile   uint32_t   reserved10;
        /* 0x08c0 */    volatile   uint32_t   reserved11;
        /* 0x08c4 */    volatile   uint32_t   reserved12;

        /* 0x08c8 */    volatile   uint32_t   minfrmsz;   /* min frm size 16 bits */
        /* 0x08cc */    volatile   uint32_t   maxfrmsz;   /* max frm size 16 bits */


        /* 0x08d0 */    volatile   uint32_t   xscr;       /* needed for pause
                                                           frame and IFG operation */

        /* 0x08d4 */    volatile   uint32_t   reserved13;
        /* 0x08d8 */    volatile   uint32_t   reserved14;
        /* 0x08dc */    volatile   uint32_t   reserved15;
        /* 0x08e0 */    volatile   uint32_t   reserved16;
        /* 0x08e4 */    volatile   uint32_t   reserved17;
        /* 0x08e8 */    volatile   uint32_t   reserved18;

        /* 0x08ec */    volatile   uint32_t   jamc;
        /* 0x08f0 */    volatile   uint32_t   jamd;

        /* 0x08f4 */    volatile   uint32_t   reserved19;
        /* 0x08f8 */    volatile   uint32_t   reserved20;
        /* 0x08fc */    volatile   uint32_t   reserved21;

        /* 0x0900 */    volatile   uint32_t   pfsarlo;
        /* 0x0904 */    volatile   uint32_t   pfsarhi;
        /* 0x0908 */    volatile   uint32_t   pfcrcQ0;
        /* 0x090c */    volatile   uint32_t   rfcrcQ0;

        /* 0x0930     volatile   uint32_t   pqer;  pause queue enable register */


} LIU_REGISTERS, *PLIU_REGISTERS;

typedef struct _LIU_SAT_TABLE_
{
  /* 0x0a00 */  volatile   uint32_t   sat0_1;    // 15-0 bits
  /* 0x0a04 */  volatile   uint32_t   sat0_2;    // 31-15 used
  /* 0x0a08 */  volatile   uint32_t   sat0_3;    // 47-32 used

  /* 0x0a0c */  volatile   uint32_t   sat1_1;    // 15-0 bits
  /* 0x0a10 */  volatile   uint32_t   sat1_2;    // 31-15 used
  /* 0x0a14 */  volatile   uint32_t   sat1_3;    // 47-32 used

  /* 0x0a18 */  volatile   uint32_t   sat2_1;    // 15-0 bits
  /* 0x0a1c */  volatile   uint32_t   sat2_2;    // 31-15 used
  /* 0x0a20 */  volatile   uint32_t   sat2_3;    // 47-32 used

  /* 0x0a24 */  volatile   uint32_t   sat3_1;    // 15-0 bits
  /* 0x0a28 */  volatile   uint32_t   sat3_2;    // 31-15 used
  /* 0x0a2c */  volatile   uint32_t   sat3_3;    // 47-32 used

  /* 0x0a30 */  volatile   uint32_t   sat4_1;    // 15-0 bits
  /* 0x0a34 */  volatile   uint32_t   sat4_2;    // 31-15 used
  /* 0x0a38 */  volatile   uint32_t   sat4_3;    // 47-32 used

  /* 0x0a3c */  volatile   uint32_t   reserved1;    // 15-0 bits

  /* 0x0a40 */  volatile   uint32_t   sat5_1;    // 15-0 bits
  /* 0x0a44 */  volatile   uint32_t   sat5_2;    // 31-15 used
  /* 0x0a48 */  volatile   uint32_t   sat5_3;    // 47-32 used

  /* 0x0a4c */  volatile   uint32_t   sat6_1;    // 15-0 bits
  /* 0x0a50 */  volatile   uint32_t   sat6_2;    // 31-15 used
  /* 0x0a54 */  volatile   uint32_t   sat6_3;    // 47-32 used

  /* 0x0a58 */  volatile   uint32_t   sat7_1;    // 15-0 bits
  /* 0x0a5c */  volatile   uint32_t   sat7_2;    // 31-15 used
  /* 0x0a60 */  volatile   uint32_t   sat7_3;    // 47-32 used

} LIU_SAT_TABLE;

#else
    #define  LIU_BASE_ADDRESS       0xb9110000
    #define  LIU2_BASE_ADDRESS      0xb9150000
    #define  LIU3_BASE_ADDRESS      0xb9190000


typedef struct _LIU_REGISTERS
{
  /* 0x0000 */  volatile   uint32_t rmon_ctrl;
    #define LIU_RMSwRq_BIT         0
    #define LIU_RMemBank_BIT       1
    #define LIU_RmRxInit_BIT       2
    #define LIU_RmTxInit_BIT       3

  /* 0x0004 */  volatile   uint32_t  reserved1;
  /* 0x0008 */  volatile   uint32_t  reserved2;
  /* 0x000c */  volatile   uint32_t  reserved3;
  /* 0x0010 */  volatile   uint32_t  reserved4;
  /* 0x0014 */  volatile   uint32_t  reserved5;
  /* 0x0018 */  volatile   uint32_t  reserved6;
  /* 0x001c */  volatile   uint32_t  reserved7;
  /* 0x0020 */  volatile   uint32_t  reserved8;
  /* 0x0024 */  volatile   uint32_t  reserved9;
  /* 0x0028 */  volatile   uint32_t  reserved10;
  /* 0x002c */  volatile   uint32_t  reserved11;

  /* Pause count occured by mac and remote mac while receiving and sending */
  /* 0x0030 */  volatile   uint32_t pauseCnt;
  /* 0x0034 */  volatile   uint32_t remPauCnt;

  /* Alternate register to check the tx status when txcmp int occurs.
   * It is same as Tx_stat register */
  /* 0x0038 */  volatile   uint32_t txConFrmStat;

  /* 0x003c */  volatile   uint32_t reserved20;

  /* 0x0040 */  volatile   uint32_t mac_ctrl;
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
    #define LIU_Link10_BIT      15      /* Link 10 status indicator */

  /* 0x0044 */  volatile   uint32_t arc_ctl;
    #define LIU_StationAcc_BIT  0       /* accept uncast packets */
    #define LIU_GroupAcc_BIT    1       /* accept multicast packets */
    #define LIU_BroadAcc_BIT    2       /* accept broadcast packets */
    #define LIU_NegARC_BIT      3       /* 0 = Accept packets ARC recognizes
                                         * 1 = Reject packets ARC recognizes */
    #define LIU_CmpEn_BIT       4       /* enable compare mode */

  /* 0x0048 */  volatile   uint32_t tx_ctrl;
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
    #define LIU_EnLCarr_BIT     10      /* Interrupt if carrier sense is not detected or is dropped*/
    #define LIU_EnExColl_BIT    11      /* Interrupt if 16 collisions occured at the same packet */
    #define LIU_EnLateColl_BIT  12      /*Interrupt if late collision (after 64 bytes) is occured in the packet */
    #define LIU_EnTxPar_BIT     13      /*Interrupt if the MAC trasmit packet has a parity error */
    #define LIU_EnComp_BIT     	14		/* Interrupt when the MAC transmits */

  /* 0x004c */  volatile   uint32_t tx_stat;

  /* 0x0050 */  volatile   uint32_t rx_ctrl;
    #define LIU_RxEn_BIT        0       /* 0 = stop reception immediately */
    #define LIU_RxHalt_BIT      1       /* Halt after reception of current pkt*/
    #define LIU_LongEn_BIT      2       /* Accept long Frames (VLAN) <1522 */
    #define LIU_EnLongErr_BIT  11       /* Accept long Frames >1518 bytes */
    #define LIU_ShortEn_BIT     3       /* Accept short Frames <64 bytes */
    #define LIU_StripCrc_BIT    4       /* srip crc from the message */
    #define LIU_PassCtl_BIT     5       /* srip crc from the message */
    #define LIU_IgnoreCrc_BIT   6       /* ignore Crc */
    #define LIU_EnGood_BIT     14       /* Interrupt on recept of good pckts */
    #define LIU_EnAlign_BIT     8       /* Interrupt on recept of misaligned */
    #define LIU_EnCRCErr_BIT    9       /* Interrupt on recept of CRC pckts */
    #define LIU_EnRxPar_BIT    13       /* Interrupt on recept of parity err */
    #define LIU_EnOver_BIT     10       /* Interrupt on recept of overflow */
    #define LIU_IgnoreLen_BIT   7        /* Ignore packet len (802.3) */
    #define LIU_EnLenErr_BIT   12       /* Interrupt on length error */

  /* 0x0054 */  volatile   uint32_t rx_stat;
    #define LIU_LenErr_BIT     4        /* Bytes 12/13 not type and not
                                         * same as pkt length
                                         */
    #define LIU_LongErr_BIT    11       /* Frame longer than 1514 bytes */
    #define LIU_Good_BIT       14       /* Frame good */

  /* 0x0058 */  volatile   uint32_t md_data;
  /* 0x005c */  volatile   uint32_t md_ca;
    #define LIU_Addr1_FST	0
    #define LIU_Addr1_LEN	5
    #define LIU_PHY_FST		5
    #define LIU_PHY_LEN		5
    #define LIU_Wr_BIT	       10
    #define LIU_Busy_BIT       11
    #define LIU_PreSup_BIT     12
    #define LIU_RxMCast_BIT    17
    #define LIU_RxBCast_BIT    18
    #define LIU_RxVLAN_BIT     19
    #define LIU_RxPause_BIT    20

  /* Address recognition circuit */
  /* 0x0060 */  volatile   uint32_t arc_addr;
  /* 0x0064 */  volatile   uint32_t arc_data;
  /* 0x0068 */  volatile   uint32_t arc_en;

  /* PROM control registers  to be used ????*/
  /* 0x006c */  volatile   uint32_t reserved13;
  /* 0x0070 */  volatile   uint32_t reserved14;
  /* 0x0074 */  volatile   uint32_t reserved15;
  /* 0x0078 */  volatile   uint32_t reserved16;
  /* 0x007c */  volatile   uint32_t miss_ctl;

  /* 0x0080 */  volatile   uint32_t CntData;

  /* 0x0084 */  volatile   uint32_t CntAcc;
    #define  LIU_Addr_FST    0
    #define  LIU_Addr_LEN    8
    #define  LIU_WrRdN_BIT   8
    #define  LIU_Clear_BIT   9

} LIU_REGISTERS, *PLIU_REGISTERS;


#endif   // vx180


#ifdef __cplusplus
}
#endif

#endif /* for the #define _liu.h */


