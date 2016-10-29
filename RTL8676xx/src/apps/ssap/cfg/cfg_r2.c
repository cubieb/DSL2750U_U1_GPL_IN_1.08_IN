
/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cfg_r2.c
 文件描述 : cfg 模块针对r2版本的优化所增加的函数
 函数列表 :

 修订记录 :
          1 创建 : 陈跃东
            日期 : 2008-11-23
            描述 :
**********************************************************************/

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


#include "cfg_api.h"
#include "cfg_file.h"
#include "cfg_prv.h"

/* 用来返回节点值 = NULL 的情况 */
STATIC const char *s_pcCfgStringNull = "";


/* 不同类型的输出值的函数 */
typedef CFG_RET (*FUNC_VAL_TYPE_GET)(const char *pcVal, void *pvOut);


typedef struct
{
    const char *pcType;
    unsigned char ucTypeLen;
    unsigned char aucSpace[3];
    FUNC_VAL_TYPE_GET pfnValTypeGet;

} ST_CFG_VAL_TYPE_GET;


/***********************************************************
接口:   获取对象类节点值
参数:   pcVal, 具体值
        pvOut, 输出值的地址, 这里输出常量""的地址
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_objectValTypeGet(const char *pcVal, void *pvOut)
{
    (void)pcVal;

    *(char **)pvOut = (char *)s_pcCfgStringNull;
    return CFG_OK;
}

/***********************************************************
接口:   获取字符串类节点值
参数:   pcVal, 具体值地址
        pvOut, 输出值的地址
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_stringValTypeGet(const char *pcVal, void *pvOut)
{
    if (NULL == pcVal)
    {
        *(char **)pvOut = (char *)s_pcCfgStringNull;
    }
    else
    {
        *(char **)pvOut = (char *)pcVal;
    }

    return CFG_OK;
}


/***********************************************************
接口:   获取int类节点值
参数:   pcVal, 具体值地址
        pvOut, 输出值的地址, 输出转换后的值
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_intValTypeGet(const char *pcVal, void *pvOut)
{
    if (NULL == pcVal)
    {
        *(int *)pvOut = 0;
    }
    else
    {
        *(int *)pvOut = strtol(pcVal, NULL, 10);
    }

    return CFG_OK;
}

/***********************************************************
接口:   获取unsigned int类节点值
参数:   pcVal, 具体值地址
        pvOut, 输出值的地址, 输出转换后的值
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_unsignedIntValTypeGet(const char *pcVal, void *pvOut)
{
    if (NULL == pcVal)
    {
        *(int *)pvOut = 0;
    }
    else
    {
        *(int *)pvOut = strtol(pcVal, NULL, 10);
    }

    return CFG_OK;
}

/***********************************************************
接口:   获取boolean类节点值
参数:   pcVal, 具体值地址
        pvOut, 输出值的地址, 输出转换后的值
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_booleanValTypeGet(const char *pcVal, void *pvOut)
{
    if (NULL == pcVal || '0' == pcVal[0])
    {
        *(int *)pvOut = 0;
    }
    else
    {
        *(int *)pvOut = 1;
    }

    return CFG_OK;
}


/***********************************************************
接口:   获取time类节点值
参数:   pcVal, 具体值地址
        pvOut, 输出值的地址, 实际上跟string类型一样
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_dateTimeValTypeGet(const char *pcVal, void *pvOut)
{
    if (NULL == pcVal)
    {
        *(char **)pvOut = (char *)s_pcCfgStringNull;
    }
    else
    {
        *(char **)pvOut = (char *)pcVal;
    }

    return CFG_OK;
}

/***********************************************************
接口:   获取base64类节点值
参数:   pcVal, 具体值地址
        pvOut, 输出值的地址, 实际上跟string类型一样
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_base64ValTypeGet(const char *pcVal, void *pvOut)
{
    if (NULL == pcVal)
    {
        *(char **)pvOut = (char *)s_pcCfgStringNull;
    }
    else
    {
        *(char **)pvOut = (char *)pcVal;
    }
    return CFG_OK;
}


#define CFG_TYPE_GET_ITEM(type) \
    {#type, sizeof(#type) - 1, {0}, CFG_##type##ValTypeGet}


STATIC ST_CFG_VAL_TYPE_GET s_astCfgValTypeGet[] =
{
    CFG_TYPE_GET_ITEM(object),
    CFG_TYPE_GET_ITEM(string),
    CFG_TYPE_GET_ITEM(int),
    CFG_TYPE_GET_ITEM(unsignedInt),
    CFG_TYPE_GET_ITEM(boolean),
    CFG_TYPE_GET_ITEM(dateTime),
    CFG_TYPE_GET_ITEM(base64)
};

#define CFG_VAL_TYPE_GET_NUM \
              (sizeof(s_astCfgValTypeGet)/sizeof(s_astCfgValTypeGet[0]))



/***********************************************************
接口:   子函数, 通过节点地址获取一个节点列表值到一个结构体中
参数:   pstNode, 节点地址
        pvStruct, 输出值的结构体地址
        ppcLeafList, 节点列表, 节点名为"" 的将被忽略
        ulLeafNum, 节点个数, 实际上是指 ppcLeafList 中的元素个数
        ucAllString, 0 或者 1, 表示是否全部按照string类型来输出.
                     这个参数的目的是为了加快速度(因为无须判断类型)
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_GetValToStructByPtr(const scew_element *pstNode, void *pvStruct,
                                char * const *ppcLeafList,
                                unsigned long ulLeafNum,
                                unsigned char ucAllString)
{
    scew_element *pstChild = NULL;
    CFG_RET ret = CFG_OK;
    scew_attribute *pstAttr =NULL;
    const char *pcType = NULL;
    const char *pcVal = NULL;
    unsigned long i = 0;
    unsigned long j = 0;
    unsigned long *pulOut = pvStruct;
    unsigned char ucHasDone = 0;

    for (i = 0; i < ulLeafNum; i++)
    {
        if (NULL == ppcLeafList[i])
        {
            CFG_ERR(ERR_CFG_PARA_INVALID);
            return ERR_CFG_PARA_INVALID;
        }

        if ('\0' == ppcLeafList[i][0])
        {
            continue;
        }
        *(pulOut + i) = 0L;
        ucHasDone = 0;

        pstChild = NULL;
        while (pstChild = scew_element_next(pstNode, pstChild),
               NULL != pstChild)
        {
            if (0 == strcmp(scew_element_name(pstChild), ppcLeafList[i]))
            {
                pcVal = scew_element_contents(pstChild);
                if (1 == ucAllString)
                {
                    ucHasDone = 1;
                    ret = CFG_stringValTypeGet(pcVal, pulOut + i);
                    if (CFG_OK != ret)
                    {
                        CFG_ERR(ret);
                        return ret;
                    }
                    break;
                }

                pstAttr = scew_attribute_by_name(pstChild, ATTR_TYPE);
                if (NULL != pstAttr)
                {
                    pcType = scew_attribute_value(pstAttr);
                    for (j = 0; j < CFG_VAL_TYPE_GET_NUM; j++)
                    {
                        if (0 == strncmp(pcType, s_astCfgValTypeGet[j].pcType,
                                          s_astCfgValTypeGet[j].ucTypeLen))
                        {
                            ucHasDone = 1;
                            ret = s_astCfgValTypeGet[j].pfnValTypeGet
                                           (pcVal, pulOut + i);
                            if (CFG_OK != ret)
                            {
                                CFG_ERR(ret);
                                return ret;
                            }
                            break;
                        }
                    }
                }

                break;
            }
        }
        if (0 == ucHasDone)
        {
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", ppcLeafList[i]);
            return ERR_CFG_PATH_NOT_EXSITED;
        }
    }

    return CFG_OK;
}

/***********************************************************
接口:   通过节点路径获取一个节点列表值到一个结构体中,
                输出的值都是根据类型转换后的
参数:   pcPath, 节点路径
        pvStruct, 输出值的结构体地址
        ppcLeafList, 节点列表, 节点名为"" 的将被忽略
        ulLeafNum, 节点个数, 实际上是指 ppcLeafList 中的元素个数
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_GetValToStruct(const char *pcPath, void *pvStruct,
                           char * const *ppcLeafList,
                           unsigned long ulLeafNum)
{
    const scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    ret = CFG_GetValToStructByPtr(pstNode, pvStruct, ppcLeafList, ulLeafNum, 0);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret, "%s", pcPath);
    }

    return ret;
}

/***********************************************************
接口:   通过节点路径获取一个节点列表值到一个结构体中,
                所有节点值按string类型输出
参数:   pcPath, 节点路径
        pvStrArr, 输出值的结构体地址, 输出的都是按string类型
        ppcLeafList, 节点列表, 节点名为"" 的将被忽略
        ulLeafNum, 节点个数, 实际上是指 ppcLeafList 中的元素个数
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_GetValToStrArr(const char *pcPath, void *pvStrArr,
                           char * const *ppcLeafList,
                           unsigned long ulLeafNum)
{
    const scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    ret = CFG_GetValToStructByPtr(pstNode, pvStrArr, ppcLeafList, ulLeafNum, 1);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret, "%s", pcPath);
    }

    return ret;
}


/* 不同类型的设置值的函数 */
typedef CFG_RET (*FUNC_VAL_TYPE_SET)(scew_element *pstNode, const void *pvVal,
                                     unsigned char *pucChanged);


typedef struct
{
    const char *pcType;
    unsigned char ucTypeLen;
    unsigned char aucSpace[3];
    FUNC_VAL_TYPE_SET pfnValTypeSet;

} ST_CFG_VAL_TYPE_SET;



/***********************************************************
接口:   设置对象类节点值
参数:   pstNode, 节点值
        pcVal, 要设置成的值
        pucChanged, 输出值是否被改变, 这个参数的主要用途在于 Notification
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_objectValTypeSet(scew_element *pstNode, const void *pvVal,
                             unsigned char *pucChanged)
{
    (void)pstNode;
    (void)pvVal;
    (void)pucChanged;

    return CFG_OK;
}

/***********************************************************
接口:   设置string类节点值
参数:   pstNode, 节点值
        pcVal, 要设置成的值
        pucChanged, 输出值是否被改变, 这个参数的主要用途在于 Notification
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_stringValTypeSet(scew_element *pstNode, const void *pvVal,
                             unsigned char *pucChanged)
{
    const char *pcNewVal = *(char * const *)pvVal;
    const char *pcOld =  scew_element_contents(pstNode);

    if (NULL == pcNewVal)
    {
        pcNewVal = s_pcCfgStringNull;
    }

    if (NULL == pcOld || 0 != strcmp(pcNewVal, pcOld))
    {
        pcNewVal = scew_element_set_contents(pstNode, pcNewVal);
        if (NULL == pcNewVal)
        {
            return CFG_FAIL;
        }
        *pucChanged = 1;
    }

    return CFG_OK;
}


/***********************************************************
接口:   设置int类节点值
参数:   pstNode, 节点值
        pcVal, 要设置成的值的地址, 这里指向的是一个int型的值
        pucChanged, 输出值是否被改变, 这个参数的主要用途在于 Notification
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_intValTypeSet(scew_element *pstNode, const void *pvVal,
                             unsigned char *pucChanged)
{
    char acIntVal[32];
    const char *pcNewVal = NULL;
    const char *pcOld =  scew_element_contents(pstNode);

    sprintf(acIntVal, "%d", *(int *)pvVal);
    if (NULL == pcOld || 0 != strcmp(acIntVal, pcOld))
    {
        pcNewVal = scew_element_set_contents(pstNode, acIntVal);
        if (NULL == pcNewVal)
        {
            return CFG_FAIL;
        }

        *pucChanged = 1;
    }

    return CFG_OK;
}

/***********************************************************
接口:   设置unsigned int类节点值
参数:   pstNode, 节点值
        pcVal, 要设置成的值的地址, 这里指向的是一个unsigned int型的值
        pucChanged, 输出值是否被改变, 这个参数的主要用途在于 Notification
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_unsignedIntValTypeSet(scew_element *pstNode, const void *pvVal,
                             unsigned char *pucChanged)
{
    char acIntVal[32];
    const char *pcNewVal = NULL;
    const char *pcOld =  scew_element_contents(pstNode);

    sprintf(acIntVal, "%u", *(unsigned int *)pvVal);
    if (NULL == pcOld || 0 != strcmp(acIntVal, pcOld))
    {
        pcNewVal = scew_element_set_contents(pstNode, acIntVal);
        if (NULL == pcNewVal)
        {
            return CFG_FAIL;
        }
        *pucChanged = 1;
    }

    return CFG_OK;
}

/***********************************************************
接口:   设置boolean类节点值
参数:   pstNode, 节点值
        pcVal, 要设置成的值的地址, 这里指向的是一个boolean型的值
        pucChanged, 输出值是否被改变, 这个参数的主要用途在于 Notification
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_booleanValTypeSet(scew_element *pstNode, const void *pvVal,
                             unsigned char *pucChanged)
{
    char acBoolVal[2] = "0";
    const char *pcOld =  scew_element_contents(pstNode);

    if (0 != *(int *)pvVal)
    {
        acBoolVal[0] = '1';
    }
    if (NULL == pcOld || 0 != strcmp(acBoolVal, pcOld))
    {
        (void)scew_element_set_contents(pstNode, acBoolVal);
        *pucChanged = 1;
    }

    return CFG_OK;
}


/***********************************************************
接口:   设置date类节点值
参数:   pstNode, 节点值
        pcVal, 要设置成的值, 这里指向的是一个字符串
        pucChanged, 输出值是否被改变, 这个参数的主要用途在于 Notification
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_dateTimeValTypeSet(scew_element *pstNode, const void *pvVal,
                             unsigned char *pucChanged)
{
    const char *pcNewVal = *(char * const *)pvVal;
    const char *pcOld =  scew_element_contents(pstNode);

    if (NULL == pcNewVal)
    {
        pcNewVal = s_pcCfgStringNull;
    }

    if (NULL == pcOld || 0 != strcmp(pcNewVal, pcOld))
    {
        pcNewVal = scew_element_set_contents(pstNode, pcNewVal);
        if (NULL == pcNewVal)
        {
            return CFG_FAIL;
        }
        *pucChanged = 1;
    }

    return CFG_OK;
}

/***********************************************************
接口:   设置base64类节点值
参数:   pstNode, 节点值
        pcVal, 要设置成的值, 这里指向的是一个字符串
        pucChanged, 输出值是否被改变, 这个参数的主要用途在于 Notification
返回值: 0:成功
备注:
***********************************************************/
CFG_RET CFG_base64ValTypeSet(scew_element *pstNode, const void *pvVal,
                             unsigned char *pucChanged)
{
    const char *pcNewVal = *(char * const *)pvVal;
    const char *pcOld =  scew_element_contents(pstNode);

    if (NULL == pcNewVal)
    {
        pcNewVal = s_pcCfgStringNull;
    }

    if (NULL == pcOld || 0 != strcmp(pcNewVal, pcOld))
    {
        pcNewVal = scew_element_set_contents(pstNode, pcNewVal);
        if (NULL == pcNewVal)
        {
            return CFG_FAIL;
        }
        *pucChanged = 1;
    }

    return CFG_OK;
}


