/*
 * Packet matching code.
 *
 * Copyright (C) 1999 Paul `Rusty' Russell & Michael J. Neuling
 * Copyright (C) 2000-2005 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/cache.h>
#include <linux/capability.h>
#include <linux/skbuff.h>
#include <linux/kmod.h>
#include <linux/vmalloc.h>
#include <linux/netdevice.h>
#include <linux/module.h>
#include <linux/icmp.h>
#include <net/ip.h>
#include <net/compat.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/err.h>
#include <linux/cpumask.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <net/netfilter/nf_log.h>

#ifdef CONFIG_RTL_HARDWARE_NAT 
/*2007-12-19*/
#include <linux/inetdevice.h>
#include <net/rtl/rtl_types.h>
#include "../../../drivers/net/rtl819x/common/rtl_errno.h" 
#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
#include <net/rtl/rtl865x_ip_api.h>
#endif
#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
//#include "l4Driver/rtl865x_nat.h"
#endif
#endif
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl865x_outputQueue.h>
#endif

#include "autoconf.h"
#ifdef CONFIG_APPS_LOGIC_FIREWALLLOG
#include <linux/mm.h>
#endif


#define	RTL865X_QOS_TABLE_NAME		"mangle"
#define	RTL865X_QOS_TABLE_LEN			6


#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
#include <net/rtl/rtl865x_netif.h>
#include "../../bridge/br_private.h"
#ifdef CONFIG_RTL8676_Dynamic_ACL
#include "../../../drivers/net/rtl819x/common/rtl_errno.h"
#include <net/rtl/rtl865x_arp_api.h>
#endif
#endif

#ifdef CONFIG_RTL_MULTI_ETH_WAN
#include <net/rtl/rtl_queue.h>

#include "../../../drivers/net/rtl819x/common/mbuf.h"
//#include "assert.h"

//#include "rtl865xc_swNic.h"
#ifdef CONFIG_RTL_LAYERED_ASIC_DRIVER
#include "../../../drivers/net/rtl819x/AsicDriver/rtl865x_asicCom.h"
#include "../../../drivers/net/rtl819x/AsicDriver/rtl865x_asicL2.h"
#include "../../../drivers/net/rtl819x/AsicDriver/rtl865x_asicL3.h"
#else
#include "rtl865xC_tblAsicDrv.h"
#include "rtl8651_aclLocal.h"
#endif

#include "../../../drivers/net/rtl819x/common/rtl865x_vlan.h"
#include "../../../drivers/net/rtl819x/common/rtl865x_netif_local.h"
#include "../../../drivers/net/rtl819x/l2Driver/rtl865x_fdb.h"
#include "../../../drivers/net/rtl819x/l3Driver/rtl865x_ppp_local.h"
#include "../../../drivers/net/rtl819x/l3Driver/rtl865x_route.h"
#include "../../../drivers/net/rtl819x/l3Driver/rtl865x_ip.h"
#include "../../../drivers/net/rtl819x/l3Driver/rtl865x_nexthop.h"
#endif
#ifdef CONFIG_APPS_LOGIC_FIREWALLLOG
static struct proc_dir_entry *proc_memshare_dir ;
static unsigned long kernel_memaddr = 0;
static unsigned long kernel_memsize= 0;
//static sem_t*   sFirewalllog_mutex = NULL;

static unsigned short  writerPointer = 0;
static unsigned short  readPointer = 0;
static int  writerPassRead = 0;


static ST_LOGINFO stLoginfo =
{
    .jiffies = 0,
    .saddr = 0,
    .daddr = 0,
   .protocol = 0,
   .szDev = {0},
};
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("IPv4 packet filter");

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
typedef struct xt_rule_to_acl_s
{
	struct list_head list;
	char iniface[IFNAMSIZ], outiface[IFNAMSIZ];
	rtl865x_AclRule_t *aclRule;	
} xt_rule_to_acl_t;

LIST_HEAD(rtl865x_iptRule2Acl_tbl_list);
LIST_HEAD(rtl865x_iptRule2Acl_def_rule_list);


static unsigned int 	acl_mode = RTL865X_ACL_Mode_Normal;
#if defined (CONFIG_RTL_IPTABLES_RULE_2_ACL) && !defined(CONFIG_RTL8676_Dynamic_ACL)
static unsigned int 	mac_filter_mode = RTL865X_ACL_MAC_FILTER_ALL_PERMIT;
#endif
#define				in_def_action 	(acl_mode==RTL865X_ACL_Mode_Always_Permit) ? RTL865X_ACL_PERMIT : RTL865X_ACL_TOCPU
#define				out_def_action 	RTL865X_ACL_PERMIT
//static unsigned int in_def_action = RTL865X_ACL_PERMIT;
//static unsigned int out_def_action = RTL865X_ACL_PERMIT;
int establish_rule_permit = 0;
#endif

#if defined(CONFIG_ETHWAN) && defined(CONFIG_RTL_IPTABLES_FAST_PATH)
extern unsigned int heavyTraffic;
#endif
/*#define DEBUG_IP_FIREWALL*/
/*#define DEBUG_ALLOW_ALL*/ /* Useful for remote debugging */
/*#define DEBUG_IP_FIREWALL_USER*/
//#define CONFIG_HARDWARE_NAT_DEBUG

#ifdef DEBUG_IP_FIREWALL
#define dprintf(format, args...)  printk(format , ## args)
#else
#define dprintf(format, args...)
#endif

#ifdef DEBUG_IP_FIREWALL_USER
#define duprintf(format, args...) printk(format , ## args)
#else
#define duprintf(format, args...)
#endif

#ifdef CONFIG_NETFILTER_DEBUG
#define IP_NF_ASSERT(x)						\
do {								\
	if (!(x))						\
		printk("IP_NF_ASSERT: %s:%s:%u\n",		\
		       __func__, __FILE__, __LINE__);	\
} while(0)
#else
#define IP_NF_ASSERT(x)
#endif

#if 0
/* All the better to debug you with... */
#define static
#define inline
#endif

#ifdef CONFIG_RTL_HARDWARE_NAT  
/*2007-12-19*/
#ifndef CONFIG_RTL_MULTI_ETH_WAN
char masq_if[IFNAMSIZ] = { '\0' };
unsigned int hw_napt_ip = 0;
struct ipt_entry *masq_entry = NULL;
#else
char masq_if[IP_NUMBER][IFNAMSIZ] = { "\0" };
unsigned int hw_napt_ip[IP_NUMBER] = {0};
struct ipt_entry *masq_entry[IP_NUMBER] = {NULL};
static unsigned int masq_index=0;
#endif
#endif



/*linux-2.6.19*/
/*patch from linux 2.4*/
#if defined(CONFIG_BRIDGE) || defined (CONFIG_BRIDGE_MODULE)
//#ifndef CONFIG_BRIDGE_NF_EBTABLES
	/* 
	 * only if not defined CONFIG_BRIDGE_NF_EBTABLES, 
	 * we should need this function to filter the phy dev in the br
	 * because you can use ebtables, not iptables
	 */
#define	CONFIG_NF_HOOK_BRIDGE_PHYDEV	1
#define CONFIG_NF_HOOK_SWITCH_PORT	1
//#endif //CONFIG_BRIDGE_NF_EBTABLES
#endif //defined(CONFIG_BRIDGE) || defined (CONFIG_BRIDGE_MODULE)



/*
   We keep a set of rules for each CPU, so we can avoid write-locking
   them in the softirq when updating the counters and therefore
   only need to read-lock in the softirq; doing a write_lock_bh() in user
   context stops packets coming through and allows user context to read
   the counters or update the rules.

   Hence the start of any table is given by get_table() below.  */

/* Returns whether matches rule or not. */
/* Performance critical - called for every packet */
static inline bool
ip_packet_match(const struct iphdr *ip,
		const char *indev,
		const char *outdev,
#ifdef CONFIG_NF_HOOK_BRIDGE_PHYDEV
		const char *physindev,
		const char *physoutdev,
#endif //CONFIG_NF_HOOK_BRIDGE_PHYDEV
#ifdef CONFIG_NF_HOOK_SWITCH_PORT
		char *switch_port,
#endif //CONFIG_NF_HOOK_SWITCH_PORT
		const struct ipt_ip *ipinfo,
		int isfrag)
{
	unsigned long ret;
#ifdef CONFIG_NF_HOOK_BRIDGE_PHYDEV
	unsigned long ret2 = 1; // ebtables
#endif //CONFIG_NF_HOOK_BRIDGE_PHYDEV

#define FWINV(bool, invflg) ((bool) ^ !!(ipinfo->invflags & (invflg)))

	if (FWINV((ip->saddr&ipinfo->smsk.s_addr) != ipinfo->src.s_addr,
		  IPT_INV_SRCIP)
	    || FWINV((ip->daddr&ipinfo->dmsk.s_addr) != ipinfo->dst.s_addr,
		     IPT_INV_DSTIP)) {
		dprintf("Source or dest mismatch.\n");

		dprintf("SRC: %pI4. Mask: %pI4. Target: %pI4.%s\n",
			&ip->saddr, &ipinfo->smsk.s_addr, &ipinfo->src.s_addr,
			ipinfo->invflags & IPT_INV_SRCIP ? " (INV)" : "");
		dprintf("DST: %pI4 Mask: %pI4 Target: %pI4.%s\n",
			&ip->daddr, &ipinfo->dmsk.s_addr, &ipinfo->dst.s_addr,
			ipinfo->invflags & IPT_INV_DSTIP ? " (INV)" : "");
		return false;
	}

	ret = ifname_compare_aligned(indev, ipinfo->iniface, ipinfo->iniface_mask);

#ifdef CONFIG_NF_HOOK_BRIDGE_PHYDEV
    #if 1
	ret2 = ifname_compare_aligned(physindev, ipinfo->iniface, ipinfo->iniface_mask);
    #else
	for (i = 0, ret2 = 0; i < IFNAMSIZ/sizeof(unsigned long); i++) {
		ret2 |= (((const unsigned long *)physindev)[i]
			^ ((const unsigned long *)ipinfo->iniface)[i])
			& ((const unsigned long *)ipinfo->iniface_mask)[i];
	}
    #endif
#endif //CONFIG_NF_HOOK_BRIDGE_PHYDEV

#ifdef CONFIG_NF_HOOK_SWITCH_PORT
	// Modified by Mason Yu. Allow switch port name(eth0_br0) to fit iptables rule.
	// Kaohj
	if (switch_port == NULL) 
		switch_port = "";
#endif //CONFIG_NF_HOOK_SWITCH_PORT

	if (
#ifdef CONFIG_NF_HOOK_BRIDGE_PHYDEV
		FWINV(	ret != 0 && ret2 != 0 , IPT_INV_VIA_IN)
#else
		FWINV(ret != 0, IPT_INV_VIA_IN)
#endif //CONFIG_NF_HOOK_BRIDGE_PHYDEV
#ifdef CONFIG_NF_HOOK_SWITCH_PORT
	      && (strcmp(switch_port, ipinfo->iniface)!=0)
#endif //CONFIG_NF_HOOK_SWITCH_PORT
	) {
		dprintf("VIA in mismatch (%s vs %s).%s\n",
			indev, ipinfo->iniface,
			ipinfo->invflags&IPT_INV_VIA_IN ?" (INV)":"");
		return false;
	}

	ret = ifname_compare_aligned(outdev, ipinfo->outiface, ipinfo->outiface_mask);

#ifdef CONFIG_NF_HOOK_BRIDGE_PHYDEV
  #if 1
	ret2 = ifname_compare_aligned(physoutdev, ipinfo->outiface, ipinfo->outiface_mask);
  #else
	for (i = 0, ret2 = 0; i < IFNAMSIZ/sizeof(unsigned long); i++) {
		ret2 |= (((const unsigned long *)physoutdev)[i]
			^ ((const unsigned long *)ipinfo->outiface)[i])
			& ((const unsigned long *)ipinfo->outiface_mask)[i];
	}
  #endif
#endif //CONFIG_NF_HOOK_BRIDGE_PHYDEV

	if (
#ifdef CONFIG_NF_HOOK_BRIDGE_PHYDEV
		FWINV(	ret != 0 && ret2 != 0, IPT_INV_VIA_OUT)
#else
		FWINV(ret != 0, IPT_INV_VIA_OUT)
#endif //CONFIG_NF_HOOK_BRIDGE_PHYDEV
	) {
		dprintf("VIA out mismatch (%s vs %s).%s\n",
			outdev, ipinfo->outiface,
			ipinfo->invflags&IPT_INV_VIA_OUT ?" (INV)":"");
		return false;
	}

	/* Check specific protocol */
	if (ipinfo->proto
	    && FWINV(ip->protocol != ipinfo->proto, IPT_INV_PROTO)) {
		dprintf("Packet protocol %hi does not match %hi.%s\n",
			ip->protocol, ipinfo->proto,
			ipinfo->invflags&IPT_INV_PROTO ? " (INV)":"");
		return false;
	}

	/* If we have a fragment rule but the packet is not a fragment
	 * then we return zero */
	if (FWINV((ipinfo->flags&IPT_F_FRAG) && !isfrag, IPT_INV_FRAG)) {
		dprintf("Fragment rule but not fragment.%s\n",
			ipinfo->invflags & IPT_INV_FRAG ? " (INV)" : "");
		return false;
	}

	return true;
}

static bool
ip_checkentry(const struct ipt_ip *ip)
{
	if (ip->flags & ~IPT_F_MASK) {
		duprintf("Unknown flag bits set: %08X\n",
			 ip->flags & ~IPT_F_MASK);
		return false;
	}
	if (ip->invflags & ~IPT_INV_MASK) {
		duprintf("Unknown invflag bits set: %08X\n",
			 ip->invflags & ~IPT_INV_MASK);
		return false;
	}
	return true;
}

static unsigned int
ipt_error(struct sk_buff *skb, const struct xt_target_param *par)
{
	if (net_ratelimit())
		printk("ip_tables: error: `%s'\n",
		       (const char *)par->targinfo);

	return NF_DROP;
}

/* Performance critical - called for every packet */
static inline bool
do_match(struct ipt_entry_match *m, const struct sk_buff *skb,
	 struct xt_match_param *par)
{
	par->match     = m->u.kernel.match;
	par->matchinfo = m->data;

	/* Stop iteration if it doesn't match */
	if (!m->u.kernel.match->match(skb, par))
		return true;
	else
		return false;
}

/* Performance critical */
static inline struct ipt_entry *
get_entry(void *base, unsigned int offset)
{
	return (struct ipt_entry *)(base + offset);
}

#ifdef CONFIG_APPS_LOGIC_FIREWALLLOG
static inline void 
doWriteLogInfo(struct sk_buff *skb)
{
	int cpyIndex;
/*****************************************************************
环形链头信息表示的意义:　

0 1 2 3 4 5 6 7 8 9 a b  -----
0-3字节存储最新的时间戳
4-5字节存储读指针
6-7字节存储写指针
8-b字节存储写指针越过读指针的次数

*****************************************************************/

		struct timeval tv;
		do_gettimeofday(&tv);
		
		stLoginfo.jiffies = tv.tv_sec;
		stLoginfo.daddr = ip_hdr(skb)->daddr;
		stLoginfo.saddr = ip_hdr(skb)->saddr;
		stLoginfo.protocol= ip_hdr(skb)->protocol;

		if(NULL != skb->switch_port )
			strcpy(stLoginfo.szDev, skb->switch_port );
		else
			memset(stLoginfo.szDev,0,DEVMAXSIZE);
//printk("====== time=%lu,sIP=%u,dIP=%u,protocol=%u,dev=%s\n",stLoginfo.jiffies,stLoginfo.saddr,stLoginfo.daddr,stLoginfo.protocol,stLoginfo.szDev);
		
		memcpy((unsigned char *) kernel_memaddr,&(tv.tv_sec),sizeof(tv.tv_sec));
		
		cpyIndex = IPT_LOG_INFO_START_POINT+writerPointer * ST_LOGINFO_SIZE;

		memcpy((unsigned char *) (kernel_memaddr+cpyIndex), &stLoginfo,ST_LOGINFO_SIZE);
		
		writerPointer++;

		memcpy(&readPointer, (unsigned char *)(kernel_memaddr + IPT_LOG_READPOINT_ADDR),sizeof(unsigned short));
			
		if(writerPointer == readPointer)
		{
			writerPassRead++;
			memcpy((unsigned char *)( kernel_memaddr + IPT_LOG_WPASSR_ADDR),&writerPassRead,sizeof(int));
		}
			
		if(writerPointer >= kernel_memsize/ST_LOGINFO_SIZE -1)
		     writerPointer = 0;
		memcpy((unsigned char *)( kernel_memaddr + IPT_LOG_WRITERPOINT_ADDR),&writerPointer,sizeof(unsigned short));
	
	return ;
}

#endif

/* All zeroes == unconditional rule. */
/* Mildly perf critical (only if packet tracing is on) */
static inline int
unconditional(const struct ipt_ip *ip)
{
	unsigned int i;

	for (i = 0; i < sizeof(*ip)/sizeof(__u32); i++)
		if (((__u32 *)ip)[i])
			return 0;

	return 1;
#undef FWINV
}

#if defined(CONFIG_NETFILTER_XT_TARGET_TRACE) || \
    defined(CONFIG_NETFILTER_XT_TARGET_TRACE_MODULE)
static const char *const hooknames[] = {
	[NF_INET_PRE_ROUTING]		= "PREROUTING",
	[NF_INET_LOCAL_IN]		= "INPUT",
	[NF_INET_FORWARD]		= "FORWARD",
	[NF_INET_LOCAL_OUT]		= "OUTPUT",
	[NF_INET_POST_ROUTING]		= "POSTROUTING",
};

enum nf_ip_trace_comments {
	NF_IP_TRACE_COMMENT_RULE,
	NF_IP_TRACE_COMMENT_RETURN,
	NF_IP_TRACE_COMMENT_POLICY,
};

static const char *const comments[] = {
	[NF_IP_TRACE_COMMENT_RULE]	= "rule",
	[NF_IP_TRACE_COMMENT_RETURN]	= "return",
	[NF_IP_TRACE_COMMENT_POLICY]	= "policy",
};

static struct nf_loginfo trace_loginfo = {
	.type = NF_LOG_TYPE_LOG,
	.u = {
		.log = {
			.level = 4,
			.logflags = NF_LOG_MASK,
		},
	},
};

/* Mildly perf critical (only if packet tracing is on) */
static inline int
get_chainname_rulenum(struct ipt_entry *s, struct ipt_entry *e,
		      char *hookname, char **chainname,
		      char **comment, unsigned int *rulenum)
{
	struct ipt_standard_target *t = (void *)ipt_get_target(s);

	if (strcmp(t->target.u.kernel.target->name, IPT_ERROR_TARGET) == 0) {
		/* Head of user chain: ERROR target with chainname */
		*chainname = t->target.data;
		(*rulenum) = 0;
	} else if (s == e) {
		(*rulenum)++;

		if (s->target_offset == sizeof(struct ipt_entry)
		   && strcmp(t->target.u.kernel.target->name,
			     IPT_STANDARD_TARGET) == 0
		   && t->verdict < 0
		   && unconditional(&s->ip)) {
			/* Tail of chains: STANDARD target (return/policy) */
			*comment = *chainname == hookname
				? (char *)comments[NF_IP_TRACE_COMMENT_POLICY]
				: (char *)comments[NF_IP_TRACE_COMMENT_RETURN];
		}
		return 1;
	} else
		(*rulenum)++;

	return 0;
}

static void trace_packet(struct sk_buff *skb,
			 unsigned int hook,
			 const struct net_device *in,
			 const struct net_device *out,
			 const char *tablename,
			 struct xt_table_info *private,
			 struct ipt_entry *e)
{
	void *table_base;
	const struct ipt_entry *root;
	char *hookname, *chainname, *comment;
	unsigned int rulenum = 0;

	table_base = (void *)private->entries[smp_processor_id()];
	root = get_entry(table_base, private->hook_entry[hook]);

	hookname = chainname = (char *)hooknames[hook];
	comment = (char *)comments[NF_IP_TRACE_COMMENT_RULE];

	IPT_ENTRY_ITERATE(root,
			  private->size - private->hook_entry[hook],
			  get_chainname_rulenum,
			  e, hookname, &chainname, &comment, &rulenum);

	nf_log_packet(AF_INET, hook, skb, in, out, &trace_loginfo,
		      "TRACE: %s:%s:%s:%u ",
		      tablename, chainname, comment, rulenum);
}
#endif

/* Returns one of the generic firewall policies, like NF_ACCEPT. */
unsigned int
ipt_do_table(struct sk_buff *skb,
	     unsigned int hook,
	     const struct net_device *in,
	     const struct net_device *out,
	     struct xt_table *table)
{
	static const char nulldevname[IFNAMSIZ] __attribute__((aligned(sizeof(long))));
	const struct iphdr *ip;
	u_int16_t datalen;
	bool hotdrop = false;
	/* Initializing verdict to NF_DROP keeps gcc happy. */
	unsigned int verdict = NF_DROP;
	const char *indev, *outdev;
	void *table_base;
	struct ipt_entry *e, *back;
	struct xt_table_info *private;
	struct xt_match_param mtpar;
	struct xt_target_param tgpar;
#ifdef CONFIG_NF_HOOK_BRIDGE_PHYDEV
	const char *physindev, *physoutdev;
#endif //CONFIG_NF_HOOK_BRIDGE_PHYDEV

	/* Initialization */
	ip = ip_hdr(skb);
	datalen = skb->len - ip->ihl * 4;
	indev = in ? in->name : nulldevname;
	outdev = out ? out->name : nulldevname;
#ifdef CONFIG_NF_HOOK_BRIDGE_PHYDEV
	physindev = (skb->nf_bridge && skb->nf_bridge->physindev) ?
		skb->nf_bridge->physindev->name : nulldevname;
	physoutdev = (skb->nf_bridge && skb->nf_bridge->physoutdev) ?
		skb->nf_bridge->physoutdev->name : nulldevname;
#endif //CONFIG_NF_HOOK_BRIDGE_PHYDEV

	/* We handle fragments by dealing with the first fragment as
	 * if it was a normal packet.  All other fragments are treated
	 * normally, except that they will NEVER match rules that ask
	 * things we don't know, ie. tcp syn flag or ports).  If the
	 * rule is also a fragment-specific rule, non-fragments won't
	 * match it. */
	mtpar.fragoff = ntohs(ip->frag_off) & IP_OFFSET;
	mtpar.thoff   = ip_hdrlen(skb);
	mtpar.hotdrop = &hotdrop;
	mtpar.in      = tgpar.in  = in;
	mtpar.out     = tgpar.out = out;
	mtpar.family  = tgpar.family = NFPROTO_IPV4;
	tgpar.hooknum = hook;

	IP_NF_ASSERT(table->valid_hooks & (1 << hook));
	xt_info_rdlock_bh();
	private = table->private;
	table_base = private->entries[smp_processor_id()];

	e = get_entry(table_base, private->hook_entry[hook]);

	/* For return from builtin chain */
	back = get_entry(table_base, private->underflow[hook]);
	
	if(ip == NULL)
	{	
		//printk("==========%s %s(%d), ip header is NULL!!!\n",__FILE__,__FUNCTION__,__LINE__);
		return NF_DROP;
	}

