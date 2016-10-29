/**********************************************************************
 Copyright (c), 1991-2010, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: customer_device.h
 文件描述: tf_gui 模块头文件

 修订记录:
        1. 作者: 
           日期: 2010-07-09
           内容: 创建文件
**********************************************************************/

#ifndef __TF_GUI_H__
#define __TF_GUI_H__

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#define MAX_PATH_LEN        257

#ifdef TF_GUI_DEBUG
    #define TF_GUI_TRACE(fmt, args...) COMMON_TRACE(MID_TF_GUI, ECHO_GREEN fmt ECHO_NONE, ##args)
    #define TF_GUI_LOG_ERR(fmt, args...) COMMON_TRACE(MID_TF_GUI, ECHO_RED fmt ECHO_NONE, ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */

    #define TF_GUI_ASSERT(expr) \
    do{ \
        if(!(expr))\
        {\
            TF_GUI_TRACE("Assert \"%s\" failed\n", #expr); \
            exit(-1); \
        } \
    }while(0);
#else
    #define TF_GUI_TRACE(fmt, args...) 
    #define TF_GUI_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_TF_GUI, fmt, ##args)
    #define CMD_PRINT 0

    #define TF_GUI_ASSERT(expr) do {} while(0);
#endif

#define  TFGUILOG_TIME_INTERVAL             10
static unsigned long g_ulTimerID = 0;     //定时器ID
typedef struct tag_ST_USB_DEV{
	 //struct list_head head;
	 char szProduct[64];
	 char szSNumber[64];
         char szType[64];
}ST_USB_DEV;

#define TF_GUI_TRACE_INTO_FUNC TF_GUI_TRACE("##In## %s\n", __func__)

/* 表示一个设备信息*/
typedef struct tag_ST_CUST_DevInfo {
    char *pszIpAddr;
    char *pszNameDevice;
    char *pszIddevice;
    char *pszMacAddr;
    char *pszInterfacetype;
    char *pszType;
    char *pszStatus;
    char *pszSnumber;
}ST_CUST_DevInfo;

/* 表示一个Rule规则需要用到的参数*/
typedef struct tag_ST_RuleInfo {
    char *pszIpAssigned;
    char *pszProtocol;
    char *pszStartWan;
    char *pszEndWan;
    char *pszStartLan;
    char *pszEndLan;
}ST_RuleInfo;

typedef enum tag_RuleOpt
{
    IPT_RULE_ADD = 0,
    IPT_RULE_DEL,
}RuleOpt;


/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define TF_GUI_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            TF_GUI_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define TF_GUI_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_TF_GUI, Ret, fmt, ##args)

#define TF_GUI_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_TF_GUI, Pointer, gotoTag, fmt, ##args)

#define TF_GUI_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            TF_GUI_TRACE(fmt, ##args); \
            return Ret; \
        }

#define TF_GUI_CHECK_NULL_PTR(ptr)   TF_GUI_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")


/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define TF_GUI_PATH_DEF_RT "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"
#define TF_GUI_NODE_WAN_IF_NAME "X_TWSZ-COM_ConnectedDeviceName"

#define TF_GUI_PATH_PREFIX  "InternetGatewayDevice.X_TWSZ-COM_Device_list."
#define TF_GUI_PATH_ExternalIPAddress "ExternalIPAddress"
#define TF_GI_EXTERNEL_URL "ExternelUrl"
#define TF_GI_FORCE_PORTAL_URL "ForcePortalUrl"
#define TF_GI_FIRST_PERFORM "FirstPerform"
#define TF_GUI_AQUI_FLAG "AquiPerform"


//customer device nodes
#define TF_GUI_NODE_DEVICE                         "Customer_Device"
#define TF_GUI_NODE_DEVICE_ID                   "IdDevice"
#define TF_GUI_NODE_DEVICE_NAME              "NameDevice"
#define TF_GUI_NODE_SERIAL_NUMBER          "SerialNumber"
#define TF_GUI_NODE_DEVICE_ICON               "IdIcon"
#define TF_GUI_NODE_INTERFACE_TYPE         "Interfacetype"
#define TF_GUI_NODE_TYPE                     "Type"
#define TF_GUI_NODE_IP_ADDRESS                "IPAddress"
#define TF_GUI_NODE_MAC_ADDRESS             "MACAddress"
#define TF_GUI_NODE_CONNECT_STATUS       "Connected"
#define TF_GUI_NODE_BLACK_LIST                 "Blacklisted"
#define TF_GUI_NODE_AQUI_PERFORM             "AquiPerform"

//Application nodes
#define TF_GUI_NODE_APP_ENABLE	                 "Enable"
#define TF_GUI_NODE_APP                         "Applications"
#define TF_GUI_NODE_APP_ID                   "IdApplication"
#define TF_GUI_NODE_APP_NAME              "NameApplication"
#define TF_GUI_NODE_ASSIGNED_IP         "IpAssigned"
#define TF_GUI_NODE_APP_ICON               "IdIcon"
#define TF_GUI_NODE_ASSIGNED_MAC      "MacAssigned"

