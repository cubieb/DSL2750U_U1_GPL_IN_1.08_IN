/* Flow label matching module */

/* (C) 2010 xuxiaodong <xuxiaodong@twsz.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter_ipv6/ip6t_fl.h>
#include <linux/netfilter_ipv6/ip6_tables.h>

MODULE_AUTHOR("xuxiaodong<xuxiaodong@twsz.com>");
MODULE_DESCRIPTION("IP6 tables Flow Label matching module");
MODULE_LICENSE("GPL");

static u32 ipv6_get_flfield(struct ipv6hdr *ipv6h)
{
	return (u32)(ntohl(*(u32 *)ipv6h)&IPV6_FLOW_LABLE_MASK);
}

static bool match(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct ip6t_fl_info *info = (struct ip6t_fl_info *)(par->matchinfo);
	u32 flowlabel = ipv6_get_flfield(ipv6_hdr(skb));

	return (flowlabel == info->flowlabel) ^ !!info->invert;
}

static struct ip6t_match fl_match = {
	.name		= "fl",
	.match		= match,
	.matchsize	= sizeof(struct ip6t_fl_info),
	.me		= THIS_MODULE,
};

static int __init ip6t_fl_init(void)
{
	return ip6t_register_match(&fl_match);
}

static void __exit ip6t_fl_fini(void)
{
	ip6t_unregister_match(&fl_match);

}

module_init(ip6t_fl_init);
module_exit(ip6t_fl_fini);

