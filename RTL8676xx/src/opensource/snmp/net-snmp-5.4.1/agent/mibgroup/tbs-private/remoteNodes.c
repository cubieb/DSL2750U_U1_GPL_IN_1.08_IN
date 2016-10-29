/*
 *  MIB group implementation - wirelessNodes.c
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "common/cmmif.h" 
#include "common/writeMethod.h"

#include "remoteNodes.h"

#ifndef TBS_SNMP_APP

int RN_GetWirelessEncryptionType(char *pszConfigPathPrefix, char *pszOutPutEncryptionType);
int RN_GetWirelessAuthenticationType(char *pszConfigPathPrefix, char *pszOutPutAuthenticationType);
unsigned char *var_RadiusNodesEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_WlanNodesEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_VapNodesEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_EthStatsEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_WirelessStatsEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_WlanClientEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_WlanClient2Entry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_WlanWepEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_WlanVapEncrytEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_WlanVapDataEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
#endif

unsigned char *var_WanInfoTblDataEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_WlanInfoTblDataEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_VlanInfoTblDataEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
unsigned char *var_WlanInfoEnableEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method);
#ifndef TBS_SNMP_APP
struct variable2 radius_variables[] = {
    {OID_radiusAuthPrimaryServerIP, ASN_IPADDRESS, RWRITE, var_RadiusNodesEntry, 2, {1, 1}},
    {OID_radiusAuthPrimaryServerPort, ASN_INTEGER, RWRITE, var_RadiusNodesEntry, 2, {1, 2}},
    {OID_radiusAuthPrimaryServerSecret, ASN_OCTET_STR, RWRITE, var_RadiusNodesEntry, 2, {1, 3}},
    {OID_radiusAuthSecondaryServerIP, ASN_IPADDRESS, RWRITE, var_RadiusNodesEntry, 2, {2, 1}},
    {OID_radiusAuthSecondaryServerPort, ASN_INTEGER, RWRITE, var_RadiusNodesEntry, 2, {2, 2}},
    {OID_radiusAuthSecondaryServerSecret, ASN_OCTET_STR, RWRITE, var_RadiusNodesEntry, 2, {2, 3}},
    {OID_radiusAccountPrimaryServerIP, ASN_IPADDRESS, RWRITE, var_RadiusNodesEntry, 2, {3, 1}},
    {OID_radiusAccountPrimaryServerPort, ASN_INTEGER, RWRITE, var_RadiusNodesEntry, 2, {3, 2}},
    {OID_radiusAccountPrimaryServerSecret, ASN_OCTET_STR, RWRITE, var_RadiusNodesEntry, 2, {3, 3}},
    {OID_radiusAccountSecondaryServerIP, ASN_IPADDRESS, RWRITE, var_RadiusNodesEntry, 2, {4, 1}},
    {OID_radiusAccountSecondaryServerPort, ASN_INTEGER, RWRITE, var_RadiusNodesEntry, 2, {4, 2}},
    {OID_radiusAccountSecondaryServerSecret, ASN_OCTET_STR, RWRITE, var_RadiusNodesEntry, 2, {4, 3}}
};
oid radius_variables_oid[] = {IGD_OID_PREFIX, 3};

/*wlanSetting*/
struct variable3 wlan_variables[] = {
    {OID_wirelessMode, ASN_INTEGER, RONLY, var_WlanNodesEntry, 3, {1, 1, 2}},
    {OID_channel, ASN_INTEGER, RWRITE, var_WlanNodesEntry, 3, {1, 1, 3}},
    {OID_txRate, ASN_INTEGER, RONLY, var_WlanNodesEntry, 3, {1, 1, 4}},
    {OID_fragmentationThreshold, ASN_INTEGER, RWRITE, var_WlanNodesEntry, 3, {1, 1, 5}},
    {OID_rtsThreshold, ASN_INTEGER, RWRITE, var_WlanNodesEntry, 3, {1, 1, 6}},
    {OID_txPower, ASN_INTEGER, RWRITE, var_WlanNodesEntry, 3, {1, 1, 7}},
    {OID_superMode, ASN_INTEGER, RWRITE, var_WlanNodesEntry, 3, {1, 1, 8}},
    {OID_wmmSupport, ASN_INTEGER, RWRITE, var_WlanNodesEntry, 3, {1, 1, 14}},
    {OID_linkintegrity, ASN_INTEGER, RWRITE, var_WlanNodesEntry, 3, {1, 1, 24}},
    {OID_wlanMaxStaNum, ASN_INTEGER, RWRITE, var_WlanNodesEntry, 3, {1, 1, 28}},
    {OID_radio, ASN_INTEGER, RWRITE, var_WlanNodesEntry, 3, {1, 1, 30}}
};
oid wlan_variables_oid[] = {IGD_OID_PREFIX, 4};
char *cszWirelessModeMatched[] = {"Auto", "0", "a", "1", "b", "2", "g", "3"};
char *csztxRateMatched[] = {"Auto", "0", "10", "10", "100", "100", "1000", "1000"};
char *csztxPowerMatched[] = {"100", "0", "50", "1", "25", "2", "12", "3", "6", "4"};

/*wlanVapSetting*/
struct variable1 vap_variables[] = {
    {OID_broadcastSsid, ASN_INTEGER, RWRITE, var_VapNodesEntry, 1, {2}},
    {OID_beaconInterval, ASN_INTEGER, RWRITE, var_VapNodesEntry, 1, {3}},
    {OID_dtim, ASN_INTEGER, RWRITE, var_VapNodesEntry, 1, {4}},
    {OID_deviceOperationMode, ASN_INTEGER, RWRITE, var_VapNodesEntry, 1, {6}}
};
oid vap_variables_oid[] = {IGD_OID_PREFIX, 4, 3, 1};
char *cszbroadcastSsid[] = {"0", "1", "1", "0"}; /*这个跟实际需要的刚好相反:是1时表示隐藏，即不支持广播，0时也是如此*/
char *cszdeviceOperationModeMatched[] = {"disabled", "0", "local", "2", "server", "3"};

/*EthernetStats*/
struct variable1 ethstats_variables[] = {
    {OID_ethernetRXPackets ,ASN_COUNTER ,RONLY ,var_EthStatsEntry ,1, {OID_ethernetRXPackets}},
    {OID_ethernetRXBytes ,ASN_COUNTER ,RONLY ,var_EthStatsEntry ,1, {OID_ethernetRXBytes}},
    {OID_ethernetTXPackets ,ASN_COUNTER ,RONLY ,var_EthStatsEntry ,1, {OID_ethernetTXPackets}},
    {OID_ethernetTXBytes ,ASN_COUNTER ,RONLY ,var_EthStatsEntry ,1, {OID_ethernetTXBytes}},
    {OID_ethernetRXPacketsError ,ASN_COUNTER ,RONLY ,var_EthStatsEntry ,1, {OID_ethernetRXPacketsError}},
    {OID_ethernetRXPacketsDropped ,ASN_COUNTER ,RONLY ,var_EthStatsEntry ,1, {OID_ethernetRXPacketsDropped}},
    {OID_ethernetTXPacketsError ,ASN_COUNTER ,RONLY ,var_EthStatsEntry ,1, {OID_ethernetTXPacketsError}},
    {OID_ethernetTXPacketsDropped ,ASN_COUNTER ,RONLY ,var_EthStatsEntry ,1, {OID_ethernetTXPacketsDropped}},
    {OID_ethernetUpkbps ,ASN_OCTET_STR, RONLY ,var_EthStatsEntry ,1, {OID_ethernetUpkbps}},
    {OID_ethernetDownkbps ,ASN_OCTET_STR, RONLY ,var_EthStatsEntry ,1, {OID_ethernetDownkbps}},
    {OID_ethernetLossTolerance ,ASN_OCTET_STR, RONLY ,var_EthStatsEntry ,1, {OID_ethernetLossTolerance}},
    {OID_ethernetTXRetryPackts ,ASN_COUNTER, RONLY ,var_EthStatsEntry ,1, {OID_ethernetTXRetryPackts}},
    {OID_ethernetRXRetryPackts ,ASN_COUNTER, RONLY ,var_EthStatsEntry ,1, {OID_ethernetRXRetryPackts}}
};
oid ethstats_variables_oid[] = {IGD_OID_PREFIX, 5, 1};
unsigned long g_ultxBytes[2] = {0}; // 0 index txbytes, 1 index the time seconds when retrieve it
unsigned long g_ulrxBytes[2] = {0}; // 0 index rxbytes, 1 index the time seconds when retrieve it

/*WirelessStatsTable*/
struct variable1 wirelessstats_variables[] = {
    {OID_wirelessRXPacketsTotal, ASN_COUNTER, RONLY, var_WirelessStatsEntry, 1, {4}},
    {OID_wirelessRXBytesTotal, ASN_COUNTER, RONLY, var_WirelessStatsEntry, 1, {5}},
    {OID_wirelessTXPacketsTotal, ASN_COUNTER, RONLY, var_WirelessStatsEntry, 1, {9}},
    {OID_wirelessTXBytesTotal, ASN_COUNTER, RONLY, var_WirelessStatsEntry, 1, {10}},
    {OID_wirlessRXPacketsError, ASN_COUNTER, RONLY, var_WirelessStatsEntry, 1, {11}},
    {OID_wirlessRXPacketsDropped, ASN_COUNTER, RONLY, var_WirelessStatsEntry, 1, {12}},
    {OID_wirlessTXPacketsError, ASN_COUNTER, RONLY, var_WirelessStatsEntry, 1, {13}},
    {OID_wirlessTXPacketsDropped, ASN_COUNTER, RONLY, var_WirelessStatsEntry, 1, {14}},
    {OID_wirelessRXPacketsDroppedRate, ASN_OCTET_STR, RONLY, var_WirelessStatsEntry, 1, {17}},
    {OID_wirelessAssocUserSum, ASN_COUNTER, RONLY, var_WirelessStatsEntry, 1, {41}}
};
oid wirelessstats_variables_oid[] = {IGD_OID_PREFIX, 5, 2, 1};

/*WlanClientNumber*/
struct variable3 wlanClientNumber_variables[] = {
    {OID_wlanClientNumber, ASN_INTEGER, RONLY, var_WlanClientEntry, 3, {3, 1, 1}},
    {OID_wlanClientNumberPercent, ASN_OCTET_STR, RONLY, var_WlanClientEntry, 3, {5, 1, 1}}
};
oid wlanClientNumber_variables_oid[] = {IGD_OID_PREFIX, 5, };

/*WlanClients*/
struct variable1 wlanclient_variables[] = {
    {OID_clientIndex, ASN_INTEGER, RONLY, var_WlanClient2Entry, 1, {1}},
    {OID_clientMac, ASN_OCTET_STR, RONLY, var_WlanClient2Entry, 1, {2}},
    {OID_clientRssi, ASN_INTEGER, RONLY, var_WlanClient2Entry, 1, {3}},
    {OID_clientStatus, ASN_OCTET_STR, RONLY, var_WlanClient2Entry, 1, {4}}
};
oid wlanclient_variables_oid[] = {IGD_OID_PREFIX, 5, 4, 1};

/*VapWepTable*/
struct variable1 wlanwep_variables[] = {
    {OID_wepKeyType, ASN_INTEGER, RWRITE, var_WlanWepEntry, 1, {1}},
    {OID_wepKey1, ASN_OCTET_STR, RWRITE, var_WlanWepEntry, 1, {2}},
    {OID_wepKey2, ASN_OCTET_STR, RWRITE, var_WlanWepEntry, 1, {3}},
    {OID_wepKey3, ASN_OCTET_STR, RWRITE, var_WlanWepEntry, 1, {4}},
    {OID_wepKey4 , ASN_OCTET_STR, RWRITE, var_WlanWepEntry, 1, {5}}
};
oid wlanwep_variables_oid[] = {IGD_OID_PREFIX, 6, 1, 1};
char *cstwepKeyTypeMatched[] = {"Disabled", "0", "40-bit", "40", "104-bit", "104"};

/*VapSettingTable*/
struct variable1 wlanvapencyt_variables[] = {
    {OID_vapIndex, ASN_INTEGER, NOACCESS, var_WlanVapEncrytEntry, 1, {1}},
    {OID_vapEnable, ASN_INTEGER, RWRITE, var_WlanVapEncrytEntry, 1, {2}},
    {OID_vapssid, ASN_OCTET_STR, RWRITE, var_WlanVapEncrytEntry, 1, {3}},
    {OID_hidenetworkname, ASN_INTEGER, RWRITE, var_WlanVapEncrytEntry, 1, {4}},
    {OID_presharekey, ASN_OCTET_STR, RWRITE, var_WlanVapEncrytEntry, 1, {7}},
    {OID_authenticationtype, ASN_INTEGER, RWRITE, var_WlanVapEncrytEntry, 1, {8}},
    {OID_encryption, ASN_INTEGER, RWRITE, var_WlanVapEncrytEntry, 1, {9}},
    {OID_wanseparator, ASN_INTEGER, RWRITE, var_WlanVapEncrytEntry, 1, {11}},
    {OID_vapMacAddr, ASN_OCTET_STR, RONLY, var_WlanVapEncrytEntry, 1, {14}}
};
oid wlanvapencyt_variables_oid[] = {IGD_OID_PREFIX, 6, 2, 1};

/*VapDataTable*/
struct variable1 wlanvapdata_variables[] = {
    {OID_vapMtu, ASN_INTEGER, RONLY, var_WlanVapDataEntry, 1, {5}},
    {OID_vapWirelessRXBytesTotal, ASN_COUNTER, RONLY, var_WlanVapDataEntry, 1, {15}},
    {OID_vapWirelessTXBytesTotal, ASN_COUNTER, RONLY, var_WlanVapDataEntry, 1, {16}},
    {OID_vapWirelessRXPacketsTotal, ASN_COUNTER, RONLY, var_WlanVapDataEntry, 1, {17}},
    {OID_vapWirelessTXPacketsTotal, ASN_COUNTER, RONLY, var_WlanVapDataEntry, 1, {18}},
    {OID_vapClientCnt, ASN_INTEGER, RONLY, var_WlanVapDataEntry, 1, {19}}
};
oid wlanvapdata_variables_oid[] = {IGD_OID_PREFIX, 6, 3, 1};

