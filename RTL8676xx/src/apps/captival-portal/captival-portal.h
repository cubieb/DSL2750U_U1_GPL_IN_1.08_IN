/**
* scb+ 2011-10-12
*
* file name: captival-portal.h
*
* implement the captival portal function.
*
* this implement need not modify  the httpd and dns.
*
* this modul need the lib file:
*	3g-lib-cmd-msg.c and 3g-lib-cmd-msg.h
*
*/

#ifndef __CAPTIVAL_PORTAL_H__
#define __CAPTIVAL_PORTAL_H__


#include <sys/types.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <paths.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <3g-lib-cmd-msg.h>

#if !defined(HAVE_DEFINE_TYPE)
typedef		unsigned int 		uint32_t;
typedef  	unsigned short int  uint16_t;
#endif

#include "timer.h"
#include "dns.h"
#include "httpd.h"
#include "iptables.h"

#define CATIVAL_PORTAL_UN_SOCKET_PATH "/var/cplpl_socket_path"
#define CAPTIVAL_PORTAL_DEBUG_FLAG_FILE "/var/cplpl_debug"

#define CPLPL_MAXPATHLEN 256

#define DNS_PORT	5053
#define HTTP_PORT	5080
#define RAW_DNS_PORT	53
#define RAW_HTTP_PORT	80
#define DNS_ADDR_START	"10.0.2.0"
#define DNS_ADDR_END	"10.0.2.255"

#define TIMER_TOKEN_LEN	80
#define IP4_ADDR_LEN	32
#define INTF_NAME_LEN	32
#define CMD_BUF_LEN	1024

#define is_debug()  (access(CAPTIVAL_PORTAL_DEBUG_FLAG_FILE, F_OK) == 0)

#define cplpl_printf(fmt, args...) \
	printf("[CPLPL:%s %d ]: " fmt, __FILE__, __LINE__, ##args);

#define d_printf(args...)	\
	do {	\
		if (is_debug())	\
			cplpl_printf(args);	\
	}while(0)

extern int   g_need_exit;
extern int   g_demand_exit;

extern char g_local_host[IP4_ADDR_LEN];
extern char g_local_mask[IP4_ADDR_LEN];
extern char g_local_intf[INTF_NAME_LEN];	

char *cplpl_sock_addr_to_p(void *data);
#endif

