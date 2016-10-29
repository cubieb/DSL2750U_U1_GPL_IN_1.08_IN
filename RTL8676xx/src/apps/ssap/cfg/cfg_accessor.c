/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cfg_accessor.c
 文件描述 : cfg 模块中各种遍历树方法的实现,
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



#define CFG_TREE_GET_OPT(pstNode, pfnGetOpt, pcAbsPath, ucLen, pvData) \
{ \
    ret = pfnGetOpt(pstNode, pcAbsPath, ucLen, pvData); \
    if (CFG_OK != ret) \
    { \
        return ret; \
    } \
}


/*************************************************************************
Function:      long CFG_TreeAccess(const scew_element *pstNode)
Description:   普通的遍历子树的函数(所有取遍历取值均用此函数)
Calls:
Data Accessed: g_pfnCfgTreeOpt
Data Updated:  g_acCfgAbsPath
Input:         pstNode, 要遍历的子树
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_TreeGetAccess(const scew_element *pstNode, FUNC_GET_OPT pfnGetOpt,
                                          char *pcAbsPath, void *pvData)
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
    ucStackTop = 1;

    /* 检查该子树是否标准 */
    CFG_PASS_NOSTANDARD(pstNodeWork, g_pcCfgAccessor, ((void)0));
    if (NULL == pstNodeWork)
    {
        CFG_ERR(ERR_CFG_NOT_STANDARD, "%s", pcAbsPath);
        return ERR_CFG_NOT_STANDARD;
    }

    /* 操作 */
    CFG_TREE_GET_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
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
                CFG_NODE_TO_NUM(pcNodeName);
                pcAbsPath[astStack[ucStackTop - 1].ucPathLen] = DELIMIT_C;
                strcpy(pcAbsPath + astStack[ucStackTop - 1].ucPathLen + 1, pcNodeName);

                /* 进栈 */
                astStack[ucStackTop].pstPath = pstNodeWork;
                astStack[ucStackTop].ucPathLen
                    = astStack[ucStackTop - 1].ucPathLen  + strlen(pcNodeName) + 1;
                ucStackTop++;

                /* 在访问者是Tr069的情况下, 放过非标准节点 */
                CFG_PASS_NOSTANDARD(pstNodeWork, g_pcCfgAccessor, break);

                /* 操作 */
                CFG_TREE_GET_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
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
            CFG_NODE_TO_NUM(pcNodeName);
            pcAbsPath[astStack[ucStackTop - 1].ucPathLen] = DELIMIT_C;
            strcpy(pcAbsPath + astStack[ucStackTop - 1].ucPathLen + 1, pcNodeName);

            /* 进栈 */
            astStack[ucStackTop].pstPath = pstNodeWork;
            astStack[ucStackTop].ucPathLen
                = astStack[ucStackTop - 1].ucPathLen  + strlen(pcNodeName) + 1;
            ucStackTop++;

            /* 在访问者是Tr069的情况下, 放过非标准节点 */
            CFG_PASS_NOSTANDARD(pstNodeWork, g_pcCfgAccessor, continue);

            /* 操作 */
            CFG_TREE_GET_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
                     astStack[ucStackTop - 1].ucPathLen, pvData);
        }
    }

    return CFG_OK;
}


/*************************************************************************
Function:      long CFG_NextLevelAccess(const scew_element *pstNode)
Description:   遍历下层节点的函数, 用在只取下层节点名称列表时
Calls:
Data Accessed: g_pfnCfgTreeOpt
Data Updated:  g_acCfgAbsPath
Input:         pstNode, 要遍历的子树
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_TreeGetNextLevelAccess(const scew_element *pstNode,
            FUNC_GET_OPT pfnGetOpt, char *pcAbsPath, void *pvData)
{
    const scew_element *pstNodeWork = pstNode;
    scew_element *pstChild = NULL;
    scew_element *pstChildPrev = NULL;
    long ret = CFG_OK;
    unsigned char ucLen = strlen(pcAbsPath);
    const char *pcNodeName = NULL;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;

    while (pstChild = scew_element_next(pstNodeWork, pstChildPrev),
           NULL != pstChild)
    {
        pstChildPrev = pstChild;

        /* 在访问者是Tr069的情况下, 放过非标准节点 */
        CFG_PASS_NOSTANDARD(pstChild, g_pcCfgAccessor, continue);

        /* 对节点名进行转义 */
        pcNodeName = scew_element_name(pstChild);
        CFG_NODE_TO_NUM(pcNodeName);

        /* 更新绝对路径 */
        pcAbsPath[ucLen] = DELIMIT_C;
        strcpy(&pcAbsPath[ucLen + 1], pcNodeName);

        /* 操作 */
        CFG_TREE_GET_OPT(pstChild, pfnGetOpt, pcAbsPath,
                ucLen + strlen(pcNodeName) + 1, pvData);
    }

    return CFG_OK;
}





#define CFG_TREE_SET_OPT(pstNode, pfnSetOpt, pvData) \
{ \
    ret = pfnSetOpt(pstNode, pvData); \
    if (CFG_OK != ret) \
    { \
        return ret; \
    } \
}



/* 对树做设置操作时的遍历函数, 之所以不是跟取值用同一个函数,
   因为取值时需要记录完整路径,而设置时则不用 */
