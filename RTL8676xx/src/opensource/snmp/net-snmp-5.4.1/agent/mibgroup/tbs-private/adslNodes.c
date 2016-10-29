/*
 *  MIB group implementation - adslNodes.c
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "common/cmmif.h" 
#include "common/writeMethod.h"

#include "adslNodes.h"

static unsigned long ulRecoverConfig = 0;
static unsigned long ulReboot = 0;
static unsigned long ulWhichAction = 0;
static oid twSysObjId[] = { 1,3,6,1,4,1,33003 };\

#define THRESHOLD_TYPE_CPU     1 
#define THRESHOLD_TYPE_MEM     2

/* CPU利用率的过限阈值(%) 95 */
int nDEV_CPU_THRESHOLD =  95 ;
/* 内存利用率的过限阈值(%) 90 */
int nDEV_MEM_THRESHOLD = 90 ;
int threshType =  THRESHOLD_TYPE_CPU ; 


unsigned char *var_remoteSnmpEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method);
unsigned char *var_localSntpEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method);

unsigned char *var_basicInfoEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method);

unsigned char *var_ethDhcpEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method);

unsigned char *var_thresholdEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method);

unsigned char *var_sysObjIdEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method);
struct variable1 remotesnmp_variables[] = {
    {OID_snmpEnabled, ASN_INTEGER, RWRITE, var_remoteSnmpEntry, 1, {1}}, 
    {OID_snmpReadCommunity, ASN_OCTET_STR, RWRITE, var_remoteSnmpEntry, 1, {2}}, 
    {OID_snmpWriteCommunity, ASN_OCTET_STR, RWRITE, var_remoteSnmpEntry, 1, {3}}, 
    {OID_snmpDescription, ASN_OCTET_STR, RONLY, var_remoteSnmpEntry, 1, {4}}, 
    {OID_snmpTrapServer, ASN_IPADDRESS, RWRITE, var_remoteSnmpEntry, 1, {5}}, 
    {OID_resetAP, ASN_INTEGER, RWRITE, var_remoteSnmpEntry, 1, {6}}, 
    {OID_restoreFactory, ASN_INTEGER, RWRITE, var_remoteSnmpEntry, 1, {7}}
};
oid remotesnmp_variables_oid[] = {IGD_OID_PREFIX, 7};

struct variable1 localsntp_variables[] = {
    {OID_timeserver, ASN_OCTET_STR, RWRITE, var_localSntpEntry, 1, {1}}, 
#if 0
    {OID_timeserverport, ASN_INTEGER, RWRITE, var_localSntpEntry, 1, {2}}, 
#endif
    {OID_currenttime, ASN_OCTET_STR, RONLY, var_localSntpEntry, 1, {3}}, 
    {OID_timezone, ASN_INTEGER, RWRITE, var_localSntpEntry, 1, {4}}, 
    {OID_daylightsaving, ASN_INTEGER, RWRITE, var_localSntpEntry, 1, {5}}
};
struct variable2 basicInfo_variables[] = {
	{OID_FirmwareRev, ASN_OCTET_STR, RONLY, var_basicInfoEntry, 2, {9,1}},
    {OID_SoftwareRev, ASN_OCTET_STR, RONLY, var_basicInfoEntry, 2, {10,1}},
    {OID_SerialNum, ASN_OCTET_STR, RWRITE, var_basicInfoEntry, 2, {11,1}},
	{OID_physicalMfgName, ASN_OCTET_STR, RONLY, var_basicInfoEntry, 2, {12,1}},
	{OID_physicalModelName, ASN_OCTET_STR, RONLY, var_basicInfoEntry,2, {13,1}}
};

struct variable1 ifNumber_variables[] = {
	{OID_IfNumber, ASN_INTEGER, RONLY, var_basicInfoEntry, 1, {1}}
};

struct variable1 sysObjId_variables[] ={
    {OID_sysObjId, ASN_OBJECT_ID, RONLY, var_sysObjIdEntry, 1, {2}}  
};

