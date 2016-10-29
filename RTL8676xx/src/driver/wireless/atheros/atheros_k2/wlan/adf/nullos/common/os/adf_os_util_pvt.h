#ifndef _ADF_CMN_OS_UTIL_PVT_H
#define _ADF_CMN_OS_UTIL_PVT_H

#include <adf_os_types.h>

/**
 * @brief Compiler-dependent macro hinting at adf_os_inline function.
 */
#define __adf_os_inline _inline

/**
 * @brief Compiler-dependent macro denoting code likely to execute.
 */
#define __adf_os_unlikely(_expr) _expr

/**
 * @brief Compiler-dependent macro denoting code unlikely to execute.
 */
#define __adf_os_likely(_expr)   _expr

/**
 * @brief read memory barrier. 
 */
#define __adf_os_wmb()

/**
 * @brief write memory barrier. 
 */
#define __adf_os_rmb()

/**
 * @brief read + write memory barrier. 
 */
#define __adf_os_mb()

/**
 * @brief return the lesser of a, b
 */ 
#define __adf_os_min(_a, _b)   (0)

/**
 * @brief return the larger of a, b
 */ 
#define __adf_os_max(_a, _b)   (0)

/**
 * @brief return the absolute value of a
 */ 
#define __adf_os_abs(_a)        (_a)

/**
 * @brief assert "expr" evaluates to false.
 */ 
#define __adf_os_assert(expr)

/**
 * @brief warn & dump backtrace if expr evaluates true
 */
#define __adf_os_warn(expr)

/**
 * @brief replace with the name of the function
 */ 
#define __adf_os_function   ""

/**
 * @brief supply pseudo-random numbers
 */
static __adf_os_inline a_status_t  
__adf_os_get_rand(adf_os_handle_t  hdl, 
                    a_uint8_t       *ptr, 
                    a_uint32_t       len)
{
    return 0;
}

/**
 * @brief return square root
 */ 
static __adf_os_inline a_uint32_t __adf_os_int_sqrt(a_uint32_t x)  
{
	return 0;
}
#endif /*_ADF_CMN_OS_UTIL_PVT_H*/
