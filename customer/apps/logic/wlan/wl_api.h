/*
   wl_api.h - uniform wireless api header file used in T&W TBS project
   Revision History :
   ----------------
   DATE                           NAME       NOTES
   2007-10-31 09:51上午       wxl          create

 */

#ifndef _WIRELESS_API_H
#define _WIRELESS_API_H

#include "common.h"
#include "../include/timer.h"
/******************************************* some macro definitions  **************************************************/
//for telefonica LED需求
//1 WPS开启就常亮，协商闪，完成(不管成功失败)常亮 
//2 wps失败后红灯亮20s
//#define CONFIG_APPS_LOGIC_WPS_LED_TELEFONICA 1

#define MAX_NET_ID               5 // net if id is in 0~3

#define MAX_SSID_LEN          33
#define MAX_WEPKEY_LEN       39
#define MAX_WPAPSK_LEN       133
#define MAX_DEVNAME_LEN   20
#define MAX_CMD_LEN 256
#define MAX_TR_MODE_LEN 16

// wireless ctrl id (part) definitions
#define CHANNEL_CFG              0
#define AUTHENC_CFG              1
#define MACFILTER_CFG            2
#define RADIUS_CFG                3
#define STATISTIC_GET            4
#define STATIONS_GET              5
#define INSTALL_WLAN_MODULE 6
#define MAKE_FIRST_WLAN_VAP 7
#define APPLY_CFG            	      8
#define CLOSE_API                   0xff

#define  INVALID_ENUM_MEMBER   0xffffffff
#define MODE_PROFILE_ENUM_TYPE   1
#define COUNTRY_ENUM_TYPE          2
#define AUTH_ENUM_TYPE               3
#define ENC_ENUM_TYPE                 4
#define WEPENC_ENUM_TYPE            5
#define TRANSRATE_ENUM_TYPE       6
#define MACADDRAUTH_ENUM_TYPE   7

// key length
#define WEP64_KEY_LEN                 15
#define WEP128_KEY_LEN                39
#define WPA_PSK_LEN                    65

// max items in mac addr filter
#define MAX_MACADDR_ITEMS          32
#define MAX_RADIUS_SERVERS          3

//some length macro
#define MACADDR_LEN                    18
#define MAX_IPADDR_LEN                16
#define MAX_RADKEY_LEN                65

#define MAX_STA_NUM 32

// operation definitions
#define OP_SET                             0
#define OP_GET                             1
#define OP_ADD                            2
#define OP_DEL                             3

// bandwidth mode of 11n definitions
#define BW_20M                 0
#define BW_20M40M_DYN     1
#define BW_40M                 2

// max tx power, unit is mW (milliwatt)
#define MAX_TX_POWER       100

// mask definitions for channel/mode config related paras
#define MASK_CHANNELCFG_AUTOCHANNEL (1<<0)   //  auto channel
#define MASK_CHANNELCFG_COUNTRY        (1<<1)   // country code
#define MASK_CHANNELCFG_DFTCHANNEL   (1<<2)   // default channel
#define MASK_CHANNELCFG_APENABLE       (1<<3)   // ap enable
#define MASK_CHANNELCFG_SSID             (1<<4)   // ssid
#define MASK_CHANNELCFG_HIDESSID       (1<<5)   // hide ssid
#define MASK_CHANNELCFG_FRAGTHR        (1<<6)   // fragment threshhold
#define MASK_CHANNELCFG_BSSRELAY       (1<<7)   // user isolation
#define MASK_CHANNELCFG_QOSWMM       (1<<8)   // Qos wmm
#define MASK_CHANNELCFG_PROFILE         (1<<9)   // 802.11 b/g/mixed mode profile
#define MASK_CHANNELCFG_RTSTHR         (1<<10)  // rts threshold
#define MASK_CHANNELCFG_RATE            (1<<11)  // transmit rate
#define MASK_CHANNELCFG_BEACONP       (1<<12)  // beacon period
#define MASK_CHANNELCFG_DTMP           (1<<13)  // dtm period
#define MASK_CHANNELCFG_VAP             (1<<14)  // create or del virtual ap
#define MASK_11N_SUPPORT                  (1<<15)  // 11n support status
#define MASK_11N_BW                         (1<<16)  // bandwidth setting in 11n mode
#define MASK_CHANNELCFG_TXPOWER (1<<17)  // tx power, unit is mW
#define MASK_CHANNELCFG_MAXSTA (1<<18) //max access station limit
#define MASK_CHANNELCFG_DEVNAME (1<<19) //device name
#define MASK_CHANNELCFG_APISOLATE (1<<20) //device name
#define MASK_CHANNELCFG_POWERSAVE (1<<21)//power save
#define MASK_CHANNELCFG_XR  (1<<22) //xr section
#define MASK_CHANNELCFG_OPMODE  (1<<23) //Operation Mode
#define MASK_CHANNELCFG_WDSENABLE  (1<<24) //wds enable
#define MASK_CHANNELCFG_PEERMAC  (1<<25) //Root AP' MAC
#define MASK_CHANNELCFG_WPSENABLE (1<<26)//wps enable
#define MASK_CHANNELCFG_PREAMBLE (1<<27) //CCK Preamble type
#define MASK_CHANNELCFG_MULTICASTRATE (1<<28) //MulticastRate
#define MASK_CHANNELCFG_CONTROLSIDEBAND (1<<29) //Control SideBand


