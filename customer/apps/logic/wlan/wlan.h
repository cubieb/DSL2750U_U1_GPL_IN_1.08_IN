/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: wlan.h
 文件描述: WLAN头文件
 修订记录:
        1. 作者: wuguoxiang
           日期: 2008-11-08
           内容: 创建文件
**********************************************************************/


#ifndef _WLAN_H_
#define _WLAN_H_

#include "common.h"
#include "tbsutil.h"
#include "tbsmsg.h"
#include "tbserror.h"
#include "cfg_api.h"
#include "pc_api.h"

#include "wl_api.h"

#include "autoconf.h"  //for CONFIG_WLAN_WPS definitions


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define WMM_ENABLE 1
//#define WLAN_DEBUG
/* print switch */
#ifdef WLAN_DEBUG
//#define WLAN_TRACE(fmt, args...) TBS_TRACE(MID_WLAN, fmt, ##args)
#define WLAN_TRACE(fmt, args...) printf(fmt, ##args)
#else
#define WLAN_TRACE(str, args...)  do { ; } while(0);
#endif

// no_print or printf
#define WLAN_ERROR  printf
//#define WLAN_TRACE  printf
#define WLAN_INFO    printf

/* wlan 参数最大节点级数 */
#define MAX_WLAN_INDEX_COUNT 3

#define MAX_VAP_NUM 5
#define MAX_BRG_NAME_LEN           16

/* 一些参数的范围设定 */
#define CHANNEL_MIN     1
#define CHANNEL_MAX     14
#define BEACONP_MIN      20
#define BEACONP_MAX      1000
#define THRESHOLD_MIN   256
#ifdef CONFIG_TELEFONICA_COLOMBIA
#define THRESHOLD_MAX   2347
#else
#define THRESHOLD_MAX   2346
#endif
#define WEPKEY_IDX_MIN  1
#define WEPKEY_IDX_MAX  4
#define MAX_COUNTRYNAME_LEN 45
#define DTIM_MIN_INTERVAL  1 
#define DTIM_MAX_INTERVAL 255

#define MAX_SSID_NUM 8


#define STATIC_NODE_DEFAULT_ROUTE   "X_TWSZ-COM_DEFAULT_GW"
/* wlan 模块的节点参数名称定义, to be added*/
#define NODE_WLAN_WIRELESS_APISOLATION_ENABLE		"WirelessMultiAPIsolationEnable"	  /* 全局*/
#define NODE_WLAN_WIRELESS_ENABLE		"WirelessEnable"	                     /* 全局*/
#define NODE_WLAN_WIRELESS_TYPE		"WirelessType"	                     /* 全局*/
#define NODE_WLAN_RELOAD_DRIVER_FINISHED		"ReloadDriverFinished"	                     /* 全局*/
#define NODE_WLAN_STANDARD				"Standard"					/* 全局*/
#define NODE_WLAN_BANDWIDTH			"X_TWSZ-COM_Bandwidth"			/* 全局*/
#define NODE_WLAN_CHANNEL				"Channel"						/* 全局*/
#define NODE_WLAN_POWER				"X_TWSZ-COM_Power"			       /* 全局*/
#define NODE_WLAN_RATE					"MaxBitRate"					/* 全局*/
#define NODE_WLAN_MULTICASTRATE			"X_TWSZ-COM_MulticastRate"
#define NODE_WLAN_MACCTRL_ENABLE		"MACAddressControlEnabled"		/* 全局*/
#define NODE_WLAN_BEACON_INTERVAL		"X_TWSZ-COM_BeaconInterval"	/* 全局*/
#define NODE_WLAN_CONN_COMPLETE_ENABLE   "ConnCompleteEnabled"            /* 全局*/
#define NODE_WLAN_FRAGLEN  "X_TWSZ-COM_FRAGLEN"                                     /* 全局*/
#define NODE_WLAN_DTIM         "X_TWSZ-COM_DTIM"                                          /* 全局*/
#define NODE_WLAN_XR              "X_TWSZ-COM_XR"                                             /* 全局*/
#define NODE_WLAN_RECONFIG_FINISH_FLAG "X_TWSZ-COM_ReconfigFinishFlag"  /* 全局*/
#define NODE_WLAN_CURRENT_COUNTRY      "X_TWSZ-COM_CurrentCountry"         /*全局*/
#define NODE_WLAN_CONTROL_SIDEBAND      "X_TWSZ-COM_ControlSideband"         /*全局*/


