#include <linux/config.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include "ethernet_dsp_tx.h"
#include "voip_types.h"
#include "voip_control.h"
#include "voip_errno.h"
#include "voip_init.h"

#define RESEND_THS	3
//#define RESEND_ERR	-3	// replaced by -EVOIP_RESEND_ERR


#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
#include "../voip_manager/voip_mgr_help_host.h"
#include "../voip_rx/rtk_trap.h"
#include "../voip_rx/ethernet_dsp_rx.h"
#endif

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
#include "../voip_manager/voip_mgr_help_dsp.h"
#endif

#ifdef ETHERNET_DSP_DEBUG_HOST
#include "../voip_manager/voip_mgr_help_host.h"
unsigned int host_ctrl_tx_cnt[DSP_DEVICE_NUM] = {0};
unsigned int host_rtp_rtcp_tx_cnt[DSP_DEVICE_NUM] = {0};
unsigned int host_t38_tx_cnt[DSP_DEVICE_NUM] = {0};
unsigned int host_ack_tx_cnt[DSP_DEVICE_NUM] = {0};
#endif

#ifdef ETHERNET_DSP_DEBUG_DSP
unsigned int dsp_resp_tx_cnt = 0;
unsigned int dsp_rtp_rtcp_tx_cnt = 0;
unsigned int dsp_t38_tx_cnt = 0;
unsigned int dsp_event_tx_cnt = 0;
#endif

static struct net_device *eth0_dev;

//dsp to host des MAC addr
unsigned char dec_mac_dsp2host[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//host to dsp des MAC addr
static unsigned char dec_mac_host2dsp0[6] = {0x02, 0xE0, 0x4C, 0x89, 0x72, 0xB0}; // dsp-0
static unsigned char dec_mac_host2dsp1[6] = {0x02, 0xE0, 0x4C, 0x89, 0x72, 0xB1}; // dsp-1
static unsigned char dec_mac_host2dsp2[6] = {0x02, 0xE0, 0x4C, 0x89, 0x72, 0xB2}; // dsp-2
static unsigned char dec_mac_host2dsp3[6] = {0x02, 0xE0, 0x4C, 0x89, 0x72, 0xB3}; // dsp-3
/* Note: Should be identical to the DSP MAC addr setting: 
	refer to AP/pana/hcd/hcd.c function ethernet_dsp_set_mac()
*/

static unsigned short eth_type = 0x8899;
static unsigned short seq_no = 0;
static unsigned char voip_ver = 0x08;
TstCheckResponse stCheckResponse;
static TstCheckAck stCheckAck;

static int __init rtk_voip_ethernet_dsp_init(void)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
	extern struct net init_net;//linux global variable
	eth0_dev = (struct net_device *)__dev_get_by_name(&init_net, "eth0");
#else
	eth0_dev = (struct net_device *)__dev_get_by_name("eth0");
#endif
	
	if (eth0_dev != NULL)
		PRINT_Y("Get the eth0 dev successfully.\n");
	else
		PRINT_Y("Get the eth0 dev NULL.\n");
		
	return 0;
}

static void __exit rtk_voip_ethernet_dsp_exit(void)
{
	eth0_dev = NULL;
}

static unsigned int host_need_response( void )
{
	return ( ( TstCheckResponse volatile * )&stCheckResponse ) ->needResponse;
}

unsigned int dsp_need_ack( void )
{
	//return ( ( TstCheckAck volatile * )stCheckAck ) ->needAck;
	return 0;
}

