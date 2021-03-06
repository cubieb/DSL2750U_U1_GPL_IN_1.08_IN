/* commhandle.c */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <errno.h>
#include <ctype.h>

#include "commhandle.h"
#include "cmmif.h"
#include "tbsutil.h"


#if 0

/*
*	函数名称: 	commhandle_ReadOnlyStr
*	函数功能: 	只读字符串通用处理函数，从配置树获取字符串节点的值，
*				并填写到requests结构中。
*	参数:		cNodename: 		[in]全路径的TR069节点名。
*				uMaxValueSize:	[in]用于保存节点值的缓冲区的大小。
*				reqinfo:		[in]壳肭笮畔ⅰ�
*				requests:		[out]包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_ReadOnlyStr(const char* cNodeName, unsigned int uMaxValueSize,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int iRet = CMM_SUCCESS;
	char* pValue = NULL;
	switch (reqinfo->mode) {
	case MODE_GET:
		pValue = (char*)malloc(uMaxValueSize);
		if (!pValue) {
			return SNMP_ERR_GENERR;
		}
		iRet = CMM_GetStr(cNodeName, pValue, uMaxValueSize);
		if (iRet!=CMM_SUCCESS) {
			free(pValue);
			pValue = NULL;
			return SNMP_ERR_NOSUCHNAME;
		}
		snmp_set_var_typed_value(requests->requestvb,
			ASN_OCTET_STR, (u_char *)pValue, strlen(pValue));
		free(pValue);
		pValue = NULL;
		break;

	default:
		snmp_log(LOG_ERR,
			"unknown mode (%d) in %s\n",
			reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}


/*
*	函数名称: 	commhandle_ReadWriteStr
*	函数功能: 	读写字符串通用处理函数，从配置树获取字符串节点的值，
*				并填写到requests结构中，或者写SNMP节点值到对应的
*				TR069节点。
*	输入:		cNodename:		[in] 全路径的TR069节点名。
*				uMaxValueSize:	[in] 用于保存节点值的缓冲区的大小。
*				reqinfo:		[in] 请求信息。
*				requests:		[in/out] 包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_ReadWriteStr(const char* cNodeName, unsigned int uMaxValueSize,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int             ret;
	int iRet = CMM_SUCCESS;
	char* pValue = NULL;

	switch (reqinfo->mode) {
	case MODE_GET:
		return commhandle_ReadOnlyStr(cNodeName, uMaxValueSize, reqinfo, requests);

	case MODE_SET_RESERVE1:
		ret = netsnmp_check_vb_type(requests->requestvb, ASN_OCTET_STR);
		if (ret != SNMP_ERR_NOERROR) {
			netsnmp_set_request_error(reqinfo, requests, ret);
		}
		break;

	case MODE_SET_RESERVE2:
	case MODE_SET_FREE:
	case MODE_SET_ACTION:
		/* Nothing to do here. */
		break;

	case MODE_SET_COMMIT:
		pValue = (char*)malloc(requests->requestvb->val_len+1);
		if (!pValue) {
			netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
			return SNMP_ERR_GENERR;
		}
		memcpy(pValue, requests->requestvb->val.string, requests->requestvb->val_len);
		pValue[requests->requestvb->val_len] = 0;
		iRet = CMM_SetStr(cNodeName, pValue);
		free(pValue);
		pValue = NULL;
		if (iRet!=CMM_SUCCESS) {
			netsnmp_set_request_error(reqinfo, requests,
				SNMP_ERR_COMMITFAILED);
		}
		break;

	case MODE_SET_UNDO:
		return SNMP_ERR_UNDOFAILED;

	default:
		snmp_log(LOG_ERR,
			"unknown mode (%d) in %s\n",
			reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}


