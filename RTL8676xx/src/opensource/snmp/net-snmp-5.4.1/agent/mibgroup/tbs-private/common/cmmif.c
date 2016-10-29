/* cmmif.c */

#include "cmmif.h"
#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "flash_layout.h"

 #ifdef ZTE_HOT_AP_SNMP_IF
#include "snmpProcess.h"
#endif

#define CALL_CMM_TIMEOUT	5 /* seconds */



static unsigned int g_uMsgId = 0;
static int g_iInit = 0;	/* 0 - uninitialized; 1 - initialized success; 2 - initialized failed */

#ifdef CMMIF_DEBUG
void SNMPA_ShowMessage(ST_MSG* pstMsg);
#endif


/*
*	函数名称: 	CMM_Init
*	函数功能: 	CMM接口模块初始化函数。
*	参数:		void
*	返回值  :	CMM_INIT_NONE - 尚未初始化。
*				CMM_INIT_SUCCESS - 初始化成功。
*				CMM_INIT_FAILED - 初始化失败。
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_Init()
{
	MSG_RET iRet;
	if (g_iInit==CMM_INIT_NONE) {
		iRet = MSG_RegModule(MID_SNMPA, NULL);	/* 在CMM_Call()中自己接收消息，所以不需要消息循环 */
		if (RET_FAILED(iRet)) {
			g_iInit = CMM_INIT_FAILED;
			CMMIF_TRACE("err: register module SNMPA failed (%d)!\n", iRet);
		} else {
			MSG_AllModStartOK();
			g_iInit = CMM_INIT_SUCCESS;
			CMMIF_TRACE("msg: register module SNMPA successed.\n");
		}
	}
	return g_iInit;
}

/*
*	函数名称: 	CMM_Free
*	函数功能: 	CMM接口模块释放函数。
*	参数:		void
*	返回值  :	void
*	创建    :	刘章平 / 2008-05-29
*/
void CMM_Free()
{
	if (g_iInit==CMM_INIT_SUCCESS) {
		MSG_UnregModule(MID_SNMPA, NULL);
		g_iInit = CMM_INIT_NONE;
		CMMIF_TRACE("msg: unregistered module SNMPA.\n");
	}
}


/*
*	函数名称: 	CMM_CreateMessage
*	函数功能: 	CMM消息创建函数。
*	参数:		usSrcMID - [in] 源模块ID
*				usDstMID - [in] 目的模块ID
*				usMsgType - [in] 消息类型
*				ulBodyLength - [in] 消息体长度。
*	返回值  :	如果消息创建成功，返回新创建消息的指针；
*				否则返回NULL。
*	创建    :	刘章平 / 2008-05-29
*/
ST_MSG* CMM_CreateMessage(
	unsigned short usSrcMID, unsigned short usDstMID,
	unsigned short usMsgType, unsigned long ulBodyLength)
{
	ST_MSG* pMsg = NULL;
	pMsg = MSG_CreateMessage(ulBodyLength);
	if (!pMsg) {
		CMMIF_TRACE("err: %s() failed - MSG_CreateMessage() returned NULL(SrcMID=%04x,DstMID=%04x,MsgType=%04x,BodySize=%08x)!\n",
			__func__, usSrcMID, usDstMID, usMsgType, (unsigned int)ulBodyLength);
		return NULL;
	}
	pMsg->stMsgHead.usSrcMID = usSrcMID;
	pMsg->stMsgHead.usDstMID = usDstMID;
	pMsg->stMsgHead.ulMsgID = ++g_uMsgId;
	pMsg->stMsgHead.usMsgType = usMsgType;
	pMsg->stMsgHead.ulBodyLength = ulBodyLength;
	return pMsg;
}

int SNMP_SaveEventsToFlash(char *szEventValue)
{
    unsigned short usLen = 0;
    int iRet = 0;

    if(szEventValue)
    {
        usLen = strlen(szEventValue) + 1;
        iRet = app_item_save(szEventValue, TBS_SNMP_EVENT, usLen);
        CMMIF_TRACE("Saved event to flash, return: %d\n", iRet);
    }

    return CMM_SUCCESS;
}

int SNMP_ReadEventFromFlash(char* szReadBuf)
{
    //char szReadBuf[65] = {0};
    unsigned short usLen = 64;

    if(app_item_get(szReadBuf, TBS_SNMP_EVENT, &usLen) == 0)
    {
        if(szReadBuf[0] != '\0')
        {
            //清除flash条目中的事件
            app_item_save("", TBS_SNMP_EVENT, 1);
        }
    }
	
    return CMM_SUCCESS;
}

/*
*	函数名称: 	CMM_CreateGetValMessage
*	函数功能: 	创建MSG_CMM_GET_VAL消息。
*	参数:		szLeafName - [in] 要获取其值的TR069叶子节点全路径名称。
*	返回值  :	如果消息创建成功，返回新创建消息的指针；
*				否则返回NULL。
*	创建    :	刘章平 / 2008-05-29
*/
ST_MSG* CMM_CreateGetValMessage(const char* szLeafName)
{
	unsigned long ulBodySize = (sizeof(unsigned long)+strlen(szLeafName)+1);
	ST_MSG* pMsg = CMM_CreateMessage(
		MID_SNMPA, MID_CMM,
		MSG_CMM_GET_VAL, ulBodySize);

	if (!pMsg) {
		return NULL;
	}

	/* Body */
	*((unsigned long*)pMsg->szMsgBody) = 1;	/* PathCount */
	strcpy(pMsg->szMsgBody+sizeof(unsigned long), szLeafName);		/* Path1 */

	return pMsg;
}


