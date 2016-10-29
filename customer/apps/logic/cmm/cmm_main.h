/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cmm_main.h
 文件描述 : cfm模块内部头文件, 主要用于全局函数声明, 内部全局数据结构定义


 修订记录 :
          1 创建 : 陈跃东
            日期 : 2007-8-23
            描述 :

**********************************************************************/
#ifndef __CMM_MAIN_H__
#define __CMM_MAIN_H__



#include "warnlog.h"
#include "cfg_api.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "tbsmsg.h"
#include "tbstype.h"
#include "tbserror.h"
#include "tbsutil.h"
#include "new_msg.h"
#include "common_msg_handler.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
#define HARDWARE_VERSION_NODE "InternetGatewayDevice.DeviceInfo.HardwareVersion"
#define SOFTWARE_VERSION_NODE "InternetGatewayDevice.DeviceInfo.SoftwareVersion"
#define FIRMWARE_VERSION_NODE "InternetGatewayDevice.DeviceInfo.ModemFirmwareVersion"

#define CMM_OK              TBS_SUCCESS
#define CMM_FAIL            TBS_FAILED

#define ERR_CMM_BUSY              ERR_BUSY
#define ERR_CMM_MALLOC_FAIL       ERR_MALLOC_FAILED
#define ERR_CMM_CREATEMSG_FAIL    ERR_CREATE_MSG_FAIL

#define LOG_CLEAR_TRIGGER_NODE  "InternetGatewayDevice.X_TWSZ-COM_Logger.LogClearTrigger"
#define CFG_NODE_LEN 512               /*节点名字/值 最大长度*/ 


#define CMM_MAX_MID_NUM          100
#define CMM_MSG_PART_STEP        16
#define CMM_MAX_PATH_LEN         256


typedef  long  CMM_RET;


#ifdef _CMM_DEBUG

#define CMM_ERR(ErrCode, arg...) \
    printf("\n[CMM]: %s %d errno: %04u%04u %s", __FILE__, __LINE__, \
        TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno)); \
    printf(" para: " arg); \
    printf("\n")

#define CMM_LOG(arg...) \
    printf("\n[CMM]: %s %d ",  __FILE__, __LINE__); \
    printf(" log: " arg); \
    printf("\n");


/*
#define CMM_ERR(ErrCode, arg...) \
    COMMON_TRACE(MID_CMM, "errno: %04u%04u %s.", \
        TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno)); \
    COMMON_TRACE(MID_CMM, " para: " arg)

#define CMM_LOG(arg...) TBS_TRACE(MID_CMM, " log " arg)
*/


/*lint -save -e652*/
/*
MSG_RET CMM_MSG_SendMessage(ST_MSG *pstMsg);
#define MSG_SendMessage CMM_MSG_SendMessage
*/
/*lint -restore*/

/*
#define CMM_PRINT_MSG_IN(pstMsg) \
    if (NULL != pstMsg) \
    { \
        CMM_LOG((CMM_LOG_HND "[CMM Recv]: MsgHead:%04x %04x %08lx %04x %lu", \
               pstMsg->stMsgHead.usSrcMID, \
               pstMsg->stMsgHead.usDstMID, pstMsg->stMsgHead.ulMsgID, \
               pstMsg->stMsgHead.usMsgType, pstMsg->stMsgHead.ulBodyLength)); \
    }

void CMM_PrintMsgIn(const ST_MSG *pstMsg);
#define CMM_PRINT_MSG_IN(pstMsg)  CMM_PrintMsgIn(pstMsg)
*/

#define CMM_PRINT_MSG_IN(pstMsg) (void)0


#else

#define CMM_ERR(ErrCode, arg...) (void)0
#define CMM_LOG(arg...) (void)0
#define CMM_PRINT_MSG_IN(pstMsg) (void)0

#endif



#define STATIC  static



#ifdef _CMM_UT

#undef STATIC
#define STATIC