int voip_dsp_L2_pkt_tx(unsigned short category, unsigned char pkt_type, unsigned char* pdata, unsigned short data_len, TstTxPktCtrl* txCtrl)
{
	struct sk_buff *skb;
	unsigned int plhdr_len=0;
	unsigned int skb_len = 0;
	
	//PRINT_R("cat=%d, type=%d\n", category, pkt_type);
	
	if (eth0_dev == NULL)
		return -1;
	
	plhdr_len = 14 + ETH_TYPE_FIELD_LEN + DSP_ID_FIELD_LEN
			+ DUMMY_FIELD_LEN + PROTOCOL_FIELD_LEN
			+ VOIP_VER_FIELD_LEN;
	
	if (pkt_type == ETH_VOICE2HOST_PKT || pkt_type == ETH_VOICE2DSP_PKT || pkt_type == ETH_T38_TO_HOST_PKT || pkt_type == ETH_T38_TO_DSP_PKT)// data packet
		plhdr_len += (CONT_LEN_FIELD_LEN);
	else if (pkt_type == ETH_CTRL_PKT)	// control packet
		plhdr_len += (CATEGORY_FIELD_LEN + SEQUENCE_FIELD_LEN + CONT_LEN_FIELD_LEN);
	else if (pkt_type == ETH_RESP_PKT)	// response packet
		plhdr_len += (CATEGORY_FIELD_LEN + SEQUENCE_FIELD_LEN + CONT_LEN_FIELD_LEN);
	else if (pkt_type == ETH_EVENT_PKT)	// event packet
		plhdr_len += (CATEGORY_FIELD_LEN + SEQUENCE_FIELD_LEN + CONT_LEN_FIELD_LEN);
	else if (pkt_type == ETH_ACK_PKT)	// ack packet
		plhdr_len += (CATEGORY_FIELD_LEN + SEQUENCE_FIELD_LEN);
	else
	{
		PRINT_R("Error paket type%d:%s:%s:%d\n", pkt_type, __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
		
	skb_len = plhdr_len + data_len;	// len_of_ether_frame
	
	if (skb_len < 64)
		skb_len = 64;
		
	skb = alloc_skb( skb_len , GFP_ATOMIC);
	if (skb == NULL)
	{
		printk("voip_dsp_L2_pkt_tx :skb_alloc return NULL.\n");
		return -1;
	}
	
	skb->len = skb_len;
	skb->dev = eth0_dev;
	memset(skb->data, 0, skb->len);
	
	/* setup ethernet header (DA, SA, type) and payload of the skb */
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
		//memcpy(skb->data, dec_mac_host, 6);
		if (txCtrl->dsp_id == 0)
			memcpy(skb->data, dec_mac_host2dsp0, 6);
		else if (txCtrl->dsp_id == 1)
			memcpy(skb->data, dec_mac_host2dsp1, 6);		/* destination eth addr */
		else if (txCtrl->dsp_id == 2)
			memcpy(skb->data, dec_mac_host2dsp2, 6);
		else if (txCtrl->dsp_id == 3)
			memcpy(skb->data, dec_mac_host2dsp3, 6);
		else
		{
			//memcpy(skb->data, dec_mac_host2dsp0, 6);
			PRINT_R("No set for dec_mac_addr, cmd= %d\n", category);
		}
#elif defined CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
		memcpy(skb->data, dec_mac_dsp2host, 6);
#endif
	memcpy(skb->data+6, eth0_dev->dev_addr, 6); 				/* source ether addr */
	*((unsigned short*)&skb->data[ETH_TYPE_SHIFT]) = htons(eth_type);	/* packet type ID field */
	*((unsigned short*)&skb->data[DSP_ID_SHIFT]) = htons(txCtrl->dsp_id);		/* DSP ID field */
	*((unsigned char*)&skb->data[PROTOCOL_SHIFT]) = htons(pkt_type);	/* payload type field */	
	*((unsigned char*)&skb->data[VOIP_VER_SHIFT]) = htons(voip_ver);	/* VoIP Version field */	
	
	if ((pkt_type == ETH_CTRL_PKT) || (pkt_type == ETH_EVENT_PKT))
	{
		if (pkt_type == ETH_CTRL_PKT)
		{
			//PRINT_R("Ctrl(%d)\n", category);
		}
		else
		{
			//PRINT_R("Event(%d)\n", category);
		}
			
		*((unsigned short*)&skb->data[CATEGORY_SHIFT]) = htons(category);		/* category field */
		
		if (txCtrl->resend_flag == 1)
		{			
			unsigned short resend_seq;
			if (seq_no>0)
				resend_seq = seq_no - 1;
			else
				resend_seq = 65535;
			*((unsigned short*)&skb->data[SEQUENCE_SHIFT]) = htons(resend_seq);	/* sequence number field */
		}
		else
			*((unsigned short*)&skb->data[SEQUENCE_SHIFT]) = htons(seq_no);
			
		*((unsigned short*)&skb->data[CONT_LEN_SHIFT]) = htons(data_len);	/* content length field */
		memcpy(skb->data+CONTENT_SHIFT, pdata, data_len); 			/* content field */
	}
	else if (pkt_type == ETH_RESP_PKT)
	{
		if (txCtrl->seq_no == -1)
			PRINT_R("Error seq no. for send response packet\n");
			
		//PRINT_R("Resp(%d)\n", category);
		*((unsigned short*)&skb->data[CATEGORY_SHIFT]) = htons(category);	/* category field */
		*((unsigned short*)&skb->data[SEQUENCE_SHIFT]) = htons(txCtrl->seq_no);	/* sequence number field */
		if (data_len>0)
		{
			*((unsigned short*)&skb->data[CONT_LEN_SHIFT]) = htons(data_len);/* content length field */
			memcpy(skb->data+CONTENT_SHIFT, pdata, data_len); 		 /* content field */
		}
	}
	else if (pkt_type == ETH_VOICE2HOST_PKT || pkt_type == ETH_VOICE2DSP_PKT || pkt_type == ETH_T38_TO_HOST_PKT || pkt_type == ETH_T38_TO_DSP_PKT)
	{
		//PRINT_Y("%d \n", data_len);
		*((unsigned short*)&skb->data[VOICE_CONT_LEN_SHIFT]) = htons(data_len);	/* content length field*/
		memcpy(skb->data+VOICE_CONTENT_SHIFT, pdata, data_len);			/* content field */
	}
	else if (pkt_type == ETH_ACK_PKT)
	{
		if (txCtrl->seq_no == -1)
			PRINT_R("Error seq no. for send ack packet\n");
			
		//PRINT_R("Ack(%d)\n", category);
		*((unsigned short*)&skb->data[CATEGORY_SHIFT]) = htons(category);	/* category field */
		*((unsigned short*)&skb->data[SEQUENCE_SHIFT]) = htons(txCtrl->seq_no);	/* sequence number field */
	}

#if 0
	if (pkt_type == ETH_VOICE2HOST_PKT || pkt_type == ETH_VOICE2DSP_PKT || pkt_type == ETH_T38_TO_HOST_PKT || pkt_type == ETH_T38_TO_DSP_PKT)// data packet
		;
	else
	{
		int i;
		printk( "TX\n" );
		for( i = 0; i < 26; i ++ )
			printk( "%02X ", *( ( unsigned char * )skb->data + i ) );
			
		printk( "\n" );
	}
#endif

	
#if 0
	skb->dev->hard_start_xmit(skb, eth0_dev);
#else
	skb->priority = 7;
	dev_queue_xmit(skb);
#endif
	//PRINT_Y("send eth pkt...\n");
		
	if ( txCtrl->resend_flag != 1)
	{
		if (pkt_type == ETH_CTRL_PKT)
		{
			// Need response for control packet
			stCheckResponse.seqNo = seq_no++;
			stCheckResponse.category = category;
			stCheckResponse.needResponse = 1;
		}
		//else if ((pkt_type == ETH_EVENT_PKT) || ((pkt_type == ETH_RESP_PKT)&& (data_len>0)))
		else if (pkt_type == ETH_EVENT_PKT)
		{
			// Need Ack for event packet (or response packet which carry content for host)
			stCheckAck.seqNo = seq_no++;
			stCheckAck.category = category;
			stCheckAck.needAck = 1;
		}
	}
	

		switch(pkt_type)
		{
			case ETH_VOICE2HOST_PKT://DSP -> Host
#ifdef ETHERNET_DSP_DEBUG_DSP
				dsp_rtp_rtcp_tx_cnt++;
#endif
				break;
				
			case ETH_T38_TO_HOST_PKT://DSP -> Host
#ifdef ETHERNET_DSP_DEBUG_DSP
				dsp_t38_tx_cnt++;
#endif
				break;
			
			case ETH_RESP_PKT://DSP -> Host
#ifdef ETHERNET_DSP_DEBUG_DSP
				dsp_resp_tx_cnt++;
#endif
				break;

			case ETH_EVENT_PKT://DSP -> Host
#ifdef ETHERNET_DSP_DEBUG_DSP
				dsp_event_tx_cnt++;
#endif
				break;

			case ETH_VOICE2DSP_PKT://Host -> DSP
#ifdef ETHERNET_DSP_DEBUG_HOST
				host_rtp_rtcp_tx_cnt[txCtrl->dsp_id]++;
#endif
				break;
			
			case ETH_T38_TO_DSP_PKT://Host -> DSP
#ifdef ETHERNET_DSP_DEBUG_HOST
				host_t38_tx_cnt[txCtrl->dsp_id]++;
#endif
				break;
			
			case ETH_CTRL_PKT://Host -> DSP
#ifdef ETHERNET_DSP_DEBUG_HOST
				host_ctrl_tx_cnt[txCtrl->dsp_id]++;
#endif
				break;
				
			case ETH_ACK_PKT://Host -> DSP
#ifdef ETHERNET_DSP_DEBUG_HOST
				host_ack_tx_cnt[txCtrl->dsp_id]++;
#endif
				break;

			default:
				PRINT_R("?? @ %s, %s, line-%d\n", __FUNCTION__, __FILE__, __LINE__);
				break;
		}
				

	
	return seq_no-1;
}

voip_initcall(rtk_voip_ethernet_dsp_init);
voip_exitcall(rtk_voip_ethernet_dsp_exit);

// -----------------------------------------------------------------
// -----------------------------------------------------------------

int ethernetDspSentL2ControlPacket(unsigned short cmd, unsigned int chid, void* pMgr, unsigned short mgrLen)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
	unsigned long timestamp, timeout=50;	
	TstTxPktCtrl stTxPktCtrl;
	unsigned int resend_cnt = 0;
	
	/* Host chid, DSP ID, DSP chid conversion */
	stTxPktCtrl.host_chid = chid;
	stTxPktCtrl.dsp_id = API_get_DSP_ID(cmd, chid);

	/* Send Control Packet*/	
	voip_dsp_L2_pkt_tx(cmd, ETH_CTRL_PKT, pMgr, mgrLen, &stTxPktCtrl);
	
      	/* Wait Response */
      	timestamp = jiffies + timeout;
      	while(host_need_response())
      	{
      		if (time_after(jiffies, timestamp) )
      		{
      			/* Re-send Control Packet*/
      			stTxPktCtrl.seq_no = 0;
      			stTxPktCtrl.resend_flag = 1;
      			voip_dsp_L2_pkt_tx(cmd, ETH_CTRL_PKT, pMgr, mgrLen, &stTxPktCtrl);
      			PRINT_R("%d ", cmd-2116);
      			timestamp = jiffies+timeout;
      			
      			resend_cnt++;
      			if (resend_cnt > RESEND_THS)
      			{
      				PRINT_R("FAIL!\n");
      				return -EVOIP_RESEND_ERR;
      			}
      		}
      	}
      	
      	return 0;
#endif
}

int ethernetDspSentL2ControlPacketNoChannel(unsigned short cmd, void* pMgr, unsigned short mgrLen)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST

	unsigned long timestamp, timeout=50;	
	TstTxPktCtrl stTxPktCtrl;
	unsigned int resend_cnt[DSP_DEVICE_NUM] = {[0 ... DSP_DEVICE_NUM-1] = 0};
	int i=0;
	
	// global setting for every DSP
	for (i=0; i<API_get_DSP_NUM(); i++)
	{
		stTxPktCtrl.dsp_id = i;

		/* Send Control Packet*/	
		voip_dsp_L2_pkt_tx(cmd, ETH_CTRL_PKT, pMgr, mgrLen, &stTxPktCtrl);
		
	      	/* Wait Response */
	      	timestamp = jiffies + timeout;
	      	while(host_need_response())
	      	{
	      		if (time_after(jiffies, timestamp) )
	      		{
	      			/* Re-send Control Packet*/
	      			stTxPktCtrl.seq_no = 0;
	      			stTxPktCtrl.resend_flag = 1;
	      			voip_dsp_L2_pkt_tx(cmd, ETH_CTRL_PKT, pMgr, mgrLen, &stTxPktCtrl);
	      			PRINT_R("%d ", cmd-2116);
	      			timestamp = jiffies+timeout;
	      			
	      			resend_cnt[i]++;
      				if (resend_cnt[i] > RESEND_THS)
      					return -EVOIP_RESEND_ERR;
	      		}
	      	}
	}
	
	return 0;
#endif
}

