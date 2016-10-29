/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: porttrigger.h

 文件描述: 端口触发适配模块的头文件

 修订记录:

           1 作者 : 匡素文
             日期 : 2009-5-21
             描述 : 创建

**********************************************************************/
#ifndef __TBS_PORTTRIGGER_H
#define __TBS_PORTTRIGGER_H

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/
/* 调试信息宏*/
//#define PT_DEBUG    1

#ifdef PT_DEBUG
#define PT_TRACE(fmt, args...)              COMMON_TRACE(MID_PORTTRIGGER, fmt"\n", ##args)
#else
#define PT_TRACE(str, args...)
#endif

#define PT_FUNC_IN(fmt, args...)                            PT_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define PT_FUNC_OUT()                                       PT_TRACE("--O %s\n", __FUNCTION__)

#define PT_LOG_ERR(fmt, args...)                            COMMON_LOG_ERROR(MID_PORTTRIGGER, fmt, ##args)

#define PT_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_PORTTRIGGER, iRet, strError, ##args)
#define PT_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_PORTTRIGGER, pRet, strError, ##args)

#define PT_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_PORTTRIGGER, iRet, strError, ##args)
#define PT_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_PORTTRIGGER, pRet, strError, ##args)

#define PT_RET_GOTO(iRet, strError, gotoTag)                RET_GOTO(MID_PORTTRIGGER, iRet, strError, gotoTag)
#define PT_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_PORTTRIGGER, pRet, gotoTag ,strError, ##args)

#define PT_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_PORTTRIGGER, iRet, strError, ##args)
#define PT_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_PORTTRIGGER, pRet, strError, ##args)

#define ERR_GET(node)                       "Get Node["node"] value failed.\n"
#define ERR_SET(node)                       "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)                   "Set Node["node"] value failed, value = %s\n"


/************************** 常量定义 **************************/

/* 添加规则/删除规则 */
#define PT_CMD_ADD                          1
#define PT_CMD_DEL                          0

/* 自定义链名称 */
#define PT_PREROUTING_CHAIN                  "PT_PREROUTING"
#define PT_FORWARD_CHAIN                     "PT_FORWARD"

/* 节点名定义 */
#define NODE_Enable                         "Enable"
#define NODE_TriggerList                    "TriggerList"

#define NODE_Name                           "Name"
#define NODE_TriggerProtocol                "TriggerProtocol"
#define NODE_TriggerStartPort               "TriggerStartPort"
#define NODE_TriggerEndPort                 "TriggerEndPort"

#define NODE_OpenProtocol                   "OpenProtocol"
#define NODE_OpenStartPort                  "OpenStartPort"
#define NODE_OpenEndPort                    "OpenEndPort"

/* LAN NODE */
#define NODE_IPInterfaceIPAddress           "IPInterfaceIPAddress"
#define NODE_IPInterfaceSubnetMask          "IPInterfaceSubnetMask"
#define NODE_MinAddress                     "MinAddress"
#define NODE_MaxAddress                     "MaxAddress"

#define NODE_IPInterface                    "IPInterface"
#define NODE_ClientClasses                  "X_TWSZ-COM_ClientClasses"

/* 节点路径定义 */
#define PT_PATH_PREFIX                      "InternetGatewayDevice.X_TWSZ-COM_PortTrigger."
#define PT_PATH_LIST_PREFIX                 PT_PATH_PREFIX NODE_TriggerList "."
#define PT_PATH_LAN_CONN                    "InternetGatewayDevice.LANDevice.1."
#define PT_PATH_LAN_HOST                    "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement."

#define PT_FORMAT_INST                      PT_PATH_LIST_PREFIX "%u."

/* 正则定义 */
#define PT_REGEX_PREFIX                     "^InternetGatewayDevice\\.X_TWSZ-COM_PortTrigger\\."
#define PT_REGEX_LIST_PREFIX                PT_REGEX_PREFIX NODE_TriggerList "\\.[0-9]{1,5}\\."
#define PT_REGEX_LAN_CONN                   "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}$"

#define FOR_EACH_LIST(pos, list)            for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)

/******************************************************************************
*                                ENUM
******************************************************************************/



/******************************************************************************
*                                STRUCT
******************************************************************************/
/* 索引结构 */
typedef struct tag_ST_PT_Index
{
    unsigned int nRuleIdx;
} ST_PT_Index;

/* 端口触发规则对应的节点信息 */
typedef struct tag_ST_PT_Nodes
{
    char *szEnable;
    char *szName;
    char *szTriggerProtocol;
    char *szTriggerStartPort;
    char *szTriggerEndPort;
    char *szOpenProtocol;
    char *szOpenStartPort;
    char *szOpenEndPort;
} ST_PT_Nodes;

/* 端口触发规则实例 */
typedef struct tag_ST_PT_Instance
{
    char acPath[MAX_PATH_LEN];
    ST_PT_Index stIndex;
    ST_PT_Nodes stNodes;
} ST_PT_Instance;


/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init && Destroy */
int PT_ModuleInit();
int PT_ModuleDestroy();


#endif//__TBS_PORTTRIGGER_H
