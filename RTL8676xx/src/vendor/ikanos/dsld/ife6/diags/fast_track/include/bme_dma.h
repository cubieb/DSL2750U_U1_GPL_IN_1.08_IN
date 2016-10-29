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

#ifndef _DMA_H_
#define _DMA_H_


#define NUM_PORTS								4
#define	NUM_DMA_CHANNELS						15

// Reserve some DMAs
#define DMA_FOR_INIT_RX							0
#define DMA_FOR_INIT_TX							1
#define DMA_FOR_INIT_MISC						2
#define DMA_FOR_SNR								3
// FEQ is 12k bytes. So uses 2 DMAs
#define DMA_FOR_FEQ1							4	
#define DMA_FOR_FEQ2							5	
#define DMA_FREE_LAST  							6
#define DMA_FREE_FIRST							14
#define DMA_FOR_HIC								15



#define DMA_CCR_TRANSFER_COUNT	  				(16)
#define	DMA_CCR_CHANNEL_ENABLE_BIT 				(BIT12)
#define	DMA_CCR_CHANNEL_INT_ENABLE_BIT 			(BIT11)
#define	DMA_CCR_CHANNEL_SRC_FIFO				(BIT10)
#define	DMA_CCR_CHANNEL_DST_FIFO				(BIT9)
#define	DMA_CCR_CHANNEL_TRANSFER_DONE      		(BIT8)
#define	DMA_CCR_CHANNEL_TRANSFER_IN_PROGRESS	(BIT6)
#define	DMA_CCR_CHANNEL_ENABLE_CHAINING	       	(BIT1)
#define	DMA_CCR_CHANNEL_DMA_BEGIN	       		(BIT0)
#define	DMA_ECR_CHANNEL			       			(BIT15)
#define	DMA_ECR_SW_INIT_DMA			    		(BIT31)
#define	DMA_ECR_ZFI_REQ							(BIT30)
#define	DMA_ECR_CKM_REQ							(BIT29)
#define	DMA_ECR_MAP_REQ							(BIT24)
#define DMA_ECR_DFR_REQ7						(BIT23)
#define DMA_ECR_DFR_REQ6						(BIT22)
#define DMA_ECR_DFR_REQ5						(BIT21)
#define DMA_ECR_DFR_REQ4						(BIT20)
#define DMA_ECR_DFR_REQ3						(BIT19)
#define DMA_ECR_DFR_REQ2						(BIT18)
#define DMA_ECR_DFR_REQ1						(BIT17)
#define DMA_ECR_DFR_REQ0						(BIT16)
#define SNR_DMA_REQ								DMA_ECR_DFR_REQ0
#define BFIFO_FFT_DMA_REQ						DMA_ECR_DFR_REQ1

#define NO_DMA_CHANNEL_AVAILABLE				-1

typedef struct  {
	int 		portnum;	 // VDSL channel number 
	int 		src;
	int 		dst;
	int 		init;
	int 		hwEvent;
	int 		dma_mode;
	int 		isFifoSrc;
	int 		isFifoDst;
	int 		length;
	int 		channel;	  // dma channel number (0 to 15), 
} DMA_DATABASE;

extern DMA_DATABASE dmaDatabase[16];

extern int setupDMA (DMA_DATABASE *p);
extern int DMAdone (int DMAchannel);
#endif