int ethernetDspCheckL2RespPacket(int cmd, void* pCont, unsigned short* pDsp_id)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
	unsigned char* pRespCont;
      	unsigned short resp_len, resp_cat, resp_dsp_id;
      	
      	/* Get Response pkt, and copy content to user*/
      	resp_len = get_response_content(&resp_dsp_id, &pRespCont, &resp_cat);
      	if (( pRespCont == NULL) || (resp_len == 0) || (resp_cat != cmd))
      	{
		PRINT_R("Error, No Content for Respnose pkt, or cmd(%d) not match(%d), %s-%s-%d\n", cmd, resp_cat,  __FILE__, __FUNCTION__, __LINE__);
      		return -1;
      	}
	memcpy(pCont, pRespCont, resp_len);
	*pDsp_id = resp_dsp_id;
 
 	return 0;     	
 #endif
}



extern int last_process_seqno;

int ethernetDspSentL2ResponsePacket(unsigned short cmd, unsigned short seq_no, void* pdata, unsigned short data_len)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP

	int ret_val = 0;
	TstTxPktCtrl stTxPktCtrl;
	stTxPktCtrl.dsp_id = Get_Ethernet_DSP_ID();
	
	stTxPktCtrl.seq_no = seq_no;
	if (last_process_seqno == seq_no)
	{
		stTxPktCtrl.resend_flag = 1;
		ret_val = -EVOIP_DUP_SEQ;
		PRINT_Y("Get the same seq ctrl pkt for cmd%d\n", cmd - 2116);
	}
	else
		stTxPktCtrl.resend_flag = 0;
		
	voip_dsp_L2_pkt_tx(cmd, ETH_RESP_PKT, pdata, data_len, &stTxPktCtrl);
	
	return ret_val;
