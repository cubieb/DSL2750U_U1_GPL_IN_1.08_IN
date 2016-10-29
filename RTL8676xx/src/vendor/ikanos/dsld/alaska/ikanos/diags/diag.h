
/**************************************************************
***************************************************************
**
** Host Application, Copyright (C) 2000-2002
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

#include "apitype.h"

/* diags error code	*/

#define	DIAGS_ERROR_BASE	                    0xD000
#define	DIAGS_ERROR_BME_REG_STRESS_0		    (DIAGS_ERROR_BASE+1)
#define	DIAGS_ERROR_BME_REG_STRESS_F		    (DIAGS_ERROR_BASE+2)
#define	DIAGS_ERROR_BME_REG_STRESS_5A		    (DIAGS_ERROR_BASE+3)
#define	DIAGS_ERROR_BME_REG_STRESS_A5		    (DIAGS_ERROR_BASE+4)
#define	DIAGS_ERROR_BME_DMA_XFIFO   		    (DIAGS_ERROR_BASE+5)
#define	DIAGS_ERROR_BME_DMA_XFIFO_INTER		    (DIAGS_ERROR_BASE+6)
#define	DIAGS_ERROR_BME_DMA_FMC_I               (DIAGS_ERROR_BASE+7)
#define	DIAGS_ERROR_BME_DMA_YFIFO_FAST_MEM      (DIAGS_ERROR_BASE+8)
#define	DIAGS_ERROR_BME_DMA_YFIFO_INTER         (DIAGS_ERROR_BASE+9)
#define	DIAGS_ERROR_BME_DMA_TONE_TABLE          (DIAGS_ERROR_BASE+10)
#define	DIAGS_ERROR_BME_DMA_MAP_GAIN            (DIAGS_ERROR_BASE+11)
#define	DIAGS_ERROR_BME_DMA_ZFIFO_MEM           (DIAGS_ERROR_BASE+12)
#define	DIAGS_ERROR_BME_DMA_CFIFO_MEM           (DIAGS_ERROR_BASE+13)
#define	DIAGS_ERROR_BME_DMA_DMC_I               (DIAGS_ERROR_BASE+14)
#define	DIAGS_ERROR_BME_DMA_DMC_D               (DIAGS_ERROR_BASE+15)
#define	DIAGS_ERROR_BME_DMA_SMC_SNR             (DIAGS_ERROR_BASE+16)
#define	DIAGS_ERROR_BME_DMA_SMC_TONE            (DIAGS_ERROR_BASE+17)
#define	DIAGS_ERROR_BME_DMA_SMC_THRES           (DIAGS_ERROR_BASE+18)
#define	DIAGS_ERROR_BME_DMA_BFIFO_FAST_MEM      (DIAGS_ERROR_BASE+19)
#define	DIAGS_ERROR_BME_DMA_BFIFO_INTER         (DIAGS_ERROR_BASE+20)
#define	DIAGS_ERROR_BME_DMA_BFIFO_DEMAP         (DIAGS_ERROR_BASE+21)
#define	DIAGS_ERROR_BME_DMA_DEMAP               (DIAGS_ERROR_BASE+22)
#define	DIAGS_ERROR_BME_DMA_FEQ_TABLE           (DIAGS_ERROR_BASE+23)
#define	DIAGS_ERROR_BME_DMA_RMC_I               (DIAGS_ERROR_BASE+24)
#define	DIAGS_ERROR_BME_DMA_RMC_D               (DIAGS_ERROR_BASE+25)
#define	DIAGS_ERROR_BME_DMA_AFIFO_MEM           (DIAGS_ERROR_BASE+26)
#define	DIAGS_ERROR_BME_DMA_FMC_D               (DIAGS_ERROR_BASE+27)
#define	DIAGS_ERROR_EXT_DMA                     (DIAGS_ERROR_BASE+28)

#define	DIAGS_ERROR_BME_DMA_VBUS0_AFE           (DIAGS_ERROR_BASE+30)
#define	DIAGS_ERROR_BME_DMA_VBUS0_IFE           (DIAGS_ERROR_BASE+31)
#define	DIAGS_ERROR_BME_DMA_VBUS1_AFE           (DIAGS_ERROR_BASE+32)
#define	DIAGS_ERROR_BME_DMA_VBUS1_IFE           (DIAGS_ERROR_BASE+33)

