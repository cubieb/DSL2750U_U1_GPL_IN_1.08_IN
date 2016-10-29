/*
 * This is a module which is used for setting the skb->priority field
 * of an skb for qdisc classification.
 */

/* (C) 2001-2002 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/checksum.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_CLASSIFY.h>
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
#include <linux/netfilter_ipv4/ip_tables.h>
#include <net/pkt_cls.h>
#include <net/rtl/rtl865x_outputQueue.h>
#include "../bridge/br_private.h"
#include <net/rtl/rtl_nic.h>
#endif


MODULE_AUTHOR("Patrick McHardy <kaber@trash.net>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Xtables: Qdisc classification");
MODULE_ALIAS("ipt_CLASSIFY");
MODULE_ALIAS("ip6t_CLASSIFY");

static unsigned int
classify_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
	const struct xt_classify_target_info *clinfo = par->targinfo;

	skb->priority = clinfo->priority;
	return XT_CONTINUE;
}

#if	defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
static int classify_target2acl(const char *tablename,
			  const void *entry,
			  const struct xt_target *target,
			  void *targinfo,
			  rtl865x_AclRule_t *rule,
			  unsigned int hook_mask, 
			  void **data,
			  unsigned char qosChain)
{
	struct net_device *dev, *matchDev;
	struct ipt_entry *e;
	const struct xt_classify_target_info *clinfo = targinfo;
	rtl865x_qos_rule_t qosRule;
	struct net_bridge_port *br_port;	

	if (!qosChain)
		return 0;

	memset(&qosRule,0,sizeof(rtl865x_qos_rule_t));
	e = (struct ipt_entry *)entry;

	if(!strcmp(e->ip.iniface,""))
	{
		//memcpy(qosRule.inIfname,RTL_DRV_LAN_NETIF_NAME,IFNAMSIZ);	
	}
	else if(rtl865x_netifExist(e->ip.iniface))  /*  the interface is in asic netif table  */
	{
		memcpy(qosRule.inIfname, e->ip.iniface, IFNAMSIZ);
	}
	else
	{
		dev = re865x_get_netdev_by_name(e->ip.iniface);
		if(dev) /*  the interface is under 0412 switch  */			
		{

			rtl865x_AclRule_t oldAclRule;

			/*  check whether this interface is  under br0 */
			br_port = dev->br_port;
			if(br_port && !strcmp(dev->br_port->br->dev->name,RTL_DRV_LAN_NETIF_NAME))										
				memcpy(qosRule.inIfname,RTL_DRV_LAN_NETIF_NAME,IFNAMSIZ);	
			else
				return 0;
				
			memcpy(&oldAclRule, rule, sizeof(rtl865x_AclRule_t));
			memset(&rule->un_ty, 0,sizeof(rtl865x_AclRuleData_t));

			if(oldAclRule.ruleType_==RTL865X_ACL_MAC)
			{
				memcpy(rule->srcFilterMac_.octet, oldAclRule.srcMac_.octet, ETH_ALEN);
				memcpy(rule->srcFilterMacMask_.octet, oldAclRule.srcMacMask_.octet, ETH_ALEN);
				rule->srcFilterIgnoreL3L4_ = 1;
				rule->srcFilterIgnoreL4_ = 1;
			}
			else if(oldAclRule.ruleType_==RTL865X_ACL_IP)
			{
				rule->srcFilterIpAddr_ = oldAclRule.srcIpAddr_;
				rule->srcFilterIpAddrMask_ = oldAclRule.srcIpAddrMask_;					
				rule->srcFilterIgnoreL4_ = 1;
			}
			else if(oldAclRule.ruleType_==RTL865X_ACL_TCP)
			{
				rule->srcFilterIpAddr_ = oldAclRule.srcIpAddr_;
				rule->srcFilterIpAddrMask_ = oldAclRule.srcIpAddrMask_;
				rule->srcFilterIpAddrLB_ = oldAclRule.tcpSrcPortLB_;
				rule->srcFilterIpAddrUB_ = oldAclRule.tcpSrcPortUB_;		
			}
			else if(oldAclRule.ruleType_==RTL865X_ACL_UDP)
			{
				rule->srcFilterIpAddr_ = oldAclRule.srcIpAddr_;
				rule->srcFilterIpAddrMask_ = oldAclRule.srcIpAddrMask_;
				rule->srcFilterIpAddrLB_ = oldAclRule.udpSrcPortLB_;
				rule->srcFilterIpAddrUB_ = oldAclRule.udpSrcPortUB_;		
			}
			else if(oldAclRule.ruleType_==RTL865X_ACL_ICMP)
			{
				rule->srcFilterIpAddr_ = oldAclRule.srcIpAddr_;
				rule->srcFilterIpAddrMask_ = oldAclRule.srcIpAddrMask_;					
			}
			else
				return 0;

			rule->ruleType_ = RTL865X_ACL_SRCFILTER;										
			rule->srcFilterPort_ = ((struct dev_priv *)dev->priv)->portmask;  /*   port id  */							
			
		}
		else
			return 0;
	}
	
	
	
	memcpy(qosRule.outIfname, e->ip.outiface, IFNAMSIZ);		
	qosRule.classify= clinfo->priority;	
	
	rule->upDown_=0;//Uplink	
	qosRule.rule = rule;

	dev = __dev_get_by_name(&init_net, e->ip.outiface);
	tc_getHandleByClassify(clinfo->priority, &qosRule.handle, dev, &matchDev);
	rtl865x_qosAddRule(&qosRule);
	

	return RTL865X_SKIP_THIS_RULE;
}
#endif


static struct xt_target classify_tg_reg __read_mostly = {
	.name       = "CLASSIFY",
	.revision   = 0,
	.family     = NFPROTO_UNSPEC,
	.table      = "mangle",
	.hooks      = (1 << NF_INET_LOCAL_OUT) | (1 << NF_INET_FORWARD) |
		      (1 << NF_INET_POST_ROUTING),
	.target     = classify_tg,
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
	.target2acl	= classify_target2acl,
#endif
	.targetsize = sizeof(struct xt_classify_target_info),
	.me         = THIS_MODULE,
};

static int __init classify_tg_init(void)
{
	return xt_register_target(&classify_tg_reg);
}

static void __exit classify_tg_exit(void)
{
	xt_unregister_target(&classify_tg_reg);
}

module_init(classify_tg_init);
module_exit(classify_tg_exit);
