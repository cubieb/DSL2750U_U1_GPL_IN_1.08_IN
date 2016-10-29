#ifndef __NET_LOGGER_H__
#define __NET_LOGGER_H__

#include <sys/types.h>			/* socket, bind, accept */
#include <sys/socket.h>			/* socket, bind, accept, setsockopt, */

#include <asm/types.h>

#include <sys/fcntl.h>

#include <sys/select.h>



#include <string.h>

#include <unistd.h>

#include <errno.h>

#include <stdarg.h>

#include <syslog.h>

#include <stdio.h>		/* stdio               */

#include <stdlib.h>	
//#include <rtk/utility.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>

#include <syslog.h>
#include <fcntl.h>
#include <ctype.h>
#include <net/if.h>
#include <signal.h>

#define NETLOGSTART 	1
#define NETLOGSTOP 		2
#define NETLOGINPUT 	3
#define NETLOGPOLL          4
#define NETLOG_RECV_BUF_SIZE  64
#undef NETLOG_DEBUG     
#define NETLOGPORT 0x1234
struct netlogIO
{
        unsigned int buflen;
	 char netlog_recv_buf[NETLOG_RECV_BUF_SIZE];
    
};

#define NLGSETSTATE             0x5495
#define NLGGETSTATE             0x5496
#define NLGSENTDATA     0x5497
#define NLGSENDCMD         0x5498
#define NLGSTART              0x5499
#define NLGSTOP                0x549a
#define NLGSETNETLOGGER  0x549b
#define NLGINPUT                 0x549c
#define NLGSENDPOLL           0x549d
#define NLGSETIP              0x549e 
#define NLGSETMAC           0x549f
#endif /*__NET_LOGGER_H__*/

