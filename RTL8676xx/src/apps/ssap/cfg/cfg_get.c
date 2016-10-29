/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cfg_get.c
 文件描述 : cfg模块get相关功能的实现
 函数列表 :

 修订记录 :
          1 创建 : 陈跃东
            日期 : 2007-11-23
            描述 :

**********************************************************************/


#include "cfg_prv.h"



#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif




/***********************************************************
接口:   释放配置模块分配的内存
参数:   pvMem, 通过配置提供的接口取得的内存
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_MemFree(void *pvMem)
{
    ST_PARA_VAL *pstMem = (ST_PARA_VAL *)pvMem;
    ST_PARA_VAL *pstMemNext = NULL;

    while (NULL != pstMem)
    {
        pstMemNext = pstMem->pstNext;
        free(pstMem);
        pstMem = pstMemNext;
    }

    return CFG_OK;
}




/***********************************************************
接口:   获取通知
参数:   ppstParaVal: 输出通知的链表头地址, 由配置模块分配,
                      使用完请用 CFG_MemFree 释放
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetNotification(ST_PARA_VAL **ppstParaVal)
{
    g_pcCfgAccessor = "";
    return CFG_GetCommVal(TREE_FLAG, 0, ppstParaVal, CFG_GetSingleNoti);
}




/***********************************************************
接口:   获取节点内容
参数:   pcPath: 节点路径
        ppstParaVal: 输出节点值链表头
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetNodeVal(const char *pcPath, ST_PARA_VAL **ppstParaVal,
                                             const char *pcAccessor)
{
    g_pcCfgAccessor = pcAccessor;

    return CFG_GetCommVal(pcPath, 0, ppstParaVal, CFG_GetSingleNodeVal);
}


/***********************************************************
接口:   获取叶节点内容和类型
参数:   pcPath: 节点路径,
        pcVal: 输出节点状态值, 内存由调用者分配,
        pulLen: 输入pcVal的有效长度,
                如果不够, 将输出实际需要多长.
        pcType和pulTypeLen 的意义同上
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetLeafValAndType(const char *pcPath,
                char *pcVal, unsigned long *pulValLen,
                char *pcType, unsigned long *pulTypeLen)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    unsigned long ulValLen = 0;
    unsigned long ulTypeLen = 0;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    unsigned char ucBufEnough = 1;
    const char *pcContent = "";

    if (NULL == pcPath
        || (NULL == pcVal && NULL == pulValLen
            && NULL == pcType && NULL == pulTypeLen)
        || (NULL != pcVal && NULL == pulValLen)
        || (NULL != pcType && NULL == pulTypeLen))
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    if (NULL != pcVal && NULL != pulValLen)
    {
        /* 输出当前节点的名称和val */
        pcContent = scew_element_contents(pstNode);
        if (NULL == pcContent)
        {
            pcContent = "";
        }
        ulValLen = strlen(pcContent);

        if (*pulValLen < ulValLen + 1)
        {
           *pulValLen = ulValLen + 1;
            ucBufEnough = 0;
        }
    }

    if (NULL != pcType && NULL != pulTypeLen)
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_TYPE);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            ulTypeLen = strlen(pcAttrVal);
            if (*pulTypeLen < ulTypeLen + 1)
            {
                *pulTypeLen = ulTypeLen + 1;
                ucBufEnough = 0;
            }
        }
    }
    if (0 == ucBufEnough)
    {
        CFG_ERR(ERR_CFG_BUF_NOT_ENOUGH, "%s %s %s", pcAttrVal, pcContent, pcPath);
        return ERR_CFG_BUF_NOT_ENOUGH;
    }

    if (NULL != pcVal && NULL != pulValLen)
    {
        *pulValLen = ulValLen;
        strcpy(pcVal, pcContent);
    }

    if (NULL != pcType && NULL != pulTypeLen)
    {
        *pulTypeLen = ulTypeLen;
        if (NULL != pcAttrVal)
        {
            strcpy(pcType, pcAttrVal);
        }
        else
        {
            pcType[0] = '\0';
        }
    }

    return CFG_OK;
}



