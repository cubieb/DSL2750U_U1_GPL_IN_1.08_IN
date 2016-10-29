/**************************************************************
$Workfile:   bmehwapi.h  $
***************************************************************
***************************************************************
**
**	Filename    : bmehwapi.h
**	Description : c header file defining the BME hardware APIs
**	Author	    : jeya
**	Date	    : 23 Feb 2001
**	Notes	    :
**  References  : Host Interface Controller Block Definition Doc
					prepared by Weiluo
**	Revision History :
**
****************************************************************/
/***************************************************************
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
$Date: 2008/07/18 11:09:33 $
***************************************************************/

#ifndef __BMEHWAPI_H__
#define __BMEHWAPI_H__

/*****************************************************************************
                       I n c l u d e s
*****************************************************************************/
#include <ipos.h>
#include <dapi_parms.h>
#ifndef USE_VXWORKS /* IXDP425 */
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#endif
/*****************************************************************************
                      D e f i n e s
*****************************************************************************/
#ifndef BIT
#define BIT(n)                        (1<< n)
#define BIT0                          (BIT(0))
#define BIT1                          (BIT(1))
#define BIT2                          (BIT(2))
#define BIT3                          (BIT(3))
#define BIT4                          (BIT(4))
#define BIT5                          (BIT(5))
#define BIT6                          (BIT(6))
#define BIT7                          (BIT(7))
#define BIT8                          (BIT(8))
#define BIT9                          (BIT(9))
#define BIT10                         (BIT(10))
#define BIT11                         (BIT(11))
#define BIT12                         (BIT(12))
#define BIT13                         (BIT(13))
#define BIT14                         (BIT(14))
#define BIT15                         (BIT(15))
#define BIT16                         (BIT(16))
#define BIT17                         (BIT(17))
#define BIT18                         (BIT(18))
#define BIT19                         (BIT(19))
#define BIT20                         (BIT(20))
#define BIT21                         (BIT(21))
#define BIT22                         (BIT(22))
#define BIT23                         (BIT(23))
#define BIT24                         (BIT(24))
#define BIT25                         (BIT(25))
#define BIT26                         (BIT(26))
#define BIT27                         (BIT(27))
#define BIT28                         (BIT(28))
#define BIT29                         (BIT(29))
#define BIT30                         (BIT(30))
#define BIT31                         (BIT(31))
#endif /* ifndef BIT */

#ifdef _PARALLEL_IO_INTERFACE_
#undef _PARALLEL_IO_INTERFACE_  /* parallel i/o port interface */
#endif /* _PARALLEL_IO_INTERFACE_ */

#define BME_RESET_VEC_CODE_SIZE 16

#define BME_CODE_SIZE 	502324 /*453048*/  /*no serial*/

#define MAX_MSG_BUFFERS_IN_DPRAM	0x0a /* buffer a max of 10 msgs in DPRAM */

#ifdef CPE_30
/* LEGACY DEFINTIONS */
//#define iposDelay          bmeDelay                   /* LEGACY: fix api def */
//#define InitEMIConfig      bmeInitEMIConfig
//#define DoRdCmd            bmeRdMessage
#endif  //#ifdef CPE_30

/*****************************************************************************
                         Jaguar Definitions
*****************************************************************************/
#if defined(CPE_30) && !defined(CPE_50)
#define PIO_CMD_REG                     (0x00)
#define PIO_ADDR_REG                    (0x01)
#define PIO_DATA_REG                    (0x02)
#define PIO_SCRATCH_REG                 (0x07)
#define PIO_ACK_REG                     (0x08)
#define PIO_IRQ_REG                     (0x09)
#define PIO_STATUS_REG                  (0x0a)
#define PIO_XTENSA_REG                  (0x0b)

/* PIO commands */
#define PIO_CMD_WRITE                   (0x00)
#define PIO_CMD_READ                    (BIT4)

/* host ack reg */
#define PIO_ACK_CLR                     (BIT1)

