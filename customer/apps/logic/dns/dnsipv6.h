/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: dnsipv6.h

 文件描述: dns模块的头文件

 修订记录:
           1 作者: 胡煜
             日期: 2009-12-18
             描述: 创建
           2 作者: 陈凯
             日期: 2011-09-22
             描述: 重构
**********************************************************************/
#ifndef _DNS_IP6_H_
#define _DNS_IP6_H_

#include "tbsutil.h"

#define CALL_FUCTION_FAILED "Call function \"%s\" failed\n"

#define DNS_RET_RETURN(iRet, strError, args...) RET_RETURN(MID_DNS, iRet, strError, ##args)
#define DNS_POINTER_RETURN(pRet, strError, args...) POINTER_RETURN(MID_DNS, pRet, strError, ##args)

/* IPv6 */
#define DPROXY_FILE_BAK "/var/dproxy.conf.bak"
#define RESOLVER_FILE_BAK "/var/resolv.conf.bak"
#define HOSTS_FILE_BAK "/var/hosts.bak"

#define MAX_IPv6_LEN_DNS 40
#define MAX_PROC_NAME_LEN 16
#define DEV_NAME_LEN 20

#define DNSIPV6_TYPE_STATIC "Static"
#define DNSIPV6_TYPE_DHCP "DHCP"

#define NODE_IPv6DNSEnabled "X_TWSZ-COM_IPv6Config.IPv6DNSEnabled"
#define NODE_IPv6DNSOverrideAllowed "X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed"
#define NODE_IPv6DNSServers "X_TWSZ-COM_IPv6Config.IPv6DNSServers"
#define NODE_UsrIPv6DNSServers "X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers"
#define NODE_DNSIPv6ConnectionStatus "X_TWSZ-COM_IPv6Config.ConnectionStatus"
#define NODE_IPV6_DEVICE_NAME "X_TWSZ-COM_IPv6Config.X_TWSZ-COM_ConnectedDeviceName"
#define NODE_IPV6_DNS_CONNECTION "IPv6DNSWanConnection"
#define NODE_CONNECTION_NAME "Name"

#define NODE_IPv6DNSRelayEnable "IPv6DNSRelayEnable"
#define NODE_IPV6_DNS_TYPE "IPv6DNSConfigType"
#define NODE_IPV6_DNS_SERVERS "IPv6DNSServers"
#define NODE_IPv6DNSConfigType "ServerType.StatefullDHCPv6.IPv6DNSConfigType"
#define NODE_IPv6_DNS_Servers  "ServerType.StatefullDHCPv6.IPv6DNSServers"

#define PATH_LAN_IPV6_HOST_CM "InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement."
#define PATH_LAN_ENABLE "InternetGatewayDevice.LANDevice.%.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.StatefullDHCPv6.IPv6DNSRelayEnable"
#define PATH_LAN_TYPE "InternetGatewayDevice.LANDevice.%.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.StatefullDHCPv6.IPv6DNSConfigType"
#define PATH_LAN_SERVER "InternetGatewayDevice.LANDevice.%.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.StatefullDHCPv6.IPv6DNSServers"
#define PATH_IPV6DNS_STATEFULL "InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.StatefullDHCPv6."

#define PATH_WAN_IPV6_CONN "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.WANIPConnection.%lu.X_TWSZ-COM_IPv6Config."
#define PATH_WAN_PPPV6_CONN "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.WANPPPConnection.%lu.X_TWSZ-COM_IPv6Config."

#define MID_PATH_WAN_IPV6_CONN "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_TWSZ-COM_IPv6Config."
#define MID_PATH_WAN_PPPV6_CONN "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}.X_TWSZ-COM_IPv6Config."

#define DNSIPV6_STATEFULL_BASE_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IPv6LANHostConfigManagement\\.ServerType\\.StatefullDHCPv6\\."
#define DNSIPV6_NODE_REGEX(node) DNSIPV6_STATEFULL_BASE_PATH_REGEX node

/* 存放LAN口绑定信息的结构体 */
typedef struct ST_IPv6LANDEV_INFO
{
    char szDevName[DEV_NAME_LEN];   /* 设备名称 */
    char bFlag;                     /* 是否绑定 */
    struct ST_IPv6LANDEV_INFO *pNext;
} ST_IPv6LANDEV_INFO;

int DNS_IPv6_IsAddrExisted(const char *pszAddress, const char *pszFileName);
int DNS_IPv6_RewriteResolv();
int DNS_DefaultWriteConf(char *pszDevice, char *pszData);
int DNS_IPv6_WriteResConf(const char *pszDnsList, const char *pszData);
int DNS_IPv6_WriteHostsConf(char *pszPath, void *pFile);
int DNS_IPv6_WriteToConf(char *pszPath, void *pData);
int DNS_IPv6_WriteConf();
int DNS_MsgIP6WANChgHandler(ST_MSG *pstMsg);
int DNS_Ip6ServersApply(const char *pszPath, const char *pszValue);

#endif

