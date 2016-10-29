#include "common.h"

#include <stdarg.h>
#include <sys/vfs.h>	/* statfs */

#include <assert.h>
//#include "Console.h"
//#include "Basic.h"
#include <CUnit/Basic.h>
#include "Automated.h"

#define CFG_INTERNAL_ERR    1
#define CFG_OK              0

#define MAX_MSG_LEN 1024

#if 0
#define TEST_TRACE(param...)        printf("\n<UT, %s, %d> ", __FUNCTION__, __LINE__); printf(param);
#else
#define TEST_TRACE(param...)
#endif


/* 桩函数的通用实现, 如果变量fail_##FUNC_NAME 为正数，
  * 表示失败几次后成功，如果为负数，表示成功几次后失败*/
#define STUB_COMM_CODE(FUNC_NAME, ERROR_RETURN)     \
    if ( fail_##FUNC_NAME > 0 )   \
    {   \
        fail_##FUNC_NAME--;   \
        return ERROR_RETURN;   \
    }   \
   \
    if ( fail_##FUNC_NAME < 0 )   \
    {   \
        fail_##FUNC_NAME++;   \
        if ( fail_##FUNC_NAME == 0 )   \
        {   \
            fail_##FUNC_NAME = 1;   \
        }   \
    }



/* 调用消息处理函数时使用的消息*/
ST_MSG *s_pstTestMsg = NULL;

/* 调用MSG_CreateMessage 返回的消息*/
ST_MSG *s_pstCreateMsg = NULL;



/*---------------------------------- 桩函数-----------------------------*/

int fail_MSG_RegModule = 0;
MSG_RET MSG_RegModule(unsigned short usMID, FUN_RECEIVE funReceive)
{
    STUB_COMM_CODE(MSG_RegModule, TBS_FAILED);
    return TBS_SUCCESS;
}


int fail_MSG_UnregModule = 0;
MSG_RET MSG_UnregModule(unsigned short usMID, FUN_RECEIVE funReceive)
{
    STUB_COMM_CODE(MSG_UnregModule, TBS_FAILED);
    return TBS_SUCCESS;
}


int fail_CFG_MemFree = 0;
long CFG_MemFree(void *pvMem)
{
    STUB_COMM_CODE(CFG_MemFree, CFG_INTERNAL_ERR);
    return CFG_OK;
}


int fail_CFG_GetNodeMaxIdx = 0;
static unsigned long s_ulMaxNodeID = 0;
long CFG_GetNodeMaxIdx(const char *pcPath, unsigned long *pulMaxIdx)
{
    STUB_COMM_CODE(CFG_GetNodeMaxIdx, CFG_INTERNAL_ERR);

    if ( pulMaxIdx == NULL )
    {
        TEST_TRACE("pulMaxIdx == NULL.\n");
        return CFG_INTERNAL_ERR;
    }
    else
    {
        *pulMaxIdx = s_ulMaxNodeID++;
        return CFG_OK;
    }
}


int fail_CFG_SetNodeMaxIdx = 0;
CFG_RET CFG_SetNodeMaxIdx(const char *pcPath, unsigned long ulMaxIdx)
{
    STUB_COMM_CODE(CFG_SetNodeMaxIdx, CFG_INTERNAL_ERR);
    return CFG_OK;
}


int fail_CFG_RegisterMID = 0;
long CFG_RegisterMID(const char *pszPath, unsigned short usMID)
{
    STUB_COMM_CODE(CFG_RegisterMID, CFG_INTERNAL_ERR);
    return CFG_OK;
}


int fail_CFG_RegisterCareMID = 0;
CFG_RET CFG_RegisterCareMID(const char *pcPath, unsigned short usMID)
{
    STUB_COMM_CODE(CFG_RegisterCareMID, CFG_INTERNAL_ERR);
    return CFG_OK;
}


int fail_CFG_AddNode = 0;
CFG_RET CFG_AddNode(const char *pcPath, const char *pcType, const char *pcVal,
                 unsigned char ucIsStandard, unsigned char ucNeedSave,
                 unsigned char ucWriteable)
{
    STUB_COMM_CODE(CFG_AddNode, CFG_INTERNAL_ERR);
    return CFG_OK;
}


int fail_CFG_DelNode = 0;
long CFG_DelNode(const char * pcPath)
{
    STUB_COMM_CODE(CFG_DelNode, CFG_INTERNAL_ERR);
    return CFG_OK;
}


int fail_CFG_GetNodeName = 0;
#define PARA_VAL_LIST_COUNT     10  /* 调用CFG_GetNodeName 最多能获取的链表个数*/
static int s_iCurListNum = 0;                /* 指定调用CFG_GetNodeName 时返回哪个链表*/
static ST_PARA_VAL *s_apstParaValList[PARA_VAL_LIST_COUNT] = { NULL };  /* 调用CFG_GetNodeName 时返回的链表*/
CFG_RET CFG_GetNodeName(const char *pcPath, unsigned char ucNextLevel,
                         ST_PARA_VAL **ppstParaInfo, const char *pcAccessor)
{
    STUB_COMM_CODE(CFG_GetNodeName, CFG_INTERNAL_ERR);

    if ( s_iCurListNum >= PARA_VAL_LIST_COUNT )
    {
        TEST_TRACE("s_iCurListNum >= PARA_VAL_LIST_COUNT.\n");
        return CFG_OK;
    }

    *ppstParaInfo = s_apstParaValList[s_iCurListNum++];

    return CFG_OK;
}


void ResetParaValList()
{
    ST_PARA_VAL *pstParaVal = NULL;
    int i = 0;

    for ( i = 0; i < PARA_VAL_LIST_COUNT; i++ )
    {
        while ( s_apstParaValList[i] )
        {
            pstParaVal = s_apstParaValList[i];
            s_apstParaValList[i] = s_apstParaValList[i]->pstNext;
            safe_free(pstParaVal->pcName);
            safe_free(pstParaVal);
        }
    }

    s_iCurListNum = 0;
}


/* 生成CFG_GetNodeName 生成的链表*/
void MakeParaValList(const char *pszPath, ...)
{
    ST_PARA_VAL *pstParaVal = NULL;
    va_list argp;
    const char *para = NULL;

    para = pszPath;
    va_start(argp, pszPath);
    do
    {
        pstParaVal = (ST_PARA_VAL *)malloc(sizeof(ST_PARA_VAL));
        if ( NULL == pstParaVal )
        {
            TEST_TRACE("malloc fail.\n");
        }

        pstParaVal->pcName = (char *)malloc(strlen(para) + 1);
        if ( NULL == pstParaVal->pcName )
        {
            TEST_TRACE("malloc fail.\n");
        }

        strcpy(pstParaVal->pcName, para);

        if ( s_iCurListNum >= PARA_VAL_LIST_COUNT )
        {
            TEST_TRACE("s_iCurListNum >= PARA_VAL_LIST_COUNT.\n");
            return;
        }

        pstParaVal->pstNext = s_apstParaValList[s_iCurListNum];
        s_apstParaValList[s_iCurListNum] = pstParaVal;

        para = va_arg(argp, const char*);
    }while ( NULL != para );
    va_end(argp);

    s_iCurListNum++;
}


int fail_CFG_GetLeafValAndType = 0;
#define LEAF_VAL_COUNT      60      /* 可以返回的节点值的个数*/
static int s_iCurLeafValNum = 0;     /* 指定当前返回的哪个节点值*/
static char s_szLeafVal[LEAF_VAL_COUNT][MAX_NODE_VALUE_LEN] = { {0} }; /* 可以返回的节点值*/
long CFG_GetLeafValAndType(const char *pcPath,
                char *pcVal, unsigned long *pulValLen,
                char *pcType, unsigned long *pulTypeLen)
{
        TEST_TRACE("\n");
    STUB_COMM_CODE(CFG_GetLeafValAndType, CFG_INTERNAL_ERR);

    if ( s_iCurLeafValNum >= LEAF_VAL_COUNT )
    {
        TEST_TRACE("s_iCurLeafValNum >= LEAF_VAL_COUNT.\n");
        return CFG_OK;
    }

    if ( NULL != pcVal )
    {
    	TEST_TRACE("leaf value:%s\n", s_szLeafVal[s_iCurLeafValNum]);
        safe_strncpy(pcVal, s_szLeafVal[s_iCurLeafValNum++], *pulValLen);
    }

    return CFG_OK;
}


void MakeLeafVal(const char *pszValue)
{
    if ( s_iCurLeafValNum >= LEAF_VAL_COUNT )
    {
        TEST_TRACE("s_iCurLeafValNum >= LEAF_VAL_COUNT.\n");
        return;
    }

    if ( NULL != pszValue )
    {
        safe_strncpy(s_szLeafVal[s_iCurLeafValNum++], pszValue, MAX_NODE_VALUE_LEN);
    }
}


void ResetLeafVal()
{
    int i = 0;

    s_iCurLeafValNum = 0;
    for ( i = 0; i < LEAF_VAL_COUNT; i++ )
    {
        s_szLeafVal[i][0] = '\0';
    }
}


void MakeLeafValList(const char *pszValue, ...)
{
    va_list argp;
    const char *para = NULL;

    ResetLeafVal();

    para = pszValue;
    va_start(argp, pszValue);
    do
    {
        if ( s_iCurLeafValNum >= LEAF_VAL_COUNT )
        {
            TEST_TRACE("s_iCurLeafValNum >= LEAF_VAL_COUNT.\n");
            break;
        }
        safe_strncpy(s_szLeafVal[s_iCurLeafValNum++], para, MAX_NODE_VALUE_LEN);
        para = va_arg(argp, const char*);
    }while ( NULL != para );
    va_end(argp);

    s_iCurLeafValNum = 0;
}


int fail_CFG_SetNodeVal = 0;
CFG_RET CFG_SetNodeVal(const char *pcPath, const char *pcVal, const char *pcAccessor)
{
    STUB_COMM_CODE(CFG_SetNodeVal, CFG_INTERNAL_ERR);
    return CFG_OK;
}


int fail_PC_SendProcessSignal = 0;
PC_RET PC_SendProcessSignal(unsigned long ulProcID, unsigned char ucSignal,
        unsigned char ucNeedReatart, const ST_PC_PROC_START *pstStartInfo)
{
    STUB_COMM_CODE(PC_SendProcessSignal, TBS_FAILED);
    return TBS_SUCCESS;
}


int fail_PC_StartProcess = 0;
PC_RET PC_StartProcess(const ST_PC_PROC_START *pstStartInfo)
{
	STUB_COMM_CODE(PC_StartProcess, TBS_FAILED);
	return TBS_SUCCESS;
}


int fail_MSG_CreateMessage = 0;
ST_MSG *MSG_CreateMessage(unsigned long ulSize)
{
    STUB_COMM_CODE(MSG_CreateMessage, NULL);
    memset(s_pstCreateMsg, 0, sizeof(ST_MSGHEAD));
    return s_pstCreateMsg;
}


int fail_MSG_ReleaseMessage = 0;
void MSG_ReleaseMessage(ST_MSG *pstMsg)
{
    STUB_COMM_CODE(MSG_ReleaseMessage, ;);
}


int fail_MSG_SendMessage = 0;
int MSG_SendMessage(ST_MSG * pstMsg)
{
    STUB_COMM_CODE(MSG_SendMessage, TBS_FAILED);
    return TBS_SUCCESS;
}


int fail_tbsSystem = 0;
int stub_tbsSystem(char *command, int printFlag)
{
    STUB_COMM_CODE(tbsSystem, TBS_FAILED);
    return TBS_SUCCESS;
}


int fail_fopen = 0;
FILE *stub_fopen(const char *path, const char *mode)
{
    STUB_COMM_CODE(fopen, NULL);
    return (FILE *)(1);
}


int fail_fclose = 0;
int stub_fclose(FILE *stream)
{
    STUB_COMM_CODE(fclose, EOF);
    return 0;
}

int fail_fprintf = 0;
int stub_fprintf(FILE *stream, const char *format, ...)
{
	STUB_COMM_CODE(fprintf, EOF);
	return 0;
}

int fail_fgets = 0;
static char s_szLineBuf[MAX_LINE_LEN] = { 0 };
char *stub_fgets(char *s, int size, FILE *stream)
{
    STUB_COMM_CODE(fgets, NULL);
    safe_strncpy(s, s_szLineBuf, MAX_LINE_LEN);
    return s;
}


int fail_fwrite = 0;
size_t stub_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    STUB_COMM_CODE(MSG_SendMessage, 0);
    return size;
}


int fail_time = 0;
unsigned int s_nTime = 0;
unsigned int stub_time(unsigned int n)
{
    return s_nTime;
}


int fail_malloc = 0;
void *stub_malloc(unsigned long ulLen)
{
    void *p = NULL;

    STUB_COMM_CODE(malloc, NULL);

    p = malloc(ulLen);
    if ( p == NULL )
    {
        TEST_TRACE("malloc fail.\n");
    }

    return p;
}

int fail_statfs = 0;
int stub_statfs(const char *path, struct statfs *buf)
{
	STUB_COMM_CODE(statfs, -1);
	return statfs(path, buf);
}

/*---------------------------------- 辅助测试函数-----------------------------*/

/* 生成ADD 消息*/
void MakeAddMsg(unsigned short usMID, const char *pszPath)
{
    char *pcPos = NULL;

    s_pstTestMsg->stMsgHead.usSrcMID = MID_CMM;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = 1;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_CMM_ADD_NODE;

    pcPos = s_pstTestMsg->szMsgBody;

    SET_STR(pcPos, pszPath);

    s_pstTestMsg->stMsgHead.ulBodyLength = pcPos - s_pstTestMsg->szMsgBody;
}


/* 生成DEL 消息*/
void MakeDelMsg(unsigned short usMID, const char *pszPath)
{
    char *pcPos = NULL;

    s_pstTestMsg->stMsgHead.usSrcMID = MID_CMM;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = 1;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_CMM_DEL_NODE;

    pcPos = s_pstTestMsg->szMsgBody;

    SET_STR(pcPos, pszPath);

    s_pstTestMsg->stMsgHead.ulBodyLength = pcPos - s_pstTestMsg->szMsgBody;
}


/* 生成GET 消息*/
void MakeGetMsg(unsigned short usMID, ...)
{
    char *pcPos = NULL;
    va_list argp;
    char *para = NULL;
    unsigned long ulCount = 0;

    s_pstTestMsg->stMsgHead.usSrcMID = MID_CMM;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = 1;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_CMM_GET_VAL;

    pcPos = s_pstTestMsg->szMsgBody;

    va_start(argp, usMID);
    while (1)
    {
        para = va_arg(argp, char*);
        if ( NULL == para )
        {
            break;
        }
        else
        {
            SET_STR(pcPos, para);
            ulCount++;
        }
    }
    va_end(argp);

    s_pstTestMsg->stMsgHead.ulBodyLength = pcPos - s_pstTestMsg->szMsgBody;

    pcPos = s_pstTestMsg->szMsgBody;

    SET_ULONG(pcPos, ulCount);
}


/* 生成SET 消息*/
void MakeSetMsg(unsigned short usMID, ...)
{
    char *pcPos = NULL;
    va_list argp;
    char *para = NULL;
    unsigned long ulCount = 0;

    s_pstTestMsg->stMsgHead.usSrcMID = MID_CMM;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = 1;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_CMM_SET_VAL;

    pcPos = s_pstTestMsg->szMsgBody;

    SET_ULONG(pcPos, 0);

    va_start(argp, usMID);
    while (1)
    {
        para = va_arg(argp, char*);
        if ( NULL == para )
        {
            break;
        }
        else
        {
            SET_STR(pcPos, para);
            ulCount++;
        }
    }
    va_end(argp);

    s_pstTestMsg->stMsgHead.ulBodyLength = pcPos - s_pstTestMsg->szMsgBody;

    pcPos = s_pstTestMsg->szMsgBody;

    SET_ULONG(pcPos, ulCount);
}


/* 生成配置更新消息*/
void MakeCfgUpdateMsg(unsigned short usMID)
{
    s_pstTestMsg->stMsgHead.usSrcMID = MID_CMM;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = 1;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_CMM_UPDATE;

    s_pstTestMsg->stMsgHead.ulBodyLength = 0;
}

/* 生成WAN连接建立消息 */
void MakeWanConnMsg(unsigned short usMID, char *pszPath, char *pszWanIfName, char *pszDNSServers)
{
	char aszItem[MAX_PATH_LEN] = {0};
	char *pcPos = NULL;

    s_pstTestMsg->stMsgHead.usSrcMID = MID_CMM;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = 1;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_WAN_CONN_EST;

	pcPos = s_pstTestMsg->szMsgBody;
	SET_ULONG(pcPos, 3);

	sprintf(aszItem, "Path=%s", pszPath);
    SET_STR(pcPos, aszItem);
	sprintf(aszItem, "InterfaceName=%s", pszWanIfName);
    SET_STR(pcPos, aszItem);
	sprintf(aszItem, "DNSServers=%s", pszDNSServers);
    SET_STR(pcPos, aszItem);

    s_pstTestMsg->stMsgHead.ulBodyLength = pcPos - s_pstTestMsg->szMsgBody;

}

void MakeProcOutputMsg(unsigned short usMID, unsigned long ulMsgID, ...)
{
    char *pcPos = NULL;
    int iCount = 0;
    va_list argp;
    char *para = NULL;
    int iMathNo = 0;

    s_pstTestMsg->stMsgHead.usSrcMID = MID_PC;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = ulMsgID;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_PC_PROC_OUTPUT;

    pcPos = s_pstTestMsg->szMsgBody;

    SET_ULONG(pcPos, 1);
    SET_UCHAR(pcPos, 1);

    va_start(argp, ulMsgID);
    while (1)
    {
        if ( iCount % 2 == 0 )
        {
            iMathNo = va_arg(argp, int);
        }
        else
        {
            para = va_arg(argp, char*);
        }

        if ( iMathNo < 0 )
        {
            break;
        }
        else
        {
            if ( iCount % 2 == 0 )
            {
                SET_UCHAR(pcPos, iMathNo);
            }
            else
            {
                SET_STR(pcPos, para);
            }
            iCount++;
        }
    }
    va_end(argp);

    pcPos = s_pstTestMsg->szMsgBody;
    SET_ULONG(pcPos, 1);
    SET_UCHAR(pcPos, iCount);

    s_pstTestMsg->stMsgHead.ulBodyLength = pcPos - s_pstTestMsg->szMsgBody;
}


void MakeStartProcAckMsg(unsigned short usMID, unsigned long ulStartOk, unsigned long ulProcId)
{
    char *pcPos = NULL;

    s_pstTestMsg->stMsgHead.usSrcMID = MID_PC;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = 1;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_PC_START_PROC_ACK;

    pcPos = s_pstTestMsg->szMsgBody;

    SET_ULONG(pcPos, ulStartOk);
    SET_ULONG(pcPos, ulProcId);

    s_pstTestMsg->stMsgHead.ulBodyLength = pcPos - s_pstTestMsg->szMsgBody;
}


void MakeSendSignalAckMsg(unsigned short usMID, unsigned long ulResult)
{
    char *pcPos = NULL;

    s_pstTestMsg->stMsgHead.usSrcMID = MID_PC;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = 1;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_PC_SEND_SIGNAL_ACK;

    pcPos = s_pstTestMsg->szMsgBody;

    SET_ULONG(pcPos, ulResult);

    s_pstTestMsg->stMsgHead.ulBodyLength = pcPos - s_pstTestMsg->szMsgBody;
}


void MakeProcStateMsg(unsigned short usMID, unsigned long ulProcId, unsigned char ucProcState, unsigned char ucSteadyFlag, unsigned char IsKilldeByUser)
{
    char *pcPos = NULL;

    s_pstTestMsg->stMsgHead.usSrcMID = MID_PC;
    s_pstTestMsg->stMsgHead.usDstMID = usMID;
    s_pstTestMsg->stMsgHead.ulMsgID = 1;
    s_pstTestMsg->stMsgHead.usMsgType = MSG_PC_PROC_STATE;

    pcPos = s_pstTestMsg->szMsgBody;

    SET_ULONG(pcPos, ulProcId);
    SET_UCHAR(pcPos, ucProcState);
    SET_UCHAR(pcPos, ucSteadyFlag);
    SET_UCHAR(pcPos, IsKilldeByUser);

    s_pstTestMsg->stMsgHead.ulBodyLength = pcPos - s_pstTestMsg->szMsgBody;
}


/* 重置环境*/
void ResetEnvironment(void *pInstanceList)
{
    /* 释放所有配置缓存*/
    FreeAllConfBuf(pInstanceList);

    /* 重置节点链表*/
    ResetParaValList();

    /* 重置节点值数组*/
    ResetLeafVal();

    /* 重置全局变量*/
    fail_MSG_RegModule= 0;
    fail_MSG_UnregModule = 0;
    fail_CFG_MemFree = 0;
    fail_CFG_GetNodeMaxIdx = 0;
    s_ulMaxNodeID = 0;
    fail_CFG_RegisterMID = 0;
    fail_CFG_RegisterCareMID = 0;
    fail_CFG_AddNode = 0;
    fail_CFG_DelNode = 0;
    fail_CFG_GetNodeName = 0;
    s_iCurListNum = 0;
    fail_CFG_GetLeafValAndType = 0;
    fail_CFG_SetNodeVal = 0;
    fail_PC_SendProcessSignal = 0;
    fail_MSG_CreateMessage = 0;
    fail_MSG_ReleaseMessage = 0;
    fail_MSG_SendMessage = 0;
    fail_tbsSystem= 0;
    fail_fopen = 0;
    fail_fclose = 0;
    fail_fgets = 0;
    s_szLineBuf[0] = 0;
    fail_fwrite = 0;
    fail_time = 0;
    s_nTime = 0;
    fail_malloc = 0;
    fail_statfs = 0;
}



int init_suit()
{
    /* 初始化s_pstTestMsg */
    s_pstTestMsg = (ST_MSG *)malloc(sizeof(ST_MSG) + MAX_MSG_LEN);
    if (NULL == s_pstTestMsg)
    {
        TEST_TRACE("malloc msg fail.\n");
        return 0;
    }

    memset(s_pstTestMsg, 0, sizeof(ST_MSG) + MAX_MSG_LEN);
    s_pstTestMsg->stMsgHead.ulBodyLength = MAX_MSG_LEN;

    /* 初始化s_pstCreateMsg */
    s_pstCreateMsg = (ST_MSG *)malloc(sizeof(ST_MSG) + MAX_MSG_LEN);
    if (NULL == s_pstCreateMsg)
    {
        TEST_TRACE("malloc msg fail.\n");
        return 0;
    }

    memset(s_pstCreateMsg, 0, sizeof(ST_MSG) + MAX_MSG_LEN);
    s_pstCreateMsg->stMsgHead.ulBodyLength = MAX_MSG_LEN;

    return 0;
}


int end_suit()
{
    safe_free(s_pstTestMsg);
    safe_free(s_pstCreateMsg);

    return 0;
}