void *ut_cmm_malloc(unsigned long ulLen);
int UT_CMM_MSG_SendMessage(ST_MSG *pstMsg);
long UT_CMM_CFG_GetNotification(ST_PARA_VAL **ppstParaVal);
ST_MSG *UT_CMM_MSG_CreateMessage(unsigned long ulSize);
void UT_CMM_MSG_ReleaseMessage(ST_MSG *pstMsg);
MSG_RET UT_CMM_MSG_RegModule(unsigned short usMID, FUN_RECEIVE funReceive);
MSG_RET UT_CMM_MSG_UnregModule(unsigned short usMID, FUN_RECEIVE funReceive);



#define malloc                    ut_cmm_malloc
#undef MSG_SendMessage
#define MSG_SendMessage           UT_CMM_MSG_SendMessage
#define CFG_GetNotification       UT_CMM_CFG_GetNotification

#define MSG_CreateMessage         UT_CMM_MSG_CreateMessage
#define MSG_ReleaseMessage        UT_CMM_MSG_ReleaseMessage
#define MSG_RegModule             UT_CMM_MSG_RegModule
#define MSG_UnregModule           UT_CMM_MSG_UnregModule

#endif





#define CMM_LOG_LEVEL   6



/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/

#define CMM_CHECK_IS_BUSY() \
{ \
    ret = CMM_CheckIsIdle(); \
    if (CMM_OK != ret) \
    { \
        CMM_ERR(ret); \
        return ret; \
    } \
}

#define CMM_SWITCH_TO_BUSY() \
{ \
    ret = CMM_SwitchToBusy(); \
    if (CMM_OK != ret) \
    { \
        CMM_ERR(ret); \
        return ret; \
    } \
}


#define CMM_CHECK_IS_EXPERCT_MSG(pstMsg) \
{ \
    ret = CMM_CheckIsEepectMsg(pstMsg); \
    if (CMM_OK != ret) \
    { \
        CMM_ERR(ret); \
        return CMM_OK; \
    } \
}


/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/

CMM_RET CMM_SaveMsgState(const ST_MSG *pstMsg, ST_MSG **ppstSaveMsg);

void CMM_SetInternalMsgType(unsigned short usMsgType);

void CMM_FillAckMsg(ST_MSG *pstAckMsg);

void CMM_FillInternalMsg(ST_MSG *pstAckMsg);

void CMM_SetCurDstMID(unsigned short usMID);

CMM_RET CMM_SwitchToBusy(void);

void CMM_SwitchToIdle(void);
CMM_RET CMM_CheckIsIdle(void);

CMM_RET CMM_CheckIsEepectMsg(const ST_MSG *pstMsg);


char *CMM_GetAccessor(void);

void CMM_Reset(void);


unsigned short *CMM_GetMIDList(void);
void CMM_DoRestOpt(CMM_RET ret, ST_MSG *pstMsg);



/* 消息转发, 针对ADD 或者 Del 类型的消息 */
/*************************************************************************
Function:      CMM_RET CMM_TransmitMsg(const ST_MSG *pstMsg)
Description:   消息转发, 针对ADD 或者 Del 类型的消息
Calls:         MSG_SendMessage
               CFG_GetNodeMID
Data Accessed: g_apcCmmAccessor
               g_ucCmmAccessor
Data Updated:  g_acCmmErrItem
Input:         pstMsg, 请求消息
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CMM_RET CMM_TransmitMsg(const ST_MSG *pstMsg);

/*************************************************************************
Function:      CMM_RET CMM_TransmitAckMsg(const ST_MSG *pstMsg)
Description:   转发应答消息,当模块应答错误时, 可直接将该消息作为应答转发给请求端
Calls:         MSG_SendMessage
Data Accessed: 无
Data Updated:  无
Input:         pstMsg, 应答消息
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CMM_RET CMM_TransmitAckMsg(const ST_MSG *pstMsg);



/*************************************************************************
Function:      void CMM_ValListCat(ST_PARA_VAL **ppstValTotalList,
                                           ST_PARA_VAL *pstValList)
Description:   链表连接, 用在处理NameGet, AttrGet时将每个Item
                                             处理结果连在一起
Calls:         无
Data Accessed: 无
Data Updated:  无
Input:         ppstValTotalList, 总链表
               pstValList, 子链表
Output:        无
Return:        无
Others:
*************************************************************************/
void CMM_ValListCat(ST_PARA_VAL **ppstValTotalList, ST_PARA_VAL *pstValList);


