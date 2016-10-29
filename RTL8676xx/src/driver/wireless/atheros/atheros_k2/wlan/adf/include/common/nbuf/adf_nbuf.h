/* (c) Copyright 2002 - 2008 Atheros Communications Inc. */

/**
 * @defgroup adf_nbuf_public network buffer API
 */ 

/**
 * @ingroup adf_nbuf_public
 * @file adf_nbuf.h
 * This file defines the network buffer abstraction.
 */ 

#ifndef _ADF_CMN_NBUF_H
#define _ADF_CMN_NBUF_H

#include <adf_os_util.h>
#include <adf_os_types.h>
#include <adf_os_dma.h>
#include <common/nbuf/adf_nbuf_pvt.h>

/**
 * @brief Platform indepedent packet abstraction
 */
typedef __adf_nbuf_t         adf_nbuf_t;

/**
 * @brief invalid handle
 */
#define ADF_NBUF_NULL   __ADF_NBUF_NULL
/**
 * @brief Platform independent packet queue abstraction
 */
typedef __adf_nbuf_queue_t   adf_nbuf_queue_t;

/**
 * BUS/DMA mapping routines
 */

/**
 * @brief Create a DMA map. This can later be used to map
 *        networking buffers. They :
 *          - need space in adf_drv's software descriptor
 *          - are typically created during adf_drv_create
 *          - need to be created before any API(adf_nbuf_map) that uses them
 * 
 * @param[in]  osdev os device
 * @param[out] dmap  map handle
 * 
 * @return status of the operation
 */
static adf_os_inline a_status_t
adf_nbuf_dmamap_create(adf_os_device_t osdev,
                       adf_os_dma_map_t *dmap)
{
    return (__adf_nbuf_dmamap_create(osdev, dmap));
}


/**
 * @brief Delete a dmap map
 * 
 * @param[in] osdev os device
 * @param[in] dmap
 */
static adf_os_inline void
adf_nbuf_dmamap_destroy(adf_os_device_t osdev, adf_os_dma_map_t dmap)
{
    __adf_nbuf_dmamap_destroy(osdev, dmap);
}


/**
 * @brief Map a buffer to local bus address space
 *
 * @param[in]  osdev   os device
 * @param[in]  bmap    map handle
 * @param[in]  buf     buf to be mapped
 * @param[in]  dir     DMA direction
 *
 * @return status of the operation
 */
static adf_os_inline a_status_t
adf_nbuf_map(adf_os_device_t        osdev, 
             adf_os_dma_map_t       bmap, 
             adf_nbuf_t             buf, 
             adf_os_dma_dir_t       dir)
{
    return __adf_nbuf_map(osdev, bmap, buf, dir);
}


/**
 * @brief Unmap a previously mapped buf
 *
 * @param[in] osdev   os device
 * @param[in] bmap    map handle
 * @param[in] dir     DMA direction
 */
static adf_os_inline void
adf_nbuf_unmap(adf_os_device_t      osdev, 
               adf_os_dma_map_t     bmap, 
               adf_os_dma_dir_t     dir)
{
    __adf_nbuf_unmap(osdev, bmap, dir);
}

/**
 * @brief returns information about the mapped buf
 * 
 * @param[in]  bmap map handle
 * @param[out] sg   map info
 */
static adf_os_inline void
adf_nbuf_dmamap_info(adf_os_dma_map_t bmap, adf_os_dmamap_info_t *sg)
{
    __adf_nbuf_dmamap_info(bmap, sg);
}



/*
 * nbuf allocation rouines
 */


/**
 * @brief Allocate adf_nbuf
 *
 * The nbuf created is guarenteed to have only 1 physical segment
 *
 * @param[in] hdl   platform device object
 * @param[in] size  data buffer size for this adf_nbuf including max header 
 *                  size
 * @param[in] reserve  headroom to start with.
 * @param[in] align    alignment for the start buffer.
 *
 * @return The new adf_nbuf instance or NULL if there's not enough memory.
 */
static adf_os_inline adf_nbuf_t 
adf_nbuf_alloc(adf_os_size_t        size,
               int                  reserve,
               int                  align)
{
    return __adf_nbuf_alloc(size, reserve,align);
}


