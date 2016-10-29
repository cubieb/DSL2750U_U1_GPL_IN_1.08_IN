/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cfg_comm.c
 文件描述 : cfg模块中一些全局通用的子函数和初始化相关函数的实现
 函数列表 :

 修订记录 :
          1 创建 : 陈跃东
            日期 : 2007-10-15
            描述 :
          2 修改 : 陈跃东
            日期 : 2009-07-06
            描述 : 增加函数CFG_StrChrTok, 并用之替换 strtok

**********************************************************************/

#include <unistd.h>
#include <time.h>
#include "cfg_file.h"
#include "cfg_prv.h"
#include "autoconf.h"
#include "tbsutil.h"
#include "tbsmsg.h"
#include <led.h>
#include "flash_layout.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

//telefonica 需求，恢复出厂设置时灯，全部闪烁。
#define CONFIG_LED_FACTORT_FLASHING 


/* 配置树 */
scew_tree *g_pstCfgTree = NULL;

const char *g_pcCfgAccessor = NULL;

#ifndef CPE_PREFIX
#define CPE_PREFIX  "X_TWSZ-COM"
#define CPE_SECOND_PREFIX  "X_TELEFONICA-ES"
#endif

#ifdef CONFIG_APPS_CMM_NODE_FIX
//出厂配置树
scew_tree *g_pstDefaultTree = NULL;
#endif
#define MAX_PATH_LEN 257
/*************************************************************************
Function:      unsigned char CFG_IsAccessorIn(const char *pcAccessList,
                                       const char *pcAccessor)
Description:   查询访问者是否在访问者列表中
Calls:         无
Data Accessed:
Data Updated:
Input:         pcAccessList, 访问者列表
               pcAccessor, 访问者
Output:        无
Return:        1, 存在
               0, 不存在
Others:
*************************************************************************/
unsigned char CFG_IsAccessorIn(const char *pcAccessList, const char *pcAccessor)
{
    const char *pcBegin = pcAccessList;
    long ret = 0;
    unsigned char ucState = 0;

    if ('\0' == pcAccessor[0])  /* 为空字符串, 表示访问者为TR069 */
    {
        return 1;
    }

    while (1)
    {
        if ((' ' == *pcAccessList) || ('\0' == *pcAccessList))
        {
            if (1 == ucState)
            {
                ucState = 0;
                ret = strncmp(pcBegin, pcAccessor,
                               (unsigned long)(pcAccessList - pcBegin));
                if (0 == ret)
                {
                    return 1;
                }
            }

            if ('\0' == *pcAccessList)
            {
                break;
            }

            pcBegin = pcAccessList + 1;
        }
        else
        {
            if (0 == ucState)
            {
                ucState = 1;
                pcBegin = pcAccessList;
            }
        }

        pcAccessList++;
    }

    return 0;
}



/*************************************************************************
Function:      char *cfg_strdup(const char *pcPath)
Description:   CFG模块用来复制路径参数,
Calls:         无
Data Accessed:
Data Updated:
Input:         pcPath, 要复制的路径
Output:        无
Return:        复制后字符串的地址
Others:        表面上是重新分配了内存, 实际上用的是静态变量, 目的是减少malloc的次数
*************************************************************************/
char *cfg_strdup(const char *pcPath)
{
    static char acPath[CFG_MAX_PATH_LEN];

    if (strlen(pcPath) >= CFG_MAX_PATH_LEN)
    {
        CFG_ERR(ERR_CFG_BUF_NOT_ENOUGH);

        return NULL;
    }

    strcpy(acPath, pcPath);

    return acPath;
}




/*
取得节点, 子函数, pcPath 可写
*/


/*************************************************************************
Function:      void CFG_InitNodeInfo(ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
Description:  初始化节点信息
Calls:         无
Data Accessed:
Data Updated:
Input:         ulMask, 节点信息掩码标志
Output:        pstNodeInfo, 初始化后的节点信息结构
Return:        无
Others:
*************************************************************************/
void CFG_InitNodeInfo(ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
{
    if (ulMask & CFG_NODE_INFO_MID)
    {
        pstNodeInfo->usMID = 0;
    }
    if (ulMask & CFG_NODE_INFO_STANDARD)
    {
        pstNodeInfo->ucStandard = 1;
    }
    if (ulMask & CFG_NODE_INFO_NOTI)
    {
        pstNodeInfo->ucNoti = 0;
    }
    if (ulMask & CFG_NODE_INFO_ACCESSLIST)
    {
        pstNodeInfo->pcAccessList = "Subscriber";
    }
}

/*************************************************************************
Function:      void CFG_UpdateNodeInfo(const scew_element *pstNode,
                        ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
Description:  更新节点信息结构
Calls:         无
Data Accessed:
Data Updated:
Input:         pstNode, 节点地址
               ulMask, 节点信息掩码标志
Output:        pstNodeInfo, 更新后的节点信息结构
Return:        无
Others:
*************************************************************************/
void CFG_UpdateNodeInfo(const scew_element *pstNode,
       ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
{
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;

    if (ulMask & CFG_NODE_INFO_MID)
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_MID);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            pstNodeInfo->usMID = strtol(pcAttrVal, NULL, CFG_MID_BASE);
        }
    }
    if (ulMask & CFG_NODE_INFO_STANDARD)
    {
        if (0 != pstNodeInfo->ucStandard)
        {
            pstAttr = scew_attribute_by_name(pstNode, ATTR_STANDARD);
            if (NULL != pstAttr)
            {
                pcAttrVal = scew_attribute_value(pstAttr);
                pstNodeInfo->ucStandard = (unsigned char )(pcAttrVal[0] - '0');
            }
        }
    }
    if (ulMask & CFG_NODE_INFO_NOTI)
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_NOTI);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            pstNodeInfo->ucNoti = (unsigned char )(pcAttrVal[0] - '0');
        }
    }
    if (ulMask & CFG_NODE_INFO_ACCESSLIST)
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_ACCESS_LIST);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            pstNodeInfo->pcAccessList = pcAttrVal;
        }
    }
}


