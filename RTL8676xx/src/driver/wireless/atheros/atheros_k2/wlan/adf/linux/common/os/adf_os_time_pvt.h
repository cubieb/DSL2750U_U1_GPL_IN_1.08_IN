#ifndef _ADF_CMN_OS_TIME_PVT_H
#define _ADF_CMN_OS_TIME_PVT_H

#include <linux/jiffies.h>
#include <linux/delay.h>

typedef unsigned long __adf_time_t;

static inline __adf_time_t
__adf_os_ticks(void)
{
    return (jiffies);
}
static inline uint32_t
__adf_os_ticks_to_msecs(unsigned long ticks)
{
    return (jiffies_to_msecs(ticks));
}
static inline __adf_time_t
__adf_os_msecs_to_ticks(a_uint32_t msecs)
{
    return (msecs_to_jiffies(msecs));
}
static inline __adf_time_t
__adf_os_getuptime(void)
{
    return jiffies;
}

static inline void
__adf_os_udelay(a_uint32_t usecs)
{
    udelay(usecs);
}

static inline void
__adf_os_mdelay(a_uint32_t msecs)
{
    mdelay(msecs);
}

/**
 * @brief Check if _a is later than _b.
 */ 
static inline a_bool_t 
__adf_os_time_after(__adf_time_t a, __adf_time_t b)
{
    return ((long)(b) - (long)(a) < 0);
}

/**
 * @brief Check if _a is prior to _b.
 */ 
static inline a_bool_t
__adf_os_time_before(__adf_time_t a, __adf_time_t b)
{
    return __adf_os_time_after(b,a);
}

/**
 * @brief Check if _a atleast as recent as _b, if not later.
 */ 
static inline a_bool_t
__adf_os_time_after_eq(__adf_time_t a, __adf_time_t b)
{
    return ((long)(a) - (long)(b) >= 0);
}

#endif
