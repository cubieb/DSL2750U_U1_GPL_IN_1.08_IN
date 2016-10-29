/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: ipconn.h

 文件描述: IPCONN模块的头文件

 修订记录:
           1 作者 : 邹德明
             日期 : 2008-03-18
             描述 :

           2 作者 : 匡素文
             日期 : 2008-10-15
             描述 : 重构

**********************************************************************/
#ifndef _TBS_IPCONN_H
#define _TBS_IPCONN_H

#include "common.h"
#include "warnlog.h"
#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "autoconf.h"

//#include <linux/led_interface.h>

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

/* 调试信息宏*/

#ifdef IPCONN_DEBUG
#define IPCONN_TRACE(fmt, args...)              COMMON_TRACE(MID_IPCONN, fmt"\n", ##args)
#define IPCONN_FUNC_IN(fmt, args...)            IPCONN_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define IPCONN_FUNC_OUT()                       IPCONN_TRACE("--O %s\n", __FUNCTION__)
#else
#define IPCONN_TRACE(fmt, args...)
#define IPCONN_FUNC_IN(fmt, args...)
#define IPCONN_FUNC_OUT()

/* 模块关闭调试时关掉内存调试 */
#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free
#endif
#ifdef calloc
#undef calloc
#endif
#ifdef realloc
#undef realloc
#endif

#endif

#define IPCONN_LOG_ERR(fmt, args...)                            COMMON_LOG_ERROR(MID_IPCONN, fmt, ##args)

#define IPCONN_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_IPCONN, iRet, strError, ##args)
#define IPCONN_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_IPCONN, pRet, strError, ##args)

#define IPCONN_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_IPCONN, iRet, strError, ##args)
#define IPCONN_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_IPCONN, pRet, strError, ##args)

#define IPCONN_RET_GOTO(iRet, strError, gotoTag)                RET_GOTO(MID_IPCONN, iRet, strError, gotoTag)
#define IPCONN_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_IPCONN, pRet, gotoTag ,strError, ##args)

#define IPCONN_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_IPCONN, iRet, strError, ##args)
#define IPCONN_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_IPCONN, pRet, strError, ##args)

#define ERR_GET(node)               "Get Node["node"] value failed.\n"
#define ERR_SET(node)               "Set Node["node"] value failed.\n"

/* 数字检查宏函数 */
#define isdigit(ch)                 (ch >= '0' && ch <= '9')


/************************** 常量定义 ************************/
/* 节点路径定义 */
#define WAN_DEV_PREFIX              "InternetGatewayDevice.WANDevice."
#define WAN_DEV_INST_PREFIX         "InternetGatewayDevice.WANDevice.%u."
#define WAN_CONN_DEV_PREFIX         "InternetGatewayDevice.WANDevice.%u.WANConnectionDevice."
#define WAN_CONN_DEV_INST_PREFIX    "InternetGatewayDevice.WANDevice.%u.WANConnectionDevice.%u."
#define WAN_IPCONN_PREFIX           "InternetGatewayDevice.WANDevice.%u.WANConnectionDevice.%u.WANIPConnection."
#define WAN_IPCONN_INST_PREFIX      "InternetGatewayDevice.WANDevice.%u.WANConnectionDevice.%u.WANIPConnection.%u."

#define WAN_CONN_DEV_INST           "InternetGatewayDevice.WANDevice.%u.WANConnectionDevice.%u"

#define WAN_IPCONN_MATCH            "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection."
#define WAN_IPCONN_INST_MATCH       "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."


/* 非标准节点名前缀*/
#define CUST_NODE_NAME_PREFIX       "X_TWSZ-COM_"

/* 节点名称*/
#define NODE_DeviceName             "X_TWSZ-COM_DeviceName"

