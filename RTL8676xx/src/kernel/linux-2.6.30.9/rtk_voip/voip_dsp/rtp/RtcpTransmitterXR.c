#include "rtk_voip.h"
#include "voip_types.h"
#include "rtpTypes.h"
#include "RtcpPacket.h"
#include "RtpPacket.h"
#include "NtpTime.h"
#include "Rtcp.h"
#include "codec_descriptor.h"
#include "lec.h"
#include "RtcpXR_RLE.h"
#include "RtpReceiverXR.h"

extern RtcpPacket RTCP_TX_DEC[RTCP_TX_DEC_NUM];
extern RtcpTransmitter RtcpTxInfo[MAX_SESS_NUM];

extern RtpReceiver* RtpRx_getInfo (uint32 sid);

extern uint32 RtcpXR_SignalDetection[ MAX_VOIP_CH_NUM ];// = {[0 ... MAX_VOIP_CH_NUM-1] = 1};
extern uint32 RtcpXR_SignalLevel[ MAX_VOIP_CH_NUM ];// = {[0 ... MAX_VOIP_CH_NUM-1] = 0};	// Q8
extern uint32 RtcpXR_NoiseLevel[ MAX_VOIP_CH_NUM ];// = {[0 ... MAX_VOIP_CH_NUM-1] = 0};	// Q8

void RtcpTx_reset( uint32 chid, uint32 sid )
{
	// do DSP related reset 
	
	// signal level 
	RtcpXR_SignalLevel[ chid ] = 0;
	RtcpXR_NoiseLevel[ chid ] = 0;
}

static int RtcpTx_addXR_LossRLE(uint32 sid, RtcpPacket* p, int npadSize)
{
	// rtcpXRBT_LossRLE = 1
	RtcpXRReportBlockLossRLE *LossRLE = 
			( RtcpXRReportBlockLossRLE * )(RtcpPkt_freeData(p));
	RtpReceiver * const rtpReceiver = RtpRx_getInfo( sid );
	RtpReceiverXR * const rtpReceiverXR = &rtpReceiver ->xr;
	uint16 data_bits;
	uint16 end_seq_real;
	uint32 chunks;
		
	// allocate block header
	RtcpPkt_allocData( p, sizeof( RtcpXRReportBlockLossRLE ) 
							- sizeof( RtcpXRChunk ) * 2 );
	
	// header 
	LossRLE ->blockType = rtcpXRBT_LossRLE;
	LossRLE ->rsvd = 1;
	LossRLE ->thinning = LOSS_THINNING;
	LossRLE ->blockLength = 2;
	LossRLE ->SSRC = rtpReceiver ->ssrc;
	
	// determine real end_seq  and data_bits 
	data_bits = ( uint16 )( rtpReceiverXR ->end_seq - 
							rtpReceiverXR ->begin_seq );
	
	if( data_bits > MAX_NUM_XR_LOSS ) {
		end_seq_real = rtpReceiverXR ->begin_seq + MAX_NUM_XR_LOSS;
		data_bits = MAX_NUM_XR_LOSS;
	} else {
		end_seq_real = rtpReceiverXR ->end_seq;
	}
	
	// header - seq 
	LossRLE ->begin_seq = rtpReceiverXR ->begin_seq;
	LossRLE ->end_seq = end_seq_real;
	
	// chunk data 
	chunks = RunLengthEncode( rtpReceiverXR ->loss_bits, 
								data_bits,
								LossRLE ->chunk );
	
	RtcpPkt_allocData( p, sizeof( RtcpXRChunk ) * chunks );
	LossRLE ->blockLength += chunks / 2;		
	
	return ( LossRLE ->blockLength + 1 ) * 4;	// +1 for header 
}

