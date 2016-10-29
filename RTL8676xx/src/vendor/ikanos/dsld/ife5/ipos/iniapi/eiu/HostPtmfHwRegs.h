#ifdef INI_SUPPORT
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
**  Filename:    HostPtmfHwRegs.h
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
**  $Date: 2008/07/18 11:08:51 $
**
**
**  **************************************************************************** 
**  Modification History:
**
**    Date         By     Description
**    -----------  ---    ------------------------------------------------------
**	  -----------  RB     Created
**    Jun-06-2002  SFA    Imported from Raminder and modified for C
**    Jul-24-2002  SFA    Integration in new host code
*/
/******************************************************************************/

/* HEADER SENTINEL                                                            */
#ifndef _HostPtmfHwRegs_H_
#define _HostPtmfHwRegs_H_

////////////////////////////////////////////////////////////////////////////////
//                             Quad Specific registers
////////////////////////////////////////////////////////////////////////////////

//Quad register addresses
// Config regs

#define XPCR     0x220                // xMII port configuration reg

#if defined(BME_CPE_20)

  #define ACR      0x221                // Address filtering config reg
  #define ASER     0x222                // Addr filt static table enable reg
  #define TFCR0    0x223                // Tx_Fifo Config register 0
  #define TFCR1    0x224                // Tx_Fifo Config register 1
  #define RFCR0    0x225                // Rx_Fifo Config register 0
  #define RFCR1    0x226                // Rx_Fifo Config register 1
  #define CGRCR    0x227                // Clock Gating/SReset config reg 
  #define DBGR     0x228                // Ikanos debug features reg 
  #define RXPF     0x229                // Receive pause frame cotrol reg

#else

  #define TFCR0    0x221                // Tx_Fifo Config register 0
  #define TFCR1    0x222                // Tx_Fifo Config register 1
  #define RFCR0    0x223                // Rx_Fifo Config register 0
  #define RFCR1    0x224                // Rx_Fifo Config register 1
  #define CGRCR    0x225                // Clock Gating/SReset config reg 
  #define DBGR     0x226                // Ikanos debug features reg 
#endif

// Parameter regs
#define THAFR    0x230                // Tx HDLC Address/Control Field Register
#define RHAFR    0x231                // Rx HDLC Address/Control Field Register
#define MIFSR    0x232                // Min Frame size register
#define MAFSR    0x233                // Max Frame size register
#define XSCR     0x234                // xMII slot count reg 

#if defined(BME_CPE_20)
  
  #define AACR     0x23a                // Addr filtering aging count register
  #define JAMC     0x23b                // Jam sequence length register
  #define JAMD     0x23c                // Jam sequence data type register
  
#endif

#define PFSARLO    0x240                // [47:40] bits of the SA for PF Qx (0xE009_8900)
#define PFSARHI    0x241                // bit0-7 [47:40] bits of the SA for PF Q4 (0xE009_8904)
#define PFCRCQ0    0x242               // Represents the Pause Frame CRC Value for queue 0
#define RFCRCQ0    0x243               // Represents the Resume Frame CRC Value for queue 0

#define PQER       0x24c                //Pause Queue Enable Register 

// Counter regs
#define TXFCR0   0x250                // Eth Tx frames
#define TXFCR1   0x251                // Eth Tx frames
#define OTCR0    0x252                // Octets transmitted counter
#define OTCR1    0x253                // Octets transmitted counter
#define EDFR0    0x254                // Ethernet dropped frame count register
                                      // [15:0]
#define EDFR1    0x255                // Ethernet dropped frame count register 
                                      // [31:16]
#define AECR0    0x256                // Alignment err counter reg
#define AECR1    0x257                // Alignment err counter reg
#define FTLCR0   0x258                // Frame too long counter reg
#define FTLCR1   0x259                // Frame too long counter reg
#define RECR0    0x25a                // Rx err counter reg
#define RECR1    0x25b                // Rx err counter reg
#define CECR0    0x25c                // Eth CRC error counter reg
#define CECR1    0x25d                // Eth CRC error counter reg

