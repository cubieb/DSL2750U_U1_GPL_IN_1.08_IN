/*
 *	Handle incoming frames
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"
#include <net/rtl/rtl_alias.h>
//import the MACRO ATM_SKB
#ifdef CONFIG_NEW_PORTMAPPING
#include <linux/atmdev.h>

/*check the sw_num bit of fgroup is set or not; 
* return 0, not set; 
* retrun 1, is set;
*/
#define SWPORT_IN_FGROUP(sw_num, fgroup)  ((1 < sw_num) & fgroup)

static int br_handle_portmap_downstrm(struct net_bridge_fdb_entry *dst, struct sk_buff *skb);
static int br_handle_portmap_upstrm(struct net_bridge_fdb_entry *dst, struct sk_buff *skb, 
												struct net_bridge *br);

#endif

#if defined (CONFIG_RTL_IGMP_SNOOPING)
#include <linux/ip.h>
#include <linux/in.h>
#if defined (CONFIG_RTL_MLD_SNOOPING)
#include <linux/ipv6.h>
#include <linux/in6.h>
#endif
#include <linux/igmp.h>
#include <net/checksum.h>
#include <net/rtl/rtl865x_igmpsnooping_glue.h>
#include <net/rtl/rtl865x_igmpsnooping.h>
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl_nic.h>

/*
 * TBS_TAG: by luolei 2008--
 * Desc:Add IGMP Snooping support
 */
#if defined(CONFIG_IGMP_SNOOPING)
#include <linux/if_vlan.h>
#include <linux/timer.h>
#include <linux/igmp.h>
#include <linux/ip.h>

union ip_array {
    unsigned int ip_addr;
    unsigned char ip_ar[4];
};
#endif


#if defined (CONFIG_RTL_IGMP_SNOOPING) && defined (CONFIG_NETFILTER)
#include <linux/netfilter_ipv4/ip_tables.h>
#endif

extern int igmpsnoopenabled;
#if defined (CONFIG_RTL_MLD_SNOOPING)
extern int mldSnoopEnabled;
#endif
extern unsigned int brIgmpModuleIndex;
extern unsigned int br0SwFwdPortMask;

#if defined (MCAST_TO_UNICAST)
extern int IGMPProxyOpened;

#if defined (IPV6_MCAST_TO_UNICAST)
#include <linux/ipv6.h>
#include <linux/in6.h>
#include <linux/icmpv6.h>
//#define	DBG_ICMPv6	//enable it to debug icmpv6 check
static int ICMPv6_check(struct sk_buff *skb , unsigned char *gmac);
#endif	//end of IPV6_MCAST_TO_UNICAST

#endif	//end of MCAST_TO_UNICAST

#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
#include <net/udp.h>
#endif

static char igmp_type_check(struct sk_buff *skb, unsigned char *gmac,unsigned int *gIndex,unsigned int *moreFlag);
static void br_update_igmp_snoop_fdb(unsigned char op, struct net_bridge *br, struct net_bridge_port *p, unsigned char *gmac
									,struct sk_buff *skb);
#endif	//end of CONFIG_RTL_IGMP_SNOOPING

#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
extern int br_filter_enter(struct sk_buff *skb);
extern unsigned char dut_br0_mac[];
extern unsigned char Filter_State;
extern int enable_filter;
#endif

/* Bridge group multicast address 802.1d (pg 51). */
const u8 br_group_address[ETH_ALEN] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };

#if defined (CONFIG_RTK_MESH)

void br_signal_pathsel(struct net_bridge *br)
{
	struct task_struct *task;
	if(br==NULL)
	{
		return;
	}
	read_lock(&tasklist_lock);
	task = find_task_by_vpid(br->mesh_pathsel_pid);
	read_unlock(&tasklist_lock);
	if(task)
	{
    	    	//printk("Send signal from kernel\n");
		send_sig(SIGUSR2,task,0);
	}
	else {
	//printk("Path selection daemon pid: %d does not exist\n", br->mesh_pathsel_pid);
	}
}

/*
void br_signal_pathsel()
{
	struct task_struct *task;

	struct net_bridge *br;

	br = find_br_by_name("br0");
	
	read_lock(&tasklist_lock);
    task = find_task_by_pid(br->mesh_pathsel_pid);
    read_unlock(&tasklist_lock);
    if(task)
    {
    	//printk("Send signal from kernel\n");
        send_sig(SIGUSR2,task,0);
		//br->stp_enabled = 1; //now pathsel daemon can turn it on
	}
    else {
        //printk("Path selection daemon pid: %d does not exist\n", br->mesh_pathsel_pid);
    }
}
*/
#endif	//CONFIG_RTK_MESH

/*************************************************************************
功能: 对于进入本机的vlan 数据，移除VLAN 头，进入协议栈处理
参数: skb 
返回: netif_receive_skb(skb);
备注:
*************************************************************************/
static int br_pass_frame_up_finish(struct sk_buff *skb)
{
#ifdef CONFIG_NETFILTER_DEBUG
	skb->nf_debug = 0;
#endif
#if 1
	/* If pass up to IP, remove VLAN header */
	if (skb->protocol == __constant_htons(ETH_P_8021Q)) {
		unsigned short proto;
		struct vlan_hdr *vhdr = (struct vlan_hdr *)(skb->data);

        /* make sure protocol is correct before passing up */
        proto = vhdr->h_vlan_encapsulated_proto;

		skb = skb_share_check(skb, GFP_ATOMIC);
		if (skb) {
			memmove(skb->data - ETH_HLEN + VLAN_HLEN,
				skb->data - ETH_HLEN, 12);
			skb_pull(skb, VLAN_HLEN);
			skb->mac_header+= VLAN_HLEN;
		}
		/* 重置skb->protocol为vlan标签后面接的协议类型，之前的protocol为0x8100(即ETH_P_8021Q)*/
		skb->protocol = proto;
		/* TODO: do we need to assign skb->priority? */
	}
#endif
	return netif_receive_skb(skb);
}

static void br_pass_frame_up(struct net_bridge *br, struct sk_buff *skb)
{
	struct net_device *indev, *brdev = br->dev;

#if 0//defined (CONFIG_RTL_IGMP_SNOOPING)
        unsigned char *dest;
        struct net_bridge_port *p;
        unsigned char macAddr[6];
        unsigned char operation;
        struct iphdr *iph;
	unsigned char proto=0;  
                          
	//iph = skb->nh.iph;
	iph=(struct iphdr *)skb_network_header(skb);
	proto =  iph->protocol;    
	dest =  eth_hdr(skb)->h_dest;
	p = skb->dev->br_port;

	if (igmpsnoopenabled && MULTICAST_MAC(dest) && (eth_hdr(skb)->h_proto == ETH_P_IP)){
                if (proto== IPPROTO_IGMP){
			uint32 fwdPortMask;
			//rtl_igmpMldProcess(brIgmpModuleIndex, skb->mac.raw, p->port_no, &fwdPortMask);
			rtl_igmpMldProcess(brIgmpModuleIndex, skb_mac_header(skb), p->port_no, &fwdPortMask);
			if ((operation=igmp_type_check(skb, macAddr)) > 0) {	
               	            br_update_igmp_snoop_fdb(operation, br, p, macAddr,skb);
                          }
		}
        }

	#if defined (MCAST_TO_UNICAST)	
	#if defined (IPV6_MCAST_TO_UNICAST)
	else if(igmpsnoopenabled 
		&& IPV6_MULTICAST_MAC(dest) 
		&& (eth_hdr(skb)->h_proto == ETH_P_IPV6) )
	{		
		operation = ICMPv6_check(skb , macAddr);
		if (operation > 0) {
				#ifdef	DBG_ICMPv6
				if( operation == 1)
					printk("ICMPv6 mac add (from frame_up)\n");
				else if(operation == 2)
					printk("ICMPv6 mac del (from frame_up)\n");	
				#endif
               	br_update_igmp_snoop_fdb(operation, br, p, macAddr,skb);
		}
	}
	#endif	//end of IPV6_MCAST_TO_UNICAST
	#endif
#endif
	brdev->stats.rx_packets++;
	brdev->stats.rx_bytes += skb->len;

	/*linux-2.6.19*/
	/*patch from linux 2.4*/
	// Kaohj -- added for ip_tables.c checking (checking interfaces under br0)
	if (skb->switch_port == 0)
		skb->switch_port = skb->dev->name;


	indev = skb->dev;
	skb->dev = brdev;

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
		br_pass_frame_up_finish);
}

/*
 * TBS_TAG: by luolei 2008--
 * Desc:Add IGMP Snooping support
 */

#if defined(CONFIG_IGMP_SNOOPING)
static void addr_conv(unsigned char *in, unsigned char * out)
{
    sprintf(out, "%02x%02x%02x%02x%02x%02x", in[0], in[1], in[2], in[3], in[4], in[5]);
}

mac_addr upnp_addr = {{0x01, 0x00, 0x5e, 0x7f, 0xff, 0xfa}};
mac_addr sys1_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x01}};
mac_addr sys2_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x02}};
mac_addr ospf1_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x05}};
mac_addr ospf2_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x06}};
mac_addr ripv2_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x09}};
mac_addr sys_addr = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};

