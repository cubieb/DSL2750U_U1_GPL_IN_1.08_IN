#ifndef _ADF_CMN_OS_TIMER_PVT_H
#define _ADF_CMN_OS_TIMER_PVT_H

#include <linux/version.h>
#include <linux/delay.h>
#include <linux/timer.h>

#if LINUX_VERSION_CODE  >= KERNEL_VERSION(2,6,19)
#include <linux/jiffies.h>
#endif
#include <adf_os_types.h>

/* 
 * timer data type 
 */
typedef struct timer_list       __adf_os_timer_t;

/*
 * ugly - but every other OS takes, sanely, a void*
 */
typedef void (*dummy_timer_func_t)(unsigned long arg);

/* 
 * Initialize a timer
 */
static inline a_status_t
__adf_os_timer_init(adf_os_handle_t      hdl,
                    struct timer_list   *timer,
                    adf_os_timer_func_t  func,
                    void                *arg)
{
    init_timer(timer);
    timer->function = (dummy_timer_func_t)func;
    timer->data = (unsigned long)arg;

    return A_STATUS_OK;
}

/* 
 * start a timer 
 */
static inline a_status_t
__adf_os_timer_start(struct timer_list *timer, a_uint32_t delay)
{
    timer->expires = jiffies + msecs_to_jiffies(delay);
    add_timer(timer);

    return A_STATUS_OK;
}

/*
 * Cancel a timer
 *
 * Return: TRUE if timer was cancelled and deactived,
 *         FALSE if timer was cancelled but already got fired.
 */
static inline a_bool_t
__adf_os_timer_cancel(struct timer_list *timer)
{
    if (likely(del_timer(timer)))
        return 1;
    else
        return 0;
}

/*
 * XXX Synchronously canel a timer
 *
 * Return: TRUE if timer was cancelled and deactived,
 *         FALSE if timer was cancelled but already got fired.
 *
 * Synchronization Rules:
 * 1. caller must make sure timer function will not use
 *    adf_os_set_timer to add iteself again.
 * 2. caller must not hold any lock that timer function
 *    is likely to hold as well.
 * 3. It can't be called from interrupt context.
 */
static inline a_bool_t
__adf_os_timer_sync_cancel(struct timer_list *timer)
{
    return del_timer_sync(timer);
}



#endif /*_ADF_OS_TIMER_PVT_H*/