/*
*	函数名称: 	commhandle_ReadOnlyLocalInt
*	函数功能: 	只读本地整数通用处理函数，从进程内部获取整数变量的值，
*				并填写到requests结构中。
*	参数:		pulValue:	[in] 指向u_long类型变量的指针。
*				ucIntType:	[in] 整数的ASN编码类型，如:ASN_INTEGER,ASN_COUNTER等。
*				reqinfo:	[in] 请求信息。
*	输出参数:	requests:	[out] 包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_ReadOnlyLocalInt(u_long* pulValue, u_char ucIntType/* ASN_INTEGER, ASN_COUNTER etc*/,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	switch (reqinfo->mode) {
	case MODE_GET:
		snmp_set_var_typed_value(requests->requestvb,
			ucIntType, (u_char *)pulValue, sizeof(*pulValue));
		break;


	default:
		snmp_log(LOG_ERR,
			"unknown mode (%d) in %s\n", reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}

/*
*	函数名称: 	commhandle_ReadWriteLocalInt
*	函数功能: 	读写本地整数通用处理函数，从进程内部获取整数变量的值，
*				并填写到requests结构中，或者设置SNMP节点值到对应的
*				内存整数变量中。
*	参数:		pulValue:	[in] 指向u_long类型变量的指针。
*				ucIntType:	[in] 整数的ASN编码类型，如:ASN_INTEGER,ASN_COUNTER等。
*				reqinfo:	[in] 请求信息。
*	输出参数:	requests:	[in/out] 包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_ReadWriteLocalInt(u_long* pulValue, u_char ucIntType/* ASN_INTEGER, ASN_COUNTER etc*/,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int ret = SNMP_ERR_NOERROR;
	switch (reqinfo->mode) {
	case MODE_GET:
		return commhandle_ReadOnlyLocalInt(pulValue, ucIntType, reqinfo, requests);

	case MODE_SET_RESERVE1:
		ret = netsnmp_check_vb_type(requests->requestvb, ucIntType);
		if (ret != SNMP_ERR_NOERROR) {
			netsnmp_set_request_error(reqinfo, requests, ret);
		}
		break;

	case MODE_SET_RESERVE2:
	case MODE_SET_FREE:
	case MODE_SET_ACTION:
		break;

	case MODE_SET_COMMIT:
		*pulValue = (u_long)(*requests->requestvb->val.integer);
		break;

	case MODE_SET_UNDO:
		return SNMP_ERR_UNDOFAILED;

	default:
		snmp_log(LOG_ERR, "unknown mode (%d) in %s\n",
			reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}


int commhandle_ReadOnlyLocalStr(const char* cValue,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	switch (reqinfo->mode) {
	case MODE_GET:
		snmp_set_var_typed_value(requests->requestvb,
			ASN_OCTET_STR, (u_char *)cValue, strlen(cValue));
		break;


	default:
		snmp_log(LOG_ERR,
			"unknown mode (%d) in %s\n", reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}

/*
*	函数名称: 	commhandle_ReadOnlyInt
*	函数功能: 	只读整数通用处理函数，从配置树获取整数节点的值，
*				并填写到requests结构中。
*	参数:		cNodename: 		[in]全路径的TR069节点名。
*				ucIntType:		[in]ASN编码整数类型，如:ASN_INTEGER,ASN_COUNTER32。
*				reqinfo:		[in]请求信息。
*				requests:		[out]包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_ReadOnlyInt(const char* cNodeName, u_char ucIntType,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int iRet = CMM_SUCCESS;
	char cValue[32];
	int iValue = 0;
	switch (reqinfo->mode) {
	case MODE_GET:
		iRet = CMM_GetStr(cNodeName, cValue, sizeof(cValue));
		if (iRet!=CMM_SUCCESS) {
			return SNMP_ERR_NOSUCHNAME;
		}
		iValue = atoi(cValue);
		snmp_set_var_typed_value(requests->requestvb,
			ucIntType, (u_char *)&iValue, sizeof(iValue));
		break;


	default:
		snmp_log(LOG_ERR,
			"unknown mode (%d) in %s\n",
			reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}

/*
*	函数名称: 	commhandle_ReadWriteInt
*	函数功能: 	读写整数通用处理函数，GET操作从配置树获取整数节点的值，
*				并填写到requests结构中。SET操作将SNMP设置的MIB节点值写到指定
*				的TR069节点。
*	参数:		cNodename: 		[in]全路径的TR069节点名。
*				ucIntType:		[in]ASN编码整数类型，如:ASN_INTEGER,ASN_COUNTER32。
*				reqinfo:		[in]请求信息。
*				requests:		[in/out]包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_ReadWriteInt(const char* cNodeName, u_char ucIntType,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int             ret;
	int iRet = CMM_SUCCESS;
	char cValue[32];
	int iValue = 0;

	switch (reqinfo->mode) {
	case MODE_GET:
		return commhandle_ReadOnlyInt(cNodeName, ucIntType, reqinfo, requests);

	case MODE_SET_RESERVE1:
		ret = netsnmp_check_vb_type(requests->requestvb, ucIntType);
		if (ret != SNMP_ERR_NOERROR) {
			netsnmp_set_request_error(reqinfo, requests, ret);
		}
		break;

	case MODE_SET_RESERVE2:
	case MODE_SET_FREE:
	case MODE_SET_ACTION:
		break;

	case MODE_SET_COMMIT:
		iValue = (int)(*requests->requestvb->val.integer);
		switch (ucIntType) {
		case ASN_COUNTER:
		case ASN_UNSIGNED:
			sprintf(cValue, "%u", (unsigned int)iValue);
			break;
		case ASN_INTEGER:
		default:
			sprintf(cValue, "%d", iValue);
			break;
		}
		iRet = CMM_SetStr(cNodeName, cValue);
		if (iRet!=CMM_SUCCESS) {
			netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
		}
		break;

	case MODE_SET_UNDO:
		return SNMP_ERR_UNDOFAILED;

	default:
		snmp_log(LOG_ERR, "unknown mode (%d) in %s\n",
			reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}

/*
*	函数名称: 	commhandle_ReadOnlyTruthValue
*	函数功能: 	只读TruthValue通用处理函数，从配置树获取boolean节点的值，
*				并填写到requests结构中。
*	参数:		cNodename: 		[in]全路径的TR069节点名。
*				reqinfo:		[in]请求信息。
*				requests:		[out]包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_ReadOnlyTruthValue(const char* cNodeName,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int iRet = CMM_SUCCESS;
	char cValue[32];
	int iValue = 0;
	switch (reqinfo->mode) {
	case MODE_GET:
		iRet = CMM_GetStr(cNodeName, cValue, sizeof(cValue));
		if (iRet!=CMM_SUCCESS) {
			return SNMP_ERR_NOSUCHNAME;
		}
		if (tbsStrToBool(cValue)) {
			iValue = TRUTHVALUE_TRUE;	/* True of TruthValue */
		} else {
			iValue = TRUTHVALUE_FALSE;	/* False of TruthValue */
		}
		snmp_set_var_typed_value(requests->requestvb,
			ASN_INTEGER, (u_char *)&iValue, sizeof(iValue));
		break;


	default:
		snmp_log(LOG_ERR,
			"unknown mode (%d) in %s\n",
			reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}

/*
*	函数名称: 	commhandle_ReadWriteTruthValue
*	函数功能: 	读写TruthValue通用处理函数。GET操作从配置树获取boolean节点的值
*				并填写到requests结构中。SET操作将requests中的值写入到配置树指定
*				节点。
*	参数:		cNodename: 		[in]全路径的TR069节点名。
*				reqinfo:		[in]请求信息。
*				requests:		[in/out]包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_ReadWriteTruthValue(const char* cNodeName,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int             ret;
	int iRet = CMM_SUCCESS;
	int iValue = 0;

	switch (reqinfo->mode) {
	case MODE_GET:
		return commhandle_ReadOnlyTruthValue(cNodeName, reqinfo, requests);

	case MODE_SET_RESERVE1:
		ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
		if (ret != SNMP_ERR_NOERROR) {
			netsnmp_set_request_error(reqinfo, requests, ret);
		}
		break;

	case MODE_SET_RESERVE2:
	case MODE_SET_FREE:
	case MODE_SET_ACTION:
		break;

	case MODE_SET_COMMIT:
		iValue = (int)(*requests->requestvb->val.integer);
		if (iValue==TRUTHVALUE_TRUE) {
			iRet = CMM_SetStr(cNodeName, "1");
		} else {
			iRet = CMM_SetStr(cNodeName, "0");
		}
		if (iRet!=CMM_SUCCESS) {
			netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
		}
		break;

	case MODE_SET_UNDO:
		return SNMP_ERR_UNDOFAILED;

	default:
		snmp_log(LOG_ERR, "unknown mode (%d) in %s\n",
			reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}

/*
*	函数名称: 	commhandle_SlotCreate
*	函数功能: 	读写xxxSlotCreate通用处理函数。
*	参数:		pulSlotCreationEnabled:
*								[in/out] xxxSlotCreate变量指针。
*				reqinfo:		[in] 请求信息。
*				requests:		[in/out] 包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_SlotCreate(u_long* pulSlotCreationEnabled,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int             ret;
	u_long	ulEnabled = TRUTHVALUE_FALSE;
	switch (reqinfo->mode) {
	case MODE_GET:
		 snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
			(u_char *)pulSlotCreationEnabled, sizeof(*pulSlotCreationEnabled));
		break;

	case MODE_SET_RESERVE1:
		ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
		if (ret != SNMP_ERR_NOERROR) {
			netsnmp_set_request_error(reqinfo, requests, ret);
		} else {
			ulEnabled = *(requests->requestvb->val.integer);
			if (ulEnabled!=TRUTHVALUE_TRUE && ulEnabled!=TRUTHVALUE_FALSE) {
				ret = SNMP_ERR_BADVALUE;
				netsnmp_set_request_error(reqinfo, requests, ret);
			}
		}
		break;

	case MODE_SET_RESERVE2:
	case MODE_SET_FREE:
	case MODE_SET_ACTION:
		break;

	case MODE_SET_COMMIT:
		*pulSlotCreationEnabled = *(requests->requestvb->val.integer);
		break;

	case MODE_SET_UNDO:
		return SNMP_ERR_UNDOFAILED;

	default:
		snmp_log(LOG_ERR,
			"unknown mode (%d) in %s\n",
			reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}

/*
*	函数名称: 	commhandle_Slot
*	函数功能: 	读写xxxSlot通用处理函数。
*	参数:		cObjName:		[in] xxxSlot对应的要创建的TR069节点。
*				pulSlot:		[in/out] xxxSlot变量指针。
*				pulSlotCreationEnabled:
*								[in/out] xxxSlotCreate变量指针。
*				reqinfo:		[in] 请求信息。
*				requests:		[in/out] 包含SNMP变量绑定的请求结构。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他SNMP错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int commhandle_Slot(const char* cObjName, u_long* pulSlot, u_long* pulCreationEnabled,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int             ret;
	int iRet = CMM_SUCCESS;
	switch (reqinfo->mode) {
	case MODE_GET:
		if (TRUTHVALUE_TRUE==*pulCreationEnabled) {/* 创建对象的功能已激活 */
			 *pulCreationEnabled = TRUTHVALUE_FALSE;	/* 关闭对象创建功能 */
			iRet = CMM_AddInstance(cObjName, pulSlot);	/* 创建对象，并返回新对象实例号 */
			if (iRet!=CMM_SUCCESS) {
				netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
				return SNMP_ERR_RESOURCEUNAVAILABLE;
			}
		}
		 snmp_set_var_typed_value(requests->requestvb, ASN_COUNTER,
			(u_char *)pulSlot, sizeof(*pulSlot));
		break;

	case MODE_SET_RESERVE1:
		ret = netsnmp_check_vb_type(requests->requestvb, ASN_COUNTER);
		if (ret != SNMP_ERR_NOERROR) {
			netsnmp_set_request_error(reqinfo, requests, ret);
		}
		break;

	case MODE_SET_RESERVE2:
	case MODE_SET_FREE:
	case MODE_SET_ACTION:
		break;

	case MODE_SET_COMMIT:
		*pulSlot = *(requests->requestvb->val.integer);
		break;

	case MODE_SET_UNDO:
		return SNMP_ERR_UNDOFAILED;

	default:
		snmp_log(LOG_ERR,
			"unknown mode (%d) in %s\n",
			reqinfo->mode, __func__);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}


#endif


/*
*	函数名称: 	CMM_GetStrColumn
*	函数功能: 	从CMM获取字符串列值。
*	参数:		cInstancePrefix:[in] 全对象实例名称。
*				cNode:			[in] 字段对应的TR069节点名。
*				cValue:			[out] 存放节点值的缓冲区。
*				uSize:			[in] 缓冲区大小。
*				pulLen:			[out] 实际获取到的字符串长度(不包括结尾的0字符)
*	返回值  :	CMM_SUCCESS - 成功
*				其他错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_GetStrColumn(const char* cInstancePrefix, const char* cNode,
	char* cValue, unsigned int uSize, size_t* pulLen, char **pv, int pvLen)
{
	char* pcBuff = NULL;
	int iRet = CMM_SUCCESS;
	char cNodeName[CMM_MAX_NODE_NAME];
	size_t iStrLen = 0;

	if (uSize==0) {
		return CMM_FAIL;
	}
	if(!cValue) return CMM_FAIL;
	memset(cValue, 0, uSize);

	/*
	* 注意:CMM_GetStr()要求值缓冲区大小包含0字符结尾，所以此处分配内存来
	* 保存包含0字符结尾的字符串。SNMP消息中字符串是没有0字符结尾的。
	*/
	pcBuff = (char*)malloc(uSize+1);
	if (!pcBuff) {
		iRet = CMM_ERR_MEM;
		CMMIF_TRACE("err: not enough memory!\n");
		goto EXIT1;
	}

	sprintf(cNodeName, "%s%s", cInstancePrefix, cNode);
	iRet = CMM_GetStr(cNodeName, pcBuff, uSize+1, pv, pvLen);
	if (iRet!=CMM_SUCCESS) {
		goto EXIT2;
	}
	iStrLen = strlen(pcBuff);
	if(pulLen) *pulLen = iStrLen;
	memcpy(cValue, pcBuff, iStrLen);

EXIT2:
	free(pcBuff);
EXIT1:
	return iRet;
}


