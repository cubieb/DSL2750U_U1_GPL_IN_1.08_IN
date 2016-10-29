/*
 *  ebtable_aroute (after route)
 *
 *	Authors:
 *	ouyangdi(ouyangdi@twsz.com)
 *
 *  11, 2009
 *
 *  This table lets you choose after routing and bridging for frames
 *  output.
 */

#include <linux/netfilter_bridge/ebtables.h>
#include <linux/module.h>
#include <linux/if_bridge.h>

extern int (*after_route_hook) (struct sk_buff *skb);

static struct ebt_entries initial_chain = {
	.name		= "AROUTING",
	.policy		= EBT_ACCEPT,
};

static struct ebt_replace_kernel initial_table =
{
	.name		= "aroute",
	.valid_hooks	= 1 << NF_BR_AROUTING,
	.entries_size	= sizeof(struct ebt_entries),
	.hook_entry	= {
		[NF_BR_AROUTING]	= &initial_chain,
	},
	.entries	= (char *)&initial_chain,
};

static int check(const struct ebt_table_info *info, unsigned int valid_hooks)
{
	if (valid_hooks & ~(1 << NF_BR_AROUTING))
		return -EINVAL;
	return 0;
}

static struct ebt_table aroute_table =
{
	.name		= "aroute",
	.table		= &initial_table,
	.valid_hooks	= 1 << NF_BR_AROUTING,
	.lock		= RW_LOCK_UNLOCKED,
	.check		= check,
	.me		= THIS_MODULE,
};

static int ebt_aroute(struct sk_buff *skb)
{
	return ebt_do_table(NF_BR_AROUTING, skb, NULL, skb->dev,
	   dev_net(skb->dev)->xt.aroute_table);
}

static int __net_init aroute_net_init(struct net *net)
{

	net->xt.aroute_table = ebt_register_table(net, &aroute_table);
	if (IS_ERR(net->xt.aroute_table))
		return PTR_ERR(net->xt.aroute_table);
	return 0;
}

static void __net_exit aroute_net_exit(struct net *net)
{
	ebt_unregister_table(net->xt.aroute_table);
}

static struct pernet_operations aroute_net_ops = {
	.init = aroute_net_init,
	.exit = aroute_net_exit,
};

static int __init ebtable_aroute_init(void)
{
	int ret;

	ret = register_pernet_subsys(&aroute_net_ops);
	if (ret < 0)
		return ret;
    /*see dev.c*/
	rcu_assign_pointer(after_route_hook, ebt_aroute);
	return 0;
}

static void __exit ebtable_aroute_fini(void)
{
	rcu_assign_pointer(after_route_hook, NULL);
	synchronize_net();
	unregister_pernet_subsys(&aroute_net_ops);
}

module_init(ebtable_aroute_init);
module_exit(ebtable_aroute_fini);
MODULE_LICENSE("GPL");
