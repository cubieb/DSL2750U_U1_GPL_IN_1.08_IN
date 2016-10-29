#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/smp_lock.h>
#include <linux/inetdevice.h>

#include "../../bridge/br_private.h"
#include "fastpath_core.h"
#include <linux/if_pppox.h>
#include <linux/autoconf.h>

#include <linux/notifier.h>
#include <linux/netfilter.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_ecache.h>

#ifdef CONFIG_ETHWAN
#else
#define __SRAM
#endif
extern __u8 fastpath_forward_flag;
extern u32 LANsub[8];
extern u32 LANmask[8];
extern u32 routeIndex;

#if defined(CONFIG_IMQ) || defined(CONFIG_IMQ_MODULE)||defined(CONFIG_ATP_SUPPORT_ETHUP)

extern int qos_enable;
#endif

unsigned int fpStartStamp, fpPktCount;
unsigned int heavyTraffic;
#define FASTPATH_POLL_THRESH	5

extern void neigh_hh_init(struct neighbour *n, struct dst_entry *dst, __be16 protocol);

__SRAM int fp_iproute_input(void *pSt  /*struct skbuff * */, struct iphdr *iph, __u32 *fp_dip)
{
	struct sk_buff *skb = (struct sk_buff *)pSt;

	//printk("%s %d enter.\n", __func__, __LINE__);
	if(ip_route_input(skb, *fp_dip, iph->saddr, iph->tos, skb->dev)) {
		//printk("%s %d fail.\n", __func__, __LINE__);
		return 0;
	}
	return 1;
}

__SRAM void setSkbDst(void *pSt  /*struct skbuff * */, void *dst /*struct dst_entry * */)
{
	struct sk_buff *skb = (struct sk_buff *)pSt;
	
	skb->dst = (struct dst_entry *)dst;
}

__SRAM void SetFPDst(void *pSt /*struct skbuff * */, void **dst /*struct dst_entry ** */)
{
	struct sk_buff *skb = (struct sk_buff *)pSt;
	
	*dst = skb->dst;
}

__SRAM int isDestLo(void *pSt /*struct skbuff * */)
{
	struct sk_buff *skb = (struct sk_buff *)pSt;
	
	if (!strcmp(skb->dst->dev->name, "lo")) return 1;

	return 0;
}

__SRAM void FastPathHoldDst(void *pSt  /*struct skbuff * */)
{
	struct sk_buff *skb = (struct sk_buff *)pSt;
	
	skb->dst->lastuse = jiffies;
	dst_hold(skb->dst); //cathy, fix dst cache full problem, dst should be held when it is referenced						
	skb->dst->__use++;
}

__SRAM void initSkbHdr(void *pSt  /*struct skbuff * */)
{
	struct sk_buff *skb = (struct sk_buff *)pSt;

	skb->ip_summed = 0x0;
	skb->dev = skb->dst->dev;
}

#if 0
__SRAM void setQoSMark(void *pSt /*struct skbuff * */, unsigned int mark)
{
	#if 0
	struct sk_buff *skb = (struct sk_buff *)pSt;

	skb->iif = 0;
	skb->imq_flags = IMQ_F_ENQUEUE;
	skb->imq_flags |= RTL_IMQ_FP;
	skb->mark = mark;
	//printk("FASTPATH: set stream mark to 0x%x.\n", mark);
	#endif
	#ifndef CONFIG_ETHWAN
	printk("FASTPATH: %s not support in this kernel version\n",__FUNCTION__);
	#endif
}
__SRAM void setQosDscp(struct iphdr *iph, unsigned int mdscp, unsigned int mark)
{
	#if 0
	int ftos_set;
	
	if (mdscp & 0x1000000) {//dscp/tc mark
		ftos_set = mdscp>>16;
		if (ftos_set & FTOS_SETFTOS) {
			if (iph->protocol != IPPROTO_UDP) {
				FASTPATH_ADJUST_CHKSUM_TOS(((iph->tos&(~0xFC))|(mdscp&0xFC)), iph->tos, iph->check);
			}
			else {
				FASTPATH_ADJUST_CHKSUM_TOS_UDP(((iph->tos&(~0xFC))|(mdscp&0xFC)), iph->tos, iph->check);
			}
			iph->tos = ((iph->tos&(~0xFC))|(mdscp&0xFC));
		}
		else if (ftos_set & FTOS_WMMFTOS) {
			if (iph->protocol != IPPROTO_UDP) {
				FASTPATH_ADJUST_CHKSUM_TOS((((mark>>PRIO_LOC_NFMARK) & PRIO_LOC_NFMASK) << DSCP_MASK_SHIFT),
					iph->tos, iph->check);
			}
			else {
				FASTPATH_ADJUST_CHKSUM_TOS_UDP((((mark>>PRIO_LOC_NFMARK) & PRIO_LOC_NFMASK) << DSCP_MASK_SHIFT),
					iph->tos, iph->check);
			}
			iph->tos = ((mark>>PRIO_LOC_NFMARK) & PRIO_LOC_NFMASK) << DSCP_MASK_SHIFT;
		}
	}
	#endif 
	#ifndef CONFIG_ETHWAN
	printk("FASTPATH: %s not support in this kernel version\n",__FUNCTION__);
	#endif
}
#endif