#else
	return 0;
#endif
	
}

void ethernetDsp_RtpTx(RtpPacket* pst)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	// Send RTP packet to Host by voice packet.
	TstTxPktCtrl stTxPktCtrl;
	stTxPktCtrl.dsp_id = Get_Ethernet_DSP_ID();
	stTxPktCtrl.seq_no = -1;
	stTxPktCtrl.resend_flag = 0;
	unsigned char rtp_wrtie_tmp[500];
	unsigned int len, mid;
	
	*(unsigned int*)rtp_wrtie_tmp= pst->chid;
	*(unsigned int*)((unsigned char*)rtp_wrtie_tmp+4)= pst->sid;
	len = getTotalUsage(pst);
	
	if (len > (500-8))
	{
		PRINT_R("Error! no enought buf size, %s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
	}
	memcpy(&rtp_wrtie_tmp[8], pst->header, len);
	
	voip_dsp_L2_pkt_tx(0, ETH_VOICE2HOST_PKT, (unsigned char*)rtp_wrtie_tmp, len+8, &stTxPktCtrl);
	// Note: pst->header-pst_offset point to the chid address, so that receiver can get chid, sid, header info. Refer to the structure RtpPacket.
	// sizeof(RtpPacket) = 1524 bytes > 1518 bytes
	// PRINT_R("%d ", getTotalUsage(pst));
#endif
}