#define CFG_TYPE_SET_ITEM(type) \
    {#type, sizeof(#type) - 1, {0}, CFG_##type##ValTypeSet}


STATIC ST_CFG_VAL_TYPE_SET s_astCfgValTypeSet[] =
{
    CFG_TYPE_SET_ITEM(object),
    CFG_TYPE_SET_ITEM(string),
    CFG_TYPE_SET_ITEM(int),
    CFG_TYPE_SET_ITEM(unsignedInt),
    CFG_TYPE_SET_ITEM(boolean),
    CFG_TYPE_SET_ITEM(dateTime),
    CFG_TYPE_SET_ITEM(base64)
};

#define CFG_VAL_TYPE_SET_NUM \
              (sizeof(s_astCfgValTypeSet)/sizeof(s_astCfgValTypeSet[0]))


/***********************************************************
接口:   子函数, 通过节点地址设置一个节点列表值, 值来源于一个结构体
参数:   pstNode, 节点地址
        pvStruct, 设置值的来源
        ppcLeafList, 节点列表, 节点名为"" 的将被忽略
        ulLeafNum, 节点个数, 实际上是指 ppcLeafList 中的元素个数,
        pstInfo, 节点信息, 主要用来处理 Notification 属性
返回值: 0:成功
        其它: 失败
备注:   该函数内部有对 Notification 做处理
***********************************************************/
CFG_RET CFG_SetValFromStructByPtr(const scew_element *pstNode, const void *pvStruct,
                           char * const *ppcLeafList, unsigned long ulLeafNum,
                           ST_CFG_NODE_INFO *pstInfo)
{
    scew_element *pstChild = NULL;
    CFG_RET ret = CFG_OK;
    scew_attribute *pstAttr =NULL;
    const char *pcType = NULL;
    unsigned long i = 0;
    unsigned long j = 0;
    const unsigned long *pulOut = pvStruct;
    unsigned char ucHasDone = 0;
    unsigned char ucChanged = 0;
    unsigned long ulMask = CFG_NODE_INFO_STANDARD | CFG_NODE_INFO_NOTI;

    for (i = 0; i < ulLeafNum; i++)
    {
        if (NULL == ppcLeafList[i])
        {
            CFG_ERR(ERR_CFG_PARA_INVALID);
            return ERR_CFG_PARA_INVALID;
        }

        if ('\0' == ppcLeafList[i][0])
        {
            continue;
        }
        ucHasDone = 0;
        pstChild = NULL;
        while (pstChild = scew_element_next(pstNode, pstChild),
               NULL != pstChild)
        {
            if (0 == strcmp(scew_element_name(pstChild), ppcLeafList[i]))
            {
                pstAttr = scew_attribute_by_name(pstChild, ATTR_TYPE);
                if (NULL != pstAttr)
                {
                    pcType = scew_attribute_value(pstAttr);
                    for (j = 0; j < CFG_VAL_TYPE_SET_NUM; j++)
                    {
                        if (0 == strncmp(pcType, s_astCfgValTypeSet[j].pcType,
                                          s_astCfgValTypeSet[j].ucTypeLen))
                        {
                            ucHasDone = 1;
                            ret = s_astCfgValTypeSet[j].pfnValTypeSet
                                         (pstChild, pulOut + i, &ucChanged);
                            if (CFG_OK != ret)
                            {
                                CFG_ERR(ret);
                                return ret;
                            }
                            if (1 == ucChanged)
                            {
                                CFG_UpdateNodeInfo(pstChild, pstInfo, ulMask);
                               (void)CFG_NotiInform(pstNode, pstInfo);
                            }

                            break;
                        }
                    }
                }

                break;
            }
        }
        if (0 == ucHasDone)
        {
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", ppcLeafList[i]);
            return ERR_CFG_PATH_NOT_EXSITED;
        }
    }

    return CFG_OK;
}


/***********************************************************
接口:   通过节点路径设置一个节点列表值, 值来源于一个结构体
参数:   pcPath, 节点路径
        pvStruct, 设置值的来源
        ppcLeafList, 节点列表, 节点名为"" 的将被忽略
        ulLeafNum, 节点个数, 实际上是指 ppcLeafList 中的元素个数
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_SetValFromStruct(const char *pcPath, const void *pvStruct,
                           char * const *ppcLeafList,
                           unsigned long ulLeafNum)
{
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;
    ST_CFG_NODE_INFO stNodeInfo;
    unsigned long ulMask = CFG_NODE_INFO_STANDARD | CFG_NODE_INFO_NOTI;

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, &stNodeInfo, ulMask);

    ret = CFG_SetValFromStructByPtr(pstNode, pvStruct,
                  ppcLeafList, ulLeafNum, &stNodeInfo);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret, "%s", pcPath);
    }

    return ret;
}

#if 1


/***********************************************************
接口:   获取一个节点的值的地址, 和类型地址
参数:   pcPath, 节点路径
        ppcVal, 输出指向节点值的指针
        ppcType, 输出指向节点类型的指针, 如果为空, 则不输出类型
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_GetNodeValPtr(const char *pcPath, const char **ppcVal,
                          const char **ppcType)
{
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;
    scew_attribute *pstAttr =NULL;
    const char *pcVal = NULL;

    if (NULL == pcPath)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    if (NULL != ppcVal)
    {
        pcVal = scew_element_contents(pstNode);
        if (NULL == pcVal)
        {
            pcVal = "";
        }

        *ppcVal = pcVal;
    }
    if (NULL != ppcType)
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_TYPE);
        if (NULL != pstAttr)
        {
            *ppcType = scew_attribute_value(pstAttr);
        }
        else
        {
            *ppcType = "";
        }
    }

    return CFG_OK;
}



#endif



#if 1


/***********************************************************
接口:   对单个节点, 如果该节点mid等于指定mid, 则将该节点和值 加入到链表中
参数:   pstNode, 节点地址
        pcAbsPath, 节点全路径
        ucLen, 节点全路径长度
        usMID, 本节点所属的mid
        usMIDBegin, 所指定的mid
        pvPara, 输出的链表的地址
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_XxxSingleNodeVal(const scew_element *pstNode, const char *pcAbsPath,
                 unsigned char ucLen, unsigned short usMID,
                 unsigned short usMIDBegin, void *pvPara)
{
    ST_PARA_VAL **ppstParaValList = (ST_PARA_VAL **)pvPara;
    ST_PARA_VAL *pstParaVal = NULL;
    unsigned long ulBufLen = 0;
    const char *pcVal = NULL;
    scew_attribute *pstAttr =NULL;
    const char *pcType = NULL;
    const char *pcAttrVal = "";

    if (usMID != usMIDBegin)
    {
        return CFG_OK;
    }

    /* 根据类型进行拦截 */
    CFG_FILTER_NON_LEAF(pstNode);
    pcType = pcAttrVal;

    pcVal = scew_element_contents(pstNode);
    if (NULL == pcVal)
    {
        pcVal = "";
    }

    ulBufLen = sizeof(ST_PARA_VAL) + strlen(pcType) + 1
              + ucLen + 1 + strlen(pcVal) + 1;
    pstParaVal = malloc(ulBufLen);
    if (NULL == pstParaVal)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }
    pstParaVal->pcType = (char *)(pstParaVal + 1);
    strcpy(pstParaVal->pcType, pcType);
    pstParaVal->pcName = pstParaVal->pcType + strlen(pstParaVal->pcType) + 1;
    strcpy(pstParaVal->pcName, pcAbsPath);
    pstParaVal->pcVal = pstParaVal->pcName + strlen(pstParaVal->pcName) + 1;
    strcpy(pstParaVal->pcVal, pcVal);

    /* 加入链表 */
    CFG_LIST_APPEND(ppstParaValList, pstParaVal);

    return CFG_OK;
}


#define CFG_TREE_XXX_OPT(pstNode, pfnGetOpt, pcAbsPath, ucLen, usMID, usMIDBegin, pvData) \
{ \
    ret = pfnGetOpt(pstNode, pcAbsPath, ucLen, usMID, usMIDBegin, pvData); \
    if (CFG_OK != ret) \
    { \
        return ret; \
    } \
}

typedef long (*FUNC_XXX_OPT)(const scew_element *pstNode, const char *pcAbsPath,
                 unsigned char ucLen, unsigned short usMID,
                 unsigned short usMIDBegin, void *pvPara);


