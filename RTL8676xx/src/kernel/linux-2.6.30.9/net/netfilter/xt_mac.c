/* Kernel module to match MAC address parameters. */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
/*
 * TBS_TAG: add by Charles Alison 2012-2-20
 * Description: iptables mac 与开源保持一致
 */ 
#if 0
#if 0

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/etherdevice.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter/xt_mac.h>
#include <linux/netfilter/x_tables.h>

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl865x_netif.h>
#include "../bridge/br_private.h"
#include <net/rtl/rtl_nic.h>
#endif


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("Xtables: MAC address match");
MODULE_ALIAS("ipt_mac");
MODULE_ALIAS("ip6t_mac");

static bool mac_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
    const struct xt_mac_info *info = par->matchinfo;
    /*linux-2.6.19*/ 
    int i, invert;
    unsigned char info_addr[ETH_ALEN];
    unsigned char pkt_addr[ETH_ALEN];

	if (info->flags & MAC_SRC) {
		invert = !!(info->flags & MAC_SRC_INV);
		if (info->flags & SRC_MASK) {
			for (i=0; i<ETH_ALEN; i++) {
				info_addr[i] = (info->srcaddr[i] & info->srcmask[i]);
				pkt_addr[i] = (eth_hdr(skb)->h_source[i] & info->srcmask[i]);
			}
			if (skb_mac_header(skb) < skb->head
				|| (skb_mac_header(skb) + ETH_HLEN) > skb->data
				|| (compare_ether_addr(pkt_addr, info_addr)
				 ^ invert))
				return 0;
		}
		else {
			if (skb_mac_header(skb) < skb->head
				|| (skb_mac_header(skb) + ETH_HLEN) > skb->data
				|| (compare_ether_addr(eth_hdr(skb)->h_source, info->srcaddr)
				 ^ invert))
				return 0;
		}
	}
	if (info->flags & MAC_DST) {
		invert = !!(info->flags & MAC_DST_INV);
		if (info->flags & DST_MASK) {
			for (i=0; i<ETH_ALEN; i++) {
				info_addr[i] = (info->dstaddr[i] & info->dstmask[i]);
				pkt_addr[i] = (eth_hdr(skb)->h_dest[i] & info->dstmask[i]);
			}
			if (skb_mac_header(skb) < skb->head
				|| (skb_mac_header(skb) + ETH_HLEN) > skb->data
				|| (compare_ether_addr(pkt_addr, info_addr)
				 ^ invert))
				return 0;
		}
		else {
			if (skb_mac_header(skb) < skb->head
				|| (skb_mac_header(skb) + ETH_HLEN) > skb->data
				|| (compare_ether_addr(eth_hdr(skb)->h_dest, info->dstaddr)
				 ^ invert))
				return 0;
		}
	}

	return 1;

#if 0
    /* Is mac pointer valid? */
    return skb_mac_header(skb) >= skb->head &&
	   skb_mac_header(skb) + ETH_HLEN <= skb->data
	   /* If so, compare... */
	   && ((!compare_ether_addr(eth_hdr(skb)->h_source, info->srcaddr))
		^ info->invert);