/* Define ipv6 multicast mac address to let them pass through control filtering.
 * All ipv6 multicast mac addresses start with 0x33 0x33. So control_filter
 * only need to compare the first 2 bytes of the address.
 */
mac_addr ipv6_mc_addr = {{0x33, 0x33, 0x00, 0x00, 0x00, 0x00}}; /* only the left two bytes are significant */

static int control_filter(unsigned char *dest)
{
    if ((!memcmp(dest, &upnp_addr, ETH_ALEN)) ||
            (!memcmp(dest, &sys1_addr, ETH_ALEN)) ||
            (!memcmp(dest, &sys2_addr, ETH_ALEN)) ||
            (!memcmp(dest, &ospf1_addr, ETH_ALEN)) ||
            (!memcmp(dest, &ospf2_addr, ETH_ALEN)) ||
            (!memcmp(dest, &sys_addr, ETH_ALEN)) ||
            (!memcmp(dest, &ripv2_addr, ETH_ALEN)) ||
            (!memcmp(dest, &ipv6_mc_addr, 2)))
        return 0;
    else
        return 1;
}

static inline void brcm_conv_ip_to_mac(char *ipa, char *maca)
{
    maca[0] = 0x01;
    maca[1] = 0x00;
    maca[2] = 0x5e;
    maca[3] = 0x7F & ipa[1];
    maca[4] = ipa[2];
    maca[5] = ipa[3];

    return;
}

static void br_process_igmpv3(struct net_bridge *br, struct sk_buff *skb, unsigned char *dest, struct igmpv3_report *report)
{
    struct igmpv3_grec *grec;
    int i;
    struct in_addr src;
    //union ip_array igmpv3_mcast;
    unsigned int ip_addr;    
    int num_src;
    unsigned char tmp[6];
    struct net_bridge_mc_fdb_entry *mc_fdb;

    if(report) {
        grec = &report->grec[0];
        for(i = 0; i < ntohs(report->ngrec); i++) {
            ip_addr = grec->grec_mca;
            brcm_conv_ip_to_mac((char *)&ip_addr, tmp);
            switch(grec->grec_type) {
                case IGMPV3_MODE_IS_INCLUDE:
                case IGMPV3_CHANGE_TO_INCLUDE:
                case IGMPV3_ALLOW_NEW_SOURCES:
                    for(num_src = 0; num_src < ntohs(grec->grec_nsrcs); num_src++) {
                        src.s_addr = grec->grec_src[num_src];
                        mc_fdb = br_mc_fdb_find(br, skb->dev->br_port, tmp, eth_hdr(skb)->h_source, &src);
                        if((NULL != mc_fdb) &&
                            (mc_fdb->src_entry.filt_mode == MCAST_EXCLUDE))
                        {
                            br_mc_fdb_remove(br, skb->dev->br_port, tmp, eth_hdr(skb)->h_source, SNOOP_EX_CLEAR, &src);
                        }
                        else {
                            br_mc_fdb_add(br, skb->dev->br_port, tmp, eth_hdr(skb)->h_source, SNOOP_IN_ADD, &src);
                        }
                    }

                    if(0 == ntohs(grec->grec_nsrcs)) {
                        src.s_addr = 0;
                        br_mc_fdb_remove(br, skb->dev->br_port, tmp, eth_hdr(skb)->h_source, SNOOP_EX_CLEAR, &src);
                    }
                    break;

                case IGMPV3_MODE_IS_EXCLUDE:
                case IGMPV3_CHANGE_TO_EXCLUDE:
                case IGMPV3_BLOCK_OLD_SOURCES:
                    for(num_src = 0; num_src < ntohs(grec->grec_nsrcs); num_src++) {
                        src.s_addr = grec->grec_src[num_src];
                        mc_fdb = br_mc_fdb_find(br, skb->dev->br_port, tmp, eth_hdr(skb)->h_source, &src);
                        if((NULL != mc_fdb) &&
                            (mc_fdb->src_entry.filt_mode == MCAST_INCLUDE))
                        {
                            br_mc_fdb_remove(br, skb->dev->br_port, tmp, eth_hdr(skb)->h_source, SNOOP_IN_CLEAR, &src);
                        }
                        else {
                            br_mc_fdb_add(br, skb->dev->br_port, tmp, eth_hdr(skb)->h_source, SNOOP_EX_ADD, &src);
                        }
                    }

                    if(0 == ntohs(grec->grec_nsrcs)) {
                        src.s_addr = 0;
                        br_mc_fdb_add(br, skb->dev->br_port, tmp, eth_hdr(skb)->h_source, SNOOP_EX_ADD, &src);
                    }
                    break;
            }
            grec = (struct igmpv3_grec *)((char *)grec + IGMPV3_GRP_REC_SIZE(grec));
        }
    }
    return;
}


int mc_forward(struct net_bridge *br, struct sk_buff *skb, unsigned char *dest,int forward, int clone)
{
    struct net_bridge_mc_fdb_entry *dst;
    struct list_head *lh;
    int status = 0;
    struct sk_buff *skb2;
    struct net_bridge_port *p;
    unsigned char tmp[6];
    struct igmpv3_report *report;
    struct iphdr *pip = ip_hdr(skb);
    struct in_addr src;
    unsigned char igmp_type = 0;

    if (!snooping)
        return 0;

    /*
       if ((snooping == SNOOPING_BLOCKING_MODE) && control_filter(dest))
       status = 1;
    */

    if (control_filter(dest))
        status = 1;

    //printk("mc forward  proto = %d  proxy = %d\n", skb->data[9], br->proxy);
    if (skb->data[9] == IPPROTO_IGMP) {
        // For proxy; need to add some intelligence here
        if (!br->proxy) {
            if(pip->ihl == 5) {
                igmp_type = skb->data[20];
            } else {
                igmp_type = skb->data[24];
            }
            
            if (((igmp_type == IGMPV2_HOST_MEMBERSHIP_REPORT) ||
                (igmp_type == IGMP_HOST_MEMBERSHIP_REPORT)) &&
                (skb->protocol == __constant_htons(ETH_P_IP))) 
            {
                src.s_addr = 0;
                br_mc_fdb_add(br, skb->dev->br_port, dest, eth_hdr(skb)->h_source, SNOOP_EX_ADD, &src);
            }
            else if((igmp_type == IGMPV3_HOST_MEMBERSHIP_REPORT) &&
                    (skb->protocol == __constant_htons(ETH_P_IP))) 
            {
                if(pip->ihl == 5) {
                    report = (struct igmpv3_report *)&skb->data[20];
                }
                else {
                    report = (struct igmpv3_report *)&skb->data[24];
                }
                
                if(report) {
                    br_process_igmpv3(br, skb, dest, report);
                }
            }
            else if (igmp_type == IGMP_HOST_LEAVE_MESSAGE) 
            {
                if(pip->ihl == 5) {
                    brcm_conv_ip_to_mac(&skb->data[24], tmp);
                } else {
                    brcm_conv_ip_to_mac(&skb->data[28], tmp);
                }

                src.s_addr = 0;
                br_mc_fdb_remove(br, skb->dev->br_port, tmp, eth_hdr(skb)->h_source, SNOOP_EX_CLEAR, &src);
            }
            else
                ;
        }
        return 0;
    }

    /*
    if (clone) {
        struct sk_buff *skb3;

        if ((skb3 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
            br->statistics.tx_dropped++;
            return;
        }

        skb = skb3;
    }
    */

    list_for_each_rcu(lh, &br->mc_list) {
        dst = (struct net_bridge_mc_fdb_entry *) list_entry(lh, struct net_bridge_mc_fdb_entry, list);
        if (!memcmp(&dst->addr, dest, ETH_ALEN)) {
            if((dst->src_entry.filt_mode == MCAST_INCLUDE) &&
               (pip->saddr == dst->src_entry.src.s_addr)) 
            {
                if (!dst->dst->dirty) {
                    skb2 = skb_clone(skb, GFP_ATOMIC);
                    if(skb2)
                    {
                    if (forward)
                    {
                        //printk("include mode br_forward to %s\n", dst->dst->dev->name);
                        br_forward(dst->dst, skb2);
                    }
                    else
                        br_deliver(dst->dst, skb2);
                    }    
                }
                dst->dst->dirty = 1;
                status = 1;
            }
            else if(dst->src_entry.filt_mode == MCAST_EXCLUDE) {
                if(0 == dst->src_entry.src.s_addr) 
                {

                    if (!dst->dst->dirty) {
                        skb2 = skb_clone(skb, GFP_ATOMIC);
                        if(skb2)
                        {
                        if (forward)
                        {
                            //printk("exclude mode br_forward to %s\n", dst->dst->dev->name);
                            br_forward(dst->dst, skb2);
                        }
                        else
                            br_deliver(dst->dst, skb2);
                        }    
                    }
                    dst->dst->dirty = 1;
                    status = 1;
                }
                else if(pip->saddr == dst->src_entry.src.s_addr) {
                    status = 1;
			dst->dst->dirty = 1;
                }
            }
        }
    }

//this is dirty
 list_for_each_rcu(lh, &br->mc_list) {
        dst = (struct net_bridge_mc_fdb_entry *) list_entry(lh, struct net_bridge_mc_fdb_entry, list);
        if (!memcmp(&dst->addr, dest, ETH_ALEN)) {
			if(dst->src_entry.filt_mode == MCAST_EXCLUDE) {
				if (0 != dst->src_entry.src.s_addr && pip->saddr != dst->src_entry.src.s_addr) {
					  if (!dst->dst->dirty) {
                       			 skb2 = skb_clone(skb, GFP_ATOMIC);
                       			 if(skb2)
                       			 {
                       			 if (forward)
                        			{
                        				if (br_igmp_debug)
                            			printk("exclude mode br_forward to %s\n", dst->dst->dev->name);
                        				 br_forward(dst->dst, skb2);
                        			}
                        			else
                           			 br_deliver(dst->dst, skb2);
                    				}
                    			}	
                    			dst->dst->dirty = 1;
                    			status = 1;
				}
			}
        	}
}

    if (status) {
        list_for_each_entry_rcu(p, &br->port_list, list) {
            p->dirty = 0;
        }
    }
    /*
    else   //add by luoxiaojian 07/30/2007
    {
        char bc_addr[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; //if have no multi entry  then do not flood the multicast frame
        if (memcmp(dest, bc_addr, ETH_ALEN))
        status = 1;
    }
    */
    if ((!forward) && (status))
        kfree_skb(skb);

    return status;
}

