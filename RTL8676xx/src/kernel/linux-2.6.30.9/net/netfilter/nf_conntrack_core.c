/* Connection state tracking for netfilter.  This is separated from,
   but required by, the NAT layer; it can also be used by an iptables
   extension. */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2003,2004 USAGI/WIDE Project <http://www.linux-ipv6.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/stddef.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/jhash.h>
#include <linux/err.h>
#include <linux/percpu.h>
#include <linux/moduleparam.h>
#include <linux/notifier.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/socket.h>
#include <linux/mm.h>
#include <linux/rculist_nulls.h>
#include <linux/ip.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_acct.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#if defined(CONFIG_RTL867X_IPTABLES_FAST_PATH)
#include "../ipv4/fastpath/fastpath_core.h"
#endif


#if defined(CONFIG_RTL_819X) || defined(CONFIG_RTL_8676HWNAT)
#include <net/rtl/rtl_types.h> 
#include <net/rtl/rtl865x_fdb_api.h>
#include <net/rtl/rtl865x_arp_api.h>
#endif //CONFIG_RTL_819X, CONFIG_RTL_8676HWNAT
#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
//#include <net/rtl/rtl_types.h>
#include <net/rtl/fastpath/fastpath_core.h>
#if defined(CONFIG_FAST_PATH_MODULE)
enum LR_RESULT (*FastPath_hook4)( enum NP_PROTOCOL protocol, ipaddr_t intIp, uint32 intPort,
                                                               ipaddr_t extIp, uint32 extPort,
                                                               ipaddr_t remIp, uint32 remPort )=NULL;
enum LR_RESULT (*FastPath_hook6)( enum NP_PROTOCOL protocol, ipaddr_t intIp, uint32 intPort,
                                                               ipaddr_t extIp, uint32 extPort,
                                                               ipaddr_t remIp, uint32 remPort,
                                                               enum NP_FLAGS flags)=NULL;
enum LR_RESULT (*FastPath_hook11)(enum NP_PROTOCOL protocol,
		ipaddr_t intIp, uint32 intPort,
		ipaddr_t extIp, uint32 extPort,
		ipaddr_t remIp, uint32 remPort,
		uint32 interval)=NULL;
EXPORT_SYMBOL(FastPath_hook4);
EXPORT_SYMBOL(FastPath_hook6);
EXPORT_SYMBOL(FastPath_hook11);
#endif
#endif

#define NF_CONNTRACK_VERSION	"0.5.0"

int (*nfnetlink_parse_nat_setup_hook)(struct nf_conn *ct,
				      enum nf_nat_manip_type manip,
				      struct nlattr *attr) __read_mostly;
EXPORT_SYMBOL_GPL(nfnetlink_parse_nat_setup_hook);

DEFINE_SPINLOCK(nf_conntrack_lock);
EXPORT_SYMBOL_GPL(nf_conntrack_lock);

unsigned int nf_conntrack_htable_size __read_mostly;
EXPORT_SYMBOL_GPL(nf_conntrack_htable_size);

unsigned int nf_conntrack_max __read_mostly;
EXPORT_SYMBOL_GPL(nf_conntrack_max);

struct nf_conn nf_conntrack_untracked __read_mostly;
EXPORT_SYMBOL_GPL(nf_conntrack_untracked);

static struct kmem_cache *nf_conntrack_cachep __read_mostly;

static int nf_conntrack_hash_rnd_initted;
static unsigned int nf_conntrack_hash_rnd;

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
static struct timer_list tcp_patten_ck;
#if defined(IMPROVE_QOS)
int smart_count=0;
#else
static int smart_count=0;
#endif
#endif

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)

unsigned int conntrack_min = 0;
unsigned int conntrack_max= 100;

LIST_HEAD(close_list);
LIST_HEAD(last_ack_list);
LIST_HEAD(close_wait_list);
LIST_HEAD(fin_wait_list);
LIST_HEAD(time_wait_list);
LIST_HEAD(syn_recv_list);
LIST_HEAD(syn_sent_list);
LIST_HEAD(established_list);
LIST_HEAD(listen_list);
LIST_HEAD(udp_unreply_list);
LIST_HEAD(udp_assured_list);

//修正Tcp_State_Hash_Head数组元素与TCP状态值没有对接上的问题
struct tcp_state_hash_head Tcp_State_Hash_Head[]={                              //enum tcp_conntrack {
	{TCP_CONNTRACK_NONE,NULL},                                          /*0*/       //	TCP_CONNTRACK_NONE,                     /*0*/
	{TCP_CONNTRACK_SYN_SENT,&syn_sent_list},                /*1*/       //	TCP_CONNTRACK_SYN_SENT,             /*1*/
	{TCP_CONNTRACK_SYN_RECV,&syn_recv_list},                /*2*/       //	TCP_CONNTRACK_SYN_RECV,             /*2*/
	{TCP_CONNTRACK_ESTABLISHED,&established_list},      /*3*/       //	TCP_CONNTRACK_ESTABLISHED,       /*3*/
	{TCP_CONNTRACK_FIN_WAIT,&fin_wait_list},                  /*4*/       //	TCP_CONNTRACK_FIN_WAIT,             /*4*/
	{TCP_CONNTRACK_CLOSE_WAIT,&close_wait_list},        /*5*/       //	TCP_CONNTRACK_CLOSE_WAIT,       /*5*/
	{TCP_CONNTRACK_LAST_ACK,&last_ack_list},                 /*6*/        //	TCP_CONNTRACK_LAST_ACK,             /*6*/
	{TCP_CONNTRACK_TIME_WAIT,&time_wait_list},           /*7*/        //	TCP_CONNTRACK_TIME_WAIT,          /*7*/
	{TCP_CONNTRACK_CLOSE,&close_list},                             /*8*/        //	TCP_CONNTRACK_CLOSE,                    /*8*/
	{TCP_CONNTRACK_LISTEN,&listen_list},                           /*9*/        //	TCP_CONNTRACK_LISTEN,                   /*9*/
	{TCP_CONNTRACK_MAX,NULL}                                           /*10*/       //	TCP_CONNTRACK_MAX,                        /*10*/
	};                                                                                                                    //	TCP_CONNTRACK_IGNORE                   /*11*/
                                                                                                                                //};


struct udp_state_hash_head Udp_State_Hash_Head[]={
	{UDP_UNREPLY,&udp_unreply_list},
	{UDP_ASSURED,&udp_assured_list}
};

//修改丢弃连接跟踪的状态优先级别
 struct DROP_PRORITY drop_priority[]={
 	{TCP_CONNTRACK_CLOSE,10},
	{TCP_CONNTRACK_LAST_ACK,30},
	{TCP_CONNTRACK_CLOSE_WAIT,60},
	{TCP_CONNTRACK_TIME_WAIT,120},
	{TCP_CONNTRACK_FIN_WAIT,120},
	{UDP_UNREPLY,30},
	{TCP_CONNTRACK_ESTABLISHED,8*60},
	{TCP_CONNTRACK_SYN_SENT,120},
	{TCP_CONNTRACK_SYN_RECV,60},
	{UDP_ASSURED,180}
};


static void init_tcp_state_hash(void)
{
	int i;
	for (i = 0; i < sizeof(Tcp_State_Hash_Head)/sizeof(struct tcp_state_hash_head); i++)
	{
		struct tcp_state_hash_head *p;
		p = &Tcp_State_Hash_Head[i];
		if (p->state_hash)
			INIT_LIST_HEAD(p->state_hash);
	}
	for (i = 0; i < sizeof(Udp_State_Hash_Head)/sizeof(struct udp_state_hash_head); i++)
	{
		struct udp_state_hash_head *p;
		p = &Udp_State_Hash_Head[i];
		if (p->state_hash)
			INIT_LIST_HEAD(p->state_hash);
	}

}



/* statistic counters, prot_limit is the threshold for selecting the protocol conntrack to drop.
    eg. if TCP is 95(95%), then when TCP connections occupy more than 95%, gc will choose 
    TCP to drop regardless its priority. Andrew 
*/

unsigned int prot_limit[PROT_MAX];
atomic_t prot_counters[PROT_MAX];
atomic_t _prot_limit[PROT_MAX];

int drop_one_conntrack(void);
static inline int drop_conntracks(struct net *net);
static inline void recalculate(void) {
	int i;
	for (i = 0; i < PROT_MAX; i++) {
		atomic_set(&_prot_limit[i], nf_conntrack_max * prot_limit[i] /100);
	}

	//printk("icmp=%u  tcp=%u  udp=%u\n", _prot_limit[PROT_ICMP], _prot_limit[PROT_TCP], _prot_limit[PROT_UDP]);
}

int conntrack_dointvec(ctl_table *table, int write, struct file *filp,
		     void *buffer, size_t *lenp, loff_t *ppos) {

	int err;

	err = proc_dointvec(table, write, filp, buffer, lenp, ppos);
	if (err != 0)
		return err;
	if (write)
		recalculate();
	return 0;
}

int conntrack_dointvec_minmax(ctl_table *table, int write, struct file *filp,
		     void *buffer, size_t *lenp, loff_t *ppos) {

	int err;

	err = proc_dointvec_minmax(table, write, filp, buffer, lenp, ppos);
	if (err != 0)
		return err;
	if (write)
		recalculate();
	return 0;
}
#endif  //CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW



static u_int32_t __hash_conntrack(const struct nf_conntrack_tuple *tuple,
				  unsigned int size, unsigned int rnd)
{
	unsigned int n;
	u_int32_t h;

	/* The direction must be ignored, so we hash everything up to the
	 * destination ports (which is a multiple of 4) and treat the last
	 * three bytes manually.
	 */
	n = (sizeof(tuple->src) + sizeof(tuple->dst.u3)) / sizeof(u32);
	h = jhash2((u32 *)tuple, n,
		   rnd ^ (((__force __u16)tuple->dst.u.all << 16) |
			  tuple->dst.protonum));

	return ((u64)h * size) >> 32;
}

static inline u_int32_t hash_conntrack(const struct nf_conntrack_tuple *tuple)
{
	return __hash_conntrack(tuple, nf_conntrack_htable_size,
				nf_conntrack_hash_rnd);
}

bool
nf_ct_get_tuple(const struct sk_buff *skb,
		unsigned int nhoff,
		unsigned int dataoff,
		u_int16_t l3num,
		u_int8_t protonum,
		struct nf_conntrack_tuple *tuple,
		const struct nf_conntrack_l3proto *l3proto,
		const struct nf_conntrack_l4proto *l4proto)
{
	memset(tuple, 0, sizeof(*tuple));

	tuple->src.l3num = l3num;
	if (l3proto->pkt_to_tuple(skb, nhoff, tuple) == 0)
		return false;

	tuple->dst.protonum = protonum;
	tuple->dst.dir = IP_CT_DIR_ORIGINAL;

	return l4proto->pkt_to_tuple(skb, dataoff, tuple);
}
EXPORT_SYMBOL_GPL(nf_ct_get_tuple);

bool nf_ct_get_tuplepr(const struct sk_buff *skb, unsigned int nhoff,
		       u_int16_t l3num, struct nf_conntrack_tuple *tuple)
{
	struct nf_conntrack_l3proto *l3proto;
	struct nf_conntrack_l4proto *l4proto;
	unsigned int protoff;
	u_int8_t protonum;
	int ret;

	rcu_read_lock();

	l3proto = __nf_ct_l3proto_find(l3num);
	ret = l3proto->get_l4proto(skb, nhoff, &protoff, &protonum);
	if (ret != NF_ACCEPT) {
		rcu_read_unlock();
		return false;
	}

	l4proto = __nf_ct_l4proto_find(l3num, protonum);

	ret = nf_ct_get_tuple(skb, nhoff, protoff, l3num, protonum, tuple,
			      l3proto, l4proto);

	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL_GPL(nf_ct_get_tuplepr);

bool
nf_ct_invert_tuple(struct nf_conntrack_tuple *inverse,
		   const struct nf_conntrack_tuple *orig,
		   const struct nf_conntrack_l3proto *l3proto,
		   const struct nf_conntrack_l4proto *l4proto)
{
	memset(inverse, 0, sizeof(*inverse));

	inverse->src.l3num = orig->src.l3num;
	if (l3proto->invert_tuple(inverse, orig) == 0)
		return false;

	inverse->dst.dir = !orig->dst.dir;

	inverse->dst.protonum = orig->dst.protonum;
	return l4proto->invert_tuple(inverse, orig);
}
EXPORT_SYMBOL_GPL(nf_ct_invert_tuple);

static void
clean_from_lists(struct nf_conn *ct)
{
	pr_debug("clean_from_lists(%p)\n", ct);
	hlist_nulls_del_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode);
	hlist_nulls_del_rcu(&ct->tuplehash[IP_CT_DIR_REPLY].hnnode);

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
if( (nf_ct_l3num(ct)==PF_INET) || (nf_ct_l3num(ct)==PF_INET6) )
{
	switch (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum) 
	{
	      	case IPPROTO_TCP:
	       	atomic_dec(&prot_counters[PROT_TCP] );
	        	break;
	        case IPPROTO_UDP:
	        	atomic_dec(&prot_counters[PROT_UDP] );
	        	break;
	        case IPPROTO_ICMP:
	        	atomic_dec(&prot_counters[PROT_ICMP] );
	        	break;
	}

	if((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP)||
		(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP))
	{
		list_del(&ct->state_tuple);
	       ct->state_tuple.next=NULL;
		ct->state_tuple.prev=NULL;	
	}
}
#endif //CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW

	/* Destroy all pending expectations */
	nf_ct_remove_expectations(ct);
}

#ifdef CONFIG_TW_DERIVED_CON
static void death_by_timeout(unsigned long ul_conntrack);
#endif

static void
destroy_conntrack(struct nf_conntrack *nfct)
{
	struct nf_conn *ct = (struct nf_conn *)nfct;
	struct net *net = nf_ct_net(ct);
	struct nf_conntrack_l4proto *l4proto;
//#ifdef CONFIG_RTL_HARDWARE_NAT 
//	struct nf_conn_nat *nat;
//#endif

	pr_debug("destroy_conntrack(%p)\n", ct);
	NF_CT_ASSERT(atomic_read(&nfct->use) == 0);
	NF_CT_ASSERT(!timer_pending(&ct->timeout));

	if (!test_bit(IPS_DYING_BIT, &ct->status))
		nf_conntrack_event(IPCT_DESTROY, ct);
	set_bit(IPS_DYING_BIT, &ct->status);

	/* To make sure we don't get any weird locking issues here:
	 * destroy_conntrack() MUST NOT be called with a write lock
	 * to nf_conntrack_lock!!! -HW */
	rcu_read_lock();
	l4proto = __nf_ct_l4proto_find(nf_ct_l3num(ct), nf_ct_protonum(ct));
	if (l4proto && l4proto->destroy)
		l4proto->destroy(ct);

	rcu_read_unlock();

#ifdef	CONFIG_RTL867X_IPTABLES_FAST_PATH
  if(nf_ct_l3num(ct)==PF_INET)
  {
	if (FastPath_Enabled()) {
		if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) {
			if ( ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum != IPPROTO_ICMP ) {
				rtl867x_delNaptConnection(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
					ct->tuplehash[IP_CT_DIR_REPLY].tuple);
			}
		}	
	}
  }
#endif
	
