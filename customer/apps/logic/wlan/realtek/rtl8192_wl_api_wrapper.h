/******************************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称:
 文件描述:
 修订记录:
         1 作者 : 陈赞辉
           日期 : 2009-05-19
           内容: 创建文件
******************************************************************************/
#ifndef _RTL8192_WIRELESS_API_WRAPPER_H_
#define _RTL8192_WIRELESS_API_WRAPPER_H_
#include "../wl_api.h"
#include <autoconf.h>
/*****************************************************************************
*                                 MACRO                                      *
******************************************************************************/

#ifdef TBS_APPS_DEBUG_WLAN
#define WRAPPER_DEBUG(fmt, args...) printf("%s:%4d "fmt, __FUNCTION__, __LINE__, ##args)
#define WRAPPER_DEBUG_POWER(fmt, args...)  do{;}while(0);
#else
#define WRAPPER_DEBUG(fmt, args...)
#define WRAPPER_DEBUG_POWER(fmt, args...)
#endif


#define MAX_WDS_NUM				8
#define MAX_WLAN_VAP 			5
#define USE_20M 				0
#define USE_40M 				1
#define BEACON_TYPE_BASIC		"Basic"
#define BEACON_TYPE_NONE		"None"
#define BEACON_TYPE_WPA	 		"WPA"
#define BEACON_TYPE_11i	 		"11i"
#define BEACON_TYPE_WPA2_MIXED 		"WPAand11i"
#define AUTH_CFGFILE_PATH   	"/var/config"
#define MAX_CMD_LENGTH 			40
#define MAX_STATION_INFO_LENGTH 8192
#define MAX_WEP_LEN 			256
#define MAX_PSK_LEN 			256
#define MAX_CCK_LEN				14
#define MAX_OFDM_LEN			216

enum CountryCode {
    FCC=1,
	IC,
	ETSI,
	SPAIN,
	FRANCE,
	MKK,
	ISRAEL,
	MKK1,
	MKK2,
	MKK3,
	PERU
};
    
typedef struct
{
    unsigned int CountryCode;
    const char * CountryString;
}ST_Conutries;

