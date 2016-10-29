/* IPID modification target for IP tables
 * This software is distributed under the terms of GNU GPL
 */

#if defined(MODVERSIONS)
#include <linux/modversions.h>
#endif
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/checksum.h>
#include <linux/random.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ipt_IPID.h>

MODULE_AUTHOR("Ssffzz1 <ssffzz1@126.com>");
MODULE_DESCRIPTION("IP tables IPID modification module");
MODULE_LICENSE("GPL");

u_int16_t get_id(const char name []){
	u_int16_t i=0;
	u_int16_t o=0;
	while(name[i] != '\0'){
		o ^= name[i];
		i++;
	}
	return(o);
}

static unsigned int 
ipt_ipid_target(struct sk_buff *skb,
			       const struct xt_target_param * IPID_tag)
{
	struct iphdr *iph;
	const struct ipt_IPID_info *info = IPID_tag->targinfo;
	static u_int16_t new_ipid[255];
	u_int16_t id;
	
	iph = ip_hdr(skb);
	id=get_id(IPID_tag->out->name);	
	
	switch(info->mode){
		case IP_IPID_PACE:
		                  new_ipid[id] += info->ipid;
		                  break;
		case IP_IPID_CHAOTIC:
		default:
		                  get_random_bytes(&(new_ipid[id]),sizeof(new_ipid[id]));
	}
	
	iph->id = htons(new_ipid[id]);
	iph->check = 0;
	iph->check = ip_fast_csum((char *)iph,iph->ihl);	
	
	return XT_CONTINUE;
}

static int ipt_ipid_checkentry(const struct xt_tgchk_param * IPID_check)
{
	//struct ipt_IPID_info *info = targinfo;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,17))
	return 1;
#else
	if (IPID_check->target->targetsize != IPT_ALIGN(sizeof(struct ipt_IPID_info))) {
		printk(KERN_WARNING "IPID: targinfosize %u != %Zu\n",
				IPID_check->target->targetsize,
				IPT_ALIGN(sizeof(struct ipt_IPID_info)));
		return 0;	
	}	

	if (strcmp(IPID_check->target->name, "mangle")) {
		printk(KERN_WARNING "IPID: can only be called from \"mangle\" table, not \"%s\"\n", tablename);
		return 0;
	}

	return 1;
#endif
}

static struct xt_target IPID_mt_reg __read_mostly = { 
	.name = "IPID",
	.target = ipt_ipid_target, 
	.targetsize = sizeof(struct ipt_IPID_info),
	.table = "mangle",
	.checkentry = ipt_ipid_checkentry, 
	.me = THIS_MODULE, 
};

static int __init init(void)
{
	return xt_register_target(&IPID_mt_reg);
}

static void __exit fini(void)
{
	xt_unregister_target(&IPID_mt_reg);
}

module_init(init);
module_exit(fini);

