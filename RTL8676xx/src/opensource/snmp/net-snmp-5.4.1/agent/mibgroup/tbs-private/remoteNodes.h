#ifndef _TBSPRIVATE_REMOTENODES_H_
#define _TBSPRIVATE_REMOTENODES_H_

config_require(tbs-private/common/cmmif);
config_require(tbs-private/common/commhandle);
config_require(tbs-private/common/writeMethod);
config_require(util_funcs);

#include "common/commhandle.h"
/*Radius*/
#define OID_radiusAuthPrimaryServerIP 1
#define OID_radiusAuthPrimaryServerPort 2
#define OID_radiusAuthPrimaryServerSecret 3
#define OID_radiusAuthSecondaryServerIP 4
#define OID_radiusAuthSecondaryServerPort 5
#define OID_radiusAuthSecondaryServerSecret 6
#define OID_radiusAccountPrimaryServerIP 7
#define OID_radiusAccountPrimaryServerPort 8
#define OID_radiusAccountPrimaryServerSecret 9
#define OID_radiusAccountSecondaryServerIP 10
#define OID_radiusAccountSecondaryServerPort 11
#define OID_radiusAccountSecondaryServerSecret 12

/*BasicSetting*/
#define OID_wirelessMode 13
#define OID_channel 14
#define OID_txRate 15
#define OID_fragmentationThreshold 16
#define OID_rtsThreshold 17
#define OID_txPower 18
#define OID_superMode 19
#define OID_wmmSupport 20
#define OID_linkintegrity 21
#define OID_wlanMaxStaNum 22
#define OID_radio 23

typedef struct wlanBasicSettingTable_data_tag
{
    unsigned long wlanInstanceID;
    unsigned long wirelessMode;
    unsigned long channel;
    unsigned long txRate;
    unsigned long fragmentationThreshold;
    unsigned long rtsThreshold;
    unsigned long txPower;
    unsigned long superMode;
    unsigned long wmmSupport;
    unsigned long linkintegrity;
    unsigned long wlanMaxStaNum;
    unsigned long radio;

    time_t cachTime;
}wlanBasicSettingTable_data;
#define TR069_NODE_wirelessMode "Standard"
#define TR069_NODE_channel "Channel"
#define TR069_NODE_txRate "MaxBitRate"
#define TR069_NODE_fragmentationThreshold "X_TWSZ-COM_FRAGLEN"
#define TR069_NODE_rtsThreshold "X_TWSZ-COM_RCSCTSThreshlod"
#define TR069_NODE_txPower "X_TWSZ-COM_Power"
#define TR069_NODE_superMode "X_TWSZ-COM_XR"
#define TR069_NODE_wmmSupport "X_TWSZ-COM_WMMEnabled"
#define TR069_NODE_linkintegrity "X_TWSZ-COM_ConnCompleteEnabled"
#define TR069_NODE_wlanMaxStaNum "X_TWSZ-COM_MAXSTATION"
#define TR069_NODE_radio "X_TWSZ-COM_WirelessEnable"

/*VapSetting*/
#define OID_broadcastSsid 1
#define OID_beaconInterval  2
#define OID_dtim 3
#define OID_deviceOperationMode 4
typedef struct wlanVapSettingTable_data_tag
{
    unsigned long vapInstanceId;
    unsigned long broadcastSsid;
    unsigned long beaconInterval;
    unsigned long dtim;
    unsigned long accessControlMode;
    
    time_t cachTime;
}wlanVapSettingTable_data;

#define TR069_NODE_broadcastSsid "X_TWSZ-COM_HideSSID"
#define TR069_NODE_beaconInterval "X_TWSZ-COM_BeaconInterval"
#define TR069_NODE_dtim "X_TWSZ-COM_DTIM"
#define TR069_NODE_deviceOperationMode "DeviceOperationMode"
#define TR069_NODE_Standard "Standard"