#define NODE_WLAN_VAPINDEX				"X_TWSZ-COM_VapIndex"
#define NODE_WLAN_DHCPS_ENABLE				"X_TWSZ-COM_DHCPEnabled"
#define NODE_WLAN_ENABLE				"Enable"
#define NODE_WLAN_CHANGE_SSID				"FlageChangeSSID"
#define NODE_WLAN_PREAMBLETYPE 			"X_TWSZ-COM_PreambleType"
#define NODE_WLAN_STATUS				"Status"
#define NODE_WLAN_BSSID				"BSSID"
//#define NODE_WLAN_HIDESSID				"X_TWSZ-COM_HideSSID"
#define NODE_WLAN_HIDESSID				"SSIDAdvertisementEnabled"
#define NODE_WLAN_BEACON_TYPE			"BeaconType"
#define NODE_WLAN_SSID					"SSID"
#define NODE_WLAN_RTS_THRESHOLD		"X_TWSZ-COM_RCSCTSThreshlod"
#define NODE_WLAN_WEPKEYIDX			"WEPKeyIndex"
#define NODE_WLAN_WEPENC				"WEPEncryptionLevel"
#define NODE_WLAN_BASIC_ENC_MODE		"BasicEncryptionModes"
#define NODE_WLAN_BASIC_AUTH_MODE		"BasicAuthenticationMode"
#define NODE_WLAN_WPA_ENC_MODE		"WPAEncryptionModes"
#define NODE_WLAN_WPA_AUTH_MODE		"WPAAuthenticationMode"
#define NODE_WLAN_WPA_GROUP_REKEY "X_TWSZ-COM_WPAGroupRekey"
#define NODE_WLAN_WPA_STRICT_REKEY "X_TWSZ-COM_WPAStrictRekey"
#define NODE_WLAN_IEEE11i_ENC_MODE		"IEEE11iEncryptionModes"
#define NODE_WLAN_IEEE11i_AUTH_MODE		"IEEE11iAuthenticationMode"
#define NODE_WLAN_PSK_EXPRESSION		"X_TWSZ-COM_PSKExpression"

#define NODE_WLAN_POSSIBLE_CHANNELS		"PossibleChannels"
#define NODE_WLAN_BASIC_DATATXRATES		"BasicDataTransmitRates"
#define NODE_WLAN_OPERATIONAL_DATATXRATES		"OperationalDataTransmitRates"
#define NODE_WLAN_POSSIBLE_DATATXRATES		"PossibleDataTransmitRates"

#define NODE_WLAN_WMM_ENABLE            "X_TWSZ-COM_WMMEnabled"     /*wmm*/
#define NODE_WLAN_MAX_STATION           "X_TWSZ-COM_MAXSTATION"     /*max station*/

#define NODE_WLAN_NAME            "X_TWSZ-COM_DeviceName"     /*wlan-name*/
#define NODE_WLAN_AP_ISOLATE "X_TWSZ-COM_APIsolate"         /* AP间隔离*/
#define NODE_WLAN_BSSRELAY "X_TWSZ-COM_BSSRelay"            /* AP转发*/
#define NODE_WLAN_POWER_SAVE "X_TWSZ-COM_PowerSave"
#define NODE_WLAN_RADIUS_BAK_ENABLE "X_TWSZ-COM_BakRadiusEnable"
#define NODE_WLAN_RADIUS_ACCOUNT_ENABLE "X_TWSZ-COM_AccountEnable"
#define NODE_WLAN_RADIUS_ACCOUNT_INTERVAL    "X_TWSZ-COM_AccountInterval"   //account interval

/* others */
#define NODE_WLAN_INSECURE_OOB_ACCESS_ENABLED       "InsecureOOBAccessEnabled"
#define NODE_WLAN_BEACON_ADV_ENABLE       "BeaconAdvertisementEnabled"
#define NODE_WLAN_RADIO_ENABLED       "RadioEnabled"
#define NODE_WLAN_AUTORATE_FALLBACK_ENABLED       "AutoRateFallBackEnabled"
#define NODE_WLAN_LOCATION_DESCRIPTION       "LocationDescription"
#define NODE_WLAN_REGULATORY_DOMAIN       "RegulatoryDomain"
#define NODE_WLAN_TOTAL_PSKFAILURES       "TotalPSKFailures"
#define NODE_WLAN_TOTAL_INTEGRITY_FAILURES       "TotalIntegrityFailures"

#define NODE_WLAN_AUTH_SERV_MODE       "AuthenticationServiceMode"


#if 0
#define NODE_WLAN_WPS_CONFIGURED   "X_TWSZ-COM_Configured"
#endif

/*下述为只读的属性*/
#define NODE_WLAN_CHANNEL_IN_USE		"ChannelsInUse"
#define NODE_WLAN_TOTAL_BYTES_TX		"TotalBytesSent"
#define NODE_WLAN_TOTAL_BYTES_RX		"TotalBytesReceived"
#define NODE_WLAN_TOTAL_PACKETS_TX		"TotalPacketsSent"
#define NODE_WLAN_TOTAL_PACKETS_RX		"TotalPacketsReceived"
#define NODE_WLAN_TOTAL_DROP_TX			"X_TWSZ-COM_TotalTxDrop"
#define NODE_WLAN_TOTAL_DROP_RX			"X_TWSZ-COM_TotalRxDrop"
#define NODE_WLAN_TOTAL_ERROR_TX		"X_TWSZ-COM_TotalTxError"
#define NODE_WLAN_TOTAL_ERROR_RX		"X_TWSZ-COM_TotalRxError"

