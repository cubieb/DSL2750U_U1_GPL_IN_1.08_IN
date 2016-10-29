/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: wanselect.h
 文件描述: wanselect模块头文件
 修订记录:
        1. 作者: WuGuoxiang
           日期: 2009-08-21
           内容: 创建文件
**********************************************************************/

#ifndef __WANSELECT_H__
#define __WANSELECT_H__

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef WANSELECT_DEBUG
    #define WANSELECT_TRACE(fmt, args...) TBS_TRACE(MID_WANSELECT, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define WANSELECT_LOG_ERR(fmt, args...) TBS_TRACE(MID_WANSELECT, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define WANSELECT_TRACE(fmt, args...)   do { ; } while(0)
    #define WANSELECT_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_WANSELECT, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define WANSELECT_TRACE_INTO_FUNC WANSELECT_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define WANSELECT_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            WANSELECT_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define WANSELECT_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_WANSELECT, Ret, fmt, ##args)

#define WANSELECT_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_WANSELECT, Pointer, gotoTag, fmt, ##args)

#define WANSELECT_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            WANSELECT_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define WANSELECT_CHECK_NULL_PTR(ptr)    WANSELECT_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define WANSELECT_SET_NODE_VAL(pszPath, pszVal) \
    { \
        iRet = CFG_SetNodeVal(pszPath, pszVal, NULL); \
        WANSELECT_RET_RETURN(iRet, "Set node \"%s\" value failed\n", pszPath); \
    }

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define WANSELECT_PATH_PREFIX            "InternetGatewayDevice.Services."\
                                         "X_TWSZ-COM_WANSelect."

#define WANSELECT_NODE_DEFCFG          "DefaultCfg"


/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/
#define WANSELECT_PATH_PREFIX_REGEX \
        "^InternetGatewayDevice\\.Services\\.X_TWSZ-COM_WANSelect\\."
#define WANSELECT_NODE_REGEX(node) WANSELECT_PATH_PREFIX_REGEX node "$"


/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define Check_PARAMETER_FAILED  "Check parameter \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"


/*****************************************************************
 *                             Others                            *
 *****************************************************************/

#define MAX_WANSELECT_PATH_LEN          64

#define MAX_START_PARAM_NUM             20
#define MAX_WANSELECT_ITEMNAME_LEN      32
#define MAX_CONFIG_COUNT                4

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/ 

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /*__WANSELECT_H__*/
