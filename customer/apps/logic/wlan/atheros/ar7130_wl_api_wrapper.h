/*
   ar7130_wl_api_wrapper.h - ar7130 wireless api wrapper header file
   Revision History :
   ----------------
   DATE                           NAME       NOTES
   2007-10-31 09:51上午       wxl          create

 */

#ifndef _AR7130_WIRELESS_API_H
#define _AR7130_WIRELESS_API_H

#include "../wl_api.h"

#define AR7130_DEBUG 1

#ifdef AR7130_DEBUG

#define WRAPPER_DEBUG(fmt, args...) printf("%s:%4d "fmt, __FUNCTION__, __LINE__, ##args)

#endif

#define CMD_PARA_MAX_LEN    30
#define CMD_PARA_MAX_NUMS 15

#define BEACON_TYPE_WPA					"WPA"
#define BEACON_TYPE_11i						"11i"

#define MAX_VAP_NUM           MAX_NET_ID

/* 需要根据rootfs中对应路径进行修改 */
#define UNINSTALL_WIFI_MODULE_SCRIPT "/etc/init.d/rc.wlan down"
#define INSTALL_SACN_MODULES "insmod /lib/modules/2.6.18.8/net/wlan_scan_ap.ko;insmod /lib/modules/2.6.18.8/net/wlan_scan_sta.ko;"
#define INSTALL_SCAN_AP_MODULE         "insmod /lib/modules/2.6.18.8/net/wlan_scan_ap.ko;ifconfig ath0 up;"
#define INSTALL_WIFI_MODULE_SCRIPT "export ATH_countrycode=156; /etc/init.d/rc.wlan up"
#define INSTALL_WIFI_FIXABLE_SCRIPT "export ATH_countrycode=%d; /etc/init.d/rc.wlan up"
#define B_MODE_STR  				      "0:RF:6:11B"
#define G_MODE_STR  				      "0:RF:6:11G"
#define NG20_MODE_STR  			      "0:RF:6:11NGHT20"
#define NGAUTO_MODE_STR  			"0:RF:6:11NGHT40PLUS"
#define NG40M_MODE_STR  				"0:RF:6:11NGHT40PLUS"

// some path settings
#define HOSTAPD_CFGFILE_PATH  		"/var/hostapd"  // path of hostapd config file
#define HOSTAPD_PID_REC_FILE_PATH 	"/var/hostapd"  //  record file path of pid of hostapd

#define WDS_BRIGE_NAME   "wdsbr"     /* 特定用于wds的桥*/

#define __FUNCTION__ (__func__)

