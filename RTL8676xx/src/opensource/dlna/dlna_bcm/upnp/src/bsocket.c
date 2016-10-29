/***************************************************************************
*     (c)2008-2009 Broadcom Corporation
*  
*  This program is the proprietary software of Broadcom Corporation and/or its licensors,
*  and may only be used, duplicated, modified or distributed pursuant to the terms and
*  conditions of a separate, written license agreement executed between you and Broadcom
*  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
*  no license (express or implied), right to use, or waiver of any kind with respect to the
*  Software, and Broadcom expressly reserves all rights in and to the Software and all
*  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
*  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
*  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
*   
*  Except as expressly set forth in the Authorized License,
*   
*  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
*  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
*  and to use this information only in connection with your use of Broadcom integrated circuit products.
*   
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
*  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
*  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
*  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
*  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
*  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
*  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
*  USE OR PERFORMANCE OF THE SOFTWARE.
*  
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
*  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
*  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
*  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
*  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
*  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
*  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
*  ANY LIMITED REMEDY.
* 
***************************************************************************/
#include "upnp_priv.h"
#include "debug.h"
#include "bsocket.h"

SOCKET BSocket_CreateTcpListener(const char* address, uint16_t port)
{
    SOCKET s;
    int32_t enable;
    struct sockaddr_in sockaddr;

    s = socket( AF_INET, SOCK_STREAM, 0);
    if (s < 0) 
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateTcpListener - socket error"));
        return 0;
    }

    enable = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&enable, sizeof(enable));

    memset( &sockaddr, 0, sizeof( struct sockaddr_in ) );
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr( address );
    sockaddr.sin_port = htons( port );

    if ( bind(s, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0 )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateTcpListener - bind error"));
        goto error; 
    }

    if ( listen(s, 10) )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateTcpListener - listen error"));
        goto error;
    }

    return s;

error:
    closesocket(s);
    return 0;
}

SOCKET BSocket_CreateUdpListener(const char* address, uint16_t port)
{
    struct in_addr  inaddr;
    struct ifreq    ifreq;
    struct sockaddr_in sockAddr;
    struct ip_mreqn mcastAddr;
    const char ttl = 4;
    SOCKET s;
    int optVal = 1;

    s = socket( AF_INET, SOCK_DGRAM, 0 );
    if (s <= 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - socket error"));
        return 0;
    }

    memset(&ifreq, 0, sizeof(ifreq));
    strcpy(ifreq.ifr_name, "br0");
    if (ioctl(s, SIOCGIFADDR, &ifreq) < 0) 
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - can't get interface address"));
        goto error;
    } 

        memcpy(&inaddr, &(((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr), sizeof(struct in_addr));

    /* make sure this interface is capable of MULTICAST...*/
    if (ioctl(s, SIOCGIFFLAGS, (int) &ifreq)) 
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - can't get interface flags"));
        goto error;
    }

    if ((ifreq.ifr_flags & IFF_MULTICAST) == 0) 
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - interface doesn't support multicast"));
        goto error;
    }  

    if ( setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*) &optVal, sizeof(optVal)) != 0 )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - setsockopt error"));
        goto error;
    }

    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(port);
    if ( bind(s, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) != 0 ) 
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - bind error"));
        goto error;
    }

    if (ioctl(s, SIOCGIFINDEX, &ifreq))
    { 
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - can't get interface index"));
            goto error;
    }

    memset( ( void * )&mcastAddr, 0, sizeof( struct ip_mreqn ) );
    mcastAddr.imr_multiaddr.s_addr = inet_addr( address );
    mcastAddr.imr_address.s_addr = sockAddr.sin_addr.s_addr;
    mcastAddr.imr_ifindex = ifreq.ifr_ifindex;
    if( setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, ( char * )&mcastAddr, sizeof( struct ip_mreqn ) ) != 0 )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - setsockopt error"));
        goto error;
    }

    setsockopt( s, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof( ttl ) );
    if( setsockopt(s, SOL_SOCKET, SO_BROADCAST,    ( char * )&optVal, sizeof( optVal ) ) != 0 )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - setsockopt error"));
        goto error;
    }

    /* restrict multicast messages sent on this socket to only go out this interface and no other */
    if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF, (char*)&inaddr, sizeof(struct in_addr)) != 0 ) 
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUdpListener - setsockopt error"));
        goto error;
    }
     
    return s;

 error:
    closesocket(s);
    return 0;
}

SOCKET BSocket_CreateTcpClient(const char* address, uint16_t port)
{
    SOCKET s;
    struct sockaddr_in sockaddr;
    struct linger optval;

    s = socket( AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateTcpClient - socket error"));
        return 0;
    }

    optval.l_onoff = 1;
    optval.l_linger = 0;
    setsockopt(s, SOL_SOCKET, SO_LINGER, (const char*)&optval, sizeof (optval));

    memset( &sockaddr, 0, sizeof( struct sockaddr_in ) );
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr( address );
    sockaddr.sin_port = htons( port );

    if (connect(s, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_in)) == -1)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateTcpClient - connect error"));
        return 0;
    }
    
    return s;
}

SOCKET BSocket_CreateUpdClient(const char* address)
{
    SOCKET s;
    const char ttl = 4;
    unsigned long addr = inet_addr(address);
    struct in_addr iface_addr;
#ifdef WIN32
    const char option = 1;
#endif

    /* set the interface address */
    iface_addr.s_addr = inet_addr(address);

    s = socket( AF_INET, SOCK_DGRAM, 0 );
    if (s < 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUpdClient - socket error"));
        return 0;
    }

    /* Set Time-to-Live */
    if (setsockopt( s, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof( ttl ) ) != 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUpdClient - setsockopt error"));
        goto error;
    }

    if ( setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF, (char*)&addr, sizeof(addr)) != 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUpdClient - setsockopt error"));
        goto error;
    }

    /* These might not be needed at all */
#ifdef WIN32
    if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &option, sizeof(option)))
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUpdClient - setsockopt error"));
        goto error;
    }

    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &option, sizeof(option)) != 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("BSocket_CreateUpdClient - setsockopt error"));
        goto error;
    }
#endif
    return s;

error:
    if ( s >= 0 )
        closesocket(s);

    return 0;
}