#define CSER0    0x260                // Carrier sense error counter
#define CSER1    0x261                // Carrier sense error counter
#define BFCR0    0x262                // Brd cast frame count register
#define BFCR1    0x263                // Brd cast frame count register
#define TPFCR    0x264                // Tx Pause frame count register
#define RXFCR0   0x265                // Eth Rx frames
#define RXFCR1   0x266                // Eth Rx frames
#define ORCR0    0x267                // Octets recieved counter
#define ORCR1    0x268                // Octets recieved counter
#define RHECR0   0x269                // Rx HDLC Error Frame Count Register
#define RHECR1   0x26a                // Rx HDLC Error Frame Count Register
#define RHICR0   0x26b                // Rx HDLC Invalid Frame Count Register
#define RHICR1   0x26c                // Rx HDLC Invalid Frame Count Register
#define PFCTR    0x26d                // Rx Pause frame count register

#define FILT0    0x26e                // Filtered frame counter0
#define FILT1    0x26f                // Filtered frame counter1

#define RHDCR0   0x270                // Rx HDLC Dropped Frame Count Register
#define RHDCR1   0x271                // Rx HDLC Dropped Frame Count Register
#define HLCCR0   0x272                // HD Late coll counter reg
#define HLCCR1   0x273                // HD Late coll counter reg
#define HECCR0   0x274                // HD Excess coll counter reg
#define HECCR1   0x275                // HD Excess coll counter reg
#define HSCCR0   0x276                // HD Single collision counter reg
#define HSCCR1   0x277                // HD Single collision counter reg
#define HMCCR0   0x278                // HD Multi collision counter reg
#define HMCCR1   0x279                // HD Multi collision counter reg
#define HDTCR0   0x27a                // HD Def Trans counter reg
#define HDTCR1   0x27b                // HD Def Trans counter reg
#define PFTMR    0x27c                // Pause Frame timer Reg

#define SAT00    0x280
#define SAT01    0x281
#define SAT02    0x282

#define SAT10    0x283
#define SAT11    0x284
#define SAT12    0x285

#define SAT20    0x286
#define SAT21    0x287
#define SAT22    0x288

#define SAT30    0x289
#define SAT31    0x28a
#define SAT32    0x28b

#define SAT40    0x28c
#define SAT41    0x28d
#define SAT42    0x28e

#define SAT50    0x290
#define SAT51    0x291
#define SAT52    0x292

#define SAT60    0x293
#define SAT61    0x294
#define SAT62    0x295

#define SAT70    0x296
#define SAT71    0x297
#define SAT72    0x298

#define MFAR0    0x300                // Ikanos fifo access register 0
#define MFAR1    0x301                // Ikanos fifo access register 1
#define MFAR2    0x302                // Ikanos fifo access register 2
#define MFAR3    0x303                // Ikanos fifo access register 3

#if defined(CPE_50)
// EIU Counters
#define	EIU_TXFCR0					0xE0098940
#define	EIU_RXFCR0					0xE0098994
#define	EIU_EDFR0					0xE0098950
#define	EIU_OTCR0					0xE0098948
#define	EIU_AECR0					0xE0098958
#define	EIU_FTLCR0					0xE0098960
#define   EIU_FTSCR0                             0xE0098968
#define	EIU_CECR0					0xE0098970
#define	EIU_CSER0					0xE0098980
#define	EIU_BFCR0					0xE0098988
#define	EIU_TPFCR					0xE0098990
#define	EIU_PFCTR					0xE00989B4
#define	EIU_ORCR0					0xE009899C
#define	EIU_RHECR0					0xE00989A4
#define	EIU_RHDCR0					0xE00989C0
#define	EIU_HLCCR0					0xE00989C8
#define	EIU_HECCR0					0xE00989D0
#define	EIU_HSCCR0					0xE00989D8
#define	EIU_HMCCR0					0xE00989E0
#define	EIU_HDTCR0					0xE00989E8

