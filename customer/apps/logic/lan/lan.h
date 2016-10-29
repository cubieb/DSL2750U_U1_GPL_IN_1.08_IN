#ifndef _LAN_H_
#define _LAN_H_

#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "common.h"
#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "warnlog.h"
#include "flash_layout.h"
#include "tbsipv6.h"

/*************************************************************************************************
                                                                   macro define
  ************************************************************************************************/

//#define LAN_DEBUG
/* 打印调试信息 */
#if LAN_DEBUG
#define LAN_TRACE(fmt, args...)   COMMON_TRACE(MID_LAN,fmt, ##args)
#else
#define LAN_TRACE(fmt, args...)
#endif

#define LAN_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_LAN, fmt, ##args)

#define LAN_RET_RETURN(iRet, strError, args...) RET_RETURN(MID_LAN, iRet, strError, ##args)
#define LAN_POINTER_RETURN(pRet, strError, args...) POINTER_RETURN(MID_LAN, pRet, strError, ##args)

#define LAN_RET_GOTO(iRet, strError, gotoTag) RET_GOTO(MID_LAN, iRet, strError, gotoTag)
#define LAN_POINTER_GOTO(pRet, gotoTag, strError, args...) POINTER_GOTO(MID_LAN, pRet, gotoTag, strError, ##args)

#define FOR_EACH_LIST(pos, list) for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)

/* 非标准节点名前缀*/
#define CUST_NODE_NAME_PREFIX "X_TWSZ-COM_"

/* TR069 节点名*/
#define NODE_IF_ENABLE "Enable"
#define NODE_IF_IP_ADDRESS "IPInterfaceIPAddress"
#define NODE_IF_SUBNET_MASK "IPInterfaceSubnetMask"
#define NODE_ALLOWED_MAC_ADDRESSES "AllowedMACAddresses"
#define NODE_IP_IF_NUM_OF_ENTRIES  "IPInterfaceNumberOfEntries"
#define NODE_LAN_ETH_IF_CFG "LANEthernetInterfaceConfig"
#define NODE_DELEGATED_CONNECTION "DelegatedConnection"

#define NODE_LAN_ETHIF_NUM_OF_ENTRIES   "LANEthernetInterfaceNumberOfEntries"
#define NODE_LAN_USBIF_NUM_OF_ENTRIES   "LANUSBInterfaceNumberOfEntries"
#define NODE_LAN_DEVICE_NUM_OF_ENTRIES "LANDeviceNumberOfEntries"
#define NODE_LAN_WLANCFG_NUM_OF_ENTRIES   "LANWLANConfigurationNumberOfEntries"
#define NODE_LAN_HOST_CFG_MANAGEMENT   "LANHostConfigManagement"
#define NODE_LAN_ETH_IF_CFG   "LANEthernetInterfaceConfig"
#define NODE_LAN_USB_IF_CFG   "LANUSBInterfaceConfig"
#define NODE_WLAN_CONFIGURATION   "WLANConfiguration"
#define NODE_HOST   "Hosts"
#define NODE_LAN_DEVICE_NAME "X_TWSZ-COM_DeviceName"
#define LAN_IPV6_IF_PATH_PREFIX "InternetGatewayDevice.LANDevice.%lu.LANHostConfigManagement.IPInterface.%lu."
#define LAN6_ADDR_SCOPE     CUST_NODE_NAME_PREFIX"IPv6InterfaceAddressScope"
#define LAN6_ADDR_STATUS    "Status"
#define LAN6_PRIFEXLEN      CUST_NODE_NAME_PREFIX"IPv6PrefixLength"
#define LAN6_ADDR           CUST_NODE_NAME_PREFIX"IPv6InterfaceAddress"
#define NODE_IPV6_INTERFACE_ADDRESS "X_TWSZ-COM_IPv6InterfaceAddress"

