#ifndef _LANIP6_H_
#define _LANIP6_H_

#define CALL_FUCTION_FAILED "Call function \"%s\" failed\n"

#define PATH_LAN_CONN	"InternetGatewayDevice.LANDevice.%lu.X_TWSZ-COM_DeviceName"

#ifdef CONFIG_TBS_SUPPORT_IPV6
#define MAX_LAN_DEV_NAME_LEN 30
#define MAX_IPv6_LEN 40
#define IP6_IP6_ADDR_FIRST_BYTE 0xfd
#define MAX_IFID_LEN 8
#define MAX_STATION_INFO_LENGTH 8192

typedef struct notify {
    char type[8];
    char device[MAX_LAN_DEV_NAME_LEN];
    char address[MAX_IPv6_LEN];
    int prefixlen;
} ST_LAN_NOTIFY;

#define NODE_LAN_DEVICE_NAME "X_TWSZ-COM_DeviceName"
#define NODE_LAN_MAC_ADDRESS "X_TWSZ-COM_MacAddress"
#define NODE_IPV6_DELEGATED_MODE "Mode"
#define NODE_IPV6_DELEGATED_CONN "DelegatedConnection"
#define NODE_IPV6_DELEGATED_ADDR "IPv6InterfaceAddress"
#define NODE_IPV6_DELEGATED_PREFIX "IPv6PrefixAddress"
#define NODE_IPV6_DELEGATED_LENGTH "IPv6PrefixLength"

#define LAN_IPV6_DELEGATED_LENGTH "64"
#define LAN_IPV6_ADDRESS_DELEGATED "0"
#define LAN_IPV6_ADDRESS_STATIC "1"
#define LAN_IPV6_DELEGATED_WAN "WANDelegated"
#define LAN_IPV6_DELEGATED_TUNNEL "TunnelDelegated"

#define V6CONN_STAT_GLOBAL "GlobalConnected"
#define V6CONN_STAT_LINKLOCAL "LinkLocalConnected"
#define V6CONN_STAT_DISCONN "Disconnected"

#define MID_LAN_CONN    "InternetGatewayDevice.LANDevice.{i}.X_TELEFONICA-ES_IPv6LANHostConfigManagement."
#define MID_RADVD_PATH  "RadvdConfig"
#define MID_PATH_LAN_IPV6_INF "InternetGatewayDevice.LANDevice.{i}.X_TELEFONICA-ES_IPv6LANHostConfigManagement.IPv6Interface."
#define LAN_ADDR_LLA_PATH "InternetGatewayDevice.LANDevice.%.LANHostConfigManagement.IPInterface.1"
#define RADVD_CONN_NODE    "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IPv6LANHostConfigManagement\\.RadvdConfig\\."
#define RADVD_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.X_TELEFONICA-ES_IPv6LANHostConfigManagement\\.RadVDConfigManagement\\."
#define RADVD_PD_CONNECTION "IPv6PDWanConnection.Interface"
#define WAN_CONNECTION_PATH "InternetGatewayDevice.WANDevice.1.WANConnectionDevice."
#define WAN_IP_CONN_NUMBER "WANIPConnectionNumberOfEntries"
#define WAN_PPP_CONN_NUMBER "WANPPPConnectionNumberOfEntries"
#define WAN_IP_CONNECTION "WANIPConnection"
#define WAN_PPP_CONNECTION "WANPPPConnection"
#define WAN_CONNECTION_NAME "Name"
#define WAN_IPV6_ENABLE "X_TELEFONICA-ES_IPv6Enabled"
#define WAN_IPV6_CONN_STAT "X_TELEFONICA-ES_IPv6ConnStatus"
#define WAN_IPV6_SITE_PREFIX "X_TELEFONICA-ES_IPv6SitePrefix"
#define LAN_IPV6_PATH "InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement"

#define LAN_IPV6_FOREACH_PATH "InternetGatewayDevice.LANDevice.%."
#define LAN_IPV6_DELEGATED_PATH "LANHostConfigManagement.IPInterface.1.X_TELEFONICA-ES_IPv6LanIntfAddress.DelegatedAddress."
#define WAN_IPV6_SITE_PREFIX "IPv6SitePrefixInfo.IPv6SitePrefix"
#define LAN_IPV6_ADDRESS_TYPE "X_TELEFONICA-ES_IPv6InterfaceAddressingType.StaticDelegated"

/*
 * ipv6 enable/disable ½Úµã
 */
#ifdef CONFIG_APPS_LOGIC_IPV6_ENABLE
#define IPV6_ENABLE_NODE "InternetGatewayDevice.X_TWSZ-COM_IPv6Enable.Enable"
#define COMMIT_IPV6_SET_ENABLE "InternetGatewayDevice\\.X_TWSZ-COM_IPv6Enable\\.Enable"
#endif

int LAN_SendIPV6ADDRChgMsg(char *path);
int LAN_UA_IPv6Device(char *pszGlobalId, char *pszSubnetId, char *pszMacAddress, char *pszIpAddress);
int LAN_LLA_IPv6Device(char *pszCurNodePath, void *pData);
int LAN_GA_IPv6Device(char *pszCurNodePath, char *pszRadvdPrefix);
int LAN_ULA_IPv6addr(char *pszPath);
int LAN_SetIp6DelegatedInst(const char *pszIp6LanPath, const char *pszIp6WanPath);
int LAN_AddIp6DelegatedAddress(const char *pszPath, void *pData);
int LANIPV6_DelIp6DelegatedInst(const char *pszPath, void *pData);
int LANIPV6_CheckInterfaceAddress(const char *pszValue);
int LAN_CheckDelegatedConnection(const char *pszValue);
int LAN_ApplyLanip6Type(char *pszPath,char *pszValue);
int LAN_ApplyLanip6SubID(char *pszPath,char *pszValue);
int LAN_ApplyLanip6Un(char *pszPath,char *pszValue);
int LANIPV6_ApplyInterfaceAddress(const char *pszPath, const char *pszValue);
int LAN_ApplyDelegatedConnection(char *pszPath, char *pszValue);
int LAN_InitIp6Address(char *pszIp6Path, void *pData);
int LAN_MsgAddrLLHandler(ST_MSG *pstMsg);
int LAN_MsgAddrHandler(ST_MSG *pstMsg);
int LANIPV6_MsgPrefixHandler(ST_MSG *pstMsg);
#endif
#endif