__SRAM int isNotFromPPPItf(void *pSt /*struct skbuff * */)
{
	struct sk_buff *skb = (struct sk_buff *)pSt;

	if(strncmp(skb->dst->dev->name, "ppp",3))
		return 1;
	else
		return 0;
}

/*
 * state: 0-unreplied    1-established
 */
#ifdef CONFIG_RTL867X_KERNEL_MIPS16_NET
__NOMIPS16
#endif
__SRAM enum LR_RESULT rtl867x_addNaptConnection(struct nf_conn *ct,struct nf_conntrack_tuple ori_tuple,
		struct nf_conntrack_tuple reply_tuple, enum NP_FLAGS flags, int state)

{
	struct nf_conntrack_tuple tpdir1, tpdir2;
	struct FP_NAPT_entry napt;
	int fwd_flag=0;
	int i;

	//exclude 127.0.0.1
	if (((ori_tuple.src.u3.ip&0xFFFFFFFF)==0x7F000001) || ((ori_tuple.dst.u3.ip&0xFFFFFFFF)==0x7F000001) ||
		((reply_tuple.src.u3.ip&0xFFFFFFFF)==0x7F000001) || ((reply_tuple.dst.u3.ip&0xFFFFFFFF)==0x7F000001))
		return 0;
		
	// for GCC 4.x.0 warning
	tpdir1.src.u3.ip = 0;
	tpdir2.src.u3.ip = 0;
	tpdir2.dst.u3.ip = 0;
	
	napt.protocol = ori_tuple.dst.protonum;
	napt.flags = flags;

	//printk("%s %d routeIndex=%d\n", __func__, __LINE__, routeIndex);
	//printk("ori_tupe: src=%x dst=%x reply_tupe: src=%x dst=%x\n", ori_tuple.src.u3.ip, ori_tuple.dst.u3.ip,
	//   reply_tuple.src.u3.ip, reply_tuple.dst.u3.ip);
	
	//cathy, for multi-subnet
	for(i=0; i< routeIndex; i++) {
		if ( (((ori_tuple.src.u3.ip & LANmask[i]) == LANsub[i]) && 
			  ((ori_tuple.dst.u3.ip & LANmask[i]) == LANsub[i])) ||
			 ((ori_tuple.dst.u3.ip & 0xF0000000) == 0xE0000000) ||
			 ((reply_tuple.dst.u3.ip & 0xF0000000) == 0xE0000000) ) {
			fwd_flag = 0;
			break;
		}
		if ( (ori_tuple.src.u3.ip & LANmask[i]) == LANsub[i] ) {
			tpdir1 = ori_tuple;
			tpdir2 = reply_tuple;
			fwd_flag = 1;
			break;
		}
		else if ( (reply_tuple.src.u3.ip & LANmask[i]) == LANsub[i] ){
			tpdir1 = reply_tuple;
			tpdir2 = ori_tuple;
			fwd_flag = 1;
			break;			
		}
	}	

	if (!fwd_flag) {
	  //printk("it is not forward packet, don't create fastpath entry.\n");
	  return 0;
	} 

	napt.intIp = tpdir1.src.u3.ip;
	napt.intPort = ntohs(tpdir1.src.u.all);
	napt.extIp = tpdir2.dst.u3.ip;
	napt.extPort = ntohs(tpdir2.dst.u.all);
	napt.remIp = tpdir2.src.u3.ip;
	napt.remPort = ntohs(tpdir2.src.u.all);
    napt.ct = ct ;
	
	return (fastpath_addNaptConnection(&napt, state));
}

#ifdef CONFIG_RTL867X_KERNEL_MIPS16_NET
__NOMIPS16
#endif
__SRAM enum LR_RESULT rtl867x_updateNaptConnection(struct nf_conntrack_tuple ori_tuple,
		struct nf_conntrack_tuple reply_tuple, unsigned int mark, unsigned int mdscp)
{
	struct nf_conntrack_tuple tpdir1, tpdir2;
	struct FP_NAPT_entry napt;
	int i;

