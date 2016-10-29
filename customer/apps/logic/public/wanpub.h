/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: wanpub.h
 文件描述: 与WAN配置相关的公共头文件
 修订记录:
            创建 : 徐晓东
            日期 : 2009-7-28
            描述 :

**********************************************************************/
#ifndef _WANPUB_H_
#define _WANPUB_H_


#include <stdlib.h>
#include "warnlog.h"
#include "new_msg.h"

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#define MID_WANPUB  0xFF

#ifdef WANPUB_DEBUG
    #define WANPUB_TRACE(fmt, args...) COMMON_TRACE(MID_WANPUB, fmt, ##args)
    #define WANPUB_LOG_ERR(fmt, args...) COMMON_TRACE(MID_WANPUB, fmt, ##args)
    #define WANPUB_CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define WANPUB_TRACE(fmt, args...)
    #define WANPUB_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_WANPUB, fmt, ##args)
    #define WANPUB_CMD_PRINT 0
#endif

#define WANPUB_TRACE_INTO_FUNC WANPUB_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define WANPUB_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            WANPUB_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define WANPUB_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_WANPUB, Ret, fmt, ##args)

#define WANPUB_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_WANPUB, Pointer, gotoTag, fmt, ##args)

#define WANPUB_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            WANPUB_TRACE(fmt, ##args); \
            return Ret; \
        }

#define WANPUB_CHECK_NULL_PTR(ptr)   WANPUB_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

