/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: acl.h

 文件描述: ACL模块头文件

 修订记录:
           1 作者 : 詹剑
             日期 : 2008-08-12
             描述 : 创建

           2 作者 : 匡素文
             日期 : 2008-12-04
             描述 : 重构

**********************************************************************/
#ifndef __TBS_ACL_H__
#define __TBS_ACL_H__

#include "common.h"
#include <arpa/inet.h>

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "autoconf.h"



/******************************************************************************
*                                 MACRO
******************************************************************************/
/* 调试信息宏*/
#ifdef ACL_DEBUG
#define ACL_TRACE(fmt, args...)                 COMMON_TRACE(MID_ACL, fmt"\n", ##args)
#define ACL_FUNC_IN(fmt, args...)               ACL_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define ACL_FUNC_OUT()                          ACL_TRACE("--O %s\n", __FUNCTION__)

#else
#define ACL_TRACE(str, args...)
#define ACL_FUNC_IN(fmt, args...)
#define ACL_FUNC_OUT()

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

#endif //#ifdef ACL_DEBUG

#define ACL_LOG_ERR(fmt, args...)                           COMMON_LOG_ERROR(MID_ACL, fmt, ##args)

#define ACL_RET_LOG(iRet, strError, args...)                RET_LOG(MID_ACL, iRet, strError, ##args)
#define ACL_POINTER_LOG(pRet, strError, args...)            POINTER_LOG(MID_ACL, pRet, strError, ##args)

#define ACL_RET_RETURN(iRet, strError, args...)             RET_RETURN(MID_ACL, iRet, strError, ##args)
#define ACL_POINTER_RETURN(pRet, strError, args...)         POINTER_RETURN(MID_ACL, pRet, strError, ##args)

#define ACL_RET_GOTO(iRet, strError, gotoTag)               RET_GOTO(MID_ACL, iRet, strError, gotoTag)
#define ACL_POINTER_GOTO(pRet, gotoTag, strError, args...)  POINTER_GOTO(MID_ACL, pRet, gotoTag ,strError, ##args)

#define ACL_RET_FALSE(iRet, strError, args...)              RET_FALSE(MID_ACL, iRet, strError, ##args)
#define ACL_POINTER_FALSE(pRet, strError, args...)          POINTER_FALSE(MID_ACL, pRet, strError, ##args)

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"

#define FOR_EACH_LIST(pos, list)        for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)


/*
 * 常量定义
 */
#define NODE_CONN_STATUS				"Connected"

/* iptables 链名 */
#define ACL_CHAIN_LOCAL_SUB             "ACL_LOCAL_SUB"         /* 挂载到 filter 表的 INPUT 链中 */
#define ACL_CHAIN_LOCAL_INPUT           "ACL_LOCAL_INPUT"       /* 挂载到 filter 表的 INPUT 链中 */

#define ACL_CHAIN_REMOTE_SUB            "ACL_REMOTE_SUB"        /* 挂载到 nat 表的 PREROUTING 链中 */
#define ACL_CHAIN_REMOTE_PRE            "ACL_REMOTE_PRE"        /* 挂载到 nat 表的 PREROUTING 链中 */
#define ACL_CHAIN_REMOTE_INPUT          "ACL_REMOTE_INPUT"      /* 挂载到 filter 表的 INPUT 链中 */

/* 添加规则/删除规则 */
#define ACL_CMD_ADD                     1
#define ACL_CMD_DEL                     0

/* 连接新增/删除/建立/断开 */
#define ACL_CMD_CONN_EST                1
#define ACL_CMD_CONN_FIN                2
#define ACL_CMD_CONN_ADD                3
#define ACL_CMD_CONN_DEL                4

/* 失败返回/成功返回 */
#define ACL_RETURN_ON_FAIL              1
#define ACL_RETURN_ON_SUCCESS           0

/* 最大ACL段数 */
#define ACL_SEPARATOR_MAX      8
#define ACL_SEPARATOR_ERROR  -1

/* ACL 相关的 TR069 节点名 */
#define ACL_NODE_RACLEnable             "RACLEnable"
#define ACL_NODE_RACLNumberOfEntries    "RACLNumberOfEntries"
#define ACL_NODE_RACL                   "RACL"

#define ACL_NODE_LACLEnable             "LACLEnable"
#define ACL_NODE_LACLNumberOfEntries    "LACLNumberOfEntries"
#define ACL_NODE_LACL                   "LACL"

/* Device node */
#define ACL_NODE_DevPath                "DevPath"
#define ACL_NODE_DeviceName             "Cache_DeviceName"
#define ACL_NODE_DeviceIP               "Cache_DeviceIP"
#define ACL_NODE_ServiceNumberOfEntries "ServiceNumberOfEntries"
#define ACL_NODE_Service                "Service"