/*EthernetStats*/
#define OID_ethernetRXPackets 1
#define OID_ethernetRXBytes 2
#define OID_ethernetTXPackets 3
#define OID_ethernetTXBytes 4
#define OID_ethernetRXPacketsError 5
#define OID_ethernetRXPacketsDropped 6
#define OID_ethernetTXPacketsError 7
#define OID_ethernetTXPacketsDropped 8
#define OID_ethernetUpkbps 9
#define OID_ethernetDownkbps 10
#define OID_ethernetLossTolerance 11
#define OID_ethernetTXRetryPackts 12
#define OID_ethernetRXRetryPackts 13

/*WirelessStatsTable*/
#define OID_wirelessRXPacketsTotal 1
#define OID_wirelessRXBytesTotal 2
#define OID_wirelessTXPacketsTotal 3
#define OID_wirelessTXBytesTotal 4
#define OID_wirlessRXPacketsError 5
#define OID_wirlessRXPacketsDropped 6
#define OID_wirlessTXPacketsError 7
#define OID_wirlessTXPacketsDropped 8
#define OID_wirelessRXPacketsDroppedRate 9
#define OID_wirelessAssocUserSum 10
#define TR069_ROOT_COM_WLAN "InternetGatewayDevice.X-TWSZ_COM_WLAN."
typedef struct wirelessStatsTable_data_tag
{
    unsigned long wirelessStatsInstanceId;
    unsigned long wirelessRXPacketsTotal;
    unsigned long wirelessRXBytesTotal;
    unsigned long wirelessTXPacketsTotal;
    unsigned long wirelessTXBytesTotal;
    unsigned long wirlessRXPacketsError;
    unsigned long wirlessRXPacketsDropped;
    unsigned long wirlessTXPacketsError;
    unsigned long wirlessTXPacketsDropped;
    char wirelessRXPacketsDroppedRate[32];
    unsigned long wirelessAssocUserSum;

    time_t cachTime;
}wirelessStatsTable_data;

/*WlanClientNumberTable*/
#define OID_wlanClientNumber 1
/*WlanClientNumberPercentTable*/
#define OID_wlanClientNumberPercent 2
typedef struct wlanClientNumberTable_data_tag
{
    unsigned long wlanClientInstanceId; 
    unsigned long wlanClientNumber;
    unsigned long wlanMaxStation;
    char wlanClientNumberPercent[32];

    time_t cachTime;
}wlanClientNumberTable_data;

/*WlanAssociateClient*/
#define OID_clientIndex 1
#define OID_clientMac 2
#define OID_clientRssi 3
#define OID_clientStatus 4
typedef struct wlanClientTable_data_tag
{
    unsigned long clientIndex;
    char clientMac[48];
    unsigned long clientRssi;
    char clientStatus[32];

    time_t cachTime;
}wlanClientTable_data;
#define TR069_ROOT_wlanConfiguration "InternetGatewayDevice.LANDevice.1.WLANConfiguration."
#define TR069_NODE_AssociatedDevice "AssociatedDevice"

/*WlanWepTable*/
#define OID_wepKeyType 1
#define OID_wepKey1 2
#define OID_wepKey2 3
#define OID_wepKey3 4
#define OID_wepKey4 5
typedef struct wlanWepTable_data_tag
{
    unsigned long configInstanceId;
    unsigned long wepKeyType;
    
    char wepKey1[136];
    char wepKey2[136];
    char wepKey3[136];
    char wepKey4[136];
    
    time_t cachTime;
} wlanWepTable_data;
#define TR069_ROOT_WepKey 	"WEPKey"
#define TR069_NODE_WepKey 	"WEPKey"
#define TR069_NODE_WepEncryptionLevel "WEPEncryptionLevel"