#define EIU_PFSARLO     0xE0098900
#define EIU_PFSARHI     0xE0098904
#define EIU_PFCRCQ0     0xE0098908
#define EIU_RFCRCQ0     0xE009890C
#define EIU_PFCRCQ1     0xE0098910
#define EIU_RFCRCQ1     0xE0098914
#define EIU_PFCRCQ2     0xE0098918
#define EIU_RFCRCQ2     0xE009891C
#define EIU_PFCRCQ3     0xE0098920
#define EIU_RFCRCQ3     0xE0098924
#define EIU_PFCRCQ4     0xE0098928
#define EIU_RFCRCQ4     0xE009892C
#define EIU_PQER           0xE0098930

#define     TX_SLB_QUEUE_USE_REG 0xE00D0060 
#define     RX_SLB_CTRL_REG            0xE00F0000
#define RX_HDE_CRC_ERR_CNT_REG 0xE0068118
#define RX_SLB_QDRP_CNT_REG       0xE00F0080      //Rx SLB Queue Drop Counters
#endif //CPE_50
#if defined(CPE_30)

#define LTRR0    0x304                // 
#define LTRR1    0x305                // 
#define LTRR2    0x306                // 
#define LTRAD    0x307                // 

#endif //#if defined(CPE_30)

//Dummies to be removed once test_prog is updated
#define TTFCR    0x30a
#define RTFCR    0x30a
#define PFSAR0   0x30a                // Pause Frame src address register SA[15:0] 
#define PFSAR1   0x30a                // Pause Frame src address register SA[31:16] 
#define PFSAR2   0x30a                // Pause Frame src address register SA[47:32] 
#define PFDAR0   0x30a                // Pause Frame dest address register DA[15:0]
#define PFDAR1   0x30a                // Pause Frame dest address register DA[31:16]
#define PFDAR2   0x30a                // Pause Frame dest address register DA[47:32]
#define PFRR     0x30a                // Pause  Frame retransmit offset register
#define RHECR    0x30a                // Rx HDLC Error Frame Count Register
#define RHICR    0x30a                // Rx HDLC Invalid Frame Count Register
#define RHDCR    0x30a                // Rx HDLC Dropped Frame Count Register
#define TFSR     0x30a
#define RFSR     0x30a

// Quad register default values

#define XPCR_DEFAULT     0x7b02       // retry limit = 16, min preamble = 3, 100BaseT, Full Duplex
#define TFCR0_DEFAULT    0x0000
#define TFCR1_DEFAULT    0x0000
#define RFCR0_DEFAULT    0x0000
#define RFCR1_DEFAULT    0x0000
#define MIFSR_DEFAULT    0x0040           // min frame size   64
#define MAFSR_DEFAULT    0x0600           // max frame size 1536
#define XSCR_DEFAULT     0x019a           // 410, 512 bit times , 100BaseT at 125MHz
#define THAFR_DEFAULT    0x03ff
#define CGRCR_DEFAULT    0x0003

#define EDFR0_DEFAULT    0x0000
#define EDFR1_DEFAULT    0x0000
#define AECR_DEFAULT     0x0000
#define FTLCR_DEFAULT    0x0000
#define RECR_DEFAULT     0x0000
#define CECR_DEFAULT     0x0000
#define CSER_DEFAULT     0x0000
#define TXFCR_DEFAULT    0x0000
#define RXFCR_DEFAULT    0x0000
#define HSCCR_DEFAULT    0x0000
#define HMCCR_DEFAULT    0x0000
#define HDTCR_DEFAULT    0x0000
#define HLCCR_DEFAULT    0x0000
#define HECCR_DEFAULT    0x0000

#define PFSAR_DEFAULT    0x0000
#define PFCTR_DEFAULT    0x0000
#define TPFCR_DEFAULT    0x0000
#define TFRPR_DEFAULT    0x0000
#define TFWPR_DEFAULT    0x0000
#define RFRPR_DEFAULT    0x0000
#define RFWPR_DEFAULT    0x0000
#define ORCR_DEFAULT     0x0000
#define RHAFR_DEFAULT    0x0000

