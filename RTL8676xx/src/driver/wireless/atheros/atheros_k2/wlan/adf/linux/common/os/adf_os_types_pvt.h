#ifndef _ADF_CMN_OS_TYPES_PVT_H
#define _ADF_CMN_OS_TYPES_PVT_H

#include <asm/types.h>
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/dma-mapping.h>
#include <linux/wireless.h>
#include <asm/byteorder.h>
#include <linux/if.h>

/**
 * max sg that we support
 */
#define __ADF_OS_MAX_SCATTER        1
#define __ADF_OS_NAME_SIZE          IFNAMSIZ

#if defined(__LITTLE_ENDIAN_BITFIELD)
#define ADF_LITTLE_ENDIAN_MACHINE
#elif defined (__BIG_ENDIAN_BITFIELD)
#define ADF_BIG_ENDIAN_MACHINE
#else
#error  "Please fix <asm/byteorder.h>"
#endif

#define __adf_os_packed_pre     
#define __adf_os_packed_post     __attribute__ ((packed))

#define __adf_os_ull(_num)  _num ## ULL

struct __adf_net_drv;

typedef int (*__adf_os_intr)(void *);
/**
 * Private definitions of general data types
 */
typedef dma_addr_t              __adf_os_dma_addr_t;
typedef size_t                  __adf_os_dma_size_t;

typedef enum __adf_os_cache_sync{
    __ADF_SYNC_PREREAD,
    __ADF_SYNC_PREWRITE,
    __ADF_SYNC_POSTREAD,
    __ADF_SYNC_POSTWRITE
}__adf_os_cache_sync_t;

typedef struct __adf_os_resource{
    unsigned long   paddr;
    void __iomem *  vaddr;
    unsigned long   len;
}__adf_os_resource_t;

/**
 * generic data types
 */
struct __adf_device  {
    void                   *drv;
    void                   *drv_hdl;
    char                   *drv_name;
    int                    irq;
    struct device          *dev;
    __adf_os_resource_t    res;
    __adf_os_intr          func;/*Interrupt handler*/
}; 

typedef struct __adf_device *__adf_os_device_t;

typedef size_t            __adf_os_size_t;
typedef off_t             __adf_os_off_t;
typedef uint8_t __iomem * __adf_os_iomem_t;

typedef struct __adf_os_segment{
    dma_addr_t  daddr;
    uint32_t    len; 
}__adf_os_segment_t;

struct __adf_os_dma_map{
    uint32_t                mapped;
    uint32_t                nsegs;
    uint32_t                coherent;
    __adf_os_segment_t      seg[__ADF_OS_MAX_SCATTER];
};
typedef struct  __adf_os_dma_map  *__adf_os_dma_map_t;

typedef uint8_t           __a_uint8_t;
typedef int8_t            __a_int8_t;
typedef uint16_t          __a_uint16_t;
typedef int16_t           __a_int16_t; 
typedef uint32_t          __a_uint32_t;
typedef int32_t           __a_int32_t;
typedef uint64_t          __a_uint64_t;
typedef int64_t           __a_int64_t;

enum __adf_net_wireless_evcode{
    __ADF_IEEE80211_ASSOC = SIOCGIWAP,
    __ADF_IEEE80211_REASSOC =IWEVCUSTOM,
    __ADF_IEEE80211_DISASSOC = SIOCGIWAP,
    __ADF_IEEE80211_JOIN = IWEVREGISTERED,
    __ADF_IEEE80211_LEAVE = IWEVEXPIRED,
    __ADF_IEEE80211_SCAN = SIOCGIWSCAN,
    __ADF_IEEE80211_REPLAY = IWEVCUSTOM,
    __ADF_IEEE80211_MICHAEL = IWEVCUSTOM,
    __ADF_IEEE80211_REJOIN = IWEVCUSTOM, 
    __ADF_CUSTOM_PUSH_BUTTON = IWEVCUSTOM,
};

#define __adf_os_print               printk
#define __adf_os_snprint             snprintf
#define __adf_os_vsnprint            vsnprintf

#define __ADF_OS_DMA_TO_DEVICE      DMA_TO_DEVICE
#define __ADF_OS_DMA_FROM_DEVICE    DMA_FROM_DEVICE

#endif /*_ADF_CMN_OS_TYPES_PVT_H*/