struct variable1 ntp_variables[] = {
	{OID_ntpStatus, ASN_INTEGER,RWRITE, var_basicInfoEntry, 1, {1}},
	{OID_ntpPrimaryServer, ASN_IPADDRESS, RONLY, var_basicInfoEntry, 1, {2}},
	{OID_ntpSecondServer, ASN_IPADDRESS, RONLY, var_basicInfoEntry, 1, {3}}
};

struct variable1 dhcp_variable[] = {
	{OID_ethDhcpsStatus, ASN_INTEGER, RONLY, var_ethDhcpEntry, 1, {1}},
    {OID_ethDhcpsIPStart,ASN_IPADDRESS, RWRITE, var_ethDhcpEntry, 1, {2}},
    {OID_ethDhcpsIPEnd,  ASN_IPADDRESS, RWRITE, var_ethDhcpEntry, 1, {3}},
    {OID_ethDhcpsNetmask, ASN_IPADDRESS, RWRITE, var_ethDhcpEntry, 1, {4}}
};

struct variable2 threshold_variables[] = {
    {OID_cpuThresholdValue, ASN_INTEGER, RWRITE, var_thresholdEntry, 2, {8,1}},
    {OID_memThresholdValue, ASN_INTEGER, RWRITE, var_thresholdEntry, 2, {8,2}},
    {OID_rebootCPE,         ASN_INTEGER,   RWRITE, var_thresholdEntry, 2, {5,1}},
    {OID_recoverConfigCPE,  ASN_INTEGER,   RWRITE, var_thresholdEntry, 2, {5,2}},
	{OID_factoryOUI,        ASN_OCTET_STR, RONLY,  var_thresholdEntry, 2, {5,3}}
};
#ifdef CONFIG_OPENSOURCE_STREAMRATE  
struct variable1 dslrate_variable[] = {
	  {OID_UpstreamCurrRate, ASN_INTEGER, RONLY, var_basicInfoEntry, 1, {1}},
    {OID_DownstreamCurrRate,ASN_INTEGER, RONLY, var_basicInfoEntry, 1, {2}}

};
#endif
oid localsntp_variables_oid[] = {IGD_OID_PREFIX, 9};
oid basicinfo_variables_oid[] = {IGD_BasicInfo_PREFIX} ;
oid interfaceNum_oid[] = {1,3,6,1,2,1,2};
oid ethDhcp_oid[] = {IGD_DHCP_PREFIX};
oid tw_oid[]={IGD_TW_PREFIX};
oid ntp_oid[] = {1,3,6,1,4,1,33003,7};
#ifdef CONFIG_OPENSOURCE_STREAMRATE  
oid dslrate_oid[] = {1,3,6,1,4,1,33003,9};
#endif
oid sysObjId_oid[] = {1,3,6,1,2,1,1};
char *cstTimeZoneMatched[] = {
"GMT-12:00", "0",
"GMT-11:00", "1",
"GMT-10:00", "2",
"GMT-09:00", "3",
"GMT-08:00", "4",
"GMT-07:00", "7",
"GMT-06:00", "9",
"GMT-05:00", "13",
"GMT-04:00", "15",
"GMT-03:00", "20",
"GMT-02:00", "22",
"GMT-01:00", "24",
"GMT-00:00", "26",
"GMT+01:00", "27",
"GMT+02:00", "37",
"GMT+03:00", "39",
"GMT+04:00", "43",
"GMT+05:00", "47",
"GMT+05:30", "48",
"GMT+06:00", "51",
"GMT+07:00", "54",
"GMT+08:00", "56",
"GMT+09:00", "61",
"GMT+10:00", "66",
"GMT+11:00", "71",
"GMT+12:00", "72"
};

static unsigned long p_ulAdslValue = 0;
static char p_szAdslValue[256] = {0};
static char p_szAdslPath[256] = {0};

