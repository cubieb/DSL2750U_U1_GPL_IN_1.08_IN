#include <linux/kernel.h>
#include "codec_descriptor.h"
#include <linux/config.h>		/*for module disable dmem*/
#ifdef CONFIG_RTK_VOIP_AMR_NB
#include "dsp_r1/amr_nb/include/amr_nb.h"
#endif

/* ==================================================================
 * Codec classified by algorithm (711a, 711u, 726 16k/24k/32k/40k ...)
 * ================================================================== */
/*
 * Calculate number of frames in current packet
 */
extern uint32 G723GetNumberOfFramesInCurrentPacket( const unsigned char *pBuffer,
											 uint32 nSize, 
											 int TempLen,
											 int TempLenSID,
											 int *pbAppendixSID );
extern uint32 GetNumberOfFramesInCurrentPacket( const unsigned char *pBuffer,
										 uint32 nSize, 
										 int TempLen,
										 int TempLenSID,
										 int *pbAppendixSID );
extern uint32 SPEEX_NBGetNumberOfFramesInCurrentPacket( const unsigned char *pBuffer,
											 uint32 nSize,
											 int TempLen,
											 int TempLenSID,
											 int *pbAppendixSID );

/*
 * Get frame infomation (frame length, SID length, and frame timestamp)
 */
extern uint32 G711GetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 G729GetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 G72616GetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 G72624GetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 G72632GetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 G72640GetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 G722GetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 G723GetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 GSMfrGetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 iLBC30msGetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 iLBC20msGetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 AMRGetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 T38GetFrameInfo_FrameLength( const unsigned char *pBuffer );
extern uint32 SPEEX_NBGetFrameInfo_FrameLength( const unsigned char *pBuffer );
#define SilenceGetFrameInfo_FrameLength	G711GetFrameInfo_FrameLength

extern uint32 G711GetFrameInfo_SidLength( uint32 nSize );
extern uint32 G729GetFrameInfo_SidLength( uint32 nSize );
extern uint32 G72616GetFrameInfo_SidLength( uint32 nSize );
extern uint32 G72624GetFrameInfo_SidLength( uint32 nSize );
extern uint32 G72632GetFrameInfo_SidLength( uint32 nSize );
extern uint32 G72640GetFrameInfo_SidLength( uint32 nSize );
extern uint32 G722GetFrameInfo_SidLength( uint32 nSize );
extern uint32 G723GetFrameInfo_SidLength( uint32 nSize );
extern uint32 GSMfrGetFrameInfo_SidLength( uint32 nSize );
extern uint32 iLBCGetFrameInfo_SidLength( uint32 nSize );
extern uint32 AMRGetFrameInfo_SidLength( uint32 nSize );
#define iLBC30msGetFrameInfo_SidLength iLBCGetFrameInfo_SidLength
#define iLBC20msGetFrameInfo_SidLength iLBCGetFrameInfo_SidLength
extern uint32 T38GetFrameInfo_SidLength( uint32 nSize );
extern uint32 SPEEX_NBGetFrameInfo_SidLength( uint32 nSize );
#define SilenceGetFrameInfo_SidLength	G711GetFrameInfo_SidLength

extern void G726CmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void G711CmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void G722CmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void G723CmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void G729CmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void GSMfrCmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void iLBC30msCmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void iLBC20msCmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void AMR_NB_CmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void T38CmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void SPEEX_NBCmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );
extern void SilenceCmdParserInitialize( uint32 sid, const CDspcodecInitializeParm *pInitializeParam );

#define M_MOS( x )		( x * ( 1 << 7 ) )

const codec_algo_desc_t g_codecAlgoDesc[] = { 
	{	/* G711 u-law */
		DSPCODEC_ALGORITHM_G711U,			/* DSPCODEC_ALGORITHM */
		G711GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		G711GetFrameInfo_SidLength,			/* R0: FN: get SID length */
		80,									/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		G711CmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 4.3 ),						/* R1: N: mos */
	}, 
	{	/* G711 a-law */
		DSPCODEC_ALGORITHM_G711A,			/* DSPCODEC_ALGORITHM */
		G711GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		G711GetFrameInfo_SidLength,			/* R0: FN: get SID length */
		80,									/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		G711CmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 4.3 ),						/* R1: N: mos */
	},
#ifdef CONFIG_RTK_VOIP_G7231
	{	/* G7231A53 */
    	DSPCODEC_ALGORITHM_G7231A53,		/* DSPCODEC_ALGORITHM */
		G723GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		G723GetFrameInfo_SidLength,			/* R0: FN: get SID length */
		240,								/* R0: N: frame timestamp */
    	G723GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
    	G723CmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.6 ),						/* R1: N: mos */
    },
    {	/* G7231A63 */
		DSPCODEC_ALGORITHM_G7231A63,		/* DSPCODEC_ALGORITHM */
		G723GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		G723GetFrameInfo_SidLength,			/* R0: FN: get SID length */
		240,								/* R0: N: frame timestamp */
		G723GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		G723CmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.8 ),						/* R1: N: mos */
    },
#endif /* CONFIG_RTK_VOIP_G7231 */
#ifdef CONFIG_RTK_VOIP_G729AB
    {	/* G729 */    
		DSPCODEC_ALGORITHM_G729,			/* DSPCODEC_ALGORITHM */
		G729GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		G729GetFrameInfo_SidLength,			/* R0: FN: get SID length */
		80,									/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		G729CmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.9 ),						/* R1: N: mos */
    },
#endif /* CONFIG_RTK_VOIP_G729AB */
#ifdef CONFIG_RTK_VOIP_G726
    {	/* G72616 */
		DSPCODEC_ALGORITHM_G72616,			/* DSPCODEC_ALGORITHM */
		G72616GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		G72616GetFrameInfo_SidLength,		/* R0: FN: get SID length */
		80,									/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		G726CmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.9 ),						/* R1: N: mos */
	},
	{	/* G72624 */
    	DSPCODEC_ALGORITHM_G72624,			/* DSPCODEC_ALGORITHM */
		G72624GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		G72624GetFrameInfo_SidLength,		/* R0: FN: get SID length */
		80,									/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		G726CmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 4.0 ),						/* R1: N: mos */
    },
    {	/* G72632 */
		DSPCODEC_ALGORITHM_G72632,			/* DSPCODEC_ALGORITHM */
		G72632GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		G72632GetFrameInfo_SidLength,		/* R0: FN: get SID length */
		80,									/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		G726CmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 4.1 ),						/* R1: N: mos */
	},
	{	/* G72640 */
		DSPCODEC_ALGORITHM_G72640,			/* DSPCODEC_ALGORITHM */
		G72640GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		G72640GetFrameInfo_SidLength,		/* R0: FN: get SID length */
		80,									/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		G726CmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 4.14 ),						/* R1: N: mos */
	},
#endif /* CONFIG_RTK_VOIP_G726 */
#ifdef CONFIG_RTK_VOIP_GSMFR
	{	/* GSM FR */
		DSPCODEC_ALGORITHM_GSMFR,			/* DSPCODEC_ALGORITHM */
		GSMfrGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		GSMfrGetFrameInfo_SidLength,		/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		GSMfrCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.7 ),						/* R1: N: mos */
	},
#endif /* CONFIG_RTK_VOIP_GSMFR */
#ifdef CONFIG_RTK_VOIP_ILBC
	{	/* iLBC 30ms */
		DSPCODEC_ALGORITHM_ILBC30MS,			/* DSPCODEC_ALGORITHM */
		iLBC30msGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		iLBC30msGetFrameInfo_SidLength,		/* R0: FN: get SID length */
		240,								/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		iLBC30msCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.9 ),						/* R1: N: mos */
	},
	{	/* iLBC 20ms */
		DSPCODEC_ALGORITHM_ILBC20MS,			/* DSPCODEC_ALGORITHM */
		iLBC20msGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */				/* R0: N: FrameLengthinbit*/
		iLBC20msGetFrameInfo_SidLength,		/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		iLBC20msCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.9 ),						/* R1: N: mos */
	},