/*************************************************************************
Function:      long CFG_GetNode(const void *pstTree, char *pcPath, void **ppvNode)
Description:   对树做设置操作时的遍历函数, 之所以不是跟取值用同一个函数,
                 因为取值时需要记录完整路径,而设置时则不用
Calls:         g_pfnCfgTreeSetOpt
Data Accessed:
Data Updated:
Input:         pstNode, 要遍历的子树
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_TreeSetAccess(const scew_element *pstNode,
                          FUNC_SET_OPT pfnSetOpt, void *pvData)
{
    const scew_element *pstNodeWork = pstNode;
    const scew_element *pstChild = NULL;
    const scew_element *pstParent = NULL;
    long ret = CFG_OK;
    const scew_element *apstStack[CFG_ACCESS_STACK_DEEPTH];
    unsigned char ucStackTop = 0;
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;

    apstStack[0] = pstNode;
    ucStackTop++;

    /* 检查该子树是否标准 */
    CFG_PASS_NOSTANDARD(pstNodeWork, g_pcCfgAccessor, ((void)0));
    if (NULL == pstNodeWork)
    {
        CFG_ERR(ERR_CFG_NOT_STANDARD, "%s", scew_element_name(pstNode));
        return ERR_CFG_NOT_STANDARD;
    }

    /* 操作 */
    CFG_TREE_SET_OPT(pstNodeWork, pfnSetOpt, pvData);

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

                /* 在访问者是Tr069的情况下, 放过非标准节点 */
                CFG_PASS_NOSTANDARD(pstNodeWork, g_pcCfgAccessor, break);

                /* 操作 */
                CFG_TREE_SET_OPT(pstNodeWork, pfnSetOpt, pvData);
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

            /* 在访问者是Tr069的情况下, 放过非标准节点 */
            CFG_PASS_NOSTANDARD(pstNodeWork, g_pcCfgAccessor, continue);

            /* 操作 */
            CFG_TREE_SET_OPT(pstNodeWork, pfnSetOpt, pvData);
        }
    }

    return CFG_OK;
}







/*************************************************************************
Function:      void CFG_FillDumpBuf(const char *pcBuf)
Description:   将字符串输出到 pstDumpBuf 中的缓冲区
Calls:
Data Accessed: pstDumpBuf
Data Updated:
Input:         pcBuf, 要输出的字符串
Output:        无
Return:        无
Others:
*************************************************************************/
void CFG_FillDumpBuf(const char *pcBuf, unsigned long ulLen, ST_CFG_DUMP_BUF *pstDumpBuf)
{
    if ((pstDumpBuf->ulInfactLen == pstDumpBuf->ulPos)
        && (pstDumpBuf->ulLen - pstDumpBuf->ulPos >= ulLen))
    {
        CFG_MemCpy(pstDumpBuf->pcBuf + pstDumpBuf->ulPos, pcBuf, ulLen);
        pstDumpBuf->ulPos += ulLen;
    }

    pstDumpBuf->ulInfactLen += ulLen;
}



/*************************************************************************
Function:      int CFG_EscapeXML(char *buf, int len, const char *text, const char **ppcNext)
Description:   字符转义
Calls:
Data Accessed: 无
Data Updated:  无
Input:         buf, 输出转义后的字符串
               len, text 的长度
               text, 原始字符串
               ppcNext, 转义后下一段字符串
Output:        无
Return:        0
Others:
*************************************************************************/
int CFG_EscapeXML(char *buf, int len, const char *text, const char **ppcNext)
{
    int c = 0;

    while(*text && len > 0)
    {
        switch(*text)
        {
            case '<':
                *buf++ = '&';
                *buf++ = 'l';
                *buf++ = 't';
                *buf++ = ';';
                len -= 4;
                c += 4;
                break;
            case '>':
                *buf++ = '&';
                *buf++ = 'g';
                *buf++ = 't';
                *buf++ = ';';
                len -= 4;
                c += 4;
                break;
            case '&':
                *buf++ = '&';
                *buf++ = 'a';
                *buf++ = 'm';
                *buf++ = 'p';
                *buf++ = ';';
                len -= 5;
                c += 5;
                break;
            case '"':
                *buf++ = '&';
                *buf++ = 'q';
                *buf++ = 'u';
                *buf++ = 'o';
                *buf++ = 't';
                *buf++ = ';';
                len -= 6;
                c += 6;
                break;
            default:
                *buf++ = *text;
                len--;
                c++;
        }
        text++;
    }

    if (NULL != ppcNext)
    {
        if (*text != '\0')
        {
            *ppcNext = text;
        }
        else
        {
            *ppcNext = NULL;
        }
    }

    return c;
}


