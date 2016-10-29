/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: firewall.h

 文件描述: 防火墙模块的头文件

 修订记录:
           1 创建 : 李伟
             日期 : *
             描述 : 创建

           2 增加 : 詹剑
             日期 : *
             描述 : 修改

           3 修改 : 匡素文
             日期 : 2008-11-18
             描述 : 重构

**********************************************************************/
#ifndef __TBS_FIREWALL_H
#define __TBS_FIREWALL_H

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/
/* 调试信息宏*/

#ifdef FW_DEBUG
#define FW_TRACE(fmt, args...)                              COMMON_TRACE(MID_FIREWALL, fmt"\n", ##args)
#define FW_FUNC_IN(fmt, args...)                            FW_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define FW_FUNC_OUT()                                       FW_TRACE("--O %s\n", __FUNCTION__)

#else
#define FW_TRACE(str, args...)
#define FW_FUNC_IN(fmt, args...)
#define FW_FUNC_OUT()


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

#define FW_LOG_ERR(fmt, args...)                            COMMON_LOG_ERROR(MID_FIREWALL, fmt, ##args)

#define FW_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_FIREWALL, iRet, strError, ##args)
#define FW_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_FIREWALL, pRet, strError, ##args)

#define FW_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_FIREWALL, iRet, strError, ##args)
#define FW_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_FIREWALL, pRet, strError, ##args)

#define FW_RET_GOTO(iRet, strError, gotoTag)                RET_GOTO(MID_FIREWALL, iRet, strError, gotoTag)
#define FW_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_FIREWALL, pRet, gotoTag ,strError, ##args)

#define FW_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_FIREWALL, iRet, strError, ##args)
#define FW_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_FIREWALL, pRet, strError, ##args)

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"

#define FOR_EACH_LIST(pos, list)        for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)


/************************** 常量定义 **************************/

/* 节点名定义 */
#define FW_NODE_IPFilterEnable          "IPFilterEnable"
#define FW_NODE_SecurityLevel           "SecurityLevel"
#define FW_NODE_CurrentLANFilterTable   "CurrentLANFilterTable"
#define FW_NODE_CurrentWANFilterTable   "CurrentWANFilterTable"

/* 过滤表节点 */
#define FW_NODE_Enable                  "Enable"
#define FW_NODE_DevPath                 "DevPath"
#define FW_NODE_DeviceName              "Cache_DeviceName"
#define FW_NODE_SrcIP                   "SrcIP"
#define FW_NODE_DestIP                  "DestIP"
#define FW_NODE_SrcMask                 "SrcMask"
#define FW_NODE_DstMask                 "DstMask"
#define FW_NODE_SrcPort                 "SrcPort"
#define FW_NODE_DestPort                "DestPort"
#define FW_NODE_Protocol                "Protocol"
#define FW_NODE_Description             "Description"

#define FW_TABLE_NAME_LEN               20
#define FW_TALBE_LanBlackFilter         "LanBlackFilter"
#define FW_TALBE_LanWhiteFilter         "LanWhiteFilter"
#define FW_TALBE_WanBlackFilter         "WanBlackFilter"
#define FW_TALBE_WanWhiteFilter         "WanWhiteFilter"



#define LAN_NODE_DeviceName             "X_TWSZ-COM_DeviceName"


/* 最多规则条数 */
#define FW_INSTANCE_MAX_NUM             20


/* 节点路径定义 */
#define FW_PATH_PREFIX                  "InternetGatewayDevice.X_TWSZ-COM_Firewall."

#define FW_PATH_IPFilterEnable          FW_PATH_PREFIX "IPFilterEnable"
#define FW_PATH_AntiDosEnabled          FW_PATH_PREFIX "AntiDosEnabled"
#define FW_PATH_SecurityLevel           FW_PATH_PREFIX "SecurityLevel"
#define FW_PATH_CurrentLANFilterTable   FW_PATH_PREFIX "CurrentLANFilterTable"
#define FW_PATH_CurrentWANFilterTable   FW_PATH_PREFIX "CurrentWANFilterTable"

#define LAN_PATH_PREFIX                 "InternetGatewayDevice.LANDevice."
#define WAN_PATH_PREFIX                 "InternetGatewayDevice.WANDevice."