#endif

/* wlan table */
struct variable1 wlanInfoTable_variables[] = {
    {OID_wlanSSID, ASN_OCTET_STR, RWRITE, var_WlanInfoTblDataEntry, 1, {1}},
    {OID_wlanSSIDBroadcast, ASN_INTEGER, RONLY, var_WlanInfoTblDataEntry, 1, {2}},
    {OID_wlanSSIDStatus,ASN_INTEGER, RWRITE, var_WlanInfoTblDataEntry, 1, {3}},
    {OID_wlanStatus, ASN_INTEGER, RWRITE, var_WlanInfoEnableEntry, 1, {4}},
    {OID_wlanNetMode, ASN_INTEGER, RONLY, var_WlanInfoTblDataEntry, 1, {5}},
};

/* wanInfoTable */
struct variable1 wanInfoTable_variables[] = {
   {OID_wanInfoIndex, ASN_INTEGER, RWRITE, var_WanInfoTblDataEntry, 1, {1}},
   {OID_wanConType, ASN_OCTET_STR, RWRITE, var_WanInfoTblDataEntry, 1, {2}},
   {OID_wanIpAddr, ASN_IPADDRESS, RWRITE, var_WanInfoTblDataEntry, 1, {3}},
   {OID_wanNetMask, ASN_IPADDRESS, RWRITE, var_WanInfoTblDataEntry, 1, {4}},
   {OID_wanDefGateWay, ASN_IPADDRESS, RWRITE, var_WanInfoTblDataEntry, 1, {5}},
   {OID_wanPrimaryDNS, ASN_IPADDRESS, RONLY, var_WanInfoTblDataEntry, 1, {6}},
   {OID_wanSecondaryDNS, ASN_IPADDRESS, RONLY, var_WanInfoTblDataEntry, 1, {7}},
   {OID_wanMtuValue, ASN_INTEGER, RONLY, var_WanInfoTblDataEntry, 1, {8}}
};

struct variable1 vlanInfoTable_variables[] = {
   {OID_vlanId, ASN_INTEGER, RONLY, var_VlanInfoTblDataEntry, 1, {1}},
   {OID_portList, ASN_OCTET_STR, RONLY, var_VlanInfoTblDataEntry, 1, {2}}
};


oid ipRouteTblData_variables_oid[] = {IGD_IPRoute_PREFIX};
oid wanInfoTblData_variables_oid[] = {1,3,6,1,4,1,33003,2,1,1};
oid wlanInfoTblData_variables_oid[] = {1,3,6,1,4,1,33003,4,1,1};
oid vlanInfoTblData_variables_oid[] = {1,3,6,1,4,1,33003,3,1,1};
static unsigned long p_ulWirelessValue = 0;
static char p_szWirelessValue[256] = {0};
static char p_szWirelessPath[256] = {0};

static unsigned long p_ulAdslValue = 0;

#ifndef TBS_SNMP_APP

static wlanBasicSettingTable_data stWlanBasicTbl;
static wlanBasicSettingTable_data *pstWlanBasicTbl = &stWlanBasicTbl; 
static lstList_t stWlanBasicList;

static wlanVapSettingTable_data stWlanVapTbl;
static wlanVapSettingTable_data *pstWlanVapTbl = &stWlanVapTbl;
static lstList_t stWlanVapList;

static wirelessStatsTable_data stWirelessStatsTbl;
static wirelessStatsTable_data *pstWirelessStatsTbl = &stWirelessStatsTbl;
static lstList_t stWirelessStatsList;

static wlanClientTable_data stWlanClientTbl;
static wlanClientTable_data *pstWlanClientTbl = &stWlanClientTbl;
static lstList_t stWlanClientList;

static wlanClientNumberTable_data stWlanClientNumberTbl;
static wlanClientNumberTable_data *pstWlanClientNumberTbl = &stWlanClientNumberTbl;
static lstList_t stWlanClientNumberList;

static wlanWepTable_data stWlanWepTbl;
static wlanWepTable_data *pstWlanWepTbl = &stWlanWepTbl;
static lstList_t stWlanWepList;

static wlanVapTable_data stWlanVapEncytTbl;
static wlanVapTable_data *pstWlanVapEncytTbl = &stWlanVapEncytTbl;
static lstList_t stWlanVapEncytList;

static wlanVapDataTable_data stWlanVapDataTbl;
static wlanVapDataTable_data *pstWlanVapDataTbl = &stWlanVapDataTbl;
static lstList_t stWlanVapDataList;

#endif

static wanInfoTable_data stWanInfoDataTbl;
static wanInfoTable_data *pstWanInfoDataTbl = &stWanInfoDataTbl;
static lstList_t stWanInfoDataList;

static wlanInfoTable_data stWlanInfoDataTbl;
static wlanInfoTable_data *pstWlanInfoDataTbl = &stWlanInfoDataTbl;
static lstList_t stWlanInfoDataList;

static vlanInfoTable_data stVlanInfoDataTbl;
static vlanInfoTable_data *pstVlanInfoDataTbl = &stVlanInfoDataTbl;
static lstList_t stVlanInfoDataList;
void init_remoteNodes(void)
{
    #ifndef TBS_SNMP_APP
    REGISTER_MIB("radiusNodes",  radius_variables, variable2, radius_variables_oid);
    REGISTER_MIB("wlanNodes",  wlan_variables, variable3, wlan_variables_oid);
    REGISTER_MIB("vapNodes",  vap_variables, variable1, vap_variables_oid);
    REGISTER_MIB("ethNodes",  ethstats_variables, variable1, ethstats_variables_oid);
    REGISTER_MIB("wirelessNodes",  wirelessstats_variables, variable1, wirelessstats_variables_oid);
    REGISTER_MIB("wlanClientNumberNodes",  wlanClientNumber_variables, variable3, wlanClientNumber_variables_oid);
    REGISTER_MIB("wlanClientsNodes",  wlanclient_variables, variable1, wlanclient_variables_oid);
    REGISTER_MIB("wlanWepNodes",  wlanwep_variables, variable1, wlanwep_variables_oid);
    REGISTER_MIB("wlanVapEncytNodes",  wlanvapencyt_variables, variable1, wlanvapencyt_variables_oid);
    REGISTER_MIB("wlanVapDatatNodes",  wlanvapdata_variables, variable1, wlanvapdata_variables_oid);
    
    stWlanBasicTbl.cachTime = 0;
    stWlanVapTbl.cachTime = 0;
    stWirelessStatsTbl.cachTime = 0;
    stWlanClientTbl.cachTime = 0;
    stWlanVapEncytTbl.cachTime = 0;
    stWlanVapDataTbl.cachTime = 0;
	#endif
    REGISTER_MIB("wanInfoDataTblNodes",  wanInfoTable_variables, variable1, wanInfoTblData_variables_oid);
    REGISTER_MIB("wlanInfoDataTblNodes", wlanInfoTable_variables, variable1, wlanInfoTblData_variables_oid);
    REGISTER_MIB("vlanInfoDataTblNodes", vlanInfoTable_variables, variable1, vlanInfoTblData_variables_oid);
	
    stWanInfoDataTbl.cachTime = 0;
    stWlanInfoDataTbl.cachTime = 0;
    stVlanInfoDataTbl.cachTime = 0;
    Init_WriteMethodStruct();
    #ifndef TBS_SNMP_APP
    {
    char cIfName[32] = {0};
    unsigned long instid = 0;
    /* 取出有线端接口名称，然后执行ifconfig */
    if (0!=GetActiveWanConnDevice(cIfName, sizeof(cIfName), &instid))
    {
        SNMPA_TRACE("Failed to retrieve the wan interface name!\n");
        return;
    }
    IfConfigRead(cIfName, NULL, "TX bytes", ":", p_szWirelessValue, sizeof(p_szWirelessValue));
    g_ultxBytes[0] = safe_atoi(p_szWirelessValue);
    g_ultxBytes[1] = times(NULL);

    IfConfigRead(cIfName, NULL, "RX bytes", ":", p_szWirelessValue, sizeof(p_szWirelessValue));
    g_ulrxBytes[0] = safe_atoi(p_szWirelessValue);
    g_ulrxBytes[1] = times(NULL);    
    }
    #endif
}

void shutdown_remoteNodes(void)
{
    Free_WriteMethodStruct();
}

#ifndef TBS_SNMP_APP
static int loadWlan_BasicTable(void)
{
    int i = 0;
    int ret = SNMP_ERR_NOERROR;
    char **ppWlanBasicList = NULL;
    char *pszWlanBasicNodePrefix = NULL;

    IS_CACH_TIMEOUT(stWlanBasicTbl.cachTime);

    ppWlanBasicList = CMM_GetInstanceList(TR069_ROOT_VapTable);
    if(!ppWlanBasicList) return -1;

    for(; ppWlanBasicList[i]; i++)
    {
        pszWlanBasicNodePrefix = ppWlanBasicList[i];
        stWlanBasicTbl.wlanInstanceID  = atoi(pszWlanBasicNodePrefix+sizeof(TR069_ROOT_VapTable) - 1); //Get the instance NO.

        /*Get values for all leaf ndoes under <pszWlanBasicNodePrefix> patrial path*/
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_wirelessMode, &(stWlanBasicTbl.wirelessMode),
                                            cszWirelessModeMatched, GET_ARRAY_BOUNADRY(cszWirelessModeMatched)) != CMM_SUCCESS)
        {
             ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_channel, &(stWlanBasicTbl.channel),
                                            NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_txRate, &(stWlanBasicTbl.txRate), 
                                            csztxRateMatched, GET_ARRAY_BOUNADRY(csztxRateMatched)) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_fragmentationThreshold, 
                                            &(stWlanBasicTbl.fragmentationThreshold), NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_rtsThreshold, &(stWlanBasicTbl.rtsThreshold),
                                            NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_txPower, &(stWlanBasicTbl.txPower), 
                                            csztxPowerMatched, GET_ARRAY_BOUNADRY(csztxPowerMatched)) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_superMode, &(stWlanBasicTbl.superMode), NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_wmmSupport, &(stWlanBasicTbl.wmmSupport), NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_linkintegrity, &(stWlanBasicTbl.linkintegrity), NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_wlanMaxStaNum, &(stWlanBasicTbl.wlanMaxStaNum), NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanBasicNodePrefix, TR069_NODE_radio, &(stWlanBasicTbl.radio), NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }

        TW_addList((char *)&stWlanBasicList, (char *)&stWlanBasicTbl, sizeof(stWlanBasicTbl), sizeof(long));
    }
PExit:    
    CMM_FreeInstanceList(ppWlanBasicList);

    return ret;
}

static int loadWlan_VapTable(void)
{
    int i = 0;
    int ret = SNMP_ERR_NOERROR;
    char **ppWlanVapList = NULL;
    char *pszWlanVapNodePrefix = NULL;

    IS_CACH_TIMEOUT(stWlanVapTbl.cachTime);

    ppWlanVapList = CMM_GetInstanceList(TR069_ROOT_VapTable);
    if(!ppWlanVapList) return -1;

    for(; ppWlanVapList[i]; i++)
    {
        pszWlanVapNodePrefix = ppWlanVapList[i];
        stWlanVapTbl.vapInstanceId  = atoi(pszWlanVapNodePrefix+sizeof(TR069_ROOT_VapTable) - 1); //Get the instance NO.

        /*Get values for all leaf ndoes under <pszWlanVapNodePrefix> patrial path*/
        if(CMM_GetIntColumn(pszWlanVapNodePrefix, TR069_NODE_broadcastSsid, &(stWlanVapTbl.broadcastSsid),
                                            cszbroadcastSsid, GET_ARRAY_BOUNADRY(cszbroadcastSsid)) != CMM_SUCCESS)
        {
             ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanVapNodePrefix, TR069_NODE_beaconInterval, &(stWlanVapTbl.beaconInterval),
                                            NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanVapNodePrefix, TR069_NODE_dtim, &(stWlanVapTbl.dtim), NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanVapNodePrefix, TR069_NODE_deviceOperationMode, &(stWlanVapTbl.accessControlMode),
                                            cszdeviceOperationModeMatched, GET_ARRAY_BOUNADRY(cszdeviceOperationModeMatched)) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        TW_addList((char *)&stWlanVapList, (char *)&stWlanVapTbl, sizeof(stWlanVapTbl), sizeof(long));
    }
PExit:    
    CMM_FreeInstanceList(ppWlanVapList);

    return ret;
}