	do {
		IP_NF_ASSERT(e);
		IP_NF_ASSERT(back);
		
		if(e == NULL)
		{
			//printk("==========%s %s(%d), ip header is NULL!!!\n",__FILE__,__FUNCTION__,__LINE__);
			return NF_DROP;
		}
		
		// Modified by Mason Yu. Allow switch port name to iptables's rule.
		if (ip_packet_match(ip, indev, outdev,
#ifdef CONFIG_NF_HOOK_BRIDGE_PHYDEV
					physindev,
					physoutdev,
#endif //CONFIG_NF_HOOK_BRIDGE_PHYDEV
#ifdef CONFIG_NF_HOOK_SWITCH_PORT
					skb->switch_port,
#endif //#ifdef CONFIG_NF_HOOK_SWITCH_PORT
					&e->ip, mtpar.fragoff)) {
			struct ipt_entry_target *t;

			if (IPT_MATCH_ITERATE(e, do_match, skb, &mtpar) != 0)
				goto no_match;

			ADD_COUNTER(e->counters, ntohs(ip->tot_len), 1);

			t = ipt_get_target(e);
			IP_NF_ASSERT(t->u.kernel.target);

#if defined(CONFIG_NETFILTER_XT_TARGET_TRACE) || \
    defined(CONFIG_NETFILTER_XT_TARGET_TRACE_MODULE)
			/* The packet is traced: log it */
			if (unlikely(skb->nf_trace))
				trace_packet(skb, hook, in, out,
					     table->name, private, e);
#endif
			/* Standard target? */
			if (!t->u.kernel.target->target) {
				int v;

				v = ((struct ipt_standard_target *)t)->verdict;
				if (v < 0) {
					/* Pop from stack? */
					if (v != IPT_RETURN) {
						verdict = (unsigned)(-v) - 1;
						break;
					}
					e = back;
					back = get_entry(table_base,
							 back->comefrom);
					continue;
				}
				if (table_base + v != (void *)e + e->next_offset
				    && !(e->ip.flags & IPT_F_GOTO)) {
					/* Save old back ptr in next entry */
					struct ipt_entry *next
						= (void *)e + e->next_offset;
					next->comefrom
						= (void *)back - table_base;
					/* set back pointer to next entry */
					back = next;
				}

				e = get_entry(table_base, v);
			} else {
				/* Targets which reenter must return
				   abs. verdicts */
				tgpar.target   = t->u.kernel.target;
				tgpar.targinfo = t->data;
#ifdef CONFIG_NETFILTER_DEBUG
				((struct ipt_entry *)table_base)->comefrom
					= 0xeeeeeeec;
#endif
				verdict = t->u.kernel.target->target(skb,
								     &tgpar);
#ifdef CONFIG_NETFILTER_DEBUG
				if (((struct ipt_entry *)table_base)->comefrom
				    != 0xeeeeeeec
				    && verdict == IPT_CONTINUE) {
					printk("Target %s reentered!\n",
					       t->u.kernel.target->name);
					verdict = NF_DROP;
				}
				((struct ipt_entry *)table_base)->comefrom
					= 0x57acc001;
#endif
				/* Target might have changed stuff. */
				ip = ip_hdr(skb);
				datalen = skb->len - ip->ihl * 4;

				if (verdict == IPT_CONTINUE)
					e = (void *)e + e->next_offset;
				else
					/* Verdict */
					break;
			}
		} else {

		no_match:
			e = (void *)e + e->next_offset;
		}
	} while (!hotdrop);
	xt_info_rdunlock_bh();

#ifdef DEBUG_ALLOW_ALL
	return NF_ACCEPT;
#else
	if (hotdrop)
    {   
#ifdef CONFIG_APPS_LOGIC_FIREWALLLOG
        doWriteLogInfo(skb);
#endif		
		return NF_DROP;
    }
	else
    {
#ifdef CONFIG_APPS_LOGIC_FIREWALLLOG
        if(NF_DROP == verdict)
        {
            doWriteLogInfo(skb);
        }
#endif        
        return verdict;
    }
#endif
}

/* Figures out from what hook each rule can be called: returns 0 if
   there are loops.  Puts hook bitmask in comefrom. */
static int
mark_source_chains(struct xt_table_info *newinfo,
		   unsigned int valid_hooks, void *entry0)
{
	unsigned int hook;

#if 0 //def CONFIG_RTL_HARDWARE_NAT  
	/*2007-12-19*/
	unsigned int masqif_set = 0;
#endif


	/* No recursion; use packet counter to save back ptrs (reset
	   to 0 as we leave), and comefrom to save source hook bitmask */
	for (hook = 0; hook < NF_INET_NUMHOOKS; hook++) {
		unsigned int pos = newinfo->hook_entry[hook];
		struct ipt_entry *e = (struct ipt_entry *)(entry0 + pos);

		if (!(valid_hooks & (1 << hook)))
			continue;

		/* Set initial back pointer. */
		e->counters.pcnt = pos;

		for (;;) {
			struct ipt_standard_target *t
				= (void *)ipt_get_target(e);
			int visited = e->comefrom & (1 << hook);

			if (e->comefrom & (1 << NF_INET_NUMHOOKS)) {
				printk("iptables: loop hook %u pos %u %08X.\n",
				       hook, pos, e->comefrom);
				return 0;
			}
			e->comefrom |= ((1 << hook) | (1 << NF_INET_NUMHOOKS));

			/* Unconditional return/END. */
			if ((e->target_offset == sizeof(struct ipt_entry)
			    && (strcmp(t->target.u.user.name,
				       IPT_STANDARD_TARGET) == 0)
			    && t->verdict < 0
			    && unconditional(&e->ip)) || visited) {
				unsigned int oldpos, size;

				if ((strcmp(t->target.u.user.name,
			    		    IPT_STANDARD_TARGET) == 0) &&
				    t->verdict < -NF_MAX_VERDICT - 1) {
					duprintf("mark_source_chains: bad "
						"negative verdict (%i)\n",
								t->verdict);
					return 0;
				}

				/* Return: backtrack through the last
				   big jump. */
				do {
					e->comefrom ^= (1<<NF_INET_NUMHOOKS);
#ifdef DEBUG_IP_FIREWALL_USER
					if (e->comefrom
					    & (1 << NF_INET_NUMHOOKS)) {
						duprintf("Back unset "
							 "on hook %u "
							 "rule %u\n",
							 hook, pos);
					}
#endif
					oldpos = pos;
					pos = e->counters.pcnt;
					e->counters.pcnt = 0;

					/* We're at the start. */
					if (pos == oldpos)
						goto next;

					e = (struct ipt_entry *)
						(entry0 + pos);
				} while (oldpos == pos + e->next_offset);

				/* Move along one */
				size = e->next_offset;
				e = (struct ipt_entry *)
					(entry0 + pos + size);
				e->counters.pcnt = pos;
				pos += size;
			} else {
				int newpos = t->verdict;

				if (strcmp(t->target.u.user.name,
					   IPT_STANDARD_TARGET) == 0
				    && newpos >= 0) {
					if (newpos > newinfo->size -
						sizeof(struct ipt_entry)) {
						duprintf("mark_source_chains: "
							"bad verdict (%i)\n",
								newpos);
						return 0;
					}
					/* This a jump; chase it. */
					duprintf("Jump rule %u -> %u\n",
						 pos, newpos);
				} else {
					/* ... this is a fallthru */
					newpos = pos + e->next_offset;
				}

				
#if 0 // defined(CONFIG_RTL_HARDWARE_NAT)
				/*2007-12-19*/
#ifdef CONFIG_HARDWARE_NAT_DEBUG
				{
					printk("%s:%d:masq_if is %s\n",__FUNCTION__,__LINE__,masq_if);
					printk("%s:%d:hook is %d\n",__FUNCTION__,__LINE__,hook);
					printk("%s:%d:t->target.u.user.name is %s\n",__FUNCTION__,__LINE__,t->target.u.user.name);
					//printk("%s:%d:t->target.u.kernel.target->name is %s\n",__FUNCTION__,__LINE__,t->target.u.kernel.target->name);
				}
#endif
				if ((masq_if[0] == '\0') && (hook == NF_INET_POST_ROUTING) &&
					((strcmp(t->target.u.user.name, "MASQUERADE") == 0)	)) 
				{
					struct net_device *dev;
					struct in_ifaddr *ina;
					int rc;
					masqif_set = 1;
					memcpy(masq_if, e->ip.outiface, IFNAMSIZ);
					dev = __dev_get_by_name(&init_net, masq_if);
					
					#ifdef CONFIG_HARDWARE_NAT_DEBUG
					/*2007-12-19*/
					printk("%s:%d:masq_if is %s\n",__FUNCTION__,__LINE__,masq_if);
					#endif

					if ((dev)&&(dev->ip_ptr)) {			
						
						ina=(struct in_ifaddr *)(((struct in_device *)(dev->ip_ptr))->ifa_list);						
						if (ina!=NULL) 
						{
							if (hw_napt_ip) {
								rc = 0;
			#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
								rc = rtl865x_delIp(hw_napt_ip);
			#endif
								#ifdef CONFIG_HARDWARE_NAT_DEBUG
								printk("LR(%s): 0x%x, errno=%d\n", "del_natip2", hw_napt_ip, rc);
								#endif
								if (rc == SUCCESS)
									hw_napt_ip = 0;
							}
							rc = 0;
			#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
							//printk("%s:%d:masq_if is %s\n",__FUNCTION__,__LINE__,masq_if);
							//printk("---------------ip(%u.%u.%u.%u)\n",NIPQUAD((u32)(ina->ifa_local)));
							rc = rtl865x_addIp(0,(u32)(ina->ifa_local),IP_TYPE_NAPT);
			#endif							

							if (rc == SUCCESS)
								hw_napt_ip = ina->ifa_local;
							#ifdef CONFIG_HARDWARE_NAT_DEBUG
							/*2007-12-19*/
							printk("%s:%d:(%s): %u.%u.%u.%u, errno=%d\n", __FUNCTION__,__LINE__,"add_natip",NIPQUAD((u32)(ina->ifa_local)), rc);
							#endif
						}
					}
				}

#endif							

				e = (struct ipt_entry *)
					(entry0 + newpos);
				e->counters.pcnt = pos;
				pos = newpos;
			}
		}
		next:
		duprintf("Finished chain %u\n", hook);
#if 0 //def CONFIG_RTL_HARDWARE_NAT
		/*2007-12-19*/
		if (masqif_set == 0 && masq_if[0] != '\0' && hook==NF_INET_POST_ROUTING) {
			masq_if[0] = '\0';
		}
#endif	

	}
	return 1;
}

static int
cleanup_match(struct ipt_entry_match *m, unsigned int *i)
{
	struct xt_mtdtor_param par;

	if (i && (*i)-- == 0)
		return 1;

	par.match     = m->u.kernel.match;
	par.matchinfo = m->data;
	par.family    = NFPROTO_IPV4;
	if (par.match->destroy != NULL)
		par.match->destroy(&par);
	module_put(par.match->me);
	return 0;
}

static int
check_entry(struct ipt_entry *e, const char *name)
{
	struct ipt_entry_target *t;

	if (!ip_checkentry(&e->ip)) {
		duprintf("ip_tables: ip check failed %p %s.\n", e, name);
		return -EINVAL;
	}

	if (e->target_offset + sizeof(struct ipt_entry_target) >
	    e->next_offset)
		return -EINVAL;

	t = ipt_get_target(e);
	if (e->target_offset + t->u.target_size > e->next_offset)
		return -EINVAL;

	return 0;
}

static int
check_match(struct ipt_entry_match *m, struct xt_mtchk_param *par,
	    unsigned int *i)
{
	const struct ipt_ip *ip = par->entryinfo;
	int ret;

	par->match     = m->u.kernel.match;
	par->matchinfo = m->data;

	ret = xt_check_match(par, m->u.match_size - sizeof(*m),
	      ip->proto, ip->invflags & IPT_INV_PROTO);
	if (ret < 0) {
		duprintf("ip_tables: check failed for `%s'.\n",
			 par.match->name);
		return ret;
	}
	++*i;
	return 0;
}

static int
find_check_match(struct ipt_entry_match *m, struct xt_mtchk_param *par,
		 unsigned int *i)
{
	struct xt_match *match;
	int ret;

	match = try_then_request_module(xt_find_match(AF_INET, m->u.user.name,
						      m->u.user.revision),
					"ipt_%s", m->u.user.name);
	if (IS_ERR(match) || !match) {
		duprintf("find_check_match: `%s' not found\n", m->u.user.name);
		return match ? PTR_ERR(match) : -ENOENT;
	}
	m->u.kernel.match = match;

	ret = check_match(m, par, i);
	if (ret)
		goto err;

	return 0;
err:
	module_put(m->u.kernel.match->me);
	return ret;
}

static int check_target(struct ipt_entry *e, const char *name)
{
	struct ipt_entry_target *t = ipt_get_target(e);
	struct xt_tgchk_param par = {
		.table     = name,
		.entryinfo = e,
		.target    = t->u.kernel.target,
		.targinfo  = t->data,
		.hook_mask = e->comefrom,
		.family    = NFPROTO_IPV4,
	};
	int ret;

	ret = xt_check_target(&par, t->u.target_size - sizeof(*t),
	      e->ip.proto, e->ip.invflags & IPT_INV_PROTO);
	if (ret < 0) {
		duprintf("ip_tables: check failed for `%s'.\n",
			 t->u.kernel.target->name);
		return ret;
	}
	return 0;
}

static int
find_check_entry(struct ipt_entry *e, const char *name, unsigned int size,
		 unsigned int *i)
{
	struct ipt_entry_target *t;
	struct xt_target *target;
	int ret;
	unsigned int j;
	struct xt_mtchk_param mtpar;

	ret = check_entry(e, name);
	if (ret)
		return ret;

	j = 0;
	mtpar.table     = name;
	mtpar.entryinfo = &e->ip;
	mtpar.hook_mask = e->comefrom;
	mtpar.family    = NFPROTO_IPV4;
	ret = IPT_MATCH_ITERATE(e, find_check_match, &mtpar, &j);
	if (ret != 0)
		goto cleanup_matches;

	t = ipt_get_target(e);
	target = try_then_request_module(xt_find_target(AF_INET,
							t->u.user.name,
							t->u.user.revision),
					 "ipt_%s", t->u.user.name);
	if (IS_ERR(target) || !target) {
		duprintf("find_check_entry: `%s' not found\n", t->u.user.name);
		ret = target ? PTR_ERR(target) : -ENOENT;
		goto cleanup_matches;
	}
	t->u.kernel.target = target;

	ret = check_target(e, name);
	if (ret)
		goto err;

	(*i)++;
	return 0;
 err:
	module_put(t->u.kernel.target->me);
 cleanup_matches:
	IPT_MATCH_ITERATE(e, cleanup_match, &j);
	return ret;
}

static int
check_entry_size_and_hooks(struct ipt_entry *e,
			   struct xt_table_info *newinfo,
			   unsigned char *base,
			   unsigned char *limit,
			   const unsigned int *hook_entries,
			   const unsigned int *underflows,
			   unsigned int *i)
{
	unsigned int h;

	if ((unsigned long)e % __alignof__(struct ipt_entry) != 0
	    || (unsigned char *)e + sizeof(struct ipt_entry) >= limit) {
		duprintf("Bad offset %p\n", e);
		return -EINVAL;
	}

	if (e->next_offset
	    < sizeof(struct ipt_entry) + sizeof(struct ipt_entry_target)) {
		duprintf("checking: element %p size %u\n",
			 e, e->next_offset);
		return -EINVAL;
	}

	/* Check hooks & underflows */
	for (h = 0; h < NF_INET_NUMHOOKS; h++) {
		if ((unsigned char *)e - base == hook_entries[h])
			newinfo->hook_entry[h] = hook_entries[h];
		if ((unsigned char *)e - base == underflows[h])
			newinfo->underflow[h] = underflows[h];
	}

	/* FIXME: underflows must be unconditional, standard verdicts
	   < 0 (not IPT_RETURN). --RR */

	/* Clear counters and comefrom */
	e->counters = ((struct xt_counters) { 0, 0 });
	e->comefrom = 0;

	(*i)++;
	return 0;
}

static int
cleanup_entry(struct ipt_entry *e, unsigned int *i)
{
	struct xt_tgdtor_param par;
	struct ipt_entry_target *t;

	if (i && (*i)-- == 0)
		return 1;

	/* Cleanup all matches */
	IPT_MATCH_ITERATE(e, cleanup_match, NULL);
	t = ipt_get_target(e);

	par.target   = t->u.kernel.target;
	par.targinfo = t->data;
	par.family   = NFPROTO_IPV4;
	if (par.target->destroy != NULL)
		par.target->destroy(&par);
	module_put(par.target->me);
	return 0;
}

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL) || defined(CONFIG_RTL_HARDWARE_NAT)
/*get the chain number of the ipt_entry*/
static int get_hookNum(struct ipt_entry *e, unsigned char *base, const unsigned int valid_hooks,const unsigned int *hook_entries)
{
	int h;
	unsigned int offset = 0;
	int hook_num = -1;

	offset = (void *)e - (void *)base;	
	for(h = 0; h < NF_INET_NUMHOOKS; h++)
	{		
		if(!(valid_hooks & (1 << h)))
			continue;
		
		if(offset >= hook_entries[h])
			hook_num = h;		
	}

	return hook_num;
}
#endif


#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
static struct xt_target ipt_standard_target;
static struct xt_target ipt_error_target; 
#if 0
static void rtl865x_print_iptRule2Acl_tbl(void)
{
	rtl865x_iptRule2Acl_tbl *listNode;
	xt_rule_to_acl_t *match2acl;
	int i ;
	printk("=======================================\n");
	list_for_each_entry(listNode,&rtl865x_iptRule2Acl_tbl_list,list)
	{
		printk("list->tblName(%s),list->priority(%d)\n",listNode->tblName,listNode->priority);
		for(i = 0; i < RTL865X_CHAINLIST_NUMBER_PER_TBL; i++)
		{
			printk("%d\n",i);
			list_for_each_entry(match2acl,&listNode->chainList[i],list)
			{
				printk("  inIf(%s) outIn(%s) aclType(0x%x) aclAction(0x%x),direction(%d)\n",match2acl->iniface,match2acl->outiface,match2acl->aclRule->ruleType_,match2acl->aclRule->actionType_,match2acl->aclRule->direction_);
			}			
		}
		
	}
	printk("=======================================\n");
}
#endif
static rtl865x_iptRule2Acl_tbl* rtl865x_get_ipt2Acl_tbl(const char *name)
{
	rtl865x_iptRule2Acl_tbl *listNode,*retEntry;	
	retEntry = NULL;
	
	list_for_each_entry(listNode,&rtl865x_iptRule2Acl_tbl_list,list)
	{
		if(memcmp(listNode->tblName,name,strlen(name)) == 0)
		{
			retEntry = listNode;
			break;
		}
	}

	return retEntry;
}

/*translate iptables ip rule to acl*/
static int ipt_ip2Acl(struct ipt_entry *e, rtl865x_AclRule_t *acl, int *all_match)
{

	if(e == NULL || acl == NULL)
		return -1;


	acl->ruleType_ = RTL865X_ACL_IP;
		
	/*proto 0=ANY */
	if(e->ip.proto == 0)		
		acl->ipProtoMask_ = 0x0;
	else
		acl->ipProtoMask_ = 0xff;

	acl->srcIpAddr_ 		= e->ip.src.s_addr;
	acl->srcIpAddrMask_	= e->ip.smsk.s_addr;
	acl->dstIpAddr_		= e->ip.dst.s_addr;
	acl->dstIpAddrMask_	= e->ip.dmsk.s_addr;
	acl->ipProto_			= e->ip.proto;

	if((e->ip.flags & IPT_F_FRAG) && ((e->ip.invflags & IPT_INV_FRAG) == 0) )
	{
		acl->ipFOP_ = 1;
		acl->ipFOM_ = 1;
	}

	if(e->ip.smsk.s_addr == 0 && e->ip.dmsk.s_addr == 0 && e->ip.proto == 0)
	{
		/*all packet match this rule... so, this rule should be add to tail...*/
		if(all_match)
			*all_match = 1;
		/*
		*hyking:
		*when all packet match this rule, we change acl->ruleType to ether type
		*2008-12-16
		*/
		acl->ruleType_ = RTL865X_ACL_MAC;
	}

	return 0;
}

static int standard_target2Acl(struct ipt_entry_target *t,rtl865x_AclRule_t *rule)
{
	switch(-(((struct ipt_standard_target *)t)->verdict) -1)
	{
		case NF_DROP:
			rule->actionType_ = RTL865X_ACL_DROP;
			break;
		case NF_ACCEPT:
			rule->actionType_  = RTL865X_ACL_PERMIT;
			break;
		case NF_QUEUE:
		case NF_REPEAT:
		case NF_STOP:
		case NF_STOLEN:
			rule->actionType_ = RTL865X_ACL_TOCPU;
			break;
		default:
			rule->actionType_ = RTL865X_ACL_TOCPU;
			break;
					
	}
	return 0;
}

