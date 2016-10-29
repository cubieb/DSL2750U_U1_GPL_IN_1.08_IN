#ifndef __TBS_TF_FIREWALL_H
#define __TBS_TF_FIREWALL_H

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "tbsipv6.h"

#define ALL_PROTOCOL "all"

/******************************************************************************
*                                 MACRO
******************************************************************************/
/* 调试信息宏*/
//#define TF_FW_DEBUG   0

#ifdef TF_FW_DEBUG
#define TF_FW_TRACE(fmt, args...)                              COMMON_TRACE(MID_TF_FIREWALL, fmt"\n", ##args)
#define TF_FW_FUNC_IN(fmt, args...)                            TF_FW_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define TF_FW_FUNC_OUT()                                       TF_FW_TRACE("--O %s\n", __FUNCTION__)
#else
#define TF_FW_TRACE(str, args...)
#define TF_FW_FUNC_IN(fmt, args...)
#define TF_FW_FUNC_OUT()



/* 模块关闭调试时关掉内存调试 */
#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free
#endif
#ifdef calloc
#undef calloc
#endif
#ifdef realloc
#undef realloc
#endif

#endif

#define TF_FW_LOG_ERR(fmt, args...)                            COMMON_LOG_ERROR(MID_FIREWALL, fmt, ##args)

#define TF_FW_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_FIREWALL, iRet, strError, ##args)
#define TF_FW_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_FIREWALL, pRet, strError, ##args)

#define TF_FW_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_FIREWALL, iRet, strError, ##args)
#define TF_FW_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_FIREWALL, pRet, strError, ##args)

#define TF_FW_RET_GOTO(iRet, strError, gotoTag)                RET_GOTO(MID_FIREWALL, iRet, strError, gotoTag)
#define TF_FW_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_FIREWALL, pRet, gotoTag ,strError, ##args)

#define TF_FW_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_FIREWALL, iRet, strError, ##args)
#define TF_FW_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_FIREWALL, pRet, strError, ##args)

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"

#define FOR_EACH_LIST(pos, list)        for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)


/************************** 常量定义 **************************/

/* 节点名定义 */
#define TF_FW_NODE_Name          "Name"
#define TF_FW_NODE_Interface                 "Interface"
#define TF_FW_NODE_Type   "Type"
#define TF_FW_NODE_DefaultAction   "DefaultAction"
//Begin: Modify by dengfeng for bug 12897
#define TF_FW_NODE_DefaultChain    "DefaultChain"
//End: Modify by dengfeng for bug 12897

/* 过滤表节点 */
#define TF_FW_NODE_Enabled                  "Enabled"
//add by dengfeng
#define TF_FW_NODE_IPProtocolVersionType    "IPProtocolVersionType"
//add by dengfeng
#define TF_FW_NODE_Protocol                "Protocol"
#define TF_FW_NODE_Action          "Action"
#define TF_FW_NODE_RejectType   "RejectType"
#define TF_FW_NODE_IcmpType                 "IcmpType"
//orig dest node

#define TF_FW_NODE_Dest                       "Destination."
#define TF_FW_NODE_Orig                          "Origin."
#define TF_FW_NODE_IPAddress              "IPAddress"
#define TF_FW_NODE_Mask                   "Mask"
#define TF_FW_NODE_StartPort                  "StartPort"
#define TF_FW_NODE_EndPort                 "EndPort"
#define TF_FW_NODE_Packets                 "Packets"
#define TF_FW_NODE_Bytes                 "Bytes"
/*
 * 增加新节点以支持如下功能：
 * FQDN (Fully Qualified Domain Name) of WAN session
 * DiffServ codepoint (IETF RFC 3260)
 * Traffic fitting an ALG filter 
 * packet length
 */
#define TF_FW_NODE_ALG 							"Alg"
#define TF_FW_NODE_DSCP 						"DSCP"
#define TF_FW_NODE_PACKETLENGTHMIN 				"PacketLengthMin"
#define TF_FW_NODE_PACKETLENGTHMAX 				"PacketLengthMax"
#define TF_FW_NODE_TCPFLAGS 				    "TcpFlags"
#define TF_FW_NODE_FQDN 						"FQDN"
#define TF_FW_NODE_FQDN_ENABLE 					"FQDNEnable"
#define TF_FW_NODE_ALG_ENABLE 					"AlgEnable"

#define ALG_CLASS "ftp,tftp,pptp,sip,,rtsp"

#define TCPFLAGS_CLASS "SYN,ACK,FIN,RST,URG,PSH"

