/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : upg_frw.h
 文件描述 : upg_frw 的头文件
 函数列表 :
 修订记录 :
          1 创建 : 李浩
            日期 : 2008-4-14
            描述 :

          2 创建 : 轩光磊
            日期 : 2009-2-23
            描述 : 增加返回错误码的共用体字段，以解决返回错
            	   码和输出信息不对应的问题

          3 创建 : 匡素文
            日期 : 2009-07-03
            描述 : 完善注释

**********************************************************************/
#ifndef __UPGFRW_H__
#define __UPGFRW_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "common.h"
#include "flash_layout.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/

/* 调试开关宏 */
#define UPG_DEBUG   1

#ifdef UPG_DEBUG
#define UPG_LogTrace(fmt, args...)      UPG_Log("%s:%4d %20s: " fmt, __FILE__, __LINE__, __func__, ##args)
#else
#define UPG_LogTrace(fmt, args...)      {do { ; } while(0);}
#endif

#ifdef CONFIG_DUAL_IMAGE
#ifndef CONFIG_DOUBLE_BACKUP
#define CONFIG_DOUBLE_BACKUP
#endif
#endif


/* 大小定义 */
#define BLOCKSIZE               1024*4
#define MAX_STR_LEN             256    /* 字符串最大长度*/

#define FIRMWARE_FILE           "/var/image.img"
#define LOGFILE                 "/var/upg_log.txt"

/* 升级文件路径 */
#define UPDATE_IMG              "/var/update_img"
#define UPDATE_BIN              "/var/update_bin"
#define UPG_ITEM_UPGRADING      "upgrading"


/* proc系统配置路径 */
#define IMG_SPACE               "/proc/llconfig/img_space"
#define FLASH_SIZE              "/proc/llconfig/flash_size"

/* 清除日志用到的节点 */
#define CFG_NODE_LEN 256
#define CLEAN_LOG_TRIGGER "InternetGatewayDevice.X_TWSZ-COM_Logger.LogClearTrigger"

/******************************************************************************
*                                ENUM
******************************************************************************/
/* 操作类型 */
enum
{
	NONE,
 	IMG_WRITE,
 	CONFIG_READ,
 	CONFIG_WRITE,
 	CONFIG_BAKUP
};


/* 文件类型 */
enum
{
        TYPE_BINARY,
        TYPE_TEXT
};

/* 可变参数定义 */
typedef char *va_list;


/******************************************************************************
*                                STRUCT
******************************************************************************/



/******************************************************************************
*                                FUNCTION
******************************************************************************/
/* API Functions */
int UPG_UpdateImg(char * filename, BOOL bClearCfg, BOOL bClearLog, unsigned short sMID);
int UPG_UpdateConfig(char * filename, unsigned short sMID);
int UPG_BackupConfig(char * filename,unsigned short sMID);
int UPG_ClearConfig(void);

int UPG_ModuleInit(void);
int UPG_ModuleDestory(void);

/* Utils Funtions */
int UPG_UpgradeSystem(void);
int UPG_SendSimpleMsg(unsigned short usDstMid, unsigned short usMsgType);
int UPG_SendSetMsg(int nValNum, char aszName[][CFG_NODE_LEN], char aszVal[][CFG_NODE_LEN]);
int UPG_ProcessAckMsg(void);
int UPG_ReplyStateToCaller(unsigned long ulResult, unsigned short sMID);

int UPG_CheckValid(char *filename);
int UPG_CheckSum(char * filename);
void UPG_Log(char *szFmt, ...);

#endif

