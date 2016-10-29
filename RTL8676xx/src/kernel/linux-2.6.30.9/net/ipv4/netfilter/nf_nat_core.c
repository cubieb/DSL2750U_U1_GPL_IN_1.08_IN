/* NAT for netfilter; shared with compatibility layer. */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/skbuff.h>
#include <net/checksum.h>
#include <net/icmp.h>
#include <net/ip.h>
#include <net/tcp.h>  /* For tcp_prot in getorigdst */
#include <linux/icmp.h>
#include <linux/udp.h>
#include <linux/jhash.h>

#include <linux/netfilter_ipv4.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_protocol.h>
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_nat_helper.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/rtl/rtl_alias.h>
#ifdef CONFIG_RTL_LAYERED_DRIVER
#include <linux/inetdevice.h>
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl_nic.h>
//#include <AsicDriver/rtl865x_asicCom.h>

#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
#include <net/rtl/rtl865x_nat.h>
#endif
#else
#define __DRAM_GEN
#define uint32 unsigned int
#endif /*CONFIG_RTL_LAYERED_DRIVER*/

#ifdef	CONFIG_RTL_IPTABLES_FAST_PATH
#include <net/rtl/fastpath/fastpath_core.h>
#if defined(IMPROVE_QOS)
#if defined(CONFIG_FAST_PATH_MODULE)
enum LR_RESULT (*FastPath_hook4)( enum NP_PROTOCOL protocol, ipaddr_t intIp, uint32 intPort,
                                                               ipaddr_t extIp, uint32 extPort,
                                                               ipaddr_t remIp, uint32 remPort )=NULL;
enum LR_RESULT (*FastPath_hook6)( enum NP_PROTOCOL protocol, ipaddr_t intIp, uint32 intPort,
                                                               ipaddr_t extIp, uint32 extPort,
                                                               ipaddr_t remIp, uint32 remPort,
                                                               enum NP_FLAGS flags)=NULL;
enum LR_RESULT (*FastPath_hook11)(enum NP_PROTOCOL protocol,
		ipaddr_t intIp, uint32 intPort,
		ipaddr_t extIp, uint32 extPort,
		ipaddr_t remIp, uint32 remPort,
		uint32 interval)=NULL;
EXPORT_SYMBOL(FastPath_hook4);
EXPORT_SYMBOL(FastPath_hook6);
EXPORT_SYMBOL(FastPath_hook11);
#endif
#endif
#endif


#if defined(CONFIG_NET_SCHED)
__DRAM_GEN int gQosEnabled=0;
#endif


#if defined(CONFIG_PROC_FS) && defined(CONFIG_NET_SCHED)
#define	QOS_CLASSIFY_INFO_LEN	16
typedef struct {
	/*	classify	*/
	unsigned int protocol;
	struct in_addr local_ip;
	struct in_addr  remote_ip;
	unsigned short lo_port;
	unsigned short re_port;

	/*	tc	*/
	uint32	mark;
	unsigned char	prio;
} rtl865x_qos_cache_t;

static struct proc_dir_entry *proc_qos=NULL;

static char *gQosSetting = NULL;
#ifdef CONFIG_FAST_PATH_MODULE
EXPORT_SYMBOL(gQosEnabled);
#endif

static int qos_read_proc(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
      int len;

      len = sprintf(page, "%s\n", gQosSetting);

      if (len <= off+count) *eof = 1;
      *start = page + off;
      len -= off;
      if (len>count) len = count;
      if (len<0) len = 0;
      return len;
}

static int qos_write_proc(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
      if ( gQosSetting==NULL || count < 2) 
	    return -EFAULT;

      if (buffer && !copy_from_user(gQosSetting, buffer, count)) {
          gQosSetting[count-1] = 0; /* remove 0x0a */
          if (gQosSetting[0] == '0')
                gQosEnabled = 0;
          else
                gQosEnabled = 1;

	    return count;
      }
      return -EFAULT;
}

#endif

static DEFINE_SPINLOCK(nf_nat_lock);

static struct nf_conntrack_l3proto *l3proto __read_mostly;

/* Calculated at init based on memory size */
static unsigned int nf_nat_htable_size __read_mostly;

#define MAX_IP_NAT_PROTO 256
static const struct nf_nat_protocol *nf_nat_protos[MAX_IP_NAT_PROTO]
						__read_mostly;

#ifdef CONFIG_RTL_HARDWARE_NAT
__DRAM_GEN int gHwNatEnabled;

int rtl865x_handle_nat(struct nf_conn *ct, int act)
{
	struct nf_conn_nat *nat;
	u_int32_t sip, dip, gip;
	u_int16_t sp, dp, gp, proto=0;
	u_int32_t timeval;
	int rc=0;


	if (gHwNatEnabled!=1)
		return -1;

	proto = (ct->tuplehash[0].tuple.dst.protonum==IPPROTO_TCP)? 1: 0;

	if (ct->status & IPS_SRC_NAT)
	{ /* outbound flow */
		sip	= ct->tuplehash[0].tuple.src.u3.ip;
		dip 	= ct->tuplehash[0].tuple.dst.u3.ip;
		gip 	= ct->tuplehash[1].tuple.dst.u3.ip;
		sp  	= (proto)? ct->tuplehash[0].tuple.src.u.tcp.port: ct->tuplehash[0].tuple.src.u.udp.port;
		dp  	= (proto)? ct->tuplehash[0].tuple.dst.u.tcp.port: ct->tuplehash[0].tuple.dst.u.udp.port;
		gp  	= (proto)? ct->tuplehash[1].tuple.dst.u.tcp.port: ct->tuplehash[1].tuple.dst.u.udp.port;
	} 
	else if (ct->status & IPS_DST_NAT)
	{ /* inbound flow */
		sip	= ct->tuplehash[1].tuple.src.u3.ip;
		dip 	= ct->tuplehash[1].tuple.dst.u3.ip;
		gip 	= ct->tuplehash[0].tuple.dst.u3.ip;
		sp  	= (proto)? ct->tuplehash[1].tuple.src.u.tcp.port: ct->tuplehash[1].tuple.src.u.udp.port;
		dp  	= (proto)? ct->tuplehash[1].tuple.dst.u.tcp.port: ct->tuplehash[1].tuple.dst.u.udp.port;
		gp  	= (proto)? ct->tuplehash[0].tuple.dst.u.tcp.port: ct->tuplehash[0].tuple.dst.u.udp.port;
	}
	else 
		return -1;

	/* do not add hardware NAPT table if protocol is UDP and source IP address is equal to gateway IP address */
	if ((act == 1) && (proto == 0) && (sip == gip))
		return -1;

	/* for TZO DDNS */
	if ((act == 1) && (proto == 1) && (dp == 21347)) {
		return -1;
	}

	if (act == 2) {
		/* query for idle */
		timeval = 0;
#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
		timeval = rtl865x_naptSync(proto, sip, sp, gip, gp, dip, dp, 0);
#endif
		if (timeval > 0)
			return 0;
		else
			return -1;
	}
	else if (act == 0) {
		/* delete */
		rc = 0;
#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
		rc = rtl865x_delNaptConnection(proto, sip, sp, gip, gp, dip, dp);
#endif
	}
	else {
		/* add */
#if defined(CONFIG_PROC_FS) && defined(CONFIG_NET_SCHED) && !defined(CONFIG_RTL_HW_QOS_SUPPORT)
		if (gQosEnabled == 0)
		{
#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
			rc =  rtl865x_addNaptConnection(proto, sip, sp, gip, gp, dip, dp, 0);
#endif
		}
		else
		{
			act = 0;
		}
#else
#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
		rc =  rtl865x_addNaptConnection(proto, sip, sp, gip, gp, dip, dp, 0);
#endif

#endif
       }

	nat = nfct_nat(ct);
	//if (!rc && act == 1) /* mark it as an asic entry */
	if (!rc && nat && act == 1) /* patch from ap team driver, 2011/4/12*/
		nat->hw_acc = 1;
	//if (!rc && act == 0) /* unmark it */
	if (!rc && nat && act == 0) /* patch from ap team driver, 2011/4/12*/
		nat->hw_acc = 0;

	#ifdef CONFIG_HARDWARE_NAT_DEBUG
	/*2007-12-19*/
	printk("%s:%d:(%s): errno=%d\n %s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) g:(%u.%u.%u.%u:%u)\n",
			__FUNCTION__,__LINE__,((act==1)?"add_nat": "del_nat"), rc, ((proto)? "tcp": "udp"), 
			NIPQUAD(sip), sp, NIPQUAD(dip), dp, NIPQUAD(gip), gp);	
	#endif

	return 0;	
}