// mask definitions for authentication/encription related paras
#define MASK_AUTHENCCFG_AUTH                    (1<<0)   //  authentication
#define MASK_AUTHENCCFG_ENC                      (1<<1)   //  encription
#define MASK_AUTHENCCFG_DFTTXKEYIDX         (1<<2)   //  default tx key index
#define MASK_AUTHENCCFG_WEPENC                (1<<3)   //  wep encription
#define MASK_AUTHENCCFG_WEP64KEY             (1<<4)   //  wep mode 64 key
#define MASK_AUTHENCCFG_WEP128KEY           (1<<5)   //  wep mode 128 key
#define MASK_AUTHENCCFG_ENAWPA1             (1<<6)   //  wpa1 enable
#define MASK_AUTHENCCFG_ENAWPA2             (1<<7)   //  wpa2/rsn enable
#define MASK_AUTHENCCFG_WPA                    (1<<8)   //  wpa mode
#define MASK_AUTHENCCFG_ENAWPAPSK           (1<<9)   //  wpa enable psk
#define MASK_AUTHENCCFG_ENAWPAEAP           (1<<10)  //  wpa enable eap
#define MASK_AUTHENCCFG_ENAWPATKIP          (1<<11)  //  wpa enable tkip
#define MASK_AUTHENCCFG_ENAWPAAESCCMP     (1<<12)  //  wpa enable aes ccmp
#define MASK_AUTHENCCFG_ENAWPA2AESCCMP    (1<<13)  //  wpa2 enable aes ccmp
#define MASK_AUTHENCCFG_ENAWPA2TKIP          (1<<14)  //  wpa2 enable tkip
#define MASK_AUTHENCCFG_ENAWPA2PREAUTH    (1<<15)  //  wpa2 enable preauth
#define MASK_AUTHENCCFG_WPAKEYPASSPHRASE    (1<<16)  //  wpa KeyPassphrase , ascii format
#define MASK_AUTHENCCFG_WPAPRESHAREKEY (1<<17)  //  wpa pre share key, hex format
#define MASK_WEP_AUTH_MODE				(1<<18)   // wep auth mode, open or share
#define MASK_AUTHENCCFG_WPAGROUPREKEY    (1<<19)  //Wpa group rekey time
#define MASK_AUTHENCCFG_WPASTRICTREKEY    (1<<20)  //Wpa group rekey when station leave
#define MASK_IAPP_ENABLE (1<<21)    //IAPP enable

#define MASK_WLANCHG_RST_DEAMON (1<<22) //RESTART daemon

// mask definitions for macaddr auth related paras
#define MASK_MACAUTHCFG_AUTH                    (1<<0)   //  mac addr auth type
#define MASK_MACAUTHCFG_MACADDR               (1<<1)   //  mac addr
#define MASK_MACAUTHCFG_MACLIST                (1<<2)   //  current mac addr list
#define MASK_MACAUTHCFG_FLUSH                    (1<<3)   //  flush mac addr

// mask definitions for radius related paras
#define MASK_RADIUSCFG_ENABLE                  (1<<0)   //  radius enable
#define MASK_RADIUSCFG_ACCINTERVAL          (1<<1)   //  radius account interval
#define MASK_RADIUSCFG_RADIUS                  (1<<2)   //  add/remove a radius
#define MASK_RADIUSCFG_RADIUSLIST            (1<<3)   //  current radius list
#define MASK_RADIUSCFG_ACCENABLE                  (1<<4)   //  radius enable

#ifdef CONFIG_APPS_LOGIC_WDS
//WDS Mask
#define MASK_WDS_BASE                   (0)
#define MASK_WDS_ENABLE                 (1 << 0)
#define MASK_WDS_MAC                  (1 << 1)
#define MASK_WDS_RPMAC                  (1 << 2)
#define MASK_WDS_ENC					(1 << 3)
#define MASK_WDS_WEP					(1 << 4)
#define MASK_WDS_PASSPHRASE				(1 << 5)
#define MASK_WDS_GUESTACCESS			(1 << 6)

#define MAX_WDS_MACADDR_LEN 18 
#define MAX_WDS_IF 8

#define BUFLEN_64                      64    //!< buffer length 64
#define BUFLEN_128                     128   //!< buffer length 128