#endif /* CONFIG_RTK_VOIP_ILBC */
#ifdef CONFIG_RTK_VOIP_G722
    {	/* G722 48k */
    	DSPCODEC_ALGORITHM_G72248,		/* DSPCODEC_ALGORITHM */
	G722GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
	0, /* volatile */			/* R0: N: volatile frame length */
	0, /* not bit length */			/* R0: N: FrameLengthinbit*/
	G722GetFrameInfo_SidLength,		/* R0: FN: get SID length */
	80,					/* R0: N: frame timestamp */
    	GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
    	G722CmdParserInitialize,		/* R1: FN: CmdParser Initialize */
		M_MOS( 4.3 ),						/* R1: N: mos */
    },
    {	/* G722 56k */
    	DSPCODEC_ALGORITHM_G72256,		/* DSPCODEC_ALGORITHM */
	G722GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
	0, /* volatile */			/* R0: N: volatile frame length */
	0, /* not bit length */			/* R0: N: FrameLengthinbit*/
	G722GetFrameInfo_SidLength,		/* R0: FN: get SID length */
	80,					/* R0: N: frame timestamp */
    	GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
    	G722CmdParserInitialize,		/* R1: FN: CmdParser Initialize */
		M_MOS( 4.3 ),						/* R1: N: mos */
    },
    {	/* G722 64k */
    	DSPCODEC_ALGORITHM_G72264,		/* DSPCODEC_ALGORITHM */
	G722GetFrameInfo_FrameLength,		/* R0: FN: get frame length */
	0, /* volatile */			/* R0: N: volatile frame length */
	0, /* not bit length */			/* R0: N: FrameLengthinbit*/
	G722GetFrameInfo_SidLength,		/* R0: FN: get SID length */
	80,					/* R0: N: frame timestamp */
    	GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
    	G722CmdParserInitialize,		/* R1: FN: CmdParser Initialize */
		M_MOS( 4.3 ),						/* R1: N: mos */
    },
#endif /* CONFIG_RTK_VOIP_G722 */
#ifdef CONFIG_RTK_VOIP_AMR_NB
    {	/* AMR NB */
    	DSPCODEC_ALGORITHM_AMR_NB,		/* DSPCODEC_ALGORITHM */
	AMRGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
	0, /* volatile */			/* R0: N: volatile frame length */
	0, /* not bit length */			/* R0: N: FrameLengthinbit*/
	AMRGetFrameInfo_SidLength,		/* R0: FN: get SID length */
	160,					/* R0: N: frame timestamp */
    	GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
    	AMR_NB_CmdParserInitialize,		/* R1: FN: CmdParser Initialize */
		M_MOS( 3.87 ),	/* 7.95k */			/* R1: N: mos */
    },
#endif /* CONFIG_RTK_VOIP_AMR_NB */
#ifdef CONFIG_RTK_VOIP_T38
	{	/* T.38 */
		DSPCODEC_ALGORITHM_T38,				/* DSPCODEC_ALGORITHM */
		T38GetFrameInfo_FrameLength,		/*(unused)*/	/* R0: FN: get frame length */
		0, /* not volatile */				/*(unused)*/	/* R0: N: volatile frame length */
		0, /* not bit length */			/* R0: N: FrameLengthinbit*/
		T38GetFrameInfo_SidLength,			/*(unused)*/	/* R0: FN: get SID length */
		80, /* no timestamp info. */		/*(unused)*/	/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/*(unused)*/	/* R0: FN: calculate frames # */
		T38CmdParserInitialize,				/* R1: FN: CmdParser Initialize */
		M_MOS( 1.0 ),						/* R1: N: mos */
	},
#endif /* CONFIG_RTK_VOIP_T38 */
#ifdef CONFIG_RTK_VOIP_SPEEX_NB
	{	/* SPEEX_NB_2P15 */
		DSPCODEC_ALGORITHM_SPEEX_NB_2P15,		/* DSPCODEC_ALGORITHM */
		SPEEX_NBGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		1, /* bit length packet legth not equal byte, may nbyte + 1~7bit */	/* R0: Y: FrameLengthinbit*/
		SPEEX_NBGetFrameInfo_SidLength,			/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		SPEEX_NBGetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		SPEEX_NBCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.84 ),							/* R1: N: mos */
	},
	{	/* SPEEX_NB_5P95 */
		DSPCODEC_ALGORITHM_SPEEX_NB_5P95,		/* DSPCODEC_ALGORITHM */
		SPEEX_NBGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		1, /* bit length packet legth not equal byte, may nbyte + 1~7bit */	/* R0: Y: FrameLengthinbit*/
		SPEEX_NBGetFrameInfo_SidLength,			/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		SPEEX_NBGetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		SPEEX_NBCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.84 ),							/* R1: N: mos */
	},
	{	/* SPEEX_NB_8 */
		DSPCODEC_ALGORITHM_SPEEX_NB_8,			/* DSPCODEC_ALGORITHM */
		SPEEX_NBGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		1, /* bit length packet legth not equal byte, may nbyte + 1~7bit */	/* R0: Y: FrameLengthinbit*/
		SPEEX_NBGetFrameInfo_SidLength,			/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		SPEEX_NBGetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		SPEEX_NBCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.84 ),							/* R1: N: mos */
	},
	{	/* SPEEX_NB_11 */
		DSPCODEC_ALGORITHM_SPEEX_NB_11,			/* DSPCODEC_ALGORITHM */
		SPEEX_NBGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		1, /* bit length packet legth not equal byte, may nbyte + 1~7bit */	/* R0: Y: FrameLengthinbit*/
		SPEEX_NBGetFrameInfo_SidLength,			/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		SPEEX_NBGetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		SPEEX_NBCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.84 ),							/* R1: N: mos */
	},
	{	/* SPEEX_NB_15 */
		DSPCODEC_ALGORITHM_SPEEX_NB_15,			/* DSPCODEC_ALGORITHM */
		SPEEX_NBGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		1, /* bit length packet legth not equal byte, may nbyte + 1~7bit */	/* R0: Y: FrameLengthinbit*/
		SPEEX_NBGetFrameInfo_SidLength,			/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		SPEEX_NBGetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		SPEEX_NBCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.84 ),							/* R1: N: mos */
	},
	{	/* SPEEX_NB_18P2 */
		DSPCODEC_ALGORITHM_SPEEX_NB_18P2,		/* DSPCODEC_ALGORITHM */
		SPEEX_NBGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		1, /* bit length packet legth not equal byte, may nbyte + 1~7bit */	/* R0: Y: FrameLengthinbit*/
		SPEEX_NBGetFrameInfo_SidLength,			/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		SPEEX_NBGetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		SPEEX_NBCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.84 ),							/* R1: N: mos */
	},
	{	/* SPEEX_NB_24P6 */
		DSPCODEC_ALGORITHM_SPEEX_NB_24P6,		/* DSPCODEC_ALGORITHM */
		SPEEX_NBGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		1, /* bit length packet legth not equal byte, may nbyte + 1~7bit */	/* R0: Y: FrameLengthinbit*/
		SPEEX_NBGetFrameInfo_SidLength,			/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		SPEEX_NBGetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		SPEEX_NBCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.84 ),							/* R1: N: mos */
	},
	{	/* SPEEX_NB_3P95 */
		DSPCODEC_ALGORITHM_SPEEX_NB_3P95,		/* DSPCODEC_ALGORITHM */
		SPEEX_NBGetFrameInfo_FrameLength,		/* R0: FN: get frame length */
		1, /* volatile */					/* R0: N: volatile frame length */
		1, /* bit length packet legth not equal byte, may nbyte + 1~7bit */	/* R0: Y: FrameLengthinbit*/
		SPEEX_NBGetFrameInfo_SidLength,			/* R0: FN: get SID length */
		160,								/* R0: N: frame timestamp */
		SPEEX_NBGetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		SPEEX_NBCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 3.84 ),							/* R1: N: mos */
	},