#define	DIAGS_ERROR_BME_TNI_PIO_XFIFO           (DIAGS_ERROR_BASE+40)
#define	DIAGS_ERROR_BME_TNI_PIO_TNI             (DIAGS_ERROR_BASE+41)
#define	DIAGS_ERROR_BME_MICRO_PIO_CONT          (DIAGS_ERROR_BASE+42)
#define	DIAGS_ERROR_BME_TNI_PIO_YFIFO           (DIAGS_ERROR_BASE+43)
#define	DIAGS_ERROR_BME_MAPPER_PIO              (DIAGS_ERROR_BASE+44)
#define	DIAGS_ERROR_BME_TNI_PIO_ZFIFO           (DIAGS_ERROR_BASE+45)
#define	DIAGS_ERROR_BME_CSTLOW_PIO              (DIAGS_ERROR_BASE+46)
#define	DIAGS_ERROR_BME_CSTHIGH_PIO             (DIAGS_ERROR_BASE+47)
#define	DIAGS_ERROR_BME_AFIFO_PIO               (DIAGS_ERROR_BASE+48)
#define	DIAGS_ERROR_BME_RCV_MICRO_PIO           (DIAGS_ERROR_BASE+49)
#define	DIAGS_ERROR_BME_FMC_MICRO_PIO           (DIAGS_ERROR_BASE+50)
#define	DIAGS_ERROR_BME_DEMAPPER_PIO            (DIAGS_ERROR_BASE+51)
#define	DIAGS_ERROR_BME_DMC_PIO                 (DIAGS_ERROR_BASE+52)
#define	DIAGS_ERROR_BME_CFIFO_PIO               (DIAGS_ERROR_BASE+53)
#define	DIAGS_ERROR_BME_RNI_PIO                 (DIAGS_ERROR_BASE+54)
#define	DIAGS_ERROR_BME_INTER_PIO               (DIAGS_ERROR_BASE+55)
#define	DIAGS_ERROR_BME_XFIFO_PIO               (DIAGS_ERROR_BASE+56)
#define	DIAGS_ERROR_BME_YFIFO_PIO               (DIAGS_ERROR_BASE+57)
#define	DIAGS_ERROR_BME_BFIFO_PIO               (DIAGS_ERROR_BASE+58)

#define PASSED	0              
#define FAILED  1
#define RAM_LOW_ADRS         0x00100000        /* RAM address for sys image  */
#define RAM_HIGH_ADRS        0x00E00000        /* RAM address for ROM boot   */
#define	COM1_BASE_ADR		 0xffef0000

//#define	PIO_BID					0xE0070014	/* GL		Clock module config options*/
//#define	CKM_CCR					0xE0070004	/* GL		Clock module config options*/

#define MAX_LINE        160	/* max line length for input to 'm' routine */
#define DEC		FALSE	/* getArg parameters */
#define HEX		TRUE
#define OPT		TRUE

#ifdef BME_CPE_20 
#define CODE_MEMORY_ADR		0x20000000
#define BME_CHIP_ID		    0x11
#else
#define BME_CHIP_ID		    0x1
#define CODE_MEMORY_ADR		0x40000000
#endif

#ifndef BIT31
#define BIT31		        0x80000000
#endif
#define MDIO_POLLING        0xe009808c


#define DMA_MEM_ADR         0xc0000000

#define MAN_DIAGS_NUM1      45
#define MAN_DIAGS_NUM2      32

#define	ADDRESS_SMCONTROLTONEMEMORY		0xD2002000	/* 8 KB		DMA		*/

#define	ADDRESS_SMCONTROLTHRESMEMORY	0xD2002180	/* 8 KB		DMA		*/

#define	CAFT_B	            0xE0058100	/* CH		A FIFO Context Table B  	AFIFO control attributes on a channel basis*/

#define	MDIO_REG_1          0xe0098080  /* MDIO register 1 */
#define	MDIO_REG_2          0xe0098084  /* MDIO register 2 */
#define	MDIO_REG_3          0xe0098088  /* MDIO register 2 */
#define	MDIO_REG_4          0xe0098090  /* MDIO register 2 */