/*************************************************************************
Function:      void CFG_TreeDumpAccess(const scew_element *pElement)
Description:   按xml格式 dump子树
Calls:
Data Accessed: pstDumpBuf
Data Updated:
Input:         pElement, 要dump的子树
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_TreeDumpAccess(const scew_element *pElement,
                 ST_CFG_DUMP_BUF *pstDumpBuf, unsigned char ucTable)
{
    scew_element *pChild = NULL;
    XML_Char const* pContents = NULL;
    scew_attribute* pstAttr = NULL;
    const char *pcAttrVal = NULL;
    char acLocalBuffer[1024];
    const char *pcNodeName = NULL;
    unsigned char i = 0;
    unsigned long ulLen = 0;
    char *pcBuf = NULL;

    if (NULL == pElement)
    {
        return ERR_CFG_PARA_INVALID;
    }

    /* 根据访问者进行拦截 */
    CFG_GET_RIGHT_CHECK(pElement, g_pcCfgAccessor);

    /* 过滤无须保存的节点 */
    CFG_PASS_NONEEDSAVE(pElement);

    /**
     * Prints the starting element tag with its attributes.
     */
    pcNodeName = scew_element_name(pElement);
    CFG_NODE_TO_NUM(pcNodeName);

    acLocalBuffer[0] = '\n';
    for (i = 0; i < ucTable; i++)
    {
        acLocalBuffer[i+1] = '\t';
    }
    acLocalBuffer[i+1] = '\0';
    CFG_FillDumpBuf(acLocalBuffer, i+1, pstDumpBuf);

    ulLen = (unsigned long)sprintf(acLocalBuffer, "<%s", pcNodeName);
    CFG_FillDumpBuf(acLocalBuffer, ulLen, pstDumpBuf);

    pstAttr = NULL;
    while (pstAttr = scew_attribute_next(pElement, pstAttr),
           NULL != pstAttr)
    {
        if (0 == strcmp(ATTR_MID, scew_attribute_name(pstAttr))
            || 0 == strcmp(ATTR_CAREMID, scew_attribute_name(pstAttr)))
        {
            continue;
        }

        pcBuf = acLocalBuffer;
        ulLen = (unsigned long)sprintf(acLocalBuffer," %s=\"",
                                   scew_attribute_name(pstAttr));
        pcBuf += ulLen;
        ulLen = (unsigned long)CFG_EscapeXML(pcBuf,
                                 sizeof(acLocalBuffer) - ulLen,
                                 scew_attribute_value(pstAttr), NULL);
        pcBuf += ulLen;
        strcpy(pcBuf, "\"");

        CFG_FillDumpBuf(acLocalBuffer,
            (unsigned long)((pcBuf - acLocalBuffer) + 1),pstDumpBuf);
    }
    CFG_FillDumpBuf(">", 1, pstDumpBuf);

    /**
     * Call print_element function again for each child of the
     * current element.
     */
    pChild = NULL;
    while (pChild = scew_element_next(pElement, pChild),
           NULL != pChild)
    {
        (void)CFG_TreeDumpAccess(pChild, pstDumpBuf, ucTable + 1);
    }

    if (0 < scew_element_count(pElement))
    {
        acLocalBuffer[0] = '\n';
        for (i = 0; i < ucTable; i++)
        {
            acLocalBuffer[i+1] = '\t';
        }
        acLocalBuffer[i+1] = '\0';
        CFG_FillDumpBuf(acLocalBuffer, i + 1, pstDumpBuf);
    }

    pContents = scew_element_contents(pElement);

    /* Prints element's content. */

    if(pContents != NULL)
    {
	if (0 == safe_strcmp(pcNodeName, "ConfigFile"))
	{
	    ulLen = strlen(pContents);
           CFG_FillDumpBuf(pContents, ulLen, pstDumpBuf);
		   
	}else{
           const char *pcNext = pContents;
           while (NULL != pcNext)
           {
               ulLen = (unsigned long)CFG_EscapeXML(acLocalBuffer,
                              sizeof(acLocalBuffer), pcNext, &pcNext);
               CFG_FillDumpBuf(acLocalBuffer, ulLen, pstDumpBuf);
           }
	}
    }

    /**
     * Prints the closing element tag.
     */
    ulLen = (unsigned long)sprintf(acLocalBuffer,"</%s>", pcNodeName);
    CFG_FillDumpBuf(acLocalBuffer, ulLen, pstDumpBuf);

    return CFG_OK;
}


#ifdef _CFG_DEBUG
/*************************************************************************
Function:      void CFG_TreeDumpAccess(const scew_element *pElement)
Description:   按xml格式 dump子树
Calls:
Data Accessed: pstDumpBuf
Data Updated:
Input:         pElement, 要dump的子树
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_TreePrintAccess(const scew_element *pstNode,
                     ST_CFG_DUMP_BUF *pstDumpBuf, unsigned char ucTable)
{
    scew_element *pChild = NULL;
    XML_Char const* pContents = NULL;
    scew_attribute* pstAttr = NULL;
    const char *pcAttrVal = NULL;
    char acLocalBuffer[1024];
    const char *pcNodeName = NULL;
    unsigned char i = 0;
    unsigned long ulLen = 0;

    if (NULL == pstNode)
    {
        return ERR_CFG_PARA_INVALID;
    }

    /* 根据访问者进行拦截 */
    CFG_GET_RIGHT_CHECK(pstNode, g_pcCfgAccessor);

    /* 过滤无须保存的节点 */

    /**
     * Prints the starting element tag with its attributes.
     */
    pcNodeName = scew_element_name(pstNode);
    CFG_NODE_TO_NUM(pcNodeName);

    acLocalBuffer[0] = '\n';
    for (i = 0; i < ucTable; i++)
    {
        acLocalBuffer[i+1] = '\t';
    }
    acLocalBuffer[i+1] = '\0';
    CFG_FillDumpBuf(acLocalBuffer, i+1, pstDumpBuf);


    ulLen = (unsigned long)sprintf(acLocalBuffer, "<%s", pcNodeName);
    CFG_FillDumpBuf(acLocalBuffer, ulLen, pstDumpBuf);

    pstAttr = NULL;
    while (pstAttr = scew_attribute_next(pstNode, pstAttr),
           NULL != pstAttr)
    {
/*      if (0 == strcmp(ATTR_MID, scew_attribute_name(pstAttr))
            || 0 == strcmp(ATTR_CAREMID, scew_attribute_name(pstAttr)))
        {
            continue;
        } */
        ulLen = (unsigned long)sprintf(acLocalBuffer," %s=\"%s\"",
                  scew_attribute_name(pstAttr), scew_attribute_value(pstAttr));
        CFG_FillDumpBuf(acLocalBuffer, ulLen, pstDumpBuf);
    }
    CFG_FillDumpBuf(">", 1, pstDumpBuf);

    /**
     * Call print_element function again for each child of the
     * current element.
     */
    pChild = NULL;
    while (pChild = scew_element_next(pstNode, pChild),
           NULL != pChild)
    {
        (void)CFG_TreePrintAccess(pChild, pstDumpBuf, ucTable + 1);
    }

    if (0 < scew_element_count(pstNode))
    {
        acLocalBuffer[0] = '\n';
        for (i = 0; i < ucTable; i++)
        {
            acLocalBuffer[i+1] = '\t';
        }
        acLocalBuffer[i+1] = '\0';
        CFG_FillDumpBuf(acLocalBuffer, i + 1, pstDumpBuf);
    }

    pContents = scew_element_contents(pstNode);

    /* Prints element's content. */
    if (pContents != NULL)
    {
        CFG_FillDumpBuf(pContents, strlen(pContents), pstDumpBuf);
    }

    /**
     * Prints the closing element tag.
     */
    ulLen = (unsigned long)sprintf(acLocalBuffer,"</%s>", pcNodeName);
    CFG_FillDumpBuf(acLocalBuffer, ulLen, pstDumpBuf);

    return CFG_OK;
}
#endif




