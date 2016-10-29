/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : btn_app.h
 文件描述 : 本文件为应用层提供监听按钮消息所需要的各种定义
 函数列表 :


 修订记录 :
          1 创建 : 张喻
            日期 : 2008-11-05
            描述 :

**********************************************************************/
#ifndef CM_BTN_H
#define CM_BTN_H

#include <btn.h>
#include "warnlog.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/

/* 调试信息宏 */
#define BTN_DEBUG

#ifdef BTN_DEBUG
#define BTN_TRACE(fmt, args...)  COMMON_TRACE(MID_LBT, fmt, ##args)
#else
#define BTN_TRACE(fmt, args...)
#endif

#define MAX_MSGSIZE	    1024
#define BLP_CMM_GROUP   0


/******************************************************************************
*                                 ENUM
******************************************************************************/
/* 按钮模式 */
typedef enum {
	CFG_APP_BTN_MODE_UNKNOW = 0,
	CFG_APP_BTN_MODE_DIRECT,
	CFG_APP_BTN_MODE_RELEASE_LESS,
	CFG_APP_BTN_MODE_RELEASE_GREATER,
	CFG_APP_BTN_MODE_TIMER,
	CFG_APP_BTN_MODE_INTERVAL,
}APP_BTN_MODE;

/* 按钮事件 */
typedef enum  {
	APP_BTN_EVEN_DOWN, 		/*通用消息:用于CFG_APP_BTN_MODE_DIRECT模式*/
	APP_BTN_EVEN_UP,		/*通用消息:用于CFG_APP_BTN_MODE_DIRECT模式*/
	APP_BTN_EVEN_REBOOT,	/*自定义消息:用于reset按钮*/
	APP_BTN_EVEN_RESET,		/*自定义消息:用于reset按钮*/
	APP_BTN_EVEN_WPS,
	APP_BTN_EVEN_WLAN,
	APP_BTN_EVEN_WPS_SWITCH,
}APP_BTN_EVEN;

struct APP_BTN_MSG {
	btn_name Name;			/* btn_name 定义在底层btn.h */
	APP_BTN_EVEN Even;
};

struct APP_BTN_CFG {
	btn_name Name;
	APP_BTN_MODE Mode;
	int Time;
	int EndTime;                    /**/
	APP_BTN_EVEN Even;		/*自定义消*/
};


/******************************************************************************
*                               FUNCTION DECLARE                              *
******************************************************************************/
/* 初始化BTN */
int BTN_ModuleInit(void);

/* 注销BTN */
void BTN_ModuleDestory(void);

/* BTN处理主程序 */
int BTN_MainProc(void);

/* BTN计时处理 */
int BTN_procTick(void);

/* 获取BTN监听的NETLINK套接字 */
int BTN_GetMsgSock(void);


#endif
