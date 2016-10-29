/******************************************************************************/
/*                                                                            */
/*         Copyright(c) 2000-2002 by Ikanos Communications Inc.               */
/*                                                                            */
/*  All rights are reserved.  Reproduction in whole or in part is prohibited  */
/*  without the written consent of the copyright owner.                       */
/*                                                                            */
/*  The information and contents of this file are the proprietary information */
/*  of Ikanos Communication and may not be disclosed or used without the      */
/*  formal written approval of Ikanos Communication Inc.                      */
/*                                                                            */
/*  This Copyright notice may not be removed or modified without prior        */
/*  written consent of Ikanos Communications, Inc.                            */
/*                                                                            */
/******************************************************************************/
/*
**  Filename:    HostPtmfApi.h
**
**  Project:     Ikanos INI ASIC (AMUR) Software Integration
**
**  Target:      MOTOROLA XPC855T based host board
**
**  Platform specific:
**    None.
**
**  Description:
**
**  $Revision: 1.1 $
**  $Date: 2008/07/18 11:09:31 $
**
**
**  **************************************************************************** 
**  Modification History:
**
**    Date         By     Description
**    -----------  ---    ------------------------------------------------------
**	  Jun-16-2002  SFA    Created
**    Jul-17-2002  SFA    Integration into new host code
*/
/******************************************************************************/

/* HEADER SENTINEL                                                            */
#ifndef _HostPtmfApi_H_
#define _HostPtmfApi_H_

/*------------------------------------------------------------------------------
  INCLUDE FILES
  ----------------------------------------------------------------------------*/
#include "HostPtmfUtils.h"

/*------------------------------------------------------------------------------
  PREPROCESSOR CONSTANTS
  ----------------------------------------------------------------------------*/
#define MAX_ENET_PORTS            (PTMF_MAX_CHIPS * PTMF_MAX_PORTS_PER_CHIP)

#define PORT_NOT_SHOWTIME         0
#define PORT_START_TX             0x01
#define PORT_START_RX             0x02
#define PORT_AUTO_NEGO_OVER       0x04
#define PORT_SHOWTIME             (PORT_START_TX | PORT_START_RX | PORT_AUTO_NEGO_OVER)

#define INI_HIC_BASE_ADDR         PTMF_FPGA_BASE_ADDR
#define MAX_UTOPIA_PHYS           16

#define UTOP_ENABLE               0x0001
#define RX_ENABLE_MODE            0x0020

#define UTOP_CELL_ALL_DATA        0x0
#define UTOP_CELL_HEC_HDR         0x1

#define UTOP_TRAFF_NO_IDLE_CELL   0x0
#define UTOP_TRAFF_IDLE_CELL      0x1

#define FLOW_CONTROL_NONE         0
#define FULL_DUPLEX_FLOW          1
#define HALF_DUPLEX_FLOW          2

#define FLOW_CONTROL_DUPLEX       3

#define DUPLEX_FULL               1
#define DUPLEX_HALF               0

#define PORT_SPEED_100            100
#define PORT_SPEED_10             10

#define FIFO_SIZE_4K              0
#define FIFO_SIZE_8K              1
#define FIFO_SIZE_16K             2
#define FIFO_SIZE_32K             3
#define FIFO_SIZE_64K             4

#define FIFO_WATERMARK_48K        96
#define FIFO_WATERMARK_24K        48
#define FIFO_WATERMARK_16K        32
#define FIFO_WATERMARK_12K        24
#define FIFO_WATERMARK_8K         16
#define FIFO_WATERMARK_4K         8

#define PSOFT_RESET               0x0040
#define PCLOCK_GATE               0x0080

#define SOFT_RESET                0x0002
#define CLOCK_GATE                0x0001

#define ENET_INVALID_LINK         0


#define FILTER_ENET_CRC           1
#define FILTER_HDLC_CRC           1

// Definitions used for MAC address filtering
#if defined(BME_CPE_20)
  #define MAX_FILTER_TABLE_ENTRY	  8
#else
  #define MAX_FILTER_TABLE_ENTRY	  16
#endif

#define MAX_BYTE_PER_ENTRY		    6

// Bit definition for bIniMode field of the iposPtmfConfig structure
#define PTMF_XMII_LOOPBACK        0x80
#define PTMF_XMII_MODE            0x70
#define PTMF_PHY_MAC_MODE         0x0f

/* Get Ethernet Status API return codes */
#define ENET_LINK_OFF             1
#define ENET_LINK_ON              2

