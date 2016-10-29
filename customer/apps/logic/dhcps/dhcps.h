/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: dhcps.h
 文件描述: 头文件
 修订记录:
        1. 作者: 李安楠
           日期: 
           内容: 创建文件
**********************************************************************/

#ifndef _DHCPS_H_
#define _DHCPS_H_

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/* 日志打印宏 */

#ifdef DHCPS_DEBUG
#define DHCPS_TRACE(fmt, args...)        TBS_TRACE(MID_DHCPS, fmt "\n", ##args)
#else
#define DHCPS_TRACE(fmt, args...)
#endif
#ifdef CONFIG_APPS_LOGIC_TF_GUI
typedef struct tag_ST_MAC_PORT{
	char pszPortName[7];
	char pszMacValue[18];
   // char pszStatus[4];
}ST_MAC_PORT;
#endif
#define DHCPS_TRACE_SUCCESS(fmt, args...) DHCPS_TRACE(GREEN INFO_SUCCESS NONE fmt, ##args)
#define DHCPS_TRACE_ERROR(fmt, args...) DHCPS_TRACE(LIGHT_RED INFO_ERROR NONE fmt, ##args)

/* 非标准节点名前缀*/
#define CUST_NODE_NAME_PREFIX "X_TWSZ-COM_"

/* DHCPS 的TR069 节点名*/
#define NODE_DHCP_SERVER_ENABLE "DHCPServerEnable"
#define NODE_MIN_ADDRESS "MinAddress"
#define NODE_MAX_ADDRESS "MaxAddress"
#define NODE_SUBNET_MASK "SubnetMask"
#define NODE_DNS_SERVERS "DNSServers"
#define NODE_DOMAIN_NAME "DomainName"
#define NODE_IP_ROUTERS "IPRouters"
#define NODE_DHCP_LEASE_TIME "DHCPLeaseTime"
#define NODE_IP_IF "IPInterface"
//add by zhdzhu 2010.6.19
#define NODE_OPTION_ENABLE	"Enable"
#define NODE_OPTION_TAG	"Tag"
#define NODE_OPTION_VALUE	"Value"

#define NODE_DHCP_RELAY "DHCPRelay"

/* DHCPS 的非TR069 节点名*/
#define NODE_DNS_RELAY_ENABLE CUST_NODE_NAME_PREFIX"DNSRelayEnable"

//modify by zhdzhu 2010.6.18
//#define NODE_CLIENT_CLASSES CUST_NODE_NAME_PREFIX"ClientClasses"
#define NODE_CLIENT_CLASSES "DHCPConditionalServingPool"
#define NODE_CLIENT_OPTIONS "DHCPOption"
#define NODE_USE_ALLOCATED_WAN  "UseAllocatedWAN"

//add by lbw
#define NODE_POOLORDER  "PoolOrder"
#define NODE_SOURCEINTERFACE  "SourceInterface"
#define NODE_VENCLIDEXCL  "VendorClassIDExclude"
#define NODE_VENCLIDMODE  "VendorClassIDMode"
#define NODE_CLIENTID  "ClientID"
#define NODE_CHADDR  "Chaddr"
#define NODE_LOCALSERVED  "LocallyServed"
#define NODE_RESERVEDADDRESS "ReservedAddresses"
//#define NODE_SUBNETMASK "SubnetMask"
#define NODE_IPROUTERS "IPRouters"
#define NODE_POOL_MIN_ADDRESS "MinAddress"
#define NODE_POOL_MAX_ADDRESS "MaxAddress"

//modify by zhdzhu 2010.6.18
//#define NODE_CLIENT_CLASSES_DNS "Dns"
//#define NODE_CLASS_ID "ClassIdentifier"
#define NODE_CLIENT_CLASSES_DNS "DNSServers"
#define NODE_CLASS_ID "VendorClassID"


/* HOST 的TR069 节点名*/

