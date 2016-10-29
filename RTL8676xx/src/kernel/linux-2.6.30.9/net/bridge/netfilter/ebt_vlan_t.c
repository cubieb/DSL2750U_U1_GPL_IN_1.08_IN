
/*
 *  ebt_vlan
 *
 *	Authors:
 *	ouyangdi <fireor@126.com>
 *
 *  July, 2008
 *
 */

/* The vlan target can be used in any chain,
 * I believe adding a mangle table just for vlaning is total overkill.
 * Marking a frame doesn't really change anything in the frame anyway.
 *
 *change log:
 *
 * 2008/06/18 ouyangdi
 * fix bug don't normal foward large packet when used --vlan-untag function
 */

#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_vlan_t.h>
#include <linux/module.h>
#include <linux/if_vlan.h>
#include <linux/ip.h>

#define OPT_VLAN_TARGET  0x01
#define OPT_VLAN_SET  0x02
#define OPT_VLAN_PRIORITY 0x04
#define OPT_VLAN_HEADER   0x08
#define OPT_VLAN_UNTAG 0x10
//#define VLAN_DEBUG 

static int ebt_set_vlan_header(struct sk_buff **pskb, unsigned char cmd)
{
       struct sk_buff *skb = *pskb;

       if (cmd & OPT_VLAN_UNTAG) {
               /* frame should be untagged */
		struct iphdr * iph;			   
		iph = skb->nh.iph;
		
              if (eth_hdr(skb)->h_proto == htons(ETH_P_8021Q)) {
                       /* remove VLAN tag */		   
                       if (skb_cloned(skb) || skb_shared(skb)) {
                               struct sk_buff *new_skb;
                               new_skb = skb_copy(skb, GFP_ATOMIC);
                               kfree_skb(skb);
                               if (!new_skb)
                                       return 1;
                               *pskb = skb = new_skb;
                       }
					   
			  skb_pull(skb, VLAN_HLEN);	
                       memmove(skb->mac.raw + VLAN_HLEN, skb->mac.raw, ETH_ALEN * 2);
                       skb->mac.raw += VLAN_HLEN;
                       //skb_pull(skb, VLAN_HLEN);
                       
			  /*reset skb->protocol,if no reset will error in local process*/		   
		         skb->protocol = *(unsigned short *)(skb->mac.raw +  ETH_ALEN * 2);
					   
			  /*skb->protocol was been reset,need to move back VLAN_HLEN bytes 
			   2008/06/18 ouyangdi*/	
			  skb->nh.raw += VLAN_HLEN; 		   
               }		   
       } else {
               /* frame should be tagged */
		if (eth_hdr(skb)->h_proto != htons(ETH_P_8021Q)) {
                       /* add VLAN tag */
                     struct vlan_ethhdr *vhdr;
		       unsigned tag = skb->vlan;
#ifdef VLAN_DEBUG
                      printk("vlan tag = %d\n",skb->vlan);
#endif

                     if (skb_cloned(skb) || skb_shared(skb)) {
                             struct sk_buff *new_skb;

                             new_skb = skb_copy(skb, GFP_ATOMIC);
                             kfree_skb(skb);
                             if (!new_skb)
                                     return 1;
                             *pskb = skb = new_skb;
                     }

                     if (skb_headroom(skb) < VLAN_HLEN) {
                             if (pskb_expand_head(skb, VLAN_HLEN, 0,
                                                  GFP_ATOMIC)) {
                                     kfree_skb(skb);
                                     return 1;
                             }
                     
		     }

                     skb_push(skb, VLAN_HLEN);
		     if(skb_headroom(skb) < ETH_HLEN) {
			int len = ETH_HLEN - skb_headroom(skb);
                               if (pskb_expand_head(skb, len, 0,
                                                    GFP_ATOMIC)) {
                                       kfree_skb(skb);
                                       return 1;
                               }
                     }
	
                     skb->mac.raw -= VLAN_HLEN;
                     memmove(skb->mac.raw, skb->mac.raw + VLAN_HLEN,
                             ETH_ALEN * 2);
                     vhdr = (struct vlan_ethhdr *)skb->mac.raw;
                     vhdr->h_vlan_proto = htons(ETH_P_8021Q);
#ifdef VLAN_DEBUG
		       printk("vlan tag = %d\n",skb->vlan);
#endif

                     vhdr->h_vlan_TCI = htons(tag);
		     skb->protocol = htons(ETH_P_8021Q);
#ifdef VLAN_DEBUG
        	       int i = 0;
        	       printk("\nmac raw \n");
        		for(i = 0; i < 16; i++)
        		{
                		if(!(i%2))
                        		printk(" ");
                		printk("%02x",*((*pskb)->mac.raw + i));
        		}
        		printk("\n");

#endif
#if 0		
			if(skb_headroom(skb) < ETH_HLEN) {
			       if (pskb_expand_head(skb, ETH_HLEN, 0,
                                                    GFP_ATOMIC)) {
                                       kfree_skb(skb);
                                       return 1;
                               }
			}
#endif
               } else {
                       /* ensure VID is correct */
                      struct vlan_ethhdr *vhdr =
                              (struct vlan_ethhdr *)skb->mac.raw;
			 vhdr->h_vlan_TCI = htons(skb->vlan);
               }
       }

       return 0;
}