#define RHECR_DEFAULT    0x0000
#define RHICR_DEFAULT    0x0000
#define RHDCR_DEFAULT    0x0000
#define TSOFPR_DEFAULT   0x0000
#define RSOFPR_DEFAULT   0x0000
#define OTCR_DEFAULT     0x0000
#define DBGR_DEFAULT     0x0000
#define BFCR_DEFAULT     0x0000
#define MFAR0_DEFAULT    0x0000
#define MFAR1_DEFAULT    0x0000
#define MFAR2_DEFAULT    0x0000
#define MFAR3_DEFAULT    0x0000

// Need as hard coded values
#define PFSAR0_DEFAULT   0x0000
#define PFSAR1_DEFAULT   0x0000
#define PFSAR2_DEFAULT   0x0000
#define PFDAR0_DEFAULT   0x0001
#define PFDAR1_DEFAULT   0xc200
#define PFDAR2_DEFAULT   0x0180
#define PFCR0_DEFAULT    0x5657
#define PFCR1_DEFAULT    0x5554
#define RFCRR0_DEFAULT   0xacad
#define RFCRR1_DEFAULT   0xaaab
#define PFRR_DEFAULT     0xfffa

#if !defined(CPE_30)
////////////////////////////////////////////////////////////////////////////////////////
//                             Utopia Specific registers
////////////////////////////////////////////////////////////////////////////////////////

#define UPR_0      0x0c0              // Utopia Poll Register 0
#define UPR_1      0x0c1              // Utopia Poll Register 1
#define UPR_2      0x0c2              // Utopia Poll Register 2
#define UPR_3      0x0c3              // Utopia Poll Register 3
#define UPR_4      0x0c4              // Utopia Poll Register 4
#define UPR_5      0x0c5              // Utopia Poll Register 5
#define UPR_6      0x0c6              // Utopia Poll Register 6
#define UPR_7      0x0c7              // Utopia Poll Register 7
#define UPR_8      0x0c8              // Utopia Poll Register 8
#define UPR_9      0x0c9              // Utopia Poll Register 9
#define UPR_A      0x0ca              // Utopia Poll Register A
#define UPR_B      0x0cb              // Utopia Poll Register B
#define UPR_C      0x0cc              // Utopia Poll Register C
#define UPR_D      0x0cd              // Utopia Poll Register D
#define UPR_E      0x0ce              // Utopia Poll Register E
#define UPR_F      0x0cf              // Utopia Poll Register F

#define UPAR       0x0d0              // Utopia Phys Active Register -Global
#define UCR0       0x0d1              // Utopia Configuration Register0 -Global
#define UCR1       0x0d2              // Utopia Configuration Register1 -Global
#define HSBR0      0x0d3              // Utopia HDLC Special Bytes Register 0 -Global

#define UMR_0      0x0e0              // Utopia Monitoring Register 0
#define UMR_1      0x0e1              // Utopia Monitoring Register 1
#define UMR_2      0x0e2              // Utopia Monitoring Register 2
#define UMR_3      0x0e3              // Utopia Monitoring Register 3
#define UMR_4      0x0e4              // Utopia Monitoring Register 4
#define UMR_5      0x0e5              // Utopia Monitoring Register 5
#define UMR_6      0x0e6              // Utopia Monitoring Register 6
#define UMR_7      0x0e7              // Utopia Monitoring Register 7
#define UMR_8      0x0e8              // Utopia Monitoring Register 8
#define UMR_9      0x0e9              // Utopia Monitoring Register 9
#define UMR_A      0x0ea              // Utopia Monitoring Register a
#define UMR_B      0x0eb              // Utopia Monitoring Register b
#define UMR_C      0x0ec              // Utopia Monitoring Register c
#define UMR_D      0x0ed              // Utopia Monitoring Register d
#define UMR_E      0x0ee              // Utopia Monitoring Register e
#define UMR_F      0x0ef              // Utopia Monitoring Register f

#endif //#if !defined(CPE_30)

#define HSBR0_DEFAULT   0x1e