/*1-11信道的默认选FCC ，1-13信道的默认选ETSI*/
ST_Conutries stCountries[] = {
	{FCC,	 "Anguilla"},
	{FCC,	 "Argentina"},
	{FCC,	 "Bermuda"}, 
	{FCC,	 "Brazil"},
	{FCC,	 "Canada"},
	{FCC,	 "Cayman Islands"}, 
	{FCC,	 "Chile"},
	{FCC,	 "China Taiwan"},
	{FCC,	 "Colombia"}, 
	{FCC,	 "Guam"},
	{FCC,	 "Mexico"},
	{FCC,	 "Micronesia"}, 
	{FCC,	 "Peru"}, 
	{FCC,	 "Puerto Rico"}, 
	{FCC,	 "USA"},
	{FCC,	 "United States"}, 
	{FCC,	 "United States Minor Outlying Islands"}, 
	{FCC,	 "Virgin Islands, U.S"}, 
	{FCC,	 "Yugoslavia"},
	{ETSI,	 "Afghanistan"}, 
	{ETSI,	 "Albania"},
	{ETSI,	 "Angola"}, 
	{ETSI,	 "Antigua And Barbuda"}, 
	{ETSI,	 "Armenia"}, 
	{ETSI,	 "Aruba"}, 
	{ETSI,	 "Australia"},
	{ETSI,	 "Austria"}, 
	{ETSI,	 "Azerbaijan"},
	{ETSI,	 "Bahamas"}, 
	{ETSI,	 "Bahrain"}, 
	{ETSI,	 "Bangladesh"}, 
	{ETSI,	 "Barbados"},
	{ETSI,	 "Belarus"}, 
	{ETSI,	 "Belgium"}, 
	{ETSI,	 "Belize"},
	{ETSI,	 "Benin"},
	{ETSI,	 "Bhutan"}, 
	{ETSI,	 "Bolivia"}, 
	{ETSI,	 "Bosnia And Herzegovina"}, 
	{ETSI,	 "Botswana"}, 
	{ETSI,	 "British Indian Ocean Territory"}, 
	{ETSI,	 "Brunei Darussalam"}, 
	{ETSI,	 "Bulgaria"}, 
	{ETSI,	 "Burkina Faso"},
	{ETSI,	 "Burundi"}, 
	{ETSI,	 "Cambodia"}, 
	{ETSI,	 "Cameroon"}, 
	{ETSI,	 "Cape Verde"}, 
	{ETSI,	 "Central African Republic"}, 
	{ETSI,	 "Chad"}, 
	{ETSI,	 "China"}, 
	{ETSI,	 "Christmas Island"}, 
	{ETSI,	 "Comoros"},
	{ETSI,	 "Congo"}, 
	{ETSI,	 "Costa Rica"}, 
	{ETSI,	 "Cote Divoire"},
	{ETSI,	 "Croatia"},
	{ETSI,	 "Cuba"}, 
	{ETSI,	 "Cyprus"}, 
	{ETSI,	 "Czech Republic"}, 
	{ETSI,	 "Denmark"}, 
	{ETSI,	 "Djibouti"},
	{ETSI,	 "Dominica"}, 
	{ETSI,	 "Dominican Republic"},
	{ETSI,	 "Ecuador"},
	{ETSI,	 "Egypt"},
	{ETSI,	 "El Salvador"}, 
	{ETSI,	 "Equatorial Guinea"}, 
	{ETSI,	 "Eritrea"}, 
	{ETSI,	 "Estonia"}, 
	{ETSI,	 "Ethiopia"},
	{ETSI,	 "Falkland Islands (malvinas)"}, 
	{ETSI,	 "Faroe Islands"}, 
	{ETSI,	 "Fiji"},
	{ETSI,	 "Finland"}, 
	{ETSI,	 "French Guiana"},
	{ETSI,	 "French Polynesia"},	
	{ETSI,	 "French Southern Territories"}, 
	{ETSI,	 "Gabon"},
	{ETSI,	 "Gambia"},
	{ETSI,	 "Georgia"}, 
	{ETSI,	 "Germany"},
	{ETSI,	 "Ghana"},
	{ETSI,	 "Gibraltar"}, 
	{ETSI,	 "Greece"},
	{ETSI,	 "Grenada"},
	{ETSI,	 "Guadeloupe"}, 
	{ETSI,	 "Guatemala"},
	{ETSI,	 "Guernsey"},
	{ETSI,	 "Guinea"}, 
	{ETSI,	 "Guinea-bissau"},
	{ETSI,	 "Guyana"},
	{ETSI,	 "Haiti"}, 
	{ETSI,	 "Holy See (vatican city state)"},
	{ETSI,	 "Honduras"},
	{ETSI,	 "China Hong Kong"},
	{ETSI,	 "Hungary"}, 
	{ETSI,	 "Iceland"},
	{ETSI,	 "India"},
	{ETSI,	 "Indonesia"}, 
	{ETSI,	 "Iran"},
	{ETSI,	 "Iraq"},
	{ETSI,	 "Ireland"}, 
	{ETSI,	 "Italy"},
	{ETSI,	 "Jamaica"},
	{ETSI,	 "Jersey"}, 
	{ETSI,	 "Kazakhstan"},
	{ETSI,	 "Kenya"},
	{ETSI,	 "Kiribati"}, 
	{ETSI,	 "Korea"},
	{ETSI,	 "Kuwait"},
	{ETSI,	 "Kyrgyzstan"}, 
	{ETSI,	 "Lao People's Democratic Republic"},
	{ETSI,	 "Latvia"},
	{ETSI,	 "Lebanon"}, 
	{ETSI,	 "Lesotho"},
	{ETSI,	 "Liberia"},
	{ETSI,	 "Libyan Arab Jamahiriya"}, 
	{ETSI,	 "Liechtenstein"},
	{ETSI,	 "Lithuania"},
	{ETSI,	 "Luxembourg"}, 
	{ETSI,	 "Macao"},
	{ETSI,	 "Macedonia"},
	{ETSI,	 "Madagascar"}, 
	{ETSI,	 "Malawi"},
	{ETSI,	 "Malaysia"},
	{ETSI,	 "Maldives"}, 
	{ETSI,	 "Mali"},
	{ETSI,	 "Malta"},
	{ETSI,	 "Man, Isle Of"}, 
	{ETSI,	 "Martinique"},
	{ETSI,	 "Mauritania"},
	{ETSI,	 "Mauritius"}, 
	{ETSI,	 "Mayotte"},
	{ETSI,	 "Moldova"},
	{ETSI,	 "Monaco"}, 
	{ETSI,	 "Mongolia"},
	{ETSI,	 "Montenegro"},
	{ETSI,	 "Montserrat"}, 
	{ETSI,	 "Morocco"},
	{ETSI,	 "Mozambique"},
	{ETSI,	 "Myanmar"}, 
	{ETSI,	 "Namibia"},
	{ETSI,	 "Nauru"},
	{ETSI,	 "Nepal"}, 
	{ETSI,	 "Netherlands"},
	{ETSI,	 "Netherlands Antilles"},
	{ETSI,	 "New Caledonia"}, 
	{ETSI,	 "New Zealand"},
	{ETSI,	 "Nicaragua"},
	{ETSI,	 "Niger"}, 
	{ETSI,	 "Nigeria"},
	{ETSI,	 "Niue"},
	{ETSI,	 "Norfolk Island"}, 
	{ETSI,	 "Northern Mariana Islands"},
	{ETSI,	 "Norway"},
	{ETSI,	 "Oman"}, 
	{ETSI,	 "Pakistan"},
	{ETSI,	 "Palau"},
	{ETSI,	 "Panama"}, 
	{ETSI,	 "Papua New Guinea"},
	{ETSI,	 "Paraguay"},
	{ETSI,	 "Philippines"}, 
	{ETSI,	 "Poland"},
	{ETSI,	 "Portugal"},
	{ETSI,	 "Qatar"}, 
	{ETSI,	 "Reunion"},
	{ETSI,	 "Romania"},
	{ETSI,	 "Russian Federation"}, 
	{ETSI,	 "Rwanda"},
	{ETSI,	 "Saint Kitts And Nevis"},
	{ETSI,	 "Saint Lucia"}, 
	{ETSI,	 "Saint Pierre And Miquelon"},
	{ETSI,	 "Saint Vincent And The Grenadines"},
	{ETSI,	 "Samoa"}, 
	{ETSI,	 "San Marino"},
	{ETSI,	 "Sao Tome And Principe"},
	{ETSI,	 "Saudi Arabia"}, 
	{ETSI,	 "Senegal"},
	{ETSI,	 "Serbia"},
	{ETSI,	 "Seychelles"}, 
	{ETSI,	 "Sierra Leone"},
	{ETSI,	 "Singapore"},
	{ETSI,	 "Slovakia"}, 
	{ETSI,	 "Slovenia"},
	{ETSI,	 "Solomon Islands"},
	{ETSI,	 "Somalia"}, 
	{ETSI,	 "South Africa"},
	{ETSI,	 "Sri Lanka"},
	{ETSI,	 "Suriname"}, 
	{ETSI,	 "Swaziland"},
	{ETSI,	 "Sweden"},
	{ETSI,	 "Switzerland"}, 
	{ETSI,	 "Tajikistan"},
	{ETSI,	 "Tanzania"},
	{ETSI,	 "Thailand"}, 
	{ETSI,	 "Togo"},
	{ETSI,	 "Tonga"},
	{ETSI,	 "Trinidad And Tobago"}, 
	{ETSI,	 "Tunisia"},
	{ETSI,	 "Turkey"},
	{ETSI,	 "Turkmenistan"}, 
	{ETSI,	 "Turks And Caicos Islands"},
	{ETSI,	 "Tuvalu"},
	{ETSI,	 "Uganda"}, 
	{ETSI,	 "Ukraine"},
	{ETSI,	 "United Arab Emirates"},
	{ETSI,	 "United Kingdom"}, 
	{ETSI,	 "Uruguay"},
	{ETSI,	 "Uzbekistan"},
	{ETSI,	 "Vanuatu"}, 
	{ETSI,	 "Venezuela"},
	{ETSI,	 "Viet Nam"},
	{ETSI,	 "Virgin Islands, British"}, 
	{ETSI,	 "Wallis And Futuna"},
	{ETSI,	 "Western Sahara"},
	{ETSI,	 "Yemen"}, 
	{ETSI,	 "Zambia"},
	{ETSI,	 "Zimbabwe"},
	{ETSI,	 "other"}, 
	{FRANCE, "France"}, 
	{FRANCE, "Jordan"},
	{ISRAEL, "Israel"},
	{MKK1,	 "Japan1"},
	{MKK,	 "Japan2"},
	{SPAIN,	 "Spain"}
};