#define NODE_IP_ADDRESS "IPAddress"
#define NODE_ADDRESS_SOURCE "AddressSource"
#define NODE_LEASE_TIME_REMAINING "LeaseTimeRemaining"
#define NODE_MAC_ADDRESS "MACAddress"
#define NODE_HOST_NAME "HostName"
#define NODE_HOST_COUNT "HostNumberOfEntries"
#define NODE_HOST_CM "LANHostConfigManagement"
#define NODE_LAYER2_INTERFACE "Layer2Interface"
#define NODE_INTERFACE_TYPE "InterfaceType"
#define NODE_ACTIVE "Active"

/* HOST 的非TR069 节点名*/
#define NODE_LEASE_EXPIRE_TIME CUST_NODE_NAME_PREFIX"LeaseTimeExpire"
//modify bu zhdzhu 2010.6.19
//#define NODE_CUST_CLASS_ID CUST_NODE_NAME_PREFIX"ClassIdentifier"
#define NODE_CUST_CLASS_ID		"VendorClassID"

/* LAN 端IP 接口 的TR069 节点名*/
#define NODE_IF_ENABLE "Enable"
#define NODE_IF_IP_ADDRESS "IPInterfaceIPAddress"
#define NODE_IF_SUBNET_MASK "IPInterfaceSubnetMask"


/* LAN 设备的非TR069 节点名*/
#define NODE_LAN_DEVICE_NAME CUST_NODE_NAME_PREFIX"DeviceName"
#define NODE_UseIPRoutersAsDNSServer    "X_TWSZ-COM_UseIPRoutersAsDNSServer"
#define NODE_WANDNSServers     "X_TWSZ-COM_WANDNSServers"

/* 生成路径的格式字符串*/
#define LAN_DEV_PATH_PREFIX "InternetGatewayDevice.LANDevice."
#define LAN_HOST_PATH_PREFIX "InternetGatewayDevice.LANDevice.%lu.Hosts.Host."
#define LAN_HOSTS_PATH "InternetGatewayDevice.LANDevice.%.Hosts."
#define LAN_HOST_PATH "InternetGatewayDevice.LANDevice.%.Hosts.Host.%."
//modify by zhdzhu 2010.6.18
//#define DHCPS_CLIENT_CLS_PATH_PREFIX "InternetGatewayDevice.LANDevice.%lu.LANHostConfigManagement.X_TWSZ-COM_ClientClasses."
#define DHCPS_CLIENT_CLS_PATH_PREFIX "InternetGatewayDevice.LANDevice.%lu.LANHostConfigManagement.DHCPConditionalServingPool."
//#define DHCPS_CLIENT_CLS_OBJ_PATH "InternetGatewayDevice.LANDevice.%.LANHostConfigManagement.X_TWSZ-COM_ClientClasses.%."
#define DHCPS_CLIENT_CLS_OBJ_PATH "InternetGatewayDevice.LANDevice.%.LANHostConfigManagement.DHCPConditionalServingPool.%"

#define DHCPS_CLIENT_CLS_OPT_PATH_PREFIX "InternetGatewayDevice.LANDevice.%lu.LANHostConfigManagement.DHCPOption."
#define DHCPS_CLIENT_CLS_OPT_OBJ_PATH "InternetGatewayDevice.LANDevice.%.LANHostConfigManagement.DHCPOption.%"

#define DHCPS_CLIENT_CLS_COND_OPT_PREFIX "InternetGatewayDevice.LANDevice.%.LANHostConfigManagement.DHCPConditionalServingPool.%.DHCPOption."
#define DHCPS_CLIENT_CLS_COND_OBJ_PATH "InternetGatewayDevice.LANDevice.%.LANHostConfigManagement.DHCPConditionalServingPool.%.DHCPOption.%"

//modify by zhdzhu 2010.6.18
//#define NODE_CLS_COUNT "X_TWSZ-COM_ClientClassNumberofEntries"
#define NODE_CLS_COUNT	"DHCPConditionalPoolNumberOfEntries"
#define NODE_CLS_OPTION_COUNT	"DHCPOptionNumberOfEntries"

#define DHCPS_IPPOOLSIZE "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.ClientIPPoolSize"
#define DHCPS_MAXIP "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.MaxAddress"
#define DHCPS_MINIP "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.MinAddress"