#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	//if ( test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) 
	{
		if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP) {
			/* TCP Connection Tracking */
			
			if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
			   == ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)
			{
				/*case  WAN->LAN(BC->AB) use C|A-B*/
				#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook4!=NULL)
				{
					FastPath_hook4(NP_TCP,
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),	
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port),				
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port)				
					) ;	
				}
				#else
				rtk_delNaptConnection(NP_TCP,
				ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),	
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port),				
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port)				
				) ;		
				#endif
			}else{
				/*case  LAN->WAN(AB->BC) use A|C-B*/
				#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook4!=NULL)
				{
					FastPath_hook4(NP_TCP, 
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.tcp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port)
					);
				}
				#else
				rtk_delNaptConnection(NP_TCP, 
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
				ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.tcp.port),
				ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port)
				);
				#endif
			}	
		} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {

			/* UDP Connection Tracking */
			/* check "LAN to WAN (AB->BC)" first */
			if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
			   == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)	
			{			
				/*case  LAN->WAN(BC->AB) use A|C-B*/
				#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook4!=NULL)
				{
					FastPath_hook4(NP_UDP, 
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port)
					);
				}
				#else
				rtk_delNaptConnection(NP_UDP, 
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
				ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port),
				ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port)
				);
				#endif
			}
			else if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
			   == ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)
			{
				/*case  WAN->LAN(BC->AB) use C|A-B*/
				#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook4!=NULL)
				{
					FastPath_hook4(NP_UDP,
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),				
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port),				
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port)
					) ;
				}
				#else
				rtk_delNaptConnection(NP_UDP,
				ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),				
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port),				
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port)
				) ;			
				#endif
			}
		}
	}	/* IPS_SEEN_REPLY_BIT == 1 */
#endif


#ifdef CONFIG_RTL_HARDWARE_NAT 
	#if 0
	nat = nfct_nat(ct);
	if (nat->hw_acc == 1) 
	{
	#else
	if(test_bit(IPS_8676HW_NAPT_BIT, &ct->status))
	{
	#endif
		if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP ||
			ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP)
		{
			#if defined (CONFIG_RTL_IPTABLES_RULE_2_ACL) && defined(CONFIG_RTL8676_Dynamic_ACL)
			rtl865x_DELETE_dynamic_acl_L34_permit(ct);
			#endif
			rtl865x_handle_nat(ct, 0);
		}
	}	
#endif


	spin_lock_bh(&nf_conntrack_lock);
	/* Expectations will have been removed in clean_from_lists,
	 * except TFTP can create an expectation on the first packet,
	 * before connection is in the list, so we need to clean here,
	 * too. */
	nf_ct_remove_expectations(ct);

	/* We overload first tuple to link into unconfirmed list. */
	if (!nf_ct_is_confirmed(ct)) {
		BUG_ON(hlist_nulls_unhashed(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode));
		hlist_nulls_del_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode);
	}

	NF_CT_STAT_INC(net, delete);
	spin_unlock_bh(&nf_conntrack_lock);

	if (ct->master)
	{
#ifdef CONFIG_TW_DERIVED_CON
		list_del(&ct->derived_list);
#endif
		nf_ct_put(ct->master);
	}
    
#ifdef CONFIG_TW_DERIVED_CON
	/* Disconnect all child connections that have infinit timeout */
	if (!list_empty(&ct->derived_connections)) {
		struct nf_conn *child, *tmp;

		list_for_each_entry_safe(child, tmp, &ct->derived_connections,
			derived_list) {
			if (child->derived_timeout == 0xFFFFFFFF &&
			    del_timer(&child->timeout))
				death_by_timeout((unsigned long)child);
		}
	}
#endif
	pr_debug("destroy_conntrack: returning ct=%p to slab\n", ct);
	nf_conntrack_free(ct);
}

static void death_by_timeout(unsigned long ul_conntrack)
{
	struct nf_conn *ct = (void *)ul_conntrack;
	struct net *net = nf_ct_net(ct);
	struct nf_conn_help *help = nfct_help(ct);
	struct nf_conntrack_helper *helper;
#ifdef CONFIG_RTL_HARDWARE_NAT
	struct nf_conn_nat *nat;
#endif

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_NAT)
/*2007-12-19*/
	unsigned long expires = ct->timeout.expires;
	unsigned long now = jiffies;
	unsigned long nf_ct_tcp_timeout_established = 5*24*60*60*HZ;
	unsigned long nf_ct_udp_timeout_stream = 180*HZ;
#endif

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	{
		if (time_after_eq(now, expires)) {
			/* really idle timeout, not force to destroy */
//			if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) 
			{
				if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP &&
					ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED) {
					/* TCP Connection Tracking */
					
					if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
						== ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)
					{
						/* case BC->AB */	
						#ifdef CONFIG_FAST_PATH_MODULE
						if(FastPath_hook11!=NULL)
						{
							if (FastPath_hook11(NP_TCP,
									ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),	
									ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port),				
									ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
									nf_ct_tcp_timeout_established) != LR_SUCCESS) {
#if !defined(IMPROVE_QOS)
								write_lock_bh(&nf_conntrack_lock);
								ct->timeout.expires = now + nf_ct_tcp_timeout_established;
								add_timer(&ct->timeout);
								write_unlock_bh(&nf_conntrack_lock);
#endif
								return;
							}
						}
						#else
						if (rtk_idleNaptConnection(NP_TCP,
								ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),	
								ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port),				
								ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
								nf_ct_tcp_timeout_established) != LR_SUCCESS) {
#if !defined(IMPROVE_QOS)
							write_lock_bh(&nf_conntrack_lock);
							ct->timeout.expires = now + nf_ct_tcp_timeout_established;
							add_timer(&ct->timeout);
							write_unlock_bh(&nf_conntrack_lock);
#endif
							return;
						}
						#endif			
					}
					else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
							 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)
					{
						/* case AB->BC */
						#ifdef CONFIG_FAST_PATH_MODULE
						if(FastPath_hook11!=NULL)
						{
							if (FastPath_hook11(NP_TCP, 
									ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
									ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.tcp.port),
									ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),
									nf_ct_tcp_timeout_established) != LR_SUCCESS) {
#if !defined(IMPROVE_QOS)
								write_lock_bh(&nf_conntrack_lock);
								ct->timeout.expires = now + nf_ct_tcp_timeout_established;
								add_timer(&ct->timeout);
								write_unlock_bh(&nf_conntrack_lock);
#endif
								return;
							}
						}
						#else
						if (rtk_idleNaptConnection(NP_TCP, 
								ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
								ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.tcp.port),
								ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),
								nf_ct_tcp_timeout_established) != LR_SUCCESS) {
#if !defined(IMPROVE_QOS)
							write_lock_bh(&nf_conntrack_lock);
							ct->timeout.expires = now + nf_ct_tcp_timeout_established;
							add_timer(&ct->timeout);
							write_unlock_bh(&nf_conntrack_lock);
#endif
							return;
						}
						#endif
					}
				} 
				else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {
					/* UDP Connection Tracking */
					
					if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
						== ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)
					{
						/* case BC->AB */
						#ifdef CONFIG_FAST_PATH_MODULE
						if(FastPath_hook11!=NULL)
						{
							if (FastPath_hook11(NP_UDP,
									ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),	
									ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port),				
									ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
									nf_ct_udp_timeout_stream) != LR_SUCCESS) {
#if !defined(IMPROVE_QOS)
								write_lock_bh(&nf_conntrack_lock);
								ct->timeout.expires = now + nf_ct_udp_timeout_stream;
								add_timer(&ct->timeout);
								write_unlock_bh(&nf_conntrack_lock);
#endif
								return;
							}	
						}
						#else
						if (rtk_idleNaptConnection(NP_UDP,
								ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),	
								ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port),				
								ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
								nf_ct_udp_timeout_stream) != LR_SUCCESS) {
#if !defined(IMPROVE_QOS)
							write_lock_bh(&nf_conntrack_lock);
							ct->timeout.expires = now + nf_ct_udp_timeout_stream;
							add_timer(&ct->timeout);
							write_unlock_bh(&nf_conntrack_lock);
#endif
							return;
						}	
						#endif
					}
					else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
							 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)				
					{
						/* case AB->BC */
						#ifdef CONFIG_FAST_PATH_MODULE
						if(FastPath_hook11!=NULL)
						{
							if (FastPath_hook11(NP_UDP, 
									ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
									ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port),
									ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
									ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),
									nf_ct_udp_timeout_stream) != LR_SUCCESS) {
#if !defined(IMPROVE_QOS)
								write_lock_bh(&nf_conntrack_lock);
								ct->timeout.expires = now + nf_ct_udp_timeout_stream;
								add_timer(&ct->timeout);
								write_unlock_bh(&nf_conntrack_lock);
#endif
								return;
							}
						}
						#else
						if (rtk_idleNaptConnection(NP_UDP, 
								ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
								ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port),
								ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
								ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),
								nf_ct_udp_timeout_stream) != LR_SUCCESS) {
#if !defined(IMPROVE_QOS)
							write_lock_bh(&nf_conntrack_lock);
							ct->timeout.expires = now + nf_ct_udp_timeout_stream;
							add_timer(&ct->timeout);
							write_unlock_bh(&nf_conntrack_lock);
#endif
							return;
						}
						#endif
					}
				}
			}	/* IPS_SEEN_REPLY_BIT == 1 */
		}
	}
#endif


#ifdef CONFIG_RTL_HARDWARE_NAT  
	{
		if (test_bit(IPS_8676HW_NAPT_BIT, &ct->status) && time_after_eq(now, expires)) 
		{
			/* really idle timeout, not force to destroy */
			nat = nfct_nat(ct);
			if (nat->hw_acc == 1) /*  this napt connection exitsed in 0412 asic napt table  */
			{
				if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP &&
					ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED) {
					if (!rtl865x_handle_nat(ct, 2)) {
						write_lock_bh(&nf_conntrack_lock);
						ct->timeout.expires = now + nf_ct_tcp_timeout_established;
						add_timer(&ct->timeout);
						write_unlock_bh(&nf_conntrack_lock);
						return;
					}
				}
				else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {
					if (!rtl865x_handle_nat(ct, 2)) {
						write_lock_bh(&nf_conntrack_lock);
						ct->timeout.expires = now + nf_ct_udp_timeout_stream;
						add_timer(&ct->timeout);
						write_unlock_bh(&nf_conntrack_lock);
						return;
					}
				}
			}
			else /*  if it does not exitsed in 0412 asic napt table  , look up L2 table instead */
			{
				rtl865x_arpMapping_entry_t  intIPArp;
				rtl865x_arpMapping_entry_t  remIPArp;
				
				if( rtl865x_getArpMapping(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,&intIPArp)==SUCCESS )  
				{
					if(rtl_get_hw_fdb_age(RTL_LAN_FID,&intIPArp.mac,FDB_DYNAMIC))
					{
						if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP &&
							ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED) 
						{
							write_lock_bh(&nf_conntrack_lock);
							ct->timeout.expires = now + nf_ct_tcp_timeout_established;
							add_timer(&ct->timeout);
							write_unlock_bh(&nf_conntrack_lock);
							return;
						}
						else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) 
						{
							write_lock_bh(&nf_conntrack_lock);
							ct->timeout.expires = now + nf_ct_udp_timeout_stream;
							add_timer(&ct->timeout);
							write_unlock_bh(&nf_conntrack_lock);
							return;
						}
					}
				}

				if( rtl865x_getArpMapping(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,&remIPArp)==SUCCESS )  
				{
					if(rtl_get_hw_fdb_age(RTL_WAN_FID,&remIPArp.mac,FDB_DYNAMIC))
					{
						if (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.protonum == IPPROTO_TCP &&
							ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED) 
						{
							write_lock_bh(&nf_conntrack_lock);
							ct->timeout.expires = now + nf_ct_tcp_timeout_established;
							add_timer(&ct->timeout);
							write_unlock_bh(&nf_conntrack_lock);
							return;
						}
						else if (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.protonum == IPPROTO_UDP) 
						{
							write_lock_bh(&nf_conntrack_lock);
							ct->timeout.expires = now + nf_ct_udp_timeout_stream;
							add_timer(&ct->timeout);
							write_unlock_bh(&nf_conntrack_lock);
							return;
						}
					}
				}
			}
		}
	}
#endif


	if (help) {
		rcu_read_lock();
		helper = rcu_dereference(help->helper);
		if (helper && helper->destroy)
			helper->destroy(ct);
		rcu_read_unlock();
	}

	spin_lock_bh(&nf_conntrack_lock);
	/* Inside lock so preempt is disabled on module removal path.
	 * Otherwise we can get spurious warnings. */
	NF_CT_STAT_INC(net, delete_list);
	clean_from_lists(ct);
	spin_unlock_bh(&nf_conntrack_lock);
	nf_ct_put(ct);
}

/*
 * Warning :
 * - Caller must take a reference on returned object
 *   and recheck nf_ct_tuple_equal(tuple, &h->tuple)
 * OR
 * - Caller must lock nf_conntrack_lock before calling this function
 */
struct nf_conntrack_tuple_hash *
__nf_conntrack_find(struct net *net, const struct nf_conntrack_tuple *tuple)
{
	struct nf_conntrack_tuple_hash *h;
	struct hlist_nulls_node *n;
	unsigned int hash = hash_conntrack(tuple);

	/* Disable BHs the entire time since we normally need to disable them
	 * at least once for the stats anyway.
	 */
	local_bh_disable();
begin:
	hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash], hnnode) {
		if (nf_ct_tuple_equal(tuple, &h->tuple)) {
			NF_CT_STAT_INC(net, found);
			local_bh_enable();
			return h;
		}
		NF_CT_STAT_INC(net, searched);
	}
	/*
	 * if the nulls value we got at the end of this lookup is
	 * not the expected one, we must restart lookup.
	 * We probably met an item that was moved to another chain.
	 */
	if (get_nulls_value(n) != hash)
		goto begin;
	local_bh_enable();

	return NULL;
}
EXPORT_SYMBOL_GPL(__nf_conntrack_find);

/* Find a connection corresponding to a tuple. */
struct nf_conntrack_tuple_hash *
nf_conntrack_find_get(struct net *net, const struct nf_conntrack_tuple *tuple)
{
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct;

	rcu_read_lock();
begin:
	h = __nf_conntrack_find(net, tuple);
	if (h) {
		ct = nf_ct_tuplehash_to_ctrack(h);
		if (unlikely(nf_ct_is_dying(ct) ||
			     !atomic_inc_not_zero(&ct->ct_general.use)))
			h = NULL;
		else {
			if (unlikely(!nf_ct_tuple_equal(tuple, &h->tuple))) {
				nf_ct_put(ct);
				goto begin;
			}
		}
	}
	rcu_read_unlock();

	return h;
}
EXPORT_SYMBOL_GPL(nf_conntrack_find_get);

static void __nf_conntrack_hash_insert(struct nf_conn *ct,
				       unsigned int hash,
				       unsigned int repl_hash)
{
	struct net *net = nf_ct_net(ct);

	hlist_nulls_add_head_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode,
			   &net->ct.hash[hash]);
	hlist_nulls_add_head_rcu(&ct->tuplehash[IP_CT_DIR_REPLY].hnnode,
			   &net->ct.hash[repl_hash]);
}

void nf_conntrack_hash_insert(struct nf_conn *ct)
{
	unsigned int hash, repl_hash;

	hash = hash_conntrack(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);
	repl_hash = hash_conntrack(&ct->tuplehash[IP_CT_DIR_REPLY].tuple);

	__nf_conntrack_hash_insert(ct, hash, repl_hash);
}
EXPORT_SYMBOL_GPL(nf_conntrack_hash_insert);