/*************************************************************************
Function:      long CFG_GetNode(const void *pstTree, char *pcPath, void **ppvNode)
Description:   查找节点
Calls:
Data Accessed:
Data Updated:
Input:         pstTree, 树根
               pcPath, 路径,
Output:        ppvNode, 节点指针
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
#ifdef CONFIG_APPS_CMM_NODE_FIX
char g_BootPeriod = 0;
#endif
CFG_RET CFG_GetNode(const void *pvTree, char *pcPath, void **ppvNode,
                    ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
{
    char *pszTmp = NULL;
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;

#ifdef CONFIG_APPS_CMM_NODE_FIX
    char pszPath[MAX_PATH_LEN];
    if (g_BootPeriod)
        strcpy(pszPath, pcPath);
#endif

    if (NULL == pvTree)
    {
        pvTree = g_pstCfgTree;
    }

    pszTmp = CFG_StrChrTok(pcPath, DELIMIT_C, &pcPath);

    pstNode = scew_tree_root(pvTree);
    if (NULL != pszTmp)
    {
        /* 如果有提供节点路径, 检查根节点是否匹配 */
        ret = strcmp(scew_element_name(pstNode), pszTmp);
        if (0 != ret)
        {
            /* 节点不存在 */
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszTmp);
            //return ERR_CFG_PATH_NOT_EXSITED;
            goto ERROR_CODE;
        }
        CFG_InitNodeInfo(pstNodeInfo, ulMask);
        CFG_UpdateNodeInfo(pstNode, pstNodeInfo, ulMask);

        while (pszTmp = CFG_StrChrTok(pcPath, DELIMIT_C, &pcPath),
               NULL != pszTmp)
        {
            /* 增加 对 pszTmp 作转义 */
            CFG_NUM_TO_NODE(pszTmp);

            pstNode = scew_element_by_name(pstNode, pszTmp);
            if (NULL == pstNode)
            {
                /* 节点不存在 */
                CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszTmp);
                //return ERR_CFG_PATH_NOT_EXSITED;
                ret = ERR_CFG_PATH_NOT_EXSITED;
                goto ERROR_CODE;
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

ERROR_CODE:
#ifdef CONFIG_APPS_CMM_NODE_FIX
    if (g_BootPeriod)
    {
        //启动过程中，如果读取指定节点失败，则先从
        //出厂配置中恢复，其次从TR069 Model上恢复
        const char *pValue = NULL;
        CFG_TRACE("###pszPath = %s###\n", pszPath);
        ret = CFG_RecoverNodeValueFromDefault(pszPath, &pValue, ppvNode);
        if ((CFG_OK != ret) || (NULL == pValue))
        {
            ret = CFG_RecoverNodeValueFromMid(pszPath, &pValue, ppvNode);
        }
        CFG_TRACE("###pValue = %s\n###", pValue);
    }
#endif
    if (CFG_OK != ret)
        return ERR_CFG_PATH_NOT_EXSITED;
    return CFG_OK;
}


CFG_RET CFG_InitMergeKeyTree(void);
CFG_RET CFG_GetUsbInitPath(char *pcPath);

/***********************************************************
接口:   采用指定接口读取配置数据然后解析成一个树
参数:   ppstTree, 输出解析成的树
        pcPath, 读取数据的路径
        pfnFunc, 读取函数
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_InitTreeByFunc(scew_tree **ppstTree, const char *pcPath,
                           FUNC_CFG_READ_CFG pfnFunc)
{
    char *pcCfg = NULL;
    unsigned long ulLen = 0;
    CFG_RET ret = CFG_OK;
    char *pcFileBufTmp = NULL;

    ret = pfnFunc(pcPath, &pcCfg, 0, CFG_NUM_TRANS_MEAN_LEN, &ulLen);
    if (CFG_OK != ret)   /* 失败 */
    {
        CFG_ERR(ret);
        return ret;
    }
    pcCfg[ulLen] = '\0';

    /*对比文件头部，如果没有指定encoding，则需要指定，如果没有指定，一些拉丁字符可能被认为非法*/
    if(strstr(pcCfg, XML_CTL_HEAD) == NULL)
    {
        pcFileBufTmp = malloc(ulLen +1+CFG_NUM_TRANS_MEAN_LEN+XML_CTL_HEAD_LEN);
        if(!pcFileBufTmp)
        {
            free(pcCfg);
             return TBS_OUT_OF_MEM;
        }
        memcpy(pcFileBufTmp, XML_CTL_HEAD, XML_CTL_HEAD_LEN);
        memcpy(pcFileBufTmp+XML_CTL_HEAD_LEN, pcCfg, ulLen+CFG_NUM_TRANS_MEAN_LEN+1);
        free(pcCfg);
        pcCfg = pcFileBufTmp;
        ulLen += XML_CTL_HEAD_LEN;
    }

    /* 增加对数字的转义 */
    CFG_FileTransMean(pcCfg, ulLen);

    /* 解析配置树 */
    ret = CFG_XmlInitPaser(ppstTree, (const char *)pcCfg);
    free(pcCfg);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* 将节点名称转义回去 */
    ret = CFG_TreeTransBack(*ppstTree);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    ret = CFG_FillDataTreeExAttrs();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    ret = CFG_FillDataTreeExNodes();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* 删除需要重启删除的节点 */
//    CFG_RmvDeletingNode(*ppstTree);

    return CFG_OK;
}