static int RtcpTx_addXR_DupRLE(uint32 sid, RtcpPacket* p, int npadSize)
{
	// rtcpXRBT_DuplicateRLE = 2
	RtcpXRReportBlockDuplicateRLE *DupRLE = 
			( RtcpXRReportBlockDuplicateRLE * )(RtcpPkt_freeData(p));
	RtpReceiver * const rtpReceiver = RtpRx_getInfo( sid );
	RtpReceiverXR * const rtpReceiverXR = &rtpReceiver ->xr;
	uint16 data_bits;
	uint16 end_seq_real;
	uint32 chunks;
		
	// allocate block header
	RtcpPkt_allocData( p, sizeof( RtcpXRReportBlockDuplicateRLE ) 
							- sizeof( RtcpXRChunk ) * 2 );
	
	// header 
	DupRLE ->blockType = rtcpXRBT_DuplicateRLE;
	DupRLE ->rsvd = 1;
	DupRLE ->thinning = DUP_THINING;
	DupRLE ->blockLength = 2;
	DupRLE ->SSRC = rtpReceiver ->ssrc;
	
	// determine real end_seq  and data_bits 
	data_bits = ( uint16 )( rtpReceiverXR ->end_seq - 
							rtpReceiverXR ->begin_seq );
	
	if( data_bits > MAX_NUM_XR_DUP ) {
		end_seq_real = rtpReceiverXR ->begin_seq + MAX_NUM_XR_DUP;
		data_bits = MAX_NUM_XR_DUP;
	} else {
		end_seq_real = rtpReceiverXR ->end_seq;
	}
	
	// header - seq 
	DupRLE ->begin_seq = rtpReceiverXR ->begin_seq;
	DupRLE ->end_seq = end_seq_real;
	
	// chunk data 
	chunks = RunLengthEncode( rtpReceiverXR ->dup_bits, 
								data_bits,
								DupRLE ->chunk );
	
	RtcpPkt_allocData( p, sizeof( RtcpXRChunk ) * chunks );
	DupRLE ->blockLength += chunks / 2;		

	return ( DupRLE ->blockLength + 1 ) * 4;	// +1 for header 
}

static int RtcpTx_addXR_PacketReceiptTimes(uint32 sid, RtcpPacket* p, int npadSize)
{
	// rtcpXRBT_PacketReceiptTimes = 3
	RtcpXRReportBlockReceiptTimes *RecvTimes = 
			( RtcpXRReportBlockReceiptTimes * )(RtcpPkt_freeData(p));
	RtpReceiver * const rtpReceiver = RtpRx_getInfo( sid );
	RtpReceiverXR * const rtpReceiverXR = &rtpReceiver ->xr;
	uint16 data_count;
	uint16 end_seq_real;
	uint16 i;
		
	// allocate block header
	RtcpPkt_allocData( p, sizeof( RtcpXRReportBlockReceiptTimes ) 
							- sizeof( uint32 ) );
	
	// header 
	RecvTimes ->blockType = rtcpXRBT_PacketReceiptTimes;
	RecvTimes ->rsvd = 1;
	RecvTimes ->thinning = ARRIVE_THINING;
	RecvTimes ->blockLength = 2;
	RecvTimes ->SSRC = rtpReceiver ->ssrc;
	
	// determine real end_seq  and data_bits 
	data_count = ( uint16 )( rtpReceiverXR ->end_seq - 
							rtpReceiverXR ->begin_seq );
	
	if( data_count > MAX_NUM_XR_ARRIVE ) {
		end_seq_real = rtpReceiverXR ->begin_seq + MAX_NUM_XR_ARRIVE;
		data_count = MAX_NUM_XR_ARRIVE;
	} else {
		end_seq_real = rtpReceiverXR ->end_seq;
	}
	
	// header - seq 
	RecvTimes ->begin_seq = rtpReceiverXR ->begin_seq;
	RecvTimes ->end_seq = end_seq_real;
	RecvTimes ->blockLength += ( RecvTimes ->end_seq - RecvTimes ->begin_seq );
	
	// recv times data 
	RtcpPkt_allocData( p, sizeof( uint32 ) * data_count );

	for( i = 0; i < data_count; i ++ )
		RecvTimes ->receiptTime[ i ] = rtpReceiverXR ->arrive_timestamp[ i ];
	
	return ( RecvTimes ->blockLength + 1 ) * 4;	// +1 for header 
}

static int RtcpTx_addXR_ReceiverReferenceTime(uint32 sid, RtcpPacket* p, int npadSize)
{
	// rtcpXRBT_ReceiverReferenceTime = 4
	RtcpXRReportBlockReceiverReferenceTime *RecvRefTime = 
			(RtcpXRReportBlockReceiverReferenceTime*) (RtcpPkt_freeData(p));
	int usage = RtcpPkt_allocData(p, sizeof(RtcpXRReportBlockReceiverReferenceTime));
	NtpTime nowNtp;
	
	Ntp_getTime(&nowNtp);
	
	RecvRefTime ->blockType = rtcpXRBT_ReceiverReferenceTime;	// =4 
	RecvRefTime ->reserved = 0;
	RecvRefTime ->blockLength = 2;
	RecvRefTime ->NTP_MSW = nowNtp.seconds;
	RecvRefTime ->NTP_LSW = nowNtp.fractional;
	
	return ( 1 + 2 ) * 4;	// header + payload 
}