#define WANPUB_ASSERT(expr) \
    do{ \
        if(!(expr))\
        {\
            WANPUB_TRACE("Assert \"%s\" failed\n", #expr); \
            exit(-1); \
        } \
    }while(0)

/********************************************************************
 *                          节点路径定义                            *
 ********************************************************************/
#define WAN_DEV_PREFIX     "InternetGatewayDevice.WANDevice."
#define WAN_COMM_IF_CONFIG "WANCommonInterfaceConfig"
#define WAN_DSL_LINK_CONF  "WANDSLLinkConfig"
#define WAN_ETH_LINK_CONF  "WANEthernetLinkConfig"

#define NODE_WAN_ACCESS_TYPE "WANAccessType"
#define NODE_ATM_LINK_TYPE  "LinkType"
#define NODE_CONNECTED_DEV_NAME    "X_TWSZ-COM_ConnectedDeviceName"
#define NODE_DEV_NAME "X_TWSZ-COM_DeviceName"

#define NODE_BYTES_SENT             "EthernetBytesSent"
#define NODE_BYTES_RECV             "EthernetBytesReceived"
#define NODE_PACKETS_SENT           "EthernetPacketsSent"
#define NODE_PACKETS_RECEV          "EthernetPacketsReceived"
#define NODE_ERRORS_SENT            "EthernetErrorsSent"
#define NODE_ERRORS_RECV            "EthernetErrorsReceived"
#define NODE_DROP_SENT              "EthernetDiscardPacketsSent"
#define NODE_DROP_RECV              "EthernetDiscardPacketsReceived"

#define NODE_PACKETS_TXUNICAST  "EthernetUnicastPacketsSent"
#define NODE_PACKETS_RXUNICAST  "EthernetUnicastPacketsReceived"
#define NODE_PACKETS_TXMULTICAST  "EthernetMulticastPacketsSent"
#define NODE_PACKETS_RXMULTICAST  "EthernetMulticastPacketsReceived"
#define NODE_PACKETS_TXBROADCAST  "EthernetBroadcastPacketsSent"
#define NODE_PACKETS_RXBROADCAST  "EthernetBroadcastPacketsReceived"

#define NODE_IP_CONNECTION          "WANIPConnection"
#define NODE_PPP_CONNECTION         "WANPPPConnection"
#define NODE_PPP_CONNTYPE           "ConnectionType"
#define NODE_INST_READY_TO_DEL      "InstReadyToDel"
#define NODE_CONN_NAME              "Name"

#define WAN_CONN_DEV_MATCH_PATH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}."
#define WAN_IP_MATCH_PATH       WAN_CONN_DEV_MATCH_PATH "WANIPConnection.{i}."
#define WAN_PPP_MATCH_PATH      WAN_CONN_DEV_MATCH_PATH "WANPPPConnection.{i}."

#define WAN_PVC_PREFIX          "PVC:"

/********************************************************************
 *                       打印模式字符串                             *
 ********************************************************************/
#define WAN_DEV_INST_PATH_FMT "InternetGatewayDevice.WANDevice.%lu."
#define WAN_DEV_INTF_FMT "InternetGatewayDevice.WANDevice.%lu.WANCommonInterfaceConfig"
#define WAN_CONNDEV_INST_PATH_FMT WAN_DEV_INST_PATH_FMT "WANConnectionDevice.%lu."

#define WAN_CONN_INST_PATH_FMT_F WAN_DEV_PREFIX "%lu.WANConnectionDevice.%lu.WAN%sConnection.%lu."
#define WAN_CONN_INST_PATH_FMT_S WAN_DEV_PREFIX "%lu.WANConnectionDevice.%lu.%*[^.].%lu"

#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

/******************************************************************************
 *                                    ENUM                                    *
 ******************************************************************************/
enum EN_WAN_CONN_MODE {
    WAN_CONN_MODE_IP = 1,
    WAN_CONN_MODE_PPP,
    WAN_CONN_MODE_UNKONWN
};

enum EN_WAN_CONN_TYPE {
    WAN_CONN_TYPE_IP_ROUTE = 1,
    WAN_CONN_TYPE_IP_BRIDGE,
    WAN_CONN_TYPE_ALL,
    WAN_CONN_TYPE_UNKNOWN,
};

enum EN_WAN_CONN_STAT {
    WAN_CONN_STAT_DISABLE,
    WAN_CONN_STAT_ENABLE,
    WAN_CONN_STAT_ALL
};

enum EN_WAN_ACCESS_TYPE {
    WAN_ACCESS_ETH = 1,
    WAN_ACCESS_DSL,
    WAN_ACCESS_POTS,
    WAN_ACCESS_UNNOKWN,
};

enum {
    WAN_LINK_DOWN = 1,
    WAN_LINK_UP,
    WAN_LINK_UNAVIBLE,
    WAN_LINK_INITING,
    WAN_LINK_OTHER_STAT
};

enum {
    WAN_PHY_DISABLED = 1,
    WAN_PHY_UP,
    WAN_PHY_INITING,
    WAN_PHY_ESTABLISHING,
    WAN_PHY_NOSIGNAL,
    WAN_PHY_NOLINK,
    WAN_PHY_ERROR,
    WAN_PHY_OTHER_STAT
};

enum EN_ATM_LINK_TYPE {
    ATM_LINK_EOA = 0,
    ATM_LINK_IPOA,
    ATM_LINK_PPPOA,
    ATM_LINK_UNKNOWN,
};

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
typedef struct tag_ST_WANCONN_STAT{
	char *pszNodeName;
	char *pszValue;
}ST_WANCONN_STAT;

typedef struct {
    BOOL bPreCheck;
    char szToDelPath[MAX_PATH_LEN];
} ST_WEBP_CHECK;
/******************************************************************************
 *                             Function Prototype                             *
 ******************************************************************************/
extern int WANPUB_MsgUpdateHandler(unsigned short usDstMID);
extern int WANPUB_GetWanAccessType(unsigned long ulWanDevIdx, int *piWanAccessType);

extern int WANPUB_GetAtmLinkType(unsigned long ulWanDevIdx, unsigned long ulWanConnDevIdx,
                                 int *piAtmLinkType);
extern int WANPUB_GetWanDevName(unsigned long ulWanDevIdx, char *pszDevName, int iLen);
extern int WANPUB_GetWanLinkDevStatus(unsigned long ulWanDevIdx, unsigned long ulWanConnDevIdx,
                                      int *piLinkStatus);
extern int WANPUB_GetWanPhyDevStatus(unsigned long ulWanDevIdx, int *piStatus);
extern int WANPUB_GetConnCountByIdx(unsigned long ulWanDevIdx, unsigned long ulWanConnDevIdx,
                                    int iWanConnMode, int iWanConnType,
                                    int iWanConnStat, unsigned long *pulConnCount);
extern int WANPUB_GetConnCountByLinkKey(unsigned long ulWanDevIdx, unsigned long ulWanConnDevIdx,
                                    int iWanConnMode, int iWanConnType, unsigned long *pulConnCount);
extern int WANPUB_WanIpPppAddInstCheck(unsigned long ulWanDevIdx, unsigned long ulWanConnDevIdx,
                                       int iWanConnMode);
extern int WANPUB_CheckEnabledPPPConnCount(char *pszPath);
extern int WANPUB_CheckEnabledBrConnCount(char *pszPath);

#endif /*_WANPUB_H_*/