/***********************************************************
接口:   读取默认配置的函数
参数:   pcFile, 文件名
        ppcBuf, 输出缓冲区地址
        ulPrevSpace, 要求缓冲区之前有多少空间
        ulPostSpace, 要求缓冲区之后有多少空间
        pulLen, 输出读取到的文件长度
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_ReadDefaultCfgByPath(const char *pcFile, char **ppcBuf, unsigned long ulPrevSpace,
                     unsigned long ulPostSpace, unsigned long *pulLen)
{
    (void)pcFile;
    return CFG_ReadDefaultCfg(ppcBuf, ulPrevSpace, ulPostSpace, pulLen);
}


/* 基本接口 */
/***********************************************************
接口:   配置库初始化
参数:   无
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_LibInit(void)
{
    CFG_RET ret = CFG_OK;

    /* 初始化 mid树 */
    ret = CFG_MIDTreeInit();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* 从 保存的配置初始化树 */
    ret = CFG_InitTreeByFunc(&g_pstCfgTree, NULL, CFG_ReadCurCfg);
    if (CFG_OK != ret)   /* 失败 */
    {
        /* 从 缺省配置初始化树 */
        ret = CFG_InitTreeByFunc(&g_pstCfgTree, NULL, CFG_ReadDefaultCfgByPath);
#ifdef CONFIG_APPS_SSAP_PROTEST

        #define MAX_WPAPSK_LEN       	133
        #define AUTH_LOGIN_NAME_LEN     64

        char PassWd[MAX_WPAPSK_LEN] = {0};
        char szValue[ITEM_DATA_LEN] = {0};
        char szPath[MAX_PATH_LEN] = {0};
        const char *pszBeaconType = NULL;
        const char *pszPassword = NULL;
        const char *pszPSKExpression = NULL;
        int iRet = 0;
        unsigned short len;
#ifdef CONFIG_APPS_CLI_SEPARATE_AUTH
        len = ITEM_DATA_LEN;
        iRet = app_item_get(szValue, TELNET_PASSWORD, &len);
        szValue[len] = '\0';
        if ((ERROR_ITEM_OK == iRet) && (strlen(szValue) > 0))
        {
            app_item_save(szValue, CUR_TELNET_PASS, strlen(szValue));
        }
        else
        {
            strcpy(szValue, "etisalat");
            app_item_save(szValue, CUR_TELNET_PASS, strlen(szValue));
        }
#endif
        /* 从FLASH读取第一个VAP的加密模式和密钥 */
        len = MAX_WPAPSK_LEN;
        iRet = app_item_get(PassWd , WLAN_PASSWORD ,&len);
        if((ERROR_ITEM_OK == iRet) && (strlen(PassWd) > 0))
        {
            sprintf(szPath, "%s.%s", WLAN_BASIC_PATH, NODE_WLAN_BEACON_TYPE);
            iRet = CFG_GetNodeValPtr(szPath, &pszBeaconType, NULL);

            if (iRet != CFG_OK)
            {
                CFG_TRACE("Error: Get %s value failed.\n", NODE_WLAN_BEACON_TYPE);
                return CFG_FAIL;
            }

            if (strcmp(pszBeaconType, BEACON_TYPE_BASIC) == 0)
            {
                if(strlen(PassWd) == 5 || strlen(PassWd) == 13 || strlen(PassWd) == 10 || strlen(PassWd) == 26)
                {
                    sprintf(szPath, "%s.WEPKey.1.%s", WLAN_BASIC_PATH, NODE_WLAN_WEPKEY);
                    CFG_SetNodeVal(szPath, PassWd, NULL);
                }
                else
                {
                    CFG_TRACE("Error: Set WEP password failed.\n");
                    //return CFG_FAIL;
                }
            }
            else if ((strcmp(pszBeaconType, BEACON_TYPE_WPA) == 0) || (strcmp(pszBeaconType, BEACON_TYPE_11i) == 0) || (strcmp(pszBeaconType, BEACON_TYPE_WPA2_MIXED) == 0))
            {
                sprintf(szPath, "%s.%s", WLAN_BASIC_PATH, NODE_WLAN_PSK_EXPRESSION);
                iRet = CFG_GetNodeValPtr(szPath, &pszPSKExpression, NULL);
				
                if (iRet != CFG_OK)
                {
                    CFG_TRACE("Error: Get %s value failed.\n", NODE_WLAN_PSK_EXPRESSION);
                    return CFG_FAIL;
                }

                /* 若PSKExpression为KeyPassphrase方式*/
                if (strcmp(pszPSKExpression, NODE_WLAN_WPA_KEY_PASSPHRASE) == 0)
				{	
                    if(strlen(PassWd) <= 63 && strlen(PassWd) >= 8)
                    {
                        sprintf(szPath, "%s.PreSharedKey.1.%s", WLAN_BASIC_PATH, NODE_WLAN_WPA_KEY_PASSPHRASE);
                        CFG_SetNodeVal(szPath, PassWd, NULL);
                        /*2nd SSID*/
                        #ifdef CONFIG_APPS_LOGIC_WLAN_2NDSSID
                        CFG_SetNodeVal("InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.PreSharedKey.1.KeyPassphrase",
                                        PassWd, NULL);
                        #endif
                    }
                    else
                    {
                        CFG_TRACE("Error: Set WPA password failed.\n");
                        //return CFG_FAIL;
                    }
                }
                /* 若PSKExpression为PreSharedKey方式*/
                else if (strcmp(pszPSKExpression, NODE_WLAN_WPA_PSK) == 0)
                {	
                    if(strlen(PassWd) == 64)
                    {
                        sprintf(szPath, "%s.PreSharedKey.1.%s", WLAN_BASIC_PATH, NODE_WLAN_WPA_PSK);
                        CFG_SetNodeVal(szPath, PassWd, NULL);
                        /*2nd SSID*/
                        #ifdef CONFIG_APPS_LOGIC_WLAN_2NDSSID
                        CFG_SetNodeVal("InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.PreSharedKey.1.PreSharedKey",
                                        PassWd, NULL);
                        #endif
                    }
                    else
                    {
                        CFG_TRACE("Error: Set WPA password failed.\n");
                        //return CFG_FAIL;
                	}
                }
            }
	    }

        /* 从FLASH读取第一个SSID的名称并写入配置 */
        len = ITEM_DATA_LEN;
        iRet = app_item_get(szValue , WLAN_SSID ,&len);
        szValue[len] = '\0';
        if((ERROR_ITEM_OK == iRet) && (strlen(szValue) > 0))
        {
            sprintf(szPath, "%s.%s", WLAN_BASIC_PATH, NODE_WLAN_SSID);
            CFG_SetNodeVal(szPath, szValue, NULL);
        }

#endif
		
    }
    else
    {
#if 0 //#ifdef CONFIG_APPS_LOGIC_USB_MASS
        char acPath[CFG_MAX_FILE_LEN] = {0};

        /* 调用从usb初始化的函数 */
        ret = CFG_GetUsbInitPath(acPath);
        if (CFG_OK == ret && '\0' != acPath[0])
        {
            scew_tree *pstTreeSave = g_pstCfgTree;

            ret = CFG_InitTreeByFunc(&g_pstCfgTree, acPath, CFG_ReadCurCfg);
            if (CFG_OK == ret)
            {
                scew_tree_free(pstTreeSave);
                ret = CFG_SaveCfg();
            }
            ret = CFG_OK;
        }
#endif
    }
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

#ifdef CONFIG_APPS_CMM_NODE_FIX
    ret = CFG_InitTreeByFunc(&g_pstDefaultTree, NULL, CFG_ReadDefaultCfgByPath);
    if (ret != CFG_OK)
    {
        CFG_TRACE("######Failed to init Default Tree######\n");
        return ret;
    }
    ret = CFG_SearchAndRecoverNodeValueFromMid();
    if (CFG_OK != ret)
    {
        CFG_TRACE("######Failed to fix the current config######\n");
        return ret;
    }
#endif

#ifdef CONFIG_APPS_SSAP_HOLD_KEY_PARA
    ret = CFG_InitMergeKeyTree();  /* 调用 初始化合并关键树的操作 */
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }
#endif

#ifdef _CFG_DEBUG
    (void)CFG_ListNoTypeNode();
//    (void)CFG_PrintToExcel(); exit(0);
#endif

    return CFG_OK;
}