/*
*	函数名称: 	CMM_SetStrColumn
*	函数功能: 	设置字符串列值到CMM。
*	参数:		cInstancePrefix:[in] 全对象实例名称。
*				cNode:			[in] 字段对应的TR069节点名。
*				cValue:			[in] 字符串列值。
*				uSize:			[in] 字符串长度(不包括结尾的0字符)
*	返回值  :	CMM_SUCCESS - 成功
*				其他错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_SetStrColumn(const char* cInstancePrefix, const char* cNode,
	const char* cValue, size_t uSize, char **pv, int pvLen)
{
	char* pcBuff = NULL;
	int iRet = CMM_SUCCESS;
	char cNodeName[CMM_MAX_NODE_NAME];

	/*
	* 注意:由SNMP消息传进来的字符串类型，不是0字符结尾的，所以
	* 此处需要拷贝出来，并加上0字符结尾，然后再调用CMM_SetStr()。
	*/
	pcBuff = (char*)malloc(uSize+1);
	if (!pcBuff) {
		iRet = CMM_ERR_MEM;
		CMMIF_TRACE("err: not enough memory!\n");
		goto EXIT1;
	}
	memcpy(pcBuff, cValue, uSize);
	pcBuff[uSize] = 0;

	sprintf(cNodeName, "%s%s", cInstancePrefix, cNode);
	iRet = CMM_SetStr(cNodeName, pcBuff, pv, pvLen);
	free(pcBuff);

