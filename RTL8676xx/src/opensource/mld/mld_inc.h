#ifndef _MLD_INC_H_
#define _MLD_INC_H_

/******************************************************************************
*                     Global Includes 
*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/param.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <bits/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <netinet/if_ether.h>
#include <net/if.h>

#include <netinet/in_systm.h>
#include <linux/ipv6.h>
#include <linux/mroute6.h>
#include <linux/icmpv6.h>
#include <linux/in6.h>
#include <linux/in.h>


#include <syslog.h>
#include <assert.h>

#include <ctype.h>
#include <stdarg.h>

/******************************************************************************
*                    Global definisions 
*******************************************************************************/

typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;

extern int log_level;


/******************************************************************************
*                    Global Macro definisions 
*******************************************************************************/
#ifdef DEBUG
#define LOG(x) debug x
#else
#define LOG(x)
#endif

#define INET6_ADDRSTRLEN   46

#define IN6_MULTICAST(x)   (IN6_IS_ADDR_MULTICAST(x))
#define VALID_ADDR(x)  (IN6_IS_ADDR_MULTICAST(x))

#define IN6_IS_ADDR_UNSPECIFIED(a) \
	(((__const uint32_t *) (a))[0] == 0				      \
	 && ((__const uint32_t *) (a))[1] == 0				      \
	 && ((__const uint32_t *) (a))[2] == 0				      \
	 && ((__const uint32_t *) (a))[3] == 0)

#define IN6_IS_ADDR_LOOPBACK(a) \
	(((__const uint32_t *) (a))[0] == 0				      \
	 && ((__const uint32_t *) (a))[1] == 0				      \
	 && ((__const uint32_t *) (a))[2] == 0				      \
	 && ((__const uint32_t *) (a))[3] == htonl (1))

#define IN6_IS_ADDR_MULTICAST(a) (((__const uint8_t *) (a))[0] == 0xff)

#define IN6_IS_ADDR_LINKLOCAL(a) \
	((((__const uint32_t *) (a))[0] & htonl (0xffc00000))		      \
	 == htonl (0xfe800000))

#define IN6_IS_ADDR_SITELOCAL(a) \
	((((__const uint32_t *) (a))[0] & htonl (0xffc00000))		      \
	 == htonl (0xfec00000))

#define IN6_IS_ADDR_V4MAPPED(a) \
	((((__const uint32_t *) (a))[0] == 0)				      \
	 && (((__const uint32_t *) (a))[1] == 0)			      \
	 && (((__const uint32_t *) (a))[2] == htonl (0xffff)))

#define IN6_IS_ADDR_V4COMPAT(a) \
	((((__const uint32_t *) (a))[0] == 0)				      \
	 && (((__const uint32_t *) (a))[1] == 0)			      \
	 && (((__const uint32_t *) (a))[2] == 0)			      \
	 && (ntohl (((__const uint32_t *) (a))[3]) > 1))

#define IN6_ARE_ADDR_EQUAL(a,b) \
	((((__const uint32_t *) (a))[0] == ((__const uint32_t *) (b))[0])     \
	 && (((__const uint32_t *) (a))[1] == ((__const uint32_t *) (b))[1])  \
	 && (((__const uint32_t *) (a))[2] == ((__const uint32_t *) (b))[2])  \
	 && (((__const uint32_t *) (a))[3] == ((__const uint32_t *) (b))[3]))


#define IN6_IS_ADDR_MC_NODELOCAL(a) \
	(IN6_IS_ADDR_MULTICAST(a)					      \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0x1))

#define IN6_IS_ADDR_MC_LINKLOCAL(a) \
	(IN6_IS_ADDR_MULTICAST(a)					      \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0x2))

#define IN6_IS_ADDR_MC_SITELOCAL(a) \
	(IN6_IS_ADDR_MULTICAST(a)					      \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0x5))

#define IN6_IS_ADDR_MC_ORGLOCAL(a) \
	(IN6_IS_ADDR_MULTICAST(a)					      \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0x8))

#define IN6_IS_ADDR_MC_GLOBAL(a) \
	(IN6_IS_ADDR_MULTICAST(a) \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0xe))


/******************************************************************************
*                    Structure definisions 
*******************************************************************************/
typedef struct _interface_list_t 
{
    char ifl_name[IFNAMSIZ];
    struct sockaddr ifl_addr;
    struct sockaddr_in6 ifl_addr6;
    struct _interface_list_t *ifl_next;
    short ifl_index; /* lo=1, eth0=2,usb=3,br0=4,nas=5 and etc*/
    short usPad;
} interface_list_t;



#endif /* _MLD_INC_H_ */