//rule nodes
#define TF_GUI_NODE_RULE                        "Rules"
#define TF_GUI_NODE_RULE_ID                   "IdRule"
#define TF_GUI_NODE_PROTOCOL                "Protocol"
#define TF_GUI_NODE_START_WAN             "StartWan"
#define TF_GUI_NODE_END_WAN                "EndWan"
#define TF_GUI_NODE_START_LAN             "StartLan"
#define TF_GUI_NODE_END_LAN                 "EndLan"

//node of other mode
#define TF_GUI_WAN_CONN_SERVICE_TYPE_NODE "X_TWSZ-COM_ServiceList"
#define TF_GUI_WAN_CONN_SERVICE_TYPE_VALUE "Internet"

#define TF_GUI_PROTOCOL_TCP "TCP"  //tcp
#define TF_GUI_PROTOCOL_UDP "UDP"  //udp
#define TF_GUI_PROTOCOL_ANY "ANY"   //tcp and udp

#define TF_GUI_NEED_REDIRECT_YES ""   //need redirect
#define TF_GUI_FORCE_PORTAL_ENABLE "echo on > /proc/pm_devurl"
#define TF_GUI_FORCE_PORTAL_DISABLE "echo off > /proc/pm_devurl"
#define TF_GUI_FORCE_PORTAL_SET_URL "echo \"PC_url=%s/html/gui/html/welcomeSplash/welcome_splash.html\" > /proc/pm_devurl"
#define TF_GUI_INTERNET_OK          "echo internet_ok > /proc/pm_devurl"
#define TF_GUI_INTERNET_NOT_OK "echo internet_not_ok > /proc/pm_devurl"
#define TF_GUI_FIRST_PERFORM "echo first_perfrom > /proc/pm_devurl"
#define TF_GUI_AQUI_FLAG_OK "echo aqui_flag_ok > /proc/pm_devurl"
#define TF_GUI_AQUI_FLAG_NOK "echo aqui_flag_nok > /proc/pm_devurl"
#define TF_GUI_NOT_FIRST_PERFORM "echo not_first_perfrom > /proc/pm_devurl"
#define TF_GUI_QUERY_CONFIGURATION "cat /proc/pm_devurl"
#define TF_GUI_FIRST_PERFORM_SPEC "first perform:"
#define TF_GUI_DNS_QUERY_REDIRECT_ADD "iptables -t nat -I PREROUTING 1 -p udp --dport 53 -j REDIRECT --to-port 53"
#define TF_GUI_DNS_QUERY_REDIRECT_DEL "iptables -t nat -D PREROUTING -p udp --dport 53 -j REDIRECT --to-port 53"

#define TF_GUI_DEV_CONNECTED "connected"
#define TF_GUI_DEV_DISCONNECTED "disconnected"
#define TF_GUI_DEV_PATH   "InternetGatewayDevice.X_TWSZ-COM_Device_list.Customer_Device."
#define TF_GUI_DEV_EXPATH   "InternetGatewayDevice.X_TWSZ-COM_Device_list.Customer_Device.%lu."


 
/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/
#define TF_GUI_DEVICE_REGEX_CU  "^InternetGatewayDevice\\.X_TWSZ-COM_Device_list\\."
#define TF_GUI_DEVICE_DEL_REGEX_CU  "^InternetGatewayDevice\\.X_TWSZ-COM_Device_list\\.Customer_Device\\.[0-9]+\\.$"
#define TF_GUI_DEVICE_REGEX_AP  "^InternetGatewayDevice\\.X_TWSZ-COM_Device_list\\.Applications\\.[0-9]+\\."
#define TF_GUI_DEVICE_DEL_REGEX_AP  "^InternetGatewayDevice\\.X_TWSZ-COM_Device_list\\.Applications\\.[0-9]+\\.$"
#define TF_GUI_DEVICE_ADD_REGEX_AP  "^InternetGatewayDevice\\.X_TWSZ-COM_Device_list\\.Applications\\.$"
//device 节点的匹配表达式
#define TF_GUI_DEVICE_NODE_REGEX(node) TF_GUI_DEVICE_REGEX_CU node "$"
//application 节点的匹配表达式
#define TF_GUI_APP_NODE_REGEX(node) TF_GUI_DEVICE_REGEX_AP node "$"
//rule 节点的匹配表达式
#define TF_GUI_RULE_REGEX            TF_GUI_DEVICE_REGEX_AP TF_GUI_NODE_RULE "\\.[0-9]+\\."
#define TF_GUI_RULE_DEL_REGEX            TF_GUI_DEVICE_REGEX_AP TF_GUI_NODE_RULE "\\.[0-9]+\\.$"
#define TF_GUI_ADD_RULE_REGEX            TF_GUI_DEVICE_REGEX_AP TF_GUI_NODE_RULE "\\.$"
#define TF_GUI_RULE_NODE_REGEX(node)  TF_GUI_RULE_REGEX node "$"


/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /*__TF_PORTMAPPING_H__*/