/***********************************************************
接口:   获取节点名称
参数:   pcPath: 节点路径,
        ucNextLevel: 参考Tr069
        ppstParaInfo: 输出节点信息链表头
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetNodeName(const char *pcPath, unsigned char ucNextLevel,
                       ST_PARA_VAL **ppstParaInfo, const char *pcAccessor)
{
    g_pcCfgAccessor = pcAccessor;

    return CFG_GetCommVal(pcPath, ucNextLevel, ppstParaInfo,
                                      CFG_GetSingleNodeName);
}


/***********************************************************
接口:   获取节点属性
参数:   pcPath: 节点路径,
        ppstAttVal: 输出节点属性链表头
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetNodeAttr(const char *pcPath, ST_PARA_VAL **ppstAttVal,
                                                 const char *pcAccessor)
{
    g_pcCfgAccessor = pcAccessor;

    return CFG_GetCommAttrVal(pcPath, 0, ppstAttVal, CFG_GetSingleNodeAttr);
}



/*************************************************************************
Function:      CFG_RET CFG_GetCommVal(const char *pcPath, unsigned char ucNextLevel,
                        ST_PARA_VAL **ppstParaVal, FUNC_OPT pfnOpt)
Description:   通用的取值(节点值\属性值\name)的函数
Calls:
Data Accessed:
Data Updated:
Input:         pcPath, 要访问的路径名,
               ucNextLevel, 是否只访问下层子节点
               pfnOpt, 操作函数, 具体的访问函数,
               比如GetName就传对单个节点取name的函数
Output:        ppstParaVal, 访问输出结果
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_GetCommVal(const char *pcPath, unsigned char ucNextLevel,
                        ST_PARA_VAL **ppstParaVal, FUNC_GET_OPT pfnOpt)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    unsigned long ulBufLen = 0;
    ST_PARA_VAL *apstParaValList[2] = {NULL, NULL};
    char acAbsPath[CFG_MAX_PATH_LEN];
    ST_CFG_NODE_INFO stNodeInfo;
    unsigned long ulMask = CFG_NODE_INFO_STANDARD;

    if (NULL == pcPath || NULL == ppstParaVal)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, &stNodeInfo, ulMask);
    CFG_GET_RIGHT_CHECK_BY_INFO(&stNodeInfo, g_pcCfgAccessor);

    ulBufLen = strlen(pcPath);
    if ('\0' == pcPath[0] || '.' == pcPath[0])
    {
        strcpy(acAbsPath, scew_element_name(pstNode));
    }
    else
    {
        strcpy(acAbsPath, pcPath);
    }

    /* pcPath 以'.' 结尾 */
    if (ulBufLen > 0 && TREE_FLAG_C == pcPath[ulBufLen - 1])
    {
        if (ulBufLen > 1)
        {
            acAbsPath[ulBufLen - 1] = '\0';
        }

        if (0 == ucNextLevel)
        {
            /* 树全遍历 */
            ret = CFG_TreeGetAccess(pstNode, pfnOpt,
                            acAbsPath, apstParaValList);
        }
        else
        {
            /* 遍历下一层子节点 */
            ret = CFG_TreeGetNextLevelAccess(pstNode, pfnOpt,
                                  acAbsPath, apstParaValList);
        }
    }
    else
    {
        ret = pfnOpt(pstNode, acAbsPath, strlen(acAbsPath), apstParaValList);
    }

    if (CFG_OK != ret)
    {
        /* 释放链表 */
        (void)CFG_MemFree(apstParaValList[0]);

        return ret;
    }

    *ppstParaVal = apstParaValList[0];

    return CFG_OK;
}








