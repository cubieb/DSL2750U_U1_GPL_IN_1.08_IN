
//static const char* const RtpTransmitter_cxx_Version =
//    "$Id: RtpTransmitter.c,v 1.4 2011/06/13 14:49:06 cathy Exp $";

//#include <time.h>
//#include <sys/types.h>
//#include <unistd.h>
//#include <string.h>

#include <linux/string.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#ifdef DEBUG_LOG
#include "cpLog.h"
#endif
/* Kao
#include "vsock.hxx"
*/

#include "assert.h"
#include "rtpTypes.h"
#include "rtpTools.h"
#include "Rtp.h"
#include "rtk_voip.h"
//#include <debug.h>

#include "codec_descriptor.h"

#include "v152_api.h"

///////////////////////////////////////////////////////////////////////

char rfc2833_period[MAX_VOIP_CH_NUM]={0};        // hc+ for voice packet interleave in rfc2833 period issue
unsigned int gRfc2833_volume[MAX_SESS_NUM] = {[0 ... MAX_SESS_NUM-1] = 0x0A};
static int transmit(RtpPacket*, BOOL);


static int cur_send_dtmf;
static int cur_send;
static BOOL bCreate;

extern int voip_ch_num;

#ifdef SUPPORT_MULTI_FRAME
extern unsigned char MFFrameNo[MAX_SESS_NUM];
#endif

unsigned char rfc2833_payload_type_remote[MAX_SESS_NUM];    /* 0: Don't send 2833 packets. 96 or 101 or ..: send 2833 packets */
static int send_2833_start[MAX_SESS_NUM]={0};              /* 0: stop send 2833 packets   1: start  send 2833 packets */
static int send_marker_flag[MAX_SESS_NUM] = {0};           /* 0: not send. 1: send */
static int send_2833_flag[MAX_SESS_NUM] = {0};             /* 0: not send. 1: send */
static int send_edge_flag[MAX_SESS_NUM] = {0};             /* 0: not send. 1: send */
static int timestamp_2833[MAX_SESS_NUM] = {0};
static unsigned short edge_sequence_2833[MAX_SESS_NUM] = {0};
int send_2833_by_ap[MAX_VOIP_CH_NUM] = {0};		/* 0: by DSP 1: by AP */	
extern int sned_flag[];
extern char dtmf_removal[];
extern int g_dynamic_pt_remote[];
extern int g_dynamic_pt_remote_vbd[];


#if defined( SUPPORT_RFC_2833 ) && defined( SEND_RFC2833_ISR )
unsigned char send_2833_count_down[MAX_VOIP_CH_NUM];
#endif

#ifdef SUPPORT_RFC2833_PLAY_LIMIT
extern int bRfc2833_play_limit[MAX_SESS_NUM]; // flag to turn on/off play time limit
extern int rfc2833_play_limit_ms[MAX_SESS_NUM];
#endif

RtpTransmitter RtpTxInfo[MAX_SESS_NUM];

#ifdef SUPPORT_RTP_REDUNDANT
typedef struct {
	int set;
	unsigned char data[ PKTDATA_SIZES ];
	int len;
	RtpTime timestamp;
	RtpPayloadType PT;
} RtpRedundantTx_t;

static RtpRedundantTx_t RtpRedundantTx[ SESS_NUM ];
#endif

/* ----------------------------------------------------------------- */
/* --- RtpTransmitter Constructor ---------------------------------- */
/* ----------------------------------------------------------------- */

void RtpTx_Init (void)
{
	int i;
	// set private variables
	cur_send = 0;
	cur_send_dtmf = 0;
	bCreate = FALSE;
	for(i=0; i<SESS_NUM; i++)
	{
		RtpTx_InitbyID(i);
		rfc2833_payload_type_remote[i] = 101; /* Init payload type to 101 */
	}
}

void RtpTx_InitbyID (uint32 sid)
{
	RtpTransmitter* pInfo = NULL;

	if(sid >= SESS_NUM)
		return;

	pInfo = &RtpTxInfo[sid];

	pInfo->ssrc = generateSRC();
/*	pInfo->seedNtpTime = Ntp_getTime(); */
#ifdef SUPPORT_RTCP
	Ntp_getTime(&pInfo->seedNtpTime);
        pInfo->prevNtpTime = pInfo->seedNtpTime;
#endif
	pInfo->seedRtpTime = generate32();
	pInfo->prevRtpTime = pInfo->seedRtpTime;
	pInfo->prevSequence = generate32();
	pInfo->markerOnce = TRUE;

	pInfo->tranOpmode = rtptran_droppacket;

	// set counters
	pInfo->packetSent = 0;
	pInfo->payloadSent = 0;

#ifdef SUPPORT_RTP_REDUNDANT	
	RtpRedundantTx[ sid ].set = 0;
#endif
}

