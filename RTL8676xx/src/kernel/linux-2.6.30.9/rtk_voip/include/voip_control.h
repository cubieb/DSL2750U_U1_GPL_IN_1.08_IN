/**
 * @file voip_control.h
 * @brief VoIP control structure
 */

#ifndef VOIP_CONTROL_H
#define VOIP_CONTROL_H

#include "voip_params.h"

#define VOIP_MGR_BASE_CTL				64+1024+64+64+900

//! VoIP Control ID @addtogroup VOIP_CONTROL
//!@{
enum {
	// testing function 
	VOIP_MGR_SET_EBL = VOIP_MGR_BASE_CTL+1,	
	VOIP_MGR_INIT_GNET, 
	VOIP_MGR_INIT_GNET2, 
	VOIP_MGR_LOOPBACK, 
	VOIP_MGR_GNET, 
	VOIP_MGR_SIGNALTEST, 
	VOIP_MGR_DSPSETCONFIG, 
	VOIP_MGR_DSPCODECSTART, 
	
	// Protocol - RTP 
	//! @addtogroup VOIP_PROTOCOL_RTP
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_SET_SESSION, 
	VOIP_MGR_UNSET_SESSION, 
	VOIP_MGR_SETRTPSESSIONSTATE, 
	VOIP_MGR_RTP_CFG, 
	VOIP_MGR_HOLD, 
	VOIP_MGR_CTRL_RTPSESSION, 
	VOIP_MGR_CTRL_TRANSESSION_ID, 
	VOIP_MGR_SETCONFERENCE, 	
	VOIP_MGR_GET_RTP_STATISTICS,
	VOIP_MGR_GET_SESSION_STATISTICS,

	// Protocol - RTCP  
	//! @addtogroup VOIP_PROTOCOL_RTCP
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_SET_RTCP_SESSION, 
	VOIP_MGR_UNSET_RTCP_SESSION, 
	VOIP_MGR_SET_RTCP_TX_INTERVAL, 	

	// DSP - General  
	//! @addtogroup VOIP_DSP_GENERAL
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_ON_HOOK_RE_INIT, 
	VOIP_MGR_SET_VOICE_GAIN, 
	VOIP_MGR_ENERGY_DETECT, 
	
	// DSP - Codec 
	//! @addtogroup VOIP_DSP_CODEC
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_SETRTPPAYLOADTYPE, 
	VOIP_MGR_DSPCODECSTOP,
	
	// DSP - FAX and Modem  
	//! @addtogroup VOIP_DSP_FAXMODEM
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_FAX_MODEM_PASS_CFG, 
	VOIP_MGR_FAX_OFFHOOK, 
	VOIP_MGR_FAX_END_DETECT, 
	VOIP_MGR_SET_FAX_MODEM_DET, 
	VOIP_MGR_FAX_DIS_DETECT,
	VOIP_MGR_FAX_DIS_TX_DETECT,
	VOIP_MGR_FAX_DIS_RX_DETECT,
	VOIP_MGR_FAX_DCN_DETECT,
	VOIP_MGR_FAX_DCN_TX_DETECT,
	VOIP_MGR_FAX_DCN_RX_DETECT,
	
	// DSP - LEC  
	//! @addtogroup VOIP_DSP_LEC
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_SET_ECHO_TAIL_LENGTH, 
	VOIP_MGR_SET_G168_LEC_CFG,
	VOIP_MGR_GET_EC_DEBUG, //add by jwsyu, echo canceller debug
		
	// DSP - DTMF  
	//! @addtogroup VOIP_DSP_DTMF
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_DTMF_DET_THRESHOLD,
	VOIP_MGR_DTMF_CFG, 
	VOIP_MGR_DTMF_GET, 
	VOIP_MGR_SET_DTMF_MODE, 
	VOIP_MGR_SEND_RFC2833_PKT_CFG, 
	VOIP_MGR_SEND_RFC2833_BY_AP, 
	VOIP_MGR_PLAY_SIP_INFO,
	VOIP_MGR_LIMIT_MAX_RFC2833_DTMF_DURATION,
	VOIP_MGR_SET_RFC2833_TX_VOLUME,

	// DSP - Caller ID  
	//! @addtogroup VOIP_DSP_CALLERID
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_DTMF_CID_GEN_CFG, 
	VOIP_MGR_GET_CID_STATE_CFG, 
	VOIP_MGR_FSK_CID_GEN_CFG, 
	VOIP_MGR_SET_FSK_VMWI_STATE, 
	VOIP_MGR_SET_FSK_AREA, 
	VOIP_MGR_FSK_ALERT_GEN_CFG, 
	VOIP_MGR_SET_CID_DTMF_MODE, 
	VOIP_MGR_SET_CID_DET_MODE, 
	VOIP_MGR_GET_FSK_CID_STATE_CFG, 
	VOIP_MGR_SET_CID_FSK_GEN_MODE, 
	VOIP_MGR_FSK_CID_VMWI_GEN_CFG,
	VOIP_MGR_SET_FSK_CLID_PARA,
	VOIP_MGR_GET_FSK_CLID_PARA,
	VOIP_MGR_FSK_CID_MDMF_GEN,
	
	// DSP - Tone  
	//! @addtogroup VOIP_DSP_TONE
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_SETPLAYTONE, 
	VOIP_MGR_SET_COUNTRY,
	VOIP_MGR_SET_COUNTRY_IMPEDANCE,
	VOIP_MGR_SET_COUNTRY_TONE,
	VOIP_MGR_SET_TONE_OF_CUSTOMIZE, 
	VOIP_MGR_SET_CUST_TONE_PARAM, 
	VOIP_MGR_USE_CUST_TONE, 
	VOIP_MGR_SET_DIS_TONE_DET,

	// DSP - AGC  
	//! @addtogroup VOIP_DSP_AGC
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_SET_SPK_AGC, 
	VOIP_MGR_SET_SPK_AGC_LVL, 
	VOIP_MGR_SET_SPK_AGC_GUP, 
	VOIP_MGR_SET_SPK_AGC_GDOWN, 
	VOIP_MGR_SET_MIC_AGC, 
	VOIP_MGR_SET_MIC_AGC_LVL, 
	VOIP_MGR_SET_MIC_AGC_GUP, 
	VOIP_MGR_SET_MIC_AGC_GDOWN,

	// DSP - Pluse Dial 
	//! @addtogroup VOIP_DSP_PLUSEDIAL
	//! @ingroup VOIP_CONTROL
	//TH: for pulse dial config
	VOIP_MGR_SET_PULSE_DIGIT_DET, 
	VOIP_MGR_SET_DIAL_MODE, 
	VOIP_MGR_GET_DIAL_MODE, 
	VOIP_MGR_PULSE_DIAL_GEN_CFG, 
	VOIP_MGR_GEN_PULSE_DIAL,

	// DSP - IVR  
	//! @addtogroup VOIP_DSP_IVR
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_PLAY_IVR, 
	VOIP_MGR_POLL_IVR, 
	VOIP_MGR_STOP_IVR, 
	
	// Driver - PCM ctrl  
	//! @addtogroup VOIP_DRIVER_PCM
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_PCM_CFG, 
	VOIP_MGR_SET_PCM_LOOP_MODE,
	
	// Driver - SLIC  
	//! @addtogroup VOIP_DRIVER_SLIC
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_SLIC_HOOK, 
	VOIP_MGR_SLIC_RING, 
	VOIP_MGR_SLIC_TONE, 
	VOIP_MGR_SLIC_RESTART, 
	VOIP_MGR_GET_SLIC_REG_VAL, 
	VOIP_MGR_SET_SLIC_TX_GAIN, 
	VOIP_MGR_SET_SLIC_RX_GAIN, 
	VOIP_MGR_SET_FLASH_HOOK_TIME, 
	VOIP_MGR_SET_SLIC_RING_CADENCE,
	VOIP_MGR_SET_SLIC_RING_FRQ_AMP,
	VOIP_MGR_SET_IMPEDANCE,
	VOIP_MGR_SET_SLIC_REG_VAL, 
	VOIP_MGR_GET_SLIC_STAT, 
	VOIP_MGR_SLIC_ONHOOK_ACTION, 	// only used in AudioCodes
	VOIP_MGR_SLIC_OFFHOOK_ACTION, 	// only used in AudioCodes
	VOIP_MGR_FLUSH_FIFO, 
	VOIP_MGR_LINE_CHECK, 
	VOIP_MGR_HOOK_FIFO_IN, 
	VOIP_MGR_GET_SLIC_RAM_VAL, 
	VOIP_MGR_SET_SLIC_RAM_VAL,

	VOIP_MGR_DTMF_INTO_FIFO, 	// For Ethernet DSP
	VOIP_MGR_HOOK_INTO_FIFO, 	// For Ethernet DSP

	VOIP_MGR_SET_RING_DETECTION, 
	VOIP_MGR_SET_FXS_FXO_LOOPBACK,
	VOIP_MGR_SET_SLIC_ONHOOK_TRANS_PCM_START,
	
	VOIP_MGR_SET_SLIC_LINE_VOLTAGE,
	VOIP_MGR_GEN_SLIC_CPC,
	
	// Driver - DAA  
	//! @addtogroup VOIP_DRIVER_DAA
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_DAA_RING, 
	VOIP_MGR_DAA_OFF_HOOK, 
	VOIP_MGR_DAA_ON_HOOK,
	VOIP_MGR_GET_DAA_LINE_VOLTAGE,
	VOIP_MGR_SET_DAA_TX_GAIN, 
	VOIP_MGR_SET_DAA_RX_GAIN, 
	VOIP_MGR_SET_DAA_ISR_FLOW, 
	VOIP_MGR_GET_DAA_ISR_FLOW,
	VOIP_MGR_SET_DAA_PCM_HOLD_CFG, 
	VOIP_MGR_GET_DAA_BUSY_TONE_STATUS, 
	VOIP_MGR_GET_DAA_CALLER_ID, 
	VOIP_MGR_GET_DAA_USED_BY_WHICH_SLIC, 
	VOIP_MGR_FXO_ON_HOOK, 
	VOIP_MGR_FXO_OFF_HOOK, 

