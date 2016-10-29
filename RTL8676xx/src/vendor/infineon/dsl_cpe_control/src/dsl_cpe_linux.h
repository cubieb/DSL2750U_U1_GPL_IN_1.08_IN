/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifdef LINUX

/** \file
   OS interface, Linux adaptation
*/

#include <stdlib.h>
#include <getopt.h>
#include <features.h>
#include <stdio.h>            /* fdopen */
#include <sys/fcntl.h>        /* open */
#include <sys/ioctl.h>        /* ioctl */
#include <string.h>        /* memset, strstr, strlen */
#include <stdlib.h>             /* strtoul */
#include <sys/time.h>         /* fd_set */
#include <sys/select.h>       /* select */
#include <ctype.h>
#include <unistd.h>             /* daemon */
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>        /* va_start... */
#include <errno.h>
#include <sys/stat.h>           /* stat */
#include <math.h>          /* log10, fabs, ceil */
#include <termios.h> /* tcgetattr, tcsetattr */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>           /* socket */

#ifndef __BIG_ENDIAN
   #define __BIG_ENDIAN       1
   #define __LITTLE_ENDIAN    2
#endif

#ifndef __BYTE_ORDER
   #define __BYTE_ORDER       __BIG_ENDIAN
#endif

#include "drv_dsl_cpe_api_types.h"

/** process/thread identifier */
typedef pthread_t                DSL_Thread_t;

/** semaphore identifier */
typedef DSL_int_t                DSL_Semaphore_t;

/** identifier for not initialized semaphores */
#define DSL_SEM_NOT_INIT         (-1)

/** map the fd_set to own type */
typedef fd_set                   DSL_fd_set_t;

/** map the sockaddr_in to own type */
typedef struct sockaddr_in DSL_sockaddr_in_t;

/** map the sockaddr to own type */
typedef struct sockaddr DSL_sockaddr_t;

/** map the sockaddr_in to own type */
typedef struct linger DSL_SockOptLinger_t;

/** map the socklen_t to own type */
typedef socklen_t DSL_socklen_t;

/** map FILE to own type */
typedef FILE                     DSL_File_t;

/** map stat structure */
typedef struct stat              DSL_stat_t;

/** map timeval structure */
typedef struct timeval DSL_TimeVal_t;

/** map to stderr */
#define DSL_STDERR stderr

/** map to stdout */
#define DSL_STDOUT stdout

/** map to stdin */
#define DSL_STDIN stdin

#define DSL_snprintf snprintf

/* stdio.h mapping */
#define DSL_FdOpen fdopen

/* arpa/inet.h mapping */
#define DSL_StringToAddress inet_aton
#define DSL_AddressToString inet_ntoa

/* sys/socket.h mapping */
#define DSL_Socket socket
#define DSL_SockOptSet setsockopt
#define DSL_Accept accept
#define DSL_SocketClose close
#define DSL_SocketRecv recv
#define DSL_SocketSend send


#ifndef FD_SETSIZE
#define DSL_FD_SETSIZE 1024
#else
#define DSL_FD_SETSIZE FD_SETSIZE
#endif

#endif /* LINUX */