#endif

#ifdef CONFIG_RTL865X_HW_PPTPL2TP

//__DRAM_GEN struct pptp_acc_info pptpAccInfo = { 0 };
extern __DRAM_GEN struct pptp_acc_info pptpAccInfo;

int rtl865x_addPppInfoChannel(struct net_device *pppDev, struct ppp_channel *pppChan)
{
	if (!pppDev || !pppChan)
		return -1;

	pptpAccInfo.pppDev = pppDev;
	pptpAccInfo.pppChan = pppChan;
	return 0;
}

int rtl865x_delPppInfoChannel(struct net_device *pppDev, struct ppp_channel *pppChan)
{
	if (!pppDev || !pppChan)
		return -1;

	if (pptpAccInfo.pppDev == pppDev) {
		pptpAccInfo.pppDev = NULL;
		pptpAccInfo.pppChan = NULL;
	}
	return 0;
}

int rtl865x_filterPptp(struct sk_buff *skb)
{
	return 0;
}

__IRAM_EXTDEV int rtl865x_acceleratePptpToWanSync(struct sk_buff *skb)
{
	struct iphdr ip, *iph = (struct iphdr *)(skb->data + ETH_HLEN);
	struct pptp_gre_hdr gre, *greh = (struct pptp_gre_hdr *)((char *)iph + (iph->ihl << 2));

	if (gHwNatEnabled != 1)
		return -1;

#if 0
	if (!pptpAccInfo.valid || !pptpAccInfo.pppDev || !pptpAccInfo.wanDev) {
		return -1;
	}
#endif

	if (iph->protocol == IPPROTO_GRE && skb->len >= (ETH_HLEN + (iph->ihl << 2) + sizeof(struct pptp_gre_hdr) - 8)) {
		if ((greh->version & 7) == GRE_VERSION_PPTP &&
			ntohs(greh->protocol) == GRE_PROTOCOL_PPTP) {
			if (skb->dev == pptpAccInfo.wanDev &&
				iph->saddr == pptpAccInfo.ourIp && 
				iph->daddr == pptpAccInfo.peerIp && 
				greh->call_id == pptpAccInfo.peerCallID) {
				/* Matched */
				if (GRE_IS_S(greh->flags)) {
					/* Others go head !? */
					unsigned int seq = ntohl(greh->seq);
					if (time_after(seq, pptpAccInfo.tx_seqno)) {
						pptpAccInfo.tx_seqno = seq + 1;	
					}
					else {
	   					greh->seq = htonl(pptpAccInfo.tx_seqno++);	
	   				}
				}			
				if (GRE_IS_A(greh->version)) {
					/* Others go head !? */
					unsigned int ack = ntohl(greh->ack);
					if (time_after(ack, pptpAccInfo.rx_seqno)) {
						pptpAccInfo.rx_seqno = ack;	
					}
					else {
	   					greh->ack = htonl(pptpAccInfo.rx_seqno);	
	   				}					
				}
			}
		}
	}			

	return 0;
}

__IRAM_EXTDEV static inline int rtl865x_acceleratePptpToWAN(struct sk_buff *skb)
{
	int	headroom;
	struct ethhdr *eth;
	struct iphdr ip, *iph;
	struct pptp_gre_hdr	gre, *greh;	
	unsigned char tos = skb->nh.iph->tos;

	headroom = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct pptp_gre_hdr);
	if (skb_headroom(skb) < headroom || skb_cloned(skb) || skb_shared(skb)) {	
		struct sk_buff *new_skb = skb_realloc_headroom(skb, headroom);				
		if (!new_skb) {
			return -1;
		}									
		dev_kfree_skb(skb);
		skb = new_skb;
	}			

	eth = (struct ethhdr *)skb_push(skb, headroom);
	memcpy(eth->h_dest, pptpAccInfo.peerMac, ETH_ALEN);
	memcpy(eth->h_source, pptpAccInfo.ourMac, ETH_ALEN);
	eth->h_proto = htons(0x0800);

	iph = (struct iphdr *)((char *)eth + sizeof(struct ethhdr));
	if (((unsigned long)iph) & 0x03) {
		ip.version  = 4;
		ip.ihl      = sizeof(struct iphdr) >> 2;
		ip.frag_off = 0;			
		ip.protocol = IPPROTO_GRE;
		ip.tos      = tos;
		ip.daddr    = pptpAccInfo.peerIp;
		ip.saddr    = pptpAccInfo.ourIp;
		ip.ttl      = IPDEFTTL;   
		ip.tot_len  = htons(skb->len - sizeof(struct ethhdr));		
		ip.id       = htons(++pptpAccInfo.tx_ipID);
	   	ip.check    = 0;
	   	/*ip.check    = ip_fast_csum((unsigned char *)&ip, ip.ihl);*/
		memcpy(iph, &ip, sizeof(struct iphdr));
	}
	else {
		iph->version  = 4;
		iph->ihl      = sizeof(struct iphdr) >> 2;
		iph->frag_off = 0;			
		iph->protocol = IPPROTO_GRE;
		iph->tos      = tos;
		iph->daddr    = pptpAccInfo.peerIp;
		iph->saddr    = pptpAccInfo.ourIp;
		iph->ttl      = IPDEFTTL;   
		iph->tot_len  = htons(skb->len - sizeof(struct ethhdr));		
		iph->id       = htons(++pptpAccInfo.tx_ipID);
	   	iph->check    = 0;
	   	/*iph->check    = ip_fast_csum((unsigned char *)iph, iph->ihl);*/
	}		
	/*skb->ip_summed = CHECKSUM_NONE;*/
	/*skb->ip_summed = CHECKSUM_PARTIAL;*/

	greh = (struct pptp_gre_hdr *)((char *)iph + sizeof(struct iphdr));
	if (((unsigned long)greh) & 0x03) {
	   	gre.flags       = GRE_FLAG_K | GRE_FLAG_S;
	   	gre.version     = GRE_VERSION_PPTP | GRE_FLAG_A;
	   	gre.protocol    = htons(GRE_PROTOCOL_PPTP);
	   	gre.payload_len = htons(skb->len - headroom);    	
	   	gre.call_id     = pptpAccInfo.peerCallID;			
	   	gre.seq         = htonl(pptpAccInfo.tx_seqno++);		
	   	gre.ack         = htonl(pptpAccInfo.rx_seqno);
		memcpy(greh, &gre, sizeof(struct pptp_gre_hdr));
	}
	else {
	   	greh->flags       = GRE_FLAG_K | GRE_FLAG_S;
	   	greh->version     = GRE_VERSION_PPTP | GRE_FLAG_A;
	   	greh->protocol    = htons(GRE_PROTOCOL_PPTP);
	   	greh->payload_len = htons(skb->len - headroom);    	
	   	greh->call_id     = pptpAccInfo.peerCallID;			
	   	greh->seq         = htonl(pptpAccInfo.tx_seqno++);		
	   	greh->ack         = htonl(pptpAccInfo.rx_seqno);
	}		

   	skb->dev = pptpAccInfo.wanDev;
	/* dev_queue_xmit(skb);*/

	{
		rtl865x_acceleratePptpToWanSync(skb);
	}

	re865x_pptp_xmit(skb, skb->dev, FALSE);

	return 0;
}