/* Service node */
#define ACL_NODE_Name                   "Name"
#define ACL_NODE_Enable                 "Enable"
#define ACL_NODE_Protocol               "Protocol"
#define ACL_NODE_SrcIP                  "SrcIP"
#define ACL_NODE_SrcMask                "SrcMask"
#define ACL_NODE_ExternalPort           "ExternalPort"
#define ACL_NODE_InternalPort           "InternalPort"
#define ACL_NODE_Description            "Description"

/* 其他相关节点路径定义 */
#define LAN_NODE_DeviceName             "X_TWSZ-COM_DeviceName"
#define WAN_NODE_ConnectedDeviceName    "X_TWSZ-COM_ConnectedDeviceName"
#define WAN_NODE_ExternalIPAddress      "ExternalIPAddress"
#define ACL_LAN_IPADDRESS                   "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"


/*
路径前缀
*/
#define ACL_PATH_PREFIX                 "InternetGatewayDevice.X_TWSZ-COM_ACL."

/* 节点路径 */
#define ACL_PATH_LACLEnable             ACL_PATH_PREFIX ACL_NODE_LACLEnable
#define ACL_PATH_LACLNumberOfEntries    ACL_PATH_PREFIX ACL_NODE_LACLNumberOfEntries
#define ACL_PATH_LACL                   ACL_PATH_PREFIX ACL_NODE_LACL

#define ACL_PATH_RACLEnable             ACL_PATH_PREFIX ACL_NODE_RACLEnable
#define ACL_PATH_RACLNumberOfEntries    ACL_PATH_PREFIX ACL_NODE_RACLNumberOfEntries
#define ACL_PATH_RACL                   ACL_PATH_PREFIX ACL_NODE_RACL


/*
正则定义
*/
#define ACL_REGEX_PREFIX                "^InternetGatewayDevice\\.X_TWSZ-COM_ACL\\."

/* WAN dev regex */
#define ACL_REGEX_WAN_CONN_IP            "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define ACL_REGEX_WAN_CONN_PPP           "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"


/* LAN dev regex */
#define ACL_REGEX_LAN_CONN              "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}$"

#define ACL_REGEX_WLAN_CONN              "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}\\.WLANConfiguration\\.[0-9]{1,5}$"



/******************************************************************************
*                                ENUM
******************************************************************************/
/* 实例类型 */
typedef enum tag_EN_ACL_INST_TYPE
{
    ACL_INST_TYPE_UNKNOWN = 0,          /* UNKNOWN */
    ACL_INST_TYPE_DEVICE,           /* dev实例 */
    ACL_INST_TYPE_SERVICE,          /* Service实例 */
} EN_ACL_InstType;

/* ACL类型 */
typedef enum tag_EN_ACL_ACLType
{
    ACL_ACLType_UNKNOWN = 0,        /* UNKNOWN */
    ACL_ACLType_LACL,               /* Local访问控制 */
    ACL_ACLType_RACL,               /* Remote访问控制  */
} EN_ACL_ACLType;


/******************************************************************************
*                                STRUCT
******************************************************************************/

/* 索引结构 */
typedef struct tag_ST_ACL_Index
{
    unsigned int nDevIdx;                 /* 对应于 DevPath 所处的index */
    unsigned int nServIdx;                /* 对应于 Service 所处的index */
} ST_ACL_Index;

/* Device实例对应节点信息 */
typedef struct tag_ST_ACL_DeviceNodes
{
    /* 设备节点 */
    char *szDevPath;
    char *szDeviceName;
    char *szDeviceIP;
} ST_ACL_DeviceNodes;


/* Service实例对应节点信息 */
typedef struct tag_ST_ACL_ServiceNodes
{
    /* 服务节点 */
    char *szName;
    char *szEnable;
    char *szProtocol;
    char *szSrcIP;
    char *szSrcMask;
    char *szInternalPort;
    char *szExternalPort;
} ST_ACL_ServiceNodes;


/* 实例结构 */
typedef struct tag_ST_ACL_Instance
{
    char acPath[MAX_PATH_LEN];              /* 实例路径 */
    EN_ACL_ACLType  enACLType;              /* 访问类型 */
    EN_ACL_InstType enInstType;             /* 实例类型 */
    ST_ACL_Index stIndex;                   /* 索引     */

    BOOL  bEnable;                          /* 标识是否启用，对应于 LACLEnable/RACLEnable */
    ST_ACL_DeviceNodes  stDeviceInfo;
    ST_ACL_ServiceNodes stServiceInfo;
} ST_ACL_Instance;


/* 遍历参数结构 */
typedef struct tag_ST_ACL_Para
{
    const char *szDevPath;
    ST_ACL_Instance *pstFoundInst;
} ST_ACL_Para;


/******************************************************************************
*                                FUNCTION
******************************************************************************/

/* Init && Destroy */
int ACL_ModuleRegister();
int ACL_ModuleInit(void);
int ACL_ModuleDestroy(void);


#endif //__TBS_ACL_H__