	// Driver - GPIO  
	//! @addtogroup VOIP_DRIVER_GPIO
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_GPIO, 
	
	// Driver - Networking  
	//! @addtogroup VOIP_DRIVER_NETWORK
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_8305_SWITCH_VAL, 
	VOIP_MGR_WAN_VLAN_TAG, 
	VOIP_MGR_BRIDGE_MODE,
	VOIP_MGR_SET_DSCP_PRIORITY, 
	VOIP_MGR_WAN_2_VLAN_TAG, 
	VOIP_MGR_WAN_3_VLAN_TAG, 
	//add by Tim
	VOIP_MGR_SET_WAN_CLONE_MAC, 
	VOIP_MGR_BANDWIDTH_MGR, 
	VOIP_MGR_GET_PORT_LINK_STATUS, 
	VOIP_MGR_SET_RTP_TOS,
	VOIP_MGR_SET_RTP_DSCP,
	VOIP_MGR_SET_SIP_TOS,
	VOIP_MGR_SET_SIP_DSCP,

	VOIP_MGR_PORT_DISABLE,
	VOIP_MGR_PORT_PRIORITY,
	VOIP_MGR_PORT_DISABLE_FLOWCONTROL,

	// Driver - DECT  
	//! @addtogroup VOIP_DRIVER_DECT
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_DECT_SET_POWER, 
	VOIP_MGR_DECT_GET_POWER, 
	VOIP_MGR_DECT_GET_PAGE,
	VOIP_MGR_DECT_BUTTON_GET,
	VOIP_MGR_DECT_SET_LED,
	
	// Miscellanous  
	//! @addtogroup VOIP_MISC
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_SIP_REGISTER, 
	VOIP_MGR_GET_FEATURE, 
	VOIP_MGR_VOIP_RESOURCE_CHECK, 
	VOIP_MGR_SET_FW_UPDATE,
		
	// IP Phone - keypad, LCM, Codec, LED and etc  
	//! @addtogroup VOIP_IPPHONE
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_CTL_KEYPAD, 
	VOIP_MGR_CTL_LCM, 
	VOIP_MGR_CTL_VOICE_PATH, 
	VOIP_MGR_CTL_LED, 
	VOIP_MGR_CTL_MISC, 
	
	// Debug  
	//! @addtogroup VOIP_DEBUG
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_DEBUG, 
	VOIP_MGR_VOICE_PLAY, 
	VOIP_MGR_GET_T38_PCMIN, 
	VOIP_MGR_GET_T38_PACKETIN, 
	VOIP_MGR_SET_GETDATA_MODE, 
	VOIP_MGR_IPHONE_TEST, 
	VOIP_MGR_PRINT,
	VOIP_MGR_COP3_CONIFG,
	
	// Ethernet DSP
	//! @addtogroup VOIP_ETHERNET_DSP
	//! @ingroup VOIP_CONTROL
	VOIP_MGR_SET_DSP_ID_TO_DSP,
	VOIP_MGR_SET_DSP_PHY_ID,
	VOIP_MGR_CHECK_DSP_ALL_SW_READY,

	VOIP_MGR_SET_MAX, 	// keep it to be the last one 
};
//!@}

//enum {
//	VOIP_MGR_GET_EBL = VOIP_MGR_BASE_CTL+1, 
//	VOIP_MGR_GET_SESSION, 
//	VOIP_MGR_GET_MAX, 	// keep it to be the last one 
//};

#ifdef CONFIG_RTK_VOIP_SRTP
#define SRTP_KEY_LEN 30
#endif

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP

#define ETH_VOICE2DSP_PKT	0x30	//    host --> dsp 
#define ETH_VOICE2HOST_PKT	0x31	//    host <-- dsp 
#define ETH_CTRL_PKT		0x32	// a) host --> dsp 
#define ETH_RESP_PKT		0x33	// a) host <-- dsp 
#define ETH_EVENT_PKT		0x34	// b) host --> dsp 
#define ETH_ACK_PKT			0x35	// b) host <-- dsp 
#define ETH_T38_TO_DSP_PKT	0x36	//    host --> dsp 
#define ETH_T38_TO_HOST_PKT	0x37	//    host <-- dsp 


#define ETH_SRC_MAC_SHIFT	6
#define ETH_TYPE_SHIFT		12
#define DSP_ID_SHIFT		14
#define DUMMY_SHIFT		16 // Add 2 bytes dummy to make content 4 byte-align
#define	PROTOCOL_SHIFT		18
#define VOIP_VER_SHIFT		19
#define CATEGORY_SHIFT		20
#define SEQUENCE_SHIFT		22
#define CONT_LEN_SHIFT		24
#define CONTENT_SHIFT		26

#define VOICE_CONT_LEN_SHIFT	20
#define VOICE_CONTENT_SHIFT	22

#define ETH_TYPE_FIELD_LEN	2
#define DSP_ID_FIELD_LEN	2
#define DUMMY_FIELD_LEN		2
#define PROTOCOL_FIELD_LEN	1
#define VOIP_VER_FIELD_LEN	1
#define CATEGORY_FIELD_LEN	2
#define SEQUENCE_FIELD_LEN	2
#define CONT_LEN_FIELD_LEN	2


typedef struct
{
	unsigned short category;
	unsigned short seqNo;
	unsigned short needResponse;
}
TstCheckResponse;

typedef struct
{
	unsigned short category;
	unsigned short seqNo;
	unsigned short needAck;
}
TstCheckAck;

typedef struct
{
	int host_chid;
	int dsp_id;
	int seq_no;
	unsigned short resend_flag;
}
TstTxPktCtrl;

#endif

/**
 * @brief This structure is a member of three way conference
 * @param ch_id Channel ID
 * @param m_id Media ID
 * @param ip_src_addr Source IP address 
 * @param ip_dst_addr Destination IP address 
 * @param udp_src_port Source port 
 * @param udp_dst_port Destination port 
 * @param protocol IP protocol number 
 * @param tos TOS for IP header 
 * @param rfc2833_payload_type_local Local payload type of RFC2833
 * @param rfc2833_payload_type_remote Remote payload type of RFC2833
 * @param local_pt Local payload type
 * @param remote_pt Remote payload type
 * @param uPktFormat RTK_VoIP predefined payload type 
 * @param nG73Type G.723 bitrate 
 * @param nFramePerPacket Frames per packet 
 * @param bVAD VAD is on or off 
 * @param nJitterDelay Jitter buffer delay 
 * @param nMaxDelay Jitter buffer maximum delay 
 * @param nJitterFactor Jitter buffer optimization factor 
 * @param state RTP session state 
 * @param result The result 
 */
typedef struct
{
	// RTP session info
	uint8 ch_id;
	int32 m_id;
	uint32 ip_src_addr;
	uint32 ip_dst_addr;
	uint16 udp_src_port;
	uint16 udp_dst_port;
	uint16 rtcp_src_port;
	uint16 rtcp_dst_port;
	uint8 protocol;
	uint8 tos;
	uint16 rfc2833_payload_type_local;
	uint16 rfc2833_payload_type_remote;
	//Payload info
	RtpPayloadType local_pt; // payload type in local
	RtpPayloadType remote_pt; // payload type in remote answer
	RtpPayloadType uPktFormat; // payload type in rtk_voip
	RtpPayloadType local_pt_vbd; // payload type in local
	RtpPayloadType remote_pt_vbd; // payload type in remote answer
	RtpPayloadType uPktFormat_vbd; // payload type in rtk_voip
	int32 nG723Type;
	int32 nFramePerPacket;
	int32 bVAD;
	uint32 nJitterDelay;
	uint32 nMaxDelay;
	uint32 nJitterFactor;
	// RTP session state
	uint8	state;
	int32 result;
	//int32	ret_val;
}
TstVoipMgrRtpCfg;

/**
 * @brief Structure for three way conference
 * @param ch_id Channel ID
 * @param enable Enable or disable three way conference 
 * @param rtp_cfg Conference these two sessions 
 * @see do_mgr_VOIP_MGR_SETCONFERENCE()
 */
typedef struct
{
	uint8 ch_id;
	uint8 enable;
	TstVoipMgrRtpCfg rtp_cfg[2];
	//int32 ret_val;
}
TstVoipMgr3WayCfg;

/**
 * @brief Structure for RTP session 
 * @param ip_src_addr Source IP address 
 * @param ip_dst_addr Destination IP address 
 * @param udp_src_port Source port 
 * @param udp_dst_port Destination port 
 * @param protocol IP protocol number 
 * @param ch_id Channel ID
 * @param m_id Media ID
 * @param result The result of RTP trap register 
 * @param tos TOS for IP header 
 * @param rfc2833_payload_type_local Local payload type of RFC2833
 * @param rfc2833_payload_type_remote Remote payload type of RFC2833
 * @param remoteSrtpKey Remote SRTP key
 * @param localSrtpKey Local SRTP key
 * @param remoteCryptAlg Remote crypto-algorithm 
 * @param localCryptAlg Local crypto-algorithm 
 * @param rtp_redundant_payload_type_local Local payload type of RTP redundant.
 * @param rtp_redundant_payload_type_remote Remote payload type of RTP redundant 
 * @param SID_payload_type_local Local payload type of SID 
 * @param SID_payload_type_remote Remoate payload type of SID 
 * @note When RTP redudant payload type is defined, it fills RTP 
 *       header's payload type field. 
 *       Original set payload types of @ref TstVoipMgrRtpCfg are used
 *       to fill RTP redundant extension header. 
 * @note SID payload type is used by codec whose timestamp clock 
 *       rate is not equal to 8000Hz, such as G.722. 
 *       Its range should be between 96 to 127, and just set it to
 *       be 0 in narrowband codec. 
 * @see do_mgr_VOIP_MGR_SET_SESSION()
 */