#endif /* CONFIG_RTK_VOIP_SPEEX_NB */
#ifdef CONFIG_RTK_VOIP_SILENCE
	{	/* silence codec */
		DSPCODEC_ALGORITHM_SILENCE,			/* DSPCODEC_ALGORITHM */
		SilenceGetFrameInfo_FrameLength,	/* R0: FN: get frame length */
		0, /* not volatile */				/* R0: N: volatile frame length */
		0, /* not bit length */			/* R0: N: FrameLengthinbit*/
		SilenceGetFrameInfo_SidLength,		/* R0: FN: get SID length */
		80,									/* R0: N: frame timestamp */
		GetNumberOfFramesInCurrentPacket,	/* R0: FN: calculate frames # */
		SilenceCmdParserInitialize,			/* R1: FN: CmdParser Initialize */
		M_MOS( 1.0 ),						/* R1: N: mos */
	}, 
#endif /* CONFIG_RTK_VOIP_SILENCE */
};


CT_ASSERT( ( sizeof( g_codecAlgoDesc ) / sizeof( g_codecAlgoDesc[ 0 ] ) ) == NUM_OF_ALGO_CODEC_DESC );
CT_ASSERT( DSPCODEC_ALGORITHM_NUMBER == NUM_OF_ALGO_CODEC_DESC );

/* ==================================================================
 * Codec classified by type (711, 726 ...)
 * ================================================================== */
extern enum CODEC_STATE g711_state[MAX_SESS_NUM];
extern enum CODEC_STATE g722_state[MAX_SESS_NUM];
extern enum CODEC_STATE g7231_state[MAX_SESS_NUM];
extern enum CODEC_STATE g729_state[MAX_SESS_NUM];
extern enum CODEC_STATE g726_state[MAX_SESS_NUM];
extern enum CODEC_STATE gsmfr_state[MAX_SESS_NUM];
extern enum CODEC_STATE iLBC20ms_state[MAX_SESS_NUM];
extern enum CODEC_STATE iLBC30ms_state[MAX_SESS_NUM];
extern enum CODEC_STATE amr_nb_state[MAX_SESS_NUM];
extern enum CODEC_STATE t38_state[MAX_SESS_NUM];
extern enum CODEC_STATE speex_nb_state[MAX_SESS_NUM];
extern enum CODEC_STATE silence_state[MAX_SESS_NUM];

extern void G711DecPhaseProcess( uint32 sid );
extern void G722DecPhaseProcess( uint32 sid );
extern void G726DecPhaseProcess( uint32 sid );
extern void G729DecPhaseProcess( uint32 sid );
extern void G723DecPhaseProcess( uint32 sid );
extern void GSMfrDecPhaseProcess( uint32 sid );
extern void iLBC30msDecPhaseProcess( uint32 sid );
extern void iLBC20msDecPhaseProcess( uint32 sid );
extern void AMR_NB_DecPhaseProcess( uint32 sid );
extern void T38DecPhaseProcess( uint32 sid );
extern void SPEEX_NB_DecPhaseProcess( uint32 sid );
extern void SilenceDecPhaseProcess( uint32 sid );

extern void G711EncPhaseProcess( uint32 sid );
extern void G722EncPhaseProcess( uint32 sid );
extern void G726EncPhaseProcess( uint32 sid );
extern void G729EncPhaseProcess( uint32 sid );
extern void G723EncPhaseProcess( uint32 sid );
extern void GSMfrEncPhaseProcess( uint32 sid );
extern void iLBC30msEncPhaseProcess( uint32 sid );
extern void iLBC20msEncPhaseProcess( uint32 sid );
extern void AMR_NB_EncPhaseProcess( uint32 sid );
extern void T38EncPhaseProcess( uint32 sid );
extern void SPEEX_NB_EncPhaseProcess( uint32 sid );
extern void SilenceEncPhaseProcess( uint32 sid );

extern void G711AddDecPlaytoneBuffer( uint32 sid, int *pFrameLen );
#define G726AddDecPlaytoneBuffer	G711AddDecPlaytoneBuffer	/* G726 and G711 are identical */
extern void G722AddDecPlaytoneBuffer( uint32 sid, int *pFrameLen );
extern void G729AddDecPlaytoneBuffer( uint32 sid, int *pFrameLen );
extern void G723AddDecPlaytoneBuffer( uint32 sid, int *pFrameLen );
extern void GSMfrAddDecPlaytoneBuffer( uint32 sid, int *pFrameLen );
extern void iLBC30msAddDecPlaytoneBuffer( uint32 sid, int *pFrameLen );
extern void iLBC20msAddDecPlaytoneBuffer( uint32 sid, int *pFrameLen );
extern void AMR_NB_AddDecPlaytoneBuffer( uint32 sid, int *pFrameLen );
#define T38AddDecPlaytoneBuffer	G711AddDecPlaytoneBuffer	/* T.38 and G711 are identical */
extern void SPEEX_NB_AddDecPlaytoneBuffer( uint32 sid, int *pFrameLen );
#define SilenceAddDecPlaytoneBuffer	G711AddDecPlaytoneBuffer

extern void DummyHighPassFiltering( uint32 sid );
#define G711HighPassFiltering	DummyHighPassFiltering
#define G722HighPassFiltering	DummyHighPassFiltering
#define G723HighPassFiltering	DummyHighPassFiltering
#define G726HighPassFiltering	DummyHighPassFiltering
extern void G729HighPassFiltering( uint32 sid );
#define GSMfrHighPassFiltering	DummyHighPassFiltering
#define iLBCHighPassFiltering	DummyHighPassFiltering
#define T38HighPassFiltering	DummyHighPassFiltering
#define AMR_NB_HighPassFiltering	DummyHighPassFiltering
#define SPEEX_NB_HighPassFiltering	DummyHighPassFiltering
#define SilenceHighPassFiltering	DummyHighPassFiltering

extern void G726CmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void G711CmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void G722CmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void G723CmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void G729CmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void GSMfrCmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void iLBC30msCmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void iLBC20msCmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void AMR_NB_CmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void T38CmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void SPEEX_NB_CmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );
extern void SilenceCmdParserStart( uint32 sid, const CDspcodecStartParm *pStartParam );

extern void G711CmdParserStop( uint32 sid );
extern void G722CmdParserStop( uint32 sid );
#define G726CmdParserStop	G711CmdParserStop	/* G726 and G711 are identical */
extern void G723CmdParserStop( uint32 sid );
extern void G729CmdParserStop( uint32 sid );
extern void GSMfrCmdParserStop( uint32 sid );
extern void iLBC30msCmdParserStop( uint32 sid );
extern void iLBC20msCmdParserStop( uint32 sid );
extern void AMR_NB_CmdParserStop( uint32 sid );
#define T38CmdParserStop	G711CmdParserStop	/* T.38 and G711 are identical */
extern void SPEEX_NB_CmdParserStop( uint32 sid );
#define SilenceCmdParserStop	G711CmdParserStop

extern int G711IsJbcSidFrame( uint32 sid, uint32 nSize );
extern int G722IsJbcSidFrame( uint32 sid, uint32 nSize );
extern int G723IsJbcSidFrame( uint32 sid, uint32 nSize );
extern int G729IsJbcSidFrame( uint32 sid, uint32 nSize );
extern int G726IsJbcSidFrame( uint32 sid, uint32 nSize );
extern int GSMfrIsJbcSidFrame( uint32 sid, uint32 nSize );
extern int iLBC30msIsJbcSidFrame( uint32 sid, uint32 nSize );
extern int iLBC20msIsJbcSidFrame( uint32 sid, uint32 nSize );
extern int AMR_NB_IsJbcSidFrame( uint32 sid, uint32 nSize );
extern int T38IsJbcSidFrame( uint32 sid, uint32 nSize );
extern int SPEEX_NB_IsJbcSidFrame( uint32 sid, uint32 nSize );
#define SilenceIsJbcSidFrame	G711IsJbcSidFrame

#ifndef CONFIG_RTK_VOIP_MODULE 
extern void G711_set_codec_mem(int state, int g726_rate);
extern void G722_set_codec_mem(int state, int g726_rate);
extern void G723_set_codec_mem(int state, int g726_rate);
extern void G729_set_codec_mem(int state, int g726_rate);
extern void G726_set_codec_mem(int state, int g726_rate);
extern void GSMfr_set_codec_mem(int state, int g726_rate);
extern void iLBC_set_codec_mem(int state, int g726_rate);
extern void AMR_NB_set_codec_mem(int state, int g726_rate);
extern void T38_set_codec_mem(int state, int g726_rate);
extern void SPEEX_NB_set_codec_mem(int state, int g726_rate);
#define Silence_set_codec_mem	G711_set_codec_mem
#endif /* CONFIG_RTK_VOIP_MODULE */

extern void G711SetLecG168SyncPoint( unsigned int chid );
extern void G722SetLecG168SyncPoint( unsigned int chid );
extern void G723SetLecG168SyncPoint( unsigned int chid );
extern void G729SetLecG168SyncPoint( unsigned int chid );
extern void G726SetLecG168SyncPoint( unsigned int chid );
extern void GSMfrSetLecG168SyncPoint( unsigned int chid );
extern void T38SetLecG168SyncPoint( unsigned int chid );

const codec_type_desc_t g_codecTypeDesc[] = {
	{
		CODEC_TYPE_G711, 				/* START_CODEC_TYPE */
		g711_state,						/* CODEC_STATE pointer */			
		G711DecPhaseProcess,			/* FN: decode phase */
		G711EncPhaseProcess,			/* FN: encode phase */
		G711AddDecPlaytoneBuffer,		/* FN: add buffer */
		G711HighPassFiltering,			/* FN: high pass filtering */
		80,		/* 10ms */				/* N: budget size for tone */
		G711CmdParserStart,				/* FN: CmdParser Start */
		G711CmdParserStop,				/* FN: CmdParser Stop */
		G711IsJbcSidFrame,				/* FN: JBC: Is SID frame */
		10,								/* N: JBC: frame period */
#ifndef CONFIG_RTK_VOIP_MODULE 
		G711_set_codec_mem,				/* FN: set codec mem */
#endif /* !CONFIG_RTK_VOIP_MODULE */
		0,								/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,				/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
#ifdef CONFIG_RTK_VOIP_G7231
	{
		CODEC_TYPE_G7231,  				/* START_CODEC_TYPE */
		g7231_state,					/* CODEC_STATE pointer */
		G723DecPhaseProcess,			/* FN: decode phase */
		G723EncPhaseProcess,			/* FN: encode phase */
		G723AddDecPlaytoneBuffer,		/* FN: add buffer */
		G723HighPassFiltering,			/* FN: high pass filtering */
		240,	/* 30ms */				/* N: budget size for tone */
		G723CmdParserStart,				/* FN: CmdParser Start */
		G723CmdParserStop,				/* FN: CmdParser Stop */
		G723IsJbcSidFrame,				/* FN: JBC: Is SID frame */
		30,								/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE 
		G723_set_codec_mem,				/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		2,	/* 0, 1, 2 */				/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,				/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
#endif
#ifdef CONFIG_RTK_VOIP_G729AB
	{
		CODEC_TYPE_G729, 				/* START_CODEC_TYPE */
		g729_state,						/* CODEC_STATE pointer */
		G729DecPhaseProcess,			/* FN: decode phase */
		G729EncPhaseProcess,			/* FN: encode phase */
		G729AddDecPlaytoneBuffer,		/* FN: add buffer */
		G729HighPassFiltering,			/* FN: high pass filtering */
		80,		/* 10ms */				/* N: budget size for tone */
		G729CmdParserStart,				/* FN: CmdParser Start */
		G729CmdParserStop,				/* FN: CmdParser Stop */
		G729IsJbcSidFrame,				/* FN: JBC: Is SID frame */
		10,								/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE 
		G729_set_codec_mem,				/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		0,								/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,				/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
#endif
#ifdef CONFIG_RTK_VOIP_G726
	{
		CODEC_TYPE_G726, 				/* START_CODEC_TYPE */
		g726_state,						/* CODEC_STATE pointer */
		G726DecPhaseProcess,			/* FN: decode phase */
		G726EncPhaseProcess,			/* FN: encode phase */
		G726AddDecPlaytoneBuffer,		/* FN: add buffer */
		G726HighPassFiltering,			/* FN: high pass filtering */
		80,		/* 10ms */				/* N: budget size for tone */
		G726CmdParserStart,				/* FN: CmdParser Start */
		G726CmdParserStop,				/* FN: CmdParser Stop */
		G726IsJbcSidFrame,				/* FN: JBC: Is SID frame */
		10,								/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE 
		G726_set_codec_mem,				/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		0,								/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,				/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
#endif
#ifdef CONFIG_RTK_VOIP_GSMFR
	{
		CODEC_TYPE_GSMFR, 				/* START_CODEC_TYPE */
		gsmfr_state,					/* CODEC_STATE pointer */
		GSMfrDecPhaseProcess,			/* FN: decode phase */
		GSMfrEncPhaseProcess,			/* FN: encode phase */
		GSMfrAddDecPlaytoneBuffer,		/* FN: add buffer */
		GSMfrHighPassFiltering,			/* FN: high pass filtering */
		160,	/* 20ms */				/* N: budget size for tone */
		GSMfrCmdParserStart,			/* FN: CmdParser Start */
		GSMfrCmdParserStop,				/* FN: CmdParser Stop */
		GSMfrIsJbcSidFrame,				/* FN: JBC: Is SID frame */
		20,								/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE 
		GSMfr_set_codec_mem,			/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		1,	/* 0, 1 */					/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,				/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
#endif
#ifdef CONFIG_RTK_VOIP_ILBC
	{
		CODEC_TYPE_ILBC30MS, 				/* START_CODEC_TYPE */
		iLBC30ms_state,					/* CODEC_STATE pointer */
		iLBC30msDecPhaseProcess,			/* FN: decode phase */
		iLBC30msEncPhaseProcess,			/* FN: encode phase */
		iLBC30msAddDecPlaytoneBuffer,		/* FN: add buffer */
		iLBCHighPassFiltering,			/* FN: high pass filtering */
		240,	/* 30ms */				/* N: budget size for tone */
		iLBC30msCmdParserStart,			/* FN: CmdParser Start */
		iLBC30msCmdParserStop,				/* FN: CmdParser Stop */
		iLBC30msIsJbcSidFrame,				/* FN: JBC: Is SID frame */
		30,								/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE 
		iLBC_set_codec_mem,			/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		2,	/* 0, 1, 2 */					/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,				/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
	{
		CODEC_TYPE_ILBC20MS, 				/* START_CODEC_TYPE */
		iLBC20ms_state,					/* CODEC_STATE pointer */
		iLBC20msDecPhaseProcess,			/* FN: decode phase */
		iLBC20msEncPhaseProcess,			/* FN: encode phase */
		iLBC20msAddDecPlaytoneBuffer,		/* FN: add buffer */
		iLBCHighPassFiltering,			/* FN: high pass filtering */
		160,	/* 20ms */				/* N: budget size for tone */
		iLBC20msCmdParserStart,			/* FN: CmdParser Start */
		iLBC20msCmdParserStop,				/* FN: CmdParser Stop */
		iLBC20msIsJbcSidFrame,				/* FN: JBC: Is SID frame */
		20,								/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE 
		iLBC_set_codec_mem,			/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		1,	/* 0, 1 */					/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,				/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
#endif
#ifdef CONFIG_RTK_VOIP_G722
	{
		CODEC_TYPE_G722,  			/* START_CODEC_TYPE */
		g722_state,				/* CODEC_STATE pointer */
		G722DecPhaseProcess,			/* FN: decode phase */
		G722EncPhaseProcess,			/* FN: encode phase */
		G722AddDecPlaytoneBuffer,		/* FN: add buffer */
		G722HighPassFiltering,			/* FN: high pass filtering */
		80,	/* 10ms */			/* N: budget size for tone */
		G722CmdParserStart,			/* FN: CmdParser Start */
		G722CmdParserStop,			/* FN: CmdParser Stop */
		G722IsJbcSidFrame,			/* FN: JBC: Is SID frame */
		10,					/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE 
		G722_set_codec_mem,			/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		0,	/* 0, 1, 2 */			/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,			/* N: UDP carry type */
		SAMPLE_WIDE_BAND,				/* N: sampling rate */
	},
#endif
#ifdef CONFIG_RTK_VOIP_AMR_NB
	{
		CODEC_TYPE_AMR_NB,  			/* START_CODEC_TYPE */
		amr_nb_state,				/* CODEC_STATE pointer */
		AMR_NB_DecPhaseProcess,			/* FN: decode phase */
		AMR_NB_EncPhaseProcess,			/* FN: encode phase */
		AMR_NB_AddDecPlaytoneBuffer,		/* FN: add buffer */
		AMR_NB_HighPassFiltering,		/* FN: high pass filtering */
		160,	/* 20ms */			/* N: budget size for tone */
		AMR_NB_CmdParserStart,			/* FN: CmdParser Start */
		AMR_NB_CmdParserStop,			/* FN: CmdParser Stop */
		AMR_NB_IsJbcSidFrame,			/* FN: JBC: Is SID frame */
		20,					/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE 
		AMR_NB_set_codec_mem,			/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		1,	/* 0, 1, 2 */			/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,			/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
#endif
#ifdef CONFIG_RTK_VOIP_T38
	{
		CODEC_TYPE_T38, 				/* START_CODEC_TYPE */
		t38_state,						/* CODEC_STATE pointer */
		T38DecPhaseProcess,				/* FN: decode phase */
		T38EncPhaseProcess,				/* FN: encode phase */
		T38AddDecPlaytoneBuffer,		/* FN: add buffer */
		T38HighPassFiltering,			/* FN: high pass filtering */
		80,	/* 10ms */					/* N: budget size for tone */
		T38CmdParserStart,				/* FN: CmdParser Start */
		T38CmdParserStop,				/* FN: CmdParser Stop */
		T38IsJbcSidFrame, /*(unused)*/	/* FN: JBC: Is SID frame */
		10,				/*(unused)*/	/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE 
		T38_set_codec_mem,				/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		0,	/* 0 */						/* N: DSP process cut step */
		UDP_CARRY_TYPE_T38,				/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
#endif
#ifdef CONFIG_RTK_VOIP_SPEEX_NB
	{
		CODEC_TYPE_SPEEX_NB, 				/* START_CODEC_TYPE */
		speex_nb_state,					/* CODEC_STATE pointer */
		SPEEX_NB_DecPhaseProcess,			/* FN: decode phase */
		SPEEX_NB_EncPhaseProcess,			/* FN: encode phase */
		SPEEX_NB_AddDecPlaytoneBuffer,		/* FN: add buffer */
		SPEEX_NB_HighPassFiltering,			/* FN: high pass filtering */
		160,	/* 20ms */				/* N: budget size for tone */
		SPEEX_NB_CmdParserStart,			/* FN: CmdParser Start */
		SPEEX_NB_CmdParserStop,				/* FN: CmdParser Stop */
		SPEEX_NB_IsJbcSidFrame,				/* FN: JBC: Is SID frame */
		20,								/* N: JBC: frame period */
 #ifndef CONFIG_RTK_VOIP_MODULE
		SPEEX_NB_set_codec_mem,			/* FN: set codec mem */
 #endif /* !CONFIG_RTK_VOIP_MODULE */
		1,	/* 0, 1 */					/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,				/* N: UDP carry type */
		SAMPLE_NARROW_BAND,				/* N: sampling rate */
	},
#endif
#ifdef CONFIG_RTK_VOIP_SILENCE
	{
		CODEC_TYPE_SILENCE, 			/* START_CODEC_TYPE */
		silence_state,					/* CODEC_STATE pointer */			
		SilenceDecPhaseProcess,			/* FN: decode phase */
		SilenceEncPhaseProcess,			/* FN: encode phase */
		SilenceAddDecPlaytoneBuffer,	/* FN: add buffer */
		SilenceHighPassFiltering,		/* FN: high pass filtering */
		80,		/* 10ms */				/* N: budget size for tone */
		SilenceCmdParserStart,			/* FN: CmdParser Start */
		SilenceCmdParserStop,			/* FN: CmdParser Stop */
		SilenceIsJbcSidFrame,			/* FN: JBC: Is SID frame */
		10,								/* N: JBC: frame period */
#ifndef CONFIG_RTK_VOIP_MODULE 
		Silence_set_codec_mem,			/* FN: set codec mem */
#endif /* !CONFIG_RTK_VOIP_MODULE */
		0,								/* N: DSP process cut step */
		UDP_CARRY_TYPE_RTP,				/* N: UDP carry type */
		SAMPLE_IGNORE,					/* N: sampling rate */
	},
#endif
};

CT_ASSERT( ( sizeof( g_codecTypeDesc ) / sizeof( g_codecTypeDesc[ 0 ] ) ) == NUM_OF_CODEC_TYPE_DESC );
CT_ASSERT( CODEC_TYPE_NUMBER == NUM_OF_CODEC_TYPE_DESC );

/* ==================================================================
 * Codec classified by rtp payload type (711u, 711a, 726 ...)
 * ================================================================== */
#ifdef SUPPORT_BASEFRAME_10MS
#define G711_FRAME_REATE		10000
#define G722_FRAME_REATE		10000
#define G723_FRAME_REATE		30000
#define G729_FRAME_REATE		10000
#define G726_FRAME_REATE		10000
#define GSMfr_FRAME_REATE		20000
#define iLBC30ms_FRAME_REATE	30000
#define iLBC20ms_FRAME_REATE	20000
#define AMR_NB_FRAME_REATE		20000
#define T38_FRAME_REATE			10000
#define SPEEX_NB_FRAME_REATE		20000
#define SILENCE_FRAME_REATE		10000
#else
#define G711_FRAME_REATE		20000
#define G722_FRAME_REATE		20000
#define G723_FRAME_REATE		30000
#define G729_FRAME_REATE		20000
#define G726_FRAME_REATE		20000
#define GSMfr_FRAME_REATE		20000
#define iLBC30ms_FRAME_REATE	30000
#define iLBC20ms_FRAME_REATE	20000
#define AMR_NB_FRAME_REATE		20000
#define T38_FRAME_REATE			10000
#define SPEEX_NB_FRAME_REATE		20000
#define SILENCE_FRAME_REATE		10000
#endif /* SUPPORT_BASEFRAME_10MS */

extern uint32 G729GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 G711GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 G722GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 G723GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 G726_16GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 G726_24GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 G726_32GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 G726_40GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 GSMfrGetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 iLBC30msGetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 iLBC20msGetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 AMR_NB_GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 T38GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
extern uint32 SPEEX_NB_GetTxNumberOfFrame( uint32 nSize, const char *pBuffer );
#define SilenceGetTxNumberOfFrame	G711GetTxNumberOfFrame

const codec_payload_desc_t g_codecPayloadDesc[] = {
	{
		rtpPayloadPCMU,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_G711U,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_G711U,		/* DSPCODEC_ALGORITHM */
		G711_FRAME_REATE,				/* N: recv rate */
		G711_FRAME_REATE,				/* N: transfer rate */
		"Set codec on PCMU RTP\n",		/* C: set codec prompt */
		80,								/* N: frame bytes */
		6,								/* N: h-thres Tx frame per packet */
		1,								/* N: tx SID frame bytes */
		10,								/* N: frame period (ms) */
		80,								/* N: frame timestamp */
		G711GetTxNumberOfFrame,			/* FN: Get Tx number of frame */
	},
	{
		rtpPayloadPCMA,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_G711A,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_G711A,		/* DSPCODEC_ALGORITHM */
		G711_FRAME_REATE,				/* N: recv rate */
		G711_FRAME_REATE,				/* N: transfer rate */
		"Set codec on PCMA RTP\n",		/* C: set codec prompt */
		80,								/* N: frame bytes */
		6,								/* N: h-thres Tx frame per packet */
		1,								/* N: tx SID frame bytes */
		10,								/* N: frame period (ms) */
		80,								/* N: frame timestamp */
		G711GetTxNumberOfFrame,			/* FN: Get Tx number of frame */
	},
#ifdef CONFIG_RTK_VOIP_G7231
	{
		rtpPayloadG723,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_G7231A63,	/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_G7231A53,	/* DSPCODEC_ALGORITHM (G723type==1) */
		G723_FRAME_REATE,				/* N: recv rate */
		G723_FRAME_REATE,				/* N: transfer rate */
		"Set codec on G723 RTP\n",		/* C: set codec prompt */
		24,		/* max(20, 24) */		/* N: frame bytes */
		3,								/* N: h-thres Tx frame per packet */
		4,								/* N: tx SID frame bytes */
		30,								/* N: frame period (ms) */
		240,							/* N: frame timestamp */
		G723GetTxNumberOfFrame,			/* FN: Get Tx number of frame */
	},
#endif /* CONFIG_RTK_VOIP_G7231 */
#ifdef CONFIG_RTK_VOIP_G729AB
	{
		rtpPayloadG729,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_G729,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_G729,		/* DSPCODEC_ALGORITHM */
		G729_FRAME_REATE,				/* N: recv rate */
		G729_FRAME_REATE,				/* N: transfer rate */
		"Set codec on G729 RTP\n",		/* C: set codec prompt */
		10,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		2,								/* N: tx SID frame bytes */
		10,								/* N: frame period (ms) */
		80,								/* N: frame timestamp */
		G729GetTxNumberOfFrame,			/* FN: Get Tx number of frame */
	},
#endif /* CONFIG_RTK_VOIP_G729AB */
#ifdef CONFIG_RTK_VOIP_G726
	{
		rtpPayloadG726_16,				/* RtpPayloadType */
		DSPCODEC_ALGORITHM_G72616,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_G72616,		/* DSPCODEC_ALGORITHM */
		G726_FRAME_REATE,				/* N: recv rate */
		G726_FRAME_REATE,				/* N: transfer rate */
		"Set codec on G726_16 RTP\n",	/* C: set codec prompt */
		20,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		1,								/* N: tx SID frame bytes */
		10,								/* N: frame period (ms) */
		80,								/* N: frame timestamp */
		G726_16GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayloadG726_24,				/* RtpPayloadType */
		DSPCODEC_ALGORITHM_G72624,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_G72624,		/* DSPCODEC_ALGORITHM */
		G726_FRAME_REATE,				/* N: recv rate */
		G726_FRAME_REATE,				/* N: transfer rate */
		"Set codec on G726_24 RTP\n",	/* C: set codec prompt */
		30,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		1,								/* N: tx SID frame bytes */
		10,								/* N: frame period (ms) */
		80,								/* N: frame timestamp */
		G726_24GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayloadG726_32,				/* RtpPayloadType */
		DSPCODEC_ALGORITHM_G72632,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_G72632,		/* DSPCODEC_ALGORITHM */
		G726_FRAME_REATE,				/* N: recv rate */
		G726_FRAME_REATE,				/* N: transfer rate */
		"Set codec on G726_32 RTP\n",	/* C: set codec prompt */
		40,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		1,								/* N: tx SID frame bytes */
		10,								/* N: frame period (ms) */
		80,								/* N: frame timestamp */
		G726_32GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayloadG726_40,				/* RtpPayloadType */
		DSPCODEC_ALGORITHM_G72640,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_G72640,		/* DSPCODEC_ALGORITHM */
		G726_FRAME_REATE,				/* N: recv rate */
		G726_FRAME_REATE,				/* N: transfer rate */
		"Set codec on G726_40 RTP\n",	/* C: set codec prompt */
		50,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		1,								/* N: tx SID frame bytes */
		10,								/* N: frame period (ms) */
		80,								/* N: frame timestamp */
		G726_40GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
#endif /* CONFIG_RTK_VOIP_G726 */
#ifdef CONFIG_RTK_VOIP_GSMFR
	{
		rtpPayloadGSM,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_GSMFR,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_GSMFR,		/* DSPCODEC_ALGORITHM */
		GSMfr_FRAME_REATE,				/* N: recv rate */
		GSMfr_FRAME_REATE,				/* N: transfer rate */
		"Set codec on GSM_FR RTP\n",	/* C: set codec prompt */
		33,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		5,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		GSMfrGetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
#endif /* CONFIG_RTK_VOIP_GSMFR */
#ifdef CONFIG_RTK_VOIP_ILBC
	{
		rtpPayload_iLBC,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_ILBC30MS,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_ILBC30MS,		/* DSPCODEC_ALGORITHM */
		iLBC30ms_FRAME_REATE,				/* N: recv rate */
		iLBC30ms_FRAME_REATE,				/* N: transfer rate */
		"Set codec on iLBC 30ms RTP\n",	/* C: set codec prompt */
		50,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		5,	/* (impossible value) */	/* N: tx SID frame bytes */
		30,								/* N: frame period (ms) */
		240,							/* N: frame timestamp */
		iLBC30msGetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayload_iLBC_20ms,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_ILBC20MS,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_ILBC20MS,		/* DSPCODEC_ALGORITHM */
		iLBC20ms_FRAME_REATE,				/* N: recv rate */
		iLBC20ms_FRAME_REATE,				/* N: transfer rate */
		"Set codec on iLBC 20ms RTP\n",	/* C: set codec prompt */
		38,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		5,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		iLBC20msGetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
#endif /* CONFIG_RTK_VOIP_ILBC */
#ifdef CONFIG_RTK_VOIP_G722
	{
		rtpPayloadG722,				/* RtpPayloadType */
		DSPCODEC_ALGORITHM_G72264,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_G72264,		/* DSPCODEC_ALGORITHM (G723type==1) */
		G722_FRAME_REATE,			/* N: recv rate */
		G722_FRAME_REATE,			/* N: transfer rate */
		"Set codec on G722 RTP\n",		/* C: set codec prompt */
		80,		/* max(20, 24) */	/* N: frame bytes */
		6,					/* N: h-thres Tx frame per packet */
		1,					/* N: tx SID frame bytes */
		10,					/* N: frame period (ms) */
		80,					/* N: frame timestamp */
		G722GetTxNumberOfFrame,			/* FN: Get Tx number of frame */
	},
#endif /* CONFIG_RTK_VOIP_G722 */
#ifdef CONFIG_RTK_VOIP_AMR_NB
	{
		rtpPayload_AMR_NB,			/* RtpPayloadType */
		DSPCODEC_ALGORITHM_AMR_NB,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_AMR_NB,		/* DSPCODEC_ALGORITHM */
		AMR_NB_FRAME_REATE,			/* N: recv rate */
		AMR_NB_FRAME_REATE,			/* N: transfer rate */
		"Set codec on AMR_NB RTP\n",		/* C: set codec prompt */
	#ifdef WENGO
		34,					/* N: frame bytes */
	#else
		33,					/* N: frame bytes */
	#endif
		9,					/* N: h-thres Tx frame per packet */
	#ifdef WENGO 
		8,
	#else
		7,					/* N: tx SID frame bytes */
	#endif
		20,					/* N: frame period (ms) */
		160,					/* N: frame timestamp */
		AMR_NB_GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
#endif /* CONFIG_RTK_VOIP_AMR_NB */
#ifdef CONFIG_RTK_VOIP_T38
	{
		rtpPayloadT38_Virtual,			/* RtpPayloadType */
		DSPCODEC_ALGORITHM_T38,			/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_T38,			/* DSPCODEC_ALGORITHM */
		T38_FRAME_REATE, /*(unused)*/	/* N: recv rate */
		T38_FRAME_REATE, /*(unused)*/	/* N: transfer rate */
		"Set codec on T.38 UDP\n",		/* C: set codec prompt */
		80,				/*(unused)*/	/* N: frame bytes */
		9,				/*(unused)*/	/* N: h-thres Tx frame per packet */
		5,				/*(unused)*/	/* N: tx SID frame bytes */
		10,				/*(unused)*/	/* N: frame period (ms) */
		160,			/*(unused)*/	/* N: frame timestamp */
		T38GetTxNumberOfFrame, /*(unused)*/	/* FN: Get Tx number of frame */
	},
#endif /* CONFIG_RTK_VOIP_T38 */
#ifdef CONFIG_RTK_VOIP_SPEEX_NB
	{
		rtpPayload_SPEEX_NB_RATE2P15,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_SPEEX_NB_2P15,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_SPEEX_NB_2P15,		/* DSPCODEC_ALGORITHM */
		SPEEX_NB_FRAME_REATE,				/* N: recv rate */
		SPEEX_NB_FRAME_REATE,				/* N: transfer rate */
		"Set codec on SPEEX-NB 2.15k RTP\n",	/* C: set codec prompt */
		6,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		4,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		SPEEX_NB_GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayload_SPEEX_NB_RATE5P95,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_SPEEX_NB_5P95,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_SPEEX_NB_5P95,		/* DSPCODEC_ALGORITHM */
		SPEEX_NB_FRAME_REATE,				/* N: recv rate */
		SPEEX_NB_FRAME_REATE,				/* N: transfer rate */
		"Set codec on SPEEX-NB 5.95k RTP\n",	/* C: set codec prompt */
		15,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		4,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		SPEEX_NB_GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayload_SPEEX_NB_RATE8,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_SPEEX_NB_8,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_SPEEX_NB_8,		/* DSPCODEC_ALGORITHM */
		SPEEX_NB_FRAME_REATE,				/* N: recv rate */
		SPEEX_NB_FRAME_REATE,				/* N: transfer rate */
		"Set codec on SPEEX-NB 8k RTP\n",	/* C: set codec prompt */
		20,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		4,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		SPEEX_NB_GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayload_SPEEX_NB_RATE11,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_SPEEX_NB_11,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_SPEEX_NB_11,		/* DSPCODEC_ALGORITHM */
		SPEEX_NB_FRAME_REATE,				/* N: recv rate */
		SPEEX_NB_FRAME_REATE,				/* N: transfer rate */
		"Set codec on SPEEX-NB 11k RTP\n",	/* C: set codec prompt */
		28,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		4,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		SPEEX_NB_GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayload_SPEEX_NB_RATE15,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_SPEEX_NB_15,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_SPEEX_NB_15,		/* DSPCODEC_ALGORITHM */
		SPEEX_NB_FRAME_REATE,				/* N: recv rate */
		SPEEX_NB_FRAME_REATE,				/* N: transfer rate */
		"Set codec on SPEEX-NB 15k RTP\n",	/* C: set codec prompt */
		38,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		4,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		SPEEX_NB_GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayload_SPEEX_NB_RATE18P2,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_SPEEX_NB_18P2,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_SPEEX_NB_18P2,		/* DSPCODEC_ALGORITHM */
		SPEEX_NB_FRAME_REATE,				/* N: recv rate */
		SPEEX_NB_FRAME_REATE,				/* N: transfer rate */
		"Set codec on SPEEX-NB 18.2k RTP\n",	/* C: set codec prompt */
		46,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		4,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		SPEEX_NB_GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayload_SPEEX_NB_RATE24P6,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_SPEEX_NB_24P6,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_SPEEX_NB_24P6,		/* DSPCODEC_ALGORITHM */
		SPEEX_NB_FRAME_REATE,				/* N: recv rate */
		SPEEX_NB_FRAME_REATE,				/* N: transfer rate */
		"Set codec on SPEEX-NB 24.6k RTP\n",	/* C: set codec prompt */
		62,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		4,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		SPEEX_NB_GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
	{
		rtpPayload_SPEEX_NB_RATE3P95,					/* RtpPayloadType */
		DSPCODEC_ALGORITHM_SPEEX_NB_3P95,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_SPEEX_NB_3P95,		/* DSPCODEC_ALGORITHM */
		SPEEX_NB_FRAME_REATE,				/* N: recv rate */
		SPEEX_NB_FRAME_REATE,				/* N: transfer rate */
		"Set codec on SPEEX-NB 3.95k RTP\n",	/* C: set codec prompt */
		10,								/* N: frame bytes */
		9,								/* N: h-thres Tx frame per packet */
		4,	/* (impossible value) */	/* N: tx SID frame bytes */
		20,								/* N: frame period (ms) */
		160,							/* N: frame timestamp */
		SPEEX_NB_GetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
#endif /* #ifdef CONFIG_RTK_VOIP_SPEEX_NB */
#ifdef CONFIG_RTK_VOIP_SILENCE
	{
		rtpPayloadSilence,				/* RtpPayloadType */
		DSPCODEC_ALGORITHM_SILENCE,		/* DSPCODEC_ALGORITHM */
		DSPCODEC_ALGORITHM_SILENCE,		/* DSPCODEC_ALGORITHM */
		SILENCE_FRAME_REATE,			/* N: recv rate */
		SILENCE_FRAME_REATE,			/* N: transfer rate */
		"Set codec on Silence Codec\n",	/* C: set codec prompt */
		80,								/* N: frame bytes */
		6,								/* N: h-thres Tx frame per packet */
		1,								/* N: tx SID frame bytes */
		10,								/* N: frame period (ms) */
		80,								/* N: frame timestamp */
		SilenceGetTxNumberOfFrame,		/* FN: Get Tx number of frame */
	},
#endif /* CONFIG_RTK_VOIP_SILENCE */
};

CT_ASSERT( ( sizeof( g_codecPayloadDesc ) / sizeof( g_codecPayloadDesc[ 0 ] ) ) == ( NUM_OF_CODEC_PAYLOAD_DESC ) );


/* ******************************************************************
 * Assistant functions
 * ****************************************************************** */
extern const codec_type_desc_t *ppStartCodecTypeDesc[MAX_SESS_NUM];


/* ---------------- Check functions ---------------- */
int IsAnyCodecDecodeState( uint32 sid )
{
	int i;
	
	for( i = 0; i < NUM_OF_CODEC_TYPE_DESC; i ++ )
		if( g_codecTypeDesc[ i ].pCodecState[ sid ] & DECODE )
			return 1;

	return 0;
}

int IsJbcSidFrameOfThisCodec( uint32 sid, uint32 nSize )
{
	const codec_type_desc_t *pCodecTypeDesc;

	pCodecTypeDesc = ppStartCodecTypeDesc[ sid ];
	
	if( pCodecTypeDesc )
		return ( *pCodecTypeDesc ->fnIsJbcSidFrame )( sid, nSize );

	return 0;	
}

enum START_CODEC_TYPE GetGlobalStartCodecType( uint32 sid )
{
	const codec_type_desc_t *pCodecTypeDesc;
	
	pCodecTypeDesc = ppStartCodecTypeDesc[ sid ];
	
	if( pCodecTypeDesc )
		return pCodecTypeDesc ->codecStartType;
	
	return CODEC_TYPE_UNKNOW;
}

int CheckG711StartCodecType( uint32 sid )
{
	return ( GetGlobalStartCodecType( sid ) == CODEC_TYPE_G711 );
}

#ifdef CONFIG_RTK_VOIP_G722
int CheckG722StartCodecType( uint32 sid )
{
	return ( GetGlobalStartCodecType( sid ) == CODEC_TYPE_G722 );
}
#else
int CheckG722StartCodecType( uint32 sid )
{
	return 0;
}
#endif

#ifdef CONFIG_RTK_VOIP_AMR_NB
int Check_AMR_NB_StartCodecType( uint32 sid )
{
	return ( GetGlobalStartCodecType( sid ) == CODEC_TYPE_AMR_NB );
}
#else
int Check_AMR_NB_StartCodecType( uint32 sid )
{
	return 0;
}
#endif

#ifdef CONFIG_RTK_VOIP_G7231
int CheckG723StartCodecType( uint32 sid )
{
	return ( GetGlobalStartCodecType( sid ) == CODEC_TYPE_G7231 );
}
#endif

#ifdef CONFIG_RTK_VOIP_G729AB
int CheckG729StartCodecType( uint32 sid )
{
	return ( GetGlobalStartCodecType( sid ) == CODEC_TYPE_G729 );
}
#else
int CheckG729StartCodecType( uint32 sid )
{
	return 0;
}
#endif

/* ------- complement functions for undefined codecs -------- */
#ifndef CONFIG_RTK_VOIP_T38
uint32 T38_API_PutPacket( uint32 chid, 
						  const unsigned char *pPacketInBuffer, 
						  uint32 nPacketInSize )
{
	return 0;
}
#endif

/* ---------------- codec type ID declare ---------------- */
const enum START_CODEC_TYPE nCodecTypeID_G711 = CODEC_TYPE_G711;

#ifdef CONFIG_RTK_VOIP_G722
const enum START_CODEC_TYPE nCodecTypeID_G722 = CODEC_TYPE_G722;
#endif
#ifdef CONFIG_RTK_VOIP_G7231
const enum START_CODEC_TYPE nCodecTypeID_G723 = CODEC_TYPE_G7231;
#endif
#ifdef CONFIG_RTK_VOIP_G729AB
const enum START_CODEC_TYPE nCodecTypeID_G729 = CODEC_TYPE_G729;
#endif
#ifdef CONFIG_RTK_VOIP_G726
const enum START_CODEC_TYPE nCodecTypeID_G726 = CODEC_TYPE_G726;
#endif
#ifdef CONFIG_RTK_VOIP_GSMFR
const enum START_CODEC_TYPE nCodecTypeID_GSMfr = CODEC_TYPE_GSMFR;
#endif
#ifdef CONFIG_RTK_VOIP_ILBC
const enum START_CODEC_TYPE nCodecTypeID_iLBC30ms = CODEC_TYPE_ILBC30MS;
const enum START_CODEC_TYPE nCodecTypeID_iLBC20ms = CODEC_TYPE_ILBC20MS;
#endif
#ifdef CONFIG_RTK_VOIP_AMR_NB
const enum START_CODEC_TYPE nCodecTypeID_AMR_NB = CODEC_TYPE_AMR_NB;
#endif
#ifdef CONFIG_RTK_VOIP_T38
const enum START_CODEC_TYPE nCodecTypeID_T38 = CODEC_TYPE_T38;
#endif
#ifdef CONFIG_RTK_VOIP_SPEEX_NB
const enum START_CODEC_TYPE nCodecTypeID_SPEEX_NB = CODEC_TYPE_SPEEX_NB;
#endif
#ifdef CONFIG_RTK_VOIP_SILENCE
const enum START_CODEC_TYPE nCodecTypeID_Silence = CODEC_TYPE_SILENCE;
#endif

/* ---------------- codec algorithm ID declare ---------------- */ 
#ifdef CONFIG_RTK_VOIP_G722
const DSPCODEC_ALGORITHM nCodecAlgorithm_G72264 = DSPCODEC_ALGORITHM_G72264;
#endif

#ifdef CONFIG_RTK_VOIP_G7231
const DSPCODEC_ALGORITHM nCodecAlgorithm_G7231A63 = DSPCODEC_ALGORITHM_G7231A63;
#endif

#ifdef CONFIG_RTK_VOIP_G726
const DSPCODEC_ALGORITHM nCodecAlgorithm_G72616 = DSPCODEC_ALGORITHM_G72616;
const DSPCODEC_ALGORITHM nCodecAlgorithm_G72624 = DSPCODEC_ALGORITHM_G72624;
const DSPCODEC_ALGORITHM nCodecAlgorithm_G72632 = DSPCODEC_ALGORITHM_G72632;
const DSPCODEC_ALGORITHM nCodecAlgorithm_G72640 = DSPCODEC_ALGORITHM_G72640;
#endif

#ifdef CONFIG_RTK_VOIP_AMR_NB
const DSPCODEC_ALGORITHM nCodecAlgorithm_AMR_NB = DSPCODEC_ALGORITHM_AMR_NB;
#endif

#ifdef CONFIG_RTK_VOIP_SPEEX_NB
const DSPCODEC_ALGORITHM nCodecAlgorithm_SPEEX_NB_2P15 = DSPCODEC_ALGORITHM_SPEEX_NB_2P15;
const DSPCODEC_ALGORITHM nCodecAlgorithm_SPEEX_NB_5P95 = DSPCODEC_ALGORITHM_SPEEX_NB_5P95;
const DSPCODEC_ALGORITHM nCodecAlgorithm_SPEEX_NB_8 = DSPCODEC_ALGORITHM_SPEEX_NB_8;
const DSPCODEC_ALGORITHM nCodecAlgorithm_SPEEX_NB_11 = DSPCODEC_ALGORITHM_SPEEX_NB_11;
const DSPCODEC_ALGORITHM nCodecAlgorithm_SPEEX_NB_15 = DSPCODEC_ALGORITHM_SPEEX_NB_15;
const DSPCODEC_ALGORITHM nCodecAlgorithm_SPEEX_NB_18P2 = DSPCODEC_ALGORITHM_SPEEX_NB_18P2;
const DSPCODEC_ALGORITHM nCodecAlgorithm_SPEEX_NB_24P6 = DSPCODEC_ALGORITHM_SPEEX_NB_24P6;
const DSPCODEC_ALGORITHM nCodecAlgorithm_SPEEX_NB_3P95 = DSPCODEC_ALGORITHM_SPEEX_NB_3P95;
#endif

/* ---------------- Get descriptor functions ---------------- */
const codec_type_desc_t *GetCodecTypeDesc( enum START_CODEC_TYPE type )
{
	if( type < NUM_OF_CODEC_TYPE_DESC )
		return &g_codecTypeDesc[ type ];

	printk( "Invalid codec_type:%d?\n", type );
	return NULL;
}

const codec_algo_desc_t *GetCodecAlgoDesc( DSPCODEC_ALGORITHM algo )
{
	if( algo < DSPCODEC_ALGORITHM_NUMBER )
		return &g_codecAlgoDesc[ algo ];
	
	return NULL;
}

/* ---------------- Do something for all codec ---------------- */
void InitializeAllCodecState( uint32 sid )
{
	int i;

	for( i = 0; i < NUM_OF_CODEC_TYPE_DESC; i ++ )
		g_codecTypeDesc[ i ].pCodecState[ sid ] = INVALID;
}

void DoAllCodecDecPhase( uint32 sid ) 
{
	int i;
	
	for( i = 0; i < NUM_OF_CODEC_TYPE_DESC; i ++ )
		( *g_codecTypeDesc[ i ].fnCodecDecPhase )( sid );
}

void DoAllCodecEncPhase( uint32 sid ) 
{
	int i;

	for( i = 0; i < NUM_OF_CODEC_TYPE_DESC; i ++ )
		( *g_codecTypeDesc[ i ].fnCodecEncPhase )( sid );
}