/*
*	函数名称: 	CMM_CreateSetValMessage
*	函数功能: 	创建MSG_CMM_SET_VAL消息。
*	参数:		szLeafName - [in] 要设置其值的TR069叶子节点全路径名称。
*				szValue - [in] 要设置的节点值。
*	返回值  :	如果消息创建成功，返回新创建消息的指针；
*				否则返回NULL。
*	创建    :	刘章平 / 2008-05-29
*/
ST_MSG* CMM_CreateSetValMessage(const char* szLeafName, const char* szValue)
{
	unsigned long ulBodySize = (sizeof(unsigned long)+strlen(szLeafName)+strlen(szValue)+2);
	ST_MSG* pMsg = CMM_CreateMessage(
		MID_SNMPA, MID_CMM,
		MSG_CMM_SET_VAL, ulBodySize);
	if (!pMsg) {
		return NULL;
	}

	/* Body */
	*((unsigned long*)pMsg->szMsgBody) = 1;	/* PathCount */
	sprintf(pMsg->szMsgBody+sizeof(unsigned long), "%s=%s", szLeafName, szValue);	/* Path=Value */

	return pMsg;
}

/*
*	函数名称: 	CMM_CreateGetNameMessage
*	函数功能: 	创建MSG_CMM_GET_NAME消息。
*	参数:		cNodeName - [in] 要获取其实例列表的TR069对象节点全路径名称。
*	返回值  :	如果消息创建成功，返回新创建消息的指针；
*				否则返回NULL。
*	创建    :	刘章平 / 2008-05-29
*/
ST_MSG* CMM_CreateGetNameMessage(const char* cNodeName)
{
	unsigned long ulBodySize = (sizeof(unsigned char)+strlen(cNodeName)+1);
	ST_MSG* pMsg = CMM_CreateMessage(
		MID_SNMPA, MID_CMM,
		MSG_CMM_GET_NAME, ulBodySize);
	if (!pMsg) {
		return NULL;
	}

	/* Body */
	pMsg->szMsgBody[0] = 1;	/* NextLevel */
	strcpy(pMsg->szMsgBody+sizeof(unsigned char), cNodeName);		/* Path */

	return pMsg;
}

/*
*	函数名称: 	CMM_CreateAddNodeMessage
*	函数功能: 	创建MSG_CMM_ADD_NODE消息。
*	参数:		cNodeName - [in] 要增加实例的TR069对象节点全路径名称。
*	返回值  :	如果消息创建成功，返回新创建消息的指针；
*				否则返回NULL。
*	创建    :	刘章平 / 2008-05-29
*/
ST_MSG* CMM_CreateAddNodeMessage(const char* cNodeName)
{
	unsigned long ulBodySize = strlen(cNodeName)+1;

	ST_MSG* pMsg = CMM_CreateMessage(
		MID_SNMPA, MID_CMM,
		MSG_CMM_ADD_NODE, ulBodySize);
	if (!pMsg) {
		return NULL;
	}

	/* Body */
	strcpy(pMsg->szMsgBody, cNodeName);		/* Path */

	return pMsg;
}


/*
*	函数名称: 	CMM_CreateDelNodeMessage
*	函数功能: 	创建MSG_CMM_ADD_NODE消息。
*	参数:		cNodeName - [in] 要删除的TR069对象实例全路径名称。
*	返回值  :	如果消息创建成功，返回新创建消息的指针；
*				否则返回NULL。
*	创建    :	刘章平 / 2008-05-29
*/
ST_MSG* CMM_CreateDelNodeMessage(const char* cNodeName)
{
	unsigned long ulBodySize = strlen(cNodeName)+1;
	ST_MSG* pMsg = CMM_CreateMessage(
		MID_SNMPA, MID_CMM,
		MSG_CMM_DEL_NODE, ulBodySize);
	if (!pMsg) {
		return NULL;
	}

	/* Body */
	strcpy(pMsg->szMsgBody, cNodeName);		/* Path */

	return pMsg;
}





/*
*	函数名称: 	CMM_Call
*	函数功能: 	向CMM发送请求消息并获取相应的应答消息。
*	参数:		pReq - [in] 已创建好的请求消息指针
*				ppRsp - [out] 消息指针的指针，用于保存返回的应答消息的指针。
*	返回值  :	CMM_SUCCESS - 成功，*ppRsp为应答消息指针
*				否则，返回出错代码，*ppRsp未定义。
*				CMM_ERR_SEND - 发送请求消息失败
*				CMM_ERR_RECV - 接收应答失败
*				CMM_ERR_TIMEOUT - 接收应答超时
*				CMM_FAIL - 其他错误
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_Call(ST_MSG* pReq, ST_MSG** ppRsp)
{
	MSG_RET iRet;
	time_t t0, t1, t;


	iRet = MSG_SendMessage(pReq);
	if (iRet!=MSG_OK) {
		CMMIF_TRACE("err: send request to CMM failed (%d)!\n", iRet);
		return CMM_ERR_SEND;
	}
#ifdef CMMIF_DEBUG
	SNMPA_ShowMessage(pReq);
#endif
	t = t0 = time(NULL);
	t1 = t0+CALL_CMM_TIMEOUT;
	for (;;) {
		t = time(NULL);
		if (t>t1) {	/* 等待应答超时 */
			CMMIF_TRACE("err: wait CMM response timeout (%d seconds)!\n", CALL_CMM_TIMEOUT);
			return CMM_ERR_TIMEOUT;
		}

		iRet = MSG_ReceiveMessage(ppRsp, t1-t);
		if (iRet!=MSG_OK) {
			CMMIF_TRACE("err: receive response from CMM failed (%d)!\n", iRet);
			return CMM_ERR_RECV;
		}
		/*  收到一个消息 */
		if (*ppRsp) {
			ST_MSG* pRsp = *ppRsp;
			if (pRsp->stMsgHead.ulMsgID==pReq->stMsgHead.ulMsgID&&
				pRsp->stMsgHead.usDstMID==pReq->stMsgHead.usSrcMID&&
				pRsp->stMsgHead.usSrcMID==pReq->stMsgHead.usDstMID)
			{
				/* 收到对应于我们请求的应答消息，退出循环 */
#ifdef CMMIF_DEBUG
				SNMPA_ShowMessage(pRsp);
#endif
				break;
			} else {
#ifdef ZTE_HOT_AP_SNMP_IF
				/* 不是对应于我们请求的应答消息，看是否是snmpa_MsgProcess所需要的，否则丢弃并继续接收 */
                            if(snmpa_MsgProcess((void*)pRsp) == TBS_SUCCESS)
                            {
                                safe_free_msg(pRsp);
                            }
                            else 
#endif
                                MSG_ReleaseMessage(*ppRsp);
			}
		} else {
			CMMIF_TRACE("err: received a null response, why?!\n");
			return CMM_FAIL;
		}
	}
	return CMM_SUCCESS;
}

