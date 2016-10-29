#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge.h>
#include <net/br_vlan.h>
#include "br_private.h"
//#include "nfmark.h"
//#define VLAN_M_DEBUG

#if 0 //Noused,by Zhang Yu
void DumpPacketvlan(char *tag,struct sk_buff *skb)
{
    int i=0;
    printk("\n###############################################\n");
   printk("\n****** %s ******\n",tag);

    for(i=0;i < 48/*i<skb->len*/;i++)
    {
             if( i%16 == 0)
                     printk("\n0x%08x:",i);
             if( i%4 == 0)
                     printk(" ");

             printk("%02x ",skb->data[i]);
     }

     printk("\n\n###############################################\n\n");
     printk("\n\n");
}
#endif



#ifdef VLAN_M_DEBUG
    #define VLAN_M_DBG printk("%s %d\n", __FUNCTION__, __LINE__)
#else
    #define VLAN_M_DBG
#endif

//cjg add start
#if defined(CONFIG_VLAN_MODE)
/* 
* TBS_TAG : by yuanke 2011-2-24 
* Desc : Merge vlan mode support from E8PonV3
*/
unsigned short br_get_vlan(struct net_bridge *br,
					  const unsigned char *addr)
{
    struct net_bridge_fdb_entry *fdb = __br_fdb_get(br, addr);
    if(fdb != NULL)
        return fdb->vid;

    return 0;
}
#endif
//cjg add end
int vlan_init_mode(struct net_device * dev)
{
    VLAN_MODE_T mode;


    if(dev->br_port == NULL)
        return VLAN_ERR;

    memset(&mode, 0, sizeof(VLAN_MODE_T));
    mode.mode = VLAN_TRANSPARENT;

 

    return vlan_set_mode(dev, &mode);
}

int vlan_set_mode(struct net_device * dev, VLAN_MODE_T *mode)
{
    if(dev == NULL || mode == NULL)
        return VLAN_PARAM;

    if(dev->mode == NULL)
        dev->mode = (VLAN_MODE_T*)kmalloc(sizeof(VLAN_MODE_T),GFP_ATOMIC);

    if(dev->mode != NULL)
        memcpy(dev->mode, mode, sizeof(VLAN_MODE_T));
    else
        return VLAN_ERR;
    
    return VLAN_OK;
}



/* the caller should ensure port in the range of 1 to VLAN_PORT_NUM */
inline VLAN_MODE_T* vlan_get_mode(struct net_device * dev)
{
    if(dev)
        return dev->mode;
    
    return NULL;
}

static inline unsigned int vlan_is_trunk_vlan(VLAN_MODE_T *mode, VLAN_T vlan)
{
    int num;

    vlan = (vlan & 0x0fff);

    for( num=0; num<mode->num; num++)
    {
        if(mode->u.trunk[num] == vlan)
            return 1;
    }

    return 0;
}

static inline unsigned int vlan_is_hybird_vlan(VLAN_MODE_T *mode, VLAN_T vlan)
{
    int num;

    vlan = (vlan & 0x0fff);

    for( num=0; num<mode->u.hybird.trans_num; num++)
    {
        if(mode->u.hybird.trans_vlan[num] == vlan)
            return 1;
    }

    return 0;
}

static inline unsigned int vlan_is_hybird_untag_vlan(VLAN_MODE_T *mode, VLAN_T vlan)
{
    int num;

    vlan = (vlan & 0x0fff);
    for( num=0; num<mode->u.hybird.untag_num; num++)
    {
        if(mode->u.hybird.untag_vlan[num] == vlan)
            return 1;
    }

    return 0;
    
}

/* return new vlan */
static inline unsigned int vlan_is_translate_vlan(VLAN_MODE_T *mode, VLAN_T vlan)
{
    int num;

    vlan = (vlan & 0x0fff);

    for( num=0; num<mode->num; num++)
    {
        if(mode->u.translate[num].old_vlan == vlan)
            return mode->u.translate[num].new_vlan;
    }

    return 0;
}

static inline unsigned int vlan_is_translate_new_vlan(VLAN_MODE_T *mode, VLAN_T vlan)
{
    int num;

    vlan = (vlan & 0x0fff);

    for( num=0; num<mode->num; num++)
    {
        if(mode->u.translate[num].new_vlan == vlan)
            return mode->u.translate[num].old_vlan;
    }

    return 0;
}