void RtpTx_renewSession (uint32 sid)
{
	RtpTransmitter* pInfo = NULL;

	if(sid >= SESS_NUM)
		return;

	pInfo = &RtpTxInfo[sid];

	pInfo->ssrc = generateSRC();
	pInfo->seedRtpTime = generate32();
	pInfo->prevRtpTime = pInfo->seedRtpTime;
	pInfo->prevSequence = generate32();
#ifdef CONFIG_RTK_VOIP_SRTP
	// Avoid losing ROC synchronization
	// Initial seguence number shall be less than 2^15
	pInfo->prevSequence &= 0x7FFF;
#endif	
	pInfo->markerOnce = TRUE;

	// set counters
	pInfo->packetSent = 0;
	pInfo->payloadSent = 0;
	
#ifdef SUPPORT_RTP_REDUNDANT
	RtpRedundantTx[ sid ].set = 0;
#endif
}

#if 0
void setRemoteAddr (const NetworkAddress& theAddr)
{
	remoteAddr = theAddr;
}
#endif

RtpTransmitter* RtpTx_getInfo (uint32 sid)
{
    RtpTransmitter* pInfo = NULL;

	if(sid >= SESS_NUM)
		return NULL;

	pInfo = &RtpTxInfo[sid];
	return pInfo;
}

/* --- send packet functions --------------------------------------- */

static RtpPacket* createPacket (uint32 chid, uint32 sid, int npadSize, int csrc_count)
{
	RtpTransmitter* pInfo = NULL;

	RtpPacket* packet;
	// create packet
	if(bCreate)
	{
		bCreate = FALSE;
		return NULL;
	}
	bCreate = TRUE;

	if(sid >= SESS_NUM)
	{
		bCreate = FALSE;
		return NULL;
	}

	pInfo = &RtpTxInfo[sid];

	packet = &RTP_TX_DTMF[cur_send_dtmf];
	if(packet->own == OWNBYDSP)
	{
		bCreate = FALSE;
    	return NULL;
	}
	cur_send_dtmf++;
	cur_send_dtmf &= (RTP_TX_DTMF_NUM-1);
	RtpPacket_Init(packet, RECV_BUF/*pInfo->payloadSize*/, npadSize, csrc_count);
	assert (packet);
	// load packet
	setSSRC (packet, pInfo->ssrc);
	setPayloadType (packet, pInfo->payloadFormat/*apiFormat*/);
	packet->chid = chid;
	packet->sid = sid;
	
	packet->own = OWNBYDSP;
	bCreate = FALSE;

    return packet;
}


// takes api RTP packet and send to network
// assumes payload size is already set
static int transmit(RtpPacket* packet, BOOL eventFlag )
{
	RtpTransmitter* pInfo = NULL;
	if( !packet )
	{
#ifdef DEBUG_LOG
		cpLog(LOG_ERR,"Attempting to transmit a NULL rtp packet");
#endif
		return -1;
	}

	RtpPacket* p = packet;

	if(p->sid >= SESS_NUM)
		return -1;

	pInfo = &RtpTxInfo[p->sid];

	if( !p->timestampSet )
	{
		setRtpTime( p, pInfo->prevRtpTime + pInfo->pktSampleSize/*network_pktSampleSize*/ );
		
		if (true == p->EventPktMarker)
		{
			timestamp_2833[p->sid] = pInfo->prevRtpTime + pInfo->pktSampleSize;
			//printk("tx: %u, sid=%d\n", timestamp_2833[p->sid], p->sid);
		}
	}
	
	if (eventFlag)
		setRtpTime( p, timestamp_2833[p->sid] );


	if( (!p->sequenceSet) )
	{
		if (eventFlag == 1)
		{
			setSequence( p, pInfo->prevSequence++ );
			
			if (p->EventPktEdge == true)
			{
				edge_sequence_2833[p->sid] = pInfo->prevSequence - 1;
			}
		}
		else
			setSequence( p, pInfo->prevSequence++ );
	}
	else
	{
		if ( (eventFlag == 1) && (p->EventPktEdge == true) )
		{
			setSequence( p, edge_sequence_2833[p->sid]);
		}
	}
		
	if( getPayloadUsage(p) < 0  ||  getPayloadUsage(p) > 1012 )
	{
#ifdef DEBUG_LOG
		cpLog(LOG_DEBUG_STACK,"Invalid data packet size %d", getPayloadUsage());
#endif
		return -1;
	}

    //set marker once
	if( pInfo->markerOnce )
	{
#ifdef DEBUG_LOG
		cpLog( LOG_DEBUG_STACK,"Setting marker bit once");
#endif
		setMarkerFlag(p, 1);
		pInfo->markerOnce = FALSE;
	}

	// for packet reuse
	p->timestampSet = FALSE;
	p->sequenceSet = FALSE;
	// transmit packet
	DSP_rtpWrite(p);

	// update counters
	pInfo->packetSent++;
	//prevSequence = getSequence(p);
	if( !eventFlag )
	{
		pInfo->payloadSent += getPayloadUsage(p);
#ifdef SUPPORT_RTCP
		Ntp_getTime(&pInfo->prevNtpTime);
#endif
		pInfo->prevRtpTime = getRtpTime(p);
	}

	// set up return value
	int result = getPayloadUsage(p);

	// exit with success
	return result;
}