EXIT1:
	return iRet;
}
#if 0
/*
*	函数名称: 	CMM_GetTruthColumn
*	函数功能: 	从CMM获取布尔型列值。
*	参数:		cInstancePrefix:[in] 全对象实例名称。
*				cNode:			[in] 字段对应的TR069节点名。
*				pulValue:		[out] 存放布尔型节点值的缓冲区。
*	返回值  :	CMM_SUCCESS - 成功
*				其他错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_GetTruthColumn(const char* cInstancePrefix, const char* cNode, u_long* pulValue)
{
	int iRet = CMM_SUCCESS;
	char cNodeName[CMM_MAX_NODE_NAME];
	char cNodeValue[256];
	*pulValue = TRUTHVALUE_FALSE;
	sprintf(cNodeName, "%s%s", cInstancePrefix, cNode);
	iRet = CMM_GetStr(cNodeName, cNodeValue, sizeof(cNodeValue));
	if (iRet!=CMM_SUCCESS) {
		return iRet;
	}
	if (tbsStrToBool(cNodeValue)) {
		*pulValue = TRUTHVALUE_TRUE;
	}
	return iRet;
}

/*
*	函数名称: 	CMM_SetTruthColumn
*	函数功能: 	设置布尔型列值到CMM。
*	参数:		cInstancePrefix:[in] 全对象实例名称。
*				cNode:			[in] 字段对应的TR069节点名。
*				ulValue:		[in] TruthValue值。
*	返回值  :	CMM_SUCCESS - 成功
*				其他错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_SetTruthColumn(const char* cInstancePrefix, const char* cNode,
	u_long ulValue)
{
	int iRet = CMM_SUCCESS;
	char cNodeName[CMM_MAX_NODE_NAME];

	sprintf(cNodeName, "%s%s", cInstancePrefix, cNode);
	if (ulValue==TRUTHVALUE_TRUE) {
		iRet = CMM_SetStr(cNodeName, "1");
	} else {
		iRet = CMM_SetStr(cNodeName, "0");
	}
	return iRet;
}
#endif
/*
*	函数名称: 	CMM_GetIntColumn
*	函数功能: 	从CMM获取整数型列值。
*	参数:		cInstancePrefix:[in] 全对象实例名称。
*				cNode:			[in] 字段对应的TR069节点名。
*				pulValue:		[out] 存放整数型节点值的缓冲区。
*	返回值  :	CMM_SUCCESS - 成功
*				其他错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_GetIntColumn(const char* cInstancePrefix, const char* cNode, u_long* pulValue, char **pv, int pvLen)
{
	int iRet;
	char cNodeName[CMM_MAX_NODE_NAME];
	char cNodeValue[256];

	*pulValue = 0;
	sprintf(cNodeName, "%s%s", cInstancePrefix, cNode);
	iRet = CMM_GetStr(cNodeName, cNodeValue, sizeof(cNodeValue), pv, pvLen);
	if (iRet!=CMM_SUCCESS) {
		return CMM_FAIL;
	}
	*pulValue = atoi(cNodeValue);
	return iRet;
}

/*
*	函数名称: 	CMM_SetIntColumn
*	函数功能: 	设置整数型列值到CMM。
*	参数:		cInstancePrefix:[in] 全对象实例名称。
*				cNode:			[in] 字段对应的TR069节点名。
*				ulValue:		[in] 整数值。
*	返回值  :	CMM_SUCCESS - 成功
*				其他错误代码。
*	创建    :	刘章平 / 2008-05-29
*/
int CMM_SetIntColumn(const char* cInstancePrefix, const char* cNode,
	u_long ulValue, char **pv, int pvLen)
{
	int iRet = CMM_SUCCESS;
	char cNodeName[CMM_MAX_NODE_NAME];
	char cNodeValue[256];

	sprintf(cNodeName, "%s%s", cInstancePrefix, cNode);

	sprintf(cNodeValue, "%lu", ulValue);
	iRet = CMM_SetStr(cNodeName, cNodeValue, pv, pvLen);
	return iRet;
}