#define ENET_LINK_10_MBPS         1
#define ENET_LINK_100_MBPS        2
#define ENET_LINK_1000_MBPS        3

#define BIT0_SET   0x1  
#define BIT1_SET   0x2 
#define BIT2_SET   0x4
#define BIT3_SET   0x8

#define BIT4_SET   0x10
#define BIT5_SET   0x20
#define BIT6_SET   0x40
#define BIT7_SET   0x80

#define BIT8_SET   0x100
#define BIT9_SET   0x200
#define BIT10_SET   0x400
#define BIT11_SET   0x800

#define BIT12_SET   0x1000
#define BIT13_SET   0x2000
#define BIT14_SET   0x4000
#define BIT15_SET   0x8000


#define ENET_LINK_HALF_DUPLEX     1
#define ENET_LINK_FULL_DUPLEX     2

#define ENET_FLOW_CONTROL_OFF     1
#define ENET_FLOW_CONTROL_ON      2

/*------------------------------------------------------------------------------
  PREPROCESSOR MACROS
  ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  ENUMERATION DEFINITIONS
  ----------------------------------------------------------------------------*/
typedef enum
{
  ENET_LINK_EN,
  ENET_LINK_SPEED,
  ENET_LINK_DUPLEX,
  ENET_LINK_FLOW_CONTROL,
  ENET_LOOPBACK,
  ENET_AUTO_NEG

} eIposPtmfParamType;

typedef enum
{
  ALL_COUNTERS,
  RX_ENET_UL2_FRAMES,
  RX_ENET_UL2_BYTES,
  RX_ENET_DROP_FRAMES,
  RX_ENET_ALIGN_ERR,
  RX_ENET_OVERSIZE_ERR,
  RX_ENET_UNDERSIZE_ERR,
  RX_ENET_CRC_ERR,
  RX_ENET_CS_ERR,

  RX_ENET_BRDCAST_FRAMES,
  RX_ENET_PAUSE_FRAMES,

  TX_ENET_FRAMES,
  TX_ENET_BYTES,

  TX_PAUSE_FRAMES,

  HDLC_CRC_ERR,
  HDLC_INVALID_FRAMES,
  HDLC_DROP_FRAMES,

  HALF_DUPLEX_LC,
  HALF_DUPLEX_EC,
  HALF_DUPLEX_SC,
  HALF_DUPLEX_MC,

  HALF_DUPLEX_DEF_TRANS
  
} eIposPtmfCounterType;


/*------------------------------------------------------------------------------
  TYPE DEFINITIONS
  ----------------------------------------------------------------------------*/
/* Structure of all the INI ASIC counters                                     */
typedef struct _tsIposPtmfCounters
{
  U32  uiEnetTxFrameCount;                /* Receive Frames through xMII 
                                             interface and transmitted on 
                                             UTOPIA to the BME                */
  U32  uiEnetOctXmitCount;                /* Total bytes received             */
  U32  uiEnetDropFrameCount;              /* Ethernet RX Dropped frame count  */
  U32  uiAllignErrorCount;                /* Ethernet RX Alignment Error      */
  U32  uiLongFrameCount;                  /* Ethernet RX Oversize Frames      */
                                          
  U32  uiRxErrorCount;                    /* Ethernet RX Undersize Frames     */
  U32  uiEnetCrcErrorCount;               /* Ethernet RX CRC errors           */
  U32  uiCarrierSenseErrorCount;          /* Ethernet RX carrier sense errors */
                                          
#ifndef CO_30
  U32  uiBroadCastFrameCount;             /* Ethernet RX Broadcast frames     */
#endif
  U32  uiTxPauseFrameCount;               /* Ethernet RX Pause frames         */
                                          
  U32  uiEnetRxFrameCount;                /* Ethernet TX total frames         */
  U32  uiRxDropCount;                          /* Drop count due to buffer full */
  U32  uiRxDropShortCount;                 /* Drop count of frames less than min size */
  U32  uiRxDropLargeCount;                 /* Drop count of frames greater than max size*/
  U32  uiPrevEnetRxFrameCount;            /* Prev Ethernet Tx total frames*/ 
  U32  uiEnetOctRcvCount;                 /* Ethernet TX total bytes          */
                                          
  U32  uiRxHdlcErrorFrameCount;           /* HDLC CRC Error count             */
  U32  uiRxHdlcInvalidFrameCount;         /* HDLC invalid frames              */
  U32  uiRxPauseFrameCount;               /* Ethernet TX pause frames         */
  U32  uiRxHdlcDropFrameCount;            /* HDLC Dropped frames              */
                                          
  U32  uiRxHdlcCount;                     /* Half Duplex late collisions      */
  U32  uiRxHdecCount;                     /* Half Duplex Excess collisions    */
  U32  uiRxHdscCount;                     /* Half Duplex single collisions    */
  U32  uiRxHdmcCount;                     /* Half Duplex multi collisions     */
                                          
  U32  uiRxHdDefTransCount;               /* HalfDuplex Deferred Transactions */

} tsIposPtmfCounters, *ptsIposPtmfCounters;

