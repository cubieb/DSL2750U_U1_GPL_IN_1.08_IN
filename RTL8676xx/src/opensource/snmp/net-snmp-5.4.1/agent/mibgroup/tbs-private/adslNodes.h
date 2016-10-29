#ifndef _TBSPRIVATE_ADSLNODES_H_
#define _TBSPRIVATE_ADSLNODES_H_

config_require(tbs-private/common/cmmif);
config_require(tbs-private/common/commhandle);
config_require(tbs-private/common/writeMethod);
config_require(tbs-private/common/snmpProcess);
config_require(util_funcs);

#include "common/commhandle.h"

/*SNMP*/
#define OID_snmpEnabled 1
#define OID_snmpReadCommunity 2
#define OID_snmpWriteCommunity 3
#define OID_snmpDescription 4
#define OID_snmpTrapServer 5
#define OID_resetAP 6
#define OID_restoreFactory 7

/*SNTP*/
#define OID_timeserver 1    
#if 0
#define OID_timeserverport 2
#endif
#define OID_currenttime 3
#define OID_timezone 4
#define OID_daylightsaving 5

#define TR069_ROOT_SNMP "InternetGatewayDevice.X_TWSZ-COM_SNMPAgent."
#define TR069_ROOT_SNTP "InternetGatewayDevice.Time."

#define TR069_DeviceInfo "InternetGatewayDevice.DeviceInfo."
#define TR069_IfNumber "InternetGatewayDevice.Layer2Bridging."
#define TR069_EthDHCP "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement."
#define TR069_IfNumber "InternetGatewayDevice.Layer2Bridging."
#define TR069_NTPServer "InternetGatewayDevice.Time."

#define TR069_SNMPAgent "InternetGatewayDevice.X_TWSZ-COM_SNMPAgent."

#ifdef CONFIG_OPENSOURCE_STREAMRATE  
#define TR069_WANDSLInterfaceConfig "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig."
#endif

#define Basic_INFO_MAGIC  1

/*1.3.6.1.2.1.47.1.1.1.1.13.1*/
#define OID_physicalModelName ((Basic_INFO_MAGIC)+1)
/*1.3.6.1.2.1.47.1.1.1.1.12.1*/
#define OID_physicalMfgName ((Basic_INFO_MAGIC)+2)
/*1.3.6.1.2.1.47.1.1.1.1.11.1*/
#define OID_SerialNum ((Basic_INFO_MAGIC)+3)
/*1.3.6.1.2.1.47.1.1.1.1.10.1*/
#define OID_SoftwareRev ((Basic_INFO_MAGIC)+4)
/*1.3.6.1.2.1.47.1.1.1.1.9.1*/
#define OID_FirmwareRev ((Basic_INFO_MAGIC)+5)

/*1.3.6.1.2.1.2.1*/
#define OID_IfNumber ((Basic_INFO_MAGIC)+6)

/* 1.3.6.1.4.1.33003.7.1 */
#define OID_ntpStatus ((Basic_INFO_MAGIC)+7)
/* 1.3.6.1.4.1.33003.7.2 */
#define OID_ntpPrimaryServer ((Basic_INFO_MAGIC)+8)
/* 1.3.6.1.4.1.33003.7.3 */
#define OID_ntpSecondServer  ((Basic_INFO_MAGIC)+9)

#ifdef CONFIG_OPENSOURCE_STREAMRATE  
/* 1.3.6.1.4.1.33003.9.1 */
#define OID_UpstreamCurrRate ((Basic_INFO_MAGIC)+10)
/* 1.3.6.1.4.1.33003.9.2 */
#define OID_DownstreamCurrRate  ((Basic_INFO_MAGIC)+11)
#endif

/*1.3.6.1.4.1.33003.1.1*/  
#define OID_ethDhcpsStatus   ((Basic_INFO_MAGIC)+1)
/*1.3.6.1.4.1.33003.1.2*/
#define OID_ethDhcpsIPStart ((Basic_INFO_MAGIC)+2)
/*1.3.6.1.4.1.33003.1.3 */
#define OID_ethDhcpsIPEnd   ((Basic_INFO_MAGIC)+3)
/*1.3.6.1.4.1.33003.1.4*/
#define OID_ethDhcpsNetmask ((Basic_INFO_MAGIC)+4)


/*1.3.6.1.4.1.33003.8.1*/  
#define OID_cpuThresholdValue ((Basic_INFO_MAGIC)+1)
/*1.3.6.1.4.1.33003.8.2*/
#define OID_memThresholdValue ((Basic_INFO_MAGIC)+2)

#define OID_rebootCPE         ((Basic_INFO_MAGIC)+3)
#define OID_recoverConfigCPE  ((Basic_INFO_MAGIC)+4)
#define OID_factoryOUI        ((Basic_INFO_MAGIC)+5)

/* 1 .3.6.1.2.1.1.2 */
#define OID_sysObjId Basic_INFO_MAGIC
#endif  /*_TBSPRIVATE_ADSLNODES_H_*/