/* return aggr vlan */
static inline unsigned int vlan_is_aggr_vlan(VLAN_MODE_T *mode, VLAN_T vlan)
{
    int num;
    int n;

    vlan = (vlan & 0x0fff);

    for( num=0; num<mode->num; num++)
    {
        for(n=0; n < mode->u.aggr[num].num; n++)
        {
            if( mode->u.aggr[num].vlan_n[n] == vlan )
                return mode->u.aggr[num].vlan_1;
        }
    }

    return 0;
}

static inline unsigned int vlan_is_aggrd_vlan(VLAN_MODE_T *mode, VLAN_T vlan)
{
    int num;
    int n;

    vlan = (vlan & 0x0fff);

    for( num=0; num<mode->num; num++)
    {
        if( mode->u.aggr[num].vlan_1 == vlan )
            return mode->u.aggr[num].vlan_1;
    }

    return 0;
}

#ifdef VLAN_M_DEBUG
void print_machead(void * data)
{
	struct vlan_ethhdr *vhdr = (struct vlan_ethhdr *)data;
	if(vhdr->h_vlan_proto == htons(ETH_P_8021Q))
	{
		printk("vlan eth header:\n");
		printk("dmac:%02x:%02x:%02x:%02x:%02x:%02x,dmac:%02x:%02x:%02x:%02x:%02x:%02x\n",
						vhdr->h_dest[0],vhdr->h_dest[1],vhdr->h_dest[2],
						vhdr->h_dest[3],vhdr->h_dest[4],vhdr->h_dest[5],
						vhdr->h_source[0],vhdr->h_source[1],vhdr->h_source[2],
						vhdr->h_source[3],vhdr->h_source[4],vhdr->h_source[5]);
		printk("h_vlan_proto:0x%x\n",htons(vhdr->h_vlan_proto));
		printk("vlan:%d\n",htons(vhdr->h_vlan_TCI) & 0xfff);
		printk("h_vlan_encapsulated_proto:0x%x\n",htons(vhdr->h_vlan_encapsulated_proto));		
		
	}else
	{
		printk("eth header:\n");
		printk("dmac:%02x:%02x:%02x:%02x:%02x:%02x,dmac:%02x:%02x:%02x:%02x:%02x:%02x\n",
						vhdr->h_dest[0],vhdr->h_dest[1],vhdr->h_dest[2],
						vhdr->h_dest[3],vhdr->h_dest[4],vhdr->h_dest[5],
						vhdr->h_source[0],vhdr->h_source[1],vhdr->h_source[2],
						vhdr->h_source[3],vhdr->h_source[4],vhdr->h_source[5]);
		printk("h_proto:0x%x\n",htons(vhdr->h_vlan_proto));
	}		
	 	
}
#endif

static inline int vlan_add_tag(VLAN_T vlan, struct sk_buff **pskb)
{
    struct sk_buff *skb = *pskb;
    struct vlan_ethhdr *vhdr;
    unsigned tag = vlan;//skb->vlan;
 

    if (skb_cloned(skb) || skb_shared(skb)) 
	  {
        struct sk_buff *new_skb;
        new_skb = skb_copy(skb, GFP_ATOMIC);
        kfree_skb(skb);
        if (!new_skb)
            return VLAN_DROP;
        *pskb = skb = new_skb;
    }
#ifdef VLAN_M_DEBUG		
		print_machead((void *)eth_hdr(skb));
#endif		

    if (skb_headroom(skb) < VLAN_HLEN) 
    {
        if (pskb_expand_head(skb, VLAN_HLEN, 0, GFP_ATOMIC)) 
        {
            kfree_skb(skb);
            return VLAN_DROP;
        }
       
    }
    
    skb_push(skb, VLAN_HLEN);
    if(skb_headroom(skb) < ETH_HLEN) 
    {
        int len = ETH_HLEN - skb_headroom(skb);
        if (pskb_expand_head(skb, len, 0, GFP_ATOMIC)) 
        {
            kfree_skb(skb);
            return VLAN_DROP;
        }
    }
    
    skb->mac_header -= VLAN_HLEN;
    memmove(skb->mac_header, skb->mac_header + VLAN_HLEN, ETH_ALEN * 2);
    vhdr = (struct vlan_ethhdr *)skb->mac_header;
    vhdr->h_vlan_proto = htons(ETH_P_8021Q);
    vhdr->h_vlan_TCI = htons(tag);
    skb->protocol = htons(ETH_P_8021Q);
    
#ifdef VLAN_M_DEBUG    
    print_machead((void *)eth_hdr(skb));
#endif    
    
    return VLAN_OK;
}



