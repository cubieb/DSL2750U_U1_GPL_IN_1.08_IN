/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cfg_prv.h
 文件描述 : cfg内部私有头文件

 修订记录 :
          1 创建 : 陈跃东
            日期 : 2007-11-23
            描述 :

**********************************************************************/


#ifndef __CFG_PRV_H__
#define __CFG_PRV_H__

#include "cfg_api.h"
#include "cfg_comm.h"

#include "tbsmsg.h"
#include "tbstype.h"

#include <stdlib.h>
#include <errno.h>

#include "scew/scew.h"
#include "scew/xelement.h"
#include "scew/xtree.h"
#include "warnlog.h"
#include "tbserror.h"


#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif






/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/







/*********************** 依赖函数宏 ************************/

#define CFG_MemCpy(pvDst, pvSrc, len)  (void)memcpy(pvDst, pvSrc, (unsigned long)(len))






/*********************** 重复代码片段 ************************/

char *cfg_strdup(const char *pcPath);

/*lint -save -e652*/
#undef strdup
#define strdup   cfg_strdup
/*lint -restore*/

#define CFG_DUP_PATH(pcPathTmp, pcPath) \
{ \
    /* 复制路径 */ \
    pcPathTmp = strdup(pcPath); \
    if (NULL == pcPathTmp) \
    { \
        CFG_ERR(ERR_CFG_MALLOC_FAIL); \
        return ERR_CFG_MALLOC_FAIL; \
    } \
}

#define CFG_FREE_PATH(pcPathTmp)  (void)0



#define CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, pstNodeInfo, ulMask) \
{ \
    char *pcPathTmp = NULL; \
 \
    /* 复制路径 */ \
    CFG_DUP_PATH(pcPathTmp, pcPath); \
 \
    /* 查找配置节点 */ \
    ret = CFG_GetNode(g_pstCfgTree, pcPathTmp, (void **)(void *)&pstNode, \
                      pstNodeInfo, ulMask); \
    CFG_FREE_PATH(pcPathTmp); \
    if (CFG_OK != ret || NULL == pstNode)  /* 没有找到 */ \
    { \
        CFG_ERR(ret, "%s", pcPath); \
        return ret; \
    } \
}

#define CFG_DUP_AND_SEARCH_NODE_BY_INDEX(pcPath, pulIndex, pstNode, pstNodeInfo, ulMask) \
{ \
    char *pcPathTmp = NULL; \
 \
    /* 复制路径 */ \
    CFG_DUP_PATH(pcPathTmp, pcPath); \
 \
    /* 查找配置节点 */ \
    ret = CFG_GetNodeByIndex(g_pstCfgTree, pcPathTmp, pulIndex, (void **)(void *)&pstNode, \
                      pstNodeInfo, ulMask); \
    CFG_FREE_PATH(pcPathTmp); \
    if (CFG_OK != ret || NULL == pstNode)  /* 没有找到 */ \
    { \
        CFG_ERR(ret, "%s", pcPath); \
        return ret; \
    } \
}


#define CFG_MATCH_ROOT(pszTmp, pcPathTmp, pcPath, pstRoot) \
{ \
    ret = strcmp(scew_element_name(pstRoot), pszTmp); \
    if (0 != ret) \
    { \
        CFG_FREE_PATH(pcPathTmp); \
\
        /* 节点不存在 */ \
        CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pcPath); \
        return ERR_CFG_PATH_NOT_EXSITED; \
    } \
}

#if 0
/* 转义成合法节点 */
#define CFG_NUM_TO_NODE(pcNum) \
{ \
    if (pcNum[0] >= '0' && pcNum[0] <= '9' ) \
    { \
        pcNum--; \
        pcNum[0] = '_'; \
    } \
}


/* 转义成合法数字节 */
#define CFG_NODE_TO_NUM(pcNode) \
{ \
    if ('_' == pcNode[0] \
        && ('0' <= pcNode[1] \
        &&  '9' >= pcNode[1])) \
    { \
        pcNode++; \
    } \
}

#endif

#define CFG_NUM_TO_NODE(pcNum) (void)0
#define CFG_NODE_TO_NUM(pcNode) (void)0
//#define CFG_IS_NODE_NUM(pcNode) ((pcNode)[0] >= '0' && (pcNode)[0] <= '9')
#define CFG_IS_NODE_NUM(pcNode) CFG_IsNodeNum(pcNode)


