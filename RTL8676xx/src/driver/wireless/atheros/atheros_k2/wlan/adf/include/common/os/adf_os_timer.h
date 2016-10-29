/**
 * @ingroup adf_os_public
 * @file adf_os_timer.h
 * This file abstracts OS timers.
 */

#ifndef _ADF_CMN_OS_TIMER_H
#define _ADF_CMN_OS_TIMER_H

#include <adf_os_types.h>
#include <adf_os_timer_pvt.h>


/**
 * @brief Platform timer object
 */
typedef __adf_os_timer_t           adf_os_timer_t;


/**
 * @brief Initialize a timer
 * 
 * @param[in] hdl       OS handle
 * @param[in] timer     timer object pointer
 * @param[in] func      timer function
 * @param[in] context   context of timer function
 */
static adf_os_inline a_status_t
adf_os_timer_init(adf_os_handle_t      hdl,
                  adf_os_timer_t      *timer,
                  adf_os_timer_func_t  func,
                  void                *arg)
{
    return __adf_os_timer_init(hdl, timer, func, arg);
}

/**
 * @brief Start a one-shot timer
 * 
 * @param[in] timer     timer object pointer
 * @param[in] delay     delay from current time to run the timer, in millisecs
 */
static adf_os_inline a_status_t
adf_os_timer_start(adf_os_timer_t *timer, a_uint32_t delay)
{
    return __adf_os_timer_start(timer, delay);
}

/**
 * @brief Cancel a timer
 *
 * @param[in] timer     timer object pointer
 * 
 * @retval    TRUE      timer was cancelled and deactived
 * @retval    FALSE     timer was cancelled but already got fired.
 */
static adf_os_inline a_bool_t
adf_os_timer_cancel(adf_os_timer_t *timer)
{
    return __adf_os_timer_cancel(timer);
}

#endif

