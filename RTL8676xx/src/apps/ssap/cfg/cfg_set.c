/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cfg_set.c
 文件描述 : cfg 模块中set相关功能的实现
 函数列表 :

 修订记录 :
          1 创建 : 陈跃东
            日期 : 2007-11-23
            描述 :
**********************************************************************/

#include <sys/time.h>
#include "cfg_prv.h"
#if defined(CONFIG_APPS_LOGIC_Argentina) || defined(CONFIG_TELEFONICA_COLOMBIA)
#include <unistd.h>
#endif

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


/* 报告可以获取通知的接口 */
STATIC FUNC_NOTI_INFORM s_pfnCfgNotiInform = NULL;



/*************************************************************************
Function:      CFG_RET CFG_ClearSingleNoti(const scew_element *pstNode, void *pvPara)
Description:   清除单个节点的通知属性
Calls:
Data Accessed:
Data Updated:
Input:         pstNode, 要遍历的子树,
               pvPara, 无意仪
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_ClearSingleNoti(const scew_element *pstNode, void *pvPara)
{
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;

    (void)pvPara;

    /* 根据类型进行拦截 */
    CFG_FILTER_NON_LEAF(pstNode);

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
        /* 将节点 neednoti 属性设为非  */
        pcAttrVal = scew_attribute_set_value(pstAttr, NONEEDNOTI);
    }

    return CFG_OK;
}



/*************************************************************************
Function:      CFG_RET CFG_SetSingleNodeAttr(const scew_element *pstNode,
                                                        void *pvPara)
Description:   对单个节点设置属性的操作的函数
Calls:
Data Accessed:
Data Updated:
Input:         pstNode, 要遍历的子树,
               pvPara, 操作参数, 实际传入的往往是属性列表
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_SetSingleNodeAttr(const scew_element *pstNode, void *pvPara)
{
    scew_attribute *pstAttr = NULL;
    const char *pcAttrVal = NULL;
    char *(*papcAttrVal)[2] = pvPara;
    unsigned long i = 0;

    while (NULL != papcAttrVal[i][0])
    {
        CFG_SETATTR(pstNode, papcAttrVal[i][0], papcAttrVal[i][1])

        i++;
    }

    return CFG_OK;
}

/*************************************************************************
Function:      CFG_RET CFG_FilterNodeDonotSetActiveNoti(const scew_element *pstNode,
                                                  void *pvPara)
Description:   过滤不能设置为主动上报的的节点
Calls:
Data Accessed:
Data Updated:
Input:         pstNode, 要遍历的子树,
               pvPara, 操作参数, 实际传入的往往是属性列表
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/

CFG_RET CFG_FilterNodeDonotSetActiveNoti(const scew_element *pstNode, void *pvPara)
{
     int idx , i = 0;
     char *(*papcAttrVal)[2] = pvPara;
     char * pcFilterNode[]={"ManufacturerOUI","SerialNumber", "ProductClass","UpTime","DeviceLog"};

     for(idx = 0 ; idx < sizeof(pcFilterNode)/sizeof(pcFilterNode[0]); idx++)
    {
           if(0==strcmp(pcFilterNode[idx], pstNode->name))
	    {
                 while (NULL != papcAttrVal[i][0])
                 {
                      if (0 == strcmp(papcAttrVal[i][0], ATTR_NOTI))
                      {
                              if (ACTIVE_NOTI_C == papcAttrVal[i][1][0])
                              {
                                     //CFG_ERR(ERR_CFG_REJECT_ATTR_SET);
                                     return ERR_CFG_REJECT_ATTR_SET;
                              }
                       }
                       i++;
                  }
            }
     }
     return CFG_OK;
}


/*************************************************************************
Function:      CFG_RET CFG_CheckSingleNodeAttrSet(const scew_element *pstNode,
                                                  void *pvPara)
Description:   对单个节点设置属性的进行检查的函数
Calls:
Data Accessed:
Data Updated:
Input:         pstNode, 要遍历的子树,
               pvPara, 操作参数, 实际传入的往往是属性列表
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_CheckSingleNodeAttrSet(const scew_element *pstNode, void *pvPara)
{
    scew_attribute *pstAttr = NULL;
    const char *pcAttrVal = NULL;
    char *(*papcAttrVal)[2] = pvPara;
    unsigned long i = 0;
    CFG_RET ret = CFG_OK;
    /* 根据类型进行拦截 */
    CFG_FILTER_NON_LEAF(pstNode);

    /* 过滤不能设置为主动上报的的节点  */
    ret = CFG_FilterNodeDonotSetActiveNoti(pstNode, pvPara);
    if(ret != CFG_OK)
    {
          return ret;
    }

    pstAttr = scew_attribute_by_name(pstNode, ATTR_STAT);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
        if (ISSTAT_C == pcAttrVal[0])
        {
            while (NULL != papcAttrVal[i][0])
            {
                if (0 == strcmp(papcAttrVal[i][0], ATTR_NOTI))
                {
                    if (ACTIVE_NOTI_C == papcAttrVal[i][1][0]
                        || PASSIVE_NOTI_C == papcAttrVal[i][1][0])
                    {
                        CFG_ERR(ERR_CFG_ATTR_STAT);

                        return ERR_CFG_ATTR_STAT;
                    }
                }

                i++;
            }
        }
    }

    return CFG_OK;
}




