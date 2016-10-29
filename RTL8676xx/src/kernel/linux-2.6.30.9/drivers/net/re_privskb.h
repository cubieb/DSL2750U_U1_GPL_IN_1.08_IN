/*	
 *	re_privskb.h
*/

#ifndef _RE_PRIVSKB_H_
#define _RE_PRIVSKB_H_
#include <linux/skbuff.h>
#include "re867x.h"

#define MBUF_LEN	1600
#define CROSS_LAN_MBUF_LEN		(MBUF_LEN+16)

#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
#ifdef DELAY_REFILL_ETH_RX_BUF
#define MAX_PRE_ALLOC_RX_SKB	64
#define MAX_ETH_SKB_NUM	(RE8670_RX_RING_SIZE + MAX_PRE_ALLOC_RX_SKB + 600)
#else
#define MAX_PRE_ALLOC_RX_SKB	160
#define MAX_ETH_SKB_NUM	(RE8670_RX_RING_SIZE + MAX_PRE_ALLOC_RX_SKB + 400)
#endif  //end DELAY_REFILL_ETH_RX_BUF

#define ETH_PRIV_SKB_PROC	1
#endif  //end CONFIG_RTL865X_ETH_PRIV_SKB

#ifdef RTK_QUE
struct ring_que {
	int qlen;
	int qmax;	
	int head;
	int tail;
	struct sk_buff *ring[MAX_PRE_ALLOC_RX_SKB+1];
};
 static struct ring_que rx_skb_queue;
#else
 static struct sk_buff_head rx_skb_queue; 
#endif  //end RTK_QUE


void init_priv_eth_skb_buf(void);
struct sk_buff *dev_alloc_skb_priv_eth(unsigned int size);

#endif /*_RE_PRIVSKB_H_*/
