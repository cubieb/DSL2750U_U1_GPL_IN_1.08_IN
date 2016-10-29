/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: wan.h
 文件描述: WAN适配模块头文件

 修订记录:
        1. 作者: 朱健文
           日期: 2008-12-18
           内容: 创建文件

        2. 作者: 徐晓东
           日期: 2009-08-08
           内容: 根据realtek项目的需求重构代码,合并WAN和ATM模块
**********************************************************************/
#ifndef _WAN_H_
#define _WAN_H_

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/

#ifdef WAN_DEBUG
    #define WAN_TRACE(fmt, args...) COMMON_TRACE(MID_WAN, fmt"\n", ##args)
    #define WAN_LOG_ERR(fmt, args...) COMMON_TRACE(MID_WAN, fmt, ##args)
#else
    #define WAN_TRACE(fmt, args...) 
    #define WAN_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_WAN, fmt, ##args)
#endif

#define WAN_TRACE_INTO_FUNC WAN_TRACE("##In## %s\n", __func__)

#define WAN_ASSERT(expr) \
    do{ \
        if(!(expr))\
        {\
            WAN_TRACE("Assert \"%s\" failed\n", #expr); \
            exit(-1); \
        } \
    }while(0)
    
/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define WAN_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            WAN_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define WAN_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_WAN, Ret, fmt, ##args)

#define WAN_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_WAN, Pointer, gotoTag, fmt, ##args)

#define WAN_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            WAN_TRACE(fmt, ##args); \
            return Ret; \
        }

#define WAN_CHECK_NULL_PTR(ptr)   WAN_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

#define WAN_OK  TBS_SUCCESS

#define WAN_CHECK_STR_VAL(pszValue, apszList) \
{ \
    int i = 0, iSize = sizeof(apszList)/sizeof(apszList[0]); \
    for (i = 0; i < iSize; i++) \
    { \
        if (0 == strcmp(pszValue, apszList[i])) \
        { \
            return TBS_SUCCESS; \
        } \
    } \
}

#define RFCNT_INCREASE(pstLinkDev)   (pstLinkDev->uiRefCount)++
#define RFCNT_DECREASE(pstLinkDev)   (pstLinkDev->uiRefCount)--

#define WAN_ACCESS_TYPE(pstLinkDev) (pstLinkDev->pstWanDev->ucAccessType)
#define WAN_DSL_LINK_PARA(Para) (pstLinkDev->unLink.stDslLink.Para)
#define WAN_ETH_LINK_PARA(Para) (pstLinkDev->unLink.stEthLink.Para)

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define WAN_NODE_ENABLE      "Enable"
#define WAN_NODE_CONN_TYPE   "ConnectionType"
#define WAN_NODE_DEV_NAME    "X_TWSZ-COM_DeviceName"
#define WAN_NODE_MAC     "X_TWSZ-COM_MACAddress"
#define WAN_NODE_LAN_INTF    "X_TWSZ-COM_LanInterface"
#define WAN_NODE_SVC_LIST    "X_TWSZ-COM_ServiceList"
#define WAN_NODE_VLAN_ID     "X_TWSZ-COM_VLANID"
#define WAN_NODE_VLAN_PRIO   "X_TWSZ-COM_VLANPriority"
#define WAN_NODE_QINQ_ENABLE "X_TWSZ-COM_QinQEnable"
#define WAN_NODE_FIREWALL_ENABLE "X_TWSZ-COM_FirewallEnable"
#define WAN_NODE_NAT_MODE    "X_TELEFONICA-ES_MultiNATMode"
#define WAN_NODE_PPPOE_NAME  "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username"
#define WAN_NODE_WAN_CHANG_FLAG "InternetGatewayDevice.X_TWSZ-CaptivePortal.X_TWSZ-COM_WanFlagFirst"

#define WAN_NODE_ENABLED_CONN_NUM  "TWSZ-COM_NumberOfEnabledConnections"
#define WAN_NODE_ACTCONN_NUM       "NumberOfActiveConnections"
#define WAN_NODE_ACTCONN_DEV_PATH  "ActiveConnectionDeviceContainer"
#define WAN_NODE_ACTCONN_PATH      "ActiveConnectionServiceID"
#define WAN_NODE_ACCTIVE_RFE       "ActiveConnectionRef"
#define WAN_NODE_3GEnable          "3GConnectionEnable"

#define WAN_NODE_IPv4Enable        "X_TELEFONICA-ES_IPv4Enabled"
#define WAN_NODE_IPv6Enable        "X_TELEFONICA-ES_IPv6Enabled"