#endif
}

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL) && !defined(CONFIG_RTL8676_Dynamic_ACL)
static int mac_match2acl(const char *tablename,
			  const void *ip,
			  const struct xt_match *match,
			  void *matchinfo,
			  rtl865x_AclRule_t *acl_rule,
			  unsigned int *invflags)
{

	const struct xt_mac_info *info = matchinfo;
	rtl865x_AclRule_t *rule = acl_rule;	
	
	
	if(matchinfo == NULL || rule == NULL)
		return 1;



	if(rtl865x_netifExist(rule->inNetif))  /*  the interface is in asic netif table  */
	{
		/*  in bridge mode	 : if  rule->inNetif = br0
		     in IPoE mode 	:  if  rule->inNetif = br0 / nas0 /ppp0 */


		rule->ruleType_ = RTL865X_ACL_MAC;	
		rule->toACL_ = 1;
		rule->macFilter_ = 1;
		
		memset(rule->srcMac_.octet, 0, ETH_ALEN);
		memset(rule->srcMacMask_.octet, 0, ETH_ALEN);
		memset(rule->dstMac_.octet, 0, ETH_ALEN);
		memset(rule->dstMacMask_.octet, 0, ETH_ALEN);

		rule->typeLen_ 		= 0x0800;
		rule->typeLenMask_ 	= 0xFFFF;
	
		memcpy(rule->srcMac_.octet, info->srcaddr, ETH_ALEN);
		memcpy(rule->dstMac_.octet, info->dstaddr, ETH_ALEN);
		if(info->flags&MAC_SRC)
		{
			if (info->flags & SRC_MASK)
				memcpy(rule->srcMacMask_.octet, info->srcmask, ETH_ALEN);				
			else 
				memset(rule->srcMacMask_.octet, 0xff, ETH_ALEN);
		}
		if(info->flags&MAC_DST)
		{
			if (info->flags & DST_MASK)
				memcpy(rule->dstMacMask_.octet, info->dstmask, ETH_ALEN);				
			else 
				memset(rule->dstMacMask_.octet, 0xff, ETH_ALEN);	
		}
		return 0;
	    
	}
	else /*  the interface is not in asic netif table  */
	{				
		struct net_device* dev;
		dev = re865x_get_netdev_by_name(rule->inNetif);
		if(dev) /*  the interface is under 0412 switch  */			
		{
			/*  in bridge mode	 : if  rule->inNetif = eth0.2~eth0.5/nas0
		     		in IPoE mode 	:  if  rule->inNetif = eth0.2~eth0.5              */
			#if 1
			rule->ruleType_ = RTL865X_ACL_MAC;	
			rule->toACL_ = 1;
			rule->macFilter_ = 1;
			
			memset(rule->srcMac_.octet, 0, ETH_ALEN);
			memset(rule->srcMacMask_.octet, 0, ETH_ALEN);
			memset(rule->dstMac_.octet, 0, ETH_ALEN);
			memset(rule->dstMacMask_.octet, 0, ETH_ALEN);

			rule->typeLen_ 		= 0x0800;
			rule->typeLenMask_ 	= 0xFFFF;
		
			memcpy(rule->srcMac_.octet, info->srcaddr, ETH_ALEN);
			memcpy(rule->dstMac_.octet, info->dstaddr, ETH_ALEN);
			if(info->flags&MAC_SRC)
			{
				if (info->flags & SRC_MASK)
					memcpy(rule->srcMacMask_.octet, info->srcmask, ETH_ALEN);				
				else 
					memset(rule->srcMacMask_.octet, 0xff, ETH_ALEN);
			}
			if(info->flags&MAC_DST)
			{
				if (info->flags & DST_MASK)
					memcpy(rule->dstMacMask_.octet, info->dstmask, ETH_ALEN);				
				else 
					memset(rule->dstMacMask_.octet, 0xff, ETH_ALEN);	
			}
			return 0;
			#else			
			/* Kevin, only source filter in 0412 acl can co-operate with iptables mac rule
			ex.  /bin/iptables -A FORWARD -i eth0.4 -m mac --mac-source 00:50:fc:32:a6:b9 -j DROP
			    only source filter has the source port id filed  , we can translate eth0.4 to port 3

			Other formats of iptables mac rule , we trap it to cpu    */

			if (  (info->flags&MAC_SRC) &&  !(info->flags&MAC_DST) ) 
			{		
				rule->ruleType_ = RTL865X_ACL_SRCFILTER;
				rule->srcFilterIgnoreL3L4_ = 1;
				rule->srcFilterIgnoreL4_ = 1;
				rule->toACL_ = 1;						
				rule->srcFilterPort_ = ((struct dev_priv *)dev->priv)->portmask;  /*   port id  */
				
				memcpy(rule->srcFilterMac_.octet, info->srcaddr, ETH_ALEN);
				if (info->flags & SRC_MASK)
					memcpy(rule->srcFilterMacMask_.octet, info->srcmask, ETH_ALEN);				
				else 
					memset(rule->srcFilterMacMask_.octet, 0xff, ETH_ALEN);			
				return 0;
			}
			else if (  !(info->flags&MAC_SRC) && (info->flags&MAC_DST) )
			{
				rule->ruleType_ = RTL865X_ACL_MAC;	
				rule->toACL_ = 1;
				//rule->forceTrapToCPU_ = 1;

				memset(rule->srcMac_.octet, 0, ETH_ALEN);
				memset(rule->srcMacMask_.octet, 0, ETH_ALEN);
				memset(rule->dstMac_.octet, 0, ETH_ALEN);
				memset(rule->dstMacMask_.octet, 0, ETH_ALEN);
				memcpy(rule->dstMac_.octet, info->dstaddr, ETH_ALEN);
				if (info->flags & DST_MASK)
					memcpy(rule->dstMacMask_.octet, info->dstmask, ETH_ALEN);				
				else 
					memset(rule->dstMacMask_.octet, 0xff, ETH_ALEN);	
				rule->typeLen_ = rule->typeLenMask_ = 0;
				return 0;
			}
			else if (  (info->flags&MAC_SRC) && (info->flags&MAC_DST) )
			{
				rule->ruleType_ = RTL865X_ACL_MAC;	
				rule->toACL_ = 1;
				//rule->forceTrapToCPU_ = 1;

				memset(rule->srcMac_.octet, 0, ETH_ALEN);
				memset(rule->srcMacMask_.octet, 0, ETH_ALEN);
				memset(rule->dstMac_.octet, 0, ETH_ALEN);
				memset(rule->dstMacMask_.octet, 0, ETH_ALEN);
				memcpy(rule->srcMac_.octet, info->srcaddr, ETH_ALEN);
				memcpy(rule->dstMac_.octet, info->dstaddr, ETH_ALEN);
				if (info->flags & SRC_MASK)
					memcpy(rule->srcMacMask_.octet, info->srcmask, ETH_ALEN);				
				else 
					memset(rule->srcMacMask_.octet, 0xff, ETH_ALEN);
				if (info->flags & DST_MASK)
					memcpy(rule->dstMacMask_.octet, info->dstmask, ETH_ALEN);				
				else 
					memset(rule->dstMacMask_.octet, 0xff, ETH_ALEN);	
				rule->typeLen_ = rule->typeLenMask_ = 0;
				return 0;
			}
			else
				return 1;
			#endif
					
		}
		else /*  the interface is not under 0412 switch , we need not to handle it */	
		{
			/*  in bridge mode	 : if  rule->inNetif = vc0 , wlan0
		     		in IPoE mode 	:  if  rule->inNetif = vc0 , wlan0       */
			return 1; /* add rule fail  */
		}		
	}
	
}
#endif