/* Confirm a connection given skb; places it in hash table */
int
__nf_conntrack_confirm(struct sk_buff *skb)
{
	unsigned int hash, repl_hash;
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct;
	struct nf_conn_help *help;
	struct hlist_nulls_node *n;
	enum ip_conntrack_info ctinfo;
	struct net *net;

	ct = nf_ct_get(skb, &ctinfo);
	net = nf_ct_net(ct);

	/* ipt_REJECT uses nf_conntrack_attach to attach related
	   ICMP/TCP RST packets in other direction.  Actual packet
	   which created connection will be IP_CT_NEW or for an
	   expected connection, IP_CT_RELATED. */
	if (CTINFO2DIR(ctinfo) != IP_CT_DIR_ORIGINAL)
		return NF_ACCEPT;

	hash = hash_conntrack(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);
	repl_hash = hash_conntrack(&ct->tuplehash[IP_CT_DIR_REPLY].tuple);

	/* We're not in hash table, and we refuse to set up related
	   connections for unconfirmed conns.  But packet copies and
	   REJECT will give spurious warnings here. */
	/* NF_CT_ASSERT(atomic_read(&ct->ct_general.use) == 1); */

	/* No external references means noone else could have
	   confirmed us. */
	NF_CT_ASSERT(!nf_ct_is_confirmed(ct));
	pr_debug("Confirming conntrack %p\n", ct);

	spin_lock_bh(&nf_conntrack_lock);

	/* See if there's one in the list already, including reverse:
	   NAT could have grabbed it without realizing, since we're
	   not in the hash.  If there is, we lost race. */
	hlist_nulls_for_each_entry(h, n, &net->ct.hash[hash], hnnode)
		if (nf_ct_tuple_equal(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
				      &h->tuple))
			goto out;
	hlist_nulls_for_each_entry(h, n, &net->ct.hash[repl_hash], hnnode)
		if (nf_ct_tuple_equal(&ct->tuplehash[IP_CT_DIR_REPLY].tuple,
				      &h->tuple))
			goto out;

	/* Remove from unconfirmed list */
	hlist_nulls_del_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode);

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
if( (nf_ct_l3num(ct)==PF_INET) || (nf_ct_l3num(ct)==PF_INET6) )
{
	//switch (ip_hdr(skb)->protocol) 
	switch(nf_ct_protonum(ct))
	{
		case IPPROTO_TCP:
			if(Tcp_State_Hash_Head[ct->proto.tcp.state].state_hash!=NULL)
			{
				list_add_tail(&ct->state_tuple,Tcp_State_Hash_Head[ct->proto.tcp.state].state_hash);		
			}
			atomic_inc(&prot_counters[PROT_TCP] );
			break;
		case IPPROTO_UDP:
			if(ct->status & IPS_SEEN_REPLY)				
				list_add_tail(&ct->state_tuple,Udp_State_Hash_Head[1].state_hash);		  
			else				
				list_add_tail(&ct->state_tuple,Udp_State_Hash_Head[0].state_hash);
			atomic_inc(&prot_counters[PROT_UDP] );
			break;
		case IPPROTO_ICMP:
			atomic_inc(&prot_counters[PROT_ICMP] );
			break;
	}
}
#endif

	/* Timer relative to confirmation time, not original
	   setting time, otherwise we'd get timer wrap in
	   weird delay cases. */
	ct->timeout.expires += jiffies;
	add_timer(&ct->timeout);
	atomic_inc(&ct->ct_general.use);
	set_bit(IPS_CONFIRMED_BIT, &ct->status);

	/* Since the lookup is lockless, hash insertion must be done after
	 * starting the timer and setting the CONFIRMED bit. The RCU barriers
	 * guarantee that no other CPU can find the conntrack before the above
	 * stores are visible.
	 */
	__nf_conntrack_hash_insert(ct, hash, repl_hash);
	NF_CT_STAT_INC(net, insert);
	spin_unlock_bh(&nf_conntrack_lock);

	help = nfct_help(ct);
	if (help && help->helper)
		nf_conntrack_event_cache(IPCT_HELPER, ct);
#ifdef CONFIG_NF_NAT_NEEDED
	if (test_bit(IPS_SRC_NAT_DONE_BIT, &ct->status) ||
	    test_bit(IPS_DST_NAT_DONE_BIT, &ct->status))
		nf_conntrack_event_cache(IPCT_NATINFO, ct);
#endif
	nf_conntrack_event_cache(master_ct(ct) ?
				 IPCT_RELATED : IPCT_NEW, ct);
	return NF_ACCEPT;

out:
	NF_CT_STAT_INC(net, insert_failed);
	spin_unlock_bh(&nf_conntrack_lock);
	return NF_DROP;
}
EXPORT_SYMBOL_GPL(__nf_conntrack_confirm);

/* Returns true if a connection correspondings to the tuple (required
   for NAT). */
int
nf_conntrack_tuple_taken(const struct nf_conntrack_tuple *tuple,
			 const struct nf_conn *ignored_conntrack)
{
	struct net *net = nf_ct_net(ignored_conntrack);
	struct nf_conntrack_tuple_hash *h;
	struct hlist_nulls_node *n;
	unsigned int hash = hash_conntrack(tuple);

	/* Disable BHs the entire time since we need to disable them at
	 * least once for the stats anyway.
	 */
	rcu_read_lock_bh();
	hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash], hnnode) {
		if (nf_ct_tuplehash_to_ctrack(h) != ignored_conntrack &&
		    nf_ct_tuple_equal(tuple, &h->tuple)) {
			NF_CT_STAT_INC(net, found);
			rcu_read_unlock_bh();
			return 1;
		}
		NF_CT_STAT_INC(net, searched);
	}
	rcu_read_unlock_bh();

	return 0;
}
EXPORT_SYMBOL_GPL(nf_conntrack_tuple_taken);

#define NF_CT_EVICTION_RANGE	8

/* There's a small race here where we may free a just-assured
   connection.  Too bad: we're in trouble anyway. */
#if !defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
static noinline int early_drop(struct net *net, unsigned int hash)
{
	/* Use oldest entry, which is roughly LRU */
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct = NULL, *tmp;
	struct hlist_nulls_node *n;
	unsigned int i, cnt = 0;
	int dropped = 0;

	rcu_read_lock();
	for (i = 0; i < nf_conntrack_htable_size; i++) {
		hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash],
					 hnnode) {
			tmp = nf_ct_tuplehash_to_ctrack(h);
			if (!test_bit(IPS_ASSURED_BIT, &tmp->status))
				ct = tmp;
			cnt++;
		}

		if (ct && unlikely(nf_ct_is_dying(ct) ||
				   !atomic_inc_not_zero(&ct->ct_general.use)))
			ct = NULL;
		if (ct || cnt >= NF_CT_EVICTION_RANGE)
			break;
		hash = (hash + 1) % nf_conntrack_htable_size;
	}
	rcu_read_unlock();

	if (!ct)
		return dropped;

	if (del_timer(&ct->timeout)) {
		death_by_timeout((unsigned long)ct);
		dropped = 1;
		NF_CT_STAT_INC_ATOMIC(net, early_drop);
	}
	nf_ct_put(ct);
	return dropped;
}
#endif

struct nf_conn *nf_conntrack_alloc(struct net *net,
				   const struct nf_conntrack_tuple *orig,
				   const struct nf_conntrack_tuple *repl,
				   gfp_t gfp)
{
	struct nf_conn *ct;

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW) && !defined(CONFIG_RTL8676)
	int nf_conntrack_threshold = (nf_conntrack_max * 80)/100;
	if((nf_conntrack_max- nf_conntrack_threshold) > 160)
		nf_conntrack_threshold = nf_conntrack_max-160;
#endif //CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW

	if (unlikely(!nf_conntrack_hash_rnd_initted)) {
		get_random_bytes(&nf_conntrack_hash_rnd,
				sizeof(nf_conntrack_hash_rnd));
		nf_conntrack_hash_rnd_initted = 1;
	}

	/* We don't want any race condition at early drop stage */
	atomic_inc(&net->ct.count);

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW) && !defined(CONFIG_RTL8676)
	if (nf_conntrack_max && unlikely(atomic_read(&net->ct.count) > nf_conntrack_threshold))
#else	
	if (nf_conntrack_max && unlikely(atomic_read(&net->ct.count) > nf_conntrack_max))
#endif
	{
#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
		//if(!drop_one_conntrack())
		if(!drop_conntracks(net))
#else	
		unsigned int hash = hash_conntrack(orig);
		/* Try dropping from this hash chain. */
		if (!early_drop(net, hash)) 
#endif		
		{
			atomic_dec(&net->ct.count);
			if (net_ratelimit())
				printk(KERN_WARNING
				       "nf_conntrack: table full, dropping"
				       " packet.\n");
			return ERR_PTR(-ENOMEM);
		}
	}

	/*
	 * Do not use kmem_cache_zalloc(), as this cache uses
	 * SLAB_DESTROY_BY_RCU.
	 */
	ct = kmem_cache_alloc(nf_conntrack_cachep, gfp);
	if (ct == NULL) {
		pr_debug("nf_conntrack_alloc: Can't alloc conntrack.\n");
		atomic_dec(&net->ct.count);
		return ERR_PTR(-ENOMEM);
	}
	/*
	 * Let ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode.next
	 * and ct->tuplehash[IP_CT_DIR_REPLY].hnnode.next unchanged.
	 */
	memset(&ct->tuplehash[IP_CT_DIR_MAX], 0,
	       sizeof(*ct) - offsetof(struct nf_conn, tuplehash[IP_CT_DIR_MAX]));
	ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple = *orig;
	ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode.pprev = NULL;
	ct->tuplehash[IP_CT_DIR_REPLY].tuple = *repl;
	ct->tuplehash[IP_CT_DIR_REPLY].hnnode.pprev = NULL;
	/* Don't set timer yet: wait for confirmation */
	setup_timer(&ct->timeout, death_by_timeout, (unsigned long)ct);
#ifdef CONFIG_NET_NS
	ct->ct_net = net;
#endif

	/*
	 * changes to lookup keys must be done before setting refcnt to 1
	 */
	smp_wmb();
	atomic_set(&ct->ct_general.use, 1);
#ifdef CONFIG_TW_DERIVED_CON 
    	INIT_LIST_HEAD(&ct->derived_connections);
 #endif     
	return ct;
}
EXPORT_SYMBOL_GPL(nf_conntrack_alloc);

void nf_conntrack_free(struct nf_conn *ct)
{
	struct net *net = nf_ct_net(ct);

	nf_ct_ext_destroy(ct);
	atomic_dec(&net->ct.count);
	nf_ct_ext_free(ct);
	kmem_cache_free(nf_conntrack_cachep, ct);
}
EXPORT_SYMBOL_GPL(nf_conntrack_free);

/* Allocate a new conntrack: we return -ENOMEM if classification
   failed due to stress.  Otherwise it really is unclassifiable. */
static struct nf_conntrack_tuple_hash *
init_conntrack(struct net *net,
	       const struct nf_conntrack_tuple *tuple,
	       struct nf_conntrack_l3proto *l3proto,
	       struct nf_conntrack_l4proto *l4proto,
	       struct sk_buff *skb,
	       unsigned int dataoff)
{
	struct nf_conn *ct;
	struct nf_conn_help *help;
	struct nf_conntrack_tuple repl_tuple;
	struct nf_conntrack_expect *exp;

	if (!nf_ct_invert_tuple(&repl_tuple, tuple, l3proto, l4proto)) {
		pr_debug("Can't invert tuple.\n");
		return NULL;
	}

	ct = nf_conntrack_alloc(net, tuple, &repl_tuple, GFP_ATOMIC);
	if (IS_ERR(ct)) {
		pr_debug("Can't allocate conntrack.\n");
		return (struct nf_conntrack_tuple_hash *)ct;
	}

	if (!l4proto->new(ct, skb, dataoff)) {
		nf_conntrack_free(ct);
		pr_debug("init conntrack: can't track with proto module\n");
		return NULL;
	}

         //设置连接跟踪的组播状态标识
         if(((tuple->src.l3num == PF_INET) && ((tuple->dst.u3.ip&htonl(0xE0000000))==htonl(0xE0000000))) ||
            ((tuple->src.l3num == PF_INET6) && ((tuple->dst.u3.ip6[0]&htonl(0xFF000000))==htonl(0xFF000000))))
            __set_bit(IPS_MULTICAST_BIT, &ct->status);


	nf_ct_acct_ext_add(ct, GFP_ATOMIC);

	spin_lock_bh(&nf_conntrack_lock);
	exp = nf_ct_find_expectation(net, tuple);
	if (exp) {
		pr_debug("conntrack: expectation arrives ct=%p exp=%p\n",
			 ct, exp);
		/* Welcome, Mr. Bond.  We've been expecting you... */
		__set_bit(IPS_EXPECTED_BIT, &ct->status);
                 //设置连接跟踪的fullconenat状态标识
                 if(exp->flags & NF_CT_EXPECT_FULLCONENAT)
                    __set_bit(IPS_FULLCONENAT_BIT, &ct->status);
		ct->master = exp->master;
        
#ifdef CONFIG_TW_DERIVED_CON
		list_add(&ct->derived_list,
			 &exp->master->derived_connections);
		if (exp->flags & NF_CT_EXPECT_DERIVED_TIMEOUT) {
			ct->derived_timeout = exp->derived_timeout;
		}
#endif
		if (exp->helper) {
			help = nf_ct_helper_ext_add(ct, GFP_ATOMIC);
			if (help)
				rcu_assign_pointer(help->helper, exp->helper);
		}

#ifdef CONFIG_NF_CONNTRACK_MARK
		ct->mark = exp->master->mark;
#endif
#ifdef CONFIG_NF_CONNTRACK_SECMARK
		ct->secmark = exp->master->secmark;
#endif
		nf_conntrack_get(&ct->master->ct_general);
		NF_CT_STAT_INC(net, expect_new);
	} else {
		__nf_ct_try_assign_helper(ct, GFP_ATOMIC);
		NF_CT_STAT_INC(net, new);
	}

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	INIT_LIST_HEAD(&ct->state_tuple);
#endif


	/* Overload tuple linked list to put us in unconfirmed list. */
	hlist_nulls_add_head_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode,
		       &net->ct.unconfirmed);

	spin_unlock_bh(&nf_conntrack_lock);

	if (exp) {
		if (exp->expectfn)
			exp->expectfn(ct, exp);
		nf_ct_expect_put(exp);
	}

	return &ct->tuplehash[IP_CT_DIR_ORIGINAL];
}

/* On success, returns conntrack ptr, sets skb->nfct and ctinfo */
static inline struct nf_conn *
resolve_normal_ct(struct net *net,
		  struct sk_buff *skb,
		  unsigned int dataoff,
		  u_int16_t l3num,
		  u_int8_t protonum,
		  struct nf_conntrack_l3proto *l3proto,
		  struct nf_conntrack_l4proto *l4proto,
		  int *set_reply,
		  enum ip_conntrack_info *ctinfo)
{
	struct nf_conntrack_tuple tuple;
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct;

	if (!nf_ct_get_tuple(skb, skb_network_offset(skb),
			     dataoff, l3num, protonum, &tuple, l3proto,
			     l4proto)) {
		pr_debug("resolve_normal_ct: Can't get tuple\n");
		return NULL;
	}

	/* look for tuple match */
	h = nf_conntrack_find_get(net, &tuple);
	if (!h) {
		h = init_conntrack(net, &tuple, l3proto, l4proto, skb, dataoff);
		if (!h)
			return NULL;
		if (IS_ERR(h))
			return (void *)h;
	}
	ct = nf_ct_tuplehash_to_ctrack(h);

