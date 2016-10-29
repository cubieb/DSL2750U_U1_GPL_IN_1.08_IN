
//
// rtpterm.cpp
//

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/string.h>
//#include <string.h>
#include "rtpterm.h"
#include "rtpTypes.h"
#include "Rtp.h"
#include "../dsp_r0/dspcodec_0.h"
#include "../dsp_r0/dspparam.h"
//#include "Slic_api.h"
#include "snd_define.h"
#include "voip_control.h"
#include "rtk_voip.h"
#include "voip_init.h"
#include "voip_proc.h"

#if defined (PULSE_DIAL_GEN) && defined (OUTBAND_AUTO_PULSE_DIAL_GEN)
//#include "Daa_api.h"
#include "snd_mux_daa.h"
extern int pulse_dial_in_dch(uint32 ch_id, char input);
#endif

//#include <debug.h>

#include "codec_descriptor.h"

///////////////////////////////////////////////////////////////////
// global variable
int m_nSIDFrameLen[MAX_SESS_NUM];                                           // the length of SID frame
RtpSessionState sessionState[MAX_SESS_NUM];
RtpSessionError sessionError[MAX_SESS_NUM];
#ifdef SUPPORT_RTCP
unsigned char RtcpOpen[MAX_SESS_NUM] = {0}; /* if RtcpOpen[sid] =1, it means Rtcp session sid if open. else if 0, close.*/
#endif
unsigned char RtpOpen[MAX_SESS_NUM] = {0}; /* if RtpOpen[sid] =1, it means Rtp session sid if open. else if 0, close.*/
///////////////////////////////////////////////////////////////////
// static variable
//static CRtpConfig m_xConfig[MAX_SESS_NUM];		// the configuration
CRtpConfig m_xConfig[MAX_SESS_NUM];
static int m_nFrameLen[MAX_SESS_NUM];	// always 0. temporal modification		// the length of frames
static int m_nFrameNum[MAX_SESS_NUM];	// always 0. temporal modification		// the num of frames
static int m_nFramePerPacket[MAX_SESS_NUM];	// always 1. // the max num of frames packed into a packet
/*static int m_nSIDFrameLen[MAX_SESS_NUM];  */		// the length of SID frame
static BOOL m_bSilenceState[MAX_SESS_NUM];		// true if in silence state
static char m_aFrameBuffer[MAX_SESS_NUM][512];		// the buffer to put frames

/**** For playing tone while receiving 2833 packet *****/
BOOL m_bPlayTone[MAX_SESS_NUM] = {0};			// the flag indicates playing tone
BOOL m_bFlashEvent[MAX_SESS_NUM] = {0};
BOOL check_2833_stop[MAX_SESS_NUM] = {0};
BOOL get_2833_edge[MAX_SESS_NUM]={0};
unsigned short play_2833_time_cnt[MAX_SESS_NUM]={0};	// must be unsigned short.
int play_2833_timeout_cnt[MAX_SESS_NUM]={0};
uint32 m_uTone[MAX_SESS_NUM];				// the event
static uint32 m_uTimestamp[MAX_SESS_NUM];		// the timestamp of the event
static int m_nCount[MAX_SESS_NUM];			// how many packets received
static uint32 session;
unsigned char rfc2833_payload_type_local[MAX_SESS_NUM];
extern void DspcodecWriteSkipSeqNo( uint32 sid, uint32 nSeq );

uint32 nTxSilencePacket[MAX_SESS_NUM];

#ifdef SUPPORT_RTP_REDUNDANT
static uint16 RtpRedundantPT_local[ SESS_NUM ];
static uint16 RtpRedundantPT_remote[ SESS_NUM ];
uint32 RtpRedundantTimestamp_remote[ SESS_NUM ];
#endif

uint16 SID_payload_type_local[ SESS_NUM ];
uint16 SID_payload_type_remote[ SESS_NUM ];
uint32 SID_count_tx[ SESS_NUM ];
uint32 SID_count_rx[ SESS_NUM ];