////////////////////////////////////////////////////////////////////////////////////////
//                             MDIO Specific registers
////////////////////////////////////////////////////////////////////////////////////////
#define MMR_CONFIG   0x000              //MDIO PHY configuration register
#define MMR_STAT_0   0x001              //MDIO PHY status register 0
#define MMR_STAT_1   0x002              //MDIO PHY status register 1
#define MMR_CLN_TEST 0x003              //MDIO collision test register
#define MMR_PHYID    0x004              //MDIO PHY ID register

#define AMUR_INDEX   0x01f              //AMUR Global Register Address Index.
////////////////////////////////////////////////////////////////////////////////
//             BURST MODE ENGINE (BME) REGISTERS      
////////////////////////////////////////////////////////////////////////////////
#define BME1_CMD     0x020              //Command Port
#define BME1_ADR     0x021              //address Port
#define BME1_DP      0x022              //data Port
#define BME1_DRA     0x023              //DMA Register Address Port
#define BME1_DL      0x024              //DMA Length Port
#define BME1_DSA     0x025              //DMA Source Address Port
#define BME1_DDA     0x026              //DMA Destn Address Port
#define BME1_DDP     0x027              //DMA Data Port
#define BME1_HACK    0x028              //DMA Host ACK port
#define BME1_DC      0x029              //DMA Control Port
#define BME1_DS      0x02a              //DMA Status Port
#define BME1_TSR     0x030              //BME1 Transaction Status Register

#if defined(BME_CPE_20)
  #define MMDI_WDAT    0x020              //data register for mdi write 
                                          //operation. A write to this 
                                          //register initiates a write 
                                          //operation
  #define MMDI_ADD     0x021              //Phy regiter address register 
                                          //for mdi read/write operation
  #define MMDI_RD      0x022              //mdi read register a read to this 
                                          //register initiates a read operation
// NOTE: Second read operation to address 024 is required to read the actual 
// data returened by mdio
  #define MMDI_OP      0x023              //list two bits reflects the state 
                                          //of read and write operation bits.
                                          //bit 0 is set when in read state
					                                //bit 1 is set when in write state
  #define MMDI_RDAT    0x024              //This register returns the read 
                                          //value

  #define MDIO_OP_READ  0x01              // Register bit definition (Bit 0)
  #define MDIO_OP_WRITE 0x02              // Register bit definition (Bit 1)

#endif

#define BME2_CMD     0x040              //Command Port
#define BME2_ADR     0x041              //address Port
#define BME2_DP      0x042              //data Port
#define BME2_DRA     0x043              //DMA Register Address Port
#define BME2_DL      0x044              //DMA Length Port
#define BME2_DSA     0x045              //DMA Source Address Port
#define BME2_DDA     0x046              //DMA Destn Address Port
#define BME2_DDP     0x047              //DMA Data Port
#define BME2_HACK    0x048              //DMA Host ACK port
#define BME2_DC      0x049              //DMA Control Port
#define BME2_DS      0x04a              //DMA Status Port
#define BME2_TSR     0x050              //BME2 Transaction Status Register

#if defined(BME_CPE_20)
  #define SMDI_STAT    0x040              //slave mdio status register R/W 
                                          //by pio read only by mdio
  #define SMDI_MISC    0x041              //smdi misc control register. 
//writing 1  to bit 0 clears interrupt, read will read the state of intrrupt
//writing 1  to bit 1 will mask the interrupt

  #define SMDI_CTL     0x042              //smdi control register. R/W by 
                                          //both pio and mdio
#endif

#define BME3_CMD     0x060              //Command Port
#define BME3_ADR     0x061              //address Port
#define BME3_DP      0x062              //data Port
#define BME3_DRA     0x063              //DMA Register Address Port
#define BME3_DL      0x064              //DMA Length Port
#define BME3_DSA     0x065              //DMA Source Address Port
#define BME3_DDA     0x066              //DMA Destn Address Port
#define BME3_DDP     0x067              //DMA Data Port
#define BME3_HACK    0x068              //DMA Host ACK port
#define BME3_DC      0x069              //DMA Control Port
#define BME3_DS      0x06a              //DMA Status Port
#define BME3_TSR     0x070              //BME3 Transaction Status Register