#define NODE_WLAN_UNICAST_PACKETS_TX		"Stats.UnicastPacketsSent"
#define NODE_WLAN_UNICAST_PACKETS_RX		"Stats.UnicastPacketsReceived"

#define NODE_WLAN_MULTICAST_PACKETS_TX		"Stats.MulticastPacketsSent"
#define NODE_WLAN_MULTICAST_PACKETS_RX		"Stats.MulticastPacketsReceived"

#define NODE_WLAN_BCASTCAST_PACKETS_TX		"Stats.BroadcastPacketsSent"
#define NODE_WLAN_BCASTCAST_PACKETS_RX		"Stats.BroadcastPacketsReceived"


#define NODE_WLAN_TOTAL_ASSOCIATIONS "TotalAssociations"

#define NODE_WLAN_ASSO_MAC "AssociatedDeviceMACAddress"
#define NODE_WLAN_ASSO_RATE "X_TWSZ-COM_AssociatedDeviceRate"
#define NODE_WLAN_ASSO_RSSI "X_TWSZ-COM_AssociatedDeviceRSSI"
#define NODE_WLAN_ASSO_STATE "X_TWSZ-COM_AssociatedDeviceState"
#define NODE_WLAN_ASSO_TYPE "X_TWSZ-COM_AssociatedDeviceType"
#define NODE_WLAN_ASSO_AUTH_STATE "AssociatedDeviceAuthenticationState"
#define NODE_WLAN_ASSO_IP    "AssociatedDeviceIPAddress"
#define NODE_WLAN_ASSO_TRANS_RATE "LastDataTransmitRate"

/* 单独处理的属性*/
#define NODE_WLAN_WEPKEY				"WEPKey"                  // hexadecima string wep key
#define NODE_WLAN_WPA_KEY_PASSPHRASE	"KeyPassphrase"           // wpa key passphrase
#define NODE_WLAN_WPA_PSK				"PreSharedKey"           // wpa key psk
#define NODE_RADIUS_IPADDR				"X_TWSZ-COM_IPAddress"			// radisu ip
#define NODE_RADIUS_PORT				"X_TWSZ-COM_Port"				// radisu port
#define NODE_RADIUS_KEY					"X_TWSZ-COM_ShareKey"			// radisu key
#define NODE_RADIUS_ACCOUNT_PORT           "X_TWSZ-COM_AccountPort"                 //account port
/*Peter Meng add define for WDS at 2008-07-14 */
#define NODE_WLAN_DEVICEOPERATIONMODE			"DeviceOperationMode"
#define NODE_WLAN_WDSENABLED			"X_TWSZ-COM_WDSEnabled"
#define NODE_WLAN_PEERBSSID				"PeerBSSID"
#define NODE_WLAN_DISTANCEFROMROOT			"DistanceFromRoot"
/* 从配置树节点读出的节点值字符串 */
#define NODE_VAL_TRUE						"True"
#define NODE_VAL_FALSE						"False"
#define NODE_VAL_TRUE1						"1"
#define NODE_VAL_FALSE0					"0"
#define NODE_VAL_DEFAULT_SSID				"ssid_temp"
#define NODE_RATE_AUTO						"Auto"
#define BEACON_TYPE_NONE					"None"
#define BEACON_TYPE_BASIC					"Basic"
#define BEACON_TYPE_WPA						"WPA"
#define BEACON_TYPE_11i						"11i"
#define BEACON_TYPE_WPA2_MIXED 				"WPAand11i"
#define BASIC_ENC_MODE_NONE					"None"
#define BASIC_ENC_MODE_WEP					"WEPEncryption"
#define WEP_ENC_DIABLED						"Disabled"
#define WEP_ENC_40BIT						"40-bit"
#define WEP_ENC_104BIT						"104-bit"
#define BASIC_AUTH_MODE_NONE				"None"
#define BASIC_AUTH_MODE_SHARE				"SharedAuthentication"
#define WPA_ENC_MODE_TKIP						"TKIPEncryption"
#define WPA_AUTH_MODE_PSK					"PSKAuthentication"
#define WPA_AUTH_MODE_EAP					"EAPAuthentication"
#define WPA2_ENCMODE_TKIP					"TKIPEncryption"
#define WPA2_ENCMODE_AES					"AESEncryption"
#define WPA2_ENCMODE_TKIPANDAES			"TKIPandAESEncryption"
#define STANDARD_A							"a"
#define STANDARD_B							"b"
#define STANDARD_G							"g"
#define STANDARD_N							"n"
#define STANDARD_BG						"bg"
#define STANDARD_NG						"ng"
#define STANDARD_BGN					"bgn"
#define N_BANDWIDTH_AUTO					"0"
#define N_BANDWIDTH_20M					"20"
#define N_BANDWIDTH_40M					"40"
#define N_WirelessType_1T2R					"0"
#define N_WirelessType_2T2R					"1"