/* interrupt mask for host irq */
#define PIO_IRQ_EN                      (BIT1)

/* msg rdy bit for xtensa */
#define PIO_XTENSA_RDY                  (BIT0)

#define HOST_RX_FLOW_ADDR               (0x40007f78)
#define HOST_TX_FLOW_ADDR               (0xE0070054)
#define HOST_TX_MSG_PEND                (BIT0)
#define HOST_RX_MSG_PEND                (BIT0)
#endif /* CPE_30 */


/*****************************************************************************
                         Aztec Definitions
*****************************************************************************/
/* 11 control ports - Aztec definitions */
#define COMMAND_PORT                    (0x00)
#define PIO_ADDR_PORT                   (0x01)
#define PIO_DATA_PORT                   (0x02)
#define DMA_REG_ADDR_PORT               (0x03)
#define DMA_LENGTH_PORT                 (0x04)
#define  DMA_SRC_ADDR_PORT              (0x05)
#define DMA_DEST_ADDR_PORT              (0x06)
#define DMA_DATA_PORT                   (0x07)
#define HOST_ACK_PORT                   (0x08)
#define DMA_CTRL_PORT                   (0x09)
#define DMA_STATUS_PORT                 (0x0a)

/* various commands supported by the command port */
#define PIO_WRITE_REG_CMD               (0x00)
#define PIO_READ_REG_CMD                (0x10)
#define DMA_WRITE_MEMORY_CMD            (0x20)
#define UPD_DMA_LEN_WR_CMD              (0x21)
#define UPD_DMA_DST_ADDR_WR_CMD         (0x22)
#define UPD_DMA_SRC_ADDR_WR_CMD         (0x24)
#define DMA_READ_MEMORY_CMD             (0x30)
#define UPD_DMA_LEN_RD_CMD              (0x31)
#define UPD_DMA_DST_ADDR_RD_CMD         (0x32)
#define UPD_DMA_SRC_ADDR_RD_CMD         (0x34)
#define UPD_DMA_CCR_REG_WR_CMD          (0x41) /* for DMA writes */
#define UPD_DMA_ECR_REG_WR_CMD          (0x42)
#define UPD_DMA_SAR_REG_WR_CMD          (0x44)
#define UPD_DMA_DAR_REG_WR_CMD          (0x48)
#define UPD_DMA_CCR_REG_RD_CMD          (0x81) /* for DMA reads */
#define UPD_DMA_ECR_REG_RD_CMD          (0x82)
#define UPD_DMA_SAR_REG_RD_CMD          (0x84)
#define UPD_DMA_DAR_REG_RD_CMD          (0x88)
#define UPD_HIC_FIFO_ADDR_CMD           (0xc0)


/* various regs supported by update destination & source registers address command */
#define DMA_CHANNEL_CTRL_REG_ADDR       (0x01)
#define DMA_CHANNEL_EVENT_CTRL_REG_ADDR (0x02)
#define DMA_CHANNEL_SRC_REG_ADDR        (0x04)
#define DMA_CHANNEL_DEST_REG_ADDR       (0x08)

/* various acknowledges supported by host ack port */
#define CLR_DW_H_DMA_WRITE_DONE          (0x01) /* reset DW_H bit */
#define CLR_MSG_RDY_IN_BME_DPRAM         (0x02) /* clear MR_H bit */

/* control bit definitions in DMA control port */
#define DMA_WRITE_DONE_INTR             (0x01) /* EN[0] bit */
#define MSG_RDY_IN_BME_DPRAM_INTR       (0x02) /* EN[1] bit */
#define UPD_DMA_WRITE_DONE_INTR         (0x10) /* MASK_EN[0] bit */
#define UPD_MSG_RDY_IN_BME_DPRAM_INTR   (0x20) /* MASK_EN[1] bit */
#define ALL_INTERRUPTS                  (0x77) /* MASK_EN[0] & MASK_EN[1] bits */

