/**
 * @ingroup adf_os_public
 * @file adf_os_util.h
 * This file defines utility functions.
 */

#ifndef _ADF_CMN_OS_UTIL_H
#define _ADF_CMN_OS_UTIL_H

#include <adf_os_util_pvt.h>

/**
 * @brief Compiler-dependent macro hinting at an inline function.
 */
#define adf_os_inline              __adf_os_inline

/**
 * @brief Compiler-dependent macro denoting code likely to execute.
 */
#define adf_os_unlikely(_expr)     __adf_os_unlikely(_expr)

/**
 * @brief Compiler-dependent macro denoting code unlikely to execute.
 */
#define adf_os_likely(_expr)       __adf_os_likely(_expr)

/**
 * @brief read memory barrier. 
 */
#define adf_os_wmb()                __adf_os_wmb()

/**
 * @brief write memory barrier. 
 */
#define adf_os_rmb()                __adf_os_rmb()

/**
 * @brief read + write memory barrier. 
 */
#define adf_os_mb()                 __adf_os_mb()

/**
 * @brief return the lesser of a, b
 */ 
#define adf_os_min(_a, _b)          __adf_os_min(_a, _b)

/**
 * @brief return the larger of a, b
 */ 
#define adf_os_max(_a, _b)          __adf_os_max(_a, _b)

/**
 * @brief return the absolute value of a
 */
#define adf_os_abs(_a)              __adf_os_abs(_a)

/**
 * @brief assert "expr" evaluates to false.
 */ 
#define adf_os_assert(expr)         __adf_os_assert(expr)
/**
 * @brief warn & dump backtrace if expr evaluates true
 */
#define adf_os_warn(expr)           __adf_os_warn(expr)

/**
 * @brief replace with the name of the current function
 */
#define adf_os_function             __adf_os_function

/**
 * @brief supply pseudo-random numbers
 */
static adf_os_inline a_status_t  adf_os_get_rand(adf_os_handle_t  hdl, 
                                   a_uint8_t       *ptr, 
                                   a_uint32_t       len)
{
    return __adf_os_get_rand(hdl, ptr, len);
}


/**
 * @brief return square root
 */
static adf_os_inline a_uint32_t 
adf_os_int_sqrt(a_uint32_t x)
{
	return __adf_os_int_sqrt(x);
}

#endif /*_ADF_OS_UTIL_H*/