static struct xt_match mac_mt_reg __read_mostly = {
	.name      = "mac",
	.revision  = 0,
	.family    = NFPROTO_UNSPEC,
	.match     = mac_mt,
	.matchsize = sizeof(struct xt_mac_info),
	.hooks     = (1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_IN) |
	             (1 << NF_INET_FORWARD),
	.me        = THIS_MODULE,
#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL) && !defined(CONFIG_RTL8676_Dynamic_ACL)
	.match2acl	= mac_match2acl,
#endif

};




static int __init mac_mt_init(void)
{
	return xt_register_match(&mac_mt_reg);
}

static void __exit mac_mt_exit(void)
{
	xt_unregister_match(&mac_mt_reg);
}

module_init(mac_mt_init);
module_exit(mac_mt_exit);

#else

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/etherdevice.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter/xt_mac.h>
#include <linux/netfilter/x_tables.h>
#include <net/dst.h>

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl865x_netif.h>
#endif


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("Xtables: MAC address match");
MODULE_ALIAS("ipt_mac");
MODULE_ALIAS("ip6t_mac");

static int compare_with_header_cache_dest_mac(const struct sk_buff *skb, char *macaddr)
{
	struct dst_entry *dst = skb->dst;
	struct hh_cache *hh;
	int ret = 0;
	
	if ((dst)&&(dst->hh)&&(dst->hh->hh_data))
	{
	    	hh = dst->hh;
	    	if (hh && (hh->hh_type==ETH_P_IP))
		{
	    		read_lock_bh(&hh->hh_lock);
	      		memcpy(skb->data - 16, hh->hh_data, 16);
	      		if (memcmp((((u8*)hh->hh_data) + 2), macaddr, ETH_ALEN) == 0)
	      		    ret = 1;
	    		read_unlock_bh(&hh->hh_lock);
	    	}
	}
	else
	{
		if(!compare_ether_addr(eth_hdr(skb)->h_dest, macaddr))
		{
			ret=1;
		}
	}
	return ret;
}

