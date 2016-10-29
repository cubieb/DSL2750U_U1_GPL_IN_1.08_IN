/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称 : tracert.h
 文件描述 : traceroute模块头文件

 修订记录 :
          1 创建 : 刘立思
            日期 : 2009-4-21
            描述 :
**********************************************************************/

#ifndef _TRACERT_H_
#define _TRACERT_H_

#include "tbstype.h"
#include "warnlog.h"
#include "pc_api.h"
#include "cfg_api.h"
#include "tbsmsg.h"
#include "tbsutil.h"
#include "tbserror.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

#define TRACERT_TREE "InternetGatewayDevice.TracerouteDiagnostics."
#define TRACERT_TREE_REG "InternetGatewayDevice\\.TracerouteDiagnostics\\."

#define TRACERT_WAN_CONN_IP  "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define TRACERT_WAN_CONN_PPP "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"
#define TRACERT_LAN_CONN     "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}\\.LANHostConfigManagement\\.IPInterface\\.[0-9]\{1,5}$"

#define TRACERT_STATE              "DiagnosticsState"
#define TRACERT_INTF               "Interface"
#define TRACERT_HOST               "Host"
#define TRACERT_WAITTIME           "Timeout"
#define TRACERT_MAXTTL             "MaxHopCount"
#define TRACERT_RESULT             "X_TWSZ-COM_TracertResult"
#define TRACERT_RHNum              "RouteHopsNumberOfEntries"

#define TRACERT_MAX_PATH_LEN    256
/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef TRACERT_DEBUG
    #define TRACERT_TRACE(fmt, args...) COMMON_TRACE(MID_TRACERT, fmt, ##args)
    #define TRACERT_ERR(ErrCode) \
        TRACERT_TRACE("errno: %04u%04u %s.", \
            TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno))

    #define TRACERT_ERR_P(ErrCode, ParaList) \
        TRACERT_TRACE("errno: %04u%04u %s. Para: ", \
            TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno)); \
        TRACERT_TRACE ParaList

    #define TRACERT_LOG(info)  TBS_TRACE info
    #define TRACERT_PRINT_MSG_IN(pstMsg) (void)0
#else
    #define TRACERT_TRACE(fmt, args...) (void)0

    #define TRACERT_ERR(ErrCode) (void)0
    #define TRACERT_ERR_P(ErrCode, ParaList) (void)0
    #define TRACERT_LOG(info) (void)0
    #define TRACERT_PRINT_MSG_IN(pstMsg) (void)0
#endif


/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define TRACERT_SET_NODE_VAL(pcPath, pcVal) \
{ \
    ret = CFG_SetNodeVal(pcPath, pcVal, NULL); \
    if (TRACERT_OK != ret) \
    { \
        TRACERT_ERR(ret); \
        return ret; \
    } \
}

#define TRACERT_CHECK_STATE(expect, errno) \
{ \
    if (expect != s_ucDgnStateMachine) \
    { \
        TRACERT_ERR(errno); \
        return errno; \
    } \
}

#define TRACERT_LEAF_AND_COUNT(list) list, sizeof(list)/sizeof(list[0])

/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/
enum{
    TRACERT_NO_ERROR,
    TRACERT_UNKNOW_HOST,
    TRACERT_NO_ROUTE,
    TRACERT_UNKNOW_ERROR
};

/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/
typedef struct
{
    char *pcState;
    char *pcIntf;
    char *pcHost;
    unsigned int uiWaitTime;
    unsigned int uiMaxTTL;

    char *TracertResult;
} ST_TRACERT_INFO;

/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/
int Tracert_ProcSignalMsg(const ST_MSG * pstMsg);
int Tracert_ProcStateMsg(const ST_MSG * pstMsg);
int Tracert_ProcOutputMsg(const ST_MSG * pstMsg);
int TRACERT_Apply(void);
ST_MSG * COMM_GetCurMsg(void);

/******************************************************************************
*                                 END                                        *
******************************************************************************/
#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif

#endif /* _TRACERT_H_ */
