#include <linux/init.h>
#include <asm/uaccess.h>
#include "voip_types.h"
//#include "type.h"
#include "voip_control.h"
#include "voip_init.h"
#include "rtk_voip.h"
#include "voip_mgr_help_host.h"
#include "voip_mgr_define.h"
#include "ethernet_dsp_tx.h"

int slic_ch_num = SLIC_CH_NUM;
int voip_ch_num = VOIP_CH_NUM;
int sess_num = SESS_NUM;
int rtcp_sid_offset = RTCP_SID_OFFSET;
int pcm_period = PCM_PERIOD;
//uint64 gVoipFeature;

/* PCM Channel Info */
//char chanEnabled[MAX_VOIP_CH_NUM] = {0};
static int pcm_ch_for_DAA[PCM_CH_NUM] = {0};


/********* Help Host to transfer Host chid to DSP ID and DSP chid ******/

/* Note: 
 * 	Host Channel can be FXS or FXO.
 *	If Host Channel Numbers are 4, there are different combination as below:
 *	4FXS, or 3FXS+1FXO, or 2FXS+2FXO, or 1FXS+3FXO.
 */


/*======== For 8 Host Channel =========*/
#if (HOST_CH_NUM == 8)
#if (DSP_DEVICE_NUM == 1)
/* DSP0(8S) configuration */
unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {8};
#elif (DSP_DEVICE_NUM == 2)
/* DSP0(4S) + DSP1(4S) configuration */
unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 0, 0, 1, 1, 1, 1};
unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {4, 4};
#elif (DSP_DEVICE_NUM == 4)
/* DSP0(2S) + DSP1(2S) + DSP2(2S) + DSP3(2S) configuration */
unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 1, 1, 2, 2, 3, 3};
unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {2, 2, 2, 2};
#endif
#endif

/*======== For 5 Host Channel =========*/
#if (HOST_CH_NUM == 5)
#if (DSP_DEVICE_NUM == 1)
/* DSP0(4S) configuration */
unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 0, 0, 0};
unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {5};
#endif
#endif

/*======== For 4 Host Channel =========*/
#if (HOST_CH_NUM == 4)
#if (DSP_DEVICE_NUM == 1)
/* DSP0(4S) configuration */
unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 0, 0};
unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {4};
#elif (DSP_DEVICE_NUM == 2)
/* DSP0(2S) + DSP1(2S) configuration */
/* DSP0(2S) + DSP1(1S1O) configuration */
unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 1, 1};
unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {2, 2};
#endif
#endif

/*======== For 2 Host Channel =========*/
#if (HOST_CH_NUM == 2)
#if (DSP_DEVICE_NUM == 1)
/* DSP0(2S) configuration */
unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0};
unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {2};
#elif (DSP_DEVICE_NUM == 2)
/* DSP0(1S) + DSP1(1S) configuration */
unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 1};
unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {1};
#endif
#endif


/*======== For 6 Host Channel =========*/
#if (HOST_CH_NUM == 6)
/* For 6 FXS: DSP0(2S) + DSP1(4S) configuration */
//#define DSP_DEVICE_NUM 2
//unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 1, 1, 1, 1};
//unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {2, 4};

/* For 6 FXS: DSP0(4S) + DSP1(2S) configuration */
//#define DSP_DEVICE_NUM 2
//unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 0, 0, 1, 1};
//unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {4, 2};

/* For 6 FXS: DSP0(2S) + DSP1(2S) + DSP2(2S) configuration */
//#define DSP_DEVICE_NUM 3
//unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 1, 1, 2, 2};
//unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {2, 2, 2};
#endif

/* For 3 FXS 1 FXO: DSP0(2S) + DSP1(1S1O) configuration */
//#define DSP_DEVICE_NUM 2
//unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 1, 1};
//unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {2, 2};

/* For 3 FXS: DSP0(2S) + DSP1(1S) configuration */
//#define DSP_DEVICE_NUM 2
//unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 1};
//unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {2, 1};

/* For 3 FXS: DSP0(1S) + DSP1(2S) configuration */
//#define DSP_DEVICE_NUM 2
//unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 1, 1};
//unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {1, 2};