/*
*	函数名称: 	CMM_GetStr
*	函数功能: 	从CMM获取一个节点的值。
*	参数:		szLeafName - [in] TR069叶子节点全路径名称。
*				szValue - [out] 用于保存TR069节点值的缓冲区。
*				uSize - [in] szValue缓冲区大小。
*	返回值  :	CMM_SUCCESS - 成功，szValue中保存指定节点值
*				否则，失败szValue未定义。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_GetStr(const char* szLeafName, char* szValue, unsigned int uSize, char **pv, int pvLen)
{
	MSG_RET iRet = CMM_SUCCESS;
	ST_MSG* pReq = NULL;
	ST_MSG* pRsp = NULL;
	char* pcItem = NULL;
	char* pc = NULL;
	u_long ulResult = 0;

	pReq = CMM_CreateGetValMessage(szLeafName);
	if (!pReq) {
		iRet = CMM_ERR_MEM;
		goto EXIT1;
	}

	iRet = CMM_Call(pReq, &pRsp);
	if (iRet!=CMM_SUCCESS) {
		goto EXIT2;
	}

	ulResult = *((u_long*)pRsp->szMsgBody);
	if (0!=ulResult) {
		iRet = CMM_FAIL;
		CMMIF_TRACE("err: get node value failed (%s result=%lu)!\n",
			szLeafName, ulResult);
		goto EXIT3;
	}

	pcItem = pRsp->szMsgBody+sizeof(unsigned long)+sizeof(unsigned long);	/* Result + Status */
	CMMIF_TRACE("msg: %s()--->%s\n", __func__, pcItem);
	pc = strtok(pcItem, "=");
	pc = strtok(NULL, "\n\r");
	if (pc) {
		strncpy(szValue, pc, uSize-1);
		szValue[uSize-1] = 0;
	} else {
		*szValue = 0;
	}
	CMMIF_TRACE("msg: value='%s'\n", szValue);

       /*map the pv and try to find the matched value*/
       if(pv && pvLen > 1) /*at least pvlen is 2*/
       {
            int found = 0;
            int i = 0;
            char *p = pv[i];
            while(1)
            {
                if(strcasecmp(szValue, pv[i]) == 0) /*compare name: value read from CMM*/
                {
                    found = 1;
                    strcpy(szValue, pv[i+1]); /*retrieve the value: value transfered to SNMP manager*/
                    break;
                }

                i += 2;
                if(i >= pvLen) break;
                
                p = pv[i];
            }
            
            if(found == 0) 
            {
                SNMPA_TRACE("Failed to get %s, because being unmathced!\n", szLeafName);
                //iRet = CMM_FAIL; /*If no item matched the value, return it as successful*/
            }
       }

EXIT3:
	MSG_ReleaseMessage(pRsp);
EXIT2:
	MSG_ReleaseMessage(pReq);
EXIT1:
	return iRet;
}


/*
*	函数名称: 	CMM_SetStr
*	函数功能: 	向CMM设置一个节点的值。
*	参数:		szLeafName - [in] TR069叶子节点全路径名称。
*				szValue - [in] 要设置的节点值。
*	返回值  :	CMM_SUCCESS - 成功
*				否则，失败。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_SetStr(const char* szLeafName, const char* szValue, char **pv, int pvLen)
{
	MSG_RET iRet = MSG_OK;
	ST_MSG* pReq = NULL;
	ST_MSG* pRsp = NULL;
	unsigned long ulResult = 0;
       char t_szValue[264] = {0};

       strncpy(t_szValue, szValue, sizeof(t_szValue));
       if(pv && pvLen>1) /*pvLen is at least 2*/
       {
            int found = 0;
            int i = 0;
            char *p = pv[i];

            while(1)
            {
                if(strcasecmp(t_szValue, pv[i+1]) == 0) /*compare value: value read from snmp manager*/
                {
                    found = 1;
                    strcpy(t_szValue, pv[i]); /*retrieve the name: value transfered to CMM*/
                    break;
                }

                i += 2;
                if(i >= pvLen) return CMM_FAIL;
                p = pv[i];
            }
            
            if(found == 0) 
            {
                SNMPA_TRACE("Failed to set %s, because being unmathced!\n", szLeafName);
                return CMM_FAIL; /*unvariable value*/
            }
       }

	pReq = CMM_CreateSetValMessage(szLeafName, t_szValue);
	if (!pReq) {
		iRet = CMM_ERR_MEM;
		goto EXIT1;
	}

	iRet = CMM_Call(pReq, &pRsp);
	if (iRet!=CMM_SUCCESS) {
		goto EXIT2;
	}

	ulResult = *((unsigned long*)pRsp->szMsgBody);
	if (0!=ulResult) {	/* Result */
		iRet = CMM_FAIL;
		CMMIF_TRACE("err: set node value failed (%s result=%lu)!\n",
			szLeafName, ulResult);
		goto EXIT3;
	}
	CMMIF_TRACE("msg: %s()--->%s='%s'\n",
		__func__, szLeafName, t_szValue);

