#ifndef ADF_OS_ATOMIC_PVT_H
#define ADF_OS_ATOMIC_PVT_H

#include <asm/atomic.h>

typedef atomic_t __adf_os_atomic_t;

static inline a_status_t
__adf_os_atomic_init(__adf_os_atomic_t *v)
{
    atomic_set(v, 0);

    return A_STATUS_OK;
}

static inline a_uint32_t
__adf_os_atomic_read(__adf_os_atomic_t *v)
{
    return (atomic_read(v));
}
static inline void
__adf_os_atomic_inc(__adf_os_atomic_t *v)
{
    atomic_inc(v);
}

static inline void
__adf_os_atomic_dec(__adf_os_atomic_t *v)
{
    atomic_dec(v);
}

static inline a_uint32_t
 __adf_os_atomic_dec_and_test(__adf_os_atomic_t *v)
{
    return(atomic_dec_and_test(v));
}

#endif