#endif


#if defined (CONFIG_RTL865X_LANPORT_RESTRICTION)
extern int32 rtl865x_BlkCheck(const unsigned char *addr);
#endif

#if defined(CONFIG_RTL_MLD_SNOOPING)
extern int re865x_getIpv6TransportProtocol(struct ipv6hdr* ipv6h);
#endif
#if defined (CONFIG_RTL_HARDWARE_MULTICAST) 
extern int rtl865x_ipMulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
												unsigned int srcPort,unsigned int srcVlanId, 
												unsigned int srcIpAddr, unsigned int destIpAddr);
#endif

#ifdef CONFIG_RTL_EAP_RELAY
extern unsigned char inband_Hostmac[];
//unsigned char Wlan_mac[6]={0x00,0xE0,0x4C,0x01,0x96,0xC0};
#endif

#if defined(CONFIG_RTL_WLAN_BLOCK_RELAY)
#define RTL_WLAN_INT_PREFIX ALIASNAME_WLAN
//#define RTL_WLAN_INT_PREFIX "wlan"
extern int rtl_wlan_block_relay_enable;
#endif

/*linux-2.6.19*/
extern int enable_port_mapping;

#ifdef CONFIG_RTL8192CD
#include <linux/igmp.h>
static void ___ConvertMulticatIPtoMacAddr(__u32 group, unsigned char *gmac)
{
	__u32 u32tmp, tmp;
	int i;

	u32tmp = group & 0x007FFFFF;
	gmac[0]=0x01; gmac[1]=0x00; gmac[2]=0x5e;
	for (i=5; i>=3; i--) {
		tmp=u32tmp&0xFF;
		gmac[i]=tmp;
		u32tmp >>= 8;
	}
}

static char ___igmp_type_check(struct sk_buff *skb, unsigned char *gmac)
{
        struct iphdr *iph;
	__u8 hdrlen;
	struct igmphdr *igmph;

#if 1
	unsigned int IGMP_Group;// add  for fit igmp v3
#endif
	
	/* check IP header information */
	iph = ip_hdr(skb);
	hdrlen = iph->ihl << 2;
	if ((iph->version != 4) &&  (hdrlen < 20))
		return -1;
	if (ip_fast_csum((u8 *)iph, iph->ihl) != 0)
		return -1;
	{ /* check the length */
	__u32 len = ntohs(iph->tot_len);
	if (skb->len < len || len < hdrlen)
		return -1; 
	}
	/* parsing the igmp packet */
	igmph = (struct igmphdr *)((u8*)iph+hdrlen);

#if 1
	/*IGMP-V3 type Report*/
	if(igmph->type == IGMPV3_HOST_MEMBERSHIP_REPORT)
	{
		//printk("rec v3 report 1\n");
		/*in 11n seem need no care igmpProxy is opened or not,plus 2008-0612*/
		#if	0
		if(IGMPProxyOpened==0){
			IGMP_Group = *(unsigned int*)((unsigned int*)igmph + 3);
			
			//printk("v3_group:%02X:%02X:%02X:%02X\n",
			//IGMP_Group>>24,(IGMP_Group<<8)>>24,(IGMP_Group<<16)>>24,(IGMP_Group<<24)>>24);
		}else{
			return -1;//don't care v3 report
		}
		#else
			IGMP_Group = *(unsigned int*)((unsigned int*)igmph + 3);
		#endif
		
	}else{	//4 V2 or V1
		//printk("igmph->group:%04X\n",igmph->group);	
		IGMP_Group = igmph->group;
	}
#endif	
#if 1

	/*check if it's protocol reserved group */
	if(!IN_MULTICAST(IGMP_Group))
	{			
			return -1;
	}
	//Brad disable 20080619
	/*
	if((IGMP_Group&0xFFFFFF00)==0xE0000000){			
			return -1;
	}
	*/
	___ConvertMulticatIPtoMacAddr(IGMP_Group, gmac);
	
#else
	if(!IN_MULTICAST(igmph->group))
			return -1;
	/* check if it's protocol reserved group */
	if((igmph->group&0xFFFFFF00)==0xE0000000)
			return -1;
				
	ConvertMulticatIPtoMacAddr(igmph->group, gmac);
#endif

	if ((igmph->type==IGMP_HOST_MEMBERSHIP_REPORT) ||
	    (igmph->type==IGMPV2_HOST_MEMBERSHIP_REPORT)) 
	{
		return 1; /* report and add it */
	}

	else if (igmph->type==IGMPV3_HOST_MEMBERSHIP_REPORT)	{ 

	/*for support igmp v3 ; plusWang add 2009-0311*/
		
		struct igmpv3_grec	*v3grec = (struct igmpv3_grec *)((unsigned char*)igmph + 8);


		if(v3grec->grec_type == IGMPV3_CHANGE_TO_INCLUDE){
			
			//printk("igmp-v3 C2I\n");
			return 2; /* leave and delete it */
			
		}else	if(v3grec->grec_type == IGMPV3_CHANGE_TO_EXCLUDE){
		
			//printk("igmp-v3 C2E\n");			
			return 1;
		}
		/*
		else{
			printk("v3grec->grec_type =%d\n",v3grec->grec_type);
			printk("no yet support igmp-v3 type\n");
		}
		*/
	
	
	}
	else if (igmph->type==IGMP_HOST_LEAVE_MESSAGE){
		return 2; /* leave and delete it */
	}	
	
	return -1;
}




static void rtl_igmp_notify(struct sk_buff *skb, void *igmp_header)
{
	int op;
	char type;
	//struct igmphdr *ih = (struct igmphdr *)igmp_header;
	unsigned char StaMacAndGroup[20];
    if(!alias_name_is_eq(CMD_NCMP,skb->dev->name,ALIASNAME_WLAN))
    //if (strncmp(skb->dev->name,"wlan",4))//	
		return;

	
	type = ___igmp_type_check(skb,StaMacAndGroup);
	//printk("%s(%d): got msg %x\n", __FUNCTION__,__LINE__,type);
	
	switch (type) {
	case 1:
		op = 0x8B80;
		break;
	case 2:
		op = 0x8B81;
		break;
	default:
		return;
		
	}
	memcpy(StaMacAndGroup+6, skb_mac_header(skb)+6, 6);
	//printk("skb->dev->do_ioctl %p, skb->dev %p, skb->dev->name %s\n", skb->dev->do_ioctl, skb->dev, skb->dev->name);
	if(skb->dev->do_ioctl != NULL){
		skb->dev->do_ioctl(skb->dev, (struct ifreq*)StaMacAndGroup, op);
	}
	else{
struct net_device *dev = __dev_get_by_name(&init_net, ALIASNAME_WLAN0);

//struct net_device *dev = __dev_get_by_name(&init_net, "wlan0");//
		dev->do_ioctl(skb->dev, (struct ifreq*)StaMacAndGroup, op);
	}
	//printk("%s %d\n", __func__, __LINE__);
}

#endif
/* note: already called with rcu_read_lock (preempt_disabled) */
int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);
	struct net_bridge *br;
	struct net_bridge_fdb_entry *dst;
	struct sk_buff *skb2;

	if (!p || p->state == BR_STATE_DISABLED)
		goto drop;

	/* insert into forwarding database after filtering to avoid spoofing */
	br = p->br;
#if defined(CONFIG_VLAN_MODE)
    br_fdb_update(br, p, eth_hdr(skb)->h_source,skb->vlan);
#else
	br_fdb_update(br, p, eth_hdr(skb)->h_source);
#endif
#ifdef CONFIG_NEW_PORTMAPPING
	if(skb->switch_port)
	{
		br_fdb_add_swname(br, p, eth_hdr(skb)->h_source, skb->switch_port);
	}
