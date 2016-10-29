/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: dhcp6s.h

 文件描述: dhcp6s模块的头文件

 修订记录:
           1 作者 : 胡煜
             日期 : 2009-12-22
             描述 : 创建
**********************************************************************/

#ifndef _DHCP6S_H_
#define _DHCP6S_H_


#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
/******************************************************************************
*                                 MACRO                                       *
******************************************************************************/

#define CALL_FUCTION_FAILED "Call function \"%s\" failed\n"

#define DHCP6S_RET_RETURN(iRet, strError, args...) RET_RETURN(MID_DHCP6S, iRet, strError, ##args)
#define DHCP6S_POINTER_RETURN(pRet, strError, args...) POINTER_RETURN(MID_DHCP6S, pRet, strError, ##args)

#define DHCP6S_RET_GOTO(iRet, strError, gotoTag) RET_GOTO(MID_DHCP6S, iRet, strError, gotoTag)
#define DHCP6S_POINTER_GOTO(pRet, gotoTag, strError, args...) POINTER_GOTO(MID_DHCP6S, pRet, gotoTag ,strError, ##args)

#define DHCP6S_TRACE_INTO_FUNC DHCP6S_TRACE("##In## %s\n", __func__)
#define DHCP6S_TRACE_OUT_FUNC DHCP6S_TRACE("##Out## %s\n", __func__)

#define FOR_EACH_LIST(pos, list) for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)

/* 日志打印宏 */
#ifdef DHCP6S_DEBUG
#define DHCP6S_TRACE(fmt, args...)        COMMON_TRACE(MID_DHCP6S, fmt "\n", ##args)
#else
#define DHCP6S_TRACE(fmt, args...)
#endif
#define DHCP6S_LOG_ERR(fmt, args...)       COMMON_LOG_ERROR(MID_DHCP6S, fmt, ##args)
#define MAX_HOST_NAME_LEN 65
#define MAX_LAN_DEV_NAME_LEN 30                         /* Lan 设备名最大长度 */
#define MAX_IPV6_LEN 50
#define MAX_PROC_NAME_LEN 16


#define NODE_DEVICE_NAME "X_TWSZ-COM_DeviceName"
#define NODE_IPV6_DOMAIN "IPv6DomainName"
#define NODE_IPV6_ENABLE "AutoConfigurationAddress"
#define NODE_STATELESS "Stateless"
#define NODE_IPv6AddScope "IPv6InterfaceAddressScope"
#define NODE_IPv6Address "X_TWSZ-COM_IPv6InterfaceAddress"
#define NODE_MIN_INTERFACE_ID "MinInterfaceID"
#define NODE_MAX_INTERFACE_ID "MaxInterfaceID"
#define NODE_DHCPV6_LEASE_TIME "DHCPv6LeaseTime"
#define NODE_IPV6_DNS_TYPE "IPv6DNSConfigType"
#define NODE_LAN_DNS_ENABLE "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_UseIPRoutersAsDNSServer"

#define DHCP6S_SERVERTYPE_CFG "ServerType."
#define DHCP6S_STATEFULL_CFG "StatefullDHCPv6."

#define PATH_LAN_CONN			"InternetGatewayDevice.LANDevice.%lu"
#define PATH_DHCP6S_CONN		"InternetGatewayDevice.LANDevice.%lu.X_TELEFONICA-ES_IPv6LANHostConfigManagement.DHCPv6ServerConfig."
#define PATH_DHCP6S_ENABLE	"InternetGatewayDevice.LANDevice.%lu.X_TELEFONICA-ES_IPv6LANHostConfigManagement.DHCPv6ServerConfig.Enable"
#define PATH_DHCP6S	"InternetGatewayDevice.LANDevice.%lu.X_TELEFONICA-ES_IPv6LANHostConfigManagement"


#define MID_PATH_DHCP6S_CONN	"InternetGatewayDevice.LANDevice.{i}.X_TELEFONICA-ES_IPv6LANHostConfigManagement.DHCPv6ServerConfig."

#define BASE_PATH_DHCP6S_ENABLE "InternetGatewayDevice.LANDevice.%.X_TELEFONICA-ES_IPv6LANHostConfigManagement.AutoConfigurationAddress"
#define BASE_PATH_DHCP6S_NAME	"InternetGatewayDevice.LANDevice.%.X_TELEFONICA-ES_IPv6LANHostConfigManagement.DHCPv6ServerConfig.IPv6DomainName"


#define NODE_IPV6_DNS_SERVERS "IPv6DNSServers"
#define NODE_IPv6DNSRelayEnable "IPv6DNSRelayEnable"

#define WAN_IPCONN_INST_MATCH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define WAN_PPPCONN_INST_MATCH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define PATH_LAN_ENABLE "InternetGatewayDevice.LANDevice.%.X_TELEFONICA-ES_IPv6LANHostConfigManagement.IPv6DNSRelayEnable"
#define PATH_LAN_SERVER "InternetGatewayDevice.LANDevice.%.X_TELEFONICA-ES_IPv6LANHostConfigManagement.IPv6DNSServers"

#define RADVD_DELEGATED_PREFIX_BASE_PATH "RadVDConfigManagement.PrefixInformation."
#define RADVD_PREFIX_CONNECTION "DelegatedConnection"
#define RADVD_PD_BASE_PATH "IPv6PDWanConnection."
#define RADVD_PD_INTERFACE_NODE "Interface"
#define RADVD_PREFIX_NODE "Prefix"
#define RADVD_ULAPREFIX_PATH "RadVDConfigManagement.ULAPrefixInfo."
#define RADVD_PREFIX_TYPE "IPv6SitePrefixConfigType.StaticDelegated"
#define WAN_CONNECTION_NAME "Name"
#define WAN_IPV6_SITE_PREFIX "X_TELEFONICA-ES_IPv6SitePrefix"
/*
 * ipv6 enable/disable 节点
 */
#ifdef CONFIG_APPS_LOGIC_IPV6_ENABLE
#define IPV6_ENABLE_NODE "InternetGatewayDevice.X_TWSZ-COM_IPv6Enable.Enable"
#define COMMIT_IPV6_SET_ENABLE "InternetGatewayDevice\\.X_TWSZ-COM_IPv6Enable\\.Enable"
#endif

int DHCP6S_ModuleInit();
int DHCP6S_ModuleDestroy();
#endif

/******************************************************************************
*                                 END                                         *
******************************************************************************/

