/* Copyright (C) 2006 Ikanos Communications 
 * See IKANOS_PROP_LICENSE.txt for license information. */


#ifndef __DEFINES_H__
#define __DEFINES_H__


typedef int8_t         INT8;
typedef int16_t        INT16;
typedef int32_t        INT32;
typedef u_int16_t      UINT16;
typedef unsigned int uint32_t;
typedef u_int32_t      UINT32;
typedef u_int8_t       UINT8;
typedef char           CHAR;
typedef void           VOID;
typedef void (*FUNCPTR)();
typedef u_int64_t      UINT64;
typedef u_char         UCHAR;
typedef unsigned char  BOOL;
typedef unsigned int   UINT;
typedef unsigned short USHORT;
typedef int            INT;
typedef unsigned long  ULONG;

# define WAIT    0x00000001
# define NOWAIT  0x00000002

# define SUCCESS          0
# define FAILURE         (-1)

# ifndef NULL
# define NULL    (void *) 0
# endif

# ifndef TRUE
# define TRUE    (BOOL) 1
# endif

# ifndef FALSE
# define FALSE   (BOOL) 0
# endif

# define MSECPERSEC     1000

# define PRI_HIGH         1
# define PRI_LOW          2


# define SETBIT(var, bit, val)        if (val) var |= (1 << bit ## _BIT ); else var &= ~(1 << bit ## _BIT)

# define GETBIT(var, bit)        (((var) >> bit ## _BIT) & 0x1)

# define ZEROMASK(fld)           ((1 << (fld ## _LEN)) - 1)
# define FLDMASK(fld)            (ZEROMASK(fld) << (fld ## _FST))
# define SETFIELD(var, fld, val) (var = (var & ~FLDMASK(fld)) | (((val) & ZEROMASK(fld)) << fld ## _FST))

// Local additions to make register setting simpler
# define VAL(bit)                (1 << (bit ## _BIT))

# define bcopy(s,d,l)            memmove(d,s,l)

#endif // __DEFINES_H__
