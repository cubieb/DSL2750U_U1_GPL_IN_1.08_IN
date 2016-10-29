/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: schedules.h

 文件描述: schedules模块头文件

 修订记录:

        1. 作者: 

           日期: 2009-7-10

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
#ifdef SCHED_DEBUG
#define SCHED_TRACE(fmt, args...) COMMON_TRACE(MID_SCHED, ECHO_GREEN fmt ECHO_NONE, ##args)
#define SCHED_LOG_ERR(fmt, args...) COMMON_TRACE(MID_SCHED, ECHO_RED fmt ECHO_NONE, ##args)
#define SCHED_TRACE_INTO_FUNC(fmt, args...)  SCHED_TRACE("##In## %s\n", __func__)
#define SCHED_TRACE_OUT_FUNC(fmt, args...)  SCHED_TRACE("##Out## %s\n", __func__)
#define APCMD_PRINT 1 /* 调用tbsSystem时打印命令 */
#else
    #define SCHED_TRACE(fmt, args...)
    #define SCHED_LOG_ERR(fmt, args...)  COMMON_LOG_ERROR(MID_SCHED, fmt, ##args)
    #define SCHED_TRACE_INTO_FUNC(fmt, args...)
    #define SCHED_TRACE_OUT_FUNC(fmt, args...) 
    #define APCMD_PRINT 0 
#endif

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define SCHED_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            SCHED_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define SCHED_RET_RETURN(Ret, fmt, args...)  \
        if ( RET_FAILED(Ret) ) \
        { \
            SCHED_TRACE(fmt, ##args); \
            return Ret; \
        }

#define SCHED_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        if (!Pointer)    \
        { \
            SCHED_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define SCHED_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            SCHED_TRACE(fmt, ##args); \
            return Ret; \
        }

#define SCHED_CHECK_NULL_PTR(ptr)   SCHED_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define SET_NODE_VAL(szPath, szValue)  \
        if (CFG_RET_FAILED(CFG_SetNodeVal(szPath, szValue, NULL))) \
        {\
            SCHED_TRACE("Set cfg value fail: %s=%s\n", szPath, szValue);  \
            return TBS_FAILED;  \
        }

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define SCHED_PATH_PREFIX     "InternetGatewayDevice.X_TWSZ-COM_SCHEDULES."
#define SCHED_PATH_SCHED         SCHED_PATH_PREFIX "SchedEntry."

#define SCHED_NODE_ENABLE     "Enable"
#define SCHED_NODE_MAX      "MaxEntries"
#define SCHED_NODE_NUMOFSCHED    "SchedulesNumberOfEntries"

#define SCHED_NODE_NAME      "SchedName"
#define SCHED_NODE_USED     "SchedUsed"
#define SCHED_NODE_ENABLEALLDAYS   "EnableAllDays"
#define SCHED_NODE_DAYS   "SelectDays"
#define SCHED_NODE_ENABLEALLTIMES   "EnableAllTimes"
#define SCHED_NODE_STARTTIME   "StartTime"
#define SCHED_NODE_ENDTIME   "EndTime"

#define TIME_PATH_PREFIX    "InternetGatewayDevice.Time."
#define TIME_NODE_ENABLE    "Enable"

/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/
#define SCHED_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.X_TWSZ-COM_SCHEDULES\\."
#define SCHED_PATH_SCHED_REGEX     SCHED_PATH_PREFIX_REGEX "SchedEntry\\.[0-9]+\\."
#define END "$"
#define SCHED_ADD_INST_REGEX SCHED_PATH_PREFIX_REGEX "SchedEntry\\." END

#define SCHED_SCHED_NODE_REGEX(node)  SCHED_PATH_SCHED_REGEX node END

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define MAX_INDEX_COUNT     1
#define SCHED_MAX_IFNAME_LEN  16
#define SCHED_SUBCMD_LEN_1    16
#define SCHED_SUBCMD_LEN_2    32
#define SCHED_SUBCMD_LEN_3    64

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
/* 实例类型定义 */
enum {
    QOS_IF = 1,
    QOS_QUEUE,
    QOS_CLASS
};

/* 生效操作方式 */
typedef enum {
    QOS_APPLY_NON,
    QOS_APPLY_ADD,
    QOS_APPLY_DEL
} EN_QOS_APPLY;

/*分类配置方式*/
enum {
    QOS_CLS_QUEUE = 1,   //分类匹配后绑定队列
    QOS_CLS_MARKING      //分类匹配后进行标记
};

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /* __SCHED_H__ */

