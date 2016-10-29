#ifndef __OS_DEFS_H__
#define __OS_DEFS_H__

#include <unistd.h>  /* for getopt */
#include <stdio.h>   /* for snprintf */
#include <stdint.h>  /* for the various integer types */
#include <stdlib.h>  /* for NULL */
#include <string.h>  /* for strlen, strncpy */
#include <ctype.h>   /* for isdigit */
#include <syslog.h>  /* for syslog */
#include <stdarg.h>  /* for va_list */
#include "cms_params.h"


typedef int  SINT32;
typedef short SINT16;
typedef char SINT8;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned long UINT32;
typedef unsigned char UBOOL8;


/** Base64 encoded string representation of binary data.
 *
 * This is to support TR69 data types.
 */
typedef char *     BASE64;

/** String representation of date and time.
 *
 * This is to support TR69 data types.
 */
typedef char *     DATETIME;

/** Invalid file descriptor number */
#define CMS_INVALID_FD  (-1)


/** Invalid process id.
 *
 * Management entities should not need to use this constant.  It is used
 * by OS dependent code in the OAL layer.  But I put this constant here
 * to make it easier to find.
 */
#define CMS_INVALID_PID   0


/** A number to identify a MdmObject (but not the specific instance of
 *  the object).
 *
 * MdmObjectId's are defined in mdm_oid.h.
 */
typedef UINT16 MdmObjectId;

#endif /* __OS_DEFS_H__ */

