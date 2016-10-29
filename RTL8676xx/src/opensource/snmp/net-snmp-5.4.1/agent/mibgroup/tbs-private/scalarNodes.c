/*
 *  MIB group implementation - scalarNodes.c
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "common/cmmif.h" 
#include "common/writeMethod.h"
 #ifdef ZTE_HOT_AP_SNMP_IF
#include "common/snmpProcess.h"
#endif

#include "scalarNodes.h"

unsigned char *var_SystemNodesEntry(struct variable *vp,
                					                  oid     *name,
                					                  size_t  *length,
                					                  int     exact,
                					                  size_t  *var_len,
                					                  WriteMethod **write_method);
unsigned char *var_IpNodesEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method);
#ifdef ZTE_HOT_AP_SNMP_IF
unsigned char *var_SysInfo2NodesEntry(struct variable *vp,
            					                  oid     *name,
            					                  size_t  *length,
            					                  int     exact,
            					                  size_t  *var_len,
            					                  WriteMethod **write_method);
#endif
/*
 * system setting
 */
struct variable1 sysinfo_variables[] = {
    {OID_sysVersion, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_sysVersion}},
    {OID_sysMacAddr, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_sysMacAddr}},
    {OID_sysAPName, ASN_OCTET_STR, RWRITE, var_SystemNodesEntry, 1, {OID_sysAPName}},
    {OID_sysSpanningTreeEnable, ASN_INTEGER, RWRITE, var_SystemNodesEntry, 1, {OID_sysSpanningTreeEnable}},
    {OID_sysCountryRegion, ASN_INTEGER, RWRITE, var_SystemNodesEntry, 1, {OID_sysCountryRegion}},
    {OID_wams, ASN_INTEGER, RWRITE, var_SystemNodesEntry, 1, {OID_wams}},
    {OID_watchdog, ASN_INTEGER, RWRITE, var_SystemNodesEntry, 1, {OID_watchdog}},
    {OID_vlanEnable, ASN_INTEGER, RWRITE, var_SystemNodesEntry, 1, {OID_vlanEnable}},
    {OID_managementVlanID, ASN_INTEGER, RWRITE, var_SystemNodesEntry, 1, {OID_managementVlanID}},
    {OID_ethernetDataRate, ASN_INTEGER, RWRITE, var_SystemNodesEntry, 1, {OID_ethernetDataRate}},
    {OID_sysRedBootVersion, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_sysRedBootVersion}},
    {OID_ntpdEnable, ASN_INTEGER, RWRITE, var_SystemNodesEntry, 1, {OID_ntpdEnable}},
    {OID_softwareVendor, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_softwareVendor}},
    {OID_softwareName, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_softwareName}},
    {OID_apBssidNum, ASN_INTEGER, RONLY, var_SystemNodesEntry, 1, {OID_apBssidNum}},
    {OID_essidBssidNum, ASN_INTEGER, RONLY, var_SystemNodesEntry, 1, {OID_essidBssidNum}},
    {OID_apSysNEId, ASN_OCTET_STR, RWRITE, var_SystemNodesEntry, 1, {OID_apSysNEId}},
    {OID_syslogEnabled, ASN_INTEGER, RWRITE, var_SystemNodesEntry, 1, {OID_syslogEnabled}},
    {OID_syslogServerIP, ASN_IPADDRESS, RWRITE, var_SystemNodesEntry, 1, {OID_syslogServerIP}},
    {OID_syslogServerPort, ASN_OCTET_STR, RWRITE, var_SystemNodesEntry, 1, {OID_syslogServerPort}},
    {OID_userName, ASN_OCTET_STR, RWRITE, var_SystemNodesEntry, 1, {OID_userName}},
    {OID_mibVersion, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_mibVersion}},
    {OID_cpuUsage, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_cpuUsage}},
    {OID_memoryUsage, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_memoryUsage}},
    {OID_memoryCapaciry, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_memoryCapaciry}},
    {OID_cpuClockSpeed, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_cpuClockSpeed}},
    {OID_apManufacturer, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_apManufacturer}},
    {OID_cpuTemperature, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_cpuTemperature}},
    {OID_flashUsableCapacity, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_flashUsableCapacity}},
    {OID_apSerial, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_apSerial}},
    {OID_nodeCreateTime, ASN_OCTET_STR, RWRITE, var_SystemNodesEntry, 1, {OID_nodeCreateTime}},
    {OID_nodeTimestamp, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_nodeTimestamp}},
    {OID_password, ASN_OCTET_STR, RWRITE, var_SystemNodesEntry, 1, {OID_password}},
    {OID_flashCapacity, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_flashCapacity}},
    {OID_hardwareversion, ASN_OCTET_STR, RONLY, var_SystemNodesEntry, 1, {OID_hardwareversion}}
};
oid sysinfo_variables_oid[] = {IGD_OID_PREFIX, 1};

