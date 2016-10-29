/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : web_cmd.h
 文件描述 : WEB 模块私有命令解析处理头文件


 函数列表 :


 修订记录 :
          1 创建 : 刘泽群 / 2007-8-21
            日期 :
            描述 :

**********************************************************************/
#ifndef _WEB_CMD_H_
#define _WEB_CMD_H_

#include "web_pub.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/


#define WEBCMD_START_TOKEN  "<?"
#define WEBCMD_END_TOKEN    "?>"
#define WEBCMD_VAR_TOKEN    '$'
#define WEBCMD_MODULE_TOKEN ':'
#define WEBCMD_QUOTE_BACK   '`'
#define WEBCMD_QUOTE_SINGLE '\''
#define WEBCMD_QUOTE_DOUBLE '\"'

#define WEBCMD_UNFIND_VAL   "-"
#define WEBCMD_VAL_TOKEN    "var:"
#define WEBCMD_LEAF_START   ".n("       // 是一个object.name的结构
#define WEBCMD_LEAF_END     ')'         // 是一个object.name的结构
#define WEBCMD_LEAF_NULL    ".n()"      // 是一个object.name的空结构

/* 命令字的宏定义 */
#define WEB_CMD_COMMENT         "--"
#define WEB_CMD_ECHO            "echo"
#define WEB_CMD_SET_VAR         "setvar"
#define WEB_CMD_SET_VAR_ONCE    "setvaronce"
#define WEB_CMD_QUERY           "query"
#define WEB_CMD_INCLUDE         "include"
#define WEB_CMD_IF              "if"
#define WEB_CMD_ERROR           "error"
#define WEB_CMD_MULTI_QUERY     "multiquery"
#define WEB_CMD_MULTI_QUERY_N   "multiquerycount"
#define WEB_CMD_OBJ_QUERY       "objget"
#define WEB_CMD_GET             "get"
#define WEB_CMD_MULTI_GET       "mget"


#define WEBCMD_TOKEN_LEN    2   /* strlen(WEBCMD_START_TOKEN),*/
                                /* 起始符变化, 该定义也要变化*/

#define WEB_IS_SEPARATOR(c)    (c == 0x20 || c == '\r' || c == '\n' || c == '\t')

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/



enum en_QUOTE_TYPE
{
    EN_QUOTE_NONE,      /* 无引号*/
    EN_QUOTE_SINGLE,    /* 单引号*/
    EN_QUOTE_DOUBLE,    /* 双引号*/
    EN_QUOTE_BACK,      /* 后引号*/

    EN_QUOTE_BUTT
};


/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/


/* 命令结构体*/
typedef struct tag_ST_WEBCMD_INFO
{
    struct tag_ST_WEBCMD_INFO  *pParent;            /* 父命令*/
    int                         nDepth;             /* 命令的嵌套深度*/
    ST_WEB_VALUE               *pstValCur;
    int                         nValIndex;

    int                         nParam;
    ST_OM_BLOCK                *apParam[WEB_NUM_PARAM];/*ST_OM_BLOCK定义为带长度的缓冲*/
}ST_WEBCMD_INFO;




/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/

int WEB_GetObjReplace(char *pBuffer, char szValMap[WEB_MAX_VAL_PER_OBJ][WEB_MAX_OBJ_LEN]);

/* 向父命令回朔指定层数的命令结构 */
ST_WEBCMD_INFO *WEB_FindBackeLvelCmdInfo(int nBackLevel,
            ST_WEBCMD_INFO *pstCmdInfo);

/* 将给定的串中的变量进行替换 */
int    WEB_VarReplace(ST_OM_BLOCK *pBlockBuff, ST_WEBCMD_INFO *pstCmdInfo);

/* 提取命令中的所有参数并加入命令结构的参数列表 */
void WEB_GetArgMap(char *szStart, int size, ST_WEBCMD_INFO *pstCmdInfo);

/* echo命令解析函数 */
void    WEB_CmdEcho(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);

/* setvar命令解析函数 */
void    WEB_CmdSetVar(ST_WEBCMD_INFO *pstCmdInfo, BOOL bReset);

/* include命令解析 */
void    WEB_CmdInclude(ST_WEBCMD_INFO *pstCmdInfo);

/* 各类查询命令的解析 */
void    WEB_CmdQuery(ST_WEBCMD_INFO *pstCmdInfo, BOOL bSingle,
                           BOOL bGetcount,  ST_OM_BLOCK *pstOutBuff);

/* 单个节点值查询命令的解析 */
void    WEB_CmdValueGet(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);
/* 多个节点值查询命令的解析 */
void    WEB_CmdMultiGet(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);
/* 对象节点查询命令的解析 */
void    WEB_CmdObjGet(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);

/* if条件命令的解析 */
void    WEB_CmdIf(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);

/* error命令解析 */
void    WEB_CmdError(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);

/* 文本串命令的解析函数入口 */
int WEB_ParseCmd(char *szStart, int nSize,
                        ST_WEBCMD_INFO *pParent, ST_OM_BLOCK *pstOutBuff);

/* 文本解析的实现函数入口 */
int    WEB_ParseString(char *szStart, int nSize, ST_WEBCMD_INFO *pCmdParent,
                       ST_OM_BLOCK *pstOutBuff);
/* 文本解析函数 */
int WEB_ParseText (char *szText, int nSize);


/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/


#endif /* __WEB_CMD_H__ */


