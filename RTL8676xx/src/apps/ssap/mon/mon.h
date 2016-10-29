/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : mon.h
 文件描述 : mon 的头文件


 函数列表 :


 修订记录 :
          1 创建 : 詹剑
            日期 : 2008-3-17
            描述 :

**********************************************************************/

#ifndef  _MON_H_
#define  _MON_H_

#include "tbsmsg.h"
#include "tbstype.h"
#include "tbsutil.h"
#include "tbserror.h"
#include "warnlog.h"
#include "common.h"

/******************************************************************************
*                                 MACRO
******************************************************************************/

/* 调试信息宏 */

#ifdef MON_DEBUG
#define MON_TRACE(fmt, args...) COMMON_TRACE(MID_MON, fmt, ##args)
#else
#define MON_TRACE(str, args...)
#endif

#define LEVEL_1_SPACE " "
#define LEVEL_5_SPACE "     "

#define MON_BACKUP_FILE_NAME "/var/mon.conf"

#define MON_INTE_LEN_MAX 16     /* ifr_name(16 bytes) in struct ifreq */
#define MON_PATH_LEN_MAX 256

#define ELM_RECEIVE_TIME_OUT    3

#define MON_OPTION_PATH     "Path="
#define MON_OPTION_NAME     "Name="
#define MON_OPTION_STATUS   "Status="

#define MON_STR_UNKNOWN         "Unknown"
#define MON_STR_CONNECTED       "Connected"
#define MON_STR_DISCONNECTED    "Disconnected"
#define MON_STR_INITIALIZING    "Initializing"
#define MON_STR_HANDSHAKING    "HandShaking"


#define MON_STR_DUP_OPT_PATH        "DUP_OPT_PATH"
#define MON_STR_DUP_OPT_NAME        "DUP_OPT_NAME"
#define MON_STR_DUP_OPT_STATUS      "DUP_OPT_STATUS"

#define MON_MID_ALL 0xffff
#define MAX_MSG_LEN 1024


/******************************************************************************
*                                 ENUM
******************************************************************************/
typedef enum tag_EN_MON_INTF_TYPE
{
    MON_INTF_TYPE_NORMAL = 1,   /* 普通设备接口，如：以太网接口 */
    MON_INTF_TYPE_WLAN,         /* WLAN 设备接口 */
    MON_INTF_TYPE_DSL,         /* DSL 设备接口 */
} EN_MON_INTF_TYPE;

/* 枚举类型中的三个枚举项分别对应一个网络设备的三种连接状态：连接、断开、状态未知 */
typedef enum tag_EN_MON_INTF_LINKSTATE
{
    MON_INTF_LINKSTATE_UNKNOWN = 1,     /* 连接状态未知 */
    MON_INTF_LINKSTATE_CONNECTED,    /* 处于连接状态 */
    MON_INTF_LINKSTATE_DISCONNECTED, /* 处于断开状态 */
    MON_INTF_LINKSTATE_INITIALIZING, /*正在初始化 */
    MON_INTF_LINKSTATE_HANDSHAKING, /*正在握手 */
} EN_MON_INTF_LINKSTATE;

/* 需要监控的接口的信息的结构，用来记录需要监控的接口的一些信息 */
typedef struct tag_ST_MON_INTE_INFO
{
    struct tag_ST_MON_INTE_INFO *pstNext;
    char szInterface[MON_INTE_LEN_MAX];     /* 接口名 */
    char szPath[MON_PATH_LEN_MAX];          /* 接口名在配置文件中的路径 */
    EN_MON_INTF_LINKSTATE enLinkState;      /* 接口连接状态，有三种状态：未知、连接、断开 */
    unsigned short usMID;                   /* 接口所属的模块ID，哪个模块向监控模块注册了这个接口，这个接口就属于那个模块 */
    char acPad[2];
} ST_MON_INTE_INFO;


/******************************************************************************
*                               FUNCTION DECLARE                              *
******************************************************************************/
void MON_SigRoutine(int dunno);
int main(int argc, char **argv);
int MON_ModuleInit(void);
void MON_MsgProcess(ST_MSG *pstMsg);
int MON_MsgRegHandler(const ST_MSG *pstMsg);
int MON_MsgUnRegHandler(const ST_MSG *pstMsg);
int MON_MsgQueryHandler(const ST_MSG *pstMsg);
int MON_LinkStateInform(unsigned long ulMsgID, const ST_MON_INTE_INFO *pstIntfInfo);
int MON_IntfStateDetect(ST_MON_INTE_INFO *pstIntfInfo);
int MON_LinkStateMonitor(void);
int MON_AddNode(ST_MON_INTE_INFO *pstNewNode);
int MON_DelNode(ST_MON_INTE_INFO *pstIntfInfo);
int MON_CreateNode(unsigned short usMID, ST_MON_INTE_INFO ** ppstNewNode);
int MON_GetLinkState(const char *pszInterfaceName, EN_MON_INTF_TYPE enIntfType, EN_MON_INTF_LINKSTATE *penState);
ST_MON_INTE_INFO *MON_FindNode(unsigned short usMID, const char *pszIntfName);
int MON_DelAllNode(void);
int MON_ViewOneIntfInfo(const ST_MON_INTE_INFO *pstIntfInfo);
int MON_ViewIntfInfos(const ST_MON_INTE_INFO *pstIntfInfo);
int MON_CheckIntfName(const char *pszIntfName);
int MON_CheckOneIntfInfo(const ST_MON_INTE_INFO *pstIntfInfo);
int MON_WriteConfToFile(void);
int MON_GetConfFromFile(void);
int MON_ModuleDestroy(void);
int MON_MainProc(void);
int MON_ShowCurrentTime(void);

#endif