void init_adslNodes(void)
{
    SNMPA_TRACE("Init adsl nodes ...\n");
    REGISTER_MIB("remotesnmp",  remotesnmp_variables, variable1, remotesnmp_variables_oid);
    REGISTER_MIB("localsntp",  localsntp_variables, variable1, localsntp_variables_oid);
	REGISTER_MIB("basicInfo",  basicInfo_variables, variable2, basicinfo_variables_oid);
	REGISTER_MIB("interfaceNum", ifNumber_variables, variable1, interfaceNum_oid);
	REGISTER_MIB("ethDhcpsIP", dhcp_variable, variable1, ethDhcp_oid);
	REGISTER_MIB("tbsNtpConfig", ntp_variables, variable1, ntp_oid);
	REGISTER_MIB("sysObjId", sysObjId_variables, variable1, sysObjId_oid);
	REGISTER_MIB("threshold", threshold_variables, variable2, tw_oid);
#ifdef CONFIG_OPENSOURCE_STREAMRATE  
	REGISTER_MIB("dslrate", dslrate_variable, variable1, dslrate_oid);
#endif
    ulRecoverConfig = 0;
    ulReboot = 0;
    ulWhichAction = 0;
    Init_WriteMethodStruct();
}

void shutdown_adslNodes(void)
{
    Free_WriteMethodStruct();
}


int WriteMethod_Mgmt(int action,
                          unsigned char *var_val,
                          unsigned char var_val_type,
                          size_t var_val_len, 
                          unsigned char *statP, 
                          oid *name, size_t length)
{
    switch(action)
    {
        case RESERVE1:
		{
	           unsigned long ulNewVal = *((unsigned long *) var_val);
	           if(var_val_len > sizeof(unsigned long)) return SNMP_ERR_WRONGLENGTH;
		    if(ulNewVal != 0 && ulNewVal != 1)return SNMP_ERR_WRONGVALUE;
        	}
		break;
        case RESERVE2:
		break;
	 case ACTION:
		if(ulWhichAction == 1)
		{
		    ulRecoverConfig = *((unsigned long *) var_val);
		    if(ulRecoverConfig == 1) CMM_RecoverConfig();
		}
		else if(ulWhichAction == 2)
		{
		    ulReboot = *((unsigned long *) var_val);
		    if(ulReboot == 1) CMM_Reboot();
		}
		else SNMPA_TRACE("Error occurs when handling mamange action to device!\n");
		break;
	 case UNDO:
		break;
        case COMMIT:
	 	break;
	 case FREE:
		break;
    }

    return SNMP_ERR_NOERROR;
}

int WriteMethod_Threshold(int action,
                          unsigned char *var_val,
                          unsigned char var_val_type,
                          size_t var_val_len, 
                          unsigned char *statP, 
                          oid *name, size_t length)
{ 
    if( WriteMethod_Comm(action,var_val,var_val_type,var_val_len, statP, name, length) ==  SNMP_ERR_GENERR )  
    {
        return SNMP_ERR_GENERR  ;
    }

	unsigned long ulValue = *((unsigned long *) var_val);

    if(threshType == THRESHOLD_TYPE_CPU)
    {
        nDEV_CPU_THRESHOLD =  ulValue ;
    }else if(threshType == THRESHOLD_TYPE_MEM)
    {
        nDEV_MEM_THRESHOLD = ulValue ;
    }
	
    return SNMP_ERR_NOERROR ; 
}