/*translate iptables rule to ACL*/
static int translate_rule2Acl(struct ipt_entry *e,  
unsigned char *base,const char *name,unsigned int size,
const unsigned int valid_hooks,const unsigned int *hook_entries)
{
	
	int match_cnt = 0;
	int hook_num = -1;
	int nxt_hookNum = -1;
	struct ipt_entry_match *__match;
	struct ipt_entry_target *t,*nxt_target;
	struct ipt_target *target;
	rtl865x_AclRule_t *rule = NULL;
	xt_rule_to_acl_t *list_node = NULL;
	struct ipt_entry *nxt_entry;
	rtl865x_iptRule2Acl_tbl *ipt2AclTbl;
	int retval;
	unsigned int last_entry = 0;
	unsigned int invflags = 0;
	int allMatch = 0;
	void	*data = NULL;
//	unsigned int default_action = RTL865X_ACL_PERMIT;
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
	char	qosIfName[IFNAMSIZ];
#endif
	static unsigned char qosChain = 0;	/*cathy, decide if the rule is for qos chain*/

	t = ipt_get_target(e);

	if (!qosChain && strcmp(t->u.kernel.target->name, IPT_ERROR_TARGET) == 0 &&
		(strstr(t->data, "ipq") || strstr(t->data, "qos"))) {
		//printk("start of qos chain: %s\n", t->data);
		qosChain = 1;
	}
	else if (qosChain && strcmp(t->u.kernel.target->name, IPT_STANDARD_TARGET) == 0) {
		//printk("end of qos chain\n");
		qosChain = 0;
	}
	if(t == NULL || (t->u.kernel.target  == &ipt_error_target) )
		goto next;
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
	memset(qosIfName, 0, IFNAMSIZ);
#endif

	hook_num = get_hookNum(e,base,valid_hooks,hook_entries);

	/*last entry of this hooknum??*/
	last_entry = 0;	
	if(((void *) e - (void *)base + e->next_offset) >= size)
		last_entry = 1;
	
	if(last_entry == 0)
	{
		nxt_entry = (struct ipt_entry *)((void *)e + e->next_offset);
		nxt_hookNum = get_hookNum(nxt_entry,base,valid_hooks,hook_entries);
		if(nxt_hookNum != hook_num)
			last_entry = 1;

		/*there are error rule at the end of the table...*/
		nxt_target = ipt_get_target(nxt_entry);
		if(nxt_target->u.kernel.target  == &ipt_error_target)
		{
			last_entry = 1;
		}
	}

	if(hook_num < 0 || hook_num >= NF_INET_NUMHOOKS)
	{
		printk("!!!!BUG!!!!%s(%d)\n",__FUNCTION__,__LINE__);
		goto next;
	}

	/*if this entry is the last entry of filter table INPUT chain or output chain*/
#if 0//QL in_def_action should be set by user.
	if(last_entry)
	{
		if(t->u.kernel.target == &ipt_standard_target)
		{			
			switch((-((struct ipt_standard_target *)t)->verdict) -1)
			{
				case NF_DROP:
					 default_action = RTL865X_ACL_DROP;
					break;
				case NF_ACCEPT:
					 default_action = RTL865X_ACL_PERMIT;
					break;
				case NF_QUEUE:
				case NF_REPEAT:
				case NF_STOP:
				case NF_STOLEN:
					 default_action = RTL865X_ACL_TOCPU;
					break;
				default:
					 default_action = RTL865X_ACL_TOCPU;
					break;
							
			}

		#if 1
			if(hook_num == NF_INET_FORWARD)			
				in_def_action = default_action;		
		#else
			if(hook_num == NF_INET_LOCAL_IN)
				in_def_action = default_action;
			else if(hook_num == NF_INET_LOCAL_OUT)
				out_def_action = default_action;
		#endif
			
		}
	}
#endif
	///*only translate input&forward chain rule*/
	//translate all chains for filter table now...
	#if 0
	if(	(hook_num >= NF_INET_LOCAL_OUT 
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
		&& memcmp(name, RTL865X_QOS_TABLE_NAME, RTL865X_QOS_TABLE_LEN)
#endif
		) || (last_entry)	)
		goto next;
	#else
	if(last_entry)
		goto next;
	
	#endif
	
	list_node = kmalloc(sizeof(xt_rule_to_acl_t),GFP_KERNEL);	
	if(!list_node)
	{
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto next;
	}
		
	rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
	if(!rule)
	{			
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto next;
	}

	
	memset(rule, 0,sizeof(rtl865x_AclRule_t));
	//default: all packet to cpu
	rule->actionType_ = RTL865X_ACL_TOCPU;
	rule->pktOpApp_ = RTL865X_ACL_ALL_LAYER;
	strcpy(rule->inNetif,e->ip.iniface);

	/*invert interface flag*/
	if(e->ip.invflags & IPT_INV_VIA_IN)
		rule->inv_flag = RTL865X_INVERT_IN_NETIF;
	
	else if(e->ip.invflags & IPT_INV_VIA_OUT)
		rule->inv_flag = RTL865X_INVERT_OUT_NETIF;
	
	target = t->u.kernel.target;		
	match_cnt = IPT_MATCH_NUMBER(e);
	retval = -1;
	if(match_cnt == 0)
	{
		/*only ipt_ip & target information in iptables rule, no match rule*/
		/*ip rule...*/
		retval = ipt_ip2Acl(e,rule,&allMatch);

		if(retval != 0)
			printk("%s(%d) BUG!!!!\n",__FUNCTION__,__LINE__);
		
		/*acl action....*/
		if(t->u.kernel.target == &ipt_standard_target)
		{
			/*standard target*/
			standard_target2Acl(t,rule);
		}
		else if (t->u.kernel.target->target2acl)
		{
			retval = t->u.kernel.target->target2acl(name, e, target, t->data,rule,e->comefrom, &data, qosChain);
			if(retval != 0)
			{
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
				/* Add the following conditions for pathing qos rules */
				if (!memcmp(name, RTL865X_QOS_TABLE_NAME, RTL865X_QOS_TABLE_LEN) && 
					e->ip.outiface[0]=='\0' && 
					retval == RTL_QOSFINDSPECIALNETIF)
				{
					memcpy(qosIfName, data, strlen(data));
					qosIfName[strlen(data)] = '\0';
					kfree(data);
				}
				else if (retval == RTL865X_SKIP_THIS_RULE)
				{
					goto next;
				}
				else
#endif
					rule->actionType_ = RTL865X_ACL_TOCPU;
			}
		}
		else
		{
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
			goto next;
#endif
			rule->actionType_ = RTL865X_ACL_TOCPU;
		}

		/*deal with invert flags...*/
		if(e->ip.invflags & (IPT_INV_SRCIP|IPT_INV_DSTIP | IPT_INV_PROTO))
		{
			if(rule->actionType_ == RTL865X_ACL_PERMIT)
				rule->actionType_ = RTL865X_ACL_DROP;
			else if (rule->actionType_ == RTL865X_ACL_DROP)
				rule->actionType_ = RTL865X_ACL_PERMIT;
		}			
	}
	else if(match_cnt == 1)
	{
		__match = (void *)(e) + sizeof(struct ipt_entry);
			
		/*translate match to ACL rule...*/
		if(__match->u.kernel.match && __match->u.kernel.match->match2acl)
		{	
			retval = __match->u.kernel.match->match2acl(name, &e->ip, __match->u.kernel.match, __match->data,rule,&invflags);
			if(retval == 0)
			{
				/*translate target to ACL action*/
				if(t->u.kernel.target == &ipt_standard_target)
				{
					/*standard target*/
					standard_target2Acl(t,rule);
					#if 0
					switch(-(((struct ipt_standard_target *)t)->verdict) -1)
					{
						case NF_DROP:
							if(invflags)
								rule->actionType_ = RTL865X_ACL_PERMIT;
							else
								rule->actionType_ = RTL865X_ACL_DROP;
							break;
						case NF_ACCEPT:
							if(invflags)
								rule->actionType_ = RTL865X_ACL_DROP;
							else
								rule->actionType_  = RTL865X_ACL_PERMIT;
							break;
						case NF_QUEUE:
						case NF_REPEAT:
						case NF_STOP:
						case NF_STOLEN:
							rule->actionType_ = RTL865X_ACL_TOCPU;
							break;
						default:
							rule->actionType_ = RTL865X_ACL_TOCPU;
							break;
						
					}
					#endif					
				}
				else if (t->u.kernel.target->target2acl)
				{
					retval = t->u.kernel.target->target2acl(name, e, target, t->data,rule,e->comefrom, &data, qosChain);
					if(retval != 0)
					{
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
						/* Add the following conditions for pathing qos rules */
						if (!memcmp(name, RTL865X_QOS_TABLE_NAME, RTL865X_QOS_TABLE_LEN) && 
							e->ip.outiface[0]=='\0' && 
							retval == RTL_QOSFINDSPECIALNETIF)
						{
							memcpy(qosIfName, data, strlen(data));
							qosIfName[strlen(data)] = '\0';
							kfree(data);
						}
						else if (retval == RTL865X_SKIP_THIS_RULE)
						{
							goto next;
						}
						else
#endif
							rule->actionType_ = RTL865X_ACL_TOCPU;
					}
					#if 0
					else
					{
						if(invflags)
						{
							if(rule->actionType_ == RTL865X_ACL_PERMIT)
								rule->actionType_ = RTL865X_ACL_DROP;
							else if(rule->actionType_ == RTL865X_ACL_DROP)
								rule->actionType_ = RTL865X_ACL_PERMIT;
						}
					}
					#endif
				}
				else
				{
					rule->actionType_ = RTL865X_ACL_TOCPU;
				}

				/*invert interface flag*/
				if(invflags)
				{
					if(rule->actionType_ == RTL865X_ACL_PERMIT)
						rule->actionType_ = RTL865X_ACL_DROP;
					else if(rule->actionType_ == RTL865X_ACL_DROP)
						rule->actionType_ = RTL865X_ACL_PERMIT;
				}
			}
			else if (retval == RTL865X_ESTABLISH_RULE)
			{
				/*translate target to ACL action*/
				if(t->u.kernel.target == &ipt_standard_target)
				{
					/*standard target*/
					standard_target2Acl(t,rule);
					#if 0
					switch(-(((struct ipt_standard_target *)t)->verdict) -1)
					{
						case NF_DROP:
							if(invflags)
								rule->actionType_ = RTL865X_ACL_PERMIT;
							else
								rule->actionType_ = RTL865X_ACL_DROP;
							break;
						case NF_ACCEPT:
							if(invflags)
								rule->actionType_ = RTL865X_ACL_DROP;
							else
								rule->actionType_  = RTL865X_ACL_PERMIT;
							break;
						case NF_QUEUE:
						case NF_REPEAT:
						case NF_STOP:
						case NF_STOLEN:
							rule->actionType_ = RTL865X_ACL_TOCPU;
							break;
						default:
							rule->actionType_ = RTL865X_ACL_TOCPU;
							break;
						
					}
					#endif
				}
				else if (t->u.kernel.target->target2acl)
				{
					retval = t->u.kernel.target->target2acl(name, e, target, t->data,rule,e->comefrom, &data, qosChain);
					if(retval != 0)
					{
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
						/* Add the following conditions for pathing qos rules */
						if (!memcmp(name, RTL865X_QOS_TABLE_NAME, RTL865X_QOS_TABLE_LEN) && 
							e->ip.outiface[0]=='\0' && 
							retval == RTL_QOSFINDSPECIALNETIF)
						{
							memcpy(qosIfName, data, strlen(data));
							qosIfName[strlen(data)] = '\0';
							kfree(data);
						}
						else if (retval == RTL865X_SKIP_THIS_RULE)
						{
							goto next;
						}
						else
#endif
							rule->actionType_ = RTL865X_ACL_TOCPU;
					}
					#if 0
					else
					{
						if(invflags)
						{
							if(rule->actionType_ == RTL865X_ACL_PERMIT)
								rule->actionType_ = RTL865X_ACL_DROP;
							else if(rule->actionType_ == RTL865X_ACL_DROP)
								rule->actionType_ = RTL865X_ACL_PERMIT;
						}
					}
					#endif
				}
				else
				{
					rule->actionType_ = RTL865X_ACL_TOCPU;
				}

				/*invert interface flag*/
				if(invflags)
				{
					if(rule->actionType_ == RTL865X_ACL_PERMIT)
						rule->actionType_ = RTL865X_ACL_DROP;
					else if(rule->actionType_ == RTL865X_ACL_DROP)
						rule->actionType_ = RTL865X_ACL_PERMIT;
				}

				if(rule->actionType_ == RTL865X_ACL_PERMIT)
					establish_rule_permit = 1;
				
				goto next;
			}			
			else if(retval == RTL865X_SKIP_THIS_RULE)
			{
				goto next;
			}
			else
			{
				rule->ruleType_		= RTL865X_ACL_MAC;
				rule->actionType_	= RTL865X_ACL_TOCPU;
				rule->pktOpApp_ = RTL865X_ACL_ALL_LAYER;
			}
		}
		else
		{
			rule->ruleType_		= RTL865X_ACL_MAC;
			rule->actionType_	= RTL865X_ACL_TOCPU;
			rule->pktOpApp_ = RTL865X_ACL_ALL_LAYER;
		}
	}
	else if(match_cnt > 1)
	{
		int		len;
		int		hasNoSpt = FALSE;
		
		len = 0;
		while(match_cnt>0)
		{
			__match = (void *)(e) + sizeof(struct ipt_entry) + len;

			if(__match->u.kernel.match && __match->u.kernel.match->match2acl)
			{
				retval = __match->u.kernel.match->match2acl(name, &e->ip, __match->u.kernel.match, __match->data,rule,&invflags);

				if(retval == 0)
				{
					len += __match->u.match_size;
					match_cnt--;
				}
				else
					break;
			}
			else
			{
				hasNoSpt = TRUE;
				match_cnt--;
//				retval = RTL865X_MATCH_NOT_SUPPORTED;
//				break;
			}
		}

		if (hasNoSpt==TRUE)
			retval = RTL865X_MATCH_NOT_SUPPORTED;
		
		if(retval == 0)
		{
		
			/*translate target to ACL action*/
			if(t->u.kernel.target == &ipt_standard_target)
			{
				/*standard target*/
				standard_target2Acl(t,rule);
				#if 0
				switch(-(((struct ipt_standard_target *)t)->verdict) -1)
				{
					case NF_DROP:
						if(invflags)
							rule->actionType_ = RTL865X_ACL_PERMIT;
						else
							rule->actionType_ = RTL865X_ACL_DROP;
						break;
					case NF_ACCEPT:
						if(invflags)
							rule->actionType_ = RTL865X_ACL_DROP;
						else
							rule->actionType_  = RTL865X_ACL_PERMIT;
						break;
					case NF_QUEUE:
					case NF_REPEAT:
					case NF_STOP:
					case NF_STOLEN:
						rule->actionType_ = RTL865X_ACL_TOCPU;
						break;
					default:
						rule->actionType_ = RTL865X_ACL_TOCPU;
						break;
					
				}
				#endif
			}
			else if (t->u.kernel.target->target2acl)
			{
				retval = t->u.kernel.target->target2acl(name, e, target, t->data,rule,e->comefrom, &data, qosChain);
				if(retval != 0)
				{
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
					/* Add the following conditions for pathing qos rules */
					if (!memcmp(name, RTL865X_QOS_TABLE_NAME, RTL865X_QOS_TABLE_LEN) && 
						e->ip.outiface[0]=='\0' && 
						retval == RTL_QOSFINDSPECIALNETIF)
					{
						memcpy(qosIfName, data, strlen(data));
						qosIfName[strlen(data)] = '\0';
						kfree(data);
					}
					else if (retval == RTL865X_SKIP_THIS_RULE)
					{
						goto next;
					}
					else
#endif
						rule->actionType_ = RTL865X_ACL_TOCPU;
				}
				else
				{
					if(invflags)
					{
						if(rule->actionType_ == RTL865X_ACL_PERMIT)
							rule->actionType_ = RTL865X_ACL_DROP;
						else if(rule->actionType_ == RTL865X_ACL_DROP)
							rule->actionType_ = RTL865X_ACL_PERMIT;
					}
				}
			}
			else
			{
				rule->actionType_ = RTL865X_ACL_TOCPU;
			}
			
			/*invert interface flag*/
			if(invflags)
			{
				if(rule->actionType_ == RTL865X_ACL_PERMIT)
					rule->actionType_ = RTL865X_ACL_DROP;
				else if(rule->actionType_ == RTL865X_ACL_DROP)
					rule->actionType_ = RTL865X_ACL_PERMIT;
			}	
			
		}
		else if(retval == RTL865X_SKIP_THIS_RULE)
		{
			goto next;
		}
		else
		{
			/*add ACL trap all packet to CPU*/
#if 0
			memset(rule, 0,sizeof(rtl865x_AclRule_t));
			rule->ruleType_		= RTL865X_ACL_MAC;
			rule->actionType_	= RTL865X_ACL_TOCPU;
			rule->pktOpApp_ = RTL865X_ACL_ALL_LAYER;
#else
			/* For some un-support type rules, only trap the special kinds of pkt instead of trap all */
			rule->actionType_	= RTL865X_ACL_TOCPU;
			rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
#endif
		}
	}
	else
		goto next;

	/*add xt_rule_to_acl to list*/
	

	/*  Kevin, just add the rule with "toACL_ bit" to acl table */
#if 1
	if(rule->toACL_ == 1)	
	{
		/*  We always add acl rule in ingress direction , and trap to cpu even though the action is drop in iptables rule */
		rule->direction_ = RTL865X_ACL_INGRESS;	
		
		#ifdef CONFIG_RTL8676_Dynamic_ACL
		/* when enable dynamic acl , we only translate iptables url/domain rule to acl , so always trap  */
		rule->actionType_ = RTL865X_ACL_TOCPU;
		#else
		/*  patch for mac filter, because in our sdk's iptables , permit mac filter rules will be set RETURN  drop mac filter rules will be set DROP
		     When translate to acl rules,  RETURN=> TRAP  , 	DROP=> DROP .
		     However, the mac filter is the last chain in FORWARD chain ,so RETURN means ACCEPT (Chain FORWARD's policy is ACCEPT) */	
		if( rule->macFilter_ && rule->actionType_ == RTL865X_ACL_TOCPU)		
			rule->actionType_ = RTL865X_ACL_PERMIT;		
		
		/*  if the forceTrapToCPU bit has been set up by any match module , force this rule trap to CPU whatever */
		if(rule->forceTrapToCPU_)
			rule->actionType_ = RTL865X_ACL_TOCPU;
		#endif
	}
	else
		goto next;	
#else
	/* 
	  *	Since we do the acl check actually in PREROUTING chain
	  *	Some toCpu pkt maybe mis-decided by FORWARD chain rules
	  *	So, we do the following patch.
	  */
	if(hook_num == NF_INET_FORWARD)
	{
		if(rule->actionType_ != RTL865X_ACL_PERMIT)
			rule->actionType_ = RTL865X_ACL_TOCPU;
		
		rule->pktOpApp_ = RTL865X_ACL_L3_AND_L4;
	}

	if(hook_num <NF_INET_LOCAL_OUT)
		rule->direction_ = RTL865X_ACL_INGRESS;
	else
		rule->direction_ = RTL865X_ACL_EGRESS;
#endif
	
	
	list_node->aclRule = rule;
#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
	if (qosIfName[0]!='\0')
	{
		memcpy(&list_node->iniface,qosIfName,strlen(qosIfName));
		list_node->iniface[strlen(qosIfName)] = '\0';
	}
	else
#endif
	{
		memcpy(&list_node->iniface,&e->ip.iniface,strlen(e->ip.iniface)+1);
		list_node->iniface[strlen(e->ip.iniface)] = '\0';
	}

	{
		memcpy(&list_node->outiface,&e->ip.outiface,strlen(e->ip.outiface)+1);
		list_node->outiface[strlen(e->ip.outiface)] = '\0';
	}

	/*  Kevin , if the interface is under bridge(ex.eth0.2~eth0.5) , convert the interface name for adding acl rule under br0  */			
	if(strcmp(e->ip.iniface,""))
	{
		struct net_device* dev;
		struct net_bridge_port *br_port;			
		dev = dev_get_by_name(&init_net,e->ip.iniface);
		if(dev)
		{
			br_port = dev->br_port;
			if(br_port && !strcmp(dev->br_port->br->dev->name,RTL_DRV_LAN_NETIF_NAME))										
				strcpy(list_node->iniface,RTL_DRV_LAN_NETIF_NAME);				
			
			dev_put(dev);
		}
	}	
	/*now, add this rule to releated chain*/

	ipt2AclTbl = rtl865x_get_ipt2Acl_tbl(name);
	if(ipt2AclTbl == NULL)
		goto next;

	if((allMatch == 1) && (!memcmp(name,"filter",strlen("filter"))))
		list_add_tail(&list_node->list,&rtl865x_iptRule2Acl_def_rule_list);
	else
	{
		#ifdef CONFIG_RTL8676_Dynamic_ACL
		list_add_tail(&list_node->list, &ipt2AclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_TRAP]);
		#else
		if(rule->forceTrapToCPU_)
			list_add_tail(&list_node->list, &ipt2AclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_FORCE_TRAP]);
		else
			list_add_tail(&list_node->list, &ipt2AclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_NORMAL]);
		#endif		
	}

	return 0;

next:

	/*free memory and return*/
	if(rule)
		kfree(rule);
	
	if(list_node)
		kfree(list_node);
		
	return 0;
	
} //translate_rule2Acl
#if 0
static int translate_ipTblRules2Acl(const char *name,
		unsigned int valid_hooks,
		struct xt_table_info *newinfo,
		void *entry0,
		unsigned int size,
		unsigned int number,
		const unsigned int *hook_entries,
		const unsigned int *underflows)
{
	int	ret;
	rtl865x_AclRule_t *rule;
	xt_rule_to_acl_t *list_node,*nxt;
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl;

#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
#else
	if(memcmp(name,"filter",strlen("filter")) != 0)
		return 0;
#endif

	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl(name);

	if(!ipt2aclTbl)
	{
		return 0;
	}

	ret = IPT_ENTRY_ITERATE(entry0, size,translate_rule2Acl, entry0,name,size,valid_hooks,hook_entries);

	/*merge def_rule_list to match_to_acl_rule_list*/
	if (!memcmp(name,"filter",strlen("filter")))
	{
		list_for_each_entry_safe(list_node,nxt,&rtl865x_iptRule2Acl_def_rule_list,list)
		{
			list_del(&list_node->list);
			list_add_tail(&list_node->list,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_4]);		
		}

	/*hyking:
		iptables rule can't deal with packets whose action is layer2 switch,
		so, patch this case when def_action is drop...
	*/
//	if((def_action == RTL865X_ACL_DROP) && !memcmp(name,"filter",strlen("filter")))
		if(establish_rule_permit == 1)
			in_def_action = RTL865X_ACL_PERMIT;
		
		if((in_def_action == RTL865X_ACL_DROP))
		{	
			/*deal with the permit multicast acl...*/
			list_node = kmalloc(sizeof(xt_rule_to_acl_t),GFP_KERNEL);	
			if(!list_node)
			{
				printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
				goto next;
			}
				
			rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
			if(!rule)
			{
				if(list_node)
					kfree(list_node);
				
				printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
				goto next;
			}
			
			/*permit all multicast packet...*/
			memset(rule, 0,sizeof(rtl865x_AclRule_t));
			rule->ruleType_ = RTL865X_ACL_MAC;
			rule->actionType_ = RTL865X_ACL_PERMIT;
			rule->pktOpApp_ = RTL865X_ACL_ALL_LAYER;

			rule->dstMac_.octet[0] = 0x01;
			rule->dstMacMask_.octet[0] = 0x01;
			
			/*add xt_rule_to_acl to list*/
			list_node->aclRule = rule;
			list_node->iniface[0] = '\0';
			list_add_tail(&list_node->list,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_3]);

			/*permit all arp packet*/
			list_node = kmalloc(sizeof(xt_rule_to_acl_t),GFP_KERNEL);	
			if(!list_node)
			{
				printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
				goto next;
			}
				
			rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
			if(!rule)
			{
				if(list_node)
					kfree(list_node);
				
				printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
				goto next;
			}
			
			/*permit all arp packet...*/
			memset(rule, 0,sizeof(rtl865x_AclRule_t));
			rule->ruleType_ = RTL865X_ACL_MAC;
			rule->actionType_ = RTL865X_ACL_PERMIT;
			rule->pktOpApp_ = RTL865X_ACL_ALL_LAYER;

			rule->typeLen_ = 0x0806;
			rule->typeLenMask_ = 0xffff;
			
			/*add xt_rule_to_acl to list*/
			list_node->aclRule = rule;
			list_node->iniface[0] = '\0';
			list_add_tail(&list_node->list,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_3]);
			
		}

		//in bound
		list_node = kmalloc(sizeof(xt_rule_to_acl_t),GFP_KERNEL);	
		if(!list_node)
		{
			printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
			goto next;
		}
			
		rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
		if(!rule)
		{
			if(list_node)
				kfree(list_node);
			
			printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
			goto next;
		}

		memset(rule, 0,sizeof(rtl865x_AclRule_t));


		{
			rule->ruleType_	= RTL865X_ACL_MAC;
			rule->actionType_	= in_def_action;
			rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
			rule->direction_ = RTL865X_ACL_INGRESS;
			
			/*add xt_rule_to_acl to list*/
			list_node->aclRule = rule;
			list_node->iniface[0] = '\0';
			list_add_tail(&list_node->list,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_4]);
		}

		//outbound
		list_node = kmalloc(sizeof(xt_rule_to_acl_t),GFP_KERNEL);	
		if(!list_node)
		{
			printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
			goto next;
		}
			
		rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
		if(!rule)
		{
			if(list_node)
				kfree(list_node);
			
			printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
			goto next;
		}

		memset(rule, 0,sizeof(rtl865x_AclRule_t));


		{
			rule->ruleType_	= RTL865X_ACL_MAC;
			rule->actionType_	= out_def_action;
			rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
			rule->direction_ = RTL865X_ACL_EGRESS;
			
			/*add xt_rule_to_acl to list*/
			list_node->aclRule = rule;
			list_node->iniface[0] = '\0';
			list_add_tail(&list_node->list,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_4]);
		}
	}

	return 0;

next:	
	return ret;
}
#endif
static int rtl865x_free_chain_inIpt2Acl_tbl(struct list_head *listHead)
{
	/*free all xtmatch rule*/
	xt_rule_to_acl_t *match2acl,*nxt;
	list_for_each_entry_safe(match2acl,nxt,listHead,list)
	{
		list_del(&match2acl->list);
		kfree(match2acl->aclRule);
		kfree(match2acl);			
	}

	return 0;
}

static int rtl865x_free_allchains_inIpt2Acl_tbl(char *name)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl = NULL;
	int i;

#if  defined(CONFIG_RTL_HW_QOS_SUPPORT)
	if (!memcmp(name, RTL865X_QOS_TABLE_NAME, RTL865X_QOS_TABLE_LEN))
	{
		rtl865x_qosFlushRule();
	}
#endif

	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl(name);

	if(!ipt2aclTbl)
		return -1;

	for(i = 0; i < RTL865X_CHAINLIST_NUMBER_PER_TBL; i++)
	{
		rtl865x_free_chain_inIpt2Acl_tbl(&ipt2aclTbl->chainList[i]);
	}

	return 0;
}