__IRAM_EXTDEV int rtl865x_acceleratePptpToLAN(struct sk_buff *skb)
{
	struct iphdr ip, *iph = skb->nh.iph;
	struct pptp_gre_hdr gre, *greh = (struct pptp_gre_hdr *)(skb->nh.raw + (iph->ihl << 2));
	int offset;
	unsigned char *p;

#if 0
	if (!pptpAccInfo.valid || !pptpAccInfo.pppDev || !pptpAccInfo.wanDev) {
		return NET_RX_SUCCESS;
	}
#endif

	/* PPTP GRE */
	if ((greh->version & 7) == GRE_VERSION_PPTP &&
		ntohs(greh->protocol) == GRE_PROTOCOL_PPTP) {
		/* Unalignment !? */
		if (((unsigned long)iph) & 0x03) {
			memcpy(&ip, iph, sizeof(struct iphdr));
			iph = &ip;
		}
		if (((unsigned long)greh) & 0x03) {
			memcpy(&gre, greh, sizeof(struct pptp_gre_hdr));
			greh = &gre;
		}

#if 0
		DEBUGP("com1: dev[%s], dip[0x%x], sip[0x%x], call_id[%d].\n", skb->dev->name, 
			iph->daddr, iph->saddr, greh->call_id);
		DEBUGP("com2: dev[%s], dip[0x%x], sip[0x%x], call_id[%d][%d].\n", pptpAccInfo.wanDev->name, 
			pptpAccInfo.ourIp, pptpAccInfo.peerIp, pptpAccInfo.ourCallID,
			pptpAccInfo.peerCallID);
#endif
		if (skb->dev == pptpAccInfo.wanDev &&
			iph->daddr == pptpAccInfo.ourIp && 
			iph->saddr == pptpAccInfo.peerIp && 
			greh->call_id == pptpAccInfo.ourCallID) {
			/* Matched */
			if (greh->payload_len == 0)
			{
				return NET_RX_SUCCESS;
			}

			offset = (iph->ihl << 2) + sizeof(struct pptp_gre_hdr) - 8;				
			if (GRE_IS_S(greh->flags)) {	
				pptpAccInfo.rx_seqno = ntohl(greh->seq);						
				offset += sizeof(greh->seq);
			}	
			if (GRE_IS_A(greh->version)) {
				offset += sizeof(greh->ack);
			}
			
			/* strip address/control field if present */
			p = skb->nh.raw + offset;
			if (p[0] == 0xff /* PPP_ALLSTATIONS */ && p[1] == 0x03 /* PPP_UI */) {			
				/* chop off address/control */
				if (greh->payload_len < 3)
				{
					return NET_RX_SUCCESS;
				}
				offset += 2;
				p += 2;
			}
			/* decompress protocol field if compressed */
			if (p[0] & 1) {
				/* protocol is compressed */
				offset -= 1;
				skb_pull(skb, offset)[0] = 0;
			} 
			else {
				if (greh->payload_len < 2)
				{
					return NET_RX_SUCCESS;
				}
				skb_pull(skb, offset);
			}

//			skb->cb[0] = 'P'; skb->cb[1] = 'P'; skb->cb[2] = 'P';
			ppp_input(pptpAccInfo.pppChan, skb);
			memset(skb->cb, '\x0', 3);
			return NET_RX_DROP;
		}
	}

	memset(skb->cb, '\x0', 3);
	return NET_RX_SUCCESS;
}

#if 1
__IRAM_EXTDEV int rtl865x_pppAcceleratePptpL2tpToWAN(struct sk_buff *skb, struct net_device *pppDev)
{
	if (gHwNatEnabled != 1)
		return -1;

#if 0
	if (!pptpAccInfo.valid || !pptpAccInfo.pppDev || !pptpAccInfo.wanDev || pptpAccInfo.pppDev != pppDev) {
		return -1;
	}
#endif
	return (rtl865x_acceleratePptpToWAN(skb));
}
#endif

__IRAM_EXTDEV int rtl865x_pppAcceleratePptpL2tpToLAN(struct sk_buff *skb, struct net_device *pppDev)
{
	struct ethhdr *eth;

	if (gHwNatEnabled != 1)
		return -1;

#if 0
	if ( (gHwNatEnabled != 1) || (!pptpAccInfo.valid || !pptpAccInfo.pppDev || !pptpAccInfo.wanDev || pptpAccInfo.pppDev != skb->dev))
	{
		return -1;
	}
#endif

	{
		struct iphdr *iph;
		struct tcphdr *tcphupuh;  //just keep one , don't care tcp or udp //

		iph = (struct iphdr*)(skb->data);
		tcphupuh = (struct tcphdr*)((__u32 *)iph + iph->ihl);

#if 0
		if ((iph->protocol==IPPROTO_TCP)&&(tcphupuh->fin||tcphupuh->rst||tcphupuh->syn))
			DEBUGP("fin %d rst %d syn %d.\n", tcphupuh->fin, tcphupuh->rst, tcphupuh->syn);
#endif
		if (!((iph->protocol==IPPROTO_UDP) || 
			((iph->protocol==IPPROTO_TCP)&&!(tcphupuh->fin||tcphupuh->rst||tcphupuh->syn))))
		{
#if 0
			DEBUGP("iph 0x%p: %u.%u.%u.%u==>%u.%u.%u.%u [0x%x]  %c.%c.%c\n", 
				iph, NIPQUAD(iph->saddr), NIPQUAD(iph->daddr), iph->protocol, 
				skb->cb[0], skb->cb[1], skb->cb[2]);
#endif
//			skb->cb[0] = skb->cb[1] = skb->cb[2] = 0;
			memset(skb->cb, '\x0', 3);
			netif_rx(skb);
			pppDev->last_rx = jiffies;
			return SUCCESS;
		}
	}
	
	if (skb_headroom(skb) < sizeof(struct ethhdr) || skb_cloned(skb) || skb_shared(skb)) {
		/*DEBUGP("header room too small: %d:%d.\n", skb_headroom(skb), sizeof(struct ethhdr));*/
		struct sk_buff *new_skb = skb_realloc_headroom(skb, sizeof(struct ethhdr));				
		if (!new_skb) {				
			return -1;
		}
		dev_kfree_skb(skb);
		skb = new_skb;
	}
		
	skb_push(skb, sizeof(struct ethhdr));
	eth = (struct ethhdr *)skb->data;
	memcpy(eth->h_dest, pptpAccInfo.ourMac, ETH_ALEN);
	memcpy(eth->h_source, pptpAccInfo.peerMac, ETH_ALEN);
	eth->h_proto = htons(0x0800);
	re865x_pptp_xmit(skb, skb->dev, TRUE);
	return 0;
}

__IRAM_FWD int rtl865x_acceleratePptpL2tp(struct sk_buff **pskb) 
{
	struct sk_buff *skb = *pskb;
	struct iphdr *iph;
	
	if (gHwNatEnabled != 1)
		return NET_RX_SUCCESS;
		
	if (ntohs(((struct ethhdr*)skb->mac.raw)->h_proto) != 0x0800) {
		return NET_RX_SUCCESS;
	}

	skb->h.raw = skb->nh.raw = skb->data;
#if 0	
	if (pptpAccInfo.pppDev!=NULL)
	{
		/* pptp wan type */
		if (!memcmp(skb->dev->name, "eth0", 4)) //
		{
			/* from lan -> wan */
			pptpAccInfo.pppDev->hard_start_xmit(skb, pptpAccInfo.pppDev);
			return NET_RX_DROP;
		}
		else if(!memcmp(skb->dev->name, "eth1", 4))
		{
			/* from wan -> lan */
			iph = skb->nh.iph;
			if (iph->protocol == IPPROTO_GRE && skb->len >= ((iph->ihl << 2) + sizeof(struct pptp_gre_hdr))) {
				return (rtl865x_acceleratePptpToLAN(skb));
			}
		}
	}

#else
//	if (skb->cb[0] == 'P' && skb->cb[1] == 'P' && skb->cb[2] == 'P')
	if (!memcmp(skb->cb, "PPP", 3))
	{
//		if (pptpAccInfo.pppDev)
		{
			pptpAccInfo.pppDev->hard_start_xmit(skb, pptpAccInfo.pppDev);
			memset(skb->cb, '\x0', 3);
			return NET_RX_DROP;
		}
	}
//	else if (skb->cb[0] == 'N' && skb->cb[1] == 'A' && skb->cb[2] == 'T')
	else if (!memcmp(skb->cb, "NNN", 3))
	{
		iph = skb->nh.iph;
		if (iph->protocol == IPPROTO_GRE && skb->len >= ((iph->ihl << 2) + sizeof(struct pptp_gre_hdr))) {
			return (rtl865x_acceleratePptpToLAN(skb));
		}
	}
#endif	
	return NET_RX_SUCCESS;	
}	
#endif




static inline const struct nf_nat_protocol *
__nf_nat_proto_find(u_int8_t protonum)
{
	return rcu_dereference(nf_nat_protos[protonum]);
}

const struct nf_nat_protocol *
nf_nat_proto_find_get(u_int8_t protonum)
{
	const struct nf_nat_protocol *p;

	rcu_read_lock();
	p = __nf_nat_proto_find(protonum);
	if (!try_module_get(p->me))
		p = &nf_nat_unknown_protocol;
	rcu_read_unlock();

	return p;
}
EXPORT_SYMBOL_GPL(nf_nat_proto_find_get);

void
nf_nat_proto_put(const struct nf_nat_protocol *p)
{
	module_put(p->me);
}
EXPORT_SYMBOL_GPL(nf_nat_proto_put);

/* We keep an extra hash for each conntrack, for fast searching. */
static inline unsigned int
hash_by_src(const struct nf_conntrack_tuple *tuple)
{
	unsigned int hash;

	/* Original src, to ensure we map it consistently if poss. */
	hash = jhash_3words((__force u32)tuple->src.u3.ip,
			    (__force u32)tuple->src.u.all,
			    tuple->dst.protonum, 0);
	return ((u64)hash * nf_nat_htable_size) >> 32;
}

