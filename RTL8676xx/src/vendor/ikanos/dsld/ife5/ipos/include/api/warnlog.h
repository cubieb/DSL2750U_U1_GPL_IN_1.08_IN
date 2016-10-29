/******************************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

  文件名称: warnlog.h
  文件描述: 告警日志。
  函数列表: WARN_LOG
  修订记录:
         1. 作者: 武萌
            日期: 2007-10-15
            内容: 创建文件
         2. 作者: Lvwz
            日期: 2007-11-30
            内容: 修改日志写入syslog中，并增加TBS_TRACE用于开发时DEBUG
 ******************************************************************************/

#ifndef __WARNLOG_H__
#define __WARNLOG_H__

#include <syslog.h>

typedef enum tag_EN_WARNLEVEL
{
    WARN_LEVEL_EMERG = LOG_EMERG,
    WARN_LEVEL_ALERT = LOG_ALERT,
    WARN_LEVEL_CRIT = LOG_CRIT,
    WARN_LEVEL_ERR = LOG_ERR,
    WARN_LEVEL_WARNING = LOG_WARNING,
    WARN_LEVEL_NOTICE = LOG_NOTICE,
    WARN_LEVEL_INFO = LOG_INFO,
    WARN_LEVEL_DEBUG = LOG_DEBUG
} EN_WARNLEVEL;

typedef struct tag_ST_MODCFG
{
    unsigned short usMID;
    unsigned char ucLevel;
    unsigned char ucPad;
} ST_MODCFG;

#if 1
/* 将日志打印到syslog */
#define TBS_TRACE(mid, fmt, args...) \
    WARN_LOG(LOG_DEBUG, mid, "%s:%25s:%4d "fmt, \
             __FILE__, __func__, __LINE__, ##args);
#else
/* 将日志打印到stdout */
#define TBS_TRACE(mid, fmt, args...) \
    printf("%04x %s:%25s:%4d "fmt, \
            mid, __FILE__, __func__, __LINE__, ##args);
#endif

/* MID 转换 */
#define MID_DEFAULT MID_NULL

int WARN_setLevel(unsigned short usMID, unsigned char ucLevel);
void WARN_LOG(unsigned char ucLevel, unsigned short usMID, char *fmt, ...);
void TBS_Log(unsigned char ucSeverity, unsigned short usMID,
    const char *pcAccessor, const char *pcMethod, const char *pcPara,
    const char *pcResult, const char *pcRemark, ...);

#endif

