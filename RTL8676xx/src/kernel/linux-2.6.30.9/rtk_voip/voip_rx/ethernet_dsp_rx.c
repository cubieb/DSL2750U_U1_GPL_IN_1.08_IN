#include "rtk_voip.h"
#include "rtk_trap.h"
#include "voip_types.h"
#include "voip_init.h"

#include "../voip_manager/voip_mgr_netfilter.h"
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
#include "../voip_manager/voip_mgr_help_host.h"
#elif defined CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
#include "../voip_manager/voip_mgr_help_dsp.h"
#include "../voip_drivers/t38_handler.h"
#endif

#include "ethernet_dsp_tx.h"
#include "ethernet_dsp_rx.h"

#ifdef ETHERNET_DSP_DEBUG_DSP
unsigned int dsp_ctrl_rx_cnt = 0;
unsigned int dsp_rtp_rtcp_rx_cnt = 0;
unsigned int dsp_t38_rx_cnt = 0;
unsigned int dsp_ack_rx_cnt = 0;
#endif

#ifdef ETHERNET_DSP_DEBUG_HOST
unsigned int host_resp_rx_cnt[DSP_DEVICE_NUM] = {0};
unsigned int host_rtp_rtcp_rx_cnt[DSP_DEVICE_NUM] = {0};
unsigned int host_t38_rx_cnt[DSP_DEVICE_NUM] = {0};
unsigned int host_event_rx_cnt[DSP_DEVICE_NUM] = {0};
#endif


#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
extern uint32 API_GetMid_Dsp(uint32 chid, uint32 sid, uint32 dsp_id);
extern void system_process_rtp_tx(unsigned char CH, unsigned int sid, void *ptr_data, unsigned int data_len);
extern int dtmf_in_from_pkt(uint32 ch_id, char input);
extern int hook_in_from_pkt(uint32 ch_id, char input);
#endif

static unsigned char* pRes_content = NULL;
static unsigned short res_len = 0, resp_category = 0, resp_dsp_id = 0;
unsigned int Fax_Modem_Event_from_pkt[PCM_CH_NUM] = {0};
unsigned int Fax_DIS_TX_Event_from_pkt[PCM_CH_NUM] = {0};
unsigned int Fax_DIS_RX_Event_from_pkt[PCM_CH_NUM] = {0};
//unsigned char res_content[100]={0};

unsigned short get_response_content(unsigned short* pDsp_Id, unsigned char** pCont, unsigned short* pCategory)
{
	*pDsp_Id = resp_dsp_id;
	*pCont = pRes_content;
	*pCategory = resp_category;
	return res_len;
}

/* Host get DSP's voice packets and then send out. */
static void voip_voice2host_parser(unsigned short dsp_id, unsigned char* pkt)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
	unsigned int chid, sid, mid, len, isRtcp = 0;
	void* header;
	
	len = *(unsigned short*)pkt;
	chid = *(unsigned int*)((unsigned char*)pkt+2);
	sid = *(unsigned int*)((unsigned char*)pkt+6);
	header = (unsigned char*)pkt+10;
	//PRINT_R("v");

	
	/* Transfer DSP-ID, chid to Host chid */
	if (sid >= (2*ChNumPerDsp[dsp_id])) // 2*CHNUM_PER_DSP is DSP's RTCP_SID_OFFSET
	{
		sid = sid - (2*ChNumPerDsp[dsp_id]);
		isRtcp = 1;
	}

	mid = API_GetMid_Dsp(chid, sid, dsp_id);
	if (mid == 255)
		PRINT_R("voip_voice2host_parser: (%d, %d)\n", chid, sid);

	chid = API_get_Host_CH(dsp_id, chid); 	// get Host chid	

	sid = API_GetSid(chid, mid);		// get Host sid
	if (sid == 255)
		PRINT_G("voip_voice2host_parser: (%d, %d)\n", chid, mid);
	
	if (isRtcp == 1)
	{
		sid += RTCP_SID_OFFSET;
		//PRINT_R("rtcp tx sid=%d\n", sid);
		//PRINT_G("len=%d\n", len-8);
	}
	
	system_process_rtp_tx(chid, sid, header, len-8);
	//PRINT_G("%d,%d\n", chid, len-8);
#endif	
}

static void voip_voice2dsp_parser(unsigned char* pkt)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	int chid, sid, len, flags;
	void* packet;
	
	len = *(unsigned short*)pkt;
	chid = *(unsigned int*)((unsigned char*)pkt+2);
	sid = *(unsigned int*)((unsigned char*)pkt+6);
	flags = *(unsigned int*)((unsigned char*)pkt+10);
	packet = (unsigned char*)pkt+14;
	//PRINT_G("v");
	
	DSP_pktRx(chid, sid, packet, len-12, flags );
	//PRINT_G("%d,%d\n", chid, len-8);