/* Is this tuple already taken? (not by us) */
int
nf_nat_used_tuple(const struct nf_conntrack_tuple *tuple,
		  const struct nf_conn *ignored_conntrack)
{
	/* Conntrack tracking doesn't keep track of outgoing tuples; only
	   incoming ones.  NAT means they don't have a fixed mapping,
	   so we invert the tuple and look for the incoming reply.

	   We could keep a separate hash if this proves too slow. */
	struct nf_conntrack_tuple reply;

	nf_ct_invert_tuplepr(&reply, tuple);
	return nf_conntrack_tuple_taken(&reply, ignored_conntrack);
}
EXPORT_SYMBOL(nf_nat_used_tuple);

/* If we source map this tuple so reply looks like reply_tuple, will
 * that meet the constraints of range. */
static int
in_range(const struct nf_conntrack_tuple *tuple,
	 const struct nf_nat_range *range)
{
	const struct nf_nat_protocol *proto;
	int ret = 0;

	/* If we are supposed to map IPs, then we must be in the
	   range specified, otherwise let this drag us onto a new src IP. */
	if (range->flags & IP_NAT_RANGE_MAP_IPS) {
		if (ntohl(tuple->src.u3.ip) < ntohl(range->min_ip) ||
		    ntohl(tuple->src.u3.ip) > ntohl(range->max_ip))
			return 0;
	}

	rcu_read_lock();
	proto = __nf_nat_proto_find(tuple->dst.protonum);
	if (!(range->flags & IP_NAT_RANGE_PROTO_SPECIFIED) ||
	    proto->in_range(tuple, IP_NAT_MANIP_SRC,
			    &range->min, &range->max))
		ret = 1;
	rcu_read_unlock();

	return ret;
}

static inline int
same_src(const struct nf_conn *ct,
	 const struct nf_conntrack_tuple *tuple)
{
	const struct nf_conntrack_tuple *t;

	t = &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple;
	return (t->dst.protonum == tuple->dst.protonum &&
		t->src.u3.ip == tuple->src.u3.ip &&
		t->src.u.all == tuple->src.u.all);
}

/* Only called for SRC manip */
static int
find_appropriate_src(struct net *net,
		     const struct nf_conntrack_tuple *tuple,
		     struct nf_conntrack_tuple *result,
		     const struct nf_nat_range *range)
{
	unsigned int h = hash_by_src(tuple);
	const struct nf_conn_nat *nat;
	const struct nf_conn *ct;
	const struct hlist_node *n;

	rcu_read_lock();
	hlist_for_each_entry_rcu(nat, n, &net->ipv4.nat_bysource[h], bysource) {
		ct = nat->ct;
		if (same_src(ct, tuple)) {
			/* Copy source part from reply tuple. */
			nf_ct_invert_tuplepr(result,
				       &ct->tuplehash[IP_CT_DIR_REPLY].tuple);
			result->dst = tuple->dst;

			if (in_range(result, range)) {
				rcu_read_unlock();
				return 1;
			}
		}
	}
	rcu_read_unlock();
	return 0;
}

/* For [FUTURE] fragmentation handling, we want the least-used
   src-ip/dst-ip/proto triple.  Fairness doesn't come into it.  Thus
   if the range specifies 1.2.3.4 ports 10000-10005 and 1.2.3.5 ports
   1-65535, we don't do pro-rata allocation based on ports; we choose
   the ip with the lowest src-ip/dst-ip/proto usage.
*/
static void
find_best_ips_proto(struct nf_conntrack_tuple *tuple,
		    const struct nf_nat_range *range,
		    const struct nf_conn *ct,
		    enum nf_nat_manip_type maniptype)
{
	__be32 *var_ipp;
	/* Host order */
	u_int32_t minip, maxip, j;

	/* No IP mapping?  Do nothing. */
	if (!(range->flags & IP_NAT_RANGE_MAP_IPS))
		return;

	if (maniptype == IP_NAT_MANIP_SRC)
		var_ipp = &tuple->src.u3.ip;
	else
		var_ipp = &tuple->dst.u3.ip;

	/* Fast path: only one choice. */
	if (range->min_ip == range->max_ip) {
		*var_ipp = range->min_ip;
		return;
	}

	/* Hashing source and destination IPs gives a fairly even
	 * spread in practice (if there are a small number of IPs
	 * involved, there usually aren't that many connections
	 * anyway).  The consistency means that servers see the same
	 * client coming from the same IP (some Internet Banking sites
	 * like this), even across reboots. */
	minip = ntohl(range->min_ip);
	maxip = ntohl(range->max_ip);
	j = jhash_2words((__force u32)tuple->src.u3.ip,
			 range->flags & IP_NAT_RANGE_PERSISTENT ?
				0 : (__force u32)tuple->dst.u3.ip, 0);
	j = ((u64)j * (maxip - minip + 1)) >> 32;
	*var_ipp = htonl(minip + j);
}

/* Manipulate the tuple into the range given.  For NF_INET_POST_ROUTING,
 * we change the source to map into the range.  For NF_INET_PRE_ROUTING
 * and NF_INET_LOCAL_OUT, we change the destination to map into the
 * range.  It might not be possible to get a unique tuple, but we try.
 * At worst (or if we race), we will end up with a final duplicate in
 * __ip_conntrack_confirm and drop the packet. */
static void
get_unique_tuple(struct nf_conntrack_tuple *tuple,
		 const struct nf_conntrack_tuple *orig_tuple,
		 const struct nf_nat_range *range,
		 struct nf_conn *ct,
		 enum nf_nat_manip_type maniptype)
{
	struct net *net = nf_ct_net(ct);
	const struct nf_nat_protocol *proto;

	/* 1) If this srcip/proto/src-proto-part is currently mapped,
	   and that same mapping gives a unique tuple within the given
	   range, use that.

	   This is only required for source (ie. NAT/masq) mappings.
	   So far, we don't do local source mappings, so multiple
	   manips not an issue.  */
	if (maniptype == IP_NAT_MANIP_SRC &&
	    !(range->flags & IP_NAT_RANGE_PROTO_RANDOM)) {
		if (find_appropriate_src(net, orig_tuple, tuple, range)) {
			pr_debug("get_unique_tuple: Found current src map\n");
			if (!nf_nat_used_tuple(tuple, ct))
				return;
		}
	}

	/* 2) Select the least-used IP/proto combination in the given
	   range. */
	*tuple = *orig_tuple;
	find_best_ips_proto(tuple, range, ct, maniptype);

	/* 3) The per-protocol part of the manip is made to map into
	   the range to make a unique tuple. */

	rcu_read_lock();
	proto = __nf_nat_proto_find(orig_tuple->dst.protonum);

	/* Change protocol info to have some randomization */
	if (range->flags & IP_NAT_RANGE_PROTO_RANDOM) {
		proto->unique_tuple(tuple, range, maniptype, ct);
		goto out;
	}
#if defined (CONFIG_RTL_HARDWARE_NAT)	&& defined (CONFIG_RTL_INBOUND_COLLISION_AVOIDANCE)	
/* Only bother mapping if it's not already in range and unique */
	if ((!(range->flags & IP_NAT_RANGE_PROTO_SPECIFIED) ||
	     proto->in_range(tuple, maniptype, &range->min, &range->max)) &&
	    !nf_nat_used_tuple(tuple, ct))
	{
		if(	(gHwNatEnabled) && (maniptype==IP_NAT_MANIP_SRC) &&
			((orig_tuple->dst.protonum==IPPROTO_TCP) ||(orig_tuple->dst.protonum==IPPROTO_UDP)))
		{
			unsigned int proto;
			unsigned int sip, dip, gip;
			unsigned short sp, dp, gp; 
			unsigned int asicNaptHashScore=0;
			
			proto = (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum==IPPROTO_TCP)? 1: 0;
		
			sip	= ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
			dip 	= ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip;
			gip 	= tuple->src.u3.ip;
			sp  	= proto?ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port:ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port;
			dp  	= proto?ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port:ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port;
			gp  	= proto?tuple->src.u.tcp.port:tuple->src.u.udp.port;
			
			rtl865x_getAsicNaptHashScore(proto, sip, sp, gip, gp, dip, dp, &asicNaptHashScore);
			/*
			printk("%s:%d:maniptype is %d,%s (%u.%u.%u.%u:%u ->  %u.%u.%u.%u:%u ->%u.%u.%u.%u:%u),asicNaptHashScore is %d\n\n\n",
				__FUNCTION__,__LINE__,maniptype, proto?"tcp":"udp", 
				NIPQUAD(sip), sp, NIPQUAD(gip), gp, NIPQUAD(dip), dp,asicNaptHashScore);	
			*/
			if(asicNaptHashScore==100)
			{
				rtl865x_preReserveConn(proto, sip, sp, gip, gp, dip, dp);
				goto out;
			}
		
		}
		else
		{
			goto out;
		}
	}
#else	
	/* Only bother mapping if it's not already in range and unique */
	if ((!(range->flags & IP_NAT_RANGE_PROTO_SPECIFIED) ||
	     proto->in_range(tuple, maniptype, &range->min, &range->max)) &&
	    !nf_nat_used_tuple(tuple, ct))
		goto out;
#endif	
	/* Last change: get protocol to try to obtain unique tuple. */
	proto->unique_tuple(tuple, range, maniptype, ct);
out:
	rcu_read_unlock();
}