EXIT3:
	MSG_ReleaseMessage(pRsp);
EXIT2:
	MSG_ReleaseMessage(pReq);
EXIT1:
	return iRet;
}

/*
*	函数名称: 	CMM_SetStr
*	函数功能: 	向CMM设置一个节点的值。
*	参数:		szLeavesName - [in] TR069叶子节点全路径名称及值，多个时用\0隔开，最后以\0结束。
*				ulLeavesLen - [in] szLeavesName的字节数，包含最后的\0。
*               iLeavesNum - [in] szLeavesName中包含的节点数
*	返回值  :	CMM_SUCCESS - 成功
*				否则，失败。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_SetStr_ForMultiLeaves(const char* szLeavesName, unsigned long ulLeavesLen, int iLeavesNum)
{
	MSG_RET iRet = MSG_OK;
	ST_MSG* pReq = NULL;
	ST_MSG* pRsp = NULL;
	unsigned long ulResult = 0;

	pReq = CMM_CreateMessage(MID_SNMPA, MID_CMM, MSG_CMM_SET_VAL, ulLeavesLen+4); /*4 is PathCount length*/
	if (!pReq)
       {
	    return CMM_FAIL;
	}

	/* Body */
	*((unsigned long*)pReq->szMsgBody) = iLeavesNum;	/* PathCount */
       memcpy(pReq->szMsgBody+sizeof(unsigned long), szLeavesName, ulLeavesLen);

	if (!pReq) {
		iRet = CMM_ERR_MEM;
		goto EXIT1;
	}

	iRet = CMM_Call(pReq, &pRsp);
	if (iRet!=CMM_SUCCESS) {
		goto EXIT2;
	}

	ulResult = *((unsigned long*)pRsp->szMsgBody);
	if (0!=ulResult) {	/* Result */
		iRet = CMM_FAIL;
		CMMIF_TRACE("err: set node value failed (%s result=%lu)!\n",
			szLeafName, ulResult);
		goto EXIT3;
	}
	CMMIF_TRACE("msg: %s()--->%s='%s'\n",
		__func__, szLeafName, t_szValue);

EXIT3:
	MSG_ReleaseMessage(pRsp);
EXIT2:
	MSG_ReleaseMessage(pReq);
EXIT1:
	return iRet;
}


int CMM_GetUlong(const char* szLeafName, u_long* puValue, char **pv, int pvLen)
{
	char cValue[32];
	int iRet = CMM_SUCCESS;
	iRet = CMM_GetStr(szLeafName, cValue, sizeof(cValue), pv, pvLen);
	if (iRet!=CMM_SUCCESS) {
		return iRet;
	}
	*puValue = (u_long)atoi(cValue);
	return iRet;
}

int CMM_SetUlong(const char* szLeafName, u_long uValue, char **pv, int pvLen)
{
	char cValue[32];
	sprintf(cValue, "%lu", uValue);
	return CMM_SetStr(szLeafName, cValue, pv, pvLen);
}



/*
*	函数名称: 	CMM_GetInstanceList
*	函数功能: 	获取对象的实例列表。
*	参数:		cNodeName - [in] 对象名称，全路径，以"."结束。
*	返回值  :	NULL - 失败
*				否则，返回一个数组，每个元素为指向实例名称的字符串，
*				数组以NULL元素结束。
*	创建    :	刘章平 / 2008-05-29
*/
char** CMM_GetInstanceList(const char* cNodeName)
{
	MSG_RET iRet = MSG_OK;
	ST_MSG* pReq = NULL;
	ST_MSG* pRsp = NULL;
	char** ppList = NULL;
	unsigned long ulResult = 0;
	unsigned long ulResCount = 0;
	unsigned long i = 0;
	char* pcItem = NULL;

	pReq = CMM_CreateGetNameMessage(cNodeName);
	if (!pReq) {
		CMMIF_TRACE("err: CMM_CreateGetNameMessage() failed!\n");
		goto ERROR4;
	}
	iRet = CMM_Call(pReq, &pRsp);
	if (iRet!=MSG_OK) {
		CMMIF_TRACE("err: CMM_Call() failed (%d)!\n", iRet);
		goto ERROR3;
	}

	ulResult = *((unsigned long*)pRsp->szMsgBody);
	if (ulResult!=0) {
		CMMIF_TRACE("err: get name list returned %lu!\n", ulResult);
		goto ERROR2;
	}
	/* 实例列表的个数 */

	CMMIF_TRACE("msg: %s()--->%s\n", __func__, cNodeName);

	ulResCount = *((unsigned long*)(pRsp->szMsgBody+sizeof(unsigned long)));
	pcItem = pRsp->szMsgBody+sizeof(unsigned long)+sizeof(unsigned long);
	ppList = (char**)malloc((ulResCount+1)*sizeof(*ppList));
	memset(ppList, 0, sizeof(*ppList)*(ulResCount+1));
	for (i = 0; i<ulResCount; i++) {
		unsigned int len = strlen(pcItem);
		char* pcName = strtok(pcItem, " ");	/* 我们只取名字部分 */
		pcItem = pcItem+len+1;	/* 指向下一项 */
		if (!pcName) {
			CMMIF_TRACE("err: get a null name!\n");
			goto ERROR1;
		}
		len = strlen(pcName);
		ppList[i] = (char*)malloc(len+1);
		if (!ppList[i]) {
			CMMIF_TRACE("err: allocate memory for name failed (len=%u)!\n", len+1);
			goto ERROR1;
		}
		strcpy(ppList[i], pcName);
		CMMIF_TRACE("msg: Instances[%lu] = %s\n",
			i, ppList[i]);
	}
	ppList[i] = NULL;	/* 因为之前已经调用memset把数组清0，所以这里不是必须的 */
	MSG_ReleaseMessage(pRsp);
	MSG_ReleaseMessage(pReq);
	return ppList;

ERROR1:
	CMM_FreeInstanceList(ppList);
ERROR2:
	MSG_ReleaseMessage(pRsp);
ERROR3:
	MSG_ReleaseMessage(pReq);
ERROR4:
	return NULL;

}

