#ifndef _ADF_CMN_OS_TIMER_PVT_H
#define _ADF_CMN_OS_TIMER_PVT_H

#include <adf_os_types.h>
#include <adf_os_util.h>

/**
 * @brief Platform timer object
 */
typedef void * __adf_os_timer_t;


/**
 * @brief Initialize a timer
 * 
 * @param[in] hdl       OS handle
 * @param[in] timer     timer object pointer
 * @param[in] func      timer function
 * @param[in] context   context of timer function
 */
static adf_os_inline a_status_t
__adf_os_timer_init(adf_os_handle_t      hdl,
                  __adf_os_timer_t      *timer,
                  adf_os_timer_func_t  func,
                  void                *arg)
{
    return A_STATUS_OK;
}

/**
 * @brief Start a one-shot timer
 * 
 * @param[in] timer     timer object pointer
 * @param[in] msec      expiration period in milliseconds
 */
static adf_os_inline a_status_t
__adf_os_timer_start(__adf_os_timer_t *timer, a_uint64_t ticks)
{
    return A_STATUS_OK;
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
__adf_os_timer_cancel(__adf_os_timer_t *timer)
{
    return A_STATUS_OK;
}

#endif /*_ADF_OS_TIMER_PVT_H*/
