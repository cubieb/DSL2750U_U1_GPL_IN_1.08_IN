/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : pc_pcb.h
 文件描述 : 进程控制块管理基本定义

 修订记录 :
          1 创建 : 陈跃东
            日期 : 2007-8-23
            描述 :

**********************************************************************/

#ifndef __PC_PCB_H__
#define __PC_PCB_H__


#include "pc_main.h"
#include <regex.h>


#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


/******************************************************************************
 *                                 MACRO                                      *
******************************************************************************/


/* 最大pcb数目定义 */
#define PC_MAX_PCB_NUM  255

/* 空指针定义 */
#define PC_LIST_NULL   0xff

/* 链表头定义 */
#define PC_LIST_HEAD   0xff



/******************************************************************************
 *                                 ENUM                                       *
******************************************************************************/


#define PC_FREE_REG(pstRegData, count) \
{ \
    long ii = count; \
    while (--ii >= 0) \
    { \
        regfree(pstRegData + ii); \
    } \
    free(pstRegData); \
}


/******************************************************************************
 *                                STRUCT                                      *
******************************************************************************/


/* pcb 的结构 */
typedef struct
{
    char acName[PC_PROC_NAME_LEN];
    unsigned long ulInstNo;

    char   **ppcArg;                 /* 启动参数列表 */
    char   **ppcMonCond;             /* 监控条件列表 */
    regex_t *pstRegData;             /* 编译后的正则表达式 */
    unsigned char    ucMonCount;             /* 监控条件个数 */
    unsigned char    ucIsCComProc;           /* 是否组件进程 */
    unsigned char    ucNeedSteady;           /* 是否需要持久化运行 */
    unsigned char    ucNeedMon;              /* 是否需要监控输出信息 */
    long     lPid;                   /* 进程的pid */
    long     lTty;                   /* 输出信息通信句柄 */

    /* char *pcLastLine; */          /* 最后一行片段, 暂时不考虑这种处理,
                                        因为没有好的办法判断最后一行是否结束,
                                        单纯用换行符来判断也不能兼容 fflush(stdout)
                                        的情况 */

    unsigned short   usComID;                /* 进程所属的组件id */
    unsigned char    ucLastSignal : 7;       /* 进程该次退出前所收到的信号 */
    unsigned char    ucNeedRestart : 1;      /* 是否需要重新拉起 */
    unsigned char    ucPrev;                 /* 前一个pcb的序号 */

#ifdef _PC_VER_R1
    unsigned char    ucVer;                  /* 版本信息, 为兼容旧消息格式而设 */
#endif

} ST_PC_PCB;


/******************************************************************************
 *                               GLOBAL VAR                                   *
******************************************************************************/



/******************************************************************************
 *                               FUNCTION                                     *
******************************************************************************/





/*************************************************************************
 Function:      PC_RET PC_PcbInit(void)
 Description:   Pcb管理模块初始化
 Calls:         无
 Data Accessed: g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Data Updated:
 Input:         无
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_PcbInit(void);

/*************************************************************************
 Function:      PC_RET PC_AllocPcb(ST_PC_PCB **ppstPcb, unsigned char *pucID)
 Description:   分配一个Pcb
 Calls:         无
 Data Accessed:
 Data Updated:  g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Input:         无
 Output:        ppstPcb, 分配到的pcb的地址
                pucID, 分配到的pcb的序号
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_AllocPcb(ST_PC_PCB **ppstPcb, unsigned char *pucID);

/*************************************************************************
 Function:      PC_RET PC_GetPcbByProcID(unsigned char ucID, ST_PC_PCB **ppstPcb)
 Description:   获取指定序号的pcb
 Calls:         无
 Data Accessed: g_ppstPcb
 Data Updated:
 Input:         ucID, 指定的序号
 Output:        ppstPcb, 获取到的pcb指针
 Return:        0,成功;
                其它, 失败
 Others:        当 ucID = PC_LIST_HEAD时, 表示取已经使用的第一个
 *************************************************************************/
PC_RET PC_GetPcbByProcID(unsigned char ucID, ST_PC_PCB **ppstPcb);

/*************************************************************************
 Function:      PC_RET PC_FreePcb(unsigned char ucID)
 Description:   释放pcb
 Calls:         无
 Data Accessed:
 Data Updated:  g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Input:         ucID, 指定的序号
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_FreePcb(unsigned char ucID);

/*************************************************************************
 Function:      PC_RET PC_GetPcbByPid(long lPid, ST_PC_PCB **ppstPcb)
 Description:   通过pid获取pcb
 Calls:         无
 Data Accessed: g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Data Updated:
 Input:         lPid, 指定的pid
 Output:        ppstPcb, 获取到的pcb指针
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_GetPcbByPid(long lPid, ST_PC_PCB **ppstPcb, unsigned char *pucID);

PC_RET PC_GetPcbByName(const char *pcName, unsigned long ulInstNo,
                       ST_PC_PCB **ppstPcb, unsigned char *pucID);


/*************************************************************************
 Function:      PC_RET PC_GetNextPcb(unsigned char ucCurID,
                           ST_PC_PCB **ppstPcb, unsigned char *pucID)
 Description:   获取下一个pcb, 这是对外提供的遍历所有正常使用的pcb的接口
 Calls:         无
 Data Accessed: g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Data Updated:
 Input:         ucCurID, 当前的pcb的序号
 Output:        ppstPcb, 获取到的下一个pcb指针
                pucID, 获取到的下一个pcb的序号
 Return:        0,成功;
                其它, 失败
 Others:        当ucCurID等于PC_LIST_HEAD时,表示获取第一个
 *************************************************************************/
PC_RET PC_GetNextPcb(unsigned char ucCurID, ST_PC_PCB **ppstPcb,
                                          unsigned char *pucID);


/*************************************************************************
 Function:      PC_RET PC_PcbFinal(void)
 Description:   Pcb管理模块去初始化
 Calls:         无
 Data Accessed: g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Data Updated:
 Input:         无
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_PcbFinal(void);


/*************************************************************************
 Function:      PC_RET PC_ChildFinal(void)
 Description:   Pcb所有子进程退出, 该函数是在PC收到 TER 和 INT 信号后的处理函数
 Calls:         无
 Data Accessed: g_ppstPcb,
 Data Updated:
 Input:         无
 Output:        无
 Return:        0,成功;
                其它, 失败
 Others:
 *************************************************************************/
PC_RET PC_ChildFinal(int sig);


/******************************************************************************
 *                                 END                                        *
******************************************************************************/


#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __PC_PCB_H__ */