static inline void vlan_remove_tag(struct sk_buff **pskb)
{
    struct sk_buff *skb = *pskb;
    
    if (skb_cloned(skb) || skb_shared(skb))
    {
        struct sk_buff *new_skb;
        new_skb = skb_copy(skb, GFP_ATOMIC);
        kfree_skb(skb);
        if (!new_skb)
            return;
        *pskb = skb = new_skb;
    }

    skb_pull(skb, VLAN_HLEN);	
    memmove(skb->mac_header + VLAN_HLEN, skb->mac_header, ETH_ALEN * 2);
    skb->mac_header += VLAN_HLEN;

    /*reset skb->protocol,if no reset will error in local process*/		   
    skb->protocol = *(unsigned short *)(skb->mac_header +  ETH_ALEN * 2);
    skb->network_header += VLAN_HLEN;

    VLAN_M_DBG;
    return;
}


static inline void vlan_replace_tag(VLAN_T vlan, struct sk_buff *skb)
{
    vlan_eth_hdr(skb)->h_vlan_TCI = vlan;

    VLAN_M_DBG;

    return;
}

static unsigned int vlan_upstream(const struct net_device *in, struct sk_buff **pskb)
{
    VLAN_MODE_T *mode = NULL;
    struct sk_buff *skb = *pskb;
    
    VLAN_M_DBG;
        
    mode = vlan_get_mode(in);
    if(mode == NULL)
        return VLAN_OK;

    if(skb == NULL)
    {
        return VLAN_OK;
    }
    
#ifdef VLAN_M_DEBUG    
    printk("%s %d %d %d\n", __FUNCTION__, __LINE__, mode->mode, skb->vlan);
#endif
//DumpPacketvlan("vlan up",*pskb);

    struct vlan_ethhdr *vhdr = (struct vlan_ethhdr *)skb->mac_header;
    switch(mode->mode)
    {
        case VLAN_TRANSPARENT: //对包不对任务处理
            break;
        case VLAN_TRUNK:
            if(PKT_IS_8021Q(vhdr))
            {
                if( !vlan_is_trunk_vlan(mode, PKT_VLAN_ID(vhdr)) )
                    return VLAN_ERR;
            }
            else
                vlan_add_tag(mode->pvlan, pskb);
            break;
        case VLAN_TAG:
            if(PKT_IS_8021Q(vhdr))
                return VLAN_ERR;
            else
                vlan_add_tag(mode->pvlan, pskb);
            break;
        case VLAN_TRANSLATE:
            if(PKT_IS_8021Q(vhdr))
            {
                VLAN_T new_vlan = vlan_is_translate_vlan(mode, PKT_VLAN_ID(vhdr));
                if( new_vlan )
                    vlan_add_tag(new_vlan, pskb);
                else
                    return VLAN_ERR;   
            }
            else
                vlan_add_tag(mode->pvlan, pskb);
            break;
        case VLAN_AGGR:
            if(PKT_IS_8021Q(vhdr))
            {
                VLAN_T new_vlan = vlan_is_aggr_vlan(mode, PKT_VLAN_ID(vhdr));
                if( new_vlan )
                    vlan_add_tag(new_vlan, pskb);
                else
                    return VLAN_ERR;   
            }
            else
                vlan_add_tag(mode->pvlan, pskb);
            break;
        case VLAN_HYBIRD:
            if(PKT_IS_8021Q(vhdr))
            {
                if( !vlan_is_hybird_vlan(mode, PKT_VLAN_ID(vhdr)) )
                {
                    return VLAN_ERR;
                }
            }
            else
            {
                vlan_add_tag(mode->pvlan, pskb);
            }    
            break;
            
        default:
            return VLAN_ERR;
    }
    
//DumpPacketvlan("vlan uped",*pskb);

    return VLAN_OK;
}

