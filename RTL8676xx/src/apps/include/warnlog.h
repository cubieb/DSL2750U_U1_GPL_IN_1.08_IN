/******************************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

  文件名称: warnlog.h
  文件描述: 告警日志。
 ******************************************************************************/

#ifndef __WARNLOG_H__
#define __WARNLOG_H__

#include <syslog.h>
#include "autoconf.h"

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

#ifdef CONFIG_TBS_APPS_DEBUG
#define COMMON_TRACE(mid, fmt, args...) \
    do {  \
        char *pszModName = #mid;  \
        if (strstr(pszModName, "MID_")) { \
            pszModName += 4; \
            printf("[%s] %s:%4d %20s: " fmt, pszModName, __FILE__, __LINE__, __func__, ##args); \
        } else { \
            printf("[%04x] %s:%4d %20s: " fmt, mid, __FILE__, __LINE__, __func__, ##args); \
        } \
    } while(0)

#define TBS_TRACE(mid, fmt, args...) \
    printf("[%04x] %s:%4d %20s: " fmt, mid, __FILE__, __LINE__, __func__, ##args)
        
#else
#define COMMON_TRACE(mid, fmt, args...)
#define TBS_TRACE(mid, fmt, args...)
#endif

#ifdef CONFIG_TBS_APPS_DEBUG
#define COMMON_LOG_ERROR(mid,fmt, args...) \
        WARN_LOG(WARN_LEVEL_ERR, mid, "%s:%4d %20s: " fmt, __FILE__, __LINE__, __func__, ##args)
#else
#define COMMON_LOG_ERROR(mid,fmt, args...)
#endif

/* MID 转换 */
#define MID_DEFAULT MID_NULL

int WARN_setLevel(unsigned short usMID, unsigned char ucLevel);
void WARN_LOG(unsigned char ucLevel, unsigned short usMID, char *fmt, ...);
void TBS_Log(unsigned char ucSeverity, unsigned short usMID,
    const char *pcAccessor, const char *pcMethod, const char *pcPara,
    const char *pcResult, const char *pcRemark, ...);

#endif