/* status bit definitions in DMA status port */
#define DW_H_DMA_WRITE_DONE             (0x01) /* DW_H bit */
#define MSG_RDY_IN_BME_DPRAM            (0x02) /* MR_H bit */
#define DW_T_DMA_WRITE_DONE             (0x04) /* HD_H bit */

/* memory activity */
#define CHECK_FOR_BME_WRITE_MEMORY      (0x01) /* DW_H bit to be zero */
#define CHECK_FOR_BME_TRANSFER_MEMORY   (0x01) /* DW_H bit to be zero */
#define CHECK_FOR_BME_READ_MEMORY       (0x01) /* DW_H bit to be one */
#define CHECK_FOR BME_READ_MSG          (0x01) /* DW_H bit to be one */
#define CHECK_FOR_BME_WRITE_MSG         (0x03) /* DW_H & DW_T bits to be zero */
#define CHECK_FOR_BME_TRANSFER_MSG      (0x05) /* DW_H bit to be zero & MR_H bit to be one */

/*****************************************************************************
                       Common (BME) Definitions
*****************************************************************************/
/* PIO Configuration regs */
/* PIO Interface */
#define PIO_MCR(base)	((base) + 0xe0070000) /* Module Control Register */
#define CKM_CCR(base)	((base) + 0xe0070004) /* Clock Module Control Register */
#define CKM_PCR(base)	((base) + 0xe0070008) /* PLL Control Register */
#define PIO_ISR(base)	((base) + 0xe007000c) /* Interrupt Source Register */
#define PIO_RSR(base)	((base) + 0xe0070010) /* Reset Status Register */
#define PIO_BID(base)	((base) + 0xe0070014) /* BME ID Register */
#define DAC_MCR(base)	((base) + 0xe0070018) /* DAC Module Control Register */

/* DMA Controller */
#define DMA_CCR(base, channelNum) ((base) + 0xe0030000 + (channelNum * 4)) /* Channel Control Register */
#define DMA_ECR(base, channelNum) ((base) + 0xe0030040 + (channelNum * 4)) /* Event Control Register */
#define DMA_SAR(base, channelNum) ((base) + 0xe0030080 + (channelNum * 4)) /* Source Address Register */
#define DMA_DAR(base, channelNum) ((base) + 0xe00300c0 + (channelNum * 4)) /* Destination Address Register */

/* DMA Channel Number */
#define DMA_CH_0						0x00
#define DMA_CH_1						0x01
#define DMA_CH_2						0x02
#define DMA_CH_3						0x03
#define DMA_CH_4						0x04
#define DMA_CH_5						0x05
#define DMA_CH_6						0x06
#define DMA_CH_7						0x07
#define DMA_CH_8						0x08
#define DMA_CH_9						0x09
#define DMA_CH_10						0x0a
#define DMA_CH_11						0x0b
#define DMA_CH_12						0x0c
#define DMA_CH_13						0x0d
#define DMA_CH_14						0x0e

#if defined (BME_CPE_20)  || defined(CO_20)
#define DMA_CH_15						0x07		/* AZTEC: use 7 for HIC DMA transfer */
#else
#define DMA_CH_15						0x0f
#endif

#ifdef   CPE_30                       /* JAGUAR: do not use dma ch 15 */
#undef DMA_CH_15
#endif

#ifdef	BME_CPE_20
/* BME-CPE 2.0 GPIO register address  */
#define GPIO_WDR(base)      ((base) + 0xE00A6000) /* GPIO write data reg      */
#define GPIO_PDR(base)      ((base) + 0xE00A6004) /* GPIO pin direction reg.  */
#define GPIO_RDR(base)      ((base) + 0xE00A6008) /* GPIO read data reg       */

/* GPIO register bit definitions */
#define GPIO_BIT0           0x01
#define GPIO_BIT1           0x02
#define GPIO_BIT2           0x04
#define GPIO_BIT3           0x08
#define GPIO_BIT4           0x10
#define GPIO_BIT5           0x20
#define GPIO_BIT6           0x40
#define GPIO_BIT7           0x80

