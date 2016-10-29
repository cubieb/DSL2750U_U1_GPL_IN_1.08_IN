/**
 * @ingroup adf_os_public
 * @file adf_os_time.h
 * This file abstracts time related functionality.
 */
#ifndef _ADF_CMN_OS_TIME_H
#define _ADF_CMN_OS_TIME_H

#include <adf_os_time_pvt.h>

typedef __adf_time_t adf_time_t;

/**
 * @brief count the number of ticks elapsed from the time when
 *        the system booted
 * 
 * @return ticks
 */
static adf_os_inline adf_time_t
adf_os_ticks(void)
{
	return __adf_os_ticks();
}

/**
 * @brief convert ticks to milliseconds
 *
 * @param[in] ticks number of ticks
 * @return time in milliseconds
 */ 
static adf_os_inline a_uint32_t
adf_os_ticks_to_msecs(adf_time_t ticks)
{
	return (__adf_os_ticks_to_msecs(ticks));
}

/**
 * @brief convert milliseconds to ticks
 *
 * @param[in] time in milliseconds
 * @return number of ticks
 */ 
static adf_os_inline adf_time_t
adf_os_msecs_to_ticks(a_uint32_t msecs)
{
	return (__adf_os_msecs_to_ticks(msecs));
}

/**
 * @brief Return a monotonically increasing time. This increments once per HZ ticks
 */
static adf_os_inline adf_time_t
adf_os_getuptime(void)
{
    return (__adf_os_getuptime());
}

/**
 * @brief Delay in microseconds
 *
 * @param[in] microseconds to delay
 */
static adf_os_inline void
adf_os_udelay(a_uint32_t usecs)
{
    __adf_os_udelay(usecs);
}

/**
 * @brief Delay in milliseconds.
 *
 * @param[in] milliseconds to delay
 */
static adf_os_inline void
adf_os_mdelay(a_uint32_t msecs)
{
    __adf_os_mdelay(msecs);
}

/**
 * @brief Check if _a is later than _b.
 */ 
static adf_os_inline a_bool_t 
adf_os_time_after(adf_time_t a, adf_time_t b)
{
    return __adf_os_time_after(a, b);
}

/**
 * @brief Check if _a is prior to _b.
 */ 
static adf_os_inline a_bool_t
adf_os_time_before(adf_time_t a, adf_time_t b)
{
    return __adf_os_time_before(a, b);
}

/**
 * @brief Check if _a atleast as recent as _b, if not later.
 */ 
static adf_os_inline a_bool_t
adf_os_time_after_eq(adf_time_t a, adf_time_t b)
{
    return __adf_os_time_after_eq(a, b);
}

#endif
    