/* 非递归遍历树的栈 */
ST_CFG_TREE_STACK_NODE s_astCfgTreeStack[CFG_ACCESS_STACK_DEEPTH];

unsigned char s_ucCfgTreeStackPos = 0;    /* 非递归遍历树的栈顶 */

/* 非递归操作的时存放绝对路径 */
char  s_acCfgSavePath[CFG_MAX_PATH_LEN];

/* 表示开始遍历时是否从子树开始, 在CFG_SetAccessBasicInfo 中设置 */
unsigned char s_ucCfgCurIsRoot = 0;


/*************************************************************************
Function:      CFG_RET CFG_SetMIDAccessInfo(const char *pcPath, const void **ppvTree,
                           unsigned short *pusMID, const char *pcAccessor,
                           char **ppcPathSave)
Description:   设置非递归遍历MID的基本信息
Calls:
Data Accessed:
Data Updated:  s_astCfgTreeStack
               s_ucCfgTreeStackPos
               s_ucCfgCurIsRoot
               s_acCfgSavePath
Input:         pcPath, 节点路径
               pcAccessor, 访问者
Output:        ppvTree, 子树指针, 如果pcPath表示为取子树
               pusMID, 节点所属的mid
               ppcPathSave, 存放全路径的缓冲区地址
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_SetMIDAccessInfo(const char *pcPath, const void **ppvTree,
           unsigned short *pusMID, const char *pcAccessor,
           char **ppcPathSave)
{
    CFG_RET ret = CFG_OK;
    unsigned long ulLen = 0;
    unsigned short usMID = 0;
    void *pvTree = NULL;

    ulLen = strlen(pcPath);
    if (ulLen > 0 && TREE_FLAG_C == pcPath[ulLen - 1])
    {
        ret = CFG_GetNodeMIDList(pcPath, pcAccessor,
                            &usMID, NULL, NULL, &pvTree, NULL);
        if (CFG_OK == ret)
        {
            if (TREE_FLAG_C == pcPath[0])   /* 当pcPath = "." 时, 表示取根 */
            {
                s_astCfgTreeStack[0].ucPathLen = 0;
            }
            else
            {
                strcpy(s_acCfgSavePath, pcPath);
                s_astCfgTreeStack[0].ucPathLen = (ulLen - 1)
                                - strlen(scew_element_name(pvTree));
            }
            s_astCfgTreeStack[0].stNodeInfo.usMID = usMID;  /* 注意, 这个赋值不可少,
                                          否则, 当该节点及以下没有MID时,
                                          遍历结果会造成所有叶子都没有MID的情况 */
            s_astCfgTreeStack[0].pstPath = NULL;
            s_ucCfgTreeStackPos = 1;
            s_ucCfgCurIsRoot = 1;   /* 表示后面开始遍历的节点是个子树 */

            *ppvTree = pvTree;
            *pusMID = usMID;
        }
    }
    else
    {
        ret = CFG_GetNodeMIDList(pcPath, pcAccessor,
                       &usMID, NULL, NULL, &pvTree, NULL);
        if (CFG_OK == ret)
        {
            if ('\0' == pcPath[0])
            {
                strcpy(s_acCfgSavePath, scew_element_name(pvTree));
            }

            *ppvTree = NULL;
            *pusMID = usMID;
        }
    }

    *ppcPathSave = s_acCfgSavePath;

    return ret;
}