#define POWER_AUTO                          "auto"
#define POWER_0 							"0"
#define POWER_1_OF_8						"12.5"
#define POWER_1_OF_20							"20"
#define POWER_1_OF_4						"25"
#define POWER_1_OF_40							"40"
#define POWER_1_OF_2						"50"
#define POWER_1_OF_60							"60"
#define POWER_1_OF_80							"80"
#define POWER_FULL							"100"
#define MAX_BITRATE							"MaxBitRate" /* 待扩展*/

/* 生成路径的格式字符串*/
#define LAN_DEVICE_NAME_PATH "InternetGatewayDevice.LANDevice.%d.X_TWSZ-COM_DeviceName"
#define WLAN_DEV_PATH "InternetGatewayDevice.LANDevice."

#define WLAN_CFG_NODE_FMT WLAN_DEV_PATH "%d.WLANConfiguration."
#define WLAN_RECONFIG_FINISH_FLAG_PATH WLAN_DEV_PATH "%d." NODE_WLAN_RECONFIG_FINISH_FLAG
#define WLAN_ADDDEL_NODE_FMT WLAN_DEV_PATH "%d.WLANConfiguration.%d"
#define WLAN_CFG_PATH_FMT WLAN_DEV_PATH "%d.WLANConfiguration.%d.%s"
#define WLAN_CFG_WEPKEY_FMT WLAN_DEV_PATH "%d.WLANConfiguration.%d.WEPKey.%d.WEPKey"
#define WLAN_CFG_WPA_PASS_PHRASE_FMT WLAN_DEV_PATH "%d.WLANConfiguration.%d.PreSharedKey.%d.KeyPassphrase"
#define WLAN_CFG_WPA_PSK_FMT WLAN_DEV_PATH "%d.WLANConfiguration.%d.PreSharedKey.%d.PreSharedKey"
#define WLAN_CFG_RADIUS_FMT WLAN_DEV_PATH "%d.WLANConfiguration.%d.X_TWSZ-COM_RadiusServer.%d.%s"
#define WLAN_CFG_ASSO_DEV_FMT WLAN_DEV_PATH"%d.WLANConfiguration.%d.AssociatedDevice.%d.%s"
#define WLAN_CFG_ASSO_PATH_FMT WLAN_DEV_PATH"%d.WLANConfiguration.%d.AssociatedDevice."

#if 0
#define WLAN_CFG_WPS_FMT WLAN_DEV_PATH "%d.WLANConfiguration.%d.X_TWSZ-COM_WPS.%s"
#endif
#define STATIC_MAIN_PATH        "InternetGatewayDevice"
#define WHITELIST_NODE_NAME ".LANHostConfigManagement.AllowedMACAddresses"

#define WLAN_EACH_VAP_PATH "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%."
#define WLAN_STATISTIC_BASIC_PATH "InternetGatewayDevice.X_TWSZ-COM_WLAN."
/* wan路径 */
#define STR_WANDEV "InternetGatewayDevice.WANDevice"
#define STR_WANCONNDEV "WANConnectionDevice"

#define WLAN_WAN_CONN_PATRN_IP_CONN     "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}\\.$"
#define WLAN_WAN_CONN_PATRN_PPP_CONN    "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}\\.$"
#define WLAN_WLAN_CONN_PATRN_SUB_TABLE  "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}\\.WLANConfiguration\\.[0-9]{1,5}\\..+"

/* 临时ssid 字符串格式 */
#define TEMP_SSID_FMT  "newssid_%lu_%lu"

#define INVALID_VAP_INDEX    0xffffffff

#define MAX_WEPKEY_NODE_INDEX    4
#define MAX_WPAPSK_NODE_INDEX    1
#define MAX_RADIUS_NODE_INDEX    2
#define MAX_ASSOCIATED_NODE_INDEX 32

#define MAX_RADIUS_PORT_LEN        9
#define MAX_WLAN_IFNAME_LEN      15
#define MAX_INTF_NAME_LEN 10

#define MAX_MACLIST_LEN                   512

#define MAX_DEV_NAME_LEN  20

#define MAX_STR_LEN   1024

/* 模块内部使用的三级节点类型定义 */
#define TYPE_BOOL   0
#define TYPE_INT     1
#define TYPE_STR     2

#define VAP_ADD_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.WLANConfiguration\\.$"
#define VAP_DEL_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.WLANConfiguration\\.[0-9]+\\.$"

//#define WLAN_BASE_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.WLANConfiguration\\.[0-9]+"
#define WLAN_FULL_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.WLANConfiguration\\.[0-9]+"
#define WLAN_BASE_PATH_REGEX ".*"