static int loadWireless_StatsTable(void)
{
    int i = 0;
    char cIfName[32] = {0};
    unsigned long instid = 0;
    float fRxDroppedRate = 0.00;

    int ret = SNMP_ERR_NOERROR;

    IS_CACH_TIMEOUT(stWirelessStatsTbl.cachTime);

    #if WIRELESS_IF_READABLE
    /* 取出有线端接口名称，然后执行ifconfig */
    if (0!=GetActiveWanConnDevice(cIfName, sizeof(cIfName), &instid))
    {
        SNMPA_TRACE("Failed to retrieve the wan interface name!\n");
        return NULL;
    }
    #else
    strcpy(cIfName, "wifi0"); /*这里先用固定的*/
    #endif

    stWirelessStatsTbl.wirelessStatsInstanceId  = 1; //Get the instance NO, 1 by default

    if(IfConfigRead(cIfName, NULL, "RX packets", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
    {
        ret = SNMP_ERR_GENERR;
        goto PExit;
    }
    stWirelessStatsTbl.wirelessRXPacketsTotal = safe_atoi(p_szWirelessValue);

    if(IfConfigRead(cIfName, NULL, "RX bytes", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
    {
        ret = SNMP_ERR_GENERR;
        goto PExit;
    }
    stWirelessStatsTbl.wirelessRXBytesTotal = safe_atoi(p_szWirelessValue);

    if(IfConfigRead(cIfName, NULL, "TX packets", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
    {
        ret = SNMP_ERR_GENERR;
        goto PExit;
    }
    stWirelessStatsTbl.wirelessTXPacketsTotal = safe_atoi(p_szWirelessValue);

    if(IfConfigRead(cIfName, NULL, "TX bytes", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
    {
        ret = SNMP_ERR_GENERR;
        goto PExit;
    }
    stWirelessStatsTbl.wirelessTXBytesTotal = safe_atoi(p_szWirelessValue);

    if(IfConfigRead(cIfName, "RX packets", "errors", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
    {
        ret = SNMP_ERR_GENERR;
        goto PExit;
    }
    stWirelessStatsTbl.wirlessRXPacketsError = safe_atoi(p_szWirelessValue);

    if(IfConfigRead(cIfName, "RX packets", "dropped", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
    {
        ret = SNMP_ERR_GENERR;
        goto PExit;
    }
    stWirelessStatsTbl.wirlessRXPacketsDropped = safe_atoi(p_szWirelessValue);

    if(IfConfigRead(cIfName, "TX packets", "errors", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
    {
        ret = SNMP_ERR_GENERR;
        goto PExit;
    }
    stWirelessStatsTbl.wirlessTXPacketsError = safe_atoi(p_szWirelessValue);

    if(IfConfigRead(cIfName, "TX packets", "dropped", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
    {
        ret = SNMP_ERR_GENERR;
        goto PExit;
    }
    stWirelessStatsTbl.wirlessTXPacketsDropped = safe_atoi(p_szWirelessValue);
    
    fRxDroppedRate = (stWirelessStatsTbl.wirlessTXPacketsDropped*100.00) / (stWirelessStatsTbl.wirelessRXBytesTotal+1.00);
    sprintf(stWirelessStatsTbl.wirelessRXPacketsDroppedRate, "%f", fRxDroppedRate);
    strcat(stWirelessStatsTbl.wirelessRXPacketsDroppedRate, "%");

    if(CMM_GetUlong(TR069_ROOT_COM_WLAN"NeighborAPNumber", &(stWirelessStatsTbl.wirelessAssocUserSum), NULL, 0) != CMM_SUCCESS)
    {
        ret = SNMP_ERR_GENERR;
        goto PExit;
    }
    TW_addList((char *)&stWirelessStatsList, (char *)&stWirelessStatsTbl, sizeof(stWirelessStatsTbl), sizeof(long));

PExit:    
    return ret;
}

static int LoadWlan_ClientTable(void)
{
    int ret = 0;
    int i = 0, j = 0;
    unsigned long ulClientIndex = 1;
    char **ppConfigList = NULL;
    char *pszConfigNodePrefix = NULL;
    unsigned long ConfigInstanceId = 0;

    char *pszWlanClientNodePrefix = NULL;
    char cWlanClientObjName[192] = {0};
    char **ppWlanClientList = NULL;

    IS_CACH_TIMEOUT(stWlanClientTbl.cachTime);
    
    ppConfigList = CMM_GetInstanceList(TR069_ROOT_wlanConfiguration);
    if(!ppConfigList) return -1;

    for(; ret==0 && ppConfigList[i]; i++)
    {
        pszConfigNodePrefix = ppConfigList[i];
	 ConfigInstanceId = atoi(pszConfigNodePrefix + sizeof(TR069_ROOT_wlanConfiguration) - 1); //Get the instance NO.
	 sprintf(cWlanClientObjName, "%s%s.", pszConfigNodePrefix, TR069_NODE_AssociatedDevice);

	 if(!(ppWlanClientList = CMM_GetInstanceList(cWlanClientObjName)))
	 {
		ret = -1;
		break;
	 }

	 for(j=0; ppWlanClientList[j]; j++)
	 {
	      pszWlanClientNodePrefix = ppWlanClientList[j];
             #if 0
	      stWlanClientTbl.vapInstanceID = ConfigInstanceId;
	      stWlanClientTbl.radInstanceID  = atoi(pszWlanClientNodePrefix+strlen(cWlanClientObjName)); //Get the instance NO.
             #else 
             stWlanClientTbl.clientIndex = ulClientIndex ++;
             #endif

	      /*Get values for all leaf ndoes under <pszWlanClientNodePrefix> patrial path*/
	      if(CMM_GetStrColumn(pszWlanClientNodePrefix, "AssociatedDeviceMACAddress", stWlanClientTbl.clientMac, 
	                                          sizeof(stWlanClientTbl.clientMac), NULL, NULL, 0) != CMM_SUCCESS)
	      {
	          return SNMP_ERR_GENERR;
	      }
	      if(CMM_GetIntColumn(pszWlanClientNodePrefix, "X_TWSZ-COM_AssociatedDeviceRSSI", 
                                                 &(stWlanClientTbl.clientRssi), NULL, 0) != CMM_SUCCESS)
	      {
	          return SNMP_ERR_GENERR;
	      }
	      if(CMM_GetStrColumn(pszWlanClientNodePrefix, "X_TWSZ-COM_AssociatedDeviceState", stWlanClientTbl.clientStatus, 
	                                          sizeof(stWlanClientTbl.clientStatus), NULL, NULL, 0) != CMM_SUCCESS)
	      {
	          return SNMP_ERR_GENERR;
	      }
	                                              
             TW_addList((char *)&stWlanClientList, (char *)&stWlanClientTbl, sizeof(stWlanClientTbl), sizeof(long));
        }
        
        CMM_FreeInstanceList(ppWlanClientList);
    }
    CMM_FreeInstanceList(ppConfigList);

    return SNMP_ERR_NOERROR;
}

static int loadWlan_ClientNumberTable(void)
{
    int i = 0;
    int ret = SNMP_ERR_NOERROR;
    char **ppWlanClientNumberList = NULL;
    char *pszWlanClientNumberNodePrefix = NULL;

    IS_CACH_TIMEOUT(stWlanClientNumberTbl.cachTime);

    ppWlanClientNumberList = CMM_GetInstanceList(TR069_ROOT_wlanConfiguration);
    if(!ppWlanClientNumberList) return -1;

    for(; ppWlanClientNumberList[i]; i++)
    {
        pszWlanClientNumberNodePrefix = ppWlanClientNumberList[i];
        stWlanClientNumberTbl.wlanClientInstanceId  = atoi(pszWlanClientNumberNodePrefix+sizeof(TR069_ROOT_wlanConfiguration) - 1); //Get the instance NO.

        /*Get values for all leaf ndoes under <pszWlanClientNumberNodePrefix> patrial path*/
        if(CMM_GetIntColumn(pszWlanClientNumberNodePrefix, "X_TWSZ-COM_MAXSTATION", &(stWlanClientNumberTbl.wlanMaxStation),
                                            NULL, 0) != CMM_SUCCESS)
        {
             ret = SNMP_ERR_GENERR;
            goto PExit;
        }
        if(CMM_GetIntColumn(pszWlanClientNumberNodePrefix, "TotalAssociations", &(stWlanClientNumberTbl.wlanClientNumber),
                                            NULL, 0) != CMM_SUCCESS)
        {
            ret = SNMP_ERR_GENERR;
            goto PExit;
        }

        if(stWlanClientNumberTbl.wlanMaxStation > 0)
            sprintf(stWlanClientNumberTbl.wlanClientNumberPercent, "%f", 
                      stWlanClientNumberTbl.wlanClientNumber * 100.00/stWlanClientNumberTbl.wlanMaxStation);
        else sprintf(stWlanClientNumberTbl.wlanClientNumberPercent, "%s", "0.00");
        strcat(stWlanClientNumberTbl.wlanClientNumberPercent, "%");

        TW_addList((char *)&stWlanClientNumberList, (char *)&stWlanClientNumberTbl, sizeof(stWlanClientNumberTbl), sizeof(long));
    }
PExit:    
    CMM_FreeInstanceList(ppWlanClientNumberList);

    return ret;
}

/*
  return : 0 successful
              non-zero failed
*/
static int LoadWlan_WepTable(void)
{
    int ret = 0;
    int i = 0, j = 0;
    char **ppConfigList = NULL;
    char *pszConfigNodePrefix = NULL;
    unsigned long ConfigInstanceId = 0;

    char *pszWepNodePrefix = NULL;
    char cWepObjName[192] = {0};
    char **ppWepList = NULL;

    IS_CACH_TIMEOUT(stWlanWepTbl.cachTime);
    
    ppConfigList = CMM_GetInstanceList(TR069_ROOT_wlanConfiguration);
    if(!ppConfigList) return -1;

    for(; ret==0 && ppConfigList[i]; i++)
    {
        pszConfigNodePrefix = ppConfigList[i];
	 ConfigInstanceId = atoi(pszConfigNodePrefix + sizeof(TR069_ROOT_wlanConfiguration) - 1); //Get the instance NO.
	 sprintf(cWepObjName, "%s%s.", pszConfigNodePrefix, TR069_ROOT_WepKey);

	 if (!(ppWepList = CMM_GetInstanceList(cWepObjName)))
	 {
		ret = SNMP_ERR_GENERR;
		break;
	 }

        if(CMM_GetIntColumn(pszConfigNodePrefix, TR069_NODE_WepEncryptionLevel, &stWlanWepTbl.wepKeyType,  
                                            cstwepKeyTypeMatched, GET_ARRAY_BOUNADRY(cstwepKeyTypeMatched)) != CMM_SUCCESS)
        {
          ret = SNMP_ERR_GENERR;
          break;
        }

	 for(j=0; ppWepList[j]; j++)
	 {
	      pszWepNodePrefix = ppWepList[j];
	      stWlanWepTbl.configInstanceId = ConfigInstanceId;

	      /*Get values for all leaf ndoes under <pszWepNodePrefix> patrial path*/
             if(j == 0)
	      if(CMM_GetStrColumn(pszWepNodePrefix, TR069_NODE_WepKey, stWlanWepTbl.wepKey1, sizeof(stWlanWepTbl.wepKey1), NULL, 
                                                 NULL, 0) != CMM_SUCCESS)
	      {
	          return SNMP_ERR_GENERR;
	      }

             else if(j == 1)
	      if(CMM_GetStrColumn(pszWepNodePrefix, TR069_NODE_WepKey, stWlanWepTbl.wepKey2, sizeof(stWlanWepTbl.wepKey2), NULL,
                                                  NULL, 0) != CMM_SUCCESS)
	      {
	          return SNMP_ERR_GENERR;
	      }

             else if(j == 2)
	      if(CMM_GetStrColumn(pszWepNodePrefix, TR069_NODE_WepKey, stWlanWepTbl.wepKey3, sizeof(stWlanWepTbl.wepKey3), NULL,
                                                  NULL, 0) != CMM_SUCCESS)
	      {
	          return SNMP_ERR_GENERR;
	      }

             else if(j == 3)
	      if(CMM_GetStrColumn(pszWepNodePrefix, TR069_NODE_WepKey, stWlanWepTbl.wepKey4, sizeof(stWlanWepTbl.wepKey4), NULL,
                                                 NULL, 0) != CMM_SUCCESS)
	      {
	          return SNMP_ERR_GENERR;
	      }
             else break;
        }
        TW_addList((char *)&stWlanWepList, (char *)&stWlanWepTbl, sizeof(stWlanWepTbl), sizeof(long));
        
        CMM_FreeInstanceList(ppWepList);
    }
    CMM_FreeInstanceList(ppConfigList);

    return SNMP_ERR_NOERROR;
}

static int LoadWlan_VapTable(void)
{
    int i = 0;
    char **ppWlanVapEncytList = NULL;
    char *pszWlanVapEncytNodePrefix = NULL;

    IS_CACH_TIMEOUT(stWlanVapEncytTbl.cachTime);

    ppWlanVapEncytList = CMM_GetInstanceList(TR069_ROOT_wlanConfiguration);
    if(!ppWlanVapEncytList) return -1;

    for(; ppWlanVapEncytList[i]; i++)
    {
        pszWlanVapEncytNodePrefix = ppWlanVapEncytList[i];
        stWlanVapEncytTbl.vapInstanceId  = atoi(pszWlanVapEncytNodePrefix+sizeof(TR069_ROOT_wlanConfiguration)-1); //Get the instance NO.
        
        /*Get values for all leaf ndoes under <pszWlanVapEncytNodePrefix> patrial path*/
        if(CMM_GetIntColumn(pszWlanVapEncytNodePrefix, TR069_NODE_vapIndex, &stWlanVapEncytTbl.vapIndex, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        if(CMM_GetIntColumn(pszWlanVapEncytNodePrefix, TR069_NODE_vapEnable, &stWlanVapEncytTbl.vapEnable, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        if(CMM_GetStrColumn(pszWlanVapEncytNodePrefix, TR069_NODE_vapssid, stWlanVapEncytTbl.vapssid, 
                                            sizeof(stWlanVapEncytTbl.vapssid), NULL, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_GetIntColumn(pszWlanVapEncytNodePrefix, TR069_NODE_hidenetworkname, 
                                            &stWlanVapEncytTbl.hidenetworkname, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_GetStrColumn(pszWlanVapEncytNodePrefix, TR069_NODE_presharekey64Hex, stWlanVapEncytTbl.presharekey, 
                                             sizeof(stWlanVapEncytTbl.presharekey), NULL, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        if(safe_strlen(stWlanVapEncytTbl.presharekey) < 4) /* if it is less than 4, we are sure it is not valid*/
                    if(CMM_GetStrColumn(pszWlanVapEncytNodePrefix, TR069_NODE_presharekey63Asc, stWlanVapEncytTbl.presharekey, 
                                                        sizeof(stWlanVapEncytTbl.presharekey), NULL, NULL, 0) != CMM_SUCCESS)
                    {
                        return SNMP_ERR_GENERR;
                    }

        
        RN_GetWirelessAuthenticationType(pszWlanVapEncytNodePrefix, p_szWirelessValue);
        stWlanVapEncytTbl.authenticationtype = safe_atoi(p_szWirelessValue);

        RN_GetWirelessEncryptionType(pszWlanVapEncytNodePrefix, p_szWirelessValue);
        stWlanVapEncytTbl.encryption= safe_atoi(p_szWirelessValue);
        
        if(CMM_GetIntColumn(pszWlanVapEncytNodePrefix, TR069_NODE_wlanseparator, 
                                            &stWlanVapEncytTbl.wlanseparator, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        if(CMM_GetStrColumn(pszWlanVapEncytNodePrefix, TR069_NODE_vapMacAddr, stWlanVapEncytTbl.vapMacAddr,
                                             sizeof(stWlanVapEncytTbl.vapMacAddr), NULL, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        
        TW_addList((char *)&stWlanVapEncytList, (char *)&stWlanVapEncytTbl, sizeof(stWlanVapEncytTbl), sizeof(long));
    }
    CMM_FreeInstanceList(ppWlanVapEncytList);

    return SNMP_ERR_NOERROR;
}

static int LoadWlan_VapDataTable(void)
{
    int i = 0;
    char **ppWlanVapDataList = NULL;
    char *pszWlanVapDataNodePrefix = NULL;

    IS_CACH_TIMEOUT(stWlanVapDataTbl.cachTime);

    ppWlanVapDataList = CMM_GetInstanceList(TR069_ROOT_wlanConfiguration);
    if(!ppWlanVapDataList) return -1;

    for(; ppWlanVapDataList[i]; i++)
    {
        pszWlanVapDataNodePrefix = ppWlanVapDataList[i];
        stWlanVapDataTbl.vapInstanceId  = atoi(pszWlanVapDataNodePrefix+sizeof(TR069_ROOT_wlanConfiguration)-1); //Get the instance NO.


        if(IfConfigRead("wifi0", NULL, "MTU", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
        {
            return SNMP_ERR_GENERR;
        }
        stWlanVapDataTbl.vapMtu= safe_atoi(p_szWirelessValue);
        
        /*Get values for all leaf ndoes under <pszWlanVapDataNodePrefix> patrial path*/
        if(CMM_GetIntColumn(pszWlanVapDataNodePrefix, TR069_NODE_vapWirelessRXBytesTotal, 
                                            &stWlanVapDataTbl.vapWirelessRXBytesTotal, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        if(CMM_GetIntColumn(pszWlanVapDataNodePrefix, TR069_NODE_vapWirelessTXBytesTotal, 
                                            &stWlanVapDataTbl.vapWirelessTXBytesTotal, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        if(CMM_GetIntColumn(pszWlanVapDataNodePrefix, TR069_NODE_vapWirelessRXPacketsTotal, 
                                            &stWlanVapDataTbl.vapWirelessRXPacketsTotal, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_GetIntColumn(pszWlanVapDataNodePrefix, TR069_NODE_vapWirelessTXPacketsTotal, 
                                            &stWlanVapDataTbl.vapWirelessTXPacketsTotal, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_GetIntColumn(pszWlanVapDataNodePrefix, TR069_NODE_vapClientCnt, 
                                            &stWlanVapDataTbl.vapClientCnt, NULL, 0) != CMM_SUCCESS)
        {
            return SNMP_ERR_GENERR;
        }
        
        TW_addList((char *)&stWlanVapDataList, (char *)&stWlanVapDataTbl, sizeof(stWlanVapDataTbl), sizeof(long));
    }
    CMM_FreeInstanceList(ppWlanVapDataList);

    return SNMP_ERR_NOERROR;
}

#endif

static int GetConnectionType(unsigned long ulWANIPCon,char* pszWanInfoNodePrefix,unsigned char* retValue ,unsigned long ulSize)
{
    char tmpValue[20];
	memset(retValue, 0, sizeof(ulSize));
    if(CMM_GetStrColumn(pszWanInfoNodePrefix, "WANDSLLinkConfig.LinkType", tmpValue, sizeof(tmpValue) ,NULL, NULL, 0) != CMM_SUCCESS)
   	{ 
   	  return CMM_FAIL;
   	}

	if(strcmp("EoA" , tmpValue) == 0)
	{
	    if( ulWANIPCon)
	   	{
            if(CMM_GetStrColumn(pszWanInfoNodePrefix, "WANIPConnection.1.ConnectionType", tmpValue,sizeof(tmpValue), NULL, NULL, 0) != CMM_SUCCESS)
		    { 
		       	return CMM_FAIL;
		    }

		    if( strcmp("IP_Bridged" , tmpValue) == 0) 
			{ 
				memcpy(retValue, "BRIDGE", strlen("BRIDGE"));
				return CMM_SUCCESS;
			}else
			{   
				 if(CMM_GetStrColumn(pszWanInfoNodePrefix, "WANIPConnection.1.AddressingType",tmpValue,sizeof(tmpValue) ,NULL, NULL, 0) == CMM_SUCCESS)
				 {
					 memcpy(retValue, tmpValue, strlen(tmpValue));
					 return CMM_SUCCESS;
				 }
			} 
	   	}else
	   	{
	   	      memcpy(retValue, "PPPoE", strlen("PPPoE"));
		      return CMM_SUCCESS;
	   	}
	}else
	{
	    memcpy(retValue, tmpValue, strlen(tmpValue));
		return CMM_SUCCESS;
	}
	return CMM_FAIL;
}

int GetStrWanNode(unsigned long ulConnNo,char* pszWanInfoNodePrefix, char* keyid, char* retValue,unsigned long ulSize)
{
    if(ulConnNo>0)
    {
        snprintf(p_szWirelessPath ,sizeof(p_szWirelessPath) , "WANIPConnection.%s",keyid);
        if(CMM_GetStrColumn(pszWanInfoNodePrefix, p_szWirelessPath, retValue,ulSize,NULL, NULL, 0) != CMM_SUCCESS)
        { 
       	    return CMM_FAIL;
        }
    }else
    {
        snprintf(p_szWirelessPath ,sizeof(p_szWirelessPath) , "WANPPPConnection.%s",keyid);
        if(CMM_GetStrColumn(pszWanInfoNodePrefix, p_szWirelessPath, retValue,ulSize,NULL, NULL, 0) != CMM_SUCCESS)
        { 
       	    return CMM_FAIL;
        } 
     }

    return CMM_SUCCESS ;
}

int GetIpAddrWanNode(unsigned long ulConnNo,char* pszWanInfoNodePrefix, char* keyid, char* retValue,unsigned long ulSize)
{
    if(ulConnNo>0)
    {
	    snprintf(p_szWirelessPath ,sizeof(p_szWirelessPath) , "%sWANIPConnection.%s",pszWanInfoNodePrefix,keyid);
		if(CMM_GetStr(p_szWirelessPath, retValue, ulSize, NULL, 0) != CMM_SUCCESS)
	    {
              return CMM_FAIL;
	    }
		
    }else
    {
        //sWANPPPConnection  没有 SubnetMask ， DefaultGateway 需要转换为RemoteIPAddress 
        if(strstr(keyid ,"SubnetMask")!= NULL)
        {
            memset(retValue, 0, ulSize);
	        memcpy(retValue, "255.255.255.255", strlen("255.255.255.255"));
        }else if(strstr(keyid,"DefaultGateway")!= NULL)
        {
            snprintf(p_szWirelessPath ,sizeof(p_szWirelessPath) , "%sWANPPPConnection.1.RemoteIPAddress",pszWanInfoNodePrefix);
			if(CMM_GetStr(p_szWirelessPath, retValue, ulSize, NULL, 0) != CMM_SUCCESS)
		    { 
	             return CMM_FAIL;
		    }
        }else
        {
            snprintf(p_szWirelessPath ,sizeof(p_szWirelessPath) , "%sWANPPPConnection.%s",pszWanInfoNodePrefix,keyid);
			if(CMM_GetStr(p_szWirelessPath, retValue, ulSize, NULL, 0) != CMM_SUCCESS)
		    { 
	             return CMM_FAIL;
		    }
        }
     }
     return CMM_SUCCESS ;
}

static int loadTbs_WlanInfoTable(void)
{
    int i = 0;
    int ret = SNMP_ERR_NOERROR;
    char **ppWlanInfoList = NULL;
    char *pszWlanInfoNodePrefix = NULL;
	char szMode[20] ={0} ;

    IS_CACH_TIMEOUT(stWlanInfoDataTbl.cachTime);
	TW_flushList((char *)&stWlanInfoDataList);
    ppWlanInfoList = CMM_GetInstanceList(TR069_ROOT_wlanInfoTbl);
    if(!ppWlanInfoList) return -1;

    for(; ppWlanInfoList[i]; i++)
    {
	        pszWlanInfoNodePrefix = ppWlanInfoList[i];
	        stWlanInfoDataTbl.wlanInstanceId  = atoi(pszWlanInfoNodePrefix+sizeof(TR069_ROOT_wlanInfoTbl) - 1); //Get the instance NO.

            if(CMM_GetStrColumn(pszWlanInfoNodePrefix, "SSID", stWlanInfoDataTbl.wlanSSID, 
		                                        sizeof(stWlanInfoDataTbl.wlanSSID), NULL, NULL, 0) != CMM_SUCCESS)
	        {
	            ret = SNMP_ERR_GENERR;
	            goto PExit; 
	        }

			/*Get values for all leaf ndoes under <pszIpRouteNodePrefix> patrial path*/
	        if(CMM_GetIntColumn(pszWlanInfoNodePrefix, "X_TWSZ-COM_HideSSID", &(stWlanInfoDataTbl.wlanSSIDBroadcast),NULL, 0) != CMM_SUCCESS)
	        {
	            ret = SNMP_ERR_GENERR;
	            goto PExit;
	        }
			if(CMM_GetIntColumn(pszWlanInfoNodePrefix, "Enable", &(stWlanInfoDataTbl.wlanSSIDStatus), NULL, 0) != CMM_SUCCESS)
	        {
	            ret = SNMP_ERR_GENERR;
	            goto PExit;
	        }
			/*if(CMM_GetIntColumn(pszWlanInfoNodePrefix, "Status", &(stWlanInfoDataTbl.wlanStatus),
	                                            NULL, 0) != CMM_SUCCESS)
	        {
	            ret = SNMP_ERR_GENERR;
	            goto PExit;
	        }*/
			if(CMM_GetStrColumn(pszWlanInfoNodePrefix, "Standard", szMode, sizeof(szMode), NULL, NULL, 0) != CMM_SUCCESS)
			{
			    ret = SNMP_ERR_GENERR;
	            goto PExit;
			}

			stWlanInfoDataTbl.wlanNetMode = 1 ; 
			if( strcmp(szMode, "b") == 0 )
			{
			 	stWlanInfoDataTbl.wlanNetMode = 1 ;
			}else if(strcmp(szMode, "g")== 0 )
			{
			 	stWlanInfoDataTbl.wlanNetMode = 2 ;
			}else if(strcmp(szMode, "n")== 0 ) 
			{
			 	stWlanInfoDataTbl.wlanNetMode = 3 ;
			}else if(strcmp(szMode, "bg")== 0 )
			{
			 	stWlanInfoDataTbl.wlanNetMode = 4 ;
			}else if(strcmp(szMode, "ng")== 0 )
			{
			 	stWlanInfoDataTbl.wlanNetMode = 5 ;
			}else if(strcmp(szMode, "bgn")== 0 )
			{
			 	stWlanInfoDataTbl.wlanNetMode = 6 ; 
			}

	        TW_addList((char *)&stWlanInfoDataList, (char *)&stWlanInfoDataTbl, sizeof(stWlanInfoDataTbl), sizeof(long));
	}
	PExit:    
	    CMM_FreeInstanceList(ppWlanInfoList);

	    return ret;
}

static int loadTbs_WanInfoTable(void)
{
    int i = 0;
    int ret = SNMP_ERR_NOERROR;
    char **ppWanInfoList = NULL;
    char *pszWanInfoNodePrefix = NULL;
	unsigned long ulConn ;
	char szDns[32] ;
	char* ptr ;

    IS_CACH_TIMEOUT(stWanInfoDataTbl.cachTime);
	TW_flushList((char *)&stWanInfoDataList);
    ppWanInfoList = CMM_GetInstanceList(TR069_ROOT_wanInfoTbl );
    if(!ppWanInfoList) return -1;

    for(; ppWanInfoList[i]; i++)
    {
        pszWanInfoNodePrefix = ppWanInfoList[i];
        stWanInfoDataTbl.wanInstanceId  = atoi(pszWanInfoNodePrefix+sizeof(TR069_ROOT_wanInfoTbl) - 1); //Get the instance NO.
        stWanInfoDataTbl.wanInfoIndex  = stWanInfoDataTbl.wanInstanceId;
	
        /* test if WANIPConnection or WANPPPConnection */
		if(CMM_GetIntColumn(pszWanInfoNodePrefix, "WANIPConnectionNumberOfEntries", &ulConn,NULL, 0) != CMM_SUCCESS) \
		{ 
		    ret = SNMP_ERR_GENERR;
	        goto PExit; 
		}

        //get mtu
        if(ulConn>0)
	    {
		    stWanInfoDataTbl.wanMtuValue =  0 ;
	    }else
	    {
		    snprintf(p_szWirelessPath ,sizeof(p_szWirelessPath) , "%sWANPPPConnection.",pszWanInfoNodePrefix);
	        if(CMM_GetIntColumn(p_szWirelessPath, "1.MaxMTUSize", &stWanInfoDataTbl.wanMtuValue, NULL, 0) != CMM_SUCCESS)
		    {
		         ret = SNMP_ERR_GENERR;
			     goto PExit;
		    }
	    }
    
		if( GetConnectionType(ulConn,pszWanInfoNodePrefix, stWanInfoDataTbl.wanConType , sizeof(stWanInfoDataTbl.wanConType)) != CMM_SUCCESS)
		{
			ret = SNMP_ERR_GENERR;
			goto PExit;
		}

		if(GetIpAddrWanNode(ulConn,pszWanInfoNodePrefix,"1.ExternalIPAddress",  stWanInfoDataTbl.wanIpAddr,sizeof(stWanInfoDataTbl.wanIpAddr)) != CMM_SUCCESS )
		{ 
		    ret = SNMP_ERR_GENERR; 
	        goto PExit; 
		}

        if(GetIpAddrWanNode(ulConn,pszWanInfoNodePrefix,"1.SubnetMask", stWanInfoDataTbl.wanNetMask,sizeof(stWanInfoDataTbl.wanNetMask)) != CMM_SUCCESS )
		{
		    ret = SNMP_ERR_GENERR; 
	        goto PExit; 
		}

		if(GetIpAddrWanNode(ulConn,pszWanInfoNodePrefix,"1.DefaultGateway", 
			        stWanInfoDataTbl.wanDefGateWay,sizeof(stWanInfoDataTbl.wanDefGateWay)) != CMM_SUCCESS )
		{
		    ret = SNMP_ERR_GENERR; 
	        goto PExit; 
		}

		if(GetStrWanNode(ulConn,pszWanInfoNodePrefix,"1.DNSServers", szDns,sizeof(szDns))!= CMM_SUCCESS )
		{
		    ret = SNMP_ERR_GENERR; 
	        goto PExit; 
		}
		
		if(strlen(szDns)<2)
		{
            if(GetStrWanNode(ulConn,pszWanInfoNodePrefix,"1.X_TWSZ-COM_UsrDNSServers", szDns,sizeof(szDns))!= CMM_SUCCESS )
			{
			    ret = SNMP_ERR_GENERR; 
		        goto PExit; 
			}
		}

        memset(stWanInfoDataTbl.wanPrimaryDNS,0 ,sizeof(stWanInfoDataTbl.wanPrimaryDNS));
		memset(stWanInfoDataTbl.wanSecondaryDNS,0 ,sizeof(stWanInfoDataTbl.wanSecondaryDNS));
		
	    if((ptr=strstr(szDns, ",")) != NULL)
		{
            memcpy(stWanInfoDataTbl.wanPrimaryDNS ,szDns , ptr- szDns);
		    ++ptr ;
		    memcpy(stWanInfoDataTbl.wanSecondaryDNS,ptr, strlen(ptr));
		}
		
        TW_addList((char *)&stWanInfoDataList, (char *)&stWanInfoDataTbl, sizeof(stWanInfoDataTbl), sizeof(long));
    }
PExit:    
    CMM_FreeInstanceList(ppWanInfoList);
    return ret;
}

static int loadTbs_VlanInfoTable(void)
{
    int i = 0;
    int ret = SNMP_ERR_NOERROR;
    char **ppVlanInfoList = NULL;
    char *pszVlanInfoNodePrefix = NULL;
    unsigned long  ulVlanId;
    char szDeviceName[32];
    char *pcPorts ;
	char szIfName[50] = {0} ;
	
    IS_CACH_TIMEOUT(stVlanInfoDataTbl.cachTime);
	TW_flushList((char *)&stVlanInfoDataList);
    ppVlanInfoList = CMM_GetInstanceList(TR069_ROOT_wanInfoTbl);
    if(!ppVlanInfoList) return -1;

    for(; ppVlanInfoList[i]; i++)
    {
        pszVlanInfoNodePrefix = ppVlanInfoList[i];
        if(CMM_GetIntColumn(pszVlanInfoNodePrefix, "X_TWSZ-COM_VLANID", &ulVlanId,NULL, 0) != CMM_SUCCESS)
		{ 
       	    ret = SNMP_ERR_GENERR; 
	        goto PExit; 
        }

		if(ulVlanId>0)
		{ 
		    if(CMM_GetStrColumn(pszVlanInfoNodePrefix, "X_TWSZ-COM_DeviceName", szIfName,sizeof(szIfName),NULL, NULL, 0) != CMM_SUCCESS)
	        { 
	       	    ret = SNMP_ERR_GENERR;
	            goto PExit; 
	       	}

            for(hdlNode=LIST_FIRST(&stVlanInfoDataList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
		    {
			    pstVlanInfoDataTbl = (vlanInfoTable_data*) hdlNode->data;
			    if( pstVlanInfoDataTbl->vlanId == ulVlanId )
				{
					pcPorts = pstVlanInfoDataTbl->portList ;
					sprintf(pcPorts + strlen(pcPorts), ",%s",szIfName);
					break ;
				}		
		    }

			if(NULL == hdlNode)
			{
               stVlanInfoDataTbl.vlanInstanceId = ulVlanId; //Get the instance NO.
		       stVlanInfoDataTbl.vlanId = ulVlanId ;
               memset(stVlanInfoDataTbl.portList ,0 ,sizeof(stVlanInfoDataTbl.portList));
			   memcpy(stVlanInfoDataTbl.portList, szIfName, strlen(szIfName));
			   TW_addList((char *)&stVlanInfoDataList, (char *)&stVlanInfoDataTbl, sizeof(stVlanInfoDataTbl), sizeof(long));
			}
		}
    }
PExit:    
    CMM_FreeInstanceList(ppVlanInfoList);

    return ret;
}

#ifndef TBS_SNMP_APP
unsigned char *var_RadiusNodesEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    if(header_generic(vp, name, length, exact, var_len, write_method) ==  MATCH_FAILED)
        return NULL;

#define RADIUS_STR_WRITEABLE_NODE(size) \
do \
{ \
    *write_method = WriteMethod_Comm; \
    Active_WriteMethod(vp->type, vp->acl, 0, size, 0, NULL, p_szWirelessPath, NULL, 0); \
    if(CMM_GetStr(p_szWirelessPath, p_szWirelessValue, sizeof(p_szWirelessValue), NULL, 0) == CMM_SUCCESS) \
    { \
        if(vp->type == ASN_IPADDRESS) \
        { \
            *var_len = sizeof(unsigned long); \
            return CHARS_TO_intIPADDR(p_szWirelessValue); \
        } \
        else \
        { \
            *var_len = strlen(p_szWirelessValue); \
            return (unsigned char*)p_szWirelessValue; \
        } \
    } \
    else return NULL; \
}while(0)

#define RADUIS_INT_WRITEABLE_NODE(min, max) \
do \
{ \
*write_method = WriteMethod_Comm; \
Active_WriteMethod(vp->type, vp->acl, 1, min, max, NULL, p_szWirelessPath, NULL, 0); \
if(CMM_GetUlong(p_szWirelessPath, &p_ulWirelessValue, NULL, 0) == CMM_SUCCESS) \
{ \
return (unsigned char*)&p_ulWirelessValue; \
} \
else return NULL; \
}while(0)

    switch(vp->magic)
    {
        case OID_radiusAuthPrimaryServerIP:
        case OID_radiusAccountPrimaryServerIP:
            sprintf(p_szWirelessPath, TR069_ROOT_Radius"1.X_TWSZ-COM_IPAddress"); 
            RADIUS_STR_WRITEABLE_NODE(32);
        case OID_radiusAuthPrimaryServerPort:
            sprintf(p_szWirelessPath, TR069_ROOT_Radius"1.X_TWSZ-COM_Port"); 
            RADUIS_INT_WRITEABLE_NODE(0, 0);
        case OID_radiusAccountPrimaryServerPort:    
            sprintf(p_szWirelessPath, TR069_ROOT_Radius"1.X_TWSZ-COM_AccountPort"); 
            RADUIS_INT_WRITEABLE_NODE(0, 0);
        case OID_radiusAuthPrimaryServerSecret:
        case OID_radiusAccountPrimaryServerSecret:
            sprintf(p_szWirelessPath, TR069_ROOT_Radius"1.X_TWSZ-COM_ShareKey"); 
            RADIUS_STR_WRITEABLE_NODE(1000);
        case OID_radiusAuthSecondaryServerIP:
        case OID_radiusAccountSecondaryServerIP:
            sprintf(p_szWirelessPath, TR069_ROOT_Radius"2.X_TWSZ-COM_IPAddress"); 
            RADIUS_STR_WRITEABLE_NODE(32);
        case OID_radiusAuthSecondaryServerPort:
            sprintf(p_szWirelessPath, TR069_ROOT_Radius"2.X_TWSZ-COM_Port"); 
            RADUIS_INT_WRITEABLE_NODE(0, 0);
        case OID_radiusAccountSecondaryServerPort:
            sprintf(p_szWirelessPath, TR069_ROOT_Radius"2.X_TWSZ-COM_AccountPort"); 
            RADUIS_INT_WRITEABLE_NODE(0, 0);
        case OID_radiusAuthSecondaryServerSecret:
        case OID_radiusAccountSecondaryServerSecret:
            sprintf(p_szWirelessPath, TR069_ROOT_Radius"2.X_TWSZ-COM_ShareKey"); 
            RADIUS_STR_WRITEABLE_NODE(1000);
        default:
            break;
    }

    return NULL;
}

unsigned char *var_WlanNodesEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};     // OID to be returned

    if(loadWlan_BasicTable() != SNMP_ERR_NOERROR) 
    {
        SNMPA_TRACE("Failed to exec loadWlan_BasicTable, it return error!\n");
        return NULL;
    }

    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWlanBasicList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWlanBasicTbl = (wlanBasicSettingTable_data*) hdlNode->data;
        rName[vp->namelen] = pstWlanBasicTbl->wlanInstanceID; /*we are sure instance id is always with index <vp->namelen>*/

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
	     memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
	     *length = vp->namelen + 1;
	     *var_len = sizeof (long);  
         
            switch(vp->magic)
            {
                case OID_wirelessMode:
                    #if 0
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_wirelessMode); 
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 3, NULL, p_szWirelessPath, 
                                                      cszWirelessModeMatched, GET_ARRAY_BOUNADRY(cszWirelessModeMatched));
                    #endif
                    return (unsigned char*)&(pstWlanBasicTbl->wirelessMode);
                case OID_channel:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_channel, pstWlanBasicTbl->wlanInstanceID);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 1, 165, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanBasicTbl->channel);
                case OID_txRate:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_txRate, pstWlanBasicTbl->wlanInstanceID); 
                    return (unsigned char*)&(pstWlanBasicTbl->txRate);
                case OID_fragmentationThreshold:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_fragmentationThreshold, pstWlanBasicTbl->wlanInstanceID);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 256, 2346, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanBasicTbl->fragmentationThreshold);
                case OID_rtsThreshold:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_rtsThreshold, pstWlanBasicTbl->wlanInstanceID);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 256, 2346, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanBasicTbl->rtsThreshold);
                case OID_txPower:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_txPower, pstWlanBasicTbl->wlanInstanceID);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 4, NULL, p_szWirelessPath, 
                                                      csztxPowerMatched, GET_ARRAY_BOUNADRY(csztxPowerMatched));
                    return (unsigned char*)&(pstWlanBasicTbl->txPower);
                case OID_superMode:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_superMode, pstWlanBasicTbl->wlanInstanceID);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanBasicTbl->superMode);
                case OID_wmmSupport:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_wmmSupport, pstWlanBasicTbl->wlanInstanceID);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanBasicTbl->wmmSupport);
                case OID_linkintegrity:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_linkintegrity, pstWlanBasicTbl->wlanInstanceID);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanBasicTbl->linkintegrity);
                case OID_wlanMaxStaNum:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_wlanMaxStaNum, pstWlanBasicTbl->wlanInstanceID);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 32, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanBasicTbl->wlanMaxStaNum);
                case OID_radio:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_radio, pstWlanBasicTbl->wlanInstanceID);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanBasicTbl->radio);
                default:
                    break;
            }
        }
    }

    return NULL;
}

unsigned char *var_VapNodesEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};     // OID to be returned

    if(loadWlan_VapTable() != SNMP_ERR_NOERROR) 
    {
        SNMPA_TRACE("Failed to exec loadWlan_VapTable, it return error!\n");
        return NULL;
    }

    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWlanVapList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWlanVapTbl = (wlanVapSettingTable_data*) hdlNode->data;
        rName[vp->namelen] = pstWlanVapTbl->vapInstanceId; /*we are sure instance id is always with index <vp->namelen>*/

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
	     memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
	     *length = vp->namelen + 1;
	     *var_len = sizeof (long);  
         
            switch(vp->magic)
            {
                case OID_broadcastSsid:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_broadcastSsid, pstWlanVapTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, 
                                                      cszbroadcastSsid, GET_ARRAY_BOUNADRY(cszbroadcastSsid));
                    return (unsigned char*)&(pstWlanVapTbl->broadcastSsid);
                case OID_beaconInterval:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_beaconInterval, pstWlanVapTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 20, 1000, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanVapTbl->beaconInterval);
                case OID_dtim:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_dtim, pstWlanVapTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 60, 65535, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&(pstWlanVapTbl->dtim);
                case OID_deviceOperationMode:
                    sprintf(p_szWirelessPath, TR069_ROOT_VapTable"%lu."TR069_NODE_deviceOperationMode, pstWlanVapTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 3, NULL, p_szWirelessPath, 
                                                      cszdeviceOperationModeMatched, GET_ARRAY_BOUNADRY(cszdeviceOperationModeMatched));
                    return (unsigned char*)&(pstWlanVapTbl->accessControlMode);
                default:
                    break;
            }
        }
    }

    return NULL;
}

unsigned char *var_EthStatsEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    char cIfName[32] = {0};
    unsigned long instid = 0;

    if(header_generic(vp, name, length, exact, var_len, write_method) ==  MATCH_FAILED)
        return NULL;

    /* 取出有线端接口名称，然后执行ifconfig */
    if (0!=GetActiveWanConnDevice(cIfName, sizeof(cIfName), &instid))
    {
        SNMPA_TRACE("Failed to retrieve the wan interface name!\n");
        return NULL;
    }

    *write_method = NULL;
    *var_len = sizeof(unsigned long);
    switch(vp->magic)
    {
        case OID_ethernetRXPackets:
            if(IfConfigRead(cIfName, NULL, "RX packets", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        case OID_ethernetRXBytes:
            if(IfConfigRead(cIfName, NULL, "RX bytes", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        case OID_ethernetTXPackets:
            if(IfConfigRead(cIfName, NULL, "TX packets", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        case OID_ethernetTXBytes:
            if(IfConfigRead(cIfName, NULL, "TX bytes", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        case OID_ethernetRXPacketsError:
            if(IfConfigRead(cIfName, "RX packets", "errors", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        case OID_ethernetRXPacketsDropped:
            if(IfConfigRead(cIfName, "RX packets", "dropped", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        case OID_ethernetTXPacketsError:
            if(IfConfigRead(cIfName, "TX packets", "errors", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        case OID_ethernetTXPacketsDropped:
            if(IfConfigRead(cIfName, "TX packets", "dropped", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        case OID_ethernetUpkbps:
            {
            unsigned long ulTemp = 0;
            float fUpkbps = 0.00;
            if(IfConfigRead(cIfName, NULL, "TX bytes", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            ulTemp = safe_atoi(p_szWirelessValue);
            
            fUpkbps = ((ulTemp - g_ultxBytes[0]) * 8.00) / (1024*(times(NULL)-g_ultxBytes[1]));
            sprintf(p_szWirelessValue, "%fkbps", fUpkbps);

             /*for the next action*/
             g_ultxBytes[0] = ulTemp;
             g_ultxBytes[1] = times(NULL);
            }

            *var_len = strlen(p_szWirelessValue);
            return p_szWirelessValue; 
        case OID_ethernetDownkbps:
            {
            unsigned long ulTemp = 0;
            float fDownkbps = 0.00;
            if(IfConfigRead(cIfName, NULL, "RX bytes", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            ulTemp = safe_atoi(p_szWirelessValue);

            fDownkbps = ((ulTemp - g_ulrxBytes[0]) * 8.00) / (1024*(times(NULL)-g_ulrxBytes[1]));
            sprintf(p_szWirelessValue, "%fkbps", fDownkbps);

             /*for the next action*/
             g_ulrxBytes[0] = ulTemp;
             g_ulrxBytes[1] = times(NULL);
            }

            *var_len = strlen(p_szWirelessValue);
            return p_szWirelessValue; 
        case OID_ethernetLossTolerance:
            {
                unsigned long txInfo[2] = {0};
                float fLossTolerance = 0.00;
                if(IfConfigRead(cIfName, "TX packets", "errors", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
                {
                    return NULL;
                }

                txInfo[0] = safe_atoi(p_szWirelessValue); 
                if(IfConfigRead(cIfName, NULL, "TX packets", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
                {
                    return NULL;
                }
                txInfo[1] = safe_atoi(p_szWirelessValue);

                fLossTolerance = (txInfo[0]*100.00) / txInfo[1];
                sprintf(p_szWirelessValue, "%f", fLossTolerance);
                strcat(p_szWirelessValue, "%");
                *var_len = strlen(p_szWirelessValue);
                return p_szWirelessValue; 
            }
            break;
        case OID_ethernetTXRetryPackts:
            if(IfConfigRead(cIfName, "TX packets", "overruns", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        case OID_ethernetRXRetryPackts:
            if(IfConfigRead(cIfName, "RX packets", "overruns", ":", p_szWirelessValue, sizeof(p_szWirelessValue)))
            {
                return NULL;
            }
            break;
        default:
            break;
    }

    p_ulWirelessValue = safe_atoi(p_szWirelessValue);
    return (unsigned char*)&p_ulWirelessValue;
}

unsigned char *var_WirelessStatsEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};     // OID to be returned

    if(loadWireless_StatsTable() != SNMP_ERR_NOERROR) 
    {
        SNMPA_TRACE("Failed to exec loadWireless_StatsTable, it return error!\n");
        return NULL;
    }

    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWirelessStatsList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWirelessStatsTbl = (wirelessStatsTable_data*) hdlNode->data;
        rName[vp->namelen] = pstWirelessStatsTbl->wirelessStatsInstanceId; /*we are sure instance id is always with index <vp->namelen>*/

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
	     memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
	     *length = vp->namelen + 1;
	     *var_len = sizeof (long);
            *write_method = NULL;

            switch(vp->magic)
            {
                case OID_wirelessRXPacketsTotal:
                    return (unsigned char*)&pstWirelessStatsTbl->wirelessRXPacketsTotal;
                case OID_wirelessRXBytesTotal:
                    return (unsigned char*)&pstWirelessStatsTbl->wirelessRXBytesTotal;
                case OID_wirelessTXPacketsTotal:
                    return (unsigned char*)&pstWirelessStatsTbl->wirelessTXPacketsTotal;
                case OID_wirelessTXBytesTotal:
                    return (unsigned char*)&pstWirelessStatsTbl->wirelessTXBytesTotal;
                case OID_wirlessRXPacketsError:
                    return (unsigned char*)&pstWirelessStatsTbl->wirlessRXPacketsError;
                case OID_wirlessRXPacketsDropped:
                    return (unsigned char*)&pstWirelessStatsTbl->wirlessRXPacketsDropped;
                case OID_wirlessTXPacketsError:
                    return (unsigned char*)&pstWirelessStatsTbl->wirlessTXPacketsError;
                case OID_wirlessTXPacketsDropped:
                    return (unsigned char*)&pstWirelessStatsTbl->wirlessTXPacketsDropped;
                case OID_wirelessRXPacketsDroppedRate:
                    *var_len = safe_strlen(pstWirelessStatsTbl->wirelessRXPacketsDroppedRate);
                    return (unsigned char*)pstWirelessStatsTbl->wirelessRXPacketsDroppedRate;
                case OID_wirelessAssocUserSum:
                    return (unsigned char*)&pstWirelessStatsTbl->wirelessAssocUserSum;
                default:
                    break;
            }
        }
    }

    return NULL;
}

unsigned char *var_WlanClientEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};    // OID to be returned

    if(loadWlan_ClientNumberTable() != SNMP_ERR_NOERROR)
    {
        SNMPA_TRACE("Failed to exec loadWlan_ClientNumberTable, it return error!\n");
        return NULL;
    }

    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWlanClientNumberList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWlanClientNumberTbl = (wlanClientNumberTable_data*) hdlNode->data;
        rName[vp->namelen] = pstWlanClientNumberTbl->wlanClientInstanceId; /*we are sure instance id is always with index <vp->namelen>*/

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
	     memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
	     *length = vp->namelen + 1;
	     *var_len = sizeof (long);
            *write_method = NULL;

            switch(vp->magic)
            {
            case OID_wlanClientNumber:
                return (unsigned char*)&pstWlanClientNumberTbl->wlanClientNumber;
            case OID_wlanClientNumberPercent:
               *var_len = strlen(pstWlanClientNumberTbl->wlanClientNumberPercent);
               return (unsigned char*)pstWlanClientNumberTbl->wlanClientNumberPercent;
                break;
            default:
                break;
            }
         }
    }

    return NULL;
}

unsigned char *var_WlanClient2Entry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};    // OID to be returned

    #if 0
    if(LoadWlan_ClientTable() != SNMP_ERR_GENERR)
    {
        SNMPA_TRACE("Failed to read LoadWlan_ClientTable!\n");
        return NULL;
    }
    #else
    LoadWlan_ClientTable(); /*regardless of the instance existance of clients, 
                            we will return something as response to request from snmp manager*/
    #endif

    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWlanClientList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWlanClientTbl = (wlanClientTable_data*) hdlNode->data;
        rName[vp->namelen] = pstWlanClientTbl->clientIndex; /*we are sure instance id is always with index <vp->namelen>*/

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
	     memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
	     *length = vp->namelen + 1;
	     *var_len = sizeof (long);
            *write_method = NULL;

            switch(vp->magic)
            {
                case OID_clientIndex:
                    return (unsigned char*)&pstWlanClientTbl->clientIndex;
                case OID_clientMac:
                    *var_len = strlen(pstWlanClientTbl->clientMac);
                    return (unsigned char*)pstWlanClientTbl->clientMac;
                case OID_clientRssi:
                    return (unsigned char*)&pstWlanClientTbl->clientRssi;
                case OID_clientStatus:
                    *var_len = strlen(pstWlanClientTbl->clientStatus);
                    return (unsigned char*)pstWlanClientTbl->clientStatus;
                default:
                    break;
            }
        }
    }

    return NULL;
}

unsigned char *var_WlanWepEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};     // OID to be returned
    
    LoadWlan_WepTable();

    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWlanWepList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWlanWepTbl = (wlanWepTable_data*)hdlNode->data;
        rName[vp->namelen] = pstWlanWepTbl->configInstanceId; 

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
	     memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
	     *length = vp->namelen + 1;
	     *var_len = sizeof (long); 

#define WLANWEP_STR_VECTOR_WRITABLE_NODE(keyid) \
do{ \
    *write_method = WriteMethod_Comm; \
    snprintf(p_szWirelessPath, sizeof(p_szWirelessPath), TR069_ROOT_wlanConfiguration"%lu.%s.%lu."TR069_NODE_WepKey, \
		    pstWlanWepTbl->configInstanceId, TR069_ROOT_WepKey, keyid); \
    Active_WriteMethod(vp->type, vp->acl, 0, sizeof(pstWlanWepTbl->wepKey##keyid), 0, NULL, p_szWirelessPath, NULL, 0);\
 \
    *var_len = strlen(pstWlanWepTbl->wepKey##keyid); \
     return (unsigned char*)pstWlanWepTbl->wepKey##keyid; \
}while(0)

            switch(vp->magic)
            {
                case OID_wepKeyType:
                    snprintf(p_szWirelessPath, sizeof(p_szWirelessPath), 
                                  TR069_ROOT_wlanConfiguration"%lu."TR069_NODE_WepEncryptionLevel, pstWlanWepTbl->configInstanceId);
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 104, NULL, p_szWirelessPath, 
                                                      cstwepKeyTypeMatched, GET_ARRAY_BOUNADRY(cstwepKeyTypeMatched));
	             *write_method = WriteMethod_Comm;
	             return (unsigned char*)&pstWlanWepTbl->wepKeyType;
                case OID_wepKey1:
                    WLANWEP_STR_VECTOR_WRITABLE_NODE(1);
                case OID_wepKey2:
                    WLANWEP_STR_VECTOR_WRITABLE_NODE(2);
                case OID_wepKey3:
                    WLANWEP_STR_VECTOR_WRITABLE_NODE(3);
                case OID_wepKey4:
                    WLANWEP_STR_VECTOR_WRITABLE_NODE(4);
                default:
                    return NULL;
            }
        }
    }
        
    return NULL;
}

/*public variable for three nodes service:presharekey, authenticationType, encryption*/
static char somenodes_write_method_prefix[192];
static int WriteMethod_presharekey(int action,
                                                                unsigned char *var_val,
                                                                unsigned char var_val_type,
                                                                size_t var_val_len, 
                                                                unsigned char *statP, 
                                                                oid *name, size_t length)
{
     switch(action)
     {
         case RESERVE1:
            if(var_val_len == 64) /*all bytes are hex*/
            {
                int i = 0;
                char *p = var_val;
                for(; i<64; i++)
                {
                    if((*p>='a' && *p<='f') ||(*p>='A' && *p<='F') ||(*p>='0' && *p<='9'))
                    {
                    }
                    else return SNMP_ERR_WRONGVALUE;

                    p++;
                }
            }
            else if(var_val_len == 63)
            {
            }
            else return SNMP_ERR_WRONGLENGTH;
            break;
         case RESERVE2:
            break;
         case ACTION:
            {
            unsigned ulPathLen = 0;
            char szMultiPath[512] = {0};
            char my_var_val[72] = {0};
            if(var_val_len == 64)
            {
                strncpy(my_var_val, var_val, 64);
                memset(szMultiPath, 0, sizeof(szMultiPath));
                sprintf(szMultiPath, "%s%s=%s", somenodes_write_method_prefix, TR069_NODE_presharekey64Hex, my_var_val);
                ulPathLen = strlen(szMultiPath) + 1;
                sprintf(szMultiPath+ulPathLen, "%s%s=", somenodes_write_method_prefix, TR069_NODE_presharekey63Asc);
                ulPathLen += strlen(somenodes_write_method_prefix) + strlen(TR069_NODE_presharekey63Asc) + 2;

                if(CMM_SetStr_ForMultiLeaves(szMultiPath, ulPathLen, 2) != CMM_SUCCESS)
                {
                    SNMPA_TRACE("Error returned after calling CMM_SetStr_ForMultiLeaves.\n");
                    return SNMP_ERR_GENERR;
                }
            }
            else /*63*/
            {
                strncpy(my_var_val, var_val, 63);
                memset(szMultiPath, 0, sizeof(szMultiPath));
                sprintf(szMultiPath, "%s%s=%s", somenodes_write_method_prefix, TR069_NODE_presharekey63Asc, my_var_val);
                ulPathLen = strlen(szMultiPath) + 1;
                sprintf(szMultiPath+ulPathLen, "%s%s=", somenodes_write_method_prefix, TR069_NODE_presharekey64Hex);
                ulPathLen += strlen(somenodes_write_method_prefix) + strlen(TR069_NODE_presharekey64Hex) + 2;

                if(CMM_SetStr_ForMultiLeaves(szMultiPath, ulPathLen, 2) != CMM_SUCCESS)
                {
                    SNMPA_TRACE("Error returned after calling CMM_SetStr_ForMultiLeaves.\n");
                    return SNMP_ERR_GENERR;
                }
            }
            }
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

int RN_GetWirelessAuthenticationType(char *pszConfigPathPrefix, char *pszOutPutAuthenticationType)
{
    char szBeaconType[64] = {0};
    char szAuthType[64] = {0};
    if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, 
                                        szBeaconType, sizeof(szBeaconType), NULL, NULL, 0) != CMM_SUCCESS)
    {
        SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s, value:\n", p_szWirelessPath);
        return SNMP_ERR_GENERR;
    }

    if(!safe_strcmp(szBeaconType, TR069_VALUE_BeaconType))
    {
        if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_BasicAuthenticationMode, 
                                            szAuthType, sizeof(szAuthType), NULL, NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s.\n", pszConfigPathPrefix, TR069_NODE_BasicAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(!safe_strcmp(szAuthType, TR069_VALUE_BeaconType))
        {
            strcpy(pszOutPutAuthenticationType, "0"/*"open"*/);
        }
        else
        {
            SNMPA_TRACE("Illegal value from %s.\n", TR069_NODE_BasicAuthenticationMode);
            return SNMP_ERR_WRONGVALUE;
        }
    }
    else if(!safe_strcmp(szBeaconType, TR069_VALUE_BasicAuthenticationMode))
    {
        if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_BasicAuthenticationMode, 
                                            szAuthType, sizeof(szAuthType), NULL, NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s.\n", pszConfigPathPrefix, TR069_NODE_BasicAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(!safe_strcmp(szAuthType, TR069_VALUE_BeaconType))
        {
            strcpy(pszOutPutAuthenticationType, "0"/*"open"*/);
        }
        else if(!safe_strcmp(szAuthType, TR069_VALUE_SharedAuthentication))
        {
            strcpy(pszOutPutAuthenticationType, "1"/*"shared"*/);
        }
        else
        {
            SNMPA_TRACE("Illegal value from %s.\n", TR069_NODE_BasicAuthenticationMode);
            return SNMP_ERR_WRONGVALUE;
        }
    }
    else if(!safe_strcmp(szBeaconType, TR069_VALUE_WPAAuthenticationMode))
    {
        if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_WPAAuthenticationMode, 
                                            szAuthType, sizeof(szAuthType), NULL, NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s.\n", pszConfigPathPrefix, TR069_NODE_WPAAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(!safe_strcmp(szAuthType, TR069_VALUE_PSKAuthentication))
        {
            strcpy(pszOutPutAuthenticationType, "16"/*"wpapsk"*/);
        }
        else if(!safe_strcmp(szAuthType, TR069_VALUE_EAPAuthentication))
        {
            strcpy(pszOutPutAuthenticationType, "2"/*"legacy802dot1x"*/);
        }
        else
        {
            SNMPA_TRACE("Illegal value from %s.\n", TR069_NODE_WPAAuthenticationMode);
            return SNMP_ERR_WRONGVALUE;
        }
    }
    else if(!safe_strcmp(szBeaconType, TR069_VALUE_IEEE11iAuthenticationMode))
    {
        if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_IEEE11iAuthenticationMode, 
                                            szAuthType, sizeof(szAuthType), NULL, NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s.\n", pszConfigPathPrefix, TR069_VALUE_IEEE11iAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(!safe_strcmp(szAuthType, TR069_VALUE_PSKAuthentication))
        {
            strcpy(pszOutPutAuthenticationType, "8"/*"wpa2psk"*/);
        }
        else if(!safe_strcmp(szAuthType, TR069_VALUE_EAPAuthentication))
        {
            strcpy(pszOutPutAuthenticationType, "5"/*"wpa2"*/);
        }
        else 
        {
            SNMPA_TRACE("Illegal value from %s.\n", TR069_VALUE_IEEE11iAuthenticationMode);
            return SNMP_ERR_WRONGVALUE;
        }
    }
    else
    {
        SNMPA_TRACE("Unknow BeaconType:%s", szBeaconType);
        return SNMP_ERR_WRONGVALUE;
    }

    return SNMP_ERR_NOERROR;
}

static int RN_SetWirelessAuthenticationType(char *pszConfigPathPrefix, char *pszInPutAuthenticationType)
{
    if(!safe_strcmp(pszInPutAuthenticationType, "0")) /*open*/
    {
        char *tmpVale= TR069_VALUE_BeaconType;
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, tmpVale, strlen(tmpVale), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, tmpVale);
            return SNMP_ERR_GENERR;
        }

        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BasicAuthenticationMode, "None", strlen("None"), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after setting %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_BasicAuthenticationMode, "None");
            return SNMP_ERR_GENERR;
        }
    }
    else if(!safe_strcmp(pszInPutAuthenticationType, "1")) /*Shared*/
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_BasicAuthenticationMode,
                                            strlen(TR069_VALUE_BasicAuthenticationMode), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_BasicAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BasicAuthenticationMode, TR069_VALUE_SharedAuthentication, 
                                            sizeof(TR069_VALUE_SharedAuthentication), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_BasicAuthenticationMode, TR069_VALUE_SharedAuthentication);
            return SNMP_ERR_GENERR;
        }
    }
    else if(!safe_strcmp(pszInPutAuthenticationType, "2")) /*legacy802dot1x*/
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_WPAAuthenticationMode, 
                                            strlen(TR069_VALUE_WPAAuthenticationMode), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_WPAAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_WPAAuthenticationMode, TR069_VALUE_EAPAuthentication, 
                                            sizeof(TR069_VALUE_EAPAuthentication), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_WPAAuthenticationMode, TR069_VALUE_EAPAuthentication);
            return SNMP_ERR_GENERR;
        }
    }
    else if(!safe_strcmp(pszInPutAuthenticationType, "5"))
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_IEEE11iAuthenticationMode, 
                                            strlen(TR069_VALUE_IEEE11iAuthenticationMode), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_IEEE11iAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_IEEE11iAuthenticationMode, TR069_VALUE_EAPAuthentication, 
                                            sizeof(TR069_VALUE_EAPAuthentication), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_IEEE11iAuthenticationMode, TR069_VALUE_EAPAuthentication);
            return SNMP_ERR_GENERR;
        }
    }
    else if(!safe_strcmp(pszInPutAuthenticationType, "8"))  /*wpa2psk*/
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_IEEE11iAuthenticationMode, 
                                            strlen(TR069_VALUE_IEEE11iAuthenticationMode), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_IEEE11iAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_IEEE11iAuthenticationMode, TR069_VALUE_PSKAuthentication, 
                                            sizeof(TR069_VALUE_PSKAuthentication), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_IEEE11iAuthenticationMode, TR069_VALUE_PSKAuthentication);
            return SNMP_ERR_GENERR;
        }
    }
    else if(!safe_strcmp(pszInPutAuthenticationType, "16")) /*wpapsk*/
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_WPAAuthenticationMode, 
                                            strlen(TR069_VALUE_WPAAuthenticationMode), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_WPAAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_WPAAuthenticationMode, TR069_VALUE_PSKAuthentication, 
                                            sizeof(TR069_VALUE_PSKAuthentication), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_WPAAuthenticationMode, TR069_VALUE_PSKAuthentication);
            return SNMP_ERR_GENERR;
        }
    }
    else return SNMP_ERR_WRONGVALUE;

    return SNMP_ERR_NOERROR;
}


