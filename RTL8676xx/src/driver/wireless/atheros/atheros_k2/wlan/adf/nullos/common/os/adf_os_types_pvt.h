#ifndef _ADF_CMN_OS_TYPES_PVT_H
#define _ADF_CMN_OS_TYPES_PVT_H

#define NULL 0			//TODO: Remove this.

typedef unsigned char   __a_uint8_t;
typedef signed char     __a_int8_t;
typedef unsigned short  __a_uint16_t;
typedef unsigned short  __a_int16_t;
typedef unsigned int    __a_uint32_t;
typedef signed int      __a_int32_t;
typedef __int64             __a_int64_t;
typedef unsigned __int64    __a_uint64_t;
typedef int             size_t;

#define __adf_os_ull(_num)  _num ## UI64

#define __ADF_OS_MAX_SCATTER 10 /* Arbitary number */

#define __ADF_OS_NAME_SIZE 16 /* Arbitary value */

/**
 * @brief denotes structure is packed.
 */ 
#define __adf_os_packed_pre     
#define __adf_os_packed_post     

/**
 * @brief Platform/bus generic handle. Used for bus specific functions.
 */
typedef void * __adf_os_device_t;

/**
 * @brief size of an object
 */
typedef __a_uint32_t __adf_os_size_t;

/**
 * @brief offset for API's that need them.
 */
typedef __a_uint32_t __adf_os_off_t;

/**
 * @brief DMA mapping object.
 */ 
typedef void * __adf_os_dma_map_t;

/**
 * @brief DMA address.
 */ 
typedef __a_uint32_t __adf_os_dma_addr_t;

/**
 * @brief DMA size.
 */ 
typedef __a_uint32_t __adf_os_dma_size_t;


/**
 *  @brief All operations specified below are performed from
 *  the host memory point of view, where a read
 *  implies data coming from the device to the host
 *  memory, and a write implies data going from the
 *  host memory to the device.  Alternately, the
 *  operations can be thought of in terms of driver
 *  operations, where reading a network packet or
 *  storage sector corresponds to a read operation in
 *  bus_dma.
 * 
 *  ADF_SYNC_PREREAD       Perform any synchronization
 *                         required prior to an update
 *                         of host memory by the DMA
 *                         read operation.
 *  ADF_SYNC_PREWRITE      Perform any synchronization
 *                         required after an update of
 *                         host memory by the CPU and
 *                         prior to DMA write
 *                         operations.
 *  ADF_SYNC_POSTREAD      Perform any synchronization
 *                         required after DMA read
 *                         operations and prior to CPU
 *                         access to host
 *                         memory.
 *  ADF_SYNC_POSTWRITE     Perform any synchronization
 *                         required after DMA write
 *                                   operations.
 */

typedef enum {
	__ADF_SYNC_PREREAD,
    __ADF_SYNC_PREWRITE,
    __ADF_SYNC_POSTREAD,
    __ADF_SYNC_POSTWRITE
};

typedef void * __adf_os_iomem_t;

/**
 * @brief DMA directions
 *        ADF_OS_DMA_TO_DEVICE (data going from device to memory)
 *        ADF_OS_DMA_FROM_DEVICE (data going from memory to device)
 */
typedef enum {
    __ADF_OS_DMA_TO_DEVICE, 
    __ADF_OS_DMA_FROM_DEVICE, 
};

#define __adf_os_print
#define __adf_os_snprint
#define __adf_os_vsnprint

typedef enum {
    __ADF_IEEE80211_ASSOC,
    __ADF_IEEE80211_REASSOC,
    __ADF_IEEE80211_DISASSOC,
    __ADF_IEEE80211_JOIN,
    __ADF_IEEE80211_LEAVE,
    __ADF_IEEE80211_SCAN,
    __ADF_IEEE80211_REPLAY,
    __ADF_IEEE80211_MICHAEL,
    __ADF_IEEE80211_REJOIN, 
    __ADF_CUSTOM_PUSH_BUTTON
}adf_net_wireless_event_t;

#endif /*_ADF_CMN_OS_TYPES_PVT_H*/
