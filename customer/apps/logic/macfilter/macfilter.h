/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: macfilter.h

 文件描述: macfilter模块头文件

 修订记录:

        1. 作者: WuGuoxiang

           日期: 2009-04-15

           内容: 创建文件

**********************************************************************/

#ifndef __MACFILTER_H__
#define __MACFILTER_H__

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef MACFILTER_DEBUG
    #define MACFILTER_TRACE(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define MACFILTER_LOG_ERR(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define MACFILTER_TRACE(fmt, args...)   do { ; } while(0)
    #define MACFILTER_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_MACFILTER, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define CONFIG_APPS_LOGIC_WLAN_MACFILTER 1
#define VAP_NUM 5

#define MACFILTER_TRACE_INTO_FUNC MACFILTER_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define MACFILTER_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            MACFILTER_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define MACFILTER_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_MACFILTER, Ret, fmt, ##args)

#define MACFILTER_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_MACFILTER, Pointer, gotoTag, fmt, ##args)

#define MACFILTER_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            MACFILTER_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define MACFILTER_CHECK_NULL_PTR(ptr)    MACFILTER_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define MACFILTER_SET_NODE_VAL(pszPath, pszVal) \
    { \
        iRet = CFG_SetNodeVal(pszPath, pszVal, NULL); \
        MACFILTER_RET_RETURN(iRet, "Set node \"%s\" value failed\n", pszPath); \
    }

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define MACFILTER_PATH_PREFIX   "InternetGatewayDevice.X_TWSZ-COM_MAC_Filter."
#define WHITELST_PATH_PREFIX    MACFILTER_PATH_PREFIX "WhiteList."
#define BLACKLST_PATH_PREFIX    MACFILTER_PATH_PREFIX "BlackList."

#define MACFILTER_NODE_WHITELST_ENTRYNO         "WhiteListEntryNumber"
#define MACFILTER_NODE_BLACKLST_ENTRYNO         "BlackListEntryNumber"
#define MACFILTER_NODE_ENABLE                   "Enable"
#define MACFILTER_NODE_FILTER_MODE              "FilterMode"
#define MACFILTER_NODE_MAC_ADDR                 "MACAddress"
#define MACFILTER_NODE_START_TIME               "StartTime"
#define MACFILTER_NODE_END_TIME                 "EndTime"
#define MACFILTER_NODE_DAYS                     "Days"

#define MACFILTER_NODE_DESCRIPTION     "Description"
#define MACFILTER_NODE_SCHEDULEMODE     "ScheduleMode"
#define MACFILTER_NODE_SCHEDULEPATH                     "SchedulePath"
#define MACFILTER_NODE_ENABLEALLDAYS                     "EnableAllDays"
#define MACFILTER_NODE_ENABLEALLTIMES                     "EnableAllTimes"


/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/
#define MACFILTER_PATH_PREFIX_REGEX \
            "^InternetGatewayDevice\\.X_TWSZ-COM_MAC_Filter\\."

#define MACFILTER_NODE_ENABLE_REGEX \
            MACFILTER_PATH_PREFIX_REGEX MACFILTER_NODE_ENABLE"$"

#define MACFILTER_NODE_FILTER_MODE_REGEX \
            MACFILTER_PATH_PREFIX_REGEX MACFILTER_NODE_FILTER_MODE"$"

#define WHITELST_PATH_PREFIX_REGEX \
            MACFILTER_PATH_PREFIX_REGEX "WhiteList\\.[0-9]+\\."
            
#define BLACKLST_PATH_PREFIX_REGEX \
            MACFILTER_PATH_PREFIX_REGEX "BlackList\\.[0-9]+\\."

#define MACFILTER_NODE_REGEX(node) \
            MACFILTER_PATH_PREFIX_REGEX \
            "(WhiteList|BlackList)\\.[0-9]+\\." node "$"

#define MACFILTER_TIME_CONFLICT_REGEX \
            MACFILTER_PATH_PREFIX_REGEX \
            "(WhiteList|BlackList)\\.[0-9]+\\.(" \
            MACFILTER_NODE_START_TIME "|" MACFILTER_NODE_START_TIME ")$"

#define MACFILTER_MAC_CONFLICT_REGEX \
            MACFILTER_PATH_PREFIX_REGEX \
            "(WhiteList|BlackList)\\.[0-9]+\\." \
            MACFILTER_NODE_MAC_ADDR "$"

#define ALLOWED_MACFILTER_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]\\.LANHostConfigManagement\\.AllowedMACAddresses$"
#define WLAN_MACADDRESS_ENABLED "^InternetGatewayDevice\\.LANDevice\\.[0-9]\\.WLANConfiguration\\.[0-9]\\.MACAddressControlEnabled$"

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define Check_PARAMETER_FAILED  "Check parameter \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"

#define WHITELST_INST_PATH_FMT  WHITELST_PATH_PREFIX "%lu."
#define BLACKLST_INST_PATH_FMT  BLACKLST_PATH_PREFIX "%lu."


/****************************************************************
 *                         过滤模式                             *
 ****************************************************************/
#if (CONFIG_APPS_HTML_WEB_STYLE_DSL2740EL == 1)
#define MACFILTER_DLINK 1
#endif
#ifdef MACFILTER_DLINK
#define FILTER_MODE_DISABLE         "Disable"
#endif
#define MACFILTER_NODE_EXAMPLE_ENABLE "Enable"
#define FILTER_MODE_ALLOW         "Allow"
#define FILTER_MODE_DENY          "Deny"

/****************************************************************
 *                         子链名称                             *
 ****************************************************************/
#ifdef MACFILTER_DLINK
#define MACFILTER_CHAIN        "MACFILTER"
#endif
#define WHITELST_CHAIN        "MACFILTER_WHITE"
#define BLACKLST_CHAIN        "MACFILTER_BLACK"

/****************************************************************
 *                         名单类型                             *
 ****************************************************************/
#define WHITE_LIST        1
#define BLACK_LIST        0

/****************************************************************
 *                         规则操作                             *
 ****************************************************************/
#define ADD_RULES        1
#define DEL_RULES        0
/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define MAX_MACFILTER_DAYS_LEN               32

#define MAX_MACFILTER_PATH_LEN               128

#define MAX_START_PARAM_NUM                  15

#define MAX_HOURS                            24
#define MAX_Minutes                          60

#define WEEK_DAYS                            7

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/ 
/******************************************************************************
*                                FUNCTION
******************************************************************************/

/* Init && Destroy */
int MACFILTER_ModuleRegister();
int MACFILTER_ModuleInit (void);
int MACFILTER_ModuleDestroy (void);

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /*__MACFILTER_H__*/
