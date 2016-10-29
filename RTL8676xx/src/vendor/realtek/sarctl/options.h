/*
 *	option.h
 */


#ifndef INCLUDE_OPTIONS_H
#define INCLUDE_OPTIONS_H

#if defined(EMBED) || defined(__KERNEL__)
#include <linux/config.h>
#else
//#include <linux/autoconf.h>
#endif

#if 0
#ifdef EMBED
#include <config/autoconf.h>
#else
#include "autoconf.h"
#endif
#endif

//jiunming, redirect the web page to specific page only once per pc for wired router
#undef WEB_REDIRECT_BY_MAC

//alex
#undef CONFIG_USBCLIENT
//jim for power_led behavior according to TR068..
#undef WLAN_BUTTON_LED
//ql--if image header error, dont reboot the system.
#define ENABLE_SIGNATURE_ADV
// try to restore the user setting when config checking error
#define KEEP_CRITICAL_HW_SETTING
#undef KEEP_CRITICAL_CURRENT_SETTING


#if defined(ZTE_GENERAL_ROUTER_SC) || defined(ZTE_531B_BRIDGE_SC)
#undef NAT_CONN_LIMIT
#undef MAC_ACL
#define ENABLE_SIGNATURE
#ifdef ENABLE_SIGNATURE
#define SIGNATURE	"ZXDSL531BIIZ22"
#endif
//jim support dsl disconnect when local-firmware upgrade
//now. the bug is fixed, no need to down dsl link....
//#define DSL_DOWN_DURING_LOCAL_UPLD
#endif
//ql-- limit upstream traffic
#undef UPSTREAM_TRAFFIC_CTL

#define INCLUDE_DEFAULT_VALUE		1
#ifndef CONFIG_GUI_WEB
#define WEB_MENU_USE_NEW		1
#undef	WEB_MENU_USE_MTM
#endif

// Mason Yu
#undef PORT_FORWARD_ADVANCE
#define APPLY_CHANGE
#undef CLOSE_ITF_BEFORE_WRITE

//#define ZTE_GENERAL_ROUTER_SC 		1
#if defined(ZTE_531B_BRIDGE_SC) || defined(ZTE_GENERAL_ROUTER_SC)
#define SECONDARY_IP
//star: for zte led request

//jim for power_led behavior according to TR068..
#define WLAN_BUTTON_LED
//replaced by upper macros
///jim
//#define ZTE_LED_REQUEST

//star: for set acl ip range
#undef ACL_IP_RANGE

//star: for set ip pool for different client type
#undef IP_BASED_CLIENT_TYPE

//star: for layer7 filter
#undef LAYER7_FILTER_SUPPORT

#define WLAN_TX_POWER_DISPLAY

#ifdef ZTE_GENERAL_ROUTER_SC
//jim: this macro is defined by ql_xu to support config-space
//     compress.... maybe should be moved to options.h
#define COMPRESS_CURRENT_SETTING
#endif
#else
#define SECONDARY_IP
#ifdef CONFIG_USER_WIRELESS_TOOLS
#undef WLAN_TX_POWER_DISPLAY
#endif
#endif

#ifdef CONFIG_USER_WIRELESS_TOOLS

#define WLAN_SUPPORT			1
#define WLAN_WPA			1
#define WLAN_8185AG			1
#ifdef CONFIG_USER_WIRELESS_WDS
#define WLAN_WDS			1
#endif
#undef WLAN_CLIENT
#define WLAN_1x				1
#define WLAN_ACL			1
#undef WIFI_TEST
#if defined(ZTE_531B_BRIDGE_SC) || defined(ZTE_GENERAL_ROUTER_SC)
#undef WLAN_QoS
#else
#undef WLAN_QoS
#endif
#ifdef CONFIG_USER_WIRELESS_MBSSID
#define WLAN_MBSSID			1
#endif
#define MAX_WLAN_VAP			4
#undef E8A_CHINA

#ifdef CONFIG_USER_WIRELESS_TOOLS_RTL8185_IAPP
#define WLAN_IAPP			1
#endif

#undef WLAN_ONOFF_BUTTON_SUPPORT

#if defined(ZTE_GENERAL_ROUTER_SC) || defined(ZTE_531B_BRIDGE_SC)
//xl_yue: support zte531b--light:wps function is ok; die:wps function failed; blink: wps connecting
#undef	REVERSE_WPS_LED
//added by xl_yue: board will not reboot after wps connection if defined this macro
#undef	MIB_WLAN_RESTART_SUPPORT
#undef	MSG_WLAN_RESTART_SUPPORT
#else
#undef	REVERSE_WPS_LED
#undef	MIB_WLAN_RESTART_SUPPORT
#undef	MSG_WLAN_RESTART_SUPPORT
#endif


