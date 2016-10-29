/******************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: logger_sysevt.h
 文件描述:
           logmonitor
 修订记录:
        1. 作者: WeiChongxu@twsz.com
           日期:
           内容: 创建

        2. 作者: HuangJidong@twsz.com
           日期: 20081219
           内容: 重构

******************************************************************/
#ifndef _LOGGER_SYSEVT_H_
#define _LOGGER_SYSEVT_H_

#include "logger.h"


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

#define LOGGER_SRC_MIN_LEN_K 40
#define LOGGER_SRC_MAX_LEN_K 4096

#define LOGGER_DST_MIN_LEN_K 40
#define LOGGER_DST_MAX_LEN_K 128

#define LOGGER_SRC_MIN_LEN    (LOGGER_SRC_MIN_LEN_K*1024)
#define LOGGER_SRC_MAX_LEN    (LOGGER_SRC_MAX_LEN_K*1024)

#define LOGGER_DST_MIN_LEN    (LOGGER_DST_MIN_LEN_K*1024)
#define LOGGER_DST_MAX_LEN    (LOGGER_DST_MAX_LEN_K*1024)

#define LOGGER_PROG_NAME    "logmonitor"

#define LOGGER_LOCKFILE     "/var/run/logmonitor.pid"
#define LOCKMODE            (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

#define LOGGER_USAGE        \
    "\nUsage:" \
    "\n" LOGGER_PROG_NAME " sourcefile uncompmaxlen destfile compmaxlen" \
    "\n      sourcefile: Original log file." \
    "\n    uncompmaxlen: Maxmum size of orignal log file, in KBytes, %d ~ %d KBytes." \
    "\n                  sourcefile will be truncated to uncompmaxlen if it exceed uncompmaxlen x 1.2" \
    "\n        destfile: Destnation of compressed log file. It will be created if no such file exists." \
    "\n      compmaxlen: Maxmum size of destfile, in KBytes, %d ~ %d KBytes.\n", \
    LOGGER_SRC_MIN_LEN_K, LOGGER_SRC_MAX_LEN_K, LOGGER_DST_MIN_LEN_K, LOGGER_DST_MAX_LEN_K

#define LOGGER_MONITOR_INTERVAL 60

/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/

int main(int argc, const char *argv[]);

int LOGGER_CompWrite(const char *pszData, unsigned long ulDataLen, const char *pszDstFile, unsigned long ulDstLen, int iFlags);
int LOGGER_CheckGlobalVar(void);
int LOGGER_CheckLenRange(long lValue, long lMin, long lMax);
int LOGGER_CheckDigitRange(const char *pszString, int iMin, int iMax);
#if defined(LOGGER_DIRECT_READ_FLASH) || defined(LOGGER_DEBUG)
ssize_t LOGGER_SafeWrite(int fd, const void *buf, size_t count);
ssize_t LOGGER_FullWrite(int fd, const void *buf, size_t len);
int LOGGER_WriteToFile(const char *pszPath, const char *pszBuf, unsigned long ulLen, int iFlags);
#endif
void LOGGER_DoRecord(int sig);
void LOGGER_ShowUsage(void);
void LOGGER_QuitSignal(int sig);
int LOGGER_AlreadyRunning(void);
int LOGGER_LockFile(int iFD);
unsigned long LOGGER_SkipDevInfo(const char *pszData, unsigned long ulDataLen);
int LOGGER_PutDevInfo(int iDstFD);

/******************************************************************************
*                                 END                                        *
******************************************************************************/

#endif