/***********************************************************
接口:   对子树遍历获取属于指定MID的节点列表
参数:   pstNode, 节点地址
        pstInfo, 节点信息, 这里主要用到其中的usMID, 用来表示pstNode 是属于哪个mid的
        pfnGetOpt, 单个节点的操作函数
        pcAbsPath, 节点全路径的存放地址
        usMID, 指定要获取的MID的节点列表
        pvPara, 输出的链表的地址
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
long CFG_TreeXxxAccess(const scew_element *pstNode, const ST_CFG_NODE_INFO *pstInfo,
                          FUNC_XXX_OPT pfnGetOpt, char *pcAbsPath,
                          unsigned short usMID, void *pvData)
{
    const scew_element *pstNodeWork = pstNode;
    const scew_element *pstChild = NULL;
    const scew_element *pstParent = NULL;
    const char *pcNodeName = NULL;
    CFG_RET ret = CFG_OK;
    ST_CFG_TREE_STACK_NODE astStack[CFG_ACCESS_STACK_DEEPTH];
    unsigned char ucStackTop = 0;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;

    /* 初始化栈 */
    astStack[0].ucPathLen = strlen(pcAbsPath);
    astStack[0].pstPath = pstNodeWork;
    astStack[0].stNodeInfo.usMID = pstInfo->usMID;
    ucStackTop = 1;

    /* 操作 */
    CFG_TREE_XXX_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
            astStack[ucStackTop - 1].ucPathLen, usMID, usMID, pvData);

    while (1)
    {
        if (NULL != pstNodeWork)
        {
            while (pstChild = scew_element_next(pstNodeWork, pstChild),
                   NULL != pstChild)   /* 取子节点, NULL !=子节点 */
            {
                /* 置为子节点 */
                pstNodeWork = pstChild;
                pstChild = NULL;

                /* 更新路径 */
                pcNodeName = scew_element_name(pstNodeWork);
                CFG_NODE_TO_NUM(pcNodeName);
                pcAbsPath[astStack[ucStackTop - 1].ucPathLen] = DELIMIT_C;
                strcpy(pcAbsPath + astStack[ucStackTop - 1].ucPathLen + 1, pcNodeName);

                /* 进栈 */
                astStack[ucStackTop].pstPath = pstNodeWork;
                astStack[ucStackTop].ucPathLen
                    = astStack[ucStackTop - 1].ucPathLen  + strlen(pcNodeName) + 1;
                pstAttr = scew_attribute_by_name(pstNodeWork, ATTR_MID);  /* 取MID */
                if (NULL == pstAttr)
                {
                    astStack[ucStackTop].stNodeInfo.usMID
                        = astStack[ucStackTop- 1].stNodeInfo.usMID;
                }
                else
                {
                    pcAttrVal = scew_attribute_value(pstAttr);
                    astStack[ucStackTop].stNodeInfo.usMID
                         = strtol(pcAttrVal, NULL, CFG_MID_BASE);
                }
                ucStackTop++;

                /* 操作 */
                CFG_TREE_XXX_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
                       astStack[ucStackTop - 1].ucPathLen,
                       astStack[ucStackTop- 1].stNodeInfo.usMID, usMID, pvData);
            }
        }

        /* 栈空 */
        if (1 == ucStackTop)
        {
            break;
        }

        /* 出栈, 恢复路径 */
        ucStackTop--;
        pstNodeWork = astStack[ucStackTop].pstPath;
        pcAbsPath[astStack[ucStackTop].ucPathLen] = '\0';

        /* 取兄弟 */
        pstParent = astStack[ucStackTop - 1].pstPath;
        pstNodeWork = scew_element_next(pstParent, pstNodeWork);
        if (NULL != pstNodeWork)/* 兄弟不为空 */
        {
            /* 更新路径 */
            pcNodeName = scew_element_name(pstNodeWork);
            CFG_NODE_TO_NUM(pcNodeName);
            pcAbsPath[astStack[ucStackTop - 1].ucPathLen] = DELIMIT_C;
            strcpy(pcAbsPath + astStack[ucStackTop - 1].ucPathLen + 1, pcNodeName);

            /* 进栈 */
            astStack[ucStackTop].pstPath = pstNodeWork;
            astStack[ucStackTop].ucPathLen
                = astStack[ucStackTop - 1].ucPathLen  + strlen(pcNodeName) + 1;
            pstAttr = scew_attribute_by_name(pstNodeWork, ATTR_MID);  /* 取MID */
            if (NULL == pstAttr)
            {
                astStack[ucStackTop].stNodeInfo.usMID
                    = astStack[ucStackTop- 1].stNodeInfo.usMID;
            }
            else
            {
                pcAttrVal = scew_attribute_value(pstAttr);
                astStack[ucStackTop].stNodeInfo.usMID
                     = strtol(pcAttrVal, NULL, CFG_MID_BASE);
            }
            ucStackTop++;

            /* 操作 */
            CFG_TREE_XXX_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
                   astStack[ucStackTop - 1].ucPathLen,
                   astStack[ucStackTop- 1].stNodeInfo.usMID, usMID, pvData);
        }
    }

    return CFG_OK;
}





/***********************************************************
接口:   对路径遍历获取属于指定MID的节点列表
参数:   pcPath, 节点路径
        usMID, 指定要获取的MID的节点列表
        ppstParaVal, 输出的链表的地址
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_GetMIDNodes(char *pcPath, unsigned short usMID, ST_PARA_VAL **ppstParaVal)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    ST_PARA_VAL *apstParaValList[2] = {NULL, NULL};
    char acAbsPath[CFG_MAX_PATH_LEN];
    ST_CFG_NODE_INFO stInfo;

    if (NULL == pcPath || NULL == ppstParaVal)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, &stInfo, CFG_NODE_INFO_MID);

    if ('\0' == pcPath[0] || '.' == pcPath[0])
    {
        strcpy(acAbsPath, scew_element_name(pstNode));
    }
    else
    {
        strcpy(acAbsPath, pcPath);
    }

    /* 树全遍历 */
    ret = CFG_TreeXxxAccess(pstNode, &stInfo, CFG_XxxSingleNodeVal,
                    acAbsPath, usMID, apstParaValList);
    if (CFG_OK != ret)
    {
        /* 释放链表 */
        (void)CFG_MemFree(apstParaValList[0]);

        return ret;
    }

    *ppstParaVal = apstParaValList[0];

    return CFG_OK;
}


/***********************************************************
接口:   获取一个节点的全路径, 这里的方法是从当前节点一步步回到根节点
参数:   pstNode, 节点地址
        pcPath, 输出的节点全路径
        pulLen, 输入的是pcPath的可写的长度, 输出的节点路径的长度
返回值: 0:成功
        其它: 失败, 缓冲区不足
备注:   该函数的作用是在对通配路径进行操作时, 省去对每个通配节点保存全路径的缓冲区,
        以时间换空间
***********************************************************/
CFG_RET CFG_GetFullPath(const scew_element *pstNode, char *pcPath,
                        unsigned long *pulLen)
{
    const char *pcName = NULL;
    unsigned long ulNameLen = 0;
    unsigned long ulLen = *pulLen;
    char *pcPathEnd = pcPath + ulLen;

    while (NULL != pstNode)
    {
        pcName = scew_element_name(pstNode);
        NODE_WILDCARD_RCVT(pcName);
        ulNameLen = strlen(pcName);

        if (pcPathEnd < pcPath + ulNameLen + 1)
        {
            CFG_ERR(ERR_CFG_BUF_NOT_ENOUGH);
            return ERR_CFG_BUF_NOT_ENOUGH;
        }
        --pcPathEnd;
        *pcPathEnd = DELIMIT_C;
        pcPathEnd -= ulNameLen;
        memcpy(pcPathEnd, pcName, ulNameLen);

        pstNode = pstNode->parent;
    }

    pcPath[ulLen - 1] = '\0';

    *pulLen = (unsigned long)(pcPath + ulLen - pcPathEnd);

    strcpy(pcPath, pcPathEnd);

    return CFG_OK;
}


/***********************************************************
接口:   对通配路径获取所属指定MID的节点列表,
参数:   pcWildPath, 通配路径
        usMID, 指定的MID
        ppstParaVal, 输出的链表的地址
返回值: 0:成功
        其它: 失败
备注:   这里的通配路径本身所属的MID等于指定的MID
***********************************************************/
CFG_RET CFG_GetWildMIDNodes(char *pcWildPath, unsigned short usMID, ST_PARA_VAL **ppstParaVal)
{
    CFG_RET ret = CFG_OK;

    scew_element *apstTree[CFG_MAX_WILDCARD_NUM];
    unsigned long ulHead = 0;
    unsigned long ulTail = 0;
    unsigned long i = 0;
    unsigned long ulLoopEnd = 0;
    unsigned long ulLoop = 0;
    ST_PARA_VAL *apstParaValList[2] = {NULL, NULL};
    char acFullPath[CFG_MAX_PATH_LEN];
    unsigned long ulLen = 0;
    ST_CFG_NODE_INFO stInfo;

    char *pcPath = pcWildPath;

    if (NULL == pcPath || 0 == usMID)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    ret = CFG_GetWildMIDNodesPtr(pcPath, apstTree, CFG_MAX_WILDCARD_NUM,
                                 &ulHead, &ulTail, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    stInfo.usMID = usMID;

    ulLoopEnd = (ulTail >= ulHead ) ? ulTail : ulTail + CFG_MAX_WILDCARD_NUM;
    for (i = ulHead; i < ulLoopEnd; i++)
    {
        ulLoop = i % CFG_MAX_WILDCARD_NUM;

        ulLen = CFG_MAX_PATH_LEN;
        ret = CFG_GetFullPath(apstTree[ulLoop], acFullPath, &ulLen);
        if (CFG_OK != ret)
        {
            (void)CFG_MemFree((void *)apstParaValList[0]);

            CFG_ERR(ret);
            return ret;
        }

        ret = CFG_TreeXxxAccess(apstTree[ulLoop], &stInfo, CFG_XxxSingleNodeVal,
                                    acFullPath, usMID, apstParaValList);

        if (CFG_OK != ret)
        {
            (void)CFG_MemFree(apstParaValList[0]);

            CFG_ERR(ret);
            return ret;
        }
    }

    *ppstParaVal = apstParaValList[0];

    return CFG_OK;
}




#endif







#if 1
/***********************************************************
接口:   单个节点的复制函数
参数:   pstNode, 节点地址
        ppstDupNode, 复制以后的节点地址
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_DupSingleNode(const scew_element *pstNode, scew_element **ppstDupNode)
{
    const char *pcValNew = NULL;
    scew_element *pstDupNode = *ppstDupNode;
    scew_attribute *pstAttr =NULL;
    scew_attribute *pstAttrOld =NULL;

    if (NULL == pstDupNode)
    {
        pstDupNode = scew_element_create(scew_element_name(pstNode));
    }
    else
    {
        pstDupNode = scew_element_add(pstDupNode, scew_element_name(pstNode));
    }
    if (NULL == pstDupNode)
    {
        /* 报告错误 */
        CFG_ERR(ERR_CFG_INTERNAL_ERR);
        return ERR_CFG_INTERNAL_ERR;
    }

    pcValNew = scew_element_contents(pstNode);
    if (NULL != pcValNew)
    {
        pcValNew = scew_element_set_contents(pstDupNode, pcValNew);
        if (NULL == pcValNew)
        {
            CFG_ERR(ERR_CFG_INTERNAL_ERR);
            return ERR_CFG_INTERNAL_ERR;
        }
    }

    pstAttrOld = NULL;
    while (pstAttrOld = scew_attribute_next(pstNode, pstAttrOld),
           NULL != pstAttrOld)
    {
        CFG_ADDATTR(pstDupNode, scew_attribute_name(pstAttrOld),
                    scew_attribute_value(pstAttrOld), (void)0);
    }

    *ppstDupNode = pstDupNode;

    return CFG_OK;
}


#define CFG_TREE_DUP_OPT(pstNode, pfnSetOpt, pstDupNode, apstDupStack, ucDupStackTop) \
{ \
    ret = pfnSetOpt(pstNode, &pstDupNode); \
    if (CFG_OK != ret) \
    { \
        return ret; \
    } \
    apstDupStack[ucDupStackTop] = pstDupNode; \
    ucDupStackTop++; \
}


/***********************************************************
接口:   复制子树的遍历函数
参数:   pstNode, 节点地址
        ppstDupNode, 复制以后的节点地址
返回值: 0:成功
        其它: 失败
备注:   不复制"i"以下的节点, 既用在生成一个对象实例, 也用在复制一棵数据树
***********************************************************/
CFG_RET CFG_DupAccess(const scew_element *pstNode, scew_element **ppstDupNode)
{
    const scew_element *pstNodeWork = pstNode;
    const scew_element *pstChild = NULL;
    const scew_element *pstParent = NULL;
    const scew_element *apstStack[CFG_ACCESS_STACK_DEEPTH];
    unsigned char ucStackTop = 0;
    scew_element *apstDupStack[CFG_ACCESS_STACK_DEEPTH];
    unsigned char ucDupStackTop = 0;
    scew_element *pstDupNode = NULL;
    CFG_RET ret = CFG_OK;

    apstStack[0] = pstNode;
    ucStackTop++;

    /* 操作 */
    CFG_TREE_DUP_OPT(pstNodeWork, CFG_DupSingleNode, pstDupNode,
                                 apstDupStack, ucDupStackTop);

    while (1)
    {
        if (NULL != pstNodeWork)
        {
            while (pstChild = scew_element_next(pstNodeWork, pstChild),
                   NULL != pstChild)   /* 取子节点, NULL !=子节点 */
            {
                /* 置为子节点 */
                pstNodeWork = pstChild;
                pstChild = NULL;

                /* 进栈 */
                apstStack[ucStackTop] = pstNodeWork;
                ucStackTop++;

                if (0 == strcmp(scew_element_name(pstNodeWork), NODE_WILDCARD))
                {
                    pstNodeWork = NULL;
                    ucDupStackTop++;
                    break;
                }

                /* 操作 */
                CFG_TREE_DUP_OPT(pstNodeWork, CFG_DupSingleNode, pstDupNode,
                                 apstDupStack, ucDupStackTop);
            }
        }

        /* 栈空 */
        if (1 == ucStackTop)
        {
            break;
        }

        /* 出栈, 恢复路径 */
        ucStackTop--;
        pstNodeWork = apstStack[ucStackTop];
        ucDupStackTop--;
        pstDupNode = apstDupStack[ucDupStackTop-1];

        /* 取兄弟 */
        pstParent = apstStack[ucStackTop - 1];
        pstNodeWork = scew_element_next(pstParent, pstNodeWork);
        if (NULL != pstNodeWork)/* 兄弟不为空 */
        {
            /* 进栈 */
            apstStack[ucStackTop] = pstNodeWork;
            ucStackTop++;

            if (0 == strcmp(scew_element_name(pstNodeWork), NODE_WILDCARD))
            {
                pstNodeWork = NULL;
                ucDupStackTop++;
                continue;
            }

            /* 操作 */
            CFG_TREE_DUP_OPT(pstNodeWork, CFG_DupSingleNode, pstDupNode,
                             apstDupStack, ucDupStackTop);
        }
    }

    *ppstDupNode = apstDupStack[0];

    return CFG_OK;
}