enum EncTypeEnum{
	ENC_MODE_NONE=0,
	ENC_MODE_WEP64,
	ENC_MODE_WEP128,
	ENC_MODE_TKIP,
	ENC_MODE_AES,
	ENC_MODE_TKIPAES,
};

typedef struct
{
	int Enable;
	//int GuestAccess;
	int IfNum;
	char WlanMac[MAX_WDS_MACADDR_LEN+1];
	char RepeaterMac[MAX_WDS_IF][MAX_WDS_MACADDR_LEN+1];

	//enum EncTypeEnum  asEncMode;
       //char asWepKey[ BUFLEN_128 + 1 ];
       //char asKeyPassphrase[ BUFLEN_64 + 1 ];
	unsigned int Mask;
	unsigned int MaskCount;
	
}TWlWDSCfgParas;


#endif


#ifdef CONFIG_APPS_LOGIC_WPS
#define MASK_ENABLE_WPS            (1<<0)
#define MASK_TRIGGER_PBC           (1<<1)
#define MASK_TRIGGER_PIN_LOCAL     (1<<2)
#define MASK_TRIGGER_PIN_PEER      (1<<3)
#define MASK_GENNERATE_PIN         (1<<4)
#define MASK_WPS_ROLE              (1<<5)

enum
{
	CMD_TRIGGER_PBC,
	CMD_TRIGGER_PIN_LOCAL,
	CMD_TRIGGER_PIN_PEER,
	CMD_GENNERATE_PIN
};

#endif

/******************************************* some enum type definitions  **********************************************/
enum WL_ProfileEnum{
    WL_PROFILE_MIXED_G_WIFI,//BG
    WL_PROFILE_B_ONLY,
    WL_PROFILE_MIXED_LONG,
    WL_PROFILE_G_ONLY,
    WL_PROFILE_A,
    WL_PROFILE_NA,
    WL_PROFILE_N_ONLY,
    WL_PROFILE_NG,

// for ralink3070
    WL_PROFILE_BGN,
    WL_PROFILE_AGN,
    WL_PROFILE_N_ONLY_5G,   //for ralink, WL_PROFILE_N_ONLY means freq 2.4G, WL_PROFILE_N_5G freq 5G
};

enum WL_CountryEnum{
        WL_US,
        WL_TW,
        WL_USI,
        WL_AT,
        WL_BE,
        WL_CH,
        WL_DE,
        WL_DK,
        WL_ES,
        WL_FI,
        WL_FR,
        WL_GB,
        WL_GR,
        WL_HU,
        WL_IE,
        WL_IS,
        WL_IT,
        WL_LI,
        WL_LU,
        WL_NL,
        WL_NO,
        WL_PL,
        WL_PT,
        WL_SE,
        WL_JP
};

enum WL_AuthenticationEnum
{
    WL_AUTHENTICATION_NONE,
    WL_AUTHENTICATION_SHARED,
    WL_AUTHENTICATION_WPA_PSK,
    WL_AUTHENTICATION_WPA_EAP
};

enum WL_PSKExpression
{
    WL_PSKEXPRESSION_NONE,
    WL_PSKEXPRESSION_PRESHAREDKEY,
    WL_PSKEXPRESSION_KEYPASSPHRASE
};

enum WL_RestartType
{
    WL_RESTARTTYPE_MODULE = 0,
    WL_RESTARTTYPE_DAEMON,
    WL_RESTARTTYPE_APPLYCFG,
    WL_RESTARTTYPE_IF
};

enum WL_EncryptionEnum{
    WL_ENCRYPTION_NONE,
    WL_ENCRYPTION_WEP64,
    WL_ENCRYPTION_WEP128,
    WL_ENCRYPTION_TKIP,
    WL_ENCRYPTION_AES_CCMP,
    WL_ENCRYPTION_TKIP_AES,
    WL_ENCRYPTION_WPA2_MIXED
};

enum WL_WepEncryptionEnum{
    WL_WEP_MODE_DISABLED,
    WL_WEP_MODE_64BIT,
    WL_WEP_MODE_128BIT
};

enum WL_TransmitRateEnum{
    WL_RATE_AUTOMATIC,
    WL_RATE_1MBPS,
    WL_RATE_2MBPS,
    WL_RATE_5DOT5MBPS,
    WL_RATE_11MBPS,
    WL_RATE_6MBPS,
    WL_RATE_9MBPS,
    WL_RATE_12MBPS,
    WL_RATE_18MBPS,
    WL_RATE_24MBPS,
    WL_RATE_36MBPS,
    WL_RATE_48MBPS,
    WL_RATE_54MBPS,