#define NODE_PROTO_TYPE  "X_TWSZ-COM_ProtocolType"
#define NODE_CONN_STATUS  "ConnectionStatus"
#define NODE_CONN_DEV_NAME  "X_TWSZ-COM_ConnectedDeviceName"
#define NODE_DSL_LINK    "WANDSLLinkConfig"
#define NODE_ATM_ENABLE  "Enable"
#define NODE_LINK_STATUS "LinkStatus"
#define NODE_LINK_TYPE   "LinkType"
#define NODE_DEST_ADDR   "DestinationAddress"
#define NODE_ATM_ENCAP   "ATMEncapsulation"
#define NODE_ATM_QOS     "ATMQoS"
#define NODE_PEAK_CELL_RATE "ATMPeakCellRate"
#define NODE_MAX_BURST_SIZE "ATMMaximumBurstSize"
#define NODE_SUS_CELL_RATE  "ATMSustainableCellRate"
#define NODE_MODULATION_TYPE "ModulationType"

#define WAN_CONN_TAB_PATH   "InternetGatewayDevice.WANDevice.{i}.WANCommonInterfaceConfig.Connection."

#define WAN_PATH_PREFIX_REGEX    "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\."
#define WAN_COMM_IF_PATH_REGEX   "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANCommonInterfaceConfig\\."
#define WAN_CONN_DEV_PATH_REGEX  "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\." 
#define WAN_IP_CONN_PATH_REGEX   WAN_CONN_DEV_PATH_REGEX "WANIPConnection\\.[0-9]+\\."
#define WAN_PPP_CONN_PATH_REGEX  WAN_CONN_DEV_PATH_REGEX "WANPPPConnection\\.[0-9]+\\."
#define WAN_IP_PPP_CONN_PATH_REGEX WAN_CONN_DEV_PATH_REGEX "WAN(IP|PPP)Connection\\.[0-9]+\\."
#define WAN_DSL_LINK_PATH_REGEX  WAN_CONN_DEV_PATH_REGEX "WANDSLLinkConfig|WANEthernetLinkConfig\\."
#define WAN_CONN_DEV_PATH_3G_REGEX  "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.X_TWSZ-COM_WAN3GInterfaceConfig"
#define WAN_CONN_DEV_NODE_REGEX(node) WAN_CONN_DEV_PATH_REGEX node "$"
#define WAN_DSL_LINK_NODE_REGEX(node) WAN_CONN_DEV_PATH_REGEX "WANDSLLinkConfig\\." node "$"
#define WAN_IP_CONN_NODE_REGEX(node) WAN_IP_CONN_PATH_REGEX node "$"
#define WAN_PPP_CONN_NODE_REGEX(node) WAN_PPP_CONN_PATH_REGEX node "$" 
#define WAN_IP_PPP_CONN_NODE_REGEX(node) WAN_IP_PPP_CONN_PATH_REGEX node "$"
#define WAN_NAT_MODE_NODE_REGEX(node) WAN_COMM_IF_PATH_REGEX node "$"

#define WAN_ADD_CONN_DEV_REGEX  "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.$"

#define WAN_COMMON_STATUS "InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig."

#define WAN_COMM_IF_PATH1_REGEX "^InternetGatewayDevice\\.WANDevice\\.1+\\.WANCommonInterfaceConfig\\.$"

#define WAN_IPV6_FORWARDING "InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.IPv6Enable"
#ifdef    CONFIG_APPS_LOGIC_ETHWAN
#define WAN_CONN_ADSL_DEV_PATH_REGEX  "^InternetGatewayDevice\\.WANDevice\\.[0-2]|[4-9]+\\.WANConnectionDevice\\.[0-9]+\\." 
#define WAN_CONN_ETH_DEV_PATH_REGEX  "^InternetGatewayDevice\\.WANDevice.3\\.WANConnectionDevice\\.[0-9]+\\." 

#define WAN_ADSL_LINK_PATH_REGEX(node) WAN_CONN_ADSL_DEV_PATH_REGEX "WANDSLLinkConfig\\." node "$"
#define WAN_ETH_LINK_PATH_REGEX(node) WAN_CONN_ETH_DEV_PATH_REGEX "WANEthernetLinkConfig\\." node "$"
#endif
#define WAN_WAN_PATH        "InternetGatewayDevice.WANDevice."
#define MAX_WAN_CONNECTONS_DEF 8
#define WAN_NUM_OF_ENTRIES  "WANConnectionNumberOfEntries"

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define WAN_ARR_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))

#define FIND_KEY_WAN_DEV   1
#define FIND_KEY_LINK_DEV  2
#define FIND_KEY_VLAN_DEV  3

#define FLAG_ENABLE_CHG     0x00000001
#define FLAG_PVC_CHG        0x00000010
#define FLAG_LINK_TYPE_CHG  0x00000100
#define FLAG_OTHER_PARA_CHG 0x00001000

#define ATM_ENABLE_CHG(ChangeFlag)    ((ChangeFlag) & FLAG_ENABLE_CHG)
#define ATM_PVC_CHG(ChangeFlag)       ((ChangeFlag) & FLAG_PVC_CHG)
#define ATM_LINK_TYPE_CHG(ChangeFlag) ((ChangeFlag) & FLAG_LINK_TYPE_CHG)
#define ATM_OTER_PARA_CHG(ChangeFlag) ((ChangeFlag) & FLAG_OTHER_PARA_CHG)

