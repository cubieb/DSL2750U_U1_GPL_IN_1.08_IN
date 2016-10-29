/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: ethwan.h

 文件描述: ethwan 的头文件

 修订记录:

        1. 作者: 伍国祥

           日期: 2008-2-22

           内容: 创建文件
           
        2. 作者: madbug

           日期: 2008-10-10

           内容: 适配模块优化

**********************************************************************/
#ifndef _ETHWAN_H
#define _ETHWAN_H

#include <stdio.h>
#include "tbserror.h"
#include <net/if.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include "warnlog.h"


#ifdef ETHWAN_DEBUG
#define ETHWAN_TRACE(fmt, args...) TBS_TRACE(MID_ETHWAN, fmt, ##args)
#else
#define ETHWAN_TRACE(str, args...)  do { ; } while(0);
#endif

/* TR069节点名 */
#define NODE_IF_NAME            "X_TWSZ-COM_DeviceName"
#define NODE_ENABLE             "Enable"
#define NODE_STATUS             "Status"
#define NODE_MAC_ADDR           "MACAddress"
#define NODE_MAX_BIT_RATE       "MaxBitRate"
#define NODE_DUPLEX_MODE        "DuplexMode"
#define NODE_BYTES_SENT         "BytesSent"
#define NODE_BYTES_RECEIVED     "BytesReceived"
#define NODE_PACKETS_SENT       "PacketsSent"
#define NODE_PACKETS_RECEIVED   "PacketsReceived"
#define NODE_WAN_TYPE     "WANAccessType"
#define STR_ETH_TYPE       "Ethernet"
#define NODE_IF_NAME_DESCRIPTION       "X_TWSZ-COM_Description"
#ifdef CONFIG_RALINK_RT3052
#define NODE_QINQ               "QINQ"
#define NODE_VLAN_STAG          "STAG"
#endif

#define STR_ETHWANPORT_NAME          "vport1"
#define STR_ETHWANPORT_DESCRIPTION          "ethernet4"
#define STR_ETHWANPORT_PATH          "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig."

#define ETHWAN_PATH_PREFIX     "InternetGatewayDevice.Layer2Bridging."
#define ETHWAN_PATH_BRIDGE     ETHWAN_PATH_PREFIX "Bridge."
#define ETHWAN_PATH_FILTER     ETHWAN_PATH_PREFIX "Filter."
#define ETHWAN_PATH_AVIBLEIF   ETHWAN_PATH_PREFIX "AvailableInterface."
#define ETHWAN_NODE_AVIBLEIFKEY  "AvailableInterfaceKey"
#define ETHWAN_NODE_IFREF        "InterfaceReference"
#define ETHWAN_NODE_IFTYPE       "InterfaceType"
#define ETHWAN_NODE_FITERENABLE    "FilterEnable"
#define ETHWAN_NODE_FITERBRIDGEREFERENCE       "FilterBridgeReference"
#define ETHWAN_NODE_FITERINTERFACE       "FilterInterface"
#define ETHWAN_NODE_VLANTAG      "AdmitOnlyVLANTagged"
#define ETHWAN_NODE_FILTERKEY     "FilterKey"
#define DEFAULT_BRIDGE_EREFERENCE_INDEX   "1"  /*br0*/
#define  DEFAULT_BRIDGE_PATH          "InternetGatewayDevice.LANDevice.1" 
/* TR069路径*/
#define STR_WANDEV          "InternetGatewayDevice.WANDevice.%lu."
#define STR_WANDEV_PREFIX   "InternetGatewayDevice.WANDevice."
#define STR_WANETHIFCFG     "WANEthernetInterfaceConfig"
#define STR_WANCOMMETHIFCFG "WANCommonInterfaceConfig"
#define STR_STATS           "Stats"
#define ETHWAN_SET_PATH     STR_WANDEV".%."STR_WANETHIFCFG"."

#define ADD_ACTION    1
#define DEL_ACTION    0

#endif