typedef struct
{
	uint32 ip_src_addr;
	uint32 ip_dst_addr;
	uint16 udp_src_port;
	uint16 udp_dst_port;
	uint8 protocol;
	uint8 ch_id;
	int32 m_id;
	int32 result;
	uint8 tos;
	uint16 rfc2833_payload_type_local;
	uint16 rfc2833_payload_type_remote;
#ifdef CONFIG_RTK_VOIP_SRTP
	unsigned char remoteSrtpKey[SRTP_KEY_LEN];
	unsigned char localSrtpKey[SRTP_KEY_LEN];
	int remoteCryptAlg;
	int localCryptAlg;
#endif /*CONFIG_RTK_VOIP_SRTP*/
#ifdef SUPPORT_RTP_REDUNDANT
	uint16 rtp_redundant_payload_type_local;
	uint16 rtp_redundant_payload_type_remote;
#endif
	uint16 SID_payload_type_local;
	uint16 SID_payload_type_remote;
	//int32	ret_val;
}
TstVoipMgrSession;

#ifdef PCM_LOOP_MODE_CONTROL
typedef struct
{
	uint8 chid;
	uint8 sid;
	uint8 isLoopBack;
	uint32 ip_src_addr;
	uint32 ip_dst_addr;
	uint16 udp_src_port;
	uint16 udp_dst_port;
	//int32	ret_val;
}
TstVoipLoopBcakInfo;
#endif


/**
 * @brief Structure for RTCP session 
 * @param ip_src_addr Source IP address 
 * @param ip_dst_addr Destination IP address 
 * @param rtcp_src_port Source port 
 * @param rtcp_dst_port Destination port 
 * @param protocol Protocol number in IP header 
 * @param ch_id Channel ID
 * @param m_id Media ID 
 * @param result The result
 * @see do_mgr_VOIP_MGR_SET_RTCP_SESSION()
 */
typedef struct
{
	uint32 ip_src_addr;
	uint32 ip_dst_addr;
	uint16 rtcp_src_port;
	uint16 rtcp_dst_port;
	uint8 protocol;
	uint8 ch_id;
	int32 m_id;
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP
	int result;
#endif
	//int32 ret_val;
}
TstVoipRtcpSession;


typedef struct
{
	uint32 ch_id;
	uint8	enable;
	//int32	ret_val;
}
TstVoipTest;

/**
 * @brief Structure for payload type
 * @param ch_id Channel ID
 * @param m_id Media ID
 * @param local_pt Local payload type
 * @param remote_pt Remote payload type
 * @param uPktFormat RTK_VoIP predefined payload type 
 * @param nG73Type G.723 bitrate 
 * @param nFramePerPacket Frames per packet 
 * @param bVAD VAD is on or off 
 * @param result The result 
 * @param nJitterDelay Jitter buffer delay 
 * @param nMaxDelay Jitter buffer maximum delay 
 * @param nJitterFactor Jitter buffer optimization factor 
 * @param nG726Packing G.726 packing method 
 * @param bT38ParamEnable If false, default settings are adopted and nT38MaxBuffer and nT38RateMgt are ignored. 
 * @prarm nT38MaxBuffer T38 Max buffer size. It can be 200 to 600ms. (default: 500)
 * @param nT38RateMgt T38 Rate management. 1: don't pass tcf data, 2: pass tcf data. (default: 2)
 * @param nT38MaxRate T38 Max Rate. 0:2400, 1:4800, 2:7200, 3:9600, 4:12000, 5:14400 (default: 5)
 * @param bT38EnableECM T38 ECM Enabled. 0: disable, 1: enable (default: 1)
 * @param nPcmMode PCM Mode Setting. 0: No action(use previous PCM enable mode), 1: Auto mode(DSP will change to proper mode according to the voice codec attribute), 2: Narrow-band Mode, 3: Wide-band Mode
 * @see do_mgr_VOIP_MGR_SETRTPPAYLOADTYPE()
 */
typedef struct
{
	uint32 ch_id;
	uint32 m_id;
	RtpPayloadType local_pt; // payload type in local
	RtpPayloadType remote_pt; // payload type in remote answer
	RtpPayloadType uPktFormat; // payload type in rtk_voip
	RtpPayloadType local_pt_vbd; // vbd payload type in local
	RtpPayloadType remote_pt_vbd; // vbd payload type in remote answer
	RtpPayloadType uPktFormat_vbd; // vbd payload type in rtk_voip
	int32 nG723Type;
	int32 nFramePerPacket;
	int32 bVAD;                                     // bool
	int32 result;
	uint32 nJitterDelay;
	uint32 nMaxDelay;
	uint32 nJitterFactor;
	uint32 nG726Packing;
	uint32 bT38ParamEnable;	// 1 to enable T38 parameters 
	uint32 nT38MaxBuffer;	// default: 500
	uint32 nT38RateMgt;		// default: 2
	uint32 nT38MaxRate; 	// default: 5, 0~5,: 2400, 4800, 7200, 9600, 12000, 14400
	uint32 bT38EnableECM;	// default: 1
	uint32 nPcmMode;
	//int32	ret_val;
}
TstVoipPayLoadTypeConfig;

/**
 * @brief Structure for RTP session state 
 * @param ch_id Channel ID
 * @param m_id Media ID
 * @param state RTP state
 * @see do_mgr_VOIP_MGR_SETRTPSESSIONSTATE()
 */
typedef struct
{
	uint32 ch_id;
	uint32 m_id;
	uint8	state;
	//int32	ret_val;
}
TstVoipRtpSessionState;

/**
 * @brief Structure for playing tone 
 * @param ch_id Channel ID
 * @param m_id Media ID
 * @param nTone Tone ID
 * @param bFlag Play or stop a tone
 * @param path Play tone locally or remotely 
 * @see do_mgr_VOIP_MGR_SETPLAYTONE()
 */
typedef struct
{
	uint32 ch_id;
	uint32 m_id;
	DSPCODEC_TONE nTone;
	uint32 bFlag;
	DSPCODEC_TONEDIRECTION path;
	//int32	ret_val;
}
TstVoipPlayToneConfig;

/**
 * @brief Structure for general purpose control. 
 * @param ch_id Channel ID
 * @param m_id Media ID
 * @param t_id Transaction (active) ID
 * @param enable Enable or disable 
 * @param cfg Configuration value 
 * @see do_mgr_VOIP_MGR_RTP_CFG() do_mgr_VOIP_MGR_UNSET_SESSION()
 *      do_mgr_VOIP_MGR_HOLD() do_mgr_VOIP_MGR_CTRL_RTPSESSION()
 *      do_mgr_VOIP_MGR_CTRL_TRANSESSION_ID() do_mgr_VOIP_MGR_UNSET_RTCP_SESSION()
 *      do_mgr_VOIP_MGR_ON_HOOK_RE_INIT() do_mgr_VOIP_MGR_FAX_MODEM_PASS_CFG()
 *      do_mgr_VOIP_MGR_FAX_OFFHOOK() do_mgr_VOIP_MGR_FAX_END_DETECT()
 *      do_mgr_VOIP_MGR_SET_FAX_MODEM_DET() do_mgr_VOIP_MGR_FAX_DIS_DETECT()
 *      do_mgr_VOIP_MGR_SET_G168_LEC_CFG()
 *      do_mgr_VOIP_MGR_DTMF_DET_THRESHOLD()
 *      do_mgr_VOIP_MGR_DTMF_CFG() do_mgr_VOIP_MGR_SET_DTMF_MODE()
 *      do_mgr_VOIP_MGR_SEND_RFC2833_BY_AP() do_mgr_VOIP_MGR_SET_CID_DET_MODE()
 *      do_mgr_VOIP_MGR_SET_CID_FSK_GEN_MODE() do_mgr_VOIP_MGR_SET_PULSE_DIGIT_DET()
 *      do_mgr_VOIP_MGR_SET_DIAL_MODE() do_mgr_VOIP_MGR_GET_DIAL_MODE()
 *      do_mgr_VOIP_MGR_PCM_CFG() do_mgr_VOIP_MGR_GET_SLIC_STAT()
 *      do_mgr_VOIP_MGR_SLIC_ONHOOK_ACTION() do_mgr_VOIP_MGR_SLIC_OFFHOOK_ACTION()
 *      do_mgr_VOIP_MGR_HOOK_FIFO_IN() do_mgr_VOIP_MGR_DAA_RING()
 *      do_mgr_VOIP_MGR_DAA_OFF_HOOK() do_mgr_VOIP_MGR_DAA_ON_HOOK()
 *      do_mgr_VOIP_MGR_FXO_ON_HOOK() do_mgr_VOIP_MGR_FXO_OFF_HOOK()
 *      do_mgr_VOIP_MGR_SIP_REGISTER() do_mgr_VOIP_MGR_GET_FEATURE()
 *      do_mgr_VOIP_MGR_VOIP_RESOURCE_CHECK()
 */
typedef struct
{
	uint32 ch_id;
	uint32 m_id;
	uint32 t_id;
	uint32 enable; //0-> off, 1->ON
	uint32 cfg;
	uint32 cfg2;
	uint32 cfg3;
	//int32	ret_val;
}
TstVoipCfg;

