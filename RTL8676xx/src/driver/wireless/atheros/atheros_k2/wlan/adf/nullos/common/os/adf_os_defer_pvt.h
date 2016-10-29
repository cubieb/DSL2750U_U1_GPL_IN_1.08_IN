#ifndef _ADF_CMN_OS_DEFER_PVT_H
#define _ADF_CMN_OS_DEFER_PVT_H

/**
 * @brief Representation of a work queue.
 */ 
typedef void *__adf_os_work_t;

/**
 * @brief Representation of a bottom half.
 */ 
typedef void *__adf_os_bh_t;



/**
 * @brief This initiallizes the Bottom half deferred handler
 * 
 * @param[in] hdl   OS handle
 * @param[in] bh    bottom instance
 * @param[in] func  deferred function to run at bottom half interrupt
 *                  context.
 * @param[in] arg   argument for the deferred function
 */
static adf_os_inline a_status_t 
__adf_os_init_bh(adf_os_handle_t  hdl, __adf_os_bh_t  *bh,
               adf_os_defer_fn_t  func,void  *arg)
{
    return A_STATUS_OK;
}


/**
 * @brief schedule a bottom half (DPC)
 * 
 * @param[in] hdl   OS handle
 * @param[in] bh    bottom instance
 */
static adf_os_inline a_status_t 
__adf_os_sched_bh(adf_os_handle_t hdl, __adf_os_bh_t *bh)
{
    return A_STATUS_OK;
}

/**
 * @brief disable the bh (synchronous)
 * 
 * @param[in] hdl   OS handle
 * @param[in] bh    bottom instance
 */
static adf_os_inline a_status_t 
__adf_os_disable_bh(adf_os_handle_t hdl, __adf_os_bh_t *bh)
{
    return A_STATUS_OK;
}

/*********************Non-Interrupt Context deferred Execution***************/

/**
 * @brief allocate a work/task queue, This runs in non-interrupt
 *        context, so can be preempted by H/W & S/W intr
 * 
 * @param[in] hdl   OS handle
 * @param[in] work  work instance
 * @param[in] func  deferred function to run at bottom half non-interrupt
 *                  context.
 * @param[in] arg   argument for the deferred function
 */
static adf_os_inline a_status_t 
__adf_os_init_work(adf_os_handle_t hdl, __adf_os_work_t  *work,
                 adf_os_defer_fn_t  func, void  *arg)
{
    return A_STATUS_OK;
}

/**
 * @brief Schedule a deferred task on non-interrupt context
 * 
 * @param[in] hdl   OS handle
 * @param[in] work  work instance
 */
static adf_os_inline  a_status_t
__adf_os_sched_work(adf_os_handle_t  hdl, __adf_os_work_t   *work)
{
    return A_STATUS_OK;
}

/**
 *@brief disable the deferred task (synchronous)
 *
 *@param[in] hdl    OS handle
 *@param[in] work   work instance
 */
static adf_os_inline  a_status_t
__adf_os_disable_work(adf_os_handle_t hdl, __adf_os_work_t *work) 
{
    return A_STATUS_OK;
}

#endif /*_ADF_CMN_OS_DEFER_PVT_H*/