/*
	tbl->priority: the minimum has highest priority
*/
int rtl865x_register_ipt2Acl_tbl(rtl865x_iptRule2Acl_tbl *tbl)
{
	rtl865x_iptRule2Acl_tbl *node,*insPos;
	
	insPos = NULL;	
	node = NULL;

	node = rtl865x_get_ipt2Acl_tbl(tbl->tblName);
	if(node != NULL)
		return -1;
	
	list_for_each_entry(node,&rtl865x_iptRule2Acl_tbl_list,list)
	{	
		if(node->priority > tbl->priority)
		{
			insPos = node;
			break;
		}
	}

	/*now, insert before the insPos*/
	if(insPos)
	{
		list_add(&tbl->list, insPos->list.prev);
	}
	else
	{
		list_add_tail(&tbl->list, &rtl865x_iptRule2Acl_tbl_list);
	}
	
	return 0;
}

int rtl865x_unregister_ipt2Acl_tbl(char *tblName)
{
	rtl865x_iptRule2Acl_tbl *listNode,*nxt;
	int i;
	list_for_each_entry_safe(listNode, nxt, &rtl865x_iptRule2Acl_tbl_list,list)
	{
		if(memcmp(listNode->tblName,tblName,strlen(tblName)) == 0)
		{
			list_del(&listNode->list);
			for(i = 0; i < RTL865X_CHAINLIST_NUMBER_PER_TBL; i++)
				rtl865x_free_chain_inIpt2Acl_tbl(&listNode->chainList[i]);
			kfree(listNode);
			return 0;			
		}
	}

	return -1;
}
		
static rtl865x_iptRule2Acl_tbl* rtl865x_create_new_tbl(int priority,const char* tblname)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	
	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl(tblname);	
	if(!ipt2aclTbl)
	{
		int i;
		ipt2aclTbl = kmalloc(sizeof(rtl865x_iptRule2Acl_tbl), GFP_KERNEL);
		if(!ipt2aclTbl)
			goto FailToCreate;
		
		memset(ipt2aclTbl, 0, sizeof(rtl865x_iptRule2Acl_tbl));
		for(i = 0; i < RTL865X_CHAINLIST_NUMBER_PER_TBL; i++)
		{
			INIT_LIST_HEAD(&ipt2aclTbl->chainList[i]);
		}
		ipt2aclTbl->priority = priority;		
		strcpy(ipt2aclTbl->tblName,tblname);
		if(rtl865x_register_ipt2Acl_tbl(ipt2aclTbl) != 0)		
			goto FailToCreate;		
	}

	
	return ipt2aclTbl;

FailToCreate:
	if(ipt2aclTbl);
		kfree(ipt2aclTbl);
	return NULL;	
}


static int rtl865x_rewrite_filterchain_into_acl(void)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl;
	xt_rule_to_acl_t *match2acl;
	int32 retval = SUCCESS;
	int32 temp_retval = SUCCESS;
	
	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");	
	if(!ipt2aclTbl)
	{	
		printk("waring ! %s return failed at line %d\n",__func__,__LINE__);
		return FAILED;		
	}

	rtl865x_flush_allAcl_fromChain(NULL,ipt2aclTbl->priority,RTL865X_ACL_INGRESS);
	rtl865x_flush_allAcl_fromChain(NULL,ipt2aclTbl->priority,RTL865X_ACL_EGRESS);

	switch(acl_mode)
	{
		case RTL865X_ACL_Mode_Always_Permit:
		case RTL865X_ACL_Mode_Always_Trap:
			break;
		case RTL865X_ACL_Mode_Normal:
			#ifdef CONFIG_RTL8676_Dynamic_ACL
			#ifdef CONFIG_RTL_MULTI_ETH_WAN
			#if 0
			list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_REDIRECT],list)
			{			
				temp_retval =  rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				if( temp_retval!=SUCCESS && temp_retval!=RTL_EENTRYALREADYEXIST)				
					retval += temp_retval; 	
			}
			#endif
			#endif
			list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_PERMIT],list)
			{			
				temp_retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				if( temp_retval!=SUCCESS && temp_retval!=RTL_EENTRYALREADYEXIST)				
					retval += temp_retval; 	
			}
			list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_TRAP],list)
			{			
				temp_retval =  rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				if( temp_retval!=SUCCESS && temp_retval!=RTL_EENTRYALREADYEXIST)				
					retval += temp_retval; 	
			}
			#ifdef CONFIG_RTL_MULTI_ETH_WAN
			list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_REDIRECT],list)
			{			
				temp_retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				if( temp_retval!=SUCCESS && temp_retval!=RTL_EENTRYALREADYEXIST)				
					retval += temp_retval; 	
			}
			#endif
			list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_PERMIT],list)
			{			
				temp_retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				if( temp_retval!=SUCCESS && temp_retval!=RTL_EENTRYALREADYEXIST)				
					retval += temp_retval; 	
			}
			#else
			list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_FORCE_TRAP],list)
			{			
				temp_retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				if( temp_retval!=SUCCESS && temp_retval!=RTL_EENTRYALREADYEXIST)				
					retval += temp_retval; 				
			}
			list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_NORMAL],list)
			{			
				temp_retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				if( temp_retval!=SUCCESS && temp_retval!=RTL_EENTRYALREADYEXIST)				
					retval += temp_retval; 	
			}
			#endif
			break;			
		default: //unrecognized acl mode
			printk("waring ! %s return failed at line %d\n",__func__,__LINE__);
			return FAILED;				
	}

	/* Kevin, when there are same rules in acl chain (rtl865x_add_acl return RTL_EENTRYALREADYEXIST) , it is a acceptable case
		ex.    when drop the specified outgoing pkts in macfilter		
   		CMD: /bin/iptables -A macfilter -i eth0.2 -m mac --mac-source 00:50:fc:65:8d:39 -j RETURN
		CMD: /bin/iptables -A macfilter -i eth0.3 -m mac --mac-source 00:50:fc:65:8d:39 -j RETURN
		CMD: /bin/iptables -A macfilter -i eth0.4 -m mac --mac-source 00:50:fc:65:8d:39 -j RETURN
		CMD: /bin/iptables -A macfilter -i eth0.5 -m mac --mac-source 00:50:fc:65:8d:39 -j RETURN	

		these iptables rules will be translated in "br0" , so they are same in the acl rule
	*/	
	if(retval!=SUCCESS)		
	{
		//printk("warning ! %s return failed (error no.:%d) at line %d\n",__func__,retval,__LINE__);
		return FAILED;	
	}
	else
		return SUCCESS;	
}


void rtl865x_rearrange_ipt2Acl_tbl(void)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl;
	xt_rule_to_acl_t *match2acl;
	int i,retval;

	//hyking:since default permit before rearrange rules to acl table,don't add permit acl now.
	//rtl865x_add_def_permit_acl();
	
	list_for_each_entry(ipt2aclTbl,&rtl865x_iptRule2Acl_tbl_list,list)
	{
		/*firstly, remove all acl which is add by user...*/
		rtl865x_flush_allAcl_fromChain(NULL,ipt2aclTbl->priority,RTL865X_ACL_INGRESS);
		rtl865x_flush_allAcl_fromChain(NULL,ipt2aclTbl->priority,RTL865X_ACL_EGRESS);

		/*  if tbl is filter, we call another function to handle it !  */
		if(!strcmp(ipt2aclTbl->tblName,"filter"))
			retval=rtl865x_rewrite_filterchain_into_acl();
		else
		{
			for(i = 0; i < RTL865X_CHAINLIST_NUMBER_PER_TBL; i++)
			{
				list_for_each_entry(match2acl,&ipt2aclTbl->chainList[i],list)
				{			
					retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				}
			}
		}
		#if 0
		/*QL: if default action is toCPU, then adjust default rule priority. */
		if (in_def_action != RTL865X_ACL_PERMIT) {
			list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_0],list)
			{			
				retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
			}
			
			list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_4],list)
			{			
				retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
			}

			for(i = RTL865x_CHAINLIST_PRIORITY_LEVEL_1; i < RTL865x_CHAINLIST_PRIORITY_LEVEL_4; i++)
			{
				list_for_each_entry(match2acl,&ipt2aclTbl->chainList[i],list)
				{			
					retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				}
			}
		}
		else {
			for(i = 0; i < RTL865X_CHAINLIST_NUMBER_PER_TBL; i++)
			{
				list_for_each_entry(match2acl,&ipt2aclTbl->chainList[i],list)
				{			
					retval = rtl865x_add_acl(match2acl->aclRule, match2acl->iniface, ipt2aclTbl->priority);
				}
			}
		}
		#endif
	}
	
	//rtl865x_del_def_permit_acl();
}


static void translate_ipTblRules2Acl(const char *name,
		unsigned int valid_hooks,
		struct xt_table_info *newinfo,
		void *entry0,
		unsigned int size,
		unsigned int number,
		const unsigned int *hook_entries,
		const unsigned int *underflows)
{

	rtl865x_iptRule2Acl_tbl *ipt2aclTbl;
	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl(name);
	if(ipt2aclTbl)
	{
		IPT_ENTRY_ITERATE(entry0, size,translate_rule2Acl, entry0,name,size,valid_hooks,hook_entries);
	}
}




int rtl865x_change_acl_default_action(void)
{	

	rtl865x_AclRule_t *rule;	
	int32 retval = SUCCESS;

	rtl865x_flush_allAcl_fromChain(NULL,RTL865X_ACL_DEFAULT,RTL865X_ACL_INGRESS);
	rtl865x_flush_allAcl_fromChain(NULL,RTL865X_ACL_DEFAULT,RTL865X_ACL_EGRESS);	
	
	/*  ingress */	
	if(acl_mode != RTL865X_ACL_Mode_Normal)
	{
		rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
		if(!rule)
		{				
			printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
			retval = FAILED;
			goto FINISH;
		}		
		memset(rule, 0,sizeof(rtl865x_AclRule_t));		
		rule->ruleType_	= RTL865X_ACL_MAC;
		rule->actionType_	= in_def_action;		
		rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
		rule->direction_ = RTL865X_ACL_INGRESS;	
		retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
		kfree(rule);
		if(retval!=SUCCESS)
			goto FINISH;
	}
	else
	{
		#ifdef CONFIG_RTL8676_Dynamic_ACL
			rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
			if(!rule)
			{				
				printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
				retval = FAILED;
				goto FINISH;
			}		
			memset(rule, 0,sizeof(rtl865x_AclRule_t));		
			rule->ruleType_	= RTL865X_ACL_MAC;
			rule->actionType_	= in_def_action;		
			rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
			rule->direction_ = RTL865X_ACL_INGRESS;	
			retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
			kfree(rule);
			if(retval!=SUCCESS)
				goto FINISH;
		#else
		switch(mac_filter_mode)
		{
			case RTL865X_ACL_MAC_FILTER_ALL_PERMIT:
				rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
				if(!rule)
				{				
					printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
					retval = FAILED;
					goto FINISH;
				}		
				memset(rule, 0,sizeof(rtl865x_AclRule_t));		
				rule->ruleType_	= RTL865X_ACL_MAC;
				rule->actionType_	= RTL865X_ACL_PERMIT;		
				rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
				rule->direction_ = RTL865X_ACL_INGRESS;	
				retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
				kfree(rule);
				if(retval!=SUCCESS)
					goto FINISH;
				break;
			case RTL865X_ACL_MAC_FILTER_IN_DROP_OUT_PERMIT:
				rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
				if(!rule)
				{				
					printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
					retval = FAILED;
					goto FINISH;
				}		
				memset(rule, 0,sizeof(rtl865x_AclRule_t));		
				rule->ruleType_ = RTL865X_ACL_SRCFILTER;
				rule->srcFilterIgnoreL3L4_ = 1;
				rule->srcFilterIgnoreL4_ = 1;							
				rule->srcFilterPort_ = 0x1E;  /*   port 1, 2, 3, 4  */
				rule->actionType_	= RTL865X_ACL_PERMIT;		
				rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
				rule->direction_ = RTL865X_ACL_INGRESS;
				retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
				kfree(rule);
				if(retval!=SUCCESS)
					goto FINISH;
				
				rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
				if(!rule)
				{				
					printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
					retval = FAILED;
					goto FINISH;
				}		
				memset(rule, 0,sizeof(rtl865x_AclRule_t));
				rule->ruleType_	= RTL865X_ACL_MAC;
				rule->typeLen_ 		= 0x0800;
				rule->typeLenMask_ 	= 0xFFFF;
				rule->actionType_	= RTL865X_ACL_DROP;		
				rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
				rule->direction_ = RTL865X_ACL_INGRESS;				
				retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
				kfree(rule);
				if(retval!=SUCCESS)
					goto FINISH;

				rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
				if(!rule)
				{				
					printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
					retval = FAILED;
					goto FINISH;
				}		
				memset(rule, 0,sizeof(rtl865x_AclRule_t));		
				rule->ruleType_	= RTL865X_ACL_MAC;
				rule->actionType_	= RTL865X_ACL_PERMIT;		
				rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
				rule->direction_ = RTL865X_ACL_INGRESS;	
				retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
				kfree(rule);
				if(retval!=SUCCESS)
					goto FINISH;				
				break;
			case RTL865X_ACL_MAC_FILTER_IN_PERMIT_OUT_DROP:
				rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
				if(!rule)
				{				
					printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
					retval = FAILED;
					goto FINISH;
				}		
				memset(rule, 0,sizeof(rtl865x_AclRule_t));		
				rule->ruleType_ = RTL865X_ACL_SRCFILTER;
				rule->srcFilterIgnoreL3L4_ = 1;
				rule->srcFilterIgnoreL4_ = 1;							
				rule->srcFilterPort_ = 0x01;  /*   port 0  */
				rule->actionType_	= RTL865X_ACL_PERMIT;		
				rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
				rule->direction_ = RTL865X_ACL_INGRESS;
				retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
				kfree(rule);
				if(retval!=SUCCESS)
					goto FINISH;
				
				rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
				if(!rule)
				{				
					printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
					retval = FAILED;
					goto FINISH;
				}		
				memset(rule, 0,sizeof(rtl865x_AclRule_t));		
				rule->ruleType_	= RTL865X_ACL_MAC;
				rule->typeLen_ 		= 0x0800;
				rule->typeLenMask_ 	= 0xFFFF;
				rule->actionType_	= RTL865X_ACL_DROP;		
				rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
				rule->direction_ = RTL865X_ACL_INGRESS;	
				retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
				kfree(rule);
				if(retval!=SUCCESS)
					goto FINISH;

				rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
				if(!rule)
				{				
					printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
					retval = FAILED;
					goto FINISH;
				}		
				memset(rule, 0,sizeof(rtl865x_AclRule_t));		
				rule->ruleType_	= RTL865X_ACL_MAC;
				rule->actionType_	= RTL865X_ACL_PERMIT;		
				rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
				rule->direction_ = RTL865X_ACL_INGRESS;	
				retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
				kfree(rule);
				if(retval!=SUCCESS)
					goto FINISH;	
				break;
			case RTL865X_ACL_MAC_FILTER_ALL_DROP:
				rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
				if(!rule)
				{				
					printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
					retval = FAILED;
					goto FINISH;
				}		
				memset(rule, 0,sizeof(rtl865x_AclRule_t));		
				rule->ruleType_	= RTL865X_ACL_MAC;
				rule->typeLen_ 		= 0x0800;
				rule->typeLenMask_ 	= 0xFFFF;
				rule->actionType_	= RTL865X_ACL_DROP;		
				rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
				rule->direction_ = RTL865X_ACL_INGRESS;	
				retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
				kfree(rule);
				if(retval!=SUCCESS)
					goto FINISH;

				rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
				if(!rule)
				{				
					printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
					retval = FAILED;
					goto FINISH;
				}		
				memset(rule, 0,sizeof(rtl865x_AclRule_t));		
				rule->ruleType_	= RTL865X_ACL_MAC;
				rule->actionType_	= RTL865X_ACL_PERMIT;		
				rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
				rule->direction_ = RTL865X_ACL_INGRESS;	
				retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
				kfree(rule);
				if(retval!=SUCCESS)
					goto FINISH;	
				break;
				
		}
		#endif
	}

	/*  egress  */	
	rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
	if(!rule)
	{				
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		retval = FAILED;
		goto FINISH;
	}
	memset(rule, 0,sizeof(rtl865x_AclRule_t));		
	rule->ruleType_	= RTL865X_ACL_MAC;
	rule->actionType_	= out_def_action;		
	rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
	rule->direction_ = RTL865X_ACL_EGRESS;		
	retval = rtl865x_add_acl(rule, NULL, RTL865X_ACL_DEFAULT);
	if(retval!=SUCCESS)
		goto FINISH;

	retval = rtl865x_rewrite_filterchain_into_acl();
FINISH:
	return retval;

}

int rtl865x_change_acl_mode(int mode)
{
	if(		mode!=RTL865X_ACL_Mode_Always_Permit 
		&& mode!=RTL865X_ACL_Mode_Always_Trap
		&& mode!=RTL865X_ACL_Mode_Normal)
		return FAILED;	

	acl_mode = mode;

	return rtl865x_change_acl_default_action();
}

int rtl865x_get_acl_mode(void)
{
	return acl_mode;
}


#ifndef CONFIG_RTL8676_Dynamic_ACL
int rtl865x_change_acl_mac_filter_mode(int mode)
{
	if(		mode!=RTL865X_ACL_MAC_FILTER_ALL_PERMIT 
		&& mode!=RTL865X_ACL_MAC_FILTER_IN_DROP_OUT_PERMIT
		&& mode!=RTL865X_ACL_MAC_FILTER_IN_PERMIT_OUT_DROP
		&& mode!=RTL865X_ACL_MAC_FILTER_ALL_DROP)
		return FAILED;	

	mac_filter_mode = mode;

	return rtl865x_change_acl_default_action();
}

int rtl865x_get_acl_mac_filter_mode(void)
{
	return mac_filter_mode;
}
#endif

#ifdef CONFIG_RTL8676_Dynamic_ACL
int rtl865x_add_filter_L2_permit(unsigned char* src_mac,unsigned char* dst_mac, char *in_ifname)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	xt_rule_to_acl_t *list_node=NULL;
	rtl865x_AclRule_t *rule=NULL;	
	xt_rule_to_acl_t *match2acl;


	//printk("Enter %s (src mac:%02X:%02X:%02X:%02X:%02X:%02X    dst mac:%02X:%02X:%02X:%02X:%02X:%02X)\n",__func__
	//		,src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5]
	//		,dst_mac[0],dst_mac[1],dst_mac[2],dst_mac[3],dst_mac[4],dst_mac[5]);

	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");		
	if(!ipt2aclTbl)
	{	
		ipt2aclTbl = rtl865x_create_new_tbl(RTL865X_ACL_USER_USED,"filter");
		if(!ipt2aclTbl)		
			goto FailToAdd;
	}


	//duplicate check
	list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_PERMIT],list)
	{			
		if(!memcmp(match2acl->aclRule->srcMac_.octet,src_mac,ETH_ALEN) && !memcmp(match2acl->aclRule->dstMac_.octet,dst_mac,ETH_ALEN))
			return SUCCESS;
	}
	

	list_node = kmalloc(sizeof(xt_rule_to_acl_t),GFP_KERNEL);	
	if(!list_node)
	{
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto FailToAdd;
	}
		
	rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
	if(!rule)
	{		
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto FailToAdd;
	}
	memset(list_node, 0,sizeof(xt_rule_to_acl_t));
	memset(rule, 0,sizeof(rtl865x_AclRule_t));
	rule->ruleType_ = RTL865X_ACL_MAC;
	rule->actionType_	= RTL865X_ACL_PERMIT;		
	rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
	rule->direction_ = RTL865X_ACL_INGRESS;
	memcpy(rule->srcMac_.octet, src_mac, ETH_ALEN);
	memcpy(rule->dstMac_.octet, dst_mac, ETH_ALEN);
	memset(rule->srcMacMask_.octet, 0xff, ETH_ALEN);
	memset(rule->dstMacMask_.octet, 0xff, ETH_ALEN);
	list_node->aclRule = rule;
	strcpy(list_node->iniface, in_ifname);
	#if 0
	#ifdef CONFIG_RTL_MULTI_ETH_WAN
	if (in_ifname[0] != 'n')
		strcpy(list_node->iniface,RTL_DRV_LAN_NETIF_NAME);
	else
		strcpy(list_node->iniface, in_ifname);
	#else
	strcpy(list_node->iniface,RTL_DRV_LAN_NETIF_NAME);
	#endif
	#endif
	list_add_tail(&list_node->list,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_PERMIT]);	

	if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
		goto FailToAdd_2;
	else
	{
		//printk("(%s)add succcess! (src mac:%02X:%02X:%02X:%02X:%02X:%02X    dst mac:%02X:%02X:%02X:%02X:%02X:%02X)\n",__func__
		//	,src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5]
		//	,dst_mac[0],dst_mac[1],dst_mac[2],dst_mac[3],dst_mac[4],dst_mac[5]);
		return SUCCESS;
	}

FailToAdd_2:
	list_del(&list_node->list);
FailToAdd:
	if(list_node)
		kfree(list_node);
	if(rule);
		kfree(rule);	
		
	return FAILED;	

}
int rtl865x_del_filter_L2_permit(unsigned char* del_mac)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	xt_rule_to_acl_t *match2acl;
	xt_rule_to_acl_t *nxt;
	int findout=false;
	
	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");		
	if(!ipt2aclTbl)
	{	
		printk("BUG!! (%s) filter tbl has not been created yet (at line %d)\n",__func__,__LINE__);
		return SUCCESS;
	}	

	list_for_each_entry_safe(match2acl,nxt,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_PERMIT],list)
	{			
		if(!memcmp(match2acl->aclRule->srcMac_.octet,del_mac,ETH_ALEN) || !memcmp(match2acl->aclRule->dstMac_.octet,del_mac,ETH_ALEN))
		{
			findout=true;
			list_del(&match2acl->list);
			kfree(match2acl->aclRule);
			kfree(match2acl);
		}
		
	}

	if(findout==true)
	{
#if defined(CONFIG_ETHWAN) && defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		if (heavyTraffic) {
			printk("\033[1m\033[40;33mWARNING! (%s) traffic busy, postponed to rewrite acl.\033[0m\n", __func__);
			return SUCCESS;
		}