#if 0
/*
*	函数名称: 	IsStrEnum
*	函数功能: 	检查值是否是枚举字符串之一。
*	参数:		szValue:[in] 要判断的字符串值
*				ulSize:	[in] 字符串值长度，不包括0字符结尾。
*				...:	[in] 字符串枚举值列表，以STRENUMEND结束。
*	返回值  :	1 - szValue是枚举字符串之一。
*				0 - szValue不是枚举字符串值之一。
*	创建    :	刘章平 / 2008-05-29
*/
int IsStrEnum(const char* szValue, u_long ulSize, ...)
{
	const char* pszEnum = STRENUMEND;
	va_list markers;
	va_start(markers, ulSize);
	if (!szValue) {
		return 0;
	}
	for (pszEnum = va_arg(markers, const char*);
		pszEnum!=STRENUMEND;
		pszEnum = va_arg(markers, const char*))
	{
		if (ulSize!=strlen(pszEnum)) {
			continue;
		}
		if (memcmp(szValue, pszEnum, ulSize)==0) {
			return 1;
		}
	}
	return 0;
}

/*
*	函数名称: 	IsIntEnum
*	函数功能: 	检查值是否是枚举整数值之一。
*	参数:		ulValue:[in] 要判断的整数值
*				...:	[in] 整数枚举值列表，以INTENUMEND结束。
*	返回值  :	1 - ulValue是枚举整数之一。
*				0 - ulValue不是枚举整数值之一。
*	创建    :	刘章平 / 2008-05-29
*/
int IsIntEnum(u_long ulValue, ...)
{
	u_long ulEnum = INTENUMEND;
	va_list markers;
	va_start(markers, ulValue);
	for (ulEnum = va_arg(markers, u_long);
		ulEnum!=INTENUMEND;
		ulEnum = va_arg(markers, u_long))
	{
		if (ulValue==ulEnum) {
			return 1;
		}
	}
	return 0;
}
#endif

/*****************************************************************************
*
* 辅助函数
*
*****************************************************************************/

/*
*	函数名称: 	GetFirstVapInstance
*	函数功能: 	获取配置树中当前第一个VAP实例名称。
*	参数:		void
*	返回值  :	NULL - 失败，或当前没有VAP实例
*				成功时返回实例的名称。使用完毕需要用free()函数释放。
*	创建    :	刘章平 / 2008-05-29
*/
char* GetFirstVapInstance()
{
	char* cFirstInstance = NULL;
	char** ppInstList = NULL;
	ppInstList = CMM_GetInstanceList(TR069_ROOT_VapTable);
	if (ppInstList) {
		if (ppInstList[0]) {
			cFirstInstance = strdup(ppInstList[0]);
		}
		CMM_FreeInstanceList(ppInstList);
	} else {
		CMMIF_TRACE("err: get VAP instance list '%s' failed!\n",
			TR069_ROOT_VapTable);
	}
	return cFirstInstance;
}

/*
*	函数名称: 	GetGlobalVapConfigName
*	函数功能: 	获取无线全局配置项全路径名称。
*	参数:		cNodeName	: [out] 返回全路径无线全局配置项名称。
*				cName		: [in] 配置项节点名(不带路径)
*	返回值  :	0 - 成功
*				其他失败
*	创建    :	刘章平 / 2008-05-29
*/
int GetGlobalVapConfigName(char* cNodeName, const char* cName)
{
	char* cFirstInstance;
	cFirstInstance = GetFirstVapInstance();
	if (!cFirstInstance) {
		CMMIF_TRACE("err: get first VAP instancefailed!\n");
		return 1;
	}
	sprintf(cNodeName, "%s%s", cFirstInstance, cName);
	free(cFirstInstance);
	return 0;
}