	//exclude 127.0.0.1
	if (((ori_tuple.src.u3.ip&0xFFFFFFFF)==0x7F000001) || ((ori_tuple.dst.u3.ip&0xFFFFFFFF)==0x7F000001) ||
		((reply_tuple.src.u3.ip&0xFFFFFFFF)==0x7F000001) || ((reply_tuple.dst.u3.ip&0xFFFFFFFF)==0x7F000001))
		return 0;
	
	napt.protocol = ori_tuple.dst.protonum;

	// for GCC 4.x.0 warning
	tpdir1.src.u3.ip = 0;
	tpdir2.src.u3.ip = 0;
	tpdir2.dst.u3.ip = 0;
	
	//cathy, for multi-subnet
	for(i=0; i< routeIndex; i++) {
		if ( (ori_tuple.src.u3.ip & LANmask[i]) == LANsub[i] ) {
			tpdir1 = ori_tuple;
			tpdir2 = reply_tuple;
			break;
		}
		else {
			tpdir1 = reply_tuple;
			tpdir2 = ori_tuple;
		}
	}	
	

	napt.intIp = tpdir1.src.u3.ip;
	napt.intPort = ntohs(tpdir1.src.u.all);
	napt.extIp = tpdir2.dst.u3.ip;
	napt.extPort = ntohs(tpdir2.dst.u.all);
	napt.remIp = tpdir2.src.u3.ip;
	napt.remPort = ntohs(tpdir2.src.u.all);

	return (fastpath_updateNaptConnection(&napt, mark, mdscp));
}

#ifdef CONFIG_RTL867X_KERNEL_MIPS16_NET
__NOMIPS16
#endif
enum LR_RESULT rtl867x_delNaptConnection (struct nf_conntrack_tuple ori_tuple,
		struct nf_conntrack_tuple reply_tuple)
{
	struct nf_conntrack_tuple tpdir1, tpdir2;
	struct FP_NAPT_entry napt;
	int i;
	
	//exclude 127.0.0.1
	if (((ori_tuple.src.u3.ip&0xFFFFFFFF)==0x7F000001) || ((ori_tuple.dst.u3.ip&0xFFFFFFFF)==0x7F000001) ||
		((reply_tuple.src.u3.ip&0xFFFFFFFF)==0x7F000001) || ((reply_tuple.dst.u3.ip&0xFFFFFFFF)==0x7F000001))
		return 0;
	
	napt.protocol = ori_tuple.dst.protonum;

	// for GCC 4.x.0 warning
	tpdir1.src.u3.ip = 0;
	tpdir2.src.u3.ip = 0;
	tpdir2.dst.u3.ip = 0;
	
	//cathy, for multi-subnet
	for(i=0; i< routeIndex; i++) {
		if ( (ori_tuple.src.u3.ip & LANmask[i]) == LANsub[i] ) {
			tpdir1 = ori_tuple;
			tpdir2 = reply_tuple;
			break;
		}
		else {
			tpdir1 = reply_tuple;
			tpdir2 = ori_tuple;
		}
	}	
	

	napt.intIp = tpdir1.src.u3.ip;
	napt.intPort = ntohs(tpdir1.src.u.all);
	napt.extIp = tpdir2.dst.u3.ip;
	napt.extPort = ntohs(tpdir2.dst.u.all);
	napt.remIp = tpdir2.src.u3.ip;
	napt.remPort = ntohs(tpdir2.src.u.all);

	return (fastpath_delNaptConnection(&napt));
}


extern void nic_tx2(struct sk_buff* skb,struct net_device *tdev);