/*************************************************************************
Function:      CFG_RET CFG_GetSingleNodeName(const scew_element *pstNode,
                    const char *pcAbsPath, void *pvPara)
Description:   对单个节点取Name的操作的函数
Calls:
Data Accessed:
Data Updated:
Input:         pstNode, 要遍历的子树,
               pcAbsPath, 当前节点的全路径,
               pvPara, 操作参数
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_GetSingleNodeName(const scew_element *pstNode,
                    const char *pcAbsPath, unsigned char ucLen, void *pvPara)
{
    ST_PARA_VAL **ppstParaInfoList = (ST_PARA_VAL **)pvPara;
    ST_PARA_VAL *pstParaInfo = NULL;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    unsigned long ulBufLen = 0;
    unsigned long ulPathLen = 0;
    unsigned char ucFullPath = 1;
    const char *pcName = NULL;

    pstAttr = scew_attribute_by_name(pstNode, ATTR_TYPE);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);

        if (0 == strcmp(pcAttrVal, TYPE_CLASS)
            || 0 == strcmp(pcAttrVal, TYPE_OBJECT))
        {
            ucFullPath = 0;
        }
    }

    ulPathLen = ucLen;
    ulBufLen = sizeof(ST_PARA_VAL) + ulPathLen + 1
                           + DEFAULT_WRITABLE_STR_LEN + 1;
    if (!ucFullPath)
    {
        ulBufLen++;
    }

    pstParaInfo = malloc(ulBufLen);
    if (NULL == pstParaInfo)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }
    pstParaInfo->pcName = (char *)(pstParaInfo + 1);
    strcpy(pstParaInfo->pcName, pcAbsPath);
    pstParaInfo->pcVal = pstParaInfo->pcName + ulPathLen + 1;
    if (!ucFullPath)
    {
        pstParaInfo->pcName[ulPathLen] = '.';
        pstParaInfo->pcName[ulPathLen + 1]= '\0';
        pstParaInfo->pcVal++;
    }
    strcpy(pstParaInfo->pcVal, DEFAULT_WRITABLE_STR);
    pstParaInfo->pcType= NULL;

    /* 检查节点名是否为数字 */
    pcName = scew_element_name(pstNode);
    if (CFG_IS_NODE_NUM(pcName))   /* 是实例节点 */
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_WRITABLE);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            if (CFG_WRITABLE_C != pcAttrVal[0])
            {
                pstParaInfo->pcVal[ATTR_WRITABLE_LEN + 2] = CFG_NO_WRITABLE_C;
            }
        }
        else
        {
            /* 取父节点 */
            pstAttr = scew_attribute_by_name(pstNode->parent, ATTR_WRITABLE);
            if (NULL != pstAttr)
            {
                pcAttrVal = scew_attribute_value(pstAttr);
                if (CFG_WRITABLE_C != pcAttrVal[0]
                    && CFG_DELETABLE_C != pcAttrVal[0])
                {
                    pstParaInfo->pcVal[ATTR_WRITABLE_LEN + 2] = CFG_NO_WRITABLE_C;
                }
            }
        }
    }
    else
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_WRITABLE);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            if (CFG_WRITABLE_C != pcAttrVal[0]
                && CFG_ADDABLE_C != pcAttrVal[0])
            {
                pstParaInfo->pcVal[ATTR_WRITABLE_LEN + 2] = CFG_NO_WRITABLE_C;
            }
        }
    }

    /* 加入链表 */
    CFG_LIST_APPEND(ppstParaInfoList, pstParaInfo);

    return CFG_OK;
}





/*************************************************************************
Function:      CFG_RET CFG_GetSingleNodeVal(const scew_element *pstNode,
                   const char *pcAbsPath, void *pvPara)
Description:   对单个节点取Val的操作的函数
Calls:
Data Accessed:
Data Updated:
Input:         pstNode, 要遍历的子树,
               pcAbsPath, 当前节点的全路径,
               pvPara, 操作参数
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_GetSingleNodeVal(const scew_element *pstNode,
                   const char *pcAbsPath, unsigned char ucLen, void *pvPara)
{
    ST_PARA_VAL **ppstParaValList = (ST_PARA_VAL **)pvPara;
    ST_PARA_VAL *pstParaVal = NULL;
    unsigned long ulBufLen = 0;
    const char *pcVal = NULL;
    scew_attribute *pstAttr =NULL;
    const char *pcType = NULL;
    const char *pcAttrVal = "";

    /* 根据类型进行拦截 */
    CFG_FILTER_NON_LEAF(pstNode);
    pcType = pcAttrVal;

    pcVal = scew_element_contents(pstNode);
    if (NULL == pcVal)
    {
        pcVal = "";
    }

    /* 如果密码属性为真, 也输出空 */
    pstAttr = scew_attribute_by_name(pstNode, ATTR_PASSWORDD);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
        if ('1' == pcAttrVal[0]
            || ('2' == pcAttrVal[0] && '\0' == g_pcCfgAccessor[0]))
        {
             pcVal = "";
        }
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


