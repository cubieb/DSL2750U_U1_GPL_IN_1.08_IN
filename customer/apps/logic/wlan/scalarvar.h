#ifndef _SCALARVAR_H_
#define _SCALARVAR_H_
#include "tbstype.h"

#define SYNED 0     /* 数据已经与配置树同步*/
#define DIRTY 1      /* 数据已被修改，商未同步 */

typedef struct tag_ST_ScalarVal
{
    char *pcNodePath;                                                        /* 节点全路径 */
    char *pcValidateRegx;                                                  /* 校验值合法性的正则表达式 */
    void *pValue;                                                                /* 配置值保存的位置 */
    unsigned int  type;                                                        /* 节点类型，boolean/int/string */
    BOOL (*pFuncScalarValueCheck)(struct tag_ST_ScalarVal *pstScalarVal, char *pszValue);    /* 节点值检查,如果不能通过正则表达式检查，则需要实现此函数，如果可以通过正则表达式完成，则为NULL */
    int (*pFuncWlanScalarValueSet)(struct tag_ST_ScalarVal *pstScalarVal, char *pszValue);		/* set过程中的钩子函数 */
    int (*pFuncWlanScalarValueCommit)(struct tag_ST_ScalarVal *pstScalarVal);	          /* 生效函数 */
    int iSetFlag;                                                                  /* set标记表示当前值是否set，初始为0，set后为1，生效恢复为0 */
}ST_ScalarVal;

int SetScalarValHook();
int CommitScalarValHook();
int CancelScalarValHook();
int InitScalarValHook();
void UninitScalarVarHook(void);
ST_ScalarVal* GetScalarByNodePath(char *pcNodePath);

/* commit call back flags */    
#define COMMIT_F_RESTART 0x1

/* added by heym, 20080817 */
int SV_SaveVarByNodePath(char *pcNodePath, int Value);


#endif