#endif	
}

static void voip_t38_to_host_parser(unsigned short dsp_id, unsigned char* pkt)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
	int chid, sid, mid, len, isRtcp = 0;
	void* header;
	
	len = *(unsigned short*)pkt;
	chid = *(unsigned int*)((unsigned char*)pkt+2);
	sid = *(unsigned int*)((unsigned char*)pkt+6);
	header = (unsigned char*)pkt+10;
	//PRINT_R("v");
	
	/* Transfer DSP-ID, chid to Host chid */
	if (sid >= (2*ChNumPerDsp[dsp_id]))
	{
		sid = sid - (2*ChNumPerDsp[dsp_id]);
		isRtcp = 1;
	}
	mid = API_GetMid_Dsp(chid, sid, dsp_id);
	chid = API_get_Host_CH(dsp_id, chid); 	// get Host chid
	sid = API_GetSid(chid, mid);		// get Host sid
	if (isRtcp == 1)
		sid += RTCP_SID_OFFSET;
	
	system_process_rtp_tx(chid, sid, header, len-8);
	//PRINT_G("%d,%d\n", chid, len-8);
	//printk("R%d ", chid);
#endif
}

static void voip_t38_to_dsp_parser(unsigned char* pkt)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	int chid, sid, len, flags;
	void* packet;
	
	len = *(unsigned short*)pkt;
	chid = *(unsigned int*)((unsigned char*)pkt+2);
	sid = *(unsigned int*)((unsigned char*)pkt+6);
	packet = (unsigned char*)pkt+10;
	
	T38_API_PutPacket( chid, packet, len-8 );
	//printk("r%d ", chid);
#endif
}

static void voip_control_parser(unsigned char* pkt)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	unsigned long flags;
	unsigned short category = 0, length = 0, seq_no = 0;
	unsigned char* pContent = NULL;
	extern int do_voip_mgr_set_ctl(unsigned short cmd, unsigned char* para, unsigned short length, unsigned short seq_no);
	
	category = *(unsigned short*)(pkt);
	seq_no = *(unsigned short*)((unsigned char*)pkt+2);
	length = *(unsigned short*)((unsigned char*)pkt+4);
	pContent = (unsigned char*)pkt+6;
	
	do_voip_mgr_set_ctl(category, pContent, length, seq_no);
#endif
}

static void voip_response_parser(unsigned short dsp_id, unsigned char* pkt)
{
	unsigned short category = 0, length = 0, seq_no = 0;
	unsigned char* pContent = NULL;
	extern TstCheckResponse stCheckResponse;
	
	category = *(unsigned short*)(pkt);
	seq_no = *(unsigned short*)((unsigned char*)pkt+2);
	length = *(unsigned short*)((unsigned char*)pkt+4);
	pContent = (unsigned char*)pkt+6;
	//PRINT_R("(%d,%d)(%d,%d)\n", category, seq_no, stCheckResponse.category, stCheckResponse.seqNo);
	// Check if received response pkt match with the send control pkt.
	if (( category == stCheckResponse.category) && (seq_no == stCheckResponse.seqNo))
	{
		stCheckResponse.needResponse = 0;
		//PRINT_G("Get Resp(%d)\n", category);
	} 
	
	resp_dsp_id = dsp_id;
	
	if (length > 0) // copy_to_user
	{
		
#if 0		
		// send ack packet which the same with category and seq_no of the response pkt
		TstTxPktCtrl pktCtrl;
		pktCtrl.seq_no = seq_no;
		pktCtrl.resend_flag = 0;
		voip_dsp_L2_pkt_tx(category, ETH_ACK_PKT, NULL, 0, &pktCtrl);
#endif
		pRes_content = pContent;
		res_len = length;
		resp_category = category;
	}
	else
	{
		pRes_content = NULL;
		res_len = 0;
	}
}