/* For 2 FXS 1 FXO: DSP0(2S1O) configuration */
//#define DSP_DEVICE_NUM 1
//unsigned char HostChannelToDspIdTable[HOST_CH_NUM] = {0, 0, 0};
//unsigned char DspIdToDspChannelNum[DSP_DEVICE_NUM] = {3};


/*************************************************************************/


unsigned short API_get_DSP_ID(int cmd, int host_chid)
{
	if ((host_chid < 0) || (host_chid > voip_ch_num))
		PRINT_R("1: Error host chid%d, cmd=%d\n", host_chid, cmd-2116);

	//PRINT_R("h%d->id%d\n", host_chid, HostChannelToDspIdTable[host_chid]);
	return HostChannelToDspIdTable[host_chid];
}

unsigned int API_get_DSP_CH(int cmd, int host_chid)
{
	int id = 0, dsp_id, tmp = 0;
	if ((host_chid < 0) || (host_chid > voip_ch_num))
		PRINT_R("2: Error host chid%d, cmd=%d\n", host_chid, cmd-2116);

	dsp_id = HostChannelToDspIdTable[host_chid];

	for (id=0; id < dsp_id; id++)
		tmp += DspIdToDspChannelNum[id];
	
	//PRINT_R("h%d->ch%d\n", host_chid, host_chid - tmp);
	return (host_chid - tmp);
}

unsigned int API_get_Host_CH(unsigned short dsp_id, int dsp_chid)
{
	int id = 0, tmp = 0;
	
	for (id=0; id < dsp_id; id++)
		tmp += DspIdToDspChannelNum[id];
	
	//PRINT_R("(%d,%d)->h%d\n", dsp_id, dsp_chid, tmp + dsp_chid);
	return (tmp + dsp_chid);
}

unsigned int API_get_DSP_NUM(void)
{
	return DSP_DEVICE_NUM;
}



/****** sid API: The same API to the dsp_main.c *******/
// for chid and mid transfer to sid

typedef struct _channel_config_s
{
#ifdef SUPPORT_3WAYS_AUDIOCONF
	uint32 isConference;
#endif
	uint32 tranSessId;
	uint32 RegSessNum;
        uint32 RegSessId[MAX_SESS_NUM];
	uint32 resource;
	
}channel_config_t;

typedef struct _session_config_s
{
	char   isOpen;
	uint32 myChanId;

}session_config_t;


/* For Host */

static uint32 nSidTbl[MAX_SESS_NUM];
static channel_config_t chanInfo[MAX_VOIP_CH_NUM];
static session_config_t sessInfo[MAX_SESS_NUM];

static int32 chanInfo_CloseSessionID(uint32 chid, uint32 sid)
{
	uint32 i;
        if(chid >= voip_ch_num)
		return FAILED;
	if(sid >= sess_num)
		return FAILED;

	for(i=0; i<sess_num; i++)
	{
		if(chanInfo[chid].RegSessId[i] == sid)
		{
			sessInfo[sid].isOpen = 0;
			chanInfo[chid].RegSessId[i] = SESSION_NULL;
			chanInfo[chid].RegSessNum--;
			break;
		}
		if(chanInfo[chid].RegSessId[i] == SESSION_NULL)
			break;
	}

	if(i == sess_num)
		return FAILED;

	for(i=i+1; i<sess_num; i++)
	{
		if(chanInfo[chid].RegSessId[i] != SESSION_NULL)
		{
			chanInfo[chid].RegSessId[i-1] = chanInfo[chid].RegSessId[i];
			chanInfo[chid].RegSessId[i] = SESSION_NULL;
		}
	}
//	if(chanInfo[chid].tranSessId == sid)
//		chanInfo[chid].tranSessId = chid;

//	if(chanInfo[chid].RegSessNum == 0)
//	{
//		chanInfo[chid].tranSessId = chid;
//		chanInfo[chid].RegSessId[0] = chid;
//	}

//		chanInfo[chid].tranSessId = SESSION_NULL;
	return SUCCESS;
}

static int32 SetSessionFree(uint32 sid)
{
	uint32 chid;
	if(sid >= sess_num)
		return FAILED;
	if(!sessInfo[sid].isOpen)
		return FAILED;

	chid = sessInfo[sid].myChanId;
	sessInfo[sid].isOpen = 0;
	sessInfo[sid].myChanId = CHANNEL_NULL;

	/* clear channel state*/
	chanInfo_CloseSessionID(chid, sid);

	return SUCCESS;
}

