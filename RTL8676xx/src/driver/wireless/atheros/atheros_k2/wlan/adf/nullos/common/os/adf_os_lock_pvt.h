#ifndef _ADF_CMN_OS_LOCK_PVT_H
#define _ADF_CMN_OS_LOCK_PVT_H

/**
 * @brief Platform spinlock object
 */
typedef void *__adf_os_spinlock_t;

/**
 * @brief Platform mutex object
 */
typedef void *__adf_os_mutex_t;

/**
 * @brief Initialize a mutex
 *
 * @param[in] m mutex to initialize
 */
static adf_os_inline a_status_t  
__adf_os_init_mutex(__adf_os_mutex_t *m)
{
    return A_STATUS_OK;
}

/**
 * @brief Take the mutex
 *
 * @param[in] m mutex to take
 */
static adf_os_inline int 
__adf_os_mutex_acquire(__adf_os_mutex_t *m)
{
    return A_STATUS_OK;
}

/**
 * @brief Give the mutex
 *
 * @param[in] m mutex to give
 */
static adf_os_inline void 
__adf_os_mutex_release(__adf_os_mutex_t *m)
{
    return;
}

/**
 * @brief Initialize a spinlock
 *
 * @param[in] lock spinlock object pointer
 */
static adf_os_inline a_status_t
__adf_os_spinlock_init(__adf_os_spinlock_t *lock)
{
    return A_STATUS_OK;
}


/**
 * @brief Acquire a spinlock by disabling the interrupts
 *
 * @param[in]  lock     spinlock object pointer
 * @param[out] flags    flags used to hold interrupt state
 */
static adf_os_inline void
__adf_os_spin_lock_irq(__adf_os_spinlock_t *lock, a_uint32_t *flags)
{
    return;
}


/**
 * @brief Release a spinlock & restore the irq
 *
 * @param[in] lock  spinlock object pointer
 * @param[in] flags flags filled in by @ref adf_os_spin_lock_irq
 */
static adf_os_inline void
__adf_os_spin_unlock_irq(__adf_os_spinlock_t *lock, a_uint32_t *flags)
{
    return;
}


/**
 * @brief locks the spinlock mutex in soft irq context
 * 
 * @param[in] lock  spinlock object pointer
 */
static adf_os_inline void
__adf_os_spin_lock_bh(__adf_os_spinlock_t   *lock)
{
    return;
}


/**
 * @brief unlocks the spinlock mutex in soft irq context
 * 
 * @param[in] lock  spinlock object pointer
 */
static adf_os_inline void
__adf_os_spin_unlock_bh(__adf_os_spinlock_t *lock)
{
    return;
}


/**
 * @brief Execute the input function with spinlock held and interrupt disabled.
 *
 * @param[in] hdl       OS handle
 * @param[in] lock      spinlock to be held for the critical region
 * @param[in] func      critical region function that to be executed
 * @param[in] context   context of the critical region function
 * 
 * @return Boolean status returned by the critical region function
 */
static adf_os_inline a_bool_t
__adf_os_spinlock_irq_exec(adf_os_handle_t           hdl,
                         __adf_os_spinlock_t        *lock,
                         adf_os_irqlocked_func_t  func,
                         void                     *arg)
{
    return A_STATUS_OK;
}

#endif /*_ADF_CMN_OS_LOCK_PVT_H*/