/*
 * ip setting
 */
struct variable3 ipsetting_variables[] = { 
    {OID_ethernetIPType, ASN_INTEGER, RWRITE, var_IpNodesEntry, 1, {5, 1}},
    {OID_ethernetIPAddress, ASN_IPADDRESS, RWRITE, var_IpNodesEntry, 2, {5, 2}},
    {OID_ethernetNetmask, ASN_IPADDRESS, RWRITE, var_IpNodesEntry, 2, {5, 3}},
    {OID_ethernetGateway, ASN_IPADDRESS, RWRITE, var_IpNodesEntry, 2, {5, 4}},
    {OID_ethernetDNSPrimary, ASN_IPADDRESS, RWRITE, var_IpNodesEntry, 2, {5, 5}},
    {OID_ethernetDNSSecondary, ASN_IPADDRESS, RWRITE, var_IpNodesEntry, 2, {5, 6}},
    {OID_ethernetDhcpsIPStart, ASN_IPADDRESS, RWRITE, var_IpNodesEntry, 3, {5, 7, 1}},
    {OID_ethernetDhcpsIPEnd, ASN_IPADDRESS, RWRITE, var_IpNodesEntry, 3, {5, 7, 2}},
    {OID_ethernetDhcpsNetmask, ASN_IPADDRESS, RWRITE, var_IpNodesEntry, 3, {5, 7, 3}},
    {OID_pppoeConnStatus, ASN_OCTET_STR, RONLY, var_IpNodesEntry, 2, {7, 1}},
    {OID_pppoeLocalIP, ASN_IPADDRESS, RONLY, var_IpNodesEntry, 2, {7, 2}},
    {OID_pppoeUserName, ASN_OCTET_STR, RWRITE, var_IpNodesEntry, 2, {7, 3}},
    {OID_pppoePassword, ASN_OCTET_STR, RWRITE, var_IpNodesEntry, 2, {7, 4}},
    {OID_pppoeTimeOut, ASN_INTEGER, RWRITE, var_IpNodesEntry, 2, {7, 5}},
    {OID_pppoeRetryTimes, ASN_INTEGER, RWRITE, var_IpNodesEntry, 2, {7, 6}},
    {OID_pppoeAuthenticationType, ASN_INTEGER, RWRITE, var_IpNodesEntry, 2, {7, 7}},
    {OID_pppoeMTU, ASN_INTEGER, RWRITE, var_IpNodesEntry, 2, {7, 8}}
};
oid ipsetting_variables_oid[] = {IGD_OID_PREFIX, 2};
#ifdef ZTE_HOT_AP_SNMP_IF
struct variable4 sysinfo2_variables[] = {
    {OID_w640aNodeID, ASN_OCTET_STR, RONLY, var_SysInfo2NodesEntry, 3, {8, 1, 1}},
    {OID_w640aHostName, ASN_OCTET_STR, RONLY, var_SysInfo2NodesEntry, 3, {8, 1, 2}},
    {OID_w640aVersion, ASN_OCTET_STR, RONLY, var_SysInfo2NodesEntry, 3, {8, 1, 3}},
    {OID_w640aCreateDate, ASN_OCTET_STR, RONLY, var_SysInfo2NodesEntry, 3, {8, 1, 4}},
    {OID_sysfileLoadFlag, ASN_INTEGER, RWRITE, var_SysInfo2NodesEntry, 4, {8, 9, 2, 1}},
    {OID_sysfileType, ASN_INTEGER, RWRITE, var_SysInfo2NodesEntry, 4, {8, 9, 2, 2}},
    {OID_sysfileSize, ASN_OCTET_STR, RWRITE, var_SysInfo2NodesEntry, 4, {8, 9, 2, 3}},
    {OID_sysfileURL, ASN_OCTET_STR, RWRITE, var_SysInfo2NodesEntry, 4, {8, 9, 2, 4}},
    {OID_cmdKey, ASN_OCTET_STR, RWRITE, var_SysInfo2NodesEntry, 4, {8, 9 ,2, 5}}
};
oid sysinfo2_variables_oid[] = {IGD_OID_PREFIX, 8, 1};
#endif