/* 非TR069 节点名*/
#define NODE_LAN_DEV_NAME CUST_NODE_NAME_PREFIX"DeviceName"
#define NODE_LAN_ETH_IF_NAME CUST_NODE_NAME_PREFIX"DeviceName"
#define NODE_LAN_DEV_PROXYARP   CUST_NODE_NAME_PREFIX"ProxyArp"
#define NODE_LAN_DEV_IPROUTERS "IPRouters"
#define NODE_LAN_DEV_SUBNETMASK "SubnetMask"
#define NODE_LAN_DEV_MINADDRESS "MinAddress"
#define NODE_LAN_DEV_MAXADDRESS "MaxAddress"
#define NODE_LAN_DEV_IPINTERFACE "IPInterface"


/* 生成路径的格式字符串*/
#define LAN_DEV_PATH_PREFIX "InternetGatewayDevice.LANDevice."
#define LAN_IP_IF_PATH_PREFIX "InternetGatewayDevice.LANDevice.%lu.LANHostConfigManagement.IPInterface."
#define LAN_DEV_PATH "InternetGatewayDevice.LANDevice.%lu.LANHostConfigManagement.%s"
#define LAN_IP_IF_PATH "InternetGatewayDevice.LANDevice.%lu.LANHostConfigManagement.IPInterface.%lu.%s"
#define LAN_DEVICE_NAME_PATH "InternetGatewayDevice.LANDevice.%lu."CUST_NODE_NAME_PREFIX"DeviceName"

#define PATH_LAN_CONN	"InternetGatewayDevice.LANDevice.%lu.X_TWSZ-COM_DeviceName"


#define MAX_LAN_DEV_NAME_LEN 30                         /* Lan 设备名最大长度*/
#define MAX_LAN_IF_NAME_LEN 30                            /* LAN 接口名最大长度*/
#define LAN_MAX_TYPE_LEN 15                                 /* 类型最大长度*/
#define LAN_MAX_NAME_LEN 30                                 /* 节点名最大长度*/
#define LAN_MAX_PATH_LEN 257                              /* 路径最大长度*/
#define LAN_MAX_CMD_LEN 256                                 /* 命令最大长度*/
#define LAN_MAX_IDX_COUNT 2                                 /* 节点路径中的实例号最大个数*/
#define MAX_INDEX_COUNT 2                                    /* 节点路径中的实例号最大个数*/
#define MAX_IP_IF_COUNT 4                                      /*接口数量限制*/
#define MAX_BR_NAME_LEN 100                                  /*桥设备名称最大长度*/
#define MAX_UNSIGNED_LEN 10                                 /* unsigned long 字符串长度 */


#define LAN_MAX_VALUE_LEN MAX_ALLOWED_MAC_ADDRESSES_LEN             /* 节点值最大长度*/
#define MAX_ALLOWED_MAC_ADDRESSES_LEN 361                                           /* 由于ebtable 命令的限制，暂时将最大白名单个数限制为20个*/
#define MAX_ALLOWED_MAC_COUNT (MAX_ALLOWED_MAC_ADDRESSES_LEN / MAX_MAC_LEN)   /* 最大允许MAC 地址个数*/

#define BOOL unsigned int

#define BIND    1
#define UNBIND  0
#define SUCCESS 0
#define FAILED  1


/*************************************************************************************************
                                                                   functions define
  ************************************************************************************************/
/*set functions*/
//CHECK_FUNC


int LAN_MsgAddBridgeHandler(ST_MSG * pstMsg);
int LAN_MsgDelBridgeHandler(ST_MSG * pstMsg);
int LAN_MsgBindPortHandler(ST_MSG * pstMsg);
int LAN_MsgUnbindPortHandler(ST_MSG * pstMsg);
int LAN_MsgDelNodeAckHandler(ST_MSG * pstMsg);
/*************************************************************************************************
                                                                   variables define
  ************************************************************************************************/
extern ST_HookEntry stLanSetHookTable[];
extern ST_HookEntry stLanCommitHookTable[];
extern ST_HookEntry stLanDeleteHookTable[];
extern ST_HookEntry s_astLanAddHookTable[];

int LAN_ModuleInit();
int LAN_ModuleDestroy();
#endif