#endif
		if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
			return FAILED;
		else
		{
			#ifdef CONFIG_HARDWARE_NAT_DEBUG
			printk("(%s)del from acl L2 permit : (mac:%02X:%02X:%02X:%02X:%02X:%02X)\n",__func__
				,del_mac[0],del_mac[1],del_mac[2],del_mac[3],del_mac[4],del_mac[5]);
			#endif
			return SUCCESS;
		}
	}
	else
		return SUCCESS;

}
int rtl865x_clean_filter_L2_permit(void)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");		
	if(!ipt2aclTbl)
	{	
		printk("BUG!! (%s) filter tbl has not been created yet (at line %d)\n",__func__,__LINE__);
		return SUCCESS;
	}	

	rtl865x_free_chain_inIpt2Acl_tbl(&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_PERMIT]);

	if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
		return FAILED;
	else
		return SUCCESS;
	
}
int rtl865x_add_filter_L34_permit(__u32 src_ip,__u16 src_port,__u32 dst_ip,__u16 dst_port,__u8 protocol,char* in_netifname)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	xt_rule_to_acl_t *list_node=NULL;
	rtl865x_AclRule_t *rule=NULL;
	xt_rule_to_acl_t *match2acl;

	//printk("Enter %s : add into acl L34 permit : (src ip:%u.%u.%u.%u:%u  dst ip:%u.%u.%u.%u:%u  in_if:%s  protocol:%s)\n",__func__
	//		,NIPQUAD(src_ip),src_port,NIPQUAD(dst_ip),dst_port,in_netifname,protocol==IPPROTO_TCP?"TCP":"UDP");

	if( protocol!=IPPROTO_TCP && protocol!=IPPROTO_UDP )	
		goto FailToAdd;
	

	if(!rtl865x_netifExist(in_netifname))  /*  the interface is in asic netif table  */		
		goto FailToAdd;
	

	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");		
	if(!ipt2aclTbl)
	{	
		ipt2aclTbl = rtl865x_create_new_tbl(RTL865X_ACL_USER_USED,"filter");
		if(!ipt2aclTbl)		
			goto FailToAdd;		
	}

	//duplicate check
	list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_PERMIT],list)
	{
		if(protocol==IPPROTO_TCP && match2acl->aclRule->ruleType_==RTL865X_ACL_TCP)
		{
			if( match2acl->aclRule->srcIpAddr_==src_ip &&  match2acl->aclRule->tcpSrcPortLB_==src_port && match2acl->aclRule->tcpSrcPortUB_==src_port &&
			    match2acl->aclRule->dstIpAddr_==dst_ip &&  match2acl->aclRule->tcpDstPortLB_==dst_port && match2acl->aclRule->tcpDstPortUB_==dst_port)
				return SUCCESS;
		}

		if(protocol==IPPROTO_UDP && match2acl->aclRule->ruleType_==RTL865X_ACL_UDP)
		{
			if( match2acl->aclRule->srcIpAddr_==src_ip &&  match2acl->aclRule->udpSrcPortLB_==src_port && match2acl->aclRule->udpSrcPortUB_==src_port &&
			    match2acl->aclRule->dstIpAddr_==dst_ip &&  match2acl->aclRule->udpDstPortLB_==dst_port && match2acl->aclRule->udpDstPortUB_==dst_port)
				return SUCCESS;
		}
	}

	list_node = kmalloc(sizeof(xt_rule_to_acl_t),GFP_KERNEL);	
	if(!list_node)
	{
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto FailToAdd;
	}
		
	rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
	if(!rule)
	{		
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto FailToAdd;
	}
	memset(list_node, 0,sizeof(xt_rule_to_acl_t));
	memset(rule, 0,sizeof(rtl865x_AclRule_t));

	rule->actionType_	= RTL865X_ACL_PERMIT;		
	rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
	rule->direction_ = RTL865X_ACL_INGRESS;
	
	if(protocol==IPPROTO_TCP)
	{
		rule->ruleType_ = RTL865X_ACL_TCP;
		rule->srcIpAddr_ 		= src_ip;
		rule->srcIpAddrMask_	= 0xFFFFFFFF;
		rule->dstIpAddr_		= dst_ip;
		rule->dstIpAddrMask_	= 0xFFFFFFFF;
		rule->tcpSrcPortLB_	= src_port;
		rule->tcpSrcPortUB_	= src_port;
		rule->tcpDstPortLB_	= dst_port;
		rule->tcpDstPortUB_	= dst_port;	
	}
	else if (protocol==IPPROTO_UDP)
	{
		rule->ruleType_ = RTL865X_ACL_UDP;	
		rule->srcIpAddr_ 		= src_ip;
		rule->srcIpAddrMask_	= 0xFFFFFFFF;
		rule->dstIpAddr_		= dst_ip;
		rule->dstIpAddrMask_	= 0xFFFFFFFF;
		rule->udpSrcPortLB_	= src_port;
		rule->udpSrcPortUB_	= src_port;
		rule->udpDstPortLB_	= dst_port;
		rule->udpDstPortUB_	= dst_port;
	}
	
	list_node->aclRule = rule;
	strcpy(list_node->iniface,in_netifname);
	list_add_tail(&list_node->list,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_PERMIT]);	

	if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
		goto FailToAdd_2;
	else
	{
		#ifdef CONFIG_HARDWARE_NAT_DEBUG
		printk("(%s)add into acl L34 permit : (src ip:%u.%u.%u.%u:%u  dst ip:%u.%u.%u.%u:%u  in_if:%s  protocol:%s)\n",__func__
			,NIPQUAD(src_ip),src_port,NIPQUAD(dst_ip),dst_port,in_netifname,protocol==IPPROTO_TCP?"TCP":"UDP");
		#endif
		return SUCCESS;
	}

FailToAdd_2:
	list_del(&list_node->list);

FailToAdd:
	if(list_node)
		kfree(list_node);
	if(rule);
		kfree(rule);	
	return FAILED;
}
int rtl865x_del_filter_L34_permit(__u32 src_ip,__u16 src_port,__u32 dst_ip,__u16 dst_port,__u8 protocol)
{

	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	xt_rule_to_acl_t *match2acl;
	xt_rule_to_acl_t *nxt;
	int findout=false;
	
	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");		
	if(!ipt2aclTbl)
	{	
		printk("BUG!! (%s) filter tbl has not been created yet (at line %d)\n",__func__,__LINE__);
		return SUCCESS;
	}	

	list_for_each_entry_safe(match2acl,nxt,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_PERMIT],list)
	{			
		if(protocol==IPPROTO_TCP && match2acl->aclRule->ruleType_==RTL865X_ACL_TCP)
		{
			if( match2acl->aclRule->srcIpAddr_==src_ip &&  match2acl->aclRule->tcpSrcPortLB_==src_port && match2acl->aclRule->tcpSrcPortUB_==src_port &&
			    match2acl->aclRule->dstIpAddr_==dst_ip &&  match2acl->aclRule->tcpDstPortLB_==dst_port && match2acl->aclRule->tcpDstPortUB_==dst_port)
			{
				list_del(&match2acl->list);
				kfree(match2acl->aclRule);
				kfree(match2acl);
				findout=true;
			}
		}

		if(protocol==IPPROTO_UDP && match2acl->aclRule->ruleType_==RTL865X_ACL_UDP)
		{
			if( match2acl->aclRule->srcIpAddr_==src_ip &&  match2acl->aclRule->udpSrcPortLB_==src_port && match2acl->aclRule->udpSrcPortUB_==src_port &&
			    match2acl->aclRule->dstIpAddr_==dst_ip &&  match2acl->aclRule->udpDstPortLB_==dst_port && match2acl->aclRule->udpDstPortUB_==dst_port)
			{
				list_del(&match2acl->list);
				kfree(match2acl->aclRule);
				kfree(match2acl);
				findout=true;
			}
		}
		
	}

	if(findout==true)
	{
#if defined(CONFIG_ETHWAN) && defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		if (heavyTraffic) {
			printk("\033[1m\033[40;33mWARNING! (%s) traffic busy, postponed to rewrite acl.\033[0m\n", __func__);
			return SUCCESS;
		}
#endif
		if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
			return FAILED;
		else
		{
			#ifdef CONFIG_HARDWARE_NAT_DEBUG
			printk("(%s)del from acl L34 permit : (src ip:%u.%u.%u.%u:%u  dst ip:%u.%u.%u.%u:%u   protocol:%s)\n",__func__
				,NIPQUAD(src_ip),src_port,NIPQUAD(dst_ip),dst_port,protocol==IPPROTO_TCP?"TCP":"UDP");
			#endif
			return SUCCESS;
		}
	}
	else {
		//printk("(%s)del from acl L34 permit failed : (src ip:%u.%u.%u.%u:%u  dst ip:%u.%u.%u.%u:%u   protocol:%s)\n",__func__
		//	,NIPQUAD(src_ip),src_port,NIPQUAD(dst_ip),dst_port,protocol==IPPROTO_TCP?"TCP":"UDP");
		return SUCCESS;
	}
}
int rtl865x_clean_filter_L34_permit(void)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");		
	if(!ipt2aclTbl)
	{	
		printk("BUG!! (%s) filter tbl has not been created yet (at line %d)\n",__func__,__LINE__);
		return SUCCESS;
	}	

	rtl865x_free_chain_inIpt2Acl_tbl(&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_PERMIT]);

	if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
		return FAILED;
	else
		return SUCCESS;
	
}
#endif /* CONFIG_RTL8676_Dynamic_ACL */
#endif

#ifdef CONFIG_RTL_MULTI_ETH_WAN
extern int32 rtl865x_getAsicArpEntry(ipaddr_t ip,rtl865x_tblAsicDrv_arpParam_t *asicArpEntry);
#if 0
int rtl865x_add_filter_L2_redirect(unsigned char* src_mac,unsigned char* dst_mac,char *in_ifname,char *out_ifname)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	xt_rule_to_acl_t *list_node=NULL;
	rtl865x_AclRule_t *rule=NULL;	
	xt_rule_to_acl_t *match2acl;
	rtl865x_tblAsicDrv_l2Param_t asic_l2;
	char outiface[IFNAMSIZ];
	uint32 idx, vid, fid, rowIdx, columIdx;

	//printk("Enter %s (src mac:%02X:%02X:%02X:%02X:%02X:%02X	 dst mac:%02X:%02X:%02X:%02X:%02X:%02X)\n",__func__
	//		,src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5]
	//		,dst_mac[0],dst_mac[1],dst_mac[2],dst_mac[3],dst_mac[4],dst_mac[5]);

	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter"); 	
	if(!ipt2aclTbl)
	{	
		ipt2aclTbl = rtl865x_create_new_tbl(RTL865X_ACL_USER_USED,"filter");
		if(!ipt2aclTbl)
			goto FailToAdd;
	}

	//duplicate check
	list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_REDIRECT],list)
	{			
		if(!memcmp(match2acl->aclRule->srcMac_.octet,src_mac,ETH_ALEN) && !memcmp(match2acl->aclRule->dstMac_.octet,dst_mac,ETH_ALEN))
			return SUCCESS;
	}
	
	list_node = kmalloc(sizeof(xt_rule_to_acl_t),GFP_KERNEL);	
	if(!list_node)
	{
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto FailToAdd;
	}
		
	rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
	if(!rule)
	{		
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto FailToAdd;
	}
	memset(list_node, 0,sizeof(xt_rule_to_acl_t));
	memset(rule, 0,sizeof(rtl865x_AclRule_t));
	rule->ruleType_ = RTL865X_ACL_MAC;
	rule->actionType_	= RTL865X_ACL_REDIRECT_ETHER;		
	rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;//RTL865X_ACL_ONLY_L2;
	rule->direction_ = RTL865X_ACL_INGRESS;
	memcpy(rule->srcMac_.octet, src_mac, ETH_ALEN);
	memcpy(rule->dstMac_.octet, dst_mac, ETH_ALEN);
	memset(rule->srcMacMask_.octet, 0xff, ETH_ALEN);
	memset(rule->dstMacMask_.octet, 0xff, ETH_ALEN);

	if (out_ifname[0] != 'n')
		strcpy(outiface, RTL_DRV_LAN_NETIF_NAME);
	else
		strcpy(outiface, out_ifname);
	if (rtl865x_getNetifIdx(outiface, &idx) != SUCCESS) {
		printk("%s %d get netif index fail.\n", __func__, __LINE__);
		goto FailToAdd;
	}
	rule->netifIdx_ = idx;

	if (rtl865x_getNetifVid(outiface, &vid) != SUCCESS) {
		printk("%s %d get netif vid fail.\n", __func__, __LINE__);
		goto FailToAdd;
	}
	rtl865x_getVlanFilterDatabaseId(vid,&fid);
	rowIdx = rtl8651_filterDbIndex( dst_mac, fid);
	if (rtl865x_Lookup_fdb_entry(fid, dst_mac, FDB_DYNAMIC, &columIdx,&asic_l2) != SUCCESS) {
		printk("%s %d get filter DBindex fail.\n", __func__, __LINE__);
		goto FailToAdd;
	}
	rule->L2Idx_ = (rowIdx<<2)|columIdx;


	list_node->aclRule = rule;
	if (in_ifname[0] != 'n')
		strcpy(list_node->iniface,RTL_DRV_LAN_NETIF_NAME);
	else
		strcpy(list_node->iniface, in_ifname);
	list_add_tail(&list_node->list,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_REDIRECT]); 

	if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
		goto FailToAdd_2;
	else
	{
		printk("(%s)add succcess! (src mac:%02X:%02X:%02X:%02X:%02X:%02X	dst mac:%02X:%02X:%02X:%02X:%02X:%02X)\n",__func__
			,src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5]
			,dst_mac[0],dst_mac[1],dst_mac[2],dst_mac[3],dst_mac[4],dst_mac[5]);
		return SUCCESS;
	}

FailToAdd:
	if(list_node)
		kfree(list_node);
	if(rule);
		kfree(rule);	
FailToAdd_2:
	return FAILED;	

}

int rtl865x_del_filter_L2_redirect(unsigned char* del_mac)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	xt_rule_to_acl_t *match2acl;
	xt_rule_to_acl_t *nxt;
	int findout=false;
	
	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter"); 	
	if(!ipt2aclTbl)
	{	
		printk("BUG!! (%s) filter tbl has not been created yet (at line %d)\n",__func__,__LINE__);
		return SUCCESS;
	}	

	list_for_each_entry_safe(match2acl,nxt,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_REDIRECT],list)
	{
		if(!memcmp(match2acl->aclRule->srcMac_.octet,del_mac,ETH_ALEN) || !memcmp(match2acl->aclRule->dstMac_.octet,del_mac,ETH_ALEN))
		{
			findout=true;
			list_del(&match2acl->list);
			kfree(match2acl);
		}
		
	}

	if(findout==true)
	{	
		if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
			return FAILED;
		else
		{
			printk("(%s)del from acl L2 permit : (mac:%02X:%02X:%02X:%02X:%02X:%02X)\n",__func__
				,del_mac[0],del_mac[1],del_mac[2],del_mac[3],del_mac[4],del_mac[5]);
			return SUCCESS;
		}
	}
	else
		return SUCCESS;

}
#endif
#ifdef CONFIG_RTL8676_Dynamic_ACL
int rtl865x_add_filter_L34_redirect(__u32 int_ip,__u16 int_port,__u32 rem_ip,__u16 rem_port,__u32 ext_ip,__u8 protocol,__u8 napt_enable,char *in_ifname,struct  dst_entry* dst)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	xt_rule_to_acl_t *list_node=NULL;
	rtl865x_AclRule_t *rule=NULL;
	xt_rule_to_acl_t *match2acl;
//	rtl865x_route_t rt;
	//rtl865x_ppp_t *ppp;
	//rtl865x_ip_entry_t *ip;
	//int nextHopIdx;
#if 0
	rtl865x_tblAsicDrv_routingParam_t asic_l3;
	rtl865x_tblAsicDrv_l2Param_t asic_l2;
	rtl865x_tblAsicDrv_arpParam_t asicArpEntry;
	rtl865x_tblAsicDrv_nextHopParam_t nxtHopEntry;
	int32 pppIdx, fid, columIdx, rowIdx;
#endif
//	char *out_ifname=NULL;

	//printk("Enter %s : add into acl L34 redirect : (src ip:%u.%u.%u.%u:%u  dst ip:%u.%u.%u.%u:%u  in_if:%s	protocol:%s)\n",__func__
	//		,NIPQUAD(src_ip),src_port,NIPQUAD(dst_ip),dst_port,in_ifname,protocol==IPPROTO_TCP?"TCP":"UDP");

	if( protocol!=IPPROTO_TCP && protocol!=IPPROTO_UDP )
		goto FailToAdd;

#if 0 
	if (!in_ifname) {
		if (rtl865x_getRouteEntry(int_ip, &rt) != SUCCESS) {
			printk("%s %d get route entry for %x fail.\n", __func__, __LINE__, int_ip);
			goto FailToAdd;
		}
		in_ifname = rt.dstNetif->name;
	}

	if (rtl865x_getRouteEntry(rem_ip, &rt) != SUCCESS) {
		printk("%s %d get route entry for %x fail.\n", __func__, __LINE__, rem_ip);
		goto FailToAdd;
	}
	out_ifname = rt.dstNetif->name;

	//printk("%s %d in_netifname:%s out_netifname:%s\n", __func__, __LINE__, in_ifname, out_ifname);
#endif
	
	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");
	if(!ipt2aclTbl)
	{
		ipt2aclTbl = rtl865x_create_new_tbl(RTL865X_ACL_USER_USED,"filter");
		if(!ipt2aclTbl)
			goto FailToAdd;
	}

	//duplicate check
	list_for_each_entry(match2acl,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_REDIRECT],list)
	{
		if(protocol==IPPROTO_TCP && match2acl->aclRule->ruleType_==RTL865X_ACL_TCP)
		{
			if( match2acl->aclRule->srcIpAddr_==int_ip &&  match2acl->aclRule->tcpSrcPortLB_==int_port && match2acl->aclRule->tcpSrcPortUB_==int_port &&
				match2acl->aclRule->dstIpAddr_==rem_ip &&  match2acl->aclRule->tcpDstPortLB_==rem_port && match2acl->aclRule->tcpDstPortUB_==rem_port)
				return SUCCESS;
		}

		if(protocol==IPPROTO_UDP && match2acl->aclRule->ruleType_==RTL865X_ACL_UDP)
		{
			if( match2acl->aclRule->srcIpAddr_==int_ip &&  match2acl->aclRule->udpSrcPortLB_==int_port && match2acl->aclRule->udpSrcPortUB_==int_port &&
				match2acl->aclRule->dstIpAddr_==rem_ip &&  match2acl->aclRule->udpDstPortLB_==rem_port && match2acl->aclRule->udpDstPortUB_==rem_port)
				return SUCCESS;
		}
	}

	list_node = kmalloc(sizeof(xt_rule_to_acl_t),GFP_KERNEL);
	if(!list_node)
	{
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto FailToAdd;
	}
	
	rule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_KERNEL);
	if(!rule)
	{
		printk("\n!!!!!!%s(%d): No memory freed for kmalloc!!!",__FUNCTION__,__LINE__);
		goto FailToAdd;
	}
	memset(list_node, 0,sizeof(xt_rule_to_acl_t));
	memset(rule, 0,sizeof(rtl865x_AclRule_t));
	/* redirect to ethernet or pppoe will cause SNAT fail. */
#if 0 /* the original */
	if (out_ifname[0] == 'p')//pppoe interface
		rule->actionType_ = RTL865X_ACL_REDIRECT_PPPOE;
	else
		rule->actionType_ = RTL865X_ACL_REDIRECT_ETHER;
#else
	rule->actionType_ = RTL865X_ACL_DEFAULT_REDIRECT;
#endif
	rule->pktOpApp_ 	= RTL865X_ACL_ALL_LAYER;
	rule->direction_ = RTL865X_ACL_INGRESS;
	
	if(protocol==IPPROTO_TCP)
	{
		rule->ruleType_ = RTL865X_ACL_TCP;
		rule->srcIpAddr_		= int_ip;
		rule->srcIpAddrMask_	= 0xFFFFFFFF;
		rule->dstIpAddr_		= rem_ip;
		rule->dstIpAddrMask_	= 0xFFFFFFFF;
		rule->tcpSrcPortLB_ = int_port;
		rule->tcpSrcPortUB_ = int_port;
		rule->tcpDstPortLB_ = rem_port;
		rule->tcpDstPortUB_ = rem_port; 
	}
	else
	{
		rule->ruleType_ = RTL865X_ACL_UDP;	
		rule->srcIpAddr_		= int_ip;
		rule->srcIpAddrMask_	= 0xFFFFFFFF;
		rule->dstIpAddr_		= rem_ip;
		rule->dstIpAddrMask_	= 0xFFFFFFFF;
		rule->udpSrcPortLB_ = int_port;
		rule->udpSrcPortUB_ = int_port;
		rule->udpDstPortLB_ = rem_port;
		rule->udpDstPortUB_ = rem_port;
	}

#if 0  /* the original */
#if 0  /* the original */
	if (rt.process == RT_PPPOE)
	{
		/* If portmapping enable, l3 routing table is empty. */
#if 1
		rule->netifIdx_ = rt.dstNetif->asicIdx;
		rtl865x_getPppIdx(rt.un.pppoe.pppInfo, &pppIdx);
		rule->pppoeIdx_ = pppIdx;
		
		rtl865x_getVlanFilterDatabaseId(rt.dstNetif->vid,&fid);
		rowIdx = rtl8651_filterDbIndex( &rt.un.pppoe.pppInfo->server_mac, fid);
		rtl865x_Lookup_fdb_entry(fid, &rt.un.pppoe.pppInfo->server_mac, FDB_DYNAMIC, &columIdx,&asic_l2);
		rule->L2Idx_ = (rowIdx<<2)|columIdx;
#else
		if (rtl8651_getAsicRouting(rt.asicIdx, &asic_l3) == FAILED)
			goto FailToAdd;
		rule->netifIdx_ = asic_l3.vidx;
		rule->pppoeIdx_ = asic_l3.pppoeIdx;
		rule->L2Idx_ = (asic_l3.nextHopRow<<2)|asic_l3.nextHopColumn;
#endif
	}
	else if (rt.process == RT_ARP)
	{
		rule->netifIdx_ = rt.dstNetif->asicIdx;
		if (rtl865x_getAsicArpEntry(rem_ip, &asicArpEntry) != SUCCESS) {
			printk("%s %d get arp entry for %x fail.\n", __func__, __LINE__, rem_ip);
			goto FailToAdd;
		}
		rule->L2Idx_ = (asicArpEntry.nextHopRow<<2)|asicArpEntry.nextHopColumn;
	}
	else if (rt.process == RT_NEXTHOP)
	{
		if (rtl8651_getAsicNextHopTable(rt.un.nxthop.nxtHopSta, &nxtHopEntry) != SUCCESS) {
			printk("%s %d get next hop for %d fail.\n", __func__, __LINE__, rt.un.nxthop.nxtHopSta);
			goto FailToAdd;
		}
		rule->netifIdx_ = nxtHopEntry.dvid;
		rule->pppoeIdx_ = nxtHopEntry.pppoeIdx;
		rule->L2Idx_ = (nxtHopEntry.nextHopRow<<2)|nxtHopEntry.nextHopColumn;
	}
#else
	if (rt.process == RT_PPPOE)
	{
		int nextHopIdx;
		/* get session ID from pppoe table */
		rtl865x_ppp_t *ppp = rtl865x_getPppByNetifName(rt.dstNetif->name);		
		if (!ppp)
			goto FailToAdd;
		if ((nextHopIdx=rtl865x_getNxtHopIdx(NEXTHOP_DEFREDIRECT_ACL, rt.dstNetif, ppp->sessionId,ext_ip)) == -1)
		rule->nexthopIdx_ = nextHopIdx;
	}
	else if (rt.process == RT_ARP)
	{
		int ret;
		//ipaddr_t extIP=0;
		int nextHopIdx;
		/* need to add nexthop entry */
		if ((nextHopIdx = rtl865x_getNxtHopIdx(NEXTHOP_DEFREDIRECT_ACL, rt.dstNetif, rem_ip,ext_ip)) == -1) {
			#if 0
			rtl865x_ip_entry_t* ip = rtl865x_getIpEntryByIdx(rt.un.arp.arpIpIdx);
			if (!ip && napt_enable) {
				printk("%s %d rtl865x_getIpEntryByIdx fail.\n", __func__, __LINE__);
				goto FailToAdd;
			}

			if (napt_enable)
				extIP = ip->extIp;
			#endif
			if ((ret=rtl865x_addNxtHop(NEXTHOP_DEFREDIRECT_ACL, NULL, rt.dstNetif, rem_ip, ext_ip)) != SUCCESS) {
				printk("%s %d rtl865x_addNxtHop fail(errno=%d).\n", __func__, __LINE__, ret);
				goto FailToAdd;
			}

			nextHopIdx = rtl865x_getNxtHopIdx(NEXTHOP_DEFREDIRECT_ACL, rt.dstNetif, rem_ip,ext_ip);
		}
		rule->nexthopIdx_ = nextHopIdx;
	}
	else if (rt.process == RT_NEXTHOP)
	{
		rule->nexthopIdx_ = rt.un.nxthop.nxtHopSta;
	}