	/* It exists; we have (non-exclusive) reference. */
	if (NF_CT_DIRECTION(h) == IP_CT_DIR_REPLY) {
		*ctinfo = IP_CT_ESTABLISHED + IP_CT_IS_REPLY;
		/* Please set reply bit if this packet OK */
		*set_reply = 1;
	} else {
		/* Once we've had two way comms, always ESTABLISHED. */
		if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) {
			pr_debug("nf_conntrack_in: normal packet for %p\n", ct);
			*ctinfo = IP_CT_ESTABLISHED;
		} else if (test_bit(IPS_EXPECTED_BIT, &ct->status)) {
			pr_debug("nf_conntrack_in: related packet for %p\n",
				 ct);
			*ctinfo = IP_CT_RELATED;
		} else {
			pr_debug("nf_conntrack_in: new packet for %p\n", ct);
			*ctinfo = IP_CT_NEW;
		}
		*set_reply = 0;
	}
	skb->nfct = &ct->ct_general;
	skb->nfctinfo = *ctinfo;
	return ct;
}

unsigned int
nf_conntrack_in(struct net *net, u_int8_t pf, unsigned int hooknum,
		struct sk_buff *skb)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	struct nf_conntrack_l3proto *l3proto;
	struct nf_conntrack_l4proto *l4proto;
	unsigned int dataoff;
	u_int8_t protonum;
	int set_reply = 0;
	int ret;
#if !defined(CONFIG_RTL_8676HWNAT) && defined(CONFIG_RTL867X_IPTABLES_FAST_PATH)
        int first_reply=0;
#endif

	
#if !defined(IMPROVE_QOS)
#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	int assured=0;
	int create_udp_link = 0;
	int create_tcp_link = 0;
#endif
#endif


	/* Previously seen (loopback or untracked)?  Ignore. */
	if (skb->nfct) {
		NF_CT_STAT_INC_ATOMIC(net, ignore);
		return NF_ACCEPT;
	}

	/* rcu_read_lock()ed by nf_hook_slow */
	l3proto = __nf_ct_l3proto_find(pf);
	ret = l3proto->get_l4proto(skb, skb_network_offset(skb),
				   &dataoff, &protonum);
	if (ret <= 0) {
		pr_debug("not prepared to track yet or error occured\n");
		NF_CT_STAT_INC_ATOMIC(net, error);
		NF_CT_STAT_INC_ATOMIC(net, invalid);
		return -ret;
	}

	l4proto = __nf_ct_l4proto_find(pf, protonum);

	/* It may be an special packet, error, unclean...
	 * inverse of the return code tells to the netfilter
	 * core what to do with the packet. */
	if (l4proto->error != NULL) {
		ret = l4proto->error(net, skb, dataoff, &ctinfo, pf, hooknum);
		if (ret <= 0) {
			NF_CT_STAT_INC_ATOMIC(net, error);
			NF_CT_STAT_INC_ATOMIC(net, invalid);
			return -ret;
		}
	}

	ct = resolve_normal_ct(net, skb, dataoff, pf, protonum,
			       l3proto, l4proto, &set_reply, &ctinfo);
	if (!ct) {
		/* Not valid part of a connection */
		NF_CT_STAT_INC_ATOMIC(net, invalid);
		return NF_ACCEPT;
	}

	if (IS_ERR(ct)) {
		/* Too stressed to deal. */
		NF_CT_STAT_INC_ATOMIC(net, drop);
		return NF_DROP;
	}

	NF_CT_ASSERT(skb->nfct);

	ret = l4proto->packet(ct, skb, dataoff, ctinfo, pf, hooknum);
	if (ret <= 0) {
		/* Invalid: inverse of the return code tells
		 * the netfilter core what to do */
		pr_debug("nf_conntrack_in: Can't track with proto module\n");
		nf_conntrack_put(skb->nfct);
		skb->nfct = NULL;
		NF_CT_STAT_INC_ATOMIC(net, invalid);
		if (ret == -NF_DROP)
			NF_CT_STAT_INC_ATOMIC(net, drop);
		return -ret;
	}

#if !defined(IMPROVE_QOS)	
#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	/*
	 * ### for iperf application test ###
	 * the behavior of iperf UDP test is LAN PC (client) will burst UDP from LAN to WAN (by one way),
	 * WAN PC (server) will only send one UDP packet (statistics) at the end of test.
	 * so the fastpath or hardware NAT will create link at the end of test.
	 *
	 * the purpose for adding the following code is to create fastpath or hardware NAT link 
	 * when we only get one packet from LAN to WAN in UDP case.
	 */
	{
		extern unsigned int _br0_ip;
		extern unsigned int _br0_mask;
		uint32 sip, dip;
		//unsigned long nf_ct_tcp_timeout_established = 5*24*60*60*HZ;
		unsigned long nf_ct_udp_timeout_stream = 180*HZ;

		sip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
		dip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;

		if ((ip_hdr(skb)->protocol == IPPROTO_UDP) &&
				((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
					 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)
#if defined(UNNUMBER_IP)
					 && (is_unnumber_ip(dip)==FALSE)
#endif
					 )) 
		{ 
			/* UDP and "LAN to WAN" */
			/* ignore some cases:  
			 *	1. sip = br0's ip -----> (ex. sip 192.168.1.254 ==> dip 239.255.255.250)
			 * 	2. (sip & br0's mask) != (br0's ip & br0's mask) -----> sip is not in br0's subnet
			 *	3. (dip & br0's mask) =  (br0's ip & br0's mask) -----> dip is in br0's subnet
			 *	4. dip != multicast IP address
			 *	5. sip != gip
			 */
			if ((sip != _br0_ip) &&
				((sip & _br0_mask) == (_br0_ip & _br0_mask)) &&
				((dip & _br0_mask) != (_br0_ip & _br0_mask)) &&
				((dip & 0xf0000000) != 0xe0000000) &&
				(sip != (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip))
				)
			{
				create_udp_link = 1;
				/* copied from last 2 line of this function **/
				set_bit(IPS_SEEN_REPLY_BIT, &ct->status);
				/* next 3 lines are copied from udp_packet() in ip_conntrack_proto_udp.c */
				nf_ct_refresh(ct,skb, nf_ct_udp_timeout_stream);
				/* Also, more likely to be important, and not a probe */
				set_bit(IPS_ASSURED_BIT, &ct->status);
 			}
#if defined(UNNUMBER_IP)	
			else if (((is_unnumber_ip(sip)==TRUE) && (is_unnumber_ip(dip)==FALSE)))
			{
				create_udp_link = 1;
			}
#endif
		}
		else if((ip_hdr(skb)->protocol == IPPROTO_TCP) &&
	 		((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip==ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip) 
#if defined(UNNUMBER_IP)
	 		&& (is_unnumber_ip(dip)==FALSE)
#endif
	 		)&& !assured)
 		{
 			
			struct iphdr *iph=ip_hdr(skb);
			struct tcphdr *tcph=(void *) iph + iph->ihl*4;
				if (!tcph->fin && !tcph->syn && !tcph->rst && tcph->psh==1 && 
					tcph->ack ==1 &&  (iph->daddr !=_br0_ip) && ((sip & _br0_mask) == (_br0_ip & _br0_mask)) &&
					((dip & _br0_mask) != (_br0_ip & _br0_mask)) && (sip != (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip))){ 
						
						smart_count++;
						if(smart_count >810){
							//panic_printk("the case hit for mart flow:tcp state=%d, assured=%d\n",ct->proto.tcp.state,test_bit(IPS_ASSURED_BIT, &ct->status));
							create_tcp_link=1;
						}
				}	

			if ((!create_tcp_link) 
#if defined(UNNUMBER_IP)
				&& (is_unnumber_ip(sip)==TRUE) && (is_unnumber_ip(dip)==FALSE) 
#endif
				)
			{
				create_tcp_link = 1;
			}
 		}
	}
#endif

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	assured = test_bit(IPS_ASSURED_BIT, &ct->status);
#endif


#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	
	/*1.add "!(ct->helper)" to fix ftp-cmd type packet 
	  2.add identify case LAN->WAN(AB->BC) or WAN->LAN(BC->AB) 
	  3.add !(ct->nat.info.helper) for best ALG avoid
	  */

	if (!(nfct_help(ct)) &&  (assured) && ((create_udp_link) || (create_tcp_link)))
	{


		if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP) {
			/* TCP Connection Tracking */

			//printk("******%s(%d), ipid(%d)\n",__FUNCTION__,__LINE__,ip_hdr(skb)->id);
			//printk("#####%s(%d), original.sip(0x%x),original.dip(0x%x),reply.sip(0x%x),reply.dip(0x%x)\n",__FUNCTION__,__LINE__,
			//ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
			//ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip
			//);
			
			if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
			   == ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)
			{
				/*case BC->AB*/
				#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook6!=NULL)
				{
					FastPath_hook6(NP_TCP,
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),	
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port),				
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
					NP_NONE);					
				}
				#else
				
				rtk_addNaptConnection(NP_TCP,
				ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),	
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port),				
				ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
				NP_NONE);		
				#endif
				
			}else if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
				 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)
			{
					
				/*case AB->BC*/
				#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook6!=NULL)
				{
					FastPath_hook6(NP_TCP, 
						ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.tcp.port),
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),
						NP_NONE);
				}
				#else
				rtk_addNaptConnection(NP_TCP, 
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.tcp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.tcp.port),
					NP_NONE);
				#endif
			}
							
		} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {
			/* UDP Connection Tracking */
			/* check "LAN to WAN (AB->BC)" first */
			 if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 
				== ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)				
			{
			/*case AB->BC*/
			#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook6!=NULL)
				{
					FastPath_hook6(NP_UDP, 
						ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port),
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),
						NP_NONE);
				}
				#else
				rtk_addNaptConnection(NP_UDP, 
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port),
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),
					NP_NONE);
				#endif
			}
			else if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
				== ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)
			{
			/*case BC->AB*/
			#ifdef CONFIG_FAST_PATH_MODULE
				if(FastPath_hook6!=NULL)
				{
					FastPath_hook6(NP_UDP,
						ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),	
						ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port),				
						ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
						ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
						NP_NONE);
				}
				#else
				rtk_addNaptConnection(NP_UDP,
					ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),	
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port),				
					ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
					ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
					NP_NONE);			
				#endif				
			}
			

		}

	}
#endif
#endif  //End of !defined(IMPROVE_QOS)


/*linux-2.6.19*/ 
#if 1
	if (set_reply && !test_bit(IPS_SEEN_REPLY_BIT, &ct->status)){
		#if !defined(CONFIG_RTL_8676HWNAT) && defined(CONFIG_RTL867X_IPTABLES_FAST_PATH)
           first_reply=1;
		#endif
        }

	#if !defined(CONFIG_RTL_8676HWNAT) && defined(CONFIG_RTL867X_IPTABLES_FAST_PATH)
	if(nf_ct_l3num(ct)==PF_INET)
	{
		struct nf_conn_help *help;
		help = nfct_help(ct);
		
		if (FastPath_Enabled()) {
			//cathy
			//info = &ct->nat.info;
			//helper = info->helper;
			//if (!ct->helper &&  set_reply && first_reply) 
			if (!(help && help->helper) &&  set_reply && first_reply) 
			{
				if( ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum != IPPROTO_ICMP ) {
					//printk("napt add !!\n");
					rtl867x_addNaptConnection(ct,ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
						ct->tuplehash[IP_CT_DIR_REPLY].tuple, NP_NONE,1);
				}
			}	/* IPS_SEEN_REPLY_BIT up */
		}
	}
	#endif

	if (set_reply && !test_bit(IPS_SEEN_REPLY_BIT, &ct->status)){
		test_and_set_bit(IPS_SEEN_REPLY_BIT, &ct->status) ;
		nf_conntrack_event_cache(IPCT_STATUS, ct);
        }

#else
	if (set_reply && !test_and_set_bit(IPS_SEEN_REPLY_BIT, &ct->status))
		nf_conntrack_event_cache(IPCT_STATUS, ct);
#endif //#if 1

	return ret;
}
EXPORT_SYMBOL_GPL(nf_conntrack_in);

bool nf_ct_invert_tuplepr(struct nf_conntrack_tuple *inverse,
			  const struct nf_conntrack_tuple *orig)
{
	bool ret;

	rcu_read_lock();
	ret = nf_ct_invert_tuple(inverse, orig,
				 __nf_ct_l3proto_find(orig->src.l3num),
				 __nf_ct_l4proto_find(orig->src.l3num,
						      orig->dst.protonum));
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL_GPL(nf_ct_invert_tuplepr);

/* Alter reply tuple (maybe alter helper).  This is for NAT, and is
   implicitly racy: see __nf_conntrack_confirm */
void nf_conntrack_alter_reply(struct nf_conn *ct,
			      const struct nf_conntrack_tuple *newreply)
{
	struct nf_conn_help *help = nfct_help(ct);

	/* Should be unconfirmed, so not in hash table yet */
	NF_CT_ASSERT(!nf_ct_is_confirmed(ct));

	pr_debug("Altering reply tuple of %p to ", ct);
	nf_ct_dump_tuple(newreply);

	ct->tuplehash[IP_CT_DIR_REPLY].tuple = *newreply;
	if (ct->master || (help && !hlist_empty(&help->expectations)))
		return;

	rcu_read_lock();
	__nf_ct_try_assign_helper(ct, GFP_ATOMIC);
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(nf_conntrack_alter_reply);

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)

#ifdef CONFIG_PROC_FS
static int getinfo_conntrack_proto_show(struct seq_file *m, void *v)
{
	seq_printf(m,
		"%08x  %08x %08x\n",
		atomic_read(&prot_counters[PROT_ICMP]),
		atomic_read(&prot_counters[PROT_TCP]),
		atomic_read(&prot_counters[PROT_UDP]));
  	return 0;
}

static int getinfo_conntrack_proto_open(struct inode *inode, struct file *file)
{
	return single_open(file, getinfo_conntrack_proto_show, NULL);
}

static const struct file_operations getinfo_conntrack_proto_fops = {
	.open		= getinfo_conntrack_proto_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.owner		= THIS_MODULE,
};
#endif //CONFIG_PROC_FS

void nf_ct_refresh_udp(struct nf_conn *ct, unsigned long extra_jiffies,char* status)
{
  	NF_CT_ASSERT(ct->timeout.data == (unsigned long)ct);
	spin_lock_bh(&nf_conntrack_lock);
	/* If not in hash table, timer will not be active yet */
	//if (!is_confirmed(ct))
	//if(!test_bit(IPS_CONFIRMED_BIT, &ct->status))	
	if (!nf_ct_is_confirmed(ct))	
		ct->timeout.expires = extra_jiffies;
	else {
		/* Need del_timer for race avoidance (may already be dying). */
		if (del_timer(&ct->timeout)) {
			ct->timeout.expires = jiffies + extra_jiffies;
			add_timer(&ct->timeout);

			list_del(&ct->state_tuple);
		 	ct->state_tuple.next=NULL;
			ct->state_tuple.prev=NULL;
			if (ct->status & IPS_SEEN_REPLY) {
				list_add_tail(&ct->state_tuple, &udp_assured_list);
			} else {
				list_add_tail(&ct->state_tuple, &udp_unreply_list);
			}			
	
		}
	}
	spin_unlock_bh(&nf_conntrack_lock);
}
void nf_ct_refresh_tcp(struct nf_conn *ct, unsigned long extra_jiffies,enum tcp_conntrack oldstate,enum tcp_conntrack newstate )
{
        NF_CT_ASSERT(ct->timeout.data == (unsigned long)ct);

	spin_lock_bh(&nf_conntrack_lock);
	/* If not in hash table, timer will not be active yet */
	//if (!is_confirmed(ct))
	//if (!test_bit(IPS_CONFIRMED_BIT, &ct->status))
	if (!nf_ct_is_confirmed(ct))
		ct->timeout.expires = extra_jiffies;
	else {
		/* Need del_timer for race avoidance (may already be dying). */
		if (del_timer(&ct->timeout)) {
			ct->timeout.expires = jiffies + extra_jiffies;
			add_timer(&ct->timeout);		
						
			list_del(&ct->state_tuple);                     
			ct->state_tuple.next=NULL;
			ct->state_tuple.prev=NULL;			
			list_add_tail(&ct->state_tuple, Tcp_State_Hash_Head[newstate].state_hash);
		}
	}
	spin_unlock_bh(&nf_conntrack_lock);
}
#endif //CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW


/* Refresh conntrack for this many jiffies and do accounting if do_acct is 1 */
void __nf_ct_refresh_acct(struct nf_conn *ct,
			  enum ip_conntrack_info ctinfo,
			  const struct sk_buff *skb,
			  unsigned long extra_jiffies,
			  int do_acct)
{
	int event = 0;

	NF_CT_ASSERT(ct->timeout.data == (unsigned long)ct);
	NF_CT_ASSERT(skb);

	spin_lock_bh(&nf_conntrack_lock);

	/* Only update if this is not a fixed timeout */
	if (test_bit(IPS_FIXED_TIMEOUT_BIT, &ct->status))
		goto acct;

	/* If not in hash table, timer will not be active yet */
	if (!nf_ct_is_confirmed(ct)) {
		ct->timeout.expires = extra_jiffies;
		event = IPCT_REFRESH;
	} else {
		unsigned long newtime = jiffies + extra_jiffies;

		/* Only update the timeout if the new timeout is at least
		   HZ jiffies from the old timeout. Need del_timer for race
		   avoidance (may already be dying). */
		if (newtime - ct->timeout.expires >= HZ
		    && del_timer(&ct->timeout)) {
			ct->timeout.expires = newtime;
			add_timer(&ct->timeout);
			event = IPCT_REFRESH;
		}
	}

acct:
	if (do_acct) {
		struct nf_conn_counter *acct;

		acct = nf_conn_acct_find(ct);
		if (acct) {
			acct[CTINFO2DIR(ctinfo)].packets++;
			acct[CTINFO2DIR(ctinfo)].bytes +=
				skb->len - skb_network_offset(skb);
		}
	}

	spin_unlock_bh(&nf_conntrack_lock);

	/* must be unlocked when calling event cache */
	if (event)
		nf_conntrack_event_cache(event, ct);
}
EXPORT_SYMBOL_GPL(__nf_ct_refresh_acct);

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)