unsigned int
nf_nat_setup_info(struct nf_conn *ct,
		  const struct nf_nat_range *range,
		  enum nf_nat_manip_type maniptype)
{
	struct net *net = nf_ct_net(ct);
	struct nf_conntrack_tuple curr_tuple, new_tuple;
	struct nf_conn_nat *nat;
	int have_to_hash = !(ct->status & IPS_NAT_DONE_MASK);

	/* nat helper or nfctnetlink also setup binding */
	nat = nfct_nat(ct);
	if (!nat) {
		nat = nf_ct_ext_add(ct, NF_CT_EXT_NAT, GFP_ATOMIC);
		if (nat == NULL) {
			pr_debug("failed to add NAT extension\n");
			return NF_ACCEPT;
		}
	}

	NF_CT_ASSERT(maniptype == IP_NAT_MANIP_SRC ||
		     maniptype == IP_NAT_MANIP_DST);
	BUG_ON(nf_nat_initialized(ct, maniptype));

	/* What we've got will look like inverse of reply. Normally
	   this is what is in the conntrack, except for prior
	   manipulations (future optimization: if num_manips == 0,
	   orig_tp =
	   conntrack->tuplehash[IP_CT_DIR_ORIGINAL].tuple) */
	nf_ct_invert_tuplepr(&curr_tuple,
			     &ct->tuplehash[IP_CT_DIR_REPLY].tuple);
	get_unique_tuple(&new_tuple, &curr_tuple, range, ct, maniptype);

	if (!nf_ct_tuple_equal(&new_tuple, &curr_tuple)) {
		struct nf_conntrack_tuple reply;

		/* Alter conntrack table so will recognize replies. */
		nf_ct_invert_tuplepr(&reply, &new_tuple);
		nf_conntrack_alter_reply(ct, &reply);

		/* Non-atomic: we own this at the moment. */
		if (maniptype == IP_NAT_MANIP_SRC)
			ct->status |= IPS_SRC_NAT;
		else
			ct->status |= IPS_DST_NAT;
	}

	/* Place in source hash if this is the first time. */
	if (have_to_hash) {
		unsigned int srchash;

		srchash = hash_by_src(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);
		spin_lock_bh(&nf_nat_lock);
		/* nf_conntrack_alter_reply might re-allocate exntension aera */
		nat = nfct_nat(ct);
		nat->ct = ct;
		hlist_add_head_rcu(&nat->bysource,
				   &net->ipv4.nat_bysource[srchash]);
		spin_unlock_bh(&nf_nat_lock);
	}

	/* It's done. */
	if (maniptype == IP_NAT_MANIP_DST)
		set_bit(IPS_DST_NAT_DONE_BIT, &ct->status);
	else
		set_bit(IPS_SRC_NAT_DONE_BIT, &ct->status);

	return NF_ACCEPT;
}
EXPORT_SYMBOL(nf_nat_setup_info);

/* Returns true if succeeded. */
static bool
manip_pkt(u_int16_t proto,
	  struct sk_buff *skb,
	  unsigned int iphdroff,
	  const struct nf_conntrack_tuple *target,
	  enum nf_nat_manip_type maniptype)
{
	struct iphdr *iph;
	const struct nf_nat_protocol *p;

	if (!skb_make_writable(skb, iphdroff + sizeof(*iph)))
		return false;

	iph = (void *)skb->data + iphdroff;

	/* Manipulate protcol part. */

	/* rcu_read_lock()ed by nf_hook_slow */
	p = __nf_nat_proto_find(proto);
	if (!p->manip_pkt(skb, iphdroff, target, maniptype))
		return false;

	iph = (void *)skb->data + iphdroff;

	if (maniptype == IP_NAT_MANIP_SRC) {
		csum_replace4(&iph->check, iph->saddr, target->src.u3.ip);
		iph->saddr = target->src.u3.ip;
	} else {
		csum_replace4(&iph->check, iph->daddr, target->dst.u3.ip);
		iph->daddr = target->dst.u3.ip;
	}
	return true;
}

/* Do packet manipulations according to nf_nat_setup_info. */
unsigned int nf_nat_packet(struct nf_conn *ct,
			   enum ip_conntrack_info ctinfo,
			   unsigned int hooknum,
			   struct sk_buff *skb)
{
	enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);
	unsigned long statusbit;
	enum nf_nat_manip_type mtype = HOOK2MANIP(hooknum);

#if defined(CONFIG_RTL_HARDWARE_NAT) || defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	int assured=0;
	u_int16_t proto=ip_hdr(skb)->protocol;
	int is_tcp =( proto == IPPROTO_TCP);
#endif

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	int create_udp_link = 0;
	int create_tcp_link = 0;
#endif
	if (mtype == IP_NAT_MANIP_SRC)
		statusbit = IPS_SRC_NAT;
	else
		statusbit = IPS_DST_NAT;

	/* Invert if this is reply dir. */
	if (dir == IP_CT_DIR_REPLY)
		statusbit ^= IPS_NAT_MASK;

	/* Non-atomic: these bits don't change. */
	if (ct->status & statusbit) {
		struct nf_conntrack_tuple target;

		/* We are aiming to look like inverse of other direction. */
		nf_ct_invert_tuplepr(&target, &ct->tuplehash[!dir].tuple);

		if (!manip_pkt(target.dst.protonum, skb, 0, &target, mtype))
			return NF_DROP;
#if 1 //#if defined(IMPROVE_QOS)
#if defined(CONFIG_RTL_HARDWARE_NAT  ) || defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	assured = (((ct->status&IPS_DST_NAT_DONE) || (ct->status&IPS_SRC_NAT_DONE) )&&
		(!is_tcp||ct->proto.tcp.state==TCP_CONNTRACK_ESTABLISHED));