/*************************************************************************
Function:      CFG_RET CFG_ParseAttrVal(char *pcVal, void **pvOut)
Description:   解析属性列表, 将 xxx="abc def"这样的格式解析成指针对
               (属性名, 属性值),
Calls:
Data Accessed:
Data Updated:
Input:         pcVal, 属性列表
Output:        pvOut, 属性指针对
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_ParseAttrVal(char *pcVal, void **pvOut)
{
    enum
    {
        NONE= 1,
        NAME,
        MIDDLE,
        VAL,
        VAL_END
    };

    unsigned long ulCount = 1;
    char *pcValWork = pcVal;
    char *(*papcAttrList)[2] = NULL;
    unsigned char ucState = NONE;

    while (0 != *pcValWork)
    {
        if ('=' == *pcValWork)
        {
            ulCount++;
        }
        pcValWork++;
    }
    pcValWork = pcVal;

    papcAttrList = malloc(2 * sizeof(char *) * ulCount);
    if (NULL == papcAttrList)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }
    ulCount = 0;

    while ('\0' != *pcValWork)
    {
        switch (*pcValWork)
        {
            case '=':
                if (NAME == ucState)
                {
                    ucState = MIDDLE;
                    *pcValWork = '\0';
                }
                break;
            case ' ':
                if (VAL_END == ucState)
                {
                    ucState = NONE;
                }
                break;
            case '\"':
                if (MIDDLE == ucState)
                {
                    ucState = VAL;
                    papcAttrList[ulCount][1] = pcValWork + 1;
                    ulCount++;
                }
                else if (VAL == ucState)
                {
                    ucState = VAL_END;
                    *pcValWork = '\0';
                }
                break;
            default:
                switch (ucState)
                {
                    case NONE:
                        ucState = NAME;
                        papcAttrList[ulCount][0] = pcValWork;
                        break;
                    default:
                        break;
                }
                break;
        }

        pcValWork++;
    }

    papcAttrList[ulCount][0] = NULL;
    papcAttrList[ulCount][1] = NULL;

    *pvOut = papcAttrList;

    return CFG_OK;
}






/* 注册通知报告的接口 */
/*************************************************************************
Function:      CFG_RET CFG_RegNotiInformIntf(FUNC_NOTI_INFORM pfnNotiInform)
Description:   注册通知报告的接口
Calls:
Data Accessed:
Data Updated:
Input:         pfnNotiInform, 函数指针
Output:
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_RegNotiInformIntf(FUNC_NOTI_INFORM pfnNotiInform)
{
    if (NULL == pfnNotiInform)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    s_pfnCfgNotiInform = pfnNotiInform;
    return CFG_OK;
}



/*************************************************************************
Function:      CFG_RET CFG_NotiInform(const scew_element *pstNode, const ST_CFG_NODE_INFO *pstInfo)
Description:   统一处理节点改变的Notification的函数
Calls:
Data Accessed:
Data Updated:
Input:         pstNode, 节点地址
               pstInfo, 节点信息.
Output:
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_NotiInform(const scew_element *pstNode, const ST_CFG_NODE_INFO *pstInfo)
{
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;
    const char *pcLimit = NULL;
    const char *pcLast = NULL;
    unsigned char ucNotiType = pstInfo->ucNoti;
    struct timeval stCurTime = {0, 0};
    unsigned long ulLast = 0;
    unsigned long ulLimit = 0;
    CFG_RET ret = CFG_OK;
    char acLst[32];

#ifndef _CFG_TREE_ALL_STANDARD
    if (0 == pstInfo->ucStandard)
    {
        return CFG_OK;
    }
#endif

    if (CFG_ACTIVE_NOTI == ucNotiType || CFG_PASSIVE_NOTI == ucNotiType)    /* 节点Noti属性为主动或者被动 */
    {
        /* 获取时间限制 */
        pstAttr = scew_attribute_by_name(pstNode, ATTR_INFORM_LIMIT);
        if (NULL != pstAttr)    /* 成功 */
        {
            pcLimit = scew_attribute_value(pstAttr);
            ulLimit = (unsigned long)strtol(pcLimit, NULL, 10);

            /* 获取当前时间 */
            ret = gettimeofday(&stCurTime, NULL);
            if (CFG_OK != ret)
            {
                stCurTime.tv_sec = 0;
                stCurTime.tv_usec = 0;
            }

            /* 获取上次通知的时间 */
            pstAttr = scew_attribute_by_name(pstNode, ATTR_LAST_INFORM);
            if (NULL != pstAttr)    /* 成功 */
            {
                pcLast = scew_attribute_value(pstAttr);
                ulLast = (unsigned char)strtol(pcLast, NULL, 10);

                /* 计算时间差 */
                if (ulLimit > stCurTime.tv_sec - ulLast)  /* 时间未到 */
                {
                    /* 直接返回 */
                    return CFG_OK;
                }
            }
        }

        /* 置节点 报告标志为 真 */
        CFG_SETATTR(pstNode, ATTR_NEED_NOTI, NEEDNOTI);

        /* 发送通知 */
        ret = s_pfnCfgNotiInform(ucNotiType);
        if (CFG_OK != ret)
        {
            CFG_ERR(ret);
        }

#if defined(CONFIG_APPS_LOGIC_Argentina) || defined(CONFIG_TELEFONICA_COLOMBIA)
        /* ACS平台的修改操作不上报  */
        if (access("/var/tmp/tr069Send", F_OK) == 0)
        {
            CFG_SETATTR(pstNode, ATTR_NEED_NOTI, NONEEDNOTI);
            system("rm -rf /var/tmp/tr069Send");
        }
#endif

        if (NULL != pcLimit)   /* 有时间限制 */
        {
            /* 更新上次通知时间为当前时间 */
            sprintf(acLst, "%lu", stCurTime.tv_sec);
            CFG_SETATTR(pstNode, ATTR_LAST_INFORM, acLst);
        }
    }

    return CFG_OK;
}




