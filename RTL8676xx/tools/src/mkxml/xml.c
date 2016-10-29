

#include "cfg_prv.h"



#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif




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



CFG_RET CFG_DupSingleNode(const scew_element *pstNode, scew_element **ppstDupNode);
CFG_RET CFG_ReadFile(const char *pcFile, char **ppcBuf,
                     unsigned long ulPrevSpace, unsigned long ulPostSpace,
                     unsigned long *pulLen);

CFG_RET CFG_XmlInitPaser(scew_tree **ppstTree, const char *pszXMLContent);


CFG_RET CFG_TreeMerge(void *pvBase, void *pvAdd)
{
    scew_element *pstBase = pvBase;
    scew_element *pstAdd = pvAdd;
    scew_element *pstNext = NULL;
    scew_element *pstParent = NULL;
    scew_element *pstNew = NULL;
    CFG_RET ret = CFG_OK;
    const char *pcAttrVal = NULL;

    pstNext = scew_element_next(pstAdd, NULL);
    if (NULL == pstNext)
    {
        return CFG_OK;
    }

    while (1)
    {
        do
        {
            pstAdd = pstNext;
            pstNew = scew_element_by_name(pstBase, scew_element_name(pstAdd));
            if (NULL == pstNew)
            {
                ret = CFG_DupSingleNode(pstAdd, &pstNew);
                if (CFG_OK != ret)
                {
                    CFG_ERR(ret);
                    return ret;
                }


                CFG_NODE_ATTR(pstNew, ATTR_TYPE, pcAttrVal);
                if (0 == strcmp(pcAttrVal, TYPE_CLASS)
                      || 0 == strcmp(pcAttrVal, TYPE_OBJECT))
                {
                    scew_element_add_elem(pstBase, pstNew);
                }
                else
                {
                    scew_element_insert_elem(pstBase, NULL, pstNew);
                }
            }
            pstBase = pstNew;

        } while (pstNext = scew_element_next(pstAdd, NULL), NULL != pstNext);

        while (1)
        {
            pstParent = pstAdd->parent;
            if (NULL == pstParent)
            {
                return CFG_OK;
            }
            pstBase = pstBase->parent;
            pstNext = scew_element_next(pstParent, pstAdd);
            if (NULL != pstNext)
            {
                break;
            }
            pstAdd = pstParent;
        }
    }
}


int main(int argc, char **argv)
{
    int i = 0;
    char acFile[256];
    CFG_RET ret = CFG_OK;
    char *pcBuf = NULL;
    unsigned long ulLen = 0;
    scew_tree *pstTree = NULL;
    scew_tree *pstTotal = NULL;
    FILE *fp = NULL;

    if (1 == argc)
    {
        printf("Nothing to merge!\n");
        return 0;
    }

    pcBuf = malloc(0xffff);
    if (NULL == pcBuf)
    {
        return -1;
    }

    for (i = 1; i < argc; i++)
    {
        sprintf(acFile, "./%s", argv[i]);

        /* 打开文件 */
        ulLen = 0xffff;
        ret = CFG_ReadFile(acFile, &pcBuf, 0, 0, &ulLen);
        if (CFG_OK != ret)
        {
            CFG_ERR(ret);
            return ret;
        }
        pcBuf[ulLen] = '\0';

        CFG_FileTransMean(pcBuf, ulLen);

        /* 解析之 */
        ret = CFG_XmlInitPaser(&pstTree, pcBuf);
        if (CFG_OK != ret)
        {
            CFG_ERR(ret, "%s", acFile);
            return ret;
        }

        /* 将节点名称转义回去 */
        ret = CFG_TreeTransBack(pstTree);
        if (CFG_OK != ret)
        {
            CFG_ERR(ret);
            return ret;
        }

        if (NULL != pstTotal)
        {
            /* 合并到总树上面 */
            ret = CFG_TreeMerge(pstTotal->root, pstTree->root);
            if (CFG_OK != ret)
            {
                CFG_ERR(ret);
                return ret;
            }

            /* 释放 */
            scew_tree_free(pstTree);
            pstTree = NULL;
        }
        else
        {
            pstTotal = pstTree;
        }
    }

    /* 释放内存 */
    free(pcBuf);

    /* 将总树打印出来 */
    strcpy(acFile, "./config_full.xml");
    fp = fopen(acFile, "w");
    if (NULL == fp)
    {
        return -1;
    }
    (void)CFG_PrintNode(pstTotal, "", 0xffff *4, fp);

    /* 释放之 */
    scew_tree_free(pstTotal);
    fclose(fp);

    return 0;
}




#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


