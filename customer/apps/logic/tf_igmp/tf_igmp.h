/*
 * =====================================================================================
 *  Copyright (c) 2008, T&W ELECTRONICS Co., Ltd.
 *
 *       Filename:  igmp.h
 *
 *    Description:  Config Igmp Proxy & Snooping
 *
 *        Version:  1.0
 *        Created:  2008年11月20日 15时40分43秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Annan Li (madbug), liannan@twsz.com
 *
 * =====================================================================================
 */



#ifndef _TF_IGMP_H
#define _TF_IGMP_H

//#define IGMP_DEBUG


/* 日志打印宏 */
#ifdef IGMP_DEBUG
#define IGMP_TRACE(fmt, args...)		TBS_TRACE(MID_IGMP, fmt "\n", ##args)
#else
#define IGMP_TRACE(fmt, args...)
#endif

#define MAX_DEV_NAME_LEN 20
#define CMD_LEN 50

#define IGMP_V3
#define CONFIG_ADVANCED_PARAMETERS

#define IGMP_TRACE_SUCCESS(fmt, args...)  IGMP_TRACE(GREEN INFO_SUCCESS NONE fmt, ##args)
#define IGMP_TRACE_ERROR(fmt, args...)    TBS_TRACE(MID_IGMP, LIGHT_RED INFO_ERROR NONE fmt "\n", ##args)

#define IGMPPROXY_CONFIG_FILE "/etc/igmpproxy.conf"

#define NODE_IGMP_PROXY "Proxy"
#define NODE_IGMP_SNOOPING "Snooping"
#define NODE_FORBID_PHY_PORT "ForbidPhyPort"
#define NODE_ENABLE "Enable"
#define NODE_LAN_PATH "LANPathName"

#define FH_NODE_IGMP_PROXY "X_TELEFONICA-ES_IGMPProxy"
#define FH_NODE_ENABLE "Enabled"
#define FH_NODE_AGEENABLE "AgeEnabled"
#define FH_NODE_VERSION "Version"
#define FH_NODE_IGMPENT "IGMPTableNumberOfEntries"
#define FH_NODE_IGMP_TABLE "IGMPTable"
#define FH_NODE_IGMP_GRPADDR "GroupAddress"
#define FH_NODE_IGMP_INTERFACE "Interface"
#define FH_NODE_IGMP_STATE "State"

#define FH_NODE_LAN_DEVICE "LANDevice"
#define FH_NODE_WAN_DEVICE "WANDevice"

#define FH_NODE_IGMP_SNOOP "X_TELEFONICA-ES_IGMPSnoop"
#define FH_NODE_LMQI "LastMemberQueryInterval"
#define FH_NODE_HTO "HostTimeout"
#define FH_NODE_MRTO "MrouterTimeout"
#define FH_NODE_LTOMG "LeaveTimeout"
#define FH_NODE_MG "MaxGroups"

#ifdef CONFIG_ADVANCED_PARAMETERS
#define NODE_ENABLE_PASSTHROUGH		"Enable_PassThrough"
#define NODE_ENABLE_FASTLEAVING		"Enable_FastLeaving"
#define NODE_GQ_INTERVAL				"GQ_Interval"
#define NODE_GQ_RESPONSE_INTERVAL	"GQ_Response_Interval"
#define NODE_MQ_INTERVAL				"MQ_Interval"
#define NODE_MQ_RESPONSE_INTERVAL	"MQ_Response_Interval"
#define NODE_MQ_COUNT					"MQ_Count"
#define NODE_LQ_INTERVAL				"LQ_Interval"
#define NODE_LQ_COUNT					"LQ_Count"
#endif

#ifdef CONFIG_APPS_LOGIC_STATIC_IGMP
#define NODE_MAX_GROUP_ENTRIES		"MaxGroupEntries"
#define NODE_GROUP_NUMBER			"GroupNumberOfEntries"
#define NODE_STATIC_GROUP			"StaticGroup"
#define NODE_STATIC_SERVICE_NAME	"ServiceName"
#define NODE_STATIC_MULTICAST_IP	"MulticastGroupIP"
#define NODE_STATIC_STB_IP			"STBClientIP"
#define NODE_STATIC_STB_MAC			"STBClientMac"
#define NODE_STATIC_LAN_PORT        "LocalPort"
#define IGMP_STATIC_PATH_PREFIX		"InternetGatewayDevice.X_TWSZ-COM_IGMP.StaticGroup."