    // below are rate for 11n
    WL_NRATE_6DOT5MBPS,
    WL_NRATE_13MBPS,
    WL_NRATE_19DOT5MBPS,
    WL_NRATE_26MBPS,
    WL_NRATE_39MBPS,
    WL_NRATE_52MBPS,
    WL_NRATE_58DOT5MBPS,
    WL_NRATE_65MBPS,
    WL_NRATE_78MBPS,
    WL_NRATE_104MBPS,
    WL_NRATE_117MBPS,
    WL_NRATE_130MBPS,
    WL_NRATE_13MBPS_MCS8,
    WL_NRATE_26MBPS_MCS9,
    WL_NRATE_39MBPS_MCS10,
    WL_NRATE_52MBPS_MCS11
};
enum WL_MulticasRateEnum{
	LOWEST,
	UPPER
};
#define MAX_RATE WL_NRATE_130MBPS

enum WL_MacAddrAuthEnum{
          WL_MAC_AUTH_MODE_Disabled,
          WL_MAC_AUTH_MODE_Whitelist,
          WL_MAC_AUTH_MODE_Blacklist
} ;

typedef enum WL_CmdSetTypeEnum{
          WL_CMDSET_TYPE_CHANNEL,
          WL_CMDSET_TYPE_AUTHENC,
          WL_CMDSET_TYPE_RADIUS,
          WL_CMDSET_TYPE_MACAUTH
}EN_TWlCmdSetType;

#ifdef CONFIG_APPS_LOGIC_WPS
typedef enum WL_WpsAuthEnum{
	WPS_AUTHENTICATION_NONE,
    WPS_AUTHENTICATION_SHARED,
    WPS_AUTHENTICATION_BOTH,
    WPS_AUTHENTICATION_WPA_PSK,
    WPS_AUTHENTICATION_WPA_EAP,
    WPS_AUTHENTICATION_WPA2_PSK,
    WPS_AUTHENTICATION_WPA2_EAP,
    WPS_AUTHENTICATION_WPA_WPA2,
    WPS_AUTHENTICATION_UNKNOW
}EN_TWlWpsAuth;

typedef enum WL_WpsEncryEnum{
	WPS_ENCRYPTION_NONE,
    WPS_ENCRYPTION_WEP,
    WPS_ENCRYPTION_TKIP,
    WPS_ENCRYPTION_AES,
    WPS_ENCRYPTION_TKIP_AES,
    WPS_ENCRYPTION_UNKNOW
}EN_TWlWpsEncry;

#endif
/*************************************** some struct type definitions ****************************************************/
/* flag for tx power */
#define TXPOWER_FIXED               0x1
#define TXPOWER_RELATIVE            0x2
#define TXPOWER_DBM                 0x4
#define TXPOWER_MWATT               0x8

#define     PERCENT_TXPOWER_AUTO    -1

/* tx power limit for judge invalid data */
#define MAX_TXPOWER_DBM         50   /*dBm */

typedef struct
{
    char AutoChannel;
    //enum WL_CountryEnum CurrentCountry;
    char CurrentCountry[32];
    unsigned int DefaultChannel;
    char AP_enable;
	unsigned int PreambleType;
    char SSID[MAX_SSID_LEN];
    char HideSSID;
    unsigned int FragThreshold;
    char IntraBSSRelay;
    enum WL_ProfileEnum MODE_802_11_Profile;
    unsigned int RTSThreshold;
    enum WL_TransmitRateEnum TransmitRate;
	enum WL_MulticasRateEnum MulticastRate;
    unsigned int BeaconPeriod;
    unsigned int DTMPeriod;
    char     Support_11N;
    char     BW_11N;

    int tx_power_percent;  /*  percent of full TransmitPower */
    int max_tx_power;      /* in mW unit */


    /* Add by Shawn Wu */
//    char NeedCreate;
    unsigned int Mask;
    int MaskCount;

/* add by liwei*/
    char Qos_WMM;
    unsigned int Max_Sta;
    char DevName[MAX_DEVNAME_LEN];
    char AP_Isolate;
    char Power_Save;
    char XR;
/* Peter Meng add it for WDS at 2008-07-14*/
    char WDS_Enable;
    char OP_Mode;/*0=Root 1=WDS 2=Client 3=Bridge*/
    char Peer_MAC[MACADDR_LEN];
    unsigned int Root_Distance;
    unsigned int wds_index;

    char WPS_Enable;     //added by heym 20080815, enable or disable WPS
    char ControlSideBand;  // 2ndchoffset (20M:0  40M:1-lower 2-upper)
    char br[16];     //added by Peter.Meng  2008-09-17
}TWlChannelCfgParas;