#define NODE_Enable                 "Enable"
#define NODE_Name                   "Name"
#define NODE_AddressingType         "AddressingType"
#define NODE_Connection_Time        "ConnectionTime"
#define NODE_ConnectionType         "ConnectionType"
#define NODE_ExternalIPAddress      "ExternalIPAddress"
#define NODE_SubnetMask             "SubnetMask"
#define NODE_DefaultGateway         "DefaultGateway"
#define NODE_DNSServers             "DNSServers"
#define NODE_DNSOverrideAllowed     "DNSOverrideAllowed"
#define NODE_UsrDNSServers          "X_TWSZ-COM_UsrDNSServers"
#define NODE_ConnectionStatus       "ConnectionStatus"
#define NODE_LastConnectionError    "LastConnectionError"
#define NODE_NATEnabled             "NATEnabled"
#define NODE_ConnectedDeviceName    "X_TWSZ-COM_ConnectedDeviceName"
#define NODE_LinkType               "WANDSLLinkConfig.LinkType"
#define NODE_NAT_PUBIPADDR          "Natipaddr"

#ifdef    CONFIG_APPS_LOGIC_WANSELECT
#define WAN_IPCONN_WANSELECT_PATH_PREFIX            "InternetGatewayDevice.Services."\
                                         "X_TWSZ-COM_WANSelect."

#define WAN_IPCONN_WANSELECT_NODE_DEFCFG          "DefaultCfg"
#endif

#ifdef CONFIG_TBS_SUPPORT_IPV6
enum
{
    EN_IPV4_ENABLE,
    EN_IPV4_DISABLE,
    EN_IPV4_NOCHANGE
};
#define NODE_ENABLE_IPV4            "X_TELEFONICA-ES_IPv4Enabled"
#endif

#define NODE_MaximumConnections     "X_TWSZ-COM_MaximumConnections"
#define NODE_MaximumIPConnections   "X_TWSZ-COM_MaximumIPConnections"
#define NODE_MaximumPPPConnections  "X_TWSZ-COM_MaximumPPPConnections"

#define NODE_WANIPConnectionNumberOfEntries     "WANIPConnectionNumberOfEntries"
#define NODE_WANPPPConnectionNumberOfEntries    "WANPPPConnectionNumberOfEntries"
#define NODE_MaximumActiveConnections           "MaximumActiveConnections"
#define NODE_NumberOfActiveConnections          "NumberOfActiveConnections"
#define NODE_NumberOfEnabledConnections         "TWSZ-COM_NumberOfEnabledConnections"


#define NODE_DomainName             "X_TWSZ-COM_DomainName"
#define NODE_GetNTPServers          "X_TWSZ-COM_GetNTPServers"
#define NODE_NTPServers             "X_TWSZ-COM_NTPServers"
/* Added by wangtao for dhcp option 121 */
/* path of static routes: InternetGatewayDevice.Layer3Forwarding.Forwarding */
#define NODE_StaticRoutes           "staticroute"

#define STR_WANDEV                  "InternetGatewayDevice.WANDevice"
#define STR_WANCONNDEV              "WANConnectionDevice"
#define STR_WANCOMMETHIFCFG         "WANCommonInterfaceConfig"

/* 某些特定返回值的最大长度*/
#define IPCONN_MAX_STATUS_LEN       20

/*DNS节点的长度*/
#define MAX_LEN_DNS                     48

/* IPCONN模块用到的一些Buf长度*/
#define IPCONN_MAX_BUF_LEN          512

/* WANConnectionDevice的链路状态*/
#define CONNECTED                   1
#define DISCONNECTED                0

/* WAN连接活动性事件*/
#define WAN_CONN_EST                1
#define WAN_CONN_FIN                0

/* 收到匹配信息标志位*/
#define MATCH_IP_BIT                1
#define MATCH_MASK_BIT              2
#define MATCH_ROUTER_BIT            4
#define MATCH_DNS_BIT               8
#define MATCH_LEASE_OBTAIN_BIT      16
#define MATCH_LEASE_LOST_BIT        32