/* 配置库去初始化 */
/***********************************************************
接口:   配置库配置库去初始化
参数:   无
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_LibFinal(void)
{
    if (NULL != g_pstCfgTree)
    {
        scew_tree_free(g_pstCfgTree);
        g_pstCfgTree = NULL;
    }
    (void)CFG_MIDTreeFinal();
    return CFG_OK;
}




/******************************************************************************
  函数名称: XML_InitPaser
  功能说明: 使用指定XML内容初始化XML分析器
  输入参数: pszXMLContent  XML完整内容, 只读
  输出参数: 无
  返 回 值: CFG_NOERROR表示成功,
            CFG_ERR_PASER_ERROR：解析器解释出错, 一般是XML格式错误
  修订记录:
         1. 作者: zhaihaichen
            日期: 2007-9-4
            内容: 创建文件
 ******************************************************************************/
CFG_RET CFG_XmlInitPaser(scew_tree **ppstTree, const char *pszXMLContent)
{
    scew_parser* pParser = NULL;
    pParser = scew_parser_create();

    scew_parser_ignore_whitespaces(pParser, 1);

    /* Loads an XML file */
    if (!scew_parser_load_buffer(pParser, pszXMLContent, strlen(pszXMLContent)))
    {
        scew_error code = scew_error_code();
        CFG_ERR(ERR_CFG_INTERNAL_ERR, "Unable to load file (error #%d: %s)\n",
                 code, scew_error_string(code));
        if (code == scew_error_expat)
        {
            #ifdef _CFG_DEBUG
            enum XML_Error expat_code = scew_error_expat_code(pParser);
            #endif
            CFG_ERR(ERR_CFG_INTERNAL_ERR, "Expat error #%d (line %d, column %d): %s\n",
                     expat_code,
                     scew_error_expat_line(pParser),
                     scew_error_expat_column(pParser),
                     scew_error_expat_string(expat_code));
        }

        /* 说明, 经过反复验证, 这里的 scew_parser_free 并不能释放所有的内存.
           这些内存都是在scew_parser_load_buffer中被分配的.
           如果单元测试走到这个分支, 可不必检查内存泄露问题.
        */

        /* Frees the SCEW parser */
        scew_parser_free(pParser);

        return ERR_CFG_INTERNAL_ERR;
    }

    /* 解析XML树 */
    *ppstTree = scew_parser_tree(pParser);

    /* Frees the SCEW parser */
    scew_parser_free(pParser);

    return CFG_OK;
}


/*************************************************************************
Function:      void CFG_FileTransMean(char *pcBuf, unsigned long ulLen)
Description:   文件内容转义, 主要是把数字节点名转为非数字节点名
Calls:
Data Accessed:
Data Updated:
Input:         pcBuf, 文件内容
               ulLen, 内容长度
Output:        pcBuf, 解析后的内容
Return:        0,成功;
               其它, 失败
Others:
*************************************************************************/
void CFG_FileTransMean(char *pcBuf, unsigned long ulLen)
{
    char *pcBufWork = pcBuf;
    unsigned long ulLenWork = ulLen;
    unsigned char ucState = 0;

    while ('\0' != *pcBufWork && ulLenWork > 0)
    {
        switch (*pcBufWork)
        {
            case '<':
                ucState = 1;
                break;
            case ' ':
            case '\t':
            case '/':
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (1 == ucState)
                {
                    MEM_BACKCPY(pcBufWork, pcBufWork + 1, ulLenWork);
                    *pcBufWork = '_';
                    pcBufWork++;
                    pcBufWork[ulLenWork] = '\0';

                    ucState = 0;
                }
                break;
            default:
                ucState = 0;
                break;
        }

        pcBufWork++;
        ulLenWork--;
    }
}

/***********************************************************
接口:   备份配置文件
参数:   pcFileContent: 缓冲区
        pulLen: 输入缓冲区长度, 函数结束会输出实际是多大的长度
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_GetCfgFile(char *pcFile, unsigned long *pulLen,
                       unsigned long ulCompress, unsigned long ulOverWrite,
                       const char *pcAccessor)
{
    scew_element *pElement = NULL;
    ST_CFG_DUMP_BUF stDumpBuf = {NULL, 0, 0, 0};
    char *pcFileBuf = NULL;
    CFG_RET ret = CFG_OK;
#ifdef CONFIG_REPLACE_NODE_PREFIX
    char *pcRet = NULL;
#endif
#ifdef CONFIG_REPLACE_NODE_SECOND_PREFIX
    char *pcSecRet = NULL;
#endif

    if (NULL == pcFile || NULL == pulLen)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    //if (1 == ulCompress)  /* 压缩形式 */
    {
         /* 文件已经存在 */
        ret = access(pcFile, F_OK);
        if (CFG_OK == ret)
        {
            if (0 == ulOverWrite) /* 无覆盖标志 */
            {
                /* 返回 错误: 文件已经存在 */
                CFG_ERR(ERR_FILE_OPEN_EXSITED);
                return ERR_FILE_OPEN_EXSITED;
            }
            else
            {
                /* 删除文件 */
                ret = remove(pcFile);
                if (CFG_OK != ret)
                {
                    int err = errno;
                    if (EACCES == err)
                    {
                        ret = ERR_FILE_NOT_ALLOWED;
                    }
                    else
                    {
                        ret = ERR_FILE_RM_UNKOWN;
                    }
                    CFG_ERR(ret);
                    return ret;
                }
            }
        }
    }

    pcFileBuf = malloc(CFG_MAX_FILE_LEN);
    if (NULL == pcFileBuf)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }

    stDumpBuf.pcBuf = pcFileBuf;
    stDumpBuf.ulLen = CFG_MAX_FILE_LEN;
    pElement = scew_tree_root(g_pstCfgTree);

    /* dump 树 */
    g_pcCfgAccessor = pcAccessor;
    (void)CFG_TreeDumpAccess(pElement, &stDumpBuf, 0);
    if (stDumpBuf.ulInfactLen != stDumpBuf.ulPos)
    {
        free(stDumpBuf.pcBuf);
        CFG_REDUMP_FILE(stDumpBuf, pElement);
    }

#ifdef CONFIG_REPLACE_NODE_PREFIX
    /*为配置文件添加结束符*/
    stDumpBuf.pcBuf = realloc(stDumpBuf.pcBuf, stDumpBuf.ulInfactLen+1);
    if(!stDumpBuf.pcBuf)
    {
        return TBS_OUT_OF_MEM;
    }
    stDumpBuf.pcBuf[stDumpBuf.ulInfactLen] = '\0';
    pcRet = tbsStringReplaceWithMalloc(stDumpBuf.pcBuf, CPE_PREFIX, CONFIG_PRODUCT_PREFIX);
    free(stDumpBuf.pcBuf);
    stDumpBuf.pcBuf = pcRet;
    stDumpBuf.ulInfactLen = strlen(stDumpBuf.pcBuf);
