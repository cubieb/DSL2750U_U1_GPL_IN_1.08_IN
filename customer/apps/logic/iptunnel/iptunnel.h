/******************************************************************************
  Copyright (c), 1991-2010, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
  
  文件名称：iptunnel.h
  文件描述：iptunnel模块头文件
  修订记录：
         1. 作者：lbw
            日期：2011-06-15
            内容：创建文件
******************************************************************************/
#ifndef _IPTUNNEL_H_
#define _IPTUNNEL_H_


#include <crypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "common.h"
#include "new_msg.h"
#include "tbserror.h"
#include "common_func.h"

/******************************************************************************
*                                   MACRO                                     *
******************************************************************************/

#ifdef  IPTUNNEL_DEBUG
#define IPTunnel_TRACE(fmt, args...)                              COMMON_TRACE(MID_IPTUNNEL, fmt"\n", ##args)
#else
#define IPTunnel_TRACE(str, args...)
#endif

#define IPTunnel_FUNC_IN(fmt, args...)                            IPTunnel_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define IPTunnel_FUNC_OUT()                                       IPTunnel_TRACE("--O %s\n", __FUNCTION__)

#define IPTunnel_LOG_ERR(fmt, args...)                            COMMON_LOG_ERROR(MID_IPTUNNEL, fmt, ##args)

#define CALL_FUCTION_FAILED "Call function \"%s\" failed\n"

#define IPTunnel_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_IPTUNNEL, iRet, strError, ##args)
#define IPTunnel_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_IPTUNNEL, pRet, strError, ##args)

#define IPTUNNEL_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_IPTUNNEL, iRet, strError, ##args)
#define IPTUNNEL_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_IPTUNNEL, pRet, strError, ##args)

#define IPTunnel_RET_GOTO(iRet, strError, gotoTag)                RET_GOTO(MID_IPTUNNEL, iRet, strError, gotoTag)
#define IPTunnel_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_IPTUNNEL, pRet, gotoTag ,strError, ##args)

#define IPTunnel_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_IPTUNNEL, iRet, strError, ##args)
#define IPTunnel_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_IPTUNNEL, pRet, strError, ##args)

#define ERR_GET(node)                                           "Get Node["node"] value failed.\n"
#define ERR_SET(node)                                           "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)                                       "Set Node["node"] value failed, value = %s\n"


#define MAX_NAME_LEN                        64
#define MAX_IPV6_LEN 50
#define DSLITE_AFTR_FILE "/var/dhcpv6/dslite_aftr.conf"

/* 节点个数节点名称定义*/
#define NODE_IPTunnel_Numbers                                   "X_TWSZ-COM_IPTunnelNumberOfEntries"
#define NODE_4in6_Numbers                                       "4in6TunnelNumberOfEntries"
#define NODE_6in4_Numbers                                       "6in4TunnelNumberOfEntries"


/* 公共节点名称定义 */
#define NODE_IPTunnel_Activated                                 "Activated"
#define NODE_IPTunnel_TunnelName                                "TunnelName"
#define NODE_IPTunnel_TunnelCounter                              "TunnelCounter"
#define NODE_IPTunnel_Mode                                      "Mode"
#define NODE_IPTunnel_Counter                                   "TunnelCounter"
#define NODE_IPTunnel_AssociatedWanIfName                       "AssociatedWanIfName"
#define NODE_IPTunnel_AssociatedLanIfName                       "AssociatedLanIfName"
#define NODE_IPTUNNEL_SELECT                                    "Select"


/* 4in6Tunnel实例节点名称定义 */
#define NODE_4in6Tunnel_ConnStatus                              "ConnStatus"
#define NODE_4in6Tunnel_Mechanism                               "Mechanism"
#define NODE_4in6Tunnel_Dynamic                                 "Dynamic"
#define NODE_4in6Tunnel_RemoteIpv6Address                       "RemoteIpv6Address"


/* 6in4Tunnel实例节点名称定义 */
#define NODE_6in4Tunnel_ConnStatus                              "ConnStatus"
#define NODE_6in4Tunnel_Mechanism                               "Mechanism"
#define NODE_6in4Tunnel_Dynamic                                 "Dynamic"
#define NODE_6in4Tunnel_IPv4MaskLen                             "IPv4MaskLen"
#define NODE_6in4Tunnel_Prefix                                  "Prefix"
#define NODE_6in4Tunnel_BorderRelayAddress                      "BorderRelayAddress"