/*
*	函数名称: 	CMM_FreeInstanceList
*	函数功能: 	释放由CMM_GetInstanceList()创建的实例列表。
*	参数:		ppList - [in] 由CMM_GetInstanceList()创建的实例列表。
*	返回值  :	void
*	创建    :	刘章平 / 2008-05-29
*/
void CMM_FreeInstanceList(char** ppList)
{
	int i;
	for (i = 0; ppList[i]; i++) {
		free(ppList[i]);
	}
	free(ppList);
	ppList = NULL;
}

/*
*	函数名称: 	CMM_AddInstance
*	函数功能: 	为对象增加一个实例。
*	参数:		cNodeName - [in]  对象名称，全路径，以"."结束。
*				pulInstanceId - [out] 用于保存新增对象的实例号。
*	返回值  :	CMM_SUCCESS - 成功，*pulInstanceId为新增实例号。
*				其他，失败，*pulInstanceId未定义。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_AddInstance(const char* cNodeName, unsigned long* pulInstanceId)
{
	MSG_RET iRet = CMM_SUCCESS;
	ST_MSG* pReq = NULL;
	ST_MSG* pRsp = NULL;
	u_long ulResult = 0;

	pReq = CMM_CreateAddNodeMessage(cNodeName);
	if (!pReq) {
		iRet = CMM_ERR_MEM;
		goto EXIT1;
	}
	iRet = CMM_Call(pReq, &pRsp);
	if (iRet!=CMM_SUCCESS) {
		goto EXIT2;
	}
	ulResult = *((u_long*)pRsp->szMsgBody);
	if (ulResult!=0) {
		iRet = CMM_FAIL;
		CMMIF_TRACE("err: %s() failed %s = %lu\n", __func__, cNodeName, ulResult);
		goto EXIT3;
	}

	*pulInstanceId = *((u_long*)(pRsp->szMsgBody+sizeof(u_long)));
	CMMIF_TRACE("msg: %s()--->%s new instance is %lu\n", __func__, cNodeName, *pulInstanceId);

EXIT3:
	MSG_ReleaseMessage(pRsp);
EXIT2:
	MSG_ReleaseMessage(pReq);
EXIT1:
	return iRet;
}


/*
*	函数名称: 	CMM_DelInstance
*	函数功能: 	删除实例。
*	参数:		cInstanceName - [in]  要删除的实例全路径名称。
*	返回值  :	CMM_SUCCESS - 成功，*pulInstanceId为新增实例号。
*				其他，失败，*pulInstanceId未定义。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_DelInstance(const char* cInstanceName)
{
	MSG_RET iRet = CMM_SUCCESS;
	ST_MSG* pReq = NULL;
	ST_MSG* pRsp = NULL;
	u_long ulResult = 0;

	pReq = CMM_CreateDelNodeMessage(cInstanceName);
	if (!pReq) {
		iRet = CMM_ERR_MEM;
		goto EXIT1;
	}
	iRet = CMM_Call(pReq, &pRsp);
	if (iRet!=CMM_SUCCESS) {
		goto EXIT2;
	}

	ulResult = *((u_long*)pRsp->szMsgBody);
	if (ulResult!=0) {
		iRet = CMM_FAIL;
		CMMIF_TRACE("err: %s() failed %s = %lu\n", __func__, cInstanceName, ulResult);
		goto EXIT3;
	}

	CMMIF_TRACE("msg: %s()--->%s\n", __func__, cInstanceName);

EXIT3:
	MSG_ReleaseMessage(pRsp);
EXIT2:
	MSG_ReleaseMessage(pReq);
EXIT1:
	return iRet;
}

int CMM_SaveConfig()
{
	int iRet = CMM_SUCCESS;
	ST_MSG* pReq = NULL;
	ST_MSG* pRsp = NULL;
	u_long ulResult = 0;

	pReq = CMM_CreateMessage(MID_SNMPA, MID_CMM, MSG_CMM_SAVE_CFG, 0);
	if (!pReq) {
		iRet = CMM_ERR_MEM;
		goto EXIT1;
	}
	iRet = CMM_Call(pReq, &pRsp);
	if (CMM_SUCCESS!=iRet) {
		goto EXIT2;
	}
	ulResult = *((u_long*)pRsp->szMsgBody);
	if (0!=ulResult) {
		iRet = CMM_FAIL;
		CMMIF_TRACE("err: %s() failed = %lu\n", __func__, ulResult);
		goto EXIT3;
	}
	CMMIF_TRACE("msg: %s()--->%s\n", __func__, "Success");

EXIT3:
	MSG_ReleaseMessage(pRsp);
EXIT2:
	MSG_ReleaseMessage(pReq);
EXIT1:
	return iRet;
}

int CMM_RecoverConfig()
{
	int iRet = CMM_SUCCESS;
	ST_MSG* pReq = NULL;
	ST_MSG* pRsp = NULL;
	u_long ulResult = 0;

	pReq = CMM_CreateMessage(MID_SNMPA, MID_CMM, MSG_CMM_RECOVER_CFG, 0);
	if (!pReq) {
		iRet = CMM_ERR_MEM;
		goto EXIT1;
	}
	iRet = CMM_Call(pReq, &pRsp);
	if (CMM_SUCCESS!=iRet) {
		goto EXIT2;
	}
	ulResult = *((u_long*)pRsp->szMsgBody);
	if (0!=ulResult) {
		iRet = CMM_FAIL;
		CMMIF_TRACE("err: %s() failed = %lu\n", __func__, ulResult);
		goto EXIT3;
	}
	CMMIF_TRACE("msg: %s()--->%s\n", __func__, "Success");

EXIT3:
	MSG_ReleaseMessage(pRsp);
EXIT2:
	MSG_ReleaseMessage(pReq);
EXIT1:
	return iRet;
}

int CMM_Reboot()
{
	int iRet = CMM_SUCCESS;
	ST_MSG* pReq = NULL;

	pReq = CMM_CreateMessage(MID_SNMPA, MID_CMM, MSG_CMM_REBOOT, 0);
	if (!pReq) {
		iRet = CMM_ERR_MEM;
		goto EXIT1;
	}
	iRet = MSG_SendMessage(pReq);
	if (CMM_SUCCESS!=iRet) {
		goto EXIT2;
	}
	CMMIF_TRACE("msg: %s()--->%s\n", __func__, "Success");

EXIT2:
	MSG_ReleaseMessage(pReq);
EXIT1:
	return iRet;
}







/*****************************************************************************
*
* Debug函数
*
*****************************************************************************/