static int RtcpTx_addXR_DLRR(uint32 sid, RtcpPacket* p, int npadSize)
{
	// rtcpXRBT_DLRR = 5
	RtcpXRReportBlockDLRR *DLRR = ( RtcpXRReportBlockDLRR * )(RtcpPkt_freeData(p));
	int i;
	int nTrans = RtcpRx_getTranInfoCount(sid);
	NtpTime nowNtp;
	uint32 midNowNtp, midTempNtp;
	
	if( !nTrans )
		return 0;
	
	// fill header 
	DLRR ->blockType = rtcpXRBT_DLRR;
	DLRR ->reserved = 0;
	DLRR ->blockLength = sizeof( RtcpXRReportBlockDLRR_sb ) / 4 * nTrans;
	
	// now NTP 
	Ntp_getTime( &nowNtp );
	midNowNtp = NTP_getMid32( &nowNtp );
	
	// allocate block header
	RtcpPkt_allocData( p, sizeof( RtcpXRReportBlockDLRR ) - sizeof( RtcpXRReportBlockDLRR_sb ) );
	
	for (i = 0; i < nTrans; i++)
	{
		RtpTranInfo * tranInfo;
		RtcpXRReportBlockDLRR_sb * const sb = &DLRR ->sb[ i ];
		
		// transaction instance 
		tranInfo = RtcpRx_getTranInfoList(sid, i);
		
		// allocate sub block  
		RtcpPkt_allocData( p, sizeof( RtcpXRReportBlockDLRR_sb ) );
		
		// fill data 
		midTempNtp = NTP_getMid32( &tranInfo ->recvLastRRTimestamp );
		
		sb ->SSRC = tranInfo ->ssrc;
		sb ->LRR = midTempNtp;
		sb ->DLRR = midNowNtp - midTempNtp;
	}
	
	return ( DLRR ->blockLength + 1 ) * 4;	// +1 for header 
}

static int RtcpTx_addXR_StatisticsSummary(uint32 sid, RtcpPacket* p, int npadSize)
{
	// rtcpXRBT_StatisticsSummary = 6
	RtcpXRReportStatisticsSummary *pSummary = ( RtcpXRReportStatisticsSummary * )(RtcpPkt_freeData(p));
	RtpReceiver * const rtpReceiver = RtpRx_getInfo( sid );
	RtpReceiverXR * const rtpReceiverXR = &rtpReceiver ->xr;
	
	// allocate block header
	RtcpPkt_allocData( p, sizeof( RtcpXRReportStatisticsSummary ) );
	
	// header 
	pSummary ->blockType = rtcpXRBT_StatisticsSummary;
	pSummary ->lossReport = 1;
	pSummary ->duplicateReport = 1;
	pSummary ->jitterReport = 1;
	pSummary ->TTLorHopLimit = 1;	// 0: none, 1: IPv4, 2: IPv6, 3: reserved 
	pSummary ->reserved = 0;
	pSummary ->blockLength = 9;
	
	// SSRC
	pSummary ->SSRC = rtpReceiver ->ssrc;
	
	// seq
	pSummary ->begin_seq = rtpReceiverXR ->begin_seq;
	pSummary ->end_seq = rtpReceiverXR ->end_seq;
	
	// summary
	RtcpXR_FillSummaryBlock( pSummary, rtpReceiverXR );
	
	return ( pSummary ->blockLength + 1 ) * 4;	// +1 for header 
}

