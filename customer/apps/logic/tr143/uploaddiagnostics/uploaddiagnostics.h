/*
 * =====================================================================================
 *  Copyright (c) 2008, T&W ELECTRONICS Co., Ltd.
 *
 *       Filename:  uploaddiagnostics.h
 *
 *    Description:
 *
 *        Version:  
 *        Created:  
 *       Revision:  
 *       Compiler:  
 *
 *         Author:  
 *
 * =====================================================================================
 */


#ifndef _UPLOADDIAG_H
#define _UPLOADDIAG_H



/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define UPLOADDIAG_PATH_PREFIX   "InternetGatewayDevice.UploadDiagnostics."

#define NODE_DIAGSTATE                   "DiagnosticsState"
#define NODE_INTERFACE             "Interface"
#define NODE_UPLOADURL                 "UploadURL"
#define NODE_DSCP               "DSCP"
#define NODE_ETHPRIORITY      "EthernetPriority"
#define NODE_ROMTIME      "ROMTime"
#define NODE_BOMTIME      "BOMTime"
#define NODE_EOMTIME      "EOMTime"

#define NODE_TOTALBYTESSENT      "TotalBytesSent"
#define NODE_TESTFILELEN      "TestFileLength"

#define NODE_TCPREQTIME     "TCPOpenRequestTime"
#define NODE_TCPRESPTIME     "TCPOpenResponseTime"


#define UPLOADDIAG_PATH_DIAGSTATE  "InternetGatewayDevice.UploadDiagnostics.DiagnosticsState"
#define UPLOADDIAG_PATH_UPLOADURL "InternetGatewayDevice.UploadDiagnostics.UploadURL"
#define UPLOADDIAG_PATH_TESTFILELEN  "InternetGatewayDevice.UploadDiagnostics.TestFileLength"

#define WAN_PATH_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+"
#define WAN_PPPCONN_REGEX WAN_PATH_REGEX"\\.WANPPPConnection\\.[0-9]+$"
#define WAN_IPCONN_REGEX WAN_PATH_REGEX"\\.WANIPConnection\\.[0-9]+$"

#define LAN_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+$"


/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/

#define UPLOADDIAG_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.UploadDiagnostics\\."

#define UPLOADDIAG_PATH_REGEX "^InternetGatewayDevice\\.UploadDiagnostics\\.*"

#define UPLOADDIAG_NODE_REGEX(node) UPLOADDIAG_PATH_PREFIX_REGEX node "$"


/* 日志打印宏 */
#ifdef UPLOADDIAG_DEBUG
#define UPLOADDIAG_TRACE(fmt, args...)		COMMON_TRACE(MID_UPLOADDIAG, fmt "\n", ##args)
#else
#define UPLOADDIAG_TRACE(fmt, args...)
#endif

#define MAX_PROC_NAME_LEN               16

#define UPLOADDIAG_TRACE_SUCCESS(fmt, args...)  UPLOADDIAG_TRACE(GREEN INFO_SUCCESS NONE fmt, ##args)
#define UPLOADDIAG_TRACE_ERROR(fmt, args...)    TBS_TRACE(MID_UDPECHO, LIGHT_RED INFO_ERROR NONE fmt "\n", ##args)

#define UPLOADDIAG_GETVAL(node)        ((char *)COMM_GetNodeValue(UPLOADDIAG_PATH_PREFIX node, CACHE_CFG))


#endif

