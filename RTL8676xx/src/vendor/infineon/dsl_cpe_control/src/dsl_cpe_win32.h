/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifdef WIN32

/** \file
   OS interface, WIN32 adaptation
*/

#pragma pack(1)

#ifndef __BIG_ENDIAN
   #define __BIG_ENDIAN       1
   #define __LITTLE_ENDIAN    2
#endif

#ifndef __BYTE_ORDER
   #define __BYTE_ORDER       __LITTLE_ENDIAN
#endif

#include "drv_dsl_cpe_api_types.h"
#include <stdio.h>              /* FILE */
#include <stdlib.h>             /* malloc, free, strlen, strcpy, strtoul */
#include <string.h>             /* memset, strstr */
#include <ctype.h>
#include <stdarg.h>             /* va_start... */
#include <fcntl.h>              /* open */
#include <sys/stat.h>           /* stat */
#include <math.h>               /* log10, fabs, ceil */
#include <winsock2.h>           /* select / FD_SET / fd_set */

#define ULONG_MAX     0xffffffffUL  /* maximum unsigned long value */

/** process/thread identifier */
typedef HANDLE                  DSL_Thread_t;

/** semaphore identifier */
typedef HANDLE                  DSL_Semaphore_t;

/** indicate that semaphore is not avaiable */
#define DSL_SEM_NOT_INIT        0

/** map the fd_set to own type */
typedef fd_set                  DSL_fd_set_t;

/** map FILE to own type */
typedef FILE                    DSL_File_t;

/** map stat structure */
typedef struct stat             DSL_stat_t;

/** map to stderr */
#define DSL_STDERR              stderr
/** map to stdout */
#define DSL_STDOUT              stdout
/** map to stdin */
#define DSL_STDIN               stdin

/** carriage return for windows */
#define DSL_CRLF                    "\n"

#ifndef _IOWR
# define _IOWR(x,y,z)   (((x)<<8)|y)
#endif
#ifndef _IOW
# define _IOW(x,y,z)    (((x)<<8)|y)
#endif
#ifndef _IOR
# define _IOR(x,y,z)    (((x)<<8)|y)
#endif
#ifndef _IO
# define _IO(x,y)       (((x)<<8)|y)
#endif

int getopt_long ( int argc,
                  char *argv[],
                  const char *long_option_string,
                  struct option *pOptions,
                  int *option_index );

extern char *optarg;
extern int optind;

struct option
{
   char *name;
   int parameter;
   int dummy;
   char val;
};


#define DSL_snprintf _snprintf

#pragma pack()

#endif /* WIN32 */


