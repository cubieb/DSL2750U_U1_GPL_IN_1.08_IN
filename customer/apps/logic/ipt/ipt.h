/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: ipt.h
 文件描述: ipt模块头文件

 修订记录:
        1. 作者: XuXiaodong
           日期: 2008-12-22
           内容: 创建文件
**********************************************************************/
#ifndef __IPT_H__
#define __IPT_H__

#include "warnlog.h"
#include "tbstype.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef IPT_DEBUG
    #define IPT_TRACE(fmt, args...) COMMON_TRACE(MID_IPT, ECHO_GREEN fmt ECHO_NONE, ##args)
    #define IPT_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_IPT, ECHO_RED fmt ECHO_NONE, ##args)
    #define CMD_PRINT 1  /* 调用tbsSystem时打印命令 */
#else
    #define IPT_TRACE(fmt, args...)   do { ; } while(0)
    #define IPT_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_IPT, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define IPT_TRACE_INTO_FUNC  IPT_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define IPT_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            IPT_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define IPT_RET_RETURN(Ret, fmt, args...)  \
    RET_RETURN(MID_IPT, Ret, fmt, ##args)

#define IPT_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
    POINTER_GOTO(MID_IPT, Pointer, gotoTag, fmt, ##args)

#define IPT_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            IPT_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define IPT_CHECK_NULL_PTR(ptr)   IPT_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/

#define DEFAULTGW_PATH_LAN_IP     "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"
#define DEFAULTGW_PATH_LAN_MASK   "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask"

#define WAN_DEV_PREFIX      "InternetGatewayDevice.WANDevice."
#define KEY_WAN_CONN_DEV    "WANConnectionDevice"
#define KEY_WAN_IP_CONN     "WANIPConnection"
#define KEY_WAN_PPP_CONN    "WANPPPConnection"

#define IPT_NODE_NAT_EN      "NATEnabled"
#define IPT_NODE_NAT_TYPE    "X_TELEFONICA-ES_NATType"
#define IPT_NODE_DMZ_EN      "X_TWSZ-COM_DMZEnabled"
#define IPT_NODE_DMZ_HOST    "X_TWSZ-COM_DMZHost"
#define IPT_NODE_NAT_PUB_IP  "Natipaddr"

/*add by chenzhuo*/
//#define IPT_NODE_SVC_TYPE "X_TWSZ-COM_SvcType"
/*add by chenzhuo*/

#define IP_CONN_INST_PATH     WAN_DEV_PREFIX "{i}." KEY_WAN_CONN_DEV ".{i}." KEY_WAN_IP_CONN ".{i}."
#define PPP_CONN_INST_PATH    WAN_DEV_PREFIX "{i}." KEY_WAN_CONN_DEV ".{i}." KEY_WAN_PPP_CONN ".{i}."

#define IPT_PATH_LAN_IP     "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"


/*****************************************************************
 *                   节点路径匹配正则表达式                      *
 *****************************************************************/
#define WAN_CONN_INST_REGEX  "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\." KEY_WAN_CONN_DEV \
                             "\\.[0-9]+\\.("KEY_WAN_IP_CONN"|"KEY_WAN_PPP_CONN")\\.[0-9]+"

#define END "$"
#define IPT_NODE_REGEX(node) WAN_CONN_INST_REGEX "\\." node END

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"

#define WAN_CONN_INST_PATH_FMT WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV ".%lu.WAN%sConnection.%lu."
#define WAN_CONN_INST_PATH_FMT_SSCANF WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV ".%lu.%*[^.].%lu"

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define CONE_NAT      "full cone"
#define SYMMETRIC_NAT "symmetric"

#define SVC_MANAGE "Manage"
#define SVC_BUSINESS "Internet"
#define SVC_BOTH "Both"

#define IPT_PRECHAIN   "IPT_PREROUTING"
#define IPT_POSTCHAIN  "IPT_POSTROUTING"

#define IPT_SUBCMD_LEN_1  8
#define IPT_SUBCMD_LEN_2  16
#define IPT_SUBCMD_LEN_3  24
#define MAX_CMD_LENGTH    128

/******************************************************************************
 *                                   ENUM                                     *
 ******************************************************************************/
typedef enum tag_EN_WAN_CONN_TYPE {
    UNKNOWN_CONN,
    WAN_IP_CONN,
    WAN_PPP_CONN
} EN_WAN_CONN_TYPE;

enum {
    IPT_APPLY_NONE,
    IPT_APPLY_ADD,
    IPT_APPLY_DEL
};

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
typedef struct tag_ST_IPT_Instance {
    struct tag_ST_IPT_Instance *pstNext;
    char szIfName[MAX_IF_NAME_LEN];
    char szIpAddr[MAX_IP_LEN];
    bool bWanConnState;
    EN_WAN_CONN_TYPE enWanConnType;
    struct {
        unsigned long ulWanDevIdx;
        unsigned long ulWanConnDevIdx;
        unsigned long ulWanConnIdx;
    }stIndex;

}ST_IPT_Instance;

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/
#endif /*__IPT_H__*/