__MIPS16 __IRAM_SYS_MIDDLE int ip_finish_output3(struct sk_buff *skb, unsigned int course)
{
	struct dst_entry *dst = skb->dst;
	struct hh_cache *hh = dst->hh;
	struct net_bridge_fdb_entry *fpdst;
	struct net_bridge *br;	//suppose skb->dev is bridge
	unsigned char *dest;

	struct neighbour *neigh =NULL;

	 
	if(!hh)
	{
		if (!(neigh = dst->neighbour)) {
				printk("dst neighbour is NULL.dst %s \n",dst->dev->name);
				goto DROP;
		}

		/* Tank:使用GRE tunnel时，这里得到的dst->dev->header_ops为空，这是因为IP GRE设备初始化时没有赋值。
		   参看ip_gre.c中的ipgre_tunnel_init函数。在其它IP设备上使用时也是需要先判断header_ops是否为空，才进行使用。
		   header_ops是网络设备驱动中关于硬件地址（MAC）处理的相关函数，网络设备通常需要从固件中获取相应的mac地址
		   并使用此结构体中的函数完成对于数据包头中相关物理地址的操作。很明显，普通数据情况下，GRE tunnel自己本身
		   不再加MAC头，由其它的IP及NAS设备来做。从GRE tunnel的初始化也可以看出，当目标地址为广播或者0的情况下，
		   才直接来创建MAC头。
		*/
		if(dst->dev->header_ops != NULL)
		{
			if(dst->dev->header_ops->cache!=NULL)
			{
				neigh_hh_init(neigh, dst, dst->ops->protocol);
				hh = dst->hh;
			}
			else
			{
				if(dev_hard_header(skb, dst->dev, ntohs(skb->protocol),
				       neigh->ha, NULL, skb->len)<0)
					goto DROP;    
			}
		}
				
	}
	
	if(hh)
	{
		int hh_alen; 
		
	
		read_lock_bh((rwlock_t *)(&hh->hh_lock));
		hh_alen = HH_DATA_ALIGN(hh->hh_len);

		//printk("hh_data    hh_alen = %d \n",hh_alen);
		 
  		memcpy(skb->data - hh_alen, hh->hh_data, hh_alen);	
		read_unlock_bh((rwlock_t *)(&hh->hh_lock));	
		//printk("%s %d skb->data %x skb->head %x \n", __FUNCTION__, __LINE__,(int)skb->data,(int)skb->head);
		skb_push(skb, hh->hh_len);
	}
//forwarding process	
	 {


		if (course == 1) {//upstream
				DEBUGP_PKT("%s xmit dev %s (%x)\n", __func__, skb->dev->name, (unsigned int)skb->dev->hard_start_xmit);

#ifdef CONFIG_ATP_SUPPORT_ETHUP
			if(qos_enable) {//nas0
				skb->imq_flags &= ~IMQ_F_ENQUEUE;
				dev_queue_xmit(skb);
			} else 
			{
				#if defined(CONFIG_COMPAT_NET_DEV_OPS)
				skb->dev->hard_start_xmit(skb,skb->dev);
				#else
				skb->dev->netdev_ops->ndo_start_xmit(skb,skb->dev);
				#endif
			}
#else 
			#if defined(CONFIG_COMPAT_NET_DEV_OPS)
			skb->dev->hard_start_xmit(skb,skb->dev);
			#else
			skb->dev->netdev_ops->ndo_start_xmit(skb,skb->dev);
			#endif
#endif
			return 1;
		}
		else {
			struct net_device *out_dev;			
			dest = eth_hdr(skb)->h_dest;			
			br = skb->dev->priv;
			if( !br) {
				printk("%s %d br is NULL.\n", __FUNCTION__, __LINE__);
				kfree_skb(skb);
				return 1;
			}

			//printk("dest mac %x %x %x %x %x %x\n",dest[0],dest[1],dest[2],dest[3],dest[4],dest[5]);

			if (dest && ((fpdst = br_fdb_get(br, dest)) != NULL))
			{
				out_dev = fpdst->dst->dev;	                
                
				//Kevin, decrese the referencing count
                br_fdb_put(fpdst);                 
                
				//if(netif_running(fpdst->dst->dev)){
				//	fpdst->dst->dev->hard_start_xmit(skb, fpdst->dst->dev);
				//} else {
				//	kfree_skb(skb);
				//}				
				//return 1;
			}
			else{
				//printk("can not found in fdb:%x %x %x %x %x %x\n",dest[0],dest[1],dest[2],dest[3],dest[4],dest[5]);
				//nic_tx2(skb,NULL);
				out_dev = skb->dev;
				//skb->dev->hard_start_xmit(skb,skb->dev);				
				//return 1;
			}
			if (netif_running(out_dev))
			{
				skb->dev = out_dev;
				#if defined(CONFIG_COMPAT_NET_DEV_OPS)
				out_dev->hard_start_xmit(skb, out_dev);				
				#else
				out_dev->netdev_ops->ndo_start_xmit(skb,out_dev);
				#endif
			}
			else
				kfree_skb(skb);
			return 1;
		}
	}

DROP:
	printk( "ip_finish_output3: %s(%s) No header cache and no neighbour!\n", dst->dev->name, (course==1)?"UP":"DOWN");
	kfree_skb(skb);
	return -EINVAL;
}