int RN_GetWirelessEncryptionType(char *pszConfigPathPrefix, char *pszOutPutEncryptionType)
{
    char szBeaconType[64] = {0};
    char szAuthType[64] = {0};
    if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, 
                                        szBeaconType, sizeof(szBeaconType), NULL, NULL, 0) != CMM_SUCCESS)
    {
        SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s, value:\n", p_szWirelessPath);
        return SNMP_ERR_GENERR;
    }

    if(!safe_strcmp(szBeaconType, TR069_VALUE_BeaconType))
    {
        if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_BasicEncryptionMode, 
                                            szAuthType, sizeof(szAuthType), NULL, NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s .\n", pszConfigPathPrefix, TR069_NODE_BasicEncryptionMode);
            return SNMP_ERR_GENERR;
        }
        
        if(!safe_strcmp(szAuthType, TR069_VALUE_BeaconType))
        {
            strcpy(pszOutPutEncryptionType, "0"/*"none"*/);
        }
        else
        {
            SNMPA_TRACE("Illegal value from %s.:%s\n", TR069_NODE_BasicEncryptionMode, szAuthType);
            return SNMP_ERR_WRONGVALUE;
        }
    }
    else if(!safe_strcmp(szBeaconType, TR069_VALUE_BasicAuthenticationMode))
    {
        if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_BasicEncryptionMode, 
                                            szAuthType, sizeof(szAuthType), NULL, NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s nod value.\n", TR069_NODE_BasicEncryptionMode);
            return SNMP_ERR_GENERR;
        }
        
        if(!safe_strcmp(szAuthType, TR069_VALUE_BeaconType))
        {
            strcpy(pszOutPutEncryptionType, "0"/*"none"*/);
        }
        else if(!safe_strcmp(szAuthType, TR069_ENC_BASIC_WEP))
        {
            strcpy(pszOutPutEncryptionType, "1"/*"wep"*/);
        }
        else
        {
            SNMPA_TRACE("Illegal value from %s.\n", TR069_NODE_BasicEncryptionMode);
            return SNMP_ERR_WRONGVALUE;
        }
    }
    else if(!safe_strcmp(szBeaconType, TR069_VALUE_WPAAuthenticationMode))
    {
        if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_WPAEncryptionMode, 
                                            szAuthType, sizeof(szAuthType), NULL, NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s nod value.\n", TR069_NODE_WPAEncryptionMode);
            return SNMP_ERR_GENERR;
        }
        
        if(!safe_strcmp(szAuthType, TR069_ENC_WPA_TKIP))
        {
            strcpy(pszOutPutEncryptionType, "2"/*"tkip"*/);
        }
        else
        {
            SNMPA_TRACE("Illegal value from %s.\n", TR069_NODE_WPAEncryptionMode);
            return SNMP_ERR_WRONGVALUE;
        }
    }
    else if(!safe_strcmp(szBeaconType, TR069_VALUE_IEEE11iAuthenticationMode))
    {
        if(CMM_GetStrColumn(pszConfigPathPrefix, TR069_NODE_IEEE11iEncryptionMode, 
                                            szAuthType, sizeof(szAuthType), NULL, NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s nod value:\n", TR069_NODE_IEEE11iEncryptionMode);
            return SNMP_ERR_GENERR;
        }
        
        if(!safe_strcmp(szAuthType, TR069_ENC_WPA2_TKIP))
        {
            strcpy(pszOutPutEncryptionType, "6"/*"tkip"*/);
        }
        else if(!safe_strcmp(szAuthType, TR069_VALUE_EAPAuthentication))
        {
            strcpy(pszOutPutEncryptionType, "4"/*"aes"*/);
        }
        else 
        {
            SNMPA_TRACE("Illegal value from %s.\n", TR069_NODE_IEEE11iEncryptionMode);
            return SNMP_ERR_WRONGVALUE;
        }
    }
    else
    {
        SNMPA_TRACE("Unknow BeaconType:%s", szBeaconType);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

static int RN_SetWirelessEncryptionType(char *pszConfigPathPrefix, char *pszInPutEncryptionType)
{
    if(!safe_strcmp(pszInPutEncryptionType, "0")) /*none*/
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_BeaconType, strlen(TR069_VALUE_BeaconType), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_BeaconType);
            return SNMP_ERR_GENERR;
        }

        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BasicEncryptionMode, TR069_ENC_BASIC_NONE, 
                                           strlen(TR069_ENC_BASIC_NONE), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after setting %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_BasicEncryptionMode, TR069_ENC_BASIC_NONE);
            return SNMP_ERR_GENERR;
        }
    }
    else if(!safe_strcmp(pszInPutEncryptionType, "1")) /*wep*/
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_BasicAuthenticationMode,
                                            strlen(TR069_VALUE_BasicAuthenticationMode), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_BasicAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BasicEncryptionMode, TR069_ENC_BASIC_WEP, 
                                            sizeof(TR069_ENC_BASIC_WEP), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_BasicEncryptionMode, TR069_ENC_BASIC_WEP);
            return SNMP_ERR_GENERR;
        }
    }
    else if(!safe_strcmp(pszInPutEncryptionType, "2")) /*tkip*/
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_WPAAuthenticationMode, 
                                            strlen(TR069_VALUE_WPAAuthenticationMode), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_WPAAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_WPAEncryptionMode, TR069_ENC_WPA_TKIP, 
                                            sizeof(TR069_ENC_WPA_TKIP), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_WPAEncryptionMode, TR069_ENC_WPA_TKIP);
            return SNMP_ERR_GENERR;
        }
    }
    else if(!safe_strcmp(pszInPutEncryptionType, "4")) /*aes*/
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_IEEE11iAuthenticationMode, 
                                            strlen(TR069_VALUE_IEEE11iAuthenticationMode), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_IEEE11iAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_IEEE11iEncryptionMode, TR069_ENC_WPA2_AES, 
                                            sizeof(TR069_ENC_WPA2_AES), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_IEEE11iEncryptionMode, TR069_ENC_WPA2_AES);
            return SNMP_ERR_GENERR;
        }
    }
    else if(!safe_strcmp(pszInPutEncryptionType, "6")) /*tkipORaes*/
    {
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_IEEE11iAuthenticationMode, 
                                            strlen(TR069_VALUE_IEEE11iAuthenticationMode), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s%s, value:%s\n", pszConfigPathPrefix, TR069_NODE_BeaconType, TR069_VALUE_IEEE11iAuthenticationMode);
            return SNMP_ERR_GENERR;
        }
        
        if(CMM_SetStrColumn(pszConfigPathPrefix, TR069_NODE_IEEE11iEncryptionMode, TR069_ENC_WPA2_TKIP, 
                                            sizeof(TR069_ENC_WPA2_TKIP), NULL, 0) != CMM_SUCCESS)
        {
            SNMPA_TRACE("Error returned after find %s%s value:%s\n", pszConfigPathPrefix, TR069_NODE_IEEE11iEncryptionMode, TR069_ENC_WPA2_TKIP);
            return SNMP_ERR_GENERR;
        }
    }
    else return SNMP_ERR_WRONGVALUE;

    return SNMP_ERR_NOERROR;
}