#define BME4_CMD     0x080              //Command Port
#define BME4_ADR     0x081              //address Port
#define BME4_DP      0x082              //data Port
#define BME4_DRA     0x083              //DMA Register Address Port
#define BME4_DL      0x084              //DMA Length Port
#define BME4_DSA     0x085              //DMA Source Address Port
#define BME4_DDA     0x086              //DMA Destn Address Port
#define BME4_DDP     0x087              //DMA Data Port
#define BME4_HACK    0x088              //DMA Host ACK port
#define BME4_DC      0x089              //DMA Control Port
#define BME4_DS      0x08a              //DMA Status Port
#define BME4_TSR     0x090              //BME4 Transaction Status Register

////////////////////////////////////////////////////////////////////////////////
//             PHY ADDRESS REGISTERS PHY 1 TO PHY 15  
////////////////////////////////////////////////////////////////////////////////
#define PHY1_ADD     0x0a1              //PHY 1 Address
#define PHY2_ADD     0x0a2              //PHY 2 Address
#define PHY3_ADD     0x0a3              //PHY 3 Address
#define PHY4_ADD     0x0a4              //PHY 4 Address
#define PHY5_ADD     0x0a5              //PHY 5 Address
#define PHY6_ADD     0x0a6              //PHY 6 Address
#define PHY7_ADD     0x0a7              //PHY 7 Address
#define PHY8_ADD     0x0a8              //PHY 8 Address
#define PHY9_ADD     0x0a9              //PHY 9 Address
#define PHY10_ADD    0x0aa              //PHY 10 Address
#define PHY11_ADD    0x0ab              //PHY 11 Address
#define PHY12_ADD    0x0ac              //PHY 12 Address
#define PHY13_ADD    0x0ad              //PHY 13 Address
#define PHY14_ADD    0x0ae              //PHY 14 Address
#define PHY15_ADD    0x0af              //PHY 15 Address

////////////////////////////////////////////////////////////////////////////////
//             HOST INTERFACE CONTROLLER REGISTERS    
////////////////////////////////////////////////////////////////////////////////
#define HIC_RST      0x100              //HIC crls and hrst
#define HIC_TSR      0x101              //HIC transaction status
#define QDM_TSR      0x102              //QDM transaction status
#define DBG_DVID     0x103              //Debug select and Device ID
#define CLK_PLLNM    0x104              //N and M values for the main clk
#define UCLK_PLLNM   0x105              //N and M values for the utopia clk
#define PLL_TEST     0x106              //PLL status bits
#define RST_STAT     0x107              //port nad hrst bits
#define HIC_INTR     0x108              //HIC intr lines
#define BME_EN       0x109              //BME ENABLE 

#define HIC_ACK      0x120              //HIC Ack Status. Broadcast mode.

////////////////////////////////////////////////////////////////////////////////
// Bit definition for bIniMdioAddr field of the iposPtmfConfig structure
#define PTMF_MDIO_ADDR_MASK       0x1f
#define PTMF_BUS_MASK             0xe0

#define PTMF_BUS_HIC_PIO          0xe0

#define AZTEC_EIU_BASE_ADDR       0xe0098000

#define PTMF_STR                  "EIU"
#define PTMF_ASIC_PART            22111
#define PTMF_ASIC_VERSION         1
#define PTMF_ASIC_TYPE            3


////////////////////////////////////////////////////////////////////////////////
// Bit definition for EIU registers
#define ADDRFILT_EN               0x0001
#define AGING_EN                  0x0002
#define LEARNING_EN               0x0004
#define MLTCST_CONFIG             0x0008
#define BRDCST_CONFIG             0x0010
#define FLUSH_TABLE               0x0020

#endif  // _HostPtmfHwRegs_H_
/*------------------------------------------------------------------------------
                                 END OF FILE
------------------------------------------------------------------------------*/
#endif
