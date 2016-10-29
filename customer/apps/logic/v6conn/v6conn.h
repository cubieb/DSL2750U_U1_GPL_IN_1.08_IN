/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: v6conn.h
 文件描述: v6conn模块的头文件

 修订记录:
        1. 作者: liwei
           日期: 2009-12-23
           内容: 创建文件
        2. 作者: xuxiaodong
           日期: 2010-01-30
           内容: 重构模块
**********************************************************************/

#ifndef _V6CONN_H_
#define _V6CONN_H_

#include "warnlog.h"
/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef V6CONN_DEBUG
    #define V6CONN_TRACE(fmt, args...) COMMON_TRACE(MID_V6CONN, ECHO_GREEN fmt ECHO_NONE, ##args)
    #define V6CONN_LOG_ERR(fmt, args...) COMMON_TRACE(MID_V6CONN, fmt, ##args)
#else
    #define V6CONN_TRACE(fmt, args...)
    #define V6CONN_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_V6CONN, fmt, ##args)
#endif

#define V6CONN_TRACE_INTO_FUNC V6CONN_TRACE("##In## %s\n", __func__)

#define V6CONN_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            V6CONN_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define V6CONN_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_V6CONN, Ret, fmt, ##args)

#define V6CONN_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_V6CONN, Pointer, gotoTag, fmt, ##args)

#define V6CONN_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            V6CONN_TRACE(fmt, ##args); \
            return Ret; \
        }