void ethernetDsp_RtcpTx(RtcpPacket* pst)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	extern int rtcp_sid_offset;
	// Send RTCP packet to Host by voice packet.
	TstTxPktCtrl stTxPktCtrl;
	stTxPktCtrl.dsp_id = Get_Ethernet_DSP_ID();
	stTxPktCtrl.seq_no = -1;
	stTxPktCtrl.resend_flag = 0;
	unsigned char rtcp_wrtie_tmp[600];
	unsigned int len;
	
	*(unsigned int*)rtcp_wrtie_tmp = pst->chid;
	*(unsigned int*)((unsigned char*)rtcp_wrtie_tmp+4) = pst->sid + rtcp_sid_offset;
	len = RtcpPkt_getTotalUsage(pst);
	
	//PRINT_Y("rtcp len=%d\n", len); //len=100
	if (len > (600-8))
	{
		PRINT_R("Error! no enought buf size, %s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
		PRINT_R("Need size: %d bytes.", len);
	}
	memcpy(&rtcp_wrtie_tmp[8], RtcpPkt_getPacketData(pst), len);
	
	voip_dsp_L2_pkt_tx(0, ETH_VOICE2HOST_PKT, (unsigned char*)rtcp_wrtie_tmp, len+8, &stTxPktCtrl);
	// Note: pst->header-pst_offset point to the chid address, so that receiver can get chid, sid, header info. Refer to the structure RtpPacket.
	//PRINT_R("%d ", getTotalUsage(pst));
#endif
}