//WDS CMD
#define RTL8192_WDS_ENABLE 			"wds_enable"
#define RTL8192_WDS_PURE			"wds_pure"
#define RTL8192_WDS_PRIORITY 		"wds_priority"
#define RTL8192_WDS_NUM				"wds_num"
#define RTL8192_WDS_ADD				"wds_add"
#define RTL8192_WDS_ENCRYPT 		"wds_encrypt"
#define RTL8192_WDS_WEPKEY      	"wds_wepkey"
#define RTL8192_WDS_PASSPHRASE		"wds_passphrase"

#if 0
#define RTL8192_REGDOMAIN		"regdomain"
#define RTL8192_LED_TYPE		"led_type"
#define RTL8192_SSID			"ssid"
#define RTL8192_RFCHIPID		"RFChipID"
#define RTL8192_BCNINT			"bcnint"
#define RTL8192_CHANNEL			"channel"
#define RTL8192_BASICRATES		"basicrates"
#define RTL8192_OPRATES			"oprates"
#define RTL8192_AUTORATE		"autorate"
#define RTL8192_RTSTHRES		"rtsthres"
#define RTL8192_FRAGTHRES		"fragthres"
#define RTL8192_PREAMBLE		"preamble"
#define RTL8192_HIDDENAP		"hiddenAP"
#define RTL8192_DTIMPERIOD		"dtimperiod"
#define RTL8192_ACLNUM			"aclmum"
#define RTL8192_ACLMODE			"aclmode"
#define RTL8192_WSC_ENABLE		"wsc_enable"
#define RTL8192_AUTHTYPE		"authtype"
#define RTL8192_ENCMODE			"encmode"
#define RTL8192_802_1X			"802_1x"
#define RTL8192_BAND			"band"
#define RTL8192_DENY_LEGACY		"deny_legacy"
#define RTL8192_USE40M			"use40M"
#define RTL8192_2NDCHOFFSET		"2ndchoffset"
#define RTL8192_SHORTGI20M		"shortGI20M"
#define RTL8192_SHORTGI40M		"shortGI40M"
#define RTL8192_QOS_ENABLE		"qos_enable"
#define RTL8192_WEPKEY1			"wepkey1"
#define RTL8192_WEPKEY2			"wepkey2"
#define RTL8192_WEPKEY3			"wepkey3"
#define RTL8192_WEPKEY4			"wepkey4"
#define RTL8192_WEPDKEYID		"wepdkeyid"
#define RTL8192_PSK_ENABLE		"psk_enable"
#define RTL8192_WPA_CIPHER		"wpa_cipher"
#define RTL8192_WPA2_CIPHER		"wpa2_cipher"
#define RTL8192_PASSPHRASE		"passphrase"
#define RTL8192_EXPIRED_TIME	"expired_time"
#define RTL8192_OPMODE			"opmode"
#define RTL8192_USE_EFUSE		"use_efuse"
#define RTL8192_VAP_ENABLE		"vap_enable"
#define RTL8192_INTF_MAP		"intf_map"
#define RTL8192_AMPDU			"ampdu"
#define RTL8192_AMSDU			"amsdu"
#define RTL8192_CTS2SELF		"cts2self"
#define RTL8192_DESABLE_PROTECTION		"desable_protection"
#define RTL8192_BLOCK_RELAY		"block_relay"
#endif
/*****************************************************************************
*                                 ENUM                                       *
******************************************************************************/


