/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: antiattack.h

 文件描述: 防攻击模块的头文件

 修订记录:

           1 作者 : 匡素文
             日期 : 2008-11-29
             描述 : 从FIREWALL模块中分离出来

**********************************************************************/
#ifndef __TBS_ANTIATTACK_H
#define __TBS_ANTIATTACK_H

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/
/* 调试信息宏*/

//#define DG_DEBUG 1
#ifdef DG_DEBUG
#define DG_TRACE(fmt, args...)                  COMMON_TRACE(MID_DIAG, fmt"\n", ##args)
//#define DG_TRACE(fmt, args...)                  printf("=========%04x %s:%25s:%4d "fmt"\n", MID_DIAG, __FILE__, __func__, __LINE__, ##args)

#define DG_FUNC_IN(fmt, args...)                DG_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define DG_FUNC_OUT()                           DG_TRACE("--O %s\n", __FUNCTION__)

#else
#define DG_TRACE(str, args...)

#define DG_FUNC_IN(fmt, args...)
#define DG_FUNC_OUT()


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

#define DG_LOG_ERR(fmt, args...)                          COMMON_LOG_ERROR(MID_DIAG, fmt, ##args)

#define DG_RET_LOG(iRet, strError, args...)               RET_LOG(MID_DIAG, iRet, strError, ##args)
#define DG_POINTER_LOG(pRet, strError, args...)           POINTER_LOG(MID_DIAG, pRet, strError, ##args)

#define DG_RET_RETURN(iRet, strError, args...)            RET_RETURN(MID_DIAG, iRet, strError, ##args)
#define DG_POINTER_RETURN(pRet, strError, args...)        POINTER_RETURN(MID_DIAG, pRet, strError, ##args)

#define DG_RET_GOTO(iRet, strError, gotoTag)              RET_GOTO(MID_DIAG, iRet, strError, gotoTag)
#define DG_POINTER_GOTO(pRet, gotoTag, strError, args...) POINTER_GOTO(MID_DIAG, pRet, gotoTag ,strError, ##args)

#define DG_RET_FALSE(iRet, strError, args...)             RET_FALSE(MID_DIAG, iRet, strError, ##args)
#define DG_POINTER_FALSE(pRet, strError, args...)         POINTER_FALSE(MID_DIAG, pRet, strError, ##args)

#define ERR_GET(node)                       "Get Node["node"] value failed.\n"
#define ERR_SET(node)                       "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)                   "Set Node["node"] value failed, value = %s\n"


/************************** 常量定义 **************************/

/* 节点名定义 */
#define NODE_DiagnosticsState               "DiagnosticsState"
#define NODE_Interface                      "Interface"
#define NODE_Status                         "Status"
#define NODE_AutoPVCSearch                  "AutoPVCSearch"

#define NODE_ADSLSynchronization            "ADSLSynchronization"
#define NODE_ATMF5SegmentLoopback           "ATMF5SegmentLoopback"
#define NODE_ATMF5EndToEndLoopback          "ATMF5EndToEndLoopback"
#define NODE_ATMF4SegmentLoopback           "ATMF4SegmentLoopback"
#define NODE_ATMF4EndToEndLoopback          "ATMF4EndToEndLoopback"
#define NODE_IPPingGateway                  "IPPingGateway"
#define NODE_IPPingPrimaryDNS               "IPPingPrimaryDNS"
#define NODE_WANATMF5LBDIAG                 "WANATMF5LoopbackDiagnostics"

#define NODE_Enable                         "Enable"
#define NODE_SearchPVC                      "SearchPVC"
#define NODE_FoundPVC                       "FoundPVC"
#define NODE_DIAG_NUM_RPT                   "NumberOfRepetitions"
#define NODE_DefaultPVCList                 "DefaultPVCList"

#define NODE_DIAG_TIME_OUT                  "Timeout"
#define NODE_DIAG_SUCCESSCOUNT              "SuccessCount"
#define NODE_DIAG_FAILURECOUNT              "FailureCount"
#define NODE_DIAG_AVERAGERESPONSETIME       "AverageResponseTime"
#define NODE_DIAG_MINRESPONSETIME           "MinimumResponseTime"
#define NODE_DIAG_MAXRESPONSETIME           "MaximumResponseTime"

#define NODE_DNSOverrideAllowed             "DNSOverrideAllowed"
#define NODE_DNSServers                     "DNSServers"
#define NODE_UsrDNSServers                  "X_TWSZ-COM_UsrDNSServers"

#define NODE_DefaultGateway                 "DefaultGateway"
#define NODE_RemoteIPAddress                "RemoteIPAddress"

/* 节点路径定义 */
#define DG_PATH_PREFIX                      "InternetGatewayDevice.X_TWSZ-COM_Diagnostics."
#define DG_STATUS_PREFIX                    DG_PATH_PREFIX NODE_Status "."
#define DG_SEARCH_PREFIX                    DG_PATH_PREFIX NODE_AutoPVCSearch "."
#define DG_PATH_WANCONNDEV                  "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANATMF5LoopbackDiagnostics."
#define DG_CONN_NAME_PATH                   "WANDSLLinkConfig.DestinationAddress"

/* 正则定义 */
#define DG_REGEX_PREFIX                     "^InternetGatewayDevice\\.X_TWSZ-COM_Diagnostics\\."
#define DG_REGEX_ATMF5_PREFIX               "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANATMF5LoopbackDiagnostics\\."

/* WAN dev regex */
#define DG_REGEX_IPCONN_INST                "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}\\.$"
#define DG_REGEX_PPPCONN_INST               "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}\\.$"

#define DG_REGEX_WAN_CONN_IP                "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define DG_REGEX_WAN_CONN_PPP               "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"



#define DG_MAX_SYNCONNECTIONS_DEF           50
#define DG_MAX_PVC_PAIRS                    32
#define MAX_STATION_INFO_LENGTH             8192

/* 添加规则/删除规则 */
#define DG_CMD_NONE                         0
#define DG_CMD_ADD                          1
#define DG_CMD_DEL                          2

#define DG_SPECCIL_NAS_SEARCH               "nas10"
#define DG_SPECCIL_VNETNAS_SEARCH           "nas10_10"
/* DiagnosticsState 几个状态值 */
#define DG_STATE_NONE                       "None"
#define DG_STATE_REQUESTED                  "Requested"
#define DG_STATE_COMPLETE                   "Complete"


/******************************************************************************
*                                ENUM
******************************************************************************/



/******************************************************************************
*                                STRUCT
******************************************************************************/

/* 诊断项 */
typedef struct tagST_DG_Diag
{
    int iDiagId;
    char *szStatusNode;
    void *fnDiagnostics;
}ST_DG_Diag;

typedef struct XtmOamCellInfo
{
    unsigned int vpi;       /* Virtual Path */
    unsigned int vci;       /* Virtual Channel */
    unsigned char ucCircuitType;
    unsigned int ulTimeout;
    unsigned int ulRepetition;
   char  szPath[MAX_CMD_LEN]; /*wan connection path*/
} XTM_OAM_CELL_INFO;

typedef int (*FN_DG_Diag)(void *pstDiag, void *pData);

/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init && Destroy */
int DG_ModuleInit();
int DG_ModuleDestroy();


#endif//__TBS_ANTIATTACK_H