static int WriteMethod_authenticationType(int action,
                                                                            unsigned char *var_val,
                                                                            unsigned char var_val_type,
                                                                            size_t var_val_len, 
                                                                            unsigned char *statP, 
                                                                            oid *name, size_t length)
{
    switch(action)
    {
        case RESERVE1:
            break;
        case RESERVE2:
            break;
         case ACTION:
            {
            char szInPutAuthenticationType[128] = {0};
            strncpy(szInPutAuthenticationType, var_val, var_val_len);
            RN_SetWirelessAuthenticationType(somenodes_write_method_prefix, szInPutAuthenticationType);
            }
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

static int WriteMethod_encryption(int action,
                                                            unsigned char *var_val,
                                                            unsigned char var_val_type,
                                                            size_t var_val_len, 
                                                            unsigned char *statP, 
                                                            oid *name, size_t length)
{
    switch(action)
    {
        case RESERVE1:
            break;
        case RESERVE2:
            break;
         case ACTION:
            {
            char szInPutEncryptionType[128] = {0};
            strncpy(szInPutEncryptionType, var_val, var_val_len);
            RN_SetWirelessEncryptionType(somenodes_write_method_prefix, szInPutEncryptionType);
            }
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


unsigned char *var_WlanVapEncrytEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};     // OID to be returned
    
    LoadWlan_VapTable();

    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWlanVapEncytList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWlanVapEncytTbl = (wlanVapTable_data*)hdlNode->data;
        rName[vp->namelen] = pstWlanVapEncytTbl->vapInstanceId; 

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
	     memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
	     *length = vp->namelen + 1;
	     *var_len = sizeof (long); 

            switch(vp->magic)
            {
                case OID_vapIndex:
                    sprintf(p_szWirelessPath, TR069_ROOT_wlanConfiguration"%lu."TR069_NODE_vapIndex, 
                              pstWlanVapEncytTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 32000, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&pstWlanVapEncytTbl->vapIndex;
                case OID_vapEnable:
                    sprintf(p_szWirelessPath, TR069_ROOT_wlanConfiguration"%lu."TR069_NODE_vapEnable, 
                              pstWlanVapEncytTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&pstWlanVapEncytTbl->vapEnable;
                case OID_vapssid:
                    sprintf(p_szWirelessPath, TR069_ROOT_wlanConfiguration"%lu."TR069_NODE_vapssid, pstWlanVapEncytTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 0, sizeof(pstWlanVapEncytTbl->vapssid), 0, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)pstWlanVapEncytTbl->vapssid;
                case OID_hidenetworkname:
                    sprintf(p_szWirelessPath, TR069_ROOT_wlanConfiguration"%lu."TR069_NODE_hidenetworkname, 
                              pstWlanVapEncytTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&pstWlanVapEncytTbl->hidenetworkname;
                case OID_presharekey:
                    sprintf(somenodes_write_method_prefix, TR069_ROOT_wlanConfiguration"%lu.", pstWlanVapEncytTbl->vapInstanceId);
                    *write_method = WriteMethod_presharekey;
                    return (unsigned char*)pstWlanVapEncytTbl->presharekey;
                case OID_authenticationtype:
                    sprintf(somenodes_write_method_prefix, TR069_ROOT_wlanConfiguration"%lu.", pstWlanVapEncytTbl->vapInstanceId);
                    *write_method = WriteMethod_authenticationType;
                    return (unsigned char*)&pstWlanVapEncytTbl->authenticationtype;
                case OID_encryption:
                    sprintf(somenodes_write_method_prefix, TR069_ROOT_wlanConfiguration"%lu.", pstWlanVapEncytTbl->vapInstanceId);
                    *write_method = WriteMethod_encryption;
                    return (unsigned char*)&pstWlanVapEncytTbl->encryption;
                case OID_wanseparator:
                    sprintf(p_szWirelessPath, TR069_ROOT_wlanConfiguration"%lu."TR069_NODE_wlanseparator, 
                              pstWlanVapEncytTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&pstWlanVapEncytTbl->wlanseparator;
                case OID_vapMacAddr:
                    sprintf(p_szWirelessPath, TR069_ROOT_wlanConfiguration"%lu."TR069_NODE_vapMacAddr, 
                              pstWlanVapEncytTbl->vapInstanceId);
                    *write_method = WriteMethod_Comm;
                    Active_WriteMethod(vp->type, vp->acl, 1, sizeof(pstWlanVapEncytTbl->vapMacAddr), 0, NULL, p_szWirelessPath, NULL, 0);
                    return (unsigned char*)&pstWlanVapEncytTbl->vapMacAddr;
                default:
                    break;
            }
        }
    }

    return NULL;
}

unsigned char *var_WlanVapDataEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};     // OID to be returned
    LoadWlan_VapDataTable();

    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWlanVapDataList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWlanVapDataTbl = (wlanVapTable_data*)hdlNode->data;
        rName[vp->namelen] = pstWlanVapDataTbl->vapInstanceId; 

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
	      memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
	      *length = vp->namelen + 1;
	      *var_len = sizeof (long); 
          *write_method = NULL;

          switch(vp->magic)
          {
             case OID_vapMtu:
                return (unsigned char*)&pstWlanVapDataTbl->vapMtu;
             case OID_vapWirelessRXBytesTotal:
                return (unsigned char*)&pstWlanVapDataTbl->vapWirelessRXBytesTotal;
             case OID_vapWirelessTXBytesTotal:
                return (unsigned char*)&pstWlanVapDataTbl->vapWirelessTXBytesTotal;
             case OID_vapWirelessRXPacketsTotal:
                return (unsigned char*)&pstWlanVapDataTbl->vapWirelessRXPacketsTotal;
             case OID_vapWirelessTXPacketsTotal:
                return (unsigned char*)&pstWlanVapDataTbl->vapWirelessTXPacketsTotal;
             case OID_vapClientCnt:
                return (unsigned char*)&pstWlanVapDataTbl->vapClientCnt;
             default:
                break;
          }
        }
    }
    return NULL;
}