/*
功能:	从属性列表文件中读取指定属性的值。属性列表文件指内容类似如下形式的
		文件:
		name1: value1
		name2: value2
		name3: value3
		......

参数:	cFileName - [in] 属性列表文件
		cFeature - [in] 要读取的属性名
		cValue - [out] 保存属性值的缓冲区
		uSize - [in] 属性值缓冲区大小
返回:	0 - 成功
		其他 - 失败

*/
int GetFeature(const char* cFileName, const char* cFeature, char* cValue, size_t uSize)
{
	int ret = -1;
	FILE* fp = NULL;
	char cBuff[256];
	size_t uFeatureLen = strlen(cFeature);
	char* p = NULL;

	fp = fopen(cFileName, "r");
	if (!fp) {
		CMMIF_TRACE("err: open '%s' failed - %s!\n",
			cFileName, strerror(errno));
		return -1;
	}
	while (!feof(fp)) {
		if (!fgets(cBuff, sizeof(cBuff), fp)) {
			break;
		}
		if (strncmp(cFeature, cBuff, uFeatureLen)!=0) {
			continue;
		}
		/* 找到该Feature名称 */
		CMMIF_TRACE("msg: %s()--->%s",
			__func__, cBuff);
		p = cBuff+uFeatureLen;
		while (*p==':'||*p==' '||*p=='\t') {
			p++;
		}
		p = strtok(p, "\n\r");
		if (p) {
			if (strlen(p)<uSize) {
				strcpy(cValue, p);
				CMMIF_TRACE("msg: %s() success--->%s=%s\n",
					__func__, cFeature, cValue);
				ret = 0;
			} else {
				CMMIF_TRACE("err: buffer size %u too small!\n",
					uSize);
				ret = -1;
			}
		}
		break;
	}
	fclose(fp);
	return ret;
}

int GetFeatureInt(const char* cFileName, const char* cFeature, u_long* pulValue, int base)
{
	char cValue[256];
	char* pcEnd = NULL;
	int ret = GetFeature(cFileName, cFeature, cValue, sizeof(cValue));
	if (0!=ret) {
		return ret;
	}
	*pulValue = strtol(cValue, &pcEnd, base);
	return ret;
}

int GetProcInt(const char* path, u_long* pulValue, int base)
{
	int ret = 0;
	FILE* fp = NULL;
	char cBuff[256];
	fp = fopen(path, "r");
	if (!fp) {
		CMMIF_TRACE("err: open '%s' failed - %s!\n",
			path, strerror(errno));
		return -1;
	}
	if (fgets(cBuff, sizeof(cBuff), fp)) {
		char* pend = NULL;
		*pulValue = strtol(cBuff, &pend, base);
	} else {
		ret = -1;
	}
	fclose(fp);
	return ret;
}

/*
功能:	通过统计mtd文件内容，计算flash总容量。
参数:	void
返回:	flash总容量

mtd文件内容实例:
# cat  mtd
dev:    size   erasesize  name
mtd0: 00010000 00010000 "boot"
mtd1: 000d808c 00010000 "kernel"
mtd2: 004a7f74 00010000 "rootfs"
mtd3: 00240000 00010000 "jffs2"
mtd4: 00010000 00010000 "appconfig"
mtd5: 00020000 00010000 "llconfig"

*/
u_long FlashTotal()
{
	u_long ulTotal = 0;
	FILE* fp = NULL;
	char cBuff[256];
	const char* cFileName = "/proc/mtd";

	fp = fopen(cFileName, "r");
	if (!fp) {
		CMMIF_TRACE("err: open '%s' failed - %s!\n",
			cFileName, strerror(errno));
		return 0;
	}
	while (!feof(fp)) {
		char* pc = NULL;
		char* pend = NULL;
		if (!fgets(cBuff, sizeof(cBuff), fp)) {
			break;
		}
		pc = strtok(cBuff, ": ");	/* 跳过第一列 */
		pc = strtok(NULL, ": ");	/* size列 */
		if (!pc) {			/* 跳过空行 */
			continue;
		}
		if (strcmp(pc, "size")==0) {	/* 跳过表头 */
			continue;
		}
		ulTotal += strtol(pc, &pend, 16);
	}
	fclose(fp);
	CMMIF_TRACE("msg: Flash Capacity %lu\n", ulTotal);
	return ulTotal;
}


#define CPU_TICK_KIND  8

#if (CPU_TICK_KIND == 4)

#define CPU_TICK_FMT  "cpu %d %d %d %d"
#define CPU_TICK_ARG(ticks) &ticks[0], &ticks[1], &ticks[2], &ticks[3]

#elif (CPU_TICK_KIND == 8)

#define CPU_TICK_FMT  "cpu %d %d %d %d %d %d %d %d"
#define CPU_TICK_ARG(ticks) &ticks[0], &ticks[1], &ticks[2], &ticks[3], &ticks[4], &ticks[5], &ticks[6], &ticks[7]

#endif



int mib_get_cur_ticks(int *ticks)
{
	FILE *pfstat = NULL;
	char *stat_file = "/proc/stat";
	char file_buf[100] = {0};
	char *pbuf = NULL;

	pfstat = fopen(stat_file, "r");
	if (NULL == pfstat) {
		CMMIF_TRACE("err: open %s failed - %s!\n",
			stat_file, strerror(errno));
		return SNMP_ERR_GENERR;
	}

	pbuf = fgets(file_buf, sizeof(file_buf), pfstat);
	fclose(pfstat);
	if (NULL == pbuf) {
		CMMIF_TRACE("err: fgets %s failed - %s!\n",
			stat_file, strerror(errno));
		return SNMP_ERR_GENERR;
	}

	sscanf(file_buf, CPU_TICK_FMT, CPU_TICK_ARG(ticks));

	return SNMP_ERR_NOERROR;
}