/**
 * @brief Free adf_nbuf
 *
 * @param[in] buf buffer to free
 */
static adf_os_inline void
adf_nbuf_free(adf_nbuf_t buf)
{
    __adf_nbuf_free(buf);
}


/**
 * @brief Reallocate such that there's required headroom in
 *        buf. Note that this can allocate a new buffer, or
 *        change geometry of the orignial buffer. The new buffer
 *        is returned in the (new_buf).
 * 
 * @param[in] buf (older buffer)
 * @param[in] headroom
 * 
 * @return newly allocated buffer
 */
static adf_os_inline adf_nbuf_t
adf_nbuf_realloc_headroom(adf_nbuf_t buf, a_uint32_t headroom)
{
    return (__adf_nbuf_realloc_headroom(buf, headroom));
}


/**
 * @brief expand the tailroom to the new tailroom, but the buffer
 *        remains the same
 * 
 * @param[in] buf       buffer
 * @param[in] tailroom  new tailroom
 * 
 * @return expanded buffer or NULL on failure
 */
static adf_os_inline adf_nbuf_t
adf_nbuf_realloc_tailroom(adf_nbuf_t buf, a_uint32_t tailroom)
{
    return (__adf_nbuf_realloc_tailroom(buf, tailroom));
}

/**
 * @brief  Create a version of the specified nbuf whose
 *         contents can be safely modified without affecting
 *         other users.If the nbuf is a clone then this function
 *         creates a new copy of the data. If the buffer is not
 *         a clone the original buffer is returned.
 * 
 * @param[in] buf   source nbuf to create a writable copy from
 * 
 * @return new buffer which is writeable
 */
static adf_os_inline adf_nbuf_t 
adf_nbuf_unshare(adf_nbuf_t buf)
{
    return(__adf_nbuf_unshare(buf));
}



/*
 * nbuf manipulation routines
 */



/**
 * @brief return the amount of headroom int the current nbuf
 * 
 * @param[in] buf   buffer
 * 
 * @return amount of head room
 */
static adf_os_inline a_uint32_t
adf_nbuf_headroom(adf_nbuf_t buf)
{
    return (__adf_nbuf_headroom(buf));
}


/**
 * @brief return the amount of tail space available
 * 
 * @param[in] buf   buffer
 * 
 * @return amount of tail room 
 */
