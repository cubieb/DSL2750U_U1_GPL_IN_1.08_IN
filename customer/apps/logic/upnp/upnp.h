/*
 * =====================================================================================
 *  Copyright (c) 2008, T&W ELECTRONICS Co., Ltd.
 *
 *       Filename:  upnp.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2008年12月12日 08时52分10秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Annan Li (madbug), liannan@twsz.com
 *
 * =====================================================================================
 */


#ifndef _UPNP_H
#define _UPNP_H

#include "ipt_common.h"



/* 日志打印宏 */
#ifdef UPNP_DEBUG
#define UPNP_TRACE(fmt, args...)		COMMON_TRACE(MID_UPNP, fmt "\n", ##args)
#else
#define UPNP_TRACE(fmt, args...)
#endif

#define MAX_PROC_NAME_LEN               16

#define UPNP_TRACE_SUCCESS(fmt, args...)  UPNP_TRACE(GREEN INFO_SUCCESS NONE fmt, ##args)
#define UPNP_TRACE_ERROR(fmt, args...)    TBS_TRACE(MID_UPNP, LIGHT_RED INFO_ERROR NONE fmt "\n", ##args)

#define UPNP_GETVAL(node)        ((char *)COMM_GetNodeValue(UPNP_PATH_PREFIX node, CACHE_CFG))

#define UPNP_CONFIG_FILE "/etc/upnpd.conf"

#define UPNP_CHAIN_NAME  "MINIUPNPD"
#define UPNP_BLACKLIST_CHAIN_NAME "UPNPBLACKLIST"

#define NODE_UPNP_ENABLE "Enable"
#define NODE_WAN_PATH "WANPathName"
#define NODE_LAN_PATH "LANPathName"
#define NODE_HOST_CM "LANHostConfigManagement"
#define NODE_LAN_INTERFACE "IPInterface"
#define NODE_IP_ROUTERS "IPRouters"
#define NODE_LAN_IPADDRESS "IPInterfaceIPAddress"

#define NODE_UPNP_BLACKLIST "Blacklist"
#define NODE_UPNP_LISTNUM "BlacklistNumberOfEntries"
#define NODE_UPNP_IP "SrcIP"
#define NODE_UPNP_IP_ENABLE_REGEX  "Blacklist\\.[0-9]+\\.Enable"
#define NODE_UPNP_IP_REGEX "Blacklist\\.[0-9]+\\.SrcIP"
#define UPNP_BALCKLIST_REGEX "Blacklist\\.[0-9]+\\."

#define CUST_NODE_NAME_PREFIX "X_TWSZ-COM_"
#define NODE_DEV_NAME CUST_NODE_NAME_PREFIX"DeviceName"
#define NODE_WANIF_NAME CUST_NODE_NAME_PREFIX"ConnectedDeviceName"
#define NODE_WANIF_STAT "ConnectionStatus"
#define NODE_WANIF_IP "ExternalIPAddress"

#define UPNP_PATH_PREFIX "InternetGatewayDevice.X_TWSZ-COM_UPNP."
#define IF_IP_PATH_PREFIX "InternetGatewayDevice.LANDevice."

#define NODE_DEF_ROUTE_PATH "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"
#define NODE_DEFAULT_LAN_PATH "InternetGatewayDevice.LANDevice.1"

/*设备信息宏定义*/
#define DEVINFO_PATH_PREFIX  "InternetGatewayDevice.DeviceInfo."
#define NODE_DEVINFO_MANUFACTURER "Manufacturer"
#define NODE_DEVINFO_MODELNAME "ModelName"
#define NODE_DEVINFO_DESCRIPTION "Description"
#define NODE_DEVINFO_PRODUCTCLASS "ProductClass"
#define NODE_DEVINFO_WANIP "WanIpAddr"
#define NODE_DEVPREURL "presentation_url"

#define WAN_PATH_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+"
#define WAN_PPPCONN_REGEX WAN_PATH_REGEX"\\.WANPPPConnection\\.[0-9]+$"
#define WAN_IPCONN_REGEX WAN_PATH_REGEX"\\.WANIPConnection\\.[0-9]+$"

#define LAN_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+$"

static int UPNP_SwtichSM();

#endif

