/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: dynamicrouting.h
 文件描述: 动态路由头文件
 修订记录:
        1. 作者: 朱健文
           日期: 
           内容: 创建文件
**********************************************************************/
#ifndef _DYNAMICROUTE_H_
#define _DYNAMICROUTING_H_

#define DYNAMIC_MOD  "[DYNAMIC :]"
/******************************************************************************
*                                 DEBUG                                      *
******************************************************************************/
//#define DYNAMIC_DEBUG 1
#ifdef  DYNAMIC_DEBUG
#define DYNAMICTRACE1  //打印必要的信息
#define DYNAMICTRACE2  //打印不必要的信息
#define DYNAMICTRACE3
#endif

#ifdef  DYNAMICTRACE1
#define DYNAMIC_TRACE1(fmt, args...)  printf(DYNAMIC_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define DYNAMIC_TRACE1(fmt, args...)  do{;}while(0);
#endif
#ifdef  DYNAMICTRACE2
#define DYNAMIC_TRACE2(fmt, args...)  printf(DYNAMIC_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define DYNAMIC_TRACE2(fmt, args...)  do{;}while(0);
#endif
#ifdef  DYNAMICTRACE3
#define DYNAMIC_TRACE3(fmt, args...)  printf(fmt, ##args)
#else
#define DYNAMIC_TRACE3(fmt, args...)  do{;}while(0);
#endif

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
/* 出错的时候打印宏定义 */
#define DYNAMIC_PRINTF(fmt, args...)  COMMON_TRACE(MID_DYNAMIC_ROUTING, DYNAMIC_MOD "[ERROR] " "%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#define DYNAMIC_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            DYNAMIC_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
            goto gotoTag; \
        }\
      }

/* 一般的宏定义 */
#define DYNAMIC_MOD_ID          MID_DYNAMIC_ROUTING
#define DYNAMIC_NODE_LENGTH     256
#define DYNAMIC_PATH_LENGTH     256
#define MAX_PROC_NAME_LEN       16
#define DYNAMIC_ZEBRA_NAME      "zebra"
#define DYNAMIC_RIP_NAME        "rip"
#define DYNAMIC_ZEBRA_ID        1 
#define DYNAMIC_RIP_ID          2
#define DYNAMIC_INIT_NAME       ""
#define MAX_DYNAMIC_EXECINFO_COUNT  5
#define MAX_INDEX_COUNT             3

#define DYNAMIC_WANDEV          "InternetGatewayDevice.WANDevice"
#define DYNAMIC_WANCONNDEV      "WANConnectionDevice"
#define DYNAMIC_WANPPPCONN      "WANPPPConnection"
#define DYNAMIC_WANIPCONN       "WANIPConnection"
#define DYNAMIC_LANDEV            "InternetGatewayDevice.LANDevice"

/* zebra的配置文件 */
#define ZEBRA_CONFIG            "/var/zebra.conf"
#define RIPD_CONFIG             "/var/ripd.conf"

/* rip版本值 */
#define DYNAMIC_VERSION_OFF     "Off"
#define DYNAMIC_VERSION_RIP1    "RIPv1"
#define DYNAMIC_VERSION_RIP2    "RIPv2"
#define DYNAMIC_VERSION_OSPF    "OSPF"

/* 注册节点的匹配路径 */
#define REG_NODE_PATH_IPCONN    "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define REG_NODE_PATH_PPPOE     "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define REG_NODE_PATH_LAN        "InternetGatewayDevice.LANDevice.{i}."

/* 动态路由维护的节点 */
#define NODE_ROUTE_RX               "RouteProtocolRx"
#define NODE_ROUTE_PASSIVE          "RipPassive"
#define NODE_DYNAMIC_INSTANCE_DEL   "X_TWSZ-COM_DelFlag"
#define NODE_DYNAMIC_WAN_STATUS     "X_TWSZ-COM_DynamicStatus"

/* 动态路由节点路径 */
#define DYNAMIC_PATH_ATTACH         "[0-9]+\\."
#define DYNAMIC_BASE_PATH_PPP       "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\."
#define DYNAMIC_BASE_PATH_IPCONN    "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANIPConnection\\."
#define DYNAMIC_BASE_PATH_LAN   "^InternetGatewayDevice\\.LANDevice\\.[0-9]\\."