#define WAN_ACCESS_TYPE_ETH "Ethernet"
#define WAN_ACCESS_TYPE_DSL "DSL"
/******************************************************************************
 *                                   ENUM                                     *
 ******************************************************************************/
enum {
    ATM_QOS_UBR = 0,
    ATM_QOS_CBR,
    ATM_QOS_VBR_NRT,
    ATM_QOS_VBR_RT,
    ATM_QOS_UBR_PLUS,
    ATM_QOS_UNKNOWN
};

enum {
    ATM_ENCAP_VC     =0,
    ATM_ENCAP_LLC    =1,
    ATM_ENCAP_VC_IP  =3,
    ATM_ENCAP_LLC_IP =4,
    ATM_ENCAP_UNKNOWN    
};

typedef enum {
    INTERNET_LED_OFF,
    INTERNET_RED_ON,
    INTERNET_GREEN_ON
} st_INTERNET_STATUS;

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
typedef CFG_RET (*FUNC_REG_MID)(const char *pcPath, unsigned short usMID);
typedef struct
{
    const char *pcLeaf;
    FUNC_REG_MID pfnRegFunc;
    unsigned char ucIp;
    unsigned char ucPpp;
    unsigned char aucSpace[2];
} ST_WAN_MID_NODE;

typedef struct tag_ST_WAN_DEV_INST {
    unsigned long ulWanDevIdx;
    unsigned char ucAccessType;
    unsigned char ucStatus; /*the status of wan phy interface, value TRUE means the 
                              interface is connected, and value FALSE means the int-
                              erface is disconnected*/
    unsigned short usLinkDevBitMap;  /*up to 16 link devs*/
    TBS_ST_LIST_HEAD lhLinkDevs;
} ST_WAN_DEV_INST;

/*if the access type is ethernet, there is only one
  link(ether) device instance for one wan device*/
typedef struct tag_ST_ETH_LINK_INST {
    char szDevName[MAX_IF_NAME_LEN];
    unsigned char ucLinkStatus;
    unsigned char ucPad[3];
} ST_ETH_LINK_INST; 

/*if the access type is dsl(adsl), there are more 
  than one link(pvc) device instance for one wan device*/
typedef struct tag_ST_DSL_LINK_INST {   
    unsigned char ucEnable;
    unsigned char ucLinkStatus;
    unsigned char ucLinkType;
    unsigned char ucVpi;
    unsigned short usVci;
    unsigned char ucEncap;
    unsigned char ucQosType;
    unsigned int  uiPcr;
    unsigned int  uiScr;
    unsigned int  uiMbs;
    unsigned long ulWanConnDevIdx; /*for IPOA and PPPOA*/    
} ST_DSL_LINK_INST;

typedef struct tag_ST_PVC{
    unsigned short usVpi;
    unsigned short usVci;
} ST_PVC;

/*eth and pvc are two different link device*/
typedef struct tag_ST_LINK_DEV_INST {
    unsigned char ucStatus; /*the status of link device(value TRUE means the link device has 
                              already be configured, FALSE means not be configured)*/
    unsigned char ucDevId;  /*now only for dsl link dev use*/
    unsigned char ucPad[2];
    unsigned int uiRefCount;
    union {
        ST_ETH_LINK_INST stEthLink;    
        ST_DSL_LINK_INST stDslLink;
    } unLink;    
    ST_WAN_DEV_INST *pstWanDev;
    TBS_ST_LIST_HEAD lhVlanDevs;
    TBS_ST_LIST_HEAD lhLinkDevs; 
} ST_LINK_DEV_INST;

typedef struct tag_ST_VLAN_DEV_INST {
    unsigned char ucStatus; /*the status of vlan virtual device*/    
    unsigned char ucVlanPrio;
    unsigned short usVlanId;
    unsigned char ucQinQEnable;
    ST_LINK_DEV_INST *pstLinkDev;
    TBS_ST_LIST_HEAD lhVlanDevs;
    TBS_ST_LIST_HEAD lhVnetDevs;
} ST_VLAN_DEV_INST; /*for adsl EoA or Ehternet*/

typedef struct tag_ST_VNET_DEV_INST {
    BOOL bStatus; /*the status of vnet virtual device*/
    unsigned long ulWanConnDevIdx;
    struct tag_ST_VLAN_DEV_INST *pstVlanDev;
    TBS_ST_LIST_HEAD lhVnetDevs;
} ST_VNET_DEV_INST; /*for adsl EoA or Ethernet*/

typedef struct tag_ST_WAN_SPI_INFO {
	char ucWANDevName[64];
	BOOL bFirewallEnabled;
	BOOL bIPv4Enabled;	
	BOOL bIPv6Enabled;
} ST_WAN_SPI_INFO; /*for wan SPI info*/


#define LED_DSL   1
#define LED_3G    2
#endif /*_WAN_H_*/