#ifdef CONFIG_RTL867X_KERNEL_MIPS16_NET
__NOMIPS16
#endif
__SRAM int
FastPath_Enter(struct sk_buff *skb)	/* Ethertype = 0x0800 (IP Packet) */
{
	//ql, I think it is impossible that skb->dev is lo here.
	//if (!strcmp(skb->dev->name, "lo")) return 0;
	
	struct net_device *dev_backup = NULL;
    struct net_bridge_port *br_port = skb->dev->br_port;
    int ret;
    
    if(br_port) 
    {
        /* Kevin, if dev is under some bridge device(ex.br0)
                  we have to re-assign br0 as skb->dev when the skb enters fastpath
                  because br0 is the source interface in the routing subsystem
                  ip_route_input() in  fp_iproute_input() will use it!
              */
        dev_backup = skb->dev;
        skb->dev = skb->dev->br_port->br->dev;
        
    }
	ret = FastPath_Process((void *)skb, (struct iphdr*)skb_network_header(skb),br_port);
    if(br_port && !ret)
    {
        /* Kevin, if it does not go into fastpath, restore the skb->dev  */
        skb->dev = dev_backup;        
    }
	//QL: check if traffic exists to decide whether we can flush ACL chain.
#ifdef CONFIG_ETHWAN
	if (1 == ret)
	{
		if(fpStartStamp==0) 
			fpStartStamp = jiffies;

		if(fpPktCount++ > FASTPATH_POLL_THRESH) {
			if((jiffies - fpStartStamp) <= 1) {
				heavyTraffic = 1;
			} else {
				heavyTraffic =0;
			}
			
			fpPktCount = 0;
			fpStartStamp = jiffies;
		}
	}
#endif
    
    return ret;
}

//a wrapper for br_fdb_update
void fp_br_fdb_update( struct net_bridge_port *br_port, struct sk_buff *pskb){
#if defined(CONFIG_VLAN_MODE)
	br_fdb_update(br_port->br, br_port, eth_hdr(((struct sk_buff *)pskb))->h_source, pskb->vlan);
#else
	br_fdb_update(br_port->br, br_port, eth_hdr(((struct sk_buff *)pskb))->h_source);
#endif
}


// todo: IMQ for fastpath
#if 0
#if defined(CONFIG_IMQ) || defined(CONFIG_IMQ_MODULE)
/*return value: 1-enqueue success         0-enqueue fail*/
__SRAM int imq_enqueue(struct sk_buff *skb)
{
	/* after entered imq1, now entering imq2 */
	if ((qos_enable) && (skb->imq_flags & IMQ_F_ENQUEUE))
	{
		//skb->imq_flags &= ~IMQ_F_ENQUEUE;
		//skb->imq_flags &= ~QOS_DEV_IMQ1;
		skb->iif = skb->dev->ifindex;
		skb->imq_flags |= QOS_DEV_IMQ1;

		/* start no queue marked packet to default queue A36D06414	by f00110348 */
		if (!(skb->mark & 0x70))
		{
			skb->mark |= 0x60;
		}
		/* end no queue marked packet to default queue A36D06414 by f00110348 */

		//printk("%s enqueue imq1.\n", __func__);
		if (imq_nf_queue(skb, NULL, NULL) > 0)
			return 1;
	}
	return 0;
}
#endif    
#endif


#if 0//CONFIG_RTL_FASTHPATH_NOTIFICATION
#if 0
inline static char *events_to_str(unsigned long events) {
	static char buf[80];
	
	buf[0] = 0;
	
	if (events & IPCT_NEW)
		strncat(buf, "NEW ", sizeof(buf));
	if (events & IPCT_RELATED)
		strncat(buf, "Related ", sizeof(buf));
	if (events & IPCT_DESTROY)
		strncat(buf, "Destroy ", sizeof(buf));
	if (events & IPCT_REFRESH)
		strncat(buf, "Refresh ", sizeof(buf));
	if (events & IPCT_STATUS)
		strncat(buf, "Status ", sizeof(buf));
	if (events & IPCT_PROTOINFO)
		strncat(buf, "Proto ", sizeof(buf));
	if (events & IPCT_PROTOINFO_VOLATILE)
		strncat(buf, "ProtoV ", sizeof(buf));
	if (events & IPCT_HELPER)
		strncat(buf, "Helper ", sizeof(buf));
	if (events & IPCT_HELPINFO)
		strncat(buf, "HelpInf ", sizeof(buf));
	if (events & IPCT_HELPINFO_VOLATILE)
		strncat(buf, "HelpInfV ", sizeof(buf));
	if (events & IPCT_NATINFO)
		strncat(buf, "NAT ", sizeof(buf));
	if (events & IPCT_COUNTER_FILLING)
		strncat(buf, "Cntr ", sizeof(buf));
	if (events & IPCT_MARK)
		strncat(buf, "Mark ", sizeof(buf));
		
	return buf;
}
#endif