#endif

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	/*
	 * ### for iperf application test ###
	 * the behavior of iperf UDP test is LAN PC (client) will burst UDP from LAN to WAN (by one way),
	 * WAN PC (server) will only send one UDP packet (statistics) at the end of test.
	 * so the fastpath or hardware NAT will create link at the end of test.
	 *
	 * the purpose for adding the following code is to create fastpath or hardware NAT link 
	 * when we only get one packet from LAN to WAN in UDP case.
	 */
	{
		extern unsigned int _br0_ip;
		extern unsigned int _br0_mask;
		extern int smart_count;
		uint32 sip, dip;

		sip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
		dip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;

		if ((ip_hdr(skb)->protocol == IPPROTO_UDP) &&
				(((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
					 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip) ||
					(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
					 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip))
#if defined(UNNUMBER_IP)
					 && (is_unnumber_ip(dip)==FALSE)
#endif

					 ) && !assured) 
		{ 
			/* UDP and "LAN to WAN" */
			/* ignore some cases:  
			 *	1. sip = br0's ip -----> (ex. sip 192.168.1.254 ==> dip 239.255.255.250)
			 * 	2. (sip & br0's mask) != (br0's ip & br0's mask) -----> sip is not in br0's subnet
			 *	3. (dip & br0's mask) =  (br0's ip & br0's mask) -----> dip is in br0's subnet
			 *	4. dip != multicast IP address
			 *	5. sip != gip
			 */
			if ((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
					 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)&&
				(sip != _br0_ip) &&
				((sip & _br0_mask) == (_br0_ip & _br0_mask)) &&
				((dip & _br0_mask) != (_br0_ip & _br0_mask)) &&
				((dip & 0xf0000000) != 0xe0000000) &&
				(sip != (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip))
				)
			{
				create_udp_link = 1;
 			}
#if defined(UNNUMBER_IP)	
			else if (((is_unnumber_ip(sip)==TRUE) && (is_unnumber_ip(dip)==FALSE)))
			{
				create_udp_link = 1;
			}
#endif
			else if ((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
					 	== ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)&&
					 ((sip & _br0_mask) != (_br0_ip & _br0_mask))&&
					 ((dip & _br0_mask) == (_br0_ip & _br0_mask)))
			{
				struct iphdr *iph=ip_hdr(skb);
				/* WAN->LAN */
				if (iph->daddr ==dip )
				{
					create_udp_link = 1;
				}
			}
		}
		else if((ip_hdr(skb)->protocol == IPPROTO_TCP) &&
	 		(((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip==ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip) ||
	 		(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip==ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip))
#if defined(UNNUMBER_IP)
	 		&&(is_unnumber_ip(dip)==FALSE)
#endif
	 		) &&!assured)
 		{
 			
			struct iphdr *iph=ip_hdr(skb);
			struct tcphdr *tcph=(void *) iph + iph->ihl*4;
				if (!tcph->fin && !tcph->syn && !tcph->rst && tcph->psh==1 && 
					tcph->ack ==1 &&  
					(((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip==ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip) &&
					(iph->daddr !=_br0_ip) && ((sip & _br0_mask) == (_br0_ip & _br0_mask)) &&
					((dip & _br0_mask) != (_br0_ip & _br0_mask)) && (sip != (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)))||
					((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip==ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip) &&
					((sip & _br0_mask) != (_br0_ip & _br0_mask)) && 
					((dip & _br0_mask) == (_br0_ip & _br0_mask))&& (sip == iph->saddr))))
				{ 
						
						smart_count++;
						if(smart_count >810){
							//panic_printk("the case hit for mart flow:tcp state=%d, assured=%d\n",ct->proto.tcp.state,test_bit(IPS_ASSURED_BIT, &ct->status));
							create_tcp_link=1;
						}
				}	
#if defined(UNNUMBER_IP)
			if ((!create_tcp_link) && (is_unnumber_ip(sip)==TRUE) && (is_unnumber_ip(dip)==FALSE) )
			{
				create_tcp_link = 1;
			}
#endif
 		}
	}
	
	/*1.add "!(ct->helper)" to fix ftp-cmd type packet 
	  2.add identify case LAN->WAN(AB->BC) or WAN->LAN(BC->AB) 
	  3.add !(ct->nat.info.helper) for best ALG avoid
	  */
				
	if (!(nfct_help(ct)) &&  (assured || (create_udp_link) || (create_tcp_link)) &&
		 ((proto==IPPROTO_TCP) || (proto==IPPROTO_UDP))) {

		if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP) {
			/* TCP Connection Tracking */
				
			if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
			   == ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)
			{
				/*case BC->AB*/
				#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook6!=NULL)
				{
					FastPath_hook6(NP_TCP,
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),	
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port),				
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
#if defined(IMPROVE_QOS)
					skb, ct, 
#endif
					NP_NONE);					
				}
				#else
				rtk_addNaptConnection(NP_TCP,
				ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),	
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port),				
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
#if defined(IMPROVE_QOS)
					skb, ct, 
#endif
					NP_NONE);		
				#endif
				
			}else if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
				 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)
			{
					
				/*case AB->BC*/
				#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook6!=NULL)
				{
					FastPath_hook6(NP_TCP, 
						ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.tcp.port),
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),
#if defined(IMPROVE_QOS)
						skb, ct, 
#endif
						NP_NONE);
				}
				#else
				rtk_addNaptConnection(NP_TCP, 
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.tcp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),
#if defined(IMPROVE_QOS)
					skb, ct, 
#endif
					NP_NONE);
				#endif
			}
							
		} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {
			/* UDP Connection Tracking */
			/* check "LAN to WAN (AB->BC)" first */
			 if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
				== ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)				
			{
			/*case AB->BC*/
			#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook6!=NULL)
				{
					FastPath_hook6(NP_UDP, 
						ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port),
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),
#if defined(IMPROVE_QOS)
						skb, ct, 
#endif
						NP_NONE);
				}
				#else
				rtk_addNaptConnection(NP_UDP, 
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),
#if defined(IMPROVE_QOS)
					skb, ct, 
#endif
					NP_NONE);
				#endif
			}
			else if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
				== ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)
			{
			/*case BC->AB*/
			#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook6!=NULL)
				{
					FastPath_hook6(NP_UDP,
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),	
						ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port),				
						ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
#if defined(IMPROVE_QOS)
						skb, ct, 
#endif
						NP_NONE);
				}
				#else
				rtk_addNaptConnection(NP_UDP,
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),	
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port),				
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
#if defined(IMPROVE_QOS)
					skb, ct, 
#endif
					NP_NONE);			
				#endif				
			}
			

		}

	}
#endif
#endif //End of defined(IMPROVE_QOS)

#if 0
#ifdef CONFIG_RTL_HARDWARE_NAT  
	struct nf_conn_help *help;
	/*2007-12-19*/
#if defined(CONFIG_RTL865x_TCPFLOW_NONE_STATUS_CHECK)
			if (((proto==IPPROTO_TCP) || (proto==IPPROTO_UDP)) &&(!(nfct_help(ct))))
#else
#if 1 /*defined(IMPROVE_QOS)*/
			if (( assured  || (create_udp_link) || (create_tcp_link) ) &&
				((proto==IPPROTO_TCP) || (proto==IPPROTO_UDP)) && ((!(nfct_help(ct))) || ((help=nfct_help(ct)) && !help->helper)))
#else
			if (((ct->status&IPS_DST_NAT_DONE_BIT) && (ct->status&IPS_SRC_NAT_DONE_BIT) )&&
				((proto==IPPROTO_TCP) || (proto==IPPROTO_UDP)) &&
				(!is_tcp||ct->proto.tcp.state==TCP_CONNTRACK_ESTABLISHED) &&(!(nfct_help(ct))))
#endif
#endif
			{
				//printk("%s %d \n", __func__, __LINE__);
				//rtl865x_handle_nat(ct, 1);
			}
#endif
#endif

	}
	return NF_ACCEPT;
}
EXPORT_SYMBOL_GPL(nf_nat_packet);

#if defined(CONFIG_RTL_HARDWARE_NAT) || defined(CONFIG_RTL_IPTABLES_FAST_PATH)
unsigned int _br0_ip;
unsigned int _br0_mask;
static void get_br0_ip_mask(void)
{
	struct in_device *in_dev;	
	struct net_device *landev;
	struct in_ifaddr *ifap = NULL;


     	if ((landev = __dev_get_by_name(&init_net, ALIASNAME_BR0)) != NULL){
//      	if ((landev = __dev_get_by_name(&init_net, "br0")) != NULL){//
		in_dev=(struct net_device *)(landev->ip_ptr);
		if (in_dev != NULL) {
			for (ifap=in_dev->ifa_list; ifap != NULL; ifap=ifap->ifa_next) {

//				if (strcmp("br0", ifap->ifa_label) == 0){//
                if(alias_name_is_eq(CMD_CMP,ifap->ifa_label,ALIASNAME_BR0)){
					_br0_ip = ifap->ifa_address;
					_br0_mask = ifap->ifa_mask;
					return; 
				}
			}
			
		}
	}	
}
#endif

#if defined(CONFIG_PROC_FS) && defined(CONFIG_RTL_HARDWARE_NAT )
static struct proc_dir_entry *proc_hw_nat=NULL;
static char gHwNatSetting[16];
//extern unsigned int ldst, lmask, wdst, wmask;

static int hw_nat_read_proc(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%s\n", gHwNatSetting);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	return len;
}

static int hw_nat_write_proc(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	if (count < 2) 
		return -EFAULT;

	if (buffer && !copy_from_user(&gHwNatSetting, buffer, 8)) {
		if (gHwNatSetting[0] == '0') { /* hardware NAT disabled, operation mode = gateway */
			gHwNatEnabled = 0;
			rtl865x_nat_reinit();
			rtl865x_changeOpMode(GATEWAY_MODE,0);
			//rtl8651_setAsicOperationLayer(4);
		}
		else if (gHwNatSetting[0] == '1') { /* hardware NAT enabled, operation mode = gateway */
			
			rtl865x_changeOpMode(GATEWAY_MODE,0);
			//rtl8651_setAsicOperationLayer(4);
			gHwNatEnabled = 1;
		}
		else if (gHwNatSetting[0] == '2') { /* hardware NAT disabled, operation mode = bridge mode*/
			//rtl865x_delRoute(wdst, wmask);
			//rtl865x_delRoute(ldst, lmask);
			rtl865x_changeOpMode(BRIDGE_MODE,0);
			//rtl8651_setAsicOperationLayer(3);
			gHwNatEnabled = 2;
		}
		else if(gHwNatSetting[0] == '3'){ /* hardware NAT disabled, operation mode = wisp mode */
			rtl865x_changeOpMode(WISP_MODE,0);
			//rtl8651_setAsicOperationLayer(3);
			gHwNatEnabled = 3;
		}
		else if(gHwNatSetting[0] == '4'){ /* hardware NAT disabled, operation mode = gateway (pure routing , no napt , all netif is LAN)*/		
			rtl865x_changeOpMode(GATEWAY_MODE,1);
			gHwNatEnabled = 4;
		}
		else if ((gHwNatSetting[0] == '8')&&
			((gHwNatSetting[1] == '5') /* L2TP */
			||(gHwNatSetting[1] == '4') /* PPTP */))
		{
			gHwNatEnabled = 0;
		}
#if defined(CONFIG_RTL_HARDWARE_NAT) || defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		else if (gHwNatSetting[0] == '9') {
			get_br0_ip_mask();
		}
#endif
		return count;
	}
	return -EFAULT;
}
#endif

