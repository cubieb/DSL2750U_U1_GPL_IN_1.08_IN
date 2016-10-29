/**********************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: upload.h
 文件描述:

 修订记录:
        1. 作者: chenzhuo
           日期: 2008-05-19
           内容: 创建函数

        2. 作者: huangjidong
           日期: 2008-11-13
           内容: TR069BE重构添加注释

**********************************************************************/
#ifndef _UPLOAD_H_
#define _UPLOAD_H_

#include "common.h"


enum EN_TR069_UpLoadState {ulIdle, ulProcess, ulDelay, ulInformWait };
#define DOWNLOADDIAG_PATH_DOWNLOADURL "InternetGatewayDevice.DownloadDiagnostics.DownloadURL"
/*
 * upload.c 中函数定义
 */

int TR069_UploadMethod(ST_MSG *pstMsg) ;
int TR069_DelayedUpload(ST_MSG *pstMsg, const int iFlag ) ;
int TR069_FirmwareUpload(char *pcBaseUrl, char *pcUserName, char *pcPassword) ;
int TR069_UploadDone(ST_MSG *pstMsg) ;
int TR069_PrintUploadMsg(ST_MSG *pstMsg) ;
int TR069_SendUploadResponse (ST_MSG *pstMsg, int iStatus);
int TR069_GetCfgFileAddress();
int TR069_GetCfgFile(ST_MSG* pstMsg);
int TR069_UploadProcCtrl(const char *pcName, unsigned long ulInstNo, unsigned char ucOpt);


#endif

