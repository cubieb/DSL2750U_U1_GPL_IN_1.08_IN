/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: macclone.h

 文件描述: macclone模块头文件

 修订记录:
 
 
 
**********************************************************************/
#ifndef _TBS_MACCLONE_H
#define _TBS_MACCLONE_H

#include "common.h"
#include "warnlog.h"
#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "pc_api.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

/* 调试信息宏*/
#ifdef MACCLONE_DEBUG
#define MACCLONE_TRACE(fmt, args...)         COMMON_TRACE(MID_MACCLONE,ECHO_GREEN fmt ECHO_NONE , ##args)
#define MACCLONE_LOG_ERR(fmt, args...) TBS_TRACE(MID_MACCLONE, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
#else
#define MACCLONE_TRACE(fmt, args...)     do { ; } while(0)
#define MACCLONE_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_MACCLONE, fmt, ##args)
/* 模块关闭调试时关掉内存调试 */
#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free
#endif
#ifdef calloc
#undef calloc
#endif
#ifdef realloc
#undef realloc
#endif

#endif

#define MACCLONE_ERR(err, arg...)  (void)0
/* 对函数返回值错误处理的简化定义   */

#define MACCLONE_TRACE_INTO_FUNC MACCLONE_TRACE("##In## %s\n", __func__)
#define MACCLONE_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            MACCLONE_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define MACCLONE_CHECK_NULL_PTR(ptr)    MACCLONE_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")
#define MACCLONE_RET_RETURN(Ret, fmt, args...)   RET_RETURN(MID_MACCLONE, Ret, fmt, ##args)
#define MACCLONE_POINTER_FALSE(pRet, strError, args...)            POINTER_FALSE(MID_MACCLONE, pRet, strError, ##args)
#define MACCLONE_RET_GOTO(iRet, strError, gotoTag)                 RET_GOTO(MID_MACCLONE, iRet, strError, gotoTag)

#define MACCLONE_COMM_ERR_PROC(ret, arg...) \
if (TBS_SUCCESS != ret) \
{ \
    MACCLONE_ERR(ret, ##arg); \
    MACCLONE_TRACE("%d %s ERROR\n", __LINE__, __FUNCTION__); \
    return ret; \
}

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"


/* 节点路径匹配正则表达式     */
#define MACCLONE_REGEX_PREFIX			    "^InternetGatewayDevice\\.X_TWSZ-COM_MacClone\\."
#define MACCLONE_NODE_REGEX(node)     MACCLONE_REGEX_PREFIX "MaccloneObject\\.[0-9]+\\." node "$"
#define MACCLONE_NODE_REGEX2(node)     MACCLONE_REGEX_PREFIX  node "$"


/* 节点路径定义     */
                                                                     
#define MACCLONE_PATH_PREFIX2		        "InternetGatewayDevice.X_TWSZ-COM_MacClone.MaccloneObject."
#define MACCLONE_PATH_PREFIX		        "InternetGatewayDevice.X_TWSZ-COM_MacClone."
#define NODE_MACCloneEnabled		         "MACCloneEnabled"
#define NODE_WANConnetPath		                "WANConnetPath"
#define NODE_MacAd		                              "MacAd"                                                                     

#define MACCLONE_PPPOE_DEVICE                            "X_TWSZ-COM_ConnectedDeviceName"
#define MACCLONE_PPPOE_LAN_DEVICE                            "X_TWSZ-COM_DeviceName"
#define MACCLONE_WANPPPCONNECTION                  "WANPPPConnection"
#define MACCLONE_WANIPCONNECTION                    "WANIPConnection"
/* 节点全路径名       */
#define MACCLONE_PATH_MACCLONE_ENABLE	 MACCLONE_PATH_PREFIX NODE_MACCloneEnabled

/*打印模式字符串  */
#define Check_PARAMETER_FAILED  "Check parameter \"%s\" failed\n"
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

#define MACCLONE_MAX_PATH_LEN   256
#define MAX_INDEX_COUNT 4
#define LAN_MAX_CMD_LEN 100                                 /* 命令最大长度*/
/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/



/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/



/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/
/* Init && Destroy */
int MACCLONE_ModuleInit();
int MACCLONE_ModuleDestroy();


#endif //_TBS_MACCLONE_H
