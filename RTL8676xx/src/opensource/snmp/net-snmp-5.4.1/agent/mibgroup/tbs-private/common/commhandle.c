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
*	º¯ÊıÃû³Æ: 	commhandle_ReadOnlyStr
*	º¯Êı¹¦ÄÜ: 	Ö»¶Á×Ö·û´®Í¨ÓÃ´¦Àíº¯Êı£¬´ÓÅäÖÃÊ÷»ñÈ¡×Ö·û´®½ÚµãµÄÖµ£¬
*				²¢ÌîĞ´µ½requests½á¹¹ÖĞ¡£
*	²ÎÊı:		cNodename: 		[in]È«Â·¾¶µÄTR069½ÚµãÃû¡£
*				uMaxValueSize:	[in]ÓÃÓÚ±£´æ½ÚµãÖµµÄ»º³åÇøµÄ´óĞ¡¡£
*				reqinfo:		[in]¿ÇëÇóĞÅÏ¢¡£
*				requests:		[out]°üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	commhandle_ReadWriteStr
*	º¯Êı¹¦ÄÜ: 	¶ÁĞ´×Ö·û´®Í¨ÓÃ´¦Àíº¯Êı£¬´ÓÅäÖÃÊ÷»ñÈ¡×Ö·û´®½ÚµãµÄÖµ£¬
*				²¢ÌîĞ´µ½requests½á¹¹ÖĞ£¬»òÕßĞ´SNMP½ÚµãÖµµ½¶ÔÓ¦µÄ
*				TR069½Úµã¡£
*	ÊäÈë:		cNodename:		[in] È«Â·¾¶µÄTR069½ÚµãÃû¡£
*				uMaxValueSize:	[in] ÓÃÓÚ±£´æ½ÚµãÖµµÄ»º³åÇøµÄ´óĞ¡¡£
*				reqinfo:		[in] ÇëÇóĞÅÏ¢¡£
*				requests:		[in/out] °üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	commhandle_ReadOnlyLocalInt
*	º¯Êı¹¦ÄÜ: 	Ö»¶Á±¾µØÕûÊıÍ¨ÓÃ´¦Àíº¯Êı£¬´Ó½ø³ÌÄÚ²¿»ñÈ¡ÕûÊı±äÁ¿µÄÖµ£¬
*				²¢ÌîĞ´µ½requests½á¹¹ÖĞ¡£
*	²ÎÊı:		pulValue:	[in] Ö¸Ïòu_longÀàĞÍ±äÁ¿µÄÖ¸Õë¡£
*				ucIntType:	[in] ÕûÊıµÄASN±àÂëÀàĞÍ£¬Èç:ASN_INTEGER,ASN_COUNTERµÈ¡£
*				reqinfo:	[in] ÇëÇóĞÅÏ¢¡£
*	Êä³ö²ÎÊı:	requests:	[out] °üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	commhandle_ReadWriteLocalInt
*	º¯Êı¹¦ÄÜ: 	¶ÁĞ´±¾µØÕûÊıÍ¨ÓÃ´¦Àíº¯Êı£¬´Ó½ø³ÌÄÚ²¿»ñÈ¡ÕûÊı±äÁ¿µÄÖµ£¬
*				²¢ÌîĞ´µ½requests½á¹¹ÖĞ£¬»òÕßÉèÖÃSNMP½ÚµãÖµµ½¶ÔÓ¦µÄ
*				ÄÚ´æÕûÊı±äÁ¿ÖĞ¡£
*	²ÎÊı:		pulValue:	[in] Ö¸Ïòu_longÀàĞÍ±äÁ¿µÄÖ¸Õë¡£
*				ucIntType:	[in] ÕûÊıµÄASN±àÂëÀàĞÍ£¬Èç:ASN_INTEGER,ASN_COUNTERµÈ¡£
*				reqinfo:	[in] ÇëÇóĞÅÏ¢¡£
*	Êä³ö²ÎÊı:	requests:	[in/out] °üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	commhandle_ReadOnlyInt
*	º¯Êı¹¦ÄÜ: 	Ö»¶ÁÕûÊıÍ¨ÓÃ´¦Àíº¯Êı£¬´ÓÅäÖÃÊ÷»ñÈ¡ÕûÊı½ÚµãµÄÖµ£¬
*				²¢ÌîĞ´µ½requests½á¹¹ÖĞ¡£
*	²ÎÊı:		cNodename: 		[in]È«Â·¾¶µÄTR069½ÚµãÃû¡£
*				ucIntType:		[in]ASN±àÂëÕûÊıÀàĞÍ£¬Èç:ASN_INTEGER,ASN_COUNTER32¡£
*				reqinfo:		[in]ÇëÇóĞÅÏ¢¡£
*				requests:		[out]°üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	commhandle_ReadWriteInt
*	º¯Êı¹¦ÄÜ: 	¶ÁĞ´ÕûÊıÍ¨ÓÃ´¦Àíº¯Êı£¬GET²Ù×÷´ÓÅäÖÃÊ÷»ñÈ¡ÕûÊı½ÚµãµÄÖµ£¬
*				²¢ÌîĞ´µ½requests½á¹¹ÖĞ¡£SET²Ù×÷½«SNMPÉèÖÃµÄMIB½ÚµãÖµĞ´µ½Ö¸¶¨
*				µÄTR069½Úµã¡£
*	²ÎÊı:		cNodename: 		[in]È«Â·¾¶µÄTR069½ÚµãÃû¡£
*				ucIntType:		[in]ASN±àÂëÕûÊıÀàĞÍ£¬Èç:ASN_INTEGER,ASN_COUNTER32¡£
*				reqinfo:		[in]ÇëÇóĞÅÏ¢¡£
*				requests:		[in/out]°üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	commhandle_ReadOnlyTruthValue
*	º¯Êı¹¦ÄÜ: 	Ö»¶ÁTruthValueÍ¨ÓÃ´¦Àíº¯Êı£¬´ÓÅäÖÃÊ÷»ñÈ¡boolean½ÚµãµÄÖµ£¬
*				²¢ÌîĞ´µ½requests½á¹¹ÖĞ¡£
*	²ÎÊı:		cNodename: 		[in]È«Â·¾¶µÄTR069½ÚµãÃû¡£
*				reqinfo:		[in]ÇëÇóĞÅÏ¢¡£
*				requests:		[out]°üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	commhandle_ReadWriteTruthValue
*	º¯Êı¹¦ÄÜ: 	¶ÁĞ´TruthValueÍ¨ÓÃ´¦Àíº¯Êı¡£GET²Ù×÷´ÓÅäÖÃÊ÷»ñÈ¡boolean½ÚµãµÄÖµ
*				²¢ÌîĞ´µ½requests½á¹¹ÖĞ¡£SET²Ù×÷½«requestsÖĞµÄÖµĞ´Èëµ½ÅäÖÃÊ÷Ö¸¶¨
*				½Úµã¡£
*	²ÎÊı:		cNodename: 		[in]È«Â·¾¶µÄTR069½ÚµãÃû¡£
*				reqinfo:		[in]ÇëÇóĞÅÏ¢¡£
*				requests:		[in/out]°üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	commhandle_SlotCreate
*	º¯Êı¹¦ÄÜ: 	¶ÁĞ´xxxSlotCreateÍ¨ÓÃ´¦Àíº¯Êı¡£
*	²ÎÊı:		pulSlotCreationEnabled:
*								[in/out] xxxSlotCreate±äÁ¿Ö¸Õë¡£
*				reqinfo:		[in] ÇëÇóĞÅÏ¢¡£
*				requests:		[in/out] °üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	commhandle_Slot
*	º¯Êı¹¦ÄÜ: 	¶ÁĞ´xxxSlotÍ¨ÓÃ´¦Àíº¯Êı¡£
*	²ÎÊı:		cObjName:		[in] xxxSlot¶ÔÓ¦µÄÒª´´½¨µÄTR069½Úµã¡£
*				pulSlot:		[in/out] xxxSlot±äÁ¿Ö¸Õë¡£
*				pulSlotCreationEnabled:
*								[in/out] xxxSlotCreate±äÁ¿Ö¸Õë¡£
*				reqinfo:		[in] ÇëÇóĞÅÏ¢¡£
*				requests:		[in/out] °üº¬SNMP±äÁ¿°ó¶¨µÄÇëÇó½á¹¹¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËûSNMP´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
*/
int commhandle_Slot(const char* cObjName, u_long* pulSlot, u_long* pulCreationEnabled,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
	int             ret;
	int iRet = CMM_SUCCESS;
	switch (reqinfo->mode) {
	case MODE_GET:
		if (TRUTHVALUE_TRUE==*pulCreationEnabled) {/* ´´½¨¶ÔÏóµÄ¹¦ÄÜÒÑ¼¤»î */
			 *pulCreationEnabled = TRUTHVALUE_FALSE;	/* ¹Ø±Õ¶ÔÏó´´½¨¹¦ÄÜ */
			iRet = CMM_AddInstance(cObjName, pulSlot);	/* ´´½¨¶ÔÏó£¬²¢·µ»ØĞÂ¶ÔÏóÊµÀıºÅ */
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
*	º¯ÊıÃû³Æ: 	CMM_GetStrColumn
*	º¯Êı¹¦ÄÜ: 	´ÓCMM»ñÈ¡×Ö·û´®ÁĞÖµ¡£
*	²ÎÊı:		cInstancePrefix:[in] È«¶ÔÏóÊµÀıÃû³Æ¡£
*				cNode:			[in] ×Ö¶Î¶ÔÓ¦µÄTR069½ÚµãÃû¡£
*				cValue:			[out] ´æ·Å½ÚµãÖµµÄ»º³åÇø¡£
*				uSize:			[in] »º³åÇø´óĞ¡¡£
*				pulLen:			[out] Êµ¼Ê»ñÈ¡µ½µÄ×Ö·û´®³¤¶È(²»°üÀ¨½áÎ²µÄ0×Ö·û)
*	·µ»ØÖµ  :	CMM_SUCCESS - ³É¹¦
*				ÆäËû´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
	* ×¢Òâ:CMM_GetStr()ÒªÇóÖµ»º³åÇø´óĞ¡°üº¬0×Ö·û½áÎ²£¬ËùÒÔ´Ë´¦·ÖÅäÄÚ´æÀ´
	* ±£´æ°üº¬0×Ö·û½áÎ²µÄ×Ö·û´®¡£SNMPÏûÏ¢ÖĞ×Ö·û´®ÊÇÃ»ÓĞ0×Ö·û½áÎ²µÄ¡£
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
*	º¯ÊıÃû³Æ: 	CMM_SetStrColumn
*	º¯Êı¹¦ÄÜ: 	ÉèÖÃ×Ö·û´®ÁĞÖµµ½CMM¡£
*	²ÎÊı:		cInstancePrefix:[in] È«¶ÔÏóÊµÀıÃû³Æ¡£
*				cNode:			[in] ×Ö¶Î¶ÔÓ¦µÄTR069½ÚµãÃû¡£
*				cValue:			[in] ×Ö·û´®ÁĞÖµ¡£
*				uSize:			[in] ×Ö·û´®³¤¶È(²»°üÀ¨½áÎ²µÄ0×Ö·û)
*	·µ»ØÖµ  :	CMM_SUCCESS - ³É¹¦
*				ÆäËû´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
*/
int CMM_SetStrColumn(const char* cInstancePrefix, const char* cNode,
	const char* cValue, size_t uSize, char **pv, int pvLen)
{
	char* pcBuff = NULL;
	int iRet = CMM_SUCCESS;
	char cNodeName[CMM_MAX_NODE_NAME];

	/*
	* ×¢Òâ:ÓÉSNMPÏûÏ¢´«½øÀ´µÄ×Ö·û´®ÀàĞÍ£¬²»ÊÇ0×Ö·û½áÎ²µÄ£¬ËùÒÔ
	* ´Ë´¦ĞèÒª¿½±´³öÀ´£¬²¢¼ÓÉÏ0×Ö·û½áÎ²£¬È»ºóÔÙµ÷ÓÃCMM_SetStr()¡£
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
*	º¯ÊıÃû³Æ: 	CMM_GetTruthColumn
*	º¯Êı¹¦ÄÜ: 	´ÓCMM»ñÈ¡²¼¶ûĞÍÁĞÖµ¡£
*	²ÎÊı:		cInstancePrefix:[in] È«¶ÔÏóÊµÀıÃû³Æ¡£
*				cNode:			[in] ×Ö¶Î¶ÔÓ¦µÄTR069½ÚµãÃû¡£
*				pulValue:		[out] ´æ·Å²¼¶ûĞÍ½ÚµãÖµµÄ»º³åÇø¡£
*	·µ»ØÖµ  :	CMM_SUCCESS - ³É¹¦
*				ÆäËû´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	CMM_SetTruthColumn
*	º¯Êı¹¦ÄÜ: 	ÉèÖÃ²¼¶ûĞÍÁĞÖµµ½CMM¡£
*	²ÎÊı:		cInstancePrefix:[in] È«¶ÔÏóÊµÀıÃû³Æ¡£
*				cNode:			[in] ×Ö¶Î¶ÔÓ¦µÄTR069½ÚµãÃû¡£
*				ulValue:		[in] TruthValueÖµ¡£
*	·µ»ØÖµ  :	CMM_SUCCESS - ³É¹¦
*				ÆäËû´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	CMM_GetIntColumn
*	º¯Êı¹¦ÄÜ: 	´ÓCMM»ñÈ¡ÕûÊıĞÍÁĞÖµ¡£
*	²ÎÊı:		cInstancePrefix:[in] È«¶ÔÏóÊµÀıÃû³Æ¡£
*				cNode:			[in] ×Ö¶Î¶ÔÓ¦µÄTR069½ÚµãÃû¡£
*				pulValue:		[out] ´æ·ÅÕûÊıĞÍ½ÚµãÖµµÄ»º³åÇø¡£
*	·µ»ØÖµ  :	CMM_SUCCESS - ³É¹¦
*				ÆäËû´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	CMM_SetIntColumn
*	º¯Êı¹¦ÄÜ: 	ÉèÖÃÕûÊıĞÍÁĞÖµµ½CMM¡£
*	²ÎÊı:		cInstancePrefix:[in] È«¶ÔÏóÊµÀıÃû³Æ¡£
*				cNode:			[in] ×Ö¶Î¶ÔÓ¦µÄTR069½ÚµãÃû¡£
*				ulValue:		[in] ÕûÊıÖµ¡£
*	·µ»ØÖµ  :	CMM_SUCCESS - ³É¹¦
*				ÆäËû´íÎó´úÂë¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	IsStrEnum
*	º¯Êı¹¦ÄÜ: 	¼ì²éÖµÊÇ·ñÊÇÃ¶¾Ù×Ö·û´®Ö®Ò»¡£
*	²ÎÊı:		szValue:[in] ÒªÅĞ¶ÏµÄ×Ö·û´®Öµ
*				ulSize:	[in] ×Ö·û´®Öµ³¤¶È£¬²»°üÀ¨0×Ö·û½áÎ²¡£
*				...:	[in] ×Ö·û´®Ã¶¾ÙÖµÁĞ±í£¬ÒÔSTRENUMEND½áÊø¡£
*	·µ»ØÖµ  :	1 - szValueÊÇÃ¶¾Ù×Ö·û´®Ö®Ò»¡£
*				0 - szValue²»ÊÇÃ¶¾Ù×Ö·û´®ÖµÖ®Ò»¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	IsIntEnum
*	º¯Êı¹¦ÄÜ: 	¼ì²éÖµÊÇ·ñÊÇÃ¶¾ÙÕûÊıÖµÖ®Ò»¡£
*	²ÎÊı:		ulValue:[in] ÒªÅĞ¶ÏµÄÕûÊıÖµ
*				...:	[in] ÕûÊıÃ¶¾ÙÖµÁĞ±í£¬ÒÔINTENUMEND½áÊø¡£
*	·µ»ØÖµ  :	1 - ulValueÊÇÃ¶¾ÙÕûÊıÖ®Ò»¡£
*				0 - ulValue²»ÊÇÃ¶¾ÙÕûÊıÖµÖ®Ò»¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
* ¸¨Öúº¯Êı
*
*****************************************************************************/

/*
*	º¯ÊıÃû³Æ: 	GetFirstVapInstance
*	º¯Êı¹¦ÄÜ: 	»ñÈ¡ÅäÖÃÊ÷ÖĞµ±Ç°µÚÒ»¸öVAPÊµÀıÃû³Æ¡£
*	²ÎÊı:		void
*	·µ»ØÖµ  :	NULL - Ê§°Ü£¬»òµ±Ç°Ã»ÓĞVAPÊµÀı
*				³É¹¦Ê±·µ»ØÊµÀıµÄÃû³Æ¡£Ê¹ÓÃÍê±ÏĞèÒªÓÃfree()º¯ÊıÊÍ·Å¡£
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
*	º¯ÊıÃû³Æ: 	GetGlobalVapConfigName
*	º¯Êı¹¦ÄÜ: 	»ñÈ¡ÎŞÏßÈ«¾ÖÅäÖÃÏîÈ«Â·¾¶Ãû³Æ¡£
*	²ÎÊı:		cNodeName	: [out] ·µ»ØÈ«Â·¾¶ÎŞÏßÈ«¾ÖÅäÖÃÏîÃû³Æ¡£
*				cName		: [in] ÅäÖÃÏî½ÚµãÃû(²»´øÂ·¾¶)
*	·µ»ØÖµ  :	0 - ³É¹¦
*				ÆäËûÊ§°Ü
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
¹¦ÄÜ:	´ÓÊôĞÔÁĞ±íÎÄ¼şÖĞ¶ÁÈ¡Ö¸¶¨ÊôĞÔµÄÖµ¡£ÊôĞÔÁĞ±íÎÄ¼şÖ¸ÄÚÈİÀàËÆÈçÏÂĞÎÊ½µÄ
		ÎÄ¼ş:
		name1: value1
		name2: value2
		name3: value3
		......

²ÎÊı:	cFileName - [in] ÊôĞÔÁĞ±íÎÄ¼ş
		cFeature - [in] Òª¶ÁÈ¡µÄÊôĞÔÃû
		cValue - [out] ±£´æÊôĞÔÖµµÄ»º³åÇø
		uSize - [in] ÊôĞÔÖµ»º³åÇø´óĞ¡
·µ»Ø:	0 - ³É¹¦
		ÆäËû - Ê§°Ü

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
		/* ÕÒµ½¸ÃFeatureÃû³Æ */
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
¹¦ÄÜ:	Í¨¹ıÍ³¼ÆmtdÎÄ¼şÄÚÈİ£¬¼ÆËãflash×ÜÈİÁ¿¡£
²ÎÊı:	void
·µ»Ø:	flash×ÜÈİÁ¿

mtdÎÄ¼şÄÚÈİÊµÀı:
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
		pc = strtok(cBuff, ": ");	/* Ìø¹ıµÚÒ»ÁĞ */
		pc = strtok(NULL, ": ");	/* sizeÁĞ */
		if (!pc) {			/* Ìø¹ı¿ÕĞĞ */
			continue;
		}
		if (strcmp(pc, "size")==0) {	/* Ìø¹ı±íÍ· */
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
   ²ÉÑùÁ½´Î£¬ÖĞ¼äÏà¸ô5Ãë
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
  ÔÚLinux/UnixÏÂ£¬CPUÀûÓÃÂÊ·ÖÎªÓÃ»§Ì¬£¬ÏµÍ³Ì¬ºÍ¿ÕÏĞÌ¬£¬·Ö±ğ±íÊ¾CPU´¦ÓÚÓÃ»§Ì¬Ö´ĞĞµÄ
  Ê±¼ä£¬ÏµÍ³ÄÚºËÖ´ĞĞµÄÊ±¼ä£¬ºÍ¿ÕÏĞÏµÍ³½ø³ÌÖ´ĞĞµÄÊ±¼ä¡£Æ½Ê±ËùËµµÄCPUÀûÓÃÂÊÊÇÖ¸£º
  CPUÖ´ĞĞ·ÇÏµÍ³¿ÕÏĞ½ø³ÌµÄÊ±¼ä / CPU×ÜµÄÖ´ĞĞÊ±¼ä¡£
  */
    for(j=0; j<2; j++)
    for (i=0; i<CPU_TICK_KIND; i++)
    {
        total[j] += ticks[j][i];
    }

    /* µÚËÄ¸öÊÇ¿ÕÏĞtickÊı */
    *cpu_occup = 100.00 - ((ticks[1][3] - ticks[0][3]) * 100.00/(total[1] - total[0]));

    return SNMP_ERR_NOERROR;
}

/*
*	º¯ÊıÃû³Æ: 	GetVapTotalStats
*	º¯Êı¹¦ÄÜ: 	»ñÈ¡ËùÓĞVAPÊÕ·¢Êı¾İÁ¿Í³¼ÆĞÅÏ¢¡£
*	²ÎÊı:		pulTotal	: [out] ·µ»ØËùÓĞVAPÊµÀıµÄÍ³¼ÆÁ¿µÄºÍ¡£
*				cName		: [in] Òª»ñÈ¡µÄÍ³¼ÆÏîÃû³Æ£¬¼´TR069½ÚµãÖµ¡£
*				ptLastUpdate: [in/out] ×îºóÒ»´Î¸üĞÂÊ±¼ä¡£Èç¹ûÃ»ÓĞ´ïµ½
*								³¬Ê±Ê±¼ä£¬²»»áÈ¥ÅäÖÃÊ÷²éÑ¯¡£
*				reqinfo		: [in] SNMPÇëÇóĞÅÏ¢¡£
*				requests	: [out] ÇëÇóÊı¾İ¡£
*	·µ»ØÖµ  :	SNMP_ERR_NOERROR - ³É¹¦
*				ÆäËû´ú±íSNMP´íÎóÂëÊ§°Ü
*	´´½¨    :	ÁõÕÂÆ½ / 2008-05-29
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
¹¦ÄÜ:	¶ÔÖ¸¶¨½Ó¿Úµ÷ÓÃifconfigÃüÁî£¬²¢·ÖÎöÊä³ö£¬²éÕÒÖ¸¶¨²ÎÊı¡£
²ÎÊı:	ifname - [in] Ö¸¶¨µÄ½Ó¿ÚÃû³Æ£¬Èçath0
		itemname - [in] Òª²éÕÒµÄ½Ó¿Ú²ÎÊı
		seperator - [in] ²ÎÊıÃûºÍ²ÎÊıÖµµÄ·Ö¸ô×Ö·û¼¯
		value - [out] ¶ÔÓĞÖµµÄ²ÎÊı(±ÈÈçMTU)£¬ĞèÒªÌá¹©»º³åÇøÀ´±£´æ²ÎÊıÖµ£¬
				¶ÔÃ»ÓĞ²ÎÊıÃûµÄ(±ÈÈçUP, BROADCASTµÈ)£¬ÉèÖÃvalueÎªNULL
		size - [in] value»º³åÇøµÄ´óĞ¡¡£
·µ»Ø:	0 - ³É¹¦
		ÆäËû - Ê§°Ü

ifconfigÃüÁîÊä³öÑùÀı:
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
	/* °ÑÊä³ö¶ÁÍê£¬È»ºó¹Ø±ÕÎÄ¼ş£¬·ñÔòËùÖ´ĞĞµÄÃüÁîÓĞ¿ÉÄÜ·¢ÉúSIG_PIPEÒì³£ */
	while (!feof(fp)) {
		if (!fgets(buff, sizeof(buff), fp)) {
			break;
		}
	}
	pclose(fp);
	return ret;
}


/*
¹¦ÄÜ:	Í¨¹ıbrctl show br0ÃüÁî£¬²éÑ¯STPÊÇ·ñ¼¤»î¡£
²ÎÊı:	pulEnabled - [out] ·µ»ØTRUTHVALUE_TRUE´ú±í¼¤»î£¬TRUTHVALUE_FALSE´ú±í
					Î´¼¤»î
·µ»Ø:	0 - ³É¹¦
		ÆäËû - Ê§°ÜExample of brctl show br0 output:

ÃüÁîÊä³öÑùÀı:
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
	/* °ÑÊä³ö¶ÁÍê£¬È»ºó¹Ø±ÕÎÄ¼ş£¬·ñÔòËùÖ´ĞĞµÄÃüÁîÓĞ¿ÉÄÜ·¢ÉúSIG_PIPEÒì³£ */
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

/* VAP¶ÔÏó¼àÊÓĞÅÏ¢ */
typedef struct vap_monitors_t_tag
{
    unsigned long instid;		/* ÊµÀıºÅ */
    char		ssid[32];			/* SSID */
    char		encrypt[32];		/* »ù±¾¼ÓÃÜÄ£Ê½ */
    unsigned long maxstations;	/* ×î´óÔÊĞíÖÕ¶ËÊı */
    unsigned long curstations;	/* µ±Ç°½ÓÈëÖÕ¶ËÊı */
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