typedef struct
{
    enum WL_AuthenticationEnum Authentication;
    enum WL_EncryptionEnum Encryption;
    char  BeaconType[15]; /* 对应于tr098中的beacon type 节点*/

    unsigned int WepDefaultTxKeyIdx;
    enum WL_WepEncryptionEnum WepEncryption;
    char Mode64Key[4][WEP64_KEY_LEN];
    char Mode128Key[4][WEP128_KEY_LEN];
    char WPAEnableWPA1;
    char WPAEnableWPA2;
    char WPA;
    char WPAEnablePSK;
    char WPAEnableEAP;
    char WPAEnableTKIP;
    char WPAEnableAES_CCMP;
    char WPA2EnableAES_CCMP;
    char WPA2EnableTKIP;
    char WPA2EnablePreauth;
    char WpaPsk[WPA_PSK_LEN];
    char WpaPassPhrase[WPA_PSK_LEN];
	char WpaPreShareKey[WPA_PSK_LEN];
    unsigned int WPAGroupRekey;
    char WPAStrictRekey;

    /* Add by Shawn Wu */
    unsigned int Mask;
    unsigned int MaskCount;
    enum WL_PSKExpression PSKExpression;

    char IAPPInterface[10];
    char OP_Mode;               //Peter Meng add it for WDS becasue WDS related two vap device
    int wds_index;              //Peter Meng add it for WDS becasue WDS related two vap device

}TWlAuthEncCfgParas;

typedef struct
{
    unsigned int  count;
    char MacListBuf[MAX_MACADDR_ITEMS][MACADDR_LEN];
}TMacAddrList;

typedef struct
{
    /* Add by Shawn Wu */
    unsigned int Mask;
    unsigned int MaskCount;
    char MacAddrCtlEnabled;

    enum WL_MacAddrAuthEnum  MacAddrAuth;
    char MacAddr[MACADDR_LEN];
    unsigned int MacCount;
    TMacAddrList MacAddrList;
}TWlMacAuthCfgParas;

typedef struct
{
    char StationName[MAX_SSID_LEN];
    char MacAddr[MACADDR_LEN];
    unsigned int TxRate;
    unsigned int RSSI;
    unsigned int state;
    char PointType[MAX_SSID_LEN];
}TStationInfo;

typedef struct
{
    unsigned int count;
    TStationInfo StationList[MAX_STA_NUM];
}TStationInfoList;

typedef struct
{
    char ipaddr[MAX_IPADDR_LEN];
    char secret[MAX_RADKEY_LEN];//128 hex length
    unsigned int   authport;
    unsigned int   acctport;
}TRadiusAttr;

typedef struct
{
    unsigned int count;
    TRadiusAttr RadList[MAX_RADIUS_SERVERS];
}TRadiusList;

typedef struct
{
    /* Add by Shawn Wu */
    unsigned int Mask;
    unsigned int MaskCount;

    char RadiusEnable;
    char RadiusAccEnable;
    char RadiusBakEnable;
    unsigned int AcctInterval;
    TRadiusAttr Radius;
    TRadiusAttr BakRadius;
//    TRadiusList RadiusList;
}TWlRadiusCfgParas;

#ifdef CONFIG_APPS_LOGIC_WPS
#define MAX_PIN_LEN 10
#define MAX_WPS_PARA_NUM 30
#define MAX_UUID_LEN 35
#define MAX_FILE_NAME  60
#define MAX_KEY_INDEX_LEN  4

/* support authentication */
#define AUTH_OPEN_CAPABILITY      0x0001
#define AUTH_WPAPSK_CAPABILITY    0x0002
#define AUTH_SHARED_CAPABILITY    0x0004
#define AUTH_WPA_CAPABILITY       0x0008
#define AUTH_WPA2_CAPABILITY      0x0010
#define AUTH_WPA2PSK_CAPABILITY   0x0020

/* support encryption */
#define ENCRYPT_NONE_CAPABILITY   0x0001
#define ENCRYPT_WEP_CAPABILITY    0x0002
#define ENCRYPT_TKIP_CAPABILITY   0x0004
#define ENCRYPT_AES_CAPABILITY    0x0008


#define	MASK_LOCAL_SSID           (1<<0) 
#define	MASK_LOCAL_PSK            (1<<1)
#define	MASK_REMOTE_SSID          (1<<2)
#define	MASK_REMOTE_PSK           (1<<3)
#define	MASK_REMOTE_AUTH          (1<<4)
#define	MASK_REMOTE_ENCRYP        (1<<5)
#define	MASK_REMOTE_KEY_INDEX     (1<<6)
#define MASK_REMOTE_WEP1          (1<<7)
#define MASK_REMOTE_WEP2          (1<<8)
#define MASK_REMOTE_WEP3          (1<<9)
#define MASK_REMOTE_WEP4          (1<<10)
#define	MASK_REMOTE_BENCON        (1<<11)

#define MASK_LOCAL_COMPLETE       (MASK_LOCAL_SSID | MASK_LOCAL_PSK)
#define MASK_REMOTE_COMPLETE      (MASK_REMOTE_SSID | MASK_REMOTE_PSK | MASK_REMOTE_AUTH | MASK_REMOTE_ENCRYP | \
	MASK_REMOTE_BENCON)

