#ifndef _RADVD_H_
#define _RADVD_H_

#include "common_msg_handler.h"
#include "warnlog.h"
#include "ipt_common.h"

/******************************************************************************
*                                  DEBUG                                      *
******************************************************************************/
//#define RADVD_DEBUG

#ifdef RADVD_DEBUG
#define RADVD_TRACE(fmt, args...) COMMON_TRACE(MID_RAD, ECHO_GREEN fmt ECHO_NONE, ##args)
#define RADVD_LOG_ERR(fmt, args...) COMMON_TRACE(MID_RAD, ECHO_RED fmt ECHO_NONE, ##args)
#else
#define RADVD_TRACE(fmt, args...)
#define RADVD_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_RAD, fmt, ##args)
#endif

#define RADVD_TRACE_INTO_FUNC RADVD_TRACE("##In## %s\n", __func__)
#define RADVD_TRACE_OUT_FUNC RADVD_TRACE("##Out## %s\n", __func__)

#define CALL_FUCTION_FAILED "Call function \"%s\" failed\n"

#define RADVD_RET_RETURN(iRet, strError, args...) RET_RETURN(MID_RAD, iRet, strError, ##args)
#define RADVD_POINTER_RETURN(pRet, strError, args...) POINTER_RETURN(MID_RAD, pRet, strError, ##args)

#define RADVD_ENABLED_BASE_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IPv6LANHostConfigManagement\\.RADVD\\."
#define RADVD_SITE_PREFIX_TYPE_BASE_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IPv6LANHostConfigManagement\\.IPv6SitePrefixConfigType\\."
#define RADVD_ULA_PREFIX_BASE_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IPv6LANHostConfigManagement\\.RadVDConfigManagement\\.ULAPrefixInfo\\."
#define RADVD_STATIC_PREFIX_BASE_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IPv6LANHostConfigManagement\\.RadVDConfigManagement\\.StaticPrefixInfo\\."
#define RADVD_STATELESS_BASE_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IPv6LANHostConfigManagement\\.ServerType\\."
#define RADVD_PD_BASE_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IPv6LANHostConfigManagement\\.IPv6PDWanConnection\\."
/******************************************************************************
*                                   Path                                      *
******************************************************************************/
#define RADVD_LANDEVICE_PATH "InternetGatewayDevice.LANDevice."
#define RADVD_ENABLED_BASE_PATH "X_TELEFONICA-ES_IPv6LANHostConfigManagement.RADVD."
#define RADVD_SITE_PREFIX_TYPE_BASE_PATH "X_TELEFONICA-ES_IPv6LANHostConfigManagement.IPv6SitePrefixConfigType."
#define RADVD_CONFIG_BASE_PATH "X_TELEFONICA-ES_IPv6LANHostConfigManagement.RadVDConfigManagement."
#define RADVD_ULA_PREFIX_BASE_PATH "X_TELEFONICA-ES_IPv6LANHostConfigManagement.RadVDConfigManagement.ULAPrefixInfo."
#define RADVD_STATIC_PREFIX_BASE_PATH "X_TELEFONICA-ES_IPv6LANHostConfigManagement.RadVDConfigManagement.StaticPrefixInfo."
#define RADVD_DELEGATED_PREFIX_BASE_PATH1 "X_TELEFONICA-ES_IPv6LANHostConfigManagement.RadVDConfigManagement.PrefixInformation.1."
#define RADVD_DELEGATED_PREFIX_BASE_PATH "X_TELEFONICA-ES_IPv6LANHostConfigManagement.RadVDConfigManagement.PrefixInformation."
#define RADVD_STATELESS_BASE_PATH "X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType."
#define RADVD_LAN_OBJECT_ADD_PATH "X_TELEFONICA-ES_IPv6LANHostConfigManagement.RadVDConfigManagement.PrefixInformation."
#define RADVD_PD_BASE_PATH "X_TELEFONICA-ES_IPv6LANHostConfigManagement.IPv6PDWanConnection."
#define RADVD_LANDEVICE_FOREACH_PATH "InternetGatewayDevice.LANDevice.%."

#define RADVD_IPINTERFACE_BASE_PATH "LANHostConfigManagement.IPInterface.1."
#define RADVD_IPv6InterfaceAddress "X_TWSZ-COM_IPv6InterfaceAddress"
#define RADVE_STATIC_PREFIX_CFG "StaticPrefixInfo"
#define RADVE_DELEGATED_PREFIX_CFG "PrefixInformation"
#define RADVD_PD_WAN_CONNECTION_CFG "IPv6PDWanConnection"

#define RADVD_DEVICE_NAME "X_TWSZ-COM_DeviceName"

#define RADVD_ENABLED_NODE "Enabled"
#define RADVD_ULA_ENABLE_NODE "Enable"
#define RADVD_SITE_PREFIX_TYPE_NODE "StaticDelegated"
#define RADVD_STATELESS_NODE "Stateless"
#define RADVD_PD_INTERFACE_NODE "Interface"

#define RADVD_PREFIX_MODE_NODE "Mode"
#define RADVD_PREFIX_CONNECTION "DelegatedConnection"
#define RADVD_PREFIX_NODE "Prefix"
#define RADVD_PREFIX_PREFERENCE_NODE "PreferredLifeTime"
#define RADVD_PREFIX_VALID_NODE "ValidLifeTime"