/*----------------------------------------------------------------------------*/
/************************* PLEASE READ THIS ***********************************/
/* Explanation of the important PTMF config structure fields
   ---------------------------------------------------------
   bIniMdioAddr : bit[7:6:5] = Bus Number or Bus Type, the PTMF can be accessed.
                               111 = PTMF is accessed through HIC interface 
                                     i.e. PIO read/write. **CPE 2.0 Chip**

                               000 - 110 = 0 till 6 MDIO bus   **PTMF Chip**
                  
                  bit[4:3:2:1:0] = Reserved for CPE 2.0 chip
                                 = 00000 - 11111 (0-0x1F) MDIO base address
                                   for the PTMF chip      

  bIniMode : bit[7]       = xMII Loopback mode ON/OFF (0:Off/1:On)
             bit[6:5:4]   = xMII mode (000:MII/001:SMII/010:SSSMII)
             bit[3:2:1:0] = PTMF chip PHY mode or MAC mode (0000:PHY/0001:MAC)

  bIniMaxEnetPort : Maximum ethernet ports used on the chip
                    1 - 16: PTMF ASIC
                    1     : CPE 2.0 chip

  uiMiiPortMap : bit[15:14:13:12:11:10:9:8:7:6:5:4:3:2:1:0]
                 [1111111111111111] if bIniMaxEnetPort = 16,
                 
                 if bIniMaxEnetPort = 8 then
                 [0101010101010101] i.e. xMII ports 0,2,4,6,8,10,12 and 14 used
                 [0011001100110011] i.e. xMII ports 0,1,4,5,8,9,12 and 13 used
                 [1001100110011001] i.e. xMII ports 0,3,4,7,8,11,12 and 15 used
                 etc.... ** USE any 2 ports in a Qaud.***

                 if bIniMaxEnetPort = 4 then
                 [0001000100010001] i.e. xMII ports 0,4,8,12 used
                 etc.... ** USE any 1 port in a Qaud.***
                 
                                                                              */
/******************************************************************************/
/* Global configuration parameter structure for the PTMF                      */
typedef struct _tsIposPtmfConfig
{
  U8  bIniMdioAddr;                       /* PHY MDIO address for base port 
                                              #0 of the INI ASIC              */
  U8  bIniMode;                           /* INI ASIC mode: MII/SMII/SSSMII   */
                                           
  U8  bIniMaxEnetPort;                    /* Max INI Enet ports: AMUR = 16    */
  U8  bEnetPhyAddr;                       /* System Ethernet PHY base address */
                                           
  U8  bIniMaxUtopiaPhy;                   /* Max INI Utopia ports: AMUR = 16  */

  U8  bIniMaxRxEnable;                    /* Max RX Enable Signal per PTMF    */
                                           
  U8  bUtopiaCellMode;                    /* Utopia Cell mode                 */
  U8  bUtopiaTrafficMode;                 /* Utopia Traffic mode              */
                                           
  U8  bHdlcSpecialBytes;                  /* HDLC Special byte:Idle Cell      */
  U8  bAtmCellHecByte;                    /* Byte 5 of an ATM cell            */
  U8  bAtmCellNonUnityHeaderByte;         /* Byte 4 of an ATM Cell            */

  U8  bCellLength;                        /* Utopia Cell Length               */
  U8  bPktStartEnd;                       /* packet start and end field       */

  U16 uiMiiPortMap;                       /* Bit Map of the xMII valid ports  */

  U8  bFlowControl;                       /* Ethernet Flow Control:           */
                                          /* PAUSE FRAME CAPABLE OR NOT       */
  U8  bDuplex;                            /* FULL or HALF DUPLEX              */
  U8  bPortSpeed;                         /* Port Speed: 10 or 100 Mbps       */


} tsIposPtmfConfig, *ptsIposPtmfConfig;