#ifdef SUPPORT_RTP_REDUNDANT
static int RtpRedundantTransmitProcess( uint32 sid, char* data, int len,
								 RtpTransmitter* pInfo,
								 RtpPacket* p )
{
	// goal of this function is to: 
	//	setPayloadUsage (p, len/*packetSize*/);
	//	memcpy (getPayloadLoc(p), data, len);
	
	RtpRedundantTx_t * const pRtpRedTx = &RtpRedundantTx[ sid ];
	int nTotalPayloadLen;	// redundant header + red/primary data
	unsigned char *pDst;
	const RtpTime curTimestamp = pInfo->prevRtpTime + pInfo->pktSampleSize;

	if( !GetRtpRedundantStatus( sid ) )
		return 0;
	
	// fill packet to transmit 
	if( pRtpRedTx ->set ) {
		nTotalPayloadLen = sizeof( RtpRedHeader ) + 
							sizeof( RtpRedPrimaryHeader ) + 
							pRtpRedTx ->len + len;
	} else {
		nTotalPayloadLen = sizeof( RtpRedPrimaryHeader ) + len;
	}
	
	setPayloadUsage (p, nTotalPayloadLen/*packetSize*/);
	
	pDst = getPayloadLoc(p);
	
	if( pRtpRedTx ->set ) {	// fill redundant header 
		( ( RtpRedHeader * )pDst ) ->F = 1;
		( ( RtpRedHeader * )pDst ) ->blockPT = pRtpRedTx ->PT;
		( ( RtpRedHeader * )pDst ) ->timestamp_offset = curTimestamp - pRtpRedTx ->timestamp;
		( ( RtpRedHeader * )pDst ) ->block_length = pRtpRedTx ->len;
		pDst += sizeof( RtpRedHeader );
	}
	
	( ( RtpRedPrimaryHeader * )pDst ) ->F = 0;	// fill primary redundant header 
	( ( RtpRedPrimaryHeader * )pDst ) ->blockPT = getPayloadType( p );
	pDst += sizeof( RtpRedPrimaryHeader );
	
	if( pRtpRedTx ->set ) {	// fill redundant data 
		memcpy( pDst, pRtpRedTx ->data, pRtpRedTx ->len );
		pDst += pRtpRedTx ->len;
	}
	
	memcpy( pDst, data, len );	// fill primary data 
	pDst += len;	
	
	// save current for next redundant 
	pRtpRedTx ->set = 1;
	memcpy( pRtpRedTx ->data, data, len );
	pRtpRedTx ->len = len;
	pRtpRedTx ->timestamp = curTimestamp;
	pRtpRedTx ->PT = getPayloadType( p );

	return 1;
}
#endif

