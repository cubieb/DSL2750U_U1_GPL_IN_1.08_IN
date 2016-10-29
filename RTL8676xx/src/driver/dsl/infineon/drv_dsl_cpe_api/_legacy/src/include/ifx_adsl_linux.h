#ifndef _IFX_ADSL_LINUX_H
#define _IFX_ADSL_LINUX_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

#ifdef __KERNEL__

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
/* define sizeof to (int)sizeof to prevent some compile warning */
#define sizeof (int)sizeof
#include <linux/netdevice.h>
#undef sizeof
#include <linux/etherdevice.h>
#include <asm/semaphore.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <asm/uaccess.h>
#include <asm/string.h>

#undef CONFIG_DEVFS_FS /* 165204:henryhsu devfs will make mei open file fail. */

#ifdef CONFIG_DEVFS_FS
#include <linux/devfs_fs_kernel.h>
#endif
#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#endif

#include <linux/list.h>
#include <linux/delay.h>
#ifndef __LINUX__
#define __LINUX__
#endif /* __LINUX__ */


#define MEI_LOCKINT(var) \
        save_flags(var);\
        cli()
#define MEI_UNLOCKINT(var) \
        restore_flags(var)

#define MEI_MUTEX_INIT(id,flag) \
        sema_init(&id,flag)
#define MEI_MUTEX_LOCK(id) \
        down_interruptible(&id)
#define MEI_MUTEX_UNLOCK(id) \
        up(&id)

#define MEI_DISABLE_IRQ \
        disable_irq
#define MEI_ENABLE_IRQ \
        enable_irq

#define REQUEST_IRQ_HANDLER \
        request_irq

#define FREE_IRQ_HANDLER \
        free_irq

#define MEI_WAIT(ms) \
        {\
                set_current_state(TASK_INTERRUPTIBLE);\
                schedule_timeout(ms);\
        }

#define MEI_INIT_WAKELIST(name,queue) \
        init_waitqueue_head(&queue)

#define MEI_WAIT_EVENT_TIMEOUT(ev,timeout)\
        interruptible_sleep_on_timeout(&ev,timeout/10)

#define MEI_WAIT_EVENT(ev)\
        interruptible_sleep_on(&ev)
#define MEI_WAKEUP_EVENT(ev)\
        wake_up_interruptible(&ev)

typedef unsigned long MEI_intstat_t;
typedef struct semaphore  MEI_mutex_t;
typedef struct file MEI_file_t;
typedef struct inode MEI_inode_t;

extern void mask_and_ack_danube_irq(unsigned int irq_nr);

#ifdef MODULE

#ifdef HAVE_CONFIG_H
#include <drv_dsl_cpe_api_config.h>
#else
#include <drv_dsl_cpe_api_danube_kernel_config.h>
#endif /* HAVE_CONFIG_H */

#ifdef INCLUDE_ADSL_MIB
#define IFX_ADSL_MIB_RFC3440
#endif

#include <ifx_adsl_cmvs.h>
#include <ifx_adsl_basic_ioctl.h>
#include <ifx_adsl_bsp.h>
#include <ifx_adsl_basic.h>
#include <ifx_adsl_autoboot.h>
#include <ifx_adsl_mib_ioctl.h>
#include <ifx_adsl_mib.h>
#if defined(INCLUDE_ADSL_CEOC)
#include <ifx_adsl_ceoc.h>
#endif
#if defined(INCLUDE_ADSL_LED)
#include <ifx_adsl_led.h>
#endif

#else /* MODULE */

#include <ifx/dsl/drv_dsl_cpe_api_danube_kernel_config.h>

#ifdef INCLUDE_ADSL_MIB
#define IFX_ADSL_MIB_RFC3440
#endif

#include <ifx/dsl/ifx_adsl_cmvs.h>
#include <ifx/dsl/ifx_adsl_basic_ioctl.h>
#include <ifx/dsl/ifx_adsl_bsp.h>
#include <ifx/dsl/ifx_adsl_basic.h>
#include <ifx/dsl/ifx_adsl_autoboot.h>
#include <ifx/dsl/ifx_adsl_mib_ioctl.h>
#include <ifx/dsl/ifx_adsl_mib.h>
#if defined(INCLUDE_ADSL_CEOC)
#include <ifx/dsl/ifx_adsl_ceoc.h>
#endif
#if defined(INCLUDE_ADSL_LED)
#include <ifx/dsl/ifx_adsl_led.h>
#endif
#endif /* MODULE */

#endif /* __KERNEL__ */
#endif /*  _IFX_ADSL_LINUX_H */
