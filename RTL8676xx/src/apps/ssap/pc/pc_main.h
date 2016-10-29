/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : pc_main.h
 文件描述 : pc模块内部私有头文件


 函数列表 :


 修订记录 :
          1 创建 : 陈跃东
            日期 : 2007-11-23
            描述 :

**********************************************************************/


#ifndef __PC_MAIN_H__
#define __PC_MAIN_H__


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "pc_api.h"
#include "tbsmsg.h"
#include "warnlog.h"

#include "pc_api.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif




/******************************************************************************
 *                                 MACRO                                      *
******************************************************************************/


/* 常用返回值 */

#ifndef PC_OK
#define PC_OK             0L
#endif

#ifndef PC_FAIL
#define PC_FAIL           (-1)
#endif



/* 本模块的错误码起始地址 */
#ifndef PC_ERR_ADDR
#define PC_ERR_ADDR                (MID_PC << 16)
#endif




/* 常用宏 */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL             0L
#endif


#define STATIC  static



/* PC模块的MID */
#define PC_MID                MID_PC

/* 进程状态 */
#define PC_PROC_OK            0
#define PC_PROC_EXIT          1


#define PC_Free(p)                 if (NULL != p)  free(p); p = NULL
#define PC_MemCpy(pv1, pv2, len)   (void)memcpy(pv1, pv2, (unsigned long)(len))

#define PC_MemSet       (void)memset
#define PC_StrLen       strlen



#ifdef _PC_UT


void PC_MSG_ReleaseMessage(ST_MSG *pstMsg);
int PC_MSG_ReceiveMessage(ST_MSG **ppstMsg, unsigned int iSecond);
int PC_MSG_SendMessage(ST_MSG *pstMsg);
void *stub_malloc(unsigned long ulLen);
int PC_MSG_RegModule(unsigned short usMid, FUN_RECEIVE pfnMsgProc);




#define PC_Malloc(len)  stub_malloc((unsigned long)(len))
#define MSG_CreateMessage(len)  stub_malloc(sizeof(ST_MSGHEAD) + (len))

#define MSG_ReleaseMessage     PC_MSG_ReleaseMessage
#define MSG_ReceiveMessage     PC_MSG_ReceiveMessage
#define MSG_SendMessage        PC_MSG_SendMessage
#define MSG_RegModule          PC_MSG_RegModule

#undef STATIC
#define STATIC

#else

#define PC_Malloc(len)  malloc((unsigned long)(len))

#endif


/* 报告错误 */

#ifdef _PC_DEBUG

#define PC_ERR(ErrCode, arg...) \
    syslog(LOG_INFO, "[PC]: %s %d errno: %04u%04u %s", __FILE__, __LINE__, \
        TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno)); \
    syslog(LOG_INFO, " para: " arg);

#define PC_LOG(arg...) \
    syslog(LOG_INFO, "[PC]: %s %d ",  __FILE__, __LINE__); \
    syslog(LOG_INFO, " log: " arg);


/*lint -save -e652*/
/*
MSG_RET PC_MSG_SendMessageShow(ST_MSG *pstMsg);
#define  MSG_SendMessage  PC_MSG_SendMessageShow
*/
/*lint -restore*/


/*
void PC_PrintMsgIn(const ST_MSG *pstMsg);
#define PC_PRINT_MSG_IN(pstMsg) PC_PrintMsgIn(pstMsg)

#define PC_PRINT_MSG_IN(pstMsg) \
    if (NULL != pstMsg) \
    { \
        PC_LOG((PC_LOG_HND "[PC Recv]: MsgHead:%04x %04x %08lx %04x %lu", \
               pstMsg->stMsgHead.usSrcMID, \
               pstMsg->stMsgHead.usDstMID, pstMsg->stMsgHead.ulMsgID, \
               pstMsg->stMsgHead.usMsgType, pstMsg->stMsgHead.ulBodyLength)); \
    }
*/

#define PC_PRINT_MSG_IN(pstMsg) (void)0

void PC_ShowProcInfo(
     char            *const apcArgVals[],
     unsigned char          ucMonCount,
     char            *const apcMonVals[]);

#define PC_SHOW_PROC_INFO(args, monc, monv) \
    PC_ShowProcInfo(args, monc, monv)


#else

#define PC_ERR(ErrCode, arg...) (void)0

#define PC_LOG(arg...) (void)0

#define PC_SHOW_PROC_INFO(args, monc, monv) (void)0

#define PC_PRINT_MSG_IN(pstMsg) (void)0

#endif

/* 配置文件名 */
#define PC_CFG_FILE          "/etc/pc.ini"


/* 核心进程 */
#define PC_CCP_KER_NAME      "/usr/bin/logic"


/******************************************************************************
 *                                 ENUM                                       *
******************************************************************************/

#define ERR_PC_CREATEMSG_FAIL  ERR_CREATE_MSG_FAIL
#define ERR_PC_FILE_OPEN       ERR_FILE_OPEN
#define ERR_PC_FILE_READ       ERR_FILE_READ




/******************************************************************************
 *                                STRUCT                                      *
******************************************************************************/




/******************************************************************************
 *                               GLOBAL VAR                                   *
******************************************************************************/




/******************************************************************************
 *                               FUNCTION                                     *
******************************************************************************/


#if defined(_PC_UT)
extern void *stub_malloc(unsigned long ulLen);
#endif


/*************************************************************************
 Function:      PC_RET PC_MsgProc(void *pvMsg)
 Description:   Pcb模块的消息处理入口
 Calls:         PC_StartProc
                PC_KillProc
                MSG_SendMessage
 Data Accessed:
 Data Updated:
 Input:         无
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_MsgProc(void *pvMsg);



/*************************************************************************
 Function:      PC_RET PC_Init(void)
 Description:   Pc模块初始化函数
 Calls:         PC_MsgReg
                PC_PcbInit
                PC_MonInit
 Data Accessed:
 Data Updated:
 Input:         无
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_Init(const char *szCfgFile);




/* 3. 接管主循环的函数 */
/*************************************************************************
 Function:      PC_RET PC_MainProc(void)
 Description:   pc模块的总调度函数
 Calls:         PC_RcvMsg
                PC_MsgProc
                PC_OutMonProc
                PC_StateMonProc
 Data Accessed:
 Data Updated:
 Input:         无
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_MainProc(void);





/******************************************************************************
 *                                 END                                        *
******************************************************************************/
#ifdef _PC_VER_R1
PC_RET PC_MsgProcOld(void *pvMsg);
#endif


#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif




#endif /* __PC_MAIN_H__ */