#ifdef CMMIF_DEBUG

/*
*	函数名称: 	MSGTYPE2NAME
*	函数功能: 	调试辅助函数，将数值形式MsgType转换为字符串可读形式。
*	输入参数: 	usMsgType - MsgType数值
*	返回值  :	对应的MsgType字符串名字。如果不是SNMPC支持的，返回
*				SNMPC_UNSUPPORTED_MSGTYPE(ddd)格式。
*	创建    : 	刘章平 / 2008-04-23
*/
const char* MSGTYPE2NAME(unsigned short usMsgType)
{
	#define MSG_TYPE_NAME(name) case name: return #name

	switch (usMsgType) {
	MSG_TYPE_NAME(MSG_CMM_SET_VAL);
	MSG_TYPE_NAME(MSG_CMM_SET_VAL_ACK);
	MSG_TYPE_NAME(MSG_CMM_GET_VAL);
	MSG_TYPE_NAME(MSG_CMM_GET_VAL_ACK);
	MSG_TYPE_NAME(MSG_CMM_COMMIT);
	MSG_TYPE_NAME(MSG_CMM_CANCEL);
	MSG_TYPE_NAME(MSG_PC_START_PROC);
	MSG_TYPE_NAME(MSG_PC_START_PROC_ACK);
	MSG_TYPE_NAME(MSG_PC_SEND_SIGNAL);
	MSG_TYPE_NAME(MSG_PC_SEND_SIGNAL_ACK);
	MSG_TYPE_NAME(MSG_PC_PROC_STATE);
	MSG_TYPE_NAME(MSG_CMM_UPDATE);
	MSG_TYPE_NAME(MSG_CMM_UPDATE_ACK);
	MSG_TYPE_NAME(MSG_CMM_ADD_NODE);
	MSG_TYPE_NAME(MSG_CMM_ADD_NODE_ACK);
	MSG_TYPE_NAME(MSG_CMM_DEL_NODE);
	MSG_TYPE_NAME(MSG_CMM_DEL_NODE_ACK);
	MSG_TYPE_NAME(MSG_CMM_GET_NAME);
	MSG_TYPE_NAME(MSG_CMM_GET_NAME_ACK);
	MSG_TYPE_NAME(MSG_CMM_SAVE_CFG);
	MSG_TYPE_NAME(MSG_CMM_SAVE_CFG_ACK);
	MSG_TYPE_NAME(MSG_CMM_RECOVER_CFG);
	MSG_TYPE_NAME(MSG_CMM_RECOVER_CFG_ACK);
	MSG_TYPE_NAME(MSG_CMM_REBOOT);
	default:
		{
			static char s_szUnsupported[64];
			sprintf(s_szUnsupported, "SNMPC_UNSUPPORTED_MSGTYPE(%lu)", (unsigned long)usMsgType);
			return s_szUnsupported;
		}
	}
}

/*
*	函数名称: 	MID2NAME
*	函数功能: 	调试辅助函数，将数值形式MID转换为字符串可读形式。
*	输入参数: 	usMID - MID数值
*	返回值  :	对应的MID字符串名字。如果不是SNMPC支持的，返回
*				SNMPC_UNSUPPORTED_MID(ddd)格式。
*	创建    : 	刘章平 / 2008-04-23
*/
const char* MID2NAME(unsigned short usMID)
{
	#define MID_NAME(name) case name: return #name
	switch (usMID) {
	MID_NAME(MID_SNMPA);
	MID_NAME(MID_SNMPC);
	MID_NAME(MID_PC);
	MID_NAME(MID_CMM);
	default:
		{
			static char s_szUnsupported[64];
			sprintf(s_szUnsupported, "SNMPC_UNSUPPORTED_MID(%lu)", (unsigned long)usMID);
			return s_szUnsupported;
		}
	}
}