/**
 * @brief Structure for general purpose control. 
 * @param ch_id Channel ID
 * @param m_id Media ID
 * @param value Configuration value 
 * @param value1 Configuration value1 
 * @param value2 Configuration value2 
 * @param value3 Configuration value3 
 * @param value4 Configuration value4 
 * @param value5 Configuration value5 
 * @param value6 Configuration value6 
 * @param value7 Configuration value7 
 * @see do_mgr_VOIP_MGR_SET_RTCP_TX_INTERVAL() do_mgr_VOIP_MGR_SET_VOICE_GAIN()
 *      do_mgr_VOIP_MGR_ENERGY_DETECT() do_mgr_VOIP_MGR_DSPCODECSTOP()
 *      do_mgr_VOIP_MGR_SET_ECHO_TAIL_LENGTH() do_mgr_VOIP_MGR_DTMF_GET()
 *      do_mgr_VOIP_MGR_PLAY_SIP_INFO() do_mgr_VOIP_MGR_SET_TONE_OF_COUNTRY()
 *      do_mgr_VOIP_MGR_SET_TONE_OF_CUSTOMIZE() do_mgr_VOIP_MGR_USE_CUST_TONE()
 *      do_mgr_VOIP_MGR_SET_SPK_AGC() do_mgr_VOIP_MGR_SET_SPK_AGC_LVL()
 *      do_mgr_VOIP_MGR_SET_SPK_AGC_GUP() do_mgr_VOIP_MGR_SET_SPK_AGC_GDOWN()
 *      do_mgr_VOIP_MGR_SET_MIC_AGC() do_mgr_VOIP_MGR_SET_MIC_AGC_LVL()
 *      do_mgr_VOIP_MGR_SET_MIC_AGC_GUP() do_mgr_VOIP_MGR_SET_MIC_AGC_GDOWN()
 *      do_mgr_VOIP_MGR_PULSE_DIAL_GEN_CFG() do_mgr_VOIP_MGR_GEN_PULSE_DIAL()
 *      do_mgr_VOIP_MGR_SET_PCM_LOOP_MODE() do_mgr_VOIP_MGR_SET_SLIC_TX_GAIN()
 *      do_mgr_VOIP_MGR_SET_SLIC_RX_GAIN() do_mgr_VOIP_MGR_SET_SLIC_RING_CADENCE()
 *      do_mgr_VOIP_MGR_FLUSH_FIFO() do_mgr_VOIP_MGR_LINE_CHECK()
 *      do_mgr_VOIP_MGR_SET_RING_DETECTION() do_mgr_VOIP_MGR_SET_DAA_TX_GAIN()
 *      do_mgr_VOIP_MGR_SET_DAA_RX_GAIN() do_mgr_VOIP_MGR_SET_DAA_ISR_FLOW()
 *      do_mgr_VOIP_MGR_GET_DAA_ISR_FLOW() do_mgr_VOIP_MGR_SET_DAA_PCM_HOLD_CFG()
 *      do_mgr_VOIP_MGR_GET_DAA_BUSY_TONE_STATUS() do_mgr_VOIP_MGR_GET_DAA_USED_BY_WHICH_SLIC()
 *      do_mgr_VOIP_MGR_DEBUG() 
 */
typedef struct
{
	uint32 ch_id;
	uint32 m_id;
	int8	value;
	int8	value1;
	int8	value2;
	int8	value3;
	int8	value4;
	int16	value5;
	int16	value6;
	int16  value7;
	//int32	ret_val;
}
TstVoipValue;

/**
 * @brief Structure for retrieving hook status 
 * @param ch_id Channel ID
 * @param change Hook status is changed 
 * @param hook_status Current hook status 
 * @see do_mgr_VOIP_MGR_SLIC_HOOK()
 */
typedef struct
{
	unsigned char ch_id;          // CH:0 - 3
	unsigned char change;         // 1: Change. 0: No-Change
	unsigned char hook_status;    // 1: Off-Hook, 0: On-Hook
	//int32	ret_val;
}
TstVoipSlicHook;

/**
 * @brief Structure for SLIC ring 
 * @param ch_id channel ID
 * @param ring_set Turn on (1) or off (0) ring
 * @see do_mgr_VOIP_MGR_SLIC_RING()
 */
typedef struct
{
	unsigned char ch_id;          // CH = 0 ~ 3
	unsigned char ring_set;       // Ring_ON: ring_set = 1 , Ring_OFF: ring_set = 0
	//int32	ret_val;
}
TstVoipSlicRing;

/**
 * @brief Structure for SLIC to generate tone 
 * @param ch_id Channel ID
 * @param tone2play The tone to play 
 * @warning Now, we use DSP to generate tone. 
 * @see do_mgr_VOIP_MGR_SLIC_TONE() VOIP_DSP_TONE
 */
typedef struct
{
	unsigned char ch_id;
	unsigned int tone2play;
	//int32	ret_val;
}
TstVoipSlicTone;

/**
 * @brief Structure for SLIC restart 
 * @param ch_id Channel ID
 * @param codec_law Codec mode (It suggests a-law (1))
 * @see do_mgr_VOIP_MGR_SLIC_RESTART() 
 */
typedef struct
{
	unsigned char ch_id;
	unsigned int codec_law; //linear: 0, a_law: 1, u_law: 2
	//int32	ret_val;
}
TstVoipSlicRestart;


/**
 * @brief Structure for caller ID 
 * @param ch_id Channel ID
 * @param daa_id DAA ID
 * @param cid_state Caller ID state 
 * @param cid_mode Caller ID mode 
 * @param cid_gain Caller ID gain 
 * @param cid_msg_type Caller ID message type 
 * @param string Caller ID - Phonenumber  
 * @param string2 Caller ID - Date and time 
 * @param cid_name Caller ID - Name 
 * @param cid_dtmf_mode Caller ID in DTMF mode. <br>
 *        0-1 bits for starting digit, and 2-3 bits for ending digit. <br>
 *        00: A, 01: B, 02: C, 03: D
 * @see do_mgr_VOIP_MGR_DTMF_CID_GEN_CFG() do_mgr_VOIP_MGR_GET_CID_STATE_CFG()
 *      do_mgr_VOIP_MGR_FSK_CID_GEN_CFG() do_mgr_VOIP_MGR_SET_FSK_VMWI_STATE()
 *      do_mgr_VOIP_MGR_SET_FSK_AREA() do_mgr_VOIP_MGR_FSK_ALERT_GEN_CFG()
 *      do_mgr_VOIP_MGR_SET_CID_DTMF_MODE() do_mgr_VOIP_MGR_GET_FSK_CID_STATE_CFG()
 *      do_mgr_VOIP_MGR_FSK_CID_VMWI_GEN_CFG() do_mgr_VOIP_MGR_GET_DAA_CALLER_ID()
 */
typedef struct
{
	unsigned char ch_id;
	unsigned char daa_id;
	unsigned char cid_state;
	unsigned char cid_mode;
	unsigned char cid_gain;
	unsigned char cid_msg_type;
	char string[21];
	char string2[9];	// for date & time sync
	char cid_name[51];
	unsigned char cid_dtmf_mode;
	//int32	ret_val;
}
TstVoipCID;//For dtmf cid generation


typedef struct
{
	unsigned long time_out;
	unsigned char cid_setup;
	unsigned char cid_mode;		/* 0:don't need ack 'D', 1:ETSI need ack 'D' */
	unsigned char cid_states;
	unsigned char cid_complete;
	char d_play_cnt;
	//int32	ret_val;

}
TstVoipFskT2Cid_det;//For fsk type2 caller id detect

typedef struct
{
	unsigned long time_out;
	unsigned char cid_setup;
	unsigned char cid_mode;
	unsigned char cid_msg_type;
	unsigned char cid_states;
	unsigned char cid_complete;
	char cid_number[21];
	char cid_date[9];	// for date & time sync
	char cid_name[51];
	//int32	ret_val;
}
TstVoipFskCid;//For fsk caller id gen


typedef struct
{
	unsigned char ch_id;
	TfskArea area;				/* area -> 0:Bellcore 1:ETSI 2:BT 3:NTT */
	unsigned int CS_cnt;			/* channel seizure signal bit count */
	unsigned int mark_cnt;			/* mark signal bit count */
	TfskClidGain mark_gain;			/* unit in dB */
	TfskClidGain space_gain;		/* unit in dB */
	unsigned int type2_expected_ack_tone;	/* DTMF A, or B, or C, or D*/
	unsigned int delay_after_1st_ring;	/* unit in ms */
	unsigned int delay_before_2nd_ring;	/* unit in ms */
	//int32	ret_val;
}
TstVoipFskPara;//For fsk caller id gen para setting

// The 2 definition should be the same to the definitions in fskcid_gen.h
#define MAX_CLID_DATA_SIZE	80
#define FSK_MDMF_SIZE		5

// This structure should be the same to the structure TstFskClidData in fskcid_gen.h
typedef struct
{
        TfskParaType type;
        char data[MAX_CLID_DATA_SIZE];
}
TstFskClid_Data;

typedef struct
{
        unsigned char ch_id;
        unsigned char service_type;
        TstFskClid_Data cid_data[FSK_MDMF_SIZE];
        //int32	ret_val;
}
TstFskClid;


/**
 * @brief Structure for sending RFC2833 packet 
 * @param ch_id Channel ID 
 * @param sid Session ID
 * @param digit A digit to send 
 * @param duration DTMF duration 
 * @see do_mgrVOIP_MGR_SEND_RFC2833_PKT_CFG()
 */
typedef struct
{
	unsigned char ch_id;
	unsigned char sid;
	unsigned char digit;
	unsigned int duration;
	unsigned char bEnable;
    unsigned int volume;
	//int32	ret_val;

}
TstVoip2833;//For sending RFC2833 packet

/**
 * @brief Structure for reading and writing SLIC register 
 * @param ch_id Channel ID
 * @param reg_num Register number 
 * @param reg_len Number of Register bytes (Max 16) 
 * @param reg_ary for Register value 
 * @see do_mgr_VOIP_MGR_GET_SLIC_REG_VAL() do_mgr_VOIP_MGR_SET_SLIC_REG_VAL()
 */
typedef struct
{
	unsigned char ch_id;
	unsigned int reg_num;
	unsigned char reg_len; 
	unsigned char reg_ary[16];
	//int32	ret_val;
}
TstVoipSlicReg;

/**
 * @brief Structure for reading and writing SLIC RAM 
 * @param ch_id Channel ID
 * @param reg_num RAM number 
 * @param reg_val RAM value 
 * @see do_mgr_VOIP_MGR_GET_SLIC_RAM_VAL() do_mgr_VOIP_MGR_SET_SLIC_RAM_VAL()
 */
typedef struct
{
	unsigned char ch_id;
	unsigned short reg_num;
	unsigned int reg_val;
	//int32	ret_val;
}
TstVoipSlicRam;

