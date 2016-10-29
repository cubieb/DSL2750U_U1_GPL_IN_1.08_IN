/**********************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

  文件名称: tr069.h
  文件描述:

  修订记录:
         1. 作者: chenzhuo
            日期: 2007-11-13
            内容: 创建文件

         2. 作者: huangjidong
            日期: 2008-11-14
            内容: 重构

**********************************************************************/

#ifndef _TR069_H_
#define _TR069_H_

#include "common.h"
#include "common_func.h"
#include "download.h"
#include "upload.h"


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
//#define TR069_BE_DEBUG 1
#ifdef TR069_BE_DEBUG
#define TR069_TRACE(fmt, args...)   COMMON_TRACE(MID_TR069BE, fmt, ##args)
#else
#define TR069_TRACE(fmt, args...)
#endif

/* Lan 设备名最大长度*/
#define TR069_MAX_LAN_DEV_NAME_LEN 	64
#define TR069_MAX_WAN_DEV_NAME_LEN 	64
#define TR069_MAX_DNSSERVERS_LEN 	128	/* TR-098 Amendment 1: DNSServers string(64) */
#define TR069_MAX_WAN_IP_LEN 	 	32
#define TR069_MAX_PATH_LEN 	 		260


#define TR069_MSG_STRLEN_MAX        (1024)
#define TR069_MAX_CAFILE_NUM        5
#define MAX_DATE_TIME_LEN 21

#define NODE_PATH_URL               "InternetGatewayDevice.ManagementServer.URL"
#define NODE_PATH_TR069ENABLE       "InternetGatewayDevice.ManagementServer.EnableCWMP"
#define NODE_PATH_PERIODICINFORM    "InternetGatewayDevice.ManagementServer.PeriodicInformEnable"
#define NODE_PATH_TR069ENABLE_REG   "^InternetGatewayDevice\\.ManagementServer\\.EnableCWMP$"
#define CWMP_MS_PARAMETERKEY        "InternetGatewayDevice.ManagementServer.ParameterKey"
#define CWMP_NODE_PATH              "InternetGatewayDevice.ManagementServer"
#define BASE_CWMP_NODE_PATH         "^InternetGatewayDevice\\.ManagementServer"
#define TR069_DEFAULT_CONN          "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"
#define	TR069_CONNREQURL			"InternetGatewayDevice.ManagementServer.ConnectionRequestURL"
#define	TR069_MNG_CONN			"InternetGatewayDevice.ManagementServer.X_TWSZ-COM_ManageWANConnectionPath"
#define Tr069_MNG_UP_INTERFACE      "InternetGatewayDevice.UploadDiagnostics.Interface"
#define Tr069_MNT_DWON_INTERFACE    "InternetGatewayDevice.DownloadDiagnostics.Interface"

#define Tr069_LAN_STATIC_DNS "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers"
#define Tr069_UseIPRoutersAsDNSServer "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_UseIPRoutersAsDNSServer"


#define TR069_L3FORWARDING_PATH "InternetGatewayDevice.Layer3Forwarding."

#ifdef TR111
#define CWMP_NODE_TR111_PATH        "InternetGatewayDevice.ManagementServer.ManageableDevice"

#define TR111_NAME_MANAGEABLEDEVICENUMBEROFENTRIES "ManageableDeviceNumberOfEntries"
///#define TR111_NAME_MANAGEABLEDEVICENOTIFICATIONLIMIT "ManageableDeviceNotificationLimit"
///#define TR111_NAME_CHECKINTERVAL "X_TWSZ-COM_CheckInterval"

#define TR111_NAME_MANUFACTUREROUI  "ManufacturerOUI"
#define TR111_NAME_SERIALNUMBER     "SerialNumber"
#define TR111_NAME_PRODUCTCLASS     "ProductClass"
#define TR111_NAME_CLIENTIP         "X_TWSZ-COM_ClientIP"
#define TR111_NAME_HOST         	"Host"