static unsigned int vlan_downstream(const struct net_device *out, struct sk_buff **pskb)
{
    struct sk_buff *skb = *pskb;
    VLAN_MODE_T *mode = NULL;

    mode = vlan_get_mode(out);
    if(mode == NULL)
        return VLAN_OK;

    if(skb==NULL)
        return VLAN_OK;

    
#ifdef VLAN_M_DEBUG
    printk("%s %d %d %d %x\n", __FUNCTION__, __LINE__, mode->mode, skb->vlan, ntohs(skb->protocol));
#endif

    struct vlan_ethhdr *vhdr = (struct vlan_ethhdr *)skb->mac_header;

    switch(mode->mode)
    {
        case VLAN_TRANSPARENT:  //对包不对任务处理
            break;
        case VLAN_TRUNK:   
            if(PKT_IS_8021Q(vhdr))
            {
                if(PKT_VLAN_ID(vhdr) == mode->pvlan)
                {
                    vlan_remove_tag(pskb);
//DumpPacketvlan("vlan trunk down",*pskb);
                    return VLAN_OK;
                }
                if( !vlan_is_trunk_vlan(mode, PKT_VLAN_ID(vhdr)) )
                    return VLAN_DROP;
            }
            else
                return VLAN_DROP;
            break;
        case VLAN_TAG:
            if( PKT_IS_8021Q(vhdr) && PKT_VLAN_ID(vhdr) == mode->pvlan)
                vlan_remove_tag(pskb);
            else
                return VLAN_DROP;
            break;
        case VLAN_TRANSLATE:
            if(PKT_IS_8021Q(vhdr))
            {
                //struct vlan_ethhdr *vhdr = (struct vlan_ethhdr *)skb->mac.raw;
                VLAN_T new_vlan = PKT_VLAN_ID(vhdr);

                if(new_vlan == mode->pvlan)
                {
                    vlan_remove_tag(pskb);
                    return VLAN_OK;
                }
                new_vlan = vlan_is_translate_new_vlan(mode, PKT_VLAN_ID(vhdr));
                if( new_vlan )
                    vlan_replace_tag(new_vlan, pskb);
                else
                    return VLAN_DROP;
            }
            else
                return VLAN_DROP;
            break;
        case VLAN_AGGR:
            if(PKT_IS_8021Q(vhdr))
            {
                //struct vlan_ethhdr *vhdr = (struct vlan_ethhdr *)skb->mac.raw;
                VLAN_T new_vlan = PKT_VLAN_ID(vhdr);
                if(new_vlan == mode->pvlan)
                {
                    vlan_remove_tag(pskb);
                    return VLAN_OK;
                }
                new_vlan = vlan_is_aggrd_vlan(mode, PKT_VLAN_ID(vhdr));
                if( new_vlan )
                {
                    
                    vlan_replace_tag(br_get_vlan(skb->dev->br_port->br, vhdr->h_dest), pskb);
                }
                else
                    return VLAN_DROP;   
            }    
            break;
            
        case VLAN_HYBIRD:
            if(PKT_IS_8021Q(vhdr))
            {
                //struct vlan_ethhdr *vhdr = (struct vlan_ethhdr *)skb->mac.raw;
                if(PKT_VLAN_ID(vhdr) == mode->pvlan
                    || vlan_is_hybird_untag_vlan(mode, PKT_VLAN_ID(vhdr)))
                {
                    vlan_remove_tag(pskb);
//DumpPacketvlan("vlan hybird down",*pskb);
                    
                    return VLAN_OK;
                }
                if( !vlan_is_hybird_vlan(mode, PKT_VLAN_ID(vhdr)) )
                {
                    return VLAN_DROP;
                }  
            }
            else
                return VLAN_DROP;
            break;
                
        default:
            return VLAN_ERR;
    }
    
//DumpPacketvlan("vlan down",*pskb);

    return VLAN_OK;
}

unsigned int vlan_forward(struct sk_buff **pskb,
				      const struct net_device *in,
				      const struct net_device *out)
{
    VLAN_M_DBG;
    if(PORT_IS_WAN(out))
    {
        return (vlan_upstream(in, pskb) == VLAN_OK) ? NF_ACCEPT:NF_DROP;
    }
    else if(PORT_IS_WAN(in))
    {
        return (vlan_downstream(out, pskb) == VLAN_OK) ? NF_ACCEPT:NF_DROP;
    }		

    return NF_ACCEPT;
}