#define RADVD_SERVICE_LIST "X_TWSZ-COM_ServiceList"
#define RADVD_DOT "."
#define RADVD_PREFIX_LEN "64"

#define V6CONN_PREFIX_STATUS_NODE "PrefixStatus"
#define V6CONN_PREFERRED_LIFETIME_NODE "PreferredLifeTime"
#define V6CONN_VALID_LIFETIME_NODE "ValidLifeTime"
#define V6CONN_PREFIX_DELEG_ENABLE_NODE "X_TELEFONICA-ES_IPv6PrefixDelegationEnabled"
#define V6CONN_CONNECTION_STATUS_NODE "X_TELEFONICA-ES_IPv6ConnStatus"
#define V6CONN_PREFIX_LIST_NODE "IPv6SitePrefixInfo"
#define V6CONN_SITE_PREFIX_NODE "IPv6SitePrefix"
#define V6CONN_SITE_PREFIX_LEN_NODE "IPv6SitePrefixLength"
#define V6CONN_DEVICE_NAME_NODE "X_TWSZ-COM_ConnectedDeviceName"
#define V6CONN_CONNECTION_NAME_NODE "Name"
#define V6CONN_IPV6_CONFIG_NODE "X_TWSZ-COM_IPv6Config"

#define V6CONN_PREFIX_TYPE_STATIC "1"
#define V6CONN_PREFIX_TYPE_DELEGATED "0"

#define V6CONN_STAT_GLOBAL "GlobalConnected"
#define V6CONN_STAT_LINKLOCAL "LinkLocalConnected"
#define V6CONN_STAT_DISCONN "Disconnected"

#define WAN_IPCONN_INST_MATCH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define WAN_PPPCONN_INST_MATCH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define WAN_CONNECTION_PATH "InternetGatewayDevice.WANDevice.1.WANConnectionDevice."
#define WAN_IPV6_PREFIX_PATH "X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo."
#define WAN_IP_CONN_NUMBER "WANIPConnectionNumberOfEntries"
#define WAN_PPP_CONN_NUMBER "WANPPPConnectionNumberOfEntries"
#define WAN_IP_CONNECTION "WANIPConnection"
#define WAN_PPP_CONNECTION "WANPPPConnection"
#define WAN_CONNECTION_NAME "Name"
#define WAN_IPV6_ENABLE "X_TELEFONICA-ES_IPv6Enabled"
#define WAN_IPV6_CONN_STAT "X_TELEFONICA-ES_IPv6ConnStatus"
#define WAN_IPV6_SITE_PREFIX "X_TELEFONICA-ES_IPv6SitePrefix"
#define WAN_IPV6_SITE_PLTIME "X_TELEFONICA-ES_IPv6SitePrefixPltime"
#define WAN_IPV6_SITE_VLTIME "X_TELEFONICA-ES_IPv6SitePrefixVLtime"

#define MAX_IPV6_ADDRESS_LEN 16
#define PREFIX_ADDRESS_LEN 8
#define MAX_IPV6_ADDRESS_STR_LEN 50
#define MAX_RAD_EXECINFO_COUNT 6
#define MAX_SUBNET_ID 255
#define MIN_PREFIX_LEN 0
#define MAX_PREFIX_LEN 128

#define FLAG_CONF_TYPE_AUTO "Auto"
#define FLAG_CONF_TYPE_MANUAL "Manual"

#define V6CONN_PREFIX_VALID "Valid"
#define V6CONN_PREFIX_INVALID "Invalid"

#define RADVD_PREFIX_WAN_DELEGATED "WANDelegated"
#define RADVD_PREFIX_TUNNEL_DELEGATED "TunnelDelegated"
/*
 * ipv6 enable/disable ½Úµã
 */
#ifdef CONFIG_APPS_LOGIC_IPV6_ENABLE
#define IPV6_ENABLE_NODE "InternetGatewayDevice.X_TWSZ-COM_IPv6Enable.Enable"
#define COMMIT_IPV6_SET_ENABLE "InternetGatewayDevice\\.X_TWSZ-COM_IPv6Enable\\.Enable"
#endif


#define RADVD_CONFIG_FILE "/var/radvd.conf"

/* Radvd debug level: from 1~5 */
#define RADVD_DEBUG_LEVEL "1"

/* RADVD Configuration */
#define RADVD_ROUTER_PREFERENCE "low"
#define RADVD_ROUTER_LIFETIME "60"
#define RADVD_MIN_RA_INTERVAL "3"
#define RADVD_MAX_RA_INTERVAL "10"
#define RADVD_FLAG_CONF_TYPE "Auto"
#define RADVD_MANAGED_FLAG ""
#define RADVD_OTHER_CONF_FLAG ""
#define RADVD_IPV6_SUBNETID "0"

#define FOR_EACH_LIST(pos, list) for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)

#define RADVD_RET_GOTO(iRet, strError, gotoTag) \
    { \
        if (RET_FAILED(iRet)) \
        { \
            printf(strError); \
            goto gotoTag; \
        } \
    }

#define RADVD_POINTER_GOTO(Pointer, gotoTag, strError, args...) \
    { \
        if (!Pointer) \
        { \
            printf(strError, ##args); \
            goto gotoTag; \
        } \
    }

#endif
