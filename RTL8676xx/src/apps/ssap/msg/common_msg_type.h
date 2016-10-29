#ifndef _COMMON_MSG_TYPE_
#define _COMMON_MSG_TYPE_

#include <tbsutil.h>

/****************hook table define and function*******************/

typedef enum
{
    /* SET MSG FUNCTION */
    CHECK_FUNC = 1,                                         /*节点检查函数*/
    CHECK_CONFLICT_FUNC,                             /*节点值冲突检查函数*/

    /* COMMIT MSG FUNCTION */
    APPLY_FUNC,                                               /*生效函数*/
    APPLY_POST_FUNC,                                     /*模块保存信息到配置树后调用的公共函数，主要用来处理模块的公共动作*/
#ifdef MODIFY_CFG
    MODIFY_CFG_FUNC,                                     /*缓存值保存到配置树后调用的函数，主要用来进行配置树的修改*/
#endif

    /* CANCEL MSG FUNCTION ,NO ONE NOW*/

    /* DELETE MSG FUNCTION */
    DELETE_CHECK_FUNC,                                 /*删除检查函数*/
    DELETE_FUNC,                                              /*删除生效函数*/

    /*ADD MSG FUNCTION*/
    ADD_CHECK_FUNC,                                       /**/
    ADD_FUNC,

    /*ADDED MSG FUNCTION*/
    ADDED_FUNC,

    /*DELETED MSG FUNCTION*/
    DELETED_FUNC,

    /*UPDATE MSG FUNCTION*/
    UPDATE_FUNC,

}HookType;

typedef int (* CheckFunc)(char *); //CHECK_FUNC
typedef int (*CheckConflictFunc)(char *,char *);//CHECK_CONFLICT_FUNC

typedef int (*ApplyFunc)(char *,char*);//APPLY_FUNC
typedef int (*PostApplyFunc)(void);//APPLY_POST_FUNC
#ifdef MODIFY_CFG
typedef int (*ModifyCFGFunc)(char *,char *);//MODIFY_CFG_FUNC
#endif

typedef int (*CheckDeleteFunc)(char *);//DELETE_CHECK_FUNC
typedef int (*DeleteFunc)(char *);//DELETE_FUNC

typedef int(*CheckAddFunc)(char *);//ADD_CHECK_FUNC
typedef int (*AddFunc)(char *,unsigned long);//ADD_FUNC

typedef int (*UpdateFunc)(void);//UPDATE_FUNC

typedef int (*AddedFunc)(char *);//ADDED_FUNC

typedef int (*DeletedFunc)(char *);//DELETED_FUNC

typedef struct tag_ST_HookEntry
{
    char *pNodePath;
    HookType eHookType;
    void *pHookFunc;
}ST_HookEntry;

/*******************************cache function****************************/
void InitCache(struct list_head * pCacheList);
void AddCacheNode(struct list_head * pCacheList,char *pszPath,char *pszValue);
void SetCacheNode(struct list_head * pCacheList,char *pszPath,char *pszValue);
void DeleteCacheNode(struct list_head * pCacheList,char*pszPath);
char * GetCacheNode(struct list_head * pCacheList,char *pszPath);
void CleanCache(struct list_head * pCacheList);

void SetCleanFlag(struct list_head * pCacheList,char *pszPath);
void CleanTagedNodes(struct list_head * pCacheList);

void DumpCache(struct list_head * pCacheList);

#endif