uint32 GetRtpRedundantStatus( uint32 sid )
{
	return RtpRedundantPT_local[ sid ];
}

RtpPayloadType GetRtpRedundantPayloadType( uint32 sid, int local )
{
	if( local )
		return ( RtpPayloadType )RtpRedundantPT_local[ sid ];
		
	return ( RtpPayloadType )RtpRedundantPT_remote[ sid ];
}

void ResetSessionTxStatistics( uint32 sid )
{
	nTxSilencePacket[ sid ] = 0;
}

void RFC2833_receiver_init(uint32 sid)
{
	m_bPlayTone[sid] = false;
	check_2833_stop[sid] = false;
	get_2833_edge[sid] = false;
	play_2833_time_cnt[sid] = 0;
	play_2833_timeout_cnt[sid] = 0;	
}

//
// CRtpTerminal - constructor/destructor
//
//static char m_pBuf[1600];
//unsigned int m_uBufSize;

void CRtpTerminal_Init(uint32 sid, CRtpConfig *pConfig)
{
	const codec_payload_desc_t *pCodecPayloadDesc;

	m_xConfig[sid] = *pConfig;
//	m_uBufSize = 1600;

	m_nFrameLen[sid] = 0;
	m_nFrameNum[sid] = 0;

	pCodecPayloadDesc = GetCodecPayloadDesc( m_xConfig[sid].m_uPktFormat );
	
	if( pCodecPayloadDesc )
	{
		m_nFramePerPacket[sid] = _idiv32(m_xConfig[sid].m_nTranFrameRate, 
										 pCodecPayloadDesc ->nTranFrameRate );
		m_nSIDFrameLen[sid] = pCodecPayloadDesc ->nSidTxFrameBytes;
 #ifdef SUPPORT_RTCP
		m_xConfig[sid].m_nPeriod = m_nFramePerPacket[sid] * 
								   pCodecPayloadDesc ->nFramePeriod;
 #endif		
	}
	else
	{
		printk("[%s] Unknown frame type %d\n", __FUNCTION__, m_xConfig[sid].m_uPktFormat);
		assert(0);
	}

	m_bSilenceState[sid] = FALSE;
	m_bPlayTone[sid] = FALSE;
	m_nCount[sid] = 0;

	m_xConfig[sid].m_uTRMode = rtp_session_sendrecv;

#ifdef SUPPORT_RTCP
	RtpOpen[sid] = 0;
	RtcpOpen[sid] = 0;
#endif
	//sessionState[sid] = rtp_session_sendrecv;

	RtpRx_setFormat(sid, m_xConfig[sid].m_uPktFormat, m_xConfig[sid].m_nRecvFrameRate);
	RtpTx_setFormat(sid, m_xConfig[sid].m_uPktFormat, m_xConfig[sid].m_nTranFrameRate);
	
	RFC2833_receiver_init(sid);

#ifdef SUPPORT_RTP_REDUNDANT
	RtpRedundantPT_local[ sid ] = 0;
	RtpRedundantPT_remote[ sid ] = 0;
	RtpRedundantTimestamp_remote[ sid ] = 0;
#endif
}

//
// CRtpTerminal - public interface
//

/* 
We get the data by the following layout:
 +---------+
 |         |
 |  Data   |
 |         |
 +---------+
 | Seq No. |
 +---------+
 |timestamp|
 +---------+
 */