/***********************************************************
接口:   设置节点内容
参数:   pcPath: 节点路径
        pcVal: 要设置成的值
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_SetNodeVal(const char *pcPath, const char *pcVal, const char *pcAccessor)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    const char *pcValSet = NULL;
    const char *pcValOld = NULL;
    ST_CFG_NODE_INFO stNodeInfo;
    unsigned long ulMask = CFG_NODE_INFO_STANDARD | CFG_NODE_INFO_NOTI;

//  ulMask |= CFG_NODE_INFO_ACCESSLIST;

    if (NULL == pcPath || NULL == pcVal)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }
    (void)pcAccessor;

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, &stNodeInfo, ulMask);

    /* 如果值一致, 则不做操作 */
    pcValOld = scew_element_contents(pstNode);
    if ((NULL != pcValOld) && (0 == strcmp(pcValOld, pcVal)))
    {
        return CFG_OK;
    }

    /* 根据访问者过滤 */
//    CFG_SET_RIGHT_CHECK(pstNode, &stNodeInfo, pcAccessor, CFG_WRITABLE_C);

    /* 为节点设置内容 */
    pcValSet = scew_element_set_contents(pstNode, pcVal);
    if (NULL == pcValSet)
    {
        CFG_ERR(ERR_CFG_INTERNAL_ERR);
        return ERR_CFG_INTERNAL_ERR;
    }

    (void)CFG_NotiInform(pstNode, &stNodeInfo);

    return CFG_OK;
}






