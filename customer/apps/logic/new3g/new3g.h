/**********************************************************************

 Copyright (c), 1991-2011, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

文件名	  : new3g.h

 文件描述: 3g wan 连接，及配置管理实现

 修订记录:

        1. 作者: 申常扳

           日期: 2011-08-22

           内容: 创建文件           
              
**********************************************************************/

#ifndef __NEW3G__
#define	__NEW3G__
#ifdef CONFIG_APPS_LOGIC_NEW3G
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define EXTERN_USE_3G_MNGR_H
#include "3g-mngr-comm.h"

//#define DEBUG_NEW3G

/*3G ppp 连接名*/
#define NEW3G_IFNAME "ppp10"

/*3g-mngr的消息开头标示*/
#define NEW3G_MNGR_MSG_TOKEN	"3g-mngr=>"

/*3g wan 名字,据此知道是否3g wan连接*/
#define NEW3G_SERVICE_NAME "pppo3g"

/*wan连接类型节点名*/
#define NEW3G_CONN_TYPE	"IP_Routed"
#define NEW3G_SERVICE_TYPE "Internet_TR069"
#define NEW3G_TRIGGER_DEMAND 	"OnDemand"
#define NEW3G_TRIGGER_ALWAYSON 	"AlwaysOn"

/*path maro define*/
#define NEW3G_DRIVER_NODE	"InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.driver"

#define NEW3G_PATH_ex	"InternetGatewayDevice.WANDevice.{i}.X_TWSZ-COM_WAN3GInterfaceConfig"
#define NEW3G_PATH	"InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig"
#define NEW3G_ELEM(e)	NEW3G_PATH"."e
#define NEW3G_WANPATH "InternetGatewayDevice.WANDevice.2.WANConnectionDevice"
#define NEW3G_PIN_PATH "InternetGatewayDevice.X_TWSZ-COM_MODEM_PIN"
#define NEW3G_PIN_ELEM(e)	NEW3G_PIN_PATH"."e

#define NEW3G_PIN_RESULT_PATH "InternetGatewayDevice.X_TWSZ-COM_MODEM_PIN_RESULT"
#define NEW3G_PIN_RESULT NEW3G_PATH".ActResult" 
#define NEW3G_PIN_NEXTPAGE NEW3G_PATH".ActJumpToPage" 
#define NEW3G_PIN_NEEDCAPTIVAL NEW3G_PATH".ActNeedCaptival" 


/*scb+ 2011-10-21 for store the pin action result page*/
#define NEW3G_PIN_RESULT_WEB_PAGE NEW3G_PATH".ResultWebPage" 


#define NEW3G_MATCH_BASE_PATH     "^InternetGatewayDevice\\.X_TWSZ-COM_MODEM\\."
#define NEW3G_MATCH_ELEM(e) NEW3G_MATCH_BASE_PATH e "$"

#define NEW3G_WANDEV_BASE "InternetGatewayDevice.WANDevice.2.WANConnectionDevice"

#define NEW3G_PRE_WANDEV NEW3G_WANDEV_BASE
#define NEW3G_PRE_WANCONN NEW3G_PRE_WANDEV".0.WANPPPConnection"

#define NEW3G_PRE_WANDEV_ELEM(e)  NEW3G_PRE_WANDEV".0."e
#define NEW3G_PRE_WANCONN_ELEN(e) NEW3G_PRE_WANCONN".0."e

#define NEW3G_PRE_WANDEV_INDEX_FMT   NEW3G_PRE_WANDEV".%d."
#define NEW3G_PRE_WANCONN_INDEX_FMT NEW3G_PRE_WANDEV_INDEX_FMT "WANPPPConnection.%d."


#define NEW3G_PRE_WANDEV_INDEX_FMT_ELEM(e)   NEW3G_PRE_WANDEV".%d."e
#define NEW3G_PRE_WANCONN_INDEX_FMT_ELEM(e) NEW3G_PRE_WANDEV_INDEX_FMT "WANPPPConnection.%d."e

#define NEW3G_WANDEV(i) NEW3G_WANDEV_BASE"."i
#define NEW3G_WANDEV_ELEM(i, e) NEW3G_WANDEV(i)"."e

