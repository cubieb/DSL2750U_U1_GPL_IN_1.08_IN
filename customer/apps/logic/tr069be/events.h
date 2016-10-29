/**********************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

  文件名称: events.h
  文件描述: events维护一个先进先出的队列。队列单个元素包括event类型名和event的commandkey。
  修订记录:
         1. 作者: chenzhuo
            日期: 2007-11-13
            内容: 创建文件

         2. 作者: huangjidong
            日期: 2008-11-13
            内容: 重构,删除和替换一些新架构用不到的代码

**********************************************************************/

#ifndef _EVENTS_H_
#define _EVENTS_H_

#include "tr069.h"

/*
 * 定义和events处理相关的宏及接口函数
 */
#define BOOTSTRAP_0	        "0 BOOTSTRAP"
#define BOOT_1	            "1 BOOT"
#define PERIODIC_2          "2 PERIODIC"
#define SCHEDULED_3         "3 SCHEDULED"
#define VALUECHANGE_4       "4 VALUE CHANGE"
#define CONNECTIONREQUEST_6 "6 CONNECTION REQUEST"
#define TRANSFERCOMPLETE_7  "7 TRANSFER COMPLETE"
#define DIAGNOSTICSTATE_8	"8 DIAGNOSTICS COMPLETE"
#define REBOOT_M            "M Reboot"
#define DOWNLOAD_M          "M Download"
#define UPLOAD_M            "M Upload"
#define SCHEDULEINFORM_M    "M ScheduleInform"

#define TR069_CFG_EVENT "InternetGatewayDevice.ManagementServer.X_TWSZ-COM_Events"

#define IS_MULTI_EVENT(x) (('M'==x[0])?1:0)


int TR069_AddEvent(char* szEvent, char* szCommandKey);
int TR069_ClearEvents (void);
int TR069_ReportEvents(ST_MSG* pstMsg);
int TR069_FindEvent (char *szEvent);
int TR069_DeleteEvent(char *szEvent);

#endif