#endif

#ifdef CONFIG_REPLACE_NODE_SECOND_PREFIX
    /*为配置文件添加结束符*/
    stDumpBuf.pcBuf = realloc(stDumpBuf.pcBuf, stDumpBuf.ulInfactLen+1);
    if(!stDumpBuf.pcBuf)
    {
        return TBS_OUT_OF_MEM;
    }
    stDumpBuf.pcBuf[stDumpBuf.ulInfactLen] = '\0';
    pcSecRet = tbsStringReplaceWithMalloc(stDumpBuf.pcBuf, CPE_SECOND_PREFIX, CONFIG_PRODUCT_SECOND_PREFIX);
    free(stDumpBuf.pcBuf);
    stDumpBuf.pcBuf = pcSecRet;
    stDumpBuf.ulInfactLen = strlen(stDumpBuf.pcBuf);
#endif

    if (1 == ulCompress)  /* 压缩形式 */
    {
        ret = CFG_SaveCurCfg(pcFile, stDumpBuf.pcBuf, stDumpBuf.ulInfactLen);
    }
    else
    {
        ret = CFG_WriteFile(pcFile, stDumpBuf.pcBuf, stDumpBuf.ulInfactLen);
    }
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    pcFile = pcFile;
    *pulLen = *pulLen;

    free(stDumpBuf.pcBuf);
    return ret;
}


/***********************************************************
接口:   更新配置文件
参数:   pcFileContent: 配置文件内容
        ulLen: 内容长度
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_SetCfgFile(const char *pcFile, unsigned long ulLen)
{
    unsigned long ulNewLen = 0;
    scew_tree *pstNewTree = NULL;
    long ret = CFG_OK;
    char *pcFileBuf = NULL;
    char *pcFileBufTmp = NULL;
    
#ifdef CONFIG_REPLACE_NODE_PREFIX
    char *pcRet = NULL;
#endif
#ifdef CONFIG_REPLACE_NODE_SECOND_PREFIX
    char *pcSecRet = NULL;
#endif
    (void)ulLen;
    if (NULL == pcFile)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }
#ifdef CONFIG_TBS_BACKUP_ENCRYPTION
	ret =  CFG_ReadCfgfile_EX(pcFile, &pcFileBuf, 0,CFG_NUM_TRANS_MEAN_LEN,&ulNewLen);
#else
    ret = CFG_ReadFile(pcFile, &pcFileBuf, 0, CFG_NUM_TRANS_MEAN_LEN, &ulNewLen);
#endif
	if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

#ifdef CONFIG_REPLACE_NODE_PREFIX
    pcFileBuf = realloc(pcFileBuf, ulNewLen+1);
    if(!pcFileBuf)
    {
        return TBS_OUT_OF_MEM;
    }
    pcFileBuf[ulNewLen] = '\0';
    pcRet = tbsStringReplaceWithMalloc(pcFileBuf, CONFIG_PRODUCT_PREFIX, CPE_PREFIX);
    free(pcFileBuf);
    pcFileBuf = pcRet;
    ulNewLen = strlen(pcFileBuf);
    /*重新分配内存时一定要多2K用来转义*/
    pcFileBuf = realloc(pcFileBuf, ulNewLen+1+CFG_NUM_TRANS_MEAN_LEN);
#endif

#ifdef CONFIG_REPLACE_NODE_SECOND_PREFIX
    pcFileBuf = realloc(pcFileBuf, ulNewLen+1);
    if(!pcFileBuf)
    {
        return TBS_OUT_OF_MEM;
    }
    pcFileBuf[ulNewLen] = '\0';
    pcSecRet = tbsStringReplaceWithMalloc(pcFileBuf, CONFIG_PRODUCT_SECOND_PREFIX, CPE_SECOND_PREFIX);
    free(pcFileBuf);
    pcFileBuf = pcSecRet;
    ulNewLen = strlen(pcFileBuf);
    /*重新分配内存时一定要多2K用来转义*/
    pcFileBuf = realloc(pcFileBuf, ulNewLen+1+CFG_NUM_TRANS_MEAN_LEN);
#endif

    /*对比文件头部，如果没有指定encoding，则需要指定，如果没有指定，一些拉丁字符可能被认为非法*/
    if(strstr(pcFileBuf, XML_CTL_HEAD) == NULL)
    {
        pcFileBufTmp = malloc(ulNewLen +1+CFG_NUM_TRANS_MEAN_LEN+XML_CTL_HEAD_LEN);
        if(!pcFileBufTmp)
        {
            free(pcFileBuf);
             return TBS_OUT_OF_MEM;
        }
        memcpy(pcFileBufTmp, XML_CTL_HEAD, XML_CTL_HEAD_LEN);
        memcpy(pcFileBufTmp+XML_CTL_HEAD_LEN, pcFileBuf, ulNewLen+CFG_NUM_TRANS_MEAN_LEN+1);
        free(pcFileBuf);
        pcFileBuf = pcFileBufTmp;
        ulNewLen += XML_CTL_HEAD_LEN;
     }
    
    /* 增加对数字的转义 */
    CFG_FileTransMean(pcFileBuf, ulNewLen);

    /* 解析新树 */
    ret = CFG_XmlInitPaser(&pstNewTree, (const char *)pcFileBuf);
    if (CFG_OK != ret)
    {
        free(pcFileBuf);

        CFG_ERR(ret);
        return ret;
    }

    free(pcFileBuf);

    /* 释放旧树 */
    scew_tree_free(g_pstCfgTree);

    /* 将全局树指针指向新树 */
    g_pstCfgTree = pstNewTree;

    ret = CFG_TreeTransBack(g_pstCfgTree);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }
    /*写文件 */
    ret = CFG_SaveCfg();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    return ret;
}