void __nf_ct_refresh_acct_proto(struct nf_conn *ct, 
				      enum ip_conntrack_info ctinfo,
				      const struct sk_buff *skb,
				      unsigned long extra_jiffies,
				      int do_acct,
				      unsigned char proto, 
				      void * extra1,
				      void * extra2)
{

	int event = 0;

	NF_CT_ASSERT(ct->timeout.data == (unsigned long)ct);
	NF_CT_ASSERT(skb);

	spin_lock_bh(&nf_conntrack_lock);

	/* Only update if this is not a fixed timeout */
	if (test_bit(IPS_FIXED_TIMEOUT_BIT, &ct->status))
		goto acct;

	/* If not in hash table, timer will not be active yet */
	if (!nf_ct_is_confirmed(ct)) {
		ct->timeout.expires = extra_jiffies;
		event = IPCT_REFRESH;
	} else {
		unsigned long newtime = jiffies + extra_jiffies;

		/* Only update the timeout if the new timeout is at least
		   HZ jiffies from the old timeout. Need del_timer for race
		   avoidance (may already be dying). */
		if (newtime - ct->timeout.expires >= HZ
		    && del_timer(&ct->timeout)) {
			ct->timeout.expires = newtime;
			add_timer(&ct->timeout);
			event = IPCT_REFRESH;

			switch(proto) {
			case 6:
				list_del(&ct->state_tuple);                     
				ct->state_tuple.next=NULL;
				ct->state_tuple.prev=NULL;			
				list_add_tail(&ct->state_tuple, Tcp_State_Hash_Head[(enum tcp_conntrack) extra2].state_hash);
				break;
			case 17:
				list_del(&ct->state_tuple);
			 	ct->state_tuple.next=NULL;
				ct->state_tuple.prev=NULL;
				if (ct->status & IPS_SEEN_REPLY) {
					list_add_tail(&ct->state_tuple, &udp_assured_list);
				} else {
					list_add_tail(&ct->state_tuple, &udp_unreply_list);
				}
				break;				
			}
		}
	}

acct:
	if (do_acct) {
		struct nf_conn_counter *acct;

		acct = nf_conn_acct_find(ct);
		if (acct) {
			acct[CTINFO2DIR(ctinfo)].packets++;
			acct[CTINFO2DIR(ctinfo)].bytes +=
				skb->len - skb_network_offset(skb);
		}
	}

	spin_unlock_bh(&nf_conntrack_lock);

	/* must be unlocked when calling event cache */
	if (event)
		nf_conntrack_event_cache(event, ct);
}


#endif //CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW


bool __nf_ct_kill_acct(struct nf_conn *ct,
		       enum ip_conntrack_info ctinfo,
		       const struct sk_buff *skb,
		       int do_acct)
{
	if (do_acct) {
		struct nf_conn_counter *acct;

		spin_lock_bh(&nf_conntrack_lock);
		acct = nf_conn_acct_find(ct);
		if (acct) {
			acct[CTINFO2DIR(ctinfo)].packets++;
			acct[CTINFO2DIR(ctinfo)].bytes +=
				skb->len - skb_network_offset(skb);
		}
		spin_unlock_bh(&nf_conntrack_lock);
	}

	if (del_timer(&ct->timeout)) {
		ct->timeout.function((unsigned long)ct);
		return true;
	}
	return false;
}
EXPORT_SYMBOL_GPL(__nf_ct_kill_acct);

#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)

#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_conntrack.h>
#include <linux/mutex.h>

/* Generic function for tcp/udp/sctp/dccp and alike. This needs to be
 * in ip_conntrack_core, since we don't want the protocols to autoload
 * or depend on ctnetlink */
int nf_ct_port_tuple_to_nlattr(struct sk_buff *skb,
			       const struct nf_conntrack_tuple *tuple)
{
	NLA_PUT_BE16(skb, CTA_PROTO_SRC_PORT, tuple->src.u.tcp.port);
	NLA_PUT_BE16(skb, CTA_PROTO_DST_PORT, tuple->dst.u.tcp.port);
	return 0;

nla_put_failure:
	return -1;
}
EXPORT_SYMBOL_GPL(nf_ct_port_tuple_to_nlattr);

const struct nla_policy nf_ct_port_nla_policy[CTA_PROTO_MAX+1] = {
	[CTA_PROTO_SRC_PORT]  = { .type = NLA_U16 },
	[CTA_PROTO_DST_PORT]  = { .type = NLA_U16 },
};
EXPORT_SYMBOL_GPL(nf_ct_port_nla_policy);

int nf_ct_port_nlattr_to_tuple(struct nlattr *tb[],
			       struct nf_conntrack_tuple *t)
{
	if (!tb[CTA_PROTO_SRC_PORT] || !tb[CTA_PROTO_DST_PORT])
		return -EINVAL;

	t->src.u.tcp.port = nla_get_be16(tb[CTA_PROTO_SRC_PORT]);
	t->dst.u.tcp.port = nla_get_be16(tb[CTA_PROTO_DST_PORT]);

	return 0;
}
EXPORT_SYMBOL_GPL(nf_ct_port_nlattr_to_tuple);

int nf_ct_port_nlattr_tuple_size(void)
{
	return nla_policy_len(nf_ct_port_nla_policy, CTA_PROTO_MAX + 1);
}
EXPORT_SYMBOL_GPL(nf_ct_port_nlattr_tuple_size);
#endif

/* Used by ipt_REJECT and ip6t_REJECT. */
static void nf_conntrack_attach(struct sk_buff *nskb, struct sk_buff *skb)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;

	/* This ICMP is in reverse direction to the packet which caused it */
	ct = nf_ct_get(skb, &ctinfo);
	if (CTINFO2DIR(ctinfo) == IP_CT_DIR_ORIGINAL)
		ctinfo = IP_CT_RELATED + IP_CT_IS_REPLY;
	else
		ctinfo = IP_CT_RELATED;

	/* Attach to new skbuff, and increment count */
	nskb->nfct = &ct->ct_general;
	nskb->nfctinfo = ctinfo;
	nf_conntrack_get(nskb->nfct);
}

/* Bring out ya dead! */
static struct nf_conn *
get_next_corpse(struct net *net, int (*iter)(struct nf_conn *i, void *data),
		void *data, unsigned int *bucket)
{
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct;
	struct hlist_nulls_node *n;

	spin_lock_bh(&nf_conntrack_lock);
	for (; *bucket < nf_conntrack_htable_size; (*bucket)++) {
		hlist_nulls_for_each_entry(h, n, &net->ct.hash[*bucket], hnnode) {
			ct = nf_ct_tuplehash_to_ctrack(h);
			if (iter(ct, data))
				goto found;
		}
	}
	hlist_nulls_for_each_entry(h, n, &net->ct.unconfirmed, hnnode) {
		ct = nf_ct_tuplehash_to_ctrack(h);
		if (iter(ct, data))
			set_bit(IPS_DYING_BIT, &ct->status);
	}
	spin_unlock_bh(&nf_conntrack_lock);
	return NULL;
found:
	atomic_inc(&ct->ct_general.use);
	spin_unlock_bh(&nf_conntrack_lock);
	return ct;
}

void nf_ct_iterate_cleanup(struct net *net,
			   int (*iter)(struct nf_conn *i, void *data),
			   void *data)
{
	struct nf_conn *ct;
	unsigned int bucket = 0;

	while ((ct = get_next_corpse(net, iter, data, &bucket)) != NULL) {
		/* Time to push up daises... */
		if (del_timer(&ct->timeout))
			death_by_timeout((unsigned long)ct);
		/* ... else the timer will get him soon. */

		nf_ct_put(ct);
	}
}
EXPORT_SYMBOL_GPL(nf_ct_iterate_cleanup);

struct __nf_ct_flush_report {
	u32 pid;
	int report;
};

static int kill_all(struct nf_conn *i, void *data)
{
	struct __nf_ct_flush_report *fr = (struct __nf_ct_flush_report *)data;

	/* get_next_corpse sets the dying bit for us */
	nf_conntrack_event_report(IPCT_DESTROY,
				  i,
				  fr->pid,
				  fr->report);
	return 1;
}

//add by liucaigen

struct conntrack_ct_match {
	unsigned short  devifid;
	u_int8_t protonum;//protocol num tcp:6 udp:17 icmp:1
	u_int8_t dir;//IP_CT_DIR_ORIGINAL=0  IP_CT_DIR_REPLY=1  both=2
	u_int32_t sip;//src ip addr
	u_int32_t smaskip;//src ip addr mask
	u_int16_t sport;//sport start
	u_int16_t sport_end;//sport end
	u_int32_t dip;//dst ip
	u_int32_t dmaskip;//dst ip mask
	u_int16_t dport;//dport start
	u_int16_t dport_end;//dport end 
	u_int8_t nat_ct_flag;//0 is not match; 1 is nat match; 2 is not nat 
	u_int8_t local_ct;//0 is not match; 1 is local_ct; 2 is not loacl match
	u_int8_t CT_DIR_ORIGINAL_FLAG;//conntrack dir default oringinal: 1 use in ip addr 
	u_int8_t CT_DIR_OREPLY_FLAG;//conntrack dir default reply :1 use in ip addr 
    u_int8_t sip_flag;//! sip
	u_int8_t dip_flag;//!dip
	u_int8_t dport_flag;//!dport
	u_int8_t sport_flag;//!sport
};

//conntrack original sport/dport
#define nf_ct_osport(x) (x)->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all 
#define nf_ct_odport(x) (x)->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all 

//conntrack reply sport/dport
#define nf_ct_rsport(x) (x)->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all 
#define nf_ct_rdport(x) (x)->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all

//conntrack original src ip/dst ip
#define nf_ct_osip(x) (x)->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip 
#define nf_ct_odip(x) (x)->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip 

//conntrack reply src ip/dst ip
#define nf_ct_rsip(x) (x)->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip 
#define nf_ct_rdip(x) (x)->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip

//struck conntrack_ct_match conntrack dir 
#define original_flag(x) (x)->CT_DIR_ORIGINAL_FLAG
#define reply_flag(x) (x)->CT_DIR_OREPLY_FLAG

/* nat conn ct
tcp      6 7 TIME_WAIT src=192.168.1.73 dst=192.168.1.1 sport=1201 dport=80 src=192.168.1.1 dst=192.168.1.73 sport=80 dport=1201 [ASSURED] use=2
*/

/*no nat conn ct
tcp      6 7 TIME_WAIT src=192.168.1.73 dst=10.10.10.10 sport=1201 dport=80 src=10.10.10.10 dst=10.10.10.1 sport=80 dport=1201 [ASSURED] use=2
*/

//conntrack is nat 
#define likely_nat_ct(x) !(((nf_ct_osip(x)) == (nf_ct_rdip(x))) && ((nf_ct_odip(x)) == (nf_ct_rsip(x) )))

//conntrack is local
#define likely_loacl_ct(x) ((dev_getipaddr(&init_net,nf_ct_osip(x),nf_ct_odip(x))) && (!likely_nat_ct(x)))



/********************************************************************
// Function Name: clear_conntrack_ct_match
// Description  : if match success return 1,other return 0
// Parameters   : nf_conn :nf_conntrack
//                data : conntrack_ct_match
// Returns      :succsee 1:failure 0;
********************************************************************/
static int clear_conntrack_ct_match(struct nf_conn *i,void *data)
{
  struct conntrack_ct_match *tuple = (struct conntrack_ct_match *)data;
  const struct nf_conn_nat *nat = nfct_nat(i);
 
  //dev id match only suppert natdev
  if(tuple->devifid)
  	{
  		if (!nat)
		   return 0;
	    if(nat->masq_index == tuple->devifid)
	    	{
	    	}
		else
		    return 0;
  	}

  //nat match
  if(tuple->nat_ct_flag)
  	switch(tuple->nat_ct_flag){
		case(1):
			if(likely_nat_ct(i))
				{
				}
			else
				return 0;
			break;
		case(2):
				if(!likely_nat_ct(i))
				{
				}
			    else
				 return 0;
			break;
		default:
					{
				}
  		}

   //local ct match
   if(tuple->local_ct)
	   switch(tuple->local_ct){
		   case(1):
			   if(likely_loacl_ct(i))
				   {
				   }
			   else
				   return 0;
			   break;
			   case(2):
				   if(!likely_loacl_ct(i))
				   {
				   }
			   else
				   return 0;
			   break;
			   default:
			   		{
			   	}
		   }

   
  //protocol
  if(tuple->protonum)
  	{
  	 if(tuple->protonum != nf_ct_protonum(i))
	 	return 0;
  	}
  //end protocol
  
  //sport match
  if(tuple->sport && (!tuple->sport_end))
  	{
  	 	if((original_flag(tuple) && (tuple->sport == nf_ct_osport(i)))  || ((tuple->sport == nf_ct_rsport(i)) && reply_flag(tuple)))
  	 		{
  	 		}
	 	else
	 		return 0;
  	}
  else if(tuple->sport && tuple->sport_end)
  	{
  	  	if((original_flag(tuple) && (tuple->sport <= nf_ct_osport(i)) && ((tuple->sport_end >= nf_ct_osport(i)))) 
			|| (reply_flag(tuple) &&(tuple->sport <= nf_ct_rsport(i)) && ((tuple->sport_end >= nf_ct_rsport(i)))) )
  	  		{
  	  		}
	 	else
	 		return 0;
  	}
   //end sport

   //dport match
	if(tuple->dport && (!tuple->dport_end))
	  {
		  if((original_flag(tuple) &&(tuple->dport == nf_ct_odport(i))) || ((tuple->dport == nf_ct_rdport(i))&& reply_flag(tuple)))
		  	{
		  	}
		  else
			  return 0;
	  }
	else if(tuple->dport && tuple->dport_end)
	  {
		  if((original_flag(tuple) &&(tuple->dport <= nf_ct_odport(i)) && ((tuple->dport_end >= nf_ct_odport(i)))) 
			  || (reply_flag(tuple) && (tuple->dport <= nf_ct_rdport(i)) && ((tuple->dport_end >= nf_ct_rdport(i)))) )
		  	{
		  	}
		  else
			  return 0;
	  }
	//end dport

    //sip match
    if(tuple->sip)
    	{
    	 if((original_flag(tuple) && ((tuple->sip & tuple->smaskip) == (nf_ct_osip(i) & tuple->smaskip)))
		 	|| (reply_flag(tuple) && ((tuple->sip & tuple->smaskip) == (nf_ct_rsip(i) & tuple->smaskip))))
    	 	{
    	 	}
		 else
			return 0;
    	}
	//end sip

	//dip match
    if(tuple->dip)
    	{
    	 if((original_flag(tuple) &&((tuple->dip & tuple->dmaskip) == (nf_ct_odip(i) & tuple->dmaskip)))
		 	|| (reply_flag(tuple) && ((tuple->dip & tuple->dmaskip) == (nf_ct_rdip(i) & tuple->dmaskip))))
    	 	{
    	 	}
		 else
			return 0;
    	}
  return 1;
}