typedef struct
{
    int   netif_enable;          // indicate whether the net interface is enable
    int   hostap_need;          // hostap only needed in wpa mode (not normal in wep case,this works as a temp workaround way)
    char cfg_filename[128];   // the associated config filename
    char pid_rec_filename[128];   // the associated config filename

    /*netdevice name, bridge,Driver interface type */
    char interface[10]; 		// 10 character for interface name is enough,such as ath0
    char bridge[10];     		// 10 character for bridge name is enough,such as br0
    char driver[15];     		// Driver interface type, in our case it is always "madwifi"
    /* hostapd event logger ,debugging configuration */
    int   logger_syslog; 		// ref value is -1
    int   logger_syslog_level;	// ref calue is 2
    int   logger_stdout;		// ref value is -1
    int   logger_stdout_level;	// ref value is 2
    int   debug;				// ref value is 0,Debugging: 0 = no, 1 = minimal, 2 = verbose, 3 = msg dumps, 4 = excessive
    char dump_file[128];		// Dump file for state information (on SIGUSR1)

    /* IEEE 802.11 related configuration */
    char ssid[33];			// SSID to be used in IEEE 802.11 management frames, eg Atheros_AP71
    int   macaddr_acl;          /* Station MAC address -based authentication
                                       0 = accept unless in deny list
                                       1 = deny unless in accept list
                                       2 = use external RADIUS server (accept/deny lists are searched first) */
    char accept_mac_file[128]; /* Accept/deny lists are read from separate files (containing list of
                                         MAC addresses, one per line). Use absolute path name to make sure that the
                                         files can be read on SIGHUP configuration reloads.
                                         eg /etc/hostapd/hostapd.accept   */
    char deny_mac_file[128];  // eg /etc/hostapd/hostapd.deny

    int   auth_algs;			/* IEEE 802.11 specifies two authentication algorithms. hostapd can be
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
                                       server. This is also needed if hostapd is configured to act as a RADIUS
                                       authentication server. */
    char eap_user_file[128];	// Path for EAP server user database,eg /etc/hostapd.eap_user
    char ca_cert[128];		// CA certificate (PEM or DER file) for EAP-TLS/PEAP/TTLS, eg /etc/hostapd.ca.pem
    char server_cert[128];	// Server certificate (PEM or DER file) for EAP-TLS/PEAP/TTLS, eg /etc/hostapd.server.pem
    char private_key[128];	/* Private key matching with the server certificate for EAP-TLS/PEAP/TTLS
                                       This may point to the same file as server_cert if both certificate and key
                                       are included in a single file. PKCS#12 (PFX) file (.p12/.pfx) can also be
                                       used by commenting out server_cert and specifying the PFX file as the private_key.
                                       eg /etc/hostapd.server.prv */
    char private_key_passwd[128]; // Passphrase for private key
    int check_crl;			// Enable CRL verification. eg 1
    char eap_sim_db[128];	/* Configuration data for EAP-SIM database/authentication gateway interface.
                                        This is a text string in implementation specific format. The example
                                        implementation in eap_sim_db.c uses this as the file name for the GSM
                                        authentication triplets. eg /etc/hostapd.sim_db */

    /* IEEE 802.11f - Inter-Access Point Protocol (IAPP) */
   char iapp_interface[10]; 	// Interface to be used for IAPP broadcast packets, eg eth0

    /* RADIUS client configuration */
    int   radius_enable;         // wxl add to indicate whether radius is enbaled or not
    int   radius_account_enable;    //liwei add for indicate whether radius is enabled
    char own_ip_addr[20];	// The own IP address of the access point (used as NAS-IP-Address), eg 127.0.0.1
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
    /* hostapd can be used as a RADIUS authentication server for other hosts. This
        requires that the integrated EAP authenticator is also enabled and both
        authentication services are sharing the same configuration. */
    char radius_server_clients[128]; // File name of the RADIUS clients configuration for the RADIUS server,
                                           // eg /etc/hostapd.radius_clients
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
    char wpa_psk_file[128];   // Optionally, WPA PSKs can be read from a separate text file, eg /etc/hostapd.wpa_psk
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
}THostapconfig;

enum EN_CountryCode {
    CTRY_DEFAULT            = 0,
    CTRY_ALBANIA              = 8,       /* Albania */
    CTRY_ALGERIA              = 12,      /* Algeria */
    CTRY_ARGENTINA            = 32,      /* Argentina */
    CTRY_ARMENIA              = 51,      /* Armenia */
    CTRY_AUSTRALIA            = 36,      /* Australia */
    CTRY_AUSTRIA              = 40,      /* Austria */
    CTRY_AZERBAIJAN           = 31,      /* Azerbaijan */
    CTRY_BAHRAIN              = 48,      /* Bahrain */
    CTRY_BELARUS              = 112,     /* Belarus */
    CTRY_BELGIUM              = 56,      /* Belgium */
    CTRY_BELIZE               = 84,      /* Belize */
    CTRY_BOLIVIA              = 68,      /* Bolivia */
    CTRY_BOSNIA_HERZEGOWINA   = 70, 	 /* Bosnia and Herzegowina */
    CTRY_BRAZIL               = 76,      /* Brazil */
    CTRY_BRUNEI_DARUSSALAM    = 96,      /* Brunei Darussalam */
    CTRY_BULGARIA             = 100,     /* Bulgaria */
    CTRY_CANADA               = 124,     /* Canada */
    CTRY_CHILE                = 152,     /* Chile */
    CTRY_CHINA                = 156,     /* People's Republic of China */
    CTRY_COLOMBIA             = 170,     /* Colombia */
    CTRY_COSTA_RICA           = 188,     /* Costa Rica */
    CTRY_CROATIA              = 191,     /* Croatia */
    CTRY_CYPRUS               = 196,
    CTRY_CZECH                = 203,     /* Czech Republic */
    CTRY_DENMARK              = 208,     /* Denmark */
    CTRY_DOMINICAN_REPUBLIC   = 214,     /* Dominican Republic */
    CTRY_ECUADOR              = 218,     /* Ecuador */
    CTRY_EGYPT                = 818,     /* Egypt */
    CTRY_EL_SALVADOR          = 222,     /* El Salvador */
    CTRY_ESTONIA              = 233,     /* Estonia */
    CTRY_FAEROE_ISLANDS       = 234,     /* Faeroe Islands */
    CTRY_FINLAND              = 246,     /* Finland */
    CTRY_FRANCE               = 250,     /* France */
    CTRY_FRANCE2              = 255,     /* France2 */
    CTRY_GEORGIA              = 268,     /* Georgia */
    CTRY_GERMANY              = 276,     /* Germany */
    CTRY_GREECE               = 300,     /* Greece */
    CTRY_GUATEMALA            = 320,     /* Guatemala */
    CTRY_HONDURAS             = 340,     /* Honduras */
    CTRY_HONG_KONG            = 344,     /* Hong Kong S.A.R., P.R.C. */
    CTRY_HUNGARY              = 348,     /* Hungary */
    CTRY_ICELAND              = 352,     /* Iceland */
    CTRY_INDIA                = 356,     /* India */
    CTRY_INDONESIA            = 360,     /* Indonesia */
    CTRY_IRAN                 = 364,     /* Iran */
    CTRY_IRAQ                 = 368,     /* Iraq */
    CTRY_IRELAND              = 372,     /* Ireland */
    CTRY_ISRAEL               = 376,     /* Israel */
    CTRY_ITALY                = 380,     /* Italy */
    CTRY_JAMAICA              = 388,     /* Jamaica */
    CTRY_JAPAN                = 392,     /* Japan */
    CTRY_JAPAN1               = 393,     /* Japan (JP1) */
    CTRY_JAPAN2               = 394,     /* Japan (JP0) */
    CTRY_JAPAN3               = 395,     /* Japan (JP1-1) */
    CTRY_JAPAN4               = 396,     /* Japan (JE1) */
    CTRY_JAPAN5               = 397,     /* Japan (JE2) */
    CTRY_JAPAN6               = 399,	 /* Japan (JP6) */
    CTRY_JAPAN7               = 4007,	 /* Japan */
    CTRY_JAPAN8               = 4008,	 /* Japan */
    CTRY_JAPAN9               = 4009,	 /* Japan */
    CTRY_JAPAN10	      = 4010,	 /* Japan */
    CTRY_JAPAN11	      = 4011,	 /* Japan */
    CTRY_JAPAN12	      = 4012,	 /* Japan */
    CTRY_JAPAN13	      = 4013,	 /* Japan */
    CTRY_JAPAN14	      = 4014,	 /* Japan */
    CTRY_JAPAN15	      = 4015,	 /* Japan */
    CTRY_JAPAN16	      = 4016,	 /* Japan */
    CTRY_JAPAN17	      = 4017,	 /* Japan */
    CTRY_JAPAN18	      = 4018,	 /* Japan */
    CTRY_JAPAN19	      = 4019,	 /* Japan */
    CTRY_JAPAN20	      = 4020,	 /* Japan */
    CTRY_JAPAN21	      = 4021,	 /* Japan */
    CTRY_JAPAN22	      = 4022,	 /* Japan */
    CTRY_JAPAN23	      = 4023,	 /* Japan */
    CTRY_JAPAN24	      = 4024,	 /* Japan */
    CTRY_JORDAN               = 400,     /* Jordan */
    CTRY_KAZAKHSTAN           = 398,     /* Kazakhstan */
    CTRY_KENYA                = 404,     /* Kenya */
    CTRY_KOREA_NORTH          = 408,     /* North Korea */
    CTRY_KOREA_ROC            = 410,     /* South Korea */
    CTRY_KOREA_ROC2           = 411,     /* South Korea */
    CTRY_KUWAIT               = 414,     /* Kuwait */
    CTRY_LATVIA               = 428,     /* Latvia */
    CTRY_LEBANON              = 422,     /* Lebanon */
    CTRY_LIBYA                = 434,     /* Libya */
    CTRY_LIECHTENSTEIN        = 438,     /* Liechtenstein */
    CTRY_LITHUANIA            = 440,     /* Lithuania */
    CTRY_LUXEMBOURG           = 442,     /* Luxembourg */
    CTRY_MACAU                = 446,     /* Macau */
    CTRY_MACEDONIA            = 807,     /* the Former Yugoslav Republic of Macedonia */
    CTRY_MALAYSIA             = 458,     /* Malaysia */
    CTRY_MEXICO               = 484,     /* Mexico */
    CTRY_MONACO               = 492,     /* Principality of Monaco */
    CTRY_MOROCCO              = 504,     /* Morocco */
    CTRY_NETHERLANDS          = 528,     /* Netherlands */
    CTRY_NEW_ZEALAND          = 554,     /* New Zealand */
    CTRY_NICARAGUA            = 558,     /* Nicaragua */
    CTRY_NORWAY               = 578,     /* Norway */
    CTRY_OMAN                 = 512,     /* Oman */
    CTRY_PAKISTAN             = 586,     /* Islamic Republic of Pakistan */
    CTRY_PANAMA               = 591,     /* Panama */
    CTRY_PARAGUAY             = 600,     /* Paraguay */
    CTRY_PERU                 = 604,     /* Peru */
    CTRY_PHILIPPINES          = 608,     /* Republic of the Philippines */
    CTRY_POLAND               = 616,     /* Poland */
    CTRY_PORTUGAL             = 620,     /* Portugal */
    CTRY_PUERTO_RICO          = 630,     /* Puerto Rico */
    CTRY_QATAR                = 634,     /* Qatar */
    CTRY_ROMANIA              = 642,     /* Romania */
    CTRY_RUSSIA               = 643,     /* Russia */
    CTRY_SAUDI_ARABIA         = 682,     /* Saudi Arabia */
    CTRY_SINGAPORE            = 702,     /* Singapore */
    CTRY_SLOVAKIA             = 703,     /* Slovak Republic */
    CTRY_SLOVENIA             = 705,     /* Slovenia */
    CTRY_SOUTH_AFRICA         = 710,     /* South Africa */
    CTRY_SPAIN                = 724,     /* Spain */
    CTRY_SRI_LANKA	      = 144,     /* Sri Lanka */
    CTRY_SWEDEN               = 752,     /* Sweden */
    CTRY_SWITZERLAND          = 756,     /* Switzerland */
    CTRY_SYRIA                = 760,     /* Syria */
    CTRY_TAIWAN               = 158,     /* Taiwan */
    CTRY_THAILAND             = 764,     /* Thailand */
    CTRY_TRINIDAD_Y_TOBAGO    = 780,     /* Trinidad y Tobago */
    CTRY_TUNISIA              = 788,     /* Tunisia */
    CTRY_TURKEY               = 792,     /* Turkey */
    CTRY_UAE                  = 784,     /* U.A.E. */
    CTRY_UKRAINE              = 804,     /* Ukraine */
    CTRY_UNITED_KINGDOM       = 826,     /* United Kingdom */
    CTRY_UNITED_STATES        = 840,     /* United States */
    CTRY_UNITED_STATES_FCC49  = 842,     /* United States (Public Safety)*/
    CTRY_URUGUAY              = 858,     /* Uruguay */
    CTRY_UZBEKISTAN           = 860,     /* Uzbekistan */
    CTRY_VENEZUELA            = 862,     /* Venezuela */
    CTRY_VIET_NAM             = 704,     /* Viet Nam */
    CTRY_YEMEN                = 887,     /* Yemen */
    CTRY_ZIMBABWE             = 716,     /* Zimbabwe */
    CTRY_AUSTRALIA2           = 5000,    /* Australia */
    CTRY_CANADA2              = 5001,     /* Canada */
    CTRY_DEBUG                 = 0x1fff
};

typedef struct
{
    unsigned int CountryCode;
    const char * CountryString;
}ST_Conutries;

ST_Conutries stCountries[] = {
    {CTRY_DEBUG,	 	"DB" },
    {CTRY_DEFAULT,	 	"NA" },
    {CTRY_ALBANIA,		"AL" },
    {CTRY_ALGERIA,		"DZ" },
    {CTRY_ARGENTINA,		"AR" },
    {CTRY_ARMENIA,		"AM" },
    {CTRY_AUSTRALIA,		"AU" },
    {CTRY_AUSTRALIA2,		"AU" },
    {CTRY_AUSTRIA,		"AT" },
    {CTRY_AZERBAIJAN,		"AZ" },
    {CTRY_BAHRAIN,		"BH" },
    {CTRY_BELARUS,		"BY" },
    {CTRY_BELGIUM,		"BE" },
    {CTRY_BELIZE,		"BZ" },
    {CTRY_BOLIVIA,		"BO" },
    {CTRY_BOSNIA_HERZEGOWINA,   "BA" },
    {CTRY_BRAZIL,		"BR" },
    {CTRY_BRUNEI_DARUSSALAM,	"BN" },
    {CTRY_BULGARIA,		"BG" },
    {CTRY_CANADA,		"CA" },
    {CTRY_CANADA2,		"CA" },
    {CTRY_CHILE,		"CL" },
    {CTRY_CHINA,		"CN" },
    {CTRY_COLOMBIA,		"CO" },
    {CTRY_COSTA_RICA,		"CR" },
    {CTRY_CROATIA,		"HR" },
    {CTRY_CYPRUS,		"CY" },
    {CTRY_CZECH,		"CZ" },
    {CTRY_DENMARK,		"DK" },
    {CTRY_DOMINICAN_REPUBLIC,	"DO" },
    {CTRY_ECUADOR,		"EC" },
    {CTRY_EGYPT,		"EG" },
    {CTRY_EL_SALVADOR,		"SV" },
    {CTRY_ESTONIA,		"EE" },
    {CTRY_FINLAND,		"FI" },
    {CTRY_FRANCE,		"FR" },
    {CTRY_FRANCE2,		"F2" },
    {CTRY_GEORGIA,		"GE" },
    {CTRY_GERMANY,		"DE" },
    {CTRY_GREECE,		"GR" },
    {CTRY_GUATEMALA,		"GT" },
    {CTRY_HONDURAS,		"HN" },
    {CTRY_HONG_KONG,		"HK" },
    {CTRY_HUNGARY,		"HU" },
    {CTRY_ICELAND,		"IS" },
    {CTRY_INDIA,		"IN" },
    {CTRY_INDONESIA,		"ID" },
    {CTRY_IRAN,			"IR" },
    {CTRY_IRELAND,		"IE" },
    {CTRY_ISRAEL,		"IL" },
    {CTRY_ITALY,		"IT" },
    {CTRY_JAPAN,		"JP" },
    {CTRY_JAPAN1,		"J1" },
    {CTRY_JAPAN2,		"J2" },
    {CTRY_JAPAN3,		"J3" },
    {CTRY_JAPAN4,		"J4" },
    {CTRY_JAPAN5,		"J5" },
    {CTRY_JAPAN6,		"J6" },
    {CTRY_JAPAN7,		"JP" },
    {CTRY_JAPAN8,		"JP" },
    {CTRY_JAPAN9,	      	"JP" },
    {CTRY_JAPAN10,	      	"JP" },
    {CTRY_JAPAN11,	      	"JP" },
    {CTRY_JAPAN12,	      	"JP" },
    {CTRY_JAPAN13,	      	"JP" },
    {CTRY_JAPAN14,	      	"JP" },
    {CTRY_JAPAN15,	      	"JP" },
    {CTRY_JAPAN16,	      	"JP" },
    {CTRY_JAPAN17,	      	"JP" },
    {CTRY_JAPAN18,	      	"JP" },
    {CTRY_JAPAN19,	      	"JP" },
    {CTRY_JAPAN20,	      	"JP" },
    {CTRY_JAPAN21,	      	"JP" },
    {CTRY_JAPAN22,	      	"JP" },
    {CTRY_JAPAN23,	      	"JP" },
    {CTRY_JAPAN24,	      	"JP" },
    {CTRY_JORDAN,		"JO" },
    {CTRY_KAZAKHSTAN,		"KZ" },
    {CTRY_KENYA,		"KE" },
    {CTRY_KOREA_NORTH,		"KP" },
    {CTRY_KOREA_ROC,		"KR" },
    {CTRY_KOREA_ROC2,		"K2" },
    {CTRY_KUWAIT,		"KW" },
    {CTRY_LATVIA,		"LV" },
    {CTRY_LEBANON,		"LB" },
    {CTRY_LIECHTENSTEIN,	"LI" },
    {CTRY_LITHUANIA,		"LT" },
    {CTRY_LUXEMBOURG,		"LU" },
    {CTRY_MACAU,		"MO" },
    {CTRY_MACEDONIA,		"MK" },
    {CTRY_MALAYSIA,		"MY" },
    {CTRY_MEXICO,		"MX" },
    {CTRY_MONACO,		"MC" },
    {CTRY_MOROCCO,		"MA" },
    {CTRY_NETHERLANDS,		"NL" },
    {CTRY_NEW_ZEALAND,		"NZ" },
    {CTRY_NORWAY,		"NO" },
    {CTRY_OMAN,			"OM" },
    {CTRY_PAKISTAN,		"PK" },
    {CTRY_PANAMA,		"PA" },
    {CTRY_PERU,			"PE" },
    {CTRY_PHILIPPINES,		"PH" },
    {CTRY_POLAND,		"PL" },
    {CTRY_PORTUGAL,		"PT" },
    {CTRY_PUERTO_RICO,		"PR" },
    {CTRY_QATAR,		"QA" },
    {CTRY_ROMANIA,		"RO" },
    {CTRY_RUSSIA,		"RU" },
    {CTRY_SAUDI_ARABIA,		"SA" },
    {CTRY_SINGAPORE,		"SG" },
    {CTRY_SLOVAKIA,		"SK" },
    {CTRY_SLOVENIA,		"SI" },
    {CTRY_SOUTH_AFRICA,		"ZA" },
    {CTRY_SPAIN,		"ES" },
    {CTRY_SRI_LANKA,            "LK" },
    {CTRY_SWEDEN,		"SE" },
    {CTRY_SWITZERLAND,		"CH" },
    {CTRY_SYRIA,		"SY" },
    {CTRY_TAIWAN,		"TW" },
    {CTRY_THAILAND,		"TH" },
    {CTRY_TRINIDAD_Y_TOBAGO,	"TT" },
    {CTRY_TUNISIA,		"TN" },
    {CTRY_TURKEY,		"TR" },
    {CTRY_UKRAINE,		"UA" },
    {CTRY_UAE,			"AE" },
    {CTRY_UNITED_KINGDOM,	"GB" },
    {CTRY_UNITED_STATES,	"US" },
    {CTRY_UNITED_STATES_FCC49,	"US" },
    {CTRY_URUGUAY,		"UY" },
    {CTRY_UZBEKISTAN,		"UZ" },
    {CTRY_VENEZUELA,		"VE" },
    {CTRY_VIET_NAM,		"VN" },
    {CTRY_YEMEN,		"YE" },
    {CTRY_ZIMBABWE,		"ZW" }
};



extern const char *ar7130_netif_strings[];
/* exported function*/
extern unsigned int ar7130_enum_member_api_to_vender(unsigned char enum_type, unsigned int member_of_api);
extern unsigned int ar7130_enum_member_vender_to_api(unsigned char enum_type, unsigned int member_of_vender);
extern int ar7130_install_wlan_module(TWlCfgParas  *ptWlCfgParas);
extern int ar7130_create_vap(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal);
extern int ar7130_delete_vap(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal);
extern int ar7130_wl_api_init(void);
extern int ar7130_wl_basic_set(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int ar7130_wl_misc_set(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int ar7130_wl_private_set(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int ar7130_wl_wep_set(int netif_id,TWlAuthEncCfgParas *ptWlAuthEncCfgVal,unsigned int cfg_mask , unsigned int mask_count );
extern int ar7130_wl_wpa_set(int netif_id,TWlAuthEncCfgParas *ptWlAuthEncCfgVal,unsigned int cfg_mask , unsigned int mask_count );
extern int ar7130_wl_macfilter_set(int netif_id, TWlMacAuthCfgParas  *ptWlMacAuthCfgVal, unsigned int cfg_mask, unsigned mask_count);
extern int ar7130_wl_radius_set(int netif_id, TWlRadiusCfgParas  *ptWlRadiusCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int ar7130_wl_qos_set(int netif_id, TWlChannelCfgParas  *ptWlChannelCfgVal, unsigned int cfg_mask,unsigned int mask_count);
extern int ar7130_wl_roaming_set(int netif_id,TWlAuthEncCfgParas *ptWlAuthEncCfgVal, unsigned int cfg_mask,unsigned int mask_count);

extern int ar7130_wl_statistic_get(int netif_id, TWlStatisticParas  *ptWlStatistic);
extern int ar7130_wl_stations_get(int netif_id, TStationInfoList *ptWlStatioin);
extern int ar7130_up_wlan_if(void);
extern int ar7130_down_wlan_if(void);
extern int ar7130_apply_wl_cfg(void);
extern int ar7130_uninstall_wlan_module(void);

const char * ar7130_netif_id_to_name(int net_id);

#endif