#ifdef CONFIG_RTL_819X_SWCORE
#if defined(CONFIG_PROC_FS) && !defined(CONFIG_RTL_HARDWARE_NAT)
static struct proc_dir_entry *proc_sw_nat=NULL;
static char gSwNatSetting[16];

static int sw_nat_read_proc(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%s\n", gSwNatSetting);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	return len;
}

static int sw_nat_write_proc(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	if (count < 2) 
		return -EFAULT;
      
	if (buffer && !copy_from_user(&gSwNatSetting, buffer, 8)) {
		if (gSwNatSetting[0] == '0'){  /* operation mode = GATEWAY */
			//SoftNAT_OP_Mode(2);
			rtl865x_changeOpMode(GATEWAY_MODE,0);
			//rtl8651_setAsicOperationLayer(2);
		}
		else if (gSwNatSetting[0] == '1'){  /* operation mode = BRIDGE*/
			//SoftNAT_OP_Mode(1);
			rtl865x_changeOpMode(BRIDGE_MODE,0);
			//rtl8651_setAsicOperationLayer(2);
		}
		else if(gSwNatSetting[0] == '2'){ /* operation mode = WISP */
			rtl865x_changeOpMode(WISP_MODE,0);
			//rtl8651_setAsicOperationLayer(2);
		}
#if defined(CONFIG_RTL_HARDWARE_NAT) || defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		else if(gSwNatSetting[0] == '9'){
			get_br0_ip_mask();
		}
#endif
		return count;
	}
	return -EFAULT;
}
#endif
#endif // CONFIG_RTL_819X_SWCORE

/* Dir is direction ICMP is coming from (opposite to packet it contains) */
int nf_nat_icmp_reply_translation(struct nf_conn *ct,
				  enum ip_conntrack_info ctinfo,
				  unsigned int hooknum,
				  struct sk_buff *skb)
{
	struct {
		struct icmphdr icmp;
		struct iphdr ip;
	} *inside;
	const struct nf_conntrack_l4proto *l4proto;
	struct nf_conntrack_tuple inner, target;
	int hdrlen = ip_hdrlen(skb);
	enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);
	unsigned long statusbit;
	enum nf_nat_manip_type manip = HOOK2MANIP(hooknum);

	if (!skb_make_writable(skb, hdrlen + sizeof(*inside)))
		return 0;

	inside = (void *)skb->data + ip_hdrlen(skb);

	/* We're actually going to mangle it beyond trivial checksum
	   adjustment, so make sure the current checksum is correct. */
	if (nf_ip_checksum(skb, hooknum, hdrlen, 0))
		return 0;

	/* Must be RELATED */
	NF_CT_ASSERT(skb->nfctinfo == IP_CT_RELATED ||
		     skb->nfctinfo == IP_CT_RELATED+IP_CT_IS_REPLY);

	/* Redirects on non-null nats must be dropped, else they'll
	   start talking to each other without our translation, and be
	   confused... --RR */
	if (inside->icmp.type == ICMP_REDIRECT) {
		/* If NAT isn't finished, assume it and drop. */
		if ((ct->status & IPS_NAT_DONE_MASK) != IPS_NAT_DONE_MASK)
			return 0;

		if (ct->status & IPS_NAT_MASK)
			return 0;
	}

	pr_debug("icmp_reply_translation: translating error %p manip %u "
		 "dir %s\n", skb, manip,
		 dir == IP_CT_DIR_ORIGINAL ? "ORIG" : "REPLY");

	/* rcu_read_lock()ed by nf_hook_slow */
	l4proto = __nf_ct_l4proto_find(PF_INET, inside->ip.protocol);

	if (!nf_ct_get_tuple(skb,
			     ip_hdrlen(skb) + sizeof(struct icmphdr),
			     (ip_hdrlen(skb) +
			      sizeof(struct icmphdr) + inside->ip.ihl * 4),
			     (u_int16_t)AF_INET,
			     inside->ip.protocol,
			     &inner, l3proto, l4proto))
		return 0;

	/* Change inner back to look like incoming packet.  We do the
	   opposite manip on this hook to normal, because it might not
	   pass all hooks (locally-generated ICMP).  Consider incoming
	   packet: PREROUTING (DST manip), routing produces ICMP, goes
	   through POSTROUTING (which must correct the DST manip). */
	if (!manip_pkt(inside->ip.protocol, skb,
		       ip_hdrlen(skb) + sizeof(inside->icmp),
		       &ct->tuplehash[!dir].tuple,
		       !manip))
		return 0;

	if (skb->ip_summed != CHECKSUM_PARTIAL) {
		/* Reloading "inside" here since manip_pkt inner. */
		inside = (void *)skb->data + ip_hdrlen(skb);
		inside->icmp.checksum = 0;
		inside->icmp.checksum =
			csum_fold(skb_checksum(skb, hdrlen,
					       skb->len - hdrlen, 0));
	}

	/* Change outer to look the reply to an incoming packet
	 * (proto 0 means don't invert per-proto part). */
	if (manip == IP_NAT_MANIP_SRC)
		statusbit = IPS_SRC_NAT;
	else
		statusbit = IPS_DST_NAT;

	/* Invert if this is reply dir. */
	if (dir == IP_CT_DIR_REPLY)
		statusbit ^= IPS_NAT_MASK;

	if (ct->status & statusbit) {
		nf_ct_invert_tuplepr(&target, &ct->tuplehash[!dir].tuple);
		if (!manip_pkt(0, skb, 0, &target, manip))
			return 0;
	}

	return 1;
}
EXPORT_SYMBOL_GPL(nf_nat_icmp_reply_translation);

/* Protocol registration. */
int nf_nat_protocol_register(const struct nf_nat_protocol *proto)
{
	int ret = 0;

	spin_lock_bh(&nf_nat_lock);
	if (nf_nat_protos[proto->protonum] != &nf_nat_unknown_protocol) {
		ret = -EBUSY;
		goto out;
	}
	rcu_assign_pointer(nf_nat_protos[proto->protonum], proto);
 out:
	spin_unlock_bh(&nf_nat_lock);
	return ret;
}
EXPORT_SYMBOL(nf_nat_protocol_register);

/* Noone stores the protocol anywhere; simply delete it. */
void nf_nat_protocol_unregister(const struct nf_nat_protocol *proto)
{
	spin_lock_bh(&nf_nat_lock);
	rcu_assign_pointer(nf_nat_protos[proto->protonum],
			   &nf_nat_unknown_protocol);
	spin_unlock_bh(&nf_nat_lock);
	synchronize_rcu();
}
EXPORT_SYMBOL(nf_nat_protocol_unregister);

/* Noone using conntrack by the time this called. */
static void nf_nat_cleanup_conntrack(struct nf_conn *ct)
{
	struct nf_conn_nat *nat = nf_ct_ext_find(ct, NF_CT_EXT_NAT);

	if (nat == NULL || nat->ct == NULL)
		return;

	NF_CT_ASSERT(nat->ct->status & IPS_NAT_DONE_MASK);

	spin_lock_bh(&nf_nat_lock);
	hlist_del_rcu(&nat->bysource);
	spin_unlock_bh(&nf_nat_lock);
}

static void nf_nat_move_storage(void *new, void *old)
{
	struct nf_conn_nat *new_nat = new;
	struct nf_conn_nat *old_nat = old;
	struct nf_conn *ct = old_nat->ct;

	if (!ct || !(ct->status & IPS_NAT_DONE_MASK))
		return;

	spin_lock_bh(&nf_nat_lock);
	new_nat->ct = ct;
	hlist_replace_rcu(&old_nat->bysource, &new_nat->bysource);
	spin_unlock_bh(&nf_nat_lock);
}