/*************************************************************************
Function:      void CFG_GetPathLenAndMID(scew_element *pstPath, unsigned
short *pusMID,
                                                  unsigned char *pucLen)
Description:   在非递归遍历中用于根据栈取得当前节点的路径和MID
Calls:
Data Accessed:
Data Updated:  s_astCfgTreeStack
               s_ucCfgTreeStackPos
               s_acCfgSavePath
Input:         pstPath, 节点指针
Output:        pusMID, 节点所属的mid
               pucLen, 路径长度
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
void CFG_GetPathLenAndMID(const scew_element *pstPath, unsigned short *pusMID,
                                                  unsigned char *pucLen)
{
    const scew_element *pstPathWork = pstPath;
    scew_attribute *pstAttr = NULL;
    const char *pcAttrVal = NULL;
    unsigned short usMID = 0;
    unsigned char  ucFormerLen = 0;
    unsigned char  ucPathLen = 0;
    const char *pName = NULL;

    usMID = s_astCfgTreeStack[s_ucCfgTreeStackPos - 1].stNodeInfo.usMID;
    ucFormerLen = s_astCfgTreeStack[s_ucCfgTreeStackPos - 1].ucPathLen;

    pstAttr = scew_attribute_by_name(pstPathWork, ATTR_MID);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
        usMID = strtol(pcAttrVal, NULL, CFG_MID_BASE);
    }

    /* 对节点名转义 */
    pName = scew_element_name(pstPathWork);
    CFG_NODE_TO_NUM(pName);

    strcpy(s_acCfgSavePath + ucFormerLen, pName);
    ucPathLen = ucFormerLen + strlen(pName);
    pstAttr = scew_attribute_by_name(pstPathWork, ATTR_TYPE);
    if (NULL != pstAttr)
    {
        pcAttrVal = scew_attribute_value(pstAttr);
        if (NULL != pcAttrVal
            && (0 == strcmp(pcAttrVal, TYPE_CLASS)
                || 0 == strcmp(pcAttrVal, TYPE_OBJECT)))
        {
            s_acCfgSavePath[ucPathLen] = DELIMIT_C;
            ucPathLen++;
            s_acCfgSavePath[ucPathLen] = '\0';
        }
    }

    *pusMID = usMID;
    *pucLen = ucPathLen;
}



/*************************************************************************
Function:     void *CFG_GetNextNode(void *pvPath, unsigned short *pusMID,
                                                   unsigned char *pucLen)
Description:   非递归遍历MID, 取下一个MID
Calls:
Data Accessed: s_astCfgTreeStack
               s_ucCfgTreeStackPos
               s_ucCfgCurIsRoot
               s_acCfgSavePath
Data Updated:  s_astCfgTreeStack
               s_ucCfgTreeStackPos
               s_ucCfgCurIsRoot
               s_acCfgSavePath
Input:         pvPath, 当前节点指针
Output:        pusMID, 节点所属的mid
               pucLen, 路径长度
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
const void *CFG_GetNextMID(const void *pvPrev, unsigned short *pusMID,
                                    unsigned char *pucLen)
{
    const scew_element *pstParent = NULL;
    const scew_element *pstChild = NULL;
    const scew_element *pstPathWork = pvPrev;
    unsigned short usMID = 0;
    unsigned char  ucPathLen = 0;

    if (NULL == pstPathWork)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return NULL;
    }

    while (1)
    {
        /* 取兄弟 */
        if (0 == s_ucCfgCurIsRoot)
        {
            if (1 == s_ucCfgTreeStackPos)  /* 栈空 */
            {
                return NULL;
            }
            pstParent = s_astCfgTreeStack[s_ucCfgTreeStackPos - 1].pstPath;
            pstPathWork = scew_element_next(pstParent, pstPathWork);
        }
        else  /* 当前这一步是寻找子树的第一个叶节点, 因此, 不进行取兄弟的操作 */
        {
            s_ucCfgCurIsRoot = 0;
        }
        if (NULL != pstPathWork)   /* NULL != 兄弟 */
        {
            while (pstChild = scew_element_next(pstPathWork, NULL),
                   NULL != pstChild)  /* 取子节点, NULL != 子节点 */
            {
                /* 取MID */
                CFG_GetPathLenAndMID(pstPathWork, &usMID, &ucPathLen);

                /* 进栈 */
                s_astCfgTreeStack[s_ucCfgTreeStackPos].pstPath = pstPathWork;
                s_astCfgTreeStack[s_ucCfgTreeStackPos].stNodeInfo.usMID = usMID;
                s_astCfgTreeStack[s_ucCfgTreeStackPos].ucPathLen = ucPathLen;
                s_ucCfgTreeStackPos++;

                pstPathWork = pstChild;
            }

            /* 取MID */
            CFG_GetPathLenAndMID(pstPathWork, &usMID, &ucPathLen);

            /* MID 跟栈父节点的MID不一样 */
            if (usMID != s_astCfgTreeStack[s_ucCfgTreeStackPos - 1].stNodeInfo.usMID)
            {
                /* 输出 */
                *pusMID = usMID;
                *pucLen = ucPathLen;

                return pstPathWork;
            }
        }
        else
        {
            /* 出栈 */
            --s_ucCfgTreeStackPos;
            pstPathWork = s_astCfgTreeStack[s_ucCfgTreeStackPos].pstPath;
            usMID = s_astCfgTreeStack[s_ucCfgTreeStackPos].stNodeInfo.usMID;
            ucPathLen = s_astCfgTreeStack[s_ucCfgTreeStackPos].ucPathLen;
            s_acCfgSavePath[ucPathLen] = '\0';

            /* MID 跟栈父节点的MID不一样, 或者栈空 */
            if (usMID != s_astCfgTreeStack[s_ucCfgTreeStackPos - 1].stNodeInfo.usMID
                || 1 == s_ucCfgTreeStackPos)
            {
                /* 输出 */
                *pusMID = usMID;
                *pucLen = ucPathLen;

                return pstPathWork;
            }
        }
    }

    /* return NULL; */  /* 无法运行到 */
}