static uint32 GetFreeSession(uint32 chid)
{
	uint32 i, sid;
	for(sid=0; sid<sess_num; sid++)
	{
		if(sessInfo[sid].isOpen)
			continue;
		if(sid < voip_ch_num)
		{
			if(sid == chid)
				break;
		}
		else
			break;
	}
	if(sid == sess_num)
		return SESSION_NULL;

	sessInfo[sid].isOpen = 1;
	sessInfo[sid].myChanId = chid;

	/* set channel state*/
	for(i=0; i<sess_num; i++)
	{
		if(chanInfo[chid].RegSessId[i] == sid)
			break;
		if(chanInfo[chid].RegSessId[i] == SESSION_NULL)
		{
			chanInfo[chid].RegSessId[i] = sid;
			chanInfo[chid].RegSessNum++;
			break;
		}
	}
	return sid;
}

uint32 API_OpenSid(uint32 chid, uint32 mid)
{
	uint32 sid, id;
	if(chid >= voip_ch_num)
		return SESSION_NULL;

	id = (chid * sess_num) + mid;
	for(sid=0; sid<sess_num; sid++)
	{
		if(nSidTbl[sid] == id)
			return sid;
	}

	sid = GetFreeSession(chid);
	if(sid == SESSION_NULL)
		return SESSION_NULL;
	if(nSidTbl[sid] != SESSION_NULL)
		PRINT_R("nSidTbl[%d] = %d, is not SESSION_NULL!\n", sid, nSidTbl[sid]);
	nSidTbl[sid] = id;
	return sid;
}

uint32 API_GetSid(uint32 chid, uint32 mid)
{
	uint32 sid, id;
	
	//printk( "API_GetSid chid=%d\n", chid );
	
	if(chid >= voip_ch_num)
		return SESSION_NULL;

	id = (chid * sess_num) + mid;
	//printk( "API_GetSid id=%d\n", id );
	for(sid=0; sid<sess_num; sid++)
	{
		//printk("nSidTbl[%d]=%d ",sid,nSidTbl[sid]);//chiminer modify
		if(id == nSidTbl[sid])
			break;
	}
		//printk("\n");//chiminer modify
	if(sid == sess_num)
		return SESSION_NULL;
		//return 1;//chiminer modify
	return sid;
}

uint32 API_GetMid(uint32 chid, uint32 sid)
{
	uint32 mid, id;

        if(chid >= voip_ch_num)
		return SESSION_NULL;

	if(sid >= sess_num)
		return SESSION_NULL;

	id = nSidTbl[sid];
	mid = id - (chid * sess_num);

	if(mid >= sess_num)
		return SESSION_NULL;

	return mid;
}

uint32 API_CloseSid(uint32 chid, uint32 mid)
{
	uint32 sid, id;

        if(chid >= voip_ch_num)
		return SESSION_NULL;

	id = (chid * sess_num) + mid;
	for(sid=0; sid<sess_num; sid++)
	{
		if(id == nSidTbl[sid])
			break;
	}
	if(sid == sess_num)
		return SESSION_NULL;

	nSidTbl[sid] = SESSION_NULL;
	SetSessionFree(sid);
	return sid;
}


/* For DSP */

unsigned int ChNumPerDsp[DSP_DEVICE_NUM] = {0};
unsigned int SessNumPerDsp[DSP_DEVICE_NUM] = {0};

//#define SESS_NUM_DSP	2*CHNUM_PER_DSP

static uint32 nSidTbl_dsp[DSP_DEVICE_NUM][MAX_SESS_NUM];
static channel_config_t chanInfo_dsp[DSP_DEVICE_NUM][MAX_VOIP_CH_NUM];
static session_config_t sessInfo_dsp[DSP_DEVICE_NUM][MAX_SESS_NUM];