/***********************************************************
接口:   设置节点属性
参数:   pcPath: 节点路径,
        pstAttVal: 属性值结构,
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_SetNodeAttrOpt(const ST_PARA_VAL *pstAttSet, FUNC_SET_OPT pfnOpt)
{
    char *pcAttrVal = NULL;
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    unsigned long ulAttrLen = 0;
    char *(*papcAttrVal)[2] = NULL;
    ST_CFG_NODE_INFO stNodeInfo;
    unsigned long ulMask = CFG_NODE_INFO_STANDARD;

    if (NULL == pstAttSet
        || NULL == pstAttSet->pcName
        || NULL == pstAttSet->pcVal)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    CFG_DUP_AND_SEARCH_NODE(pstAttSet->pcName, pstNode, &stNodeInfo, ulMask);
    CFG_GET_RIGHT_CHECK_BY_INFO(&stNodeInfo, g_pcCfgAccessor);

    ulAttrLen = strlen(pstAttSet->pcVal) + 1;
    pcAttrVal = malloc(ulAttrLen);
    if (NULL == pcAttrVal)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }

    /* 解析两个属性值 */
    strcpy(pcAttrVal, pstAttSet->pcVal);
    ret = CFG_ParseAttrVal(pcAttrVal, (void **)(void *)&papcAttrVal);
    if (CFG_OK != ret)
    {
        free(pcAttrVal);

        CFG_ERR(ret);
        return ret;
    }

    if (pfnOpt == CFG_SetSingleNodeAttr)
    {
        /* 注意, 此处应当检查路径和节点类型是否对应 */

        /* 直接调用操作接口 */
        ret = pfnOpt(pstNode, papcAttrVal);
    }
    else
    {
        /* pcPath 以'.' 结尾 */
        if (TREE_FLAG_C == pstAttSet->pcName[strlen(pstAttSet->pcName) - 1])
        {
            /* 调用全遍历接口 CFG_TreeSetAccess */
            ret = CFG_TreeSetAccess(pstNode, pfnOpt, papcAttrVal);
        }
        else
        {
            /* 直接调用操作接口 */
            ret = pfnOpt(pstNode, papcAttrVal);
        }
    }
    free(papcAttrVal);
    free(pcAttrVal);

    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }
    return ret;
}










