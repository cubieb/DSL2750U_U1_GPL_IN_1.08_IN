/* -- updatedd: get_connection.h --
 *
 * Copyright (C) 2002, 2003, 2004 Philipp Benner
 *
 * This file is part of UpdateDD - http://updatedd.philipp-benner.de.
 *
 * UpdateDD is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * UpdateDD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UpdateDD; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

//-------------------------------------------------------------------------------------
// Copyright 2005, Texas Instruments Incorporated
//
// This program has been modified from its original operation by Texas Instruments
// to do the following:
// 
// 1. NSP Policy Routing Framework
//
// THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
// "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS 
// OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY 
// PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR 
// DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS, 
// COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS. 
// See The GNU General Public License for more details.
//
// These changes are covered under version 2 of the GNU General Public License, 
// dated June 1991.
//-------------------------------------------------------------------------------------

#ifndef GET_CONNECTION_H
#  define GET_CONNECTION_H

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/sockios.h>
#include <linux/if.h>
#include <errno.h>
//#include <linux/pr.h>
#include <stdio.h>

static inline int get_connection(const char *hostname, const char *iface, const int port, const char **err)
{

	struct	sockaddr_in addr;
	struct	hostent *host;
	int s;

	struct  ifreq ifr;
	struct sockaddr_in ifaddr;
		
//    	unsigned int pr_mark = UPDATEDD_PR_MARK;

    /* Adding an option which allows to pass a mark value for a particular
     * socket. */


	addr.sin_family	=	AF_INET;
	addr.sin_port	=	htons(port);
	if(inet_aton(hostname, &(addr.sin_addr)) ==0)
	{
	
		
			if((host = gethostbyname(hostname)) == NULL) {
				*err = "gethostbyname() failed.namesrverr";
				return -1;
			}
		#if 0
			if((host = ti_gethostbyname(hostname, pr_mark)) == NULL) {
				*err = "ti_gethostbyname() failed.namesrverr";
				return -1;
			}
		#endif
			addr.sin_addr = *(struct in_addr*)host->h_addr;
	}	

	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == -1) {
		printf("socket() failed : %d",errno);
		*err = "socket() failed";
		return -1;
	}
	
	if(iface != NULL)
	{
	        strcpy(ifr.ifr_name,iface);
		if(ioctl(s, SIOCGIFADDR, (void*)&ifr) < 0)
		{
			printf("Error: Unable to get interface address for %s Error=%d\n",ifr.ifr_name, errno);
			return -1;
		}
        
	        memcpy((void*)&ifaddr,(void*)(struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr,sizeof(ifaddr));

		if(bind(s, (struct sockaddr_in*)&ifaddr, sizeof(ifaddr))< 0)
		{
			printf("Error: Unable to bind the socket to the iterface\n");
			return -1;
		}
	
		printf("Socket bound to interface %s\n",iface);	
	}

	if(connect(s, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		printf("connect() failed : %s\n",strerror(errno));	
		*err = "connect() failed";
		return -1;
	}

	return s;

}

#endif /* GET_CONNECTION_H */