#define TR111_VALUE_COMMON_SIZE             64
#define TR111_VALUE_MANUFACTUREROUI_SIZE    6
#define TR111_VALUE_CLIENTIP_SIZE           16
#define TR111_VALUE_HOST_SIZE               1024
/* by far, max of client which can connect to gw is 4, it can be changed at any moment */
#define TR111_MAX_CLIENTIP                  4

/* Process ID used for ping */
#define TR111_MSG_ID_STATE_PING             0x111001
/* Timer name used for TR111 ping periodically */
#define TR111_TNAME_PING            "t111-ping"

#endif

#define TR069_NAME_URL              "URL"
#define TR069_NAME_USERNAME         "Username"
#define TR069_NAME_PASSWORD         "Password"
#define TR069_NAME_CONNQSTURL       "ConnectionRequestURL"
#define TR069_NAME_CONNQSTENABLE  "ConnectionRequestEnable"
#define TR069_NAME_CONNQSTUSERNAME  "ConnectionRequestUsername"
#define TR069_NAME_CONNQSTPASSWORD  "ConnectionRequestPassword"
#define TR069_NAME_PERIODICINFORM   "PeriodicInformEnable"
#define TR069_NAME_PERIODICINTERVAL "PeriodicInformInterval"
#define TR069_NAME_PARAMETERKEY     "ParameterKey"
#define TR069_NAME_CAFILEPATH       "X_TWSZ-COM_CAFilePath"

#define TR069_NAME_COMMANDKEY       "X_TWSZ-COM_CommandKey"
#define TR069_NAME_FAULTSTRUCT      "X_TWSZ-COM_FaultStruct"
#define TR069_NAME_DOWNLOADSTARTTIME    "X_TWSZ-COM_DownloadStartTime"
#define TR069_NAME_DOWNLOADCOMPLETTIME  "X_TWSZ-COM_DownloadCompleteTime"

/*
 * 1.定义和tr069fe交互的信息
 */
// Process IDs used by TR-069 Back End
#define TR069_MSG_ID_START_FE               0xbe001
#define TR069_MSG_ID_START_COMLINE_DOWNLOAD 0xbe002
#define TR069_MSG_ID_START_COMLINE_UPLOAD   0xbe003
#define TR069_MSG_ID_GET_CFG_FILE           0xbe004
#define CASUBJECTLEN                      0x100
/* Timer names used in TR-069 Back-End */
#define TR069_TNAME_PERIODIC        "t69-prdc"
#define TR069_TNAME_DNLOAD          "t69-dnload"
#define TR069_TNAME_UPLOAD          "t69-upload"
#define TR069_TID_INFORM_DELAY      "t69-ifm-dly"
#define TR069_ITD_SCHEDULE_DELAY    "t69-schd-dly"

/* Path of the Config file */
#define TR069_PATH_CONFIG           "/var/tmp/cfg.xml"
#define TR069_PATH_LOG              "/var/log/sysevent.txt"
#define TR069_PATH_CAFILE           "/var/tmp/cacert.pem"
#define TR069_PATH_TEMPFILE         "/var/tmp/tempfile"
#define TR069_PATH_CERT				"/var/tmp/cert/"

#define TR069_GETVAL(node)          ((char *)COMM_GetNodeValue(CWMP_NODE_PATH"."node, CACHE_CFG))
#define TR069_STR_ARR(apc)  apc, sizeof(apc)/sizeof(apc[0])

#ifndef TR069BE_INFO_TO_TR069FE
#define TR069BE_ACSURL_TO_TR069FE "/var/tr069_acsurl"
#define TR069BE_LOCALCN_TO_TR069FE "/var/tr069_localcn"
#endif

/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/
/*
 * RPC处理状态机定义
 */
enum EN_TR069_FrontEndState {feDown, feUp, feWait};
enum EN_TR069_ResetState    {resetIdle, rebootPend, facResetPend, DiagnosticPend };
#ifdef TR111
enum EN_TR111_PingState     {pgDown, pgUp, pgWait};
#endif

/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/


/*
  *定义ManageSever的CFG结构
  *为配置命名和TR098统一，没有采用TBS命名格式
  */