/********************************************************************
// Function Name: clear_all_conntrack
// Description  : clear all 
// Parameters   : nf_conn :nf_conntrack
//                data : conntrack_ct_match
// Returns      :succsee 1:failure 0;
********************************************************************/
static int clear_all_conntrack(struct nf_conn *i,void *data)
{
	return 1;
}


/********************************************************************
// Function Name: read_clear_conntrack_match
// Description  : proc file read function
// Parameters   : nf_conn :nf_conntrack
//                data : conntrack_ct_match
// Returns      :succsee 1:failure 0;
********************************************************************/
static int read_clear_conntrack_match(char *page, char **start, off_t off,
				 int count, int *eof, void *data)
{
	int len = 0;
	len += sprintf(page + len, "input example\n");
	len += sprintf(page + len, "dev=ppp0 src=192.168.1.2/255.255.255.0 dst=192.168.1.10 dir=0/1/2 sport=100:101 dport=200:900 protocol=tcp !nat !localct \n");
	len += sprintf(page + len, "protocol support: tcp udp dccp sctp gre icmp esp\n");
	len += sprintf(page + len, "dir is conntrack Direction dir=0 meaning original dir=1 meaning reply dir=2 meaning both\n");
	len += sprintf(page + len, "dev=ppp0 only support nat dev MASQUERADE\n");
	len += sprintf(page + len, "nat !nat meanging conntrack nat,if input !nat can clear all local conntrack\n");
	len += sprintf(page + len, "if you clear all please input: clearall\n");

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	return len;
}

/********************************************************************
// Function Name: inet_aton
// Description  : ip string to inet 
// Parameters   : char * cp
//                data : unsigned int inaddr
// Returns      :succsee 1:failure 0;
********************************************************************/

int inet_aton (const char *cp, u_int32_t *inaddr)
{
  int dots = 0;
  register u_long addr = 0;
  register u_long val = 0, base = 10;

  do
    {
      register char c = *cp;

      switch (c)
	{
	case '0': case '1': case '2': case '3': case '4': case '5':
	case '6': case '7': case '8': case '9':
	  val = (val * base) + (c - '0');
	  break;
	case '.':
	  if (++dots > 3)
	    return 0;
	case '\0':
	  if (val > 255)
	    return 0;
	  addr = addr << 8 | val;
	  val = 0;
	  break;
	default:
	  return 0;
	}
    } while (*cp++) ;

  if (dots < 3)
    addr <<= 8 * (3 - dots);
  if (inaddr)
    *inaddr = htonl (addr);
  return 1;
}

/********************************************************************
// Function Name: write_clear_conntrack_match
// Description  : proc file write function 
// Parameters   : 
// Returns      :succsee 1:failure 0;
********************************************************************/
static int write_clear_conntrack_match(struct file *file, const char *buffer, 
				unsigned long count, void *data)
{

	 char buf[256] = "";
	 char buff[256] = "";
	 char buff_mask[24] ="";
	 char buf_tmp[100] = "";
	 uint var = 0;
	 char *p = NULL;
	 char *cp = NULL;
	 char flag = 0;
	 int dir;
	 struct conntrack_ct_match tuple;
	 struct net_device *dst_dev = NULL;
	 memset(buf,0,sizeof(buf));
        if (count > 256) {
                printk("arg is too long!!\n");
                return -EFAULT;
        }
        if (copy_from_user(buf, buffer, count))
                return -EFAULT;
    tuple.devifid = 0;
	tuple.dip     = 0;
	tuple.dmaskip = 0xffffffff;
	tuple.dport   = 0;
	tuple.dport_end = 0;
	tuple.protonum  = 0;
	tuple.sip       = 0;
	tuple.smaskip   = 0xffffffff;
	tuple.sport     = 0;
	tuple.sport_end = 0;
	tuple.CT_DIR_ORIGINAL_FLAG =1;
	tuple.CT_DIR_OREPLY_FLAG =1;
	tuple.nat_ct_flag = 0;
    memset(buf_tmp,0,sizeof(buf_tmp));
    //clear all match
    p = strstr(buf,"clearall");
    if(p)
    	{
    	 	nf_ct_iterate_cleanup(&init_net,clear_all_conntrack, NULL);
	        return count;
    	}
	//match nat
	if(strstr(buf,"!nat"))
		{
		 tuple.nat_ct_flag = 2;
		 flag = 1;
		}
	//match !nat
	else if(strstr(buf,"nat"))
		{
		 tuple.nat_ct_flag = 1;
		 flag = 1;
		}
    else
		tuple.nat_ct_flag = 0;

	//match loacl ct
	if(strstr(buf,"!localct"))
		{
		tuple.local_ct = 2;
		flag = 1;
		}
	else if(strstr(buf,"localct"))
		{
			if(tuple.nat_ct_flag == 1)
			   printk("Warning:in the local mode ct is mast be !nat");
			tuple.local_ct = 1;
			tuple.nat_ct_flag = 0;
			flag = 1;
		}
	else
		tuple.local_ct = 0;

	
	p = strstr(buf,"dir=");
	if(p)
		{
		 sscanf(p,"dir=%s",buff);
		 if(sscanf(buff,"%d",&dir))
		 	{
		 	  if(dir == 0)//match roingal
		 	  	{
			  	 tuple.CT_DIR_ORIGINAL_FLAG = 1;
				 tuple.CT_DIR_OREPLY_FLAG = 0;
		 	  	}
			  else if(dir == 1)//match reply
			  	{
			  	  tuple.CT_DIR_ORIGINAL_FLAG = 0;
				  tuple.CT_DIR_OREPLY_FLAG = 1;
			  	}
			  else if(dir == 2)//match both
			  	{
			  	  tuple.CT_DIR_ORIGINAL_FLAG = 1;
				  tuple.CT_DIR_OREPLY_FLAG = 1;
			  	}
			  else//default match both
			  	{
			  	  tuple.CT_DIR_ORIGINAL_FLAG = 1;
				  tuple.CT_DIR_OREPLY_FLAG = 1;
			  	}
		 	}
		}
    //dev match
    p = strstr(buf,"dev=");
	if(p)
		{
	    if(sscanf(p,"dev=%s",buff))
	       {
		     dst_dev = dev_get_by_name(&init_net, buff);
		     if(dst_dev)
		     	{
			     tuple.devifid = dst_dev->ifindex;
				 flag = 1;
				 dev_put(dst_dev);
		     	}
		     else
			  {
			    printk("can not get the dev %s\n",buff);
				return -EFAULT;
			  }
		      
	        }
		else
			{
				printk("can not get the dev %s\n",p);
				return -EFAULT;
			}
		}
	//protocol match
	p = strstr(buf,"protocol=");
	if(p)
		{
			if(sscanf(p,"protocol=%s",buff))
				{
				if(!memcmp(buff,"tcp",3))
					tuple.protonum = IPPROTO_TCP;
				else if(!memcmp(buff,"udp",3))
					tuple.protonum = IPPROTO_UDP;
				else if(!memcmp(buff,"icmp",4))
					tuple.protonum = IPPROTO_ICMP;
				else if(!memcmp(buff,"dccp",4))
					tuple.protonum = IPPROTO_DCCP;
				else if(!memcmp(buff,"gre",3))
					tuple.protonum = IPPROTO_GRE;
				else if(!memcmp(buff,"esp",3))
					tuple.protonum = IPPROTO_ESP;
				else if(!memcmp(buff,"sctp",4))
					tuple.protonum = IPPROTO_SCTP;
				else
					{
					  printk("bad protocol %s\n",p);
					  return -EFAULT;
					}
				flag = 1;
				}
			else
				{
					printk("can not get the protocol %s\n",buf);
					return -EFAULT;
				}
		}
	
    //sport match
    var = 0;
	p = strstr(buf,"sport=");
	if(p)
		{
		if(sscanf(p,"sport=%s ",buff))
			{
			    cp = strstr(buff,":");
				if(cp)
			   	{
			   	  memcpy(buf_tmp,buff,cp-buff);
				  if(sscanf(buf_tmp,"%u",&var))
				  	{
					  if(var)
					  	{
					  	 tuple.sport = (u_int16_t)var;
						 flag = 1;
					  	}
					  else
					  	{
					  	 printk("bad sport %s\n",buf_tmp);
						 return -EFAULT;
					  	}
				  	}
				  strcpy(buf_tmp,cp+1);
				  if(sscanf(buf_tmp,"%u",&var))
				  	{
					  if(var)
					  	{
					  	 tuple.sport_end = (u_int16_t)var;
						 if(tuple.sport_end < tuple.sport)
						 	{
						 	 printk("error!! sport:%d > sportend:%d\n",tuple.sport,tuple.sport_end);
							 return -EFAULT;
						 	}
						 flag = 1;
					  	}
					  else
					  	{
					  	 printk("bad sportend %s\n",buf_tmp);
						 return -EFAULT;
					  	}
				  	}
			   	}
				else
					{
					 sscanf(p,"sport=%u",&var);
					 if(var)
					  	{
					  	 tuple.sport = (u_int16_t)var;
						 flag = 1;
					  	}
					  else
					  	{
					  	 printk("bad dport %s\n",p);
						 return -EFAULT;
					  	}
					}

		  }
		else
			{
				printk("can not get sport %s\n",buf);
				return -EFAULT;
			}
	}

#if 0
	//sportend match
	var = 0;
	p = strstr(buf,"sportend=");
	if(p)
		{
		if(sscanf(p,"sportend=%u",&var))
			{
			  if(var)
			  	{
			  	 tuple.sport_end = (u_int16_t)var;
				 if((tuple.sport > tuple.sport_end) || (tuple.sport == 0))
				 	{
				 	  printk("sport > sportend or the sport is 0\n");
					  return -EFAULT;
				 	}
				 flag = 1;
			  	}
			  else
			  	{
			  	 printk("bad sportend %s\n",p);
				 return -EFAULT;
			  	}
			}
		else
			{
				printk("can not ge sportend %s\n",buf);
				return -EFAULT;
			
			}
		}
#endif

	//dport match
	var = 0;
	p = strstr(buf,"dport=");
	if(p)
		{
		if(sscanf(p,"dport=%s ",buff))
			{
			    cp = strstr(buff,":");
				if(cp)
			   	{
			   	  memcpy(buf_tmp,buff,cp-buff);
				  if(sscanf(buf_tmp,"%u",&var))
				  	{
					  if(var)
					  	{
					  	 tuple.dport = (u_int16_t)var;
						 flag = 1;
					  	}
					  else
					  	{
					  	 printk("bad dport %s\n",buf_tmp);
						 return -EFAULT;
					  	}
				  	}
				  strcpy(buf_tmp,cp+1);
				  if(sscanf(buf_tmp,"%u",&var))
				  	{
					  if(var)
					  	{
					  	 tuple.dport_end = (u_int16_t)var;
						 if(tuple.dport_end < tuple.dport)
						 	{
						 	 printk("error!! dport:%d > dportend:%d\n",tuple.dport,tuple.dport_end);
							 return -EFAULT;
						 	}
						 flag = 1;
					  	}
					  else
					  	{
					  	 printk("bad dportend %s\n",buf_tmp);
						 return -EFAULT;
					  	}
				  	}
			   	}
				else
					{
					 sscanf(p,"dport=%u",&var);
					 if(var)
					  	{
					  	 tuple.dport = (u_int16_t)var;
						 flag = 1;
					  	}
					  else
					  	{
					  	 printk("bad dport %s\n",p);
						 return -EFAULT;
					  	}
					}

		  }
		else
			{
				printk("can not get dport %s\n",buf);
				return -EFAULT;
			}
	}

#if 0
    //dportend match
    var = 0;
	p = strstr(buf,"dportend=");
	if(p)
		{
	    if(sscanf(p,"dportend=%u",&var))
	    	{
	    	 if(var)
			  	{
			  	 tuple.dport_end= (u_int16_t)var;
				 if((tuple.dport > tuple.dport_end) || (tuple.dport == 0))
				 	{
				 	  printk("dport > dportend or the dport is 0\n");
					  return -EFAULT;
				 	}
				 flag = 1;
			  	}
			  else
			  	{
			  	 printk("bad dportend %s\n",p);
				 return -EFAULT;
			  	}
	    	}
		else
			{
				printk("can no get dportend %s\n",buf);
				return -EFAULT;
			}
		}
#endif

	//sip match
	p = strstr(buf,"src=");
	if(p)
		{
		if(sscanf(p,"src=%s ",buff))
			{
			cp = strstr(buff,"/");
			   if(cp)
			   	{
			   	  memcpy(buf_tmp,buff,cp-buff);
				  if(inet_aton(buf_tmp,&tuple.sip))
				      {
						 flag = 1;
				       }
				  else
					{
					   printk("bad src %s\n",buf_tmp);
					   return -EFAULT;
					 }
				  strcpy(buff_mask,cp+1);
				  if(inet_aton(buff_mask,&tuple.smaskip))
				     {
						flag = 1;
				     }
				  else
					 {
					   	printk("bad src mask %s\n",buff_mask);
						return -EFAULT;
					 }

			   	}
			   else
			   	{
			       if(inet_aton(buff,&tuple.sip))
			       	{
					   flag = 1;
			       	}
				   else
				   	{
				   	 printk("bad src %s\n",buff);
					 return -EFAULT;
				   	}
			   	}
			}
		else
			{
				printk("can not get  src %s\n",buf);
				return -EFAULT;
			}
		}
	//dip match
	p = strstr(buf,"dst=");
	if(p)
		{
		if(sscanf(p,"dst=%s",buff))
			{
			   cp = strstr(buff,"/");
			   if(cp)
			   	{
			   	  memcpy(buf_tmp,buff,cp-buff);
				  if(inet_aton(buf_tmp,&tuple.dip))
				      {
						 flag = 1;
				       }
				  else
					{
					   printk("bad dst %s\n",buf_tmp);
					   return -EFAULT;
					 }
				  strcpy(buff_mask,cp+1);
				  if(inet_aton(buff_mask,&tuple.dmaskip))
				     {
						flag = 1;
				     }
				  else
					 {
					   	printk("bad src mask %s\n",buff_mask);
						return -EFAULT;
					 }

			   	}
			   else
			   	{
			       if(inet_aton(buff,&tuple.dip))
			       	{
					   flag = 1;
			       	}
				   else
				   	{
				   	 printk("bad dst %s\n",buff);
					 return -EFAULT;
				   	}
			   	}
			}
		else
			{
				printk("can no get dst %s\n",buf);
				return -EFAULT;
			}
		}
	
	#if 0
    printk("dev %d\n",tuple.devifid);
	printk("protocol %d\n",tuple.protonum);
	printk("dip %x/%x\n",tuple.dip,tuple.dmaskip);
	printk("sip %x/%x\n",tuple.sip,tuple.smaskip);
	printk("sport %d:%d\n",tuple.sport,tuple.sport_end);
	printk("dport %d:%d\n",tuple.dport,tuple.dport_end);
	printk("local nat is %d\n",tuple.local_ct);
	printk("nat ct flag is %d\n",tuple.nat_ct_flag);
	printk("ct diy is %d\n",tuple.CT_DIR_ORIGINAL_FLAG);
	printk("ct diy is %d\n",tuple.CT_DIR_OREPLY_FLAG);
	#endif
	
	if(flag)
		{
			nf_ct_iterate_cleanup(&init_net,clear_conntrack_ct_match, &tuple);
		}
	else
		{
			printk("no match rule please try again\n");
			printk("please input < cat > clear_conntrack_tunk >  to help\n");
			printk("example:\n");
			printk("dev=ppp0 src=192.168.1.1 dst=192.168.1.10/255.255.255.255 sport=100:101 dport=200:900 protocol=tcp !nat !localct\n");
		}
	
	return count;
	
}