#define NEW3G_WANCONN(i,j) NEW3G_WANDEV_ELEM(i, "WANPPPConnection")"."j
#define NEW3G_WANCONN_ELEM(i,j,e) NEW3G_WANCONN(i,j)"."e

#define NEW3G_DEFAULTGW_USER_PATH      "InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService"
#define NEW3G_DEFAULT_CONN_PATH		"InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"

#define NODE_NAME "Name"


#define DEFAULTGW_DNS_SERVER	    "DNSServers"
#define DEFAULTGW_USR_DNS_SERVER	"X_TWSZ-COM_UsrDNSServers"

/*length define*/
#define NEW3G_WANPATH_LEN 255
#define NEW3G_WANNAME_LEN NEW3G_WANPATH_LEN
#define NEW3G_CMD_LEN		MNGR_LEN_CMD
#define NEW3G_MNGR_MSG_LEN		127

#define NEW3G_WAN_DEV_PREFIX      "InternetGatewayDevice.WANDevice.1.WANConnectionDevice"
#define NEW3G_KEY_WAN_IP_CONN     "WANIPConnection"
#define NEW3G_KEY_WAN_PPP_CONN    "WANPPPConnection"
#define NEW3G_IP_CONN_INST_PATH     NEW3G_WAN_DEV_PREFIX ".{i}." NEW3G_KEY_WAN_IP_CONN ".{i}."
#define NEW3G_PPP_CONN_INST_PATH    NEW3G_WAN_DEV_PREFIX ".{i}." NEW3G_KEY_WAN_PPP_CONN ".{i}."

/*defined at defaultGW.h*/
#define NEW3G_DEFAULTGW_ASSIGN_PATH             "InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService"

/*sim card status:
*  NO USB CARD
*  lock disable
*  lock enable
*  NEED PIN CODE
*  NEED PUK CODE 
*/
#define constSimStsNoUsb  		"NO USB CARD"
#define constSimStsLockDis  		"lock disable"
#define constSimStsLockEnbl  		"lock enable"
#define constSimStsLockNeedPin  	"NEED PIN CODE"
#define constSimStsLockNeedPuk  	"NEED PUK CODE"

#define PIN_ACTION_ENBL_PIN 	"En_pin"
#define PIN_ACTION_DIS_PIN 		"Dis_pin"
#define PIN_ACTION_UN_PIN 		"Un_pin"
#define PIN_ACTION_UN_PUK 		"Un_puk"
#define PIN_ACTION_CHG_PIN 	"Chg_pin"

/*debug相关定义*/
#ifdef IS_3G_DEBUG
#undef 	IS_3G_DEBUG
#endif

#ifdef d_printf
#undef 	d_printf
#endif

/*debug 判断*/
#define IS_3G_DEBUG() IS_MNGR_DEBUG("/var/3g_debug_wan")