static bool mac_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_mac_info *info = par->matchinfo;   

	if (info->flags & MAC_SRC) {
	     /* Is mac pointer valid? */
	    if ((skb_mac_header(skb) >= skb->head
		    && (skb_mac_header(skb) + ETH_HLEN) <= skb->data
		    /* If so, compare... */
		    && ((!compare_ether_addr(eth_hdr(skb)->h_source, info->srcaddr.macaddr))
			^ !!(info->flags & MAC_SRC_INV)))==0)
	    	{
			return 0;
	    	}
	}

	if (info->flags & MAC_DST) {
	     /* Is mac pointer valid? */
	    if( (skb_mac_header(skb) >= skb->head
		    && (skb_mac_header(skb) + ETH_HLEN) <= skb->data
		    /* If so, compare... */
		    && (compare_with_header_cache_dest_mac(skb, (char*)(info->dstaddr.macaddr)) ^ !!(info->flags & MAC_DST_INV)))==0)
	    	{
			return 0;
	    	}
	}

	return 1;

}

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
static int mac_match2acl(const char *tablename,
			  const void *ip,
			  const struct xt_match *match,
			  void *matchinfo,
			  void *acl_rule,
			  unsigned int *invflags)
{

	const struct xt_mac_info *info = matchinfo;
	rtl865x_AclRule_t *rule = (rtl865x_AclRule_t *)acl_rule;
	if(matchinfo == NULL || rule == NULL)
		return 1;

	rule->ruleType_ = RTL865X_ACL_MAC;

	//To initial first
	memset(rule->srcMac_.octet, 0, ETH_ALEN);
	memset(rule->srcMacMask_.octet, 0, ETH_ALEN);
	memset(rule->dstMac_.octet, 0, ETH_ALEN);
	memset(rule->dstMacMask_.octet, 0, ETH_ALEN);
	
	if (info->flags & MAC_SRC) {
		memcpy(rule->srcMac_.octet, info->srcaddr.macaddr, ETH_ALEN);
		memset(rule->srcMacMask_.octet, 0xff, ETH_ALEN);
	}

	if (info->flags & MAC_DST) {
		memcpy(rule->dstMac_.octet, info->dstaddr.macaddr, ETH_ALEN);
		memset(rule->dstMacMask_.octet, 0xff, ETH_ALEN);
	}
	
	rule->typeLen_ = rule->typeLenMask_ = 0;
	
	return 0;
}
#endif

static struct xt_match mac_mt_reg __read_mostly = {
	.name      = "mac",
	.revision  = 0,
	.family    = NFPROTO_UNSPEC,
	.match     = mac_mt,
	.matchsize = sizeof(struct xt_mac_info),
/*	.hooks     = (1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_IN) |
	             (1 << NF_INET_FORWARD),
*/
	.me        = THIS_MODULE,
#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
	.match2acl	= mac_match2acl,
#endif

};

static int __init mac_mt_init(void)
{
	return xt_register_match(&mac_mt_reg);
}

static void __exit mac_mt_exit(void)
{
	xt_unregister_match(&mac_mt_reg);
}

module_init(mac_mt_init);
module_exit(mac_mt_exit);

#endif
#else
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/etherdevice.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter/xt_mac.h>
#include <linux/netfilter/x_tables.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("Xtables: MAC address match");
MODULE_ALIAS("ipt_mac");
MODULE_ALIAS("ip6t_mac");

static bool mac_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
    const struct xt_mac_info *info = par->matchinfo;

    /* Is mac pointer valid? */
    return skb_mac_header(skb) >= skb->head &&
	   skb_mac_header(skb) + ETH_HLEN <= skb->data
	   /* If so, compare... */
	   && ((!compare_ether_addr(eth_hdr(skb)->h_source, info->srcaddr))
		^ info->invert);
}

static struct xt_match mac_mt_reg __read_mostly = {
	.name      = "mac",
	.revision  = 0,
	.family    = NFPROTO_UNSPEC,
	.match     = mac_mt,
	.matchsize = sizeof(struct xt_mac_info),
	.hooks     = (1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_IN) |
	             (1 << NF_INET_FORWARD),
	.me        = THIS_MODULE,
};

static int __init mac_mt_init(void)
{
	return xt_register_match(&mac_mt_reg);
}

static void __exit mac_mt_exit(void)
{
	xt_unregister_match(&mac_mt_reg);
}

module_init(mac_mt_init);
module_exit(mac_mt_exit);

#endif
/*
 * TBS_END_TAG
 */
