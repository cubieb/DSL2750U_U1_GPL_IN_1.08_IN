/**
* @file tr064.h 
* 定义. 
* &copy; Copyright (c) 2010-2011 深圳市同维电子 DSL 事业部
* @since ZXONM N100（V2.0）
* @author 喻金石
* @date 2010年2月3号
* @note 2010年2月3号  喻金石  创建
*/

#ifndef __TBS_TR064C_H
#define __TBS_TR064C_H

#include <time.h>
#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"

#include "autoconf.h"


/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
/* 调试信息宏*/

#ifdef TR064C_DEBUG
#define TR064C_TRACE(fmt, args...)               COMMON_TRACE(MID_TR064C, fmt"\n", ##args)
#define TR064C_FUNC_IN(fmt, args...)             Tr064C_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define TR064C_FUNC_OUT()                        Tr064C_TRACE("--O %s\n", __FUNCTION__)


#else
#define TR064C_TRACE(str, args...)
#define TR064C_FUNC_IN(fmt, args...)
#define TR064C_FUNC_OUT()

#endif

#define TR064C_LOG_ERR(fmt, args...)                             COMMON_LOG_ERROR(MID_TR064C, fmt, ##args)

#define TR064C_RET_LOG(iRet, strError, args...)                  RET_LOG(MID_TR064C, iRet, strError, ##args)
#define TR064C_POINTER_LOG(pRet, strError, args...)              POINTER_LOG(MID_TR064C, pRet, strError, ##args)

#define TR064C_RET_RETURN(iRet, strError, args...)               RET_RETURN(MID_TR064C, iRet, strError, ##args)
#define TR064C_POINTER_RETURN(pRet, strError, args...)           POINTER_RETURN(MID_TR064C, pRet, strError, ##args)

#define TR064C_RET_GOTO(iRet, strError, gotoTag)                 RET_GOTO(MID_TR064C, iRet, strError, gotoTag)
#define TR064C_POINTER_GOTO(pRet, gotoTag, strError, args...)    POINTER_GOTO(MID_TR064C, pRet, gotoTag ,strError, ##args)

#define TR064C_RET_FALSE(iRet, strError, args...)                RET_FALSE(MID_TR064C, iRet, strError, ##args)
#define TR064C_POINTER_FALSE(pRet, strError, args...)            POINTER_FALSE(MID_TR064C, pRet, strError, ##args)

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"



/************************** 常量定义 ************************/
#define TR064C_NAME			    "/sbin/tb_tr064"
#define TR064C_MAX_ARGS  	        10                  /* snmpd最大参数个数 */

/* 正则 */
#define TR064C_REGEX_PREFIX			    "^InternetGatewayDevice\\.X_TWSZ-COM_TR064\\."

/* 节点路径定义 */
#define TR064C_PATH_PREFIX		        "InternetGatewayDevice.X_TWSZ-COM_TR064."

#define NODE_Enable				        "Enable"
#define NODE_Status				        "Status"

/* 节点全路径名 */
#define TR064C_PATH_ENABLE				TR064C_PATH_PREFIX NODE_Enable
#define TR064C_PATH_STATUS				TR064C_PATH_PREFIX NODE_Status


/* 节点枚举值定义 */
#define TR064C_STATUS_STOPPED	    0
#define TR064C_STATUS_STARTED	    1
#define TR064C_STATUS_STOPPING	    2
#define TR064C_STATUS_STARTING	    3

/* Agent进程启动失败最大重试次数 */
#define TR064C_MAX_START_FAIL	    3

/******************************************************************************
 *                                ENUM                                      *
 ******************************************************************************/



/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/

typedef struct tag_ST_TR064_Nodes
{
    int     bEnable;
    int     iStatus;
}ST_TR064_Nodes;

typedef struct tag_ST_TR064_Private
{
    int                     iStatus;            /* TR064进程状态:TR064_STATUS_xxxxx */
    unsigned long           ulStartFail;        /* TR064进程启动失败计数 */
    unsigned long           ulAbnormallyExit;   /* TR064进程异常退出次数 */
    unsigned char           ucMachineState;     /* TR064进程状态,PC记录 */
} ST_TR064_Private;                             /* TR064运行时信息 */


/* TR064运行信息 */
typedef struct tagST_TR064_RUNTIME_INFO
{
    ST_TR064_Nodes stNodes;
    ST_TR064_Private stPrivate;
} ST_TR064_RUNTIME_INFO;


/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init and Destroy */
int TR064_ModuleInit(void);
int TR064_ModuleDestroy(void);


#endif //__TBS_SNMPC_H