#define	FPGA_REG_1          0xe00a6000  /* FPGA register 1 */
#define	FPGA_REG_2          0xe00a6004  /* FPGA register 2 */
#define	FPGA_REG_3          0xe00a6008  /* FPGA register 3 */
#define	FPGA_REG_4          0xe00a600c  /* FPGA register 4 */


#define	UART_REG_1          0xe00a0000  /* UART register 1 */
#define	UART_REG_2          0xe00a0004  /* UART register 2 */
#define	UART_REG_3          0xe00a0008  /* UART register 3 */
#define	UART_REG_4          0xe00a000c  /* UART register 4 */


#define FLASH_PROGRAM_ADDR  				0x40000010
#define FLASH_PROGRAM_FLAG1  				0x12345678
#define FLASH_PROGRAM_FLAG2  				0x87654321

#define HL_FLASH_SECT_SIZE	0x200000

#define writeControlPort_diag(x,y,z)			(*(x+y) = z);iposDelay()
#define readControlPort_diag(x,y,z)			    z = (*(x+y));iposDelay()
/******************************************************************/
  // BroadCom BCM5221 specific definitions (USED IN CPE 2.0 MII)
  #define BC_ENET_PHY_SPEED_STATUS_REG   5
  #define BC_ENET_PHY_LINK_SPEED         0x180

  #define BC_ENET_PHY_DUPLEX_STATUS_REG  5
  #define BC_ENET_PHY_LINK_DUPLEX        0x0a0

  #define BC_ENET_PHY_PAUSE_STATUS_REG   5
  #define BC_ENET_LINK_PAUSE_CAPABLE     0x0400
/******************************************************************/
  // Intel LXT971A specific definitions (USED IN CPE 2.0 MII)
  #define INTEL_ENET_PHY_SPEED_STATUS_REG   17
  #define INTEL_ENET_PHY_LINK_SPEED         0x4000

  #define INTEL_ENET_PHY_DUPLEX_STATUS_REG  17
  #define INTEL_ENET_PHY_LINK_DUPLEX        0x200

  #define INTEL_ENET_PHY_PAUSE_STATUS_REG   5
  #define INTEL_ENET_LINK_PAUSE_CAPABLE     0x400

/******************************************************************/
  // Davicom DM9161 specific definitions (USED IN CPE 2.0 RMII)
  #define DM_ENET_PHY_SPEED_STATUS_REG   17
  #define DM_ENET_PHY_LINK_SPEED         0xC000

  #define DM_ENET_PHY_DUPLEX_STATUS_REG  17
  #define DM_ENET_PHY_LINK_DUPLEX        0xA000
  
  #define DM_ENET_PHY_PAUSE_STATUS_REG      5
  #define DM_ENET_LINK_PAUSE_CAPABLE        0x400

  #define BC_PTMF_ENET_PHY_BASE_ADDR     (0xe4)
  #define INTEL_PTMF_ENET_PHY_BASE_ADDR     (0xf8)
  #define DM_PTMF_ENET_PHY_BASE_ADDR        (0xf8)
  #define AT_PTMF_ENET_PHY_BASE_ADDR        (0xf9)
  
  #define BC_PTMF_ENET_PHY_TEST_ADDR        (0xe4)
  #define INTEL_PTMF_ENET_PHY_TEST_ADDR     (0x14)
  #define DM_PTMF_ENET_PHY_TEST_ADDR        (0xf8)
  #define AT_PTMF_ENET_PHY_TEST_ADDR        (0x1e)
  
typedef struct diagConfTbl {
    U8  PhyBaseAddr;
    U8  PhyMdioTestAddr;
    U32 SDRAMBuswidth;
    U32 SDRAMClkSpeed;
} tdiagConfTbl;


/*------------------------------------------------------------------------------
  PRIVATE FUNCTION PROTOTYPE
  ----------------------------------------------------------------------------*/