static unsigned long p_ulScalarValue = 0;
static char p_szScalarValue[256] = {0};
static char p_szScalarPath[256] = {0};


void init_scalarNodes(void)
{
    REGISTER_MIB("sysinfoNodes",  sysinfo_variables, variable1, sysinfo_variables_oid);
#ifdef ZTE_HOT_AP_SNMP_IF    
    REGISTER_MIB("sysinfo2Nodes",  sysinfo2_variables, variable4, sysinfo2_variables_oid);
#endif
    REGISTER_MIB("ipsettingNodes",  ipsetting_variables, variable3, ipsetting_variables_oid);
    SNMPA_TRACE("Init scalar nodes ...\n")
    Init_WriteMethodStruct();
}

void shutdown_scalarNodes(void)
{
    Free_WriteMethodStruct();
}

/*
 * retrieve the ap mac. 
 * first, get wan interface name
 * second, execute ifconfig <wanif> to parse mac address
 * szAPMac <->output the mac of ap interface
 */
static int SCA_getApMac(char *szAPMac)
{
    int i = 0;
    char **ppLanIfList = NULL;
    char *pszLanIfNodePrefix = NULL;

    //IS_CACH_TIMEOUT(stEthLanIfTbl.cachTime);

    ppLanIfList = CMM_GetInstanceList(TR069_ROOT_LanDevice);
    if(!ppLanIfList) return -1;

    for(; ppLanIfList[i]; i++)
    {
        pszLanIfNodePrefix = ppLanIfList[i];
        if(CMM_GetStrColumn(pszLanIfNodePrefix, "X_TWSZ-COM_DeviceName", szAPMac, sizeof(szAPMac), NULL,
                                            NULL, 0) != CMM_SUCCESS)
        {
            break; 
        }

        if(strlen(szAPMac) > 0) /*device interface length must be more than 0, we can promise ok*/
        {
            char szTmp[32] = {0};
            if(IfConfigRead(szAPMac, NULL, "HWaddr", " ", szTmp, sizeof(szTmp)) == 0)
            {
                strcpy(szAPMac, szTmp);
                break; /*We just get the first one, and skip the left*/
            }
        }
    }
    CMM_FreeInstanceList(ppLanIfList);

    return strlen(szAPMac);
}



static unsigned long SCA_getManagementVlanID(unsigned long *ulVlanID)
{
    int i = 0;
    char **ppVlanList = NULL;
    char *pszVlanNodePrefix = NULL;
    char szServiceType[8] = {0};

    //IS_CACH_TIMEOUT(stValTbl.cachTime);
    
    ppVlanList = CMM_GetInstanceList(TR069_ROOT_VlanTable);
    if(!ppVlanList) return -1;

    *ulVlanID = 0; //init to 0 to return
    for(; ppVlanList[i]; i++)
    {
        pszVlanNodePrefix = ppVlanList[i];
        if(CMM_GetStrColumn(pszVlanNodePrefix, "ServiceType", szServiceType, sizeof(szServiceType), NULL,
                                            NULL, 0) == CMM_SUCCESS)
        {
             if(!strcasecmp(szServiceType, "tr069"))
             {
                CMM_GetIntColumn(pszVlanNodePrefix, "VLANIDMark", ulVlanID, NULL, 0);
                break;  /* force to exit loop */
             }
        }
    }
    CMM_FreeInstanceList(ppVlanList);

    return *ulVlanID;
}