/* 格式定义 */
#define FW_FORMAT_INST                  FW_PATH_PREFIX "%[^.].%u."


/* 正则定义 */
#define FW_REGEX_PREFIX                 "^InternetGatewayDevice\\.X_TWSZ-COM_Firewall\\."
#define FW_REGEX_TABLE_PREFIX           FW_REGEX_PREFIX"(Wan|Lan)(Black|White)Filter\\.[0-9]{1,5}\\."


/* WAN dev regex */
#define FW_REGEX_IPCONN_INST            "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}\\.$"
#define FW_REGEX_PPPCONN_INST           "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}\\.$"

#define FW_REGEX_WAN_CONN_IP            "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define FW_REGEX_WAN_CONN_PPP           "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"

/* LAN dev regex */
#define FW_REGEX_LAN_CONN               "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}$"
#define FW_REGEX_WLAN_CONN              "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}\\.WLANConfiguration\\.[0-9]{1,5}$"


/*
命令所用参数
*/

/* 连接新增/删除/建立/断开 */
#define FW_CMD_CONN_EST                 1
#define FW_CMD_CONN_FIN                 2
#define FW_CMD_CONN_ADD                 3
#define FW_CMD_CONN_DEL                 4

/* 添加规则/删除规则 */
#define FW_CMD_ADD                      1
#define FW_CMD_DEL                      0

/* 添加/删除 wan/lan 设备 */
#define FW_NOCHANG_DEV                  0
#define FW_ADD_WAN_DEV                  1
#define FW_DEL_WAN_DEV                  2
#define FW_ADD_LAN_DEV                  3
#define FW_DEL_LAN_DEV                  4


/* 输出格式控制*/
#define FMT_SUCC                        ECHO_GREEN"[Success] "ECHO_NONE
#define FMT_FAIL                        ECHO_RED"[Failed] "ECHO_NONE


/******************************************************************************
*                                ENUM
******************************************************************************/
/* IP过滤表枚举 */
typedef enum tag_EN_FW_Filter
{
    EN_LanWhiteFilter = 1,
    EN_LanBlackFilter = 2,
    EN_WanWhiteFilter = 3,
    EN_WanBlackFilter = 4,
} EN_FW_Filter;

/* 防火墙安全级别 */
typedef enum tag_EN_FW_SecurityLevel
{
    EN_User_Ddefined_SecurityLevel  = 0, /* 自定义 */
    EN_Low_SecurityLevel            = 1, /* 低 */
    EN_Medium_SecurityLevel         = 2, /* 中 */
    EN_High_SecurityLevel           = 3, /* 高 */
} EN_FW_SecurityLevel;


/******************************************************************************
*                                STRUCT
******************************************************************************/

/* 路径对应设备名 */
typedef struct tag_ST_CONNPATH_DEVNAME
{
    struct list_head head;
    /* 注意:没有'.'后缀 */
    char szConnPath[MAX_PATH_LEN];
    char szDevName[MAX_IF_NAME_LEN];
} ST_ConnPath_DevName;

typedef struct tag_ST_FW_Index
{
    unsigned int nTableIdx;
    unsigned int nRuleIdx;
} ST_FW_Index;

/* Firewall单个实例对应节点信息 */
typedef struct tag_ST_FW_Nodes
{
    char *szEnable;
    char *szDevPath;
    char *szProtocol;
    char *szSrcIP;
    char *szDestIP;
    char *szSrcPort;
    char *szDestPort;
    char *szSrcMask;
    char *szDstMask;
    char *szDescription;
} ST_FW_Nodes;

/* firewall实例 */
typedef struct tag_ST_FW_Instance
{
    char acPath[MAX_PATH_LEN];
    ST_FW_Index stIndex;
    ST_FW_Nodes stNodes;
    struct tag_ST_FW_Instance *pstNext;
} ST_FW_Instance;


typedef struct tag_ST_FW_PARA
{
    char *szDevPath;
    int iType;
} ST_FW_Para;


/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init && Destroy */
int FW_ModuleInit();
int FW_ModuleDestroy();




#endif//__TBS_FIREWALL_H