/*VAPSettingTable*/
#define OID_vapIndex 1
#define OID_vapEnable 2
#define OID_vapssid 3
#define OID_hidenetworkname 4
#define OID_presharekey 5
#define OID_authenticationtype 6
#define OID_encryption 7
#define OID_wanseparator 8
#define OID_vapMacAddr 9
typedef struct wlanVapTable_data_tag
{
    unsigned long vapInstanceId;

    unsigned long vapIndex;
    unsigned long vapEnable;
    char vapssid[72];
    unsigned long hidenetworkname;
    char presharekey[72];
    unsigned long authenticationtype;
    unsigned long encryption;
    unsigned long wlanseparator;
    char vapMacAddr[48];
    
    time_t cachTime;
} wlanVapTable_data;
#define TR069_NODE_vapIndex "X_TWSZ-COM_VapIndex"
#define TR069_NODE_vapEnable "Enable"
#define TR069_NODE_vapssid "SSID"
#define TR069_NODE_hidenetworkname "X_TWSZ-COM_HideSSID"
#define TR069_NODE_presharekey64Hex "PreSharedKey.1.PreSharedKey"
#define TR069_NODE_presharekey63Asc "PreSharedKey.1.KeyPassphrase"
#define TR069_NODE_wlanseparator "X_TWSZ-COM_APIsolate"
#define TR069_NODE_vapMacAddr "PreSharedKey.1.AssociatedDeviceMACAddress"

#define TR069_NODE_BeaconType "BeaconType"
#define TR069_NODE_BasicAuthenticationMode "BasicAuthenticationMode"
#define TR069_NODE_WPAAuthenticationMode "WPAAuthenticationMode"
#define TR069_NODE_IEEE11iAuthenticationMode "IEEE11iAuthenticationMode"
#define TR069_NODE_BasicEncryptionMode "BasicEncryptionModes"
#define TR069_NODE_WPAEncryptionMode "WPAEncryptionModes"
#define TR069_NODE_IEEE11iEncryptionMode "IEEE11iEncryptionModes"
/* 1.3.6.1.4.1.33003.2.1.1.1 */
#define OID_wanInfoIndex 1 
/* 1.3.6.1.4.1.33003.2.1.1.2 */
#define OID_wanConType   2
/* 1.3.6.1.4.1.33003.2.1.1.3 */
#define OID_wanIpAddr 3
/* 1.3.6.1.4.1.33003.2.1.1.4 */
#define OID_wanNetMask 4
/* 1.3.6.1.4.1.33003.2.1.1.5 */
#define OID_wanDefGateWay 5
/* 1.3.6.1.4.1.33003.2.1.1.6 */
#define OID_wanPrimaryDNS 6
/* 1.3.6.1.4.1.33003.2.1.1.7 */
#define OID_wanSecondaryDNS 7
/* 1.3.6.1.4.1.33003.2.1.1.8 */
#define OID_wanMtuValue 8

typedef struct wanInfoTable_data_tag
{
   unsigned long wanInstanceId;
   unsigned long wanInfoIndex;
   unsigned char wanConType[20];
   unsigned char wanIpAddr[32];
   unsigned char wanNetMask[32];
   unsigned char wanDefGateWay[32];
   unsigned char wanPrimaryDNS[32];
   unsigned char wanSecondaryDNS[32];
   unsigned long wanMtuValue ;

    time_t cachTime;
} wanInfoTable_data;

/*1.3.6.1.4.1.33003.4.1.1.1 */
#define OID_wlanSSID 1
/*1.3.6.1.4.1.33003.4.1.1.2  */
#define OID_wlanSSIDBroadcast 2
/* 1.3.6.1.4.1.33003.4.1.1.3 */
#define OID_wlanSSIDStatus 3
/* 1.3.6.1.4.1.33003.4.1.1.4 */
#define OID_wlanStatus 4
/* 1.3.6.1.4.1.33003.4.1.1.5 */
#define OID_wlanNetMode 5

typedef struct wlanInfoTable_data_tag
{
    unsigned long wlanInstanceId;  
    unsigned char wlanSSID[32] ; //SSID
	unsigned long wlanSSIDBroadcast;// X_TWSZ-COM_HideSSID
	unsigned long wlanSSIDStatus;//Enable
	unsigned long wlanStatus;//Status
	unsigned long wlanNetMode;//Standard

	time_t cachTime;
}wlanInfoTable_data;