// takes rawdata, buffers it, and send network packets
int RtpTx_transmitRaw (uint32 chid, uint32 sid, char* data, int len)
{
	RtpTransmitter* pInfo = NULL;
	// send out packets from buffer
	int result = 0;

	extern uint32 rtpConfigOK[];
#ifdef SUPPORT_COMFORT_NOISE
        extern int m_nSIDFrameLen[MAX_SESS_NUM];                                            // the length of SID frame
#endif
	extern uint16 SID_payload_type_remote[ SESS_NUM ];
	extern uint32 SID_count_tx[ SESS_NUM ];
	// create packet
	RtpPacket* p = &RTP_TX_DEC[cur_send];

	if(chid >= voip_ch_num)
		return 0;
	
	if(sid >= SESS_NUM)
		return 0;

	if(rtpConfigOK[sid] == 0)
		return 0;

	if(p->own == OWNBYDSP)
		return 0;

	if(!isTranMode(sid))
		return 0;

	pInfo = &RtpTxInfo[sid];

	cur_send++;
	cur_send &= (RTP_TX_DEC_NUM-1);
	RtpPacket_Init(p, RECV_BUF/*pInfo->payloadSize*/, 0, 0);
	p->chid = chid;
	p->sid = sid;
	assert (p);
	setSSRC (p, pInfo->ssrc);
#ifdef SUPPORT_COMFORT_NOISE
	if ((len == m_nSIDFrameLen[sid]) && (g_dynamic_pt_remote[sid] != rtpPayload_AMR_NB) && (g_dynamic_pt_remote[sid] != rtpPayloadG729))
	{
		if( SID_payload_type_remote[ sid ] )
			setPayloadType( p, SID_payload_type_remote[ sid ] );
		else
			setPayloadType (p, 13);
		
		SID_count_tx[ sid ] ++;
	} else
#endif
	{
		RtpPayloadType type;

#ifdef SUPPORT_V152_VBD		
		if( V152_CheckIfSendVBD( sid ) )
			type = g_dynamic_pt_remote_vbd[sid];
		else
#endif
		{
			type = g_dynamic_pt_remote[sid];
		}
			
		setPayloadType (p, type); // support dynamic payload.
		//setPayloadType (p, pInfo->payloadFormat/*networkFormat*/);
	}

#ifdef SUPPORT_RTP_REDUNDANT
	if( RtpRedundantTransmitProcess( sid, data, len, pInfo, p ) ) {
		/* overwrite payload type with redundant one */
		setPayloadType (p, GetRtpRedundantPayloadType( sid, 1 /* local */ ) );
	} else
#endif
	{
		setPayloadUsage (p, len/*packetSize*/);
	
		memcpy (getPayloadLoc(p), data, len);
	}
	p->own = OWNBYDSP;
	result += transmit(p, FALSE);
	p->own = OWNBYRTP;
	return result;
}

#ifdef SUPPORT_RFC_2833
int RtpTx_transmitEvent( uint32 chid, uint32 sid, int event, int delay_ms)
{
        extern char dtmf_mode[MAX_VOIP_CH_NUM]/*_inband*/ ;
	int num,i;
	unsigned short n = 160;
	RtpTransmitter* pInfo = NULL;

        if(chid >= voip_ch_num)
		return 0;

	if(sid >= SESS_NUM)
		return 0;
		
	pInfo = &RtpTxInfo[sid];

    // Howard. 2004.12.30 for prevent to send dtmf digit using Outband when user enables the Inband
    if( dtmf_mode[chid]/*_inband*/ != 0 )	// Howard. 2005.2.24 when dtmf_mode is not RFC2833 , then we can't generate RFC2833 packet, so we return here.
		return ( -1 ) ;

	rfc2833_period[chid] = 1;	// hc+ for avoid voice packet interleave issue

	//sessionError = session_success;
	RtpPacket* eventPacket = createPacket(chid, sid, 0, 0);
	// Howard. 2005.3.16
	setPayloadType( eventPacket, rfc2833_payload_type_remote[sid]);
	setPayloadUsage( eventPacket, sizeof( RtpEventDTMFRFC2833 ) );
	setMarkerFlag(eventPacket, 1);
	RtpEventDTMFRFC2833* eventPayload = (RtpEventDTMFRFC2833*)( getPayloadLoc(eventPacket) );

	// reset event payload
	eventPayload->event = event; 
	eventPayload->volume = gRfc2833_volume[sid];
	eventPayload->reserved = 0;
	eventPayload->edge = 0;
	eventPayload->duration = n;	//htons(n);

	// send onedge packet
	// jimmylin - let's send it for delay_ms
	num = (delay_ms/20) - 2;
	//Howard. 2005.3.16 to transmit 3 times first Digit ( Marker bit set to 1 )
	for( i = 0 ; i < 3 ; i++ )
	{
		eventPacket->timestampSet = true ;
		transmit( eventPacket, true ) ;
		eventPacket->sequenceSet = true ;
	}
	setMarkerFlag(eventPacket,0);
	udelay(20);

	for(i=0; i<num; i++)	
	{
		n += 160;
		eventPayload->duration = n;	//htons(n);
		eventPacket->timestampSet = true;
		transmit( eventPacket, true );
		udelay(20);
	}

	// send on packet
	eventPayload->edge = 1;
	// jimmylin - retransmit 3 times according to Rfc2833
	for(i=0; i<3; i++)
	{
		eventPacket->timestampSet = true;
		transmit( eventPacket, true );
		eventPacket->sequenceSet = true;
	}
	pInfo->prevRtpTime += 640;	// hc+ 1101 timestamp issue

	rfc2833_period[chid] = 0;	// hc+ for avoid voice packet interleave issue
	
	eventPacket->own = OWNBYRTP;
	return 0;
}