#endif
#endif

	/* check output netif */
	if(!dst->neighbour || !dst->neighbour->dev)
		goto FailToAdd;
	else
	{
		rtl865x_netif_local_t *netIf;		
		netIf = _rtl865x_getSWNetifByName(dst->neighbour->dev->name);
		if(!netIf)
			goto FailToAdd;
		
		if(netIf->if_type == IF_PPPOE)
		{
			int nextHopIdx;
			rtl865x_ppp_t *ppp;
			
			/* get session ID from pppoe table */
			ppp = rtl865x_getPppByNetifName(netIf->name);		
			if (!ppp)
				goto FailToAdd;
			
			nextHopIdx=rtl865x_getNxtHopIdx(NEXTHOP_DEFREDIRECT_ACL, netIf, ppp->sessionId,ext_ip);
			if(nextHopIdx==-1)
				goto FailToAdd;
			
			rule->nexthopIdx_ = nextHopIdx;			
		}
		else if(netIf->if_type == IF_ETHER)
		{
			int nextHopIdx;
			u32 nexthop_ip;
			
			read_lock_bh(&dst->neighbour->lock);			
			nexthop_ip = *(u32*)dst->neighbour->primary_key;
			read_unlock_bh(&dst->neighbour->lock);
			
			if ((nextHopIdx = rtl865x_getNxtHopIdx(NEXTHOP_DEFREDIRECT_ACL, netIf, nexthop_ip,ext_ip)) == -1)
			{
				if ( rtl865x_addNxtHop(NEXTHOP_DEFREDIRECT_ACL, NULL, netIf, nexthop_ip, ext_ip) != SUCCESS) 
					goto FailToAdd;
			}		
			
			nextHopIdx = rtl865x_getNxtHopIdx(NEXTHOP_DEFREDIRECT_ACL, netIf, nexthop_ip,ext_ip);
			if(nextHopIdx==-1)
				goto FailToAdd;
			
			rule->nexthopIdx_ = nextHopIdx;
		}
		else
			goto FailToAdd;
	}
	list_node->aclRule = rule;
	strcpy(list_node->iniface,in_ifname);
	list_add_tail(&list_node->list,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_REDIRECT]); 

	if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
		goto FailToAdd_2;
	else
	{
		//printk("(%s)add into acl L34 redirect : (src ip:%u.%u.%u.%u:%u  dst ip:%u.%u.%u.%u:%u  in_if:%s  protocol:%s)\n",__func__
		//	,NIPQUAD(src_ip),src_port,NIPQUAD(dst_ip),dst_port,in_ifname,protocol==IPPROTO_TCP?"TCP":"UDP");
		return SUCCESS;
	}
FailToAdd_2:
	list_del(&list_node->list);

FailToAdd:
	if(list_node)
		kfree(list_node);
	if(rule);
		kfree(rule);	
	return FAILED;
}

int rtl865x_del_filter_L34_redirect(__u32 src_ip,__u16 src_port,__u32 dst_ip,__u16 dst_port,__u8 protocol)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	xt_rule_to_acl_t *match2acl;
	xt_rule_to_acl_t *nxt;
	int findout=false;

	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter"); 	
	if(!ipt2aclTbl)
	{	
		printk("BUG!! (%s) filter tbl has not been created yet (at line %d)\n",__func__,__LINE__);
		return SUCCESS;
	}	

	list_for_each_entry_safe(match2acl,nxt,&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_REDIRECT],list)
	{			
		if(protocol==IPPROTO_TCP && match2acl->aclRule->ruleType_==RTL865X_ACL_TCP)
		{
			if( match2acl->aclRule->srcIpAddr_==src_ip &&  match2acl->aclRule->tcpSrcPortLB_==src_port && match2acl->aclRule->tcpSrcPortUB_==src_port &&
				match2acl->aclRule->dstIpAddr_==dst_ip &&  match2acl->aclRule->tcpDstPortLB_==dst_port && match2acl->aclRule->tcpDstPortUB_==dst_port)
			{
				list_del(&match2acl->list);
				kfree(match2acl->aclRule);
				kfree(match2acl);
				findout=true;
			}
		}

		if(protocol==IPPROTO_UDP && match2acl->aclRule->ruleType_==RTL865X_ACL_UDP)
		{
			if( match2acl->aclRule->srcIpAddr_==src_ip &&  match2acl->aclRule->udpSrcPortLB_==src_port && match2acl->aclRule->udpSrcPortUB_==src_port &&
				match2acl->aclRule->dstIpAddr_==dst_ip &&  match2acl->aclRule->udpDstPortLB_==dst_port && match2acl->aclRule->udpDstPortUB_==dst_port)
			{
				list_del(&match2acl->list);
				kfree(match2acl->aclRule);
				kfree(match2acl);
				findout=true;
			}
		}
		
	}

	if(findout==true)
	{
#if defined(CONFIG_ETHWAN) && defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		if (heavyTraffic) {
			printk("\033[1m\033[40;33mWARNING! (%s) traffic busy, postponed to rewrite acl.\033[0m\n", __func__);
			return SUCCESS;
		}
#endif
		if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
			return FAILED;
		else
		{
			//printk("(%s)del from acl L34 permit : (src ip:%u.%u.%u.%u:%u  dst ip:%u.%u.%u.%u:%u   protocol:%s)\n",__func__
			//	,NIPQUAD(src_ip),src_port,NIPQUAD(dst_ip),dst_port,protocol==IPPROTO_TCP?"TCP":"UDP");
			return SUCCESS;
		}
	}
	else
		return SUCCESS;
}

int rtl865x_del_filter_rule_by_interface(char *in_ifname)
{
	rtl865x_iptRule2Acl_tbl *ipt2aclTbl=NULL;
	xt_rule_to_acl_t *match2acl,*nxt;
	struct list_head *listHead;
	int i;

	ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");		
	if(!ipt2aclTbl)
	{	
		printk("BUG!! (%s) filter tbl has not been created yet (at line %d)\n",__func__,__LINE__);
		return SUCCESS;
	}	

	printk("%s %d %s\n", __func__, __LINE__, in_ifname);
	//rtl865x_free_chain_inIpt2Acl_tbl(&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_PERMIT]);
	for (i=RTL865x_CHAINLIST_PRIORITY_LEVEL_L2_UNI_PERMIT; i<=RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_PERMIT; i++) {
		if (i == RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_TRAP)
			continue;
		
		listHead = &ipt2aclTbl->chainList[i];
		list_for_each_entry_safe(match2acl,nxt, listHead, list)
		{
			if (!strcmp(match2acl->iniface, in_ifname)) {
				list_del(&match2acl->list);
				kfree(match2acl->aclRule);
				kfree(match2acl);
			}
		}
	}
	

	if(rtl865x_rewrite_filterchain_into_acl()!=SUCCESS)
		return FAILED;
	else
		return SUCCESS;
	
}
#endif
#endif

#if defined(CONFIG_RTL_HARDWARE_NAT)
#ifndef CONFIG_RTL_MULTI_ETH_WAN
static int rtl_get_masquerade_netif(struct xt_table_info *private)
{
	void *table_base;
	struct ipt_entry *e, *back;
	struct ipt_entry_target *t;
	char masq_name[IFNAMSIZ]={'\0'};
	
	if(masq_entry && masq_entry->ip.outiface[0] !='\0')
	{			
		memcpy(masq_if, masq_entry->ip.outiface, IFNAMSIZ);
		return 0;
	}
	else	
	{
		table_base = private->entries[smp_processor_id()];
		e = get_entry(table_base, private->hook_entry[NF_INET_POST_ROUTING]);		
		back = get_entry(table_base, private->underflow[NF_INET_POST_ROUTING]);		
		
		//clear masq_name;
		memset(masq_name,0,IFNAMSIZ);		
		while(e)
		{
		
			if(e == masq_entry)
				break;
			
			//record the entry's outif name
			if(e->ip.outiface[0] !='\0')
				memcpy(masq_name,e->ip.outiface,IFNAMSIZ);

			//target
			t = ipt_get_target(e);
			IP_NF_ASSERT(t->u.kernel.target);

			#if 0
			{					
				printk("%s:%d:t->u.kernel.target->name is %s,e(%p)\n",__FUNCTION__,__LINE__,t->u.kernel.target->name,e);
				printk("%s:%d:e->src(%u.%u.%u.%u),e->dst(%u.%u.%u.%u),e->nextoffset(%d)\n",__FUNCTION__,
					__LINE__,NIPQUAD(e->ip.src.s_addr),NIPQUAD(e->ip.dst.s_addr),e->next_offset);
			}
			#endif

			//if error target
			if(strcmp(t->u.kernel.target->name, "ERROR") == 0)
			{
				memset(masq_name,0,IFNAMSIZ);
				break;
			}
			
			/* Standard target? */
			if (!t->u.kernel.target->target) 
			{
				int v;
				v = ((struct ipt_standard_target *)t)->verdict;				
				
				if (v < 0 )
				{
					if(v == IPT_RETURN)
					{
						e = back;
						back = get_entry(table_base, back->comefrom);						
					}
					else
					{
						e = (void *)e + e->next_offset;
					}

					continue;				
				}

				//jump ?
				//printk("%s:%d:t->u.kernel.target->name is %s,v(%d)\n",__FUNCTION__,__LINE__,t->u.kernel.target->name,v);
				if (table_base + v != (void *)e + e->next_offset
				    && !(e->ip.flags & IPT_F_GOTO)) {
					/* Save old back ptr in next entry */
					struct ipt_entry *next
						= (void *)e + e->next_offset;
					next->comefrom
						= (void *)back - table_base;
					/* set back pointer to next entry */
					back = next;
				}
				e = get_entry(table_base, v);
				continue;
			} 
			
			/*user define target?*/
			e = (void *)e + e->next_offset;
		}		
		
	}

	memcpy(masq_if,masq_name,IFNAMSIZ);

	return 0;
}

static int rtl_check_for_masquerade_entry(struct ipt_entry *e,  
	unsigned char *base,
	const char *name,
	unsigned int size,
	const unsigned int valid_hooks,
	const unsigned int *hook_entries)
{
	struct ipt_entry_target *t;
	unsigned int hook;
	int ret = 0;
	
	t = ipt_get_target(e);	
	if ( !t) 
	{		
		goto err;
	}

#if 0
	if(t->u.kernel.target)
	{
		goto err;
	}
#endif

	hook = get_hookNum(e,base,valid_hooks,hook_entries);		
	if ((hook == NF_INET_POST_ROUTING) &&
		((strcmp(t->u.kernel.target->name, "MASQUERADE") == 0)||(strcmp(t->u.kernel.target->name, "SNAT") == 0))) 
		{
			masq_entry =  e;
			//printk("====%s(%d),bingo!! masq_entry found!entry(%p)\n",__FUNCTION__,__LINE__,masq_entry);
		}
 err:	
	return ret;
}

/* check is MASQURADE rule exist. */
static int rtl_check_for_extern_ip(const char *name,
		unsigned int valid_hooks,
		struct xt_table_info *newinfo,
		void *entry0,
		unsigned int size,
		unsigned int number,
		const unsigned int *hook_entries,
		const unsigned int *underflows)
{
	masq_entry = NULL;
	memset(masq_if,0,IFNAMSIZ);
	IPT_ENTRY_ITERATE(entry0, size,rtl_check_for_masquerade_entry, entry0,name,size,valid_hooks,hook_entries);
	
	//found masq entry
	if(masq_entry)
	{	
		rtl_get_masquerade_netif(newinfo);
	}
	
	if(masq_if[0] !='\0')	
	{
		struct net_device *dev;
		struct in_ifaddr *ina;
		int rc;
		
		dev = __dev_get_by_name(&init_net, masq_if);

		if ((dev)&&(dev->ip_ptr)) { 		
			
			ina=(struct in_ifaddr *)(((struct in_device *)(dev->ip_ptr))->ifa_list);
			if (ina!=NULL) 
			{
				
				//if (hw_napt_ip == ina->ifa_local)
				//	return 0;
				
				//delete old ip
				if(hw_napt_ip)
				{
					rc = 0;
					#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
					rc = rtl865x_delIp(hw_napt_ip);
					#endif
				
					if (rc == SUCCESS)
						hw_napt_ip = 0;
				}
				
				rc = 0;
				#ifdef CONFIG_RTL_LAYERED_DRIVER_L3				
				rc = rtl865x_addIp(0,(u32)(ina->ifa_local),IP_TYPE_NAPT,dev->name);
				#endif							

				if (rc == SUCCESS)
					hw_napt_ip = ina->ifa_local;
				#ifdef CONFIG_HARDWARE_NAT_DEBUG
				/*2007-12-19*/
				//printk("==========%s:%d:(%s): %u.%u.%u.%u, errno=%d\n", __FUNCTION__,__LINE__,"add_natip",NIPQUAD((ina->ifa_local)), rc);
				#endif
			}
		}
	}
	return 0;
}
#else
static int rtl_get_masquerade_netif(struct xt_table_info *private, int IPidx)
{
	void *table_base;
	struct ipt_entry *e, *back;
	struct ipt_entry_target *t;
	char masq_name[IFNAMSIZ]={'\0'};
	
	if(masq_entry[IPidx] && masq_entry[IPidx]->ip.outiface[0] !='\0')
	{			
		memcpy(masq_if[IPidx], masq_entry[IPidx]->ip.outiface, IFNAMSIZ);
		return 0;
	}
	else
	{
		table_base = private->entries[smp_processor_id()];
		e = get_entry(table_base, private->hook_entry[NF_INET_POST_ROUTING]);		
		back = get_entry(table_base, private->underflow[NF_INET_POST_ROUTING]);		
		
		//clear masq_name;
		memset(masq_name,0,IFNAMSIZ);		
		while(e)
		{
		
			if(e == masq_entry[IPidx])
				break;
			
			//record the entry's outif name
			if(e->ip.outiface[0] !='\0')
				memcpy(masq_name,e->ip.outiface,IFNAMSIZ);

			//target
			t = ipt_get_target(e);
			IP_NF_ASSERT(t->u.kernel.target);

			#if 0
			{					
				printk("%s:%d:t->u.kernel.target->name is %s,e(%p)\n",__FUNCTION__,__LINE__,t->u.kernel.target->name,e);
				printk("%s:%d:e->src(%u.%u.%u.%u),e->dst(%u.%u.%u.%u),e->nextoffset(%d)\n",__FUNCTION__,
					__LINE__,NIPQUAD(e->ip.src.s_addr),NIPQUAD(e->ip.dst.s_addr),e->next_offset);
			}
			#endif

			//if error target
			if(strcmp(t->u.kernel.target->name, "ERROR") == 0)
			{
				memset(masq_name,0,IFNAMSIZ);
				break;
			}
			
			/* Standard target? */
			if (!t->u.kernel.target->target) 
			{
				int v;
				v = ((struct ipt_standard_target *)t)->verdict;				
				
				if (v < 0 )
				{
					if(v == IPT_RETURN)
					{
						e = back;
						back = get_entry(table_base, back->comefrom);						
					}
					else
					{
						e = (void *)e + e->next_offset;
					}

					continue;				
				}

				//jump ?
				//printk("%s:%d:t->u.kernel.target->name is %s,v(%d)\n",__FUNCTION__,__LINE__,t->u.kernel.target->name,v);
				if (table_base + v != (void *)e + e->next_offset
				    && !(e->ip.flags & IPT_F_GOTO)) {
					/* Save old back ptr in next entry */
					struct ipt_entry *next
						= (void *)e + e->next_offset;
					next->comefrom
						= (void *)back - table_base;
					/* set back pointer to next entry */
					back = next;
				}
				e = get_entry(table_base, v);
				continue;
			} 
			
			/*user define target?*/
			e = (void *)e + e->next_offset;
		}		
		
	}

	memcpy(masq_if[IPidx],masq_name,IFNAMSIZ);

	return 0;
}

static int rtl_check_for_masquerade_entry(struct ipt_entry *e,  
	unsigned char *base,
	const char *name,
	unsigned int size,
	const unsigned int valid_hooks,
	const unsigned int *hook_entries)
{
	struct ipt_entry_target *t;
	unsigned int hook;
	int ret = 0;
	
	t = ipt_get_target(e);	
	if ( !t) 
	{		
		goto err;
	}

#if 0
	if(t->u.kernel.target)
	{
		goto err;
	}
#endif

	hook = get_hookNum(e,base,valid_hooks,hook_entries);		
	if ((hook == NF_INET_POST_ROUTING) &&
		((strcmp(t->u.kernel.target->name, "MASQUERADE") == 0)||(strcmp(t->u.kernel.target->name, "SNAT") == 0))) 
		{
			masq_entry[masq_index++] =  e;
			//printk("====%s(%d),bingo!! masq_entry found!entry(%p)\n",__FUNCTION__,__LINE__,masq_entry);
		}
 err:	
	return ret;
}

/* check is MASQURADE rule exist. */
static int rtl_check_for_extern_ip(const char *name,
		unsigned int valid_hooks,
		struct xt_table_info *newinfo,
		void *entry0,
		unsigned int size,
		unsigned int number,
		const unsigned int *hook_entries,
		const unsigned int *underflows)
{
	int i;

	//printk("%s %d enter=======================>\n", __func__, __LINE__);
	for (i=0; i<IP_NUMBER; i++)
	{
		int rc;
		
		masq_entry[i] = NULL;
		memset(masq_if[i], 0, IFNAMSIZ);
		
		//delete old ip
		if(hw_napt_ip[i])
		{
			rc = 0;
	#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
			rc = rtl865x_delIp(hw_napt_ip[i]);
	#endif
			if (rc == SUCCESS)
				hw_napt_ip[i] = 0;
		}
	}
	masq_index = 0;
	
	IPT_ENTRY_ITERATE(entry0, size,rtl_check_for_masquerade_entry, entry0,name,size,valid_hooks,hook_entries);
	
	//found masq entry
	for (i=0; i<masq_index; i++) {
		if(masq_entry[i])
		{
			rtl_get_masquerade_netif(newinfo, i);
		}

		//printk("IPidx %d masq_if: %s\n", i, masq_if[i]);
		if(masq_if[i][0] !='\0')	
		{
			struct net_device *dev;
			struct in_ifaddr *ina;
			int rc;
			
			dev = __dev_get_by_name(&init_net, masq_if[i]);

			if ((dev)&&(dev->ip_ptr)) { 		
				
				ina=(struct in_ifaddr *)(((struct in_device *)(dev->ip_ptr))->ifa_list);
				if (ina!=NULL) 
				{
					
					//if (hw_napt_ip == ina->ifa_local)
					//	return 0;
					
					rc = 0;
					#ifdef CONFIG_RTL_LAYERED_DRIVER_L3				
					rc = rtl865x_addIp(0,(u32)(ina->ifa_local),IP_TYPE_NAPT,dev->name);
					#endif							

					if (rc == SUCCESS)
						hw_napt_ip[i] = ina->ifa_local;
					#ifdef CONFIG_HARDWARE_NAT_DEBUG
					/*2007-12-19*/
					printk("==========%s:%d:(%s): %u.%u.%u.%u, errno=%d\n", __FUNCTION__,__LINE__,"add_natip",NIPQUAD((ina->ifa_local)), rc);
					#endif
				}
			}
		}
	}
	//printk("=======================>%s %d exit.\n", __func__, __LINE__);
	return 0;
}
#endif
#endif //endif CONFIG_RTL_HARDWARE_NAT

/* Checks and translates the user-supplied table segment (held in
   newinfo) */
static int
translate_table(const char *name,
		unsigned int valid_hooks,
		struct xt_table_info *newinfo,
		void *entry0,
		unsigned int size,
		unsigned int number,
		const unsigned int *hook_entries,
		const unsigned int *underflows)
{
	unsigned int i;
	int ret;
	newinfo->size = size;
	newinfo->number = number;
	/* Init all hooks to impossible value. */
	for (i = 0; i < NF_INET_NUMHOOKS; i++) {
		newinfo->hook_entry[i] = 0xFFFFFFFF;
		newinfo->underflow[i] = 0xFFFFFFFF;
	}

	duprintf("translate_table: size %u\n", newinfo->size);
	i = 0;
	/* Walk through entries, checking offsets. */
	ret = IPT_ENTRY_ITERATE(entry0, newinfo->size,
				check_entry_size_and_hooks,
				newinfo,
				entry0,
				entry0 + size,
				hook_entries, underflows, &i);
	if (ret != 0)
		return ret;

	if (i != number) {
		duprintf("translate_table: %u not %u entries\n",
			 i, number);
		return -EINVAL;
	}
	/* Check hooks all assigned */
	for (i = 0; i < NF_INET_NUMHOOKS; i++) {
		/* Only hooks which are valid */
		if (!(valid_hooks & (1 << i)))
			continue;
		if (newinfo->hook_entry[i] == 0xFFFFFFFF) {
			duprintf("Invalid hook entry %u %u\n",
				 i, hook_entries[i]);
			return -EINVAL;
		}
		if (newinfo->underflow[i] == 0xFFFFFFFF) {
			duprintf("Invalid underflow %u %u\n",
				 i, underflows[i]);
			return -EINVAL;
		}
	}	
	if (!mark_source_chains(newinfo, valid_hooks, entry0))
		return -ELOOP;
	/* Finally, each sanity check must pass */
	i = 0;
	ret = IPT_ENTRY_ITERATE(entry0, newinfo->size,
				find_check_entry, name, size, &i);
	if (ret != 0) {
		IPT_ENTRY_ITERATE(entry0, newinfo->size,
				cleanup_entry, &i);
		return ret;
	}

#if defined(CONFIG_RTL_HARDWARE_NAT)	
	//hyking:check masquerade and add ip
	//QL BUG: if no out dev assigned in masquerade rule, then masq_entry and masq_if will not match.
	if(strcmp(name,"nat") == 0)
		rtl_check_for_extern_ip(name,valid_hooks,newinfo,entry0,size,number,hook_entries,underflows);
#endif

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
	{
		rtl865x_iptRule2Acl_tbl *tbl;
//		int i,ret;
		int32_t priority = 1024;

		tbl = rtl865x_get_ipt2Acl_tbl(name);
		if(tbl==NULL)
		{
		
		//#ifdef CONFIG_RTL_IPTABLES_RULE_2_ACL_FILTER	
			if(memcmp(name,"filter",strlen("filter")) == 0)
				priority = RTL865X_ACL_USER_USED;
		//#endif
		//#ifdef CONFIG_RTL_HW_QOS_SUPPORT
			if(memcmp(name,RTL865X_QOS_TABLE_NAME,RTL865X_QOS_TABLE_LEN) == 0)
				priority = RTL865X_ACL_QOS_USED2;
		//#endif	

			if(priority < 1024)
			{
				tbl = rtl865x_create_new_tbl(priority,name);	
				#if 0
				tbl = kmalloc(sizeof(rtl865x_iptRule2Acl_tbl), GFP_KERNEL);
				if(!tbl)
					return 0;
				memset(tbl, 0, sizeof(rtl865x_iptRule2Acl_tbl));
				for(i = 0; i < RTL865X_CHAINLIST_NUMBER_PER_TBL; i++)
				{
					INIT_LIST_HEAD(&tbl->chainList[i]);
				}
				tbl->priority = priority;
				memcpy(tbl->tblName,name,strlen(name));
				tbl->tblName[strlen(name)] = '\0';

				ret = rtl865x_register_ipt2Acl_tbl(tbl);
				
				if(ret != 0)
				{
					kfree(tbl);
				}			
				#endif
			}	
		}
		if(tbl != NULL)		
			translate_ipTblRules2Acl(name,valid_hooks,newinfo,entry0,size,number,hook_entries,underflows);
	}
#endif

	/* And one copy for every other CPU */
	for_each_possible_cpu(i) {
		if (newinfo->entries[i] && newinfo->entries[i] != entry0)
			memcpy(newinfo->entries[i], entry0, newinfo->size);
	}
	return ret;
}

