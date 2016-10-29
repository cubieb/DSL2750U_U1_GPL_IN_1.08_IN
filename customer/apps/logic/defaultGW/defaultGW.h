/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 
 文件名称: defaultGW.h

 文件描述: defaultGW模块头文件

 修订记录:
             作者 : kevin
             日期 : 2009-07-29
             描述 : 创建
**********************************************************************/
#ifndef __TBS_DEFAULT_GATEWAY_H__
#define __TBS_DEFAULT_GATEWAY_H__
/*********************************************************************
**************************包含头文件*****************************
**********************************************************************/
#include "common.h"
#include <arpa/inet.h>
#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "autoconf.h"

/*********************************************************************
**************************宏变量定义*****************************
**********************************************************************/

 
//调试宏信息
#define DEFAULT_GATEWAY_DEBUG 1

#ifdef DEFAULT_GATEWAY_DEBUG
#define DFGW_TRACE(fmt, args...)                 COMMON_TRACE(MID_DEFAULTGW, fmt"\n", ##args)
#define DFGW_RET_GOTO(iRet, strError, gotoTag)               RET_GOTO(MID_DEFAULTGW, iRet, strError, gotoTag)
#else
#define DFGW_TRACE(fmt, args...)  
#define DFGW_RET_GOTO(iRet, strError, gotoTag)  
#endif

// 常用的宏定义
#define MAX_RULE_LENGTH         256
#define DEFAULTGW_PATH_LENGTH   256
#define MAX_CMD_LENGTH          128
#define DEFAULTGW_NAME_INIT        ""

//defaultGW模块路径前缀
#define DEFAULTGW_ASSIGN_PATH                             "InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService"
#define DEFAULTGW_EFFECT_PATH                 "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"
#define DEFAULTGW_PATH_PREFIX                 "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW."
#define DEFAULTGW_WAN_STATE   "ConnectionStatus"
#define DEFAULTGW_WAN_CONNECTED      "Connected"
#define DEFAULT_ROUTE_LOG_FILE  "/var/log/defaultrouting"

//device regex
#define DEVICE_REGEX_WAN_CONN_IP                 "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define DEVICE_REGEX_WAN_CONN_PPP               "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"
#define DEVICE_REGEX_LAN_CONN                       "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}$"

#define DEFAULTGW_WAN_DEV_STATUS "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Status"
#define DEFAULTGW_WAN_DSL_UP "Up"

#define DEFAULTGW_DNS_SERVER	    "DNSServers"
#define DEFAULTGW_CONNTYPE	        "ConnectionType"
#define DEFAULTGW_CONNTYPE_IP_BRIDGE      "IP_Bridged"
#define DEFAULTGW_PPP_CONNTRIGGER "ConnectionTrigger"
#define DEFAULTGW_PPP_ONDEMAND "OnDemand"
#define DEFAULTGW_PPP_ENABLE "Enable"
#define DEFAULTGW_USR_DNS_SERVER	"X_TWSZ-COM_UsrDNSServers"
#define DEFAULTGW_WAN_SERVICE_TYPE  "X_TWSZ-COM_ServiceList"
#define DEFAULTGW_TR069_TYPE	    "TR069"
#define DEFAULTGW_WANTYPE_OTHER     "OTHER"
#define DEFAULTGW_WAN_DEVICE        "InternetGatewayDevice.WANDevice"
#define DEFAULTGW_WAN_CONNDEVICE    "WANConnectionDevice"
#define DEFAULTGW_PPPCONN_INSTANCE  "WANPPPConnection"
#define DEFAULTGW_IPCONN_INSTANCE      "WANIPConnection"
#define DEFAULTGW_PPP_NAME          "Name"

#define DEFAULTGW_PATH_LAN_IP     "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"
#define DEFAULTGW_PATH_LAN_MASK   "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask"

#define NODE_DNSOverrideAllowed         "DNSOverrideAllowed"

int DefaultGW_Rule_Work(int flag);
// 节点路径正则表达式
//DefaultConnectionService节点
#define DEFAULTGW_REGEX_USERASSIGN            "InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService" "$"

// 默认路由匹配项
#define DEFAULTGW_WAN_PATH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}."
/***********************************************************************
**************************函数原型**********************************
************************************************************************/
int DefaultGateway_ModuleInit(void);

/* 接口信息数据结构 */
typedef struct tag_ST_INTERFACE_INFO{
    char szIfName[256];      /* 接口名称 */
    char szIfPath[256];      /* 接口的全路径 */
    char szRemoveIp[256];    /* 远端IP地址 */
    int  iIfState;                          /* 此接口连接状态 */
    struct tag_ST_INTERFACE_INFO *pstNext;
}ST_INTERFACE_INFO;

/* 默认路由信息 */
typedef struct tag_ST_DEFAULT_INFO{
    int  iDefaultRouteInKernel;         /* 默认路由是否在kernel table里 */
    int  iDefaultRuleFlag;              /* 节点X_TWSZ-COM_DEFAULT_GW是否存在默认路由标志 */
    char szIfPath[DEFAULTGW_PATH_LENGTH];  /* 接口的全路径 */
}ST_DEFAULT_INFO;

extern ST_INTERFACE_INFO *STATIC_FindInterfaceOfInternet(char *pExcludePath);
extern ST_INTERFACE_INFO *g_pstStaticInterfaceList;  /* 静态路由接口链表头指针 */
enum
{
    IF_STATE_DISCONNECTED = 0,
    IF_STATE_CONNECTED,
};

/* 路由规则是否在kernel的路由表里 */
enum
{
    ROUTE_IN_KERNEL_TABLE = 0,
    ROUTE_OUT_KERNEL_TABLE,
};

/* 路由规则是否enable */
enum
{
    ROUTE_RULE_DISENABLE = 0,
    ROUTE_RULE_ENABLE  = 1,
};

/* 节点X_TWSZ-COM_DEFAULT_GW是否存在默认路由 */
enum
{
    HAS_DEFAULT_RULE = 0,
    NOT_HAS_DEFAULT_RULE  = 1,
};

#endif