#define CFG_NODE_ATTR(pstNode, pcName, pcVal) \
{ \
    scew_attribute *pstAttr = NULL; \
    pstAttr = scew_attribute_by_name(pstNode, pcName); \
    if (NULL != pstAttr) \
    { \
        pcVal = scew_attribute_value(pstAttr); \
    } \
    else \
    { \
        pcVal = ""; \
    } \
}


/* 过滤非叶节点 */
#define CFG_FILTER_NON_LEAF(pstNode)\
{ \
    pstAttr = scew_attribute_by_name(pstNode, ATTR_TYPE); \
    if (NULL != pstAttr) \
    { \
        pcAttrVal = scew_attribute_value(pstAttr); \
 \
        /* 根据类型进行拦截 */ \
        if (0 == strcmp(pcAttrVal, TYPE_CLASS) \
            || 0 == strcmp(pcAttrVal, TYPE_OBJECT)) \
        { \
            return CFG_OK; \
        } \
    } \
}


/* 往高地址复制 */
#define MEM_BACKCPY(pcSrc, pcDst, ulLen) \
{ \
    long lLenWrok = (long)(ulLen) - 1; \
    while (lLenWrok >= 0) \
    { \
        (pcDst)[lLenWrok] = (pcSrc)[lLenWrok]; \
        lLenWrok--; \
    } \
}

/* 添加尾节点 */
#define CFG_LIST_APPEND(list, node) \
{ \
    if (NULL == (list)[0]) \
    { \
        (list)[0] = (node); \
        (list)[1] = (node); \
    } \
    else \
    { \
       (list)[1]->pstNext = (node); \
       (list)[1] = (node); \
    } \
    (node)->pstNext = NULL; \
}




#define CFG_SETATTR(pstNode, pcAttrName, pcNewAttrVal) \
{ \
    pstAttr = scew_attribute_by_name(pstNode, pcAttrName); \
    if (NULL == pstAttr) \
    { \
        pstAttr = attribute_create(pcAttrName, pcNewAttrVal); \
        if (NULL == pstAttr) \
        { \
            CFG_ERR(ERR_CFG_INTERNAL_ERR); \
            return ERR_CFG_INTERNAL_ERR; \
        } \
        (void)attribute_list_add(pstNode->attributes, pstAttr); \
    } \
    else \
    { \
        pcAttrVal = scew_attribute_set_value(pstAttr, pcNewAttrVal); \
        if (NULL == pcAttrVal) \
        { \
            CFG_ERR(ERR_CFG_INTERNAL_ERR); \
            return ERR_CFG_INTERNAL_ERR; \
        } \
    } \
}



#define CFG_ADDATTR(pstNode, pcAttrName, pcNewAttrVal, expt) \
{ \
    pstAttr = attribute_create(pcAttrName, pcNewAttrVal); \
    if (NULL == pstAttr) \
    { \
        expt; \
        CFG_ERR(ERR_CFG_INTERNAL_ERR); \
        return ERR_CFG_INTERNAL_ERR; \
    } \
    (void)attribute_list_add(pstNode->attributes, pstAttr); \
}


#ifndef _CFG_TREE_ALL_STANDARD

#define CFG_SET_RIGHT_CHECK(pstNode, pstNodeInfo, pcAccessor, Extra) \
{ \
    unsigned char ucIsAccessorIn = 0; \
    if (NULL != pcAccessor) \
    { \
        if ('\0' == pcAccessor[0]) \
        { \
            /* TR069, 先检查是否标准, 然后直接取write */ \
            if (0 == (pstNodeInfo)->ucStandard) \
            { \
                CFG_ERR(ERR_CFG_NOT_STANDARD); \
                return ERR_CFG_NOT_STANDARD; \
            } \
 \
            pstAttr = scew_attribute_by_name(pstNode, ATTR_WRITABLE); \
            if (NULL != pstAttr) \
            { \
                pcAttrVal = scew_attribute_value(pstAttr); \
                if ('1' != pcAttrVal[0] && Extra != pcAttrVal[0]) \
                { \
                    CFG_ERR(ERR_CFG_NOT_WRITABLE); \
                    return ERR_CFG_NOT_WRITABLE; \
                } \
            } \
        } \
        else \
        { \
            /* Lan, 取AccessList */ \
            ucIsAccessorIn = CFG_IsAccessorIn((pstNodeInfo)->pcAccessList, pcAccessor); \
            if (0 == ucIsAccessorIn) \
            { \
                CFG_ERR(ERR_CFG_NOT_IN_ACCESSLIST); \
                return ERR_CFG_NOT_IN_ACCESSLIST; \
            } \
 \
        } \
    } \
}