#define WLAN_WIRLESS_APISOLATION_ENABLE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WIRELESS_APISOLATION_ENABLE"$"
#define WLAN_WIRLESS_ENABLE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WIRELESS_ENABLE"$"
#define WLAN_DHCPS_ENABLE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_DHCPS_ENABLE"$"
#define WLAN_RELOAD_DRIVER_FINISHED_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_RELOAD_DRIVER_FINISHED"$"
#define WLAN_VAPIDX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_VAPINDEX"$"
#define WLAN_AP_ENABLE_REGEX WLAN_FULL_PATH_REGEX"\\."NODE_WLAN_ENABLE"$"
#define WLAN_PREAMBLETYPE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_PREAMBLETYPE"$"
#define WLAN_STATUS_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_STATUS"$"
#define WLAN_BSSID_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_BSSID"$"
#define WLAN_HIDESSID_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_HIDESSID"$"
#define WLAN_CURR_COUNTRY_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_CURRENT_COUNTRY"$"
#define WLAN_CONTROL_SIDEBAND_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_CONTROL_SIDEBAND"$"
#define WLAN_MAX_BITRATE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_RATE"$"
#define WLAN_CHANNEL_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_CHANNEL"$"
#define WLAN_BEACON_INTERVAL_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_BEACON_INTERVAL"$"
#define WLAN_RCSCTS_THRESHHOLD_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_RTS_THRESHOLD"$"
#define WLAN_SSID_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_SSID"$"
#define WLAN_BEACON_TYPE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_BEACON_TYPE"$"
#define WLAN_DEV_NAME_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_NAME"$"
#define WLAN_WMM_ENABLED_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WMM_ENABLE"$"
#define WLAN_MAX_STATION_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_MAX_STATION"$"
#define WLAN_AP_ISOLATE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_AP_ISOLATE"$"
#define WLAN_BSSRELAY_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_BSSRELAY"$"
#define WLAN_POWER_SAVE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_POWER_SAVE"$"
#define WLAN_CONN_COMPLETE_ENABLED_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_CONN_COMPLETE_ENABLE"$"
#define WLAN_FRAG_LEN_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_FRAGLEN"$"
#define WLAN_DTIM_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_DTIM"$"
#define WLAN_XR_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_XR"$"
#define WLAN_ACCOUNT_ENABLE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_RADIUS_ACCOUNT_ENABLE"$"
#define WLAN_ACCOUNT_INTERVAL_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_RADIUS_ACCOUNT_INTERVAL"$"
#define WLAN_BAK_RADIUS_ENABLE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_RADIUS_BAK_ENABLE"$"
#define WLAN_WPA_STRICT_REKEY_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WPA_STRICT_REKEY"$"
#define WLAN_WPA_GROUP_REKEY_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WPA_GROUP_REKEY"$"
#define WLAN_MACCTL_ENABLED_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_MACCTRL_ENABLE"$"
#define WLAN_STANDARD_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_STANDARD"$"
#define WLAN_BANDWIDTH_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_BANDWIDTH"$"
#define WLAN_POWER_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_POWER"$"
#define WLAN_MULTICASTRATE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_MULTICASTRATE"$"

#define WLAN_WEP_KEYIDX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WEPKEYIDX"$"
#ifdef CONFIG_TELEFONICA_COLOMBIA
#define WLAN_KEY_PASSPHRASE_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.WLANConfiguration\\.[0-9]+\\."NODE_WLAN_WPA_KEY_PASSPHRASE"$"
#else
#define WLAN_KEY_PASSPHRASE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WPA_KEY_PASSPHRASE"$"
#endif
#define WLAN_WEPENC_LEVEL_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WEPENC"$"
#define WLAN_BASIC_ENCMODES_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_BASIC_ENC_MODE"$"
#define WLAN_BASIC_AUTHMODE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_BASIC_AUTH_MODE"$"
#define WLAN_WPA_ENCMODES_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WPA_ENC_MODE"$"
#define WLAN_WPA_AUTHMODE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WPA_AUTH_MODE"$"
#define WLAN_IEEE11I_ENCMODES_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_IEEE11i_ENC_MODE"$"
#define WLAN_IEEE11I_AUTHMODE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_IEEE11i_AUTH_MODE"$"
#define WLAN_PSK_EXPRESSION_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_PSK_EXPRESSION"$"

#define WLAN_POSSIBLE_CHANNELS_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_POSSIBLE_CHANNELS"$"
#define WLAN_BASIC_DATA_TXRATES_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_BASIC_DATATXRATES"$"
#define WLAN_OPERATIONAL_DATA_TXRATES_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_OPERATIONAL_DATATXRATES"$"
#define WLAN_POSSIBLE_DATA_TXRATES_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_POSSIBLE_DATATXRATES"$"

#define WLAN_INSECURE_OOB_ACCESS_ENABLED_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_INSECURE_OOB_ACCESS_ENABLED"$"
#define WLAN_BEACON_ADV_ENABLED_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_BEACON_ADV_ENABLE"$"
#define WLAN_RADIO_ENABLED_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_RADIO_ENABLED"$"
#define WLAN_AUTORATE_FALLBACK_ENABLED_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_AUTORATE_FALLBACK_ENABLED"$"
#define WLAN_LOCATION_DESCRIPTION_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_LOCATION_DESCRIPTION"$"
#define WLAN_REGULATORY_DOMAIN_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_REGULATORY_DOMAIN"$"
#define WLAN_TOTAL_PSKFAILURES_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_PSKFAILURES"$"
#define WLAN_TOTAL_INTEGRITY_FAILURES_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_INTEGRITY_FAILURES"$"
#define WLAN_CHANNELS_INUSE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_CHANNEL_IN_USE"$"
#define WLAN_DEV_OP_MODE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_DEVICEOPERATIONMODE"$"
#define WLAN_WDS_ENABLED_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_WDSENABLED"$"
#define WLAN_DISTANCE_FROM_ROOT_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_DISTANCEFROMROOT"$"
#define WLAN_PEERBSSID_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_PEERBSSID"$"
#define WLAN_AUTH_SERV_MODE_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_AUTH_SERV_MODE"$"