static int RtcpTx_addXR_VoIPMetrics(uint32 sid, RtcpPacket* p, int npadSize)
{
	// rtcpXRBT_VoIPMetrics = 7
	extern int tx_jit_buf_high_threshold[MAX_SESS_NUM];
	extern uint32 GetSessionFramePeriod( uint32 ssid );
	extern int JbcRtcpXR_GetNominalDelay( uint32 ssid );
	extern uint32 JbcStatistics_GetPlayoutDelay( uint32 ssid );
	extern unsigned char MFFrameNo[MAX_SESS_NUM];
	extern int RtcpRx_getAvgRoundTripDelay(uint32 sid);
	extern uint32 chanInfo_GetChannelbySession(uint32 sid);
	extern const codec_algo_desc_t *ppNowCodecAlgorithmDesc[MAX_SESS_NUM];
	extern uint32 JbcStatistics_GetMosFactor( uint32 ssid );
	
	RtcpXRReportVoIPMetrics *pMetrics = ( RtcpXRReportVoIPMetrics * )(RtcpPkt_freeData(p));
	RtpReceiver * const rtpReceiver = RtpRx_getInfo( sid );
	RtpReceiverXR * const rtpReceiverXR = &rtpReceiver ->xr;
	const uint32 frame_period = GetSessionFramePeriod( sid );
	NtpTime nowNtp;
	uint32 delta;
	int t;
	
	uint32 chid = chanInfo_GetChannelbySession( sid );
	const codec_algo_desc_t * const pCodecAlgoDesc = ppNowCodecAlgorithmDesc[ sid ];
	//uint32 delta_seq;
	
	// allocate block header
	RtcpPkt_allocData( p, sizeof( RtcpXRReportVoIPMetrics ) );
	
	// header 
	pMetrics ->blockType = rtcpXRBT_VoIPMetrics;
	pMetrics ->reserved = 0;
	pMetrics ->blockLength = 8;

	// SSRC
	pMetrics ->SSRC = rtpReceiver ->ssrc;
	
	// packet loss and discard metrics 
	// (lossRate, discardRate, burstDensity, gapDensity, burstDuration, gapDuration)
	RtpRx_FillBurstPacketLossCalc( sid, rtpReceiverXR, pMetrics );
	
#if 0
	delta_seq = rtpReceiverXR ->end_seq - rtpReceiverXR ->global_begin_seq;

	if( delta_seq ) {
		pMetrics ->lossRate = 
			( rtpReceiverXR ->global_loss_count * 256 ) / delta_seq;
		pMetrics ->discardRate = 
			( rtpReceiverXR ->global_discard_count * 256 ) / delta_seq;
	}
#endif
	
	// delay metrics 
	pMetrics ->roundTripDelay = RtcpRx_getAvgRoundTripDelay( sid );
	
	pMetrics ->endSystemDelay = 
		PCM_PERIOD * 10 * 3 / 2 +
		1 * frame_period +
		MFFrameNo[ sid ] * frame_period +
		PCM_PERIOD * 10 * 3 / 2 +
		JbcStatistics_GetPlayoutDelay( sid ) * frame_period / 10 +
		1 * frame_period +
		5;
	
	// signal related metrics 
	if( chid < MAX_VOIP_CH_NUM && RtcpXR_SignalDetection[ chid ] ) {
		pMetrics ->signalLevel = RtcpXR_SignalLevel[ chid ] >> 8;
		pMetrics ->noiseLevel = RtcpXR_NoiseLevel[ chid ] >> 8;
	} else {
		pMetrics ->signalLevel = 127;
		pMetrics ->noiseLevel = 127;
	}

#ifdef CONFIG_RTK_VOIP_WIDEBAND_SUPPORT
	pMetrics ->RERL = 127; // TODO not support yet
#else
	pMetrics ->RERL = LEC_g168_Get_ERLE( chid ) + 12;
#endif
	
	// call quality or transmission quality metrics (not implement)
	pMetrics ->Rfactor = 127;		// ITU-T G.107 and ETSI TS 101 329-5
	pMetrics ->extRfactor = 127;	// ITU-T G.107 and ETSI TS 101 329-5 
	pMetrics ->MOS_LQ = 127;
	pMetrics ->MOS_CQ = 127;

	if( pCodecAlgoDesc ) {
		uint32 factor = JbcStatistics_GetMosFactor( sid );	// Q15 
		
		pMetrics ->MOS_LQ = ( pCodecAlgoDesc ->nMosRef * 10 ) >> 7;
		pMetrics ->MOS_CQ = pCodecAlgoDesc ->nMosRef * factor * 10 >> 22;	// Q7 * Q15 >> 22 = Q0 
		
		if( pMetrics ->MOS_CQ < 10 )	// lowest is 1.0 
			pMetrics ->MOS_CQ = 10;
		
		t = ( pMetrics ->MOS_CQ ) * 4 - 72;
		if( t < 10 )
			t = 10;
		else if( t > 100 )
			t = 100;
		pMetrics ->Rfactor = t;
		
		//printk( "pMetrics ->MOS_CQ=%d, pMetrics ->Rfactor=%d\n\n", 
		//			pMetrics ->MOS_CQ, pMetrics ->Rfactor );
	}
	
	// configuration parameters 
	pMetrics ->Gmin = XR_GMIN;		// gap threshold. ETSI TS 101 329-5 
	pMetrics ->RXconfig.PLC = 0x03;	// 11: standard, 10: enhanced, 01: disabled, 00: unspecified 
	pMetrics ->RXconfig.JBA = 0x03;	// 11: adaptive, 10: non-adaptive, 01: reserved, 00: unknown
	pMetrics ->RXconfig.JBrate = 0;	// // rate = 0 - 15, adjustment time = 2 * J * frame size (ms)
	
	// jitter buffer parameters 
	pMetrics ->JBnominal = JbcRtcpXR_GetNominalDelay( sid ) * frame_period;
	pMetrics ->JBmaximum = tx_jit_buf_high_threshold[ sid ] * frame_period;
	pMetrics ->JBabsMax = 30 * frame_period;
	
	return ( pMetrics ->blockLength + 1 ) * 4;	// +1 for header 
}