#endif //BME_CPE_20

/* EMI Interface */
#define EMI_MCR(base)	((base) + 0xe0038000) /* Module Control Register */
#define EMI_RPR(base)	((base) + 0xe0038004) /* Module Control Register */
#define EMI_RCR(base)	((base) + 0xe0038008) /* Module Control Register */
#define EMI_PCR(base)	((base) + 0xe003800c) /* Module Control Register */
#define EMI_SCR(base)	((base) + 0xe0038010) /* Module Control Register */
#define EMI_BIR(base)	((base) + 0xe0038020) /* Module Control Register */

/* EMI configuration values */
#define EMI_MEM_SRAM						0x00000000 /* SRAM */
#define EMI_MEM_SDRAM						0x00000001 /* SDRAM */
#define EMI_SRAM_1MB						0x00000000 /* SRAM - 1 MB size */
#define EMI_SRAM_2MB						0x00000002 /* SRAM - 2 MB size */
#define EMI_SDRAM_16MB						0x00000000 /* SDRAM - 16 MB size */
#define EMI_SDRAM_22MB						0x00000002 /* SDRAM - 22 MB size */
#define EMI_SDRAM_8MB						0x00000004 /* SDRAM - 8 MB size */
#define EMI_CAS_LAT_3_CLOCKS				0x00000000 /* 3 clocks */
#define EMI_CAS_LAT_2_CLOCKS				0x00000008 /* 2 clocks */
#define EMI_RC_DEL_2_CLOCKS					0x00000000 /* 2 clocks */
#define EMI_RC_DEL_3_CLOCKS					0x00000010 /* 3 clocks */
#define EMI_ROW_PRE_2_CLOCKS				0x00000000 /* 2 clocks */
#define EMI_ROW_PRE_3_CLOCKS				0x00000020 /* 3 clocks */
#define EMI_ROW_ACT_6_CLOCKS				0x00000000 /* 6 clocks */
#define EMI_ROW_ACT_7_CLOCKS				0x00000040 /* 7 clocks */
#define EMI_ROW_CYC_11_CLOCKS				0x00000000 /* 11 clocks */
#define EMI_ROW_CYC_10_CLOCKS				0x00000080 /* 10 clocks */

/* API return types */
#define BME_API_SUCCESS                (0 )
#define BME_API_INVALID_LENGTH         (-1)
#define BME_API_FAILED                 (-2)
#define BME_API_OVERFLOW               (-3)

/* constants */
#define MAX_ADDR_LINES					0x04
#define MAX_DATA_LENGTH					0xff
#define MAX_WAIT_COUNT					0x10
#define MAX_DELAY_COUNT					0x32
#define BME_RESET_VEC_CODE_DNLD_ADDR	0x40000000
#define BME_CODE_DNLD_ADDR				0x40010000


#define DPRAM_BASE_ADDR		1
#define DPRAM_TX_BASE_ADDR	0x40008100
#define DPRAM_RX_BASE_ADDR	0x40008000

#define DWT_CLR_TIMEOUT 100

#ifndef _PARALLEL_IO_INTERFACE_1

#ifndef ALASKA
#if defined(CPE_30) && !defined(CPE_50)
static int inline InitMemoryMapPointer()
{
/* this function is called from appstart.c for device opening 
and other stuff. Include appropriate GPIO 
definitions as per requirement IXDP425*/
}

static void inline writeControlPort(U8 *base,U32 offset,U8 data)
{
#if defined(USE_VXWORKS)
  *(volatile U8 *)(base + offset) = data;
#else
/* IXDP425 related writeControlPort should be here */
#endif
#ifdef CPE_40_EMUL
	{
		volatile int i;
		for (i = 0;i < 0x20;i++);

	}
//  iposDelay();
#endif
}
static void inline readControlPort(U8 *base,U32 offset,U8 *data)
{
#if defined(USE_VXWORKS)
  *(U8 *)data = base[offset];
#else
/* IXDP425 related readControlPort should be here */
#endif
#ifdef CPE_40_EMUL
  {
	  volatile int i;
	  for (i = 0;i < 0x20;i++);

  }
//  iposDelay();
#endif
}
#elif defined (CO_50_EMULATION_BOARD)
 #define writeControlPort(x,y,z)			(*(x+y) = z);{volatile int i; for (i = 0;i < 0x20;i++);}
 #define readControlPort(x,y,z)			z = (*(x+y));{volatile int i; for (i = 0;i < 0x20;i++);}