/*****************************************************************************
*                                STRUCT                                      *
******************************************************************************/
typedef struct wpaConf_entry {
	unsigned int  needsatart;
	unsigned int  encryption; 			// encrypt type, defined as ENCRYPT_t
	unsigned char ssid[MAX_SSID_LEN] ; 			// SSID
	unsigned int  enable1x ; 		// enable/disable 802.1x
	unsigned int  enableMacAuth;
	unsigned int  supportNonWpaClient;
	unsigned int  wepKey;			//1:WEP64 2:WEP128
	unsigned char wepGroupKey[MAX_WEP_LEN+1]; //"" NO use
	unsigned int  authentication;
	unsigned int  unicastCipher;
	unsigned int  wpa2UnicastCipher;
	unsigned int  usePassphrase;
	unsigned char psk[MAX_PSK_LEN+1];
	unsigned int  groupRekeyTime;
	unsigned int  rsPort;
	unsigned char rsIP[MAX_IPADDR_LEN];
	unsigned char rsPassword[MAX_RADKEY_LEN];
	unsigned int  rs2Port;
	unsigned char rs2IP[MAX_IPADDR_LEN];
	unsigned char rs2Password[MAX_RADKEY_LEN];
	unsigned int  rsMaxReq;
	unsigned int  rsAWhile;
	unsigned int  accountRsEnabled;
	unsigned int  accountRsPort;
	unsigned char accountRsIP[MAX_IPADDR_LEN];
	unsigned char accountRsPassword[MAX_RADKEY_LEN];
	unsigned int  accountRsUpdateEnabled;
	unsigned int  accountRsUpdateTime;
	unsigned int  accountRsMaxReq;
	unsigned int  accountRsAWhile;
} WPACONF_ENTRY, *pstWPACONF_ENTRY;


