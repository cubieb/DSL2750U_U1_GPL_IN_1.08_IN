/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: droute.h
 文件描述: 动态路由头文件
 修订记录:
        1. 作者: 朱健文
           日期:
           内容: 创建文件
**********************************************************************/
#ifndef _DROUTE_H
#define _DROUTE_H

/******************************************************************************
*                                 DEBUG                                      *
******************************************************************************/

#ifdef  DROUTE_DEBUG
#define DROUTE_TRACE(fmt, args...)  COMMON_TRACE(MID_DROUTE, fmt, ##args)
#else
#define DROUTE_TRACE(fmt, args...)  do{;}while(0);
#endif


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
/* 出错的时候打印宏定义 */
#define DROUTE_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            DROUTE_TRACE(strError); \
            goto gotoTag; \
        }\
      }

/* 一般的宏定义 */
#define DROUTE_MOD_ID                   MID_DROUTE
#define DROUTE_NODE_LENGTH              256
#define DROUTE_PATH_LENGTH              256
#define MAX_PROC_NAME_LEN               16
#define DROUTE_ZEBRA_NAME               "zebra"
#define DROUTE_RIP_NAME                 "rip"
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define DROUTE_RIPNG_NAME               "ripng"
#endif
#define DROUTE_ZEBRA_ID                 1
#define DROUTE_RIP_ID                   2
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define DROUTE_RIPNG_ID                 3
#endif
#define DROUTE_INIT_NAME                ""
#define MAX_DROUTE_EXECINFO_COUNT       5
#define MAX_INDEX_COUNT                 3

#define DROUTE_WANDEV                   "InternetGatewayDevice.WANDevice"
#define DROUTE_WANCONNDEV               "WANConnectionDevice"
#define DROUTE_WANPPPCONN               "WANPPPConnection"
#define DROUTE_WANIPCONN                "WANIPConnection"
#define DROUTE_LANDEV                   "InternetGatewayDevice.LANDevice"

/* WAN 连接状态 */
#define WAN_IPV4_CONNECTED              "Connected"
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define WAN_IPV6_CONNECTED              "GlobalConnected"
#endif

/* zebra的配置文件 */
#define ZEBRA_CONFIG                    "/var/zebra.conf"
#define RIPD_CONFIG                     "/var/ripd.conf"
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define RIPNGD_CONFIG                   "/var/ripngd.conf"
#endif

/* rip版本值 */
#define DROUTE_VERSION_OFF              "Off"
#define DROUTE_VERSION_RIP1             "RIPv1"
#define DROUTE_VERSION_RIP2             "RIPv2"
#define DROUTE_VERSION_BOTH             "Both"
#define DROUTE_RIP_PORT            "520"

/* 注册节点的匹配路径 */
#define REG_NODE_PATH_IPCONN            "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define REG_NODE_PATH_PPPOE             "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define REG_NODE_PATH_LAN               "InternetGatewayDevice.LANDevice.{i}."

/* 动态路由维护的节点 */
#define NODE_ROUTE_RX                   "RouteProtocolRx"
#define NODE_ROUTE_PASSIVE          "RipPassive"
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define NODE_ROUTE_RIPNG_ENABLED        "RIPngEnabled"
#endif
#define NODE_DROUTE_INSTANCE_DEL        "X_TWSZ-COM_DelFlag"
#define NODE_DROUTE_WAN_STATUS          "X_TWSZ-COM_DynamicStatus"

/* WAN Connection 节点 */
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define NODE_WAN_IPV6_CONNECTION_STATUS "X_TWSZ-COM_IPv6Config.ConnectionStatus"
#define NODE_WAN_IPV6_DEVICE_NAME       "X_TWSZ-COM_IPv6Config.X_TWSZ-COM_ConnectedDeviceName"
#define NODE_WAN_IPV6_ENABLE            "X_TELEFONICA-ES_IPv6Enabled"
#endif

/* 动态路由节点路径 */
#define DROUTE_PATH_ATTACH              "[0-9]+\\."
#define DROUTE_BASE_PATH_PPP            "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\."
#define DROUTE_BASE_PATH_IPCONN         "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANIPConnection\\."
#define DROUTE_BASE_PATH_LAN            "^InternetGatewayDevice\\.LANDevice\\.[0-9]\\."

