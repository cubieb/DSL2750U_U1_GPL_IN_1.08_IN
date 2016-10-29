/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : Ping_dgn.h
 文件描述 :
 函数列表 :
 修订记录 :
          1 创建 : 陈明鹏
            日期 : 2007-11-7
            描述 :
          1 修订 : 龙俊
            日期 : 2009-7-6
            描述 :
**********************************************************************/

#ifndef _PING_DGN_H
#define _PING_DGN_H

#include "ping_dgn.h"
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




#define DGN_TREE "InternetGatewayDevice.IPPingDiagnostics."

#define DGN_TREE_REG "InternetGatewayDevice\\.IPPingDiagnostics\\."

#define DGN_STATE              "DiagnosticsState"
#define DGN_INTF               "Interface"
#define DGN_HOST               "Host"
#define DGN_REPEAT             "NumberOfRepetitions"
#define DGN_TIMEOUT            "Timeout"
#define DGN_SIZE               "DataBlockSize"
#define DGN_DSCP               "DSCP"

#define DGN_SUCC               "SuccessCount"
#define DGN_FAIL               "FailureCount"
#define DGN_AVRG               "AverageResponseTime"
#define DGN_MIN                "MinimumResponseTime"
#define DGN_MAX                "MaximumResponseTime"


typedef struct
{
    char *pcState;
    char *pcIntf;
    char *pcHost;
    unsigned int uiRepeat;
    unsigned int uiTimeOut;
    unsigned int uiSize;
    unsigned int uiDscp;

    unsigned int uiSucc;
    unsigned int uiFail;
    unsigned int uiAvrg;
    unsigned int uiMin;
    unsigned int uiMax;

} ST_DGN_INFO;




/* 报告错误 */

#ifdef _DGN_DEBUG


/*
#define DGN_ERR_HND

#define DGN_ERR(ErrCode) \
    printf("\n[DGN]: %s %d errno: %04u%04u %s.\n", __FILE__, __LINE__, \
        TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno))

#define DGN_ERR_P(ErrCode, ParaList) \
    printf("\n[DGN]: %s %d errno: %04u%04u %s. Para: ", __FILE__, __LINE__, \
        TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno)); \
    printf ParaList; \
    printf("\n")

#define DGN_LOG_HND

#define DGN_LOG(info) \
    printf info; \
    printf("\n")
*/


#define DGN_ERR_HND MID_DGN,

#define DGN_ERR(ErrCode) \
    TBS_TRACE(MID_DGN, "errno: %04u%04u %s.", \
        TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno))

#define DGN_ERR_P(ErrCode, ParaList) \
    TBS_TRACE(MID_DGN, "errno: %04u%04u %s. Para: ", \
        TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno)); \
    TBS_TRACE ParaList


#define DGN_LOG_HND MID_DGN,
#define DGN_LOG(info)  TBS_TRACE info



/*lint -save -e652*/
/*
MSG_RET DGN_MSG_SendMessage(ST_MSG *pstMsg);
#define MSG_SendMessage   DGN_MSG_SendMessage
*/
/*lint -restore*/


/*
#define DGN_PRINT_MSG_IN(pstMsg) \
    if (NULL != pstMsg) \
    { \
        DGN_LOG((DGN_LOG_HND "[DGN Recv]: MsgHead:%04x %04x %08lx %04x %lu", \
               pstMsg->stMsgHead.usSrcMID, \
               pstMsg->stMsgHead.usDstMID, pstMsg->stMsgHead.ulMsgID, \
               pstMsg->stMsgHead.usMsgType, pstMsg->stMsgHead.ulBodyLength)); \
    }

void DGN_PrintMsgIn(const ST_MSG *pstMsg);
#define DGN_PRINT_MSG_IN(pstMsg)  DGN_PrintMsgIn(pstMsg)
*/

#define DGN_PRINT_MSG_IN(pstMsg) (void)0




#else

#define DGN_ERR(ErrCode) (void)0
#define DGN_ERR_P(ErrCode, ParaList) (void)0

#define DGN_LOG(info) (void)0

#define DGN_PRINT_MSG_IN(pstMsg) (void)0

#endif


#define STATIC static


#ifdef _DGN_UT

#undef STATIC
#define STATIC

#endif


#define DGN_OK  TBS_SUCCESS

#define ERR_DGN_MALLOC_FAIL    ERR_MALLOC_FAILED
#define ERR_DGN_BUSY           ERR_BUSY
#define ERR_DGN_INTERNAL       ERR_INTERNAL_ERR
#define ERR_DGN_CREATEMSG_FAIL ERR_CREATE_MSG_FAIL


typedef int  DGN_RET;


/* 实例运行结果定义 */
typedef struct
{
    char *pcState;

    unsigned int uiSucc;
    unsigned int uiFail;
    unsigned int uiMin;
    unsigned int uiAvrg;
    unsigned int uiMax;
    unsigned int uiSent;
    unsigned int uiUnreach;
    unsigned int uiBadAddr;

} ST_DGN_RESULT_INFO;



#define DGN_MAX_PATH_LEN    256


#define DGN_SET_NODE_VAL(pcPath, pcVal) \
{ \
    ret = CFG_SetNodeVal(pcPath, pcVal, NULL); \
    if (DGN_OK != ret) \
    { \
        DGN_ERR(ret); \
        return ret; \
    } \
}

#define DGN_CHECK_STATE(expect, errno) \
{ \
    if (expect != s_ucDgnStateMachine) \
    { \
        DGN_ERR(errno); \
        return errno; \
    } \
}

#define DGN_LEAF_AND_COUNT(list) list, sizeof(list)/sizeof(list[0])


int DGN_CheckIntfFirst(const char *pcVal);
int DGN_CheckIntf(const char *pcVal, unsigned char ucConfictCheck);

int DGN_CheckHost(const char *pcVal);

int DGN_CheckRepeat(const char *pcVal);

int DGN_CheckTimeout(const char *pcVal);

int DGN_CheckSize(const char *pcVal);

int DGN_CheckDSCP(const char *pcVal);

int DGN_CheckState(const char *pcVal);

int DGN_ConflictCheck(void);

int DGN_Apply(void);

int DGN_ProcSignalMsg(const ST_MSG * pstMsg);
int DGN_ProcStateMsg(const ST_MSG * pstMsg);
int DGN_ProcOutputMsg(const ST_MSG * pstMsg);

ST_MSG * COMM_GetCurMsg(void);

int DGN_ProcCtrl(const char *pcName, unsigned long ulInstNo, unsigned char ucOpt);


#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* _PING_DGN_H */