#if defined(ZTE_531B_BRIDGE_SC) || defined(ZTE_GENERAL_ROUTER_SC)
#undef ENABLE_WPAAES_WPA2TKIP
#else
#undef ENABLE_WPAAES_WPA2TKIP
#endif

#endif

#undef NATIP_FORWARDING
#define MAC_FILTER			1
#ifdef CONFIG_USER_AUTO_PROVISIONING
#define AUTO_PROVISIONING		1
#endif
#define PPPOE_PASSTHROUGH
#define IP_PASSTHROUGH			1
#undef PORT_TRIGGERING
#define URL_BLOCKING_SUPPORT		1
#undef  URL_ALLOWING_SUPPORT
#define DOMAIN_BLOCKING_SUPPORT		1
#define IP_ACL				1
//uncomment for TCP/UDP connection limit
//#define TCP_UDP_CONN_LIMIT	1
#undef ADDRESS_MAPPING
#undef  MULTI_ADDRESS_MAPPING
#undef CONFIG_IGMP_FORBID
#define FORCE_IGMP_V2			1


#ifdef CONFIG_8M_SDRAM
#define MINIMIZE_RAM_USAGE 1
#undef SUPPORT_AUTH_DIGEST
#endif

#undef SUPPORT_AUTH_DIGEST

#define WEB_UPGRADE			1
//#undef WEB_UPGRADE			// jimluo e8-A spec, unsupport web upgrade.

// Mason Yu
#if defined(ZTE_531B_BRIDGE_SC) || defined(ZTE_GENERAL_ROUTER_SC) || defined(CONFIG_GUI_WEB)
#undef AUTO_PVC_SEARCH_TR068_OAMPING
#undef AUTO_PVC_SEARCH_PURE_OAMPING
#undef AUTO_PVC_SEARCH_AUTOHUNT
#else
#undef AUTO_PVC_SEARCH_TR068_OAMPING
#define AUTO_PVC_SEARCH_PURE_OAMPING
#define AUTO_PVC_SEARCH_AUTOHUNT
#endif

#ifndef AUTO_PVC_SEARCH_TR068_OAMPING
#define AUTO_PVC_SEARCH_TABLE
#endif
#undef BOA_UNAUTH_FILE
#ifdef CONFIG_GUI_WEB
#define BOA_UNAUTH_FILE
#endif

#ifdef CONFIG_USER_VSNTP
#define TIME_ZONE			1
#endif

#ifdef CONFIG_USER_IPROUTE2_TC_TC
//ql 20081117 START replace IP_QOS with NEW_IP_QOS_SUPPORT
#define IP_QOS				1
/*ql 20081114 START new IP QoS*/
#undef NEW_IP_QOS_SUPPORT
/*ql 20081114 END*/

#ifdef CONFIG_NET_SCH_DSMARK
#define QOS_DIFFSERV
#endif
#define	IP_QOS_VPORT		1
#undef   CONFIG_8021P_PRIO
#ifdef CONFIG_IP_NF_TARGET_DSCP
#define QOS_DSCP		1
#endif
//#ifdef NEW_IP_QOS_SUPPORT
#if defined(NEW_IP_QOS_SUPPORT) || defined(QOS_DIFFSERV)
#ifdef CONFIG_IP_NF_MATCH_DSCP
#define QOS_DSCP_MATCH		1
#endif
#endif
#endif
#ifdef CONFIG_USER_IPROUTE2_IP_IP
#ifdef IP_QOS
#define IP_POLICY_ROUTING		1
#endif
#endif
#define ITF_GROUP			1
#undef VLAN_GROUP
#undef ELAN_LINK_MODE
#undef ELAN_LINK_MODE_INTRENAL_PHY
#define DIAGNOSTIC_TEST			1

//xl_yue
#ifdef ZTE_GENERAL_ROUTER_SC
#define	DOS_SUPPORT		1
#else
#undef	DOS_SUPPORT
#endif

#ifdef CONFIG_GUI_WEB
#define DEFAULT_GATEWAY_V2
#define QSETUP_WEB_REDIRECT	1
#define SPPPD_NO_IDLE	1
#define PPP_QUICK_CONNECT	1
#else
#define DEFAULT_GATEWAY_V1	//set dgw per pvc
#undef DEFAULT_GATEWAY_V2	// set dgw interface in routing page
#undef QSETUP_WEB_REDIRECT
#undef SPPPD_NO_IDLE
#undef PPP_QUICK_CONNECT
#endif	//CONFIG_GUI_WEB
#ifndef DEFAULT_GATEWAY_V2
#ifndef DEFAULT_GATEWAY_V1
#define DEFAULT_GATEWAY_V1	1
#endif
#endif
#ifdef DEFAULT_GATEWAY_V2
#define AUTO_PPPOE_ROUTE	1
//#undef AUTO_PPPOE_ROUTE
#endif

//alex_huang
#if defined(ZTE_531b_BRIDGE_SC) || defined(ZTE_GENERAL_ROUTER_SC)
	#define CONFIG_SPPPD_STATICIP
#else
	#undef  CONFIG_SPPPD_STATICIP
#endif
#undef XOR_ENCRYPT
#undef XML_TR069
#define TELNET_IDLE_TIME	600 //10*60 sec. Please compiler boa and telnetd

#ifdef CONFIG_USER_CWMP_TR069
// Mason Yu
#ifndef XML_TR069
#define XML_TR069
#endif  //XML_TR069
#define _CWMP_MIB_				1
#ifdef CONFIG_USER_CWMP_WITH_SSL
#define _CWMP_WITH_SSL_				1
#endif //CONFIG_USER_CWMP_WITH_SSL
#define	_CWMP_APPLY_				1
#define _PRMT_SERVICES_				1
#define _PRMT_CAPABILITIES_			1
#define _PRMT_DEVICECONFIG_			1
//#define _PRMT_USERINTERFACE_			1
/*disable connection request authentication*/
//#define _TR069_CONREQ_AUTH_SELECT_		1
#ifdef CONFIG_USER_TR143
#define _PRMT_TR143_				1
#endif //CONFIG_USER_TR143
#ifdef CONFIG_USB_ETH
#define _PRMT_USB_ETH_				1
#endif //CONFIG_USB_ETH


/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#define _PRMT_WT107_					1
#ifdef _PRMT_WT107_
#ifdef CONFIG_USER_BUSYBOX_TRACEROUTE
#define _SUPPORT_TRACEROUTE_PROFILE_		1
#endif //CONFIG_USER_BUSYBOX_TRACEROUTE
#define _SUPPORT_CAPTIVEPORTAL_PROFILE_		1
#define _SUPPORT_ADSL2DSLDIAG_PROFILE_		1
#define _SUPPORT_ADSL2WAN_PROFILE_		1
#endif //_PRMT_WT107_



#define _PRMT_X_TELEFONICA_ES_DHCPOPTION_	        1
/*ping_zhang:20081217 END*/

/*ping_zhang:20081223 START:define for support multi server by default*/
#define SNTP_MULTI_SERVER			1
/*ping_zhang:20081223 END*/


//#define _PRMT_X_CT_EXT_ENABLE_			1
#ifdef _PRMT_X_CT_EXT_ENABLE_
	/*TW's ACS has some problem with this extension field*/
	#define _INFORM_EXT_FOR_X_CT_		1
    #ifdef _PRMT_SERVICES_
	#define _PRMT_X_CT_COM_IPTV_		1
	#define _PRMT_X_CT_COM_MWBAND_		1
	//#define _PRMT_X_CT_COM_MONITOR_		1
	//#define _PRMT_X_CT_COM_VPDN_		1
    #endif //_PRMT_SERVICES_
	#define	_PRMT_X_CT_COM_DDNS_		1
	#define _PRMT_X_CT_COM_ALG_		1
	#define _PRMT_X_CT_COM_ACCOUNT_		1
	#define _PRMT_X_CT_COM_RECON_		1
	#define _PRMT_X_CT_COM_PORTALMNT_	1
	#define _PRMT_X_CT_COM_SRVMNG_		1	/*ServiceManage*/
	//#define _PRMT_X_CT_COM_PPPOE_		1
	#define _PRMT_X_CT_COM_PPPOEv2_		1
    #ifdef WLAN_SUPPORT
	#define _PRMT_X_CT_COM_WLAN_		1
    #endif //WLAN_SUPPORT
	#define _PRMT_X_CT_COM_DHCP_		1
	#define _PRMT_X_CT_COM_WANEXT_		1
#endif //_PRMT_X_CT_EXT_ENABLE_
#endif //CONFIG_USER_CWMP_TR069




#ifdef WEB_UPGRADE
#define	UPGRADE_V1			1
#undef	UPGRADE_V2
#undef	UPGRADE_V3
// Jenny, porting from star
/*star:20090413 START when reboot after upload new img, check if current setting entities is added or deled*/
#undef CHECK_SETTING
#endif // if WEB_UPGRADE

//Jimluo
#if	defined(ZTE_531B_BRIDGE_SC) || defined(ZTE_GENERAL_ROUTER_SC)
#define DEVICE_TYPE_5
#endif


//Jimluo
#if	defined(ZTE_531B_BRIDGE_SC) || defined(ZTE_GENERAL_ROUTER_SC)
#define CTC_WAN_NAME
#else
#undef CTC_WAN_NAME
#endif

//ql add
#undef	RESERVE_KEY_SETTING

//xl_yue
#if defined(ZTE_531B_BRIDGE_SC) || defined(ZTE_GENERAL_ROUTER_SC) || defined(CONFIG_GUI_WEB)
#define SYSLOG_SUPPORT			1
#else
#undef	SYSLOG_SUPPORT
#endif
#undef WEB_ENABLE_PPP_DEBUG
#ifdef SYSLOG_SUPPORT
#undef SYSLOG_REMOTE_LOG
#endif

// Mason Yu
#undef SEND_LOG

//xl_yue add,when finishing maintenance,inform ITMS to change password
#undef	FINISH_MAINTENANCE_SUPPORT

//xl_yue add,web logining is maintenanced by web server
#undef	USE_LOGINWEB_OF_SERVER

#ifdef USE_LOGINWEB_OF_SERVER
//xl_yue add,if have logined error for three times continuely,please relogin after 1 minute
#define LOGIN_ERR_TIMES_LIMITED 1
//xl_yue add,only one user can login with the same account at the same time
#define ONE_USER_LIMITED	1
//xl_yue add,if no action lasting for 5 minutes,auto logout
#define AUTO_LOGOUT_SUPPORT	1
#undef USE_BASE64_MD5_PASSWD
#endif

#ifndef USE_LOGINWEB_OF_SERVER
//xl_yue add, not used
//#define ACCOUNT_LOGIN_CONTROL		1
#endif


/*######################*/
//jim 2007-05-22
//4 jim_luo Bridge Mode only access on web
//#define BRIDGE_ONLY_ON_WEB
#undef  BRIDGE_ONLY_ON_WEB

//4 E8-A unsupport save and restore configuration file, then should remark belwo macro CONFIG_SAVE_RESTORE
#define CONFIG_SAVE_RESTORE

//E8-A unsupport web upgrade image, we should enable #undef WEB_UPGRADE at line 52
/*########################*/

//jiunming, for e8b telecomacount enable
//#define CTC_TELECOM_ACCOUNT		1

//add by ramen
#undef  DNS_BIND_PVC_SUPPORT
#undef QOS_SPEED_LIMIT_SUPPORT

#define  DHCPS_POOL_COMPLETE_IP
#undef  DHCPS_DNS_OPTIONS
/* Modify default config for Telefonica */
#undef  TELEFONICA_DEFAULT_CFG
#undef  ZHONE_DEFAULT_CFG
#undef ACCOUNT_CONFIG
#undef MULTI_USER_PRIV
#ifdef ZHONE_DEFAULT_CFG
#define MULTI_USER_PRIV
#endif
#ifdef MULTI_USER_PRIV
#define ACCOUNT_CONFIG
#endif

/*ql:20080729 START: different image header for different IC type*/
#undef MULTI_IC_SUPPORT
/*ql:20080729 END*/

/*xl_yue:20090210 add cli cmdedit*/
#ifdef CONFIG_USER_CMD_CLI
#define CONFIG_CLI_CMD_EDIT
#define CONFIG_CLI_TAB_FEATURE
#endif

//added by ql to support imagenio service
//#define IMAGENIO_IPTV_SUPPORT


#endif  // INCLUDE_OPTIONS_H

#undef AUTO_DETECT_DMZ
#ifdef CONFIG_USER_PARENTAL_CONTROL
#define PARENTAL_CTRL
#endif
// add by yq_zhou
#undef CONFIG_11N_SAGEM_WEB

// Magician
//#define COMMIT_IMMEDIATELY

//support reserved IP addresses for DHCP, jiunming
#define SUPPORT_DHCP_RESERVED_IPADDR	1