#endif

unsigned char *var_WlanInfoTblDataEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};     // OID to be returned
    if(loadTbs_WlanInfoTable() != SNMP_ERR_NOERROR)
    {
        SNMPA_TRACE("Failed to exec LoadTbs_WlanTable, it return error!\n");
        return NULL;
    }
	
    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWlanInfoDataList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWlanInfoDataTbl = (wlanInfoTable_data*)hdlNode->data;
        rName[vp->namelen] = pstWlanInfoDataTbl->wlanInstanceId; 

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
		    memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
		    *length = vp->namelen + 1;
		    *var_len = sizeof (long); 
	        switch(vp->magic)
	        {
	            case OID_wlanSSID:
	                snprintf(p_szWirelessPath, sizeof(p_szWirelessPath), 
	                              TR069_ROOT_wlanConfiguration"%lu."TR069_NODE_wLanSSID, pstWlanInfoDataTbl->wlanInstanceId);
					Active_WriteMethod(vp->type, vp->acl, 0, sizeof(pstWlanInfoDataTbl->wlanSSID), 0, NULL, p_szWirelessPath, NULL, 0);
					*write_method = WriteMethod_Comm;
					*var_len = strlen(pstWlanInfoDataTbl->wlanSSID);
	                return (unsigned char*)pstWlanInfoDataTbl->wlanSSID;
	            case OID_wlanSSIDBroadcast:
	                return (unsigned char*)&(pstWlanInfoDataTbl->wlanSSIDBroadcast);
	            case OID_wlanSSIDStatus:  
	                snprintf(p_szWirelessPath, sizeof(p_szWirelessPath), 
	                              TR069_ROOT_wlanConfiguration"%lu."TR069_NODE_wlanSSIDStatus, pstWlanInfoDataTbl->wlanInstanceId);
					Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
					*write_method = WriteMethod_Comm;
	                return (unsigned char*)&(pstWlanInfoDataTbl->wlanSSIDStatus);
	            /*case OID_wlanStatus:
	                snprintf(p_szWirelessPath, sizeof(p_szWirelessPath), 
	                              TR069_ROOT_wlanEnable"WirelessEnable");
					Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
					*write_method = WriteMethod_Comm;
	                return (unsigned char*)&(pstWlanInfoDataTbl->wlanStatus);*/
	            case OID_wlanNetMode:
	                return (unsigned char*)&(pstWlanInfoDataTbl->wlanNetMode);
	            default:
	                return NULL;
	        }
        }
    }  
    return NULL;
}
/********************add* lbw****************/
unsigned char *var_WlanInfoEnableEntry(struct variable *vp,
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
	           
	      case OID_wlanStatus:
	           snprintf(p_szWirelessPath, sizeof(p_szWirelessPath), 
	                              TR069_ROOT_wlanInfoTbl"1.Enable");
					   Active_WriteMethod(vp->type, vp->acl, 1, 0, 1, NULL, p_szWirelessPath, NULL, 0);
					   *write_method = WriteMethod_Comm;
	           if(CMM_GetUlong(TR069_ROOT_wlanInfoTbl"1.Enable", &p_ulAdslValue, NULL, 0) == CMM_SUCCESS)
		         {
		            *var_len = sizeof(p_ulAdslValue);
	                return (unsigned char*)&p_ulAdslValue;
		         }
		         else return NULL;

	      default:
	           return NULL;
	   
    }  
    return NULL;
}
/********************end****************/

