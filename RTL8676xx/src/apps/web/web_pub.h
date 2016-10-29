/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : web_pub.h
 文件描述 : WEB服务器部分的公共信息头文件

 函数列表 :


 修订记录 :
          1 创建 : 刘泽群 / 2007-8-21
            日期 :
            描述 :

**********************************************************************/
#ifndef _WEB_PUB_H_
#define _WEB_PUB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../omlib/om_buffer.h"
#include "../omlib/om_nameval.h"

#include "../include/om.h"

#include "web_stub.h"
#include "web_utils.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
//调试开关
//#define WEBPROC_DEBUG

#ifdef WEBPROC_DEBUG
#define WEB_TRACE(fmt, args...) WEB_LogTrace("%s:%4d %20s: "fmt, __FILE__, __LINE__, __func__, ##args)
#else
#define WEB_TRACE(fmt, args...) do{}while(0)
#endif

#ifdef CONFIG_APPS_LOGIC_NEW3G
extern FILE *cfp;
extern unsigned long g_ulIsNew3g;
extern unsigned long g_ulTagMid;

#ifdef d_printf
#undef d_printf
#endif

#define d_printf(fmt, args...) \
	do{\
		if (cfp <= 0) \
			cfp = fdopen(open("/dev/console", O_RDWR), "w");\
		if (cfp > 0)	\
                        if (access("/var/3g_debug_wan", F_OK) == 0)   \
			    fprintf(cfp, "[WEBPROC] %s:%d "fmt, __FILE__, __LINE__, ##args);\
	}while(0)

#ifdef WEBPROC_DEBUG
#undef  WEB_TRACE
#define WEB_TRACE(fmt, args...) d_printf("%s:%4d %20s: "fmt, __FILE__, __LINE__, __func__, ##args)
#endif

#else
#ifdef d_printf
#undef d_printf
#endif
#define d_printf(fmt, args...) 
#endif


#define WEB_FUNC_IN(fmt, args...)   WEB_TRACE("++I"fmt"\n", ##args)
#define WEB_FUNC_OUT()              WEB_TRACE("--O\n")



#define  WEB_LEN_TEXT_BUF    (1024*24)      /* 文本缓冲，每次解析的最大文本长度 */
#define  WEB_NUM_PARAM       10        /* 每一条命令的最大参数个数 */
#define  WEB_MAX_CMD_DEPTH   5         /* 允许的最大嵌套数量 */
#define  WEB_TIMEOUT_RESP    10        /* 响应的超时时间 */
#define  WEB_MAX_VAL_PER_OBJ 32        /* 每个对象可以包含的参数，这个类容限制到 */
#define  WEB_MAX_OBJ_LEN     512       /* 每个对象的长度 */
#define  WEB_MAX_OBJ_NUM     260       /* 每个对象的长度 */

#define  WEB_NUM_STRING      16        /* 一个数字串的最大长度 */

#define  WEB_STRUCT_MAGIC    0x01105AA5

#define Name2MID safe_atol
/*------------常用配置-----------------*/
#define WEB_NEED_MARK     1
/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
/* 响应消息的检查结果 */
enum en_RESPONSE_STATE
{
    EN_RESPONSE_SUCCESS,    /* 结果OK */
    EN_RESPONSE_ERROR,      /* 结果错误 */
    EN_RESPONSE_TIMEOUT,    /* 等待超时 */
    EN_RESPONSE_UNREPECT,   /* 非预期消息 */

    EN_RESPONSE_BUTT
};


#define WEB_OutToTerm  printf

/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/


/* 本模块定义的值*/
typedef struct tag_ST_WEB_VALUE
{
    struct tag_ST_WEB_VALUE *pNext;
    struct tag_ST_WEB_VALUE *pPrev;
    int                      nCount;
    char                    *value[WEB_MAX_VAL_PER_OBJ];
}ST_WEB_VALUE;

/* POST信息 */
typedef struct tag_ST_POST_INFO
{
    unsigned long   ulMagic;        /* 正确初始化为 WEB_STRUCT_MAGIC*/
    int			    nPostNum;       /* POST数量 */

    unsigned long   ulStatus;       /* 状态值 */
    BOOL            bPostError;     /* 是否出错*/
    char		   *szPostResult;
}ST_POST_INFO, *PST_POST_INFO;

/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/


/* 设置一项变量， 相当于 szName = szvalue */
void WEB_SetVar(char *szName, char *szValue);
/* 获取一项变量 */
ST_NAME_VAL * WEB_GetVar(char *szName);

/* 文本解析入口，返回解析的实际长度 */
int WEB_ParseText (char *pText, int nSize);

/* 获取响应, 返回值取值于en_RESPONSE_STATE */
int  WEB_GetResponse();

/* POSTINFO的清除函数*/
void WEB_PostInfoClean();

/* 全局资源清理函数 */
void Web_CleanUp();


#if WEB_NEED_MARK
    /* 进入新文件 */
    void WEB_MarkFileIn(PCSTR strFile);
    /* 离开文件 */
    void WEB_MarkFileOut();
    /* 清除资源 */
    void WEB_MarkClearUp();
    /* 进入新地点 */
    void WEB_MarkGotoPos(int nPos, PCSTR strCmd, int nLen);
    /* 累加已解析内容 */
    void WEB_MarkPosIncr(int nCount);
    /* 错误提示 */
    #define WEB_MarkError(str) WEB_MarkErrorEx(__FILE__, __LINE__, str);
    void WEB_MarkErrorEx(PCSTR szFile, int nLine, PCSTR strError);
    /* 是否产生错误 */
    BOOL WEB_MarkIsError();
#else
    #define  WEB_MarkFileIn(...)
    #define  WEB_MarkFileOut(...)
    #define  WEB_MarkClearUp(...)
    #define  WEB_MarkGotoPos(...)
    #define  WEB_MarkPosIncr(...)
    #define  WEB_MarkError(...)
    #define  WEB_MarkIsError() FALSE
#endif


/******************************************************************************
 *                           PUB EXTERN VAR                                   *
 ******************************************************************************/

extern ST_NAME_VAL        *g_pstWebVars;
extern ST_NAME_VAL        *g_pstNetVars;
extern ST_NAME_VAL        *g_pstPostVars;
extern ST_POST_INFO        g_stPostInfo;
extern unsigned long       g_ulMsgSerial;


//int read(int, void *, unsigned int);
//int open(const char *, int, ...);
//int close(int);
//char * getcwd(char *, int);
//int write(int, const void *, unsigned int);


/* 安全的释放消息*/
#define safe_free_msg(pointer) \
    { \
        if ( pointer ) \
        { \
            MSG_ReleaseMessage(pointer); \
            pointer = NULL; \
        } \
    }


/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/




#endif /* _WEB_PUB_H_ */