#define CONFIG_METHODS_USB 0x0001 //USBA (Flash Drive)
#define CONFIG_METHODS_ETH 0x0002 //Ethernet
#define CONFIG_METHODS_LABEL 0x0004 //Label
#define CONFIG_METHODS_DISPLAY 0x0008 //Display
#define CONFIG_METHODS_NFCE 0x0010 //External NFC Token
#define CONFIG_METHODS_NFCI 0x0020 //Integrated NFC Token
#define CONFIG_METHODS_NFC 0x0040 //NFC Interface
#define CONFIG_METHODS_PBC 0x0080 //PushButton
#define CONFIG_METHODS_KEYPAD 0x0100 //Keypad
typedef struct{
   char Enable;    //wps enable
   char upnpd_enable; //
char wps_cfg[MAX_FILE_NAME];   //config file
char fifo_name[MAX_FILE_NAME];   //for realtek ,need fifo name

   char SSID[MAX_SSID_LEN];       //此VAP SSID
   char NewDevName[MAX_DEVNAME_LEN];  //for new VAP name
   int  Authentication;           //此VAP 认证方式
   int  Encryption;               //此VAP 加密方式
   char WpaKey[WPA_PSK_LEN];      //WPAPSK
   int  WepKeyIndex;           //wep key index
   char WepKey[4][WEP128_KEY_LEN];
   char szWlanPath[MAX_PATH_LEN];   //启动WPS的WLAN实例的实例路径
   char UUID[MAX_UUID_LEN];

/* 缓存进程输出信息 */
char p_BenconType[MAX_NODE_VALUE_LEN];
char p_SSID[MAX_SSID_LEN];
char p_WpaPsk[WPA_PSK_LEN];
char p_WepKey[4][WEP128_KEY_LEN];
char p_KeyIndex[MAX_KEY_INDEX_LEN];
char p_Auth[MAX_NODE_VALUE_LEN];
char p_Encryp[MAX_NODE_VALUE_LEN];
   char LocalPin[MAX_PIN_LEN];               //设备pin
   char PeerPin[MAX_PIN_LEN];                //远端PIN
   int StartWpsSession;           //触发协商按钮，0:未触发  1:PBC  2:station PIN的PIN协商 3:AP PIN的PIN协商
   int GeneratePin;               //生成设备PIN按钮，0: 未触发 1:触发产生设备PIN

   int IsMatchCondition;              //是否符合启动WPS的条件，0:不符合， 1:符合
   int IsStartUp;                //WPS守护进程是否启动，0:未UP， 1:up
   int GenPinFlag;   //指示产生PIN码是否完成标志，防止产生PIN码的过程中再此点击产生PIN码的按钮
   int WpsSessionStatus;
int WpsStatus; //WPS Status，0:configured, 1:unconfigured 
int WpsRole;  //WPS Role:Registrar:0, Enrollee:1, Proxy:2

   unsigned int argc;  //启动守护进程参数个数
   char *argv[MAX_WPS_PARA_NUM];    //启动守护进程的参数
   unsigned char enSMCurState;
   unsigned long ulWpsId;
   char szModuleName[20];

	ST_TIMER_ENTRY TimerChangePIN;  //生成PIN码的定时器
	ST_TIMER_ENTRY TimerStartWPS;  //协商定时器
#ifdef CONFIG_APPS_LOGIC_WPS_LED_TELEFONICA
	ST_TIMER_ENTRY TimerWPSFail; 
#else
	ST_TIMER_ENTRY TimerClientLed;
#endif
	/* BIT0:PBC, BIT1:STA_PIN, BIT2:AP_PIN, BIT3:GENERATE PIN */
	unsigned long WpsRestartFlag;

	/* 生成PIN码操作，0:公共函数生成PIN，1:用方案提供的接口生成PIN */
	unsigned int WpsGenPinFlag;

	/* AUTH_OPEN=1, AUTH_WPAPSK=2, AUTH_SHARED=4, AUTH_WPA=8, AUTH_WPA2=0x10,
	  AUTH_WPA2PSK=0x20 */
	int auth_type_flags;
	/* ENCRYPT_NONE=1, ENCRYPT_WEP=2, ENCRYPT_TKIP=4, ENCRYPT_AES=8 */
	int encrypt_type_flags;
int restart_Wlan_Flag;

/* 厂商 */ 
char Manufacturer[64]; 
/* 序列号 */ 
char SerialNumber[64]; 
/* 型号 */
char szModelName[64];

/* WPS 版本 */ 
int version; //WPS VERSION 

int config_methods;

/* 操作的mask */ 
unsigned long F_Mask; //功能操作mask，指示需要进行哪些操作 
unsigned long P_Mask; //进程输出信息Mask，指示进程输出了哪些信息 
}TWlWpsCfgParas;

#endif

