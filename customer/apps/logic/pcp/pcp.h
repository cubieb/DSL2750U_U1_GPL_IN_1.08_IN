/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: pcp.h
 文件描述: pcp模块头文件

 修订记录:
        1. 作者: 
           日期: 2012-05-28
           内容: 创建文件
**********************************************************************/
#ifndef __PCP_H__
#define __PCP_H__

#include "warnlog.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/

#ifdef PCP_DEBUG
    #define PCP_TRACE(fmt, args...) COMMON_TRACE(MID_PCP, ECHO_GREEN fmt ECHO_NONE, ##args)
    #define PCP_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_PCP, ECHO_RED fmt ECHO_NONE, ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define PCP_TRACE(fmt, args...)
    #define PCP_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_PCP, fmt, ##args)
    #define CMD_PRINT 0   
#endif

#define PCP_TRACE_INTO_FUNC  PCP_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define PCP_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            PCP_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define PCP_RET_RETURN(Ret, fmt, args...)  \
    RET_RETURN(MID_PCP, Ret, fmt, ##args)

#define PCP_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
    POINTER_GOTO(MID_PCP, Pointer, gotoTag, fmt, ##args)

#define PCP_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            PCP_TRACE(fmt, ##args); \
            return Ret; \
        }

#define PCP_CHECK_NULL_PTR(ptr)   PCP_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define WAN_DEV_PREFIX          "InternetGatewayDevice.WANDevice."
#define KEY_WANConnectionDevice "WANConnectionDevice"
#define KEY_WANIPConnection     "WANIPConnection"
#define KEY_WANPPPConnection    "WANPPPConnection"
#define KEY_PCP                 "X_TWSZ-COM_PCP"
#define KEY_Map                 "Map"
#define KEY_Peer                "Peer"

#define IP_CONN_INST_PATH     WAN_DEV_PREFIX "{i}." KEY_WANConnectionDevice ".{i}." KEY_WANIPConnection ".{i}."
#define PPP_CONN_INST_PATH    WAN_DEV_PREFIX "{i}." KEY_WANConnectionDevice ".{i}." KEY_WANPPPConnection ".{i}."

#define PCP_NODE_NUM_OF_ENTRY       "PcpNumberOfEntries"
#define PCP_NODE_MaxPcpEntries      "X_TWSZ-COM_MaxPcpEntries"

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.
 * InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}.
 */
#define PCP_NODE_MaxPcpEntries      "X_TWSZ-COM_MaxPcpEntries"
#define PCP_NODE_PcpNumberOfEntries "X_TWSZ-COM_PcpNumberOfEntries"

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_TWSZ-COM_PCP.{i}.
 * InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}.X_TWSZ-COM_PCP.{i}.
 */
#define PCP_NODE_ServerName         "ServerName"
#define PCP_NODE_PcpPid             "PcpPid"
#define PCP_NODE_MaxMapEntries      "MaxMapEntries"
#define PCP_NODE_MapNumberOfEntries "MapNumberOfEntries"
#define PCP_NODE_MaxPeerEntries     "MaxPeerEntries"
#define PCP_NODE_PeerNumberOfEntries    "PeerNumberOfEntries"

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_TWSZ-COM_PCP.{i}.Map.{i}.
 * InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}.X_TWSZ-COM_PCP.{i}.Peer.{i}.
 * InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_TWSZ-COM_PCP.{i}.Map.{i}.
 * InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}.X_TWSZ-COM_PCP.{i}.Peer.{i}.
 */
#define PCP_NODE_Protocol           "Protocol"
#define PCP_NODE_InternalPort       "InternalPort"
#define PCP_NODE_ExternalAddr6      "ExternalAddr6"
#define PCP_NODE_ExternalPort       "ExternalPort"
#define PCP_NODE_RemoteAddr6        "RemoteAddr6"
#define PCP_NODE_RemotePort         "RemotePort"
#define PCP_NODE_Lifetime           "Lifetime"
#define PCP_NODE_State              "State"
#define PCP_NODE_ThirdPartyAddr6    "ThirdPartyAddr6"
#define PCP_NODE_PreferFailure      "PreferFailure"

#if 0
#define PCP_NODE_RemoteMask     "X_TWSZ-COM_RemoteMask"
#define PCP_NODE_InternalPortEndRange   "InternalPortEndRange"
#define PCP_NODE_InternalClient    "InternalClient"
#define PCP_NODE_PortMappingDescription     "PortMappingDescription"
#define PCP_OBJECT_PORTMAPPING   "PortMapping"
#define PCP_NODE_Weight          "X_TWSZ-COM_Weight"
#define PCP_NODE_SchedulePath         "SchedulePath"

#define PCP_PATH_LAN_IP        "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"
#define IPT_PATH_LAN_DEVICENAME    "InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_DeviceName"
#endif


/*****************************************************************
                    节点路径匹配正则表达式
*****************************************************************/
#define KEY_END "$"
#define WAN_CONN_INST_REGEX \
                "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\." KEY_WANConnectionDevice \
                "\\.[0-9]+\\.(" KEY_WANIPConnection "|" KEY_WANPPPConnection ")\\.[0-9]+"