unsigned char *var_remoteSnmpEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    if(header_generic(vp, name, length, exact, var_len, write_method) ==  MATCH_FAILED)
        return NULL;

    switch(vp->magic)
    {
        case OID_snmpEnabled:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, TR069_ROOT_SNMP "Enable", NULL, 0);
	    if(CMM_GetUlong(TR069_ROOT_SNMP "Enable", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
	    {
                return (unsigned char*)&p_ulAdslValue;
	    }
	    else return NULL;
        case OID_snmpReadCommunity:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 32, 0, NULL, TR069_ROOT_SNMP "ROCommunity", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_SNMP "ROCommunity", p_szAdslValue, sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
	    {
                *var_len = strlen(p_szAdslValue);
                return (unsigned char*)p_szAdslValue;
	    }
           return NULL;
        case OID_snmpWriteCommunity:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 32, 0, NULL, TR069_ROOT_SNMP "RWCommunity", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_SNMP "RWCommunity", p_szAdslValue, sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
	    {
                *var_len = strlen(p_szAdslValue);
                return (unsigned char*)p_szAdslValue;
	    }
           return NULL;
        case OID_snmpDescription:
	    if(CMM_GetUlong(TR069_ROOT_SNMP "TrapVersion", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
	    {
                sprintf(p_szAdslValue, "Current Trap version is %lu.", p_ulAdslValue);
                *var_len = strlen(p_szAdslValue);
                return (unsigned char*)p_szAdslValue;
	    }
	    else return NULL;
        case OID_snmpTrapServer:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 32, 0, NULL, TR069_ROOT_SNMP "TrapHost", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_SNMP "TrapHost", p_szAdslValue, sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
	    {
               return CHARS_TO_intIPADDR(p_szAdslValue);
	    }
           return NULL;
        case OID_resetAP:
            *write_method = WriteMethod_Mgmt;
	     ulWhichAction = 2;
            return (unsigned char*)&ulReboot;
        case OID_restoreFactory:
            *write_method = WriteMethod_Mgmt;
	     ulWhichAction = 1;
            return (unsigned char*)&ulRecoverConfig;
        default:
            break;
    }
    
    return NULL;
}

/* device basic info*/
unsigned char *var_basicInfoEntry(struct variable *vp,
				                  oid     *name,
				                  size_t  *length,
				                  int     exact,
				                  size_t  *var_len,
				                  WriteMethod **write_method)
{
    if(header_generic(vp, name, length, exact, var_len, write_method) ==  MATCH_FAILED)
        return NULL;

    switch(vp->magic)
    {
        case OID_physicalModelName:
		    if(CMM_GetStr(TR069_DeviceInfo "ModelName", p_szAdslValue,sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
		    {
	               *var_len = strlen(p_szAdslValue);
	                return (unsigned char*)p_szAdslValue;
		    }
		    else return NULL;

		case OID_physicalMfgName:
		    if(CMM_GetStr(TR069_DeviceInfo "ProductClass", p_szAdslValue,sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
		    {
	                *var_len = strlen(p_szAdslValue);
	                return (unsigned char*)p_szAdslValue;
		    }
		    else return NULL;
		case OID_SoftwareRev:
		    if(CMM_GetStr(TR069_DeviceInfo "SoftwareVersion", p_szAdslValue,sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
		    {
	                *var_len = strlen(p_szAdslValue);
	                return (unsigned char*)p_szAdslValue;
		    }
		    else return NULL;
        case OID_FirmwareRev:
		    if(CMM_GetStr(TR069_DeviceInfo "ModemFirmwareVersion", p_szAdslValue,sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
		    {
	                *var_len = strlen(p_szAdslValue);
	                return (unsigned char*)p_szAdslValue;
		    }
		    else return NULL;

		case OID_SerialNum:
	        *write_method = WriteMethod_Comm;
	        Active_WriteMethod(vp->type, vp->acl, 0, 32, 0, NULL, TR069_DeviceInfo "SerialNumber", NULL, 0);
		    if(CMM_GetStr(TR069_DeviceInfo "SerialNumber", p_szAdslValue, sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
		    {
	                *var_len = strlen(p_szAdslValue);
	                return (unsigned char*)p_szAdslValue;
		    }
	        return NULL;

	    case OID_IfNumber:
		   if(CMM_GetUlong(TR069_IfNumber "AvailableInterfaceNumberOfEntries", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
		    {
		            *var_len = sizeof(p_ulAdslValue);
	                return (unsigned char*)&p_ulAdslValue;
		    }
		    else return NULL;
			
        case OID_ntpStatus:
		    *write_method = WriteMethod_Comm;
	        Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, TR069_NTPServer "X_TWSZ-COM_Enable", NULL, 0);
		    if(CMM_GetUlong(TR069_NTPServer "X_TWSZ-COM_Enable", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
		    {       
		            *var_len = sizeof(p_ulAdslValue);
	                return (unsigned char*)&p_ulAdslValue;
		    }
	        return NULL;
			
		case OID_ntpPrimaryServer:
            if(CMM_GetStr(TR069_NTPServer"NTPServer1", p_szAdslValue, sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
	        {
                return CHARS_TO_intIPADDR(p_szAdslValue);
	        }
		    else return NULL;
			
		case OID_ntpSecondServer:
		    if(CMM_GetStr(TR069_NTPServer "NTPServer2", p_szAdslValue, sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
		    {      
		        return CHARS_TO_intIPADDR(p_szAdslValue);
		    }
		    else return NULL;
#ifdef CONFIG_OPENSOURCE_STREAMRATE  
	  case OID_UpstreamCurrRate:
		   if(CMM_GetUlong(TR069_WANDSLInterfaceConfig "UpstreamCurrRate", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
		    {
		            *var_len = sizeof(p_ulAdslValue);
	                return (unsigned char*)&p_ulAdslValue;
		    }
		    else return NULL;
		case OID_DownstreamCurrRate:
		   if(CMM_GetUlong(TR069_WANDSLInterfaceConfig "DownstreamCurrRate", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
		    {
		            *var_len = sizeof(p_ulAdslValue);
	                return (unsigned char*)&p_ulAdslValue;
		    }
		    else return NULL;
#endif	
   default:
            break;
    }
    
    return NULL;
}


unsigned char *var_sysObjIdEntry(struct variable *vp,
				                  oid     *name,
				                  size_t  *length,
				                  int     exact,
				                  size_t  *var_len,
				                  WriteMethod **write_method)
{
    if(header_generic(vp, name, length, exact, var_len, write_method) ==  MATCH_FAILED)
        return NULL;
    
    switch(vp->magic)
    {
        case OID_sysObjId:
		     *var_len = sizeof(twSysObjId);
	         return (unsigned char*)twSysObjId;
        default:
            break;
    }
    
    return NULL;
}

unsigned char *var_thresholdEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method)
{
   if(header_generic(vp, name, length, exact, var_len, write_method) ==  MATCH_FAILED)
        return NULL;
  
   switch(vp->magic)
    {
        case OID_cpuThresholdValue:
			threshType = THRESHOLD_TYPE_CPU ;
		    *write_method = WriteMethod_Threshold;
	        Active_WriteMethod(vp->type, vp->acl, 1, 1, 99, NULL,TR069_SNMPAgent "CPUThreshold" , NULL, 0);
		    if(CMM_GetUlong(TR069_SNMPAgent"CPUThreshold", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
		    {       
		            *var_len = sizeof(p_ulAdslValue);
	                return (unsigned char*)&p_ulAdslValue;
		    }
	        return NULL;
			
         case OID_memThresholdValue:
		 	threshType = THRESHOLD_TYPE_MEM ;
            *write_method = WriteMethod_Threshold;
	        Active_WriteMethod(vp->type, vp->acl, 1, 1, 99, NULL, TR069_SNMPAgent "MemThreshold", NULL, 0);
		    if(CMM_GetUlong(TR069_SNMPAgent"MemThreshold", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
		    {       
		            *var_len = sizeof(p_ulAdslValue);
	                return (unsigned char*)&p_ulAdslValue;
		    }
	        return NULL;
			
         case OID_rebootCPE:
            *write_method = WriteMethod_Mgmt;
	         ulWhichAction = 2;
             return (unsigned char*)&ulReboot;
         case OID_recoverConfigCPE:
            *write_method = WriteMethod_Mgmt;
	         ulWhichAction = 1;
			 return (unsigned char*)&ulRecoverConfig;
	
         case OID_factoryOUI:
		 	if(CMM_GetStr(TR069_DeviceInfo "ManufacturerOUI", p_szAdslValue,sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
		    {
	                *var_len = strlen(p_szAdslValue);
	                return (unsigned char*)p_szAdslValue;
		    }
		    else return NULL;
         default:
            break;
    }
}

unsigned char *var_ethDhcpEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method)
{
   if(header_generic(vp, name, length, exact, var_len, write_method) ==  MATCH_FAILED)
        return NULL;
   
#define DHCP_IPAddr_WRITABLE_NODE(NodeName) \
do{ \
	memset(p_szAdslPath,0 ,sizeof(p_szAdslPath));\
	sprintf(p_szAdslPath ,"%s" NodeName,TR069_EthDHCP);\
	char szIpAddr[20];\
	if(CMM_GetStr(p_szAdslPath, szIpAddr,sizeof(szIpAddr), NULL, 0) == CMM_SUCCESS)\
	{\
        *write_method = WriteMethod_Comm; \
	    Active_WriteMethod(vp->type, vp->acl, 0, 4, 0, NULL, p_szAdslPath, NULL, 0);\
		unsigned char *pIpAddr = CHARS_TO_intIPADDR(szIpAddr);\
		memset(p_szAdslValue ,0 ,sizeof(p_szAdslValue));\
		memcpy(p_szAdslValue ,pIpAddr,strlen(pIpAddr)) ;\
	    *var_len = strlen(p_szAdslValue);\
	    return (unsigned char *)p_szAdslValue;\
	}\
	return NULL;\
}while(0)

    switch(vp->magic)
    {
        case OID_ethDhcpsStatus:
		    if(CMM_GetUlong(TR069_EthDHCP "DHCPServerEnable", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
		    {
		            *var_len = sizeof(p_ulAdslValue);
	                return (unsigned char*)&p_ulAdslValue;
		    }
			return NULL;
			
         case OID_ethDhcpsIPStart:
			DHCP_IPAddr_WRITABLE_NODE("MinAddress");
	     case OID_ethDhcpsIPEnd:
		 	DHCP_IPAddr_WRITABLE_NODE("MaxAddress");
	     case OID_ethDhcpsNetmask:
		 	DHCP_IPAddr_WRITABLE_NODE("SubnetMask");
         default:
            break;
    }
}
static int SCA_getCurDate(char *szCurDate)
{
    int cnt = 0;
    FILE* fp = NULL;
    char cmd[16];
    char buff[48];

    strcpy(cmd, "date");
    fp = popen(cmd, "r");
    if (!fp)
    {
    	CMMIF_TRACE("err: open pipe of '%s' for read failed - %s!\n", cmd, strerror(errno));
    	return -1;
    }

    while (!feof(fp)) 
    {
    	if (!fgets(buff, sizeof(buff), fp))  /* 把输出读完，然后关闭文件，否则所执行的命令有可能发生SIG_PIPE异常 */
       {
    	    break;
    	}

       if(cnt ++ == 0) //we just return the first line
       {
            strncpy(szCurDate, buff, strlen(buff)-1); //-1 means the last char is LR
       }
    }
    pclose(fp);

    return strlen(szCurDate);
}
unsigned char *var_localSntpEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method)
{
    if(header_generic(vp, name, length, exact, var_len, write_method) ==  MATCH_FAILED)
        return NULL;

    switch(vp->magic)
    {
        case OID_timeserver:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 64, 0, NULL, TR069_ROOT_SNTP "NTPServer1", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_SNTP "NTPServer1", p_szAdslValue, sizeof(p_szAdslValue), NULL, 0) == CMM_SUCCESS)
	    {
                *var_len = strlen(p_szAdslValue);
                return (unsigned char*)p_szAdslValue;
	    }
           return NULL;
#if 0           
        case OID_timeserverport:
#endif
        case OID_currenttime:
            if(SCA_getCurDate(p_szAdslValue) > 0)
            {
                *var_len = strlen(p_szAdslValue);
                return (unsigned char*)p_szAdslValue;
            }
           return NULL;
        case OID_timezone:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 1, 0, 72, NULL, TR069_ROOT_SNTP "LocalTimeZoneName", cstTimeZoneMatched, GET_ARRAY_BOUNADRY(cstTimeZoneMatched));
	    if(CMM_GetUlong(TR069_ROOT_SNTP "LocalTimeZoneName", &p_ulAdslValue, 
                                        cstTimeZoneMatched, GET_ARRAY_BOUNADRY(cstTimeZoneMatched)) == CMM_SUCCESS)
	    {
                return (unsigned char*)&p_ulAdslValue;
	    }
	    else return NULL;
        case OID_daylightsaving:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, TR069_ROOT_SNTP "DaylightSavingsUsed", NULL, 0);
	    if(CMM_GetUlong(TR069_ROOT_SNTP "DaylightSavingsUsed", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
	    {
                return (unsigned char*)&p_ulAdslValue;
	    }
	    else return NULL;
        default:
            break;
    }

    return NULL;
}