/*************************************************************************
Function:      unsigned long CMM_CalValAckMsgLen(ST_PARA_VAL *pstValList,
                                                 unsigned long *pulCount)
Description:   计算NotiGet处理完后,所有Item的总长度, 同时计算总个数
Calls:         无
Data Accessed: 无
Data Updated:  无
Input:         pstValList, Item链表
Output:        pulCount, 输出Item的个数
Return:        消息长度
Others:
*************************************************************************/
unsigned long CMM_CalValAckMsgLen(ST_PARA_VAL *pstValList, unsigned long *pulCount);

/*************************************************************************
Function:      void CMM_FillValAckMsg(ST_PARA_VAL *pstValList, char *pcMsg)
Description:   填充Noti应答消息
Calls:         无
Data Accessed: 无
Data Updated:  无
Input:         pstValList, Item链表
Output:        pcMsg, 填充后的消息体
Return:        无
Others:
*************************************************************************/
void CMM_FillValAckMsg(ST_PARA_VAL *pstValList, char *pcMsg);


void CMM_SetErrNode(const char *pcNode);



#define CMM_GET_ULONG(ptr, i)        (((unsigned long *)ptr)[i])
#define CMM_SET_ULONG(ptr, i, val)   \
   (((unsigned long *)ptr)[i] = (unsigned long)(val))



typedef CMM_RET (*FUNC_CMM_MSG_PROC)(ST_MSG *pstMsg);
typedef void (*FUNC_CMM_GET_COUNT)(char *pcMsgBody, unsigned long *pulCount, char **ppcItem);

CMM_RET CMM_ProcValSetMsg(ST_MSG *pstMsg);
CMM_RET CMM_ProcValSetAckMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcValGetMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcUpdateAckMsg(const ST_MSG *pstMsg);
#if 0
CMM_RET CMM_ProcAddMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcAddAckMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcDelMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcDelAckMsg(const ST_MSG *pstMsg);
#endif
CMM_RET CMM_ProcNameGetMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcAttrGetMsg(ST_MSG *pstMsg);
CMM_RET CMM_ProcAttrSetMsg(ST_MSG *pstMsg);
CMM_RET CMM_ProcNotiGetMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcNotiClearMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcCfgSaveMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcCfgRecoverMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcRebootMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcCfgUpgMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcCfgBkpMsg(const ST_MSG *pstMsg);
CMM_RET CMM_ProcCfgClearMsg(const ST_MSG *pstMsg);

CMM_RET CMM_ProcAddMsgNew(const ST_MSG *pstMsg);
CMM_RET CMM_ProcAddAckMsgNew(const ST_MSG *pstMsg);
CMM_RET CMM_ProcDelMsgNew(const ST_MSG *pstMsg);
CMM_RET CMM_ProcDelAckMsgNew(const ST_MSG *pstMsg);


CMM_RET CMM_SendErrAckMsg(CMM_RET ret, const ST_MSG *pstMsg);
char *CMM_StandardErrCode(CMM_RET ret, unsigned long *pulResult,
                                       const char **ppcRemark);


void CMM_FreeAllToSendMsg(void);
void CMM_FreeAllValGetMsg(void);

CMM_RET CMM_NotiInform(unsigned char usNotiType);

void CMM_SetValGetCommProc(FUNC_CMM_MSG_PROC pfnCommProc);
void CMM_SetValGetCountGet(FUNC_CMM_GET_COUNT pfnCountGet);

CMM_RET CMM_SetLeaf(int nValNum, char aszName[][CFG_NODE_LEN], char aszVal[][CFG_NODE_LEN]);
/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif

#endif /* __CMM_MAIN_H__ */