/***********************************************************
接口:   写配置
参数:   无
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_SaveCfg(void)
{
    unsigned long ulLen = CFG_MAX_FILE_LEN;
    ST_CFG_DUMP_BUF stDumpBuf = {NULL, 0, 0, 0};
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    
#ifdef CONFIG_APPS_LOGIC_WANSELECT
    //LAN上行切换后，不要保存配置，否则会冲掉LAN上行配置
    if(access("/var/wanModeChangedFlag", F_OK) == 0)
    {
        return CFG_OK;
    }
#endif    

    g_pcCfgAccessor = NULL;
    pstNode = scew_tree_root(g_pstCfgTree);

    stDumpBuf.pcBuf = malloc(ulLen);
    if (NULL == stDumpBuf.pcBuf)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }
    stDumpBuf.ulLen = ulLen;

    /* 将节点树dump 出来, */
    (void)CFG_TreeDumpAccess(pstNode, &stDumpBuf, 0);
    if (stDumpBuf.ulInfactLen != stDumpBuf.ulPos)
    {
        free(stDumpBuf.pcBuf);
        CFG_REDUMP_FILE(stDumpBuf, pstNode);
    }

    /* 调用file接口保存 */
    ret = CFG_SaveCurCfg(NULL, stDumpBuf.pcBuf, stDumpBuf.ulInfactLen);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    free(stDumpBuf.pcBuf);

    /* Added by yjs 20100115 config change so notify snmpd */
    FILE *fp;
    fp = fopen( "/var/tmp/cfgchg.smp", "w" );
	if( fp != NULL )
	{
		fprintf( fp, "%d", 1 );
		fclose( fp );
	}
    
    if( ( fp = fopen( "/var/tmp/tr064_change", "w" ) ) != NULL )
    {
        if( fp != NULL )
		{
			fprintf( fp, "%d", 1 );
			fclose( fp );
		}
    }
    
    return ret;
}
#ifdef CONFIG_LED_FACTORT_FLASHING 
/*************************************************************************
功能: Telefonica需求，在重启的时候，所有的led都按2HZ闪烁。
参数: 无
返回: 无
备注:
*************************************************************************/
void Restore_Factory_Led_Flashing()
{
	char cmd[128] = {0};
	char temp[64] = {0};
	int i =0;

	sprintf(cmd, "for i in ");
	for(i=0;i<led_end;i++)
		{
		sprintf(temp, "%d ", i);
		strcat(cmd, temp);
		}
	sprintf(temp, "; do echo $i %d > /proc/led; done", led_flash_2hz_trig);
	/* 通过写proc文件系统来操作LED */
	strcat(cmd, temp);
	tbsSystem(cmd, 0);
}
#endif
/* 恢复出厂设置 */
/***********************************************************
接口:   恢复出厂设置
参数:   无
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_RecoverFactSet(const char *pcAccessor)
{
    long ret = CFG_OK;

#ifdef CONFIG_LED_FACTORT_FLASHING      
    Restore_Factory_Led_Flashing();
#endif

    /* 将出厂配置设置为当前配置 */
    ret = CFG_RecoverFactCfg(pcAccessor);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    return ret;
}


/* 清除当前配置 */
/***********************************************************
接口:   清除当前配置
参数:   无
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_ClearCfg(void)
{
    CFG_RET ret = CFG_OK;

    ret = CFG_ClearCurCfg();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }
    
    /*
     设置Reset flag,不允许再写配置
     只能重启后再配置 add by pengyao 20121015
    */
    g_bResetedFlag = 1;

    return ret;
}

#ifdef CONFIG_APPS_LOGIC_WANSELECT

/* 清除所有配置 */
/***********************************************************
接口:   清除所有配置
参数:   无
返回值: 0:成功
        其它:失败
***********************************************************/
CFG_RET CFG_ClearAllCfg(void)
{
    CFG_RET ret = CFG_OK;

    ret = CFG_ClearAllTheCfg();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    return ret;
}
#endif

/***********************************************************
接口:   判断节点是否为数字节点
参数:   pcNode, 节点名
返回值: 0:不是
        其它:是
***********************************************************/
unsigned char CFG_IsNodeNum(const char *pcNode)
{
    while ('\0' != *pcNode)
    {
        if (*pcNode > '9' || *pcNode < '0')
        {
            return 0;
        }
        pcNode++;
    }

    return 1;
}

/***********************************************************
接口:   类似于strtok的功能, 专用于cfg模块解析以'.'分隔的路径
参数:   pcStr, 路径
        cKey, 分隔符
        ppcNext, 下一段路径
返回值: 解析到的节点名
***********************************************************/
char *CFG_StrChrTok(char *pcStr, char cKey, char **ppcNext)
{
    char *pcPos = NULL;

    if (NULL == pcStr || '\0' == pcStr[0] || cKey == pcStr[0])
    {
        return NULL;
    }

    pcPos = strchr(pcStr, cKey);
    if (NULL != pcPos)
    {
        *pcPos = '\0';
        *ppcNext = pcPos + 1;
    }
    else
    {
        *ppcNext = NULL;
    }

    return pcStr;
}

/*************************************************************************
Function:      scew_element *scew_element_insert_elem(scew_element* element,
                                 scew_element *left, scew_element* new_elem)
Description:   将节点到另一节点指定子节点的后面
Calls:         无
Data Accessed:
Data Updated:
Input:         element, 宿主节点地址
               left, 插入的位置
               new_elem, 新插入的节点地址
Output:        无
Return:        new_elem
Others:
*************************************************************************/
scew_element *scew_element_insert_elem(scew_element* element,
                           scew_element *left, scew_element* new_elem)
{
    scew_element* current = NULL;

    element->n_children++;

    new_elem->parent = element;
    if (element->last_child == NULL)
    {
        element->last_child = new_elem;
    }
    else
    {
        if (NULL != left)
        {
            if (NULL != left->right)
            {
                left->right->left = new_elem;
                new_elem->right = left->right;
            }
            else
            {
                element->last_child = new_elem;
            }
            new_elem->left = left;
            left->right = new_elem;
        }
        else
        {
            current = element->child;
            current->left = new_elem;
            new_elem->right = current;
        }
    }
    if (NULL == left || NULL == element->child)
    {
        element->child = new_elem;
    }

    return new_elem;
}