static int ebt_target_vlan(struct sk_buff **pskb, unsigned int hooknr,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
{
	struct  ebt_vlan_t_info *info = (struct ebt_vlan_t_info *)data;
	
	switch(info->cmd)
	{
		case OPT_VLAN_SET:
			if (eth_hdr(*pskb)->h_proto != htons(ETH_P_8021Q))  {
				/*if info->vlan=0,can't set vlan tag*/
				if(!info->vlan && ((*pskb)->vlan == 0))
					break;

				if(info->vlan > 0)	
					(*pskb)->vlan = info->vlan;

				if(info->priority) {
					(*pskb)->vlan &= ~0xe000;
					(*pskb)->vlan |= (info->priority <<13 & 0xe000);
				}	
			} else {
				if(!info->vlan) {
					 struct vlan_ethhdr *vhdr =
                               		(struct vlan_ethhdr *)(*pskb)->mac.raw;
					 //(*pskb)->vlan = (vhdr->h_vlan_TCI & VLAN_VID_MASK);
					/*vlan value include CFI bit value*/ 
					(*pskb)->vlan = (vhdr->h_vlan_TCI & 0x1fff);
				}
				else 
					(*pskb)->vlan = info->vlan;
				
				if(!info->priority) {
					struct vlan_ethhdr *vhdr =
                               		(struct vlan_ethhdr *)(*pskb)->mac.raw;
					(*pskb)->vlan |= (vhdr->h_vlan_TCI & 0xe000);
				} else {
					(*pskb)->vlan &= ~0xe000;
					(*pskb)->vlan |= (info->priority <<13 & 0xe000);
				}
			}	
			break;

		case OPT_VLAN_HEADER:
			if((*pskb)->vlan == 0)
				return EBT_ACCEPT;
			if(ebt_set_vlan_header(pskb,info->cmd))
				return EBT_DROP;
			break;
			
		case OPT_VLAN_UNTAG:
			if(ebt_set_vlan_header(pskb,info->cmd))
				return EBT_DROP;
			break;

		default:
			break;
	}
	return info->target;
}

static int ebt_target_vlan_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
{
	struct ebt_vlan_t_info *info = (struct ebt_vlan_t_info *)data;

	if (datalen != EBT_ALIGN(sizeof(struct ebt_vlan_t_info)))
		return -EINVAL;
	if (BASE_CHAIN && info->target == EBT_RETURN)
		return -EINVAL;
	CLEAR_BASE_CHAIN_BIT;
	if (INVALID_TARGET)
		return -EINVAL;
	return 0;
}

static struct ebt_target vlan_target =
{
	.name		= EBT_VLAN_TARGET,
	.target		= ebt_target_vlan,
	.check		= ebt_target_vlan_check,
	.me		= THIS_MODULE,
};

static int __init ebt_vlan_init(void)
{
	return ebt_register_target(&vlan_target);
}

static void __exit ebt_vlan_fini(void)
{
	ebt_unregister_target(&vlan_target);
}

module_init(ebt_vlan_init);
module_exit(ebt_vlan_fini);
MODULE_LICENSE("GPL");

