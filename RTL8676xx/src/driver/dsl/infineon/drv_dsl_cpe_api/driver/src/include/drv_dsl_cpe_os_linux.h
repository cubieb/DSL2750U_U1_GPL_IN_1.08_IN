/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_OS_LINUX_H
#define _DRV_DSL_CPE_OS_LINUX_H

#ifdef __cplusplus
   extern "C" {
#endif

#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


#include <linux/version.h>

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17))
   #include <linux/utsrelease.h>
#endif

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>

/*$$
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <asm/semaphore.h>
#include <linux/ioport.h>
*/
/*$$
#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#endif
*/
/*$$
#include <linux/list.h>
#include <linux/delay.h>*/

#ifndef __LINUX__
#define __LINUX__
#endif /*__LINUX__*/

#ifndef KERNEL_VERSION
   #define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

/* The major number of a CPE API device
      Typically it is a Voodoo 3dfx device (107) for Danube/Amazon-SE and
      logical volume manager (109) for VINAX
*/
#if defined (INCLUDE_DSL_CPE_API_DANUBE) || defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
   #define DRV_DSL_CPE_API_DEV_MAJOR 107
#elif defined (INCLUDE_DSL_CPE_API_VINAX)
   #define DRV_DSL_CPE_API_DEV_MAJOR 109
#else
   #error "Device is not defined!"
#endif

/**
   Definition of the plattform endianess.
*/
#define DSL_LITTLE_ENDIAN  0x1234
#define DSL_BIG_ENDIAN     0x4321

#if defined ( __LITTLE_ENDIAN )
   #define DSL_BYTE_ORDER             DSL_LITTLE_ENDIAN
#elif defined ( __BIG_ENDIAN )
   #define DSL_BYTE_ORDER             DSL_BIG_ENDIAN
#else
#error "Unknown Byteorder !!!"
#endif


/* The device driver related types */
typedef struct file DSL_DRV_file_t;
typedef struct inode DSL_DRV_inode_t;
#ifndef _lint
typedef loff_t DSL_DRV_offset_t;
#else
typedef int DSL_DRV_offset_t;
#endif

/* operating system types */
typedef size_t DSL_size_t;
typedef ssize_t DSL_ssize_t;
typedef struct semaphore DSL_mutex_t;
typedef wait_queue_head_t DSL_waitQueueHead_t;
typedef struct completion DSL_completion_t;
typedef struct timeval DSL_TimeVal_t;
typedef struct poll_table_struct DSL_DRV_Poll_Table_t;

/* mutex macros */
#define DSL_MUTEX_INIT(id,flag) \
        sema_init(&(id),flag)
#define DSL_MUTEX_LOCK(id) \
        down_interruptible(&(id))
#define DSL_MUTEX_UNLOCK(id) \
        up(&(id))

#define DSL_WAIT(ms) \
        {\
                set_current_state(TASK_INTERRUPTIBLE);\
                schedule_timeout(ms);\
        }

#define DSL_INIT_WAKELIST(name,queue) \
        init_waitqueue_head(&(queue))

#define DSL_INIT_COMPLETION(name,queue) \
        init_completion(&(queue))
#define DSL_WAIT_COMPLETION(q)\
        wait_for_completion(&(q))

/* wait for an event, timeout is measured in ms */
#define DSL_WAIT_EVENT_TIMEOUT(ev,timeout)\
        interruptible_sleep_on_timeout(&ev,timeout * HZ / 1000)

#define DSL_WAIT_EVENT(ev)\
        interruptible_sleep_on(&(ev))
#define DSL_WAKEUP_EVENT(ev)\
        wake_up_interruptible(&(ev))

/*#define DSL_WAIT_CONST (HZ * 10)*/

#define DSL_IsTimeNull(timevalstruct) (timevalstruct.tv_sec == 0)
#define DSL_GetTimeOfDay(timevalstruct) do_gettimeofday(timevalstruct)
#define DSL_TimeSecGet(timevalstruct) (timevalstruct.tv_sec)
#define DSL_Le2Cpu(le) le16_to_cpu(le)

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
   #define DSL_OS_ModUseCountIncrement() MOD_INC_USE_COUNT
   #define DSL_OS_ModUseCountDecrement() MOD_DEC_USE_COUNT
#else
   #define DSL_OS_ModUseCountIncrement() while(0) {}
   #define DSL_OS_ModUseCountDecrement() while(0) {}
#endif

typedef DSL_int_t (*DSL_ThreadFunc_t)(DSL_void_t *pArg);

DSL_uint32_t DSL_GetTime(DSL_uint32_t nOffset);

#define DSL_CRLF "\n"

#ifdef __cplusplus
}
#endif

#endif