/* 统计字段列号*/
#define IPCONN_STS_IFC_COLS         8
#define IPCONN_STS_RX_BYTE          0
#define IPCONN_STS_RX_PACKET        1
#define IPCONN_STS_TX_BYTE          4
#define IPCONN_STS_TX_PACKET        5

/* 默认最大连接个数*/
#define MAX_WAN_CONNECTONS_DEF      8

/* 默认最大激活连接个数*/
#define MAX_ACTIVE_CONNECTONS_DEF   8


/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
enum EN_IPCONN_CLR_COUNT
{
    EN_IPCONN_CLR_COUNT_BASE = 5,
#ifdef ENABLE_OPT15
    EN_IPCONN_CLR_COUNT_OPTION15,
#endif
#ifdef ENABLE_OPT42
    EN_IPCONN_CLR_COUNT_OPTION42,
#endif
    EN_IPCONN_CLR_COUNT_MAX,
};

#define IPCONN_MAX_NODENUM_NEED_CLR (EN_IPCONN_CLR_COUNT_MAX - 1)
#define IPCONN_MAX_START_DHCPC_ARGS 10
#define IPCONN_MAX_STS_COLS_IN_PROC_FILE 17


/* 连接类型改变 */
enum
{
    EN_IPCONN_NoChanged = 0,
    EN_IPCONN_UnconfiguredToBridged,
    EN_IPCONN_UnconfiguredToRouted,
    EN_IPCONN_BridgedToUnconfigured,
    EN_IPCONN_BridgedToRouted,
    EN_IPCONN_RoutedToUnconfigured,
    EN_IPCONN_RoutedToBridged
};


/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/

/* IPCONN的节点 */
typedef struct tagST_IPCONN_Nodes
{
    int  iEnable;
    char *pcName;
    char *pcConnectionType;
    char *pcAddressingType;
    char *pcExternalIPAddress;
    char *pcSubnetMask;
    char *pcDefaultGateway;
    char *pcDNSServers;
    char *pcConnectionStatus;
#ifdef ENABLE_OPT42
    int  iGetNTPServers;
#endif
}ST_IPCONN_Nodes;


typedef struct tagST_IPCONN_Index
{
    unsigned int nWanDevIdx;
    unsigned int nWanConnDevIdx;
    unsigned int nWanIPConnIdx;
}ST_IPCONN_Index;


typedef struct tagST_IPCONN_Private
{
    unsigned char ucMatchMap;
    unsigned char ucLinkType;
    unsigned char bWanConnDevState;
    unsigned char ucReserved;
    char szWanConnDevName[20];
}ST_IPCONN_Private;

/* IPCONN实例*/
typedef struct tagST_IPCONN_Instance
{
    char acPath[MAX_PATH_LEN];

    ST_IPCONN_Nodes stNodes;
    ST_IPCONN_Index stIndex;
    ST_IPCONN_Private stPrivate;
}ST_IPCONN_Instance;

typedef struct tagST_IPCONN_Stats
{
    unsigned char  ucMatchMap;
    unsigned char  ucMachineState;
}ST_IPCONN_Stats;

/* 状态机链表实例节点 */
typedef struct tagST_IPCONN_Stats_Lst
{
    ST_IPCONN_Index stIdx;
    ST_IPCONN_Stats stStats;
    struct tagST_IPCONN_Stats_Lst *next;    
}IPCONN_Stats_Lst;

/* 进程输出匹配信息 */
typedef struct tagST_IPCONN_MatchInfo
{
    char *pszMatchCond;             /* 匹配条件 */
    char *pszNodeName;              /* 匹配节点名 */
    unsigned int iMatchBit;         /* 匹配标志位 */
}ST_IPCONN_MatchInfo;


/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/
/* Moudule Init && Destroy */
int IPCONN_ModuleInit();
int IPCONN_ModuleDestroy();



#endif //_TBS_IPCONN_H