int mib_get_cpu_occup(float *cpu_occup)
{
    int ret = 0;
    int ticks[2][CPU_TICK_KIND];
    int i = 0, j = 0;
    int total[2] = {0, 0};

    /*
   采样两次，中间相隔5秒
   */
    ret = mib_get_cur_ticks(ticks[0]);
    if (0 != ret) {
        CMMIF_TRACE("err: mib_get_cur_ticks failed : %08x\n", ret);
        return ret;
    }

    usleep(500000);

    ret = mib_get_cur_ticks(ticks[1]);
    if (0 != ret) {
        CMMIF_TRACE("err: mib_get_cur_ticks failed : %08x\n", ret);
        return ret;
    }

    /*
  在Linux/Unix下，CPU利用率分为用户态，系统态和空闲态，分别表示CPU处于用户态执行的
  时间，系统内核执行的时间，和空闲系统进程执行的时间。平时所说的CPU利用率是指：
  CPU执行非系统空闲进程的时间 / CPU总的执行时间。
  */
    for(j=0; j<2; j++)
    for (i=0; i<CPU_TICK_KIND; i++)
    {
        total[j] += ticks[j][i];
    }

    /* 第四个是空闲tick数 */
    *cpu_occup = 100.00 - ((ticks[1][3] - ticks[0][3]) * 100.00/(total[1] - total[0]));

    return SNMP_ERR_NOERROR;
}

/*
*	函数名称: 	GetVapTotalStats
*	函数功能: 	获取所有VAP收发数据量统计信息。
*	参数:		pulTotal	: [out] 返回所有VAP实例的统计量的和。
*				cName		: [in] 要获取的统计项名称，即TR069节点值。
*				ptLastUpdate: [in/out] 最后一次更新时间。如果没有达到
*								超时时间，不会去配置树查询。
*				reqinfo		: [in] SNMP请求信息。
*				requests	: [out] 请求数据。
*	返回值  :	SNMP_ERR_NOERROR - 成功
*				其他代表SNMP错误码失败
*	创建    :	刘章平 / 2008-05-29
*/
int GetVapTotalStats(u_long* pulTotal, const char* cNmae, time_t* ptLastUpdate)
{
	if (*ptLastUpdate==0) {
		*ptLastUpdate = time(NULL)-SNMP_CACHE_DEFAULT_TIMEOUT;
	}
	if (time(NULL)-*ptLastUpdate>=SNMP_CACHE_DEFAULT_TIMEOUT) {
		char** ppVapList = CMM_GetInstanceList(TR069_ROOT_VapTable);
		*pulTotal = 0;
		if (ppVapList) {
			int i;
			for (i = 0; ppVapList[i]; i++) {
				char cNodeName[CMM_MAX_NODE_NAME];
				int iRet = CMM_SUCCESS;
				char cValue[32];

				sprintf(cNodeName, "%s%s", ppVapList[i], cNmae);
				iRet = CMM_GetStr(cNodeName, cValue, sizeof(cValue), NULL, 0);
				if (CMM_SUCCESS == iRet) {
					*pulTotal += atoi(cValue);
				}
			}
			CMM_FreeInstanceList(ppVapList);
		}

		*ptLastUpdate = time(NULL);
	}

       return 0;
}

/*
功能:	对指定接口调用ifconfig命令，并分析输出，查找指定参数。
参数:	ifname - [in] 指定的接口名称，如ath0
		itemname - [in] 要查找的接口参数
		seperator - [in] 参数名和参数值的分隔字符集
		value - [out] 对有值的参数(比如MTU)，需要提供缓冲区来保存参数值，
				对没有参数名的(比如UP, BROADCAST等)，设置value为NULL
		size - [in] value缓冲区的大小。
返回:	0 - 成功
		其他 - 失败

ifconfig命令输出样例:
# ifconfig  ath0
ath0      Link encap:Ethernet  HWaddr 00:03:7F:06:81:93
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:9 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:0 (0.0 B)  TX bytes:1305 (1.2 KiB)
*/

int IfConfigRead(const char* ifname, const char *pszPrefix, const char* itemname, const char* seperator, char* value, size_t size)
{
	int ret = 0;
	FILE* fp = NULL;
	char cmd[64];
	char buff[256];
	char* pos = NULL;

	sprintf(cmd, "ifconfig %s", ifname);
	fp = popen(cmd, "r");
	if (!fp) {
		CMMIF_TRACE("err: open pipe of '%s' for read failed - %s!\n",
			cmd, strerror(errno));
		return -1;
	}

	ret = -1;		/* default not found */
	while (!feof(fp)) {
		if (!fgets(buff, sizeof(buff), fp)) {
			break;
		}
              //wei added it
              if(pszPrefix) 
              {/*to continue the followint strstr() marked 2, it must meet the condition in this embrace*/
                  char *tp = strstr(buff, pszPrefix);
                  if(!tp) continue;
              }
              //END wei added it
		pos = strstr(buff, itemname);   // 2 
		if (!pos) {
			continue;
		}
		ret = 0;
		CMMIF_TRACE("msg: found '%s' in '%s'.\n", itemname, buff);
		if (value && size) {
			pos += strlen(itemname);
			pos = strtok(pos, seperator);
			if (strlen(pos)<size) {
				strcpy(value, pos);
				break;
			} else {
				CMMIF_TRACE("err: buffer size too small!\n");
				ret = -1;
				break;
			}
		}
		break;
	}
	/* 把输出读完，然后关闭文件，否则所执行的命令有可能发生SIG_PIPE异常 */
	while (!feof(fp)) {
		if (!fgets(buff, sizeof(buff), fp)) {
			break;
		}
	}
	pclose(fp);
	return ret;
}