/*************************************************************************
Function:      CFG_RET CFG_GetSingleNoti(const scew_element *pstNode,
                   const char *pcAbsPath, void *pvPara)
Description:   对单个节点取通知的操作的函数
Calls:
Data Accessed:
Data Updated:
Input:         pstNode, 要遍历的子树,
               pcAbsPath, 当前节点的全路径,
               pvPara, 操作参数
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_GetSingleNoti(const scew_element *pstNode,
                   const char *pcAbsPath, unsigned char ucLen, void *pvPara)
{
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = "";
    const char *pcNodeVal = NULL;
    const char *pcType = NULL;
    ST_PARA_VAL **ppstNotiList = (ST_PARA_VAL **)pvPara;
    ST_PARA_VAL *pstNotiVal = NULL;
    unsigned long ulLen = 0;

    /* 根据类型进行拦截 */
    CFG_FILTER_NON_LEAF(pstNode);
    pcType = pcAttrVal;

    /* 取节点通知标志属性 */
    pstAttr = scew_attribute_by_name(pstNode, ATTR_NEED_NOTI);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
    }
    else
    {
        return CFG_OK;
    }

    if (NEEDNOTI_C == pcAttrVal[0])   /* 通知标志为真 */
    {
        pcNodeVal = scew_element_contents(pstNode);
        if (NULL == pcNodeVal)
        {
            pcNodeVal = "";
        }
        ulLen = sizeof(ST_PARA_VAL) + ucLen + 1
                + strlen(pcNodeVal) + 1 + strlen(pcType) + 1;

        /* 分配链表内存 */
        pstNotiVal = malloc(ulLen);
        if (NULL == pstNotiVal)
        {
            CFG_ERR(ERR_CFG_MALLOC_FAIL);
            return ERR_CFG_MALLOC_FAIL;
        }

        /* 依次填充 节点名\类型\值 */
        pstNotiVal->pcType = (char *)(pstNotiVal + 1);
        strcpy(pstNotiVal->pcType, pcType);
        pstNotiVal->pcName = pstNotiVal->pcType
                             + strlen(pstNotiVal->pcType) + 1;
        strcpy(pstNotiVal->pcName, pcAbsPath);
        pstNotiVal->pcVal = pstNotiVal->pcName
                            + strlen(pstNotiVal->pcName) + 1;
        strcpy(pstNotiVal->pcVal, pcNodeVal);

        /* 将节点加入链表 */
        CFG_LIST_APPEND(ppstNotiList, pstNotiVal);
    }

    return CFG_OK;
}