#define CFG_CHECK_AND_DEL_NODE(pstNode, expt) \
{ \
    pstAttr = scew_attribute_by_name(pstNode, ATTR_STARTDEL); \
    if (NULL != pstAttr) \
    { \
        pcAttrVal = scew_attribute_value(pstAttr); \
        if ('1' == pcAttrVal[0]) \
        { \
            void *pvLeft = pstNode->left; \
            scew_element_del(pstNode); \
            pstNode = pvLeft; \
            if (NULL == pstNode) \
            { \
                expt; \
            } \
            continue; \
        } \
    } \
}





/*************************************************************************
Function:      void CFG_RmvDeletingNode(scew_tree *pstTree)
Description:   删除启动时需要删除的节点
Calls:
Data Accessed: 无
Data Updated:  无
Input:         pstTree, 要操作的树指针
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
void CFG_RmvDeletingNode(scew_tree *pstTree)
{
    scew_element *pstNodeWork = NULL;
    scew_element *pstChild = NULL;
    scew_element *pstNodeNext = NULL;
    scew_element *pstParent = NULL;
    scew_element *apstStack[CFG_ACCESS_STACK_DEEPTH];
    unsigned char ucStackTop = 0;
    scew_attribute *pstAttr = NULL;
    const char *pcAttrVal = NULL;

    scew_tree *pstTreeWork = pstTree;
    if (NULL == pstTreeWork)
    {
        pstTreeWork = g_pstCfgTree;
    }

    pstNodeWork = scew_tree_root(pstTreeWork);
    apstStack[1] = pstNodeWork;
    ucStackTop++;

    while (1)
    {
        if (NULL != pstNodeWork)
        {
            while (pstChild = scew_element_next(pstNodeWork, pstChild),
                   NULL != pstChild)   /* 取子节点, NULL !=子节点 */
            {
                CFG_CHECK_AND_DEL_NODE(pstChild, break);

                /* 置为子节点 */
                pstNodeWork = pstChild;
                pstChild = NULL;

                /* 进栈 */
                apstStack[ucStackTop] = pstNodeWork;
                ucStackTop++;

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

        while (pstNodeNext = scew_element_next(pstParent, pstNodeWork),
               NULL != pstNodeNext)
        {
            CFG_CHECK_AND_DEL_NODE(pstNodeNext, (void)0);

            pstNodeWork = pstNodeNext;

            /* 进栈 */
            apstStack[ucStackTop] = pstNodeWork;
            ucStackTop++;

            break;
        }
    }

    return;
}



#define CFG_TREE_ATTR_GET_OPT(pstNode, pfnGetOpt, pcAbsPath, ucLen, pstNodeInfo, pvData) \
{ \
    ret = pfnGetOpt(pstNode, pcAbsPath, ucLen, pstNodeInfo, pvData); \
    if (CFG_OK != ret) \
    { \
        return ret; \
    } \
}

/*************************************************************************
Function:      long CFG_TreeAccess(const scew_element *pstNode)
Description:   普通的遍历子树的函数(所有取遍历取值均用此函数)
Calls:
Data Accessed: g_pfnCfgTreeOpt
Data Updated:  g_acCfgAbsPath
Input:         pstNode, 要遍历的子树
Output:        无
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
CFG_RET CFG_TreeAttrGetAccess(const scew_element *pstNode, FUNC_ATTR_GET_OPT pfnGetOpt,
                       char *pcAbsPath, const ST_CFG_NODE_INFO *pstNodeInfo, void *pvData)
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
    astStack[0].stNodeInfo = *pstNodeInfo;
    ucStackTop = 1;

    /* 检查该子树是否标准 */
    CFG_PASS_NOSTANDARD(pstNodeWork, g_pcCfgAccessor, ((void)0));
    if (NULL == pstNodeWork)
    {
        CFG_ERR(ERR_CFG_NOT_STANDARD, "%s", pcAbsPath);
        return ERR_CFG_NOT_STANDARD;
    }

    /* 操作 */
    CFG_TREE_ATTR_GET_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
            astStack[ucStackTop - 1].ucPathLen,
            &astStack[ucStackTop - 1].stNodeInfo, pvData);

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

                astStack[ucStackTop].stNodeInfo
                    = astStack[ucStackTop - 1].stNodeInfo;
                CFG_UpdateNodeInfo(pstNodeWork, &astStack[ucStackTop].stNodeInfo,
                     CFG_NODE_INFO_NOTI | CFG_NODE_INFO_ACCESSLIST);

                ucStackTop++;

                /* 在访问者是Tr069的情况下, 放过非标准节点 */
                CFG_PASS_NOSTANDARD(pstNodeWork, g_pcCfgAccessor, break);

                /* 操作 */
                CFG_TREE_ATTR_GET_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
                       astStack[ucStackTop - 1].ucPathLen,
                       &astStack[ucStackTop - 1].stNodeInfo, pvData);
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

            astStack[ucStackTop].stNodeInfo
                = astStack[ucStackTop - 1].stNodeInfo;
            CFG_UpdateNodeInfo(pstNodeWork, &astStack[ucStackTop].stNodeInfo,
                 CFG_NODE_INFO_NOTI | CFG_NODE_INFO_ACCESSLIST);

            ucStackTop++;

            /* 在访问者是Tr069的情况下, 放过非标准节点 */
            CFG_PASS_NOSTANDARD(pstNodeWork, g_pcCfgAccessor, continue);

            /* 操作 */
            CFG_TREE_ATTR_GET_OPT(pstNodeWork, pfnGetOpt, pcAbsPath,
                     astStack[ucStackTop - 1].ucPathLen,
                     &astStack[ucStackTop - 1].stNodeInfo, pvData);
        }
    }

    return CFG_OK;
}

