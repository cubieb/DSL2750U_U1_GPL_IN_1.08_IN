/*
 * Linux implemenation of skbuf
 */
#ifndef __ADF_HST_NBUF_PVT_H
#define __ADF_HST_NBUF_PVT_H

/**
 * @brief this will expand both tail & head room for a given
 *        buffer, you may or may not get a new buffer.Use it
 *        only when its required to expand both. Otherwise use
 *        realloc (head/tail) will solve the purpose. Reason for
 *        having an extra API is that some OS do this in more
 *        optimized way, rather than calling realloc (head/tail)
 *        back to back.
 * 
 * @param[in] buf       buffer
 * @param[in] headroom  new headroom  
 * @param[in] tailroom  new tailroom
 * 
 * @return expanded buffer
 */
static adf_os_inline __adf_nbuf_t
__adf_nbuf_expand(__adf_nbuf_t buf, a_uint32_t headroom, a_uint32_t tailroom)
{
    return NULL;
}


/**
 * @brief Copy src buffer into dst. This API is useful, for
 *        example, because most native buffer provide a way to
 *        copy a chain into a single buffer. Therefore as a side
 *        effect, it also "linearizes" a buffer (which is
 *        perhaps why you'll use it mostly). It creates a
 *        writeable copy.
 * 
 * @param[in] buf source nbuf to copy from
 * 
 * @return the new nbuf
 */
static adf_os_inline __adf_nbuf_t
__adf_nbuf_copy(__adf_nbuf_t buf)
{
    return NULL;
}




/**
 * @brief clone the nbuf (copy is readonly)
 * 
 * @param[in] buf nbuf to clone from
 * 
 * @return cloned buffer
 */
static adf_os_inline __adf_nbuf_t 
__adf_nbuf_clone(__adf_nbuf_t buf)
{
    return NULL;
}



/*
 * nbuf extension routines XXX
 */



/**
 * @brief Gets the tx checksumming to be performed on this buf
 * 
 * @param[in]  buf       buffer
 * @param[out] hdr_off   the (tcp) header start
 * @param[out] where     the checksum offset
 */
static adf_os_inline adf_net_cksum_type_t
__adf_nbuf_tx_cksum_info(__adf_nbuf_t buf, a_uint8_t **hdr_off, a_uint8_t **where)
{
    return 0;
}


/**
 * @brief Drivers that support hw checksumming use this to
 *        indicate checksum info to the stack.
 * 
 * @param[in]  buf      buffer
 * @param[in]  cksum    checksum
 */
static adf_os_inline a_status_t
__adf_nbuf_set_rx_cksum(__adf_nbuf_t buf, adf_nbuf_rx_cksum_t *cksum)
{
    return A_STATUS_OK;
}


/**
 * @brief Drivers that are capable of TCP Large segment offload
 *        use this to get the offload info out of an buf.
 * 
 * @param[in]  buf  buffer
 * @param[out] tso  offload info
 */
static adf_os_inline a_status_t 
__adf_nbuf_get_tso_info(__adf_nbuf_t buf, adf_nbuf_tso_t *tso)
{
    return A_STATUS_OK;
}


/*static adf_os_inline void
adf_nbuf_set_vlan_info(__adf_nbuf_t buf, adf_net_vlan_tag_t vlan_tag)
{
    __adf_nbuf_set_vlan_info(buf, vlan_tag);
}*/

/**
 * @brief This function extracts the vid & priority from an
 *        nbuf
 * 
 * 
 * @param[in] hdl   net handle
 * @param[in] buf   buffer
 * @param[in] vlan  vlan header
 * 
 * @return status of the operation
 */
static adf_os_inline a_status_t
__adf_nbuf_get_vlan_info(adf_net_handle_t hdl, __adf_nbuf_t buf, 
                       adf_net_vlanhdr_t *vlan)
{
    return A_STATUS_OK;
}

#endif /*_ADF_HST_NBUF_PVT_H */