//support all WAN/LAN/INTERFACES
#define TF_FW_WAN_INTERFACES_NAME         "WAN"
#define TF_FW_LAN_INTERFACES_NAME         "LAN"
#define TF_FW_ALL_INTERFACES_NAME         "WAN/LAN"
#define TF_FW_FAKEDEV_NAME                "fakedev"
#define TF_FW_WAN_INTERFACE				  "InternetGatewayDevice.WANDevice"	
#define WAN_IPCONN_INST_MATCH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define WAN_CONNECTION_NAME           "Name"
#define WAN_PPPCONN_INST_MATCH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define WAN_CONNECTION_NAME "Name"
#define V6CONN_IPV6_CONFIG_NODE "X_TWSZ-COM_IPv6Config"

#define TF_FW_TABLE_NAME_LEN               16



#define NODE_DeviceName             "X_TWSZ-COM_DeviceName"
#define WAN_NODE_DeviceName         "X_TWSZ-COM_ConnectedDeviceName"
#define LAN_NODE_DeviceName         "X_TWSZ-COM_DeviceName"

/* 最多规则条数 */
#define TF_FW_INSTANCE_MAX_NUM             20


/* 节点路径定义 */
#define TF_FW_PATH_PREFIX                  "InternetGatewayDevice.X_TELEFONICA-ES_Firewall."


#define LAN_PATH_PREFIX                 "InternetGatewayDevice.LANDevice."
#define WAN_PATH_PREFIX                 "InternetGatewayDevice.WANDevice."

/* 格式定义 */
#define TF_FW_FORMAT_FIREWALL            TF_FW_PATH_PREFIX "Firewall.%u."
#define TF_FW_FORMAT_RULE                    TF_FW_FORMAT_FIREWALL "Rule.%u."


/* 正则定义 */
#define TF_FW_REGEX_PREFIX                 "^InternetGatewayDevice\\.X_TELEFONICA-ES_Firewall\\."
#define TF_FW_REGEX_FIREWALL_PREFIX   TF_FW_REGEX_PREFIX "Firewall\\.[0-9]{1,5}\\."
#define TF_FW_REGEX_RULE_PREFIX           TF_FW_REGEX_FIREWALL_PREFIX "Rule\\.[0-9]{1,5}\\."

#define REGEX_F(node)           TF_FW_REGEX_FIREWALL_PREFIX node "$"
#define REGEX_R(node)            TF_FW_REGEX_RULE_PREFIX node "$"
#define REGEX_T(node)            TF_FW_REGEX_RULE_PREFIX "(Destination|Origin)\\." node "$"
#define REGEX_SF(node)           TF_FW_REGEX_FIREWALL_PREFIX "stats\\."  node "$"
//#define REGEX_SR(node)           TF_FW_REGEX_RULE_PREFIX   "stats\\."  node "$"
#define REGEX_SR(node)           TF_FW_REGEX_RULE_PREFIX   "sstats\\."  node "$"

#define TF_FW_REGEX_ADDFIREWALL            TF_FW_REGEX_PREFIX "Firewall\\.$"
#define TF_FW_REGEX_ADDRULE                    TF_FW_REGEX_FIREWALL_PREFIX "Rule\\.$"

/* WAN dev regex */
#define TF_FW_REGEX_IPCONN_INST            "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}\\.$"
#define TF_FW_REGEX_PPPCONN_INST           "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}\\.$"

#define TF_FW_REGEX_WANDEVICE              "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]\{1,5}$"
#define TF_FW_REGEX_WAN_CONN_IP            "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define TF_FW_REGEX_WAN_CONN_PPP           "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"
#define TF_FW_REGEX_WAN_PROTOCOLTYPE       "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WAN(IP|PPP)Connection\\.[0-9]+\\.X_TWSZ-COM_IPv(6|4)Enabled$"
#define TF_FW_WAN_DEV_PREFIX               "InternetGatewayDevice.WANDevice."

/* LAN dev regex */
#define TF_FW_REGEX_LAN_CONN               "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}$"
#define TF_FW_REGEX_WLAN_CONN              "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}\\.WLANConfiguration\\.[0-9]{1,5}$"


/*
命令所用参数
*/

/* 连接新增/删除/建立/断开 */
#define TF_FW_CMD_CONN_EST                 1
#define TF_FW_CMD_CONN_FIN                 2
#define TF_FW_CMD_CONN_ADD                 3
#define TF_FW_CMD_CONN_DEL                 4

/* 添加规则/删除规则 */
#define TF_FW_CMD_ADD                      1
#define TF_FW_CMD_DEL                      0

/* 添加/删除 wan/lan 设备 */
#define TF_FW_NOCHANG_DEV                  0
#define TF_FW_ADD_WAN_DEV                  1
#define TF_FW_DEL_WAN_DEV                  2
#define TF_FW_ADD_LAN_DEV                  3
#define TF_FW_DEL_LAN_DEV                  4