typedef struct
{
	unsigned char ch_id;
	unsigned char s_id;
	RtpPayloadType uPktFormat;
	unsigned char channel_enable;
	//int32	ret_val;
}
TstTwoChannelCfg; //for two channel simplify g729

/**
 * @brief Structure for setting flash hook time
 * @param ch_id Channel ID
 * @param flash_time Upper bound of flash hook time (ms) 
 * @param flash_time_min Lower bound of flash hook time (ms) 
 * @param dummy Dummy
 * @see do_mgr_VOIP_MGR_SET_FLASH_HOOK_TIME()
 */
typedef struct
{
	unsigned char ch_id;
	unsigned int  flash_time;
	unsigned int  flash_time_min;
	unsigned int  dummy;
	//int32	ret_val;
}
TstVoipHook;


/**
 * @brief Structure for tone configuration 
 * @see do_mgr_VOIP_MGR_SET_CUST_TONE_PARAM()
 */
typedef struct
{
	unsigned long	toneType;	///< see _TONE_TYPE_
	unsigned short	cycle;		///< "<0": illegal value, "0": represent "continuous tone", ">0": cycle number

	unsigned short	cadNUM;		///< Cadence number (in SUCC and SUCC_ADD mode, it represent repeat number of sequence)

	unsigned long	CadOn0;		///< Cadence On0 time (ms)
	unsigned long	CadOn1;		///< Cadence On1 time (ms)
	unsigned long	CadOn2;		///< Cadence On2 time (ms)
	unsigned long	CadOn3;		///< Cadence On3 time (ms)
	unsigned long	CadOff0;	///< Cadence Off0 time (ms)
	unsigned long	CadOff1;	///< Cadence Off1 time (ms)
	unsigned long	CadOff2;	///< Cadence Off2 time (ms)
	unsigned long	CadOff3;	///< Cadence Off3 time (ms)

	unsigned long PatternOff;	///< pattern Off time (ms)
	unsigned long ToneNUM;		///< tone number (1..4)

	unsigned long	Freq1;		///< Freq1 (Hz)
	unsigned long	Freq2;		///< Freq2 (Hz)
	unsigned long	Freq3;		///< Freq3 (Hz)
	unsigned long	Freq4;		///< Freq4 (Hz)

	long Gain1;					///< Gain1 (db)
	long Gain2;					///< Gain2 (db)
	long Gain3;					///< Gain3 (db)
	long Gain4;					///< Gain4 (db)
	//int32	ret_val;
}
TstVoipToneCfg;

/**
 * @brief Structure for disconnection tone detection 
 * @see do_mgr_VOIP_MGR_SET_DIS_TONE_DET
 */
typedef struct
{
	unsigned int distone_num;	///< the dis-connect tone number 0, 1, 2, 3, 4 

	unsigned int tone1_accuracy;	///< defaut:16, larger value more accuracy , lesser value more tolerance in frequency 
	unsigned int tone1_level;	///< default:800, larger valre need more amplitude 
	unsigned int tone1_frequency1; ///< the first dis-connect tone detect frequency unit: Hz. samling rate = 8000hz, allow 1~ 3999
	unsigned int tone1_frequency2; ///< the second dis-connect tone detect frequency unit: Hz. samling rate = 8000hz, allow 0~ 3999 
	                         /*  0 for only one frequency. */
	int tone1_distone_on_low_limit; ///< the lower limit of dis-tone ON time unit:10ms, ex 0.5s=500ms = 50unit  
	int tone1_distone_on_up_limit;  ///< the upper limit of dis-tone ON time unit:10ms 
	int tone1_distone_off_low_limit; ///< the lower limit of dis-tone OFF time unit:10ms
	int tone1_distone_off_up_limit;  ///< the upper limit of dis-tone OFF time unit:10ms

	unsigned int tone2_accuracy;	///< defaut:16, larger value more accuracy , lesser value more tolerance in frequency 
	unsigned int tone2_level;	///< default:800, larger valre need more amplitude 
	unsigned int tone2_frequency1; ///< the first dis-connect tone detect frequency unit: Hz. samling rate = 8000hz, allow 1~ 3999
	unsigned int tone2_frequency2; ///<the second dis-connect tone detect frequency unit: Hz. samling rate = 8000hz, allow 0~ 3999
	                         /*  0 for only one frequency. */
	int tone2_distone_on_low_limit; ///< the lower limit of dis-tone ON time unit:10ms, ex 0.5s=500ms = 50unit 
	int tone2_distone_on_up_limit;  ///< the upper limit of dis-tone ON time unit:10ms 
	int tone2_distone_off_low_limit; ///< the lower limit of dis-tone OFF time unit:10ms 
	int tone2_distone_off_up_limit;  ///< the upper limit of dis-tone OFF time unit:10ms

	unsigned int tone3_accuracy;	///< defaut:16, larger value more accuracy , lesser value more tolerance in frequency 
	unsigned int tone3_level;	///< default:800, larger valre need more amplitude 
	unsigned int tone3_frequency1; ///< the first dis-connect tone detect frequency unit: Hz. samling rate = 8000hz, allow 1~ 3999
	unsigned int tone3_frequency2; ///<the second dis-connect tone detect frequency unit: Hz. samling rate = 8000hz, allow 0~ 3999
	                         /*  0 for only one frequency. */
	int tone3_distone_on_low_limit; ///< the lower limit of dis-tone ON time unit:10ms, ex 0.5s=500ms = 50unit 
	int tone3_distone_on_up_limit;  ///< the upper limit of dis-tone ON time unit:10ms 
	int tone3_distone_off_low_limit; ///< the lower limit of dis-tone OFF time unit:10ms 
	int tone3_distone_off_up_limit;  ///< the upper limit of dis-tone OFF time unit:10ms

	unsigned int tone4_accuracy;	///< defaut:16, larger value more accuracy , lesser value more tolerance in frequency 
	unsigned int tone4_level;	///< default:800, larger valre need more amplitude 
	unsigned int tone4_frequency1; ///< the first dis-connect tone detect frequency unit: Hz. samling rate = 8000hz, allow 1~ 3999
	unsigned int tone4_frequency2; ///<the second dis-connect tone detect frequency unit: Hz. samling rate = 8000hz, allow 0~ 3999
	                         /*  0 for only one frequency. */
	int tone4_distone_on_low_limit; ///< the lower limit of dis-tone ON time unit:10ms, ex 0.5s=500ms = 50unit 
	int tone4_distone_on_up_limit;  ///< the upper limit of dis-tone ON time unit:10ms 
	int tone4_distone_off_low_limit; ///< the lower limit of dis-tone OFF time unit:10ms 
	int tone4_distone_off_up_limit;  ///< the upper limit of dis-tone OFF time unit:10ms

	//int32	ret_val;
}
TstVoipdistonedet_parm;

/**
 * @brief Structure for 8305/8306 manipulation 
 * @param phy Physical ID 
 * @param reg Resgiter number 
 * @param value Register value 
 * @param read_write Read or write register value 
 * @see do_mgr_VOIP_MGR_8305_SWITCH_VAL()
 */
typedef struct
{
	unsigned short phy;
	unsigned short reg;
	unsigned short value;
	unsigned short read_write;
	//int32	ret_val;
} TstVoipSwitch;

#define MAX_LEN_OF_IVR_TEXT		40
/**
 * @brief Structure for playing textual IVR
 * @param ch_id Defined in @ref TstVoipPlayIVR_Header
 * @param type Defined in @ref TstVoipPlayIVR_Header
 * @param playing_period_10ms Defined in @ref TstVoipPlayIVR_Header
 * @param ret1_unused Unused parameter 
 * @param direction Play IVR locally or remotely 
 * @param szText2speech String for playing 
 * @see TstVoipPlayIVR_Header
 */
typedef struct {
	unsigned char ch_id;
	IvrPlayType_t type;		/* == IVR_PLAY_TYPE_TEXT */
	unsigned int playing_period_10ms;	/* output */
	unsigned int ret1_unused;			/* output */
	/* --- Above part should be identical to TstVoipPlayIVR_Header --- */
	IvrPlayDir_t direction;
	unsigned char szText2speech[ MAX_LEN_OF_IVR_TEXT + 1 ];
	//int32	ret_val;
} TstVoipPlayIVR_Text;

#define MAX_FRAMES_OF_G72363	10
/**
 * @brief Structure for playing G.723-6.3k IVR
 * @param ch_id Defined in @ref TstVoipPlayIVR_Header
 * @param type Defined in @ref TstVoipPlayIVR_Header
 * @param playing_period_10ms Defined in @ref TstVoipPlayIVR_Header
 * @param nRetCopiedFrames Number of frames are copied, so the residual has to be given again 
 * @param nFramesCount Number of frames in data buffer 
 * @param data G.723 6.3k data 
 * @note nRetCopiedFrames should be smaller to equal to nFramesCount
 * @see TstVoipPlayIVR_Header TstVoipPlayIVR_Header_v2
 */
typedef struct {
	unsigned char ch_id;
	IvrPlayType_t type;		/* == IVR_PLAY_TYPE_G723_63 */
	unsigned int playing_period_10ms;	/* output */
	unsigned int nRetCopiedFrames;		/* output */
	/* --- Above part should be identical to TstVoipPlayIVR_Header --- */
	unsigned int  nFramesCount;
	unsigned char data[ 24 * MAX_FRAMES_OF_G72363 ];
	/* --- Above part should be identical to TstVoipPlayIVR_Header_v2 --- */
	//int32	ret_val;
} TstVoipPlayIVR_G72363;

#define MAX_FRAMES_OF_G729	10
/**
 * @brief Structure for playing G.729 IVR 
 * @param ch_id Defined in @ref TstVoipPlayIVR_Header
 * @param type Defined in @ref TstVoipPlayIVR_Header
 * @param playing_period_10ms Defined in @ref TstVoipPlayIVR_Header
 * @param nRetCopiedFrames Number of frames are copied, so the residual has to be given again 
 * @param nFramesCount Number of frames in data buffer 
 * @param data G.729 data 
 * @note nRetCopiedFrames should be smaller to equal to nFramesCount
 * @see TstVoipPlayIVR_Header TstVoipPlayIVR_Header_v2
 */
