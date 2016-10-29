/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: vpn.h
 文件描述: PPPOE适配 头文件

 修订记录:
        1. 作者: 李创
           日期: 2009-07-22
           内容: 增加vpn连接

**********************************************************************/

#ifndef __VPN_H__

/* 调试信息宏*/

//#define VPN_DEBUG

#ifdef VPN_DEBUG
#define VPN_TRACE(fmt, args...)   		printf("%s:%4d "fmt, __FUNCTION__, __LINE__, ##args)
#else
#define VPN_TRACE(fmt, args...)         
#endif

#define L2TPD_NAME                 "l2tpd"
#define PPTP_NAME                  "pptp"

/* VPN相关的节点 */
#define NODE_VPN_SERVER 			"X_TWSZ-COM_VPN_SERVER"
#define NODE_VPN_CLIENT 			"X_TWSZ-COM_VPN_CLIENT"
#define NODE_VPN_NETMASK 			"X_TWSZ-COM_VPN_NETMASK"
#define NODE_VPN_GATEWAY 			"X_TWSZ-COM_VPN_GATEWAY"
#define NODE_VPN_TYPE				"ConnectionType"

//#define NODE_WAN_DEV_NAME		"^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.X_TWSZ-COM_DeviceName"
#define NODE_WAN_DEV_NAME		"X_TWSZ-COM_DeviceName"

#ifndef PPPOE_SET_BASE_PATH
#define PPPOE_SET_BASE_PATH     "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\.[0-9]+\\."
#endif

#define VPN_SET_SERVER	PPPOE_SET_BASE_PATH NODE_VPN_SERVER
#define VPN_SET_CLIENT	PPPOE_SET_BASE_PATH NODE_VPN_CLIENT
#define VPN_SET_NETMASK	PPPOE_SET_BASE_PATH NODE_VPN_NETMASK
#define VPN_SET_GATEWAY	PPPOE_SET_BASE_PATH NODE_VPN_GATEWAY
#define VPN_SET_TYPE	PPPOE_SET_BASE_PATH NODE_VPN_TYPE

/* 前置声明 */
struct tag_ST_PPPOE_Instance;

/**************************** VPN 模块的对外API ****************************/

/* 鉴权参数合理性相关的API */
int VPN_CheckServer(const char *pVpnServer);
int VPN_CheckClient(const char *pVpnClient);
int VPN_CheckNetmask(const char *pNetmask);
int VPN_CheckGateway(const char *pGateway);

/* 对进程输出的响应处理函数 */
int VPN_L2tpdStarted(void *pInstance, const char *pOutput);

/* 其它辅助函数 */
int VPN_IsVpnConnection(struct tag_ST_PPPOE_Instance *pPppInstance);
int VPN_ProcCtrl(struct tag_ST_PPPOE_Instance *pPppInstance, const char *pcName,
				unsigned long ulInstNo, unsigned char ucOpt);
//int VPN_IsVpnConnection(ST_PPPOE_Instance *pPppInstance);

#endif /* __VPN_H__ */