/*
功能:	通过brctl show br0命令，查询STP是否激活。
参数:	pulEnabled - [out] 返回TRUTHVALUE_TRUE代表激活，TRUTHVALUE_FALSE代表
					未激活
返回:	0 - 成功
		其他 - 失败Example of brctl show br0 output:

命令输出样例:
# brctl show br0
bridge name     bridge id               STP enabled     interfaces
br0             8000.00037f068193       no              eth0
                                                        ath0
                                                        ath1
*/
int GetStp(u_long* pulEnabled)
{
	int ret = 0;
	FILE* fp = NULL;
	char buff[256];
	char* pos = NULL;
	const char* br_name = "br0";
	char cmd[32];
	sprintf(cmd, "brctl show %s", br_name);
	fp = popen(cmd, "r");
	if (!fp) {
		CMMIF_TRACE("err: open pipe of '%s' for read failed - %s!\n",
			cmd, strerror(errno));
		return -1;
	}
	ret = -1;
	while (!feof(fp)) {
		if (!fgets(buff, sizeof(buff), fp)) {
			break;
		}
		if (strncmp(buff, br_name, strlen(br_name))!=0) {
			continue;
		}

		CMMIF_TRACE("msg: '%s' ---> %s\n",
			cmd, buff);

		pos = strtok(buff, " \t");	/* bridge name */
		pos = strtok(NULL, " \t");	/* bridge id */
		pos = strtok(NULL, " \t");	/* STP enabled */
		if (!pos) {
			CMMIF_TRACE("err: field 'STP enabled' is null!\n");
			ret = -1;
			break;
		}
		if (strcmp(pos, "no")==0) {
			*pulEnabled = TRUTHVALUE_FALSE;
			ret = 0;
		} else if (strcmp(pos, "yes")==0) {
			*pulEnabled = TRUTHVALUE_TRUE;
			ret = 0;
		} else {
			ret = -1;
			CMMIF_TRACE("err: field 'STP enabled' value invalid (%s)!\n", pos);
			break;
		}
		break;
	}
	/* 把输出读完，然后关闭文件，否则所执行的命令有可能发生SIG_PIPE异常 */
	while (!feof(fp)) {
		if (!fgets(buff, sizeof(buff), fp)) {
			break;
		}
	}
	pclose(fp);
	return ret;
}

int IsHexStr(char* value, u_long size)
{
	int i;
	if (!size) {
		return 0;
	}
	for (i = 0; i<size; i++) {
		if (!isxdigit(value[i])) {
			return 0;
		}
	}
	return 1;
}

/*
 ***************************************************************************
 *
 * LIST Handle Method
 *
 ***************************************************************************/
/*
  parameter: l -  pointer to the list in which data will be inserted
                   data - ponter to the data to be inserted
                   len - data size
                   refBytes - how many bytes to be compared
*/
void TW_addList (char *l, char *data, int len, int refBytes)
{
  lstList_t* list = (lstList_t*)l;

  // create a new node and the data that goes in it
  nwlNode = malloc(sizeof(struct lstNode));
  nwlNode->data = malloc(len);
  memcpy (nwlNode->data, data, len);

  // this deals with an empty list
  if( LIST_EMPTY ( list )) 
  {
    LIST_INSERT_HEAD (list, nwlNode, nodes);
    return;
  }

  // this deals with UIDs that match
  for(hdlNode=LIST_FIRST(list); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
  {
    if(memcmp(data, hdlNode->data, refBytes) == 0 ) // found matching UID
    {
      LIST_INSERT_AFTER (hdlNode, nwlNode, nodes);
      if (hdlNode->data) free(hdlNode->data);
      LIST_REMOVE (hdlNode, nodes);
      free (hdlNode);
      return;
    }
  }

  // this deals with inserting a new UID in the list
  for(hdlNode=LIST_FIRST(list); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes))
  {
    lstNode = hdlNode;
    if (memcmp(hdlNode->data, data, refBytes) > 0 ) // old ID > new ID AND
    {
      LIST_INSERT_BEFORE (hdlNode, nwlNode, nodes);
      return;
    }
  }

  // this deals with a UID that needs to go on the end of the list
  LIST_INSERT_AFTER (lstNode, nwlNode, nodes);

  return;
} 

void TW_flushList ( char *l )
{
    lstList_t*	list;
    list = (lstList_t*)l;

    while ( !LIST_EMPTY ( list ))
    {
        hdlNode = LIST_FIRST ( list );
        if ( hdlNode->data )
        {
            free ( hdlNode->data );
        }
        LIST_REMOVE ( hdlNode, nodes );
        free ( hdlNode );
    }
}

/* VAP对象监视信息 */
typedef struct vap_monitors_t_tag
{
    unsigned long instid;		/* 实例号 */
    char		ssid[32];			/* SSID */
    char		encrypt[32];		/* 基本加密模式 */
    unsigned long maxstations;	/* 最大允许终端数 */
    unsigned long curstations;	/* 当前接入终端数 */
} vap_monitors_t;
void TW_copyList( char *d, char *s, int dataLen )
{
    lstList_t *dList = (lstList_t*)d;
    lstList_t *sList = (lstList_t*)s;

    if(LIST_EMPTY(sList) || dataLen<1 ||!d || !s)
        return;

    for(hdlNode=LIST_FIRST(sList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        if(hdlNode->data)
        {
            nwlNode = malloc(sizeof(struct lstNode));
            nwlNode->data = malloc(dataLen);
            memcpy (nwlNode->data, hdlNode->data, dataLen);
            
            // this deals with an empty list
            if( LIST_EMPTY ( dList )) 
            {
                LIST_INSERT_HEAD (dList, nwlNode, nodes);
            }
            else LIST_INSERT_AFTER (lstNode, nwlNode, nodes);
            lstNode = nwlNode;
        }
    }
}