/***********************************************************
接口:   获取一个对象所允许的最大的实例个数
参数:   pstObj, 节点地址
        pulCount, 输出的最大的个数,
返回值: 0:成功
        其它: 失败
备注:   该函数的实现是取节点的MaxNode属性,然后已经该属性值找到指定最大值的节点,
        然后将节点值读出来, 转成unsigned int
***********************************************************/
CFG_RET CFG_GetObjMaxAllowCount(const scew_element *pstObj,
                               unsigned long *pulCount)
{
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    const scew_element *pstNode = NULL;
    const char *pcVal = NULL;

    pstAttr = scew_attribute_by_name(pstObj, ATTR_MAX_NODE);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
        pstNode = scew_element_by_name(pstObj->parent, pcAttrVal);
        if (NULL == pstNode)
        {
            /* 返回失败 */
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pcAttrVal);
            return ERR_CFG_PATH_NOT_EXSITED;
        }

        pcVal = scew_element_contents(pstNode);
        *pulCount = (unsigned long)strtol(pcVal, NULL, 10);
    }
    else
    {
        *pulCount = (unsigned long)~0;
    }

    return CFG_OK;
}

/***********************************************************
接口:   更新一个对象当前的实例个数
参数:   pstObj, 节点地址
返回值: 0:成功
        其它: 失败
备注:   该函数的实现是取节点的CurNode属性,然后已经该属性值找到该节点,
        然后将该节点的值设置成当前对象的子树个数
***********************************************************/
CFG_RET CFG_UpdateObjCurInstCount(const scew_element *pstObj)
{
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    scew_element *pstNode = NULL;
    char acVal[32];

    pstAttr = scew_attribute_by_name(pstObj, ATTR_CUR_NODE);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
        pstNode = scew_element_by_name(pstObj->parent, pcAttrVal);
        if (NULL == pstNode)
        {
            /* 返回失败 */
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pcAttrVal);
            return ERR_CFG_PATH_NOT_EXSITED;
        }

        sprintf(acVal, "%lu", (unsigned long)scew_element_count(pstObj));

        (void)scew_element_set_contents(pstNode, acVal);
    }

    return CFG_OK;
}



/* *pulIndex = 0, 表示自动生成; 将输出实际的结果 */
/***********************************************************
接口:   为一个对象路径自动添加一个实例
参数:   pcPath, 节点路径
        pulIndex, 当*pulIndex = 0, 表示自动生成, 将输出实际的结果
                  否则, 新实例的索引按照 *pulIndex 来生成
返回值: 0:成功
        其它: 失败
备注:   不会添加"i"以下的节点
***********************************************************/
CFG_RET CFG_AddObjInst(const char *pcPath, unsigned long *pulIndex)
{
    char *pszTmp = NULL;
    char *pcPathTmp = NULL;
    scew_element *pstChild = NULL;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    long ret = CFG_OK;

    scew_element *pstBig = NULL;
    scew_element *pstMatch = NULL;

    unsigned long ulIndex = 0;
    char acIndexNode[32];
    unsigned long ulMax = 0;

    if (NULL == pcPath || NULL == pulIndex)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    ulIndex = *pulIndex;

    pstBig = scew_tree_root(g_pstCfgTree);
    pstMatch = scew_tree_root(CFG_GetMIDTree());

    /* 复制路径 */
    CFG_DUP_PATH(pcPathTmp, pcPath);

    pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp);

    if (NULL != pszTmp)
    {
        /* 如果有提供节点路径, 检查根节点是否匹配 */
        CFG_MATCH_ROOT(pszTmp, pcPathTmp, pcPath, pstBig);

        /* 逐级解析路径 */
        while (pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp),
               NULL != pszTmp)
        {
            /* 增加 对 pszTmp 作转义 */
            CFG_NUM_TO_NODE(pszTmp);

            /* 取得小树上匹配分支 */
            if (NULL != pstMatch)
            {
                if (CFG_IS_NODE_NUM(pszTmp))  /* 当前节点是数字 */
                {
                    /* 直接查找通配分支 */
                    pstMatch = scew_element_by_name(pstMatch, NODE_WILDCARD);
                }
                else
                {
                    /* 精确查找 */
                    pstMatch = scew_element_by_name(pstMatch, pszTmp);
                }
            }
            /* 在大树上寻找节点 */
            pstChild = scew_element_by_name(pstBig, pszTmp);
            if (NULL == pstChild)  /* 不存在 */
            {
                CFG_FREE_PATH(pcPathTmp);

                CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s %s", pszTmp, pcPath);
                return ERR_CFG_PATH_NOT_EXSITED;
            }
            pstBig = pstChild;
        }
    }

    CFG_FREE_PATH(pcPathTmp);

    /* 判断小树上是否有i */
    if (NULL != pstMatch)
    {
        pstMatch = scew_element_by_name(pstMatch, NODE_WILDCARD);
    }
    if (NULL == pstMatch || 0 == scew_element_count(pstMatch))
    {
        /* 返回错误 */
        CFG_ERR(ERR_CFG_INVALID_PATH, "%s", pcPath);
        return ERR_CFG_INVALID_PATH;
    }

    /* 取节点的最大允许个数 */
    ret = CFG_GetObjMaxAllowCount(pstBig, &ulMax);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret, "%s", pcPath);
        return ret;
    }
    if (ulMax <= scew_element_count(pstBig))  /* 最大允许个数 <= 当前个数 */
    {
        /* 返回失败 */
        CFG_ERR(ERR_CFG_OBJ_INST_FULL, "%lu %lu %s", ulMax,
                      (unsigned long)scew_element_count(pstBig), pcPath);
        return ERR_CFG_OBJ_INST_FULL;
    }

    if (0 == ulIndex)
    {
        /* 取得maxidx */
        pstAttr = scew_attribute_by_name(pstBig, ATTR_MAX_IDX);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            ulIndex = (unsigned long)strtol(pcAttrVal, NULL, 10);
        }
        ulIndex++;
    }
    sprintf(acIndexNode, "%lu", ulIndex);

    /* 在当前节点上查找节点 */
    pstChild = scew_element_by_name(pstBig, acIndexNode);
    if (NULL != pstChild)  /* 已存在 */
    {
        /* 报告错误 */
        CFG_ERR(ERR_CFG_INTERNAL_ERR);
        return ERR_CFG_INTERNAL_ERR;
    }

    /* 复制结构 */
    ret = CFG_DupAccess(pstMatch, &pstChild);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* 修改节点名 */
    (void)scew_element_set_name(pstChild, acIndexNode);

    /* 添加到大树上 */
    (void)scew_element_add_elem(pstBig, pstChild);

    ret = CFG_UpdateMaxIdx(pstBig);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* 更新当前个数 */
    (void)CFG_UpdateObjCurInstCount(pstBig);

    /* 输出索引 */
    *pulIndex = ulIndex;

    return CFG_OK;
}


typedef struct tagST_CFG_MID_PATH
{
    unsigned short usMID;
    unsigned char aucSpace[2];

    char *pcPath;

    struct tagST_CFG_MID_PATH *pstNext;

} ST_CFG_MID_PATH;


STATIC ST_CFG_MID_PATH *g_pstCfgMIDPathHead = NULL;
STATIC ST_CFG_MID_PATH *g_pstCfgMIDPathTail = NULL;

/***********************************************************
接口:   添加一个MID对应的注册路径
参数:   pcPath, 注册路径
        usMID,  路径所属的MID
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_AddMIDPath(const char *pcPath, unsigned short usMID)
{
    ST_CFG_MID_PATH *pstMIDPath = g_pstCfgMIDPathHead;
    unsigned long ulLen = 0;

    while (NULL != pstMIDPath)
    {
        if (!strcmp(pstMIDPath->pcPath, pcPath))
        {
            pstMIDPath->usMID = usMID;
            return CFG_OK;
        }

        pstMIDPath = pstMIDPath->pstNext;
    }

    ulLen = sizeof(ST_CFG_MID_PATH) + strlen(pcPath) + 1;
    pstMIDPath = malloc(ulLen);
    if (NULL == pstMIDPath)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }

    pstMIDPath->pcPath = (char *)(pstMIDPath + 1);

    pstMIDPath->usMID = usMID;
    strcpy(pstMIDPath->pcPath, pcPath);
    pstMIDPath->pstNext = NULL;

    if (NULL == g_pstCfgMIDPathHead)
    {
        g_pstCfgMIDPathHead = pstMIDPath;
    }
    else
    {
        g_pstCfgMIDPathTail->pstNext = pstMIDPath;
    }
    g_pstCfgMIDPathTail = pstMIDPath;

    return CFG_OK;
}

/***********************************************************
接口:   链表连接, 将pstValList 链表连接在 ppstValTotalList 尾部
参数:   ppstValTotalList, 宿主链表 第一个元素表示头, 第二个元素标志尾
        pstValList,  要添加的连表
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
void CFG_ValListCat(ST_PARA_VAL **ppstValTotalList, ST_PARA_VAL *pstValList)
{
    ST_PARA_VAL *pstValListTail = pstValList;

    if (NULL == pstValList)
    {
        return;
    }

    while (NULL != pstValListTail->pstNext)
    {
        pstValListTail = pstValListTail->pstNext;
    }

    if (NULL == ppstValTotalList[1])
    {
        ppstValTotalList[0] = pstValList;
        ppstValTotalList[1] = pstValListTail;
    }
    else
    {
        ppstValTotalList[1]->pstNext = pstValList;
        ppstValTotalList[1] = pstValListTail;
    }
}


/***********************************************************
接口:   列出配置树中指定MID的所有节点
参数:   usMID, 指定的MID
        ppstParaVal, 输出节点列表
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_ListMIDAllNodes(unsigned short usMID, ST_PARA_VAL **ppstParaVal)
{
    ST_CFG_MID_PATH *pstMIDPath = g_pstCfgMIDPathHead;
    CFG_RET ret = CFG_OK;
    ST_PARA_VAL *apstValTotalList[2] = {NULL, NULL};
    ST_PARA_VAL *pstValList = NULL;

    while (NULL != pstMIDPath)
    {
        if (pstMIDPath->usMID == usMID)
        {
            ret = CFG_GetWildMIDNodes(pstMIDPath->pcPath, usMID, &pstValList);
            if (CFG_OK != ret)
            {
                /* 释放原来的总表 */
                (void)CFG_MemFree(apstValTotalList[0]);

                CFG_ERR(ret);
                return ret;
            }

            /* 将输出的链表串在总链表中 */
            CFG_ValListCat(apstValTotalList, pstValList);
        }

        pstMIDPath = pstMIDPath->pstNext;
    }

    *ppstParaVal = apstValTotalList[0];

    return CFG_OK;
}


#endif

/*
1. get: 取逻辑树
2. Add: 取数据树

*/

STATIC unsigned short *s_pusCfgMIDList = NULL;
STATIC unsigned long s_ulCfgMIDCount = 0;

/***********************************************************
接口:   将MID添加到MID列表中,
参数:   usMID, 指定的MID
返回值: 无
备注:   这个函数主要用来获取一个子树下所有的MID,
        第一个元素存放其所属的MID
***********************************************************/
void CFG_AddMIDToList(unsigned short usMID)
{
    unsigned long i = 0;

    for (i = 0; i < s_ulCfgMIDCount; i++)
    {
        if (usMID == s_pusCfgMIDList[i])
        {
            return;
        }
    }

    s_pusCfgMIDList[s_ulCfgMIDCount++] = usMID;
}

