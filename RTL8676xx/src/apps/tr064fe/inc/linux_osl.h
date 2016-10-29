/*
 * $Copyright Open Broadcom Corporation$
 *
 * $Id: linux_osl.h,v 1.2.48.2 2003/10/31 21:31:36 mthawani Exp $
 */

#include <arpa/inet.h>	    
#include <assert.h>	    
#include <ctype.h>	    
#include <errno.h>
#include <fcntl.h>	    
#include <netdb.h>	    
#include <netinet/tcp.h>    
#include <stdio.h>	    
#include <stdlib.h>	    
#include <string.h>	    
#include <sys/socket.h>	    
#include <sys/stat.h>	    
#include <sys/uio.h>
#include <time.h>	    
#include <unistd.h>	    
#include <stdarg.h>	    // for va_list, etc.


/*  damn - the unix and vxworks version of inet_aton return different error codes. */
#define UPNP_INET_ATON(a,b)   (inet_aton(a,b)!=0)

#define OSL_NULL_FILE "/dev/null"
