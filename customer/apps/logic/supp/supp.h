/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: supp.h
 文件描述: supp模块头文件

 修订记录:
        1. 作者: LiuLisi
           日期: 2009-3-25
           内容: 创建文件
**********************************************************************/

#ifndef __SUPP_H__
#define __SUPP_H__

#include "warnlog.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
#define SUPP_MOD_ID          MID_SUPP
#define SUPP_PATH_LENGTH     256
#define MAX_PROC_NAME_LEN       16
#define MAX_WAN_CONNECTONS_DEF 8

#define SUPP_WANDEV          "InternetGatewayDevice.WANDevice"
#define SUPP_WANCONNDEV      "WANConnectionDevice"
#define SUPP_WANPPPCONN      "WANPPPConnection"
#define SUPP_WANIPCONN       "WANIPConnection"
/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef SUPP_DEBUG
    #define SUPP_TRACE(fmt, args...) COMMON_TRACE(MID_SUPP, fmt, ##args)
    #define SUPP_LOG_ERR(fmt, args...) COMMON_TRACE(fmt, ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
    #define SUPP_FUNC_IN(fmt, args...) SUPP_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
    #define SUPP_FUNC_OUT()      SUPP_TRACE("--O %s\n", __FUNCTION__)
#else
    #define SUPP_TRACE(fmt, args...)   do { ; } while(0)
    #define SUPP_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_SUPP, fmt, ##args)
    #define CMD_PRINT 0
    #define SUPP_FUNC_IN(fmt, args...)
    #define SUPP_FUNC_OUT()
#endif

#define SUPP_TRACE_INTO_FUNC SUPP_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define SUPP_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            SUPP_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define SUPP_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            SUPP_TRACE(strError); \
            goto gotoTag; \
        }\
      }

#define SUPP_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_SUPP, Pointer, gotoTag, fmt, ##args)

#define SUPP_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            SUPP_TRACE(fmt, ##args); \
            return Ret; \
        }

#define SUPP_CHECK_NULL_PTR(ptr)   SUPP_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

#define SUPP_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_SUPP, iRet, strError, ##args)
#define SUPP_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_SUPP, pRet, strError, ##args)

#define SUPP_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_SUPP, iRet, strError, ##args)
#define SUPP_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_SUPP, pRet, strError, ##args)

#define SUPP_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_SUPP, pRet, gotoTag ,strError, ##args)

#define SUPP_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_SUPP, iRet, strError, ##args)
#define SUPP_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_SUPP, pRet, strError, ##args)

#define ERR_GET(node)               "Get Node["node"] value failed.\n"
#define ERR_SET(node)               "Set Node["node"] value failed.\n"

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define SUPP_SET_NODE_VAL(pszPath, pszVal) \
    { \
        iRet = CFG_SetNodeVal(pszPath, pszVal, NULL); \
        SUPP_RET_RETURN(iRet, "Set node \"%s\" value failed\n", pszPath); \
    }

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define REG_NODE_PATH_IPCONN_SUPP    "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define REG_NODE_PATH_PPPOE_SUPP     "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."


/*****************************************************************
 *                     维护的节点                                *
 *****************************************************************/

#define NODE_SUPP_DeviceName             "X_TWSZ-COM_DeviceName"
#define NODE_SUPP_ConnectionStatus       "ConnectionStatus"
#define NODE_SUPP_ConnectedDeviceName    "X_TWSZ-COM_ConnectedDeviceName"
#define NODE_SUPP_PPP_NAME    "InterfaceName"
#define NODE_SUPP_WAN_ENABLE "Enable"


#define NODE_SUPP_ENABLE  		"SuppEnable"
#define NODE_SUPP_USERNAME		"SuppUsername"
#define NODE_SUPP_PASS			"SuppPass"

#define SUPP_PATH_ATTACH         "[0-9]+\\."
#define SUPP_BASE_PATH_PPP       "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\."
#define SUPP_BASE_PATH_IPCONN    "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANIPConnection\\."

