/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: ddns.h

 文件描述: ddns模块头文件

 修订记录:

        1. 作者: WuGuoxiang

           日期: 2009-04-07

           内容: 创建文件

**********************************************************************/

#ifndef __DDNS_H__
#define __DDNS_H__

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef DDNS_DEBUG
    #define DDNS_TRACE(fmt, args...) TBS_TRACE(MID_DDNS, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define DDNS_LOG_ERR(fmt, args...) TBS_TRACE(MID_DDNS, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define DDNS_TRACE(fmt, args...)   do { ; } while(0)
    #define DDNS_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_DDNS, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define DDNS_TRACE_INTO_FUNC DDNS_TRACE("##In## %s\n", __func__)
#define DDNS_TRACE_OUT_FUNC DDNS_TRACE("##Out## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define DDNS_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            DDNS_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define DDNS_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_DDNS, Ret, fmt, ##args)

#define DDNS_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_DDNS, Pointer, gotoTag, fmt, ##args)

#define DDNS_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            DDNS_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define DDNS_CHECK_NULL_PTR(ptr)    DDNS_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define DDNS_SET_NODE_VAL(pszPath, pszVal) \
    { \
        iRet = CFG_SetNodeVal(pszPath, pszVal, NULL); \
        DDNS_RET_RETURN(iRet, "Set node \"%s\" value failed\n", pszPath); \
    }

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define DDNS_PATH_PREFIX            "InternetGatewayDevice.X_TWSZ-COM_DDNS."

#define DDNS_NODE_PROVIDER          "DDNSProvider"
#define DDNS_NODE_HOSTNAME          "Hostname"
#define DDNS_NODE_WAN_CONN_ID       "WANConnectionID"
#define DDNS_NODE_WAN_INFSTATE      "WANInterfaceState"
#define DDNS_NODE_USERNAME          "Username"
#define DDNS_NODE_PASSWORD          "Password"
#define DDNS_NODE_METHOD          "Method"
#define DDNS_NODE_CGI_PATH          "CgiPath"
#define DDNS_NODE_DDNS_HOST          "DdnsHost"
#define DDNS_NODE_EMAIL             "Email"
#define DDNS_NODE_KEY               "Key"
#define DDNS_NODE_HASHKEY               "HashKey"
#define DDNS_NODE_STATE             "DDNSState"

#define WAN_DEV_PREFIX              "InternetGatewayDevice.WANDevice."
#define KEY_WAN_CONN_DEV            "WANConnectionDevice"
#define KEY_WAN_IP_CONN             "WANIPConnection"
#define KEY_WAN_PPP_CONN            "WANPPPConnection"

#define NODE_WAN_INTERFACE_NAME    "X_TWSZ-COM_ConnectedDeviceName"
#define NODE_WAN_IPV4_ENABLE       "X_TELEFONICA-ES_IPv4Enabled"
#define NODE_WAN_IPV6_ENABLE       "X_TELEFONICA-ES_IPv6Enabled"
#define NODE_CONNECTION_STATUS      "ConnectionStatus"
#define NODE_IP4_CONNECTION_STATUS      "ConnectionStatus"
#define NODE_IP6_CONNECTION_STATUS      "X_TWSZ-COM_IPv6Config.ConnectionStatus"

/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/
#define DDNS_PATH_PREFIX_REGEX \
        "^InternetGatewayDevice\\.X_TWSZ-COM_DDNS\\.[0-9]+\\."
#define DDNS_NODE_REGEX(node) DDNS_PATH_PREFIX_REGEX node "$"

#define WAN_IPCONN_INST_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\."\
                              KEY_WAN_CONN_DEV "\\.[0-9]+\\." \
                              KEY_WAN_IP_CONN "\\.[0-9]+$"

#define WAN_PPPCONN_INST_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\."\
                               KEY_WAN_CONN_DEV "\\.[0-9]+\\." \
                               KEY_WAN_PPP_CONN "\\.[0-9]+$"

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define Check_PARAMETER_FAILED  "Check parameter \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"

#define WAN_IPCONN_INST_PATH_FMT WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV ".%lu."\
                                 KEY_WAN_IP_CONN ".%lu"
                                                            
#define WAN_PPPCONN_INST_PATH_FMT WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV".%lu."\
                                  KEY_WAN_PPP_CONN ".%lu"

#define DDNS_INST_PATH_FMT  DDNS_PATH_PREFIX "%lu."

/****************************************************************
 *                         DDNS状态                             *
 ****************************************************************/
#define STATE_START_PROC_FAILED     "Start process failed"
#define STATE_WAN_CONN_DISCONNED    "WAN Connection disconnected"
#define STATE_IP_UPDATE_SUCC        "IP updated successful"
#define STATE_DOMAIN_NOT_REG        "Domain Name is not registered"
#define STATE_WRONG_USERPASS        "Wrong user name or password"
#define STATE_NO_CHANGES            "No changes"
#define STATE_INCORRECT_DOMAIN      "Not Fully-Qualified Domain Name"

/****************************************************************
 *                         WAN连接状态                          *
 ****************************************************************/
#define WAN_STATE_CONNECTED         "Connected"
#define WAN_STATE_DISCONNECTED      "Disconnected"
#define WAN_IP4_STAT_CONNECT        "Connected"
#define WAN_IP6_STAT_CONNECT        "GlobalConnected"

/****************************************************************
 *                         DDNS Provider                        *
 ****************************************************************/
#define DDNS_PROVIDER_DYNDNS        "DynDNS.org"
#define DDNS_PROVIDER_FREEDNS           "freedns.afraid.org"
#define DDNS_PROVIDER_3322          "3322.org"
#define DDNS_PROVIDER_DLINK          "dlinkddns.com"
#define DDNS_PROVIDER_NOIP         "www.noip.com"

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define MAX_DDNS_HOSTNAME_LEN           64
#define MAX_DDNS_WAN_CONN_ID_LEN        256
#define MAX_DDNS_GNUDIP_DDNS_HOST        256
#define MAX_DDNS_USERNAME_LEN           64
#define MAX_DDNS_PASSWORD_LEN           64
#define MAX_DDNS_EMAIL_LEN              64
#define MAX_DDNS_KEY_LEN                64
#define MAX_DDNS_PROCESS_LEN        64

#define MAX_DDNS_PATH_LEN               64

#define MAX_START_PARAM_NUM             20


/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/ 

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /*__DDNS_H__*/
