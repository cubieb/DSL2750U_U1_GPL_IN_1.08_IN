#ifndef _SNMPPROCESS_H_
#define _SNMPPROCESS_H_

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

/*1：版本文件; 2：配置文件; 3：日志文件*/
enum FILE_TYPE
{
    ZERO,
    IMG_FILE,
    CONFIG_FILE,
    LOG_FILE
};
enum ACTION_TYPE
{
    EMPTY,
    DOWNLOAD,
    UPLOAD
};

typedef struct traps_info_tag
{    
    char *trap_msg;    
    unsigned long trap_code;  
    oid trap_oid[48];    
    unsigned long trap_oid_len;
}traps_info;

int SNMPA_CreateCfgFile(char *szRemoteURL);
void snmpa_handleMessage();

int snmpa_MsgProcess(void *pstMsg_void);
int SNMPA_StartComline(int action, int whichFile, char *szRemoteURL);
void ZTE_AddTrapPremsg(netsnmp_variable_list **vars, 
                                                 oid *trapoid, unsigned long ulTrapOidLen,
                                                 unsigned long ulAlarmCode, 
                                                 unsigned long ulAlarmLevel, char *pszAlarmTitle, char *pszAlarmInfo);
int  SNMPA_SendDl_UlSuccTrap(void *pInstance, const char *pszOutInfo);
int  SNMPA_SendDl_UlFailTrap_paraEmpty(void *pInstance, const char *pszOutInfo);

#define SNMPA_CustomizedTrap_AsFail SNMPA_SendDl_UlFailTrap_paraEmpty
int SNMPA_GetTrapEventSavedInFlash(char* buf);


int  SNMPA_do_upgrade(int mode);
int  SNMPA_HandleProcStart(ST_MSG *pstMsg);
void SNMPA_MsgProcOutputHandler(ST_MSG *pstMsg);
int  SNMPA_SendDl_UlFailTrap(void *pInstance, const char *pszOutInfo);
int  SNMPA_SaveEventsToFlash(char *szEventValue);
void SNMPA_Send_Comm_Trap(traps_info *pstTraps);
void SNMPA_Send_StartTrap();
void SNMPA_Send_WarnStartTrap();
void tw_AddTrapMsg(netsnmp_variable_list **vars, traps_info* pTrapInfo);

#endif