#ifdef TR111
typedef struct tag_ST_TR069_ManagementServerConfig {
    char ManageableDeviceNumberOfEntries[8];
    char COM_ClientIP[TR111_MAX_CLIENTIP][16];  /* ip address length is 16*/
    char PINGingIP[16];                         /* ip which is ping ing now*/
} ST_TR069_ManagementServerConfig;

typedef struct tag_ST_TR111_ManageableDeviceConfig{
    char ManufacturerOUI[TR111_VALUE_MANUFACTUREROUI_SIZE + 1];
    char SerialNumber[TR111_VALUE_COMMON_SIZE + 1];
    char ProductClass[TR111_VALUE_COMMON_SIZE + 1];
    char COM_ClientIP[TR111_VALUE_CLIENTIP_SIZE + 1];
	char Host[TR111_VALUE_HOST_SIZE + 1];
}ST_TR111_ManageableDeviceConfig;
#endif

typedef struct tag_ST_TR069_Private
{
    enum EN_TR069_FrontEndState enFEState;
    unsigned long ulFrontEndPID;
    enum EN_TR069_DownLoadState enDLState;
    unsigned long ulDownLoadPID;
    enum EN_TR069_UpLoadState enULState;
    unsigned long ulUpLoadPID;
    enum EN_TR069_ResetState enResetState;
#ifdef TR111
    enum EN_TR111_PingState enPingState;
    unsigned char ulPingPID;
    int iClientIdx; /* index to COM_ClientIP, indicateing which client is pinging currently*/
#endif
    int iNotificationPending;
    int iOnURLChange;
    int iError;
    int iGetCfgFile;
    char* pcErrorMsg;
    unsigned long ulPeriodicTimerID;
    unsigned long ulDownloadTimerID;
    unsigned long ulUploadTimerID;
    unsigned long ulScheduleTimerID;
#ifdef TR111
    unsigned long ulPingTimerID;
#endif
    unsigned int iDownloadType;
    unsigned int iUploadType;
} ST_TR069_Private;

typedef struct
{
    int iEnable;
    const char *pcConnType;
    const char *pcConnStatus;
    const char *pcSvcType;
} ST_TR069_MNG_INFO;

typedef struct
{
    const char *pcIp;
    const char *pcGw;

    const char *pcDnsEnable;
    const char *pcDns;
    const char *pcDnsOver;
    const char *pcUsrDns;

    const char *pcDev;

} ST_TR069_MNG_ADDR_INFO;


#ifdef CONFIG_APPS_LOGIC_WAN_SERVICETYPE

typedef struct tag_ST_TR069_WanConnInfo {
	int iActive;									/* 非0 - Online; 0 - Offline */
	char acPath[TR069_MAX_PATH_LEN];
	char acDeviceName[TR069_MAX_WAN_DEV_NAME_LEN];
	char acExternalIP[TR069_MAX_WAN_IP_LEN];
	char acRemoteIP[TR069_MAX_WAN_IP_LEN];
	char acDnsServers[TR069_MAX_DNSSERVERS_LEN];
	struct tag_ST_TR069_WanConnInfo *pstNext;
} ST_TR069_WanConnInfo;

typedef struct tag_ST_TR069_LanDeviceInfo {
	char acPath[TR069_MAX_PATH_LEN];
	char acDeviceName[TR069_MAX_LAN_DEV_NAME_LEN];
	struct tag_ST_TR069_LanDeviceInfo *pstNext;
} ST_TR069_LanDeviceInfo;

#endif

/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/

int TR069_CheckURL(const char *pszValue);
int TR069_CheckCAfile(const char *pszValue);
int TR069_UploadCAfile(char *pszPath,char *pszValue);
int TR069_ReadCAfileFromCfg(void);
int TR069_MaintainComplete(char *pszPath,char *pszValue);
int TR069_SetCAfilePath(char *pszPath,char *pszValue);
int TR069_URLChange(char *pszPath,char *pszValue);
int TR069_CheckConflict(char * pszPath,char *pszValue);
int TR069_DNSChange(ST_MSG* pstMsg);
int TR069_CheckCAname(const char *pszValue);
int TR069_ReadSubjet( char *Subjetvalue);
int TR069_Readtype( char *Subjetvalue);
/*
 * 定义消息处理相关的函数
 */

