#ifndef _ADF_CMN_NET_PVT_BUF_H
#define _ADF_CMN_NET_PVT_BUF_H


typedef void * __adf_nbuf_t;
#define __ADF_NBUF_NULL   NULL
typedef void * __adf_nbuf_queue_t;

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
__adf_nbuf_dmamap_create(adf_os_device_t osdev,
                       adf_os_dma_map_t *dmap)
{
    return A_STATUS_OK;
}


/**
 * @brief Delete a dmap map
 * 
 * @param[in] osdev os device
 * @param[in] dmap
 */
static adf_os_inline void
__adf_nbuf_dmamap_destroy(adf_os_device_t osdev, adf_os_dma_map_t dmap)
{
    return;
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
__adf_nbuf_map(adf_os_device_t        osdev, 
             adf_os_dma_map_t       bmap, 
             __adf_nbuf_t             buf, 
             adf_os_dma_dir_t       dir)
{
    return A_STATUS_OK;
}


/**
 * @brief Unmap a previously mapped buf
 *
 * @param[in] osdev   os device
 * @param[in] bmap    map handle
 * @param[in] dir     DMA direction
 */
static adf_os_inline void
__adf_nbuf_unmap(adf_os_device_t      osdev, 
               adf_os_dma_map_t     bmap, 
               adf_os_dma_dir_t     dir)
{
    return;
}

/**
 * @brief returns information about the mapped buf
 * 
 * @param[in]  bmap map handle
 * @param[out] sg   map info
 */
static adf_os_inline void
__adf_nbuf_dmamap_info(adf_os_dma_map_t bmap, adf_os_dmamap_info_t *sg)
{
    return;
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
static adf_os_inline __adf_nbuf_t 
__adf_nbuf_alloc(adf_os_size_t        size,
               int                  reserve,
               int                  align)
{
    return NULL;
}


/**
 * @brief Free adf_nbuf
 *
 * @param[in] buf buffer to free
 */
static adf_os_inline void
__adf_nbuf_free(__adf_nbuf_t buf)
{
    return;
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
static adf_os_inline __adf_nbuf_t
__adf_nbuf_realloc_headroom(__adf_nbuf_t buf, a_uint32_t headroom)
{
    return NULL;
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
static adf_os_inline __adf_nbuf_t
__adf_nbuf_realloc_tailroom(__adf_nbuf_t buf, a_uint32_t tailroom)
{
    return NULL;
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
static adf_os_inline __adf_nbuf_t 
__adf_nbuf_unshare(__adf_nbuf_t buf)
{
    return NULL;
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
__adf_nbuf_headroom(__adf_nbuf_t buf)
{
    return A_STATUS_OK;
}


/**
 * @brief return the amount of tail space available
 * 
 * @param[in] buf   buffer
 * 
 * @return amount of tail room 
 */
static adf_os_inline a_uint32_t
__adf_nbuf_tailroom(__adf_nbuf_t buf)
{
    return A_STATUS_OK;
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
__adf_nbuf_push_head(__adf_nbuf_t buf, adf_os_size_t size)
{
    return NULL;
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
__adf_nbuf_put_tail(__adf_nbuf_t buf, adf_os_size_t size)
{
    return NULL;
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
__adf_nbuf_pull_head(__adf_nbuf_t buf, adf_os_size_t size)
{
    return NULL;
}

/**
 * @brief Get the length of the buf
 *
 * @param[in] buf the buf instance
 *
 * @return The total length of this buf.
 */
static adf_os_inline adf_os_size_t
__adf_nbuf_len(__adf_nbuf_t buf)
{
    return A_STATUS_OK;
}

/**
 * @brief test whether the nbuf is cloned or not
 * 
 * @param[in] buf   buffer
 * 
 * @return TRUE if it is cloned, else FALSE
 */
static adf_os_inline a_bool_t
__adf_nbuf_is_cloned(__adf_nbuf_t buf)
{
    return A_STATUS_OK;
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
__adf_nbuf_frag_info(__adf_nbuf_t buf, adf_os_sglist_t *sg) 
{
    return;
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
__adf_nbuf_peek_header(__adf_nbuf_t buf, a_uint8_t **addr, a_uint32_t *len)
{
    return;
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
__adf_nbuf_get_priv(__adf_nbuf_t buf)
{
    return NULL;
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
__adf_nbuf_queue_init(__adf_nbuf_queue_t *head)
{
    return A_STATUS_OK;
}


/**
 * @brief Append a nbuf to the tail of the buf queue
 *
 * @param[in] head  buf queue head
 * @param[in] buf   buf
 */
static adf_os_inline void
__adf_nbuf_queue_add(__adf_nbuf_queue_t *head, __adf_nbuf_t buf)
{
    return;
}


/**
 * @brief Retrieve a buf from the head of the buf queue
 *
 * @param[in] head    buf queue head
 *
 * @return The head buf in the buf queue.
 */
static adf_os_inline __adf_nbuf_t
__adf_nbuf_queue_remove(__adf_nbuf_queue_t *head)
{
    return NULL;
}


/**
 * @brief get the length of the queue
 * 
 * @param[in] head  buf queue head
 * 
 * @return length of the queue
 */
static adf_os_inline a_uint32_t
__adf_nbuf_queue_len(__adf_nbuf_queue_t *head)
{
    return A_STATUS_OK;
}


/**
 * @brief get the first guy/packet in the queue
 * 
 * @param[in] head  buf queue head
 * 
 * @return first buffer in queue
 */
static adf_os_inline __adf_nbuf_t 
__adf_nbuf_queue_first(__adf_nbuf_queue_t *head)
{
    return NULL;
}


/**
 * @brief get the next guy/packet of the given buffer (or
 *        packet)
 * 
 * @param[in] buf   buffer
 * 
 * @return next buffer/packet
 */
static adf_os_inline __adf_nbuf_t
__adf_nbuf_queue_next(__adf_nbuf_t buf)
{
    return NULL;
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
__adf_nbuf_is_queue_empty(__adf_nbuf_queue_t * nbq)
{
    return A_STATUS_OK;
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
__adf_nbuf_trim_tail(__adf_nbuf_t buf, adf_os_size_t size)
{
    return;
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
__adf_nbuf_cat(__adf_nbuf_t dst,__adf_nbuf_t src)
{
    return A_STATUS_OK;
}

#endif /*_adf_nbuf_PVT_H */