typedef struct {
	unsigned char ch_id;
	IvrPlayType_t type;		/* == IVR_PLAY_TYPE_G729 */
	unsigned int playing_period_10ms;	/* output */
	unsigned int nRetCopiedFrames;		/* output */
	/* --- Above part should be identical to TstVoipPlayIVR_Header --- */
	unsigned int  nFramesCount;
	unsigned char data[ 10 * MAX_FRAMES_OF_G729 ];
	/* --- Above part should be identical to TstVoipPlayIVR_Header_v2 --- */
	//int32	ret_val;
} TstVoipPlayIVR_G729;

#define MAX_FRAMES_OF_G711	10
/**
 * @brief Structure for playing G.711 IVR 
 * @param ch_id Defined in @ref TstVoipPlayIVR_Header
 * @param type Defined in @ref TstVoipPlayIVR_Header
 * @param playing_period_10ms Defined in @ref TstVoipPlayIVR_Header
 * @param nRetCopiedFrames Number of frames are copied, so the residual has to be given again 
 * @param nFramesCount Number of frames in data buffer 
 * @param data G.711 data 
 * @note nRetCopiedFrames should be smaller to equal to nFramesCount
 * @see TstVoipPlayIVR_Header TstVoipPlayIVR_Header_v2
 */
typedef struct {
	unsigned char ch_id;
	IvrPlayType_t type;		/* == IVR_PLAY_TYPE_G711A */
	unsigned int playing_period_10ms;	/* output */
	unsigned int nRetCopiedFrames;		/* output */
	/* --- Above part should be identical to TstVoipPlayIVR_Header --- */
	unsigned int  nFramesCount;
	unsigned char data[ 80 * MAX_FRAMES_OF_G711 ];
	/* --- Above part should be identical to TstVoipPlayIVR_Header_v2 --- */
	//int32	ret_val;
} TstVoipPlayIVR_G711;

#define MAX_FRAMES_OF_LINEAR_8K	10
/**
 * @brief Structure for playing Linear 8k IVR 
 * @param ch_id Defined in @ref TstVoipPlayIVR_Header
 * @param type Defined in @ref TstVoipPlayIVR_Header
 * @param playing_period_10ms Defined in @ref TstVoipPlayIVR_Header
 * @param nRetCopiedFrames Number of frames are copied, so the residual has to be given again 
 * @param nFramesCount Number of frames in data buffer 
 * @param data linear 8k data 
 * @note nRetCopiedFrames should be smaller to equal to nFramesCount
 * @see TstVoipPlayIVR_Header TstVoipPlayIVR_Header_v2
 */
typedef struct {
	unsigned char ch_id;
	IvrPlayType_t type;		/* == IVR_PLAY_TYPE_LINEAR_8K */
	unsigned int playing_period_10ms;	/* output */
	unsigned int nRetCopiedFrames;		/* output */
	/* --- Above part should be identical to TstVoipPlayIVR_Header --- */
	unsigned int  nFramesCount;
	unsigned char data[ 160 * MAX_FRAMES_OF_LINEAR_8K ];
	/* --- Above part should be identical to TstVoipPlayIVR_Header_v2 --- */
	//int32	ret_val;
} TstVoipPlayIVR_8K;

#define MAX_FRAMES_OF_LINEAR_16K	10
/**
 * @brief Structure for playing Linear 16k IVR 
 * @param ch_id Defined in @ref TstVoipPlayIVR_Header
 * @param type Defined in @ref TstVoipPlayIVR_Header
 * @param playing_period_10ms Defined in @ref TstVoipPlayIVR_Header
 * @param nRetCopiedFrames Number of frames are copied, so the residual has to be given again 
 * @param nFramesCount Number of frames in data buffer 
 * @param data linear 16k data 
 * @note nRetCopiedFrames should be smaller to equal to nFramesCount
 * @see TstVoipPlayIVR_Header TstVoipPlayIVR_Header_v2
 */
typedef struct {
	unsigned char ch_id;
	IvrPlayType_t type;		/* == IVR_PLAY_TYPE_LINEAR_16K */
	unsigned int playing_period_10ms;	/* output */
	unsigned int nRetCopiedFrames;		/* output */
	/* --- Above part should be identical to TstVoipPlayIVR_Header --- */
	unsigned int  nFramesCount;
	unsigned char data[ 320 * MAX_FRAMES_OF_LINEAR_16K ];
	/* --- Above part should be identical to TstVoipPlayIVR_Header_v2 --- */
	//int32	ret_val;
} TstVoipPlayIVR_16K;

/**
 * @brief General structure header for playing IVR
 * @param ch_id Channel ID 
 * @param type Type of codec 
 * @param playing_period_10ms How much data in buffer 
 * @param ret1 Used by various codec 
 * @see do_mgr_VOIP_MGR_PLAY_IVR()
 */
typedef struct {
	unsigned char ch_id;
	IvrPlayType_t type;
	unsigned int playing_period_10ms;	/* output */
	unsigned int ret1;					/* output, each codec has different meaning */
	//int32	ret_val;
} TstVoipPlayIVR_Header;

/**
 * @brief General structure header version 2 for playing IVR
 * @param ch_id Channel ID 
 * @param type Type of codec 
 * @param playing_period_10ms How much data in buffer 
 * @param nRetCopiedFrames Number of frames are copied, so the residual has to be given again 
 * @param nFramesCount Number of frames in data buffer 
 * @param data Data for each codec 
 * @note nRetCopiedFrames should be smaller to equal to nFramesCount
 * @see do_mgr_VOIP_MGR_PLAY_IVR() TstVoipPlayIVR_Header
 */
typedef struct {
	unsigned char ch_id;
	IvrPlayType_t type;
	unsigned int playing_period_10ms;	/* output */
	unsigned int nRetCopiedFrames;		/* output */
	/* --- Above part should be identical to TstVoipPlayIVR_Header --- */
	unsigned int  nFramesCount;
	unsigned char data[ 1 ];
} TstVoipPlayIVR_Header_v2;

/**
 * @brief Structure for polling IVR
 * @param ch_id Channel ID 
 * @param bPlaying If it is playing, 1 is given. Otherwise, 0 is given. 
 * @see do_mgr_VOIP_MGR_POLL_IVR()
 */
typedef struct {
	unsigned char ch_id;
	unsigned char bPlaying;		/* output: play or not */
	//int32	ret_val;
} TstVoipPollIVR;


/**
 * @brief Structure for stoping IVR
 * @param ch_id Channel ID 
 * @see do_mgr_VOIP_MGR_STOP_IVR()
 */
typedef struct {
	unsigned char ch_id;
	//int32	ret_val;
} TstVoipStopIVR;

/**
 * @brief Structure for WAN's VLAN tag 
 * @param enable Enable or disable VLAN 
 * @param vlanId VLAN ID
 * @param priority Priority 
 * @param cfi CFI 
 * @see do_mgr_VOIP_MGR_WAN_VLAN_TAG() 
 */
typedef struct
{
	unsigned char enable;
	unsigned short vlanId;
	unsigned char priority;
	unsigned char cfi;
} TstVoipSwitch_VLAN_tag;

/**
 * @brief Structure for WAN's 2 VLAN tag 
 * @param enable Enable or disable 2 VLAN 
 * @param vlanIdVoice VLAN ID for voice 
 * @param priorityVoice Priority for voice 
 * @param cfiVoice CFI for voice 
 * @param vlanIdData VLAN ID for data 
 * @param priorityData Priority for data 
 * @param cfiData CFI for data 
 * @see do_mgr_VOIP_MGR_WAN_2_VLAN_TAG() 
 */
typedef struct
{
	unsigned char enable;
	unsigned short vlanIdVoice;
	unsigned char priorityVoice;
	unsigned char cfiVoice;
	unsigned short vlanIdData;
	unsigned char priorityData;
	unsigned char cfiData;
} TstVoipSwitch_2_VLAN_tag;

/**
 * @brief Structure for WAN's 3 VLAN tag 
 * @param enable Enable or disable 3 VLAN 
 * @param vlanIdVoice VLAN ID for voice 
 * @param priorityVoice Priority for voice 
 * @param cfiVoice CFI for voice 
 * @param vlanIdData VLAND ID for data 
 * @param priorityData Priority for data 
 * @param cfiData CFI for data 
 * @param vlanIdVideo VLAND ID for video 
 * @param priorityVideo Priority for video 
 * @param cfiVideo CFI for video 
 * @see do_mgr_VOIP_MGR_WAN_3_VLAN_TAG()
 */
typedef struct
{
	unsigned char enable;
	unsigned short vlanIdVoice;
	unsigned char priorityVoice;
	unsigned char cfiVoice;
	unsigned short vlanIdData;
	unsigned char priorityData;
	unsigned char cfiData;
	unsigned short vlanIdVideo;
	unsigned char priorityVideo;
	unsigned char cfiVideo;
} TstVoipSwitch_3_VLAN_tag;

/**
 * @brief Structure for T38 PCM-in debug 
 * @param pcmIn Input PCM data 
 * @param snPcm Sequence number of PCM data 
 * @param ret Number of retrieved PCM data 
 * @note These code are comment! 
 * @see do_mgr_VOIP_MGR_GET_T38_PCMIN()
 */
typedef struct {
	unsigned short pcmIn[ 480 ];
	unsigned int snPcm;
	unsigned int ret;
	//int32	ret_val;
} TstT38PcmIn;

/**
 * @brief Structure for T38 packet-in debug 
 * @param packetIn Input packet data 
 * @param nSize Size of packet 
 * @param snPcm Sequence number of PCM data used to keep input timing 
 * @note These code are comment! 
 * @see do_mgr_VOIP_MGR_GET_T38_PACKETIN()
 */
typedef struct {
	unsigned short packetIn[ 300 ];
	unsigned int nSize;
	unsigned int snPcm;
	//int32	ret_val;
} TstT38PacketIn;

