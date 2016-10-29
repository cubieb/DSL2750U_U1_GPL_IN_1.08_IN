#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "dns_io.h"
#include "dns_decode.h"
#include "string.h"

extern ST_LAN_INFO g_defaultLanInfo;

/*****************************************************************************/
int dns_read_packet(int sock, dns_request_t *m, int protocol, unsigned char ucFlag)
{
    unsockaddr unsa;
    socklen_t salen;
    char szRecvDev[64] = {0};
    ST_LAN_INFO *pLanInfo;
    /* Read in the actual packet */
    salen = sizeof(unsa);
    char addr[64] = {0};
    unsigned int LanPortMark = 0;
    unsigned long routeRmark = 0;
    int marklen = sizeof(unsigned int) + sizeof(unsigned long);
    int ret;
  
    m->numread = recvfrom(sock, m->original_buf, sizeof(m->original_buf), 0,
		                  (struct sockaddr *)&unsa, &salen);  
    if ( m->numread < 0) {
        debug_perror("dns_read_packet: recvfrom error\n");
        return -1;
    }


    /*Get device name only from the packets from the LAN side.*/
	if (ucFlag)
	{
          memcpy((unsigned char *)&LanPortMark, m->original_buf, sizeof(unsigned int));
          
          debug("LanPortMark=%d, original_buf=%s \n", LanPortMark, m->original_buf);
          /*0xffffff means the packet had been expand*/
          if(0xffffff00 == LanPortMark)
          {
             //printf("...........dns from lan....%d.. \n",LanPort & 0xf);
             memcpy((unsigned char *)&routeRmark, (m->original_buf)+sizeof(unsigned int), sizeof(unsigned long));
   		  
             memmove(m->original_buf, m->original_buf+marklen, m->numread-marklen);
             m->numread -= marklen;
             sprintf(szRecvDev, "%lu", routeRmark);

	      debug("%s.%dlanport:%s\n", __FUNCTION__,__LINE__, szRecvDev);
          }
	}

    /* TODO: check source addr against list of allowed hosts */
    /* record where it came from */
    if (protocol == PROTO_IPV4) {
        inet_ntop(AF_INET, &unsa.sa_in.sin_addr, addr, sizeof(addr));
        memcpy(&m->src_addr.in_addr, &unsa.sa_in.sin_addr, sizeof(struct in_addr));
        m->src_port = ntohs(unsa.sa_in.sin_port);
        m->protocol = PROTO_IPV4;
	        /*记录lan侧报文是ipv4还是ipv6*/
        if (ucFlag)
        {
           m->srcisipv4 = 1;
        }
    }
    else
    {
        inet_ntop(AF_INET6, &unsa.sa_in6.sin6_addr, addr, sizeof(addr));
        memcpy(&m->src_addr.in6_addr, &unsa.sa_in6.sin6_addr, sizeof(struct in6_addr));
        m->src_port = ntohs(unsa.sa_in6.sin6_port);
        m->protocol = PROTO_IPV6;
	        /*记录lan侧报文是ipv4还是ipv6*/
        if (ucFlag)
        {
           m->srcisipv4 = 0;
        }
    }
    
    debug("recv packet from %s\n", addr);
    debug("flag = %d\n", ucFlag);
  	if (ucFlag)
  	{
	  	debug("lanport list dev:%s, len = %d\n", szRecvDev, strlen(szRecvDev));
		for (pLanInfo = g_stConfig.pLanInfo; pLanInfo; pLanInfo = pLanInfo->pNext)
		{			
			if (!strncmp(pLanInfo->szName, szRecvDev, strlen(pLanInfo->szName)))
			{
				debug("find %s\n", szRecvDev);
				break;
			}
		}
		
		if (!pLanInfo) /*没有找到对应的接口*/
		{
			pLanInfo = &g_defaultLanInfo;
		}
		m->pNode = pLanInfo;
		
              /*检查dns是否可用,不可用，切换 */
              if(m->protocol == PROTO_IPV4)
              {
                 if(pLanInfo &&(!(pLanInfo->ipPro & IPV4_ENABLE)))
                 {
                    m->protocol = PROTO_IPV6;
                 }
              }
              else
              {
                 if(pLanInfo &&(!(pLanInfo->ipPro & IPV6_ENABLE)))
                 {
                    m->protocol = PROTO_IPV4;
                 }
              }
       

		m->ucIndex = 0;
  	}
	
    /* check that the message is long enough */
    if( m->numread < sizeof(m->message.header) ) {
        debug("dns_read_packet: packet from '%s' to short to be dns packet\n", addr);
        return -1;
    }

    /* reset cache flag */
    m->cache=0;

    /* pass on for full decode */
    dns_decode_request( m );
    return 0;
}

/*****************************************************************************/

int dns_write_packet(int sock, unaddr *addr, int port, dns_request_t *m)
{
    unsockaddr unsa;
    int retval;

    /* Zero it out */
    memset((void *)&unsa, 0, sizeof(unsa));

    /* Fill in the information */
    if (m->protocol == PROTO_IPV4)
    {
        memcpy(&unsa.sa_in.sin_addr, &addr->in_addr, sizeof(struct in_addr));
        unsa.sa_in.sin_port = htons(port);
        unsa.sa_in.sin_family = AF_INET;
    }
    else
    {
        memcpy(&unsa.sa_in6.sin6_addr, &addr->in6_addr, sizeof(struct in6_addr));
        unsa.sa_in6.sin6_port = htons(port);
        unsa.sa_in6.sin6_family = AF_INET6;
    }

    retval = sendto(sock, m->original_buf, m->numread, 0,
		            (struct sockaddr *)&unsa.sa, sizeof(unsa));
    if( retval < 0 ) {
        debug_perror("dns_write_packet: sendto\n");
    }
    return retval;
}