typedef int( *fnRtcpXR_ReportBlock )( uint32 sid, RtcpPacket* p, int npadSize );

typedef struct {
	fnRtcpXR_ReportBlock	fn;
	//const char *			name;
} RtcpXR_ReportBlock_t;

//#define _M_XRRB( x )	{ x, #x }
#define _M_XRRB( x )	{ x }

static const RtcpXR_ReportBlock_t RtcpXR_ReportBlock[] = {
	_M_XRRB( RtcpTx_addXR_LossRLE ),
	_M_XRRB( RtcpTx_addXR_DupRLE ),
	_M_XRRB( RtcpTx_addXR_PacketReceiptTimes ),
	_M_XRRB( RtcpTx_addXR_ReceiverReferenceTime ),
	_M_XRRB( RtcpTx_addXR_DLRR ),
	_M_XRRB( RtcpTx_addXR_StatisticsSummary ),
	_M_XRRB( RtcpTx_addXR_VoIPMetrics ),
};

#undef _M_XRRB

#define KINDS_OF_XR_REPORT_BLOCK		( sizeof( RtcpXR_ReportBlock ) / sizeof( RtcpXR_ReportBlock[ 0 ] ) )


int RtcpTx_addXR (uint32 sid, RtcpPacket* p, int npadSize)
{
	// header
	RtcpXRPacketHeader* header = (RtcpXRPacketHeader*) (RtcpPkt_freeData(p));
	int usage = RtcpPkt_allocData(p, sizeof(RtcpXRPacketHeader));
	RtcpTransmitter *pInfo;
	NtpTime nowNtp, thenNtp;
	int i;
	
	if(sid > SESS_NUM)
		return -1;
	pInfo = &RtcpTxInfo[sid];
	
	header->version = RTP_VERSION;
	header->padding = (npadSize > 0) ? 1 : 0;
	header->reserved = 0;
	header->type = rtcpTypeXR;		// =207 
	header->length = 0;	// modify later 
	header->SSRC = pInfo->tran->ssrc;
	
	// add XR report block 
	for( i = 0; i < KINDS_OF_XR_REPORT_BLOCK; i ++ )
		usage += ( *RtcpXR_ReportBlock[ i ].fn )( sid, p, npadSize );
	
	// reset statstics 
	RtpRx_ResetPeriodicXR( &RtpRx_getInfo( sid ) ->xr );
	
	// padding
	if (npadSize > 0)
	{
		// future: not implemented
		assert (0);
	}
	
	// overall packet must ends on 32-bit count
	assert (usage % 4 == 0);

#if 0
	header->length = htons((usage / 4) - 1);
#else
	header->length = (usage / 4) - 1;
#endif
	//cpLog (LOG_DEBUG_STACK, "RTCP:  SR/RR packet used %d bytes/ %d words",
	//       usage, usage/4);
	return usage;
}