static unsigned short duration[SESS_NUM] = {0};

#define EVENT_PKT_FIFO_NUM 10
static RtpPacket eventPkt_fifo[MAX_VOIP_CH_NUM][EVENT_PKT_FIFO_NUM]={{{0}}};
static int eventPkt_fifo_r[MAX_VOIP_CH_NUM]={0}, eventPkt_fifo_w[MAX_VOIP_CH_NUM]={0};

static int eventPkt_fifoWrite(int CH, RtpPacket *eventPkt)
{
	RtpPacket* p;
	
	if(((eventPkt_fifo_w[CH]+1)%EVENT_PKT_FIFO_NUM) == eventPkt_fifo_r[CH])
	{
		printk("RTP Event PKT FIFO Full\n");
		return 0;
	}
		
	p = &eventPkt_fifo[CH][eventPkt_fifo_w[CH]];
	memcpy(p, eventPkt, sizeof(RtpPacket));
	p->packetData = p->packetbuf;
	p->header = (RtpHeader*) (char*)(p->packetData);
	eventPkt_fifo_w[CH] = (eventPkt_fifo_w[CH] + 1)%EVENT_PKT_FIFO_NUM;
	//printk("w[%d]=%d\n", CH, eventPkt_fifo_w[CH]);
		
	return 1;
}

static int eventPkt_fifoRead(int CH, RtpPacket** eventPacket)
{

	if (eventPkt_fifo_r[CH] == eventPkt_fifo_w[CH])
	{
		//printk("RTP Event PKT FIFO Empty\n");
		//printk(".");
		return 0;
	}

	*eventPacket = &eventPkt_fifo[CH][eventPkt_fifo_r[CH]];
	
	return 1; 
	
}

static void eventPkt_fifoReadDone(int CH)
{
	unsigned long flags;

	save_flags(flags); cli();
	eventPkt_fifo_r[CH] = (eventPkt_fifo_r[CH]+1)%EVENT_PKT_FIFO_NUM;
	restore_flags(flags);
	// printk("r[%d]=%d\n", CH, eventPkt_fifo_r[CH]);
}

struct tasklet_struct	event_send_tasklet;

void event_send(unsigned long *dummy)
{
	unsigned char chid;
	RtpPacket* eventPacket = NULL;
	
	for(chid=0; chid < VOIP_CH_NUM; chid++)
	{
		while(eventPkt_fifoRead(chid, &eventPacket))
		{
			transmit(eventPacket, true);
			eventPkt_fifoReadDone(chid);
		}
	}
}