static int32 chanInfo_CloseSessionID_Dsp(uint32 chid, uint32 sid, uint32 dsp_id)
{
	uint32 i;
        if(chid >= ChNumPerDsp[dsp_id])
		return FAILED;
	if(sid >= SessNumPerDsp[dsp_id])
		return FAILED;

	for(i=0; i<SessNumPerDsp[dsp_id]; i++)
	{
		if(chanInfo_dsp[dsp_id][chid].RegSessId[i] == sid)
		{
			sessInfo_dsp[dsp_id][sid].isOpen = 0;
			chanInfo_dsp[dsp_id][chid].RegSessId[i] = SESSION_NULL;
			chanInfo_dsp[dsp_id][chid].RegSessNum--;
			break;
		}
		if(chanInfo_dsp[dsp_id][chid].RegSessId[i] == SESSION_NULL)
			break;
	}

	if(i == SessNumPerDsp[dsp_id])
		return FAILED;

	for(i=i+1; i<SessNumPerDsp[dsp_id]; i++)
	{
		if(chanInfo_dsp[dsp_id][chid].RegSessId[i] != SESSION_NULL)
		{
			chanInfo_dsp[dsp_id][chid].RegSessId[i-1] = chanInfo_dsp[dsp_id][chid].RegSessId[i];
			chanInfo_dsp[dsp_id][chid].RegSessId[i] = SESSION_NULL;
		}
	}
//	if(chanInfo_dsp[chid].tranSessId == sid)
//		chanInfo_dsp[chid].tranSessId = chid;

//	if(chanInfo_dsp[chid].RegSessNum == 0)
//	{
//		chanInfo_dsp[chid].tranSessId = chid;
//		chanInfo_dsp[chid].RegSessId[0] = chid;
//	}

//		chanInfo_dsp[chid].tranSessId = SESSION_NULL;
	return SUCCESS;
}

static int32 SetSessionFree_Dsp(uint32 sid, uint32 dsp_id)
{
	uint32 chid;
	if(sid >= SessNumPerDsp[dsp_id])
		return FAILED;
	if(!sessInfo_dsp[dsp_id][sid].isOpen)
		return FAILED;

	chid = sessInfo_dsp[dsp_id][sid].myChanId;
	sessInfo_dsp[dsp_id][sid].isOpen = 0;
	sessInfo_dsp[dsp_id][sid].myChanId = CHANNEL_NULL;

	/* clear channel state*/
	chanInfo_CloseSessionID_Dsp(chid, sid, dsp_id);

	return SUCCESS;
}

static uint32 GetFreeSession_Dsp(uint32 chid, uint32 dsp_id)
{
	uint32 i, sid;
	for(sid=0; sid<SessNumPerDsp[dsp_id]; sid++)
	{
		if(sessInfo_dsp[dsp_id][sid].isOpen)
			continue;
                if(sid < ChNumPerDsp[dsp_id])
		{
			if(sid == chid)
				break;
		}
		else
			break;
	}
	if(sid == SessNumPerDsp[dsp_id])
		return SESSION_NULL;

	sessInfo_dsp[dsp_id][sid].isOpen = 1;
	sessInfo_dsp[dsp_id][sid].myChanId = chid;

	/* set channel state*/
	for(i=0; i<SessNumPerDsp[dsp_id]; i++)
	{
		if(chanInfo_dsp[dsp_id][chid].RegSessId[i] == sid)
			break;
		if(chanInfo_dsp[dsp_id][chid].RegSessId[i] == SESSION_NULL)
		{
			chanInfo_dsp[dsp_id][chid].RegSessId[i] = sid;
			chanInfo_dsp[dsp_id][chid].RegSessNum++;
			break;
		}
	}
	return sid;
}

uint32 API_OpenSid_Dsp(uint32 chid, uint32 mid, uint32 dsp_id)
{
	uint32 sid, id;
        if(chid >= ChNumPerDsp[dsp_id])
		return SESSION_NULL;
		
	id = (chid * SessNumPerDsp[dsp_id]) + mid;
	for(sid=0; sid<SessNumPerDsp[dsp_id]; sid++)
	{
		if(nSidTbl_dsp[dsp_id][sid] == id)
			return sid;
	}

	sid = GetFreeSession_Dsp(chid, dsp_id);
	if(sid == SESSION_NULL)
		return SESSION_NULL;
	if(nSidTbl_dsp[dsp_id][sid] != SESSION_NULL)
		PRINT_R("nSidTbl_dsp[%d][%d] = %d, is not SESSION_NULL!\n", dsp_id, sid, nSidTbl_dsp[dsp_id][sid]);
	nSidTbl_dsp[dsp_id][sid] = id;
	return sid;
}