static void voip_event_parser(unsigned short dsp_id, unsigned char* pkt)
{

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST

	unsigned short category = 0, length = 0, seq_no = 0;
	unsigned char* pContent = NULL;
	static int last_rev_seqno = -1;
	TstTxPktCtrl pktCtrl;
	
	category = *(unsigned short*)(pkt);
	seq_no = *(unsigned short*)((unsigned char*)pkt+2);
	length = *(unsigned short*)((unsigned char*)pkt+4);
	pContent = (unsigned char*)pkt+6;
	//PRINT_R("cat=%d, len=%d\n", category, length);
	
	if (last_rev_seqno == seq_no)
	{
		PRINT_R("Get the same event pkt with seqno=%d\n", seq_no);
		// re-send ack packet which the same with category and seq_no of the event pkt
		pktCtrl.dsp_id = dsp_id;
		pktCtrl.seq_no = 0;
		pktCtrl.resend_flag = 1;
		voip_dsp_L2_pkt_tx(category, ETH_ACK_PKT, NULL, 0, &pktCtrl);
		
		return;
	}
	
	switch(category)
	{
		case VOIP_MGR_DTMF_INTO_FIFO:
		{
			TstDtmfEvent stDtmfEvent;
			memcpy((unsigned char*)&stDtmfEvent, pContent, length);
			/* Transfer DSP-ID, DSP chid to Host chid */
			stDtmfEvent.chid = API_get_Host_CH(dsp_id, (int)stDtmfEvent.chid);
			dtmf_in_from_pkt(stDtmfEvent.chid, stDtmfEvent.digit);
			//dtmf_in(stDtmfEvent.chid, stDtmfEvent.digit);
			PRINT_MSG("DTMF_INTO_FIFO,ch=%d, digit=%c\n", stDtmfEvent.chid, stDtmfEvent.digit);
			break;
		}
		case VOIP_MGR_HOOK_INTO_FIFO:
		{
			TstHookEvent stHookEvent;
			memcpy((unsigned char*)&stHookEvent, pContent, length);
			/* Transfer DSP-ID, DSP chid to Host chid */
			stHookEvent.chid = API_get_Host_CH(dsp_id, (int)stHookEvent.chid);
			hook_in_from_pkt(stHookEvent.chid, stHookEvent.hook);
			//hook_in(stHookEvent.chid, stHookEvent.hook);
			//PRINT_R("VOIP_MGR_HOOK_INTO_FIFO\n");
			break;
		}
		case VOIP_MGR_FAX_MODEM_PASS_CFG:
		{
			TstFaxModemEvent stFaxModemEvent;
			memcpy((unsigned char*)&stFaxModemEvent, pContent, length);
			/* Transfer DSP-ID, DSP chid to Host chid */
			stFaxModemEvent.chid = API_get_Host_CH(dsp_id, (int)stFaxModemEvent.chid);
			Fax_Modem_Event_from_pkt[stFaxModemEvent.chid] = stFaxModemEvent.fax_modem;
			PRINT_MSG("FAX_MODEM_PASS_CFG, ch=%d\n", stFaxModemEvent.chid);
			break;
		}
		case VOIP_MGR_FAX_DIS_TX_DETECT:
		{
			TstFaxDisTxEvent stFaxFaxDisTxEvent;
			memcpy((unsigned char*)&stFaxFaxDisTxEvent, pContent, length);
			/* Transfer DSP-ID, DSP chid to Host chid */
			stFaxFaxDisTxEvent.chid = API_get_Host_CH(dsp_id, (int)stFaxFaxDisTxEvent.chid);
			Fax_DIS_TX_Event_from_pkt[stFaxFaxDisTxEvent.chid] = stFaxFaxDisTxEvent.fax_dis_tx;
			PRINT_MSG("VOIP_MGR_FAX_DIS_TX_DETECT, ch=%d\n", stFaxFaxDisTxEvent.chid);
			break;
		}
		case VOIP_MGR_FAX_DIS_RX_DETECT:
		{
			TstFaxDisRxEvent stFaxFaxDisRxEvent;
			memcpy((unsigned char*)&stFaxFaxDisRxEvent, pContent, length);
			/* Transfer DSP-ID, DSP chid to Host chid */
			stFaxFaxDisRxEvent.chid = API_get_Host_CH(dsp_id, (int)stFaxFaxDisRxEvent.chid);
			Fax_DIS_RX_Event_from_pkt[stFaxFaxDisRxEvent.chid] = stFaxFaxDisRxEvent.fax_dis_rx;
			PRINT_MSG("VOIP_MGR_FAX_DIS_RX_DETECT, ch=%d\n", stFaxFaxDisRxEvent.chid);
			break;
		}
	}


	// send ACK packet which the same with category and seq_no of the event pkt
	pktCtrl.dsp_id = dsp_id;
	pktCtrl.seq_no = seq_no;
	pktCtrl.resend_flag = 0;
	voip_dsp_L2_pkt_tx(category, ETH_ACK_PKT, NULL, 0, &pktCtrl);	
#endif
}

static void voip_ack_parser(unsigned char* pkt)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	unsigned short category = 0, seq_no = 0;
	extern TstCheckAck stCheckAck;
	
	category = *(unsigned short*)(pkt);
	seq_no = *(unsigned short*)((unsigned char*)pkt+2);
	//PRINT_G("Get Ack(%d)\n", category);

	extern void clean_event_cheak_ack(unsigned short category, unsigned short rev_seq);
	clean_event_cheak_ack(category, seq_no);