/********************************************************************
// Function Name: init_clear_conntrack_tunk
// Description  : init proc file
// Parameters   : 
// Returns      :void
********************************************************************/

void init_clear_conntrack_tunk(void)
{
	struct proc_dir_entry *clear_conntrack_tunk;
	clear_conntrack_tunk = create_proc_entry("clear_conntrack_tunk", 0, init_net.proc_net);
	if (clear_conntrack_tunk == NULL)
		{
		  printk("init clear_conntrack_tunk failed !\n");
		  return;
		}
	clear_conntrack_tunk->read_proc = read_clear_conntrack_match;
	clear_conntrack_tunk->write_proc = write_clear_conntrack_match;
}

void nf_ct_free_hashtable(void *hash, int vmalloced, unsigned int size)
{
	if (vmalloced)
		vfree(hash);
	else
		free_pages((unsigned long)hash,
			   get_order(sizeof(struct hlist_head) * size));
}
EXPORT_SYMBOL_GPL(nf_ct_free_hashtable);

void nf_conntrack_flush(struct net *net, u32 pid, int report)
{
	struct __nf_ct_flush_report fr = {
		.pid 	= pid,
		.report = report,
	};
	nf_ct_iterate_cleanup(net, kill_all, &fr);
}
EXPORT_SYMBOL_GPL(nf_conntrack_flush);

static void nf_conntrack_cleanup_init_net(void)
{
	nf_conntrack_helper_fini();
	nf_conntrack_proto_fini();
	kmem_cache_destroy(nf_conntrack_cachep);
}

static void nf_conntrack_cleanup_net(struct net *net)
{
	nf_ct_event_cache_flush(net);
	nf_conntrack_ecache_fini(net);
 i_see_dead_people:
	nf_conntrack_flush(net, 0, 0);
	if (atomic_read(&net->ct.count) != 0) {
		schedule();
		goto i_see_dead_people;
	}
	/* wait until all references to nf_conntrack_untracked are dropped */
	while (atomic_read(&nf_conntrack_untracked.ct_general.use) > 1)
		schedule();

	nf_ct_free_hashtable(net->ct.hash, net->ct.hash_vmalloc,
			     nf_conntrack_htable_size);
	nf_conntrack_acct_fini(net);
	nf_conntrack_expect_fini(net);
	free_percpu(net->ct.stat);
}

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)


/* It's possible that no conntrack deleted due to large threshold value. 
     Perhaps we can try to dynamically reduce the threshold if nothing can be removed.
*/
static inline int __drop_one_udp(void){
	int i = 0;
	struct list_head *ptr;
	spin_lock_bh(&nf_conntrack_lock);       
	for(i =0; i<sizeof(drop_priority)/sizeof(struct DROP_PRORITY); i++)
	{
		
		if(drop_priority[i].state <= TCP_CONNTRACK_MAX )	 //ramen--drop tcp conntrack				
			continue;
	 		
	 	ptr = Udp_State_Hash_Head[drop_priority[i].state-UDP_UNREPLY].state_hash; 		
 		if (!list_empty(ptr))
		{

			struct nf_conn* tmp=list_entry(ptr->next,struct nf_conn, state_tuple);

			del_timer(&tmp->timeout);
			death_by_timeout((unsigned long)tmp);
			spin_unlock_bh(&nf_conntrack_lock);
			return 1;
		}
	  	
	}
	spin_unlock_bh(&nf_conntrack_lock);
       return 0;
}


static inline int __drop_one_tcp(void){
	int i = 0;
	struct list_head *ptr;
	spin_lock_bh(&nf_conntrack_lock);       
	for(i =0; i<sizeof(drop_priority)/sizeof(struct DROP_PRORITY); i++)
	{
		
		if(drop_priority[i].state >= TCP_CONNTRACK_MAX )	 //ramen--drop tcp conntrack				
			continue;
	 		
	 	ptr = Tcp_State_Hash_Head[drop_priority[i].state].state_hash;	
 		if (!list_empty(ptr))
		{
			struct nf_conn* tmp=list_entry(ptr->next,struct nf_conn, state_tuple);

			del_timer(&tmp->timeout);
			death_by_timeout((unsigned long)tmp);
			spin_unlock_bh(&nf_conntrack_lock);
			return 1;
 		}

	}
	spin_unlock_bh(&nf_conntrack_lock);
       return 0;
}

static inline int __drop_one_conntrack(int factor)
{
       //DEBUGP("enter the drop_one_tcp_conntrack.............\n");
	int i = 0;
	struct list_head *ptr;
	struct list_head* it;
	struct nf_conn* tmp;
	spin_lock_bh(&nf_conntrack_lock);       
	for(i =0; i<sizeof(drop_priority)/sizeof(struct DROP_PRORITY); i++)
	{
		if(drop_priority[i].state < TCP_CONNTRACK_MAX )	 //ramen--drop tcp conntrack	
		{
			ptr = Tcp_State_Hash_Head[drop_priority[i].state].state_hash;
			if(!list_empty(ptr))	
			{				
				list_for_each(it, ptr) {
					tmp = list_entry(it, struct nf_conn, state_tuple);
					if (((((tmp->timeout.expires - jiffies)/HZ) >> factor)<=drop_priority[i].threshold )
						&&del_timer(&tmp->timeout)) 
					{
						#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW_DEBUG)
						printk("%s %s(%d) Drop TCP %p\n", __FILE__,__FUNCTION__,__LINE__,tmp);
						#endif
						death_by_timeout((unsigned long)tmp);								
				       	spin_unlock_bh(&nf_conntrack_lock);
						return 1;
					} else {
						// give up this list because it's sorted and we've seen the smallest that doesn't fit our criteria.
						break;
					}
				}
			}
			else
				continue;
		}
		else //ramen--drop udp conntrack
 		{
 			ptr = Udp_State_Hash_Head[drop_priority[i].state-UDP_UNREPLY].state_hash;	 	 
	 		if(!list_empty(ptr))
			{
				list_for_each(it, ptr) 
				{
					tmp = list_entry(it, struct nf_conn, state_tuple);
					if (((((tmp->timeout.expires - jiffies)/HZ) >> factor)<=drop_priority[i].threshold )
						&&del_timer(&tmp->timeout)) 
					{
						#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW_DEBUG)
						printk("%s %s(%d) Drop UDP %p\n", __FILE__,__FUNCTION__,__LINE__,tmp);
						#endif
						death_by_timeout((unsigned long)tmp);					
			       		spin_unlock_bh(&nf_conntrack_lock);
						return 1;
					} else {
						// give up this list because it's sorted and we've seen the smallest that doesn't fit our criteria.
						break;
					}				
				}	
 			}
			else
				continue;
		}
	} 		
	spin_unlock_bh(&nf_conntrack_lock);
       return 0;
}


int drop_one_conntrack(void) {
	if ((atomic_read(&_prot_limit[PROT_TCP]) < atomic_read(&prot_counters[PROT_TCP])) && __drop_one_tcp()) {
		return 1;
	}

	if ((atomic_read(&_prot_limit[PROT_UDP]) < atomic_read(&prot_counters[PROT_UDP])) && __drop_one_udp()) {
		return 1;
	}
	if ( __drop_one_conntrack(0) || 
	     __drop_one_conntrack(1) ||
#ifdef CONFIG_RTL8676
	     __drop_one_conntrack(3)	// andrew, seems x>>32 == x>>0
#else
	     __drop_one_conntrack(31)
#endif //CONFIG_RTL8676
	)
		return 1;
	else
		return 0;		
}

//将连接跟踪总数分成100份
#define PIECES_OF_CONNTRACK    100
//连接跟踪的最大删除个数为20个
#define MAX_DROP_CONNTRACKS 20
//需要检查的时间间隔
#define TCP_NOCHECK_ESTABLISHED_TIME    ((5*60+1)*HZ)
#define TCP_NOCHECK_OTHERSTATE_TIME ((5+1)*HZ)
#define UDP_NOCHECK_UNREPLY_TIME ((5+1)*HZ)

unsigned long lastTime = 0;
bool lastState = true;
extern unsigned int nf_ct_udp_timeout;
extern unsigned int nf_ct_udp_timeout_stream;
//删除成功返回1，失败返回0
static inline int drop_conntracks(struct net *net)
{
    int i = 0;
    struct list_head *ptr;
    struct list_head *it, *ptr1;
    struct nf_conn* tmp;
    
    int currcount = atomic_read(&net->ct.count) ;    
    int needDrops = 1;
    int needDropsCpy = 1;

    //计算本次最多可删除连接跟踪的个数
    if(currcount > PIECES_OF_CONNTRACK)
        if((needDrops = currcount/PIECES_OF_CONNTRACK) > MAX_DROP_CONNTRACKS)
            needDrops = MAX_DROP_CONNTRACKS;
    needDropsCpy = needDrops;
    
    spin_lock_bh(&nf_conntrack_lock);    
    //如果删除连接失败，则暂停2秒处理
    if(!lastState)
    {
        long difftime = jiffies-lastTime;
        if(difftime < 0 || difftime > 2*HZ)
        {
            //如果差值小于0，意味着jiffies从max->min了，直接退出
            //如果差值大于2s，意味着暂停处理的时间超过两秒，继续处理
            lastState = true;
            lastTime = jiffies;
            if(difftime < 0)
                {spin_unlock_bh(&nf_conntrack_lock); return 0;}
        }
        else
            {spin_unlock_bh(&nf_conntrack_lock); return 0;}
    }
    
    //drop_priority数组的的元素序号从小到大依次查找合适的连接跟踪删除
    for(i =0; i<sizeof(drop_priority)/sizeof(struct DROP_PRORITY); i++)
    {
        //连接状态满足下列条件之一，跳过该处理:
        //(1)tcp ESTABLISHED状态超时时间小于等于5分钟
        //(2)tcp SYN_SENT状态超时时间小于等于5秒钟
        //(3)tcp SYN_RECV状态超时时间小于等于5秒钟
        //(4)udp ASSURED状态并且当前没有成功删除任何连接跟踪
        if(drop_priority[i].state == TCP_CONNTRACK_ESTABLISHED)
            {if(tcp_timeouts[TCP_CONNTRACK_ESTABLISHED] < TCP_NOCHECK_ESTABLISHED_TIME) continue;}
        if(drop_priority[i].state == TCP_CONNTRACK_SYN_SENT)
            {if(tcp_timeouts[TCP_CONNTRACK_SYN_SENT] < TCP_NOCHECK_OTHERSTATE_TIME) continue;}
        if(drop_priority[i].state == TCP_CONNTRACK_SYN_RECV)
            {if(tcp_timeouts[TCP_CONNTRACK_SYN_RECV] < TCP_NOCHECK_OTHERSTATE_TIME) continue;}
        if(drop_priority[i].state == UDP_ASSURED)
            {if(needDropsCpy != needDrops) continue;}

        if(drop_priority[i].state < TCP_CONNTRACK_MAX )
        {
            //处理tcp相关状态的连接跟踪
            ptr = Tcp_State_Hash_Head[drop_priority[i].state].state_hash;
            if(!ptr || list_empty(ptr)) {continue;}

            //因为本轮操作目的是删除处理，需要使用安全遍历方式
            list_for_each_safe(it, ptr1, ptr) 
            {
                tmp = list_entry(it, struct nf_conn, state_tuple);
                if(!tmp || nf_ct_is_dying(tmp)) { continue;}

                //tcp连接跟踪满足下列条件之一，跳过该处理:
                //(1)tcp ESTABLISHED距离下一次的交互时间在301秒钟内
                //(2)tcp SYN_SENT距离下一次的交互时间在6秒钟内
                //(3)tcp SYN_RECV距离下一次的交互时间在6秒钟内
                //(4)当前连接跟踪为信令建立
                if(drop_priority[i].state == TCP_CONNTRACK_ESTABLISHED)
                    if(tcp_timeouts[TCP_CONNTRACK_ESTABLISHED]-(int)(tmp->timeout.expires-jiffies) < TCP_NOCHECK_ESTABLISHED_TIME)
                        {continue;}
                if(drop_priority[i].state == TCP_CONNTRACK_SYN_SENT)
                    if(tcp_timeouts[TCP_CONNTRACK_SYN_SENT]-(int)(tmp->timeout.expires-jiffies) < TCP_NOCHECK_OTHERSTATE_TIME)
                        {continue;}
                if(drop_priority[i].state == TCP_CONNTRACK_SYN_RECV)
                    if(tcp_timeouts[TCP_CONNTRACK_SYN_RECV]-(int)(tmp->timeout.expires-jiffies) < TCP_NOCHECK_OTHERSTATE_TIME)
                        {continue;}
                if(test_bit(IPS_SIGNALFLOW_BIT, &tmp->status)){continue;}

                //如果删除成功，检查当前的删除个数是否满足条件:
                //      满足条件直接退出，否则继续查找下一个连接跟踪处理
                //如果删除失败，检查下一个状态链表
                if (del_timer(&tmp->timeout)) 
                {
                    death_by_timeout((unsigned long)tmp);								
                    if(--needDrops) 
                        {continue;}
                    else
                        {spin_unlock_bh(&nf_conntrack_lock); /*printk("drop total is %d\n", (needDropsCpy-needDrops));*/ return 1;}
                } 
                break;
            }

        }
        else
        {
            //处理udp相关状态的连接跟踪
            ptr = Udp_State_Hash_Head[drop_priority[i].state-UDP_UNREPLY].state_hash;
            if(!ptr || list_empty(ptr)) {continue;}

            list_for_each_safe(it, ptr1, ptr) 
            {
                tmp = list_entry(it, struct nf_conn, state_tuple);
                if(!tmp || nf_ct_is_dying(tmp)) {continue;}

                //udp连接跟踪满足下列条件之一，跳过该处理:
                //(1)当前连接跟踪为alg数据流建立
                //(2)当前连接跟踪为组播建立
                //(3)当前连接跟踪为信令建立
                //(4)udp UNREPLY距离下一次的交互时间在6秒钟内
                if(test_bit(IPS_EXPECTED_BIT, &tmp->status) && !test_bit(IPS_FULLCONENAT_BIT, &tmp->status))
                    {continue;}
                if(test_bit(IPS_MULTICAST_BIT, &tmp->status))
                    {continue;}
                if(test_bit(IPS_SIGNALFLOW_BIT, &tmp->status))
                    {continue;}
                if(drop_priority[i].state == UDP_UNREPLY)
                    if((int)nf_ct_udp_timeout-(int)(tmp->timeout.expires-jiffies) < UDP_NOCHECK_UNREPLY_TIME)
                        {continue;}

                //如果删除成功，检查当前的删除个数是否满足条件:
                //      满足条件直接退出，否则继续查找下一个连接跟踪处理
                //如果删除失败，检查下一个状态链表
                if (del_timer(&tmp->timeout)) 
                {
                    death_by_timeout((unsigned long)tmp);					
                    if(--needDrops) 
                        {continue;}
                    else
                        {spin_unlock_bh(&nf_conntrack_lock); /*printk("drop total is %d\n", (needDropsCpy-needDrops));*/ return 1;}
                } 
                break;
            }	
        }
    } 	

    if(needDropsCpy != needDrops)
    {
        spin_unlock_bh(&nf_conntrack_lock);
        /*printk("drop total is %d\n", (needDropsCpy-needDrops));*/
        return 1;
    }

    //代码到这里几乎是不可能，除非受到攻击
    //我们必须找到一个连接跟踪
    //上面的处理只有established_list可能没有被包含在内
    //所以重新对established_list状态链表操作
    //但是我们将要操作的是活跃的tcp established连接跟踪，所以尽量少删除
    ptr = &established_list;
    if(ptr && !list_empty(ptr))
    {
        list_for_each_safe(it, ptr1, ptr) 
        {
            tmp = list_entry(it, struct nf_conn, state_tuple);
            if(!tmp || nf_ct_is_dying(tmp)) {continue;}

            //这里只排除信令连接跟踪
            if(test_bit(IPS_SIGNALFLOW_BIT, &tmp->status)) {continue;}

            //如果删除失败，还是需要继续查找下一个连接跟踪
            //如果删除成功，退出
            if (del_timer(&tmp->timeout)) 
            {
                death_by_timeout((unsigned long)tmp);								
                --needDrops; 
                spin_unlock_bh(&nf_conntrack_lock);
                /*printk("have find one tcp conntrack\n"); */
                return 1;
            } 
            continue;
        }
    }

    //最后还是有可能删除失败
    //导致失败的原因从某中意义上说是受到针对连接跟踪的恶意攻击
    //为了避免这种情况导致的函数不必要的处理我们引入超时机制
    lastTime = jiffies;
    lastState = false;
    spin_unlock_bh(&nf_conntrack_lock);
    /*printk("fail, delay 2sec\n");*/
    return 0;
}



#endif //CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW


/* Mishearing the voices in his head, our hero wonders how he's
   supposed to kill the mall. */
void nf_conntrack_cleanup(struct net *net)
{
	if (net_eq(net, &init_net))
		rcu_assign_pointer(ip_ct_attach, NULL);

	/* This makes sure all current packets have passed through
	   netfilter framework.  Roll on, two-stage module
	   delete... */
	synchronize_net();

	nf_conntrack_cleanup_net(net);

	if (net_eq(net, &init_net)) {
		rcu_assign_pointer(nf_ct_destroy, NULL);
		nf_conntrack_cleanup_init_net();
	}
}

void *nf_ct_alloc_hashtable(unsigned int *sizep, int *vmalloced, int nulls)
{
	struct hlist_nulls_head *hash;
	unsigned int nr_slots, i;
	size_t sz;

	*vmalloced = 0;

	BUILD_BUG_ON(sizeof(struct hlist_nulls_head) != sizeof(struct hlist_head));
	nr_slots = *sizep = roundup(*sizep, PAGE_SIZE / sizeof(struct hlist_nulls_head));
	sz = nr_slots * sizeof(struct hlist_nulls_head);
	hash = (void *)__get_free_pages(GFP_KERNEL | __GFP_NOWARN | __GFP_ZERO,
					get_order(sz));
	if (!hash) {
		*vmalloced = 1;
		printk(KERN_WARNING "nf_conntrack: falling back to vmalloc.\n");
		hash = __vmalloc(sz, GFP_KERNEL | __GFP_ZERO, PAGE_KERNEL);
	}

	if (hash && nulls)
		for (i = 0; i < nr_slots; i++)
			INIT_HLIST_NULLS_HEAD(&hash[i], i);

	return hash;
}
EXPORT_SYMBOL_GPL(nf_ct_alloc_hashtable);

int nf_conntrack_set_hashsize(const char *val, struct kernel_param *kp)
{
	int i, bucket, vmalloced, old_vmalloced;
	unsigned int hashsize, old_size;
	int rnd;
	struct hlist_nulls_head *hash, *old_hash;
	struct nf_conntrack_tuple_hash *h;

	/* On boot, we can set this without any fancy locking. */
	if (!nf_conntrack_htable_size)
		return param_set_uint(val, kp);

	hashsize = simple_strtoul(val, NULL, 0);
	if (!hashsize)
		return -EINVAL;

	hash = nf_ct_alloc_hashtable(&hashsize, &vmalloced, 1);
	if (!hash)
		return -ENOMEM;

	/* We have to rehahs for the new table anyway, so we also can
	 * use a newrandom seed */
	get_random_bytes(&rnd, sizeof(rnd));

	/* Lookups in the old hash might happen in parallel, which means we
	 * might get false negatives during connection lookup. New connections
	 * created because of a false negative won't make it into the hash
	 * though since that required taking the lock.
	 */
	spin_lock_bh(&nf_conntrack_lock);
	for (i = 0; i < nf_conntrack_htable_size; i++) {
		while (!hlist_nulls_empty(&init_net.ct.hash[i])) {
			h = hlist_nulls_entry(init_net.ct.hash[i].first,
					struct nf_conntrack_tuple_hash, hnnode);
			hlist_nulls_del_rcu(&h->hnnode);
			bucket = __hash_conntrack(&h->tuple, hashsize, rnd);
			hlist_nulls_add_head_rcu(&h->hnnode, &hash[bucket]);
		}
	}
	old_size = nf_conntrack_htable_size;
	old_vmalloced = init_net.ct.hash_vmalloc;
	old_hash = init_net.ct.hash;

	nf_conntrack_htable_size = hashsize;
	init_net.ct.hash_vmalloc = vmalloced;
	init_net.ct.hash = hash;
	nf_conntrack_hash_rnd = rnd;
	spin_unlock_bh(&nf_conntrack_lock);

	nf_ct_free_hashtable(old_hash, old_vmalloced, old_size);
	return 0;
}
EXPORT_SYMBOL_GPL(nf_conntrack_set_hashsize);

module_param_call(hashsize, nf_conntrack_set_hashsize, param_get_uint,
		  &nf_conntrack_htable_size, 0600);

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
static void tcp_patten_ck_fn(unsigned long arg)
{	
	smart_count = 0;
      	mod_timer(&tcp_patten_ck, jiffies + 100);
}
#endif


static int nf_conntrack_init_init_net(void)
{
	int max_factor = 8;
	int ret;

	/* Idea from tcp.c: use 1/16384 of memory.  On i386: 32MB
	 * machine has 512 buckets. >= 1GB machines have 16384 buckets. */
	if (!nf_conntrack_htable_size) {
		nf_conntrack_htable_size
			= (((num_physpages << PAGE_SHIFT) / 16384)
			   / sizeof(struct hlist_head));
		if (num_physpages > (1024 * 1024 * 1024 / PAGE_SIZE))
			nf_conntrack_htable_size = 16384;
		if (nf_conntrack_htable_size < 32)
			nf_conntrack_htable_size = 32;

		/* Use a max. factor of four by default to get the same max as
		 * with the old struct list_heads. When a table size is given
		 * we use the old value of 8 to avoid reducing the max.
		 * entries. */
		max_factor = 4;
	}
	nf_conntrack_max = max_factor * nf_conntrack_htable_size;

	printk("nf_conntrack version %s (%u buckets, %d max)\n",
	       NF_CONNTRACK_VERSION, nf_conntrack_htable_size,
	       nf_conntrack_max);

	nf_conntrack_cachep = kmem_cache_create("nf_conntrack",
						sizeof(struct nf_conn),
						0, SLAB_DESTROY_BY_RCU, NULL);
	if (!nf_conntrack_cachep) {
		printk(KERN_ERR "Unable to create nf_conn slab cache\n");
		ret = -ENOMEM;
		goto err_cache;
	}
	init_clear_conntrack_tunk();

	ret = nf_conntrack_proto_init();
	if (ret < 0)
		goto err_proto;

	ret = nf_conntrack_helper_init();
	if (ret < 0)
		goto err_helper;

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	init_tcp_state_hash();
	/*linux-2.6.19*/
	//proc_net_create ("conntrack_proto", 0, getinfo_conntrack_proto);
#ifdef CONFIG_PROC_FS
	proc_create("conntrack_proto", 0, init_net.proc_net,
			   &getinfo_conntrack_proto_fops);
#endif
	#ifdef CONFIG_RTL8676
	prot_limit[PROT_ICMP] = 0;
	prot_limit[PROT_TCP] = 95;
	prot_limit[PROT_UDP] = 5;	
	#else //CONFIG_RTL8676
	prot_limit[PROT_ICMP] = 1;
	prot_limit[PROT_TCP] = 94;
	prot_limit[PROT_UDP] = 5;
	#endif //CONFIG_RTL8676
	recalculate();
#endif


#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	init_timer(&tcp_patten_ck);
      tcp_patten_ck.expires  = jiffies + 100;
      tcp_patten_ck.data     = 0L;
      tcp_patten_ck.function = tcp_patten_ck_fn;
      mod_timer(&tcp_patten_ck, jiffies + 100);
#endif

	return 0;

err_helper:
	nf_conntrack_proto_fini();
err_proto:
	kmem_cache_destroy(nf_conntrack_cachep);
err_cache:
	return ret;
}

static int nf_conntrack_init_net(struct net *net)
{
	int ret;

	atomic_set(&net->ct.count, 0);
	INIT_HLIST_NULLS_HEAD(&net->ct.unconfirmed, 0);
	net->ct.stat = alloc_percpu(struct ip_conntrack_stat);
	if (!net->ct.stat) {
		ret = -ENOMEM;
		goto err_stat;
	}
	ret = nf_conntrack_ecache_init(net);
	if (ret < 0)
		goto err_ecache;
	net->ct.hash = nf_ct_alloc_hashtable(&nf_conntrack_htable_size,
					     &net->ct.hash_vmalloc, 1);
	if (!net->ct.hash) {
		ret = -ENOMEM;
		printk(KERN_ERR "Unable to create nf_conntrack_hash\n");
		goto err_hash;
	}
	ret = nf_conntrack_expect_init(net);
	if (ret < 0)
		goto err_expect;
	ret = nf_conntrack_acct_init(net);
	if (ret < 0)
		goto err_acct;

	/* Set up fake conntrack:
	    - to never be deleted, not in any hashes */
#ifdef CONFIG_NET_NS
	nf_conntrack_untracked.ct_net = &init_net;
#endif
	atomic_set(&nf_conntrack_untracked.ct_general.use, 1);
	/*  - and look it like as a confirmed connection */
	set_bit(IPS_CONFIRMED_BIT, &nf_conntrack_untracked.status);

	return 0;

err_acct:
	nf_conntrack_expect_fini(net);
err_expect:
	nf_ct_free_hashtable(net->ct.hash, net->ct.hash_vmalloc,
			     nf_conntrack_htable_size);
err_hash:
	nf_conntrack_ecache_fini(net);
err_ecache:
	free_percpu(net->ct.stat);
err_stat:
	return ret;
}


/*
 * TBS_TAG: by kuangsuwen
 * Desc:Add alg switch
 */
#ifdef NF_ALG_CTL

#define ALG_CTL_DEF(type, val)  [alg_type_##type] = {#type, val}

struct alg_entry
{
    char *name;
    int enable;
};

/* alg 控制开关数组 */
struct alg_entry alg_list[alg_type_end] =
{
    ALG_CTL_DEF(ftp,  1),
    ALG_CTL_DEF(tftp, 1),
    ALG_CTL_DEF(rtsp, 1),
    ALG_CTL_DEF(pptp, 1),
    ALG_CTL_DEF(l2tp, 1),
    ALG_CTL_DEF(ipsec,1),
    ALG_CTL_DEF(sip,  1),
    ALG_CTL_DEF(h323, 1),
};

int alg_enable(int type)
{
    return alg_list[type].enable;
}
EXPORT_SYMBOL_GPL(alg_enable);

/*
alg开关控制
*/
static struct proc_dir_entry *proc_alg = NULL;

static int proc_alg_debug_read(char *page, char **start, off_t off,
			  int count, int *eof, void *data)
{
    char *out = page;
	int len = 0;
    int i = 0;

    out += sprintf(out, "\n===================================================\n");
    for (i = 0; i < alg_type_end; i++)
    {
        out += sprintf(out, "| %s=%d\n", alg_list[i].name, alg_list[i].enable);
    }
    out += sprintf(out, "---------------------------------------------------\n");

	len = out - page;
	len -= off;
	if (len < count) {
		*eof = 1;
		if (len <= 0)
            return 0;
	} else
		len = count;

	*start = page + off;
	return len;
}

static int proc_alg_debug_write( struct file *filp, const char __user *buf,unsigned long len, void *data )
{
	int ret;
	char str_buf[256];
	char action[20] = {0};
	int val = 0;
    int i = 0;

	if(len > 255)
	{
		printk("Usage: echo ftp 1 > /proc/alg \n");
		return len;
	}

	copy_from_user(str_buf,buf,len);
	str_buf[len] = '\0';

	ret = sscanf(str_buf, "%s %d", action, (int*)&val);
	if(ret != 2 || val < 0 )
	{
		printk("Error. Sample: echo ftp 1 > /proc/alg \n");
		return len;
	}

    for (i = 0; i < alg_type_end; i++)
    {
        if (0 == strcmp(action, alg_list[i].name))
        {
            alg_list[i].enable = val;
            return len;
        }
    }

    printk("Error: Unkown command.\n");

	return len;
}
#endif

int nf_conntrack_init(struct net *net)
{
	int ret;

	if (net_eq(net, &init_net)) {
		ret = nf_conntrack_init_init_net();
		if (ret < 0)
			goto out_init_net;
	}
	ret = nf_conntrack_init_net(net);
	if (ret < 0)
		goto out_net;

	if (net_eq(net, &init_net)) {
		/* For use by REJECT target */
		rcu_assign_pointer(ip_ct_attach, nf_conntrack_attach);
		rcu_assign_pointer(nf_ct_destroy, destroy_conntrack);
	}

#ifdef NF_ALG_CTL
    proc_alg = create_proc_entry("alg", 0, NULL);
	if (proc_alg) {
		proc_alg->read_proc = proc_alg_debug_read;
		proc_alg->write_proc = proc_alg_debug_write;
	}
#endif

	return 0;

out_net:
	if (net_eq(net, &init_net))
		nf_conntrack_cleanup_init_net();
out_init_net:
	return ret;
}