extern int xfifo_fast_mem_test(void);
extern int xfifo_inter_test(void);
extern int ext_test(void);
extern int fmc_i_test(void);
extern int fmc_d_test(void);
extern int Yfifo_fast_mem_test(void);
extern int yfifo_inter_test(void);
extern int tone_table_test(void);
extern int mapper_gain_test(void);
extern int zfifo_mem_test(void);
extern int cfifo_mem_test(void);
extern int dmc_i_test(void);
extern int dmc_d_test(void);
extern int smc_snr_test(void);
extern int smc_tone_test(void);
extern int smc_thres_test(void);
extern int bfifo_fast_mem_test(void);
extern int bfifo_inter_test(void);
extern int bfifo_demapper_test(void);
extern int demapper_test(void);
extern int feq_test(void);
extern int teq_test(void);
extern int rmc_i_test(void);
extern int rmc_d_test(void);
extern int afifo_mem_test(void);
extern void bme_dma_test(void);
extern int vbus0_test(void);
extern int vbus1_test(void);

extern UINT32 TNI_PioTst();
extern UINT32 Xfifo_PioTst();
extern UINT32 Micro_cont_PioTst();
extern UINT32 Yfifo_PioTst();
extern UINT32 Mapper_PioTst();
extern UINT32 Zfifo_PioTst();
extern UINT32 VBus_PioTst();
extern UINT32 Afifo_PioTst();
extern UINT32 Rcv_Micro_cont_PioTst();
extern UINT32 Dmap_PioTst();
extern UINT32 Bfifo_PioTst();
extern UINT32 Dfrm_Micro_cont_PioTst();
extern UINT32 Cfifo_PioTst();
extern UINT32 RNI_PioTst();
extern UINT32 Pio_Interface_PioTst();


extern void HostEpldInterfaceInit (void);
extern void HostAfeAddrSet(char);
extern void sysBMEInterfaceInit (void);
extern void hostIntInit();
extern void sysConfigPortPinsForBMEInterface ( void );
extern void cloop (UINT32 loopCount, UINT32 cellCount);
extern void afe2_recv(int line_num, int trial);
extern void afe2_lb(int, int);
extern void co_tx(int, int, int);
extern int ref_afe_vbus0_pio_rw (int afenum);
extern int ref_afe_vbus1_pio_rw (int afenum);
extern void bmeHwResetApi (int BmeNum);
extern UINT32 ref_host_pio_rd_wr (int argc, char * argv[]);
extern int HostIniFpgaProgm(void);
extern void x2cloop (UINT32 loopCount);
extern void iposDelay (void);
extern void testPIOWrite_diag (int BmeNum, unsigned long addr, unsigned long data);
extern int testPIORead_diag (int BmeNum, unsigned long addr);
extern void configVbus0();
extern void configVbus1();
extern UINT32 rd0Reg(UINT32 adr);
extern void iniread(BYTE addr, BYTE *data);

extern char imageFlagSet(char index);
extern char indexFlagGet(void);
extern char bootloadInfo(void);
extern void getFlashInfo(char	*info, int offset, int len);
extern void saveFlashInfo(char	*info, int offset, int len);
extern void cx_loop (int cellCount);
extern void dbgEnetPhyWrite (U16 uiEnetPhyAddr, U16 uiRegAddr, U16 uiWriteData);
extern void ethDl (int clkRate,int iMode);
extern STATUS remoteDownLoadImage( int vxWorksImage, char *hostName, char *fileName);
extern void doDmaWrite(UINT32 destAddr, BYTE *dataPtr, UINT32 dataLen, int destFifoFlag);
extern void getDiagFlashInfo(char	*info, int offset, int len);
extern void saveDiagFlashInfo(char	*info, int offset, int len);
extern int calCS (int address, int length);
extern void lftftp (int len, char *hostName, char *fileName);
extern char getSysInfo (char *sysInfo);
extern void init_sdram_32clk ();
extern void init_sdram_16clk ();
extern void EventLog(const char *output, ...);
extern void setProdMode (unsigned char enable);

void testPIOWrite_diag (int BmeNum, unsigned long addr, unsigned long data);
void PIOReadData( int * sAddr, int * pAddr, int Len);
void diag_menu(void);

void bmeHicTest(void);
void dacOutTest (void);
void resetTest(void); 
void v_busTest(int, int);
int bmeExtTest(void);                        
void dataPathTest(void);
int bmeIDTest (void);

void SdramRWDma (int width, int speed, UINT32 StartData);
void testGpio (void);
void AztecRegReadWrite (void);
void testMmdio (UINT32, UINT32);
void testSmdio ();
void ConfigUart (int UartNo);
void InitSdram (int width, int speed); 
void LoopbackUart (int TxUart);
void ethDl (int clkRate,int iMode);

