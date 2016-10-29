#ifndef _BME_H
#define _BME_H
               
#ifdef VX_WORKS
#include "vxWorks.h"
#else
#define	INT8		char
#define INT16		short
#define INT32		int
#define UINT8		unsigned char
#define UINT16		unsigned short
#define UINT32		unsigned int
#define logMsg      printf
#endif
              
#ifndef TRUE 
#define TRUE			(1)
#endif

#ifndef FALSE
#define FALSE			(0)
#endif

#define BIT(n)			(1 << n)
#define BIT0			(BIT(0))
#define BIT1			(BIT(1))
#define BIT2			(BIT(2))
#define BIT3			(BIT(3))
#define BIT4			(BIT(4))
#define BIT5			(BIT(5))
#define BIT6			(BIT(6))
#define BIT7			(BIT(7))
#define BIT8			(BIT(8))
#define BIT9			(BIT(9))
#define BIT10			(BIT(10))
#define BIT11   		(BIT(11))
#define BIT12			(BIT(12))
#define BIT13   		(BIT(13))
#define BIT14   		(BIT(14))
#define BIT15			(BIT(15))
#define BIT16   		(BIT(16))
#define BIT17			(BIT(17))
#define BIT18   		(BIT(18))
#define BIT19			(BIT(19))
#define BIT20			(BIT(20))
#define BIT21   		(BIT(21))
#define BIT22			(BIT(22))
#define BIT23   		(BIT(23))
#define BIT24   		(BIT(24))
#define BIT25   		(BIT(25))
#define BIT26   		(BIT(26))
#define BIT27   		(BIT(27))
#define BIT28   		(BIT(28))
#define BIT29   		(BIT(29))
#define BIT30   		(BIT(30))
#define BIT31   		(BIT(31))

#define XFIFO_ACCESS_BY_CORE	 1
#define XFIFO_ACCESS_BY_ATM		 0

#define MASK_OUT_CACHE_ADDRESS_BIT			(~(0x20000000))

#define int8			INT8
#define int16			INT16
#define int32			INT32
#define uint8			UINT8
#define uint16			UINT16
#define uint32			UINT32
	
#define s8				INT8
#define u8				UINT8
#define s16				INT16
#define u16				UINT16
#define s32				INT32
#define u32				UINT32

typedef	long			Fast32;
typedef	int				Fast16;

typedef	long			Word32;
typedef	short			Word16;
typedef	char			Word8;
typedef	unsigned long	uWord32;
typedef	unsigned short	uWord16;
typedef	unsigned char	uWord8;
typedef	int				Flag;

typedef unsigned short  uint;	/* unsigned integer */
typedef unsigned long	ulong;	/* unsigned long integer */
typedef short           tint; 
typedef tint 			bool;  	/* logical variable */ 

#define	FIFO_AS_MEM		0x80
#define MAX_DMA_SIZE	(8*1024)
#define	MAX_BANDPLAN	2
#define B_OFFSET		4

typedef enum {INACTIVE = 0, ACTIVE} CHANNEL_STATES;
typedef enum {UP_STREAM = 0, DOWNSTREAM} LOCATION;
typedef enum {MXU_FLEX = 0, FLEX_998} BAND_PLANS;
typedef enum {PIO = 0, DMA_POLLING,  DMA_INT,  DMA_BUSY_POLLING} IO_TYPE; 
typedef enum {DMA_IN_PROGRESS = 0, DMA_DONE} DMA_STATUS;

extern DMA_STATUS	dma_status;
extern int			curr_dma_chan;
extern int			clk_val;
extern int			host_dma_delay;

typedef struct {
	int 	sReal;
	int 	sImag;
}tComplex_s32;

typedef struct {
	int16 sReal;
	int16 sImag;
}tComplex_s16;

typedef struct {
	s8 sReal;
	s8 sImag;
}tComplex_s8;

#endif