uint32 API_GetSid_Dsp(uint32 chid, uint32 mid, uint32 dsp_id)
{
	uint32 sid, id;

        if(chid >= ChNumPerDsp[dsp_id])
		return SESSION_NULL;
	
	id = (chid * SessNumPerDsp[dsp_id]) + mid;
	for(sid=0; sid<SessNumPerDsp[dsp_id]; sid++)
	{
		//printk("nSidTbl_dsp[%d]=%d ",sid,nSidTbl_dsp[sid]);//chiminer modify
		if(id == nSidTbl_dsp[dsp_id][sid])
			break;
	}
		//printk("\n");//chiminer modify
	if(sid == SessNumPerDsp[dsp_id])
		return SESSION_NULL;
		//return 1;//chiminer modify
	return sid;
}

uint32 API_GetMid_Dsp(uint32 chid, uint32 sid, uint32 dsp_id)
{
	uint32 mid, id;

        if(chid >= ChNumPerDsp[dsp_id])
		return SESSION_NULL;
	
	if(sid >= SessNumPerDsp[dsp_id])
		return SESSION_NULL;

	id = nSidTbl_dsp[dsp_id][sid];
	mid = id - (chid * SessNumPerDsp[dsp_id]);

	if(mid >= SessNumPerDsp[dsp_id])
		return SESSION_NULL;

	return mid;
}

uint32 API_CloseSid_Dsp(uint32 chid, uint32 mid, uint32 dsp_id)
{
	uint32 sid, id;

        if(chid >= ChNumPerDsp[dsp_id])
		return SESSION_NULL;

	id = (chid * SessNumPerDsp[dsp_id]) + mid;
	for(sid=0; sid<SessNumPerDsp[dsp_id]; sid++)
	{
		if(id == nSidTbl_dsp[dsp_id][sid])
			break;
	}
	if(sid == SessNumPerDsp[dsp_id])
		return SESSION_NULL;

	nSidTbl_dsp[dsp_id][sid] = SESSION_NULL;
	SetSessionFree_Dsp(sid, dsp_id);
	return sid;
}

/************************************************************************/

int Is_DAA_Channel(int chid)
{
	return pcm_ch_for_DAA[chid];
}

/* T.38 Info */
t38_running_state_t t38RunningState[MAX_VOIP_CH_NUM]; // need T.38 info for Host in rx trap.

/* RTP Statistic Info */
unsigned char RtcpOpen[MAX_SESS_NUM] = {0};
unsigned char RtpOpen[MAX_SESS_NUM] = {0};
uint32 rtpHold[MAX_SESS_NUM];
uint32 rtpConfigOK[MAX_SESS_NUM];
uint32 nRxRtpStatsLostPacket[MAX_VOIP_CH_NUM];