/*************************************************************************
Function:      CFG_RET CFG_GetSingleNodeAttr(const scew_element *pstNode,
                   const char *pcAbsPath, void *pvPara)
Description:   对单个节点取Attr(属性)的操作的函数
Calls:
Data Accessed:
Data Updated:
Input:         pstNode, 要遍历的子树,
               pcAbsPath, 当前节点的全路径,
               pvPara, 操作参数
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_GetSingleNodeAttr(const scew_element *pstNode,
                    const char *pcAbsPath, unsigned char ucLen,
                    const ST_CFG_NODE_INFO *pstNodeInfo, void *pvPara)
{
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    char pcNoti[] = DEFAULT_NOTI;
    const char *pcAccess = DEFAULT_ACCESS_LIST;
    unsigned long ulPathLen = 0;
    unsigned long aulAttrLen[2] = {0, 0};
    unsigned long ulLen = 0;
    ST_PARA_VAL *pstAttrNode = NULL;
    ST_PARA_VAL **ppstAttrNodeList = (ST_PARA_VAL **)pvPara;
    char *pcVal = NULL;

    /* 根据类型进行拦截 */
    CFG_FILTER_NON_LEAF(pstNode);

    pcAccess = pstNodeInfo->pcAccessList;

    /* 根据长度分配内存 */
    ulPathLen = ucLen;
    aulAttrLen[0] = 1;
    aulAttrLen[1] = strlen(pcAccess);
    ulLen = ulPathLen + 1
            + ATTR_NOTI_LEN + 1 + 2 + aulAttrLen[0] + 1
            + ATTR_ACCESS_LIST_LEN + 1 + 2 + aulAttrLen[1] + 1
            + sizeof(ST_PARA_VAL);
    pstAttrNode = malloc(ulLen);
    if (NULL == pstAttrNode)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }
    pstAttrNode->pcType = NULL;

    /* 填充 neame 和 属性值  */
    pstAttrNode->pcName = (char *)(pstAttrNode + 1);
    strcpy(pstAttrNode->pcName, pcAbsPath);
    pstAttrNode->pcVal = pstAttrNode->pcName + ulPathLen + 1;
    pcVal = pstAttrNode->pcVal;
    CFG_MemCpy(pcVal, ATTR_NOTI, ATTR_NOTI_LEN);
    pcVal += ATTR_NOTI_LEN;
    pcVal[0] = '=';
    pcVal[1] = '\"';
    pcVal += 2;
    pcNoti[0] += (char)pstNodeInfo->ucNoti;
    CFG_MemCpy(pcVal, pcNoti, aulAttrLen[0]);
    pcVal += aulAttrLen[0];
    pcVal[0] = '\"';
    pcVal[1] = ' ';
    pcVal += 2;
    CFG_MemCpy(pcVal, ATTR_ACCESS_LIST, ATTR_ACCESS_LIST_LEN);
    pcVal += ATTR_ACCESS_LIST_LEN;
    pcVal[0] = '=';
    pcVal[1] = '\"';
    pcVal += 2;
    CFG_MemCpy(pcVal, pcAccess, aulAttrLen[1]);
    pcVal += aulAttrLen[1];
    pcVal[0] = '\"';
    pcVal[1] = '\0';

    /* 加入链表 */
    CFG_LIST_APPEND(ppstAttrNodeList, pstAttrNode);

    return CFG_OK;
}



/*************************************************************************
Function:      CFG_RET CFG_GetCommVal(const char *pcPath, unsigned char ucNextLevel,
                        ST_PARA_VAL **ppstParaVal, FUNC_OPT pfnOpt)
Description:   通用的取值(节点值\属性值\name)的函数
Calls:
Data Accessed:
Data Updated:
Input:         pcPath, 要访问的路径名,
               ucNextLevel, 是否只访问下层子节点
               pfnOpt, 操作函数, 具体的访问函数,
               比如GetName就传对单个节点取name的函数
Output:        ppstParaVal, 访问输出结果
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_GetCommAttrVal(const char *pcPath, unsigned char ucNextLevel,
                        ST_PARA_VAL **ppstParaVal, FUNC_ATTR_GET_OPT pfnOpt)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    unsigned long ulBufLen = 0;
    ST_PARA_VAL *apstParaValList[2] = {NULL, NULL};
    char acAbsPath[CFG_MAX_PATH_LEN];
    ST_CFG_NODE_INFO stNodeInfo;
    unsigned long ulMask = CFG_NODE_INFO_STANDARD
                         | CFG_NODE_INFO_NOTI | CFG_NODE_INFO_ACCESSLIST;

    if (NULL == pcPath || NULL == ppstParaVal)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, &stNodeInfo, ulMask);
    CFG_GET_RIGHT_CHECK_BY_INFO(&stNodeInfo, g_pcCfgAccessor);

    ulBufLen = strlen(pcPath);
    if ('\0' == pcPath[0] || '.' == pcPath[0])
    {
        strcpy(acAbsPath, scew_element_name(pstNode));
    }
    else
    {
        strcpy(acAbsPath, pcPath);
    }

    /* pcPath 以'.' 结尾 */
    if (ulBufLen > 0 && TREE_FLAG_C == pcPath[ulBufLen - 1])
    {
        if (ulBufLen > 1)
        {
            acAbsPath[ulBufLen - 1] = '\0';
        }

        if (0 == ucNextLevel)
        {
            /* 树全遍历 */
            ret = CFG_TreeAttrGetAccess(pstNode, pfnOpt,
                            acAbsPath, &stNodeInfo, apstParaValList);
        }
        else
        {
            ;
        }
    }
    else
    {
        ret = pfnOpt(pstNode, acAbsPath, strlen(acAbsPath),
                             &stNodeInfo, apstParaValList);
    }

    if (CFG_OK != ret)
    {
        /* 释放链表 */
        (void)CFG_MemFree(apstParaValList[0]);

        return ret;
    }

    *ppstParaVal = apstParaValList[0];

    return CFG_OK;
}








