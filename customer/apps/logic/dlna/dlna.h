/********************************************************************
  Copyright (c), 1991-2010, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
  
  文件名称：dlna.h
  文件描述：DLNA模块头文件
  修订记录：
         1. 作者：chenkai
            日期：2011-03-14
            内容：创建文件
********************************************************************/

#ifndef __DLNA_H__
#define __DLNA_H__

#include "common_msg_handler.h"
#include "warnlog.h"


/******************************************************************************
*                                 MACRO                                       *
******************************************************************************/
/* 常量定义*/

#define BASE_PATH_DLNA "^InternetGatewayDevice\\.X_TWSZ-COM_DLNA\\."
#define PATH_DLNA      "InternetGatewayDevice.X_TWSZ-COM_DLNA."


//#define NSM_DEBUG         /*调试开关*/

#ifdef  DLNA_DEBUG
#define DLNA_TRACE(fmt, args...)        TBS_TRACE(MID_DLNA, fmt, ##args)
#else
#define DLNA_TRACE(fmt, args...)
#endif

/* 错误日志 */
#ifdef  DLNA_DEBUG
#define DLNA_ERROR(fmt, args...)        printf("DLNA:__%d__"fmt, __LINE__, ##args)
#else
#define DLNA_ERROR(fmt, args...)        COMMON_LOG_ERROR(MID_DLNA, fmt, ##args)
#endif


/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/


/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/


int DLNA_ModuleInit(void);
int DLNA_ModuleDestroy(void);

/******************************************************************************
*                                 END                                        *
******************************************************************************/

#endif

