/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: eth.h
 文件描述: eth通用接口头文件

 修订记录:
        1.  创建 : 李安楠
            日期 : 2008-10-?
            描述 : 创建文件
        2.  修改 : 徐晓东
            日期 : 2009-7-3
            描述 : 添加代码注释
**********************************************************************/
#ifndef __ETH_H__
#define __ETH_H__

#include <stdio.h>
#include <net/if.h>
#include <linux/socket.h>
#include <linux/sockios.h>

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/******************************************************************
 *                              DEBUG                             *
 ******************************************************************/
#ifdef PUBLIC_ETH_DEBUG
#define PUBLIC_TRACE(fmt, args...)    TBS_TRACE(MID_CCP, fmt, ##args)
#else
#define PUBLIC_TRACE(fmt, args...)    do { ; } while(0)
#endif

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define FILE_PROCNET_DEV "/proc/net/dev"

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
typedef struct tag_ST_ETH_MIIDATA {
	unsigned short       usPhyID;
	unsigned short       usRegNum;
	unsigned short       usValIn;
	unsigned short       usValOut;
}ST_ETH_MIIDATA;

typedef struct tag_ST_ETH_DEVSTAT {
    unsigned long long ullRxPackets;	/* total packets received       */
    unsigned long long ullTxPackets;	/* total packets transmitted    */
    unsigned long long ullRxBytes;	/* total bytes received         */
    unsigned long long ullTxBytes;	/* total bytes transmitted      */
    unsigned long ulRxErrors;	/* bad packets received         */
    unsigned long ulTxErrors;	/* packet transmit problems     */
    unsigned long ulRxDropped;	/* no space in linux buffers    */
    unsigned long ulTxDropped;	/* no space available in linux  */
    unsigned long ulRxMulticast;	/* multicast packets received   */
    unsigned long ulRxCompressed;
    unsigned long ulTxCompressed;
    unsigned long ulCollisions;

    /* detailed rx_errors: */
    unsigned long ulRxLengthErrors;
    unsigned long ulRxOverErrors;	/* receiver ring buff overflow  */
    unsigned long ulRxCrcErrors;	/* recved pkt with crc error    */
    unsigned long ulRxFrameErrors;	/* recv'd frame alignment error */
    unsigned long ulRxFifoErrors;	/* recv'r fifo overrun          */
    unsigned long ulRxMissedErrors;	/* receiver missed packet     */
    /* detailed tx_errors */
    unsigned long ulTxAbortedErrors;
    unsigned long ulTxCarrierErrors;
    unsigned long ulTxFifoErrors;
    unsigned long ulTxHeartbeatErrors;
    unsigned long ulTxWindowErrors;
}ST_ETH_DEVSTAT;

typedef struct tag_ST_ETH_INTERFACE {
    struct tag_ST_LAN_INTERFACE *pstNext, *pstPrev;
    char szName[IFNAMSIZ];	/* interface name        */
    short sType;			/* if type               */
    short sFlags;		/* various flags         */
    int iMetric;			/* routing metric        */
    int iMtu;			/* MTU value             */
    int iAsymmtu;		/* Asymmetric MTU value  */
    int iTxQueueLen;		/* transmit queue length */
    struct ifmap stMap;		/* hardware setup        */
    struct sockaddr stAddr;	/* IP address            */
    struct sockaddr stDstAddr;	/* P-P IP address        */
    struct sockaddr stBroadAddr;	/* IP broadcast address  */
    struct sockaddr stNetMask;	/* IP network mask       */
    struct sockaddr stIpxAddrBb;	/* IPX network address   */
    struct sockaddr stIpxAddrSn;	/* IPX network address   */
    struct sockaddr stIpxAddrE3;	/* IPX network address   */
    struct sockaddr stIpxAddrE2;	/* IPX network address   */
    struct sockaddr stDdpAddr;	/* Appletalk DDP address */
    struct sockaddr stEcAddr;	/* Econet address        */
    int iHasIp;
    int iHasIpxBb;
    int iHasIpxSn;
    int iHasIpxE3;
    int iHasIpxE2;
    int iHasAx25;
    int iHasDdp;
    int iHasEconet;
    char szHwAddr[32];		/* HW address            */
    int iStatisticsValid;
    ST_ETH_DEVSTAT stStats;		/* statistics            */
    int iKeepAlive;		/* keepalive value for SLIP */
    int iOutFill;		/* outfill value for SLIP */
}ST_ETH_INTERFACE;

/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/
int CheckMaxBitRate(const char *pszMaxBitRate);
int CheckDuplexMode(const char *pszDuplexMode);
int ETH_GetMacAddr(char * pszValue, const char * pszIfName);
int ETH_ProcNetDevVersion(char *pszBuf);
int ETH_ProcNetDevGetFields(char *pszBuf, ST_ETH_INTERFACE *pstInterface, int iProcNetDevVsn);
int ETH_GetIfStateFromProc(ST_ETH_INTERFACE ** ppstInterface, const char * pszIfName);
int ETH_GetBytesSent(char *pszValue, const char *pszIfName);
int ETH_GetBytesReceived(char *pszValue, const char * pszIfName);
int ETH_GetPacketsSent(char * pszValue, const char * pszIfName);
int ETH_GetPacketsReceived(char * pszValue, const char * pszIfName);
int ETH_GetPacketsReceivedERROR(char * pszValue, const char * pszIfName);
int ETH_GetPacketsSentERROR(char * pszValue, const char * pszIfName);
int ETH_GetPacketsReceivedDROP(char * pszValue, const char * pszIfName);
int ETH_GetPacketsSentDROP(char * pszValue, const char * pszIfName);

int ETH_GetPacketsSentUnicast(char * pszValue, const char * pszIfName);
int ETH_GetPacketsReceivedUnicast(char * pszValue, const char * pszIfName);
int ETH_GetPacketsSentMulticast(char * pszValue, const char * pszIfName);
int ETH_GetPacketsReceivedMulticast(char * pszValue, const char * pszIfName);
int ETH_GetPacketsSentBroadcast(char * pszValue, const char * pszIfName);
int ETH_GetPacketsReceivedBroadcast(char * pszValue, const char * pszIfName);

#endif /*__ETH_H__*/