static int SCA_getRedBootVersion(char *szSysVer)
{
    int cnt = 0;
    FILE* fp = NULL;
    char cmd[32];
    char buff[32];

    strcpy(cmd, "uname -r");
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
            strncpy(szSysVer, buff, strlen(buff)-1); //-1 means the last char is LR
       }
    }
    pclose(fp);

    return strlen(szSysVer);
}

unsigned char *var_SystemNodesEntry(struct variable *vp,
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
        case OID_sysVersion:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo"SoftwareVersion", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
	    break;
        case OID_sysMacAddr:
           if(!SCA_getApMac(p_szScalarValue))
           {
                return NULL;
	    }
	    break;
        case OID_sysAPName:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 256, 0, NULL, TR069_ROOT_DeviceInfo"Description", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo"Description", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
	    break;
        case OID_sysSpanningTreeEnable:
            return NULL;
        case OID_sysCountryRegion:
            return NULL;
        case OID_wams:
            return NULL;
        case OID_watchdog:
            return NULL;
        case OID_vlanEnable:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, TR069_ROOT_Vlan "Enable", NULL, 0);
	    if(CMM_GetUlong(TR069_ROOT_Vlan "Enable", &p_ulScalarValue, NULL, 0) == CMM_SUCCESS)
	    {
                return (unsigned char*)&p_ulScalarValue;
	    }
	    else return NULL;
        case OID_managementVlanID:
            SCA_getManagementVlanID(&p_ulScalarValue);
            return (unsigned char*)&p_ulScalarValue;
        case OID_ethernetDataRate:
            {
                static char *szMatch[] = {"Auto", "0", "10", "10", "100", "100"};
                *write_method = WriteMethod_Comm;
                Active_WriteMethod(vp->type, vp->acl, 1, 0, 100, NULL, 
                                                  TR069_ROOT_WanEthIfConf "MaxBitRate", szMatch, GET_ARRAY_BOUNADRY(szMatch));
                if(CMM_GetStr(TR069_ROOT_WanEthIfConf "MaxBitRate", p_szScalarValue, sizeof(p_szScalarValue),
                                         szMatch, GET_ARRAY_BOUNADRY(szMatch)) == CMM_SUCCESS)
                {
                    p_ulScalarValue = atoi(p_szScalarValue);
                    return (unsigned char*)&p_ulScalarValue;
                }
                return NULL;
            }
        case OID_sysRedBootVersion:
            if(!SCA_getRedBootVersion(p_szScalarValue))
            {
                return NULL;
            }
            break;
        case OID_ntpdEnable:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, TR069_ROOT_Ntp "X_TWSZ-COM_SNTPEnabled", NULL, 0);
	    if(CMM_GetUlong(TR069_ROOT_Ntp "X_TWSZ-COM_SNTPEnabled", &p_ulScalarValue, NULL, 0) == CMM_SUCCESS)
	    {
                return (unsigned char*)&p_ulScalarValue;
	    }
	    else return NULL;
        case OID_softwareVendor:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo "APVendor", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
	    break;
        case OID_softwareName:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo "ModelName", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
	    break;
        case OID_apBssidNum:
            return NULL;
        case OID_essidBssidNum:
            return NULL;            
        case OID_apSysNEId:
            return NULL;
        case OID_syslogEnabled:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, TR069_ROOT_Logger "LoggerEnabled", NULL, 0);
	    if(CMM_GetUlong(TR069_ROOT_Logger "LoggerEnabled", &p_ulScalarValue, NULL, 0) == CMM_SUCCESS)
	    {
	        return (unsigned char*)&p_ulScalarValue;
	    }
           else return NULL;
        case OID_syslogServerIP:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 32, 0, NULL, TR069_ROOT_Logger "LogTFTPServer", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_Logger "LogTFTPServer", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) == CMM_SUCCESS)
	    {
               return CHARS_TO_intIPADDR(p_szScalarValue);
	    }
           return NULL;
        case OID_syslogServerPort:
            return NULL;
        case OID_userName:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 256, 0, NULL, TR069_ROOT_Auth "X_TWSZ-COM_UserName", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_Auth "X_TWSZ-COM_UserName", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
	        return NULL;
	    }
           break;
        case OID_mibVersion:
            sprintf(p_szScalarValue, "%lu.0", (unsigned long)IGDMIB_OID);
            break;
        case OID_cpuUsage:
	    {
		  int ret = 0;
                float fResult = 0.00;
		  if ((ret = mib_get_cpu_occup(&fResult)) == SNMP_ERR_NOERROR)
		  {
                    memset(p_szScalarValue, 0, sizeof(p_szScalarValue));
                    sprintf(p_szScalarValue, "%f", fResult);
                    strcat(p_szScalarValue, "%");
		  }
                else SNMPA_TRACE("mib_get_cpu_occup failed. ret: %08x\n", ret);
	         break;
	    }
        case OID_memoryUsage:
           {
                unsigned long ulFree = 0;
                unsigned long ulTotal = 0;
                if ( GetFeatureInt(PROC_MEMINFO, MEMTOTAL, &ulTotal, 10)==0 &&
                      GetFeatureInt(PROC_MEMINFO, MEMFREE, &ulFree, 10)==0) 
                {
                    float fResult = 0.00;
                    fResult = ((ulTotal - ulFree) * 100.00)/ulTotal;
                    memset(p_szScalarValue, 0, sizeof(p_szScalarValue));
                    sprintf(p_szScalarValue, "%f", fResult);
                    strcat(p_szScalarValue, "%");
                }
                else SNMPA_TRACE("GetFeatureInt failed!\n");
                break;
           }
        case OID_memoryCapaciry:
	    if (GetFeatureInt(PROC_MEMINFO, MEMTOTAL, &p_ulScalarValue, 10) == 0)
	    {
                sprintf(p_szScalarValue, "%lu", p_ulScalarValue);
           }
           else SNMPA_TRACE("GetFeatureInt failed!\n");
	    break;
        case OID_cpuClockSpeed:
	    if (GetFeature(PROC_CPUINFO, CPUFRQ, p_szScalarValue, sizeof(p_szScalarValue)) != 0)
	    {
               SNMPA_TRACE("GetFeature failed!\n");
               return NULL;
	    }
           break;
        case OID_apManufacturer:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo "Manufacturer", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
	    break;
        case OID_cpuTemperature:
            return NULL;
        case OID_flashUsableCapacity:
           {
                unsigned long ulFree = 0;
                p_ulScalarValue = FlashTotal();
                p_ulScalarValue >>= 10;	/* unit is Kb */
                
                if (GetProcInt(PROC_FLASHFREE, &ulFree, 16) == 0)
                {
                    ulFree >>= 10;	/* unit is KB */
                    p_ulScalarValue -= ulFree;
                    sprintf(p_szScalarValue, "%lu", p_ulScalarValue);
                }
                else SNMPA_TRACE("GetFeatureInt failed!\n");
                break;
           }
        case OID_apSerial:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo "SerialNumber", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
                return NULL;
	    }
	    break;
        case OID_nodeCreateTime:
            return NULL;
        case OID_nodeTimestamp:
            return NULL;
        case OID_password:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 256, 0, NULL, TR069_ROOT_Auth "X_TWSZ-COM_Password", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_Auth "X_TWSZ-COM_Password", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
	        return NULL;
	    }
           break;
        case OID_flashCapacity:
	    p_ulScalarValue = FlashTotal();
           sprintf(p_szScalarValue, "%lu", p_ulScalarValue>>10);
           break;
        case OID_hardwareversion:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo "HardwareVersion", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
                return NULL;
	    }
	    break;
        default:
            break;
    }

    *var_len = strlen(p_szScalarValue);
    return (unsigned char*)p_szScalarValue;
}

