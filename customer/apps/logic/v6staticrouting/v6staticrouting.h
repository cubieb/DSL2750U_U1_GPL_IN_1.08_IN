/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: staticrouting.h
 文件描述: 静态路由头文件
 修订记录:
        1. 作者: 黄继东
           日期:
           内容: 创建文件
**********************************************************************/

#ifndef _V6STATICROUTING_H_
#define _V6STATICROUTING_H_

#define V6STATIC_MOD  "[V6STATIC :]"
/******************************************************************************
*                                 DEBUG                                      *
******************************************************************************/
#ifdef  V6STATIC_DEBUG
#define V6STATICTRACE1  /* 打印必要的信息 */
#define V6STATICTRACE2  /* 打印不必要的信息 */
#define V6STATICTRACE3
#define V6STATICASSERT
#endif

#ifdef  V6STATICTRACE1
#define V6STATIC_TRACE1(fmt, args...)  printf(V6STATIC_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define V6STATIC_TRACE1(fmt, args...)  do{;}while(0);
#endif
#ifdef  V6STATICTRACE2
#define V6STATIC_TRACE2(fmt, args...)  printf(V6STATIC_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define V6STATIC_TRACE2(fmt, args...)  do{;}while(0);
#endif
#ifdef  V6STATICTRACE3
#define V6STATIC_TRACE3(fmt, args...)  printf(fmt, ##args)
#else
#define V6STATIC_TRACE3(fmt, args...)  do{;}while(0);
#endif

#ifdef V6STATICASSERT
#define V6STATIC_RT_ASSERT(exp) \
if (!(exp)) \
{ \
    V6STATIC_TRACE1("ASSERT FAIL"); \
    exit(-1); \
}
#else
#define V6STATIC_RT_ASSERT(exp)  do{;}while(0);
#endif

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define  V6STATIC_RT_TRACE_INTO_FUNC  V6STATIC_TRACE1("##In## %s\n", __func__)

/* 出错的时候打印宏定义 */
#define V6STATIC_PRINTF(fmt, args...)  printf(V6STATIC_MOD "[ERROR] " "%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#define V6STATIC_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            V6STATIC_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
            goto gotoTag; \
        }\
      }

/* 常用的宏定义 */
#define V6STATIC_System(para0)    tbsSystem(para0, 0)
#define V6STATIC_MOD_ID           MID_V6STATIC_ROUTING
#define V6STATIC_NODE_LENGTH      256
#define V6STATIC_PATH_LENGTH      256
#define MAX_INDEX_COUNT         1
#define MAX_CMD_LENGTH          256
#define MAX_RULE_LENGTH         256
#define MAX_IPV6_LEN            40
#define MAX_COUNT_PER_ITEM      16
#define V6STATIC_NAME_INIT        ""
#define V6STATIC_ROUTE_RULE_INIT  ""
#define ROUTE_ADD_RULE          "route add"
#define ROUTE_DEL_RULE          "route del"
#define DEFAULT_ROUTE_DEL       "route del default"

#define NOT_ERROR_INFO          ""
#define DEFAULT_ERROR_INFO      "ERROR"
#define DEVICE_NOT_CONNECTED_ERROR  "Device not connected"

#define LAN_DEVICE_NAME         "X_TWSZ-COM_DeviceName"
#define LAN_DEVICE_PPP_IF_NAME  "X_TWSZ-COM_ConnectedDeviceName"
#define LAN_PHY_DEVICE_PATH     "InternetGatewayDevice.LANDevice"

#define V6STATIC_IPCONN_DEVICE              "InternetGatewayDevice.WANDevice"
#define V6STATIC_IPCONN_CONNDEVICE          "WANConnectionDevice"
#define V6STATIC_IPCONN_INSTANCE            "WANIPConnection"
#define V6STATIC_V6WANCONN_INSTANCE         "X_TWSZ-COM_IPv6Config"
#define V6STATIC_PPPCONN_INSTANCE           "WANPPPConnection"
#define V6STATIC_IPCONN_NODE_CONN_STATUS    "ConnectionStatus"
#define V6STATIC_IPCONN_NODE_DEF_RT_LIST    "IPv6DefaultRouterList"
#define V6STATIC_IPCONN_NODE_DEF_RT_ADDR    "DefaultRouter"
#define V6STATIC_LAN_DEVICE_FEATURE         ".LANDevice."
#define V6STATIC_WAN_SERVICE_TYPE           "X_TWSZ-COM_ServiceList"
#define V6STATIC_WAN_SERVICE_INTERNET       "INTERNET"

#define V6STATIC_IPCONN_CONN_STATUS         "GlobalConnected"
#define V6STATIC_IPCONN_CONN_STATUS_DISCONNECTED         "Disconneted"
#define V6STATIC_IPCONN_DEF_RT_STATUS       "Valid"
#define V6STATIC_LAN_DEVICE_NAME            "br0"

/* 内部消息关键字段 */
#define V6STATIC_VALUE_ENABLE     VALUE_ENABLE
#define V6STATIC_VALUE_DISABLE    VALUE_DISABLE
#define V6STATIC_VALUE_ERROR      VALUE_ERROR
#define V6STATIC_KEY_PATH         KEY_PATH
#define V6STATIC_KEY_INTERFACE_NAME   KEY_INTERFACE_NAME
#define V6STATIC_KEY_REMOVE_IP    KEY_REMOTE_IP

/* 配置规则的时候错误的LOG文件 */
#define ROUTE_LOG_FILE          "/var/log/staticrouting"
#define DEFAULT_ROUTE_LOG_FILE  "/var/log/defaultrouting"