/* Gets counters. */
static inline int
add_entry_to_counter(const struct ipt_entry *e,
		     struct xt_counters total[],
		     unsigned int *i)
{
	ADD_COUNTER(total[*i], e->counters.bcnt, e->counters.pcnt);

	(*i)++;
	return 0;
}

static inline int
set_entry_to_counter(const struct ipt_entry *e,
		     struct ipt_counters total[],
		     unsigned int *i)
{
	SET_COUNTER(total[*i], e->counters.bcnt, e->counters.pcnt);

	(*i)++;
	return 0;
}

static void
get_counters(const struct xt_table_info *t,
	     struct xt_counters counters[])
{
	unsigned int cpu;
	unsigned int i;
	unsigned int curcpu;

	/* Instead of clearing (by a previous call to memset())
	 * the counters and using adds, we set the counters
	 * with data used by 'current' CPU.
	 *
	 * Bottom half has to be disabled to prevent deadlock
	 * if new softirq were to run and call ipt_do_table
	 */
	local_bh_disable();
	curcpu = smp_processor_id();

	i = 0;
	IPT_ENTRY_ITERATE(t->entries[curcpu],
			  t->size,
			  set_entry_to_counter,
			  counters,
			  &i);

	for_each_possible_cpu(cpu) {
		if (cpu == curcpu)
			continue;
		i = 0;
		xt_info_wrlock(cpu);
		IPT_ENTRY_ITERATE(t->entries[cpu],
				  t->size,
				  add_entry_to_counter,
				  counters,
				  &i);
		xt_info_wrunlock(cpu);
	}
	local_bh_enable();
}

static struct xt_counters * alloc_counters(struct xt_table *table)
{
	unsigned int countersize;
	struct xt_counters *counters;
	struct xt_table_info *private = table->private;

	/* We need atomic snapshot of counters: rest doesn't change
	   (other than comefrom, which userspace doesn't care
	   about). */
	countersize = sizeof(struct xt_counters) * private->number;
	counters = vmalloc_node(countersize, numa_node_id());

	if (counters == NULL)
		return ERR_PTR(-ENOMEM);

	get_counters(private, counters);

	return counters;
}

static int
copy_entries_to_user(unsigned int total_size,
		     struct xt_table *table,
		     void __user *userptr)
{
	unsigned int off, num;
	struct ipt_entry *e;
	struct xt_counters *counters;
	const struct xt_table_info *private = table->private;
	int ret = 0;
	const void *loc_cpu_entry;

	counters = alloc_counters(table);
	if (IS_ERR(counters))
		return PTR_ERR(counters);

	/* choose the copy that is on our node/cpu, ...
	 * This choice is lazy (because current thread is
	 * allowed to migrate to another cpu)
	 */
	loc_cpu_entry = private->entries[raw_smp_processor_id()];
	if (copy_to_user(userptr, loc_cpu_entry, total_size) != 0) {
		ret = -EFAULT;
		goto free_counters;
	}

	/* FIXME: use iterator macros --RR */
	/* ... then go back and fix counters and names */
	for (off = 0, num = 0; off < total_size; off += e->next_offset, num++){
		unsigned int i;
		const struct ipt_entry_match *m;
		const struct ipt_entry_target *t;

		e = (struct ipt_entry *)(loc_cpu_entry + off);
		if (copy_to_user(userptr + off
				 + offsetof(struct ipt_entry, counters),
				 &counters[num],
				 sizeof(counters[num])) != 0) {
			ret = -EFAULT;
			goto free_counters;
		}

		for (i = sizeof(struct ipt_entry);
		     i < e->target_offset;
		     i += m->u.match_size) {
			m = (void *)e + i;

			if (copy_to_user(userptr + off + i
					 + offsetof(struct ipt_entry_match,
						    u.user.name),
					 m->u.kernel.match->name,
					 strlen(m->u.kernel.match->name)+1)
			    != 0) {
				ret = -EFAULT;
				goto free_counters;
			}
		}

		t = ipt_get_target(e);
		if (copy_to_user(userptr + off + e->target_offset
				 + offsetof(struct ipt_entry_target,
					    u.user.name),
				 t->u.kernel.target->name,
				 strlen(t->u.kernel.target->name)+1) != 0) {
			ret = -EFAULT;
			goto free_counters;
		}
	}

 free_counters:
	vfree(counters);
	return ret;
}

#ifdef CONFIG_COMPAT
static void compat_standard_from_user(void *dst, void *src)
{
	int v = *(compat_int_t *)src;

	if (v > 0)
		v += xt_compat_calc_jump(AF_INET, v);
	memcpy(dst, &v, sizeof(v));
}

static int compat_standard_to_user(void __user *dst, void *src)
{
	compat_int_t cv = *(int *)src;

	if (cv > 0)
		cv -= xt_compat_calc_jump(AF_INET, cv);
	return copy_to_user(dst, &cv, sizeof(cv)) ? -EFAULT : 0;
}

static inline int
compat_calc_match(struct ipt_entry_match *m, int *size)
{
	*size += xt_compat_match_offset(m->u.kernel.match);
	return 0;
}

static int compat_calc_entry(struct ipt_entry *e,
			     const struct xt_table_info *info,
			     void *base, struct xt_table_info *newinfo)
{
	struct ipt_entry_target *t;
	unsigned int entry_offset;
	int off, i, ret;

	off = sizeof(struct ipt_entry) - sizeof(struct compat_ipt_entry);
	entry_offset = (void *)e - base;
	IPT_MATCH_ITERATE(e, compat_calc_match, &off);
	t = ipt_get_target(e);
	off += xt_compat_target_offset(t->u.kernel.target);
	newinfo->size -= off;
	ret = xt_compat_add_offset(AF_INET, entry_offset, off);
	if (ret)
		return ret;

	for (i = 0; i < NF_INET_NUMHOOKS; i++) {
		if (info->hook_entry[i] &&
		    (e < (struct ipt_entry *)(base + info->hook_entry[i])))
			newinfo->hook_entry[i] -= off;
		if (info->underflow[i] &&
		    (e < (struct ipt_entry *)(base + info->underflow[i])))
			newinfo->underflow[i] -= off;
	}
	return 0;
}

static int compat_table_info(const struct xt_table_info *info,
			     struct xt_table_info *newinfo)
{
	void *loc_cpu_entry;

	if (!newinfo || !info)
		return -EINVAL;

	/* we dont care about newinfo->entries[] */
	memcpy(newinfo, info, offsetof(struct xt_table_info, entries));
	newinfo->initial_entries = 0;
	loc_cpu_entry = info->entries[raw_smp_processor_id()];
	return IPT_ENTRY_ITERATE(loc_cpu_entry, info->size,
				 compat_calc_entry, info, loc_cpu_entry,
				 newinfo);
}
#endif

static int get_info(struct net *net, void __user *user, int *len, int compat)
{
	char name[IPT_TABLE_MAXNAMELEN];
	struct xt_table *t;
	int ret;

	if (*len != sizeof(struct ipt_getinfo)) {
		duprintf("length %u != %zu\n", *len,
			 sizeof(struct ipt_getinfo));
		return -EINVAL;
	}

	if (copy_from_user(name, user, sizeof(name)) != 0)
		return -EFAULT;

	name[IPT_TABLE_MAXNAMELEN-1] = '\0';
#ifdef CONFIG_COMPAT
	if (compat)
		xt_compat_lock(AF_INET);
#endif
	t = try_then_request_module(xt_find_table_lock(net, AF_INET, name),
				    "iptable_%s", name);
	if (t && !IS_ERR(t)) {
		struct ipt_getinfo info;
		const struct xt_table_info *private = t->private;

#ifdef CONFIG_COMPAT
		if (compat) {
			struct xt_table_info tmp;
			ret = compat_table_info(private, &tmp);
			xt_compat_flush_offsets(AF_INET);
			private = &tmp;
		}
#endif
		info.valid_hooks = t->valid_hooks;
		memcpy(info.hook_entry, private->hook_entry,
		       sizeof(info.hook_entry));
		memcpy(info.underflow, private->underflow,
		       sizeof(info.underflow));
		info.num_entries = private->number;
		info.size = private->size;
		strcpy(info.name, name);

		if (copy_to_user(user, &info, *len) != 0)
			ret = -EFAULT;
		else
			ret = 0;

		xt_table_unlock(t);
		module_put(t->me);
	} else
		ret = t ? PTR_ERR(t) : -ENOENT;
#ifdef CONFIG_COMPAT
	if (compat)
		xt_compat_unlock(AF_INET);
#endif
	return ret;
}

static int
get_entries(struct net *net, struct ipt_get_entries __user *uptr, int *len)
{
	int ret;
	struct ipt_get_entries get;
	struct xt_table *t;

	if (*len < sizeof(get)) {
		duprintf("get_entries: %u < %zu\n", *len, sizeof(get));
		return -EINVAL;
	}
	if (copy_from_user(&get, uptr, sizeof(get)) != 0)
		return -EFAULT;
	if (*len != sizeof(struct ipt_get_entries) + get.size) {
		duprintf("get_entries: %u != %zu\n",
			 *len, sizeof(get) + get.size);
		return -EINVAL;
	}

	t = xt_find_table_lock(net, AF_INET, get.name);
	if (t && !IS_ERR(t)) {
		const struct xt_table_info *private = t->private;
		duprintf("t->private->number = %u\n", private->number);
		if (get.size == private->size)
			ret = copy_entries_to_user(private->size,
						   t, uptr->entrytable);
		else {
			duprintf("get_entries: I've got %u not %u!\n",
				 private->size, get.size);
			ret = -EAGAIN;
		}
		module_put(t->me);
		xt_table_unlock(t);
	} else
		ret = t ? PTR_ERR(t) : -ENOENT;

	return ret;
}

static int
__do_replace(struct net *net, const char *name, unsigned int valid_hooks,
	     struct xt_table_info *newinfo, unsigned int num_counters,
	     void __user *counters_ptr)
{
	int ret;
	struct xt_table *t;
	struct xt_table_info *oldinfo;
	struct xt_counters *counters;
	void *loc_cpu_old_entry;

	ret = 0;
	counters = vmalloc(num_counters * sizeof(struct xt_counters));
	if (!counters) {
		ret = -ENOMEM;
		goto out;
	}

	t = try_then_request_module(xt_find_table_lock(net, AF_INET, name),
				    "iptable_%s", name);
	if (!t || IS_ERR(t)) {
		ret = t ? PTR_ERR(t) : -ENOENT;
		goto free_newinfo_counters_untrans;
	}

	/* You lied! */
	if (valid_hooks != t->valid_hooks) {
		duprintf("Valid hook crap: %08X vs %08X\n",
			 valid_hooks, t->valid_hooks);
		ret = -EINVAL;
		goto put_module;
	}

	oldinfo = xt_replace_table(t, num_counters, newinfo, &ret);
	if (!oldinfo)
		goto put_module;

	/* Update module usage count based on number of rules */
	duprintf("do_replace: oldnum=%u, initnum=%u, newnum=%u\n",
		oldinfo->number, oldinfo->initial_entries, newinfo->number);
	if ((oldinfo->number > oldinfo->initial_entries) ||
	    (newinfo->number <= oldinfo->initial_entries))
		module_put(t->me);
	if ((oldinfo->number > oldinfo->initial_entries) &&
	    (newinfo->number <= oldinfo->initial_entries))
		module_put(t->me);

	/* Get the old counters, and synchronize with replace */
	get_counters(oldinfo, counters);

	/* Decrease module usage counts and free resource */
	loc_cpu_old_entry = oldinfo->entries[raw_smp_processor_id()];
	IPT_ENTRY_ITERATE(loc_cpu_old_entry, oldinfo->size, cleanup_entry,
			  NULL);
	xt_free_table_info(oldinfo);
	if (copy_to_user(counters_ptr, counters,
			 sizeof(struct xt_counters) * num_counters) != 0)
		ret = -EFAULT;
	vfree(counters);
	xt_table_unlock(t);
	return ret;

 put_module:
	module_put(t->me);
	xt_table_unlock(t);
 free_newinfo_counters_untrans:
	vfree(counters);
 out:
	return ret;
}

static int
do_replace(struct net *net, void __user *user, unsigned int len)
{
	int ret;
	struct ipt_replace tmp;
	struct xt_table_info *newinfo;
	void *loc_cpu_entry;

	if (copy_from_user(&tmp, user, sizeof(tmp)) != 0)
		return -EFAULT;

	/* overflow check */
	if (tmp.num_counters >= INT_MAX / sizeof(struct xt_counters))
		return -ENOMEM;

	newinfo = xt_alloc_table_info(tmp.size);
	if (!newinfo)
		return -ENOMEM;

	/* choose the copy that is on our node/cpu */
	loc_cpu_entry = newinfo->entries[raw_smp_processor_id()];
	if (copy_from_user(loc_cpu_entry, user + sizeof(tmp),
			   tmp.size) != 0) {
		ret = -EFAULT;
		goto free_newinfo;
	}
#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
	/*firstly, free all iptblAcl which added last time*/
	establish_rule_permit = 0;
	#ifdef CONFIG_RTL8676_Dynamic_ACL
	if(!strcmp(tmp.name,"filter"))
	{
		rtl865x_iptRule2Acl_tbl *ipt2aclTbl = NULL;
		ipt2aclTbl = rtl865x_get_ipt2Acl_tbl("filter");
		if(ipt2aclTbl)
			rtl865x_free_chain_inIpt2Acl_tbl(&ipt2aclTbl->chainList[RTL865x_CHAINLIST_PRIORITY_LEVEL_L3_TRAP]);
	}
	else
		rtl865x_free_allchains_inIpt2Acl_tbl(tmp.name);
	#else
	rtl865x_free_allchains_inIpt2Acl_tbl(tmp.name);
	#endif
#endif
	ret = translate_table(tmp.name, tmp.valid_hooks,
			      newinfo, loc_cpu_entry, tmp.size, tmp.num_entries,
			      tmp.hook_entry, tmp.underflow);
	if (ret != 0)
		goto free_newinfo;

	duprintf("ip_tables: Translated table\n");

	ret = __do_replace(net, tmp.name, tmp.valid_hooks, newinfo,
			   tmp.num_counters, tmp.counters);
	if (ret)
		goto free_newinfo_untrans;
	
#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
	//rtl865x_rearrange_iptblAcl();
	//rtl865x_print_iptRule2Acl_tbl();
	rtl865x_rearrange_ipt2Acl_tbl();
#endif

	return 0;

 free_newinfo_untrans:
	IPT_ENTRY_ITERATE(loc_cpu_entry, newinfo->size, cleanup_entry, NULL);
 free_newinfo:
 	
#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
	rtl865x_free_allchains_inIpt2Acl_tbl(tmp.name);
#endif

	xt_free_table_info(newinfo);
	return ret;
}

/* We're lazy, and add to the first CPU; overflow works its fey magic
 * and everything is OK. */
static int
add_counter_to_entry(struct ipt_entry *e,
		     const struct xt_counters addme[],
		     unsigned int *i)
{
	ADD_COUNTER(e->counters, addme[*i].bcnt, addme[*i].pcnt);

	(*i)++;
	return 0;
}

static int
do_add_counters(struct net *net, void __user *user, unsigned int len, int compat)
{
	unsigned int i, curcpu;
	struct xt_counters_info tmp;
	struct xt_counters *paddc;
	unsigned int num_counters;
	const char *name;
	int size;
	void *ptmp;
	struct xt_table *t;
	const struct xt_table_info *private;
	int ret = 0;
	void *loc_cpu_entry;
#ifdef CONFIG_COMPAT
	struct compat_xt_counters_info compat_tmp;

	if (compat) {
		ptmp = &compat_tmp;
		size = sizeof(struct compat_xt_counters_info);
	} else
#endif
	{
		ptmp = &tmp;
		size = sizeof(struct xt_counters_info);
	}

	if (copy_from_user(ptmp, user, size) != 0)
		return -EFAULT;

#ifdef CONFIG_COMPAT
	if (compat) {
		num_counters = compat_tmp.num_counters;
		name = compat_tmp.name;
	} else
#endif
	{
		num_counters = tmp.num_counters;
		name = tmp.name;
	}

	if (len != size + num_counters * sizeof(struct xt_counters))
		return -EINVAL;

	paddc = vmalloc_node(len - size, numa_node_id());
	if (!paddc)
		return -ENOMEM;

	if (copy_from_user(paddc, user + size, len - size) != 0) {
		ret = -EFAULT;
		goto free;
	}

	t = xt_find_table_lock(net, AF_INET, name);
	if (!t || IS_ERR(t)) {
		ret = t ? PTR_ERR(t) : -ENOENT;
		goto free;
	}

	local_bh_disable();
	private = t->private;
	if (private->number != num_counters) {
		ret = -EINVAL;
		goto unlock_up_free;
	}

	i = 0;
	/* Choose the copy that is on our node */
	curcpu = smp_processor_id();
	loc_cpu_entry = private->entries[curcpu];
	xt_info_wrlock(curcpu);
	IPT_ENTRY_ITERATE(loc_cpu_entry,
			  private->size,
			  add_counter_to_entry,
			  paddc,
			  &i);
	xt_info_wrunlock(curcpu);
 unlock_up_free:
	local_bh_enable();
	xt_table_unlock(t);
	module_put(t->me);
 free:
	vfree(paddc);

	return ret;
}

#ifdef CONFIG_COMPAT
struct compat_ipt_replace {
	char			name[IPT_TABLE_MAXNAMELEN];
	u32			valid_hooks;
	u32			num_entries;
	u32			size;
	u32			hook_entry[NF_INET_NUMHOOKS];
	u32			underflow[NF_INET_NUMHOOKS];
	u32			num_counters;
	compat_uptr_t		counters;	/* struct ipt_counters * */
	struct compat_ipt_entry	entries[0];
};

static int
compat_copy_entry_to_user(struct ipt_entry *e, void __user **dstptr,
			  unsigned int *size, struct xt_counters *counters,
			  unsigned int *i)
{
	struct ipt_entry_target *t;
	struct compat_ipt_entry __user *ce;
	u_int16_t target_offset, next_offset;
	compat_uint_t origsize;
	int ret;

	ret = -EFAULT;
	origsize = *size;
	ce = (struct compat_ipt_entry __user *)*dstptr;
	if (copy_to_user(ce, e, sizeof(struct ipt_entry)))
		goto out;

	if (copy_to_user(&ce->counters, &counters[*i], sizeof(counters[*i])))
		goto out;

	*dstptr += sizeof(struct compat_ipt_entry);
	*size -= sizeof(struct ipt_entry) - sizeof(struct compat_ipt_entry);

	ret = IPT_MATCH_ITERATE(e, xt_compat_match_to_user, dstptr, size);
	target_offset = e->target_offset - (origsize - *size);
	if (ret)
		goto out;
	t = ipt_get_target(e);
	ret = xt_compat_target_to_user(t, dstptr, size);
	if (ret)
		goto out;
	ret = -EFAULT;
	next_offset = e->next_offset - (origsize - *size);
	if (put_user(target_offset, &ce->target_offset))
		goto out;
	if (put_user(next_offset, &ce->next_offset))
		goto out;

	(*i)++;
	return 0;
out:
	return ret;
}

static int
compat_find_calc_match(struct ipt_entry_match *m,
		       const char *name,
		       const struct ipt_ip *ip,
		       unsigned int hookmask,
		       int *size, unsigned int *i)
{
	struct xt_match *match;

	match = try_then_request_module(xt_find_match(AF_INET, m->u.user.name,
						      m->u.user.revision),
					"ipt_%s", m->u.user.name);
	if (IS_ERR(match) || !match) {
		duprintf("compat_check_calc_match: `%s' not found\n",
			 m->u.user.name);
		return match ? PTR_ERR(match) : -ENOENT;
	}
	m->u.kernel.match = match;
	*size += xt_compat_match_offset(match);

	(*i)++;
	return 0;
}

static int
compat_release_match(struct ipt_entry_match *m, unsigned int *i)
{
	if (i && (*i)-- == 0)
		return 1;

	module_put(m->u.kernel.match->me);
	return 0;
}

static int
compat_release_entry(struct compat_ipt_entry *e, unsigned int *i)
{
	struct ipt_entry_target *t;

	if (i && (*i)-- == 0)
		return 1;

	/* Cleanup all matches */
	COMPAT_IPT_MATCH_ITERATE(e, compat_release_match, NULL);
	t = compat_ipt_get_target(e);
	module_put(t->u.kernel.target->me);
	return 0;
}

static int
check_compat_entry_size_and_hooks(struct compat_ipt_entry *e,
				  struct xt_table_info *newinfo,
				  unsigned int *size,
				  unsigned char *base,
				  unsigned char *limit,
				  unsigned int *hook_entries,
				  unsigned int *underflows,
				  unsigned int *i,
				  const char *name)
{
	struct ipt_entry_target *t;
	struct xt_target *target;
	unsigned int entry_offset;
	unsigned int j;
	int ret, off, h;

	duprintf("check_compat_entry_size_and_hooks %p\n", e);
	if ((unsigned long)e % __alignof__(struct compat_ipt_entry) != 0
	    || (unsigned char *)e + sizeof(struct compat_ipt_entry) >= limit) {
		duprintf("Bad offset %p, limit = %p\n", e, limit);
		return -EINVAL;
	}

	if (e->next_offset < sizeof(struct compat_ipt_entry) +
			     sizeof(struct compat_xt_entry_target)) {
		duprintf("checking: element %p size %u\n",
			 e, e->next_offset);
		return -EINVAL;
	}

	/* For purposes of check_entry casting the compat entry is fine */
	ret = check_entry((struct ipt_entry *)e, name);
	if (ret)
		return ret;

	off = sizeof(struct ipt_entry) - sizeof(struct compat_ipt_entry);
	entry_offset = (void *)e - (void *)base;
	j = 0;
	ret = COMPAT_IPT_MATCH_ITERATE(e, compat_find_calc_match, name,
				       &e->ip, e->comefrom, &off, &j);
	if (ret != 0)
		goto release_matches;

	t = compat_ipt_get_target(e);
	target = try_then_request_module(xt_find_target(AF_INET,
							t->u.user.name,
							t->u.user.revision),
					 "ipt_%s", t->u.user.name);
	if (IS_ERR(target) || !target) {
		duprintf("check_compat_entry_size_and_hooks: `%s' not found\n",
			 t->u.user.name);
		ret = target ? PTR_ERR(target) : -ENOENT;
		goto release_matches;
	}
	t->u.kernel.target = target;

	off += xt_compat_target_offset(target);
	*size += off;
	ret = xt_compat_add_offset(AF_INET, entry_offset, off);
	if (ret)
		goto out;

	/* Check hooks & underflows */
	for (h = 0; h < NF_INET_NUMHOOKS; h++) {
		if ((unsigned char *)e - base == hook_entries[h])
			newinfo->hook_entry[h] = hook_entries[h];
		if ((unsigned char *)e - base == underflows[h])
			newinfo->underflow[h] = underflows[h];
	}

	/* Clear counters and comefrom */
	memset(&e->counters, 0, sizeof(e->counters));
	e->comefrom = 0;

	(*i)++;
	return 0;

out:
	module_put(t->u.kernel.target->me);
release_matches:
	IPT_MATCH_ITERATE(e, compat_release_match, &j);
	return ret;
}

static int
compat_copy_entry_from_user(struct compat_ipt_entry *e, void **dstptr,
			    unsigned int *size, const char *name,
			    struct xt_table_info *newinfo, unsigned char *base)
{
	struct ipt_entry_target *t;
	struct xt_target *target;
	struct ipt_entry *de;
	unsigned int origsize;
	int ret, h;

	ret = 0;
	origsize = *size;
	de = (struct ipt_entry *)*dstptr;
	memcpy(de, e, sizeof(struct ipt_entry));
	memcpy(&de->counters, &e->counters, sizeof(e->counters));

	*dstptr += sizeof(struct ipt_entry);
	*size += sizeof(struct ipt_entry) - sizeof(struct compat_ipt_entry);