/* 输出格式控制*/
#define FMT_SUCC                        ECHO_GREEN"[Success] "ECHO_NONE
#define FMT_FAIL                        ECHO_RED"[Failed] "ECHO_NONE


/*防火墙其它相关宏*/
#define TF_FW_TMP_DIR                       "/var/firewall"
#define TF_FW_TMP_CONTENTFILE               "/var/firewall/content"
#define TF_FW_TMP_CHAIN                     "Chain "
#define TF_FW_TMP_IPTABLES                  "iptables"
#define TF_FW_TMP_FILTER                    "filter"
#define TF_FW_TMP_IP6TABLES                 "ip6tables"
#define TF_FW_TMP_FORMAT_CHAIN              "Chain %s"
#define TF_FW_TMP_FORMAT_EXCUTE_TO_FILE     "%s -t %s --line-number -nvL > %s"
#define TF_FW_TMP_FORMAT_EXCUTE_DELETE      "%s -t %s -D %s %u"
#define TF_FW_TMP_FORMAT_CHAINNAME          "TF_FW_%u"
#define TF_FW_TMP_FORMAT_WAN_CONN_DEVICE    "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%u."
#define TF_FW_TMP_MAXRULE_WITH_SAMENAME     32
#define TF_FW_TMP_NAME_LENGTH               32
#define TF_FW_TMP_EXIST_CHAIN               1
#define TF_FW_TMP_NON_EXIST_CHAIN           0
#define TF_FW_TMP_IPV4                      4
#define TF_FW_TMP_IPV6                      6
#define TF_FW_TMP_DEFAULTCHAINIDX           1
#define TF_FW_TMP_STR_IPV4_6                "IPv4_6"
#define TF_FW_TMP_STR_IPV4                  "IPv4"
#define TF_FW_TMP_STR_IPV6                  "IPv6"
#define TF_FW_TMP_STR_IPV6_NODE_NAME        "X_TWSZ-COM_IPv6Config." 
#define TF_FW_TMP_STR_TCMPV6                "icmpv6"
#define TF_FW_IP6_CONN_STAT_GLOBAL          "GlobalConnected"
#define TF_FW_IP6_CONN_STAT_LOCAL           "LinkLocalConnected"
#define TF_FW_IP6_CONN_STAT_DISCONN         "Disconnected"
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define TF_FW_WAN_ENABLE_IPV6     "X_TELEFONICA-ES_IPv6Enabled"
#endif
#define TF_FW_WAN_ENABLE_IPV4     "X_TELEFONICA-ES_IPv4Enabled"



/******************************************************************************
*                                STRUCT
******************************************************************************/

/* 路径对应设备名 */

typedef struct tag_ST_CONNPATH_DEVNAME
{
    struct list_head head;
    /* 注意:没有'.'后缀 */
    char szConnPath[MAX_PATH_LEN];
    char szDevName[MAX_IF_NAME_LEN];
} ST_ConnPath_DevName;

/* Firewall单个实例对应节点信息 */

/* firewall实例 */
typedef struct tag_ST_TF_FW_Firewall_Instance
{
    unsigned int nFirewallIdx; 	
    unsigned int nIndex;	
    unsigned int nRuleNum; 	
    char *szName;	
    char *szInterface;	
    char *szType;
    char *szDefaultAction; 
    char *szDefaultChain;
    char *szPackets;
    char *szBytes;	
    int pkts;
    int bytes;
} ST_TF_FW_Firewall_Instance;

typedef struct tag_ST_TF_FW_Rule_Instance
{
    unsigned int nFirewallIdx; 	
    unsigned int nRuleIdx;
    unsigned int nIndex;
    char *szEnabled;
    char *szIPProtocolVersionType;
    char *szProtocol;
    char *szAction;
    char *szRejectType;
    char *szIcmpType;
    char *szSrcIPAddress;	
    char *szSrcMask;
    char *szSrcStartPort;	
    char *szSrcEndPort;
	
    char *szDestIPAddress;	
    char *szDestMask;
    char *szDestStartPort;
    char *szDestEndPort;
    char *szPackets;
    char *szBytes;	
	char *szAlg;
	char *szDSCP;
	char *szFQDN;
	char *szFQDNEnable;
	char *szAlgEnable;
	char *szPacketLengthMin;
	char *szPacketLengthMax;
	char *szTcpFlags;
    int pkts;
    int bytes;	
} ST_TF_FW_Rule_Instance;


/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init && Destroy */
int TF_FW_ModuleRegister();
int TF_FW_ModuleInit();
int TF_FW_ModuleDestroy();




#endif//__TBS_TF_FIREWALL_H





