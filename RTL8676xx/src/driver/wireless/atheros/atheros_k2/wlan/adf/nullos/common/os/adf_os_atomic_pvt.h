#ifndef ADF_OS_ATOMIC_PVT_H
#define ADF_OS_ATOMIC_PVT_H

/**
 * @brief Atomic type of variable.
 * Use this when you want a simple resource counter etc. which is atomic
 * across multiple CPU's. These maybe slower than usual counters on some
 * platforms/OS'es, so use them with caution.
 */
typedef void *__adf_os_atomic_t;

/** 
 * @brief Initialize an atomic type variable
 * @param[in] v a pointer to an opaque atomic variable
 */
static adf_os_inline a_status_t
__adf_os_atomic_init(__adf_os_atomic_t *v)
{
    return A_STATUS_OK;
}

/**
 * @brief Read the value of an atomic variable.
 * @param[in] v a pointer to an opaque atomic variable
 *
 * @return the current value of the variable
 */
static adf_os_inline a_uint32_t
__adf_os_atomic_read(__adf_os_atomic_t *v)
{
    return 0;
}

/**
 * @brief Increment the value of an atomic variable.
 * @param[in] v a pointer to an opaque atomic variable
 */
static adf_os_inline void
__adf_os_atomic_inc(__adf_os_atomic_t *v)
{
    return;
}

/**
 * @brief Decrement the value of an atomic variable.
 * @param v a pointer to an opaque atomic variable
 */
static adf_os_inline void
__adf_os_atomic_dec(__adf_os_atomic_t *v)
{
    return;
}

static adf_os_inline a_uint32_t
 __adf_os_atomic_dec_and_test(__adf_os_atomic_t *v)
{
    return 0;
}

#endif