typedef struct
{
    char DriverNeedRestart;
    TWlChannelCfgParas ChannelCfgVal[MAX_NET_ID];
    TWlAuthEncCfgParas  AuthEncCfgVal[MAX_NET_ID];
    TWlMacAuthCfgParas MacAuthCfgVal[MAX_NET_ID];
    TWlRadiusCfgParas    RadiusCfgVal[MAX_NET_ID];
#ifdef CONFIG_APPS_LOGIC_WPS
	TWlWpsCfgParas    WpsCfgVal[MAX_NET_ID];
#endif
#ifdef CONFIG_APPS_LOGIC_WDS
    TWlWDSCfgParas	WdsConfig;
#endif

}TWlCfgParas;

typedef struct
{
    unsigned long long rx_bytes;
    unsigned long long rx_packets;
    unsigned int rx_errors;
    unsigned int rx_dropped;
    unsigned int rx_fifo_errors;
    unsigned int rx_frame_errors;
    unsigned int rx_compressed;
    unsigned int rx_multicast;

    unsigned long long tx_bytes;
    unsigned long long tx_packets;
    unsigned int tx_errors;
    unsigned int tx_dropped;
    unsigned int tx_fifo_errors;
    unsigned int collisions;
    unsigned int tx_carrier_errors;
    unsigned int tx_compressed;

	unsigned char BSSID[MACADDR_LEN];
}TWlStatisticParas;


#ifdef CONFIG_RTL8192CD

//#ifdef CONFIG_APPS_TRMODE_2T2R   bugid 44788 tr mode Automatic identification
#define BGN_RF_ENTRY 8
//#else
//#define BGN_RF_ENTRY 6
//#endif

struct RF_param
{
    char name[32];
    char defvalue[32];
};
#endif


typedef struct
{
    int   (*pfunc_install_module)(TWlCfgParas  *ptWlCfgParas);
    int   (*pfunc_api_init)(void);
    int   (*pfunc_create_vap)(TWlCfgParas  *ptWlCfgParas);
    int   (*pfunc_delete_vap)(int netif_id,TWlChannelCfgParas *ptWlChannelCfgVal);
    int   (*pfunc_basic_set)(int netif_id,TWlChannelCfgParas *ptWlChannelCfgVal,unsigned int cfg_mask, int mask_count);
    int   (*pfunc_private_set)(int netif_id,TWlChannelCfgParas *ptWlChannelCfgVal,unsigned int cfg_mask, int mask_count);
    int   (*pfunc_misc_set)(int netif_id,TWlChannelCfgParas *ptWlChannelCfgVal,unsigned int cfg_mask, int mask_count);
    int   (*pfunc_wep_set)(int netif_id, TWlAuthEncCfgParas  *ptWlAuthEncCfgVal,unsigned int cfg_mask,  int mask_count );
    int   (*pfunc_wpa_set)(int netif_id, TWlAuthEncCfgParas  *ptWlAuthEncCfgVal,unsigned int cfg_mask,  int mask_count );
    int   (*pfunc_wpa2_set)(int netif_id, TWlAuthEncCfgParas  *ptWlAuthEncCfgVal,unsigned int cfg_mask,  int mask_count );
    int   (*pfunc_roaming_set)(int netif_id, TWlAuthEncCfgParas  *ptWlAuthEncCfgVal,unsigned int cfg_mask,  int mask_count );
    int   (*pfunc_macfilter_set)(int netif_id, TWlMacAuthCfgParas  *ptWlMacAuthCfgVal,unsigned int cfg_mask,  int mask_count);
    int   (*pfunc_radius_set)(int netif_id, TWlRadiusCfgParas  *ptWlRadiusCfgVal, unsigned int cfg_mask,  int mask_count );
    int   (*pfunc_qos_set)(int netif_id,TWlChannelCfgParas *ptWlChannelCfgVal,unsigned int cfg_mask, int mask_count);
    int   (*pfunc_statistic_get)(int netif_id, TWlStatisticParas  *ptWlStatistic);
    int   (*pfunc_stations_get)(int netif_id,TStationInfoList *ptSationList);
    int   (*pfunc_para_is_global)(int iMask, EN_TWlCmdSetType enType);
    int   (*pfunc_need_restart)(int iMask, EN_TWlCmdSetType enType, int iRestartType);
    const char*   (*pfunc_netif_id_to_name)(int netif_id);
    int   (*pfunc_apply_cfg)(TWlCfgParas  *ptWlCfgParas);
    void (*pfunc_start_ap_daemon)(TWlCfgParas  *ptWlCfgParas);
    void (*pfunc_kill_ap_daemon)(void);
    void (*pfunc_enable_ap)(int ap_enable);
    int   (*pfunc_up_wlan_if)(TWlCfgParas  *ptWlCfgParas);
    int   (*pfunc_down_wlan_if)(TWlCfgParas  *ptWlCfgParas);
    int   (*pfunc_uninstall_module)(void);
    #ifdef CONFIG_APPS_LOGIC_WDS 	
    int   (*pfunc_wds_set)(TWlCfgParas *ptWlCfgParas);
	  #endif	
#ifdef CONFIG_APPS_LOGIC_WPS
	int   (*pfunc_create_config_file)(TWlWpsCfgParas *WpsCfgVal);
	int   (*pfunc_wps_init_paras)(TWlWpsCfgParas **WpsCfgVal);
	int   (*pfunc_trigger_pbc)(TWlWpsCfgParas *WpsCfgVal);
	int   (*pfunc_trigger_pin_local)(TWlWpsCfgParas *WpsCfgVal);
	int   (*pfunc_trigger_pin_peer)(TWlWpsCfgParas *WpsCfgVal);
	int   (*pfunc_generate_pin)(TWlWpsCfgParas *WpsCfgVal);
    int   (*pfunc_wps_stop_handle)(TWlWpsCfgParas *WpsCfgVal);
	int   (*pfunc_wps_after_start_handle)(TWlCfgParas * cfg_para);
#endif

}TWlApiFuncSet;