int RtpTx_transmitEvent_ISR( uint32 chid, uint32 sid, int event)
{
	
	int i;
	RtpTransmitter* pInfo = NULL;
		
	if(chid >= voip_ch_num)
	{
		sned_flag[chid] = 0;
		return 0;
	}

	if(sid >= SESS_NUM)
	{
		sned_flag[chid] = 0;
		return 0;
	}
	
	if(!isTranMode(sid))
		return 0;
		
	if (rfc2833_payload_type_remote[sid] == 0) /* 0: Don't send 2833 packets. */
	{
		sned_flag[chid] = 0;
		return 0;
	}

	if (!send_2833_start[sid])
	{
  		if(((send_2833_count_down[ chid ] != 0) && (send_2833_by_ap[chid] == 1) ) ||
  			((dtmf_removal[chid] == 1) && (send_2833_by_ap[chid] == 0))) 
		{
			send_2833_start[sid] = 1;
			send_marker_flag[sid] = 1;
			//printk("s\n");	
		}
		else
		{
			sned_flag[chid] = 0;
			return 0;
		}
	}
	else
	{
  		if(((send_2833_count_down[ chid ] == 0) && (send_2833_by_ap[chid] == 1) ) ||
  			((dtmf_removal[chid] == 0) && (send_2833_by_ap[chid] == 0))) 
		{
			send_edge_flag[sid] = 1;
			send_2833_flag[sid] = 0;
			//printk("e\n");	
		}
		else
			send_2833_flag[sid] = 1;
	}


	pInfo = &RtpTxInfo[sid];


	RtpPacket* eventPacket = createPacket(chid, sid, 0, 0);
	setPayloadType( eventPacket, rfc2833_payload_type_remote[sid]);
	setPayloadUsage( eventPacket, sizeof( RtpEventDTMFRFC2833 ) );
	RtpEventDTMFRFC2833* eventPayload = (RtpEventDTMFRFC2833*)( getPayloadLoc(eventPacket) );

	// reset event payload
	eventPayload->event = event; 
	eventPayload->volume = gRfc2833_volume[sid];
	eventPayload->reserved = 0;
	eventPayload->edge = 0;
	eventPayload->duration = duration[sid];
	setMarkerFlag(eventPacket,0);
	eventPacket->EventPktMarker = false;
	eventPacket->EventPktBody = false;
	eventPacket->EventPktEdge = false;
	

	/* Send Marker Packet */
	if (send_marker_flag[sid] == 1)
	{
		duration[sid] = 0;
		setMarkerFlag(eventPacket, 1);
		eventPayload->duration = duration[sid];
		eventPacket->timestampSet = false ;
		eventPacket->sequenceSet = false ;
		eventPacket->EventPktMarker = true;
		//transmit( eventPacket, true ) ;
		eventPkt_fifoWrite(chid, eventPacket);
		
		send_marker_flag[sid] = 0;
		//printk("ss\n");	
	}

#ifdef SUPPORT_RFC2833_PLAY_LIMIT
	if (bRfc2833_play_limit[sid] == 1)
	{
		if (duration[sid] >= (rfc2833_play_limit_ms[sid]*8 - 80*PCM_PERIOD))
		{
			send_edge_flag[sid] = 1;
			send_2833_flag[sid] = 0;
			//printk("==> stop 2833 event gen, sid=%d\n", sid);
		}	
	}		
#endif


	/* Send 2833 Event Packet */
	if (send_2833_flag[sid] == 1)
	{
		duration[sid] += 80*PCM_PERIOD;
		eventPayload->duration = duration[sid];
		eventPacket->timestampSet = true;
		eventPacket->sequenceSet = false ;
		eventPacket->EventPktBody = true;
		//transmit( eventPacket, true );
		eventPkt_fifoWrite(chid, eventPacket);
		//printk("st\n");	

	}
	

	/* Send Edge Packet */
	if (send_edge_flag[sid] == 1)
	{
		eventPayload->edge = 1;
		duration[sid] += 80*PCM_PERIOD;
		eventPayload->duration = duration[sid];
		
		/*** send three the same sequence number edge packets ***/
		
		eventPacket->timestampSet = true;
		eventPacket->sequenceSet = false;
		eventPacket->EventPktEdge = true;
		//transmit( eventPacket, true );
		eventPkt_fifoWrite(chid, eventPacket);
		
		send_edge_flag[sid] = 0;	/* Must be set to 0 after send first edge packets */
		
		for(i=0; i<2; i++)
		{
			eventPacket->timestampSet = true;
			eventPacket->sequenceSet = true;
			eventPacket->EventPktEdge = true;
			//transmit( eventPacket, true );
			eventPkt_fifoWrite(chid, eventPacket);
		}
		
		send_2833_start[sid] = 0;
		sned_flag[chid] = 0;
		//printk("se\n");	
	}
	
	tasklet_hi_schedule(&event_send_tasklet);
	eventPacket->own = OWNBYRTP;

  	if (send_2833_by_ap[chid] == 1)
  	{
		if( send_2833_count_down[ chid ] ) 
			send_2833_count_down[ chid ] --;	/* dec. 2833 count */
	}
	
	return 1;
}
#endif


