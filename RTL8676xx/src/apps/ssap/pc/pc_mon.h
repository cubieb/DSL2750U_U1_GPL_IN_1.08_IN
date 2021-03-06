/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : pc_mon.h
 文件描述 : 进程监控功能实现的相关定义和声明


 函数列表 :


 修订记录 :
          1 创建 : 陈跃东
            日期 : 2007-8-23
            描述 :

**********************************************************************/


#ifndef __PC_MON_H__
#define __PC_MON_H__


#include "pc_main.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif



/******************************************************************************
 *                                 MACRO                                      *
******************************************************************************/

/* 配置文件一行最大长度� */
#define PC_FILE_LINE_MAX_LEN   512

/* 最大启动参数个数 */
#define PC_START_MAX_ARGC      20

/* 组件进程输出最大长度 */
#define PC_PROC_MAX_OUT_LEN    1024

/* 匹配到的信息的最大长度( 不含消息头) */
#define PC_MSG_MAX_OUT_LEN      5*1024




/******************************************************************************
 *                                 ENUM                                       *
******************************************************************************/



/******************************************************************************
 *                                STRUCT                                      *
******************************************************************************/



/******************************************************************************
 *                               GLOBAL VAR                                   *
******************************************************************************/




/******************************************************************************
 *                               FUNCTION                                     *
******************************************************************************/



/*************************************************************************
 Function:      PC_RET PC_MonInit(void)
 Description:   监控管理模块初始化函数
 Calls:         PC_StartKeyProc
 Data Accessed:
 Data Updated:
 Input:         无
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_MonInit(const char *pcCfgFile);

/*************************************************************************
 Function:      PC_RET PC_StartKeyProc(char *pcCfgFile)
 Description:   启动系统关键进程
 Calls:         PC_AllocPcb,
                PC_GetNextWord,
                PC_GetStartInfo
 Data Accessed:
 Data Updated:
 Input:         pcCfgFile, 配置文件名, 含路径
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_StartKeyProc(const char *pcCfgFile);


/*************************************************************************
 Function:      PC_RET PC_OutMonProc(void)
 Description:   输出信息监控处理函数
 Calls:         PC_GetNextPcb
                MSG_SendMessage
 Data Accessed:
 Data Updated:
 Input:         无
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_OutMonProc(void);


/*************************************************************************
 Function:      PC_RET PC_StateMonProc(void)
 Description:   进程状态监控处理函数
 Calls:         PC_GetPcbByPid
                MSG_SendMessage
                forkpty
 Data Accessed:
 Data Updated:
 Input:         pcCfgFile, 系统配置文件
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_StateMonProc(void);


PC_RET PC_MsgFunc(ST_PC_PROC_CTRL_INFO *pstPcMsg, unsigned short usComID,
                  unsigned char *pucProcID);


/******************************************************************************
 *                                 END                                        *
******************************************************************************/

#ifdef _PC_VER_R1

#include "pc_pcb.h"

PC_RET PC_StartProcOld(
    unsigned short           usComID,
    unsigned char            ucNeedSteady,
    char            *apcArgVals[],
    unsigned char            ucMonCount,
    char            *apcMonVals[],
    unsigned char           *pucProcID);


PC_RET PC_SendProcSignalOld(
    unsigned short           usComID,
    unsigned char            ucProcID,
    unsigned char            ucSignal,
    unsigned char            ucNeedRestart,
    unsigned char            ucNeedSteady,
    char                    *apcArgVals[],
    unsigned char            ucMonCount,
    char                    *apcMonVals[]);

PC_RET PC_StateMonProcOld(ST_PC_PCB *pstPcb, unsigned char ucProcID);


PC_RET PC_OutMonProcOld(const ST_PC_PCB *pstPcb, unsigned char ucProcID);

#endif



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __PC_MON_H__ */