/* 静态路由节点路径 */
#define V6STATIC_MAIN_PATH        "InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding"
#define V6STATIC_FORWARD_PATH     "InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.IPv6Forwarding"

/* tr069节点 */
#define V6STATIC_NODE_DEFAULT_ROUTE   "DefaultConnectionService"
#define V6STATIC_NODE_INSTANCE_NUM    "ForwardNumberOfEntries"

#define V6STATIC_NODE_ENABLE          "Enable"
#define V6STATIC_NODE_STATUS          "Status"
#define V6STATIC_NODE_DESTIP          "DestIPv6Address"
#define V6STATIC_NODE_DESTMASK        "DestPrefixLength"
#define V6STATIC_NODE_SRC_IP          "SourceIPv6Address"
#define V6STATIC_NODE_SRC_MASK        "SourcePrefixLength"
#define V6STATIC_NODE_GATEWAY         "GatewayIPv6Address"
#define V6STATIC_NODE_INTERFACE       "Interface"
#define V6STATIC_NODE_METRIC          "ForwardingMetric"
#define V6STATIC_NODE_MTU             "MTU"

#define V6STATIC_NODE_FLAG            "StaticRoute"
#define V6STATIC_NODE_ERRORINFO       "X_TWSZ-COM_ErrorInfo"

/* 匹配路径 */
#define V6STATIC_DEFAULT_ROUTE_PATH   "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPv6Layer3Forwarding\\."
#define V6STATIC_BASE_PATH            "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPv6Layer3Forwarding\\.IPv6Forwarding\\."
#define V6STATIC_INSTANCE_PATH        "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPv6Layer3Forwarding\\.IPv6Forwarding\\.[0-9]+\\."

/* ADD消息匹配 */
#define V6STATIC_ADD_MATCH_PATH       V6STATIC_BASE_PATH"$"

/* SET消息匹配 */
#define V6STATIC_SET_ENABLE   V6STATIC_INSTANCE_PATH    V6STATIC_NODE_ENABLE"$"
#define V6STATIC_SET_DESTIP   V6STATIC_INSTANCE_PATH    V6STATIC_NODE_DESTIP "$"
#define V6STATIC_SET_DESTMASK   V6STATIC_INSTANCE_PATH    V6STATIC_NODE_DESTMASK "$"
#define V6STATIC_SET_SRCTIP   V6STATIC_INSTANCE_PATH    V6STATIC_NODE_SRC_IP "$"
#define V6STATIC_SET_SRCMASK   V6STATIC_INSTANCE_PATH    V6STATIC_NODE_SRC_MASK "$"
#define V6STATIC_SET_GATEWAY   V6STATIC_INSTANCE_PATH   V6STATIC_NODE_GATEWAY "$"
#define V6STATIC_SET_INTERFACE   V6STATIC_INSTANCE_PATH   V6STATIC_NODE_INTERFACE "$"
#define V6STATIC_SET_METRIC   V6STATIC_INSTANCE_PATH     V6STATIC_NODE_METRIC"$"
#define V6STATIC_SET_DEFAULT_ROUTE   V6STATIC_DEFAULT_ROUTE_PATH   V6STATIC_NODE_DEFAULT_ROUTE "$"

/* COMMIT消息匹配 */
#define V6STATIC_COMMIT_DEFAULT_ROUTE V6STATIC_SET_DEFAULT_ROUTE
#define V6STATIC_COMMIT_NODE  V6STATIC_SET_ENABLE "|" V6STATIC_SET_DESTIP "|" V6STATIC_SET_DESTMASK "|" \
                            V6STATIC_SET_GATEWAY "|" V6STATIC_SET_INTERFACE "|" V6STATIC_SET_METRIC

/* 默认路由匹配 */
#define V6STATIC_DEL_MATCH_PATH V6STATIC_INSTANCE_PATH"$"


/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/
enum
{
    V6STATIC_RT_ADD = 0,
    V6STATIC_RT_DEL,
};

/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/
/* V6默认路由信息 */
typedef struct tag_ST_V6DEFAULT_INFO{
    struct tag_ST_V6DEFAULT_INFO *pNext;
    struct tag_ST_V6DEFAULT_INFO *pPre;
    char szIfAddr[MAX_IPV6_LEN];        /* 默认路由的IP地址 */
    char szIfName[MAX_IF_NAME_LEN];     /* 默认路由的接口名 */
}ST_V6DEFAULT_INFO;

/* V6 静态路由信息 */
typedef struct tag_ST_V6STATIC_RT_INFO{
    struct tag_ST_V6STATIC_RT_INFO *pNext;
    char szV6DstAddr[MAX_IPV6_LEN];     /* 目的IP地址 */
    char szV6DstPrefix[32];             /* 目的IP的掩码 */
    char szV6SrcAddr[MAX_IPV6_LEN];     /* 源IP地址 */
    char szV6SrcPrefix[32];             /* 源IP的掩码 */
    char szMetric[32];                  /* metric */
    char szV6GWAddr[MAX_IPV6_LEN];      /* 默认网关IP地址*/
    char szDevName[MAX_IF_NAME_LEN];    /* 设备名 */
}ST_V6STATIC_RT_INFO;

/* V6默认路由信息链表头 */
typedef struct tag_ST_V6DEFAULT_INFO_HEAD{
    struct tag_ST_V6DEFAULT_INFO *pNext;
    struct tag_ST_V6DEFAULT_INFO *pPre;
}ST_V6DEFAULT_INFO_HEAD;


#define TBS_E8_TELECOM

/******************************************************************************
*                                FUNCTION                                      *
******************************************************************************/
int V6STATIC_GetDefault(char *pszValue);
#endif

/******************************************************************************
*                                 END                                        *
******************************************************************************/