#ifdef CONFIG_APPS_CMM_NODE_FIX
/*************************************************************************
Function: CFG_RET CFG_RecoverNodeValueFromDefault(const char *pszPath, const char **pValue, void **ppvNode)
Description: 软件升级前的配置缺少新增功能的配置节点，这里从出厂配置树上恢复
Calls:
Data Accessed:
Data Updated:
Input: pszPath 当前配置上访问不到的节点路径
Output: pValue 从出厂配置上恢复的节点的值（作用不大）
        ppvNode 从出厂配置上恢复的节点的地址（这个没用，可以考虑删除）
Return: 成功为CFG_OK
Others: 这个函数主要用于恢复非实例节点
*************************************************************************/
CFG_RET CFG_RecoverNodeValueFromDefault(const char *pszPath, const char **pValue, void **ppvNode)
{
    char szPathTmp[MAX_PATH_LEN] = {0};
    char *pszTmp = NULL;
    char *pcPathTmp = szPathTmp;
    scew_element *pstBig = NULL;
    scew_element *pstDef = NULL;
    scew_element *pstChild = NULL;
    long ret = CFG_OK;

    CFG_TRACE("%d %s Entry\n", __LINE__, __FUNCTION__);
    pstBig = scew_tree_root(g_pstCfgTree);
    pstDef = scew_tree_root(g_pstDefaultTree);

    strcpy(pcPathTmp, pszPath);
    pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp);
    if (NULL != pszTmp)
    {
        ret = strcmp(scew_element_name(pstBig), pszTmp);
        if (0 != ret)
        {
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszPath);
            return ERR_CFG_PATH_NOT_EXSITED;
        }
        
        //通过拆分节点路径，逐层查找可以从出厂配置中恢复的节点
        while (pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp),
                   NULL != pszTmp)
        {
            //检查出厂配置树中是否有这个节点
            pstDef = scew_element_by_name(pstDef, pszTmp);
            if (NULL != pstDef)
            {
                //检查当前配置树中是否有这个节点
                pstChild = scew_element_by_name(pstBig, pszTmp);
                if (NULL == pstChild)
                {
                    //开始从出厂配置树中恢复节点
                    ret = CFG_DupAccess(pstDef, &pstChild);
                    if (CFG_OK != ret)
                    {
                        CFG_ERR(ret);
                        return ret;
                    }
                    (void)scew_element_add_elem(pstBig, pstChild);
                    if (CFG_IS_NODE_NUM(pszTmp))
                    {
                        CFG_UpdateMaxIdx(pstBig);
                        (void)CFG_UpdateObjCurInstCount(pstBig);
                    }
                    //这里可以直接break，之后的子节点已经同时复制了。
                }
                pstBig = pstChild;
            }
            else
            {
                return CFG_FAIL;
            }
        }
    }

    *pValue = scew_element_contents(pstChild);
    if (NULL != ppvNode)  //没用可以删除
        *ppvNode = pstChild;
    CFG_TRACE("%d %s Exit\n", __LINE__, __FUNCTION__);
    return CFG_OK;
}

/*************************************************************************
Function: CFG_RET CFG_RecoverNodeValueFromMid(const char *pszPath, const char **pValue, void **ppvNode)
Description: 软件升级前的配置缺少新增功能的配置节点，这里从TR069 Model上恢复
Calls:
Data Accessed:
Data Updated:
Input: pszPath 当前配置上访问不到的节点路径
Output: pValue 从TR069 Model上恢复的节点的值（作用不大）
        ppvNode 从TR069 Model上恢复的节点的地址（这个没用，可以考虑删除）
Return: 成功为CFG_OK
Others: 这个函数主要用于恢复实例节点
*************************************************************************/
CFG_RET CFG_RecoverNodeValueFromMid(const char *pszPath, const char **pValue, void **ppvNode)
{
    char szPathTmp[MAX_PATH_LEN] = {0};
    char *pszTmp = NULL;
    char *pcPathTmp = szPathTmp;
    scew_element *pstBig = NULL;
    scew_element *pstMid = NULL;
    scew_element *pstChild = NULL;
    long ret = CFG_OK;

    CFG_TRACE("%d %s Entry\n", __LINE__, __FUNCTION__);
    pstBig = scew_tree_root(g_pstCfgTree);
    pstMid = scew_tree_root(CFG_GetMIDTree());

    strcpy(pcPathTmp, pszPath);
    pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp);
    if (NULL != pszTmp)
    {
        ret = strcmp(scew_element_name(pstBig), pszTmp);
        if (0 != ret)
        {
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszPath);
            return ERR_CFG_PATH_NOT_EXSITED;
        }

        while (pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp),
                   NULL != pszTmp)
        {
            if (CFG_IS_NODE_NUM(pszTmp)) /* 当前节点是数字 */
            {
                pstMid = scew_element_by_name(pstMid, NODE_WILDCARD);
            }
            else
            {
                pstMid = scew_element_by_name(pstMid, pszTmp);
            }
            if (NULL != pstMid)
            {
                pstChild = scew_element_by_name(pstBig, pszTmp);
                if (NULL == pstChild)
                {
                    if (CFG_IS_NODE_NUM(pszTmp))
                    {
                        /* 添加一个实例对象 */
                        ret = CFG_DupAccess(pstMid, &pstChild);
                        if (CFG_OK != ret)
                        {
                            CFG_ERR(ret);
                            return ret;
                        }
                        /* 修改节点名 */
                        (void)scew_element_set_name(pstChild, pszTmp);
                        (void)scew_element_add_elem(pstBig, pstChild);
                        CFG_UpdateMaxIdx(pstBig);
                        (void)CFG_UpdateObjCurInstCount(pstBig);
                        continue; /* 可能会有2层以上的节点 */
                    }
                    else
                    {
                        /* 添加一个属性 */
                        ret = CFG_DupAccess(pstMid, &pstChild);
                        if (CFG_OK != ret)
                        {
                            CFG_ERR(ret);
                            return ret;
                        }
                        (void)scew_element_add_elem(pstBig, pstChild);
                        break;
                    }
                }
                pstBig = pstChild;
            }
            else
            {
                return CFG_FAIL;
            }
        }
    }

    *pValue = scew_element_contents(pstChild);
    if (NULL != ppvNode)  //没用可以删除
        *ppvNode = pstChild;
    CFG_TRACE("%d %s Exit\n", __LINE__, __FUNCTION__);
    return CFG_OK;
}

