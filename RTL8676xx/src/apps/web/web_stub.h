/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : EB_Stub.h
 文件描述 : WEB模块桩函数


 函数列表 :


 修订记录 :
          1 创建 : 刘泽群 / 2007-8-22
            日期 :
            描述 :

**********************************************************************/

#include "../include/om.h"

#ifdef WIN32
#include <windows.h>
#else
#include <errno.h>
#endif

#ifdef WIN32
#define kill(...)
#define snprintf _snprintf
#define sleep(d) Sleep(d*1000)
#define EAGAIN 11
#endif /* WIN32 */

#define syslog(err, str) printf(str)
#define LOG_EMERG	0
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERR		3
#define LOG_WARNING	4
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7

#define WEB_ASSERT(ex)
#define ERR_OUT_OF_MEM()

#define WEB_TIME_STR_LEN    24

#ifndef isspace
#define isspace(ch) (ch == 0x20)
#endif
#ifndef isdigit
#define isdigit(ch) (ch >= '0' && ch <= '9')
#endif /* isdigit */
#ifndef isalnum
#define isalnum(ch) (isdigit(ch) || (ch >= 'a' && ch <= 'z') )
#endif /* isalnum */

#define STDOUT_FILENO 1
#ifndef SIGKILL
#define SIGKILL 0
#endif
#define O_RDONLY 00
#define O_NOCTTY 0400

#define WEB_MY_PID  5

#define WEB_EXIT(n) WEB_ProcExit(n, __FILE__, __LINE__);

#ifdef WIN32
int    safe_strcmp(const char *s1, const char *s2);
int    safe_strncmp(const char *s1, const char *s2, int n);
int safe_strlen(const char *s);
char *strchr(    const char     *s,    int        c);
#endif /* WIN32 */
char *strchrR(char	*s,	int	c);
char *valid_string(char *s);
/*char *strtok(char  *s1, char *s2); */
/*char *strdup(const char *str); */
char *strndup(register const char *s1, int n);
#ifdef WIN32
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, int n);
#endif /* WIN32 */
char *My_getenv(char *strEnv);
int My_ReadStd(char *pszReqContent, int nReqLength);

void WEB_ProcExit(int nRetCode, char *szFile, int nLine);

void My_getCurrentTime(char* currentTime);