/* 1.3.6.1.4.1.33003.3.1.1 */
#define OID_vlanId  1
/* 1.3.6.1.4.1.33004.3.1.1 */
#define OID_portList 2
typedef struct vlanInfoTable_data_tag
{
	 unsigned long vlanInstanceId;  
     unsigned long vlanId;
	 unsigned char portList[128];
	 time_t cachTime ;
}vlanInfoTable_data;

#define TR069_ROOT_ipRouteTbl "InternetGatewayDevice.Layer3Forwarding.Forwarding."
#define TR069_ROOT_wanInfoTbl "InternetGatewayDevice.WANDevice.1.WANConnectionDevice."
#define TR069_ROOT_wlanInfoTbl "InternetGatewayDevice.LANDevice.1.WLANConfiguration."
#define TR069_ROOT_wlanEnable  "InternetGatewayDevice.X_TWSZ-COM_WLAN."

#define TR069_NODE_wLanSSID  "SSID"
#define TR069_NODE_wlanSSIDBroadcast  "X_TWSZ-COM_HideSSID"
#define TR069_NODE_wlanSSIDStatus  "Enable"
#define TR069_NODE_wlanStatus  "WirelessEnable"
#define TR069_NODE_wlanNetMode  "Standard"

/*Wireless Beacon Type*/
#define TR069_VALUE_BeaconType "None"
#define TR069_VALUE_BasicAuthenticationMode "Basic"
#define TR069_VALUE_WPAAuthenticationMode "WPA"
#define TR069_VALUE_IEEE11iAuthenticationMode "11i"

/*Wireless Authentication Type*/
#define TR069_VALUE_SharedAuthentication "SharedAuthentication"
#define TR069_VALUE_PSKAuthentication "PSKAuthentication"
#define TR069_VALUE_EAPAuthentication "EAPAuthentication"

/*Wireless Encryption Type*/
#define TR069_ENC_BASIC_NONE "None"
#define TR069_ENC_BASIC_WEP "WEPEncryption"
#define TR069_ENC_WPA_TKIP "TKIPEncryption"
#define TR069_ENC_WPA2_TKIP "TKIPEncryption"
#define TR069_ENC_WPA2_AES "AESEncryption"


/*vapDataTable*/
#define OID_vapMtu 2
#define OID_vapWirelessRXBytesTotal 3
#define OID_vapWirelessTXBytesTotal 4
#define OID_vapWirelessRXPacketsTotal 5
#define OID_vapWirelessTXPacketsTotal 6
#define OID_vapClientCnt 7
typedef struct wlanVapDataTable_data_tag
{
    unsigned long vapInstanceId;

    unsigned long vapMtu;
    unsigned long vapWirelessRXBytesTotal;
    unsigned long vapWirelessTXBytesTotal;
    unsigned long vapWirelessRXPacketsTotal;
    unsigned long vapWirelessTXPacketsTotal;
    unsigned long vapClientCnt;
    
    time_t cachTime;
} wlanVapDataTable_data;
#define TR069_NODE_vapWirelessRXBytesTotal "TotalBytesReceived"
#define TR069_NODE_vapWirelessTXBytesTotal "TotalPacketsSent"
#define TR069_NODE_vapWirelessRXPacketsTotal "TotalPacketsReceived"
#define TR069_NODE_vapWirelessTXPacketsTotal "TotalPacketsSent"
#define TR069_NODE_vapClientCnt "TotalAssociations"

/*instanceNO=1 means primary; instanceNO=2 means secondary
 *WLANConfiguration has multiple instances, but we just use the first.
 */
#define TR069_ROOT_Radius "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_RadiusServer."

int GetIpAddrWanNode(unsigned long ulConnNo,char* pszWanInfoNodePrefix, char* keyid, char* retValue,unsigned long ulSize);
int GetStrWanNode(unsigned long ulConnNo,char* pszWanInfoNodePrefix, char* keyid, char* retValue,unsigned long ulSize);
#endif    /*_TBSPRIVATE_WIRELESSNODES_H_*/