int TR069_HandleClearEvents(ST_MSG* pstMsg);
int TR069_HandleHttpdConnect(ST_MSG* pstMsg);
int TR069_HandleDiagnosticComplete(ST_MSG* pstMsg);
int TR069_HandleTimerTimeout(ST_MSG* pstMsg);
int TR069_CFMCommitHandle(char *pszPath,char *pszValue);
int TR069_Be_ModuleInit();
int TR069_Be_ModuleDestroy();
int TR069_SetParameterKey(ST_MSG* pstMsg);
int TR069_HandleReboot(ST_MSG* pstMsg);
int TR069_HandleFactoryReset(ST_MSG* pstMsg);
int TR069_HandleScheduleInform(ST_MSG* pstMsg);
int TR069_HandleDownload(ST_MSG* pstMsg);
int TR069_HandleUpload(ST_MSG* pstMsg);
int TR069_HandleTransferComplete(ST_MSG* pstMsg);
int TR069_StartFrontEnd(unsigned int uiForceRestart);
int TR069_Tr069feProcCtrl(const char *pcName, unsigned long ulInstNo, unsigned char ucOpt);
int TR069_SessionTerminated (void);
int TR069_ResetPeriodicInform (int forcetoexec_tr069fe);
int TR069_StopPeriodInform(void);
int TR069_PostReboot();
int TR069_PostFactoryReset();
int TR069_RegIntervalTimer(unsigned short usMID, unsigned long *ulTimerID, unsigned long ulInterval, char* pcTimerName);
int TR069_HandleProcState (ST_MSG* pstMsg);
int TR069_MsgSendSignalAckHandler(ST_MSG *pstMsg);
int TR069_HandleNotifications (ST_MSG* pstMsg);
int TR069_HandleTimerReport (ST_MSG* pstMsg);
int TR069_InitUPFile();
int TR069_HandleMonitorEvent(ST_MSG* pstMsg);
int TR069_SetConnectRequestURL(char *pcRoute);
int TR069_HandleWanConnEst(ST_MSG *pstMsg);
int TR069_TimeChangeHandle(ST_MSG* pstMsg);
int TR069_DefaultRouteChangeHandle(ST_MSG* pstMsg);
int TR069_UpgradeResultHandle(ST_MSG* pstMsg);

#ifdef CONFIG_APPS_LOGIC_WAN_SERVICETYPE

int TR069_HandleWanServiceTypeConnectEst(ST_MSG *pstMsg);
int TR069_WanSetChgMsgProc(ST_MSG *pstMsg);
int TR069_UpdateMngConn(const char *pcOldMng, const char *pcNewMng);
int TR069_URLtoIP(char *pcURL,char *pcIpAddr, char *pcPort , char *pcRoute, char *pcConnection);
int TR069_URLChange(char *pszPath,char *pszValue);
int TR069_SetForwardFilterRule(const char *pcWanPath, char *pcDevName, int iAction);
int TR069_SetOutputFilterRule(const char *pcWanPath, char *pcDevName,  int iAction);
int TR069_ManageConnectionCtlChain(void);
int TR069_HandleWanConnFin(ST_MSG *pstMsg);
int TR069_AddRPCMethodsRoute(char *pcURL, char *pcFlag);
int TR069_DelRPCMethodsRoute(char *pcURL, char *pcFlag);


#endif
#ifdef TR111

int TR111_HandleDeviceAdd(ST_MSG* pstMsg);
int TR111_HandleDeviceDel(ST_MSG*pstMsg);
int TR111_MsgProcOutputHandler(ST_MSG *pstMsg);
int TR111_PING_CheckValid(void *pInstance, const char *pszOutInfo);
int TR111_StopPeriodPing(void);
int TR111_ResetPeriodicPing (void);

#endif


int TR069_SaveEventsToFlash(char *szEventValue);
int TR069_AddEventSavedInFlash(void);



/******************************************************************************
*                                 END                                        *
******************************************************************************/
#endif

