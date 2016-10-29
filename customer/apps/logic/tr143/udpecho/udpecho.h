/*
 * =====================================================================================
 *  Copyright (c) 2008, T&W ELECTRONICS Co., Ltd.
 *
 *       Filename:  upnp.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2008年12月12日 08时52分10秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  
 *
 * =====================================================================================
 */


#ifndef _UDPECHO_H
#define _UDPECHO_H


/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define UDPECHO_PATH_PREFIX   "InternetGatewayDevice.UDPEchoConfig."

#define UDPECHO_NODE_ENABLE                   "Enable"
#define UDPECHO_NODE_INTERFACE             "Interface"
#define UDPECHO_NODE_SRCIP                 "SourceIPAddress"
#define UDPECHO_NODE_UDPPORT               "UDPPort"
#define UDPECHO_NODE_UDPPLUSENABLED      "EchoPlusEnabled"

#define UDPECHO_NODE_PACKETSRCV      "PacketsReceived"
#define UDPECHO_NODE_PACKETSRESP      "PacketsResponded"
#define UDPECHO_NODE_BYTESRCV      "BytesReceived"
#define UDPECHO_NODE_BYTESRESP      "BytesResponded"
#define UDPECHO_NODE_TIMEFIRSTPACKETSRCV      "TimeFirstPacketReceived"
#define UDPECHO_NODE_TIMELASTPACKETSRCV      "TimeLastPacketReceived"

#define UDPECHO_PATH_ENABLE  "InternetGatewayDevice.UDPEchoConfig.Enable"
#define UDPECHO_PATH_INTERFACE  "InternetGatewayDevice.UDPEchoConfig.Interface"
#define UDPECHO_PATH_UDPPORT  "InternetGatewayDevice.UDPEchoConfig.UDPPort"
#define UDPECHO_PATH_SRCIP  "InternetGatewayDevice.UDPEchoConfig.SourceIPAddress"

#define WAN_PATH_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+"
#define WAN_PPPCONN_REGEX WAN_PATH_REGEX"\\.WANPPPConnection\\.[0-9]+$"
#define WAN_IPCONN_REGEX WAN_PATH_REGEX"\\.WANIPConnection\\.[0-9]+$"

#define LAN_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+$"


/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/

#define UDPECHO_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.UDPEchoConfig\\."

#define UDPECHO_PATH_REGEX "^InternetGatewayDevice\\.UDPEchoConfig\\.*"

#define UDPECHO_NODE_REGEX(node) UDPECHO_PATH_PREFIX_REGEX node "$"


/* 日志打印宏 */
#ifdef UDPECHO_DEBUG
#define UDPECHO_TRACE(fmt, args...)		COMMON_TRACE(MID_UDPECHO, fmt "\n", ##args)
#else
#define UDPECHO_TRACE(fmt, args...)
#endif

#define MAX_PROC_NAME_LEN               16

#define UDPECHO_TRACE_SUCCESS(fmt, args...)  UDPECHO_TRACE(GREEN INFO_SUCCESS NONE fmt, ##args)
#define UDPECHO_TRACE_ERROR(fmt, args...)    TBS_TRACE(MID_UDPECHO, LIGHT_RED INFO_ERROR NONE fmt "\n", ##args)

#define UDPECHO_GETVAL(node)        ((char *)COMM_GetNodeValue(UDPECHO_PATH_PREFIX node, CACHE_CFG))



static int UDPECHO_SwtichSM();

#endif