/* 取得节点最大的已经使用的实例序号 */
/***********************************************************
接口:   获取当前对象集合下已使用过的最大的实例编号
参数:   pcPath, 节点路径
        pulMaxIdx, 输出最大编号
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetNodeMaxIdx(const char *pcPath, unsigned long *pulMaxIdx)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;

    if (NULL == pcPath || NULL == pulMaxIdx)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    *pulMaxIdx = 0;
    pstAttr = scew_attribute_by_name(pstNode, ATTR_MAX_IDX);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
        *pulMaxIdx = (unsigned long)strtol(pcAttrVal, NULL, 10);
    }

    return CFG_OK;
}

/* 设置节点最大的已经使用的实例序号 */
/***********************************************************
接口:   获取当前对象集合下已使用过的最大的实例编号
参数:   pcPath, 节点路径
        ulMaxIdx, 要设置成的最大编号
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_SetNodeMaxIdx(const char *pcPath, unsigned long ulMaxIdx)
{
    scew_element* pstNode = NULL;
    long ret = CFG_OK;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    char acBuf[20];

    if (NULL == pcPath)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    sprintf(acBuf, "%lu", ulMaxIdx);

    pstAttr = scew_attribute_by_name(pstNode, ATTR_MAX_IDX);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_set_value(pstAttr, acBuf);
        if (NULL == pcAttrVal)
        {
            CFG_ERR(ERR_CFG_INTERNAL_ERR);
            return ERR_CFG_INTERNAL_ERR;
        }
    }
    else
    {
        pstAttr = attribute_create(ATTR_MAX_IDX, acBuf);
        if (NULL == pstAttr)
        {
            CFG_ERR(ERR_CFG_INTERNAL_ERR);
            return ERR_CFG_INTERNAL_ERR;
        }

        (void)attribute_list_add(pstNode->attributes, pstAttr);
    }

    return CFG_OK;
}




/* 取得节点子树个数 */
/***********************************************************
接口:   获取节点下一层子节点个数
参数:   pcPath, 节点路径
        pulCount, 输出个数
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetNodeChildCount(const char *pcPath, unsigned long *pulCount)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;

    if (NULL == pcPath || NULL == pulCount)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    *pulCount = scew_element_count(pstNode);
    return CFG_OK;
}

#ifdef _CFG_DEBUG

/***********************************************************
接口:   单个节点类型是否合法的处理函数
参数:   pstNode, 节点地址,
        pcAbsPath, 全路径存放地址
        ucLen, 全路径的长度,
        pvPara, 如果类型不合法, 则输出到链表中
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetSingleNoTypeNode(const scew_element *pstNode,
                    const char *pcAbsPath, unsigned char ucLen, void *pvPara)
{
    ST_PARA_VAL **ppstParaValList = (ST_PARA_VAL **)pvPara;
    ST_PARA_VAL *pstParaVal = NULL;
    unsigned long ulBufLen = 0;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = "";
    unsigned char ucNotype = 1;

    char *apcType[]= {"object", "string", "int", "unsignedInt",
                      "boolean", "dateTime", "base64"};
    unsigned long i = 0;

    pstAttr = scew_attribute_by_name(pstNode, ATTR_TYPE);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);

        if ('\0' != pcAttrVal[0])
        {
            ucNotype = 0;
        }
        ucNotype = 1;
        for (i = 0; i < sizeof(apcType)/sizeof(apcType[0]); i++)
        {
            if (0 == strncmp(pcAttrVal, apcType[i], strlen(apcType[i])))
            {
                ucNotype = 0;
                break;
            }
        }
    }

    if (!ucNotype)
    {
        return CFG_OK;
    }

    ulBufLen = sizeof(ST_PARA_VAL) + ucLen + 1;
    pstParaVal = malloc(ulBufLen);
    if (NULL == pstParaVal)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }

    pstParaVal->pcName = (char *)(pstParaVal + 1);
    strcpy(pstParaVal->pcName, pcAbsPath);

    /* 加入链表 */
    CFG_LIST_APPEND(ppstParaValList, pstParaVal);

    return CFG_OK;
}


