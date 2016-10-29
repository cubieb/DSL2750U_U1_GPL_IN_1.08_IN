#ifndef _ADF_CMN_OS_LOCK_PVT_H
#define _ADF_CMN_OS_LOCK_PVT_H

#include <asm/semaphore.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <adf_os_types.h>

typedef spinlock_t       __adf_os_spinlock_t;
typedef struct semaphore __adf_os_mutex_t;

static inline a_status_t
__adf_os_init_mutex(struct semaphore *m)
{
    init_MUTEX(m);

    return A_STATUS_OK;
}

static inline int
__adf_os_mutex_acquire(struct semaphore *m)
{
    return down_interruptible(m);
}

static inline void
__adf_os_mutex_release(struct semaphore *m)
{
    up(m);
}

static inline a_status_t 
__adf_os_spinlock_init(spinlock_t *lock)
{
    spin_lock_init(lock);

    return A_STATUS_OK;
}

/*
 * Synchronous versions - only for OS' that have interrupt disable
 */
static inline void
__adf_os_spin_lock_irq(spinlock_t *lock, uint32_t *flags)
{
    spin_lock_irqsave(lock, *flags);
}

static inline void
__adf_os_spin_unlock_irq(spinlock_t *lock, uint32_t *flags)
{
    unsigned long tflags = *flags;
    spin_unlock_irqrestore(lock, tflags);
}

static inline void
__adf_os_spin_lock_bh(__adf_os_spinlock_t *lock)
{
	if (irqs_disabled()) 
		spin_lock(lock);	
	else 
		spin_lock_bh(lock);	
   
}
static inline void
__adf_os_spin_unlock_bh(__adf_os_spinlock_t *lock)
{
	if (irqs_disabled()) 
		spin_unlock(lock);	
	else 
		spin_unlock_bh(lock);	
}

static inline a_bool_t
__adf_os_spinlock_irq_exec(adf_os_handle_t  hdl, 
                           spinlock_t      *lock, 
                           adf_os_irqlocked_func_t func, 
                           void            *arg)
{
    unsigned long flags;
    a_bool_t ret;

    spin_lock_irqsave(lock, flags);
    ret = func(arg);
    spin_unlock_irqrestore(lock, flags);

    return ret;
}


#endif /*_ADF_CMN_OS_LOCK_PVT_H*/
