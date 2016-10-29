/******************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: DynModuleCtl.c
 文件描述:

 修订记录:
        1. 作者: 欧军和, oujunhe@twsz.com
           日期: 2009年05月31日
           内容: Create

******************************************************************/

#ifndef __DYN_MODULE_CTRL_H__
#define __DYN_MODULE_CTRL_H__

#include "common_msg_handler.h"
#include "warnlog.h"


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define NO_FREE_ITEM -1
#define ITEM_NOT_EXIST -1
#define IN_PARAMETER_POINTER_NULL -2
#define GET_NODE_VALUE_FAILE    -3
#define UNEXCEPT_RESULT    -4
#define FIND_MSG_NO_ITEM -5
#define DYN_CTRL_INFO_LIST_FULL -6
#define MODULE_NO_REGISTERED -7
#define OPEN_DYNAMIC_LIBRARY_FAIL -8
#define CLOSE_DYNAMIC_LIBRARY_FAIL -9
#define LIBRARY_NOT_LOADED -10
#define LIBRARY_LOADED -11

#define MAX_CARE 20


#define DynModuleCtl_TRACE(fmt, args...)        TBS_TRACE(MID_DYN_MODULE_CTRL, fmt, ##args)


/******************************************************************************
*                               STRUCT                                     *
******************************************************************************/


struct DynCtrlInfo
{
    unsigned short usMID;
    unsigned short ucIsModuleAlive;
    int iEnable;
    int iTimerScanCount;
    void *pDynLinkHandle;
    char szModuleName[64];
    char szEnablePath[256];
};
/*
usMID: 模块MID
ucIsModuleAlive:
        0: 未加载
        1:已经加载
        大于1:无法加载
        默认: 0
iEnable:用于记录模块是enabled的还是disabled的
iTimerScanCount：定时器扫描的次数。在有SET消息时置0，定时器扫描时增1，
        大于5时模块会被卸载
pDynLinkHandle:
        动态链接库的句柄
szModuleName:模块名字，用以构造模块的动态库名、初始化函数名、
        销毁函数名
szEnablePath:
        enable节点的路径
*/


/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/

int	        DynModuleCtl_RequestDynCtrl(struct DynCtrlInfo* pstDynCtrlInfo);
int         DynModuleCtl_LoadModule(unsigned short usMID);
int         DynModuleCtl_UnloadModule(unsigned short usMID);

int         DynModuleCtl_GetModuleState(unsigned short usMID);
int         DynModuleCtl_GetEnablePath(unsigned short usMID, char* pBuf, int iBufSize);
int         DynModuleCtl_SetModuleState(unsigned short usMID, unsigned short usNewState);
void*       DynModuleCtl_GetModuleInitFun(unsigned short usMID);
void*       DynModuleCtl_GetModuleDestroyFun(unsigned short usMID);
int         DynModuleCtl_SetModuleEnable(unsigned short usMID, int iNewEnableState);
int         DynModuleCtl_TimerScanCountIncrease(unsigned short usMID);
int         DynModuleCtl_TimerScanCountInit(unsigned short usMID);
int         DynModuleCtl_GetTimerScanCount(unsigned short usMID);
int         DynModuleCtl_GetModuleEnableState(unsigned short usMID);
void        DynModuleCtl_DealModuleDynCtrl(void);

int         DynModuleCtl_ModuleInit(void);
int         DynModuleCtl_ModuleDestroy(void);
void        DynModuleCtl_MSGHandle(ST_MSG * pstMSG);


/******************************************************************************
*                                 END                                        *
******************************************************************************/

#endif