static int ctnetlink_conntrack_event(struct notifier_block *this,
                                     unsigned long events, void *ptr)
{
	#define NF_MASK (IPCT_STATUS | IPCT_REFRESH)

	struct nf_ct_event *item = (struct nf_ct_event *)ptr;
	struct nf_conn *ct = item->ct;

	if (!FastPath_Enabled()) {
		return NOTIFY_DONE;
	}
	
	if (ct == &nf_conntrack_untracked)
		return NOTIFY_DONE;
	
	if (nf_ct_l3num(ct) != PF_INET)
		return NOTIFY_DONE;

	if (IPPROTO_ICMP == nf_ct_protonum(ct))
		return NOTIFY_DONE;

	#if 1
	
	//printk("nf_event: %x %s status:%lx\n", (int)events, events_to_str(events), ct->status);

	if (events & IPCT_DESTROY) {
		//if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) {
		//	{
		rtl867x_delNaptConnection(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
				ct->tuplehash[IP_CT_DIR_REPLY].tuple);
		//	}
		//}			
	} else if ((events & NF_MASK)==NF_MASK) {
		if (ct->status & IPS_ASSURED)		
			rtl867x_addNaptConnection(ct,ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
				ct->tuplehash[IP_CT_DIR_REPLY].tuple, NP_NONE,1);						
		
	}
	#endif
	
	#if 1
	if (1/*events & (IPCT_NEW | IPCT_DESTROY)*/) {
		
				
	    printk("%p(%08x:%04x->%02x,%08x:%04x)-(%08x:%04x->%02x,%08x:%04x)", 
		ct,
		//ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.protonum,
		ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip,
		ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all,
		ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum,
		ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip,
		ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all,
		
		//ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.protonum,
		ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip,
		ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all,
		ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.protonum,
		ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip,
		ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all);
		printk(" Evt:%x St=%lx\n", (int)events, ct->status);
		// calls to add/del API
	}
	#endif

	return NOTIFY_DONE;
}



static struct notifier_block ctnl_notifier = {
	.notifier_call	= ctnetlink_conntrack_event,
};


#define MODULE_NAME "rtkFPMgmt"

struct fflow_t {
	struct task_struct *thread;
	struct socket *sock;
	struct sockaddr_nl addr;
	u8   recvbuf[256];
	char running;
};

static struct fflow_t  *fflow;

#if 0
static void memDump (void *start, u32 size, char * strHeader)
{
	int row, column, index, index2, max;
//	uint32 buffer[5];
	u8 *buf, *line, ascii[17];
	char empty = ' ';

	if(!start ||(size==0))
		return;
	line = (u8*)start;

	/*
	16 bytes per line
	*/
	if (strHeader)
		printk("%s", strHeader);
	column = size % 16;
	row = (size / 16) + 1;
	for (index = 0; index < row; index++, line += 16) 
	{
		buf = line;

		memset (ascii, 0, 17);

		max = (index == row - 1) ? column : 16;
		if ( max==0 ) break; /* If we need not dump this line, break it. */

		printk("\n%08x ", (u32) line);
		
		//Hex
		for (index2 = 0; index2 < max; index2++)
		{
			if (index2 == 8)
			printk("  ");
			printk("%02x ", (u8) buf[index2]);
			ascii[index2] = ((u8) buf[index2] < 32) ? empty : buf[index2];
		}

		if (max != 16)
		{
			if (max < 8)
				printk("  ");
			for (index2 = 16 - max; index2 > 0; index2--)
				printk("   ");
		}

		//ASCII
		printk("  %s", ascii);
	}
	printk("\n");
	return;
}
#endif 