/*
*	函数名称: 	SNMPA_ShowMessage
*	函数功能: 	以可读形式显示消息内容，用于DEBUG。
*	输入参数: 	pstMsg - 指向要显示消息的指针.
*	返回值  :	void
*	创建    : 	刘章平 / 2008-04-17
*/
void SNMPA_ShowMessage(ST_MSG* pstMsg)
{
	char* pcPos = (char*)pstMsg->szMsgBody;
	char* pcEnd = (pcPos+pstMsg->stMsgHead.ulBodyLength);

	/* Temp values */
	unsigned long uLong;
	unsigned char uChar;

/*************************************
*
* MACROS for body show.
*
*************************************/

#define INDENTS "                                                                          "
#define INDENT_UNIT	2
#define GET_INDENT(indent) (INDENTS+(sizeof(INDENTS)-1)-((indent)*INDENT_UNIT))

/*
* 功能:提取并显示一个unsigned long类型的量
* name - 变量名字，字符串
* indent - 打印信息时缩进量
*/
#define SHOW_ULONG(name, indent) \
	do {\
		if (pcPos+sizeof(uLong)>pcEnd) { \
			printf("\tBad message - get %s(unsigned long) failed!\n", name); \
			return; \
		} else { \
			GET_ULONG(pcPos, uLong); \
			printf("%s%s=%lu\n", GET_INDENT(indent), name, uLong);\
		} \
	} while (0)

/*
* 功能:提取并显示一个unsigned char类型的量
* name - 变量名字，字符串
* indent - 打印信息时缩进量
*/
#define SHOW_UCHAR(name, indent) \
	do {\
		if (pcPos+sizeof(uChar)>pcEnd) { \
			printf("\tBad message - get %s(unsigned char) failed!\n", name); \
			return; \
		} else { \
			GET_UCHAR(pcPos, uChar); \
			printf("%s%s=%lu\n", GET_INDENT(indent), name, (unsigned long)uChar);\
		} \
	} while (0)

/*
* 功能:提取并显示一个列表的内容。
* name - 列表名字，字符串
* get_count - 提取列表项目个数的方法，GET_ULONG或者GET_UCHAR等
* count_type - 列表项个数的类型，与get_count一致，可以为unsigned long或者unsigned char等
* indent - 打印信息时缩进量
*/
#define SHOW_LIST(name, get_count, count_type, indent) \
	do {\
		count_type count; \
		char* item; \
		count_type i; \
		if (pcPos+sizeof(count)>pcEnd) { \
			printf("%sBad message - get  %sCount failed!\n", GET_INDENT(indent), name); \
			return;\
		}\
		get_count(pcPos, count);\
		printf("%s%sCount=%lu\n", GET_INDENT(indent), name, (unsigned long)count);\
		for (i = 0; i<count; i++) {\
			if (pcPos>=pcEnd) { \
				printf("%sBad message - item count more than actual item count!\n", GET_INDENT(indent+1)); \
				return; \
			} \
			GET_STR(pcPos, item);\
			printf("%s%s%lu='%s'\n", GET_INDENT(indent+1), name, (unsigned long)(i+1), (item?item:"(null)"));\
		}\
	} while (0)

/*
* 功能:提取并显示错误代码和错误信息列表。
* indent - 打印信息时缩进量
*/
#define SHOW_ERROR(indent) \
	do { \
		SHOW_ULONG("ErrNo", indent); \
		SHOW_LIST("ErrItem", GET_ULONG, unsigned long, indent); \
	} while (0)

/*************************************
*
* MACROS for body show end.
*
*************************************/


	printf("Head:\n");
	printf("%sSrcMID=%s\n", GET_INDENT(1), MID2NAME(pstMsg->stMsgHead.usSrcMID));
	printf("%sDstMID=%s\n", GET_INDENT(1), MID2NAME(pstMsg->stMsgHead.usDstMID));
	printf("%sMsgID=%lu\n", GET_INDENT(1), pstMsg->stMsgHead.ulMsgID);
	printf("%sMsgType=%s\n", GET_INDENT(1), MSGTYPE2NAME(pstMsg->stMsgHead.usMsgType));
	printf("%usMsgType=%u\n", GET_INDENT(1), (unsigned int)pstMsg->stMsgHead.usMsgType);
	printf("%usFirstMID=%02X\n", GET_INDENT(1), (unsigned int)pstMsg->stMsgHead.usFirstMID);
	printf("%sBodyLength=%lu\n", GET_INDENT(1), pstMsg->stMsgHead.ulBodyLength);
	printf("Head.\n");

	printf("Body:\n");

	switch (pstMsg->stMsgHead.usMsgType) {
	case MSG_CMM_SET_VAL:
		SHOW_LIST("SetItem", GET_ULONG, unsigned long, 1);
		break;

	case MSG_CMM_SET_VAL_ACK:
		SHOW_ULONG("Result", 1);
		if (uLong) {	// failed
			SHOW_ERROR(1);
		} else {	// success
			SHOW_ULONG("Status", 1);
		}
		break;

	case MSG_CMM_GET_VAL:
		SHOW_LIST("Path", GET_ULONG, unsigned long, 1);
		break;

	case MSG_CMM_GET_VAL_ACK:
		SHOW_ULONG("Result", 1);
		if (uLong) {
			SHOW_ERROR(1);
		} else {
			SHOW_LIST("ResItem", GET_ULONG, unsigned long, 1);
		}

	case MSG_CMM_COMMIT:
	case MSG_CMM_CANCEL:
		// No  message body.
		break;

	case MSG_PC_START_PROC:
		SHOW_UCHAR("SteadyFlag", 1);
		SHOW_LIST("ArgVal", GET_UCHAR, unsigned char, 1);
		SHOW_LIST("MonVal", GET_UCHAR, unsigned char, 1);
		break;

	case MSG_PC_START_PROC_ACK:
		SHOW_ULONG("StartOK", 1);
		SHOW_ULONG("ProcID", 1);
		break;

	case MSG_PC_SEND_SIGNAL:
		SHOW_ULONG("ProcID", 1);
		SHOW_UCHAR("Signal", 1);
		SHOW_UCHAR("NeedRestart", 1);
		SHOW_UCHAR("NeedSteady", 1);
		SHOW_LIST("ArgVal", GET_UCHAR, unsigned char, 1);
		SHOW_LIST("MonVal", GET_UCHAR, unsigned char, 1);
		break;

	case MSG_PC_SEND_SIGNAL_ACK:
		SHOW_ULONG("Result", 1);
		break;

	case MSG_PC_PROC_STATE:
		SHOW_ULONG("ProcID", 1);
		SHOW_UCHAR("ProcState", 1);
		SHOW_UCHAR("SteadyFlag", 1);
		SHOW_UCHAR("IsKilledByUser", 1);
		break;

	case MSG_CMM_UPDATE:
		// No message body
		break;

	case MSG_CMM_UPDATE_ACK:
		SHOW_ULONG("Result", 1);
		break;

	case MSG_CMM_ADD_NODE:
	case MSG_CMM_DEL_NODE:
		printf("%sObjPath=%s\n", GET_INDENT(1), pcPos);
		break;

	case MSG_CMM_ADD_NODE_ACK:
		SHOW_ULONG("Result", 1);
		if (uLong) {
			SHOW_ERROR(1);
		} else {
			SHOW_ULONG("ObjNo", 1);
			SHOW_ULONG("ObjStatus", 1);
		}
		break;

	case MSG_CMM_DEL_NODE_ACK:
		SHOW_ULONG("Result", 1);
		if (uLong) {
			SHOW_ERROR(1);
		} else {
			SHOW_ULONG("ObjStatus", 1);
		}
		break;

	case MSG_CMM_GET_NAME:
		SHOW_UCHAR("NextLevel", 1);
		printf("%sPath=%s\n", GET_INDENT(1), pcPos);
		break;

	case MSG_CMM_GET_NAME_ACK:
		SHOW_ULONG("Result", 1);
		if (uLong) {
			SHOW_ERROR(1);
		} else {
			SHOW_LIST("ResItem", GET_ULONG, unsigned long, 1);
		}
		break;

	case MSG_CMM_SAVE_CFG:
	case MSG_CMM_RECOVER_CFG:
	case MSG_CMM_REBOOT:
		/* no body */
		break;

	case MSG_CMM_SAVE_CFG_ACK:
	case MSG_CMM_RECOVER_CFG_ACK:
		SHOW_ULONG("Result", 1);
		if (uLong) {
			SHOW_ERROR(1);
		}
		break;

	default:
		break;
	}
	printf("Body.\n");
}