static struct nf_ct_ext_type nat_extend __read_mostly = {
	.len		= sizeof(struct nf_conn_nat),
	.align		= __alignof__(struct nf_conn_nat),
	.destroy	= nf_nat_cleanup_conntrack,
	.move		= nf_nat_move_storage,
	.id		= NF_CT_EXT_NAT,
	.flags		= NF_CT_EXT_F_PREALLOC,
};

#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)

#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_conntrack.h>

static const struct nla_policy protonat_nla_policy[CTA_PROTONAT_MAX+1] = {
	[CTA_PROTONAT_PORT_MIN]	= { .type = NLA_U16 },
	[CTA_PROTONAT_PORT_MAX]	= { .type = NLA_U16 },
};

static int nfnetlink_parse_nat_proto(struct nlattr *attr,
				     const struct nf_conn *ct,
				     struct nf_nat_range *range)
{
	struct nlattr *tb[CTA_PROTONAT_MAX+1];
	const struct nf_nat_protocol *npt;
	int err;

	err = nla_parse_nested(tb, CTA_PROTONAT_MAX, attr, protonat_nla_policy);
	if (err < 0)
		return err;

	npt = nf_nat_proto_find_get(nf_ct_protonum(ct));
	if (npt->nlattr_to_range)
		err = npt->nlattr_to_range(tb, range);
	nf_nat_proto_put(npt);
	return err;
}

static const struct nla_policy nat_nla_policy[CTA_NAT_MAX+1] = {
	[CTA_NAT_MINIP]		= { .type = NLA_U32 },
	[CTA_NAT_MAXIP]		= { .type = NLA_U32 },
};

static int
nfnetlink_parse_nat(struct nlattr *nat,
		    const struct nf_conn *ct, struct nf_nat_range *range)
{
	struct nlattr *tb[CTA_NAT_MAX+1];
	int err;

	memset(range, 0, sizeof(*range));

	err = nla_parse_nested(tb, CTA_NAT_MAX, nat, nat_nla_policy);
	if (err < 0)
		return err;

	if (tb[CTA_NAT_MINIP])
		range->min_ip = nla_get_be32(tb[CTA_NAT_MINIP]);

	if (!tb[CTA_NAT_MAXIP])
		range->max_ip = range->min_ip;
	else
		range->max_ip = nla_get_be32(tb[CTA_NAT_MAXIP]);

	if (range->min_ip)
		range->flags |= IP_NAT_RANGE_MAP_IPS;

	if (!tb[CTA_NAT_PROTO])
		return 0;

	err = nfnetlink_parse_nat_proto(tb[CTA_NAT_PROTO], ct, range);
	if (err < 0)
		return err;

	return 0;
}

static int
nfnetlink_parse_nat_setup(struct nf_conn *ct,
			  enum nf_nat_manip_type manip,
			  struct nlattr *attr)
{
	struct nf_nat_range range;

	if (nfnetlink_parse_nat(attr, ct, &range) < 0)
		return -EINVAL;
	if (nf_nat_initialized(ct, manip))
		return -EEXIST;

	return nf_nat_setup_info(ct, &range, manip);
}
#else
static int
nfnetlink_parse_nat_setup(struct nf_conn *ct,
			  enum nf_nat_manip_type manip,
			  struct nlattr *attr)
{
	return -EOPNOTSUPP;
}
#endif

static int __net_init nf_nat_net_init(struct net *net)
{
	net->ipv4.nat_bysource = nf_ct_alloc_hashtable(&nf_nat_htable_size,
						      &net->ipv4.nat_vmalloced, 0);
	if (!net->ipv4.nat_bysource)
		return -ENOMEM;
	return 0;
}

/* Clear NAT section of all conntracks, in case we're loaded again. */
static int clean_nat(struct nf_conn *i, void *data)
{
	struct nf_conn_nat *nat = nfct_nat(i);

	if (!nat)
		return 0;
	memset(nat, 0, sizeof(*nat));
	i->status &= ~(IPS_NAT_MASK | IPS_NAT_DONE_MASK | IPS_SEQ_ADJUST);
	return 0;
}

static void __net_exit nf_nat_net_exit(struct net *net)
{
	nf_ct_iterate_cleanup(net, &clean_nat, NULL);
	synchronize_rcu();
	nf_ct_free_hashtable(net->ipv4.nat_bysource, net->ipv4.nat_vmalloced,
			     nf_nat_htable_size);
}

static struct pernet_operations nf_nat_net_ops = {
	.init = nf_nat_net_init,
	.exit = nf_nat_net_exit,
};

static int __init nf_nat_init(void)
{
	size_t i;
	int ret;

	need_ipv4_conntrack();

	ret = nf_ct_extend_register(&nat_extend);
	if (ret < 0) {
		printk(KERN_ERR "nf_nat_core: Unable to register extension\n");
		return ret;
	}

	/* Leave them the same for the moment. */
	nf_nat_htable_size = nf_conntrack_htable_size;

	ret = register_pernet_subsys(&nf_nat_net_ops);
	if (ret < 0)
		goto cleanup_extend;

	/* Sew in builtin protocols. */
	spin_lock_bh(&nf_nat_lock);
	for (i = 0; i < MAX_IP_NAT_PROTO; i++)
		rcu_assign_pointer(nf_nat_protos[i], &nf_nat_unknown_protocol);
	rcu_assign_pointer(nf_nat_protos[IPPROTO_TCP], &nf_nat_protocol_tcp);
	rcu_assign_pointer(nf_nat_protos[IPPROTO_UDP], &nf_nat_protocol_udp);
	rcu_assign_pointer(nf_nat_protos[IPPROTO_ICMP], &nf_nat_protocol_icmp);
	spin_unlock_bh(&nf_nat_lock);

	/* Initialize fake conntrack so that NAT will skip it */
	nf_conntrack_untracked.status |= IPS_NAT_DONE_MASK;

	l3proto = nf_ct_l3proto_find_get((u_int16_t)AF_INET);

	BUG_ON(nf_nat_seq_adjust_hook != NULL);
	rcu_assign_pointer(nf_nat_seq_adjust_hook, nf_nat_seq_adjust);
	BUG_ON(nfnetlink_parse_nat_setup_hook != NULL);
	rcu_assign_pointer(nfnetlink_parse_nat_setup_hook,
			   nfnetlink_parse_nat_setup);

#if defined(CONFIG_PROC_FS) && defined(CONFIG_RTL_HARDWARE_NAT)
	proc_hw_nat = create_proc_entry("hw_nat", 0, NULL);
	if (proc_hw_nat) {
	    proc_hw_nat->read_proc = hw_nat_read_proc;
	    proc_hw_nat->write_proc = hw_nat_write_proc;
      }  
#endif

#if defined(CONFIG_PROC_FS) && defined(CONFIG_NET_SCHED)
	proc_qos = create_proc_entry("qos", 0, NULL);
	if (proc_qos) {
	    proc_qos->read_proc = qos_read_proc;
	    proc_qos->write_proc = qos_write_proc;
      }
#endif

#ifdef CONFIG_RTL_819X_SWCORE
#if defined(CONFIG_PROC_FS) && !defined(CONFIG_RTL_HARDWARE_NAT)
	proc_sw_nat = create_proc_entry("sw_nat", 0, NULL);
	if (proc_sw_nat) {
		proc_sw_nat->read_proc = sw_nat_read_proc;
		proc_sw_nat->write_proc = sw_nat_write_proc;
	}  
#endif
#endif

#ifdef CONFIG_RTL_HARDWARE_NAT
	gHwNatEnabled=1;
#endif

#if defined(CONFIG_PROC_FS) && defined(CONFIG_NET_SCHED)
	gQosSetting = kmalloc(1024, GFP_KERNEL);
#endif


	return 0;

 cleanup_extend:
	nf_ct_extend_unregister(&nat_extend);
	return ret;
}

static void __exit nf_nat_cleanup(void)
{

#if defined(CONFIG_PROC_FS) && defined(CONFIG_NET_SCHED)
	kfree(gQosSetting);
#endif

	unregister_pernet_subsys(&nf_nat_net_ops);
	nf_ct_l3proto_put(l3proto);
	nf_ct_extend_unregister(&nat_extend);
	rcu_assign_pointer(nf_nat_seq_adjust_hook, NULL);
	rcu_assign_pointer(nfnetlink_parse_nat_setup_hook, NULL);
	synchronize_net();
}

MODULE_LICENSE("GPL");
MODULE_ALIAS("nf-nat-ipv4");

module_init(nf_nat_init);
module_exit(nf_nat_cleanup);
