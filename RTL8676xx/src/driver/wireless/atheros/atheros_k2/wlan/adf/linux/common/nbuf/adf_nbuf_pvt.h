/*
 * Linux implemenation of skbuf
 */
#ifndef _ADF_CMN_NET_PVT_BUF_H
#define _ADF_CMN_NET_PVT_BUF_H

#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/dma-mapping.h>
#include <asm/types.h>
#include <asm/scatterlist.h>

#define __ADF_NBUF_NULL   NULL

/*
 * Use socket buffer as the underlying implentation as skbuf .
 * Linux use sk_buff to represent both packet and data,
 * so we use sk_buffer to represent both skbuf .
 */
typedef struct sk_buff *        __adf_nbuf_t;

typedef struct __adf_nbuf_qhead {
    struct sk_buff   *head;
    struct sk_buff   *tail;
    unsigned int     qlen;
}__adf_nbuf_queue_t;

// typedef struct sk_buff_head     __adf_nbuf_queue_t;

// struct anet_dma_info {
//     dma_addr_t  daddr;
//     uint32_t    len;
// };
//
// struct __adf_nbuf_map {
//     int                   nelem;
//     struct anet_dma_info  dma[1];
// };
//
// typedef struct __adf_nbuf_map     *__adf_nbuf_dmamap_t;


/*
 * Use sk_buff_head as the implementation of adf_nbuf_queue_t.
 * Because the queue head will most likely put in some structure,
 * we don't use pointer type as the definition.
 */


/*
 * prototypes. Implemented in adf_nbuf_pvt.c
 */
__adf_nbuf_t    __adf_nbuf_alloc(size_t size, int reserve, int align);
void            __adf_nbuf_free (struct sk_buff *skb);
a_status_t      __adf_nbuf_dmamap_create(__adf_os_device_t osdev, 
                                         __adf_os_dma_map_t *dmap);
void            __adf_nbuf_dmamap_destroy(__adf_os_device_t osdev, 
                                          __adf_os_dma_map_t dmap);
a_status_t      __adf_nbuf_map(__adf_os_device_t osdev, __adf_os_dma_map_t dmap,
                                struct sk_buff *skb, adf_os_dma_dir_t dir);
void            __adf_nbuf_unmap(__adf_os_device_t osdev, __adf_os_dma_map_t dmap,
                               adf_os_dma_dir_t dir);
void            __adf_nbuf_dmamap_info(__adf_os_dma_map_t bmap, adf_os_dmamap_info_t *sg);
void            __adf_nbuf_frag_info(struct sk_buff *skb, adf_os_sglist_t  *sg);


static inline a_status_t
__adf_os_to_status(signed int error)
{
    switch(error){
    case 0:
        return A_STATUS_OK;
    case ENOMEM:
    case -ENOMEM:
        return A_STATUS_ENOMEM;
    default:
        return A_STATUS_ENOTSUPP;
    }
}



/**
 * @brief This keeps the skb shell intact expands the headroom
 *        in the data region. In case of failure the skb is
 *        released.
 * 
 * @param skb
 * @param headroom
 * 
 * @return skb or NULL
 */
static inline struct sk_buff *
__adf_nbuf_realloc_headroom(struct sk_buff *skb, uint32_t headroom) 
{
    if(pskb_expand_head(skb, headroom, 0, GFP_ATOMIC)){
        dev_kfree_skb_any(skb);
        skb = NULL;
    }
    return skb;
}
/**
 * @brief This keeps the skb shell intact exapnds the tailroom
 *        in data region. In case of failure it releases the
 *        skb.
 * 
 * @param skb
 * @param tailroom
 * 
 * @return skb or NULL
 */
static inline struct sk_buff *
__adf_nbuf_realloc_tailroom(struct sk_buff *skb, uint32_t tailroom)
{
    if(likely(!pskb_expand_head(skb, 0, tailroom, GFP_ATOMIC)))
        return skb;
    /**
     * unlikely path
     */
    dev_kfree_skb_any(skb);
    return NULL;
}
/**
 * @brief return the amount of valid data in the skb, If there
 *        are frags then it returns total length.
 * 
 * @param skb
 * 
 * @return size_t
 */
static inline size_t
__adf_nbuf_len(struct sk_buff * skb)
{
    return skb->len;
}


/**
 * @brief link two nbufs, the new buf is piggybacked into the
 *        older one. The older (src) skb is released.
 *
 * @param dst( buffer to piggyback into)
 * @param src (buffer to put)
 *
 * @return a_status_t (status of the call) if failed the src skb
 *         is released
 */