int32 Host_pktRx( uint32 chid, uint32 sid, void* packet, uint32 pktLen, uint32 flags )
{

	// Send RTP packet to DSP by voice packet.
	TstTxPktCtrl stTxPktCtrl;
	stTxPktCtrl.seq_no = -1;
	stTxPktCtrl.resend_flag = 0;
	unsigned char rtp_read_tmp[1500];
	unsigned int chid_dsp, sid_dsp, mid;
	
	stTxPktCtrl.dsp_id = API_get_DSP_ID(0, chid);
	
	/* Transfer Host chid, sid to DSP ID, chid */	
	chid_dsp = API_get_DSP_CH(0, chid);
	mid = API_GetMid(chid, sid);
	sid_dsp = API_GetSid_Dsp(chid_dsp, mid, stTxPktCtrl.dsp_id);
	
	*(uint32*)rtp_read_tmp= chid_dsp;
	*(uint32*)((unsigned char*)rtp_read_tmp+4)= sid_dsp;
	*(uint32*)((unsigned char*)rtp_read_tmp+8)= flags;
	
	if (pktLen > (1500-12))
	{
		PRINT_R("Error! no enought buf size, %s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
		PRINT_R("Need %d bytes, sid=%d\n", pktLen, sid);
	}
	memcpy(&rtp_read_tmp[12], packet, pktLen);
	
	voip_dsp_L2_pkt_tx(0, ETH_VOICE2DSP_PKT, (unsigned char*)rtp_read_tmp, pktLen+12, &stTxPktCtrl);
	
	return SUCCESS;
}

/************************************************************************/
static void read_mgr_chid( uint32 *mgr_chid, void *user, unsigned int len, const voip_mgr_entry_t *p_entry )
{
	unsigned char *field;
	
	if( p_entry ->field_offset + p_entry ->field_size > len ) {
		PRINT_R( "Bad field offset + size (%d > %d) in reading!\n",
				 p_entry ->field_offset + p_entry ->field_size, len );
		return;
	}
	
	field = ( unsigned char * )user;
	field += p_entry ->field_offset;
	
	switch( p_entry ->field_size ) {
	case 1:
		*mgr_chid = *( ( uint8 * )field );
		break;
	case 2:
		*mgr_chid = *( ( uint16 * )field );
		break;
	case 4:
		*mgr_chid = *( ( uint32 * )field );
		break;
	default:
		PRINT_R( "Bad field_size=%u in reading!\n", p_entry ->field_size );
	}
}

static void write_mgr_chid( uint32 mgr_chid, void *user, unsigned int len, const voip_mgr_entry_t *p_entry )
{
	unsigned char *field;
	
	if( p_entry ->field_offset + p_entry ->field_size > len ) {
		PRINT_R( "Bad field offset + size (%d > %d) in writing!\n",
				 p_entry ->field_offset + p_entry ->field_size, len );
		return;
	}
	
	field = ( unsigned char * )user;
	field += p_entry ->field_offset;
	
	switch( p_entry ->field_size ) {
	case 1:
		*( ( uint8 * )field ) = mgr_chid;
		break;
	case 2:
		*( ( uint16 * )field ) = mgr_chid;
		break;
	case 4:
		*( ( uint32 * )field ) = mgr_chid;
		break;
	default:
		PRINT_R( "Bad field_size=%u in writing!\n", p_entry ->field_size );
	}
}

#define HSND_PRINTK		PRINT_Y

#define USER_BUFFER_SIZE		1500
static unsigned char user_buffer[ USER_BUFFER_SIZE ];

int do_voip_mgr_ctl_in_host( int cmd, void *user, unsigned int len, const voip_mgr_entry_t *p_entry )
{
	const uint32 flags = p_entry ->flags;
	int ret = 0;
	uint32 mgr_chid, dsp_chid;
	unsigned short dsp_id;
	
	static int entry = 0;

#if 0	
	switch( cmd ) {
	case VOIP_MGR_CHECK_DSP_ALL_SW_READY:
	case VOIP_MGR_SLIC_HOOK:
	case VOIP_MGR_DTMF_GET:
	case VOIP_MGR_FAX_MODEM_PASS_CFG:
		break;
	
	default:
		HSND_PRINTK( ">%d %d\n", cmd, len );
		break;
	}
#endif
	
	//PRINT_MSG( "%s\n", p_entry ->cmd_name );
	
	// check re-entry. Because we have only one copy of user_buffer 
	if( entry ) {
		PRINT_R( "host voip_mgr_ctl re-entry!!\n" );
	}
	
	entry = 1;
	
	// check type size 
	if( ( flags & MF_AUTOFW ) ) {
		if( ( flags & MF_CHANNEL ) && ( p_entry ->type_size != len ) )
			PRINT_R( "cmd %d field size is different %d!=%d\n", cmd, p_entry ->type_size, len );
		if( len > USER_BUFFER_SIZE ) {
			PRINT_R( "cmd %d length is too long %d>%d\n", cmd, len, USER_BUFFER_SIZE );
			return -EFBIG;
		}
	}
	
	// main process 
	if( flags & MF_AUTOFW ) {
		
		copy_from_user( user_buffer, user, len );
		
		if( flags & MF_CHANNEL ) {
			//mgr_chid = stVoipCfg.ch_id;
			read_mgr_chid( &mgr_chid, user_buffer, len, p_entry );
			//stVoipCfg.ch_id = API_get_DSP_CH(cmd, stVoipCfg.ch_id);	// convert to DSP chid
			dsp_chid = API_get_DSP_CH(cmd, mgr_chid);	// convert to DSP chid
			PRINT_MSG( "%s ch_id=%d;%d\n", p_entry ->cmd_name, mgr_chid, dsp_chid );
			write_mgr_chid( dsp_chid, user_buffer, len, p_entry );
			//ret = ethernetDspSentL2ControlPacket(cmd, mgr_chid, &stVoipCfg, sizeof(TstVoipCfg));
			ret = ethernetDspSentL2ControlPacket(cmd, mgr_chid, user_buffer, len );
			//stVoipCfg.ch_id = mgr_chid;
			write_mgr_chid( mgr_chid, user_buffer, len, p_entry );
			//stVoipCfg.ret_val = ret;
		} else {
			PRINT_MSG( "%s\n", p_entry ->cmd_name );
			ret = ethernetDspSentL2ControlPacketNoChannel( cmd, user_buffer, len );
		}
	}
	
	if( flags & MF_BODY ) {
		// do_mgr function do copy_from_user by itself 
		ret = p_entry ->do_mgr( cmd, user, len, 0 );
	}
	
	if( flags & MF_AUTOFW ) {
		if( flags & MF_FETCH ) {
			//ethernetDspCheckL2RespPacket(cmd, &stSlicReg, &dsp_id);
			ethernetDspCheckL2RespPacket(cmd, user_buffer, &dsp_id);
			//stSlicReg.ch_id = API_get_Host_CH( dsp_id, stSlicReg.ch_id);/* Get Host chid */
			read_mgr_chid( &dsp_chid, user_buffer, len, p_entry );
			mgr_chid = API_get_Host_CH( dsp_id, dsp_chid );/* Get Host chid */
			write_mgr_chid( mgr_chid, user_buffer, len, p_entry );
			
			//stSlicReg.ret_val = ret; // update ret_val must after check response ack      	
			
			copy_to_user( user, user_buffer, len );
		}	
	}
	
	entry = 0;
	
	return ret;
}

/**********************************************/

void __init host_init(void)
{
	PRINT_Y("=== Ethernet DSP: Host Init ===\n");	
        
	int i = 0, j = 0;
	
	for(i=0; i<voip_ch_num; i++)
	{
		//chanEnabled[i] = 0;
		
		if ((i - (slic_ch_num-1)) > 0)
			pcm_ch_for_DAA[i] = 1;	// this pcm channel is for DAA(FXO). 
		else
			pcm_ch_for_DAA[i] = 0;	// this pcm channel is for Slic(FXS).

	}
	
	for(i=0; i<sess_num; i++)
	{
		nSidTbl[i] = SESSION_NULL;
	}
	

	for (j=0; j<DSP_DEVICE_NUM; j++)
	{
		for(i=0; i<MAX_SESS_NUM; i++)
		{
			nSidTbl_dsp[j][i] = SESSION_NULL;
		}
	
		PRINT_Y("DSP%d: \n", j);
		ChNumPerDsp[j] = DspIdToDspChannelNum[j];
		SessNumPerDsp[j] = 2*ChNumPerDsp[j];
		PRINT_Y("ChNum= %d, SesNum= %d\n", ChNumPerDsp[j], SessNumPerDsp[j]);

		for (i=0; i<SessNumPerDsp[j]; i++)
		{
			API_OpenSid_Dsp(i, 0, j); // mid 0
			API_OpenSid_Dsp(i, 1, j); // mid 1
			PRINT_Y("Open sid%d\n", i);
		}
	}

#if 0	
	PRINT_Y("--- nSidTbl_dsp ---\n");
	
	for( j = 0; j < DSP_DEVICE_NUM; j ++ ) {
		for( i = 0; i < MAX_SESS_NUM; i ++ ) {
			PRINT_Y("[%d][%d]=%d\n", j, i, nSidTbl_dsp[j][i] );
		}
	}
#endif
	
	PRINT_Y("=== Done ===\n");
	
}

void __exit host_exit(void)
{
	PRINT_Y("============= Realtek Ethernet DSP: Host exit ============\n");
	
	PRINT_Y("==> Done !\n");
}

#ifndef CONFIG_RTK_VOIP_MODULE
voip_initcall(host_init);
voip_exitcall(host_exit);
#endif