#endif
#if defined (CONFIG_RTL865X_LANPORT_RESTRICTION)
	if (rtl865x_BlkCheck(eth_hdr(skb)->h_source) == TRUE)
	{

		kfree_skb(skb);
		goto out;
	}
#endif
	
#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)	
	if(enable_filter){
		struct iphdr *iph_check;
		iph_check = (struct iphdr *)skb_network_header(skb);	
		if(Filter_State==0){
			if(memcmp(dut_br0_mac, dest, 6)){
				if(iph_check->protocol==IPPROTO_ICMP){
					goto drop;
				}
			}
		}
#if 0		
		else if(Filter_State==1 && (dest[0] & 1 == 0)){
			struct udphdr *udph;
			udph=(void *)iph_check + iph_check->ihl*4;
			if(iph_check->protocol==IPPROTO_UDP && udph->dest ==68){ //if dhcp server packet if unicast
				if(br_filter_enter(skb))
					goto err;
			}
		}
#endif		
	}
#endif

#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
	if(enable_filter){			
		if(br_filter_enter(skb))
			goto drop;
	}	
#endif	


	if (p->state == BR_STATE_LEARNING)
		goto drop;

	/* The packet skb2 goes to the local host (NULL to skip). */
	skb2 = NULL;

	if (br->dev->flags & IFF_PROMISC)
		skb2 = skb;

	dst = NULL;

	if (is_multicast_ether_addr(dest)) {
#ifdef CONFIG_RTL8192CD
		if(skb->protocol == htons(ETH_P_IPV6))
		{
			char tmpOp = 0 ;
			int op;
			unsigned char macAddr[6];
			unsigned char StaMacAndGroup[20];
			tmpOp=ICMPv6_check(skb , macAddr);
			if(tmpOp > 0){
#ifdef DBG_ICMPv6
				if( tmpOp == 1)
					printk("icmpv6 add  macAddr\n");
				else if(tmpOp == 2)
					printk("icmpv6 del  macAddr\n");	
#endif
				switch (tmpOp) {
					case 1:
						op = 0x8B80;
						break;
					case 2:
						op = 0x8B81;
						break;
					default:
						return;
					
				}
				memcpy(StaMacAndGroup, macAddr, 6);
				memcpy(StaMacAndGroup+6, skb_mac_header(skb)+6, 6);
				if(skb->dev->do_ioctl != NULL){
					skb->dev->do_ioctl(skb->dev, (struct ifreq*)StaMacAndGroup, op);
				}
				else{
					struct net_device *dev = __dev_get_by_name(&init_net, ALIASNAME_WLAN0);
					dev->do_ioctl(skb->dev, (struct ifreq*)StaMacAndGroup, op);
				}
			}

		}else{
			struct iphdr *iph;		
			if (unlikely((*(unsigned short *)(skb_mac_header(skb) + ETH_ALEN * 2) == __constant_htons(ETH_P_IP)))) {
				iph = (struct iphdr *)(skb_mac_header(skb) + ETH_HLEN);
				skb->network_header = (sk_buff_data_t)(iph);
				if (unlikely(iph->protocol == IPPROTO_IGMP)) {
					skb->transport_header = skb->network_header + (iph->ihl * 4);
					rtl_igmp_notify(skb, skb->transport_header);
				}
			}
		}
#endif
		br->dev->stats.multicast++;
		skb2 = skb;
	} else if ((dst = __br_fdb_get(br, dest)) && dst->is_local) {
#ifdef CONFIG_RTL_EAP_RELAY
		if(eth_hdr(skb)->h_proto == 0x888E)
		{			
			memcpy(eth_hdr(skb)->h_dest,inband_Hostmac,6);
			//else if(!memcmp(eth_hdr(skb)->h_source,inband_Hostmac,6)) // if src_mac == host .
			//	memcpy(eth_hdr(skb)->h_source,Wlan_mac,6);
			dst = __br_fdb_get(br, dest);					
		}
		else
#endif	
		{		
			skb2 = skb;
			/* Do not forward the packet since it's local. */
			skb = NULL;
		}
	}

	if (skb2 == skb)
		skb2 = skb_clone(skb, GFP_ATOMIC);

	if (skb2)
		br_pass_frame_up(br, skb2);


	if (skb) {
#if defined(CONFIG_RTL_WLAN_BLOCK_RELAY)
		if(rtl_wlan_block_relay_enable && dst){
			if(!memcmp(skb->dev->name,RTL_WLAN_INT_PREFIX,sizeof(RTL_WLAN_INT_PREFIX)-1)){
				if(!memcmp(dst->dst->dev->name,RTL_WLAN_INT_PREFIX,sizeof(RTL_WLAN_INT_PREFIX)-1))
					goto drop;
			}
		}
#endif

/*
 * TBS_TAG: by luolei 2008--
 * Desc:Add IGMP Snooping support
 */
#if defined(CONFIG_IGMP_SNOOPING)
        if (is_multicast_ether_addr(dest) && snooping)
        {
			if (mc_forward(br, skb, (unsigned char*)dest, 1, 0))
            {         
                return 0;
            }
        }
#endif

#if defined (CONFIG_RTL_IGMP_SNOOPING)
	if (is_multicast_ether_addr(dest) && igmpsnoopenabled) {
		struct iphdr *iph=NULL;
#if defined (CONFIG_RTL_MLD_SNOOPING) 	
		struct ipv6hdr *ipv6h=NULL;
#endif //end CONFIG_RTL_MLD_SNOOPING
		uint32 fwdPortMask=0;
#if defined (CONFIG_RTL_HARDWARE_MULTICAST)
		unsigned int srcPort=skb->srcPort;
		unsigned int srcVlanId=skb->srcVlanId;
#endif //end CONFIG_RTL_HARDWARE_MULTICAST
	struct udphdr *udphd = NULL;

		unsigned char proto=0;
		unsigned char reserved=0;
		int ret=FAILED;
		
		unsigned char macAddr[6];
		unsigned char operation;
		char tmpOp;
		unsigned int gIndex=0;
		unsigned int moreFlag=1;
		
		struct rtl_multicastDataInfo multicastDataInfo;
		struct rtl_multicastFwdInfo multicastFwdInfo;

		if ( !(br->dev->flags & IFF_PROMISC) 
		 &&MULTICAST_MAC(dest) 
		&& (eth_hdr(skb)->h_proto == ETH_P_IP))
		{

			iph=(struct iphdr *)skb_network_header(skb);
			if(iph->daddr== 0xEFFFFFFA)
			{
				/*for microsoft upnp*/
				reserved=1;
			}
#if 0
			if((iph->daddr&0xFFFFFF00)==0xE0000000)
			reserved=1;
#endif
			proto =  iph->protocol; 

            //for RIP
            if(proto == IPPROTO_UDP)
        	{
        	    udphd = (struct udphdr *)((u8*)iph+sizeof(struct iphdr));
				if(udphd && (udphd->dest == htons(RIP_PROTO_PORT)))
					reserved=1;
        	}

			if (proto == IPPROTO_IGMP) 
			{	
#if defined (CONFIG_RTL_IGMP_SNOOPING) && defined (CONFIG_NETFILTER)
				//filter igmp pkts by upper hook like iptables 
				extern unsigned int (*IgmpRxFilter_Hook)(struct sk_buff *skb,
				     unsigned int hook,
				     const struct net_device *in,
				     const struct net_device *out,
				     struct xt_table *table);
				if(IgmpRxFilter_Hook != NULL)
				{
					if(IgmpRxFilter_Hook(skb, NF_INET_PRE_ROUTING,  skb->dev, NULL,dev_net(skb->dev)->ipv4.iptable_filter) !=NF_ACCEPT)
					{
						DEBUG_PRINT(" filter a pkt:%d %s:% \n", k, skb->dev->name, &(dev_net(skb->dev)->ipv4.iptable_filter->name[0]));
						goto drop;
					}
				}else
					DEBUG_PRINT("IgmpRxFilter_Hook is NULL\n");
#endif
				while(moreFlag)
				{
					tmpOp=igmp_type_check(skb, macAddr, &gIndex, &moreFlag);
					if(tmpOp>0)
					{
						//printk("%s:%d,macAddr is 0x%x:%x:%x:%x:%x:%x\n",__FUNCTION__,__LINE__,macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]);
						operation=(unsigned char)tmpOp;
						//operation :   2 - leave         1 - join
						br_update_igmp_snoop_fdb(operation, br, p, macAddr,skb);
					}
				}
				
				rtl_igmpMldProcess(brIgmpModuleIndex, skb_mac_header(skb), p->port_no, &fwdPortMask);
				//br_multicast_forward(br, fwdPortMask, skb, 0);
				/*2012-03-30 dengyanbing 解决在特定网络环境下bridge模式下snooping开启情况下组播流不能下来的问题(bug:29889).
				br_multicast_forward 中针对fwdPortMask进行判断，导致IGMP业务报文不能正常转发，影响WAN则组播流不能下来
				由于RTK 对fwdPortMask处理比较复杂，修改存在风险，所以直接调用linux原版函数br_flood_forward进行IGMP业务组播转发*/
				br_flood_forward(br, skb);
			}
			else if(((proto ==IPPROTO_UDP) ||(proto ==IPPROTO_TCP)) && (reserved ==0))
			{

				iph=(struct iphdr *)skb_network_header(skb);
				multicastDataInfo.ipVersion=4;
				multicastDataInfo.sourceIp[0]=  (uint32)(iph->saddr);
				multicastDataInfo.groupAddr[0]=  (uint32)(iph->daddr);
				
				ret= rtl_getMulticastDataFwdInfo(brIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);

				if(ret==SUCCESS)
				{
#if 0
					if ( ((1<<p->port_no)&br0SwFwdPortMask)==0)
					{
						br_multicast_forward(br, multicastFwdInfo.fwdPortMask&br0SwFwdPortMask, skb, 0);
					}
					else
					{
						br_multicast_forward(br, multicastFwdInfo.fwdPortMask, skb, 0);
					}
#else
					//br_multicast_forward(br, multicastFwdInfo.fwdPortMask, skb, !passedup);		
						//br_multicast_forward(br, multicastFwdInfo.fwdPortMask, skb, 0);
						br_multicast_forward(br, multicastFwdInfo.fwdPortMask & br0SwFwdPortMask, skb, 0);
#endif
					#if defined  (CONFIG_RTL_HARDWARE_MULTICAST)
					if((srcVlanId!=0) && (srcPort!=0xFFFF))
					{
						#if defined(CONFIG_RTK_VLAN_SUPPORT)
						if(rtk_vlan_support_enable == 0)
						{
							rtl865x_ipMulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask,srcPort,srcVlanId, multicastDataInfo.sourceIp[0], multicastDataInfo.groupAddr[0]);
						}
						#else
						rtl865x_ipMulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask,srcPort,srcVlanId, multicastDataInfo.sourceIp[0], multicastDataInfo.groupAddr[0]);
						#endif
					}	
					#endif
				}
				else
				{
					/*drop unknown multicast data*/
					if(multicastFwdInfo.unknownMCast)
					{
						goto drop;
					}
					
					br_flood_forward(br, skb);
				}

			}
			else
			{
				br_flood_forward(br, skb);
			}
		}
		else if(!(br->dev->flags & IFF_PROMISC) 
			&& IPV6_MULTICAST_MAC(dest)
			&& (eth_hdr(skb)->h_proto == ETH_P_IPV6))
		{

#if defined (IPV6_MCAST_TO_UNICAST)
			tmpOp=ICMPv6_check(skb , macAddr);
			if(tmpOp > 0){
				operation=(unsigned char)tmpOp;
#ifdef	DBG_ICMPv6
			if( operation == 1)
				printk("icmpv6 add from frame finish\n");
			else if(operation == 2)
				printk("icmpv6 del from frame finish\n");	
#endif
				br_update_igmp_snoop_fdb(operation, br, p, macAddr,skb);
			}
#endif

#if defined (CONFIG_RTL_MLD_SNOOPING)
			if(mldSnoopEnabled)
			{
				ipv6h=(struct ipv6hdr *)skb_network_header(skb);
				proto =  re865x_getIpv6TransportProtocol(ipv6h);
				/*icmp protocol*/
				if (proto == IPPROTO_ICMPV6) 
				{	
					rtl_igmpMldProcess(brIgmpModuleIndex, skb_mac_header(skb), p->port_no, &fwdPortMask);	
   				    //br_multicast_forward(br, fwdPortMask, skb, 0);
   				    /*2012-03-30 dengyanbing  解决在特定网络环境下bridge模式下snooping开启情况下组播流不能下来的问题(bug:29889).
   				       br_multicast_forward 中针对fwdPortMask进行判断，导致IGMP业务报文不能正常转发，影响WAN侧组播流不能下来
   				       由于RTK 对fwdPortMask处理比较复杂，修改存在风险，所以直接调用linux原版函数br_flood_forward进行IGMP业务组播转发*/
   				    br_flood_forward(br, skb);					
				}
				else if ((proto ==IPPROTO_UDP) ||(proto ==IPPROTO_TCP))
				{
					multicastDataInfo.ipVersion=6;
					memcpy(&multicastDataInfo.sourceIp, &ipv6h->saddr, sizeof(struct in6_addr));
					memcpy(&multicastDataInfo.groupAddr, &ipv6h->daddr, sizeof(struct in6_addr));	
					ret= rtl_getMulticastDataFwdInfo(brIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
					if(ret==SUCCESS)
					{						
						br_multicast_forward(br, multicastFwdInfo.fwdPortMask, skb, 0);
					}
					else
					{
						if (multicastFwdInfo.unknownMCast)
							goto drop;

						br_flood_forward(br, skb);
					}
				}
				else
				{
					br_flood_forward(br, skb);
				}	
			}
			else
#endif				
			{
				br_flood_forward(br, skb);
			}

		}
		else
		{
			br_flood_forward(br, skb);
		}

	
	}
	else
	{

		/*known/unknown unicast packet*/
		if (dst)
		{
#if 0
#ifdef CONFIG_NEW_PORTMAPPING
			int ret;
			ret = br_handle_portmap_downstrm(dst, skb);


			if(1 == ret)
				goto out;
			else if(2 == ret)
				goto drop;

			ret = br_handle_portmap_upstrm(dst, skb, br);

			if(1 == ret)
				goto out;
			else
#endif
#endif

			br_forward(dst->dst, skb);
		}
		else
			br_flood_forward(br, skb);
	}	
