/*
 * Linux implemenation of skbuf
 */
#ifndef __ADF_HST_NBUF_PVT_H
#define __ADF_HST_NBUF_PVT_H

#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/dma-mapping.h>
#include <asm/types.h>
#include <asm/scatterlist.h>


/*
 * Use sk_buff_head as the implementation of adf_nbuf_queue_t.
 * Because the queue head will most likely put in some structure,
 * we don't use pointer type as the definition.
 */


/*
 * prototypes. Implemented in adf_nbuf_pvt.c
 */
a_status_t          __adf_nbuf_set_rx_cksum(struct sk_buff *skb, 
                                        adf_nbuf_rx_cksum_t *cksum);
a_status_t      __adf_nbuf_get_vlan_info(adf_net_handle_t hdl, 
                                         struct sk_buff *skb, 
                                         adf_net_vlanhdr_t *vlan);
/*
 * adf_nbuf_pool_init() implementation - do nothing in Linux
 */
static inline a_status_t
__adf_nbuf_pool_init(adf_net_handle_t anet)
{
    return A_STATUS_OK;
}

/*
 * adf_nbuf_pool_delete() implementation - do nothing in linux
 */
#define __adf_nbuf_pool_delete(osdev)


/**
 * @brief Expand both tailroom & headroom. In case of failure
 *        release the skb.
 * 
 * @param skb
 * @param headroom
 * @param tailroom
 * 
 * @return skb or NULL
 */
static inline struct sk_buff *
__adf_nbuf_expand(struct sk_buff *skb, uint32_t headroom, uint32_t tailroom)
{
    if(likely(!pskb_expand_head(skb, headroom, tailroom, GFP_ATOMIC)))
        return skb;

    dev_kfree_skb_any(skb);
    return NULL;
}


/**
 * @brief clone the nbuf (copy is readonly)
 *
 * @param src_nbuf (nbuf to clone from)
 * @param dst_nbuf (address of the cloned nbuf)
 *
 * @return status
 * 
 * @note if GFP_ATOMIC is overkill then we can check whether its
 *       called from interrupt context and then do it or else in
 *       normal case use GFP_KERNEL
 * @example     use "in_irq() || irqs_disabled()"
 * 
 * 
 */
static inline struct sk_buff *
__adf_nbuf_clone(struct sk_buff *skb)
{   
    return skb_clone(skb, GFP_ATOMIC);
}
/**
 * @brief returns a private copy of the skb, the skb returned is
 *        completely modifiable
 * 
 * @param skb
 * 
 * @return skb or NULL
 */
static inline struct sk_buff *
__adf_nbuf_copy(struct sk_buff *skb)
{
    return skb_copy(skb, GFP_ATOMIC);
}


/***********************XXX: misc api's************************/
static inline a_bool_t
__adf_nbuf_tx_cksum_info(struct sk_buff *skb, uint8_t **hdr_off, 
                         uint8_t **where)
{
//     if (skb->ip_summed == CHECKSUM_NONE)
//         return A_FALSE;
//
//     if (skb->ip_summed == CHECKSUM_HW) {
//         *hdr_off = (uint8_t *)(skb->h.raw - skb->data);
//         *where   = *hdr_off + skb->csum;
//         return A_TRUE;
//     }

    adf_os_assert(0);
    return A_FALSE;
}


/*
 * XXX What about other unions in skb? Windows might not have it, but we
 * should penalize linux drivers for it.
 * Besides this function is not likely doint the correct thing.
 */
static inline a_status_t
__adf_nbuf_get_tso_info(struct sk_buff *skb, adf_nbuf_tso_t *tso)
{
    adf_os_assert(0);
    return A_STATUS_ENOTSUPP;
/*
    if (!skb_shinfo(skb)->tso_size) {
        tso->type = adf_net_TSO_NONE;
        return;
    }

    tso->mss      = skb_shinfo(skb)->tso_size;
*/  
//     tso->hdr_off  = (uint8_t)(skb->h.raw - skb->data);
//
//     if (skb->protocol == ntohs(ETH_P_IP))
//         tso->type =  ADF_NET_TSO_IPV4;
//     else if (skb->protocol == ntohs(ETH_P_IPV6))
//         tso->type =  ADF_NET_TSO_ALL;
//     else
//         tso->type = ADF_NET_TSO_NONE;
}




#endif /*_ADF_HST_NBUF_PVT_H */