#else
  /* writeControlPort macro definition */
  /* writeControlPort macro definition */
  #define writeControlPort(x,y,z)			(*(x+y) = z)
  #define readControlPort(x,y,z)			z = (*(x+y))
#endif
#endif
#endif /* _PARALLEL_IO_INTERFACE_ */




/*****************************************************************************
                          T y p e s
*****************************************************************************/
/* various status of BME FIFO */
typedef enum eFifoStatus
	{
		FIFO_EMPTY = 0x0,
		FIFO_WRITE_IN_PROGRESS,
		FIFO_WRITE,
		FIFO_READ_IN_PROGRESS,
		FIFO_READ
	} eFifoStatus;

typedef enum HWAPI_RET_VAL
{
	HWAPI_WRITE_SUCCESS = 1,
	HWAPI_TRANSFER_SUCCESS,
	HWAPI_READ_SUCCESS,
	HWAPI_PIO_READ_SUCCESS,
	HWAPI_PIO_WRITE_SUCCESS,
	HWAPI_DNLD_SUCCESS,
	HWAPI_DNLD_FAILURE,
	HWAPI_DNLD_TIMEOUT,
	HWAPI_FAILED,
	HWAPI_FAILURE_FIFO_NOT_AVAILABLE,
	HWAPI_FAILURE_DATA_NOT_PRESENT,
	HWAPI_INVALID_LENGTH,
	HWAPI_CODE_CHKSUM_TIMEOUT,
	HWAPI_CODE_CHKSUM_SUCCESS,
   HWAPI_CODE_CHKSUM_FAILED,
   HWAPI_MALLOC_FAILURE,
} HWAPI_RET_VAL;

/*****************************************************************************
                          D a t a
*****************************************************************************/
/*****************************************************************************
                P r i v a t e  F u n c t i o n s
*****************************************************************************/
/*****************************************************************************
                  P u b l i c  F u n c t i o n s
*****************************************************************************/
extern void InitDmaChannel15 (tBmeStruct *bmeStruct);
extern HWAPI_RET_VAL bmePioRead (tBmeStruct *bmeStruct, UINT32 srcAddr, BYTE *dataPtr);
extern HWAPI_RET_VAL bmePioWrite (tBmeStruct *bmeStruct, UINT32 destAddr, BYTE *dataPtr);

#if !defined(CPE_30) || defined(CPE_50)
extern void bmeIntEnable(tBmeStruct *bmeStruct, BYTE intrMask);
extern void bmeIntDisable(tBmeStruct *bmeStruct, BYTE intrMask);
extern BYTE bmeRdStatus(tBmeStruct *bmeStruct);
extern void bmeClrStatus(tBmeStruct *bmeStruct, BYTE statusMask);
extern HWAPI_RET_VAL bmeWrMemory(tBmeStruct *bmeStruct, UINT32 destAddr, BYTE *data, int numData);
extern HWAPI_RET_VAL bmeTrMemory(tBmeStruct *bmeStruct, UINT32 srcAddr, int numData);
extern HWAPI_RET_VAL bmeRdMemory(tBmeStruct *bmeStruct, BYTE *data);
extern HWAPI_RET_VAL bmeWrMessage(tBmeStruct *bmeStruct, UINT32 destAddr, BYTE *data, int numData);
extern HWAPI_RET_VAL bmeTrMessage(tBmeStruct *bmeStruct);
extern HWAPI_RET_VAL bmeRdMessage(tBmeStruct *bmeStruct, BYTE *message);
extern HWAPI_RET_VAL bmePioBulkRead (tBmeStruct *bmeStruct, UINT32 srcBaseAddr, BYTE *dataPtr, UINT32 numBytes);
extern HWAPI_RET_VAL bmePioBulkWrite (tBmeStruct *bmeStruct, UINT32 destBaseAddr, BYTE *dataPtr, UINT32 numBytes);
extern eBmeRv bmePollDmaStatus (tBmeStruct *bmeStruct);
extern HWAPI_RET_VAL bmeSoftwareDnld (tBmeStruct *bmeStruct);