/***********************************************************
接口:   列出数据树中类型不合法的节点路径
参数:   无
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_ListNoTypeNode(void)
{
    CFG_RET ret = CFG_OK;
    ST_PARA_VAL *pstParaVal = NULL;
    ST_PARA_VAL *pstParaValHead = NULL;

    g_pcCfgAccessor = NULL;

    ret =  CFG_GetCommVal(".", 0, &pstParaVal, CFG_GetSingleNoTypeNode);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    pstParaValHead = pstParaVal;
    while (NULL != pstParaVal)
    {
        printf("%s\n", pstParaVal->pcName);
        pstParaVal = pstParaVal->pstNext;
    }

    (void)CFG_MemFree(pstParaValHead);

    return CFG_OK;
}


#define CFG_TREE_PRINT_EXCEL_OPT(pstNode, pfnGetOpt, pcAbsPath, ucLen, pvData) \
{ \
    ret = pfnGetOpt(pstNode, pcAbsPath, ucLen, pvData); \
    if (CFG_OK != ret) \
    { \
        return ret; \
    } \
}



/***********************************************************
接口:   将节点树打印成Excl格式的遍历函数
参数:   pstNode, 节点树
        pfnGetOpt, 单节点操作函数
        pcAbsPath, 全路径存放地址
        pvData, 节点链表, 遍历的结果是得到这个链表, 再挨个输出
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_TreePrintExcelAccess(const scew_element *pstNode, FUNC_GET_OPT pfnGetOpt,
                                          char *pcAbsPath, void *pvData)
{
    const scew_element *pstNodeWork = pstNode;
    const scew_element *pstChild = NULL;
    const scew_element *pstParent = NULL;
    const char *pcNodeName = NULL;
    CFG_RET ret = CFG_OK;
    ST_CFG_TREE_STACK_NODE astStack[CFG_ACCESS_STACK_DEEPTH];
    unsigned char ucStackTop = 0;

    /* 初始化栈 */
    astStack[0].ucPathLen = strlen(pcAbsPath);
    astStack[0].pstPath = pstNodeWork;
    ucStackTop = 1;

    /* 操作 */
    CFG_TREE_PRINT_EXCEL_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
            astStack[ucStackTop - 1].ucPathLen, pvData);

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
                NODE_WILDCARD_RCVT(pcNodeName);
                pcAbsPath[astStack[ucStackTop - 1].ucPathLen] = DELIMIT_C;
                strcpy(pcAbsPath + astStack[ucStackTop - 1].ucPathLen + 1, pcNodeName);

                /* 进栈 */
                astStack[ucStackTop].pstPath = pstNodeWork;
                astStack[ucStackTop].ucPathLen
                    = astStack[ucStackTop - 1].ucPathLen  + strlen(pcNodeName) + 1;
                ucStackTop++;

                /* 操作 */
                CFG_TREE_PRINT_EXCEL_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
                       astStack[ucStackTop - 1].ucPathLen, pvData);
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
            NODE_WILDCARD_RCVT(pcNodeName);
            pcAbsPath[astStack[ucStackTop - 1].ucPathLen] = DELIMIT_C;
            strcpy(pcAbsPath + astStack[ucStackTop - 1].ucPathLen + 1, pcNodeName);

            /* 进栈 */
            astStack[ucStackTop].pstPath = pstNodeWork;
            astStack[ucStackTop].ucPathLen
                = astStack[ucStackTop - 1].ucPathLen  + strlen(pcNodeName) + 1;
            ucStackTop++;

            /* 操作 */
            CFG_TREE_PRINT_EXCEL_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
                     astStack[ucStackTop - 1].ucPathLen, pvData);
        }
    }

    return CFG_OK;
}