static inline a_status_t
__adf_nbuf_cat(struct sk_buff *dst, struct sk_buff *src)
{
    a_status_t error = 0;

    adf_os_assert(dst && src);

    if(!(error = pskb_expand_head(dst, 0, src->len, GFP_ATOMIC)))
        memcpy(skb_put(dst, src->len), src->data, src->len);

    dev_kfree_skb_any(src);

    return __adf_os_to_status(error);
}
/**
 * @brief  create a version of the specified nbuf whose contents
 *         can be safely modified without affecting other
 *         users.If the nbuf is a clone then this function
 *         creates a new copy of the data. If the buffer is not
 *         a clone the original buffer is returned.
 *
 * @param skb (source nbuf to create a writable copy from)
 *
 * @return skb or NULL
 */
static inline struct sk_buff *
__adf_nbuf_unshare(struct sk_buff *skb)
{
    return skb_unshare(skb, GFP_ATOMIC);
}
/**************************nbuf manipulation routines*****************/

static inline int
__adf_nbuf_headroom(struct sk_buff *skb)
{
    return skb_headroom(skb);
}
/**
 * @brief return the amount of tail space available
 *
 * @param buf
 *
 * @return amount of tail room
 */
static inline uint32_t
__adf_nbuf_tailroom(struct sk_buff *skb)
{
    return skb_tailroom(skb);
}

/**
 * @brief Push data in the front
 *
 * @param[in] buf      buf instance
 * @param[in] size     size to be pushed
 *
 * @return New data pointer of this buf after data has been pushed,
 *         or NULL if there is not enough room in this buf.
 */
static inline uint8_t *
__adf_nbuf_push_head(struct sk_buff *skb, size_t size)
{
    return skb_push(skb, size);
}
/**
 * @brief Puts data in the end
 *
 * @param[in] buf      buf instance
 * @param[in] size     size to be pushed
 *
 * @return data pointer of this buf where new data has to be
 *         put, or NULL if there is not enough room in this buf.
 */
static inline uint8_t *
__adf_nbuf_put_tail(struct sk_buff *skb, size_t size)
{
   return skb_put(skb, size);
}

/**
 * @brief pull data out from the front
 *
 * @param[in] buf   buf instance
 * @param[in] size  size to be popped
 *
 * @return New data pointer of this buf after data has been popped,
 *         or NULL if there is not sufficient data to pull.
 */
static inline uint8_t *
__adf_nbuf_pull_head(struct sk_buff *skb, size_t size)
{
    return skb_pull(skb, size);
}
/**
 *
 * @brief trim data out from the end
 *
 * @param[in] buf   buf instance
 * @param[in] size     size to be popped
 *
 * @return none
 */
static inline void
__adf_nbuf_trim_tail(struct sk_buff *skb, size_t size)
{
    return skb_trim(skb, skb->len - size);
}

/**
 * @brief test whether the nbuf is cloned or not
 *
 * @param buf
 *
 * @return a_bool_t (TRUE if it is cloned or else FALSE)
 */
static inline a_bool_t
__adf_nbuf_is_cloned(struct sk_buff *skb)
{
        return skb_cloned(skb);
}


/*********************nbuf private buffer routines*************/

/**
 * @brief get the priv pointer from the nbuf'f private space
 *
 * @param buf
 *
 * @return data pointer to typecast into your priv structure
 */
static inline uint8_t *
__adf_nbuf_get_priv(struct sk_buff *skb)
{
        return &skb->cb[8];
}

/**
 * @brief This will return the header's addr & m_len
 */
static inline void
__adf_nbuf_peek_header(struct sk_buff *skb, uint8_t   **addr, 
                       uint32_t    *len)
{
    *addr = skb->data;
    *len  = skb->len;
}

 /* adf_nbuf_queue_init() - initialize a skbuf queue
 */
/* static inline void */
/* __adf_nbuf_queue_init(struct sk_buff_head *qhead) */
/* { */
/*     skb_queue_head_init(qhead); */
/* } */

/* /\* */
/*  * adf_nbuf_queue_add() - add a skbuf to the end of the skbuf queue */
/*  * */
/*  * We use the non-locked version because */
/*  * there's no need to use the irq safe version of spinlock. */
/*  * However, the caller has to do synchronization by itself. */
/*  *\/ */
/* static inline void */
/* __adf_nbuf_queue_add(struct sk_buff_head *qhead,  */
/*                      struct sk_buff *skb) */
/* { */
/*     __skb_queue_tail(qhead, skb); */
/*     adf_os_assert(qhead->next == qhead->prev); */
/* } */