#define DHCPS_MAX_IP_MASK_LEN 16

#define MAX_LEN_DNS 48

/* 定时器时间间隔 */
#define DHCPS_TIME_INTERVAL  60
static unsigned long g_ulTimerID = 0;     //定时器ID
#define RELEASE_NUM  5
#define RELEASE_TIME_LIMIT  240

/* 文件名最大长度*/
#define DHCPS_MAX_FILE_NAME_LEN 20

#define MAX_DOMAIN_NAME_LEN 65

#define MAX_DNS_SERVER_COUNT 3

/* 地址源最大长度 */
#define MAX_ADDR_SRC_LEN 20

/* 主机名最大长度 */
#define MAX_HOST_NAME_LEN 65

/* 最大客户类型个数*/
#define MAX_CLIENT_CLS_COUNT 16

/* 客户类型名最大长度 */
#define MAX_CLASS_ID_LEN 30

//add by zhdzhu 2010 6.22 for option value
#define MAX_OPTION_VAL_LEN 340

/* 配置文件可保存最多主机数目 */
#define DHCPS_MAX_HOST_NUM 10

#ifdef ENABLE_OPT125
#define MAX_OUI_SUBOPT_LEN 7
#define MAX_SN_SUBOPT_LEN 65
#define MAX_PC_SUBOPT_LEN 65

#define DEF_OPT125_SUBOPT_VALUE "-"
#endif

/* LanDevice 下唯一的IP 接口的Index */
#define LAN_IF_FIXED_IDX (1ul)

/* 主机表中动态主机的最小Index */
#define DHCPS_DYN_HOST_MIN_INDEX     (256ul)

/* 主机地址源类型*/
#define ADDR_SRC_DHCP "DHCP"
#define ADDR_SRC_STATIC "Static"

#define ADDR_SRC_AUTOIP "AutoIP"

/* 主机接口类型*/
#define IF_TYPE_ETHERNET "Ethernet"
#define IF_TYPE_USB "USB"
#define IF_TYPE_80211 "802.11"
#define IF_TYPE_HOMEPNA "HomePNA"
#define IF_TYPE_HOMEPLUG "HomePlug"
#define IF_TYPE_OTHER "Other"


/* 配置文件名 */
#define DHCPS_CONFIG_FILE "/var/udhcpd.conf"

/* 保存lease 文件的路径 */
#define DHCPS_LEASE_DIR "/var/"

#ifdef ENABLE_OPT125
/* 保存选项125 字段值的配置文件 */
#define DHCPS_VENDOR_FILE "/var/vendor"
#endif

/* 无限租约时间 */
#define INF_LEASE_TIME     604800
#define INF_LEASE_TIME_STR "4294967295"

/* 缺省MAC 地址 */
#define DEFAULT_HOST_MAC_ADDR "00:00:00:00:00:00"

/* 缺省客户类型*/
#define DEFAULT_CLASS_ID    "Computer"

/* 无效客户类型*/
#define INVALID_CLASS_ID    "-"

/* 缺省主机名 */
#define DEFAULT_HOST_NAME    "unknown"


#define LAN_DEV_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.$"
#define DHCPS_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.LANHostConfigManagement"
//#define IP_IF_PATH_REGEX DHCPS_PATH_REGEX"\\.""IPInterface\\.1"

//modify by zhdzhu 2010.6.18
//#define CLS_ADD_PATH_REGEX DHCPS_PATH_REGEX"\\.""X_TWSZ-COM_ClientClasses\\.$"
//#define CLS_DEL_PATH_REGEX DHCPS_PATH_REGEX"\\.""X_TWSZ-COM_ClientClasses\\.[0-9]+\\.$"
#define CLS_ADD_PATH_REGEX DHCPS_PATH_REGEX"\\.""DHCPConditionalServingPool\\.$"
#define CLS_DEL_PATH_REGEX DHCPS_PATH_REGEX"\\.""DHCPConditionalServingPool\\.[0-9]+\\.$"

#define CLS_OPT_ADD_PATH_REGEX DHCPS_PATH_REGEX"\\.""DHCPOption\\.$"
#define CLS_OPT_DEL_PATH_REGEX DHCPS_PATH_REGEX"\\.""DHCPOption\\.[0-9]+\\.$"

