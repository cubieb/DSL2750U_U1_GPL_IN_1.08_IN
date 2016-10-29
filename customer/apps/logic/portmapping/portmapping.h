/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: portmapping.h
 文件描述: portmapping模块头文件

 修订记录:
        1. 作者: XuXiaodong
           日期: 2008-12-23
           内容: 创建文件
**********************************************************************/
#ifndef __PTMP_H__
#define __PTMP_H__

#include "warnlog.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef PTMP_DEBUG
    #define PTMP_TRACE(fmt, args...) COMMON_TRACE(MID_PORTMAPPING, ECHO_GREEN fmt ECHO_NONE, ##args)
    #define PTMP_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_PORTMAPPING, ECHO_RED fmt ECHO_NONE, ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define PTMP_TRACE(fmt, args...)
    #define PTMP_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_IPT, fmt, ##args)
    #define CMD_PRINT 0   
#endif


#define PTMP_TRACE_INTO_FUNC  PTMP_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define PTMP_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            PTMP_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define PTMP_RET_RETURN(Ret, fmt, args...)  \
    RET_RETURN(MID_PORTMAPPING, Ret, fmt, ##args)

#define PTMP_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
    POINTER_GOTO(MID_PORTMAPPING, Pointer, gotoTag, fmt, ##args)

#define PTMP_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            PTMP_TRACE(fmt, ##args); \
            return Ret; \
        }

#define PTMP_CHECK_NULL_PTR(ptr)   PTMP_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define WAN_DEV_PREFIX      "InternetGatewayDevice.WANDevice."
#define KEY_WAN_CONN_DEV    "WANConnectionDevice"
#define KEY_WAN_IP_CONN     "WANIPConnection"
#define KEY_WAN_PPP_CONN    "WANPPPConnection"

#define PTMP_NODE_NUM_OF_ENTRY    "PortMappingNumberOfEntries"
#define PTMP_NODE_MAX_ENTRY_NUM   "X_TWSZ-COM_MaxPortMappingEntries"
#define PTMP_NODE_ENABLE          "PortMappingEnabled"
#define PTMP_NODE_DURATION        "PortMappingLeaseDuration"
#define PTMP_NODE_REMOTE_HOST     "RemoteHost"
#define PTMP_NODE_REMOTE_MASK     "X_TWSZ-COM_RemoteMask"
#define PTMP_NODE_EXTER_PORT      "ExternalPort"
#define PTMP_NODE_EXTER_PORTEND   "ExternalPortEndRange"
#define PTMP_NODE_INTER_PORT      "InternalPort"
#define PTMP_NODE_INTER_PORTEND   "InternalPortEndRange"
#define PTMP_NODE_PROTOCOL        "PortMappingProtocol"
#define PTMP_NODE_INTER_CLIENT    "InternalClient"
#define PTMP_NODE_DESCRIPTION     "PortMappingDescription"
#define PTMP_OBJECT_PORTMAPPING   "PortMapping"
#define PTMP_NODE_WEIGHT          "X_TWSZ-COM_Weight"
#define PTMP_NODE_TIME    "PortMappingTime"
#define ACL_PORT_PATH    "InternetGatewayDevice.X_TWSZ-COM_ACL.RACL."
#define ACL_NODE_PORT   "ExternalPort"
#define ACL_NODE_PROTO  "Protocol"
#define ACL_NODE_SERVICE  "Service"
#define ACL_NODE_DEVPATH  "DevPath"

#define LAN_HOST_PATH "InternetGatewayDevice.LANDevice.1.Hosts.Host."
#define MAX_HOST_NAME_LEN 257
#define MAX_INDEX_COUNT                 3

#define IP_CONN_INST_PATH     WAN_DEV_PREFIX "{i}." KEY_WAN_CONN_DEV ".{i}." KEY_WAN_IP_CONN ".{i}."
#define PPP_CONN_INST_PATH    WAN_DEV_PREFIX "{i}." KEY_WAN_CONN_DEV ".{i}." KEY_WAN_PPP_CONN ".{i}."

/*****************************************************************
                    节点路径匹配正则表达式
*****************************************************************/
#define WAN_CONN_INST_REGEX  "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\." KEY_WAN_CONN_DEV \
                             "\\.[0-9]+\\.("KEY_WAN_IP_CONN"|"KEY_WAN_PPP_CONN")\\.[0-9]+"

#define WAN_CONN_PTMP_INST_REGEX  WAN_CONN_INST_REGEX "\\." PTMP_OBJECT_PORTMAPPING "\\.[0-9]+\\."
#define END "$"
#define PTMP_NODE_REGEX(node) WAN_CONN_PTMP_INST_REGEX node END

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"
#define INVALID_NODE_VALUE      "Invalid \"%s\" value: %s\n"

#define WAN_CONN_INST_PATH_FMT WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV ".%lu.WAN%sConnection.%lu."
#define WAN_CONN_INST_PATH_FMT_SSCANF WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV ".%lu.%*[^.].%lu"
#define WAN_CONN_PTMP_INST_FMT_SSCANF WAN_DEV_PREFIX "%*u." KEY_WAN_CONN_DEV \
                                      ".%*u.%*[^.].%*u." PTMP_OBJECT_PORTMAPPING ".%lu"

#define PTMP_PRE_SUBCHAIN_FMT  "PTMP_PRECHAIN_%s_%lu_%lu_%lu"
#define PTMP_POST_SUBCHAIN_FMT "PTMP_POSTCHAIN_%s_%lu_%lu_%lu"

/*****************************************************************
 *                        规则权重位                             *
 *****************************************************************/
#define REMOTE_HOST_EXPLICIT 0x00100000UL
#define REMOTE_HOST_WILDCARD 0x00010000UL
#define EXTERN_PORT_EXPLICIT 0x00000010UL
#define EXTERN_PORT_WILDCARD 0x00000001UL

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define PTMP_PRECHAIN   "PTMP_PREROUTING"
#define PTMP_POSTCHAIN  "PTMP_POSTROUTING"

#define PTMP_ADD_SUBCHAIN 1
#define PTMP_DEL_SUBCHAIN 2

#define PTMP_BUF_LEN_1  8
#define PTMP_BUF_LEN_2  16
#define PTMP_BUF_LEN_3  24
#define PTMP_BUF_LEN_4  32
#define PTMP_BUF_LEN_5  128

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
/* WAN连接类型 */
typedef enum tag_EN_WAN_CONN_TYPE {
    UNKNOWN_CONN,
    WAN_IP_CONN,
    WAN_PPP_CONN
} EN_WAN_CONN_TYPE;

/* 规则应用类型 */
enum {
    PTMP_APPLY_NONE,
    PTMP_APPLY_ADD,
    PTMP_APPLY_DEL
};

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
/* portmapping实例结构 */
typedef struct tag_ST_PTMP_INSTANCE {
    const char *pszEnable;
    const char *pszRemoteHost;
    const char *pszRemoteMask;
    const char *pszExternPort;
    const char *pszExternPortEnd;
    const char *pszInternalPort;
    //const char *pszInternalPortEnd;
    const char *pszProtocol;
    const char *pszInterClient;
    const char *pszWeight;
    const char *pszTime;
    unsigned long ulPtmpIndex;
} ST_PTMP_INSTANCE;

/* WAN连接实例结构 */
typedef struct tag_ST_PTMP_WANCONN_INSTANCE {
    struct tag_ST_PTMP_WANCONN_INSTANCE *pstNext;
    char szIpAddr[MAX_IP_LEN];
    bool bWanConnState;
    EN_WAN_CONN_TYPE enWanConnType;
    struct {
        unsigned long ulWanDevIdx;
        unsigned long ulWanConnDevIdx;
        unsigned long ulWanConnIdx;
    }stIndex;
} ST_PTMP_WANCONN_INSTANCE;

#define PTMP_DOMAIN_SUPPORT
#ifdef PTMP_DOMAIN_SUPPORT
typedef struct tag_ST_PTMP_USED_HOST {
    char szHostName[MAX_HOST_NAME_LEN];
    char szHostIp[MAX_IP_LEN];
    int iUsedCount;
    TBS_ST_LIST_HEAD lhPtmpHost;
}ST_PTMP_USED_HOST;
#endif

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/
#endif /*__PTMP_H__*/