#define WLAN_TOTAL_BYTES_TX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_BYTES_TX"$"
#define WLAN_TOTAL_BYTES_RX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_BYTES_RX"$"
#define WLAN_TOTAL_PACKETS_TX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_PACKETS_TX"$"
#define WLAN_TOTAL_PACKETS_RX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_PACKETS_RX"$"
#define WLAN_TOTAL_ASSOCIATIONS_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_ASSOCIATIONS"$"
#define WLAN_TOTAL_DROP_TX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_DROP_TX"$"
#define WLAN_TOTAL_DROP_TX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_DROP_TX"$"
#define WLAN_TOTAL_ERROR_TX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_ERROR_TX"$"
#define WLAN_TOTAL_ERROR_TX_REGEX WLAN_BASE_PATH_REGEX"\\."NODE_WLAN_TOTAL_ERROR_TX"$"

//#define WLAN_WPS_BASE_PATH_REGEX WLAN_BASE_PATH_REGEX"\\.X_TWSZ-COM_WPS\\.[0-9]+"
//#define WLAN_WPS_BASE_PATH_REGEX ".*"

//#define WLAN_WPS_CONFIGURED_REGEX WLAN_WPS_BASE_PATH_REGEX"\\."NODE_WLAN_WPS_CONFIGURED"$"

//#define WLAN_ASSODEV_BASE_PATH_REGEX WLAN_BASE_PATH_REGEX"\\.AssociatedDevice\\.[0-9]+"
#define WLAN_ASSODEV_BASE_PATH_REGEX ".*"

#define WLAN_ASSODEV_MAC_REGEX WLAN_ASSODEV_BASE_PATH_REGEX"\\."NODE_WLAN_ASSO_MAC"$"
#define WLAN_ASSODEV_RATE_REGEX WLAN_ASSODEV_BASE_PATH_REGEX"\\."NODE_WLAN_ASSO_RATE"$"
#define WLAN_ASSODEV_RSSI_REGEX WLAN_ASSODEV_BASE_PATH_REGEX"\\."NODE_WLAN_ASSO_RSSI"$"
#define WLAN_ASSODEV_STATE_REGEX WLAN_ASSODEV_BASE_PATH_REGEX"\\."NODE_WLAN_ASSO_STATE"$"
#define WLAN_ASSODEV_TYPE_REGEX WLAN_ASSODEV_BASE_PATH_REGEX"\\."NODE_WLAN_ASSO_TYPE"$"

//WEP 
#define WLAN_WEPKEY_REGEX WLAN_BASE_PATH_REGEX"\\.WEPKey\\.[0-9]+\\."NODE_WLAN_WEPKEY"$"

//WPA 
#define WLAN_PSK_BASE_PATH_REGEX WLAN_BASE_PATH_REGEX"\\.PreSharedKey\\.[0-9]+"
//#define WLAN_PSK_BASE_PATH_REGEX ".*PreSharedKey\\.[0-9]+"
#define WLAN_PSK_REGEX WLAN_PSK_BASE_PATH_REGEX"\\."NODE_WLAN_WPA_PSK"$"
#define WLAN_PSK_KEY_PASSPHRASE_REGEX WLAN_PSK_BASE_PATH_REGEX"\\."NODE_WLAN_WPA_KEY_PASSPHRASE"$"
#define WLAN_PSK_ASSODEV_MAC_REGEX WLAN_PSK_BASE_PATH_REGEX"\\."NODE_WLAN_ASSO_MAC"$"


//Radius 
#define WLAN_RADIUSERVER  "X_TWSZ-COM_RadiusServer"
#define WLAN_RADIUS_BASE_PATH_REGEX WLAN_BASE_PATH_REGEX"\\."WLAN_RADIUSERVER"\\.[0-9]+"
//#define WLAN_RADIUS_BASE_PATH_REGEX ".*"
#define WLAN_RADIUS_IPADDR_REGEX WLAN_RADIUS_BASE_PATH_REGEX"\\."NODE_RADIUS_IPADDR"$"
#define WLAN_RADIUS_PORT_REGEX WLAN_RADIUS_BASE_PATH_REGEX"\\."NODE_RADIUS_PORT"$"
#define WLAN_RADIUS_SHAREKEY_REGEX WLAN_RADIUS_BASE_PATH_REGEX"\\."NODE_RADIUS_KEY"$"
#define WLAN_RADIUS_ACCOUNT_PORT_REGEX WLAN_RADIUS_BASE_PATH_REGEX"\\."NODE_RADIUS_ACCOUNT_PORT"$"