#ifdef _CFG_DEBUG

/* 通过mid获取下标 */
#define MID2INDEX(mid) ((mid) & 0x00ff)

CFG_RET CFG_AddBranchToSepTree(const void *pstBranch, unsigned short usMID,
                               void **ppvTreeList)
{
    unsigned long i = 0;
    void *pvNode = NULL;
    CFG_RET ret = CFG_OK;
    void *pvTree = ppvTreeList[MID2INDEX(usMID)];
    unsigned long ulEnd = s_ucCfgTreeStackPos + 1;

    if (NULL == pvTree)
    {
        ret = CFG_DupSingleNode(s_astCfgTreeStack[1].pstPath,
                                (scew_element **)(void *)&pvNode);
        if (CFG_OK != ret)
        {
            CFG_ERR(ret);
            return ret;
        }
        ppvTreeList[MID2INDEX(usMID)] = pvNode;
        pvTree = pvNode;
    }

    s_astCfgTreeStack[s_ucCfgTreeStackPos].pstPath = pstBranch;
    for (i = 2; i < ulEnd; i++)
    {
        pvNode = scew_element_by_name(pvTree,
                scew_element_name(s_astCfgTreeStack[i].pstPath));
        if (NULL == pvNode) /* 不存在 */
        {
            /* 复制节点 */
            ret = CFG_DupSingleNode(s_astCfgTreeStack[i].pstPath,
                                    (scew_element **)(void *)&pvNode);
            if (CFG_OK != ret)
            {
                CFG_ERR(ret);
                return ret;
            }

            /* 添加到 */
            (void)scew_element_add_elem(pvTree, pvNode);
        }

        /* 更新节点 */
        pvTree = pvNode;
    }

    return CFG_OK;
}


CFG_RET CFG_TreeSepAccess(const char *pcPath, void **ppvTreeList)
{
    const scew_element *pstParent = NULL;
    const scew_element *pstChild = NULL;
    const scew_element *pstPathWork = NULL;
    unsigned short usMID = 0;
    unsigned char  ucPathLen = 0;
    CFG_RET ret = CFG_OK;

    ret = CFG_SetMIDAccessInfo(pcPath, (const void **)(void *)&pstPathWork, &usMID,
                               NULL, (char **)(void *)&pcPath);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    while (1)
    {
        /* 取兄弟 */
        if (0 == s_ucCfgCurIsRoot)
        {
            if (1 == s_ucCfgTreeStackPos)  /* 栈空 */
            {
                return CFG_OK;
            }
            pstParent = s_astCfgTreeStack[s_ucCfgTreeStackPos - 1].pstPath;
            pstPathWork = scew_element_next(pstParent, pstPathWork);
        }
        else  /* 当前这一步是寻找子树的第一个叶节点, 因此, 不进行取兄弟的操作 */
        {
            s_ucCfgCurIsRoot = 0;
        }
        if (NULL != pstPathWork)   /* NULL != 兄弟 */
        {
            while (pstChild = scew_element_next(pstPathWork, NULL),
                   NULL != pstChild)  /* 取子节点, NULL != 子节点 */
            {
                /* 取MID */
                CFG_GetPathLenAndMID(pstPathWork, &usMID, &ucPathLen);

                /* 进栈 */
                s_astCfgTreeStack[s_ucCfgTreeStackPos].pstPath = pstPathWork;
                s_astCfgTreeStack[s_ucCfgTreeStackPos].stNodeInfo.usMID = usMID;
                s_astCfgTreeStack[s_ucCfgTreeStackPos].ucPathLen = ucPathLen;
                s_ucCfgTreeStackPos++;

                pstPathWork = pstChild;
            }

            /* 取MID */
            CFG_GetPathLenAndMID(pstPathWork, &usMID, &ucPathLen);

            ret = CFG_AddBranchToSepTree(pstPathWork, usMID, ppvTreeList);
            if (CFG_OK != ret)
            {
                CFG_ERR(ret);
                return ret;
            }
        }
        else
        {
            /* 出栈 */
            --s_ucCfgTreeStackPos;
            pstPathWork = s_astCfgTreeStack[s_ucCfgTreeStackPos].pstPath;
        }
    }

    /* return ERR_CFG_FAIL; */  /* 无法运行到 */
}

typedef struct
{
    unsigned long usMID;
    char *pcName;

} ST_MOD_NAME;