static void handle_route_event(struct nlmsghdr *nlm, int size) {
	struct rtmsg *rtm;
	struct nlattr *nla[RTA_MAX+1];
	//const char *msgtype;
	struct net_device *dev;	
	int nlalen = size - sizeof(*rtm);
	rtm = nlmsg_data(nlm);
	
	#if 0
	printk("RT: fm:%d, dst:%d src:%d tos:%d tbl:%d proto:%d scope:%d type:%d flags:%x\n", 
		rtm->rtm_family, rtm->rtm_dst_len, rtm->rtm_src_len, rtm->rtm_tos, 
		rtm->rtm_table, rtm->rtm_protocol, rtm->rtm_scope, rtm->rtm_type, rtm->rtm_flags);
	printk("NLA: len %d\n", nla_len);
	memDump( &rtm[1], nla_len, "NLA");
	#endif

	nla_parse(nla, RTA_MAX, (struct nlattr *)&rtm[1], nlalen, 0);
	dev = dev_get_by_index(&init_net, nla_get_be32(nla[RTA_OIF]));

	switch(nlm->nlmsg_type) {
	case RTM_NEWROUTE: 
		//msgtype = "AddRt"; 
		if (nlm->nlmsg_flags & NLM_F_REPLACE)
			rtl867x_modifyRoute(nla[RTA_DST] ? nla_get_be32(nla[RTA_DST]) : 0,
			inet_make_mask(rtm->rtm_dst_len),
			nla[RTA_GATEWAY] ? nla_get_be32(nla[RTA_GATEWAY]) : 0,
			dev ? dev->name : 0,RT_NONE,rtm->rtm_type);
		else
			rtl867x_addRoute(nla[RTA_DST] ? nla_get_be32(nla[RTA_DST]) : 0,
			inet_make_mask(rtm->rtm_dst_len),
			nla[RTA_GATEWAY] ? nla_get_be32(nla[RTA_GATEWAY]) : 0,
			dev ? dev->name : 0,RT_NONE,rtm->rtm_type);
			
		break;
	case RTM_DELROUTE: 
		//msgtype = "DelRt"; 
		rtl867x_delRoute(nla[RTA_DST] ? nla_get_be32(nla[RTA_DST]) : 0, inet_make_mask(rtm->rtm_dst_len));
		break;
	//default: msgtype = "***";
	}
	#if 0
	printk("%s: %08x/%08x gw=%08x if=%s flag=%08x\n", 
		msgtype,
		nla[RTA_DST] ? nla_get_be32(nla[RTA_DST]) : 0, 
		inet_make_mask(rtm->rtm_dst_len), 
		nla[RTA_GATEWAY] ? nla_get_be32(nla[RTA_GATEWAY]) : 0, 
		dev ? dev->name : "<dev>",  
		rtm->rtm_type);
	#endif

	if (dev)
		dev_put(dev);

}

static void handle_neigh_event(struct nlmsghdr *nlm, int size) {
	struct ndmsg *ndm;
	//const char *msgtype;
	struct nlattr *nla[NDA_MAX+1];	
	u32 key;
	u8  ha[16];
	int nlalen = size - sizeof(*ndm);
	ndm = nlmsg_data(nlm);
	
	memset(nla, 0, sizeof(nla));
	memset(ha, 0, sizeof(ha));
	key = 0;

	if (ndm->ndm_family != AF_INET)
		return;

	//if (!(ndm->ndm_state & NUD_VALID))
	//	return;

	if (ndm->ndm_type!=RTN_UNICAST)
		return;

	nla_parse(nla, NDA_MAX, (struct nlattr *)&ndm[1], nlalen, 0);
	if (!nla[NDA_DST])
		return;	
	memcpy(&key, nla_data(nla[NDA_DST]), 4);
	if (nla[NDA_LLADDR])
		memcpy(&ha, nla_data(nla[NDA_LLADDR]), nla_len(nla[NDA_LLADDR]));
	
	switch(nlm->nlmsg_type) {
	case RTM_NEWNEIGH: 
		//msgtype = "AddArp"; 
		if (ndm->ndm_state & NUD_VALID) {
			if (rtl867x_addArp(key,(ether_t *)ha,ARP_NONE))
				rtl867x_modifyArp(key,(ether_t *)ha,ARP_NONE);
		} else if ((ndm->ndm_state & NUD_FAILED) && (!nla[NDA_LLADDR])) {
			/* entry changed  to INVALID, should remove it now */
			rtl867x_delArp(key);	
		}
		break;
	case RTM_DELNEIGH: 
		//msgtype = "DelArp"; 
		rtl867x_delArp(key);
		break;
	//default: msgtype = "***";
	}
	#if 0
	memDump(nlm, size + sizeof(*nlm), "NEIGH");
	printk("nlm=%p ndm=%p nla=%p,probe=%p,ci=%p\n",nlm,ndm,&ndm[1],nla[NDA_PROBES],nla[NDA_CACHEINFO]);
	
	#endif
}

static int ksocket_receive(struct socket* sock, unsigned char* buf, int len)
{
	//struct nlmsghdr *h;
	struct sockaddr_nl nladdr;
	struct msghdr msg;
	struct iovec iov;
	mm_segment_t oldfs;
	int size = 0;

	if (sock->sk==NULL) return 0;

	iov.iov_base = buf;
	iov.iov_len = len;

	msg.msg_flags = 0;
	msg.msg_name = &nladdr;
	msg.msg_namelen  = sizeof(nladdr);
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;

	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;

	oldfs = get_fs();
	set_fs(KERNEL_DS);
	size = sock_recvmsg(sock,&msg,len,msg.msg_flags);
	set_fs(oldfs);

	//h = (struct nlmsghdr *)buf; 
	return size;
}