#ifdef CONFIG_APPS_LOGIC_WPS
#define WLAN_ENC_AND_AUTH_REGEX WLAN_BEACON_TYPE_REGEX"|"WLAN_BASIC_ENCMODES_REGEX \
                                                               "|"WLAN_WEPENC_LEVEL_REGEX "|"WLAN_BASIC_AUTHMODE_REGEX \
                                                               "|"WLAN_WPA_ENCMODES_REGEX"|"WLAN_WPA_AUTHMODE_REGEX \
                                                               "|"WLAN_IEEE11I_ENCMODES_REGEX"|"WLAN_IEEE11I_AUTHMODE_REGEX\
                                                               "|"REGEX_WPS_VERSION
#else
#define WLAN_ENC_AND_AUTH_REGEX WLAN_BEACON_TYPE_REGEX"|"WLAN_BASIC_ENCMODES_REGEX \
                                                               "|"WLAN_WEPENC_LEVEL_REGEX "|"WLAN_BASIC_AUTHMODE_REGEX \
                                                               "|"WLAN_WPA_ENCMODES_REGEX"|"WLAN_WPA_AUTHMODE_REGEX \
                                                               "|"WLAN_IEEE11I_ENCMODES_REGEX"|"WLAN_IEEE11I_AUTHMODE_REGEX

#endif

#define WLAN_WPA_PASSPH_AND_PSK_REGEX WLAN_PSK_KEY_PASSPHRASE_REGEX"|"WLAN_PSK_REGEX

#define WLAN_SET_ALL_CONFLICT_REGEX ".*"
#define WLAN_WEP_KEY_CONFLICT_REGEX WLAN_WEP_KEYIDX_REGEX"|"WLAN_WEPENC_LEVEL_REGEX"|"WLAN_WEPKEY_REGEX
#define WLAN_PSK_EXPRESSION_CONFLICT_REGEX WLAN_WPA_PASSPH_AND_PSK_REGEX"|"WLAN_PSK_EXPRESSION_REGEX
#define WLAN_MAX_BITRATE_CONFLICT_REGEX NODE_WLAN_RATE"|"WLAN_STANDARD_REGEX

//#define CONFIG_APPS_LOGIC_WDS
#ifdef  CONFIG_APPS_LOGIC_WDS
#define WLAN_NODE_X_WDS_ENABLE		   	"Enable"
#define WLAN_NODE_WDS_MACADDR_1		"MACAddress1"
#define WLAN_NODE_WDS_MACADDR_2		 "MACAddress2"
#define WLAN_NODE_WDS_MACADDR_3	       "MACAddress3"
#define WLAN_NODE_WDS_MACADDR_4      "MACAddress4"
//#define WLAN_NODE_X_WDS_ENC				"EncryptionModes"
//#define WLAN_NODE_X_WDS_WEPKEY			"WEPKey"
//#define WLAN_NODE_X_WDS_PASSPHRASE		"Passphrase"
#define WLAN_WDS_OBJ_REPEATER			"Repeater"

//#define WLAN_BASE_PATH_REGEX ".*"
/* basic full path */

#define WLAN_WDS_PATH_BASE "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TWSZ-COM_WDSConfiguration"
#define WLAN_WDS_REGEX(node) 				WLAN_WDS_PATH_BASE"\\."node"$"
#define WLAN_WDS_REPEATER_REGEX(node) 		WLAN_WDS_PATH_BASE"\\."WLAN_WDS_OBJ_REPEATER"\\."node"$"


#endif

//#define CONFIG_APPS_LOGIC_WPS

#ifdef CONFIG_APPS_LOGIC_WPS

#define WPS_NODE_ENABLE  "Enable"
#define WPS_LOCAL_PIN  "DevicePassword"
#define WPS_PEER_PIN   "PeerPassword"
#define WPS_TRIGGER_SESSION  "StartWpsSession"
#define WPS_GENERATE_PIN     "GeneratePin"
#define WPS_SESSION_STATUS   "WpsSessionStatus"
#define WPS_GENERATEPIN_STATUS   "GeneratePinStatus"
#define WPS_ISMATCH_CONDITION    "IsMatchCondition"
#define WPS_DEVICE_NAME        "DeviceName"
#define WPS_NODE_UUID          "UUID"
#define WPS_NODE_VERSION       "Version"
#define WPS_CONFIGMETHODS_SUPPORT   "ConfigMethodsSupported"
#define WPS_CONFIGMETHODS_ENABLE    "ConfigMethodsEnabled"

#define WPS_ORIG_SSID              "ORIG_SSID"
#define WPS_ORIG_BEANCONTYPE       "ORIG_BeaconType"
#define WPS_ORIG_AUTHEN            "ORIG_Authen"
#define WPS_ORIG_ENCRY            "ORIG_Encry"
#define WPS_ORIG_NETWORKKEY        "ORIG_NetworkKey"
#define WPS_ORIG_KEY_INDEX        "ORIG_KeyIndex"
#define WPS_RESET_OOB             "X_TWSZ-COM_ResetOob"
#define WPS_STATUS                "ConfigurationState"
#define WPS_SSID_KEYWORD          "X_TWSZ-COM_WPSKeyWord"
#define WPS_ROLE                  "X_TWSZ-COM_Role"
#define WPS_CLIENT_LED            "X_TWSZ-COM_ClientLed"