extern HWAPI_RET_VAL DoInvokeRead (tBmeStruct *pBme);
extern HWAPI_RET_VAL DoTrCmd (tBmeStruct *pBme);
extern HWAPI_RET_VAL DoRdCmd (tBmeStruct *pBme);
extern HWAPI_RET_VAL DoTxCmd (tBmeStruct *pBme);

extern HWAPI_RET_VAL bmeDnldProcedure (tBmeStruct *bmeStruct);
#else


/*****************************************************************************
  Function:		bmeGetResetVecCode

  Description: Returns pointer to BME vector code

  Input:       void
  Output:      void
  Returns:     pointer

*****************************************************************************/
extern char *bmeGetResetVecCode(void);

/*****************************************************************************
  Function:		bmeSetClk

  Description: Set BME clock

  Input:       pBme - pointer to bme structure
               num  - value to program
  Output:      clock register written
  Returns:     void

  Notes:

*****************************************************************************/
extern void bmeSetClk(tBmeStruct * pBme, int num);

/*****************************************************************************
  Function:		bmeTrMemory

  Description: Dummy function for legacy API

  Input:
  Output:
  Returns:

  Notes:

*****************************************************************************/
extern HWAPI_RET_VAL bmeTrMemory(tBmeStruct * bmeStruct, UINT32 srcAddr, int numData);

/*****************************************************************************
  Function:		bmeClrStatus

  Description: Legacy support function to clear handshake

  Input:       pBme - pointer to BME
               status - which status to clear
  Output:      platform call to clear status called
  Returns:     void

  Notes:

*****************************************************************************/
extern void bmeClrStatus(tBmeStruct *pBme,U8 status);

/*****************************************************************************
  Function:		bmeRdStatus

  Description: Reads status of handshake

  Input:       pBme - pointer to BME
  Output:      platform calls used to obtain status
  Returns:     MSG_RDY_IN_BME_DPRAM - indicates target->host message waiting
               DW_H_DMA_WRITE_DONE - indicates host->target message complete

  Notes:

*****************************************************************************/
extern BYTE bmeRdStatus(tBmeStruct *pBme);

/*****************************************************************************
  Function:		doDmaRead_diag

  Description: Read function - hardwired to BME 1

  Input:       srcAddr - source address (target)
               codeLen - data length
               BmeCodeArrayPtr - dest addr (host)
               destFifoFlag - not used
  Output:      data copied to BmeCodeArrayPtr
  Returns:     void

  Notes:      Need to consolidate all this - doDmaRead is equivilent

*****************************************************************************/
extern void doDmaRead_diag(U32 srcAddr, U32 codeLen, U8 * BmeCodeArrayPtr, int destFifoFlag);

/*****************************************************************************
  Function:		hostReadBMEMemories

  Description: yet another read DMA function

  Input:       dst - host address of buffer
               src - bme address of source
               len - length of data
               read_mode, isFifo - not used
               bmeChNum - BME number
  Output:
  Returns:    OK (1) if success, ERROR otherwise

  Notes:      Need to consolidate all this - doDmaRead is equivilent

*****************************************************************************/
extern int hostReadBMEMemories(char *dst, char *src, int len, int read_mode, int isFifo, int bmeChNum);


#endif //#ifndef CPE_30

#endif __BMEHWPI_H__

/* END_OF_FILE */
