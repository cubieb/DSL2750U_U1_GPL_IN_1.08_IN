/*
 *  ebt_classify
 *
 *	Authors: XuXiaodong
 *
 *  2009-03-25
 *
 */

/* The classify target must be used in POSTROUTING chain,
   it can set skb->priority value
 */

#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_classify.h>
#include <linux/module.h>
#include <linux/netfilter/x_tables.h>


static unsigned int
ebt_classify_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
	const struct ebt_classify_target_info *info = par->targinfo;

	if (skb->priority != info->priority)
		skb->priority = info->priority;

	return info->target;
}

static bool ebt_classify_tg_check(const struct xt_tgchk_param *par)
{
	struct ebt_classify_target_info *info = par->targinfo;
	unsigned int hook_mask;

	if (BASE_CHAIN && info->target == EBT_RETURN)
		return false;
    
	hook_mask = par->hook_mask & ~(1 << NF_BR_NUMHOOKS);
	if (hook_mask & ~( (1 << NF_BR_POST_ROUTING) | (1 << NF_BR_AROUTING)))
		return false;
	if (INVALID_TARGET)
		return false;
	return true;
}

static struct xt_target ebt_classify_tg_reg __read_mostly = {
	.name		= EBT_CLASSIFY_TARGET,
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.hooks		= (1 << NF_BR_NUMHOOKS) | (1 << NF_BR_POST_ROUTING) |
			      (1 << NF_BR_AROUTING),
	.target		= ebt_classify_tg,
	.checkentry	= ebt_classify_tg_check,
	.targetsize	= XT_ALIGN(sizeof(struct ebt_classify_target_info)),
	.me		= THIS_MODULE,
};


static int __init ebt_classify_init(void)
{
	return xt_register_target(&ebt_classify_tg_reg);
}

static void __exit ebt_classify_fini(void)
{
	xt_unregister_target(&ebt_classify_tg_reg);
}

module_init(ebt_classify_init);
module_exit(ebt_classify_fini);
MODULE_LICENSE("GPL");