/**
 * @brief Structure for voice record debug 
 * @param ch_id Channel ID 
 * @param write_enable Initial writing. (0: initial, 1: do nothing)
 * @param length Retrieved voice data length 
 * @param mode Dump mode whose format is<br>
 *        DUMP_MODE_3 | [RECORD_PCM_HANDLER | RECORD_TX_SINE_RX_ECHO | RECORD_PCM_INTERFACE | RECORD_PCM_LEC]<br>
 *        Where, DUMP_MODE_3: 16, RECORD_PCM_HANDLER: 1, RECORD_TX_SINE_RX_ECHO: 2, RECORD_PCM_INTERFACE: 4, RECORD_PCM_LEC: 8
 * @param buf Retrieved voice data 
 * @note Using this debug function, one has to uncomment RTK_VOICE_RECORD in rtk_voip.h and <br>
 *       VOIPBOX_VOICERECORD in voipbox. 
 * @see do_mgr_VOIP_MGR_SET_GETDATA_MODE()
 */
typedef struct
{
	long ch_id;
	long write_enable;
	long length;
	long mode;// 1:tx, 0:rx
	char buf[1120];
	//int32	ret_val;
} TstVoipdataget_o;

typedef struct
{
	long tx_readindex;	//tx data read index
	long tx_writeindex;	//tx data write index
	long write_enable;
	long rx_readindex;
	long rx_writeindex;
	long rx_readindex2;
	long rx_writeindex2;
	char *txbuf;		//tx buffer
	char *rxbuf;		//rx buffer
	char *rxbuf2;		//rx2 buffer
	//int32	ret_val;
}
TstVoipdataget;


typedef struct
{
	long ch_id;
	long write_enable;
	long length;
	long mode;
	//int32	ret_val;
	char buf[1120];
} TstVoipdataput_o;

typedef struct
{
	long readindex;	//data read index
	long writeindex;	//data write index
	long write_enable;
	char *txbuf;		//tx buffer
	//int32	ret_val;
}
TstVoipdataput;

/**
 * @brief Structure for retrieving RTP statistics 
 * @param[in] chid Channel ID 
 * @param[in] bResetStatistics Reset statistics 
 * @param[out] nRxRtpStatsCountByte Number of Rx RTP bytes 
 * @param[out] nRxRtpStatsCountPacket Number of Rx RTP packets 
 * @param[out] nRxRtpStatsLostPacket Number of Rx packet loss
 * @param[out] nTxRtpStatsCountByte Number of Tx RTP bytes 
 * @param[out] nTxRtpStatsCountPacket Number of Tx RTP packets 
 * @see do_mgr_VOIP_MGR_GET_RTP_STATISTICS()
 */
typedef struct {
	unsigned char chid;						/* input */
	unsigned char bResetStatistics;			/* input */
	/* follows are output */
	unsigned long nRxRtpStatsCountByte;
	unsigned long nRxRtpStatsCountPacket;
	unsigned long nRxRtpStatsLostPacket;
	unsigned long nTxRtpStatsCountByte;
	unsigned long nTxRtpStatsCountPacket;
	//int32	ret_val;
} TstVoipRtpStatistics;

/**
 * @brief Structure for retrieving statistics by session 
 * @param[in] sid Session ID 
 * @param[in] bResetStatistics Reset statistics 
 * @param[out] nRxSilencePacket Number of silence packet 
 * @param[out] nTxSilencePacket Number of silence packet 
 * @param[out] nAvgPlayoutDelay Average playout delay (in unit of 1ms)
 * @param[out] nCurrentJitterBuf Current jitter buffering packets 
 * @param[out] nEarlyPacket Early packet for playout
 * @param[out] nLatePacket Late packet for playout 
 * @param[out] nSilenceSpeech Count of playout a silence speech (in unit of 10ms)
 */
typedef struct {
	unsigned char chid;						/* input */
	unsigned char sid;						/* input */
	unsigned char bResetStatistics;			/* input */
	/* follows are output */
	unsigned long nRxSilencePacket;
	unsigned long nTxSilencePacket;
	unsigned long nAvgPlayoutDelay;
	unsigned long nCurrentJitterBuf;
	unsigned long nEarlyPacket;
	unsigned long nLatePacket;
	unsigned long nSilenceSpeech;
	//int32	ret_val;
} TstVoipSessionStatistics;

typedef struct 
{
	unsigned int event;
	unsigned int marker;
	unsigned int duration;
	unsigned int volume;
	unsigned int edge;
	unsigned int play; // for check if play or not
	//int32	ret_val;
}
TstRfc2833event;

typedef struct 
{
	Voip_reosurce_t resource;
	TstVoipMgr3WayCfg _3way_session_info;
	//int32	ret_val;
} TstVoipResourceCheck;

/**
 * @brief Structure for WAN's clone MAC. 
 * @param CloneMACAddress Clone MAC address 
 * @see do_mgr_VOIP_MGR_SET_WAN_CLONE_MAC()
 */
//add by Tim 1/8/2008
typedef struct
{
	unsigned char CloneMACAddress[6];
}TstVoipCloneMAC; 

/**
 * @brief Structure for port link status. 
 * @param status Port link status, and its value is resolved by following:<br>
 *        PORT_LINK_LAN_ALL		0x0000000F	<br>
 *        PORT_LINK_LAN0		0x00000001	<br>
 *        PORT_LINK_LAN1		0x00000002	<br>
 *        PORT_LINK_LAN2		0x00000004	<br>
 *        PORT_LINK_LAN3		0x00000008	<br>
 *        PORT_LINK_RESERVED1	0x000000F0	<br>
 *        PORT_LINK_WAN			0x00000100	<br>
 * @see do_mgr_VOIP_MGR_GET_PORT_LINK_STATUS()
 */
typedef struct {
	uint32 status;
} TstVoipPortLinkStatus;

/**
 * @brief Structure for bandwidth management. 
 * @param port Port ID 
 * @param dir Direction. 0: ingress, 1: egress
 * @param ban Bandwidth 
 * @see do_mgr_VOIP_MGR_BANDWIDTH_MGR()
 */
typedef struct
{
	unsigned int port;
	unsigned int dir;
	unsigned int ban;
}TstVoipBandwidthMgr; 

/**
 * @brief Structure for Port Config. 
 * @param port Port ID 
 * @param port conifg. 0: enable, 1: disable
 * @see do_mgr_VOIP_MGR_PORT_HIGH_PRIORITY()
 * @see do_mgr_VOIP_MGR_PORT_DISABLE_FLOWCONTROL ()
 * @see do_mgr_VOIP_MGR_PORT_DISABLE()
*/
typedef struct
{
	unsigned int port;
	unsigned int config;
}TstVoipPortConfig;
 
/**
 * @brief Structure for GPIO read, write and initialization 
 * @see do_mgr_VOIP_MGR_GPIO()
 */
// rock 2008.1.16
typedef struct {
	unsigned int action;	//!< init=0, read=1, write=2
	unsigned int pid;		//!< GPIO id = (gpio_group << 16) | (gpip_pin & 0xffff)
	unsigned int value;		//!< GPIO read/write: 						<br>
							//!<	- bit 0: disable = 0, enable = 1	<br>
							//!< GPIO init:								<br>
							//!<	- bit0~1: GPIO_PERIPHERAL			<br>
							//!<	- bit2: GPIO_DIRECTION				<br>
							//!<	- bit3: GPIO_INTERRUPT_TYPE			<br>
	unsigned int result;	//!< 0: success, 1: failed
	//int32	ret_val;
} TstVoipGPIO;

/**
 * @brief Structure for single value ioctl
 * @param value Value 
 * @see do_mgr_VOIP_MGR_DECT_SET_POWER()
 * @see do_mgr_VOIP_MGR_DECT_GET_POWER()
 * @see do_mgr_VOIP_MGR_DECT_GET_PAGE()
*/
typedef struct
{
	uint32 value;
} TstVoipSingleValue;

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP
typedef struct 
{
	unsigned char chid;
	unsigned char digit;
	unsigned int check_ack;
	unsigned short seq;
	
} TstDtmfEvent;

typedef struct 
{
	unsigned char chid;
	unsigned char hook;
	unsigned int check_ack;
	unsigned short seq;
	
} TstHookEvent;

typedef struct 
{
	unsigned char chid;
	unsigned char fax_modem;
	unsigned int check_ack;
	unsigned short seq;
	
} TstFaxModemEvent;

typedef struct 
{
	unsigned char chid;
	unsigned char fax_dis_tx;
	unsigned int check_ack;
	unsigned short seq;
	
} TstFaxDisTxEvent;

typedef struct 
{
	unsigned char chid;
	unsigned char fax_dis_rx;
	unsigned int check_ack;
	unsigned short seq;
	
} TstFaxDisRxEvent;
#endif

#ifdef CONFIG_RTK_VOIP_IP_PHONE
 #ifdef __KERNEL__
  #include <linux/types.h>	// pid_t
 #else
  #include <sys/types.h>	// pid_t
 #endif

/* ------- We list keypad control in follow ------- */
typedef unsigned short wkey_t;	/* wide key */

/**
 * @brief General structure header for keypad
 * @param cmd Command type 
 * @see TstKeypadCtl do_mgr_VOIP_MGR_CTL_KEYPAD()
 */
typedef struct TstKeypadGeneral_s {
	keypad_cmd_t cmd;
} TstKeypadGeneral;

/**
 * @brief Structure for giving a target process 
 * @param cmd Command type, and it has to be KEYPAD_CMD_SET_TARGET
 * @param pid Target process ID 
 * @see TstKeypadCtl TstKeypadGeneral
 */
typedef struct TstKeypadSetTarget_s {
	keypad_cmd_t	cmd;	// = KEYPAD_CMD_SET_TARGET
	pid_t			pid;
} TstKeypadSetTarget;

