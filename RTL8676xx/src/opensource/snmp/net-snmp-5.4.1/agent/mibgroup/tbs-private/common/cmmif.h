/* cmmif.h */

#ifndef __cmmif_h__
#define __cmmif_h__

#ifdef __cplusplus
extern          "C" {
#endif

#include "tbsmsg.h"
#include "tbserror.h"
#include "cfg_api.h"
#include "pc_api.h"
#include "tbsutil.h"
#include "warnlog.h"
#include "common.h"

#define CMM_INIT_NONE			0
#define CMM_INIT_SUCCESS		1
#define CMM_INIT_FAILED		2

//#define CMM_MAX_NODE_NAME	1024 wei changed to==>
#define CMM_MAX_NODE_NAME	264

#define CMM_SUCCESS	MSG_OK
#define CMM_FAIL		(MSG_OK+1)
#define CMM_ERR_SEND	(MSG_OK+2)
#define CMM_ERR_RECV	(MSG_OK+3)
#define CMM_ERR_TIMEOUT	(MSG_OK+4)
#define CMM_ERR_MEM	(MSG_OK+5)

/*
#define CMMIF_DEBUG	1
*/
#ifdef CMMIF_DEBUG
#define CMMIF_TRACE(fmt, args...) \
	printf("%s:%s:%d "fmt,  __FILE__, __func__, __LINE__, ##args)
#else
#define CMMIF_TRACE(fmt, args...) ((void)(fmt))
#endif

/*
 * 在init_MIBMODULE()中调用CMM_Init()
 */
int CMM_Init();
void CMM_Free();

int SNMP_ReadEventFromFlash(char* szReadBuf);
int SNMP_SaveEventsToFlash(char *szEventValue);

int CMM_GetStr(const char* szLeafName, char* szValue, unsigned int uSize, char **pv, int pvLen);
int CMM_SetStr(const char* szLeafName, const char* szValue, char **pv, int pvLen);
int CMM_SetStr_ForMultiLeaves(const char* szLeavesName, unsigned long ulLeavesLen, int iLeavesNum);
int CMM_GetUlong(const char* szLeafName, u_long* puValue, char **pv, int pvLen);
int CMM_SetUlong(const char* szLeafName, u_long uValue, char **pv, int pvLen);


char** CMM_GetInstanceList(const char* cNodeName);
void CMM_FreeInstanceList(char** ppList);

int CMM_AddInstance(const char* cNodeName, unsigned long* pulInstanceId);
int CMM_DelInstance(const char* cInstanceName);

int CMM_SaveConfig();
int CMM_RecoverConfig();
int CMM_Reboot();

#define LINKUP_REF_NUM 4
int CMM_GetZTE_LinkupRef(char **ref);


#define SNMPA_PRIVATE_DEBUG
#ifdef SNMPA_PRIVATE_DEBUG
//#define SNMPA_TRACE(fmt, args...) TBS_TRACE(MID_SNMPA, "[%s %s %d] "fmt, __FILE__, __func__, __LINE__, ##args)
//#define SNMAP_TRACE(fmt, args...) syslog(LOG_INFO, "[%s %s %d] "fmt, __FILE__, __func__, __LINE__, ##args);
//#define SNMPA_TRACE(fmt, args...) syslog(LOG_INFO, fmt, ##args)
#define SNMPA_TRACE(fmt, args...) printf("%s %s %d "fmt, __FILE__, __func__, __LINE__, ##args);
#else
#define SNMAP_TRACE(fmt, args...) do{;} while(0);
#endif

unsigned char *CHARS_TO_intIPADDR(const char *ipaddr);
char *intIPAddr_TO_CHARS(char *pszIpAddr, unsigned long netSortIntIpaddr);

/*twoboundariesarray must be char *[] array*/
#define GET_ARRAY_BOUNADRY(twoboundariesarray) sizeof(twoboundariesarray)/sizeof(twoboundariesarray[0])

int GetActiveWanConnDevice(char* cIfName, size_t uSize, unsigned long* pulInstId);

#ifdef __cplusplus
}
#endif

#endif

