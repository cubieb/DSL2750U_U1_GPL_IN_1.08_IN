/*
 * =====================================================================================
 *  Copyright (c) 2008, T&W ELECTRONICS Co., Ltd.
 *
 *       Filename:  downloaddiagnostics.h
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


#ifndef _DOWNLOADDIAG_H
#define _DOWNLOADDIAG_H



/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define DOWNLOADDIAG_PATH_PREFIX   "InternetGatewayDevice.DownloadDiagnostics."

#define NODE_DIAGSTATE                   "DiagnosticsState"
#define NODE_INTERFACE             "Interface"
#define NODE_DOWNLOADURL                 "DownloadURL"
#define NODE_DSCP               "DSCP"
#define NODE_ETHPRIORITY      "EthernetPriority"
#define NODE_ROMTIME      "ROMTime"
#define NODE_BOMTIME      "BOMTime"
#define NODE_EOMTIME      "EOMTime"
#define NODE_TESTBYTERCV      "TestBytesReceived"
#define NODE_TOTALBYTERCV      "TotalBytesReceived"
#define NODE_TCPREQTIME     "TCPOpenRequestTime"
#define NODE_TCPRESPTIME     "TCPOpenResponseTime"


#define DOWNLOADDIAG_PATH_DIAGSTATE  "InternetGatewayDevice.DownloadDiagnostics.DiagnosticsState"
#define DOWNLOADDIAG_PATH_DOWNLOADURL "InternetGatewayDevice.DownloadDiagnostics.DownloadURL"
//#define DOWNLOADDIAG_PATH_UDPPORT  "InternetGatewayDevice.downloaddiagnostics.UDPPort"

#define WAN_PATH_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+"
#define WAN_PPPCONN_REGEX WAN_PATH_REGEX"\\.WANPPPConnection\\.[0-9]+$"
#define WAN_IPCONN_REGEX WAN_PATH_REGEX"\\.WANIPConnection\\.[0-9]+$"

#define LAN_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+$"


/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/

#define DOWNLOADDIAG_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.DownloadDiagnostics\\."

#define DOWNLOADDIAG_PATH_REGEX "^InternetGatewayDevice\\.DownloadDiagnostics\\.*"

#define DOWNLOADDIAG_NODE_REGEX(node) DOWNLOADDIAG_PATH_PREFIX_REGEX node "$"


/* 日志打印宏 */
#ifdef DOWNLOADDIAG_DEBUG
#define DOWNLOADDIAG_TRACE(fmt, args...)		COMMON_TRACE(MID_DOWNLOADDIAG, fmt "\n", ##args)
#else
#define DOWNLOADDIAG_TRACE(fmt, args...)
#endif

#define MAX_PROC_NAME_LEN               16

#define DOWNLOADDIAG_TRACE_SUCCESS(fmt, args...)  DOWNLOADDIAG_TRACE(GREEN INFO_SUCCESS NONE fmt, ##args)
#define DOWNLOADDIAG_TRACE_ERROR(fmt, args...)    TBS_TRACE(MID_UDPECHO, LIGHT_RED INFO_ERROR NONE fmt "\n", ##args)

#define DOWNLOADDIAG_GETVAL(node)        ((char *)COMM_GetNodeValue(DOWNLOADDIAG_PATH_PREFIX node, CACHE_CFG))


#endif