/* ADD消息匹配 */
#define DROUTE_ADD_PATH_PPP             DROUTE_BASE_PATH_PPP"$"
#define DROUTE_ADD_PATH_IPCONN          DROUTE_BASE_PATH_IPCONN"$"
/* DEL消息匹配 */
#define DROUTE_DEL_PATH_PPP             DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH"$"
#define DROUTE_DEL_PATH_IPCONN          DROUTE_BASE_PATH_IPCONN   DROUTE_PATH_ATTACH"$"
/* SET消息匹配 */
#define DROUTE_SET_PPP_ROUTE            DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH  NODE_ROUTE_RX"$"
#define DROUTE_SET_IPCONN_ROUTE         DROUTE_BASE_PATH_IPCONN  DROUTE_PATH_ATTACH   NODE_ROUTE_RX"$"
/* COMMIT消息匹配 */
#define DROUTE_COMMIT_PPP_ROUTE         DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH  NODE_ROUTE_RX"$"
#define DROUTE_COMMIT_IPCONN_ROUTE      DROUTE_BASE_PATH_IPCONN  DROUTE_PATH_ATTACH   NODE_ROUTE_RX"$"
#define DROUTE_COMMIT_LAN_ROUTE         DROUTE_BASE_PATH_LAN NODE_ROUTE_RX"$"

#define DROUTE_COMMIT_PPP_PASSIVE       DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH  NODE_ROUTE_PASSIVE"$"
#define DROUTE_COMMIT_IPCONN_PASSIVE    DROUTE_BASE_PATH_IPCONN  DROUTE_PATH_ATTACH   NODE_ROUTE_PASSIVE"$"
#define DROUTE_COMMIT_LAN_PASSIVE       DROUTE_BASE_PATH_LAN NODE_ROUTE_PASSIVE"$"

#ifdef CONFIG_TBS_SUPPORT_IPV6
#define DROUTE_COMMIT_PPP_RIPNG_ENABLED       DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH  NODE_ROUTE_RIPNG_ENABLED"$"
#define DROUTE_COMMIT_IPCONN_RIPNG_ENABLED    DROUTE_BASE_PATH_IPCONN  DROUTE_PATH_ATTACH   NODE_ROUTE_RIPNG_ENABLED"$"
#endif

/* 内部消息关键字 */
#define DROUTE_PATH_KEY                 KEY_PATH
#define DROUTE_KEY_INTERFACE_NAME       KEY_INTERFACE_NAME

/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/
/* WAN连接的状态 */
enum
{
    IF_STATE_DISCONNECTED = 0,
    IF_STATE_CONNECTED,
};

enum
{
    WAN_IPCONN = 0,
    WAN_PPPOE,
    LAN_CONN,
};

/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/
/* 状态机数据结构 */
typedef struct tag_ST_DROUTE_PROC{
    unsigned char   ucZebraState;                /* breza的重启状态机的状态 */
    unsigned char   ucRipState;                   /* rip的重启状态机的状态 */
#ifdef CONFIG_TBS_SUPPORT_IPV6
    unsigned char   ucRipngState;                   /* ripng的重启状态机的状态 */
#else
    unsigned char   pad_char_1;                   /* 4 Bytes aligned */
#endif
    unsigned char   pad_char_2;                   /* 4 Bytes aligned */
    unsigned long   ulZebraId;                    /* breza对应的id */
    char            szZebraName[MAX_PROC_NAME_LEN];       /*breza名字*/
    unsigned long   ulRipId;                    /* rip对应的id */
    char            szRipName[MAX_PROC_NAME_LEN];       /*rip名字*/
#ifdef CONFIG_TBS_SUPPORT_IPV6
    unsigned long   ulRipngId;                    /* ripng对应的id */
    char            szRipngName[MAX_PROC_NAME_LEN];       /*ripng名字*/
#endif
}ST_DROUTE_PROC;

/* 建立动态路由配置文件的时候, 需要获取此节点列表的信息 */
typedef struct tag_ST_DROUTE_REBUILD_FILE_LIST{
    char *szIfStatus;
    char *szIfName;
    char *iDelFlag;
}ST_DROUTE_REBUILD_FILE_LIST;

/*LAN侧节点列表信息*/
typedef struct tag_ST_DROUTE_LAN_FILE_LIST{
    char *szIfName;
}ST_DROUTE_LAN_FILE_LIST;

/* 判断是否启动zebra进程的时候,需要或者此节点列表的信息 */
typedef struct tag_ST_DROUTE_OPERATE_LIST{
    char *szIfStatus;
    char *szVersion;
    char *iDelFlag;
}ST_DROUTE_OPERATE_LIST;

#endif

/******************************************************************************
*                                 END                                        *
******************************************************************************/
