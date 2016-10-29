/* traffic class matching module */

/* (C) 2010 xuxiaodong <xuxiaodong@twsz.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter_ipv6/ip6t_tc.h>
#include <linux/netfilter_ipv6/ip6_tables.h>

MODULE_AUTHOR("xuxiaodong<xuxiaodong@twsz.com>");
MODULE_DESCRIPTION("IP6 tables Traffic Class matching module");
MODULE_LICENSE("GPL");

static u16 ipv6_get_tcfield(struct ipv6hdr *ipv6h)
{
	return (ntohs(*(u16 *)ipv6h)&IPV6_TC_MASK)>>4;
}

static bool match(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct ip6t_tc_info *info = (struct ip6t_tc_info *)(par->matchinfo);
	u8 tc = (u8)ipv6_get_tcfield(ipv6_hdr(skb));

	return (tc == info->tc) ^ !!info->invert;
}

static struct ip6t_match tc_match = {
	.name		= "tc",
	.match		= match,
	.matchsize	= sizeof(struct ip6t_tc_info),
	.me		= THIS_MODULE,
};

static int __init ip6t_tc_init(void)
{
	return ip6t_register_match(&tc_match);
}

static void __exit ip6t_tc_fini(void)
{
	ip6t_unregister_match(&tc_match);

}

module_init(ip6t_tc_init);
module_exit(ip6t_tc_fini);