/*----------------------------------------------------------------------------*/
/* Port specific configuration parameters                                     */
typedef struct _tsIposPtmfPortConfig
{
  U8        bPortIndex;         /* Port Index = 0 - 15 per PTMF chip          */
  U8        bPtmfBaseAddr;      /* Base address of the PTMF chip of           
                                   which is port is part of                   */
  U8        bRxEnable;          /* RX enable slection for this port
                                   Equivalent to the BME number for the port  */
  U8        bPortAddr;          /* MDIO Address for the Port                  */
  U8        bUtopiaPort;        /* Corresponding Utopia Phy address           */
                                                                              
  U8        bFlowControl;       /* Ethernet Flow Control:                     */
                                /* NO_FLOW or FULL_DUPLEX                     */
  U8        bDuplex;            /* FULL or HALF DUPLEX                        */
  U8        bPortSpeed;         /* Port Speed: 10 or 100 Mbps                 */
                                                                              
  U8        bPauseFrameSrcAddr; /* Pause frames source address                */
  U32       uiPauseFrameCrc;    /* CRC for the pause frames                   */
  U32       uiResumeFrameCrc;   /* Resume frame CRC                           */
                                                                              
  U8        bTxFifoSize;        /* TX FIFO sizes                              */
  U8        bTxFifoAddr;        /* TX FIFO start address                      */
  U8        bTxFifoLwm;         /* TX FIFO Low water mark                     */
  U8        bTxFifoHwm;         /* TX FIFO High water mark                    */
                                                                              
  U8        bRxFifoSize;        /* RX FIFO sizes                              */
  U8        bRxFifoAddr;        /* RX FIFO start address                      */
  U8        bRxFifoHwm;         /* RX FIFO High water mark                    */
                                                                              
  U8        bTxHdlcHcf;         /* TX HDLC Control field                      */
  U8        bTxHdlcHaf;         /* TX HDLC Address field                      */
                                                                              
  U8        bFilterHdlcCrc;     /* Suppress HDLC CRC errors                   */
  U8        bFilterEnetCrc;     /* Suppress Ethernet CRC errors               */
                                                                              
  U8        bEnetLinkStatus;    /* Ethernet Link Status                       */
  U8        bEnetPhyAddr;       /* If PTM port = MAC mode, corresponding
                                   Ethernet PHY MDIO address                  */
  U8        bMiiMode;           /* PTMF mode (xMII loopback/PHY/MAC...etc)    */
  U8        bPtmfPortState;     /* Indicates port state; In showtime or not   */

} tsIposPtmfPortConfig, *ptsIposPtmfPortConfig;

typedef struct _tsIposPtmfEnetAddrFilterCfg
{
  U8        bAddrFilterEn;
  U8        bLearningEn;
  U8        bAgingEn;
  U8        bMultiCastFilterEn;
  U8        bBroadCastFilterEn;
  U16       uiAgingCount;
  U16       uiAddrEntryEn;
  U8        bStaticAddrCount;
  U8        bMaxReadCount;
  U8        abEnetAddr[MAX_FILTER_TABLE_ENTRY][6];

}tsIposPtmfEnetAddrFilterCfg, *ptsIposPtmfEnetAddrFilterCfg;


/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  EXTERNAL REFERENCES
  ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  PUBLIC FUNCTION PROTOTYPE
  ----------------------------------------------------------------------------*/
void 
  iposPtmfDbgLevelSet (U8 bDbgLevel);

void 
  iposPtmfGetVersion(char *psVerString);

/* Setup the PTMF framer                                                      */
eIniRetCode
  iposPtmfSetup (ptsIposPtmfConfig  ptsPtmfConfig);

eIniRetCode 
  iposPtmfInit (ptsIposPtmfConfig  ptsPtmfConfig);

eIniRetCode 
  iposPtmfStartTx (U8 bEnetPortNumber, ptsIposPtmfPortConfig ptsPtmfPortConfig);

eIniRetCode 
  iposPtmfStartRx (U8 bEnetPortNumber, ptsIposPtmfPortConfig ptsPtmfPortConfig);

eIniRetCode 
  iposPtmfStopTx (U8 bEnetPortNumber);

eIniRetCode 
  iposPtmfStopRx (U8 bEnetPortNumber);

void 
  iposPtmfReadCounters(U8 bEnetPortCount);

U32 
  iposPtmfEnetRxFrames (U8 bPortNumber) ;

U32 
  iposPtmfEnetRxBytes (U8 bPortNumber) ;

U32 
  iposPtmfEnetRxPauseFrames (U8 bPortNumber) ;

#ifndef CO_30
U32 
  iposPtmfEnetRxBrdcastFrames (U8 bPortNumber) ;
#endif

U32 
  iposPtmfEnetRxDroppedFrames (U8 bPortNumber) ;

