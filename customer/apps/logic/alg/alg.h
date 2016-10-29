/******************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: alg.h
 文件描述:

 修订记录:
        1. 作者: Annan Li (madbug), lianan@twsz.com
           日期: 2008年03月17日
           内容: Create

        2. 作者: Huangjidong@twsz.com
           日期: 2008.11.6
           内容: 重构

******************************************************************/

#ifndef __ALG_H__
#define __ALG_H__

#include "common_msg_handler.h"
#include "warnlog.h"
#include "ipt_common.h"


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

#define BASE_PATH_ALG       "^InternetGatewayDevice\\.Services\\.X_TWSZ-COM_ALGAbility\\."
#define PATH_ALG            "InternetGatewayDevice.Services.X_TWSZ-COM_ALGAbility."

#define PATH_LAN_DEVNAME          "InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_DeviceName"
#define MAX_ALG_NAME_LEN    10
#define SIP_CONFIG_NAME     "/etc/siproxd.conf"
#define SIP_CONFIG_TPLT     "/etc/siproxd.tplt"
#define SIP_PROC_PATH       "/sbin/"
#define SIP_PROC_NAME       "siproxd"
#define NULL2EMPTY(arg)    ((arg)?(arg):(""))
#define SIP_REMOTE_INPUT    CN_ACL_REMOTE_INPUT

#undef  ALG_IPSEC


#ifdef  ALG_DEBUG
#define ALG_TRACE(fmt, args...)        TBS_TRACE(MID_ALG, fmt, ##args)
#else
#define ALG_TRACE(fmt, args...)
#endif


/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/

int ALG_FtpCommitHandler();
int ALG_TftpCommitHandler();
int ALG_RtspCommitHandler();
int ALG_PptpCommitHandler();
int ALG_L2tpCommitHandler();
int ALG_H323CommitHandler();
int ALG_SIPCommitHandler();
int ALG_IPSECCommitHandler();

int ALG_ModuleInit();
int ALG_ModuleDestroy();

/******************************************************************************
*                                 END                                        *
******************************************************************************/

#endif

