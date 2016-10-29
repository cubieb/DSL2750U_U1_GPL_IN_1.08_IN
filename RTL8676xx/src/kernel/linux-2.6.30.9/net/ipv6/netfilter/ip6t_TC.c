/*
 * This module is for setting the IPv6 traffic class field
 * (C) 2010 xuxiaodong <xuxiaodong@twsz.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/netfilter_ipv6/ip6t_TC.h>

MODULE_AUTHOR("xuxiaodong<xuxiaodong@twsz.com>");
MODULE_DESCRIPTION("IP6 tables traffic class setting module");
MODULE_LICENSE("GPL");

static u16 ipv6_get_tcfield(struct ipv6hdr *ipv6h)
{
	return (ntohs(*(u16 *)ipv6h)&IPV6_TC_MASK)>>4;
}

static void ipv6_change_tcfield(struct ipv6hdr *ipv6h, u8 value)
{
    u16 tmp;

    tmp = ntohs(*(u16 *) ipv6h);
    tmp &= ~IPV6_TC_MASK;
    tmp |= value<<4;
    *(__u16 *) ipv6h = htons(tmp);
}

static unsigned int target(struct sk_buff *skb, const struct xt_target_param *par)
{
	const struct ip6t_TC_info *info = (struct ip6t_TC_info *)(par->targinfo);
	u_int8_t tc = (u_int8_t)ipv6_get_tcfield(ipv6_hdr(skb));

	if (tc != info->tc) {
		if (!skb_make_writable(skb, sizeof(struct ipv6hdr)))
			return NF_DROP;

		ipv6_change_tcfield(ipv6_hdr(skb), info->tc);
	}
	return XT_CONTINUE;
}

static bool checkentry(const struct xt_tgchk_param *par)
{
	const u_int8_t tc = ((struct ip6t_TC_info *)(par->targinfo))->tc;

	if (tc > 255) {
		printk(KERN_WARNING "traffic class: tc %x out of range\n", tc);
		return 0;
	}
	return 1;
}

static struct ip6t_target ip6t_tc_target = {

		.name		= "TC",
		.checkentry	= checkentry,
		.target		= target,
		.targetsize	= sizeof(struct ip6t_TC_info),
		.table		= "mangle",
		.me		= THIS_MODULE,
};

static int __init ip6t_tc_target_init(void)
{
	return ip6t_register_target(&ip6t_tc_target);
}

static void __exit ip6t_tc_target_fini(void)
{
	ip6t_unregister_target(&ip6t_tc_target);
}

module_init(ip6t_tc_target_init);
module_exit(ip6t_tc_target_fini);

