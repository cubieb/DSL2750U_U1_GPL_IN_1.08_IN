

#ifndef _RTL8192_WL_WPS_H_
#define _RTL8192_WL_WPS_H_

#define RTL_WPS_CONFIG_FILE  "/var/wscd.conf"

#define AUTH_OPEN       "1"
#define AUTH_WPAPSK     "2"
#define AUTH_SHARED     "4"
#define AUTH_WPA        "8"
#define AUTH_WPA2       "16"  //0x10
#define AUTH_WPA2PSK    "32"  //0x20
#define AUTH_WPA2PSKMIXED "34"  //0x22

#define ENCRYPT_NONE    "1"
#define ENCRYPT_WEP     "2"
#define ENCRYPT_TKIP    "4"
#define ENCRYPT_AES     "8"
#define ENCRYPT_TKIPAES  "12"


typedef struct tag_ST_WPS_AUTH
{
    int tbs_auth;
	char rtl_auth[30];
} ST_WPS_AUTH;


typedef struct tag_ST_WPS_ENCRYPT
{
    int tbs_encrypt;
	char rtl_encrypt[30];
} ST_WPS_ENCRYPT;

#endif