#define WPS_STATUS_CONFIGURED    "Configured"
#define WPS_STATUS_UNCONFIGURED  "Not configured"

#define WPS_ROLE_REGISTRAR  "Registrar"
#define WPS_ROLE_ENROLLEE  "Enrollee"
#define WPS_ROLE_PROXY  "Proxy"
#define WPS_VERSION_1 "1"
#define WPS_VERSION_2 "2"


//#define WPS_COMMON_PATH "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.WLANConfiguration\\.[0-9]+\\.WPS\\."

#define REGEX_WPS_ENABLE    WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_NODE_ENABLE "$"
#define REGEX_LOCAL_PIN   WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_LOCAL_PIN "$"
#define REGEX_PEER_PIN   WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_PEER_PIN "$"
#define REGEX_TRIGGER_SESSION  WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_TRIGGER_SESSION "$"
#define REGEX_GENERATE_PIN   WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_GENERATE_PIN "$"
#define REGEX_CONFIGMETHODS_ENABLE WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_CONFIGMETHODS_ENABLE "$"
#define REGEX_WPS_STATUS WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_STATUS "$"
#define REGEX_WPS_VERSION WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_NODE_VERSION "$"


#define REGEX_RESET_OOB    WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_RESET_OOB "$"
#define REGEX_WPS_ROLE     WLAN_BASE_PATH_REGEX"\\.WPS\\."WPS_ROLE "$"

#define WLAN_INSTANCE_PATH "InternetGatewayDevice.LANDevice.%.WLANConfiguration.%."

#define WPS_PROESS_ID 0x100
#define WPS_PROESS_NAME  "wpsProess"

#define WPS_TIMER_NAME_CHANGE_PIN           "WPS_CHANGE_PIN"   /* PIN is changing */
#define WPS_TIMER_NAME_START_WPS            "WPS_START_WPS"    /* WPS session is going */
#define WPS_TIMER_NAME_CLIENT_LED            "WPS_CLIENT_LED"    /* WPS session is going */
#define WPS_CLIENT_LED_TIME_OUT             300   /* sec */
#define WPS_CHANGE_PIN_TIME_OUT             10   /* sec */
#define WPS_START_WPS_TIME_OUT              150   /* sec */
#define MAX_PROC_NAME_LEN 16

#define WPS_STATUS_LEN    30

#define WPS_STATUS_INPROCESS    "INPROCESS"
#define WPS_STATUS_ERROR       "ERROR"
#define WPS_STATUS_OVERLAP     "OVERLAP"
#define WPS_STATUS_SUCCESS     "SUCCESS"

#define WPS_CLIENT_LED_ON      "On"
#define WPS_CLIENT_LED_OFF     "Off"

#define VAR_PASSWD_PATH   "/var/wlanpasswd"
#define VAR_SSID_PATH     "/var/ssid"
#define MSG_BODY_LENGTH   64*1024

typedef enum tag_WPS_WpsSessionStatus
{
	WPS_SESSION_INPROESS = 1,
	WPS_SESSION_ERROR,
	WPS_SESSION_OVERLAP,
	WPS_SESSION_SUCCESS
	
}WpsSessionStatus;



#endif

/* 当前QR-Code只在页面显示Master SSID相关信息，所以使用固定路径 */
#define WLAN_BASIC_PATH "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1"


/*lan host path*/
#define LAN_HOST_PATH "InternetGatewayDevice.LANDevice.1.Hosts.Host."
/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/


/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/
#if 0
typedef struct tag_wlan_configuration {
char cWirelessEnabled;              /* 无线模块总开关标志 */
char cMacAddrCtlEnabled;          /* 白名单开关 */
char cRecfgFinishFlg;                 /* 重配置完成标志:0 表示未完，1 表示已经完成 */
char cDriverModStat;                  /* 用于标记WLAN驱动模块装载状态*/
char cConnCompletionEnable;    /* 是否开启链路完整性功能*/
char cWanDevIfState;                 /* wan 设备接口状态，1为连接，0为断开*/
char cDriverRestartFlg;              /* 用于标记WLAN驱动模块是否需要重启*/
TWlChannelCfgParas stChannelVal[MAX_NET_ID];
TWlAuthEncCfgParas  stAuthEncVal[MAX_NET_ID];
TWlMacAuthCfgParas stMacAuthCfgVal[MAX_NET_ID];
TWlRadiusCfgParas    stWlRadiusCfgVal[MAX_NET_ID];
}ST_WlanConfiguration;
#endif
/******************************************************************************
*                               GLOBAL VAR                                   *
******************************************************************************/


/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/

BOOL WlanIsEnableWLAN(void);

#endif


/******************************************************************************
*                                 END                                        *
******************************************************************************/