#define CFG_GET_RIGHT_CHECK(pstNode, pcAccessor) \
{ \
    if (NULL != pcAccessor) \
    { \
        if ('\0' == pcAccessor[0]) \
        { \
            /* TR069, 检查是否标准 */ \
            pstAttr = scew_attribute_by_name(pstNode, ATTR_STANDARD); \
            if (NULL != pstAttr) \
            { \
                pcAttrVal = scew_attribute_value(pstAttr); \
                if ('0' == pcAttrVal[0]) \
                { \
                    return CFG_OK; \
                } \
            } \
        } \
    } \
}

#define CFG_GET_RIGHT_CHECK_BY_INFO(pstNodeInfo, pcAccessor) \
{ \
    if (NULL != pcAccessor) \
    { \
        if ('\0' == pcAccessor[0]) \
        { \
            if (0 == (pstNodeInfo)->ucStandard) \
            { \
                CFG_ERR(ERR_CFG_NOT_STANDARD); \
                return ERR_CFG_NOT_STANDARD; \
            } \
        } \
    } \
}

#define CFG_PASS_NOSTANDARD(pstNode, pcAccessor, flag) \
{ \
    if (NULL != pcAccessor) \
    { \
        if ('\0' == pcAccessor[0]) \
        { \
            /* TR069, 检查是否标准 */ \
            pstAttr = scew_attribute_by_name(pstNode, ATTR_STANDARD); \
            if (NULL != pstAttr) \
            { \
                pcAttrVal = scew_attribute_value(pstAttr); \
                if ('0' == pcAttrVal[0]) \
                { \
                    pstNode = NULL; \
                    flag; \
                } \
            } \
        } \
    } \
}

#else

#define CFG_SET_RIGHT_CHECK(pstNode, pstNodeInfo, pcAccessor, Extra) \
{ \
    unsigned char ucIsAccessorIn = 0; \
    if (NULL != pcAccessor) \
    { \
        if ('\0' == pcAccessor[0]) \
        { \
            pstAttr = scew_attribute_by_name(pstNode, ATTR_WRITABLE); \
            if (NULL != pstAttr) \
            { \
                pcAttrVal = scew_attribute_value(pstAttr); \
                if ('1' != pcAttrVal[0] && Extra != pcAttrVal[0]) \
                { \
                    CFG_ERR(ERR_CFG_NOT_WRITABLE); \
                    return ERR_CFG_NOT_WRITABLE; \
                } \
            } \
        } \
        else \
        { \
            /* Lan, 取AccessList */ \
            ucIsAccessorIn = CFG_IsAccessorIn((pstNodeInfo)->pcAccessList, pcAccessor); \
            if (0 == ucIsAccessorIn) \
            { \
                CFG_ERR(ERR_CFG_NOT_IN_ACCESSLIST); \
                return ERR_CFG_NOT_IN_ACCESSLIST; \
            } \
 \
        } \
    } \
}




#define CFG_GET_RIGHT_CHECK(pstNode, pcAccessor) \
                    (void)pstAttr; (void)pcAttrVal; (void)0

#define CFG_GET_RIGHT_CHECK_BY_INFO(pstNode, pcAccessor) (void)0

#define CFG_PASS_NOSTANDARD(pstNode, pcAccessor, flag) \
                    (void)pstAttr; (void)pcAttrVal; (void)0



#endif




#define CFG_PASS_NONEEDSAVE(pstNode) \
{ \
    pstAttr = scew_attribute_by_name(pstNode, ATTR_NEEDSAVE); \
    if (NULL != pstAttr) \
    { \
        pcAttrVal = scew_attribute_value(pstAttr); \
        if ('0' == pcAttrVal[0]) \
        { \
            return CFG_OK; \
        } \
    } \
}





#define CFG_GET_CARE_MID(pstNode, pusCareMIDList, pulCareMIDCount) \
{ \
    /* 取CareMID */ \
    pstAttr = scew_attribute_by_name(pstNode, ATTR_CAREMID); \
    if (NULL != pstAttr) \
    { \
        pcAttrVal = scew_attribute_value(pstAttr); \
\
        /* 挨个输出 */ \
        if (strlen(pcAttrVal) >= 4) \
        { \
            do \
            { \
                *pusCareMIDList = strtol(pcAttrVal, NULL, CFG_MID_BASE); \
                pusCareMIDList++; \
                (*pulCareMIDCount)++; \
                if ('\0' == pcAttrVal[4]) \
                { \
                    break; \
                } \
                pcAttrVal += 5; \
            } while (1); \
        } \
    } \
}


#define CFG_ADD_CARE_MID(pstNode, usMID) \
do { \
    ret = CFG_AddCareMID(pstNode, usMID); \
    if (CFG_OK != ret) \
    { \
        CFG_ERR(ret); \
        return ret; \
    } \
} while (0)



#define CFG_REDUMP_FILE(stDumpBuf, pstNode) \
{ \
    stDumpBuf.pcBuf = malloc(stDumpBuf.ulInfactLen); \
    if (NULL == stDumpBuf.pcBuf) \
    { \
        CFG_ERR(ERR_CFG_MALLOC_FAIL); \
        return ERR_CFG_MALLOC_FAIL; \
    } \
    stDumpBuf.ulLen = stDumpBuf.ulInfactLen; \
    stDumpBuf.ulInfactLen = 0; \
    stDumpBuf.ulPos = 0; \
    (void)CFG_TreeDumpAccess(pstNode, &stDumpBuf, 0); \
    if (stDumpBuf.ulInfactLen != stDumpBuf.ulPos) \
    { \
        free(stDumpBuf.pcBuf); \
 \
        CFG_ERR(ERR_CFG_INTERNAL_ERR); \
        return ERR_CFG_INTERNAL_ERR; \
    } \
}




#define CFG_MAX(a, b)  (((a) >= (b)) ? (a) : (b))


/******************* MID树特殊节点 *********************/
#define NODE_WILDCARD        "i"

#define NODE_WILDCARD_EXP    "{i}"


#define NODE_WILDCARD_CVT(pcNode) \
{ \
    if (0 == strcmp(pcNode, NODE_WILDCARD_EXP)) \
    { \
        pcNode++; \
        pcNode[1] = '\0'; \
    } \
}

#define NODE_WILDCARD_RCVT(pcNode) \
{ \
    if (0 == strcmp(pcNode, NODE_WILDCARD)) \
    { \
        pcNode = NODE_WILDCARD_EXP; \
    } \
}


/*********************** 属性名 ************************/

#define ATTR_NOTI            "Notification"
#define ATTR_NEED_NOTI       "NeedNoti"
#define ATTR_TYPE            "type"
#define ATTR_MID             "MID"
#define ATTR_MAX_IDX         "MaxIdx"

#define ATTR_STANDARD        "Standard"
#define ATTR_NEEDSAVE        "NeedSave"

#define ATTR_CAREMID         "CareMID"

#define ATTR_ACCESS_LIST     "AccessList"
#define ATTR_WRITABLE        "Writable"
#define ATTR_STARTDEL        "StartDel"
#define ATTR_PASSWORDD       "Password"
#define ATTR_STAT            "Stat"
#define ATTR_CACHE           "Cache"
#define ATTR_INFORM_LIMIT    "InformLimit"
#define ATTR_LAST_INFORM     "LastInform"
#define ATTR_MAX_NODE        "MaxNode"
#define ATTR_CUR_NODE        "CurNode"



#define ATTR_NOTI_LEN        (sizeof(ATTR_NOTI) - 1)
#define ATTR_ACCESS_LIST_LEN (sizeof(ATTR_ACCESS_LIST) - 1)
#define ATTR_WRITABLE_LEN    (sizeof(ATTR_WRITABLE) - 1)

#define ACTIVE_NOTI          "2"
#define ACTIVE_NOTI_C        '2'

#define PASSIVE_NOTI         "1"
#define PASSIVE_NOTI_C       '1'

#define DEFAULT_NOTI         "0"


#define NEEDNOTI             "1"
#define NEEDNOTI_C           '1'

#define NONEEDNOTI           "0"
#define NONEEDNOTI_C         '0'

#define ISSTAT               "1"
#define ISSTAT_C             '1'

#define DELIMIT              "."
#define DELIMIT_C            '.'

#define TREE_FLAG_C          '.'
#define TREE_FLAG            "."

#define TYPE_OBJECT          "object"
#define TYPE_CLASS           "class"

#define DEFAULT_ACCESS_LIST  "Subscriber"
#define DEFAULT_WRITABLE     "1"
#define DEFAULT_WRITABLE_STR  ATTR_WRITABLE "=\"" DEFAULT_WRITABLE "\""
#define DEFAULT_WRITABLE_STR_LEN (sizeof(DEFAULT_WRITABLE_STR) - 1)


#define CFG_NO_WRITABLE_C    ('0' + CFG_NO_WRITABLE)
#define CFG_WRITABLE_C       ('0' + CFG_WRITABLE)
#define CFG_ADDABLE_C        ('0' + CFG_ADDABLE)
#define CFG_DELETABLE_C      ('0' + CFG_DELETABLE)


/*********************** 特定数值 ************************/
#define CFG_MAX_PATH_LEN   256   /* 最大路径长度 */

#define CFG_NUM_TRANS_MEAN_LEN      2048  /* 转义文件后长度增长值 */

#define CFG_MID_BASE      16     /* 树上mid按16进制表示 */

#define CFG_ACCESS_STACK_DEEPTH  25  /* 非递归遍历树时的栈的深度 */

#define CFG_MAX_FILE_LEN   0xffff    /* 尝试dump文件的长度 */

#define CFG_MAX_WILDCARD_NUM   256

#define CFG_MAX_MIDLIST_LEN   256







/*********************** 单元测试桩 ************************/
#ifdef _CFG_UT

#undef STATIC
#define STATIC

extern unsigned long g_ulCfgMaxWildCardNum;

#undef CFG_MAX_WILDCARD_NUM
#define CFG_MAX_WILDCARD_NUM   g_ulCfgMaxWildCardNum

char *stub_strdup(const char *s);
void *stub_malloc(unsigned long ulLen);
scew_attribute *stub_attribute_create(XML_Char const* name, XML_Char const* value);
XML_Char const *stub_scew_attribute_set_value(scew_attribute* attribute, XML_Char const*value);
scew_element *stub_scew_element_add(scew_element* element, XML_Char const *name);
XML_Char const *stub_scew_element_set_contents(scew_element* element, XML_Char const* data);
FILE *stub_fopen(const char *path, const char *mode);
size_t stub_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int stub_read(int fd, void *buf, size_t count);
ssize_t stub_write(int fd, const void *buf, size_t count);
unsigned long stub_crc32(unsigned long val, const unsigned char *s, unsigned int len);
int stub_compress(char *pacDest, unsigned long *pulDestLength,
                  const char* pacSrc, unsigned long ulSrcLength);
int stub_uncompress(char *pacDest, unsigned long *pulDestLength,
                  const char* pacSrc, unsigned long ulSrcLength);
void SetInfactLenAddr(unsigned long *pulLen);
void *stub_memcpy(void *dest, const void *src, size_t n);
unsigned int cfg_ut_scew_parser_load_buffer(scew_parser* parser, char const* buffer,
                        unsigned int size);

#define strdup                    stub_strdup
#define malloc                    stub_malloc
#define attribute_create          stub_attribute_create
#define scew_attribute_set_value  stub_scew_attribute_set_value
#define scew_element_add          stub_scew_element_add
#define scew_element_set_contents stub_scew_element_set_contents
#define fopen                     stub_fopen
#define fread                     stub_fread
#define read                      stub_read
#define write                     stub_write
#define crc32                     stub_crc32
#define compress                  stub_compress
#define uncompress                stub_uncompress
#define scew_parser_load_buffer   cfg_ut_scew_parser_load_buffer

#undef CFG_MemCpy
#define CFG_MemCpy(pvDst, pvSrc, len) \
    (void)stub_memcpy(pvDst, pvSrc, (unsigned long)(len))

#endif












/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/



/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/





/* 配置树 */
extern scew_tree *g_pstCfgTree;

/* 访问者, "" 表示acs, "Subscriber" 表示web */
extern const char *g_pcCfgAccessor;


/* 遍历树进行Get操作时的操作函数形式 */
typedef CFG_RET (*FUNC_GET_OPT)(const scew_element *pstNode,
                 const char *pcAbsPath, unsigned char ucLen, void *pvPara);

/* 遍历树进行Set操作时的操作函数形式 */
typedef CFG_RET (*FUNC_SET_OPT)(const scew_element *pstNode, void *pvPara);



/* 按xml格式dump的结构 */
typedef struct
{
    char *pcBuf;                /* 输出的缓冲区 */
    unsigned long ulLen;        /* 缓冲区的有效长度 */
    unsigned long ulPos;        /* 当前可输出的位置 */
    unsigned long ulInfactLen;  /* 实际应该输出的长度, 当 ulInfactLen > ulPos
                                   时, 表示 pcBuf 长度不够 */
} ST_CFG_DUMP_BUF;


#define CFG_NODE_INFO_MID        0x00000001
#define CFG_NODE_INFO_STANDARD   0x00000002
#define CFG_NODE_INFO_NOTI       0x00000004
#define CFG_NODE_INFO_ACCESSLIST 0x00000008



typedef struct
{
    unsigned short usMID;
    unsigned char  ucStandard;
    unsigned char  ucNoti;
    const char *pcAccessList;

} ST_CFG_NODE_INFO;

typedef long (*FUNC_ATTR_GET_OPT)(const scew_element *pstNode,
                    const char *pcAbsPath, unsigned char ucLen,
                    const ST_CFG_NODE_INFO *pstNodeInfo, void *pvPara);

typedef CFG_RET (*FUNC_CFG_READ_CFG)(const char *pcPath, char **ppcBuf,
                          unsigned long ulPrevSpace,
                          unsigned long ulPostSpace, unsigned long *pulLen);


/* 非递归操作的遍历栈结构 */
typedef struct
{
    const scew_element *pstPath;
    ST_CFG_NODE_INFO stNodeInfo;
    unsigned char   ucPathLen;
    unsigned char   aucSpace[3];
} ST_CFG_TREE_STACK_NODE;



/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/



/* 普通的遍历子树的函数(所有取遍历取值均用此函数) */
CFG_RET CFG_TreeGetAccess(const scew_element *pstNode, FUNC_GET_OPT pfnGetOpt,
                                          char *pcAbsPath, void *pvData);

/* 遍历下层节点的函数 */
CFG_RET CFG_TreeGetNextLevelAccess(const scew_element *pstNode,
            FUNC_GET_OPT pfnGetOpt, char *pcAbsPath, void *pvData);

/* 对单个节点取Name的操作的函数 */
CFG_RET CFG_GetSingleNodeName(const scew_element *pstNode,
                    const char *pcAbsPath, unsigned char ucLen, void *pvPara);


/* 对单个节点取Val */
CFG_RET CFG_GetSingleNodeVal(const scew_element *pstNode,
                    const char *pcAbsPath, unsigned char ucLen, void *pvPara);

/* 解析pszXMLContent 成树, 由ppstTree输出 */
CFG_RET CFG_XmlInitPaser(scew_tree **ppstTree, const char *pszXMLContent);

/* 对单个节点取通知 */
CFG_RET CFG_GetSingleNoti(const scew_element *pstNode,
                   const char *pcAbsPath, unsigned char ucLen, void *pvPara);

/* 通用的取值(节点值\属性值\name) */
CFG_RET CFG_GetCommVal(const char *pcPath, unsigned char ucNextLevel,
                        ST_PARA_VAL **ppstParaVal, FUNC_GET_OPT pfnOpt);

/* 解析属性列表, 属性列表格式请参考ST_PARA_VAL */
CFG_RET CFG_ParseAttrVal(char *pcVal, void **pvOut);

/* 对单个节点设置属性 */
CFG_RET CFG_SetSingleNodeAttr(const scew_element *pstNode, void *pvPara);

/* 对树做设置操作时的遍历函数, 之所以不是跟取值用同一个函数,
   因为取值时需要记录完整路径,而设置时则不用 */
CFG_RET CFG_TreeSetAccess(const scew_element *pstNode,
                       FUNC_SET_OPT pfnSetOpt, void *pvData);

/* 对单个节点取Attr(属性) */
CFG_RET CFG_GetSingleNodeAttr(const scew_element *pstNode,
                    const char *pcAbsPath, unsigned char ucLen,
                    const ST_CFG_NODE_INFO *pstNodeInfo, void *pvPara);

/* 文件内容转义, 主要是把数字节点名转为非数字节点名 */
void CFG_FileTransMean(char *pcBuf, unsigned long ulLen);

/* 按xml格式 dump子树 */
CFG_RET CFG_TreeDumpAccess(const scew_element *pElement,
              ST_CFG_DUMP_BUF *pstDumpBuf, unsigned char ucTable);

/* 更新对象集合的最大已使用的实例名 */
CFG_RET CFG_UpdateMaxIdx(const scew_element *pstNode);