/* ADD消息匹配 */
#define DYNAMIC_ADD_PATH_PPP        DYNAMIC_BASE_PATH_PPP"$"
#define DYNAMIC_ADD_PATH_IPCONN     DYNAMIC_BASE_PATH_IPCONN"$"
/* DEL消息匹配 */
#define DYNAMIC_DEL_PATH_PPP        DYNAMIC_BASE_PATH_PPP  DYNAMIC_PATH_ATTACH"$"
#define DYNAMIC_DEL_PATH_IPCONN     DYNAMIC_BASE_PATH_IPCONN   DYNAMIC_PATH_ATTACH"$"
/* SET消息匹配 */
#define DYNAMIC_SET_PPP_ROUTE       DYNAMIC_BASE_PATH_PPP  DYNAMIC_PATH_ATTACH  NODE_ROUTE_RX"$"
#define DYNAMIC_SET_IPCONN_ROUTE    DYNAMIC_BASE_PATH_IPCONN  DYNAMIC_PATH_ATTACH   NODE_ROUTE_RX"$"
#define DYNAMIC_SET_LAN_ROUTE       DYNAMIC_BASE_PATH_LAN NODE_ROUTE_RX"$"
/* COMMIT消息匹配 */
#define DYNAMIC_COMMIT_PPP_ROUTE    DYNAMIC_BASE_PATH_PPP  DYNAMIC_PATH_ATTACH  NODE_ROUTE_RX"$"
#define DYNAMIC_COMMIT_IPCONN_ROUTE    DYNAMIC_BASE_PATH_IPCONN  DYNAMIC_PATH_ATTACH   NODE_ROUTE_RX"$"
#define DYNAMIC_COMMIT_LAN_ROUTE   DYNAMIC_BASE_PATH_LAN NODE_ROUTE_RX"$"

#define DYNAMIC_COMMIT_PPP_PASSIVE    DYNAMIC_BASE_PATH_PPP  DYNAMIC_PATH_ATTACH  NODE_ROUTE_PASSIVE"$"
#define DYNAMIC_COMMIT_IPCONN_PASSIVE    DYNAMIC_BASE_PATH_IPCONN  DYNAMIC_PATH_ATTACH   NODE_ROUTE_PASSIVE"$"
#define DYNAMIC_COMMIT_LAN_PASSIVE   DYNAMIC_BASE_PATH_LAN NODE_ROUTE_PASSIVE"$"
/* 内部消息关键字 */
#define DYNAMIC_PATH_KEY            KEY_PATH
#define DYNAMIC_KEY_INTERFACE_NAME  KEY_INTERFACE_NAME

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
typedef struct tag_ST_DYNAMIC_PROC{
    unsigned char   ucZebraState;                /* breza的重启状态机的状态 */
    unsigned char   ucRipState;                   /* rip的重启状态机的状态 */
    unsigned long   ulZebraId;                    /* breza对应的id */
    char            szZebraName[MAX_PROC_NAME_LEN];       /*breza名字*/
    unsigned long   ulRipId;                    /* rip对应的id */
    char            szRipName[MAX_PROC_NAME_LEN];       /*rip名字*/
}ST_DYNAMIC_PROC;

/* 建立动态路由配置文件的时候, 需要获取此节点列表的信息 */
typedef struct tag_ST_DYNAMIC_REBUILD_FILE_LIST{
    char *szIfStatus;
    char *szIfName;
    char *iDelFlag;
}ST_DYNAMIC_REBUILD_FILE_LIST;

/*LAN侧节点列表信息*/
typedef struct tag_ST_DYNAMIC_LAN_FILE_LIST{
    char *szIfName;
}ST_DYNAMIC_LAN_FILE_LIST;

/* 判断是否启动zebra进程的时候,需要或者此节点列表的信息 */
typedef struct tag_ST_DYNAMIC_OPERATE_LIST{
    char *szIfStatus;
    char *szVersion;
    char *iDelFlag;
}ST_DYNAMIC_OPERATE_LIST;

#endif

/******************************************************************************
*                                 END                                        *
******************************************************************************/