/***********************************************************
接口:   将节点树打印成Excl格式的单个节点的处理函数
参数:   pstNode, 节点树
        pfnGetOpt, 单节点操作函数
        pcAbsPath, 全路径存放地址
        pvData, 节点链表
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_PrintSingleNodeExecl(const scew_element *pstNode,
                    const char *pcAbsPath, unsigned char ucLen, void *pvPara)
{
    ST_PARA_VAL **ppstParaValList = (ST_PARA_VAL **)pvPara;
    ST_PARA_VAL *pstParaVal = NULL;
    const char *pcName = scew_element_name(pstNode);
    const char *pcType = "";
    const char *pcWritable = "W";
    scew_attribute *pstAttr =NULL;
    unsigned long ulLen = 0;
    unsigned long ulNameLen = 0;
    unsigned long ulTypeLen = 0;
    unsigned long ulValLen = 0;
    const scew_element *pstChild = scew_element_next(pstNode, NULL);

    if (NULL != pstChild
        && 0 == strcmp(NODE_WILDCARD, scew_element_name(pstChild)))
    {
        /* 忽略 */
        return CFG_OK;
    }

    /* 取类型 */
    pstAttr = scew_attribute_by_name(pstNode, ATTR_TYPE);
    if (NULL != pstAttr)
    {
        pcType = scew_attribute_value(pstAttr);
    }
    /* 取可写属性 */
    pstAttr = scew_attribute_by_name(pstNode, ATTR_WRITABLE);
    if (NULL != pstAttr)
    {
        pcWritable = scew_attribute_value(pstAttr);
        if ('0' == pcWritable[0])
        {
            pcWritable = "-";
        }
        else
        {
            pcWritable = "W";
        }
    }
    else
    {
        if (NULL != pstChild
            && 0 != strcmp(NODE_WILDCARD, scew_element_name(pstChild)))
        {
            pcWritable = "-";
        }
    }

    ulTypeLen = strlen(pcType);
    ulValLen = strlen(pcWritable);
    if (0 == strcmp(pcType, TYPE_OBJECT))/* 类型为对象 */
    {
        ulNameLen = ucLen + 1;
    }
    else
    {
        ulNameLen = strlen(pcName);
    }
    ulLen = sizeof(ST_PARA_VAL) + ulNameLen + 1 + ulTypeLen + 1 + ulValLen + 1;

    pstParaVal = malloc(ulLen);
    if (NULL == pstParaVal)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }
    pstParaVal->pcName = (char *)(pstParaVal + 1);
    if (0 == strcmp(pcType, TYPE_OBJECT))/* 类型为对象 */
    {
        memcpy(pstParaVal->pcName, pcAbsPath, ulNameLen - 1);
        pstParaVal->pcName[ulNameLen - 1] = DELIMIT_C;
        pstParaVal->pcName[ulNameLen] = '\0';
    }
    else
    {
        strcpy(pstParaVal->pcName, pcName);
    }
    pstParaVal->pcType = pstParaVal->pcName + ulNameLen + 1;
    strcpy(pstParaVal->pcType, pcType);
    pstParaVal->pcVal = pstParaVal->pcType + ulTypeLen + 1;
    strcpy(pstParaVal->pcVal, pcWritable);

    /* 加入链表 */
    CFG_LIST_APPEND(ppstParaValList, pstParaVal);

    return CFG_OK;
}

/***********************************************************
接口:   将节点树打印成Excl格式
参数:   无
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_PrintToExcel(void)
{
    CFG_RET ret = CFG_OK;
    ST_PARA_VAL *pstParaVal = NULL;
    char acAbsPath[CFG_MAX_PATH_LEN];
    ST_PARA_VAL *apstParaValList[2] = {NULL, NULL};

    scew_tree *pstTree = CFG_GetMIDTree();
    const scew_element *pstNode = scew_tree_root(pstTree);

    strcpy(acAbsPath, scew_element_name(pstNode));

    ret = CFG_TreePrintExcelAccess(pstNode, CFG_PrintSingleNodeExecl,
                                         acAbsPath, apstParaValList);
    if (CFG_OK != ret)
    {
        (void)CFG_MemFree(apstParaValList[0]);

        CFG_ERR(ret);
        return ret;
    }

    pstParaVal = apstParaValList[0];
    while (NULL != pstParaVal)
    {
        printf("%s\t%s\t%s\r\n", pstParaVal->pcName, pstParaVal->pcType,
                             pstParaVal->pcVal);
        pstParaVal = pstParaVal->pstNext;
    }

    (void)CFG_MemFree(apstParaValList[0]);
    return CFG_OK;
}

#endif









#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif



