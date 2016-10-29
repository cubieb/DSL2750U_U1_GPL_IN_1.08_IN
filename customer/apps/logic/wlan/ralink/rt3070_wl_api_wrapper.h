/*
   rt3070_wl_api_wrapper.h - rt3070 wireless api wrapper header file 
   Revision History :
   ----------------
   DATE                           NAME       NOTES
   2007-10-31 09:51上午       wxl          create

 */

#ifndef _rt3070_WIRELESS_API_H
#define _rt3070_WIRELESS_API_H

#include "../wl_api.h"

#define rt3070_DEBUG 1

#ifdef rt3070_DEBUG

#define WRAPPER_DEBUG(fmt, args...) printf("%s:%4d "fmt, __FUNCTION__, __LINE__, ##args)

#endif 

#define CMD_PARA_MAX_LEN    30
#define CMD_PARA_MAX_NUMS 15

#define ATETXBW_20M 0
#define ATETXBW_40M 1

#define BEACON_TYPE_WPA					"WPA"
#define BEACON_TYPE_11i						"11i"


/* 需要根据rootfs中对应路径进行修改 */
#define UNINSTALL_WIFI_MODULE_SCRIPT "rmmod ralink_wlan.ko"
#define INSTALL_SACN_MODULES "insmod /lib/modules/2.6.18.8/net/wlan_scan_ap.ko;insmod /lib/modules/2.6.18.8/net/wlan_scan_sta.ko;"
#define INSTALL_SCAN_AP_MODULE         "insmod /lib/modules/2.6.18.8/net/wlan_scan_ap.ko;ifconfig ath0 up;"
//#define INSTALL_WIFI_MODULE_SCRIPT "insmod ralink_wlan.ko;ifconfig ra0 up"
#define INSTALL_WIFI_MODULE_SCRIPT "insmod ralink_wlan.ko;ifconfig ra0 up"
#define B_MODE_STR  				      "0:RF:6:11B"
#define G_MODE_STR  				      "0:RF:6:11G"
#define NG20_MODE_STR  			      "0:RF:6:11NGHT20"
#define NGAUTO_MODE_STR  			"0:RF:6:11NGHT40PLUS"
#define NG40M_MODE_STR  				"0:RF:6:11NGHT40PLUS"

// some path settings
#define RT2860APD_CFGFILE_PATH  		"/etc/Wireless/ralink/ralink_wlan.dat"  // path of rt2860apd config file

#define WDS_BRIGE_NAME   "wdsbr"     /* 特定用于wds的桥*/

#define __FUNCTION__ (__func__)

typedef struct
{
    int   netif_enable;          // indicate whether the net interface is enable 
    int   rt2860apd_need;    // rt2860apd only needed in radius auth mode 
    
    /*netdevice name, bridge,Driver interface type */
    char interface[10]; 		// 10 character for interface name is enough,such as ath0
    char bridge[10];     		// 10 character for bridge name is enough,such as br0
    char driver[15];     		// Driver interface type, in our case it is always "madwifi"

    /* IEEE 802.11 related configuration */
    char ssid[33];			// SSID to be used in IEEE 802.11 management frames, eg Atheros_AP71
    
    int   auth_algs;			/* IEEE 802.11 specifies two authentication algorithms. rt2860apd can be
                                        configured to allow both of these or only one. Open system authentication 
                                        should be used with IEEE 802.1X.
                                        Bit fields of allowed authentication algorithms:
                                        bit 0 = Open System Authentication
                                        bit 1 = Shared Key Authentication (requires WEP) */

    /* IEEE 802.1X-REV related configuration */
    int ieee8021x;			// Require IEEE 802.1X authorization, 1 if needed
    char eap_message[255];	// Optional displayable message sent with EAP Request-Identity, eg "hello"
    int wep_key_len_broadcast;/* WEP rekeying (disabled if key lengths are not set or are set to 0)
                                        Key lengths for default/broadcast and individual/unicast keys:
                                        5 = 40-bit WEP (also known as 64-bit WEP with 40 secret bits)
                                        13 = 104-bit WEP (also known as 128-bit WEP with 104 secret bits) */
    int wep_key_len_unicast;
    int wep_rekey_period;		// Rekeying period in seconds. 0 = do not rekey (i.e., set keys only once)
    int eapol_key_index_workaround; /* EAPOL-Key index workaround (set bit7) for WinXP Supplicant (needed only if
                                               only broadcast keys are used) */
    int eap_reauth_period;	/* EAP reauthentication period in seconds (default: 3600 seconds; 0 = disable
                                        reauthentication). */
    
    /* Integrated EAP server */
    int eap_server;			/* Use integrated EAP server instead of external RADIUS authentication
                                       server. This is also needed if rt2860apd is configured to act as a RADIUS
                                       authentication server. */
    char eap_user_file[128];	// Path for EAP server user database,eg /etc/rt2860apd.eap_user
    char ca_cert[128];		// CA certificate (PEM or DER file) for EAP-TLS/PEAP/TTLS, eg /etc/rt2860apd.ca.pem
    char server_cert[128];	// Server certificate (PEM or DER file) for EAP-TLS/PEAP/TTLS, eg /etc/rt2860apd.server.pem
    char private_key[128];	/* Private key matching with the server certificate for EAP-TLS/PEAP/TTLS
                                       This may point to the same file as server_cert if both certificate and key
                                       are included in a single file. PKCS#12 (PFX) file (.p12/.pfx) can also be
                                       used by commenting out server_cert and specifying the PFX file as the private_key.
                                       eg /etc/rt2860apd.server.prv */
    char private_key_passwd[128]; // Passphrase for private key
    int check_crl;			// Enable CRL verification. eg 1
    char eap_sim_db[128];	/* Configuration data for EAP-SIM database/authentication gateway interface.
                                        This is a text string in implementation specific format. The example
                                        implementation in eap_sim_db.c uses this as the file name for the GSM
                                        authentication triplets. eg /etc/rt2860apd.sim_db */

    /* IEEE 802.11f - Inter-Access Point Protocol (IAPP) */
   char iapp_interface[10]; 	// Interface to be used for IAPP broadcast packets, eg eth0

    /* RADIUS client configuration */
    int   radius_enable;         // wxl add to indicate whether radius is enbaled or not
    int   radius_account_enable;    //liwei add for indicate whether radius is enabled 
    char nas_identifier[128];	// Optional NAS-Identifier string for RADIUS messages, eg ap.example.com
    
    char primary_auth_server_addr[20];// RADIUS authentication server ip, eg 127.0.0.1
    int   primary_auth_server_port;	// RADIUS authentication server  port ,eg 1812
    char primary_auth_server_shared_secret[128];// secret string
    char  primary_acct_server_addr[20];// RADIUS account server ip, eg 127.0.0.1
    int    primary_acct_server_port;	 // RADIUS account server port ,eg 1813
    char  primary_acct_server_shared_secret[128];//secrec string
    
    /*  Secondary RADIUS servers; to be used if primary one does not reply to
         RADIUS packets. These are optional and there can be more than one secondary
         server listed. same config item as primary*/
    char secondary_auth_server_addr[20];// RADIUS authentication server ip, eg 127.0.0.2
    int   secondary_auth_server_port;	// RADIUS authentication server  port ,eg 1812
    char secondary_auth_server_shared_secret[128];// secret string
    char  secondary_acct_server_addr[20];// RADIUS account server ip, eg 127.0.0.2
    int    secondary_acct_server_port;	 // RADIUS account server port ,eg 1813
    char  secondary_acct_server_shared_secret[128];//secrec string

    int radius_retry_primary_interval;// Retry interval for trying to return to the primary RADIUS server(in seconds),
                                            // ref value 600
    int radius_acct_interim_interval;  // Interim accounting update interval (in seconds), ref value 600

    /* RADIUS authentication server configuration */
    /* rt2860apd can be used as a RADIUS authentication server for other hosts. This
        requires that the integrated EAP authenticator is also enabled and both
        authentication services are sharing the same configuration. */
    char radius_server_clients[128]; // File name of the RADIUS clients configuration for the RADIUS server,
                                           // eg /etc/rt2860apd.radius_clients
    int  radius_server_auth_port;    // The UDP port number for the RADIUS authentication server,eg 1812
    int  radius_server_ipv6;		// Use IPv6 with RADIUS server (IPv4 will also be supported using IPv6 API), eg 1

    /* WPA/IEEE 802.11i configuration */
    int wpa;			/* Enable WPA,This field is a bit field that can be used to enable WPA (IEEE 802.11i/D3.0)
                                 and/or WPA2 (full IEEE 802.11i/RSN):
                                 bit0 = WPA
                                 bit1 = IEEE 802.11i/RSN (WPA2) (dot11RSNAEnabled)
                              */
    char wpa_passphrase[64]; // WPA pre-shared keys for WPA-PSK, secret passphrase,in ascii format
    char wpa_presharekey[64]; // WPA pre-shared keys for WPA-PSK, secret passphrase,in hex format
    char wpa_psk_file[128];   // Optionally, WPA PSKs can be read from a separate text file, eg /etc/rt2860apd.wpa_psk
    char wpa_key_mgmt[30];  /* Set of accepted key management algorithms (WPA-PSK, WPA-EAP, or both). The
                                       entries are separated with a space. eg "WPA-PSK WPA-EAP" */
    char wpa_pairwise[30];   /* Set of accepted cipher suites (encryption algorithms) for pairwise keys, This is a space separated 
                                  list of algorithms 
                                  CCMP = AES in Counter mode with CBC-MAC [RFC 3610, IEEE 802.11i/D7.0]
                                  TKIP = Temporal Key Integrity Protocol [IEEE 802.11i/D7.0]
                                  eg "TKIP CCMP" */
    int wpa_group_rekey;  /* Time interval for rekeying GTK (broadcast/multicast encryption keys) in
                                   seconds, eg 600 */
    int wpa_strict_rekey; // Rekey GTK when any STA that possesses the current GTK is leaving the BSS, eg 1
    int wpa_gmk_rekey;   // Time interval for rekeying GMK (master key used internally to generate GTKs(in seconds), eg 86400
    int rsn_preauth;		// Enable IEEE 802.11i/RSN/WPA2 pre-authentication, eg 1
    char rsn_preauth_interfaces[10]; /* Space separated list of interfaces from which pre-authentication frames are
                                                accepted, eg eth0 */
    char OP_Mode;		//Peter Meng add it for WDS becasue WDS related two vap device
    int wds_index;		//Peter Meng add it for WDS becasue WDS related two vap device

    char WPS_Enable;    //added by heym, 20080820 for WPS
}TVapPrivateCfg;

typedef struct
{
    int   session_timeout_interval; /* session_timeout_interval is for 802.1x reauthentication setting.
                                                           set zero to disable 802.1x reauthentication service for each session.
                                                           session_timeout_interval unit is second and must be larger than 60. */
    char EAPifname[10]; /* EAPifname is assigned as the binding interface for EAP negotiation. */
    char PreAuthifname[10]; /* PreAuthifname is assigned as the binding interface for WPA2 Pre-authentication. */                      
    char cfg_filename[128];   /* the associated config filename */
    int   debug;    /* Debugging: 0 = RT_DEBUG_OFF, 1 = RT_DEBUG_ERROR, 2 = RT_DEBUG_WARN, 
                                                3 = RT_DEBUG_TRACE, 4 = RT_DEBUG_INFO */
    char own_ip_addr[20];	/* The own IP address of the access point (used as NAS-IP-Address), eg 127.0.0.1 */         
    char ap_enable;
    TVapPrivateCfg stPrivCfg[MAX_NET_ID];
}TRt2860apdConfig;

typedef struct
{
    unsigned int CountryNum;
    char * IsoName;
    char Support80211A;
    char CountryRegion_11A;
    char Support80211G;
    char CountryRegion_11G;
}ST_Country_Info;

ST_Country_Info CountryInfo[] = {
    {0,     "DB",   1, 7, 1, 5},
    {8,     "AL",    0, 0, 1, 1},
    {12,    "DZ",   0, 0, 1, 1},
    {32,    "AR",   1, 3, 1, 1},
    {51,    "AM",   1, 2, 1, 1},
    {36,    "AU",   1, 0, 1, 1},
    {40,    "AT",   1, 1, 1, 1},    
    {31,    "AZ",   1, 2, 1, 1},
    {48,    "BH",   1, 0, 1, 1},
    {112,  "BY",   0, 0, 1, 1},
    {56,    "BE",   1, 1, 1, 1},
    {84,    "BZ",   1, 4, 1, 1},
    {68,    "BO",   1, 4, 1, 1},
    {76,    "BR",   1, 1, 1, 1},
    {96,    "BN",   1, 4, 1, 1},
    {100,  "BG",   1, 1, 1, 1},
    {124,  "CA",   1, 0, 1, 0},
    {152,  "CL",   1, 0, 1, 1},
    {156,  "CN",   1, 4, 1, 1},
    {170,  "CO",   1, 0, 1, 0},
    {188,  "CR",   0, 0, 1, 1},
    {191,  "HR",   1, 2, 1, 1},
    {196,  "CY",   1, 1, 1, 1},
    {203,  "CZ",   1, 2, 1, 1},
    {208,  "DK",   1, 1, 1, 1},
    {214,  "DO",   1, 0, 1, 0},
    {218,  "EC",   0, 0, 1, 1},
    {818,  "EG",   1, 2, 1, 1},
    {222,  "SV",   0, 0, 1, 1},
    {233,  "EE",   1, 1, 1, 1},
    {246,  "FI",    1, 1, 1, 1},
    {250,  "FR",   1, 2, 1, 1},
    {268,  "GE",   1, 2, 1, 1},
    {276,  "DE",   1, 1, 1, 1},
    {300,  "GR",   1, 1, 1, 1},
    {320,  "GT",   1, 0, 1, 0},
    {340,  "HN",   0, 0, 1, 1},
    {344,  "HK",   1, 0, 1, 1},
    {348,  "HU",   1, 1, 1, 1},
    {352,  "IS",   1, 1, 1, 1},
    {356,  "IN",   1, 0, 1, 1},
    {360,  "ID",   1, 4, 1, 1},
    {364,  "IR",   1, 4, 1, 1},
    {372,  "IE",   1, 1, 1, 1},
    {376,  "IL",   0, 0, 1, 1},
    {380,  "IT",   1, 1, 1, 1},
    {392,  "JP",   1, 9, 1, 1},
    {400,  "JO",   1, 0, 1, 1},
    {398,  "KZ",   0, 0, 1, 1},
    {408,  "KP",   1, 5, 1, 1},
    {410,  "KR",   1, 5, 1, 1},
    {414,  "KW",   0, 0, 1, 1},
    {428,  "LV",   1, 1, 1, 1},
    {422,  "LB",   0, 0, 1, 1},
    {438,  "LI",   1, 1, 1, 1},
    {440,  "LT",   1, 1, 1, 1},
    {442,  "LU",   1, 1, 1, 1},
    {446,  "MO",   1, 0, 1, 1},
    {807,  "MK",   0, 0, 1, 1},
    {458,  "MY",   1, 0, 1, 1},
    {484,  "MX",   1, 0, 1, 0},
    {492,  "MC",   1, 2, 1, 1},
    {504,  "MA",   0, 0, 1, 1},
    {528,  "NL",   1, 1, 1, 1},
    {554,  "NZ",   1, 0, 1, 1},
    {578,  "NO",   1, 0, 1, 0},
    {512,  "OM",   1, 0, 1, 1},
    {586,  "PK",   0, 0, 1, 1},
    {591,  "PA",   1, 0, 1, 0},
    {604,  "PE",   1, 4, 1, 1},
    {608,  "PH",   1, 4, 1, 1},
    {616,  "PL",   1, 1, 1, 1},
    {620,  "PT",   1, 1, 1, 1},
    {630,  "PR",   1, 0, 1, 0},
    {634,  "QA",   0, 0, 1, 1},
    {642,  "RO",   0, 0, 1, 1},
    {643,  "RU",   0, 0, 1, 1},
    {682,  "SA",   0, 0, 1, 1},
    {702,  "SG",   1, 0, 1, 1},
    {703,  "SK",   1, 1, 1, 1},
    {705,  "SI",   1, 1, 1, 1},
    {710,  "ZA",   1, 1, 1, 1},
    {724,  "ES",   1, 1, 1, 1},
    {752,  "SE",   1, 1, 1, 1},
    {756,  "CH",   1, 1, 1, 1},
    {760,  "SY",   0, 0, 1, 1},
    {158,  "TW",   1, 3, 1, 0},
    {764,  "TH",   0, 0, 1, 1},
    {780,  "TT",   1, 2, 1, 1},
    {788,  "TN",   1, 2, 1, 1},
    {792,  "TR",   1, 2, 1, 1},
    {804,  "UA",   0, 0, 1, 1},
    {784,  "AE",   0, 0, 1, 1},
    {826,  "GB",   1, 1, 1, 1},
    {840,  "US",   1, 0, 1, 0},
    {858,  "UY",   1, 5, 1, 1},
    {860,  "UZ",   1, 1, 1, 0},
    {862,  "VE",   1, 5, 1, 1},
    {704,  "VN",   0, 0, 1, 1},
    {887,  "YE",   0, 0, 1, 1},
    {716,  "ZW",   0, 0, 1, 1},
};

extern const char *rt3070_netif_strings[];
/* exported function*/
extern unsigned int rt3070_enum_member_api_to_vender(unsigned char enum_type, unsigned int member_of_api);
extern unsigned int rt3070_enum_member_vender_to_api(unsigned char enum_type, unsigned int member_of_vender);
extern int rt3070_install_wlan_module(TWlCfgParas  *ptWlCfgParas);
extern int rt3070_create_vap(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal);
extern int rt3070_delete_vap(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal);
extern int rt3070_wl_api_init(void);
extern int rt3070_wl_basic_set(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int rt3070_wl_misc_set(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int rt3070_wl_private_set(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int rt3070_wl_wep_set(int netif_id,TWlAuthEncCfgParas *ptWlAuthEncCfgVal,unsigned int cfg_mask , unsigned int mask_count );
extern int rt3070_wl_wpa_set(int netif_id,TWlAuthEncCfgParas *ptWlAuthEncCfgVal,unsigned int cfg_mask , unsigned int mask_count );
extern int rt3070_wl_macfilter_set(int netif_id, TWlMacAuthCfgParas  *ptWlMacAuthCfgVal, unsigned int cfg_mask, unsigned mask_count);
extern int rt3070_wl_radius_set(int netif_id, TWlRadiusCfgParas  *ptWlRadiusCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int rt3070_wl_qos_set(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int rt3070_wl_roaming_set(int netif_id,TWlAuthEncCfgParas *ptWlAuthEncCfgVal, unsigned int cfg_mask,unsigned int mask_count);

extern int rt3070_wl_statistic_get(int netif_id, TWlStatisticParas  *ptWlStatistic);
extern int rt3070_wl_stations_get(int netif_id, TStationInfoList *ptWlStatioin);
extern int rt3070_apply_wl_cfg(void);
extern void rt3070_enable_ap(int ap_enable);
extern int rt3070_up_wlan_if(void);
extern int rt3070_down_wlan_if(void);
extern int rt3070_uninstall_wlan_module(void);
#endif