/*log打印*/
#define   LOG_3G(fmt, args...)	\
	do{printf("[NEW3G: ] %s:%d " fmt, __FILE__, __LINE__, ##args);}while(0)

#ifdef DEBUG_NEW3G
#define  d_printf(args...)	LOG_3G(args)
#else
#define  d_printf(args...)		\
	if (IS_3G_DEBUG())	\
		LOG_3G(args)
#endif

/*打印函数*/
#define NEW3G_PRINTF(args...)  LOG_3G(args)

/*定义出错宏*/
#define NEW3G_GOTO(iRet,strError,gotoTag)         \
	do{\
		if ( RET_FAILED(iRet) )    \
		{   \
			NEW3G_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
			goto gotoTag; \
		}\
	}while(0)

/**定义设置节点值的宏*/
#define NEW3G_SET_NODE_VAL(pstPath, pstVal) \
    do{ \
        if ( RET_FAILED(CFG_SetNodeVal((pstPath), (pstVal), NULL)) )  \
        	LOG_3G("set %s to %s false\n", pstPath, pstVal); \
    }while(0)



/* 设置消息头 */
#define NEW3G_MSGHEAD(pstMsg, type, id, usDst) \
{ \
    pstMsg->stMsgHead.usSrcMID = MID_WEB; \
    pstMsg->stMsgHead.usDstMID = (usDst); \
    pstMsg->stMsgHead.ulMsgID = id; \
    pstMsg->stMsgHead.usMsgType = type; \
}

/* 创建消息 */
#define NEW3G_CREATE_MSG(pstMsg, len, type, id, usDst) \
{ \
    pstMsg = MSG_CreateMessage(len); \
    if (NULL == pstMsg) \
    { \
        LOG_3G("create msg error\n"); \
        return ERR_CREATE_MSG_FAIL; \
    } \
    NEW3G_MSGHEAD(pstMsg, type, id, usDst); \
}

/* 发送消息 */
#define NEW3G_MSG_SEND(pstMsg) \
({ \
    ST_MSG *__pstMsg = (pstMsg);\
    d_printf("send msg:0x%x--->0x%x type=0x%x\n", \
    	__pstMsg->stMsgHead.usSrcMID,\
    	__pstMsg->stMsgHead.usDstMID,\
    	__pstMsg->stMsgHead.usMsgType);\
    	\
    int iRet = MSG_SendMessage(pstMsg); \
    MSG_ReleaseMessage(pstMsg); \
    if ( RET_FAILED(iRet) )    \
		LOG_3G("send msg error:\n");\
    iRet;\
})

typedef struct {  
	char provider_code[80];  
	char provider_name[80];
	char country[80];
} provider_table;



/*定义执行3g-mngr命令的宏*/
//#define CMD_3G(a,b,c)    do{d_printf("3G_CMD:%s\n", (a));}while(0)
#define CMD_3G(a,b,c) 	do{d_printf("3G_CMD:%s\n", (a)); cmd_3g((a), (b), (c));}while(0)


/*定义宏,便于知道在什么地方调用*/

/*知道旧状态*/
void 		__NEW3G_DoForStatusChange(const char *pszPath, const char *pszOldStatus, const char *pszNewStatus);
#define 		NEW3G_DoForStatusChange(a,b,c) \
	do {d_printf("WanConn %s status change\n", (a)); __NEW3G_DoForStatusChange((a),(b),(c));}while(0)

/*不知道旧状态*/
void 		__NEW3G_DoForStatusChange1(void *pstInstance, const char *pszOldStatus, const char *pszNewStatus);
#define 		NEW3G_DoForStatusChange1(a,b,c) \
	do {d_printf("WanConn status change:\n"); __NEW3G_DoForStatusChange1((a),(b),(c));}while(0)


extern int iNotUpDate3GWanStatus;
	
/*函数声明*/
int 			NEW3G_Is3GWanEnable();
int  			NEW3G_SendDefaultGWTo3G();
int 			NEW3G_SetAllConnTo3GWan();
int 			NEW3G_strcmp(const char *s1, const char *s2);
int 			NEW3G_DelAllNon3GDWWanFrom3G();
int 			NEW3G_Is3GWanPPPConn(const char *pszPath);
int 			NEW3G_SetOneWanInfo(const char *pcPath, void *pvAct);
int 			NEW3G_DefGWSetProcessFunc(int iAct, const char *szWanPath);
int 			NEW3G_SendLinkStatus(const char *pcPath, int iAct, const char *szNewStatus);
int 			NEW3G_FixStaticRoutingDefaultRouteInfo(const char * pszWanConnPath, int iPos);

/*void 类型*/
void 		NEW3G_UpDate3GWannInfoFromCFG();
void 		NEW3G_NotifyDNSToBuckup(const char *pszWanConnPath);
void 		NEW3G_SetInfoForApplyFunc(const char *pszPath, const char *pszValue);
void   NEW3G_DownLoadModemCfgInfo();
 void NEW3G_GetParamFromCache();

/*字符 类型*/
const char *   NEW3G_FixFetchAction();
const char *	NEW3G_CreatePathFromInst(void *pstPPPIns);
const char *	NEW3G_Get3GWANPPPValueByIns(void *pstIns,  char * pszElem);

#endif
#endif

