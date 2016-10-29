/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: urlfilter.h

 文件描述: urlfilter模块头文件

 修订记录:

        1. 作者: WuGuoxiang

           日期: 2009-04-21

           内容: 创建文件

**********************************************************************/

#ifndef __URLFILTER_H__
#define __URLFILTER_H__

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef URLFILTER_DEBUG
    #define URLFILTER_TRACE(fmt, args...) TBS_TRACE(MID_URLFILTER, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define URLFILTER_LOG_ERR(fmt, args...) TBS_TRACE(MID_URLFILTER, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define URLFILTER_TRACE(fmt, args...)   do { ; } while(0)
    #define URLFILTER_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_URLFILTER, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define URLFILTER_TRACE_INTO_FUNC URLFILTER_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define URLFILTER_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            URLFILTER_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define URLFILTER_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_URLFILTER, Ret, fmt, ##args)

#define URLFILTER_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_URLFILTER, Pointer, gotoTag, fmt, ##args)

#define URLFILTER_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            URLFILTER_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define URLFILTER_CHECK_NULL_PTR(ptr)    URLFILTER_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define URLFILTER_SET_NODE_VAL(pszPath, pszVal) \
    { \
        iRet = CFG_SetNodeVal(pszPath, pszVal, NULL); \
        URLFILTER_RET_RETURN(iRet, "Set node \"%s\" value failed\n", pszPath); \
    }

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define URLFILTER_PATH_PREFIX   "InternetGatewayDevice.X_TWSZ-COM_URL_Filter."
#define WHITELST_PATH_PREFIX    URLFILTER_PATH_PREFIX "WhiteList."
#define BLACKLST_PATH_PREFIX    URLFILTER_PATH_PREFIX "BlackList."

#define URLFILTER_NODE_WHITELST_ENTRYNO         "WhiteListEntryNumber"
#define URLFILTER_NODE_BLACKLST_ENTRYNO         "BlackListEntryNumber"
#define URLFILTER_NODE_ENABLE                   "Enable"
#define URLFILTER_NODE_FILTER_MODE              "FilterMode"
#define URLFILTER_NODE_URL                      "URL"
#define URLFILTER_NODE_START_TIME               "StartTime"
#define URLFILTER_NODE_END_TIME                 "EndTime"
#define URLFILTER_NODE_DAYS                     "Days"

#define URLFILTER_NODE_SCHEDULEMODE     "ScheduleMode"
#define URLFILTER_NODE_SCHEDULEPATH                     "SchedulePath"
#define URLFILTER_NODE_ENABLEALLDAYS                     "EnableAllDays"
#define URLFILTER_NODE_ENABLEALLTIMES                     "EnableAllTimes"


/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/
#define URLFILTER_PATH_PREFIX_REGEX \
            "^InternetGatewayDevice\\.X_TWSZ-COM_URL_Filter\\."

#define URLFILTER_NODE_ENABLE_REGEX \
            URLFILTER_PATH_PREFIX_REGEX URLFILTER_NODE_ENABLE"$"

#define URLFILTER_NODE_FILTER_MODE_REGEX \
            URLFILTER_PATH_PREFIX_REGEX URLFILTER_NODE_FILTER_MODE"$"

#define WHITELST_PATH_PREFIX_REGEX \
            URLFILTER_PATH_PREFIX_REGEX "WhiteList\\.[0-9]+\\."
            
#define BLACKLST_PATH_PREFIX_REGEX \
            URLFILTER_PATH_PREFIX_REGEX "BlackList\\.[0-9]+\\."

#define URLFILTER_NODE_REGEX(node) \
            URLFILTER_PATH_PREFIX_REGEX \
            "(WhiteList|BlackList)\\.[0-9]+\\." node "$"

#define URLFILTER_TIME_CONFLICT_REGEX \
            URLFILTER_PATH_PREFIX_REGEX \
            "(WhiteList|BlackList)\\.[0-9]+\\.(" \
            URLFILTER_NODE_START_TIME "|" URLFILTER_NODE_END_TIME ")$"

#define URLFILTER_URL_CONFLICT_REGEX \
            URLFILTER_PATH_PREFIX_REGEX \
            "(WhiteList|BlackList)\\.[0-9]+\\." \
            URLFILTER_NODE_URL "$"

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
#define FILTER_MODE_ALLOW         "Allow"
#define FILTER_MODE_DENY          "Deny"

/****************************************************************
 *                         子链名称                             *
 ****************************************************************/
#define WHITELST_CHAIN        "URLFILTER_WHITE"
#define BLACKLST_CHAIN        "URLFILTER_BLACK"

/****************************************************************
 *                         名单类型                             *
 ****************************************************************/
#define WHITE_LIST        1
#define BLACK_LIST        0

/****************************************************************
 *                         规则操作                             *
 ****************************************************************/
#define URLFILTER_ADD_RULES        1
#define URLFILTER_DEL_RULES        0
/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define MAX_URLFILTER_DAYS_LEN               32     /* Days 最多允许的字符数 */
#define MAX_URLFILTER_URL_LEN                256    /* URL 最多允许的字符数 */

#define MAX_URLFILTER_PATH_LEN               128

#define MAX_START_PARAM_NUM                  15

#define MAX_HOURS                            24
#define MAX_Minutes                          60

#define WEEK_DAYS                            7

#define CHAIN_CONDITION_RULE   "-m string --algo bm --string \"HTTP\""

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/ 

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /*__URLFILTER_H__*/
