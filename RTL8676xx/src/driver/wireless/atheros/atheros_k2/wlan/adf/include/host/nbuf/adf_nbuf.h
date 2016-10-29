/* (c) Copyright 2002 - 2008 Atheros Communications Inc. */

/**
 * @defgroup adf_nbuf_public network buffer API
 */ 

/**
 * @ingroup adf_nbuf_public
 * @file adf_nbuf.h
 * This file defines the network buffer abstraction.
 */ 

#ifndef _ADF_HST_NBUF_H
#define _ADF_HST_NBUF_H

#include <adf_os_types.h>
#include <adf_net_types.h>
#include <common/nbuf/adf_nbuf.h>
#include <host/nbuf/adf_nbuf_pvt.h>


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
static adf_os_inline adf_nbuf_t
adf_nbuf_expand(adf_nbuf_t buf, a_uint32_t headroom, a_uint32_t tailroom)
{
    return (__adf_nbuf_expand(buf,headroom,tailroom));
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
static adf_os_inline adf_nbuf_t
adf_nbuf_copy(adf_nbuf_t buf)
{
    return(__adf_nbuf_copy(buf));
}




/**
 * @brief clone the nbuf (copy is readonly)
 * 
 * @param[in] buf nbuf to clone from
 * 
 * @return cloned buffer
 */
static adf_os_inline adf_nbuf_t 
adf_nbuf_clone(adf_nbuf_t buf)
{
    return(__adf_nbuf_clone(buf));
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
adf_nbuf_tx_cksum_info(adf_nbuf_t buf, a_uint8_t **hdr_off, a_uint8_t **where)
{
    return(__adf_nbuf_tx_cksum_info(buf, hdr_off, where));
}


/**
 * @brief Drivers that support hw checksumming use this to
 *        indicate checksum info to the stack.
 * 
 * @param[in]  buf      buffer
 * @param[in]  cksum    checksum
 */
static adf_os_inline a_status_t
adf_nbuf_set_rx_cksum(adf_nbuf_t buf, adf_nbuf_rx_cksum_t *cksum)
{
    return __adf_nbuf_set_rx_cksum(buf, cksum);
}


/**
 * @brief Drivers that are capable of TCP Large segment offload
 *        use this to get the offload info out of an buf.
 * 
 * @param[in]  buf  buffer
 * @param[out] tso  offload info
 */
static adf_os_inline a_status_t 
adf_nbuf_get_tso_info(adf_nbuf_t buf, adf_nbuf_tso_t *tso)
{
    return __adf_nbuf_get_tso_info(buf, tso);
}


/*static adf_os_inline void
adf_nbuf_set_vlan_info(adf_nbuf_t buf, adf_net_vlan_tag_t vlan_tag)
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
adf_nbuf_get_vlan_info(adf_net_handle_t hdl, adf_nbuf_t buf, 
                       adf_net_vlanhdr_t *vlan)
{
    return __adf_nbuf_get_vlan_info(hdl, buf, vlan);
}
#endif