/* /\* */
/*  * adf_nbuf_queue_remove() - remove a skbuf from the head of the skbuf queue */
/*  * */
/*  * We use the non-locked version because */
/*  * there's no need to use the irq safe version of spinlock. */
/*  * However, the caller has to do synchronization by itself. */
/*  *\/ */
/* static inline struct sk_buff * */
/* __adf_nbuf_queue_remove(struct sk_buff_head * qhead) */
/* { */
/*     adf_os_assert(qhead->next == qhead->prev); */
/*     return __skb_dequeue(qhead); */
/* } */

/* static inline uint32_t */
/* __adf_nbuf_queue_len(struct sk_buff_head * qhead) */
/* { */
/*         adf_os_assert(qhead->next == qhead->prev); */
/*         return qhead->qlen; */
/* } */
/* /\** */
/*  * @brief returns the first guy in the Q */
/*  * @param qhead */
/*  *  */
/*  * @return (NULL if the Q is empty) */
/*  *\/ */
/* static inline struct sk_buff *    */
/* __adf_nbuf_queue_first(struct sk_buff_head *qhead) */
/* { */
/*    adf_os_assert(qhead->next == qhead->prev); */
/*    return (skb_queue_empty(qhead) ? NULL : qhead->next); */
/* } */
/* /\** */
/*  * @brief return the next packet from packet chain */
/*  *  */
/*  * @param buf (packet) */
/*  *  */
/*  * @return (NULL if no packets are there) */
/*  *\/ */
/* static inline struct sk_buff *    */
/* __adf_nbuf_queue_next(struct sk_buff *skb) */
/* { */
/*   return NULL; // skb->next; */
/* } */
/* /\** */
/*  * adf_nbuf_queue_empty() - check if the skbuf queue is empty */
/*  *\/ */
/* static inline a_bool_t */
/* __adf_nbuf_is_queue_empty(struct sk_buff_head *qhead) */
/* { */
/*     adf_os_assert(qhead->next == qhead->prev); */
/*     return skb_queue_empty(qhead); */
/* } */

/******************Custom queue*************/



/**
 * @brief initiallize the queue head
 * 
 * @param qhead
 */
static inline a_status_t
__adf_nbuf_queue_init(__adf_nbuf_queue_t *qhead)
{
    memset(qhead, 0, sizeof(struct __adf_nbuf_qhead));
    return A_STATUS_OK;
}


/**
 * @brief add an skb in the tail of the queue. This is a
 *        lockless version, driver must acquire locks if it
 *        needs to synchronize
 * 
 * @param qhead
 * @param skb
 */
static inline void
__adf_nbuf_queue_add(__adf_nbuf_queue_t *qhead, 
                     struct sk_buff *skb)
{
    skb->next = NULL;/*Nullify the next ptr*/

    if(!qhead->head)
        qhead->head = skb;
    else
        qhead->tail->next = skb;
    
    qhead->tail = skb;
    qhead->qlen++;
}

/**
 * @brief remove a skb from the head of the queue, this is a
 *        lockless version. Driver should take care of the locks
 * 
 * @param qhead
 * 
 * @return skb or NULL
 */
static inline struct sk_buff *
__adf_nbuf_queue_remove(__adf_nbuf_queue_t * qhead)
{
    struct sk_buff  *tmp;

    if((tmp = qhead->head) == NULL)     
        return NULL;/*Q was empty already*/
    
    qhead->head = tmp->next;/*XXX:tail still points to tmp*/
    qhead->qlen--;
    tmp->next   = NULL;/*remove any references to the Q for the tmp*/
    
    return tmp;
}
/**
 * @brief return the queue length
 * 
 * @param qhead
 * 
 * @return uint32_t
 */
static inline uint32_t
__adf_nbuf_queue_len(__adf_nbuf_queue_t * qhead)
{
        return qhead->qlen;
}
/**
 * @brief returns the first skb in the queue
 * 
 * @param qhead
 * 
 * @return (NULL if the Q is empty)
 */
static inline struct sk_buff *   
__adf_nbuf_queue_first(__adf_nbuf_queue_t *qhead)
{
    return qhead->head;
}
/**
 * @brief return the next skb from packet chain, remember the
 *        skb is still in the queue
 * 
 * @param buf (packet)
 * 
 * @return (NULL if no packets are there)
 */
static inline struct sk_buff *   
__adf_nbuf_queue_next(struct sk_buff *skb)
{
    return skb->next;
}
/**
 * @brief check if the queue is empty or not
 * 
 * @param qhead
 * 
 * @return a_bool_t
 */
static inline a_bool_t
__adf_nbuf_is_queue_empty(__adf_nbuf_queue_t *qhead)
{
    return (qhead->qlen == 0);
}


#endif /*_adf_nbuf_PVT_H */