#define CLS_COND_OPT_ADD_PATH_REGEX DHCPS_PATH_REGEX"\\.""DHCPConditionalServingPool\\.[0-9]+\\.DHCPOption\\.$"
#define CLS_COND_OPT_DEL_PATH_REGEX DHCPS_PATH_REGEX"\\.""DHCPConditionalServingPool\\.[0-9]+\\.DHCPOption\\.[0-9]+\\.$"

#if 0
#define CLIENT_CLASS_PATH_REGEX DHCPS_PATH_REGEX"\\.""X_TWSZ-COM_ClientClasses\\.[0-9]+"


#define DHCPS_ENABLE_REGEX DHCPS_PATH_REGEX"\\."NODE_DHCP_SERVER_ENABLE"$"
#define DHCPS_MIN_ADDRESS_REGEX DHCPS_PATH_REGEX"\\."NODE_MIN_ADDRESS"$"
#define DHCPS_MAX_ADDRESS_REGEX DHCPS_PATH_REGEX"\\."NODE_MAX_ADDRESS"$"
#define DHCPS_DOMAIN_NAME_REGEX DHCPS_PATH_REGEX"\\."NODE_DOMAIN_NAME"$"
#define DHCPS_LEASE_TIME_REGEX DHCPS_PATH_REGEX"\\."NODE_DHCP_LEASE_TIME"$"

#define IF_ENABLE_REGEX IP_IF_PATH_REGEX"\\."NODE_IF_ENABLE"$"
#define IF_IP_ADDRESS_REGEX IP_IF_PATH_REGEX"\\."NODE_IF_IP_ADDRESS"$"
#define IF_IP_SUBNET_MASK_REGEX IP_IF_PATH_REGEX"\\."NODE_IF_SUBNET_MASK"$"

#define CLIENT_CLASS_ID_REGEX CLIENT_CLASS_PATH_REGEX"\\."NODE_CLASS_ID"$"
#define CLIENT_CLASS_MIN_ADDR_REGEX CLIENT_CLASS_PATH_REGEX"\\."NODE_MIN_ADDRESS"$"
#define CLIENT_CLASS_MAX_ADDR_REGEX CLIENT_CLASS_PATH_REGEX"\\."NODE_MAX_ADDRESS"$"
#endif

#define	SUPPORT_DHCPS_FILTER	//radar

#ifdef	SUPPORT_DHCPS_FILTER
#define	NODE_DHCP_FILTER	"X_TWSZ-COM_DHCPFilter"
#define	NODE_DHCP_FILTER_ENABLED	"Enabled"
#define	NODE_DHCP_FILTER_WHITELST	"WhiteList"
#define	NODE_DHCP_FILTER_BLACKLST	"BlackList"
/* InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.X_TWSZ-COM_DHCPFilter.WhiteList */
#define   LAN_DHCPWHITELISTFILTER_PATH   "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DHCPFilter.WhiteList."
#define	NODE_FILTER_IP	"FilterIP"
#define	NODE_FILTER_MAC	"FilterMAC"
#define   NODE_FILTER_EN       "PerEnabled"
#define   NODE_FILTER_PCname       "FilterPCName"
#endif

extern int remove(const char *pathname);

/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/

/* 表示一个主机信息*/
typedef struct tag_ST_DHCPS_HostInfo {
    char *pszIpAddr;
    char *pszAddrSrc;
    char *pszLeaseTime;
    char *pszExpireTime;
    char *pszMacAddr;
    char *pszClassId;
    char *pszHostName;
    char *pszL2Ifc;
    char *pszIfType;
    char *pszActive;
}ST_DHCPS_HostInfo;

/* 记录release的主机信息 */
typedef struct tag_ST_DHCPS_ReleaseInfo {
    char szIpAddr[16];
    char szMacAddr[18];
    int iReleaseTime;
}ST_DHCPS_ReleasetInfo;


#endif

/******************************************************************************
*                                 END                                        *
******************************************************************************/