unsigned char *var_WanInfoTblDataEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};     // OID to be returned
    if(loadTbs_WanInfoTable() != SNMP_ERR_NOERROR)
    {
        SNMPA_TRACE("Failed to exec loadTbs_WanInfoTable, it return error!\n");
        return NULL;
    }
	
    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stWanInfoDataList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstWanInfoDataTbl = (wanInfoTable_data*) hdlNode->data;
        rName[vp->namelen] = pstWanInfoDataTbl->wanInstanceId; /*we are sure instance id is always with index <vp->namelen>*/

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
		    memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
		    *length = vp->namelen + 1;
			*var_len = sizeof(long);
	        *write_method = NULL;

#define WAN_IPAddr_VECTOR_WRITABLE_NODE(keyid,member,ipSize) \
do{ \
     *write_method = WriteMethod_Comm; \
     snprintf(p_szWirelessPath, sizeof(p_szWirelessPath), TR069_ROOT_wanInfoTbl"%lu."keyid, \
		      pstWanInfoDataTbl->wanInstanceId); \
     Active_WriteMethod(vp->type, vp->acl, 0, ipSize, 0, NULL, p_szWirelessPath, NULL, 0);\
     *var_len = sizeof(unsigned long); \
     return CHARS_TO_intIPADDR(pstWanInfoDataTbl->member); \
}while(0)
          
		    switch(vp->magic)
		    {
		        case OID_wanInfoIndex:
		             return (unsigned char*)&pstWanInfoDataTbl->wanInfoIndex;
				case OID_wanConType:
					 *var_len = strlen(pstWanInfoDataTbl->wanConType);
					 return (unsigned char*)&pstWanInfoDataTbl->wanConType;
				case OID_wanIpAddr:
					 WAN_IPAddr_VECTOR_WRITABLE_NODE("ExternalIPAddress", wanIpAddr,32);
                case OID_wanNetMask:
		             WAN_IPAddr_VECTOR_WRITABLE_NODE("SubnetMask", wanNetMask,32);
				case OID_wanDefGateWay:
		             WAN_IPAddr_VECTOR_WRITABLE_NODE("RemoteIPAddress", wanDefGateWay,32);
				case OID_wanPrimaryDNS:
		             WAN_IPAddr_VECTOR_WRITABLE_NODE("DNSServers", wanPrimaryDNS,32);
				case OID_wanSecondaryDNS:
					 WAN_IPAddr_VECTOR_WRITABLE_NODE("X_TWSZ-COM_UsrDNSServers", wanSecondaryDNS,32);
		        case OID_wanMtuValue:
		            return (unsigned char*)&pstWanInfoDataTbl->wanMtuValue;
		        default:
		            break;
		     }
        }
    }

    return NULL;
}