int32 RtpTerminal_Read(uint32* pchid, uint32* psid, uint8 *pBuf, int32 nSize)
{
	RtpPacket *p = NULL;
	RtpEventDTMFRFC2833 *eventPayload = NULL;
	int32 nGet = 0, nPos;
	uint32 chid, sid;
	RtpSeqNumber seqNo;

	extern int rfc2833_event_fifo_wrtie(uint32 s_id, unsigned int event);
	extern void Update_current_event_fifo_state(uint32 s_id, RtpEventDTMFRFC2833* pEvent);
	extern int rfc2833_event_fifo_read(uint32 s_id);
	extern int hook_in(uint32 ch_id, char input);
	extern int Is_DAA_Channel(int chid);

	if(++session >= SESS_NUM)
		session = 0;
	

	assert(pBuf);
	p = Rtp_receive();

#if defined (PULSE_DIAL_GEN) && defined (OUTBAND_AUTO_PULSE_DIAL_GEN)
	static char into_pulse_fifo[VOIP_CH_NUM] = {0};
	static char event_in_pulse_fifo[VOIP_CH_NUM] = {0};
#endif

	if(p)
	{
		*pchid = p->chid;	/* set receive channel id */
		*psid = p->sid;
		chid = *pchid;
		sid = *psid;
		seqNo = getSequence(p);

		//printk("%d\n", rfc2833_payload_type_local[sid]);
		if((getPayloadType(p) == rfc2833_payload_type_local[sid]) && (rfc2833_payload_type_local[sid])!= 0)
		{
			eventPayload = (RtpEventDTMFRFC2833*)(getPayloadLoc(p));
			
			if( (!m_bPlayTone[sid]) && (!m_bFlashEvent[sid]))	/* no playtone */
			{
				DspcodecWriteSkipSeqNo(sid, seqNo);
				
				if(!eventPayload->edge)	/* not edge */
				{
					if ( (eventPayload->event < 0) || (eventPayload->event > 16))
					{
						printk(AC_FORE_RED "Receive unrecognized RFC2833 event %d\n" AC_RESET, eventPayload->event);
					}
					else	/* get timestamp & DTMF digit then playtone */
					{						
#if defined (PULSE_DIAL_GEN) && defined (OUTBAND_AUTO_PULSE_DIAL_GEN)
						if ( (1 == DAA_Get_Dial_Mode(chid)) && ( 1 == Is_DAA_Channel(chid)) )
						{
							if ( (into_pulse_fifo[chid] == 0) || (event_in_pulse_fifo[chid] != eventPayload->event) )
							{
								pulse_dial_in_dch(chid, eventPayload->event);
								into_pulse_fifo[chid] = 1;
								event_in_pulse_fifo[chid] = eventPayload->event;
								//printk("in\n");
							}
						}
						else
#endif
						{
							if ( 16 == eventPayload->event )// flash event
							{
								m_bFlashEvent[sid] = true;
								hook_in(chid, OUTBAND_FLASH_EVENT);
								//PRINT_G("hook_in(%d, OUTBAND_FLASH_EVENT)\n", chid);
							}
							else
							{
								rfc2833_event_fifo_wrtie(sid, eventPayload->event);
								Update_current_event_fifo_state(sid, eventPayload);
								m_bPlayTone[sid] = true;
							}
							m_uTimestamp[sid] = getRtpTime(p);
							m_uTone[sid] = eventPayload->event;
							
							//printk("Start(sid=%d)\n", sid);
							PRINT_MSG("Get 2833 DTMF Event:%d (sid=%d)\n", eventPayload->event, sid);
						}

					}
				}
#if defined (PULSE_DIAL_GEN) && defined (OUTBAND_AUTO_PULSE_DIAL_GEN)
				else
				{
					//printk("edge(sid=%d)\n", sid);
					into_pulse_fifo[chid] = 0;
				}
#endif
			}
			else	/* playing tone */
			{
				DspcodecWriteSkipSeqNo(sid, seqNo);
				
				if(getRtpTime(p) == m_uTimestamp[sid])
				{
					if(eventPayload->edge)
					{
						if ( 16 == eventPayload->event )// flash event
							m_bFlashEvent[sid] = false;
						else
						{
							Update_current_event_fifo_state(sid, eventPayload);
							get_2833_edge[sid] = true;
							m_bPlayTone[sid] = false;
						}
						//printk("edge(sid=%d)\n", sid);
					}
					else
					{
						if ( 16 != eventPayload->event )
						Update_current_event_fifo_state(sid, eventPayload);
					}
				}
				else /* get another event, getRtpTime() != m_uTimestamp */
				{
					if ( (eventPayload->event < 0) || (eventPayload->event > 16))
					{
						rfc2833_event_fifo_read(sid);
						m_bFlashEvent[sid] = false;
						m_bPlayTone[sid] = false;
						printk(AC_FORE_RED "Receive unrecognized RFC2833 event %d\n" AC_RESET, eventPayload->event);
					}
					else
					{	/* Thlin: Should not go to here*/
						rfc2833_event_fifo_read(sid);
						m_bPlayTone[sid] = false;
						printk(AC_FORE_RED "RFC2833 receive error, sid=%d, event=%d\n" AC_RESET, sid, eventPayload->event);
					
						// Get diff timestamp 2833 event, stop previous DTMF tone, and then accept this diff timestamp 2833 event as new one.
						if ( 16 == eventPayload->event )// flash event
							m_bFlashEvent[sid] = true;
						else
						{		
							rfc2833_event_fifo_wrtie(sid, eventPayload->event);
							Update_current_event_fifo_state(sid, eventPayload);
							m_bPlayTone[sid] = true;
						}
						m_uTimestamp[sid] = getRtpTime(p);
						m_uTone[sid] = eventPayload->event;
						//printk(AC_FORE_RED "timestamp(sid=%d)= %u\n" AC_RESET, sid, m_uTimestamp[sid]);
						//printk(AC_FORE_RED "get pkt timestamp= %u\n" AC_RESET, getRtpTime(p));
					}
				}
			}
			/* invalid length to distinguish that it is an event packet */
			nGet = -1;
		}
		else	/* non RFC2833 packet */
		{

				if(nSize >= getPayloadUsage(p) + 8)
					nGet = getPayloadUsage(p);
				else
					assert(0);

				memcpy(pBuf, getPayloadLoc(p), nGet);

#if 1
				// remove padding bytes 
				if( getPaddingFlag( p ) ) {
					nGet -= pBuf[ nGet - 1 ];
					
					if( nGet <= 0 ) {
						nGet = 0;
						printk( "PZ " );
					} 
				}
#endif

				if(nGet & 0x00000003)		/* not align */
					nPos = 4 - (nGet & 0x00000003) + nGet;
				else
					nPos = nGet;
				*((uint32 *)(pBuf + nPos)) = getSequence(p);
				*((uint32 *)(pBuf + nPos + 4)) = getRtpTime(p);
		}
		p->own = OWNBYDSP;
	}

#ifdef SUPPORT_RTCP
	//printk("--- RtcpOpen[%d] = %d --- \n", session, RtcpOpen[session]);
	if(RtcpOpen[session] == 1)	//thlin: if RtcpOpen =1 , do RtpSession_processRTCP, eles NOT.
	{
		RtpSession_processRTCP(session);
	}
#endif
	return nGet;
}

