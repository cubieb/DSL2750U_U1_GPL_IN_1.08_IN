/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : wanpath_list.h
 文件描述 : 设备名和连接名对应的队列头文件，使用normal_list.h中通用链表封装


 函数列表 :

 修订记录 :
          1 创建 : 李伟
            时间 : 2008-8-27
            描述 :

**********************************************************************/
#ifndef _WANPATH_LIST_H_
#define _WANPATH_LIST_H_

#include "tbsutil.h"

typedef enum tagEN_CACHE_CLEAN_FLAG
{
    VALID = 0,
    CLEAN
}EN_ACL_INST_TYPE;

typedef struct tag_ST_CONNPATH_DEVNAME{
	struct list_head head;
	char	*pszPath;
	char *pszValue;
	char cCleanFlag;
}ST_Cache_Entry;


/* 链表使用函数，具体用法见devname_list.c中例子 */
void CacheList_Init(struct list_head *head);
void CacheList_PrintAll(struct list_head *head);
void CacheList_DropAll(struct list_head *head);
void CacheList_InsertHead(struct list_head *head, char *pszPath, char *pszValue);
void CacheList_AddTail(struct list_head *head, char *pszPath, char *pszValue);
void CacheList_DeleteEntry(struct list_head *head, char *pszValue);
char *CacheList_GetValue(struct list_head *head, char *pszPath);
int CacheList_IsInList(struct list_head *head, char *pszValue, BOOL bIsFullMatch);
void CacheList_SetEntry(struct list_head *head,char *pszPath,char *pszValue);

void CacheList_SetCleanFlag(struct list_head *head, char *pszPath);
void CacheList_DropAllWithCleanFlag(struct list_head *head);

#endif