#endif	
}

void voip_dsp_L2_pkt_rx(unsigned char* eth_pkt)
{
	unsigned short pltype, dsp_id;
	
	dsp_id = *(unsigned short*)((unsigned char*)eth_pkt+DSP_ID_SHIFT);
	//printk("dsp_id=%d\n", dsp_id);

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	if (dsp_id == Get_Ethernet_DSP_ID())
#endif
	{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
		// Update Host sourc MAC addr
		static unsigned char update_host_mac = 1;
		extern unsigned char dec_mac_dsp2host[6];
		if (update_host_mac)
		{
			memcpy((unsigned char*)dec_mac_dsp2host, (unsigned char*)(eth_pkt+ETH_SRC_MAC_SHIFT), 6);
			update_host_mac = 0;
			printk("====> Update Host MAC addr: ");
			int i=0;
			for (i=0; i<6; i++)
				printk("%x: ", dec_mac_dsp2host[i]);
			printk("\n");
		}
#endif
	
		pltype = *(unsigned char*)((unsigned char*)eth_pkt+PROTOCOL_SHIFT);
		
#if 0
		if( pltype != 0x31 ) {
			int i;
			
			printk("pt%x\n", pltype);
			
			for( i = 0; i < 26; i ++ )
				printk( "%02X ", *( eth_pkt + i ) );
			
			printk( "\n" );
		}
#endif
		
		switch(pltype)
		{
			case ETH_VOICE2HOST_PKT://DSP -> Host
				voip_voice2host_parser(dsp_id, (unsigned char*)eth_pkt+VOICE_CONT_LEN_SHIFT);
#ifdef ETHERNET_DSP_DEBUG_HOST
				host_rtp_rtcp_rx_cnt[dsp_id]++;
#endif
				//PRINT_G("V\n");
				break;
				
			case ETH_VOICE2DSP_PKT://Host -> DSP
				voip_voice2dsp_parser((unsigned char*)eth_pkt+VOICE_CONT_LEN_SHIFT);
#ifdef ETHERNET_DSP_DEBUG_DSP
				dsp_rtp_rtcp_rx_cnt++;
#endif
				//PRINT_G("V\n");
				break;
			
			case ETH_T38_TO_HOST_PKT:
				voip_t38_to_host_parser(dsp_id, (unsigned char*)eth_pkt+VOICE_CONT_LEN_SHIFT);
#ifdef ETHERNET_DSP_DEBUG_HOST
				host_t38_rx_cnt[dsp_id]++;
#endif
				//PRINT_G("38->H\n");
				break;
			
			case ETH_T38_TO_DSP_PKT:
				voip_t38_to_dsp_parser((unsigned char*)eth_pkt+VOICE_CONT_LEN_SHIFT);
#ifdef ETHERNET_DSP_DEBUG_DSP
				dsp_t38_rx_cnt++;
#endif
				//PRINT_G("38->D\n");
				break;
			
			case ETH_CTRL_PKT:
				voip_control_parser((unsigned char*)eth_pkt+CATEGORY_SHIFT);
#ifdef ETHERNET_DSP_DEBUG_DSP
				dsp_ctrl_rx_cnt++;
#endif
				//PRINT_G("C\n");
				break;
				
			case ETH_RESP_PKT:
				voip_response_parser(dsp_id, (unsigned char*)eth_pkt+CATEGORY_SHIFT);
#ifdef ETHERNET_DSP_DEBUG_HOST
				host_resp_rx_cnt[dsp_id]++;
#endif
				//PRINT_G("R\n");
				break;
				
			case ETH_EVENT_PKT:
				voip_event_parser(dsp_id, (unsigned char*)eth_pkt+CATEGORY_SHIFT);
#ifdef ETHERNET_DSP_DEBUG_HOST
				host_event_rx_cnt[dsp_id]++;
#endif
				//PRINT_G("E\n");
				break;
				
			case ETH_ACK_PKT:
				//PRINT_G("Get Ack\n");
				voip_ack_parser((unsigned char*)eth_pkt+CATEGORY_SHIFT);
#ifdef ETHERNET_DSP_DEBUG_DSP
				dsp_ack_rx_cnt++;
#endif
				break;
			//default:
				//PRINT_R("?? @ %s, %s, line-%d\n", __FUNCTION__, __FILE__, __LINE__);
				//break;
		}
	
	}
}

static int __init ethernet_dsp_rx_init( void )
{
	extern void ( *voip_dsp_L2_pkt_rx_trap )(unsigned char* eth_pkt);
	
	voip_dsp_L2_pkt_rx_trap = voip_dsp_L2_pkt_rx;
	
	return 0;
}

voip_initcall( ethernet_dsp_rx_init );

