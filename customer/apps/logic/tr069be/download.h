/**********************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

  文件名称: download.h
  文件描述: TR069模块中下载子模块

  修订记录:
         1. 作者: chenzhuo
            日期: 2007-11-13
            内容: 创建文件

         2. 作者: huangjidong
            日期: 2008-11-13
            内容: 重构

**********************************************************************/
#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#include "common.h"
#define NEW3G_DRIVER_NODE	"InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.driver"
#define DOWNLOADDIAG_PATH_DOWNLOADURL "InternetGatewayDevice.DownloadDiagnostics.DownloadURL"
enum EN_TR069_DownLoadState {dlIdle, dlProcess, dlDelay, dlUpgWait };

/*
 * download.c 中函数定义
 */
char* TR069_GetArgument(char * pcArguments, int iNumber);
char* TR069_MakeUrl(char *pcBaseUrl, char *pcUserName, char *pcPassword);

int TR069_DownloadMethod(ST_MSG *pstMsg) ;
int TR069_DelayedDownload(ST_MSG *pstMsg, const int iFlag ) ;
int TR069_FirmwareDownload(char *pcBaseUrl, char *pcUserName, char *pcPassword) ;
int TR069_DownloadDone(ST_MSG *pstMsg) ;
int TR069_PrintDownloadMsg(ST_MSG *pstMsg) ;
int TR069_SendDownloadResponse (ST_MSG *pstMsg, int iStatus);
int TR069_DownloadProcCtrl(const char *pcName, unsigned long ulInstNo, unsigned char ucOpt);


#endif