void RtpTx_setFormat (uint32 sid, RtpPayloadType newtype, int frameRate)
{
	const codec_payload_desc_t *pCodecPayloadDesc;
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return;

	pInfo = &RtpTxInfo[sid];

	pInfo->payloadFormat = newtype;

	pCodecPayloadDesc = GetCodecPayloadDesc( newtype );
	
	if( pCodecPayloadDesc ) { 
		pInfo->bitRate = 8000;
		pInfo->payloadSize = 
			_imul32(pCodecPayloadDesc ->nFrameBytes, 
					_idiv32(frameRate, pCodecPayloadDesc ->nTranFrameRate));
	#ifdef SUPPORT_MULTI_FRAME
		pInfo->pktSampleSize = pCodecPayloadDesc ->nFrameTimestamp * MFFrameNo[sid];
	#else
		pInfo->pktSampleSize = pCodecPayloadDesc ->nFrameTimestamp;
	#endif		
	} else {
		pInfo->bitRate = 8000;
		pInfo->payloadSize = 160;
		pInfo->pktSampleSize = 160;
	}
}

void RtpTx_addTimestamp (uint32 sid)
{
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return;
	
	pInfo = &RtpTxInfo[sid];
    pInfo->prevRtpTime += pInfo->pktSampleSize;
}

void RtpTx_addTimestampOfOneFrame (uint32 sid)
{
	/*
	 * During silence period, 'Not Tx' is generated in period of one frame.
	 */
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return;

	pInfo = &RtpTxInfo[sid];
#if 0
	// Satisfy with G711 and G729 only.
    	 pInfo->prevRtpTime += 80;		// 10ms
#else
 #ifdef SUPPORT_MULTI_FRAME
     pInfo->prevRtpTime += ( pInfo->pktSampleSize / MFFrameNo[sid] );
 #else
	 pInfo->prevRtpTime += pInfo->pktSampleSize;
 #endif
#endif
}

void RtpTx_subTimestamp(uint32 sid)
{
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return;
	
	pInfo = &RtpTxInfo[sid];
    pInfo->prevRtpTime -= pInfo->pktSampleSize;
}

 #ifdef CONFIG_RTK_VOIP_G729AB
uint32 G729GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/* case rtpPayloadG729:	// G729 */
	return ( nSize + 9 ) / 10;	// G729 length: 10 bytes
}
 #endif /* CONFIG_RTK_VOIP_G729AB */

uint32 G711GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/*
	case rtpPayloadPCMU:	// G711
	case rtpPayloadPCMA:
	*/
	return ( nSize + 79 ) / 80;	// G711 length: 80 bytes
}

 #ifdef CONFIG_RTK_VOIP_G722
uint32 G722GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{		
	return ( nSize + 79 ) / 80;	// G722 length: 80 bytes;
}
 #endif /* CONFIG_RTK_VOIP_G722 */

 #ifdef CONFIG_RTK_VOIP_G7231
uint32 G723GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{		
	/* case rtpPayloadG723:	// G723 */
	if( pBuffer[ 0 ] & 0x02 ) 		// VAD FLAG --> SID 
		return 1;
	else if( pBuffer[ 0 ] & 0x01 )	// RATE FLAG 
		return ( nSize + 19 ) / 20;	// G723 5.3k
	else
		return ( nSize + 23 ) / 24;	// G723 6.3k
}
 #endif /* CONFIG_RTK_VOIP_G7231 */

 #ifdef CONFIG_RTK_VOIP_G726
uint32 G726_16GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/* case rtpPayloadG726_16:	// G726 16k */
	return ( nSize + 19 ) / 20;	// G726-16k length: 20 bytes
}

uint32 G726_24GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/* case rtpPayloadG726_24:	// G726 24k */
	return ( nSize + 29 ) / 30;	// G726-24k length: 30 bytes
}

uint32 G726_32GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/* case rtpPayloadG726_32:	// G726 32k */
	return ( nSize + 39 ) / 40;	// G726-32k length: 40 bytes
}

uint32 G726_40GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/* case rtpPayloadG726_40:	// G726 40k */
	return ( nSize + 49 ) / 50;	// G726-40k length: 50 bytes
}
 #endif /* CONFIG_RTK_VOIP_G726 */

 #ifdef CONFIG_RTK_VOIP_GSMFR