/* ADD消息匹配 */
#define SUPP_ADD_PATH_PPP        SUPP_BASE_PATH_PPP"$"
#define SUPP_ADD_PATH_IPCONN     SUPP_BASE_PATH_IPCONN"$"
/* DEL消息匹配 */
#define SUPP_DEL_PATH_PPP        SUPP_BASE_PATH_PPP  SUPP_PATH_ATTACH"$"
#define SUPP_DEL_PATH_IPCONN     SUPP_BASE_PATH_IPCONN   SUPP_PATH_ATTACH"$"
/* SET消息匹配 */
#define SUPP_SET_PPP_SUPP_ENABLE       SUPP_BASE_PATH_PPP  SUPP_PATH_ATTACH  NODE_SUPP_ENABLE"$"
#define SUPP_SET_PPP_SUPP_USERNAME       SUPP_BASE_PATH_PPP  SUPP_PATH_ATTACH  NODE_SUPP_USERNAME"$"
#define SUPP_SET_PPP_SUPP_PASS       SUPP_BASE_PATH_PPP  SUPP_PATH_ATTACH  NODE_SUPP_PASS"$"
#define SUPP_SET_IPCONN_SUPP_ENABLE    SUPP_BASE_PATH_IPCONN  SUPP_PATH_ATTACH   NODE_SUPP_ENABLE"$"
#define SUPP_SET_IPCONN_SUPP_USERNAME     SUPP_BASE_PATH_IPCONN  SUPP_PATH_ATTACH   NODE_SUPP_USERNAME"$"
#define SUPP_SET_IPCONN_SUPP_PASS    SUPP_BASE_PATH_IPCONN  SUPP_PATH_ATTACH   NODE_SUPP_PASS"$"
/* COMMIT消息匹配 */
#define SUPP_COMMIT_PPP_SUPP_ENABLE    SUPP_SET_PPP_SUPP_ENABLE
#define SUPP_COMMIT_PPP_SUPP_USERNAME    SUPP_SET_PPP_SUPP_USERNAME
#define SUPP_COMMIT_PPP_SUPP_PASS    SUPP_SET_PPP_SUPP_PASS
#define SUPP_COMMIT_IPCONN_SUPP_ENABLE   SUPP_SET_IPCONN_SUPP_ENABLE
#define SUPP_COMMIT_IPCONN_SUPP_USERNAME  SUPP_SET_IPCONN_SUPP_USERNAME
#define SUPP_COMMIT_IPCONN_SUPP_PASS  SUPP_SET_IPCONN_SUPP_PASS

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define SUPP_IPCONN_INST_PREFIX      "InternetGatewayDevice.WANDevice.%u.WANConnectionDevice.%u.WANIPConnection.%u."
#define SUPP_PPP_INST_PREFIX      "InternetGatewayDevice.WANDevice.%u.WANConnectionDevice.%u.WANPPPConnection.%u."

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
/* SUPP的节点 */
typedef struct tagST_SUPP_Nodes
{
    int  iSuppEnable;
    char *pcSuppUserName;
    char *pcSuppPasswd;
}ST_SUPP_Nodes;


#define SUPP_WAN_IPCONN "ipcon"
#define SUPP_WAN_PPP "pppoe"

typedef struct tagST_SUPP_Index
{
    unsigned int nWanConnType;   //ipconn: 0; ppp: 1
    unsigned int nWanDevIdx;
    unsigned int nWanConnDevIdx;
    unsigned int nWanConnIdx;
}ST_SUPP_Index;


typedef struct tagST_SUPP_Private
{
    char szWanConnDevName[20];
}ST_SUPP_Private;

/* SUPP实例*/
typedef struct tagST_SUPP_Instance
{
    char acPath[MAX_PATH_LEN];

    ST_SUPP_Nodes stNodes;
    ST_SUPP_Index stIndex;
    ST_SUPP_Private stPrivate;
}ST_SUPP_Instance;


/******************************************************************************
 *                                 FUNCTION                                   *
 ******************************************************************************/
int SUPP_WanEstHandle(ST_MSG *pstMsg);
int SUPP_ModuleInit();
int SUPP_ModuleDestroy();

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /*__SUPP_H__*/
