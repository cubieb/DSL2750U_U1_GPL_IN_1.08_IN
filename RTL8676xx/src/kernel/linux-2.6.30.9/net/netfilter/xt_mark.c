/*
 *	xt_mark - Netfilter module to match NFMARK value
 *
 *	(C) 1999-2001 Marc Boucher <marc@mbsi.ca>
 *	Copyright © CC Computer Consultants GmbH, 2007 - 2008
 *	Jan Engelhardt <jengelh@computergmbh.de>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter/xt_mark.h>
#include <linux/netfilter/x_tables.h>

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl865x_netif.h>
#endif


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marc Boucher <marc@mbsi.ca>");
MODULE_DESCRIPTION("Xtables: packet mark match");
MODULE_ALIAS("ipt_mark");
MODULE_ALIAS("ip6t_mark");

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
static int mark_match2acl(const char *tablename,
			  const void *ip,
			  const struct xt_match *match,
			  void *matchinfo,
			  rtl865x_AclRule_t *acl_rule,
			  unsigned int *invflags)
{

	const struct xt_mark_info *info = matchinfo;
	rtl865x_AclRule_t *rule = acl_rule;

	if(info == NULL || rule == NULL)
		return 1;	

	if(!(info->mark & 0xff)) /* in nic driver ,802.1p is at the last 8 bits of skb->mark */
		return 1;

	/* in nic driver , skb->mark = (the value of 802.1p)+1 */
	rule->ruleType_ = RTL865X_ACL_802D1P;
	rule->vlanTagPri_ = (info->mark & 0xff) - 1;

	return 0;
}
#endif


static bool
mark_mt_v0(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_mark_info *info = par->matchinfo;

#if 1
        /*linux-2.6.19*/
        /*ping_zhang:20090121 START:patch from e8b to fix match mark failure*/
#ifdef CONFIG_RE8306_USE_CPUTAG
        if (skb->mark)
                return (((skb->mark & info->mask) == info->mark) ^ info->invert);
        else
                return (((skb->isVlanPack & info->mask) == info->mark) ^ info->invert);
#else
        return ((skb->mark & info->mask) == info->mark) ^ info->invert;
#endif
        /*ping_zhang:20090121 END*/
#else
        return ((skb->mark & info->mask) == info->mark) ^ info->invert;
#endif
}

static bool
mark_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_mark_mtinfo1 *info = par->matchinfo;

#if 1
        /*linux-2.6.19*/
        /*ping_zhang:20090121 START:patch from e8b to fix match mark failure*/
#ifdef CONFIG_RE8306_USE_CPUTAG
        if (skb->mark)
                return (((skb->mark & info->mask) == info->mark) ^ info->invert);
        else
                return (((skb->isVlanPack & info->mask) == info->mark) ^ info->invert);
#else
        return ((skb->mark & info->mask) == info->mark) ^ info->invert;
#endif
        /*ping_zhang:20090121 END*/
#else
        return ((skb->mark & info->mask) == info->mark) ^ info->invert;
#endif
}

static bool mark_mt_check_v0(const struct xt_mtchk_param *par)
{
	const struct xt_mark_info *minfo = par->matchinfo;

	if (minfo->mark > 0xffffffff || minfo->mask > 0xffffffff) {
		printk(KERN_WARNING "mark: only supports 32bit mark\n");
		return false;
	}
	return true;
}

#ifdef CONFIG_COMPAT
struct compat_xt_mark_info {
	compat_ulong_t	mark, mask;
	u_int8_t	invert;
	u_int8_t	__pad1;
	u_int16_t	__pad2;
};

static void mark_mt_compat_from_user_v0(void *dst, void *src)
{
	const struct compat_xt_mark_info *cm = src;
	struct xt_mark_info m = {
		.mark	= cm->mark,
		.mask	= cm->mask,
		.invert	= cm->invert,
	};
	memcpy(dst, &m, sizeof(m));
}

static int mark_mt_compat_to_user_v0(void __user *dst, void *src)
{
	const struct xt_mark_info *m = src;
	struct compat_xt_mark_info cm = {
		.mark	= m->mark,
		.mask	= m->mask,
		.invert	= m->invert,
	};
	return copy_to_user(dst, &cm, sizeof(cm)) ? -EFAULT : 0;
}
#endif /* CONFIG_COMPAT */

static struct xt_match mark_mt_reg[] __read_mostly = {
	{
		.name		= "mark",
		.revision	= 0,
		.family		= NFPROTO_UNSPEC,
		.checkentry	= mark_mt_check_v0,
		.match		= mark_mt_v0,
		.matchsize	= sizeof(struct xt_mark_info),
#ifdef CONFIG_COMPAT
		.compatsize	= sizeof(struct compat_xt_mark_info),
		.compat_from_user = mark_mt_compat_from_user_v0,
		.compat_to_user	= mark_mt_compat_to_user_v0,
#endif
#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
		.match2acl	= mark_match2acl,
#endif
		.me		= THIS_MODULE,
	},
	{
		.name           = "mark",
		.revision       = 1,
		.family         = NFPROTO_UNSPEC,
		.match          = mark_mt,
		.matchsize      = sizeof(struct xt_mark_mtinfo1),
		.me             = THIS_MODULE,
	},
};

static int __init mark_mt_init(void)
{
	return xt_register_matches(mark_mt_reg, ARRAY_SIZE(mark_mt_reg));
}

static void __exit mark_mt_exit(void)
{
	xt_unregister_matches(mark_mt_reg, ARRAY_SIZE(mark_mt_reg));
}

module_init(mark_mt_init);
module_exit(mark_mt_exit);
