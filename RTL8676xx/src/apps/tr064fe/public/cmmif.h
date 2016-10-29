/* cmmif.h */

#ifndef __cmmif_h__
#define __cmmif_h__

#ifdef __cplusplus
extern          "C" {
#endif

#include "upnp.h"
#include "tbsmsg.h"
#include "tbserror.h"
#include "cfg_api.h"
#include "pc_api.h"
#include "tbsutil.h"
#include "warnlog.h"
#include "common.h"
#include "tr64utils.h"

#define CMM_INIT_NONE			0
#define CMM_INIT_SUCCESS		1
#define CMM_INIT_FAILED		2

#define CMM_MAX_NODE_LEN	128
#define CMM_MAX_LEAF_LEN	40

#define CMM_SUCCESS	MSG_OK
#define CMM_FAIL		(MSG_OK+1)
#define CMM_ERR_SEND	(MSG_OK+2)
#define CMM_ERR_RECV	(MSG_OK+3)
#define CMM_ERR_TIMEOUT	(MSG_OK+4)
#define CMM_ERR_MEM	(MSG_OK+5)

/* 公共TR069节点名称和路径名称 */
#define TR064_ROOT_LanDevice 	"InternetGatewayDevice.LANDevice."
#define TR064_ROOT_LanHostMgmt 	"InternetGatewayDevice.LANDevice.1.LANHostConfigManagement."
#define TR064_ROOT_LanEthIf 	"InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig."
#define TR064_ROOT_WLanCfg 	    "InternetGatewayDevice.LANDevice.1.WLANConfiguration."
#define TR064_ROOT_WANDSLIfCfg 	"InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig."
#define TR064_ROOT_LanUSBIfCfg 	"InternetGatewayDevice.LANDevice.1.LANUSBInterfaceConfig."
#define TR064_ROOT_LanHostCfg 	"InternetGatewayDevice.LANDevice.1.Hosts."
#define TR064_ROOT_LANCfgSec    "InternetGatewayDevice.LANConfigSecurity."
 

#define TR064_ROOT_WANEthLinkCfg "InternetGatewayDevice.WANDevice.1.hosts."
#define TR064_ROOT_DeviceInfo "InternetGatewayDevice.DeviceInfo."

#define TR064_ROOT_L2BrdgCfg 	"InternetGatewayDevice.Layer2Bridging."
#define TR064_ROOT_L2BragIfCfg  "InternetGatewayDevice.Layer2Bridging.AvailableInterface."
#define TR064_ROOT_WanDevice 	"InternetGatewayDevice.WANDevice."
#define TR064_ROOT_WanWANCommonInterface 	"InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig."
#define TR064_ROOT_Layer3Forwarding    "InternetGatewayDevice.Layer3Forwarding."
#define TR064_ROOT_RouteInfo "InternetGatewayDevice.X_TWSZ-COM_RouteInfo."
#define TR064_ROOT_WanConnectionDevice 	"InternetGatewayDevice.WANDevice.1.WANConnectionDevice."

#define TR064_ROOT_ManagementServer 	"InternetGatewayDevice.ManagementServer."

#define TR064_ROOT_TimeCfg 	    "InternetGatewayDevice.Time."

#define TR064_ROOT_QueueMngCfg 	 "InternetGatewayDevice.QueueManagement."

typedef struct tag_ST_TR064STR_LINK
{
    char *pstrValue;
    struct tag_ST_TR064STR_LINK *pNext;
} ST_TR064STR_LINK;

void TR064_DestroyStrLink(ST_TR064STR_LINK *pHead);

/*
 * 在init_MIBMODULE()中调用CMM_Init()
 */
int CMM_Init();
void CMM_Free();

int TR064_PostFactoryReset();
int TR064_PostReboot();

int CMM_GetStr(const char* szLeafName, char* szValue, unsigned int uSize, char **pv, int pvLen);
ST_TR064STR_LINK* CMM_GetStrs(const char *szPathName, char* aszLeafName[],int nValNum);
ST_TR064STR_LINK* CMM_FullPathGetStrs(char* aszFullName[],int nValNum);

int CMM_SetStr(const char* szLeafName, char* szValue, char **pv, int pvLen);
int CMM_SetStrs(char* ppcPathName[], char* ppcPathValue[],int nValNum);

int CMM_GetUlong(const char* szLeafName, u_long* puValue, char **pv, int pvLen);
int CMM_SetUlong(const char* szLeafName, u_long uValue, char **pv, int pvLen);

char** CMM_GetInstanceList(const char* cNodeName);
void CMM_FreeInstanceList(char** ppList);

int CMM_AddInstance(const char* cNodeName, unsigned long* pulInstanceId);
int CMM_DelInstance(const char* cInstanceName);

int CMM_SaveConfig();
int CMM_RecoverConfig();
int CMM_Reboot();

unsigned int *CHARS_TO_intIP(const char *ipaddr);
unsigned char *CHARS_TO_intIPADDR(const char *ipaddr);
char *intIPAddr_TO_CHARS(char *pszIpAddr, unsigned long netSortIntIpaddr);

#define SetTr64Value(szLeafName,relatedVar) \
do{ \
   struct Param *pParams;\
   pParams = findActionParamByRelatedVar(ac,relatedVar); \
   if (pParams != NULL) \
   { \
       int nRet = CMM_SetStr(szLeafName, pParams->value, NULL, 0);\
	   if(nRet != CMM_SUCCESS)\
	   { \
	       soap_error( uclient, SOAP_ACTIONFAILED ); \
	       TR64FE_TRACE("set of %s failed\n",szLeafName); \
	       return FALSE; \
	   } \
   } \
   else \
   { \
      soap_error( uclient, SOAP_ACTIONFAILED ); \
      return FALSE; \
   } \
   return TRUE; \
}while(0)


#define SetTr64Values(szLeafNames,relatedVars ,nNumVar) \
do{ \
   char* szLeafValues[nNumVar] ={0}; \
   struct Param *pParams;\
   int i = 0 ; \
   for( ; i< nNumVar ;i++) \
   {\
      pParams = findActionParamByRelatedVar(ac,relatedVars[i]); \
	  if (pParams != NULL) \
	  {\
	      szLeafValues[i] = pParams->value ;\
	  } \
	  else \
	  { \
	     soap_error( uclient, SOAP_ACTIONFAILED ); \
	     return FALSE; \
	  } \
   } \
   int nRet = CMM_SetStrs(szLeafNames,szLeafValues, nNumVar) ; \
   if(nRet != CMM_SUCCESS)\
   { \
      TR64FE_TRACE("set of %s failed", szLeafNames[0] ); \
      soap_error( uclient, SOAP_ACTIONFAILED ); \
      return FALSE; \
   } \
   return TRUE; \
}while(0)


#ifdef __cplusplus
}
#endif

#endif