int32 RtpTerminal_Write(uint32 chid, uint32 sid, uint8 *pBuf, int32 nSize)
{
	/* no more frame */
	if(nSize == 0)
		return 0;

	/* silence period */
	else if(nSize == -1)	// invalid length to distinguish that it is silence
	{
		RtpTx_addTimestampOfOneFrame(sid);
#if 0	// pkshih: why?
 #ifndef __ECOS
		if(m_xConfig[sid].m_uPktFormat == rtpPayloadG729)
			RtpTx_addTimestamp(sid);
 #endif
#endif
		return 0;
	}

	/* normal frame or SID frame */
	else
	{
		assert(pBuf);
//		ASSERT(pBuf);
		//send_data_in_ppp_format( 2 , "write rtpTerminal" , 17 ) ;		// Howard. 2004.12.8 for debug
		if(nSize == m_nSIDFrameLen[sid])	// SID frame
		{
			nTxSilencePacket[ sid ] ++;
			
			if(m_bSilenceState[sid])
			{
				RtpTx_setMarkerOnce(sid);
				RtpTx_transmitRaw(chid, sid, (char*)pBuf, (int)nSize);
				RtpTx_subTimestampIfNecessary( sid, ( char * )pBuf, nSize );
#if 0	// pkshih: why??
 #ifndef __ECOS
				if(m_xConfig[sid].m_uPktFormat == rtpPayloadG729)
					RtpTx_addTimestamp(sid);
 #endif
				if((m_xConfig[sid].m_uPktFormat == rtpPayloadPCMU) || (m_xConfig[sid].m_uPktFormat == rtpPayloadPCMA))
					RtpTx_addTimestamp(sid);
#endif
			}
			else
			{
				if(m_nFrameLen[sid] != 0)
				{	// always false ( 0 != 0 )
					RtpTx_subTimestamp(sid);
					RtpTx_transmitRaw(chid, sid, m_aFrameBuffer[sid], m_nFrameLen[sid]);
					RtpTx_subTimestampIfNecessary( sid, m_aFrameBuffer[sid], m_nFrameLen[sid] );
					m_nFrameLen[sid] = 0;
					m_nFrameNum[sid] = 0;
				}
				RtpTx_transmitRaw(chid, sid, (char*)pBuf, (int)nSize);
				RtpTx_subTimestampIfNecessary( sid, ( char * )pBuf, nSize );
#if 0	// pkshih: why??
 #ifndef __ECOS
				if(m_xConfig[sid].m_uPktFormat == rtpPayloadG729)
					RtpTx_addTimestamp(sid);
 #endif
				if((m_xConfig[sid].m_uPktFormat == rtpPayloadPCMU) || (m_xConfig[sid].m_uPktFormat == rtpPayloadPCMA))
					RtpTx_addTimestamp(sid);
#endif
				m_bSilenceState[sid] = true;
			}
		}
		else	// normal frame
		{
			if(m_bSilenceState[sid])
			{
				RtpTx_setMarkerOnce(sid);
				m_bSilenceState[sid] = false;
			}
			memcpy(m_aFrameBuffer[sid]+m_nFrameLen[sid], pBuf, nSize);
			m_nFrameLen[sid] += nSize;
			m_nFrameNum[sid]++;
			if(m_nFrameNum[sid] == m_nFramePerPacket[sid])
			{	// always true ( 1 == 1 )
				RtpTx_transmitRaw(chid, sid, m_aFrameBuffer[sid], m_nFrameLen[sid]);
				RtpTx_subTimestampIfNecessary( sid, m_aFrameBuffer[sid], m_nFrameLen[sid] );
				m_nFrameLen[sid] = 0;
				m_nFrameNum[sid] = 0;
			}
			else
				RtpTx_addTimestamp(sid);
		}

		return nSize;
	}
}