#endif

#include "commhandle.h"
int GetActiveWanConnDevice(char* cIfName, size_t uSize, unsigned long* pulInstId)
{
    int ret = -1;
    char** ppWanDevList = NULL;
    size_t i = 0;
    ppWanDevList = CMM_GetInstanceList(TR069_ROOT_WanConnDev);
    if (!ppWanDevList)
    {
    	return ret;
    }
    *cIfName = 0;
    for (i = 0; ppWanDevList[i]; i++)
    {
    	char cNodeName[CMM_MAX_NODE_NAME];
    	sprintf(cNodeName, "%s%s", ppWanDevList[i], "X_TWSZ-COM_DeviceName");
    	if (CMM_GetStr(cNodeName, cIfName, uSize, NULL, 0) != CMM_SUCCESS)
       {
    		continue;
    	}
    	if (0 == *cIfName)
       {
    		continue;
    	}

       if(pulInstId) 	*pulInstId = atoi(ppWanDevList[i]+sizeof(TR069_ROOT_WanConnDev)-1);
       
    	ret = 0;
    	break;
    }
    CMM_FreeInstanceList(ppWanDevList);

    return ret;
}
static int get_wan_ip_addr(unsigned long* instid, char* cIpAddr, size_t size)
{
	char cIfName[32];
	/* 查找WAN Device Name不为空的一个实例，并获取接口名和实例号 */
	if (0!=GetActiveWanConnDevice(cIfName, sizeof(cIfName), instid)) {
		return -1;
	}

	/* 查询接口的IP地址 */
	if (0!=IfConfigRead(cIfName, NULL, IFCONFIG_IP, ":", cIpAddr, size)) {
		*cIpAddr = 0;
	}
	return 0;
}
/*
 * read the references for ZTE HOT AP when snmp sending linkup trap
 * include serialNumber, wanipaddress, productClass, softwareVersion
 * NOTE - after invoking the method, please pay attention to free <ref>
 * 0 - successful, non-zero fail
 */
int CMM_GetZTE_LinkupRef(char **ref)
{
    char *szPath[] = {
                        "InternetGatewayDevice.DeviceInfo.SerialNumber",
                        "InternetGatewayDevice.DeviceInfo.ProductClass",
                        "InternetGatewayDevice.DeviceInfo.SoftwareVersion"};
    int i = 0;
    int iPathLen = 64;
    unsigned long ulInstId = 0;
    char *ppRef[LINKUP_REF_NUM] = {NULL};
    if (get_wan_ip_addr(&ulInstId, ref[0], iPathLen))
    {
    	return -1;
    }

    for(i=0; i<LINKUP_REF_NUM-1; i++)
    {
        if(CMM_GetStr(szPath[i], ref[i+1], iPathLen, NULL, 0) != CMM_SUCCESS)
        {
            return -1;
        }
    }

    return 0;
}

/*跟下面的方法功能刚好相反*/
static struct sockaddr_in loginsrv_addrlist; //only for the following method
unsigned char *CHARS_TO_intIPADDR(const char *ipaddr)
{
    unsigned long sock_add_val = inet_addr(ipaddr);
    loginsrv_addrlist.sin_addr.s_addr = 0;
    //if(sock_add_val != INADDR_NONE)
        loginsrv_addrlist.sin_addr.s_addr = htonl(sock_add_val);

    return (unsigned char*)&(loginsrv_addrlist.sin_addr.s_addr);
}

/*把网络字节序的整形IP地址转换成以.隔开的字符串形IP地址*/
char *intIPAddr_TO_CHARS(char *pszIpAddr, unsigned long netSortIntIpaddr)
{
    char *ipaddr = inet_ntoa(*(struct in_addr*)&netSortIntIpaddr);
    if(ipaddr) strcpy(pszIpAddr, ipaddr);
    else return NULL;

    return pszIpAddr;
}