static void kthread_start(void)
{
	int size, err;
	
	/* kernel thread initialization */
	lock_kernel();
	fflow->running = 1;
	current->flags |= PF_NOFREEZE;
	
	/* daemonize (take care with signals, after daemonize() they are disabled) */
	daemonize(MODULE_NAME);
	allow_signal(SIGKILL);
	unlock_kernel();
	
	/* create a socket */
	if ( (err = sock_create(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE, &fflow->sock)) < 0)
	{
		printk(KERN_INFO MODULE_NAME": Could not create a datagram socket, error = %d\n", -ENXIO);
		goto out;
	}
	
	memset(&fflow->addr, 0, sizeof(struct sockaddr));
	
	fflow->addr.nl_family = AF_NETLINK;      
	fflow->addr.nl_groups = ((1 << (RTNLGRP_IPV4_ROUTE- 1)) | (1 << (RTNLGRP_NEIGH-1)));
	
	if ( (err = fflow->sock->ops->bind(fflow->sock, (struct sockaddr *)&fflow->addr, sizeof(struct sockaddr) ) ) < 0) 
	{
		printk(KERN_INFO MODULE_NAME": Could not bind or connect to socket, error = %d\n", -err);
		goto close_and_out;
	}
	
	printk(KERN_INFO MODULE_NAME": listening \n");
	
	/* main loop */
	for (;;)
	{
		memset(fflow->recvbuf, 0, sizeof(fflow->recvbuf));
		size = ksocket_receive(fflow->sock, fflow->recvbuf, sizeof(fflow->recvbuf));
		
		if (signal_pending(current))
			break;

		if (!FastPath_Enabled())
			continue;
		
		if (size < 0)
			printk(KERN_INFO MODULE_NAME": error getting datagram, sock_recvmsg error = %d\n", size);
		else 
		{          
			struct nlmsghdr *h = (struct nlmsghdr *)fflow->recvbuf;
			
			int len = h->nlmsg_len;
			int l = len - sizeof(*h);

			if (l<0 || len>sizeof(fflow->recvbuf)) {
				printk("malformed message: len=%d\n", len);
				continue;
			}
			
			//printk(MODULE_NAME ":read %d(%d/%d)\n", size, len, l);
			//printk(" nlmsg: type:%d,flag:%x,seq:%d,pid:%d\n", h->nlmsg_type,h->nlmsg_flags,h->nlmsg_seq,h->nlmsg_pid);

			switch (h->nlmsg_type) {
			case RTM_NEWROUTE:
			case RTM_DELROUTE:
				handle_route_event(h, l);				
				break;
			case RTM_NEWNEIGH:
			case RTM_DELNEIGH: 
				handle_neigh_event(h, l);
				break;
			}
		}
	}

close_and_out:
	sock_release(fflow->sock);       
	fflow->sock = NULL;       

out:
	fflow->thread = NULL;
	fflow->running = 0;
}



static int __init fflow_init(void)
{	
	//if (nf_conntrack_register_notifier(&ctnl_notifier) < 0) {
	//	printk("failed to register NF notifier: %s(%d)\n",__FUNCTION__,__LINE__);
	//}

	fflow = kmalloc(sizeof(struct fflow_t), GFP_KERNEL);
	memset(fflow, 0, sizeof(struct fflow_t));
	
	/* start kernel thread */
	fflow->thread = kthread_run((void *)kthread_start, NULL, MODULE_NAME);
	if (IS_ERR(fflow->thread)) 
	{
		printk(KERN_INFO MODULE_NAME": unable to start kernel thread\n");
		kfree(fflow);
		fflow = NULL;
		return -ENOMEM;
	}
	
	return 0;
}

static void __exit fflow_exit(void)
{
	int err;

	if (fflow->thread==NULL)
		printk(KERN_INFO MODULE_NAME": no kernel thread to kill\n");
	else 
	{
		lock_kernel();
		err = kill_pid(find_get_pid(fflow->thread->pid), SIGKILL, 1);
		unlock_kernel();

		/* wait for kernel thread to die */
		if (err < 0)
			printk(KERN_INFO MODULE_NAME": unknown error %d while trying to terminate kernel thread\n",-err);
		else 
		{
			while (fflow->running == 1)
				msleep(10);
			printk(KERN_INFO MODULE_NAME": succesfully killed kernel thread!\n");
		}
	}

	/* free allocated resources before exit */
	if (fflow->sock != NULL) 
	{
		sock_release(fflow->sock);
		fflow->sock = NULL;
	}

	kfree(fflow);
	fflow = NULL;

	printk(KERN_INFO MODULE_NAME": module unloaded\n");
}


module_init(fflow_init);
module_exit(fflow_exit);

#endif //CONFIG_RTL_FASTHPATH_NOTIFICATION

