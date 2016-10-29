#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H

#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#include <asm/types.h>

/* bsd */
typedef unsigned char           u_char;
typedef unsigned short          u_short;
typedef unsigned int            u_int;
typedef unsigned long           u_long;

typedef unsigned char           unchar;
typedef unsigned short          ushort;
typedef unsigned int            uint;
typedef unsigned long           ulong;
typedef unsigned char  BOOL;
typedef unsigned char  bit8u;
typedef unsigned short bit16u;
typedef unsigned int   bit32u;
typedef unsigned int   u4;
typedef unsigned short u2;
typedef unsigned char  u1;
typedef signed int     s4;
typedef signed char    s1;
typedef signed short   s2;


typedef         __u8            uint8_t;
typedef         __u16           uint16_t;
typedef         __u32           uint32_t;
typedef		__u32		U32;


#ifndef __BIT_TYPES_DEFINED__
#define __BIT_TYPES_DEFINED__

typedef         __u8            u_int8_t;
typedef         __s8            int8_t;
typedef         __u16           u_int16_t;
typedef         __s16           int16_t;
typedef         __u32           u_int32_t;
typedef         __s32           int32_t;

#endif /* !(__BIT_TYPES_DEFINED__) */


typedef int		        size_t;

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef int      ptrdiff_t;
#endif


#endif /* _LINUX_TYPES_H */
