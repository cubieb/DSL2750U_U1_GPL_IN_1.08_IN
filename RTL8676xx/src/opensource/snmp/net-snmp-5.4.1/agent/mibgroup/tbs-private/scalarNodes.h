#ifndef _TBSPRIVATE_SCALARNODES_H_
#define _TBSPRIVATE_SCALARNODES_H_

config_require(tbs-private/common/cmmif);
config_require(tbs-private/common/commhandle);
config_require(tbs-private/common/writeMethod);
 #ifdef ZTE_HOT_AP_SNMP_IF
config_require(tbs-private/common/snmpProcess);
 #endif
config_require(util_funcs);

#include "common/commhandle.h"

/*--------------------------------------
 * System setting
 *--------------------------------------*/
#define OID_sysVersion 1
#define OID_sysMacAddr 2
#define OID_sysAPName 3
#define OID_sysSpanningTreeEnable 4
#define OID_sysCountryRegion 5
#define OID_wams 6
#define OID_watchdog 7
#define OID_vlanEnable 8
#define OID_managementVlanID 9
#define OID_ethernetDataRate 10
#define OID_sysRedBootVersion 11
#define OID_ntpdEnable 12
#define OID_softwareVendor 13
#define OID_softwareName 14
#define OID_apBssidNum 15
#define OID_essidBssidNum 16
#define OID_apSysNEId 17
#define OID_syslogEnabled 18
#define OID_syslogServerIP 19
#define OID_syslogServerPort 20
#define OID_userName 21
#define OID_mibVersion 22
#define OID_cpuUsage 23
#define OID_memoryUsage 24
#define OID_memoryCapaciry 25
#define OID_cpuClockSpeed 26
#define OID_apManufacturer 27
#define OID_cpuTemperature 28
#define OID_flashUsableCapacity 29
#define OID_apSerial 30
#define OID_nodeCreateTime 31
#define OID_nodeTimestamp 32
#define OID_password 33
#define OID_flashCapacity 34
#define OID_hardwareversion 35

/*------------------------------------------
 * IP setting
 *-----------------------------------------*/
#define OID_ethernetIPType 1
#define OID_ethernetIPAddress 2
#define OID_ethernetNetmask 3
#define OID_ethernetGateway 4
#define OID_ethernetDNSPrimary 5
#define OID_ethernetDNSSecondary 6
#define OID_ethernetDhcpsIPStart 7
#define OID_ethernetDhcpsIPEnd 8
#define OID_ethernetDhcpsNetmask 9
#define OID_pppoeConnStatus 10
#define OID_pppoeLocalIP 11
#define OID_pppoeUserName 12
#define OID_pppoePassword 13
#define OID_pppoeTimeOut 14
#define OID_pppoeRetryTimes 15
#define OID_pppoeAuthenticationType 	16
#define OID_pppoeMTU 17 

#ifdef ZTE_HOT_AP_SNMP_IF
/*------------------------------------------
 * SysInfo
 *-----------------------------------------*/
#define OID_w640aNodeID 1
#define OID_w640aHostName 2
#define OID_w640aVersion 3
#define OID_w640aCreateDate 4
#define OID_sysfileLoadFlag 5
#define OID_sysfileType 6
#define OID_sysfileSize 7
#define OID_sysfileURL 8
#define OID_cmdKey 9
#endif

#define TR069_ROOT_Ntp	"InternetGatewayDevice.Time."
#define TR069_ROOT_DeviceInfo "InternetGatewayDevice.DeviceInfo."
#define TR069_ROOT_Logger "InternetGatewayDevice.X_TWSZ-COM_Logger."
#define TR069_ROOT_Auth "InternetGatewayDevice.X_TWSZ-COM_Authentication.1."
#ifdef ZTE_HOT_AP_SNMP_IF
#define TR069_ROOT_FILETRANS "InternetGatewayDevice.X_TWSZ-COM_SNMPAgent.X_TWSZ-COM_FileTransfer."
#endif

#endif         /*_TBSPRIVATE_SCALARNODES_H_*/