#define PCP_INST_REGEX \
                WAN_CONN_INST_REGEX "\\." KEY_PCP "\\.[0-9]+\\."

#define PCP_MAP_INST_REGEX \
                PCP_INST_REGEX KEY_Map "\\.[0-9]+\\."

#define PCP_PEER_INST_REGEX \
                PCP_INST_REGEX KEY_Peer "\\.[0-9]+\\."

#define WAN_INST_NODE_REGEX(node) \
                WAN_CONN_INST_REGEX "\\." node KEY_END

#define PCP_INST_NODE_REGEX(node) \
                PCP_INST_REGEX node KEY_END

#define PCP_MAP_INST_NODE_REGEX(node) \
                PCP_MAP_INST_REGEX node KEY_END

#define PCP_PEER_INST_NODE_REGEX(node) \
                PCP_PEER_INST_REGEX node KEY_END

#define MAX_WAN_CONN_INDEX_COUNT             4

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"
#define INVALID_NODE_VALUE      "Invalid \"%s\" value: %s\n"

#define WAN_CONN_INST_PATH_FMT WAN_DEV_PREFIX "%lu." KEY_WANConnectionDevice ".%lu.WAN%sConnection.%lu."
#define WAN_CONN_INST_PATH_FMT_SSCANF WAN_DEV_PREFIX "%lu." KEY_WANConnectionDevice ".%lu.%*[^.].%lu"
#define WAN_CONN_PCP_INST_FMT_SSCANF WAN_DEV_PREFIX "%*u." KEY_WANConnectionDevice \
                                      ".%*u.%*[^.].%*u." KEY_PCP ".%lu"
#define PCP_BUF_LEN_8       8
#define PCP_BUF_LEN_16      16
#define PCP_BUF_LEN_24      24
#define PCP_BUF_LEN_32      32
#define PCP_BUF_LEN_64      64
#define PCP_BUF_LEN_128     128

#define UINT32              (unsigned int)
#define PCP_PATH_LENGTH           256

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
/* WAN连接类型 */
typedef enum tag_EN_WAN_CONN_TYPE {
    UNKNOWN_CONN,
    WAN_IP_CONN,
    WAN_PPP_CONN
} EN_WAN_CONN_TYPE;

/* 规则应用类型 */
enum {
    PCP_APPLY_NONE,
    PCP_APPLY_ADD,
    PCP_APPLY_DEL
};

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
/* pcp peer 实例结构 */
typedef struct tag_ST_PCP_PEER_INSTANCE {
    const char *pszProtocol;
    const char *pszInternalPort;
    const char *pszExternalAddr6;
    const char *pszExternalPort;
    const char *pszLifetime;
    const char *pszState;
    const char *pszThirdPartyAddr6;
    const char *pszPreferFailure;
    unsigned long ulPeerIndex;
} ST_PCP_PEER_INSTANCE;

/* pcp map 实例结构 */
typedef struct tag_ST_PCP_MAP_INSTANCE {
    const char *pszProtocol;
    const char *pszInternalPort;
    const char *pszExternalAddr6;
    const char *pszExternalPort;
    const char *pszRemoteAddr6;
    const char *pszRemotePort;
    const char *pszLifetime;
    const char *pszState;
    const char *pszThirdPartyAddr6;
    const char *pszPreferFailure;
    unsigned long ulMapIndex;
} ST_PCP_MAP_INSTANCE;

/* pcp 实例结构 */
typedef struct tag_ST_PCP_SRV_INSTANCE {
    const char *pszServerName;
    EN_WAN_CONN_TYPE enWanConnType;
    struct
    {
        unsigned long ulWanDevIdx;                  /* 节点路径WANDevice 的index */
        unsigned long ulWanConnDevIdx;              /* 节点路径WANConnectionDevice 的index */
        unsigned long ulWanPppOrIpConnIdx;              /* 节点路径WANPPPConnection 的index */
        unsigned long ulPcpSrvIndex;                /* 节点路径X_TWSZ-COM_PCP 的index */
    } stIndex;
    struct
    {
        unsigned char ucStateMachine;               /* 实例的重启状态机的状态 */
        unsigned long ulPcpUniqueId;
    } stPrivate;
    struct tag_ST_PCP_SRV_INSTANCE *pstNext;
} ST_PCP_SRV_INSTANCE;

/* WAN连接实例结构 */
typedef struct tag_ST_PCP_WANCONN_INSTANCE {
    struct tag_ST_PCP_WANCONN_INSTANCE *pstNext;
    char szIpAddr[MAX_IP_LEN];
    bool bWanConnState;
    EN_WAN_CONN_TYPE enWanConnType;
    struct {
        unsigned long ulWanDevIdx;
        unsigned long ulWanConnDevIdx;
        unsigned long ulWanConnIdx;
    }stIndex;
} ST_PCP_WANCONN_INSTANCE;

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/
#endif /*__PCP_H__*/