void RtpTerminal_setFormat(uint32 sid, RtpPayloadType type, int recvRate, int tranRate)
{
	RtpTx_setFormat(sid, type, tranRate);
	RtpRx_setFormat(sid, type, recvRate);
}

/* The very function that used to config endpoint */
void RtpTerminal_SetConfig(uint32 chid, uint32 sid, void *pConfig)
{
	//bool rtpConfig_changed = false;
	//bool rtcpConfig_changed = false;
	CRtpConfig* pRtpConfig = NULL;
	const codec_payload_desc_t *pCodecPayloadDesc;

	assert(pConfig);
	pRtpConfig = (CRtpConfig *)pConfig;

#if 0
//#ifdef SUPPORT_RTCP

	/* RTCP part */
	if(m_xConfig[sid].m_nRtcpRemotePort != pRtpConfig->m_nRtcpRemotePort)
	{
		rtcpConfig_changed = true;
		m_xConfig[sid].m_nRtcpRemotePort = pRtpConfig->m_nRtcpRemotePort;
	}
	if(m_xConfig[sid].m_nRtcpLocalPort != pRtpConfig->m_nRtcpLocalPort)
	{
		rtcpConfig_changed = true;
		m_xConfig[sid].m_nRtcpLocalPort = pRtpConfig->m_nRtcpLocalPort;
	}

if(rtcpConfig_changed==true)
	{
		if(RtpOpen[sid])	/* if Rtp is open, then can open and close Rtcp session*/
		{
			if((pRtpConfig->m_nRtcpRemotePort == 0) || (pRtpConfig->m_nRtcpLocalPort == 0))
			{
				/*
				   As our rule, we close the rtcp session,
				   when the Rtcp remote port or local port is zero.
				*/
				if(RtcpOpen[sid])
				{
					RtcpTx_transmitRTCPBYE(sid);
					rtcp_Session_UnConfig(chid, sid);
					RtcpOpen[sid] = 0;
				}
			}

		}
		else
		{
			/*
			   If rtp session is already closed,
			   we only need to close the flag and do not need to close rtcp session.
			   The rtcp session is already closed when rtp_session_inactive.
			*/
			if(RtcpOpen[sid])
				if((pRtpConfig->m_nRtcpRemotePort == 0) || (pRtpConfig->m_nRtcpLocalPort == 0))
					RtcpOpen[sid] = 0;
			return;
		}
	}
	/*-------------------------------------------------------------------------*/

	if (pRtpConfig->m_uTRMode == rtp_session_inactive)/*Wish to stop RTP transfer*/
	{
		if(RtpOpen[sid])
		{

			if(RtcpOpen[sid])
			{
				RtcpTx_transmitRTCPBYE(sid);
				rtcp_Session_UnConfig(chid, sid);
			}

			//rtp_Session_UnConfig(chid, sid);	/* clear voip port register by ROME driver function */


			RtpOpen[sid] = 0;
		}
		m_xConfig[sid].m_uTRMode = pRtpConfig->m_uTRMode;
		RtpSession_setSessionState(sid, m_xConfig[sid].m_uTRMode);
		printk("\nRTP session stop on ch:%d system session:%d\n", chid, sid);
		DSP_CodecStop(chid, sid);
		return;
	}

	
#endif //by bruce
	m_xConfig[sid].m_uPktFormat = pRtpConfig->m_uPktFormat;
	m_xConfig[sid].m_nRecvFrameRate = pRtpConfig->m_nRecvFrameRate;
	m_xConfig[sid].m_nTranFrameRate = pRtpConfig->m_nTranFrameRate;
	m_xConfig[sid].m_uTRMode = pRtpConfig->m_uTRMode;
	RtpTerminal_setFormat(sid, m_xConfig[sid].m_uPktFormat, m_xConfig[sid].m_nRecvFrameRate, m_xConfig[sid].m_nTranFrameRate);
	RtpSession_setSessionState(sid, m_xConfig[sid].m_uTRMode);

	m_nFrameLen[sid] = 0;
	m_nFrameNum[sid] = 0;

	pCodecPayloadDesc = GetCodecPayloadDesc( m_xConfig[sid].m_uPktFormat );
	
	if( pCodecPayloadDesc ) {
		m_nFramePerPacket[sid] = _idiv32(m_xConfig[sid].m_nTranFrameRate, 
										 pCodecPayloadDesc ->nTranFrameRate );
		m_nSIDFrameLen[sid] = pCodecPayloadDesc ->nSidTxFrameBytes;
	} else {
		printk("[%s] Unknown frame type %d\n", __FUNCTION__, m_xConfig[sid].m_uPktFormat);
		assert(0);
	}

	m_bSilenceState[sid] = false;
	m_bPlayTone[sid] = false;
	m_nCount[sid] = 0;

	RtpRx_setFormat(sid, m_xConfig[sid].m_uPktFormat, m_xConfig[sid].m_nRecvFrameRate);
	RtpTx_setFormat(sid, m_xConfig[sid].m_uPktFormat, m_xConfig[sid].m_nTranFrameRate);
#if 0
	if (pRtpConfig->m_uTRMode == rtp_session_inactive)
	{
		//rtp_Session_UnConfig(chid, sid);	// clear voip port register
//		chanInfo_CloseSessionID(chid, sid);

		m_xConfig[sid].m_uTRMode = pRtpConfig->m_uTRMode;
		RtpSession_setSessionState(sid, m_xConfig[sid].m_uTRMode);

		m_xConfig[sid].m_nRtpRemoteIP = 0;
		m_xConfig[sid].m_nRtpRemotePort = 0;

		return;
	}
	
#endif
	if (pRtpConfig->m_uTRMode == rtp_session_inactive)
	{
		m_xConfig[sid].m_nRtpRemoteIP = 0;
		m_xConfig[sid].m_nRtpRemotePort = 0;
	}
}