/* Wan and Lan节点名称定义 */
#define NODE_IPV6RD_WAN_IP                                      "ExternalIPAddress"
#define NODE_IPV6RD_LAN_IP                                      "X_TWSZ-COM_IPv6InterfaceAddress"

/* 节点路径定义 */
#define IPTunnel_PATH                                           "InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel."
#define DSLITE_PATH                                           "InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.{i}.4in6Tunnel."
#define IPV6RD_PATH                                           "InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.{i}.6in4Tunnel."

/* 路径正则表达式定义 */
#define IPTunnel_REGEX_DEL                                          "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPTunnel\\.[0-9]{1,5}\\.$"
#define IPTunnel_4in6Tunnel_REGEX_DEL                               "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPTunnel\\.[0-9]{1,5}\\.4in6Tunnel\\.[0-9]+\\.$"
#define IPTunnel_6in4Tunnel_REGEX_DEL                               "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPTunnel\\.[0-9]{1,5}\\.6in4Tunnel\\.[0-9]+\\.$"

#define IPTunnel_REGEX_ADD                                          "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPTunnel\\.$"
#define IPTunnel_4in6Tunnel_REGEX_ADD                               "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPTunnel\\.[0-9]{1,5}\\.4in6Tunnel\\.$"
#define IPTunnel_6in4Tunnel_REGEX_ADD                              "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPTunnel\\.[0-9]{1,5}\\.6in4Tunnel\\.$"

#define IPTunnel_REGEX                                          "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPTunnel\\.[0-9]{1,5}\\."
#define IPTunnel_4in6Tunnel_REGEX                               "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPTunnel\\.[0-9]{1,5}\\.4in6Tunnel\\.[0-9]+\\."
#define IPTunnel_6in4Tunnel_REGEX                              "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPTunnel\\.[0-9]{1,5}\\.6in4Tunnel\\.[0-9]+\\."

#define REGEX_IPT(node)                                         IPTunnel_REGEX node "$"
#define REGEX_4in6(node)                                        IPTunnel_4in6Tunnel_REGEX node "$"
#define REGEX_6in4(node)                                        IPTunnel_6in4Tunnel_REGEX node "$"

/* LAN IPv6节点路径 */
#define LAN_IPV6_PATH          "InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement."
#define LAN_IPV6_RADVD         "RadVDConfigManagement"
#define LAN_IPV6_DHCPV6        "ServerType.StatefullDHCPv6"
#define LAN_IPV6_SLAAC_ENABLE  "ServerType.Stateless"
#define LAN_IPV6_DHCPV6_ENABLE "ServerType.Stateless"
#define LAN_IPV6_PREFIX_TYPE   "IPv6SitePrefixConfigType.StaticDelegated"
#define LAN_IPV6_ULA_PREFIX    "ULAPrefixInfo.Prefix"

#define IPV6_LIST                           "X_TWSZ-COM_IPv6Config.IPv6AddressList.1.IPv6Address"
#define LAN_LIST                            "LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"
#define IPV6_DEV_NAME "X_TWSZ-COM_IPv6Config.X_TWSZ-COM_ConnectedDeviceName"
#define IPV6_DEFAULT_ROUTER_LST "X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList"
#define IPV6_DEFAULT_ROUTER "DefaultRouter"
#define IPV6_ROUTER_CFG_TYPE "ConfigType"
#define IPV6_EXTERNAL_ADDRESS "X_TELEFONICA-ES_ExternalIPv6Address"

#define LAN_IPV6_ENABLE  "1"
#define LAN_IPV6_DISABLE "0"

//#define LAN_IPV6_STATIC    "Static"
//#define LAN_IPV6_DELEGATED "Delegated"

#define LAN_IPV6_STATIC    "1"
#define LAN_IPV6_DELEGATED "0"

#define FOR_EACH_LIST(pos, list) for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)

/******************************************************************************
*                            FUNC DECLEARATION                                *
******************************************************************************/
/* Init && Destroy */
int IPTunnel_ModuleInit();
int IPTunnel_ModuleDestroy();

#endif