#else // CONFIG_RTL_IGMP_SNOOPING


	if (dst != NULL) 
	{
#if 0
#ifdef CONFIG_NEW_PORTMAPPING

		int ret;
		ret = br_handle_portmap_downstrm(dst, skb);



		if(1 == ret)
			goto out;
		else if(2 == ret)
			goto drop;


		ret = br_handle_portmap_upstrm(dst, skb, br);


		if(1 == ret)
			goto out;
		else
#endif	
#endif

		//if we do not need any port-mapping action, just forward it!
        {
		br_forward(dst->dst, skb);
		goto out;
		}

	}

	br_flood_forward(br, skb);

#endif	// CONFIG_RTL_IGMP_SNOOPING
	}

out:
	return 0;
drop:
	kfree_skb(skb);
	goto out;
}

/* note: already called with rcu_read_lock (preempt_disabled) */
static int br_handle_local_finish(struct sk_buff *skb)
{
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);

	if (p)
#if defined(CONFIG_VLAN_MODE)
        br_fdb_update(p->br, p, eth_hdr(skb)->h_source, 0);
        #else
		br_fdb_update(p->br, p, eth_hdr(skb)->h_source);
#endif
	return 0;	 /* process further */
}

/* Does address match the link local multicast address.
 * 01:80:c2:00:00:0X
 */
static inline int is_link_local(const unsigned char *dest)
{
	__be16 *a = (__be16 *)dest;
	static const __be16 *b = (const __be16 *)br_group_address;
	static const __be16 m = cpu_to_be16(0xfff0);

	return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | ((a[2] ^ b[2]) & m)) == 0;
}

/*
 * Called via br_handle_frame_hook.
 * Return NULL if skb is handled
 * note: already called with rcu_read_lock (preempt_disabled)
 */
struct sk_buff *br_handle_frame(struct net_bridge_port *p, struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	int (*rhook)(struct sk_buff *skb);

	if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
		goto drop;

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (!skb)
		return NULL;	

	if (unlikely(is_link_local(dest))) {
		/* Pause frames shouldn't be passed up by driver anyway */
		if (skb->protocol == htons(ETH_P_PAUSE))
			goto drop;

		/* If STP is turned off, then forward */
		if (p->br->stp_enabled == BR_NO_STP && dest[5] == 0)
			goto forward;

		if (NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, skb->dev,
			    NULL, br_handle_local_finish))
			return NULL;	/* frame consumed by filter */
		else
			return skb;	/* continue processing */
	}
#if 0
#if defined (STP_DISABLE_ETH)
//chris: auto stp on
	if (!(p->br->dev->flags & IFF_UP))
		goto err;

	if (p->state==BR_STATE_DISABLED && p->disable_by_mesh ==0){
		goto err;
	}else if (!strncmp(p->dev->name,"eth", 3)){
			p->br->stp_enabled = 1;
			mod_timer(&p->br->eth0_autostp_timer, jiffies+p->br->eth0_monitor_interval);
	}
#endif	
#endif 

