/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: schedules.h

 文件描述: schedules模块头文件

 修订记录:

        1. 作者: 

           日期: 2008-12-23

           内容: 创建文件

**********************************************************************/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
//#define BRIDGE_FILTER_DEBUG

#ifdef BRIDGE_FILTER_DEBUG
    #define BRIDGE_FILTER_TRACE(fmt, args...) TBS_TRACE(MID_BRIDGE_FILTER, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define BRIDGE_FILTER_LOG_ERR(fmt, args...) TBS_TRACE(MID_BRIDGE_FILTER, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define BRIDGE_FILTER_TRACE(fmt, args...)   do { ; } while(0)
    #define BRIDGE_FILTER_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_BRIDGE_FILTER, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define BRIDGE_FILTER_TRACE_INTO_FUNC BRIDGE_FILTER_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define BRIDGE_FILTER_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            BRIDGE_FILTER_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define BRIDGE_FILTER_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_BRIDGE_FILTER, Ret, fmt, ##args)

#define BRIDGE_FILTER_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_BRIDGE_FILTER, Pointer, gotoTag, fmt, ##args)

#define BRIDGE_FILTER_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            BRIDGE_FILTER_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define BRIDGE_FILTER_CHECK_NULL_PTR(ptr)    BRIDGE_FILTER_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define BRIDGE_FILTER_SET_NODE_VAL(pszPath, pszVal) \
    { \
        iRet = CFG_SetNodeVal(pszPath, pszVal, NULL); \
        BRIDGE_FILTER_RET_RETURN(iRet, "Set node \"%s\" value failed\n", pszPath); \
    }


/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
 #define BRIDGE_PATH_PREFIX2		        "InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeFilterList."
#define ETHERNET_ENABLE 	"InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig."
#define BRIDGE_PATH_PREFIX     "InternetGatewayDevice.X_TWSZ-COM_BridgeFilter."
#define BRIDGE_PATH_BRIDGE         BRIDGE_PATH_PREFIX "BridgeFilterList."
#define BRIDGE_WANPPPCONNECTION                  "WANPPPConnection"
#define BRIDGE_WANIPCONNECTION                    "WANIPConnection"
#define NODE_WANConnetPathex		                "BridgeRuleWan"

#define BRIDGE_NODE_ENABLE     "Enable"
#define BRIDGE_NODE_MAX      "MaxBridgeFilterEntries"
#define BRIDGE_NODE_NUMOFBRIDGE    "BridgeFilterNumberOfEntries"
#define BRIDGE_NODE_RULE  "BridgeRule"


#define BRIDGE_NODE_PROTOCOL      "ProtocolType"
#define BRIDGE_NODE_DSTMAC     "DstMac"
#define BRIDGE_NODE_SRCMAC   "SrcMac"
#define BRIDGE_NODE_LANDIR   "LanDirection"
#define BRIDGE_NODE_MODE   "BridgeRule"
#define BRIDGE_NODE_WAN   "BridgeRuleWan"
#define BRIDGE_NODE_TIME  "TimeSchedule"
#define BRIDGE_NODE_USRPRIORITY  "UsrPriority"
#define BRIDGE_NODE_VLANID  "VlanId"

/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/
#define BRIDGE_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.X_TWSZ-COM_BridgeFilter\\."
#define BRIDGE_PATH_BRIDGE_REGEX     BRIDGE_PATH_PREFIX_REGEX "BridgeFilterList\\.[0-9]+\\."
#define END "$"

#define BRIDGE_BRIDGE_NODE_REGEX(node)  BRIDGE_PATH_BRIDGE_REGEX node END
#define BRIDGE_NODE_REGEX(node) BRIDGE_PATH_PREFIX_REGEX node END

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define MAX_MACFILTER_PATH_LEN 128
#define BRIDGECLONE_MAX_PATH_LEN   256

	
#define DEL_RULES 0
#define ADD_RULES 1

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /* __SCHED_H__ */