/*+++++ add by Jack for set session state+++++*/
void RtpTerminal_SetSessionState(uint32 sid, uint32 state){
	m_xConfig[sid].m_uTRMode = state;
	RtpSession_setSessionState(sid, m_xConfig[sid].m_uTRMode);

}
/*-----end-----*/
void RtpTerminal_GetConfig(uint32 sid, void *pConfig)
{
	assert(pConfig);
	*((CRtpConfig *)pConfig) = m_xConfig[sid];
}

#ifdef SUPPORT_RFC_2833
unsigned char RtpTerminal_SendDTMFEvent(uint32 chid, uint32 sid, int32 nEvent, int duration)

{
	return RtpTx_transmitEvent(chid, sid, nEvent, duration);
}
#endif

#if 0
void RtpTerminal_TransmitData(void)
{
	// transmit data from source terminals to destination terminals
	// note:
	// the data multiplexing is done in target (sink) terminals rather than in stream
	//
	uint32 chid;
	int nRealSize;
	while (1)
	{
		nRealSize = RtpTerminal_Read(&chid, m_pBuf, m_uBufSize);
		DSP_Write(chid, m_pBuf, nRealSize);
		if(nRealSize == 0)
			break;
	}

	while(1)
	{
		nRealSize = DSP_Read(&chid, m_pBuf, m_uBufSize);
		RtpTerminal_Write(chid, m_pBuf, nRealSize);
		if(nRealSize == 0)
			break;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////
// for RTP session
void RtpSession_setSessionState (uint32 sid, RtpSessionState state)
{
	sessionState[sid] = state;

	switch(state)
	{
		case rtp_session_inactive:
			//PRINT_MSG("+++++Debug:go to rtp_session_inactive %d+++++\n",sid);
			RtpRx_setMode(sid, rtprecv_droppacket);
			RtpTx_setMode(sid, rtptran_droppacket);
			break;
		case rtp_session_sendonly:
			//PRINT_MSG("+++++Debug:go to rtp_session_sendonly %d+++++\n",sid);
			RtpRx_setMode(sid, rtprecv_droppacket);
			RtpTx_setMode(sid, rtptran_normal);
			break;
		case rtp_session_recvonly:
			//PRINT_MSG("+++++Debug:go to rtp_session_recvonly %d+++++\n",sid);
			RtpRx_setMode(sid, rtprecv_normal);
			RtpTx_setMode(sid, rtptran_droppacket);
			break;
		case rtp_session_sendrecv:
			//PRINT_MSG("+++++Debug:go to rtp_session_sendrecv %d+++++\n",sid);
			RtpRx_setMode(sid, rtprecv_normal);
			RtpTx_setMode(sid, rtptran_normal);
			break;
		default:
			assert(0);
			break;
	}
}

void RtpSession_renew(uint32 sid, 
		uint16 redundantPT_local, uint16 redundantPT_remote,
		uint16 SID_PT_local, uint16 SID_PT_remote )
{
	if( sid < SESS_NUM ) {
#ifdef SUPPORT_RTP_REDUNDANT
		if( redundantPT_local < 96 || redundantPT_remote < 96 ||
			redundantPT_local > 127 || redundantPT_remote > 127 )
		{
			redundantPT_local = redundantPT_remote = 0;
		}
	
		RtpRedundantPT_local[ sid ] = redundantPT_local;
		RtpRedundantPT_remote[ sid ] = redundantPT_remote;
		RtpRedundantTimestamp_remote[ sid ] = 0;
#endif

		// SID local and remote payload type are indenpendent 
		if( SID_PT_local < 96 || SID_PT_local > 127 )
			SID_PT_local = 0;
		
		if( SID_PT_remote < 96 || SID_PT_remote > 127 )
			SID_PT_remote = 0;
		
		SID_payload_type_local[ sid ] = SID_PT_local;
		SID_payload_type_remote[ sid ] = SID_PT_remote;
		
		SID_count_tx[ sid ] = SID_count_rx[ sid ] = 0;
	}

	RtpTx_renewSession(sid);
	/*
	 * pkshih uncomment following. 
	 * Because sourceSet[] is set, updateSource() will try to change 
	 * source and drop packets in begining of session. 
	 * We guess that it affects voice quality only if VAD is on, 
	 * because voice frames are almost silence in begining.
	 * In other words, drop these silence frames will not affect 
	 * voice quality.
	 *
	 * WARNING: I don't know why someone comment it!!
	 */
	RtpRx_renewSession(sid);

#ifdef SUPPORT_RTCP
	if( sid >= RTCP_SID_OFFSET ) {
		sid -= RTCP_SID_OFFSET;
		
		RtcpTx_InitByID( sid );
		RtcpRx_InitByID( sid );
	}
#endif
}

#ifdef SUPPORT_RTCP
void RtpSession_processRTCP (uint32 sid)
{

	if (RtcpTx_checkIntervalRTCP(sid))
	{
		RtcpTx_transmitRTCP(sid);
		//printk("T%d\n", sid);
	}

	RtcpRx_receiveRTCP(sid);

    return ;
}
#endif

static int voip_rtpterm_read_proc( char *buf, char **start, off_t off, int count, int *eof, void *data )
{
	extern int g_dynamic_pt_remote_vbd[];
	extern int g_dynamic_pt_local_vbd[];
	extern TstVoipPayLoadTypeConfig astVoipPayLoadTypeConfig[];
	int ss;
	int n = 0;

	if( off ) {	/* In our case, we write out all data at once. */
		*eof = 1;
		return 0;
	}
	
	if( IS_CH_PROC_DATA( data ) ) {
		//ch = CH_FROM_PROC_DATA( data );
		//n = sprintf( buf, "channel=%d\n", ch );
	} else {
		ss = SS_FROM_PROC_DATA( data );
		
		n = sprintf( buf, "session=%d\n", ss );
#ifdef SUPPORT_RTP_REDUNDANT
		n += sprintf( buf + n, "RtpRedundantPT local/remote:\t%d/%d\n", RtpRedundantPT_local[ ss ], RtpRedundantPT_remote[ ss ] );
		n += sprintf( buf + n, "RtpRedundantTimestamp  remote:\t%d\n", RtpRedundantTimestamp_remote[ ss ] );
#endif
		n += sprintf( buf + n, "SID_payload_type local/remote:\t%d/%d\n", SID_payload_type_local[ ss ], SID_payload_type_remote[ ss ] );
		n += sprintf( buf + n, "SID_count rx/tx:\t%lu/%lu\n", SID_count_rx[ ss ], SID_count_tx[ ss ] );
	}
	
	*eof = 1;
	return n;
}

int __init voip_rtpterm_proc_init( void )
{
	//create_voip_channel_proc_read_entry( "rtpterm", voip_rtpterm_read_proc );
	create_voip_session_proc_read_entry( "rtpterm", voip_rtpterm_read_proc );

	return 0;
}

void __exit voip_rtpterm_proc_exit( void )
{
	//remove_voip_channel_proc_entry( "rtpterm" );
	remove_voip_session_proc_entry( "rtpterm" );
}

voip_initcall_proc( voip_rtpterm_proc_init );
voip_exitcall( voip_rtpterm_proc_exit );