unsigned char *var_VlanInfoTblDataEntry(struct variable *vp,
        					                  oid     *name,
        					                  size_t  *length,
        					                  int     exact,
        					                  size_t  *var_len,
        					                  WriteMethod **write_method)
{
    oid rName [ MAX_OID_LEN ] = {0};     // OID to be returned

    if(loadTbs_VlanInfoTable() != SNMP_ERR_NOERROR)
    {
        SNMPA_TRACE("Failed to exec loadTbs_VlanInfoTable, it return error!\n");
        return NULL;
    }
	
    memcpy((char *)rName, (char *)vp->name, (int)vp->namelen * sizeof (oid));
    for(hdlNode=LIST_FIRST(&stVlanInfoDataList); hdlNode!=NULL; hdlNode=LIST_NEXT(hdlNode, nodes)) 
    {
        pstVlanInfoDataTbl = (vlanInfoTable_data*) hdlNode->data;
        rName[vp->namelen] = pstVlanInfoDataTbl->vlanInstanceId; /*we are sure instance id is always with index <vp->namelen>*/

        if((exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) == 0)) || //request
            (!exact && (snmp_oid_compare(rName, vp->namelen+1, name, *length) >  0)))  //exact == 0 means request next
        {
			 memcpy((char *)name, (char *)rName, (vp->namelen + 1) * sizeof(oid));
		     *length = vp->namelen + 1;
			 *var_len = sizeof(long);
	         *write_method = NULL;
		     switch(vp->magic)
		     {
		        case OID_vlanId:
					return (unsigned char*)&pstVlanInfoDataTbl->vlanId;
				case OID_portList:
					*var_len = strlen(pstVlanInfoDataTbl->portList);
		            return (unsigned char*)pstVlanInfoDataTbl->portList;
		        default:
		            break;
		     }
        }
    }
    return NULL;
}
