/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: dhcpr.h
 文件描述: 实现通过配置dhcp-fwd进程及其配置文件实现DHCP Relay服务
 修订记录:
        1. 作者: liannan
           日期: 2008-11-11
           内容: 创建文件
**********************************************************************/

#ifndef _DHCPS_H
#define _DHCPS_H

/* 日志打印宏 */

#ifdef DHCPR_DEBUG
#define DHCPR_TRACE(fmt, args...)		TBS_TRACE(MID_DHCPR, fmt "\n", ##args)
#else
#define DHCPR_TRACE(fmt, args...)
#endif

#define DHCPR_TRACE_SUCCESS(fmt, args...) DHCPR_TRACE(GREEN INFO_SUCCESS NONE fmt, ##args)
#define DHCPR_TRACE_ERROR(fmt, args...) DHCPR_TRACE(LIGHT_RED INFO_ERROR NONE fmt, ##args)


/* 非标准节点名前缀*/
#define CUST_NODE_NAME_PREFIX "X_TWSZ-COM_"

/* DHCP Relay 的TR069 节点名*/
#define NODE_DHCP_RELAY "DHCPRelay"

#define NODE_HOST_CM "LANHostConfigManagement"

/* DHCP Relay 的非TR069 节点名*/
#define NODE_DHCPR_ADDR CUST_NODE_NAME_PREFIX"DHCPRelayAddress"
#define NODE_DEV_NAME CUST_NODE_NAME_PREFIX"DeviceName"
#define NODE_WAN_IF_NAME CUST_NODE_NAME_PREFIX"ConnectedDeviceName"

/* WAN 的TR069 节点名 */
#define NODE_WAN_PPP_CONN "WANPPPConnection"
#define NODE_WAN_IP_CONN "WANIPConnection"
#define NODE_ENABLE  "Enable"
#define NODE_EXTERNAL_IP_ADDR  "ExternalIPAddress"
#define NODE_WAN_ETH_LINK_CONFIG "WANEthernetLinkConfig"
#define NODE_WAN_DSL_LINK_CONFIG "WANDSLLinkConfig"
#define NODE_ETH_LINK_STATUS "EthernetLinkStatus"
#define NODE_DSL_LINK_STATUS "DSLLinkStatus"

#define NODE_DHCP_SERVER_ENABLE "DHCPServerEnable"

#define LAN_DEV_PATH_PREFIX "InternetGatewayDevice.LANDevice."

#define WAN_CONN_PATH "InternetGatewayDevice.WANDevice.%.WANConnectionDevice.%."

#define DHCPR_CONFIG_FILE "/var/dhcp-fwd.conf"

#define LAN_DEV_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.$"

#endif
