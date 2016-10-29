/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: traditionalnat.h

 文件描述: 

 修订记录:

        1. 作者: 

           日期: 

           内容: 

**********************************************************************/

#ifndef __TDNATFILTER_H__
#define __TDNATFILTER_H__

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/

#ifdef TDNAT_DEBUG
    #define TDNAT_TRACE(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define TDNAT_LOG_ERR(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define TDNAT_TRACE(fmt, args...)   do { ; } while(0)
    #define TDNAT_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_MACFILTER, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define TDNAT_TRACE_INTO_FUNC TDNAT_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define TDNAT_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            TDNAT_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define TDNAT_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_MACFILTER, Ret, fmt, ##args)

#define TDNAT_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_MACFILTER, Pointer, gotoTag, fmt, ##args)

#define TDNAT_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            TDNAT_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define TDNAT_CHECK_NULL_PTR(ptr)    TDNAT_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define TDNAT_SET_NODE_VAL(pszPath, pszVal) \
    { \
        iRet = CFG_SetNodeVal(pszPath, pszVal, NULL); \
        TDNAT_RET_RETURN(iRet, "Set node \"%s\" value failed\n", pszPath); \
    }

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define TDNAT_PATH_PREFIX   "InternetGatewayDevice.X_TWSZ-COM_TraditionalNAT."

#define TDNAT_ENABLE                   "Enable"
#define TDNAT_NODE_ENABLE                   "X_TWSZ-COM_Enable"
#define TDNAT_NODE_ENTRYNAME              "X_TWSZ-COM_EntryName"
#define TDNAT_NODE_INTERNALIP_TYPE                 "X_TWSZ-COM_InternalIPType"
#define TDNAT_NODE_STA_INIP               "X_TWSZ-COM_StartInternalIP"
#define TDNAT_NODE_END_INIP                 "X_TWSZ-COM_EndInternalIP"

#define TDNAT_NODE_EXTERNAL_TYPE                     "X_TWSZ-COM_ExternalIPType"
#define TDNAT_NODE_STA_EXIP                     "X_TWSZ-COM_StartExternalIP"
#define TDNAT_NODE_END_EXIP                     "X_TWSZ-COM_EndExternalIP"


/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/

#define TDNAT_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.X_TWSZ-COM_TraditionalNAT\\."
#define TDNAT_PATH_ENTRY_REGEX     TDNAT_PATH_PREFIX_REGEX "TraditionalNATEntry\\.[0-9]+\\."


#define TDNAT_NODE_ENABLE_REGEX \
            TDNAT_PATH_PREFIX_REGEX TDNAT_ENABLE"$"

#define TDNAT_NODE_REGEX(node) TDNAT_PATH_ENTRY_REGEX node "$"

#define TDNAT_INTERNALIP_CONFLICT_REGEX TDNAT_PATH_ENTRY_REGEX TDNAT_NODE_STA_INIP "|" TDNAT_NODE_END_INIP ")$"

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define Check_PARAMETER_FAILED  "Check parameter \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"


/****************************************************************
 *                         规则操作                             *
 ****************************************************************/
#define ADD_RULES        1
#define DEL_RULES        0
/*****************************************************************
 *                             Others                            *
 *****************************************************************/


/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/ 

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /*__TDNAT_H__*/