/* 检查访问者是否在列表中 */
unsigned char CFG_IsAccessorIn(const char *pcAccessList, const char *pcAccessor);

/* 统一处理节点改变的Notification的函数 */
CFG_RET CFG_NotiInform(const scew_element *pstNode, const ST_CFG_NODE_INFO *pstInfo);

/* 取得节点, 子函数, pcPath 可写 */
CFG_RET CFG_GetNode(const void *pvTree, char *pcPath, void **ppvNode,
                    ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask);

/* 通用的取值(节点值\属性值\name)的函数 */
CFG_RET CFG_GetCommAttrVal(const char *pcPath, unsigned char ucNextLevel,
                        ST_PARA_VAL **ppstParaVal, FUNC_ATTR_GET_OPT pfnOpt);

/* 普通的遍历子树的函数(所有取遍历取值均用此函数) */
CFG_RET CFG_TreeAttrGetAccess(const scew_element *pstNode, FUNC_ATTR_GET_OPT pfnGetOpt,
                   char *pcAbsPath, const ST_CFG_NODE_INFO *pstNodeInfo, void *pvData);

/* 获取mid树指针 */
void *CFG_GetMIDTree(void);

/* 逻辑树初始化 */
CFG_RET CFG_MIDTreeInit(void);

/* 逻辑树释放 */
CFG_RET CFG_MIDTreeFinal(void);

/* 将子树节点名称转义回去 */
CFG_RET CFG_TreeTransBack(scew_tree *pstTree);

/* 删除启动时需要删除的节点 */
void CFG_RmvDeletingNode(scew_tree *pstTree);


typedef struct
{
    unsigned char ucTotalLevel;
    unsigned char ucLevelCount;
    unsigned char aucLevelList[CFG_MAX_DEPTH];

} ST_CFG_MATCH_TREE_INFO;

/* 获取通配路径的节点地址列表 */
CFG_RET CFG_GetWildMIDNodesPtr(const char *pcWildPath, scew_element **apstTree,
                               unsigned long ulBufNum,
                               unsigned long *pulHead, unsigned long *pulTail,
                               ST_CFG_MATCH_TREE_INFO *pstMatchInfo);

/* 在非递归遍历中用于根据栈取得当前节点的路径和MID */
void CFG_GetPathLenAndMID(const scew_element *pstPath, unsigned short *pusMID,
                                                  unsigned char *pucLen);
/* 初始化节点信息 */
void CFG_InitNodeInfo(ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask);

/* 更新节点信息结构 */
void CFG_UpdateNodeInfo(const scew_element *pstNode,
       ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask);

/* 通过通配路径和其index列表获取节点地址 */
CFG_RET CFG_GetNodeByIndex(const void *pvTree, char *pcMathPath,
                    const unsigned long *pulIndexList, void **ppvNode,
                    ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask);

/* 将逻辑树上属性为NeedSave=0的节点填充到数据树上 */
CFG_RET CFG_FillDataTreeExNodes(void);

/* 将逻辑树上一些属性列表填充到数据树上 */
CFG_RET CFG_FillDataTreeExAttrs(void);

/* 更新一个对象当前的实例个数 */
CFG_RET CFG_UpdateObjCurInstCount(const scew_element *pstObj);

/* 复制子树的遍历函数 */
CFG_RET CFG_DupAccess(const scew_element *pstNode, scew_element **ppstDupNode);

/* 判断节点是否为数字节点 */
unsigned char CFG_IsNodeNum(const char *pcNode);

/* 类似于strtok的功能, 专用于cfg模块解析以'.'分隔的路径 */
char *CFG_StrChrTok(char *pcStr, char cKey, char **ppcNext);

/* 将节点到另一节点指定子节点的后面 */
scew_element *scew_element_insert_elem(scew_element* element,
                      scew_element *left, scew_element* new_elem);


#ifdef _CFG_DEBUG

CFG_RET CFG_ListNoTypeNode(void);
CFG_RET CFG_PrintToExcel(void);
void CFG_SepTree(void);
CFG_RET CFG_DupSingleNode(const scew_element *pstNode, scew_element **ppstDupNode);
CFG_RET CFG_TreePrintAccess(const scew_element *pElement,
                     ST_CFG_DUMP_BUF *pstDumpBuf, unsigned char ucTable);

#endif


/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __CFG_PRV_H__ */