U32 
  iposPtmfEnetRxAllignErrors (U8 bPortNumber) ;

U32 
  iposPtmfEnetRxOversize (U8 bPortNumber) ;

U32 
  iposPtmfEnetRxUndersize (U8 bPortNumber) ;

U32 
  iposPtmfEnetRxCrcErrors (U8 bPortNumber) ;

U32 
  iposPtmfEnetRxCarrierSenseErrors (U8 bPortNumber) ;

U32 
  iposPtmfEnetTxFrames (U8 bPortNumber) ;

U32 
  iposPtmfEnetTxBytes (U8 bPortNumber) ;

U32 
  iposPtmfEnetTxPauseFrames (U8 bPortNumber) ;

U32 
  iposPtmfHDLateCollisions (U8 bPortNumber) ;

U32 
  iposPtmfHDExcessCollisions (U8 bPortNumber) ;

U32 
  iposPtmfHDSingleCollisions (U8 bPortNumber) ;

U32 
  iposPtmfHDMultiCollisions (U8 bPortNumber) ;

U32 
  iposPtmfHDDeferredTrans (U8 bPortNumber) ;

U32 
  iposPtmfHdlcCrcErrors (U8 bPortNumber) ;

U32 
  iposPtmfHdlcInvalidFrames (U8 bPortNumber) ;

U32 
  iposPtmfHdlcDroppedFrames (U8 bPortNumber) ;

U32 
  iposPtmfHdlcGeneralErrors (U8 bPortNumber) ;

void 
  iposPtmfPrintCounters(U8 bEnetPortNumber);

void iposPtmfGetVersion(char *psVerString);

void 
  iposPtmfPrintPortStruct(U8 bEnetPortNumber);


eIniRetCode 
  iposPtmfReadPortConfig (U8 bEnetPortNumber, 
                          ptsIposPtmfPortConfig ptsPtmfPortConfig);
eIniRetCode 
  iposPtmfWritePortConfig (U8 bEnetPortNumber, 
                          ptsIposPtmfPortConfig ptsPtmfPortConfig);


/* Address filtering related function prototypes  */
eIniRetCode 
  iposPtmfRemoveAddrEntry (U8 addr);

eIniRetCode 
  iposPtmfReadAddrEntry (U8 bTableIndex, U8 *pbEnetAddr);

eIniRetCode 
  iposPtmfAddrTabInit ( tsIposPtmfEnetAddrFilterCfg  *ptsEnetAddrFilterConfig);

eIniRetCode 
  iposPtmfReadAddrTabStatus (
    tsIposPtmfEnetAddrFilterCfg  *ptsEnetAddrFilterConfig);

eIniRetCode iposPtmfAddAddrEntry (U8 bTableIndex, U8 *pbEnetAddr);

eIniRetCode 
  iposPtmfEnetAddrFilterClearTable ( void);

/* New functions  */
eIniRetCode
  iposPtmfGetConfigParam (U8 bPortNumber, 
                          eIposPtmfParamType eParamType, 
                          U32 *puiParam);
eIniRetCode
  iposPtmfGetCounter (U8 bPortNumber, 
                      eIposPtmfCounterType eCounterType, 
                      U32 *puiCounter);

eIniRetCode
  iposMdioRegRead(U8 bPortNumber, 
                  U16 uiRegAddr, 
                  U16 *puiRegData);

eIniRetCode
  iposMdioRegWrite(U8 bPortNumber, 
                  U16 uiRegAddr, 
                  U16 *puiRegData);

eIniRetCode
  iposPtmfSetConfigParam (U8 bPortNumber, 
                          eIposPtmfParamType eParamType, 
                          U32 uiParam);
eIniRetCode
  iposPtmfGetVersionNumber(U8 *pbType, U16 *puiPartNo, U8 *pbVerNo);

eIniRetCode
  iposPtmfSetCounter (U8 bPortNumber, 
                      eIposPtmfCounterType eCounterType, 
                      U32 uiCounter);

eIniRetCode
  dbgPtmfPortCounters (U8 bEnetPortNumber);

#if defined(CPE_30)
#ifndef CO_30
eIniRetCode 
  iposPtmfReadLearnedAddr (U8 bTableIndex, U8 *pbEnetAddr);
#endif
#endif //#if defined(CPE_30)

#if !defined(BME_CPE_20)
void iposPtmfReset(void);
#endif //#if !defined(BME_CPE_20)

#endif  // _HostPtmfApi_H_
/*------------------------------------------------------------------------------
                                 END OF FILE
------------------------------------------------------------------------------*/