void ethernetDsp_T38Tx( unsigned int chid, unsigned int sid, void* packet, unsigned int pktLen)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP

	// Send T38 packet to Host
	TstTxPktCtrl stTxPktCtrl;
	stTxPktCtrl.dsp_id = Get_Ethernet_DSP_ID();
	stTxPktCtrl.seq_no = -1;
	stTxPktCtrl.resend_flag = 0;
	unsigned char t38_tx_tmp[500];
	unsigned int chid_new, sid_new, mid, len;
	
	*(unsigned int*)t38_tx_tmp = chid;
	*(unsigned int*)((unsigned char*)t38_tx_tmp+4) = sid;
	
	if (pktLen > (500-8))
	{
		PRINT_R("Error! no enought buf size, %s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
	}
	memcpy(&t38_tx_tmp[8], packet, pktLen);
	
	voip_dsp_L2_pkt_tx(0, ETH_T38_TO_HOST_PKT, (unsigned char*)t38_tx_tmp, pktLen+8, &stTxPktCtrl);
	//printk("t%d ", chid);
	
#endif
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
void ethernetDsp_Host2DSP_T38Tx( const struct RTK_TRAP_profile *ptr, 
								 const struct sk_buff *skb )
{
	/* Send T.38 packet to DSP */
	TstTxPktCtrl stTxPktCtrl;
	unsigned char t38_read_tmp[1500];
	unsigned int chid, mid, sid, len;
	stTxPktCtrl.dsp_id = API_get_DSP_ID(0, ptr->c_id);
	stTxPktCtrl.seq_no = -1;
	stTxPktCtrl.resend_flag = 0;
	
	/* Transfer Host chid, sid to DSP ID, chid */
	chid = API_get_DSP_CH(0, ptr->c_id);
	mid = API_GetMid(ptr->c_id, ptr->s_id);
	sid = API_GetSid(chid, mid);
	
	*(uint32*)t38_read_tmp = chid; // chid
	*(uint32*)((unsigned char*)t38_read_tmp+4) = sid; // sid
	
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,21))
	struct iphdr *iph = ip_hdr(skb);
	len = (iph->tot_len) - 28;
#else
	len = ((skb->nh.iph)->tot_len) - 28;
#endif		
	if ( len > (1500-8))
	{
		PRINT_R("Error! no enought buf size, %s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
		PRINT_R("Need %d bytes, sid=%d\n", len, ptr->s_id);
	}
	memcpy(&t38_read_tmp[8], skb->data + sizeof(struct udphdr), len);
	
	voip_dsp_L2_pkt_tx(0, ETH_T38_TO_DSP_PKT, (unsigned char*)t38_read_tmp, len+8, &stTxPktCtrl);
	//printk("T%d ", ptr->c_id);
}
#endif

