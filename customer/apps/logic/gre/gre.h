/******************************************************************************
  Copyright (c), 1991-2010, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
  
  文件名称：gre.h
  文件描述：gre模块头文件
  修订记录：
         1. 作者：wangjiaming
            日期：2012-04-24
            内容：创建文件
******************************************************************************/
#ifndef _GRE_H_
#define _GRE_H_


#include <crypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "common.h"
#include "new_msg.h"
#include "tbserror.h"
#include "common_func.h"

/******************************************************************************
*                                   MACRO                                     *
******************************************************************************/
#ifdef  GRE_DEBUG
#define GRE_TRACE(fmt, args...)                              COMMON_TRACE(MID_GRE, fmt"\n", ##args)
#else
#define GRE_TRACE(str, args...)
#endif

#define GRE_FUNC_IN(fmt, args...)                            GRE_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define GRE_FUNC_OUT()                                       GRE_TRACE("--O %s\n", __FUNCTION__)

#define GRE_LOG_ERR(fmt, args...)                            COMMON_LOG_ERROR(MID_GRE, fmt, ##args)

#define CALL_FUCTION_FAILED "Call function \"%s\" failed\n"
#define GRE_PRINT_CMD 1

#define GRE_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_GRE, iRet, strError, ##args)
#define GRE_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_GRE, pRet, strError, ##args)

#define GRE_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_IPTUNNEL, iRet, strError, ##args)
#define GRE_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_IPTUNNEL, pRet, strError, ##args)

#define GRE_RET_GOTO(iRet, strError, gotoTag)                RET_GOTO(MID_GRE, iRet, strError, gotoTag)
#define GRE_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_GRE, pRet, gotoTag ,strError, ##args)

#define GRE_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_GRE, iRet, strError, ##args)
#define GRE_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_GRE, pRet, strError, ##args)

#define ERR_GET(node)                                           "Get Node["node"] value failed.\n"
#define ERR_SET(node)                                           "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)                                       "Set Node["node"] value failed, value = %s\n"


#define MAX_NAME_LEN                        64
#define MAX_IPV6_LEN 50

/* 公共节点名称定义 */
#define NODE_TunnelName                                "TunnelName"
#define NODE_AssociatedWanIfName                       "AssociatedWanIfName"
#define NODE_RemoteIpv4WanAddress                      "RemoteIpv4WanAddress"
#define NODE_PeerLocalAddress                          "PeerLocalAddress"
#define NODE_PeerRemoteAddress                         "PeerRemoteAddress"
#define NODE_PeerIPv4Address                           "PeerIPv4Address"
#define NODE_PeerIPv4MaskLen                           "PeerIPv4MaskLen"

#define NODE_CONN_DEV_NAME                             "X_TWSZ-COM_ConnectedDeviceName"
#define NODE_CONN_PROTOCOL                             "X_TWSZ-COM_ProtocolType"
#define NODE_IP_GATEWAY                                "DefaultGateway"
#define NODE_PPP_REMOTE_IP                             "RemoteIPAddress"

/* Wan and Lan节点名称定义 */
#define NODE_WAN_IP                                      "ExternalIPAddress"

/* 节点路径定义 */
#define GRE_PATH                                           "InternetGatewayDevice.X_TELEFONICA-ES_GreTunnel."

/* 路径正则表达式定义 */
#define Gre_REGEX_ADD                                          "^InternetGatewayDevice\\.X_TELEFONICA-ES_GreTunnel\\.[0-9]{1,5}\\.$"
#define Gre_REGEX_DEL                                          "^InternetGatewayDevice\\.X_TELEFONICA-ES_GreTunnel\\.[0-9]{1,5}\\.$"
#define Gre_REGEX                                          "^InternetGatewayDevice\\.X_TELEFONICA-ES_GreTunnel\\.[0-9]{1,5}\\."
#define REGEX_GRE(node)                                         Gre_REGEX node "$"

#define FOR_EACH_LIST(pos, list) for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)

/******************************************************************************
*                            FUNC DECLEARATION                                *
******************************************************************************/
/* Init && Destroy */
int Gre_ModuleInit();
int Gre_ModuleDestroy();

#endif


