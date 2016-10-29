/*
 *  ebtable_proute.c
 *
 *	Authors:
 *	XuXiaodong  borrowed heavily from ebtable_broute.c
 *
 *  2009-03-24
 *
 *  This table lets you match packets and do target before any other netfilter tables.
 *  See net/bridge/br_input.c.
 */

#include <linux/netfilter_bridge/ebtables.h>
#include <linux/module.h>
#include <linux/if_bridge.h>

extern int (*pioneer_hook_func) (struct sk_buff *skb);

static struct ebt_entries initial_chain = {
	.name		= "PROUTING",
	.policy		= EBT_ACCEPT,
};

static struct ebt_replace_kernel initial_table =
{
	.name		= "proute",
	.valid_hooks	= 1 << NF_BR_PROUTING,
	.entries_size	= sizeof(struct ebt_entries),
	.hook_entry	= {
		[NF_BR_PROUTING]	= &initial_chain,
	},
	.entries	= (char *)&initial_chain,
};

static int check(const struct ebt_table_info *info, unsigned int valid_hooks)
{
	if (valid_hooks & ~(1 << NF_BR_PROUTING))
		return -EINVAL;
	return 0;
}

static struct ebt_table proute_table =
{
	.name		= "proute",
	.table		= &initial_table,
	.valid_hooks	= 1 << NF_BR_PROUTING,
	.lock		= RW_LOCK_UNLOCKED,
	.check		= check,
	.me		= THIS_MODULE,
};

static int ebt_proute(struct sk_buff *skb)
{
	return ebt_do_table(NF_BR_PROUTING, skb, skb->dev, NULL, 
        dev_net(skb->dev)->xt.proute_table);
}

static int __net_init proute_net_init(struct net *net)
{
	net->xt.proute_table = ebt_register_table(net, &proute_table);
	if (IS_ERR(net->xt.proute_table))
		return PTR_ERR(net->xt.proute_table);
	return 0;
}

static void __net_exit proute_net_exit(struct net *net)
{
	ebt_unregister_table(net->xt.proute_table);
}

static struct pernet_operations proute_net_ops = {
	.init = proute_net_init,
	.exit = proute_net_exit,
};

static int __init ebtable_proute_init(void)
{
	int ret;
    
	ret = register_pernet_subsys(&proute_net_ops);
	if (ret < 0)
		return ret;
    
	rcu_assign_pointer(pioneer_hook_func, ebt_proute);
	return 0;
}

static void __exit ebtable_proute_fini(void)
{
	rcu_assign_pointer(pioneer_hook_func, NULL);
	synchronize_net();
	unregister_pernet_subsys(&proute_net_ops);
}

module_init(ebtable_proute_init);
module_exit(ebtable_proute_fini);
MODULE_LICENSE("GPL");