/***********************************************************
接口:   设置节点属性
参数:   pcPath: 节点路径,
        pstAttVal: 属性值结构,
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_SetNodeAttr(const ST_PARA_VAL *pstAttSet, const char *pcAccessor)
{
    g_pcCfgAccessor = pcAccessor;

    return CFG_SetNodeAttrOpt(pstAttSet, CFG_SetSingleNodeAttr);
}


/***********************************************************
接口:   设置节点属性
参数:   pcPath: 节点路径,
        pstAttVal: 属性值结构,
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_CheckNodeAttrSet(const ST_PARA_VAL *pstAttSet, const char *pcAccessor)
{
    g_pcCfgAccessor = pcAccessor;

    return CFG_SetNodeAttrOpt(pstAttSet, CFG_CheckSingleNodeAttrSet);
}



/***********************************************************
接口:   清除通知
参数:   无
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_ClearNoti(void)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;

    g_pcCfgAccessor = "";
    pstNode = scew_tree_root(g_pstCfgTree);

    ret = CFG_TreeSetAccess(pstNode, CFG_ClearSingleNoti, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }
    return ret;
}



/***********************************************************
接口:   将单个节点名称转义回去
参数:   pstNode, 节点地址
        pvPara, 无意义
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_SingleNodeTransBack(scew_element *pstNode, void *pvPara)
{
    char acNodeName[64];
    const char *pcNodeName = NULL;

    (void)pvPara;

    pcNodeName = scew_element_name(pstNode);
    if (pcNodeName[1] >= '0' && pcNodeName[1] <= '9'
                           && '_' == pcNodeName[0])
    {
        strcpy(acNodeName, &pcNodeName[1]);
        pcNodeName = scew_element_set_name(pstNode, acNodeName);
        if (NULL == pcNodeName)
        {
            CFG_ERR(ERR_CFG_INTERNAL_ERR);
            return ERR_CFG_INTERNAL_ERR;
        }
    }

    return CFG_OK;
}

/* 将节点树再转义回去 */
/***********************************************************
接口:   将子树节点名称转义回去
参数:   pstTree, 子树地址
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_TreeTransBack(scew_tree *pstTree)
{
    CFG_RET ret = CFG_OK;
    scew_element *pstNode = NULL;
    scew_tree *pstTreeWork = pstTree;

    if (NULL == pstTreeWork)
    {
        pstTreeWork = g_pstCfgTree;
    }

    g_pcCfgAccessor = NULL;
    pstNode = scew_tree_root(pstTreeWork);
    ret = CFG_TreeSetAccess(pstNode, (FUNC_SET_OPT)CFG_SingleNodeTransBack, NULL);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    return ret;
}







/***********************************************************
接口:   检查删除节点的合法性
参数:   pcPath, 节点路径,
        pcAccessor, 访问者
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_CheckAndSetNodeDeleting(const char *pcPath, const char *pcAccessor)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    scew_attribute *pstAttr = NULL;
    const char *pcAttrVal = NULL;
    const char *pcName = NULL;
    ST_CFG_NODE_INFO stNodeInfo;
    unsigned long ulMask = CFG_NODE_INFO_STANDARD
                         | CFG_NODE_INFO_ACCESSLIST;

    if (NULL == pcPath)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, &stNodeInfo, ulMask);

    /* 检查节点名是否为数字 */
    pcName = scew_element_name(pstNode);
    if (!CFG_IS_NODE_NUM(pcName))
    {
        CFG_ERR(ERR_CFG_REJECT_DELETE_SET);
        return ERR_CFG_REJECT_DELETE_SET;
    }

    /* 取父节点, 检查权限 */
    CFG_SET_RIGHT_CHECK(pstNode->parent, &stNodeInfo, pcAccessor, CFG_DELETABLE_C);
    CFG_SET_RIGHT_CHECK(pstNode, &stNodeInfo, pcAccessor, CFG_WRITABLE_C);

//    CFG_SETATTR(pstNode, ATTR_STARTDEL, "1");

    return CFG_OK;
}


/***********************************************************
接口:   设置节点属性值
参数:   pcPath, 节点路径,
        pcAttr, 属性名
        pcVal, 要设置成的属性值
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_SetNodeAttrVal(const char *pcPath,
         const char *pcAttr, const char *pcVal)
{
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    scew_attribute *pstAttr = NULL;
    const char *pcAttrVal = NULL;

    if (NULL == pcPath || NULL == pcAttr || NULL == pcVal)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    /* 复制路径并查找节点 */
    CFG_DUP_AND_SEARCH_NODE(pcPath, pstNode, NULL, 0);

    CFG_SETATTR(pstNode, pcAttr, pcVal);

    return CFG_OK;
}




#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