/***********************************************************
接口:   初始化MID列表
参数:   pusMIDList, 实际存放列表的地址
        usMID, 第一个MID
返回值: 无
备注:
***********************************************************/
void CFG_InitMIDList(unsigned short *pusMIDList, unsigned short usMID)
{
    s_pusCfgMIDList = pusMIDList;
    if (0 != usMID)
    {
        s_ulCfgMIDCount = 1;
        s_pusCfgMIDList[0] = usMID;
    }
    else
    {
        s_ulCfgMIDCount = 0;
    }
}


/***********************************************************
接口:   获取单个节点的MID列表
参数:   pstNode, 节点地址
        pvPara, 无意义
返回值: 0: 成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_GetSingleNodeMID(const scew_element *pstNode, void *pvPara)
{
    scew_attribute *pstAttr = NULL;
    const char *pcAttrVal = NULL;
    unsigned short usMID = 0;

    (void)pvPara;

    pstAttr = scew_attribute_by_name(pstNode, ATTR_MID);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
        usMID = strtol(pcAttrVal, NULL, 16);
        if (0 != usMID)
        {
            CFG_AddMIDToList(usMID);
        }
    }

    return CFG_OK;
}



/***********************************************************
接口:   从数据树上获取一个路径下所有的MID
参数:   pcPath, 节点路径
        pusMIDList, 输出MID列表
        pulCount, 输出mid的个数
返回值: 0: 成功
        其它: 失败
备注:   主要用在删除一个对象实例时, 需要通知其下的所有模块
***********************************************************/
CFG_RET CFG_GetObjMIDListFromDataTree(const char *pcPath,
                unsigned short *pusMIDList, unsigned long *pulCount)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    ST_CFG_NODE_INFO stNodeInfo;
    unsigned long ulMask = CFG_NODE_INFO_MID;


    if (NULL == pcPath || NULL == pusMIDList)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, &stNodeInfo, ulMask);

    CFG_InitMIDList(pusMIDList, stNodeInfo.usMID);

    ret = CFG_TreeSetAccess(pstNode, CFG_GetSingleNodeMID, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    *pulCount = s_ulCfgMIDCount;

    return ret;
}



/***********************************************************
接口:   从逻辑树上获取一个路径下所有的MID
参数:   pcPath, 节点路径
        pusMIDList, 输出MID列表
        pulCount, 输出mid的个数
返回值: 0: 成功
        其它: 失败
备注:   主要用在添加一个对象实例时, 需要通知其下的所有模块
***********************************************************/
CFG_RET CFG_GetObjMIDListFromLogicTree(const char *pcPath,
                unsigned short *pusMIDList, unsigned long *pulCount)
{
    char *pszTmp = NULL;
    char *pcPathTmp = NULL;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    long ret = CFG_OK;

    scew_element *pstMatch = NULL;
    unsigned short usMID = 0;

    if (NULL == pcPath || NULL == pusMIDList || NULL == pulCount)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    pstMatch = scew_tree_root(CFG_GetMIDTree());

    /* 复制路径 */
    CFG_DUP_PATH(pcPathTmp, pcPath);

    pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp);

    if (NULL != pszTmp)
    {
        /* 如果有提供节点路径, 检查根节点是否匹配 */
        CFG_MATCH_ROOT(pszTmp, pcPathTmp, pcPath, pstMatch);

        /* 逐级解析路径 */
        while (pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp),
               NULL != pszTmp)
        {
            /* 增加 对 pszTmp 作转义 */
            CFG_NUM_TO_NODE(pszTmp);

            /* 取得小树上匹配分支 */
            if (NULL != pstMatch)
            {
                if (CFG_IS_NODE_NUM(pszTmp))  /* 当前节点是数字 */
                {
                    /* 直接查找通配分支 */
                    pstMatch = scew_element_by_name(pstMatch, NODE_WILDCARD);
                }
                else
                {
                    /* 精确查找 */
                    pstMatch = scew_element_by_name(pstMatch, pszTmp);
                }
            }
            if (NULL == pstMatch)
            {
                CFG_FREE_PATH(pcPathTmp);

                CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s %s ", pszTmp, pcPath);
                return ERR_CFG_PATH_NOT_EXSITED;
            }

            pstAttr = scew_attribute_by_name(pstMatch, ATTR_MID);
            if (NULL != pstAttr)
            {
                pcAttrVal = scew_attribute_value(pstAttr);
                usMID = strtol(pcAttrVal, NULL, 16);
            }
        }
    }
    CFG_FREE_PATH(pcPathTmp);

    CFG_InitMIDList(pusMIDList, usMID);

    ret = CFG_TreeSetAccess(pstMatch, CFG_GetSingleNodeMID, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    *pulCount = s_ulCfgMIDCount;

    return CFG_OK;
}

/***********************************************************
接口:   获取对象下的mid列表, 该函数是前面两个函数的封装
参数:   ucTree, 树的类型
        pcPath, 节点路径
        pusMIDList, 输出MID列表
        pulCount, 输出mid的个数
返回值: 0: 成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_GetObjMIDList(unsigned char ucTree, const char *pcPath,
                unsigned short *pusMIDList, unsigned long *pulCount)
{
    g_pcCfgAccessor = NULL;

    if (CFG_DATA_TREE == ucTree)
    {
        return CFG_GetObjMIDListFromDataTree(pcPath, pusMIDList, pulCount);
    }
    else
    {
        return CFG_GetObjMIDListFromLogicTree(pcPath, pusMIDList, pulCount);
    }
}


CFG_RET CFG_GetNodeMIDFromFullTree(const char *pcPath, unsigned short *pusMID)
{
    CFG_RET ret = CFG_OK;
    unsigned short ausMID[64] = {0};
    unsigned long ulCount = 0;

    ret = CFG_GetObjMIDListFromLogicTree(pcPath, ausMID, &ulCount);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }
    *pusMID = ausMID[0];

    return CFG_OK;
}



/***********************************************************
接口:   设置节点名称
参数:   pcPath, 节点路径
        pcNewName, 新的名称
返回值: 0: 成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_SetNodeName(const char *pcPath, const char *pcNewName)
{
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;
    const char *pcName = NULL;

    if (NULL == pcPath || NULL == pcNewName)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    pcName = scew_element_set_name(pstNode, pcNewName);
    if (NULL == pcName)
    {
        CFG_ERR(ERR_CFG_INTERNAL_ERR);
        return ERR_CFG_INTERNAL_ERR;
    }

    return CFG_OK;
}

/***********************************************************
接口:   通过节点地址设置节点名称
参数:   pvNode, 节点地址
        pcNewName, 新的名称
返回值: 0: 成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_SetNodeNameByPtr(void *pvNode, const char *pcNewName)
{
    const char *pcName = NULL;

    pcName = scew_element_set_name(pvNode, pcNewName);
    if (NULL == pcName)
    {
        CFG_ERR(ERR_CFG_INTERNAL_ERR);
        return ERR_CFG_INTERNAL_ERR;
    }

    return CFG_OK;
}


/***********************************************************
接口:   将节点路径对应的子树切下来
参数:   pcPath, 节点路径
        ppvNode, 切下来的子树地址
返回值: 0: 成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_DelNodeToPtr(const char *pcPath, void **ppvNode)
{
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;

    if (NULL == pcPath || NULL == ppvNode)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    if (NULL != pstNode->parent)
    {
        pstNode->parent->n_children--;
        if (pstNode == pstNode->parent->last_child)
        {
            pstNode->parent->last_child = pstNode->left;
        }
        if (pstNode == pstNode->parent->child)
        {
            pstNode->parent->child = pstNode->right;
        }
    }
    if (NULL != pstNode->left)
    {
        pstNode->left->right = pstNode->right;
    }
    if (NULL != pstNode->right)
    {
        pstNode->right->left = pstNode->left;
    }
    pstNode->parent = NULL;
    pstNode->right = NULL;
    pstNode->left = NULL;

    *ppvNode = pstNode;

    return CFG_OK;
}

/***********************************************************
接口:   将子树添加到指定的路径下
参数:   pcPath, 节点路径
        pvNode, 要添加的子树
返回值: 0: 成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_AddNodeFromPtr(const char *pcPath, void *pvNode)
{
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;

    if (NULL == pcPath || NULL == pvNode)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    (void)scew_element_add_elem(pstNode, pvNode);

    return CFG_OK;
}



/***********************************************************
接口:   获取通配路径的节点地址列表
参数:   pcWildPath, 节点通配路径
        apstTree, 输出节点地址列表,
        ulBufNum, apstTree 可输出的最大个数
        pulHead, 列举完成后数组的起始元素下标
        pulTail, 列举完成后数组的结束元素下标
        pstMatchInfo, 存储通配路径中通配符的位置信息, 这个用来在后面将所有节点的通配的index找出来
返回值: 0: 成功
        其它: 失败
备注:   该函数内部用了一中类似环形队列或者说是滚动队列的方法
***********************************************************/
CFG_RET CFG_GetWildMIDNodesPtr(const char *pcWildPath, scew_element **apstTree,
                               unsigned long ulBufNum,
                               unsigned long *pulHead, unsigned long *pulTail,
                               ST_CFG_MATCH_TREE_INFO *pstMatchInfo)
{
    char *pszTmp = NULL;
    char *pcPathTmp = NULL;
    scew_element* pstNode = NULL;
    scew_element* pstChild = NULL;
    long ret = CFG_OK;

    unsigned long ulHead = 0;
    unsigned long ulTail = 0;
    unsigned long ulNewTail = 0;
    unsigned long i = 0;
    unsigned long ulLoopEnd = 0;
    unsigned long ulLoop = 0;

    unsigned char ucTotalLevel = 0;
    unsigned char ucLevelCount = 0;

    const char *pcPath = pcWildPath;

    pstNode = scew_tree_root(g_pstCfgTree);
    apstTree[ulTail] = pstNode;
    ulTail++;

    /* 复制路径 */
    CFG_DUP_PATH(pcPathTmp, pcPath);

    pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp);

    if (NULL != pszTmp)
    {
        /* 如果有提供节点路径, 检查根节点是否匹配 */
        CFG_MATCH_ROOT(pszTmp, pcPathTmp, pcPath, pstNode);
        ucTotalLevel++;

        while (pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp),
               NULL != pszTmp)
        {
            ucTotalLevel++;
            if (0 == strcmp(pszTmp, NODE_WILDCARD_EXP))  /* 通配符 */
            {
                if (NULL != pstMatchInfo)
                {
                    pstMatchInfo->aucLevelList[ucLevelCount++] = ucTotalLevel;
                }

                ulLoopEnd = (ulTail >= ulHead ) ? ulTail : ulTail + ulBufNum;
                ulNewTail = ulTail;
                for (i = ulHead; i < ulLoopEnd; i++)
                {
                    ulLoop = i % ulBufNum;

                    pstChild = NULL;
                    while (pstChild = scew_element_next(apstTree[ulLoop], pstChild),
                           NULL != pstChild)
                    {
                        if (ulNewTail == ulLoop)
                        {
                            /* 报告队列已满 */
                            CFG_FREE_PATH(pcPathTmp);
                            return ERR_CFG_WILDCARD_QUE_FULL;
                        }

                        /* 添加新指针 */
                        apstTree[ulNewTail] = pstChild;
                        ulNewTail = (ulNewTail + 1) % ulBufNum;
                    }  /* while */
                } /* for */
            }
            else  /* 非通配节点 */
            {
                ulLoopEnd = (ulTail >= ulHead ) ? ulTail : ulTail + ulBufNum;
                ulNewTail = ulTail;
                for (i = ulHead; i < ulLoopEnd; i++)
                {
                    ulLoop = i % ulBufNum;

                    /* 增加 对 pszTmp 作转义 */
                    CFG_NUM_TO_NODE(pszTmp);

                    pstChild = scew_element_by_name(apstTree[ulLoop], pszTmp);
                    if (NULL == pstChild)
                    {
                        continue;
                    }

                    apstTree[ulNewTail] = pstChild;
                    ulNewTail = (ulNewTail + 1) % ulBufNum;
                } /* for */
            }

            ulHead = ulTail;
            ulTail = ulNewTail;
        }
    }

    CFG_FREE_PATH(pcPathTmp);

    *pulHead = ulHead;
    *pulTail = ulTail;

    if (NULL != pstMatchInfo)
    {
        pstMatchInfo->ucTotalLevel = ucTotalLevel;
        pstMatchInfo->ucLevelCount = ucLevelCount;
    }

    return CFG_OK;
}


/* 非递归遍历树的栈 */
extern ST_CFG_TREE_STACK_NODE s_astCfgTreeStack[CFG_ACCESS_STACK_DEEPTH];

extern unsigned char s_ucCfgTreeStackPos;    /* 非递归遍历树的栈顶 */

/* 非递归操作的时存放绝对路径 */
extern char  s_acCfgSavePath[CFG_MAX_PATH_LEN];

/*
STATIC ST_CFG_MID_PATH *s_pstCfgCurMatchPath = NULL;
STATIC scew_element *s_apstCfgTreeList[CFG_MAX_WILDCARD_NUM];
STATIC unsigned long s_ulCfgTreeHead = 0;
STATIC unsigned long s_ulCfgTreeTail = 0;
STATIC unsigned long s_ulCfgTreeCur = 0;
*/
STATIC const scew_element *s_pstCfgCurNode = NULL;
STATIC unsigned short s_usCfgListMID = 0;





/***********************************************************
接口:   为列举指定mid的所有节点而做的准备工作
参数:   usMID, 指定的mid
返回值: 无
备注:
***********************************************************/
void CFG_MIDInstListBegin(unsigned short usMID)
{
    char *pcParh = NULL;

    s_usCfgListMID = usMID;
    (void)CFG_SetMIDAccessInfo(".", (const void **)(void *)&s_pstCfgCurNode,
                               &usMID, NULL, &pcParh);
}


/***********************************************************
接口:   为列举指定mid的所有节点而做的准备工作, 该函数可以指定路径
参数:   usMID, 指定的mid
        pcPath, 指定的路径
返回值: 无
备注:
***********************************************************/
void CFG_MIDInstListBeginEx(unsigned short usMID, const char *pcPath)
{
    char *pcParh = NULL;

    s_usCfgListMID = usMID;
    (void)CFG_SetMIDAccessInfo(pcPath, (const void **)(void *)&s_pstCfgCurNode,
                               &usMID, NULL, &pcParh);
}




/***********************************************************
接口:   获取一个节点下的子节点中同属于指定mid的节点列表, 同时,
        获取本节点中下一个对象节点
参数:   pstCurNode, 当前节点地址
        pcAbsPath, 当前节点绝对路径存放地址,
        ucLen, 当前节点绝对路径长度,
        usCurMID, 当前节点所属于的MID,
        ppstParaVal, 输出节点列表,
        ppstFirstChildObj, 输出其下的第一个对象节点地址,
        pusNextMID, 输出第一个对象节点地址的MID
返回值: 0, 成功
        其它, 失败
备注:   一般来说, 配置树中的所有节点在排列时遵循一个规律, 叶子在前面, 对象在后面,
        但本函数考虑到意外情况, 所以采取了遍历其下层所有节点的做法,
***********************************************************/
CFG_RET CFG_NodesLeafList(const scew_element *pstCurNode,
                          char *pcAbsPath,  unsigned ucLen,
                          unsigned short usCurMID,
                          ST_PARA_VAL **ppstParaVal,
                          const scew_element **ppstFirstChildObj,
                          unsigned short *pusNextMID)
{
    const scew_element *pstNodeWork = pstCurNode;
    scew_element *pstChild = NULL;
    scew_element *pstChildPrev = NULL;
    const char *pcNodeName = NULL;
    scew_attribute *pstAttr =NULL;
    const char *pcType = "";
    const char *pcMID = "";
    const char *pcVal = "";
    ST_PARA_VAL *pstParaVal = NULL;
    unsigned long ulBufLen = 0;
    ST_PARA_VAL *apstParaValList[2] = {NULL, NULL};

    unsigned short usMID = usCurMID;
    unsigned char ucHasGetObj = 0;
    unsigned char ucNameLen = 0;

    while (pstChild = scew_element_next(pstNodeWork, pstChildPrev),
           NULL != pstChild)
    {
        pstChildPrev = pstChild;
        usMID = usCurMID;

        /* 取节点类型 */
        pstAttr = scew_attribute_by_name(pstChild, ATTR_TYPE);
        if (NULL != pstAttr)
        {
            pcType = scew_attribute_value(pstAttr);
        }

        /* 取mid */
        pstAttr = scew_attribute_by_name(pstChild, ATTR_MID);
        if (NULL != pstAttr)
        {
            pcMID = scew_attribute_value(pstAttr);
            usMID = strtol(pcMID, NULL, 16);
        }

        if (0 == strcmp(pcType, TYPE_OBJECT)) /* 对象 */
        {
            if (0 == ucHasGetObj) /* 还没有取到对象节点 */
            {
                /* 设置标志 */
                ucHasGetObj = 1;
                *ppstFirstChildObj = pstChild;
                *pusNextMID = usMID;
            }
        }
        else
        {
            if (usMID == s_usCfgListMID) /* mid 一致 */
            {
                /* 对节点名进行转义 */
                pcNodeName = scew_element_name(pstChild);
                CFG_NODE_TO_NUM(pcNodeName);

                ucNameLen = strlen(pcNodeName);

                /* 更新绝对路径 */
//                pcAbsPath[ucLen] = DELIMIT_C;
                strcpy(&pcAbsPath[ucLen], pcNodeName);

                pcVal = scew_element_contents(pstChild);
                if (NULL == pcVal)
                {
                    pcVal = "";
                }

                /* 操作 */
                ulBufLen = sizeof(ST_PARA_VAL) + strlen(pcType) + 1
                          + ucLen + ucNameLen + 1 + strlen(pcVal) + 1;
                pstParaVal = malloc(ulBufLen);
                if (NULL == pstParaVal)
                {
                    CFG_ERR(ERR_CFG_MALLOC_FAIL);
                    return ERR_CFG_MALLOC_FAIL;
                }
                pstParaVal->pcType = (char *)(pstParaVal + 1);
                strcpy(pstParaVal->pcType, pcType);
                pstParaVal->pcName = pstParaVal->pcType + strlen(pstParaVal->pcType) + 1;
                strcpy(pstParaVal->pcName, pcAbsPath);
                pstParaVal->pcVal = pstParaVal->pcName + ucNameLen + ucLen + 1;
                strcpy(pstParaVal->pcVal, pcVal);

                /* 加入链表 */
                CFG_LIST_APPEND(apstParaValList, pstParaVal);
            }
        }
    }

    *ppstParaVal = apstParaValList[0];

    return CFG_OK;
}

/***********************************************************
接口:   获取本节点同级的下一个兄弟节点
参数:   pstCurNode, 当前节点地址
返回值: 非空, 表示下一个兄弟节点
        空, 则表示没有下一个兄弟了
备注:
***********************************************************/
const scew_element *CFG_GetNextBratherObj(const scew_element *pstCurNode)
{
    const scew_element *pstParent = s_astCfgTreeStack[s_ucCfgTreeStackPos - 1].pstPath;
    const scew_element *pstNext = pstCurNode;
    scew_attribute *pstAttr = NULL;
    const char *pcType = "";

    while (pstNext = scew_element_next(pstParent, pstNext),
           NULL != pstNext)
    {
        pstAttr = scew_attribute_by_name(pstNext, ATTR_TYPE);
        if (NULL != pstAttr)
        {
            pcType = scew_attribute_value(pstAttr);
            if (0 == strcmp(pcType, TYPE_OBJECT)) /* 对象 */
            {
                return pstNext;
            }
        }
    }

    return NULL;
}



/***********************************************************
接口:   遍历函数, 获取下一个对象下 属于指定mid的节点列表
参数:   ppstParaVal, 输出节点列表, 如果为空, 则意味着已经遍历结束
返回值: 0, 成功
        其它, 失败
备注:   该函数调用前须调用 CFG_MIDInstListBegin 系列
***********************************************************/
CFG_RET CFG_ListMIDNextInstNodes(ST_PARA_VAL **ppstParaVal)
{
    const scew_element *pstNext = NULL;
    CFG_RET ret = CFG_OK;
    ST_PARA_VAL *pstParaVal = NULL;
    unsigned short usMID = 0;
    unsigned short usNextMID = 0;
    unsigned char ucLen = 0;

    *ppstParaVal = NULL;

getlist:
    if (NULL != s_pstCfgCurNode) /* NULL != 当前节点 */
    {
        /* 注: 在删除 CFG_GetNextMID 后, 此函数中可删除对类型进行判断 */
        CFG_GetPathLenAndMID(s_pstCfgCurNode, &usMID, &ucLen);

        /* 取列表 */
        pstNext = NULL;
        ret = CFG_NodesLeafList(s_pstCfgCurNode, s_acCfgSavePath, ucLen,
                                usMID, &pstParaVal, &pstNext, &usNextMID);
        if (CFG_OK != ret)
        {
            CFG_ERR(ret);
            return ret;
        }

        /* 进栈 */
        s_astCfgTreeStack[s_ucCfgTreeStackPos].pstPath = s_pstCfgCurNode;
        s_astCfgTreeStack[s_ucCfgTreeStackPos].stNodeInfo.usMID = usMID;
        s_astCfgTreeStack[s_ucCfgTreeStackPos].ucPathLen = ucLen;
        s_ucCfgTreeStackPos++;

        /* 移动到next */
        s_pstCfgCurNode = pstNext;

        if (NULL == pstParaVal) /* 列表为空 */
        {
            goto getlist;
        }
        else
        {
            /* 返回 */
            *ppstParaVal = pstParaVal;
            return CFG_OK;
        }
    }

pop:
    /* 出栈 */
    --s_ucCfgTreeStackPos;
    if (1 >= s_ucCfgTreeStackPos)  /* 栈空 */
    {
        return CFG_OK;
    }

    s_pstCfgCurNode = s_astCfgTreeStack[s_ucCfgTreeStackPos].pstPath;
    usMID = s_astCfgTreeStack[s_ucCfgTreeStackPos].stNodeInfo.usMID;
    ucLen = s_astCfgTreeStack[s_ucCfgTreeStackPos].ucPathLen;
    s_acCfgSavePath[ucLen] = '\0';

    /* 取同级对象兄弟 */
    s_pstCfgCurNode = CFG_GetNextBratherObj(s_pstCfgCurNode);
    if (NULL == s_pstCfgCurNode)/* 为空 */
    {
        goto pop;
    }

    goto getlist;

    /* return CFG_OK; */
}


/***********************************************************
接口:   填充单个节点到数据树上
参数:   pstNode, 逻辑树上的节点地址
        pvData, 无意义
返回值: 0, 成功
        其它, 失败
备注:   逻辑树上一个节点可能对应数据树上多个实例下的节点,
        函数考虑了这一点
***********************************************************/
CFG_RET CFG_FillSingleNode(const scew_element *pstNode, void *pvData)
{
    char acPath[CFG_MAX_PATH_LEN];
    unsigned long ulLen = 0;
    CFG_RET ret = CFG_OK;
    scew_element *pstDup = NULL;
    scew_element *apstTree[CFG_MAX_WILDCARD_NUM];
    unsigned long ulHead = 0;
    unsigned long ulTail = 0;
    unsigned long i = 0;
    unsigned long ulLoopEnd = 0;
    unsigned long ulLoop = 0;

    /* 获取父节点的全路径 */
    ulLen = CFG_MAX_PATH_LEN;
    ret = CFG_GetFullPath(pstNode->parent, acPath, &ulLen);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* 获取数据树上的父节点指针列表 */
    ret = CFG_GetWildMIDNodesPtr(acPath, apstTree, CFG_MAX_WILDCARD_NUM,
                                 &ulHead, &ulTail, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* 复制子树 */
    ulLoopEnd = (ulTail >= ulHead ) ? ulTail : ulTail + CFG_MAX_WILDCARD_NUM;
    for (i = ulHead; i < ulLoopEnd; i++)
    {
        ulLoop = i % CFG_MAX_WILDCARD_NUM;

        ret = CFG_DupAccess(pstNode, &pstDup);
        if (CFG_OK != ret)
        {
            CFG_ERR(ret);
            return ret;
        }

        /* 添加到数据树上 */
        (void)scew_element_add_elem(apstTree[ulLoop], pstDup);
    }

    (void)pvData;

    return CFG_OK;
}

#define CFG_TREE_EX_NODE_OPT(pstNode, pfnSetOpt, pvData, exp) \
{ \
    pcAttrVal = NULL;\
    pstAttr = scew_attribute_by_name(pstNode, ATTR_NEEDSAVE); \
    if (NULL != pstAttr) \
    { \
        pcAttrVal = scew_attribute_value(pstAttr); \
    } \
 \
    if (NULL != pcAttrVal && '0' == pcAttrVal[0]) \
    { \
        ret = pfnSetOpt(pstNode, pvData); \
        if (CFG_OK != ret) \
        { \
            return ret; \
        } \
        pstNode = NULL; \
        exp; \
    } \
}


/***********************************************************
接口:   填充多余节点的遍历函数
参数:   pstNode, 逻辑树上的节点地址
        pfnSetOpt, 遍历中的操作函数
        pvData, 无意义
返回值: 0, 成功
        其它, 失败
备注:
***********************************************************/
CFG_RET CFG_TreeExNodeAccess(const scew_element *pstNode,
                          FUNC_SET_OPT pfnSetOpt, void *pvData)
{
    const scew_element *pstNodeWork = pstNode;
    const scew_element *pstChild = NULL;
    const scew_element *pstParent = NULL;
    CFG_RET ret = CFG_OK;
    const scew_element *apstStack[CFG_ACCESS_STACK_DEEPTH];
    unsigned char ucStackTop = 0;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;

    apstStack[0] = pstNode;
    ucStackTop++;

    /* 操作 */
    CFG_TREE_EX_NODE_OPT(pstNodeWork, pfnSetOpt, pvData, (void)0);

    while (1)
    {
        if (NULL != pstNodeWork)
        {
            while (pstChild = scew_element_next(pstNodeWork, pstChild),
                   NULL != pstChild)   /* 取子节点, NULL !=子节点 */
            {
                /* 置为子节点 */
                pstNodeWork = pstChild;
                pstChild = NULL;

                /* 进栈 */
                apstStack[ucStackTop] = pstNodeWork;
                ucStackTop++;

                /* 操作 */
                CFG_TREE_EX_NODE_OPT(pstNodeWork, pfnSetOpt, pvData, break);
            }
        }

        /* 栈空 */
        if (1 == ucStackTop)
        {
            break;
        }

        /* 出栈, 恢复路径 */
        ucStackTop--;
        pstNodeWork = apstStack[ucStackTop];

        /* 取兄弟 */
        pstParent = apstStack[ucStackTop - 1];
        pstNodeWork = scew_element_next(pstParent, pstNodeWork);
        if (NULL != pstNodeWork)/* 兄弟不为空 */
        {
            /* 进栈 */
            apstStack[ucStackTop] = pstNodeWork;
            ucStackTop++;

            /* 操作 */
            CFG_TREE_EX_NODE_OPT(pstNodeWork, pfnSetOpt, pvData, continue);
        }
    }

    return CFG_OK;
}



/***********************************************************
接口:   填充单个节点的属性列表
参数:   pstNode, 逻辑树上的节点地址
        pvData, 需要添加的属性列表
返回值: 0, 成功
        其它, 失败
备注:
***********************************************************/
CFG_RET CFG_FillSingleAttr(const scew_element *pstNode, void *pvData)
{
    char acPath[CFG_MAX_PATH_LEN];
    unsigned long ulLen = 0;
    CFG_RET ret = CFG_OK;
    scew_element *apstTree[CFG_MAX_WILDCARD_NUM];
    unsigned long ulHead = 0;
    unsigned long ulTail = 0;
    unsigned long i = 0;
    unsigned long ulLoopEnd = 0;
    unsigned long ulLoop = 0;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    char *(*papcAttrList)[2] = pvData;
    unsigned long j = 0;

    /* 获取本节点的全路径 */
    ulLen = CFG_MAX_PATH_LEN;
    ret = CFG_GetFullPath(pstNode, acPath, &ulLen);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* 获取数据树上的本节点指针列表 */
    ret = CFG_GetWildMIDNodesPtr(acPath, apstTree, CFG_MAX_WILDCARD_NUM,
                                 &ulHead, &ulTail, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* 挨个添加或者设置属性 */
    ulLoopEnd = (ulTail >= ulHead ) ? ulTail : ulTail + CFG_MAX_WILDCARD_NUM;
    for (i = ulHead; i < ulLoopEnd; i++)
    {
        ulLoop = i % CFG_MAX_WILDCARD_NUM;
        for (j = 0; NULL != papcAttrList[j][0]; j++)
        {
            if (NULL != papcAttrList[j][1])
            {
                CFG_SETATTR(apstTree[ulLoop], papcAttrList[j][0],
                            papcAttrList[j][1]);
            }
        }
    }

    return CFG_OK;
}


#define CFG_TREE_EX_ATTR_OPT(pstNode, pfnSetOpt, pvData) \
{ \
    ucHasAttr = 0; \
    for (i = 0; NULL != pvData[i][0]; i++) \
    { \
        pvData[i][1] = NULL; \
        pstAttr = scew_attribute_by_name(pstNode, pvData[i][0]); \
        if (NULL != pstAttr) \
        { \
            pvData[i][1] = (char *)scew_attribute_value(pstAttr); \
            ucHasAttr = 1; \
        } \
    } \
    if (0 != ucHasAttr) \
    { \
        ret = pfnSetOpt(pstNode, pvData); \
        if (CFG_OK != ret) \
        { \
            return ret; \
        } \
    } \
}


/***********************************************************
接口:   填充节点多余属性列表的遍历函数
参数:   pstNode, 逻辑树上的节点地址
        pfnSetOpt, 遍历中的操作函数
        pvData, 属性列表
返回值: 0, 成功
        其它, 失败
备注:
***********************************************************/
CFG_RET CFG_TreeExAttrAccess(const scew_element *pstNode,
                          FUNC_SET_OPT pfnSetOpt, void *pvData)
{
    const scew_element *pstNodeWork = pstNode;
    const scew_element *pstChild = NULL;
    const scew_element *pstParent = NULL;
    CFG_RET ret = CFG_OK;
    const scew_element *apstStack[CFG_ACCESS_STACK_DEEPTH];
    unsigned char ucStackTop = 0;
    scew_attribute *pstAttr =NULL;
    char *(*papcAttrList)[2] = pvData;
    unsigned long i = 0;
    unsigned char ucHasAttr = 0;

    apstStack[0] = pstNode;
    ucStackTop++;

    /* 操作 */
    CFG_TREE_EX_ATTR_OPT(pstNodeWork, pfnSetOpt, papcAttrList);

    while (1)
    {
        if (NULL != pstNodeWork)
        {
            while (pstChild = scew_element_next(pstNodeWork, pstChild),
                   NULL != pstChild)   /* 取子节点, NULL !=子节点 */
            {
                /* 置为子节点 */
                pstNodeWork = pstChild;
                pstChild = NULL;

                /* 进栈 */
                apstStack[ucStackTop] = pstNodeWork;
                ucStackTop++;

                /* 操作 */
                CFG_TREE_EX_ATTR_OPT(pstNodeWork, pfnSetOpt, papcAttrList);
            }
        }

        /* 栈空 */
        if (1 == ucStackTop)
        {
            break;
        }

        /* 出栈, 恢复路径 */
        ucStackTop--;
        pstNodeWork = apstStack[ucStackTop];

        /* 取兄弟 */
        pstParent = apstStack[ucStackTop - 1];
        pstNodeWork = scew_element_next(pstParent, pstNodeWork);
        if (NULL != pstNodeWork)/* 兄弟不为空 */
        {
            /* 进栈 */
            apstStack[ucStackTop] = pstNodeWork;
            ucStackTop++;

            /* 操作 */
            CFG_TREE_EX_ATTR_OPT(pstNodeWork, pfnSetOpt, papcAttrList);
        }
    }

    return CFG_OK;
}


/***********************************************************
接口:   将逻辑树上属性为NeedSave=0的节点填充到数据树上
参数:   无
返回值: 0, 成功
        其它, 失败
备注:
***********************************************************/
CFG_RET CFG_FillDataTreeExNodes(void)
{
    const scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;

    pstNode = scew_tree_root(CFG_GetMIDTree());

    ret = CFG_TreeExNodeAccess(pstNode, CFG_FillSingleNode, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    return ret;
}

/***********************************************************
接口:   将逻辑树上一些属性列表填充到数据树上
参数:   无
返回值: 0, 成功
        其它, 失败
备注:
***********************************************************/
CFG_RET CFG_FillDataTreeExAttrs(void)
{
    const scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;
    char *aapcExAttrList[][2] = {{ATTR_MAX_NODE, NULL},
                                 {ATTR_CUR_NODE, NULL},
                                 {ATTR_STAT, NULL},
								 {ATTR_CACHE, NULL},
                                 {NULL, NULL}};

    pstNode = scew_tree_root(CFG_GetMIDTree());

    ret = CFG_TreeExAttrAccess(pstNode, CFG_FillSingleAttr, aapcExAttrList);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    return ret;
}



/***********************************************************
接口:   根据通配遍历信息获取一个节点对应的通配的index
参数:   pstNode, 节点地址
        pstInfo, 通配信息, 这里存放的是第几个节点是对应通配路径上的{i}
        pulIndex, 输出取得的index列表
返回值: 0, 成功
        其它, 失败
备注:
***********************************************************/
CFG_RET CFG_GetNodeIndexList(const scew_element *pstNode,
                             const ST_CFG_MATCH_TREE_INFO *pstInfo,
                             unsigned long *pulIndex)
{
    unsigned char ucTotalLevel = pstInfo->ucTotalLevel;
    unsigned char ucLevelCount = pstInfo->ucLevelCount;
    unsigned long ulIndex = 0;

    do
    {
        if (ucTotalLevel == pstInfo->aucLevelList[ucLevelCount - 1])
        {
            ulIndex = (unsigned long)strtol(scew_element_name(pstNode), NULL, 10);
            pulIndex[ucLevelCount - 1] = ulIndex;
            if (0 == ucLevelCount)
            {
                break;
            }
            ucLevelCount--;
        }

        pstNode = pstNode->parent;
        ucTotalLevel--;

    } while (NULL != pstNode);

    return CFG_OK;
}


/* 获取一个通配路径下的所有实例的结构链表 */
/***********************************************************
接口:   获取一个通配路径下的所有实例的结构链表
参数:   pcMatchPath, 通配路径
        ppcLeafList, 叶子节点列表
        ulLeafNum, 叶子节点个数
        ppstInstList, 输出结构链表
返回值: 0, 成功
        其它, 失败
备注:
***********************************************************/
CFG_RET CFG_GetPathInstList(const char *pcMatchPath, char * const *ppcLeafList,
                           unsigned long ulLeafNum,
                           ST_CFG_INST_NODE **ppstInstList)
{
    CFG_RET ret = CFG_OK;
    const char *pcPath = pcMatchPath;
    scew_element *apstTree[CFG_MAX_WILDCARD_NUM];
    unsigned long ulHead = 0;
    unsigned long ulTail = 0;
    unsigned long i = 0;
    unsigned long ulLoopEnd = 0;
    unsigned long ulLoop = 0;
    ST_CFG_INST_NODE *apstInstList[2] = {NULL, NULL};
    ST_CFG_INST_NODE *pstInst = NULL;
    ST_CFG_MATCH_TREE_INFO stInfo;

    ret = CFG_GetWildMIDNodesPtr(pcPath, apstTree, CFG_MAX_WILDCARD_NUM,
                                 &ulHead, &ulTail, &stInfo);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    ulLoopEnd = (ulTail >= ulHead ) ? ulTail : ulTail + CFG_MAX_WILDCARD_NUM;
    for (i = ulHead; i < ulLoopEnd; i++)
    {
        ulLoop = i % CFG_MAX_WILDCARD_NUM;

        /* 分配内存, 加入链表 */
        pstInst = malloc(sizeof(ST_CFG_INST_NODE) + ulLeafNum * sizeof(long));
        if (NULL == pstInst)
        {
            (void)CFG_MemFree(apstInstList[0]);

            CFG_ERR(ERR_CFG_MALLOC_FAIL);
            return ERR_CFG_MALLOC_FAIL;
        }

        if (NULL == apstInstList[0])
        {
            apstInstList[0] = pstInst;
        }
        else
        {
            apstInstList[1]->pstNext = pstInst;
        }
        apstInstList[1] = pstInst;
        pstInst->pstNext = NULL;

        /* 获取index列表 */
        (void)CFG_GetNodeIndexList(apstTree[ulLoop], &stInfo,
                                   pstInst->aulIndexList);

        /* 获取结构体 */
        ret = CFG_GetValToStructByPtr(apstTree[ulLoop], pstInst->acData,
                                      ppcLeafList, ulLeafNum, 0);
        if (CFG_OK != ret)
        {
            (void)CFG_MemFree(apstInstList[0]);

            CFG_ERR(ret, "%s", pcMatchPath);
            return ret;
        }
    }

    *ppstInstList = apstInstList[0];

    return CFG_OK;
}


/***********************************************************
接口:   通过通配路径和其index列表获取节点地址
参数:   pvTree, 树指针,
        pcMathPath, 通配路径,
        pulIndexList, index列表
        ppvNode, 输出的节点地址,
        pstNodeInfo, 输出节点信息
        ulMask, 要获取的节点信息的掩码
返回值: 0, 成功
        其它, 失败
备注:
***********************************************************/
CFG_RET CFG_GetNodeByIndex(const void *pvTree, char *pcMathPath,
                    const unsigned long *pulIndexList, void **ppvNode,
                    ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
{
    char *pszTmp = NULL;
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;
    char acName[32];
    unsigned char ucLevel = 0;

    if (NULL == pvTree)
    {
        pvTree = g_pstCfgTree;
    }

    pszTmp = CFG_StrChrTok(pcMathPath, DELIMIT_C, &pcMathPath);

    pstNode = scew_tree_root(pvTree);
    if (NULL != pszTmp)
    {
        /* 如果有提供节点路径, 检查根节点是否匹配 */
        ret = strcmp(scew_element_name(pstNode), pszTmp);
        if (0 != ret)
        {
            /* 节点不存在 */
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszTmp);
            return ERR_CFG_PATH_NOT_EXSITED;
        }
        CFG_InitNodeInfo(pstNodeInfo, ulMask);
        CFG_UpdateNodeInfo(pstNode, pstNodeInfo, ulMask);

        while (pszTmp = CFG_StrChrTok(pcMathPath, DELIMIT_C, &pcMathPath),
               NULL != pszTmp)
        {
            if (0 == strcmp(pszTmp, NODE_WILDCARD_EXP))
            {
                sprintf(acName, "%lu", pulIndexList[ucLevel++]);
                pszTmp = acName;
            }

            pstNode = scew_element_by_name(pstNode, pszTmp);
            if (NULL == pstNode)
            {
                /* 节点不存在 */
                CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszTmp);
                return ERR_CFG_PATH_NOT_EXSITED;
            }

            CFG_UpdateNodeInfo(pstNode, pstNodeInfo, ulMask);
        }
    }
    else
    {
        CFG_InitNodeInfo(pstNodeInfo, ulMask);
        CFG_UpdateNodeInfo(pstNode, pstNodeInfo, ulMask);
    }

    *ppvNode = pstNode;
    return CFG_OK;
}




/***********************************************************
接口:   通过通配路径和index列表获取一个对象下叶子节点值到一个结构体中
参数:   pcMatchPath, 节点通配路径
        pulIndexList, index列表
        pvStruct, 设置值的来源
        ppcLeafList, 节点列表, 节点名为"" 的将被忽略
        ulLeafNum, 节点个数, 实际上是指 ppcLeafList 中的元素个数
返回值: 0, 成功
        其它, 失败
备注:   该函数是CFG_GetValToStructEx的一种扩展
***********************************************************/
CFG_RET CFG_GetValToStructEx(const char *pcMatchPath,
                           const unsigned long *pulIndexList,
                           void *pvStruct, char * const *ppcLeafList,
                           unsigned long ulLeafNum)
{
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;

    /* 复制路径 */
    CFG_DUP_AND_SEARCH_NODE_BY_INDEX(pcMatchPath,
                 pulIndexList, pstNode, NULL, 0);

    ret = CFG_GetValToStructByPtr(pstNode, pvStruct, ppcLeafList, ulLeafNum, 0);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret, "%s", pcMatchPath);
    }

    return ret;
}