void CFG_SepTree(void)
{
    CFG_RET ret = CFG_OK;
    void *apvTree[64] = {NULL};
    unsigned long i = 0;
    unsigned long j = 0;
    void *pvTmp = g_pstCfgTree;
    FILE *fp = NULL;
    char acFile[256];
    scew_tree stTree;

ST_MOD_NAME astModName[] =
{
    {0,                   "public"         },
    {MID_AUTH,            "auth"           },
    {MID_IGMP,            "igmp"           },
    {MID_CMM,             "cmm"            },
    {MID_LAN,             "lan"            },
    {MID_IPT,             "ipt"            },
    {MID_ETHLAN,          "ethlan"         },
    {MID_ETHWAN,          "ethwan"         },
    {MID_PPPOE,           "pppoe"          },
    {MID_WLAN,            "wlan"           },
    {MID_TR069BE,         "tr069be"        },
    {MID_DGN,             "ping"           },
    {MID_DHCPR,           "dhcpr"          },
    {MID_DHCPS,           "dhcps"          },
    {MID_TIMER,           "timer"          },
    {MID_IPCONN,          "ipconn"         },
    {MID_FIREWALL,        "firewall"       },
    {MID_SNMPC,           "snmpc"          },
    {MID_QOS,             "qos"            },
    {MID_STATIC_ROUTING,  "staticrouting"  },
    {MID_VDSL,            "vdsl"           },
    {MID_DNS,             "dns"            },
    {MID_ALG,             "alg"            },
    {MID_WAN,             "wan"            },
    {MID_DROUTE, "dynamicrouting" },
    {MID_SNTP,            "sntp"           },
    {MID_IPSEC,           "ipsec"          },
    {MID_VLAN,            "vlan"           },
    {MID_USB_MASS,        "usbmass"        },
    {MID_LOG,             "logger"         },
    {MID_FTPD,            "ftpd"           },
    {MID_NATPRIO,         "natprio"        },
    {MID_WPS,             "wps"            },
    {MID_ACL,             "acl"            },
    {MID_UPNP,            "upnp"           },
    {MID_LSVLAN,          "lanside_vlan"   },
    {MID_PORTOFF,         "portoff"        },
    {MID_ANTIATTACK,      "antiattack"     },
    {MID_SPT,      		  "shareprotection"},
    {MID_DEVINFO,         "devinfo"        },
    {MID_PORTMAPPING,     "portmapping"    },
    {MID_URLFILTER,       "urlfilter"      },
    {MID_ATM,             "atm"            },
    {MID_DSL,             "dsl"            },
    {MID_DDNS,            "ddns"           },
    {MID_PROUTE,          "proute"         },
    {MID_CFG,             "cfg"            },
    {MID_SUPP,            "supp"           },
    {MID_MACFILTER,       "macfilter"      },
    {MID_TRACERT,         "tracert"        },
    {MID_IPPD,            "ippd"           },
    {MID_WEBP,            "webp"           },
    {MID_BRIDGE_FILTER,   "brfilter"       },
    {MID_SCHED,           "sched"          },
    {MID_PORTTRIGGER,     "porttriger"     },
    {MID_IP_ACL,          "ipacl"          },
    {MID_DEFAULTGW,       "defaultgw"      },
    {MID_DIAG,            "diag"           },
    {MID_WANSELECT,       "wanselect"      },
    {MID_DEVCONFIG,       "devconfig"      },
    {MID_TRADITIONALNAT,  "traditionnat"   },
    {MID_FIREWALLLOG,     "firewalllog"    },
    {MID_IPMACFILTER,     "ipmacfilter"    },
    {MID_UDPECHO,         "udpecho"        },
    {MID_DOWNLOADDIAG,    "downloaddiag"   },
    {MID_UPLOADDIAG,      "uploaddiag"     },
    {MID_SAMBA,           "samba"          },
    {MID_USB3G,           "usb3g"          },
    {MID_TF_PORTMAPPING,  "tf_portmapping" },
    {MID_TF_FIREWALL,     "tf_firewall"    },
    {MID_OS_INFO,         "osinfo"         },
    {MID_WEB,             "web"            },
    {MID_TR069FE,         "tr069fe"        },
    {MID_TF_GUI,		  "tf_gui"		   },
    {MID_TR069DIAG,		  "dslloopdiag"	   }
        
};

    /* 交换两指针 */
    g_pstCfgTree = CFG_GetMIDTree();

    ret = CFG_TreeSepAccess(".", apvTree);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return;
    }

    /* 交换回来 */
    g_pstCfgTree = pvTmp;

    /* 打印树 */
    for (i = 0; i < sizeof(apvTree)/sizeof(apvTree[0]); i++)
    {
        if (NULL != apvTree[i])
        {
            for (j = 0; j < sizeof(astModName)/sizeof(astModName[0]); j++)
            {
                if (MID2INDEX(astModName[j].usMID) == (unsigned short)i)
                {
                    break;
                }
            }

            sprintf(acFile, "../../../../../src/apps/logic/%s/%s.xml",
                           astModName[j].pcName, astModName[j].pcName);

            stTree.root = apvTree[i];

            fp = fopen(acFile, "w");
            ret = CFG_PrintNode(&stTree, "", 0xffff * 2, fp);
            if (0 != ret)
            {
                printf("i = %lu\n", i);
            }
            if (NULL != fp)
            {
                fclose(fp);
            }
        }
    }

    /* 释放树 */
    for (i = 0; i < sizeof(apvTree)/sizeof(apvTree[0]); i++)
    {
        if (NULL != apvTree[i])
        {
            scew_element_del(apvTree[i]);
        }
    }
}

#endif



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif




