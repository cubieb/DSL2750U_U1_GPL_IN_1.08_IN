#ifndef _ADF_CMN_OS_TIME_PVT_H
#define _ADF_CMN_OS_TIME_PVT_H

#include <adf_os_types.h>

typedef a_uint64_t __adf_time_t;

/**
 * @brief count the number of ticks elapsed from the time when
 *        the system booted
 * 
 * @return ticks
 */
static adf_os_inline __adf_time_t
__adf_os_ticks(void)
{
	return 0;
}

/**
 * @brief convert ticks to milliseconds
 *
 * @param[in] ticks number of ticks
 * @return time in milliseconds
 */ 
static adf_os_inline a_uint32_t
__adf_os_ticks_to_msecs(__adf_time_t ticks)
{
	return 0;
}

/**
 * @brief convert milliseconds to ticks
 *
 * @param[in] time in milliseconds
 * @return number of ticks
 */ 
static adf_os_inline __adf_time_t
__adf_os_msecs_to_ticks(a_uint32_t msecs)
{
	return 0;
}

/**
 * @brief Return a monotonically increasing time. This increments once per HZ ticks
 */
static adf_os_inline __adf_time_t
__adf_os_getuptime(void)
{
    return 0;
}

/**
 * @brief Delay in microseconds
 *
 * @param[in] microseconds to delay
 */
static adf_os_inline void
__adf_os_udelay(a_uint32_t usecs)
{
    return;
}

/**
 * @brief Delay in milliseconds.
 *
 * @param[in] milliseconds to delay
 */
static adf_os_inline void
__adf_os_mdelay(a_uint32_t msecs)
{
    return;
}

/**
 * @brief Check if _a is later than _b.
 */ 
static adf_os_inline a_bool_t 
__adf_os_time_after(__adf_time_t a, __adf_time_t b)
{
    return A_TRUE;
}

/**
 * @brief Check if _a is prior to _b.
 */ 
static adf_os_inline a_bool_t
__adf_os_time_before(__adf_time_t a, __adf_time_t b)
{
    return A_TRUE;
}

/**
 * @brief Check if _a atleast as recent as _b, if not later.
 */ 
static adf_os_inline a_bool_t
__adf_os_time_after_eq(__adf_time_t a, __adf_time_t b)
{
    return A_TRUE;
}

#endif