forward:
	switch (p->state) {
	case BR_STATE_FORWARDING:
		rhook = rcu_dereference(br_should_route_hook);
		if (rhook != NULL) {
			if (rhook(skb))
				return skb;
			dest = eth_hdr(skb)->h_dest;
		}
		/* fall through */
	case BR_STATE_LEARNING:
		if (!compare_ether_addr(p->br->dev->dev_addr, dest))
			skb->pkt_type = PACKET_HOST;

#if defined (CONFIG_RTK_MESH)
if (p->state == BR_STATE_FORWARDING) {
		//brian modify for trigger portal-enable event
		if(!strncmp(p->dev->name,RTL_PS_ETH_NAME, 3)){
			if(p->br->mesh_pathsel_pid!= 0){
				if( !(p->br->eth0_received) )
				{
					p->br->eth0_received = 1;
					p->br->stp_enabled = 1;
					br_signal_pathsel(p->br);
					printk(KERN_INFO,"eth0 learning, event pathsel daemon \n");
				}
				
				mod_timer(&p->br->eth0_monitor_timer, jiffies+p->br->eth0_monitor_interval);
				
			}
		}
}
#endif	//CONFIG_RTK_MESH

		NF_HOOK(PF_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		break;
	default:
drop:
		kfree_skb(skb);
	}
	return NULL;
}

#if defined (CONFIG_RTL_IGMP_SNOOPING)

#if defined (IPV6_MCAST_TO_UNICAST)
/*Convert  MultiCatst IPV6_Addr to MAC_Addr*/
static void CIPV6toMac
	(unsigned char* icmpv6_McastAddr, unsigned char *gmac )
{
	/*ICMPv6 valid addr 2^32 -1*/
	gmac[0] = 0x33;
	gmac[1] = 0x33;
	gmac[2] = icmpv6_McastAddr[12];
	gmac[3] = icmpv6_McastAddr[13];
	gmac[4] = icmpv6_McastAddr[14];
	gmac[5] = icmpv6_McastAddr[15];			
}



static int ICMPv6_check(struct sk_buff *skb , unsigned char *gmac)
{
	
	struct ipv6hdr *ipv6h;
	char* protoType;	
	
	/* check IPv6 header information */
	//ipv6h = skb->nh.ipv6h;
	ipv6h = (struct ipv6hdr *)skb_network_header(skb);
	if(ipv6h->version != 6){	
		//printk("ipv6h->version != 6\n");
		return -1;
	}


	/*Next header: IPv6 hop-by-hop option (0x00)*/
	if(ipv6h->nexthdr == 0)	{
		protoType = (unsigned char*)( (unsigned char*)ipv6h + sizeof(struct ipv6hdr) );	
	}else{
		//printk("ipv6h->nexthdr != 0\n");
		return -1;
	}

	if(protoType[0] == 0x3a){
		
		//printk("recv icmpv6 packet\n");
		struct icmp6hdr* icmpv6h = (struct icmp6hdr*)(protoType + 8);
		unsigned char* icmpv6_McastAddr ;
	
		if(icmpv6h->icmp6_type == 0x83){
			
			icmpv6_McastAddr = (unsigned char*)((unsigned char*)icmpv6h + 8);
			#ifdef	DBG_ICMPv6					
			printk("Type: 0x%x (Multicast listener report) \n",icmpv6h->icmp6_type);
			#endif

		}else if(icmpv6h->icmp6_type == 0x8f){		
		
			icmpv6_McastAddr = (unsigned char*)((unsigned char*)icmpv6h + 8 + 4);
			#ifdef	DBG_ICMPv6					
			printk("Type: 0x%x (Multicast listener report v2) \n",icmpv6h->icmp6_type);
			#endif			
		}else if(icmpv6h->icmp6_type == 0x84){
		
			icmpv6_McastAddr = (unsigned char*)((unsigned char*)icmpv6h + 8 );			
			#ifdef	DBG_ICMPv6					
			printk("Type: 0x%x (Multicast listener done ) \n",icmpv6h->icmp6_type);
			#endif			
		}
		else{
			#ifdef	DBG_ICMPv6
			printk("Type: 0x%x (unknow type)\n",icmpv6h->icmp6_type);
			#endif			
			return -1;
		}				

		#ifdef	DBG_ICMPv6			
		printk("MCAST_IPV6Addr:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x \n",
			icmpv6_McastAddr[0],icmpv6_McastAddr[1],icmpv6_McastAddr[2],icmpv6_McastAddr[3],
			icmpv6_McastAddr[4],icmpv6_McastAddr[5],icmpv6_McastAddr[6],icmpv6_McastAddr[7],
			icmpv6_McastAddr[8],icmpv6_McastAddr[9],icmpv6_McastAddr[10],icmpv6_McastAddr[11],
			icmpv6_McastAddr[12],icmpv6_McastAddr[13],icmpv6_McastAddr[14],icmpv6_McastAddr[15]);
		#endif

		CIPV6toMac(icmpv6_McastAddr, gmac);
		
		#ifdef	DBG_ICMPv6					
		printk("group_mac [%02x:%02x:%02x:%02x:%02x:%02x] \n",
			gmac[0],gmac[1],gmac[2],
			gmac[3],gmac[4],gmac[5]);
		#endif
			


		if(icmpv6h->icmp6_type == 0x83){

			return 1;//icmpv6 listener report (add)
		}
		else if(icmpv6h->icmp6_type == 0x8f){
			return 1;//icmpv6 listener report v2 (add) 
		}
		else if(icmpv6h->icmp6_type == 0x84){
			return 2;//icmpv6 Multicast listener done (del)
		}
	}		
	else{
		//printk("protoType[0] != 0x3a\n");		
		return -1;//not icmpv6 type
	}
		
	return -1;
}

#endif	//end of IPV6_MCAST_TO_UNICAST

/*2008-01-15,for porting igmp snooping to linux kernel 2.6*/
static void ConvertMulticatIPtoMacAddr(__u32 group, unsigned char *gmac)
{
	__u32 u32tmp, tmp;
	int i;

	u32tmp = group & 0x007FFFFF;
	gmac[0]=0x01; gmac[1]=0x00; gmac[2]=0x5e;
	for (i=5; i>=3; i--) {
		tmp=u32tmp&0xFF;
		gmac[i]=tmp;
		u32tmp >>= 8;
	}
}
static char igmp_type_check(struct sk_buff *skb, unsigned char *gmac,unsigned int *gIndex,unsigned int *moreFlag)
{
        struct iphdr *iph;
	__u8 hdrlen;
	struct igmphdr *igmph;
	int i;
	unsigned int groupAddr=0;// add  for fit igmp v3
	
	/* check IP header information */
	iph=(struct iphdr *)skb_network_header(skb);
	hdrlen = iph->ihl << 2;
	if ((iph->version != 4) &&  (hdrlen < 20))
		goto  ERROR_BF_MOREFLAG; 
	if (ip_fast_csum((u8 *)iph, iph->ihl) != 0)
		goto  ERROR_BF_MOREFLAG; 
	{ /* check the length */
		__u32 len = ntohs(iph->tot_len);
		if (skb->len < len || len < hdrlen)
			goto  ERROR_BF_MOREFLAG; 
	}
	/* parsing the igmp packet */
	igmph = (struct igmphdr *)((u8*)iph+hdrlen);

	*moreFlag=0;
	
	if ((igmph->type==IGMP_HOST_MEMBERSHIP_REPORT) ||
	    (igmph->type==IGMPV2_HOST_MEMBERSHIP_REPORT)) 
	{
		groupAddr = igmph->group;
		if(!IN_MULTICAST(groupAddr))
		{			
				return -1;
		}
		
		ConvertMulticatIPtoMacAddr(groupAddr, gmac);
		
		return 1; /* report and add it */
	}
	else if (igmph->type==IGMPV3_HOST_MEMBERSHIP_REPORT)	{ 
		
	
		/*for support igmp v3 ; plusWang add 2009-0311*/   	
		struct igmpv3_report *igmpv3report=(struct igmpv3_report * )igmph;
		struct igmpv3_grec	*igmpv3grec=NULL; 
		//printk("%s:%d,*gIndex is %d,igmpv3report->ngrec is %d\n",__FUNCTION__,__LINE__,*gIndex,igmpv3report->ngrec);
		if(*gIndex>=igmpv3report->ngrec)
		{
			*moreFlag=0;
			return -1;
		}
	
		for(i=0;i<igmpv3report->ngrec;i++)
		{

			if(i==0)
			{
				igmpv3grec = (struct igmpv3_grec *)(&(igmpv3report->grec)); /*first igmp group record*/
			}
			else
			{
				igmpv3grec=(struct igmpv3_grec *)((unsigned char*)igmpv3grec+8+igmpv3grec->grec_nsrcs*4+(igmpv3grec->grec_auxwords)*4);
				
				
			}
			
			if(i!=*gIndex)
			{	
				
				continue;
			}
			
			if(i==(igmpv3report->ngrec-1))
			{
				/*last group record*/
				*moreFlag=0;
			}
			else
			{
				*moreFlag=1;
			}
			
			/*gIndex move to next group*/
			*gIndex=*gIndex+1;	
			
			groupAddr=igmpv3grec->grec_mca;
			//printk("%s:%d,groupAddr is %d.%d.%d.%d\n",__FUNCTION__,__LINE__,NIPQUAD(groupAddr));
			if(!IN_MULTICAST(groupAddr))
			{			
				return -1;
			}
			
			ConvertMulticatIPtoMacAddr(groupAddr, gmac);
			if(((igmpv3grec->grec_type == IGMPV3_CHANGE_TO_INCLUDE) || (igmpv3grec->grec_type == IGMPV3_MODE_IS_INCLUDE))&& (igmpv3grec->grec_nsrcs==0))
			{	
				return 2; /* leave and delete it */	
			}
			else if((igmpv3grec->grec_type == IGMPV3_CHANGE_TO_EXCLUDE) ||
				(igmpv3grec->grec_type == IGMPV3_MODE_IS_EXCLUDE) ||
				(igmpv3grec->grec_type == IGMPV3_ALLOW_NEW_SOURCES))
			{
				return 1;
			}
			else
			{
				/*ignore it*/
			}
			
			return -1;
		}
		
		/*avoid dead loop in case of initial gIndex is too big*/
		if(i>=(igmpv3report->ngrec-1))
		{
			/*last group record*/
			*moreFlag=0;
			return -1;
		}
		
	
	}
	else if (igmph->type==IGMP_HOST_LEAVE_MESSAGE){

		groupAddr = igmph->group;
		if(!IN_MULTICAST(groupAddr))
		{			
				return -1;
		}
		
		ConvertMulticatIPtoMacAddr(groupAddr, gmac);
		return 2; /* leave and delete it */
	}
	
	
	return -1;

ERROR_BF_MOREFLAG:
    *moreFlag = 0;
    return -1;
}

extern int chk_igmp_ext_entry(struct net_bridge_fdb_entry *fdb ,unsigned char *srcMac);
extern void add_igmp_ext_entry(	struct net_bridge_fdb_entry *fdb , unsigned char *srcMac , unsigned char portComeIn);
extern void update_igmp_ext_entry(	struct net_bridge_fdb_entry *fdb ,unsigned char *srcMac , unsigned char portComeIn);
extern void del_igmp_ext_entry(	struct net_bridge_fdb_entry *fdb ,unsigned char *srcMac , unsigned char portComeIn );

static void br_update_igmp_snoop_fdb(unsigned char op, struct net_bridge *br, struct net_bridge_port *p, unsigned char *dest 
										,struct sk_buff *skb)
{
	struct net_bridge_fdb_entry *dst;
	unsigned char *src;
	unsigned short del_group_src=0;
	unsigned char port_comein;
	int tt1;

#if defined (MCAST_TO_UNICAST)
	struct net_device *dev; 
	if(!dest)	return;
	if( !MULTICAST_MAC(dest)
#if defined (IPV6_MCAST_TO_UNICAST)
		&& !IPV6_MULTICAST_MAC(dest)
#endif	
	   )
	   { 
	   	return; 
	   }
#endif

#if defined( CONFIG_RTL_HARDWARE_MULTICAST) || defined(CONFIG_RTL865X_LANPORT_RESTRICTION)

	if(skb->srcPort!=0xFFFF)
	{
		port_comein = 1<<skb->srcPort;
	}
	else
	{
		port_comein=0x80;//wlan
	}
	
#else
	if(p && p->dev && p->dev->name && !memcmp(p->dev->name, RTL_PS_LAN_P0_DEV_NAME, 4))
	{
		port_comein = 0x01;
	}
	
	if(p && p->dev && p->dev->name && !memcmp(p->dev->name, RTL_PS_WLAN_NAME, 4))
	{
		port_comein=0x80;
	}
	
#endif
//	src=(unsigned char*)(skb->mac.raw+ETH_ALEN);
	src=(unsigned char*)(skb_mac_header(skb)+ETH_ALEN);
	/* check whether entry exist */
	dst = __br_fdb_get(br, dest);

	if (op == 1) /* add */
	{	
	
#if defined (MCAST_TO_UNICAST)
		/*process wlan client join --- start*/
		if (dst && p && p->dev && p->dev->name && !memcmp(p->dev->name, RTL_PS_WLAN_NAME, 4)) 
		{ 
			dst->portlist |= 0x80;
			port_comein = 0x80;
			//dev = __dev_get_by_name(&init_net,RTL_PS_WLAN0_DEV_NAME);	
			dev=p->dev;
			if (dev) 
			{			
				unsigned char StaMacAndGroup[20];
				memcpy(StaMacAndGroup, dest, 6);
				memcpy(StaMacAndGroup+6, src, 6);	
			#if defined(CONFIG_COMPAT_NET_DEV_OPS)
				if (dev->do_ioctl != NULL) 
				{
					dev->do_ioctl(dev, (struct ifreq*)StaMacAndGroup, 0x8B80);
			#else
			//	if (dev->netdev_ops->ndo_do_ioctl != NULL) 
			//	{
			//		dev->netdev_ops->ndo_do_ioctl(dev, (struct ifreq*)StaMacAndGroup, 0x8B80);
			#endif
					DEBUG_PRINT("... add to wlan mcast table:  DA:%02x:%02x:%02x:%02x:%02x:%02x ; SA:%02x:%02x:%02x:%02x:%02x:%02x\n", 
						StaMacAndGroup[0],StaMacAndGroup[1],StaMacAndGroup[2],StaMacAndGroup[3],StaMacAndGroup[4],StaMacAndGroup[5],
						StaMacAndGroup[6],StaMacAndGroup[7],StaMacAndGroup[8],StaMacAndGroup[9],StaMacAndGroup[10],StaMacAndGroup[11]);	
				}										
			}
		}
	/*process wlan client join --- end*/
#endif

		if (dst) 
		{
	        dst->group_src = dst->group_src | (1 << p->port_no);

			dst->ageing_timer = jiffies;

			tt1 = chk_igmp_ext_entry(dst , src); 
			if(tt1 == 0)
			{
				add_igmp_ext_entry(dst , src , port_comein);									
			}
			else
			{
				/* insert one fdb entry */
				DEBUG_PRINT("insert one fdb entry\n");
				br_fdb_insert(br, p, dest);
				dst = __br_fdb_get(br, dest);
				if(dst !=NULL)
				{
					dst->igmpFlag=1;
					dst->is_local=0;
					dst->portlist = port_comein; 
					dst->group_src = dst->group_src | (1 << p->port_no);
				}
			}
		}
	
	}
	else if (op == 2 && dst) /* delete */
	{
		DEBUG_PRINT("dst->group_src = %x change to ",dst->group_src);		
			del_group_src = ~(1 << p->port_no);
			dst->group_src = dst->group_src & del_group_src;
		DEBUG_PRINT(" %x ; p->port_no=%x \n",dst->group_src ,p->port_no);

		/*process wlan client leave --- start*/
		if (p && p->dev && p->dev->name && !memcmp(p->dev->name, RTL_PS_WLAN_NAME, 4)) 
		{ 			
			#ifdef	MCAST_TO_UNICAST
			//struct net_device *dev = __dev_get_by_name(&init_net,RTL_PS_WLAN0_DEV_NAME);
			struct net_device *dev=p->dev;
			if (dev) 
			{			
				unsigned char StaMacAndGroup[12];
				memcpy(StaMacAndGroup, dest , 6);
				memcpy(StaMacAndGroup+6, src, 6);
			#if defined(CONFIG_COMPAT_NET_DEV_OPS)
				if (dev->do_ioctl != NULL) 
				if (dev->do_ioctl != NULL) {
					dev->do_ioctl(dev, (struct ifreq*)StaMacAndGroup, 0x8B81);
			#else
			//	if (dev->netdev_ops->ndo_do_ioctl != NULL) 
			//	{
			//		dev->netdev_ops->ndo_do_ioctl(dev, (struct ifreq*)StaMacAndGroup, 0x8B81);
			#endif	
											
				DEBUG_PRINT("(del) wlan0 ioctl (del) M2U entry da:%02x:%02x:%02x-%02x:%02x:%02x; sa:%02x:%02x:%02x-%02x:%02x:%02x\n",
						StaMacAndGroup[0],StaMacAndGroup[1],StaMacAndGroup[2],StaMacAndGroup[3],StaMacAndGroup[4],StaMacAndGroup[5],
						StaMacAndGroup[6],StaMacAndGroup[7],StaMacAndGroup[8],StaMacAndGroup[9],StaMacAndGroup[10],StaMacAndGroup[11]);
				}
				
			}
			#endif	
			//dst->portlist &= ~0x80;	// move to del_igmp_ext_entry
			port_comein	= 0x80;
		}
		/*process wlan client leave --- end*/

		/*process entry del , portlist update*/
		del_igmp_ext_entry(dst , src ,port_comein);
		
		if (dst->portlist == 0)  // all joined sta are gone
		{
			DEBUG_PRINT("----all joined sta are gone,make it expired----\n");
			dst->ageing_timer -=  300*HZ; // make it expired		
		}
		

	}
}

#endif // CONFIG_RTL_IGMP_SNOOPING

/*
* august, 2011114:
* As we set portmapping by using ebtable in user space, these codes are
* temporarily unused. HOWEVER, it can not be removed, because it can solve a bug that ebtable can't sovle
* 
*/
#if 0
/**
return 1 ===> goto out
return 2 ===> goto drop
**/
//downstream
#ifdef CONFIG_NEW_PORTMAPPING
static int br_handle_portmap_downstrm(struct net_bridge_fdb_entry *dst, struct sk_buff *skb)
{
	if(dst && dst->sw_name  && (IFF_DOMAIN_WAN & skb->dev->priv_flags) 
							&& skb->dev->br_port->fgroup)
	{
		int32_t dst_sw_num;
		uint16_t tmp_fgroup;
	
#ifdef CONFIG_RTL_8676HWNAT
#ifdef	CONFIG_RTL_MULTI_ETH_WAN
		if((skb->dev->priv_flags & IFF_DOMAIN_WAN) && (skb->dev->priv_flags & IFF_OSMUX))
			tmp_fgroup = *(skb->dev->br_port->fgroup) >> 1;
		else
			tmp_fgroup = *(skb->dev->br_port->fgroup);
#else
		tmp_fgroup = *(skb->dev->br_port->fgroup);
#endif
		
#else//CONFIG_RTL_8676HWNAT
		tmp_fgroup = *(skb->dev->br_port->fgroup);
#endif
	
		#ifdef CONFIG_RTL_ALIASNAME	
    #ifdef CONFIG_RTL_ALIASNAME_DEBUG
            if((dst->sw_name[0] == 'e')!=(strncmp(dst->sw_name,ALIASNAME_ETH,strlen(ALIASNAME_ETH) )==0))
            {
                 printk("==================\n\n\n\n");
                 printk("[DEBUG Here %s %d\n]",__FUNCTION__,__LINE__);
            }
            if(dst->sw_name[0] == 'e')//
    #else
            if(strncmp(dst->sw_name,ALIASNAME_ETH,strlen(ALIASNAME_ETH) )==0)
        
    #endif
#else

		if(dst->sw_name[0] == 'e')//
#endif	

//    if(dst->sw_name[0] == 'e')//
       if(alias_name_are_eq(2,dst->sw_name,"eth",ALIASNAME_ETH,ALIASNAME_ETH0DOT))
       {

#ifdef CONFIG_RTL_8676HWNAT
			//get the 6th char of eth0.*
			
			#ifdef CONFIG_RTL_ALIASNAME	

            TOKEN_NUM(skb->switch_port,&dst_sw_num);
            dst_sw_num-=simple_strtol((const char *)ORIGINATE_NUM, NULL, 0);
            //dst_sw_num-=atoi_num(ORIGINATE_NUM);

            #else
			dst_sw_num = (int32_t)((skb->switch_port)[5] - '2');//the eth0.* begin from eth0.2
            #endif			

#else
			//get the 8th char of eth0_sw*
			
			#ifdef CONFIG_RTL_ALIASNAME	

            TOKEN_NUM(skb->sw_name,&dst_sw_num);
			#else
			dst_sw_num = (int32_t)((dst->sw_name)[7] - '0');
			#endif
#endif
}
//else if(dst->sw_name[5] == '-')//
else if(alias_name_are_eq(1,dst->sw_name,"wlan0-vap",ALIASNAME_WLAN0_VAP)){
    #ifdef CONFIG_RTL_ALIASNAME
         TOKEN_NUM(dst->sw_name,&dst_sw_num);
            dst_sw_num+=5;
    #else 
        dst_sw_num = 5 + (int32_t)((dst->sw_name)[9] - '0');//
    #endif
}

		else
			dst_sw_num = 4;
		
		//printk("[%s %d] the sw_num is %d, the fgroup is %d the dst->sw_name is %s\n",
								//__FILE__, __LINE__, dst_sw_num, tmp_fgroup, dst->sw_name);
								
		if(SWPORT_IN_FGROUP(dst_sw_num, tmp_fgroup))
		{
			br_forward(dst->dst, skb);
			return 1;
			//goto out;
		}
		else
			return 2;
			//goto drop;

	}

	return 0;
}



/**
return 1 ===> goto out
return 2 ===> goto drop
**/
//upstream
static int br_handle_portmap_upstrm(struct net_bridge_fdb_entry *dst, struct sk_buff *skb, 
												struct net_bridge *br)
{
		int32_t sw_num;

		if(NULL == dst->dst->fgroup)
		{
			br_forward(dst->dst, skb);
			return 1;
		}
		
#ifdef CONFIG_RTL_ALIASNAME	
    #ifdef CONFIG_RTL_ALIASNAME_DEBUG
		if((skb->switch_port[0] == 'e')!=(strncmp(skb->switch_port,ALIASNAME_ETH,strlen(ALIASNAME_ETH) )== 0))
		{
			 printk("==================\n\n\n\n");
			 printk("[DEBUG Here %s %d\n]",__FUNCTION__,__LINE__);
		}
			if(skb->switch_port && (skb->switch_port[0] == 'e'))//
    #else
				if(skb->switch_port && strncmp(skb->switch_port,ALIASNAME_ETH,strlen(ALIASNAME_ETH) )== 0)
	
    #endif
#else	
			//printk("<%s : %d> dev: %s. switch_port: %s. dst: %s\n", __FILE__, __LINE__, skb->dev, skb->switch_port,  dst->dst->dev);
			//if(dst->dst->fgroup)
				//printk("The dst->dst->fgroup is 0x%x\n",	*dst->dst->fgroup);
				
			//skb is from elan
			if(skb->switch_port && (skb->switch_port[0] == 'e'))
#endif	

        if(skb->switch_port && 
        //(skb->switch_port[0] == 'e'))//
        (alias_name_are_eq(2,skb->switch_port,"eth",ALIASNAME_ETH,ALIASNAME_ETH0DOT)))

			{
				uint16_t tmp_fgroup;
				
#ifdef CONFIG_RTL_8676HWNAT
#ifdef CONFIG_RTL_ALIASNAME	
					
								TOKEN_NUM(skb->switch_port,&sw_num);
								sw_num-=simple_strtol((const char *)ORIGINATE_NUM, NULL, 0);
								//sw_num-=atoi_num(ORIGINATE_NUM);

#else
				//get the 6th char of eth0.*
				sw_num = (int32_t)((skb->switch_port)[5] - '2'); //the eth0.* begin from eth0.2
#endif	
				
#ifdef	CONFIG_RTL_MULTI_ETH_WAN
				if((dst->dst->dev->priv_flags & IFF_DOMAIN_WAN) && (dst->dst->dev->priv_flags & IFF_OSMUX))
					tmp_fgroup = *(dst->dst->fgroup) >> 1;
				else
					tmp_fgroup = *(dst->dst->fgroup);
#else
				tmp_fgroup = *(dst->dst->fgroup);
#endif

#else
#ifdef CONFIG_RTL_ALIASNAME	
								TOKEN_NUM(skb->switch_port,&sw_num);
								
#else
				//get the 8th char of eth0_sw* 
				sw_num = (int32_t)((skb->switch_port)[7] - '0');
#endif	
				tmp_fgroup = *(dst->dst->fgroup);
#endif
	
				if( dst->dst->dev	&& (IFF_DOMAIN_WAN & dst->dst->dev->priv_flags)
									&& dst->dst->fgroup
									&& !SWPORT_IN_FGROUP(sw_num, tmp_fgroup))
				{
					//printk("[%s : %d : %s] The fdb conflict with portmapping. Need to flood!\n", __FILE__, __LINE__, __func__);
					br_flood_forward(br, skb);
					return 1;
					//goto out;
				}
				else
				{
					br_forward(dst->dst, skb);
					return 1;
					//goto out;
				}
	
			}
			//skb form wlan
			else if(skb->switch_port && 
			//(skb->switch_port[0] == 'w'))//
			(alias_name_are_eq(1,skb->switch_port,"wlan",ALIASNAME_WLAN)))
			{
				uint16_t tmp_fgroup;
	
#ifdef CONFIG_RTL_8676HWNAT
#ifdef	CONFIG_RTL_MULTI_ETH_WAN
				if((dst->dst->dev->priv_flags & IFF_DOMAIN_WAN) && (dst->dst->dev->priv_flags & IFF_OSMUX))
					tmp_fgroup = *(dst->dst->fgroup) >> 1;
				else
					tmp_fgroup = *(dst->dst->fgroup);
#else
				tmp_fgroup = *(dst->dst->fgroup);
#endif
#else
				tmp_fgroup = *(dst->dst->fgroup);
#endif
			
				//get the 10th char of wlan0-vap*
#ifdef CONFIG_RTL_ALIASNAME	
	
	
	TOKEN_NUM(skb->switch_port,&sw_num);
		sw_num += 5;
	
#else
				sw_num = 5 + ((skb->switch_port)[9] - '0');//
#endif
				//the wlan0' 10th char is 'null'
				if(sw_num < 0)
					sw_num = 4;
	
				if( dst->dst->dev	&& (IFF_DOMAIN_WAN & dst->dst->dev->priv_flags)
									&& dst->dst->fgroup
									&& !SWPORT_IN_FGROUP(sw_num, tmp_fgroup))
				{
					//printk("[%s : %d : %s] The fdb conflict with portmapping. Need to flood!\n", __FILE__, __LINE__, __func__);
					br_flood_forward(br, skb);
					return 1;
					//goto out;
				}
				else
				{
					br_forward(dst->dst, skb);
					return 1;
					//goto out;
				}
			}

		return 0;
}
#endif // CONFIG_NEW_PORTMAPPING 
#endif //0