uint32 GSMfrGetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/* case rtpPayloadGSM: */
	return ( nSize + 32 ) / 33;
}
 #endif /* CONFIG_RTK_VOIP_GSMFR */
		
 #ifdef CONFIG_RTK_VOIP_ILBC
uint32 iLBC30msGetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/* case rtpPayloadiLBC30ms: */
	return ( nSize + 49 ) / 50;
}
uint32 iLBC20msGetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/* case rtpPayloadiLBC20ms: */
	return ( nSize + 37 ) / 38;
}
 #endif /* CONFIG_RTK_VOIP_ILBC */
 
  #ifdef CONFIG_RTK_VOIP_AMR_NB
uint32 AMR_NB_GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{		
	// @@-amr
	//return ( nSize + 33 ) / 34;	// 12.2K
	return 1;
}
 #endif /* CONFIG_RTK_VOIP_AMR_NB */
	
 #ifdef CONFIG_RTK_VOIP_T38
uint32 T38GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	/* case rtpPayloadT38_Virtual: */
	/* This is a dummy function */
	return 1;
}
 #endif /* CONFIG_RTK_VOIP_T38 */
		
  #ifdef CONFIG_RTK_VOIP_SPEEX_NB
uint32 SPEEX_NB_GetTxNumberOfFrame( uint32 nSize, const char *pBuffer )
{
	// @@-speex nb

	return 1;
}
 #endif /* CONFIG_RTK_VOIP_SPEEX_NB */
		
void RtpTx_subTimestampIfNecessary( uint32 sid, char *pBuf, int32 size )
{
	/*
	 * In original design, we assume that every packet contains a
	 * fixed amount of frames, so (80 * framePerPkt) is always added 
	 * to time stamp.
	 * In newer design, subtract time stamp in two cases:
	 *  1) a packet may contain various frames, which 
	 *     could combine voice and SID frames.
	 *     PS. G729 only
	 *  2) While the last frame of a packet is SID, the packet may not  
	 *     satisfy number of frame per packet. 
	 */

	const codec_payload_desc_t *pCodecPayloadDesc;
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return;

	pInfo = &RtpTxInfo[sid];


	int32 nNumOfFrame;
	uint32 nDeltaOfTimestamp;
	uint32 nFrameTimestamp;		// frame interval

	// consider SID is a part of packet
	pCodecPayloadDesc = GetCodecPayloadDesc( pInfo->payloadFormat );
	
	if( pCodecPayloadDesc ) {
		nNumOfFrame = ( *pCodecPayloadDesc ->fnGetTxNumberOfFrame )( size, pBuf );
		nFrameTimestamp = pCodecPayloadDesc ->nFrameTimestamp;
	} else {
		printk( "ST " );
		return;
	}

	// ok. a normal packet
	if( nNumOfFrame == MFFrameNo[sid] )
		return;
			
	nDeltaOfTimestamp = ( MFFrameNo[sid] - nNumOfFrame ) * nFrameTimestamp;
	
	pInfo->prevRtpTime -= nDeltaOfTimestamp;
}

void RtpTx_setMarkerOnce(uint32 sid)
{
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return;

	pInfo = &RtpTxInfo[sid];
	pInfo->markerOnce = TRUE;
}

RtpTime RtpTx_getPrevRtpTime (uint32 sid)
{
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return 0;

	pInfo = &RtpTxInfo[sid];
	return pInfo->prevRtpTime;
}

RtpSrc RtpTx_getSSRC (uint32 sid)
{
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return 0;

	pInfo = &RtpTxInfo[sid];
	return pInfo->ssrc;
}

///
int RtpTx_getPacketSent (uint32 sid)
{
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return 0;

	pInfo = &RtpTxInfo[sid];
	return pInfo->packetSent;
}
///
int RtpTx_getPayloadSent (uint32 sid)
{
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return 0;

	pInfo = &RtpTxInfo[sid];
	return pInfo->payloadSent;
}

void RtpTx_setMode(uint32 sid, RtpTransmitMode opmode)
{
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return;

	pInfo = &RtpTxInfo[sid];
	pInfo->tranOpmode = opmode;
}

int32 isTranMode(uint32 sid)
{
	RtpTransmitter* pInfo = NULL;
	if(sid >= SESS_NUM)	// Sandro+
		return 0;

	pInfo = &RtpTxInfo[sid];
	if(pInfo->tranOpmode == rtptran_droppacket)
		return 0;
	return 1;
}