/*************************************************************************
Function: CFG_RET CFG_CheckAndFixNode(scew_element *pstRoot, scew_element *pstMid, scew_element *pstCurrent)
Description: 根据TR069 Model，即Mid Tree补充当前实例下缺少的节点
Calls: CFG_SearchAndRecoverNodeValueFromMid
Data Accessed:
Data Updated:
Input: pstRoot 当前配置树的根节点指针
       pstMid TR069 Model的根节点指针
       pstCurrent 当前配置树上需要从TR069 Model上恢复节点的实例指针
Output:
Return: 成功为CFG_OK
Others: 这个函数主要用于恢复实例节点
*************************************************************************/
CFG_RET CFG_CheckAndFixNode(scew_element *pstRoot, scew_element *pstMid, scew_element *pstCurrent)
{
    long ret = CFG_OK;
    char szPathData[16][256]; //用二维数组保存路径
    int iDepth = 0;
    scew_element *pstNode = NULL;
    scew_element *pstChild = NULL;
    scew_element *pstNew = NULL;

    memset(szPathData, 0, 16*256);
    CFG_TRACE("%d %s Entry\n", __LINE__, __FUNCTION__);

    //从pstCurrent一级级向上获取出路径
    pstNode = pstCurrent;
    while(pstNode != pstRoot)
    {
        strcpy(szPathData[iDepth], pstNode->name);
        iDepth++;
        pstNode = pstNode->parent;
    }
    //strcpy(szPathData[iDepth], "InternetGatewayDevice"); //复制头结点(头结点不需要复制)
    //iDepth++;
    pstNode = pstMid;
    while(iDepth > 0)
    {
        //根据szPath，在pstMid中取到指定的节点
        if (strlen(szPathData[iDepth - 1]) > 0)
        {
            if (CFG_IS_NODE_NUM(szPathData[iDepth - 1])) /* 当前节点是数字 */
            {
                pstNode = scew_element_by_name(pstNode, NODE_WILDCARD);
            }
            else
            {
                pstNode = scew_element_by_name(pstNode, szPathData[iDepth - 1]);
            }
            if (NULL == pstNode)
            {
                 //如果Mid tree没有这个节点，则不作修复
                 CFG_TRACE("CFG : Mid tree do not have node : %s\n", szPathData[iDepth - 1]);
                 return CFG_OK;
            }
        }
        else
        {
            CFG_TRACE("CFG : The path data has empty string.\n");
            ret = CFG_FAIL;
            break;
        }
        CFG_TRACE("%s.", szPathData[iDepth - 1]);
        iDepth--;
    }

    CFG_TRACE("\nCFG : Begin search and fix the node\n");
    //从pstMid上指定节点的子节点上遍历查找pstCurrent的完整性
    while(pstChild = scew_element_next(pstNode, pstChild),
              NULL != pstChild)
    {
        //如果含有子节点，则不补充节点，否则复制节点会很难控制
        if (pstChild->child == NULL)
        {
            if (NULL == scew_element_by_name(pstCurrent, pstChild->name))
            {
                //当前配置树上没有这个节点，需要从pstMid上补充
                ret = CFG_DupAccess(pstChild, &pstNew);
                if (CFG_OK != ret)
                {
                    CFG_ERR(ret);
                    return ret;
                }
                scew_element_add_elem(pstCurrent, pstNew);
            }
        }
    }

    CFG_TRACE("%d %s Exit\n", __LINE__, __FUNCTION__);
    return ret;
}

/*************************************************************************
Function: CFG_RET CFG_SearchAndRecoverNodeValueFromMid()
Description: 遍历当前配置树实例节点，检查并补充缺少的节点
Calls:
Data Accessed:
Data Updated:
Input:
Output:
Return: 成功为CFG_OK
Others:
    1)考虑补充实例中的属性，不考虑非实例节点。
    2)这个函数不能完全取代CFG_RecoverNodeValueFromMid()函数，
      虽然函数遍历了所有实例节点，但对于ACL中添加
      新的访问控制实例时，这个函数就覆盖不到了。
    3)另外对于非实例节点，要靠CFG_RecoverNodeValueFromDefault()函数。
    4)此节点恢复功能，可以在软件升级后执行一次即可，
      在硬配置区添加一个标记来实现。
*************************************************************************/
CFG_RET CFG_SearchAndRecoverNodeValueFromMid()
{
    scew_element *pstRoot = NULL;
    scew_element *pstParent = NULL;
    scew_element *pstMid = NULL;
    scew_element *pstChild = NULL;
    long ret = CFG_OK;

    CFG_TRACE("%d %s Entry\n", __LINE__, __FUNCTION__);
    pstRoot = scew_tree_root(g_pstCfgTree);
    pstMid = scew_tree_root(CFG_GetMIDTree());
    if ((NULL == pstRoot) || (NULL == pstMid))
    {
        CFG_TRACE("%s is NULL\n", (NULL == pstRoot)?"g_pstCfgTree":"s_pstCfgMIDTree");
        return CFG_FAIL;
    }
    pstParent = pstRoot;

    do
    {
        pstChild = scew_element_next(pstParent, pstChild);
        if (NULL != pstChild)
        {
            if (NULL != pstChild->child)
            {
                //子节点不为空
                pstParent = pstChild;
                pstChild = NULL;
                continue;
            }
        }
        else
        {
            //子节点访问完毕，返回上一级
            pstChild = pstParent;
            pstParent = pstChild->parent;
            //如果父节点已经是最后一个节点，则进入实例节点的判断
            if (NULL != pstChild->right)
                continue;
        }

        if (NULL == pstChild->right)
        {
            //兄弟节点为空，已经到同级队列尾部
            //判断是否是实例
            if (NULL == pstParent)  //父节点为空，子节点就是pstRoot
                continue;
            if (CFG_IS_NODE_NUM(scew_element_name(pstParent)))
            {
                ret = CFG_CheckAndFixNode(pstRoot, pstMid, pstParent);
                if (CFG_OK != ret)
                {
                     CFG_TRACE("%s : Fix node failed.\n", __FUNCTION__);
                     break;
                }
            }

            //取父节点的右节点，即兄弟节点
            //pstChild = pstParent;
            //pstParent = pstChild->parent;
        }
    }while(pstChild != pstRoot);

    CFG_TRACE("%d %s Exit\n", __LINE__, __FUNCTION__);
    return ret;
}
#endif

#ifdef _CFG_DEBUG

CFG_RET CFG_PrintNode(const void *pvTree, const char *pcPath,
                      unsigned long ulLen, void *pvFile)
{
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;
    ST_CFG_DUMP_BUF stDumpBuf = {NULL, 0, 0, 0};
    char *pcPathTmp = NULL;

    if (NULL == pvFile)
    {
        pvFile = stdout;
    }

    CFG_DUP_PATH(pcPathTmp, pcPath);

    /* 查找节点 */
    ret = CFG_GetNode(pvTree, pcPathTmp, (void **)(void *)&pstNode, NULL, 0);
    CFG_FREE_PATH(pcPathTmp);
    if (CFG_OK != ret || NULL == pstNode)  /* 没有找到 */
    {
        printf("Node not failed\n");
        return  ret;
    }

    stDumpBuf.pcBuf = malloc(ulLen);
    if (NULL == stDumpBuf.pcBuf)
    {
        printf("Malloc buffer failed\n");
        return ERR_CFG_MALLOC_FAIL;
    }
    stDumpBuf.ulLen = ulLen;

    g_pcCfgAccessor = NULL;
    (void)CFG_TreePrintAccess(pstNode, &stDumpBuf, 0);
    if (stDumpBuf.ulPos == stDumpBuf.ulLen)
    {
        stDumpBuf.pcBuf[stDumpBuf.ulPos - 1] = '\0';
    }
    else
    {
        stDumpBuf.pcBuf[stDumpBuf.ulPos] = '\0';
    }
    fprintf(pvFile, "%s", stDumpBuf.pcBuf);
    fprintf(pvFile, "\n");

    free(stDumpBuf.pcBuf);
    return CFG_OK;
}

#endif




#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