extern const char *ProfileEnumStrings[];
extern const char *AuthenticationEnumStrings[];
extern const char *EncryptionEnumStrings[];
extern const char *WepEncryptionEnumStrings[];
extern const char *TransmitRateEnumStrings[];
extern const char *MacAddressAuthEnumStrings[];
extern const char *CurrentCountryEnumStrings[];

// some convert functions
extern char * wlan_netif_id_to_name(int netif_id);

/******************************************* exported functions declaration *********************************************/
extern int wl_install_module(TWlCfgParas  *ptWlCfgParas);
extern int wl_uninstall_module(void);
extern int wl_init(TWlCfgParas * cfg_para);
extern int wl_create_vap(TWlCfgParas  *ptWlCfgParas);
extern int wl_delete_vap(int netif_id , void *ptVal);
extern int wl_basic_set(int netif_id , void *ptVal,unsigned int cfg_mask , int mask_count );
extern int wl_private_set(int netif_id , void *ptVal,unsigned int cfg_mask , int mask_count );
extern int wl_misc_set(int netif_id , void *ptVal,unsigned int cfg_mask , int mask_count );
extern int wl_qos_set(int netif_id , void *ptVal,unsigned int cfg_mask , int mask_count );
extern int wl_wep_set(int netif_id , void *ptVal,unsigned int cfg_mask , int mask_count );
extern int wl_wpa_set(int netif_id , void *ptVal,unsigned int cfg_mask , int mask_count );
extern int wl_wpa2_set(int netif_id , void *ptVal,unsigned int cfg_mask , int mask_count );
extern int wl_radius_set(int netif_id , void *ptVal,unsigned int cfg_mask , int mask_count );

extern int wl_macfilter_set(int netif_id , void *ptVal,unsigned int cfg_mask , int mask_count );
extern int wl_roaming_set(int netif_id, void *ptVal, unsigned int cfg_mask,  int mask_count );
extern int wl_statistic_get(int netif_id , void *ptVal );
extern int wl_stations_get(int netif_id , void *ptVal );
extern int wl_enable_ap(int ap_enable);
extern int wl_kill_ap_daemon(void);
extern int wl_start_ap_daemon(TWlCfgParas  *ptWlCfgParas);
extern int wl_up_wlan_if(TWlCfgParas  *ptWlCfgParas);
extern int wl_down_wlan_if(TWlCfgParas  *ptWlCfgParas);
extern int wl_apply_config(TWlCfgParas  *ptWlCfgParas);
extern int wl_commit(TWlCfgParas * cfg_para);
extern int wl_get_next_vapidx(void);
extern void wl_set_vapidx_flg(int vapidx);
extern void wl_reclaim_vapidx(int vapidx);
extern int wl_para_is_global(int cfg_mask, EN_TWlCmdSetType set_type);
extern int wl_need_restart(int cfg_mask, EN_TWlCmdSetType set_type, int restart_type);
extern const char * wl_netif_id_to_name(int netif_id);

extern void wl_register(void);
extern void wl_api_reg(TWlApiFuncSet * pstApiFunSet);

int wl_strtok_ex(char *szBuf,char *szDelim,int iMinLine,int iMaxLine,int iParaNum,...);
int wl_system_ex(const char *pszCmd,char * pszResult,int iCount);

int tbs_is_ifup(char *ifname);
int get_txpower_mW(const char *ifname, int* power);

#ifdef CONFIG_APPS_LOGIC_WPS
int wps_createConfigFile(TWlWpsCfgParas *WpsCfgVal);
int wps_commit(TWlWpsCfgParas *WpsCfgVal, int iCmd);
int wps_init_paras(TWlWpsCfgParas *WpsCfgVal);
extern void wps_api_reg(TWlApiFuncSet * pstApiFunSet);
void wps_register(void);
int wps_stop_handle(TWlWpsCfgParas *WpsCfgVal);
int wps_after_start_handle(TWlCfgParas * cfg_para);
#endif

#endif  // #ifndef _WIRELESS_API_H