/***********************************************************
接口:   通过节点通配路径设置一个节点列表值, 值来源于一个结构体
参数:   pcMatchPath, 节点通配路径
        pulIndexList, index列表
        pvStruct, 设置值的来源
        ppcLeafList, 节点列表, 节点名为"" 的将被忽略
        ulLeafNum, 节点个数, 实际上是指 ppcLeafList 中的元素个数
返回值: 0:成功
        其它: 失败
备注:
***********************************************************/
CFG_RET CFG_SetValFromStructEx(const char *pcMatchPath,
                           const unsigned long *pulIndexList,
                           const void *pvStruct, char * const *ppcLeafList,
                           unsigned long ulLeafNum)
{
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;
    ST_CFG_NODE_INFO stNodeInfo;
    unsigned long ulMask = CFG_NODE_INFO_STANDARD | CFG_NODE_INFO_NOTI;

    /* 复制路径 */
    CFG_DUP_AND_SEARCH_NODE_BY_INDEX(pcMatchPath,
                 pulIndexList, pstNode, &stNodeInfo, ulMask);

    ret = CFG_SetValFromStructByPtr(pstNode, pvStruct,
                 ppcLeafList, ulLeafNum, &stNodeInfo);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret, "%s", pcMatchPath);
    }

    return ret;
}

/***********************************************************
接口:   获取通配路径的叶子节点列表
参数:   pcMatchPath: 节点通配路径,
        ucNextLevel: 参考Tr069
        ppstParaInfo: 输出节点信息链表头
        pcAccessor: 访问者
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetMatchNodeName(const char *pcMatchPath,
                             unsigned char ucNextLevel,
                             ST_PARA_VAL **ppstParaInfo, const char *pcAccessor)
{
    CFG_RET ret = CFG_OK;

    scew_element *apstTree[CFG_MAX_WILDCARD_NUM];
    unsigned long ulHead = 0;
    unsigned long ulTail = 0;
    unsigned long i = 0;
    unsigned long ulLoopEnd = 0;
    unsigned long ulLoop = 0;
    ST_PARA_VAL *apstParaValList[2] = {NULL, NULL};
    char acFullPath[CFG_MAX_PATH_LEN];
    unsigned long ulLen = 0;

    const char *pcPath = pcMatchPath;

    if (NULL == pcPath)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    g_pcCfgAccessor = pcAccessor;

    ret = CFG_GetWildMIDNodesPtr(pcPath, apstTree, CFG_MAX_WILDCARD_NUM,
                                 &ulHead, &ulTail, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    ulLoopEnd = (ulTail >= ulHead ) ? ulTail : ulTail + CFG_MAX_WILDCARD_NUM;
    for (i = ulHead; i < ulLoopEnd; i++)
    {
        ulLoop = i % CFG_MAX_WILDCARD_NUM;

        ulLen = CFG_MAX_PATH_LEN;
        ret = CFG_GetFullPath(apstTree[ulLoop], acFullPath, &ulLen);
        if (CFG_OK != ret)
        {
            (void)CFG_MemFree((void *)apstParaValList[0]);

            CFG_ERR(ret);
            return ret;
        }

        /* pcPath 以'.' 结尾 */
        if (TREE_FLAG_C == pcPath[strlen(pcPath) - 1])
        {
            if (0 == ucNextLevel)
            {
                /* 树全遍历 */
                ret = CFG_TreeGetAccess(apstTree[ulLoop], CFG_GetSingleNodeName,
                                acFullPath, apstParaValList);
            }
            else
            {
                /* 遍历下一层子节点 */
                ret = CFG_TreeGetNextLevelAccess(apstTree[ulLoop],
                           CFG_GetSingleNodeName, acFullPath, apstParaValList);
            }
        }
        else
        {
            ret = CFG_GetSingleNodeName(apstTree[ulLoop],
                            acFullPath, strlen(acFullPath), apstParaValList);
        }

        if (CFG_OK != ret)
        {
            (void)CFG_MemFree(apstParaValList[0]);

            CFG_ERR(ret);
            return ret;
        }
    }

    *ppstParaInfo = apstParaValList[0];

    return CFG_OK;
}


/***********************************************************
接口:   获取节点属性值指针
参数:   pcPath, 节点路径
        pcAttr, 属性名
        ppcVal, 输出值的指针
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetNodeAttrValPtr(const char *pcPath, const char *pcAttr,
                              const char **ppcVal)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    scew_attribute *pstAttr = NULL;
    const char *pcAttrVal = "";

    if (NULL == pcPath || NULL == pcAttr || NULL == ppcVal)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    pstAttr = scew_attribute_by_name(pstNode, pcAttr);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
    }

    *ppcVal = pcAttrVal;
    return CFG_OK;
}


/***********************************************************
接口:   对节点值进行增加指定的值, 只只对int类节点有意义
参数:   pcPath, 节点路径
        lVal, 要增加的值
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_IncIntNodeVal(const char *pcPath, long lVal)
{
    const char *pcPos = NULL;
    char acPath[CFG_MAX_PATH_LEN] = {0};
    const char *pcLeaf = NULL;
    CFG_RET ret = CFG_OK;
    long lValOld = 0;
    const char *pcType = NULL;

    /* 先对类型进行判断 */
    ret = CFG_GetNodeAttrValPtr(pcPath, ATTR_TYPE, &pcType);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    if (0 != strncmp(pcType, "int", 3) && 0 != strncmp(pcType, "unsignedInt", 11))
    {
        CFG_ERR(ERR_CFG_INVALID_TYPE);
        return ERR_CFG_INVALID_TYPE;
    }

    /* 取到上级路径 */
    pcPos = pcPath + strlen(pcPath) - 2;
    while (DELIMIT_C != *pcPos)
    {
        pcPos--;
    }
    pcLeaf = pcPos + 1;

    memcpy(acPath, pcPath, (unsigned long)(pcPos - pcPath));
    ret = CFG_GetValToStruct(acPath, &lValOld, (char * const *)&pcLeaf, 1);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    lValOld += lVal;
    ret = CFG_SetValFromStruct(acPath, &lValOld, (char * const *)&pcLeaf, 1);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    return CFG_OK;
}



/***********************************************************
接口:   根据节点顺序获取节点名称
参数:   pcPath, 节点路径
        ulOrder, 节点顺序, 从1 开始
        ppstPara, 输出节点的全路径
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetNodeOrderName(const char *pcPath, unsigned long ulOrder,
                             ST_PARA_VAL **ppstPara)
{
    scew_element *pstNode = NULL;
    scew_element *pstChild = NULL;
    CFG_RET ret = CFG_OK;
    unsigned long i = ulOrder;
    char acAbsPath[CFG_MAX_PATH_LEN];
    unsigned long ulBufLen = 0;
    ST_PARA_VAL *apstParaList[2] = {NULL, NULL};

    if (NULL == pcPath || 0 == i || NULL == ppstPara)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    if ('\0' == pcPath[0] || '.' == pcPath[0])
    {
        ulBufLen = (unsigned long)snprintf(acAbsPath, CFG_MAX_PATH_LEN,
                                   "%s.", scew_element_name(pstNode));
    }
    else
    {
        ulBufLen = strlen(pcPath);
        if (TREE_FLAG_C == pcPath[ulBufLen - 1])
        {
            strcpy(acAbsPath, pcPath);
        }
        else
        {
            ulBufLen = (unsigned long)snprintf(acAbsPath, CFG_MAX_PATH_LEN, "%s.", pcPath);
        }
    }

    if (CFG_LAST_NODE == i)
    {
        /* 取最后一个 */
        pstChild = pstNode->last_child;
    }
    else
    {
        while (pstChild = scew_element_next(pstNode, pstChild),
               NULL != pstChild)
        {
            i--;
            if (0 == i)
            {
                break;
            }
        }
    }
    if (NULL == pstChild)
    {
        CFG_ERR(ERR_CFG_PATH_NOT_EXSITED);
        return ERR_CFG_PATH_NOT_EXSITED;
    }

    strcat(acAbsPath, scew_element_name(pstChild));
    ulBufLen += strlen(scew_element_name(pstChild));

    ret = CFG_GetSingleNodeName(pstChild, acAbsPath, ulBufLen, apstParaList);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    *ppstPara = apstParaList[0];
    return CFG_OK;
}



/***********************************************************
接口:   清除通配路径对象节点下的实例
参数:   pcMatchPath, 通配路径
        ucRestIdx, 表示是否将索引值复位
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_ClearObjInstEx(const char *pcMatchPath, unsigned char ucRestIdx)
{
    CFG_RET ret = CFG_OK;
    const char *pcPath = pcMatchPath;
    scew_element *apstTree[CFG_MAX_WILDCARD_NUM];
    unsigned long ulHead = 0;
    unsigned long ulTail = 0;
    unsigned long i = 0;
    unsigned long ulLoopEnd = 0;
    unsigned long ulLoop = 0;

    scew_element *pstChild = NULL;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;

    ret = CFG_GetWildMIDNodesPtr(pcPath, apstTree, CFG_MAX_WILDCARD_NUM,
                                 &ulHead, &ulTail, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    ulLoopEnd = (ulTail >= ulHead ) ? ulTail : ulTail + CFG_MAX_WILDCARD_NUM;
    for (i = ulHead; i < ulLoopEnd; i++)
    {
        ulLoop = i % CFG_MAX_WILDCARD_NUM;

        while (pstChild = scew_element_next(apstTree[ulLoop], NULL),
               NULL != pstChild)
        {
            scew_element_del(pstChild);
        }

        (void)CFG_UpdateObjCurInstCount(apstTree[ulLoop]);

        if (1 == ucRestIdx)
        {
            CFG_SETATTR(apstTree[ulLoop], "MaxIdx", "0");
        }
    }

    return CFG_OK;
}



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif




