#include "codec_descriptor.h"

/*
 * Calculate number of frames in current packet
 */
uint32 G723GetNumberOfFramesInCurrentPacket( const unsigned char *pBuffer,
											 uint32 nSize, 
											 int TempLen,
											 int TempLenSID,
											 int *pbAppendixSID )
{
	/* 
	 * ITU G.723.1 (page 1) says, "It is possible to switch between
	 * the two rates at any 30ms frame boundary."
	 */
	uint32 nFrames = 0;
	
	while( nSize > 0 ) {
		if( pBuffer[ 0 ] & 0x02 ) {			// SID
			*pbAppendixSID = 1;
			break;
		} else if( pBuffer[ 0 ] & 0x01 )	// Voice: 5.3k
			TempLen = 20;
		else								// Voice: 6.4k
			TempLen = 24;
		
		nFrames ++;
		nSize -= TempLen;
		pBuffer += TempLen;
	}
	
	return nFrames;
}

uint32 SPEEX_NBGetNumberOfFramesInCurrentPacket( const unsigned char *pBuffer,
											 uint32 nSize,
											 int TempLen,
											 int TempLenSID,
											 int *pbAppendixSID )
{

	uint32 nFrames = 0;

	/* BUG : FIXED ME, NEED RE-FINE */

	return 1;
}

uint32 GetNumberOfFramesInCurrentPacket( const unsigned char *pBuffer,
										 uint32 nSize, 
										 int TempLen,
										 int TempLenSID,
										 int *pbAppendixSID )
{
	uint32 nFrames = 0;

	while( nSize > 0 ) {
#ifdef SUPPORT_APPENDIX_SID
		if( nSize >= TempLen )
			nFrames ++;
		else if( nSize == TempLenSID ) {
			*pbAppendixSID = 1;	// NOTE: Appendix SID is not included in i!
			break;
		} else
			break; 		
#else
		nFrames ++;
#endif // SUPPORT_APPENDIX_SID
		nSize -= TempLen;
	}
	
	return nFrames;
}

/*
 * Get frame infomation of frame length
 */
uint32 G711GetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 80;
}

#ifdef CONFIG_RTK_VOIP_G729AB
uint32 G729GetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 10;
}
#endif

#ifdef CONFIG_RTK_VOIP_G726
uint32 G72616GetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 20;
}

uint32 G72624GetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 30;
}

uint32 G72632GetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 40;
}
		
uint32 G72640GetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 50;
}
#endif /* CONFIG_RTK_VOIP_G726 */

#ifdef CONFIG_RTK_VOIP_G722
uint32 G722GetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 80;
}
#endif

#ifdef CONFIG_RTK_VOIP_G7231
uint32 G723GetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	if( pBuffer[ 0 ] & 0x01 )	// RATE FLAG
		return 20;	// 1: 5.3k
	else
		return 24;	// 0: 6.3k		
}
#endif

#ifdef CONFIG_RTK_VOIP_GSMFR
uint32 GSMfrGetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 33;
}
#endif

#ifdef CONFIG_RTK_VOIP_ILBC
uint32 iLBC30msGetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 50;
}
uint32 iLBC20msGetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	return 38;
}
#endif

#ifdef CONFIG_RTK_VOIP_AMR_NB
#include "../dsp_r1/amr_nb/include/amr_nb.h"
uint32 AMRGetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	uint32 len = 0, ft = 0;
	
	ft = ((*(pBuffer+1))&0x78) >> 3;

	switch ( ft )
	{
		case 0:
			len = 14;
			break;
		case 1:
			len = 15;
			break;
		case 2:
			len = 17;
			break;
		case 3:
			len = 19;
			break;
		case 4:
			len = 21;
			break;
		case 5:
			len = 22;
			break;
		case 6:
			len = 28;
			break;
		case 7:
			len = 33;
			break;
		case 8:
			len = 33; // any length which larger then 7 or 8 bytes
			break;
		default:
			len = 33;
			printk("Error! AMR get frame len error.\n");
			break;
	}

#ifdef WENGO
	len++;
#endif
	return len;

}
#endif

#ifdef CONFIG_RTK_VOIP_T38
uint32 T38GetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	/* This is a dummy function */
	return 80;
}
#endif

#ifdef CONFIG_RTK_VOIP_SPEEX_NB
uint32 SPEEX_NBGetFrameInfo_FrameLength( const unsigned char *pBuffer )
{
	uint32 len = 0, ft = 0;

	if ((*pBuffer) & 0x80)
	{
		return 0;/* ERROR Wideband */
		printk("Error! SPEEX get Wideband, frame len error.\n");
	}
	ft = ((*pBuffer)&0x78) >> 3;

	switch ( ft )
	{
		case 1:
			len = 6;
			break;
		case 2:
			len = 15;
			break;
		case 3:
			len = 20;
			break;
		case 4:
			len = 28;
			break;
		case 5:
			len = 38;
			break;
		case 6:
			len = 46;
			break;
		case 7:
			len = 62;
			break;
		case 8:
			len = 10;
			break;
		default:
			len = 20;
			printk("Error! SPEEX get frame len error.\n");
			break;
	}

	return len;
}
#endif

/*
 * Get frame infomation of SID length
 */
uint32 G711GetFrameInfo_SidLength( uint32 nSize )
{
	// According to ITU G711 appendix II: SID length = M + 1 bytes (M >=0)
	return nSize - (nSize/80)*80;
}

#ifdef CONFIG_RTK_VOIP_G729AB
uint32 G729GetFrameInfo_SidLength( uint32 nSize )
{
	return 2;
}
#endif

#ifdef CONFIG_RTK_VOIP_G726
uint32 G72616GetFrameInfo_SidLength( uint32 nSize )
{
	return nSize - (nSize/20)*20;
}

uint32 G72624GetFrameInfo_SidLength( uint32 nSize )
{
	return nSize - (nSize/30)*30;
}

uint32 G72632GetFrameInfo_SidLength( uint32 nSize )
{
	return nSize - (nSize/40)*40;
}

uint32 G72640GetFrameInfo_SidLength( uint32 nSize )
{
	return nSize - (nSize/50)*50;	
}			
#endif /* CONFIG_RTK_VOIP_G726 */

#ifdef CONFIG_RTK_VOIP_G722
uint32 G722GetFrameInfo_SidLength( uint32 nSize )
{
	return nSize - (nSize/80)*80;
}
#endif

#ifdef CONFIG_RTK_VOIP_G7231
uint32 G723GetFrameInfo_SidLength( uint32 nSize )
{
	return 4;
}
#endif

#ifdef CONFIG_RTK_VOIP_GSMFR
uint32 GSMfrGetFrameInfo_SidLength( uint32 nSize )
{
	/* GSM has no SID frame!! */
	return 5;	/* impossible value */
}
#endif

#ifdef CONFIG_RTK_VOIP_ILBC
uint32 iLBCGetFrameInfo_SidLength( uint32 nSize )
{
	/* iLBC has no SID frame!! */
	return 5;	/* impossible value */
}
#endif

#ifdef CONFIG_RTK_VOIP_AMR_NB
uint32 AMRGetFrameInfo_SidLength( uint32 nSize )
{
#ifdef WENGO 
	return 8;
#else
	return 7;
#endif
}
#endif

#ifdef CONFIG_RTK_VOIP_T38
uint32 T38GetFrameInfo_SidLength( uint32 nSize )
{
	/* This is a dummy function */
	return nSize;	/* impossible value */
}
#endif

#ifdef CONFIG_RTK_VOIP_SPEEX_NB
uint32 SPEEX_NBGetFrameInfo_SidLength( uint32 nSize )
{
	/* SPEEX NB fix-point has no SID frame!! */
	return 5;	/* impossible value */
}
#endif