#define STATIC_GROUP_FILE	"/tmp/static_group"
#define IGMP_PID_FILE		"/var/run/igmp_pid"
#define BRCTL_ADD_STATIC_IGMP_SNOOPING 22
#define BRCTL_DEL_STATIC_IGMP_SNOOPING 23
#endif

#define BRCTL_EABLE_AGE_IGMP_SNOOPING 24
#define BRCTL_DISABLE_AGE_IGMP_SNOOPING 25

#define CUST_NODE_NAME_PREFIX "X_TWSZ-COM_"
#define NODE_DEV_NAME CUST_NODE_NAME_PREFIX"DeviceName"
#define NODE_WANIF_NAME CUST_NODE_NAME_PREFIX"ConnectedDeviceName"
#define NODE_WANIF_STAT "ConnectionStatus"

#define IGMP_PATH_PREFIX "InternetGatewayDevice.X_TWSZ-COM_IGMP."
#define IF_NAME_PATH "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.X_TWSZ-COM_DeviceName"

#define WAN_PATH_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+"
#define WAN_PPPCONN_REGEX WAN_PATH_REGEX"\\.WANPPPConnection\\.[0-9]+$"
#define WAN_IPCONN_REGEX WAN_PATH_REGEX"\\.WANIPConnection\\.[0-9]+$"

#define LAN_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+$"

#define FH_PROXY_PATH_PREFIX "InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy."
#define FH_SNOOP_PATH_PREFIX "InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop."
#define FH_WAN_PATH_PREFIX "InternetGatewayDevice.WANDevice.WANConnectionDevice.WANDSLLinkConfig."

#define X_TWSZ_IGMPSNOOP "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IGMPSnoop"

#define IGMP_WAN_PPPCONN_INST_MATCH     "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define IGMP_WAN_IPCONN_INST_MATCH      "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define IGMP_CONNTYPE_IP_BRIDGE     "IP_Bridged"
#define IGMP_CONNTYPE_PPP_BRIDGE    "PPPoE_Bridged"
#define IGMP_PPP_CONNTYPE	        "ConnectionType"
#define IGMP_IPCONN_DEVICE          "InternetGatewayDevice.WANDevice"
#define IGMP_IPCONN_CONNDEVICE      "WANConnectionDevice"
#define IGMP_IPCONN_INSTANCE        "WANIPConnection"
#define IGMP_PPPCONN_INSTANCE       "WANPPPConnection"
#define IGMP_WAN_STATE              "ConnectionStatus"
#define IGMP_WAN_CONNECTED          "Connected"
#define IGMP_DSL_LINK               "WANDSLLinkConfig"
#define IGMP_PPP_CONNTRIGGER        "ConnectionTrigger"
#define IGMP_PPP_ENABLE             "Enable"
#define IGMP_WAN_DEV_STATUS         "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Status"
#define IGMP_PPP_ONDEMAND           "OnDemand"
#define IGMP_WAN_DSL_UP             "Up"
#define NODE_DSL_IGMP_ENABLE        "X_TELEFONICA-ES_IGMPEnabled"

#define IGMP_MOD  "[IGMP :]"
#ifdef    CONFIG_APPS_LOGIC_ETHWAN
#define IGMP_ETH_LINK               "WANEthernetLinkConfig"
#endif
#define IGMP_RET_RETURN(Ret, fmt, args...)  \
	RET_RETURN(MID_IGMP, Ret, fmt, ##args)

#define IGMP_PRINTF(fmt, args...)  printf(IGMP_MOD "[ERROR] " "%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)

#define IGMP_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            IGMP_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
            goto gotoTag; \
        }\
      }

#define IGMP_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_IGMP, Pointer, gotoTag, fmt, ##args)
        
#define IGMP_ASSERT(expr) \
    do{ \
        if(!(expr))\
        {\
            IGMP_TRACE("Assert \"%s\" failed\n", #expr); \
            exit(-1); \
        } \
    }while(0)
    
#endif
