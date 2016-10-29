#ifndef	_NETINET_IN_H
#define	_NETINET_IN_H	1

#include "glibc-bugs.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/in.h>


       unsigned long htonl(unsigned long hostlong);

       unsigned short htons(unsigned short hostshort);

       unsigned long ntohl(unsigned long netlong);

       unsigned short ntohs(unsigned short netshort);
       
#define SOL_IP	0

#endif	/* netinet/in.h */