unsigned char *var_IpNodesEntry(struct variable *vp,
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
        case OID_ethernetIPType:
        case OID_ethernetIPAddress:
        case OID_ethernetNetmask:
        case OID_ethernetGateway:
        case OID_ethernetDNSPrimary:
        case OID_ethernetDNSSecondary:
        case OID_ethernetDhcpsIPStart:
        case OID_ethernetDhcpsIPEnd:
        case OID_ethernetDhcpsNetmask:
        case OID_pppoeConnStatus:
        case OID_pppoeLocalIP:
        case OID_pppoeUserName:
        case OID_pppoePassword:
        case OID_pppoeTimeOut:
        case OID_pppoeRetryTimes:
        case OID_pppoeAuthenticationType:
        case OID_pppoeMTU:
        default:
            break;
    }
    
    return NULL;
}

#ifdef ZTE_HOT_AP_SNMP_IF
/*
 * launch the action of upload or download
 */
static int SCA_writeMethod_DoTransfer(int action, 
                                                                unsigned char *var_val, 
                                                                unsigned char var_val_type, 
                                                                size_t var_val_len, 
                                                                unsigned char *statP, 
                                                                oid *name, 
                                                                size_t length)
{
    unsigned long ulNewVal = *((unsigned long *) var_val);
    static unsigned long ulFileType = ZERO;
    switch(action)
    {
        case RESERVE1:
              if(var_val_len > sizeof(unsigned long)) return SNMP_ERR_WRONGLENGTH;
	       if(ulNewVal != DOWNLOAD && ulNewVal != UPLOAD) return SNMP_ERR_WRONGVALUE;

              if(CMM_GetUlong(TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileType", &ulFileType, NULL, 0) != CMM_SUCCESS)
              {
                  return SNMP_ERR_GENERR;
              }
SNMPA_TRACE("ulFileType:%lu\n", ulFileType); //wei              
              if(ulNewVal == UPLOAD)
              {
                if(ulFileType == IMG_FILE) 
                {/*When upload, firemare type can't be supported*/
                    SNMPA_TRACE("Bad command: can't upload firmeware IMAGE file to server!!!\n");
                    return SNMP_ERR_WRONGVALUE;
                }
              }
              else 
              {
                 if(ulFileType == LOG_FILE) /*don't support logger file*/
                 {
                    SNMPA_TRACE("Bad command: can't download log file to CPE!!!\n");
                    return SNMP_ERR_WRONGVALUE;
                 }
              }
		break;
        case RESERVE2:
		break;
	 case ACTION:
              if(CMM_SetUlong(TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileLoadFlag", ulNewVal, NULL, 0) != CMM_SUCCESS)
              {
                  return SNMP_ERR_GENERR;
              }              
              break;
	 case UNDO:
		break;
        case COMMIT:
              {
                  int isFail = 0;
                  char szFileSize[64] = {0};
                  char szFileUrl[256] = {0};
                  const char *FAIL_TRAP_INFO[] = 
                  {
                    "Failed to read upload/download parameters.",
                    "Failed to read current mem size.",
                    "File size is too big, no enough room for download.",
                    "Internal error"
                  };

                  if(CMM_GetStr(TR069_ROOT_FILETRANS "X_TWSZ-COM_cmdKey", szFileSize, sizeof(szFileSize), NULL, 0) != CMM_SUCCESS ||
                      CMM_GetStr(TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileURL", szFileUrl, sizeof(szFileUrl), NULL, 0) != CMM_SUCCESS)
                  {
                      isFail = 1; /*reading relative references for upload/download has been wrong*/
                      goto fail_trap;
                  }

                  if(ulNewVal == DOWNLOAD)
                  {
                       /*make sure that free falsh on ap is more than <filesize>*/
                      unsigned long ulFree = 0;
                       if (GetProcInt(PROC_FLASHFREE, &ulFree, 16) != 0) /*unit is byte*/
                      {
                          isFail = 2; /*Fail to get flash free size*/
                          SNMPA_TRACE("GetFeatureInt failed!\n");
                          goto fail_trap;
                      }

                      if(ulFree < atoi(szFileSize))
                      {
                          isFail = 3; /*no enough room for download*/
                          goto fail_trap;
                      }
                    SNMPA_TRACE("ulNewVal:%lu, ulFileType:%lu\n", ulNewVal, ulFileType); //wei
                      if(SNMPA_StartComline(ulNewVal, ulFileType, szFileUrl) == TBS_SUCCESS)
                          goto succ_exit;
                      
                      isFail = 4;
                      goto fail_trap;
                  }
                  else /*Upload*/
                  {
                      if(ulFileType == LOG_FILE)
                      {
                          if(SNMPA_StartComline(ulNewVal, ulFileType, szFileUrl) == TBS_SUCCESS)
                              goto succ_exit;

                          isFail = 4;
                          goto fail_trap;
                      }
                      else /*THis must be Config file now*/
                      if(SNMPA_CreateCfgFile(szFileUrl) == TBS_SUCCESS)
                          goto succ_exit;
                  }

                  succ_exit:
                      SNMPA_TRACE("Good, everything is perfect.......\n"); //wei
                      goto exit_all;
                  fail_trap:
                     SNMPA_TRACE("My God, Something is wrong.......\n"); //wei
                     SNMPA_CustomizedTrap_AsFail(NULL, FAIL_TRAP_INFO[isFail-1]);
              }
	 	break;
	 case FREE:
		break;
    }
exit_all:
    return SNMP_ERR_NOERROR;
}

unsigned char *var_SysInfo2NodesEntry(struct variable *vp,
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
        case OID_w640aNodeID:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo "SerialNumber", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
        case OID_w640aHostName:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo "ModelName", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
        case OID_w640aVersion:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo "SoftwareVersion", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
        case OID_w640aCreateDate:
	    if(CMM_GetStr(TR069_ROOT_DeviceInfo "X_TWSZ-COM_w640aCreateDate", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
        case OID_sysfileLoadFlag:
            *write_method = SCA_writeMethod_DoTransfer;
	    if(CMM_GetUlong(TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileLoadFlag", &p_ulScalarValue, NULL, 0) == CMM_SUCCESS)
	    {
	        return (unsigned char*)&p_ulScalarValue;
	    }
	    else return NULL;
        case OID_sysfileType:
            *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 1, IMG_FILE, LOG_FILE, NULL, TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileType", NULL, 0);
	    if(CMM_GetUlong(TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileType", &p_ulScalarValue, NULL, 0) == CMM_SUCCESS)
	    {
	        return (unsigned char*)&p_ulScalarValue;
	    }
	    else return NULL;
        case OID_sysfileSize:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 64, 0, NULL, TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileSize", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileSize", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
           break;
        case OID_sysfileURL:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 256, 0, NULL, TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileURL", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_FILETRANS "X_TWSZ-COM_sysfileURL", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
           break;
        case OID_cmdKey:
           *write_method = WriteMethod_Comm;
           Active_WriteMethod(vp->type, vp->acl, 0, 256, 0, NULL, TR069_ROOT_FILETRANS "X_TWSZ-COM_cmdKey", NULL, 0);
	    if(CMM_GetStr(TR069_ROOT_FILETRANS "X_TWSZ-COM_cmdKey", p_szScalarValue, sizeof(p_szScalarValue), NULL, 0) != CMM_SUCCESS)
	    {
               return NULL;
	    }
           break;
        default:
            break;
    }
    
    *var_len = strlen(p_szScalarValue);
    return (unsigned char*)p_szScalarValue;
}
#endif