#define V6CONN_ASSERT(expr) \
    do{ \
        if(!(expr))\
        {\
            V6CONN_TRACE("Assert \"%s\" failed\n", #expr); \
            exit(-1); \
        } \
    }while(0)

#define V6CONN_CHECK_NULL_PTR(ptr)   V6CONN_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")    

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/

/*------------- Telefonica IPV6 Tr069 node define -----------------------*/
#define V6CONN_TELE_WAN_ENABLE_IPV6     "X_TELEFONICA-ES_IPv6Enabled"
#define V6CONN_TELE_WAN_ENABLE_IPV4     "X_TELEFONICA-ES_IPv4Enabled"
#define V6CONN_TELE_CONN_STAT           "X_TELEFONICA-ES_IPv6ConnStatus"
#define V6CONN_TELE_ADDRING_TYPE        "X_TELEFONICA-ES_IPv6AddressingType"
#define V6CONN_TELE_IANA_ENABLE         "X_TELEFONICA-ES_DHCP6cForAddress"
#define V6CONN_TELE_IPV6_ADDR           "X_TELEFONICA-ES_ExternalIPv6Address"
#define V6CONN_TELE_ADDR_PREFIX_LEN     "X_TELEFONICA-ES_ExternalIPv6AddressPrefixLength"
#define V6CONN_TELE_DFLT_ROUTER         "X_TELEFONICA-ES_DefaultIPv6Gateway"      
#define V6CONN_TELE_IAPD_ENABLE         "X_TELEFONICA-ES_IPv6PrefixDelegationEnabled"
#define V6CONN_TELE_SITE_PREFIX         "X_TELEFONICA-ES_IPv6SitePrefix"
#define V6CONN_TELE_PREFIX_PLTIME       "X_TELEFONICA-ES_IPv6SitePrefixPltime"
#define V6CONN_TELE_PREFIX_VLTIME       "X_TELEFONICA-ES_IPv6SitePrefixVLtime"
#define V6CONN_TELE_DNS_SVRS            "X_TELEFONICA-ES_IPv6DNSServers"
#define V6CONN_TELE_ISUNNUM_MODE        "X_TELEFONICA-ES_UnnumberedModel"
#define V6CONN_TELE_PPP_IFSATUS         "X_TELEFONICA-ES_IPv6PppUp"
/*------------- Telefonica IPV6 Tr069 node define end--------------------*/

#define STR_WAN_IPCONN    "WANIPConnection"
#define STR_WAN_PPPCONN   "WANPPPConnection"
#define STR_IPV6_CONFIG   "X_TWSZ-COM_IPv6Config."

#define STR_IPV6_CONFIG_PREFIX_ADDR   "X_TWSZ-COM_IPv6Config.IPv6AddressList.1."

#define V6CONN_WAN_ENABLE     "Enable"
#define V6CONN_CONN_STAT      "ConnectionStatus"
#define V6CONN_CONN_DEVNAME   "X_TWSZ-COM_ConnectedDeviceName"
#define V6CONN_DUID_CONF_TYPE "DUIDConfigType"
#define V6CONN_DUID           "DUID"

#define V6CONN_ADDR_LIST       "IPv6AddressList."
#define V6CONN_ADDR_SCOPE      "AddressScope"
#define V6CONN_ADDRING_TYPE    "AddressingType"
#define V6CONN_IPV6_ADDR       "IPv6Address"
#define V6CONN_ADDR_PREFIX_LEN "PrefixLength"
#define V6CONN_ADDR_STATUS     "AddressStatus"
#define V6CONN_CONN_TRIGGER    "ConnectionTrigger"

#define V6CONN_DFLT_ROUTER_PATH  "X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.1."
#define V6CONN_DFLT_ROUTER_LIST "IPv6DefaultRouterList."
#define V6CONN_ROUTER_CONF_TYPE "ConfigType"
#define V6CONN_DFLT_ROUTER      "DefaultRouter"
#define V6CONN_ROUTER_STAT      "RouterStatus"

#define V6CONN_PREFIX_INFO       "IPv6SitePrefixInfo."
#define V6CONN_PREFIX_INFO_PATH  "X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo."

#define V6CONN_SITE_PREFIX       "IPv6SitePrefix"
#define V6CONN_SITE_PREFIX_LEN   "IPv6SitePrefixLength"
#define V6CONN_PREFIX_PLTIME     "PreferredLifeTime"
#define V6CONN_PREFIX_VLTIME     "ValidLifeTime"
#define V6CONN_PREFIX_STAT       "PrefixStatus"

#define V6CONN_DNS_ENABLE        "IPv6DNSEnabled"
#define V6CONN_DNS_OVERRIDE      "IPv6DNSOverrideAllowed"
#define V6CONN_DNS_SVRS          "IPv6DNSServers"
#define V6CONN_DNS_USER_DEFINED  "UsrIPv6DNSServers"

#define V6CONN_NTP_ENABLE        "GetIPv6NTPServers"
#define V6CONN_NTP_SVRS           "IPv6NTPServers"

#define V6CONN_REMOTE_LINK_ADDR "RemoteIPv6LinkLocalAddress" /*for pppv6*/
#define V6CONN_SERVICE_LIST     "X_TWSZ-COM_ServiceList"
#define V6CONN_CONN_TYPE        "ConnectionType"
/************************************************************************/
#define IP6_CONN_STAT_GLOBAL  "GlobalConnected"
#define IP6_CONN_STAT_LOCAL   "LinkLocalConnected"
#define IP6_CONN_STAT_DISCONN "Disconnected"

#define IP6_INFO_STAT_STALE   "Stale"
#define IP6_INFO_STAT_VALID   "Valid"
#define IP6_INFO_STAT_INVALID "Invalid"

#define STR_IP6_CONN_STATUS_GLOBAL  "GlobalConnected"
#define STR_IP6_CONN_STATUS_LOCAL   "LinkLocalConnected"
#define STR_IP6_CONN_STATUS_DISCONN "Disconnected"

/**************************************************************************/
#define WAN_CONN_NODE_MATCH_PATH  "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.%s.{i}.%s%s"
#define WAN_CONN_MATCH_PATH  "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.%s."
#define WAN_CONN_INST_MATCH_PATH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.%s.{i}."
#define IP6_CONN_MATCH_PATH "InternetGatewayDevice.WANDevice.%%.WANConnectionDevice.%%.%s.%%.X_TWSZ-COM_IPv6Config.%s"

#define WAN_CONN_INST_REGEX  "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\." \
                             "("STR_WAN_IPCONN"|"STR_WAN_PPPCONN")\\.[0-9]+\\."

#define WAN_CONN_PATH_FMT "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.%s.%lu."
#define IP6_CONN_PATH_FMT "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.%s.%lu.X_TWSZ-COM_IPv6Config."
#define WAN_CONN_PATH_FMT_SSCANF "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.%*[^.].%lu"

#define WAN_CONN_NODE_REGEX(node) WAN_CONN_INST_REGEX node
#define V6CONN_NODE_REGEX(node)   WAN_CONN_INST_REGEX "X_TWSZ-COM_IPv6Config\\." node
#define V6CONN_ADDR_NODE_REGEX(node) WAN_CONN_INST_REGEX "X_TWSZ-COM_IPv6Config\\.IPv6AddressList\\.[0-9]+\\." node
#define V6CONN_ROUTER_NODE_REGEX(node) WAN_CONN_INST_REGEX "X_TWSZ-COM_IPv6Config\\.IPv6DefaultRouterList\\.[0-9]+\\." node
#define V6CONN_PREFIX_NODE_REGEX(node) WAN_CONN_INST_REGEX "X_TWSZ-COM_IPv6Config\\.IPv6SitePrefixInfo\\." node
/*
 * ipv6 enable/disable 节点
 */
#ifdef CONFIG_APPS_LOGIC_IPV6_ENABLE
#define IPV6_ENABLE_NODE "InternetGatewayDevice.X_TWSZ-COM_IPv6Enable.Enable"
#endif
/**************************************************************************/
#define FOR_EACH_LIST(pos, list)        for ((pos) = (list); (pos) != NULL; (pos) = (pos)->pstNext)

#define WAN_CONN_MODE_MASK      0xF0000000
#define WAN_DEV_INST_MASK       0x0F000000
#define WAN_CONN_DEV_INST_MASK  0x00FFFF00
#define WAN_CONN_INST_MASK      0x000000FF

enum {
    IPV6_CONN_STAT_DISCONNECTED,
    IPV6_CONN_STAT_LOCALCONNECTED,
    IP6V_CONN_STAT_GLOBALCONNECTED,
};

typedef long (*FUNC_REG_MID)(const char *pcPath, unsigned short usMID);
typedef struct
{
    FUNC_REG_MID pfnRegFunc;
    const char *pszWANConnType;
    const char *pszExtraPath;
    const char *pszNode;
} ST_V6CONN_MANAGE_NODE;

typedef struct {
    const char *pszConnMode;
    unsigned long ulWanDevIdx;
    unsigned long ulWanConnDevIdx;
    unsigned long ulWanConnIdx;
} ST_COMMIT_PATH_INFO;

#define MAX_COMMIT_PATH_NUM 5

#endif /*_V6CONN_H_*/