	ret = COMPAT_IPT_MATCH_ITERATE(e, xt_compat_match_from_user,
				       dstptr, size);
	if (ret)
		return ret;
	de->target_offset = e->target_offset - (origsize - *size);
	t = compat_ipt_get_target(e);
	target = t->u.kernel.target;
	xt_compat_target_from_user(t, dstptr, size);

	de->next_offset = e->next_offset - (origsize - *size);
	for (h = 0; h < NF_INET_NUMHOOKS; h++) {
		if ((unsigned char *)de - base < newinfo->hook_entry[h])
			newinfo->hook_entry[h] -= origsize - *size;
		if ((unsigned char *)de - base < newinfo->underflow[h])
			newinfo->underflow[h] -= origsize - *size;
	}
	return ret;
}

static int
compat_check_entry(struct ipt_entry *e, const char *name,
				     unsigned int *i)
{
	struct xt_mtchk_param mtpar;
	unsigned int j;
	int ret;

	j = 0;
	mtpar.table     = name;
	mtpar.entryinfo = &e->ip;
	mtpar.hook_mask = e->comefrom;
	mtpar.family    = NFPROTO_IPV4;
	ret = IPT_MATCH_ITERATE(e, check_match, &mtpar, &j);
	if (ret)
		goto cleanup_matches;

	ret = check_target(e, name);
	if (ret)
		goto cleanup_matches;

	(*i)++;
	return 0;

 cleanup_matches:
	IPT_MATCH_ITERATE(e, cleanup_match, &j);
	return ret;
}

static int
translate_compat_table(const char *name,
		       unsigned int valid_hooks,
		       struct xt_table_info **pinfo,
		       void **pentry0,
		       unsigned int total_size,
		       unsigned int number,
		       unsigned int *hook_entries,
		       unsigned int *underflows)
{
	unsigned int i, j;
	struct xt_table_info *newinfo, *info;
	void *pos, *entry0, *entry1;
	unsigned int size;
	int ret;

	info = *pinfo;
	entry0 = *pentry0;
	size = total_size;
	info->number = number;

	/* Init all hooks to impossible value. */
	for (i = 0; i < NF_INET_NUMHOOKS; i++) {
		info->hook_entry[i] = 0xFFFFFFFF;
		info->underflow[i] = 0xFFFFFFFF;
	}

	duprintf("translate_compat_table: size %u\n", info->size);
	j = 0;
	xt_compat_lock(AF_INET);
	/* Walk through entries, checking offsets. */
	ret = COMPAT_IPT_ENTRY_ITERATE(entry0, total_size,
				       check_compat_entry_size_and_hooks,
				       info, &size, entry0,
				       entry0 + total_size,
				       hook_entries, underflows, &j, name);
	if (ret != 0)
		goto out_unlock;

	ret = -EINVAL;
	if (j != number) {
		duprintf("translate_compat_table: %u not %u entries\n",
			 j, number);
		goto out_unlock;
	}

	/* Check hooks all assigned */
	for (i = 0; i < NF_INET_NUMHOOKS; i++) {
		/* Only hooks which are valid */
		if (!(valid_hooks & (1 << i)))
			continue;
		if (info->hook_entry[i] == 0xFFFFFFFF) {
			duprintf("Invalid hook entry %u %u\n",
				 i, hook_entries[i]);
			goto out_unlock;
		}
		if (info->underflow[i] == 0xFFFFFFFF) {
			duprintf("Invalid underflow %u %u\n",
				 i, underflows[i]);
			goto out_unlock;
		}
	}

	ret = -ENOMEM;
	newinfo = xt_alloc_table_info(size);
	if (!newinfo)
		goto out_unlock;

	newinfo->number = number;
	for (i = 0; i < NF_INET_NUMHOOKS; i++) {
		newinfo->hook_entry[i] = info->hook_entry[i];
		newinfo->underflow[i] = info->underflow[i];
	}
	entry1 = newinfo->entries[raw_smp_processor_id()];
	pos = entry1;
	size = total_size;
	ret = COMPAT_IPT_ENTRY_ITERATE(entry0, total_size,
				       compat_copy_entry_from_user,
				       &pos, &size, name, newinfo, entry1);
	xt_compat_flush_offsets(AF_INET);
	xt_compat_unlock(AF_INET);
	if (ret)
		goto free_newinfo;

	ret = -ELOOP;
	if (!mark_source_chains(newinfo, valid_hooks, entry1))
		goto free_newinfo;

	i = 0;
	ret = IPT_ENTRY_ITERATE(entry1, newinfo->size, compat_check_entry,
				name, &i);
	if (ret) {
		j -= i;
		COMPAT_IPT_ENTRY_ITERATE_CONTINUE(entry0, newinfo->size, i,
						  compat_release_entry, &j);
		IPT_ENTRY_ITERATE(entry1, newinfo->size, cleanup_entry, &i);
		xt_free_table_info(newinfo);
		return ret;
	}

	/* And one copy for every other CPU */
	for_each_possible_cpu(i)
		if (newinfo->entries[i] && newinfo->entries[i] != entry1)
			memcpy(newinfo->entries[i], entry1, newinfo->size);

	*pinfo = newinfo;
	*pentry0 = entry1;
	xt_free_table_info(info);
	return 0;

free_newinfo:
	xt_free_table_info(newinfo);
out:
	COMPAT_IPT_ENTRY_ITERATE(entry0, total_size, compat_release_entry, &j);
	return ret;
out_unlock:
	xt_compat_flush_offsets(AF_INET);
	xt_compat_unlock(AF_INET);
	goto out;
}

static int
compat_do_replace(struct net *net, void __user *user, unsigned int len)
{
	int ret;
	struct compat_ipt_replace tmp;
	struct xt_table_info *newinfo;
	void *loc_cpu_entry;

	if (copy_from_user(&tmp, user, sizeof(tmp)) != 0)
		return -EFAULT;

	/* overflow check */
	if (tmp.size >= INT_MAX / num_possible_cpus())
		return -ENOMEM;
	if (tmp.num_counters >= INT_MAX / sizeof(struct xt_counters))
		return -ENOMEM;

	newinfo = xt_alloc_table_info(tmp.size);
	if (!newinfo)
		return -ENOMEM;

	/* choose the copy that is on our node/cpu */
	loc_cpu_entry = newinfo->entries[raw_smp_processor_id()];
	if (copy_from_user(loc_cpu_entry, user + sizeof(tmp),
			   tmp.size) != 0) {
		ret = -EFAULT;
		goto free_newinfo;
	}

	ret = translate_compat_table(tmp.name, tmp.valid_hooks,
				     &newinfo, &loc_cpu_entry, tmp.size,
				     tmp.num_entries, tmp.hook_entry,
				     tmp.underflow);
	if (ret != 0)
		goto free_newinfo;

	duprintf("compat_do_replace: Translated table\n");

	ret = __do_replace(net, tmp.name, tmp.valid_hooks, newinfo,
			   tmp.num_counters, compat_ptr(tmp.counters));
	if (ret)
		goto free_newinfo_untrans;
	return 0;

 free_newinfo_untrans:
	IPT_ENTRY_ITERATE(loc_cpu_entry, newinfo->size, cleanup_entry, NULL);
 free_newinfo:
	xt_free_table_info(newinfo);
	return ret;
}

static int
compat_do_ipt_set_ctl(struct sock *sk,	int cmd, void __user *user,
		      unsigned int len)
{
	int ret;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case IPT_SO_SET_REPLACE:
		ret = compat_do_replace(sock_net(sk), user, len);
		break;

	case IPT_SO_SET_ADD_COUNTERS:
		ret = do_add_counters(sock_net(sk), user, len, 1);
		break;

	default:
		duprintf("do_ipt_set_ctl:  unknown request %i\n", cmd);
		ret = -EINVAL;
	}

	return ret;
}

struct compat_ipt_get_entries {
	char name[IPT_TABLE_MAXNAMELEN];
	compat_uint_t size;
	struct compat_ipt_entry entrytable[0];
};

static int
compat_copy_entries_to_user(unsigned int total_size, struct xt_table *table,
			    void __user *userptr)
{
	struct xt_counters *counters;
	const struct xt_table_info *private = table->private;
	void __user *pos;
	unsigned int size;
	int ret = 0;
	const void *loc_cpu_entry;
	unsigned int i = 0;

	counters = alloc_counters(table);
	if (IS_ERR(counters))
		return PTR_ERR(counters);

	/* choose the copy that is on our node/cpu, ...
	 * This choice is lazy (because current thread is
	 * allowed to migrate to another cpu)
	 */
	loc_cpu_entry = private->entries[raw_smp_processor_id()];
	pos = userptr;
	size = total_size;
	ret = IPT_ENTRY_ITERATE(loc_cpu_entry, total_size,
				compat_copy_entry_to_user,
				&pos, &size, counters, &i);

	vfree(counters);
	return ret;
}

static int
compat_get_entries(struct net *net, struct compat_ipt_get_entries __user *uptr,
		   int *len)
{
	int ret;
	struct compat_ipt_get_entries get;
	struct xt_table *t;

	if (*len < sizeof(get)) {
		duprintf("compat_get_entries: %u < %zu\n", *len, sizeof(get));
		return -EINVAL;
	}

	if (copy_from_user(&get, uptr, sizeof(get)) != 0)
		return -EFAULT;

	if (*len != sizeof(struct compat_ipt_get_entries) + get.size) {
		duprintf("compat_get_entries: %u != %zu\n",
			 *len, sizeof(get) + get.size);
		return -EINVAL;
	}

	xt_compat_lock(AF_INET);
	t = xt_find_table_lock(net, AF_INET, get.name);
	if (t && !IS_ERR(t)) {
		const struct xt_table_info *private = t->private;
		struct xt_table_info info;
		duprintf("t->private->number = %u\n", private->number);
		ret = compat_table_info(private, &info);
		if (!ret && get.size == info.size) {
			ret = compat_copy_entries_to_user(private->size,
							  t, uptr->entrytable);
		} else if (!ret) {
			duprintf("compat_get_entries: I've got %u not %u!\n",
				 private->size, get.size);
			ret = -EAGAIN;
		}
		xt_compat_flush_offsets(AF_INET);
		module_put(t->me);
		xt_table_unlock(t);
	} else
		ret = t ? PTR_ERR(t) : -ENOENT;

	xt_compat_unlock(AF_INET);
	return ret;
}

static int do_ipt_get_ctl(struct sock *, int, void __user *, int *);

static int
compat_do_ipt_get_ctl(struct sock *sk, int cmd, void __user *user, int *len)
{
	int ret;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case IPT_SO_GET_INFO:
		ret = get_info(sock_net(sk), user, len, 1);
		break;
	case IPT_SO_GET_ENTRIES:
		ret = compat_get_entries(sock_net(sk), user, len);
		break;
	default:
		ret = do_ipt_get_ctl(sk, cmd, user, len);
	}
	return ret;
}
#endif

static int
do_ipt_set_ctl(struct sock *sk, int cmd, void __user *user, unsigned int len)
{
	int ret;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case IPT_SO_SET_REPLACE:
		ret = do_replace(sock_net(sk), user, len);
		break;

	case IPT_SO_SET_ADD_COUNTERS:
		ret = do_add_counters(sock_net(sk), user, len, 0);
		break;

	default:
		duprintf("do_ipt_set_ctl:  unknown request %i\n", cmd);
		ret = -EINVAL;
	}

	return ret;
}

static int
do_ipt_get_ctl(struct sock *sk, int cmd, void __user *user, int *len)
{
	int ret;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case IPT_SO_GET_INFO:
		ret = get_info(sock_net(sk), user, len, 0);
		break;

	case IPT_SO_GET_ENTRIES:
		ret = get_entries(sock_net(sk), user, len);
		break;

	case IPT_SO_GET_REVISION_MATCH:
	case IPT_SO_GET_REVISION_TARGET: {
		struct ipt_get_revision rev;
		int target;

		if (*len != sizeof(rev)) {
			ret = -EINVAL;
			break;
		}
		if (copy_from_user(&rev, user, sizeof(rev)) != 0) {
			ret = -EFAULT;
			break;
		}

		if (cmd == IPT_SO_GET_REVISION_TARGET)
			target = 1;
		else
			target = 0;

		try_then_request_module(xt_find_revision(AF_INET, rev.name,
							 rev.revision,
							 target, &ret),
					"ipt_%s", rev.name);
		break;
	}

	default:
		duprintf("do_ipt_get_ctl: unknown request %i\n", cmd);
		ret = -EINVAL;
	}

	return ret;
}

struct xt_table *ipt_register_table(struct net *net, struct xt_table *table,
				    const struct ipt_replace *repl)
{
	int ret;
	/*xt_table_info是实际描述表的数据结构*/
	struct xt_table_info *newinfo;
	struct xt_table_info bootstrap
		= { 0, 0, 0, { 0 }, { 0 }, { } };
	void *loc_cpu_entry;
	struct xt_table *new_table;

	/*分配规则空间*/
	newinfo = xt_alloc_table_info(repl->size);
	if (!newinfo) {
		ret = -ENOMEM;
		goto out;
	}

	/* choose the copy on our node/cpu, but dont care about preemption */
	loc_cpu_entry = newinfo->entries[raw_smp_processor_id()];
	memcpy(loc_cpu_entry, repl->entries, repl->size);

	
	/*translate_table函数将newinfo表示的table的各个规则进行边界检查，
	* 然后对于newinfo所指的ipt_talbe_info结构中的hook_entries和underflows赋予
	* 正确的值，最后将表项向其他cpu拷贝
	*/
	ret = translate_table(table->name, table->valid_hooks,
			      newinfo, loc_cpu_entry, repl->size,
			      repl->num_entries,
			      repl->hook_entry,
			      repl->underflow);
	if (ret != 0)
		goto out_free;

	new_table = xt_register_table(net, table, &bootstrap, newinfo);
	if (IS_ERR(new_table)) {
		ret = PTR_ERR(new_table);
		goto out_free;
	}

	return new_table;

out_free:
	xt_free_table_info(newinfo);
out:
	return ERR_PTR(ret);
}

void ipt_unregister_table(struct xt_table *table)
{
	struct xt_table_info *private;
	void *loc_cpu_entry;
	struct module *table_owner = table->me;

	private = xt_unregister_table(table);

	/* Decrease module usage counts and free resources */
	loc_cpu_entry = private->entries[raw_smp_processor_id()];
	IPT_ENTRY_ITERATE(loc_cpu_entry, private->size, cleanup_entry, NULL);
	if (private->number > private->initial_entries)
		module_put(table_owner);
	xt_free_table_info(private);
}

/* Returns 1 if the type and code is matched by the range, 0 otherwise */
static inline bool
icmp_type_code_match(u_int8_t test_type, u_int8_t min_code, u_int8_t max_code,
		     u_int8_t type, u_int8_t code,
		     bool invert)
{
	return ((test_type == 0xFF) ||
		(type == test_type && code >= min_code && code <= max_code))
		^ invert;
}

static bool
icmp_match(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct icmphdr *ic;
	struct icmphdr _icmph;
	const struct ipt_icmp *icmpinfo = par->matchinfo;

	/* Must not be a fragment. */
	if (par->fragoff != 0)
		return false;

	ic = skb_header_pointer(skb, par->thoff, sizeof(_icmph), &_icmph);
	if (ic == NULL) {
		/* We've been asked to examine this packet, and we
		 * can't.  Hence, no choice but to drop.
		 */
		duprintf("Dropping evil ICMP tinygram.\n");
		*par->hotdrop = true;
		return false;
	}

	return icmp_type_code_match(icmpinfo->type,
				    icmpinfo->code[0],
				    icmpinfo->code[1],
				    ic->type, ic->code,
				    !!(icmpinfo->invflags&IPT_ICMP_INV));
}

static bool icmp_checkentry(const struct xt_mtchk_param *par)
{
	const struct ipt_icmp *icmpinfo = par->matchinfo;

	/* Must specify no unknown invflags */
	return !(icmpinfo->invflags & ~IPT_ICMP_INV);
}

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
static int icmp_match2acl(const char *tablename,
			  const void *ip,
			  const struct xt_match *match,
			  void *matchinfo,
			  rtl865x_AclRule_t *acl_rule,
			  unsigned int *invflags)
{
	const struct ipt_ip *ip_info = (struct ipt_ip *) ip;
	const struct ipt_icmp *icmpinfo = matchinfo;
	unsigned int code_range = 0;
	int i = 0;
	rtl865x_AclRule_t *rule = acl_rule;
	

	if(ip == NULL || matchinfo == NULL)
		return 1;

	rule->ruleType_ = RTL865X_ACL_ICMP;
	rule->srcIpAddr_ 		= ip_info->src.s_addr;
	rule->srcIpAddrMask_	= ip_info->smsk.s_addr;
	rule->dstIpAddr_		= ip_info->dst.s_addr;
	rule->dstIpAddrMask_	= ip_info->dmsk.s_addr;

	rule->icmpType_ 	= icmpinfo->type;
	rule->icmpTypeMask_ 	= 0xff;
	//rule->icmpCode_	= icmpinfo->code;
	code_range = icmpinfo->code[1] - icmpinfo->code[0];
	for(i = 0; i <8 ; i++)
	{
		if(code_range >> i)
			continue;
		break;
	}
	rule->icmpCode_	= icmpinfo->code[0];
	rule->icmpCodeMask_ = 0xff << i;

	if(icmpinfo->invflags & IPT_ICMP_INV)
		if(invflags)
			*invflags = 1;	
	
	return 0;
}
#endif
/* The built-in targets: standard (NULL) and error. */
static struct xt_target ipt_standard_target __read_mostly = {
	.name		= IPT_STANDARD_TARGET,
	.targetsize	= sizeof(int),
	.family		= AF_INET,
#ifdef CONFIG_COMPAT
	.compatsize	= sizeof(compat_int_t),
	.compat_from_user = compat_standard_from_user,
	.compat_to_user	= compat_standard_to_user,
#endif
};

static struct xt_target ipt_error_target __read_mostly = {
	.name		= IPT_ERROR_TARGET,
	.target		= ipt_error,
	.targetsize	= IPT_FUNCTION_MAXNAMELEN,
	.family		= AF_INET,
};

static struct nf_sockopt_ops ipt_sockopts = {
	.pf		= PF_INET,
	.set_optmin	= IPT_BASE_CTL,
	.set_optmax	= IPT_SO_SET_MAX+1,
	.set		= do_ipt_set_ctl,
#ifdef CONFIG_COMPAT
	.compat_set	= compat_do_ipt_set_ctl,
#endif
	.get_optmin	= IPT_BASE_CTL,
	.get_optmax	= IPT_SO_GET_MAX+1,
	.get		= do_ipt_get_ctl,
#ifdef CONFIG_COMPAT
	.compat_get	= compat_do_ipt_get_ctl,
#endif
	.owner		= THIS_MODULE,
};

static struct xt_match icmp_matchstruct __read_mostly = {
	.name		= "icmp",
	.match		= icmp_match,
	.matchsize	= sizeof(struct ipt_icmp),
	.checkentry	= icmp_checkentry,
	.proto		= IPPROTO_ICMP,
	.family		= AF_INET,
#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
	.match2acl	= icmp_match2acl,
#endif
};

static int __net_init ip_tables_net_init(struct net *net)
{
	return xt_proto_init(net, AF_INET);
}

static void __net_exit ip_tables_net_exit(struct net *net)
{
	xt_proto_fini(net, AF_INET);
}

static struct pernet_operations ip_tables_net_ops = {
	.init = ip_tables_net_init,
	.exit = ip_tables_net_exit,
};

#ifdef CONFIG_APPS_LOGIC_FIREWALLLOG
static int proc_read_phymem_addr(char *buffer, char **start, off_t offset, 
               int length, int *eof, void *data)
{
        return sprintf(buffer, "%08lx\n", __pa(kernel_memaddr));
}
static int proc_read_phymem_size(char *buffer, char **start, off_t offset,
			   int length, int *eof, void *data)
{
        return sprintf(buffer, "%lu\n", kernel_memsize);
}

static int  logmmap_init(void)
{
        /*build proc dir "memshare"and two proc files: phymem_addr, phymem_size in the dir*/

         proc_memshare_dir = proc_mkdir(PROC_MEMSHARE_DIR, NULL);
         create_proc_read_entry(PROC_MEMSHARE_PHYADDR, 0, proc_memshare_dir, proc_read_phymem_addr, NULL);
         create_proc_read_entry(PROC_MEMSHARE_SIZE, 0, proc_memshare_dir, proc_read_phymem_size, NULL);

        /*alloc one page*/
         kernel_memaddr =__get_free_pages(GFP_KERNEL, PAGE_ORDER);
        if(!kernel_memaddr)
        {
                 printk("Allocate memory failure!\n");
        }
        else
        {
                 SetPageReserved(virt_to_page((const volatile void *)kernel_memaddr));

                 kernel_memsize = PAGES_NUMBER * PAGE_SIZE;
                 printk("Allocate memory success!. The phy mem addr=%08lx, size=%lu\n", __pa(kernel_memaddr), kernel_memsize);
		   memset((char*)kernel_memaddr,0,kernel_memsize);
        }
        return 0;
}
#endif


static int __init ip_tables_init(void)
{
	int ret;

#ifdef CONFIG_APPS_LOGIC_FIREWALLLOG
	logmmap_init();    // init mmap
#endif

	ret = register_pernet_subsys(&ip_tables_net_ops);
	if (ret < 0)
		goto err1;

	/* Noone else will be downing sem now, so we won't sleep */
	ret = xt_register_target(&ipt_standard_target);
	if (ret < 0)
		goto err2;
	ret = xt_register_target(&ipt_error_target);
	if (ret < 0)
		goto err3;
	ret = xt_register_match(&icmp_matchstruct);
	if (ret < 0)
		goto err4;

	/* Register setsockopt */
	ret = nf_register_sockopt(&ipt_sockopts);
	if (ret < 0)
		goto err5;

	printk(KERN_INFO "ip_tables: (C) 2000-2006 Netfilter Core Team\n");
	return 0;

err5:
	xt_unregister_match(&icmp_matchstruct);
err4:
	xt_unregister_target(&ipt_error_target);
err3:
	xt_unregister_target(&ipt_standard_target);
err2:
	unregister_pernet_subsys(&ip_tables_net_ops);
err1:
	return ret;
}

#ifdef CONFIG_APPS_LOGIC_FIREWALLLOG
static void  mmap_fini(void)
{
         printk("The content written by user is: %s\n", (unsigned char *) kernel_memaddr);
         ClearPageReserved(virt_to_page((const volatile void *)kernel_memaddr));
         free_pages(kernel_memaddr, PAGE_ORDER);
         remove_proc_entry(PROC_MEMSHARE_PHYADDR, proc_memshare_dir);
         remove_proc_entry(PROC_MEMSHARE_SIZE, proc_memshare_dir);
         remove_proc_entry(PROC_MEMSHARE_DIR, NULL);

        return;
}
#endif

static void __exit ip_tables_fini(void)
{
#if defined (CONFIG_RTL_IGMP_SNOOPING) && defined (CONFIG_NETFILTER)
		extern unsigned int (*IgmpRxFilter_Hook)(struct sk_buff *skb,
			 unsigned int hook,
			 const struct net_device *in,
			 const struct net_device *out,
			 struct xt_table *table);
		IgmpRxFilter_Hook = NULL;
#endif

#ifdef CONFIG_APPS_LOGIC_FIREWALLLOG
	mmap_fini(); //mmap finished
#endif

	nf_unregister_sockopt(&ipt_sockopts);

	xt_unregister_match(&icmp_matchstruct);
	xt_unregister_target(&ipt_error_target);
	xt_unregister_target(&ipt_standard_target);

	unregister_pernet_subsys(&ip_tables_net_ops);

}

EXPORT_SYMBOL(ipt_register_table);
EXPORT_SYMBOL(ipt_unregister_table);
EXPORT_SYMBOL(ipt_do_table);
module_init(ip_tables_init);
module_exit(ip_tables_fini);