typedef struct
{
int wds_enable;
int wds_pure;
int wds_priority;
int wds_num;
char wds_add[12];
int wds_encrypt;
char wds_wepkey[128];
char wds_pasphrase[128];
}WDS_CONFIG;
WDS_CONFIG wdsconfig;


typedef struct
{
    int  netif_enable;          // indicate whether the net interface is enable
    int  radius_need;    		// RTL8192apd only needed in radius auth mode
    int  enable1x;
	char DevName[10];
	enum WL_ProfileEnum mode_profile;
}TVapPrivateCfg;

typedef struct
{
    char ap_enable;
    TVapPrivateCfg stPrivCfg[MAX_NET_ID];
}RTL8192apdConfig;

typedef struct
{
	unsigned char state;
	unsigned char channel;
	unsigned char txRate;
	unsigned char bssid[6];
	unsigned char rssi;
	unsigned char sq;
	unsigned char ssid[33];
}RTL8192_bss_info;
/*****************************************************************************
*                               GLOBAL VAR                                   *
******************************************************************************/
const char IWCONTROLPID[]= "/var/run/iwcontrol.pid";
const char WSCDPID[] = "/var/run/wscd-wlan0.pid";
const char AUTH_PID[] = "/var/run/auth-wlan0.pid";
const char *NETIF_RF[]=
{
	"/proc/wlan0/mib_rf","/proc/wlan0-vap0/mib_rf",
	"/proc/wlan0-vap1/mib_rf","/proc/wlan0-vap2/mib_rf",
	"/proc/wlan0-vap3/mib_rf","/proc/wlan0-vap4/mib_rf",0
};
// interface name of VAPs
const char *RTL8192_netif_name[] =
{
    "wlan0","wlan0-vap0","wlan0-vap1","wlan0-vap2","wlan0-vap3", ""
};
const char *RTL8192_wdsif_name[]=
{
	"wlan0-wds0","wlan0-wds1","wlan0-wds2","wlan0-wds3","wlan0-wds4","wlan0-wds5","wlan0-wds6",	"wlan0-wds7",""
};
const char *RTL8192_txRate[]=
{
	"0","1","2","4","8","16","32","64","128","256","512","1024","2048",
	"4096","8192","16384","32768","65536","131072","262144","524288","16777216","33554432","67108864","134217728","1048576","2097152","4194304","8388608",
};
const char *WLANAUTHPID[] = {
	"/var/run/auth-wlan0.pid", "/var/run/auth-wlan0-vap0.pid", "/var/run/auth-wlan0-vap1.pid", "/var/run/auth-wlan0-vap2.pid", "/var/run/auth-wlan0-vap3.pid", 0
};

const char *WLANAUTHCONF[] = {
	"/var/config/wlan0.conf", "/var/config/wlan0-vap0.conf", "/var/config/wlan0-vap1.conf", "/var/config/wlan0-vap2.conf", "/var/config/wlan0-vap3.conf", 0
};

const char *WLANAUTHFIFO[] = {
	"/var/auth-wlan0.fifo", "/var/auth-wlan0-vap0.fifo", "/var/auth-wlan0-vap1.fifo", "/var/auth-wlan0-vap2.fifo", "/var/auth-wlan0-vap3.fifo", 0
};


#endif
/*****************************************************************************
*                                 END                                        *
******************************************************************************/




