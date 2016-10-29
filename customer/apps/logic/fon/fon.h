/**********************************************************************
 Copyright (c), 2012-2022, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: fon.h
 文件描述: fon模块头文件

 修订记录:
        1. 作者: zhangzhihua
           日期: 2012-02-25
           内容: 创建文件
**********************************************************************/
#ifndef __FON_H__
#define __FON_H__

#include "warnlog.h"
#include "tbstype.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define FON_MAX_ARGS 16
#define CMD_STR_LEN  500
#define MAC_STR_LEN  18
#define MAC_ADDR_LEN 6
#define FON_NAME	"chilli"
#define FON_BIN_NAME "/bin/chilli"
#define FON_CONF_FILE "/tmp/chilli.conf"
/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef FON_DEBUG
    #define FON_TRACE(fmt, args...) COMMON_TRACE(MID_FON, ECHO_GREEN fmt ECHO_NONE, ##args)
    #define FON_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_FON, ECHO_RED fmt ECHO_NONE, ##args)
    #define CMD_PRINT 1  /* 调用tbsSystem时打印命令 */
#else
    #define FON_TRACE(fmt, args...)   do { ; } while(0)
    #define FON_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_FON, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define FON_TRACE_INTO_FUNC  FON_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define FON_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            FON_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define FON_RET_RETURN(Ret, fmt, args...)  \
    RET_RETURN(MID_FON, Ret, fmt, ##args)

#define FON_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
    POINTER_GOTO(MID_FON, Pointer, gotoTag, fmt, ##args)

#define FON_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            FON_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define FON_CHECK_NULL_PTR(ptr)   FON_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
//无线使能节点路径
#define STATIC_WLANENABLE 			"InternetGatewayDevice.X_TWSZ-COM_WLAN.WirelessEnable"
/* 静态路由节点路径 */
#define STATIC_MAIN_PATH        	"InternetGatewayDevice"
#define STATIC_NODE_DEFAULT_ROUTE   "X_TWSZ-COM_DEFAULT_GW"
#define NODE_WLAN_VAPINDEX			"X_TWSZ-COM_VapIndex"
#define STATIC_FONWLANENABLE		"InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.Enable"
#define NODE_CONN_DEV_NAME  		"X_TWSZ-COM_DeviceName"
#define NODE_WAN_CONN_TYPE			"ConnectionType"
#define NODE_WAN_CONN_STATUS		"ConnectionStatus"
#define NODE_WAN_CONN_DEVNAME		"X_TWSZ-COM_ConnectedDeviceName"
#define NODE_DHCP_ENABLE			"InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPServerEnable"


#define NODE_VAL_TRUE						"True"
#define NODE_VAL_TRUE1						"1"
#define NODE_CONN_TYPE						"IP_Routed"
#define NODE_CONN_STATUS					"Connected"

/*****************************************************************
 *                   节点路径匹配正则表达式                      *
 *****************************************************************/
#define END "$"
#define WAN_ENABLE  "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.WLANConfiguration\\.[0-9]+\\.Enable" 

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"

#define WAN_CONN_INST_PATH_FMT WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV ".%lu.WAN%sConnection.%lu."
#define WAN_CONN_INST_PATH_FMT_SSCANF WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV ".%lu.%*[^.].%lu"


/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/


/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/
#endif /*__FON_H__*/