static adf_os_inline a_uint32_t
adf_nbuf_tailroom(adf_nbuf_t buf)
{
    return (__adf_nbuf_tailroom(buf));
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
static adf_os_inline a_uint8_t *
adf_nbuf_push_head(adf_nbuf_t buf, adf_os_size_t size)
{
    return __adf_nbuf_push_head(buf, size);
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
static adf_os_inline a_uint8_t *
adf_nbuf_put_tail(adf_nbuf_t buf, adf_os_size_t size)
{
    return __adf_nbuf_put_tail(buf, size);
}


/**
 * @brief pull data out from the front
 *
 * @param[in] buf   buf instance
 * @param[in] size     size to be popped
 *
 * @return New data pointer of this buf after data has been popped,
 *         or NULL if there is not sufficient data to pull.
 */
static adf_os_inline a_uint8_t *
adf_nbuf_pull_head(adf_nbuf_t buf, adf_os_size_t size)
{
    return __adf_nbuf_pull_head(buf, size);
}

/**
 * @brief Get the length of the buf
 *
 * @param[in] buf the buf instance
 *
 * @return The total length of this buf.
 */
static adf_os_inline adf_os_size_t
adf_nbuf_len(adf_nbuf_t buf)
{
    return (__adf_nbuf_len(buf));
}

/**
 * @brief test whether the nbuf is cloned or not
 * 
 * @param[in] buf   buffer
 * 
 * @return TRUE if it is cloned, else FALSE
 */
static adf_os_inline a_bool_t
adf_nbuf_is_cloned(adf_nbuf_t buf)
{
    return (__adf_nbuf_is_cloned(buf));
}



/*
 * nbuf frag routines
 */

/**
 * @brief return the frag pointer & length of the frag
 * 
 * @param[in]  buf   buffer
 * @param[out] sg    this will return all the frags of the nbuf
 * 
 */
static adf_os_inline void 
adf_nbuf_frag_info(adf_nbuf_t buf, adf_os_sglist_t *sg) 
{
    __adf_nbuf_frag_info(buf, sg);
}
/**
 * @brief return the data pointer & length of the header
 * 
 * @param[in]  buf  nbuf
 * @param[out] addr data pointer
 * @param[out] len  length of the data
 *
 */
static adf_os_inline void
adf_nbuf_peek_header(adf_nbuf_t buf, a_uint8_t **addr, a_uint32_t *len)
{
    __adf_nbuf_peek_header(buf, addr, len);
}
/*
 * nbuf private context routines
 */

/**
 * @brief get the priv pointer from the nbuf'f private space
 * 
 * @param[in] buf
 * 
 * @return data pointer to typecast into your priv structure
 */
static adf_os_inline a_uint8_t *
adf_nbuf_get_priv(adf_nbuf_t buf)
{
    return (__adf_nbuf_get_priv(buf));
}


/*
 * nbuf queue routines
 */


/**
 * @brief Initialize buf queue
 *
 * @param[in] head  buf queue head
 */
static adf_os_inline a_status_t
adf_nbuf_queue_init(adf_nbuf_queue_t *head)
{
    return __adf_nbuf_queue_init(head);
}


/**
 * @brief Append a nbuf to the tail of the buf queue
 *
 * @param[in] head  buf queue head
 * @param[in] buf   buf
 */
static adf_os_inline void
adf_nbuf_queue_add(adf_nbuf_queue_t *head, adf_nbuf_t buf)
{
    __adf_nbuf_queue_add(head, buf);
}


/**
 * @brief Retrieve a buf from the head of the buf queue
 *
 * @param[in] head    buf queue head
 *
 * @return The head buf in the buf queue.
 */
static adf_os_inline adf_nbuf_t
adf_nbuf_queue_remove(adf_nbuf_queue_t *head)
{
    return __adf_nbuf_queue_remove(head);
}


/**
 * @brief get the length of the queue
 * 
 * @param[in] head  buf queue head
 * 
 * @return length of the queue
 */
static adf_os_inline a_uint32_t
adf_nbuf_queue_len(adf_nbuf_queue_t *head)
{
    return __adf_nbuf_queue_len(head);
}


/**
 * @brief get the first guy/packet in the queue
 * 
 * @param[in] head  buf queue head
 * 
 * @return first buffer in queue
 */
static adf_os_inline adf_nbuf_t 
adf_nbuf_queue_first(adf_nbuf_queue_t *head)
{
    return (__adf_nbuf_queue_first(head));
}


/**
 * @brief get the next guy/packet of the given buffer (or
 *        packet)
 * 
 * @param[in] buf   buffer
 * 
 * @return next buffer/packet
 */
static adf_os_inline adf_nbuf_t
adf_nbuf_queue_next(adf_nbuf_t buf)
{
    return (__adf_nbuf_queue_next(buf));
}

/**
 * @brief Check if the buf queue is empty
 * 
 * @param[in] nbq   buf queue handle
 * 
 * @return    TRUE  if queue is empty
 * @return    FALSE if queue is not emty
 */
static adf_os_inline a_bool_t
adf_nbuf_is_queue_empty(adf_nbuf_queue_t * nbq)
{
    return __adf_nbuf_is_queue_empty(nbq);
}


/**
 * @brief trim data out from the end
 * 
 * @param[in] buf   buf instance
 * @param[in] size     size to be popped
 *       
 * @return none
 */
static adf_os_inline void
adf_nbuf_trim_tail(adf_nbuf_t buf, adf_os_size_t size)
{
    if(size < 0) adf_os_print("trim with size<0\n");
    __adf_nbuf_trim_tail(buf, size);
}


/**
 * @brief link two nbufs, the new buf is piggybacked into the
 *        older one.
 * 
 * @param[in] dst   buffer to piggyback into
 * @param[in] src   buffer to put
 *
 * @return status of the call
 */  
static adf_os_inline a_status_t
adf_nbuf_cat(adf_nbuf_t dst,adf_nbuf_t src)
{
    return __adf_nbuf_cat(dst, src);
}


#endif