/**
 * @brief Structure for debuging keypad function 
 * @param cmd Command type, and it has to be KEYPAD_CMD_SIG_DEBUG
 * @param wkey Key ID. <br>
 *        This ID will be put into key buffer, so another function can read it normally.
 * @see TstKeypadCtl TstKeypadGeneral
 */
typedef struct TstKeypadSignalDebug_s {
	keypad_cmd_t	cmd;	// = KEYPAD_CMD_SIG_DEBUG
	wkey_t			wkey;
} TstKeypadSignalDebug;

/**
 * @brief Structure for debuging keypad function 
 * @param cmd Command type, and it has to be KEYPAD_CMD_READ_KEY
 * @param wkey Key ID
 * @param validKey Determine whether wkey field is valid or not 
 * @see TstKeypadCtl TstKeypadGeneral
 */
typedef struct TstKeypadReadKey_s {
	keypad_cmd_t	cmd;	// = KEYPAD_CMD_READ_KEY
	wkey_t			wkey;
	unsigned char	validKey;
} TstKeypadReadKey;

/**
 * @brief Structure for debuging keypad function 
 * @param cmd Command type, and it has to be KEYPAD_CMD_HOOK_STATUS
 * @param wkey status Get hook status 
 * @see TstKeypadCtl TstKeypadGeneral
 */
typedef struct TstKeypadHookStatus_s {
	keypad_cmd_t	cmd;	// = KEYPAD_CMD_HOOK_STATUS
	unsigned char	status;	/* 0: on-hook, 1: off-hook */
} TstKeypadHookStatus;

/**
 * @brief Structure for keypad 
 * @see do_mgr_VOIP_MGR_CTL_KEYPAD()
 */
typedef union {
	TstKeypadGeneral		General;		//!< General header 
	TstKeypadSetTarget		SetTarget;		//!< Set application target PID 
	TstKeypadSignalDebug	SignalDebug;	//!< Debug purpose. Put a key into buffer. 
	TstKeypadReadKey		ReadKey;		//!< Read a key 
	TstKeypadHookStatus		HookStatus;		//!< Read hook status 
} TstKeypadCtl;

/* ------- We list LCM control in follow ------- */
/**
 * @brief Structure for LCM display on or off 
 * @param cmd Command type. It has to be LCD_CMD_DISPLAY_ON_OFF. 
 * @param bDisplayOnOff Display on or off. (1: on, 0: off)
 * @param bCursorOnOff Cursor on or on. (1: on, 0: off)
 * @param bCursorBlink Cursor blink. (1: on, 0: off)
 * @see TstLcmCtl TstLcmGeneral
 */
typedef struct TstLcmDisplayOnOff_s {
	lcm_cmd_t		cmd;	// = LCD_CMD_DISPLAY_ON_OFF
	unsigned char	bDisplayOnOff;	/* 1: on, 0: off, others: invalid */
	unsigned char	bCursorOnOff;	/* 1: on, 0: off, others: invalid */
	unsigned char	bCursorBlink;	/* 1: on, 0: off, others: invalid */
} TstLcmDisplayOnOff;

/**
 * @brief Structure for LCM cursor position 
 * @param cmd Command type. It has to be LCM_CMD_MOVE_CURSOR_POS. 
 * @param x Cursor position in X-axis 
 * @param y Cursor position in Y-axis
 * @note Character mode only 
 * @see TstLcmCtl TstLcmGeneral
 */
typedef struct TstLcmMoveCursorPosition_s {
	lcm_cmd_t		cmd;	// = LCM_CMD_MOVE_CURSOR_POS
	int				x;
	int				y;
} TstLcmMoveCursorPosition;

#define MAX_LEN_OF_DRAW_TEXT	16	/* NOT include null-terminator */
/**
 * @brief Structure for drawing text  
 * @param cmd Command type. It has to be LCM_CMD_DRAW_TEXT. 
 * @param x Text position in X-axis 
 * @param y Text position in Y-axis
 * @param szText Text buffer 
 * @param len Text length in buffer 
 * @note Character mode only 
 * @see TstLcmCtl TstLcmGeneral
 */
typedef struct TstLcmDrawText_s {
	lcm_cmd_t		cmd;	// = LCM_CMD_DRAW_TEXT
	int				x;
	int				y;
	unsigned char 	szText[ MAX_LEN_OF_DRAW_TEXT ];
	int				len;
} TstLcmDrawText;

#define MAX_LEN_OF_WRITE_DATA	160	/* for 80x16 bits LCD */
/**
 * @brief Structure for writing data 
 * @param cmd Command type. It has to be LCM_CMD_WRITE_DATA. 
 * @param start Start position of VRAM 
 * @param pixels Pixels data 
 * @param len Length in pixels parameter 
 * @note Graphic mode only 
 * @see TstLcmCtl TstLcmGeneral
 */
typedef struct TstLcmWriteData_s {
	lcm_cmd_t		cmd;	// = LCM_CMD_WRITE_DATA
	int				start;
	unsigned char 	pixels[ MAX_LEN_OF_WRITE_DATA ];
	int				len;
} TstLcmWriteData;

#define MAX_LEN_OF_WRITE_DATA2	132	/* for 132*65 bits LCD */
/**
 * @brief Structure for writing data (mode 2)
 * @param cmd Command type. It has to be LCM_CMD_WRITE_DATA2. 
 * @param page Start page
 * @param col Start column 
 * @param pixels Pixels data 
 * @param len Length in pixels parameter 
 * @note Graphic mode only 
 * @see TstLcmCtl TstLcmGeneral
 */
typedef struct TstLcmWriteData2_s {
	lcm_cmd_t		cmd;	// = LCM_CMD_WRITE_DATA2
	int				page;
	int				col;
	unsigned char 	pixels[ MAX_LEN_OF_WRITE_DATA2 ];
	int				len;
} TstLcmWriteData2;
	
/**
 * @brief Structure for LCM refresh from MMAP 
 * @param cmd Command type. It has to be LCM_CMD_DIRTY_MMAP. 
 * @param start Start position of VRAM 
 * @param len Refresh length after start position 
 * @note Graphic mode only 
 * @note Application has to use MMAP as its VRAM first. 
 * @see TstLcmCtl TstLcmGeneral
 */
typedef struct TstLcmDirtyMmap_s {
	lcm_cmd_t		cmd;	// = LCM_CMD_DIRTY_MMAP
	int				start;
	int				len;
} TstLcmDirtyMmap;

/**
 * @brief Structure for LCM refresh from MMAP (mode 2)
 * @param cmd Command type. It has to be LCM_CMD_DIRTY_MMAP2. 
 * @param page Start page 
 * @param col Start column 
 * @param len Refresh length after start position 
 * @param rows Refresh rows number 
 * @note Graphic mode only 
 * @note Application has to use MMAP as its VRAM first. 
 * @see TstLcmCtl TstLcmGeneral
 */
typedef struct TstLcmDirtyMmap2_s {
	lcm_cmd_t		cmd;	// = LCM_CMD_DIRTY_MMAP2
	int				page;
	int				col;
	int				len;
	int				rows;
} TstLcmDirtyMmap2;

/**
 * @brief General structure header for LCM 
 * @param cmd Command type.
 * @see TstLcmCtl TstLcmGeneral
 */
typedef struct TstLcmGeneral_s {
	lcm_cmd_t		cmd;
} TstLcmGeneral;

/**
 * @brief Structure for LCM  
 * @see do_mgr_VOIP_MGR_CTL_LCM()
 */
typedef union TstLcmCtl_s {
	TstLcmGeneral				General;			//!< General header 
	TstLcmDisplayOnOff			DisplayOnOff;		//!< Display on/off
	TstLcmMoveCursorPosition	MoveCursorPosition;	//!< Move cursor (textual mode)
	TstLcmDrawText				DrawText;			//!< Draw text (textual mode)
	TstLcmWriteData				WriteData;			//!< Write data (graphic mode) 
	TstLcmWriteData2			WriteData2;			//!< Write data (graphic mode) 
	TstLcmDirtyMmap				DirtyMmap;			//!< Refresh from MMAP (graphic mode) 
	TstLcmDirtyMmap2			DirtyMmap2;			//!< Refresh from MMAP (graphic mode) 
} TstLcmCtl;

/* ------- Control voice path ------- */
/**
 * @brief Structure for voice patch control
 * @param vpath Voice path 
 * @see do_mgr_VOIP_MGR_CTL_VOICE_PATH()
 */
typedef struct TstVoicePath_s {
	vpath_t	vpath;
} TstVoicePath_t;

/* ------- Control LED display ------- */
/**
 * @brief Structure for LED display 
 * @param led Each bit represents a LED 
 * @see do_mgr_VOIP_MGR_CTL_LED()
 */
typedef struct TstLedCtl_s {
	unsigned long led;
} TstLedCtl;

/* ------- IP phone miscellanous options ------- */
/**
 * @brief Structure for retrieving miscellanous options 
 * @param buildno Build number 
 * @param builddate Build date 
 * @see do_mgr_VOIP_MGR_CTL_MISC()
 */
typedef struct TstMiscCtl_s {
	unsigned long buildno;
	unsigned long builddate;
} TstMiscCtl_t;
#endif /* CONFIG_RTK_VOIP_IP_PHONE */

#ifdef CONFIG_RTK_VOIP_DRIVERS_IP_PHONE
/**
 * @brief Structure for IP phone debug 
 * @param function_type Function type. It can be <br>
 *        0: codec write, 1: codec read, <br>
 *        2: codec loopback on, 3: codec loopback off, <br>
 *        4: LED test 
 * @param reg Register number 
 * @param value Register value 
 * @note Use 'iphone_test' to debug 
 * @see do_mgr_VOIP_MGR_IPHONE_TEST()
 */
typedef struct
{
	unsigned int function_type;
	unsigned int reg;
	unsigned int value;
} IPhone_test;
#endif

typedef struct
{
	long mode;	// 0:init_to lec, 1:echo canceller, 2:init_to aec
	short buf1[128];	//input,
	short buf2[128];	//echo,
	//int32	ret_val;
}
TstVoipEcDebug;

#endif


