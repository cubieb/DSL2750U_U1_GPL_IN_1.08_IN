#include "tbsmsg.h"
#include "tbstype.h"
#include "tbsutil.h"
#include "tbserror.h"
#include "warnlog.h"
#include "common.h"
#include "tbsflash.h"

#include "cmmif.h"
#include "snmpProcess.h"
#include "tbs-private/remoteNodes.h"
#include "tbs-private/adslNodes.h"


#define SNMPA_GET_UCHAR(p, value) \
    value = *( (unsigned char *)p ); \
    p += sizeof(unsigned char);

#define SNMPA_GET_ULONG(p, value) \
    value = *( (unsigned long *)p ); \
    p += sizeof(unsigned long);

static char gszUploadDestPath[64];

/*Variables for upload and download*/
#define _FIRMWARE_FILE   "/var/image.img"
#define _CONFIG_FILE     "/var/config.xml"
#define _LOG_FILE "/var/log/sysevent.txt"
enum 
{
    NONE,
    IMG_WRITE,
    CONFIG_READ,
    CONFIG_WRITE
};

#define TBSTrap_STR_SIZE                   32 /* 最常用的缓存大小 */
#define WAN_SEND_SECOND                      3  /* Wan的地址变更,则发送trap的次数 */
#define WAN_SEND_TIME                        60 /* Wan的地址变更,每次发送的间隔时间 */
#define TRAP_POLL_INTERVAL           5  /* every X second (s) */

typedef struct __wan_if_manage_t
{
    unsigned char szIp[5];
	int iWanState, iWanStateTi;
    char sIfName[ TBSTrap_STR_SIZE ];

	struct __wan_if_manage_t *pNext;
}wan_if_manage_t;


static wan_if_manage_t *wanMgr_head = NULL;
#define MSG_SNMPA_STATE_DOWNLOAD 0x8989 /*msg id for download when executing comline*/
#define MSG_SNMPA_STATE_UPLOAD 0x9898 /*msg id for upload when executing comline*/
enum EN_SNMPA_ComlineState {clDown, clUp, clWait};
typedef struct tag_ST_SNMPA_Private
{
	enum EN_SNMPA_ComlineState enComlineState;
        unsigned long ulComlinePID;
}ST_SNMPA_Private;
/*marks the status of executing comline*/
ST_SNMPA_Private g_stSNMPAPrivate = {clDown, 0x0};

#define SNMPA_RECEIVE_TIME_OUT 5


int isChangeConfig = 1 ;
static int isFirmwareFile = ZERO;  /*FILE_TYPE*/
static int gisDownload = EMPTY;  /*ACTION_TYPE*/
static oid download_upload_oid[] = {1, 3, 6, 1, 4, 1, 3902, 1, 8, 1, 8, 10, 0, 9};

static traps_info authFailedTrapMsg     ={"Auth Failed",  21306,   {1,3,6,1,6,3,1,1,5,6},10};

/* 执行comline的输出匹配信息列表 */
/*process output 
 *(匹配"Done!"一行就表示成功了)0-传送成功；
 *(匹配"Fatal Error"这一行就可以知道失败的原因了)2－传送失败 trap，
 */
char *apcRegExpress[] = {
"(unspecified)", 
"(Sorry)",
"Fatal Error:(.*)\r\n",
"(Done!)"                  /*修改了这个的index，DOWNLOAD_DONE_INDEX也必须更新*/
};
#define DOWNLOAD_DONE_INDEX 3 /*in apcRegExpress array, (Done!) 's index*/
ST_ProcOutMatchInfo  g_astSNMPMatchInfoList[ ] = {
    {NULL, SNMPA_SendDl_UlFailTrap_paraEmpty},
    {NULL,  SNMPA_SendDl_UlFailTrap_paraEmpty},
    {NULL, SNMPA_SendDl_UlFailTrap},
    {NULL, SNMPA_SendDl_UlSuccTrap}
};
static int gisDownloadSuccessful = 0; /*mark if the download action is successful*/
#define TRAP_START "onetrapwhenstart"



//////////////////////START HERE--------------------------------------------------------------------
/* 这些鸟东西来自中兴"第三方AP接入中兴网管接口规范V1.0"，如果它有变化，注意修改这里就是了............
sysUpTime  	.1.3.6.1.2.1.1.3	TimeTicks	上电时间,是协议规定需实现的
snmpTrapOID	.1.3.6.1.6.3.1.1.4.1（替换实际的告警OID）	SnmpOid	TrapOid（可以填写告警描述信息）
nodeId 	.1.3.6.1.4.1.3902.1.8.1.8.1.1	DisplayString	设备标识，在全网中必须唯一，可引用Mac地址，也可在开通时设定
alarmCause	.1.3.6.1.4.1.3902.1.8.1.8.10.5	Integer	告警码
alarmLevel	.1.3.6.1.4.1.3902.1.8.1.8.10.3	Integer	告警级别，1-严重；2-重要；3-一般；4-轻微
alarmEventTime	.1.3.6.1.4.1.3902.1.8.1.8.10.6	TimeTicks	告警时间
alarmTitle      	.1.3.6.1.4.1.3902.1.8.1.8.10.8	DisplayString	告警描述,与告警码对应（可选）
alarmAdditionalInfo     	.1.3.6.1.4.1.3902.1.8.1.8.10.9	DisplayString	告警附加信息，其他辅助说明
*/
#ifndef IGD_ENTERPRISE_OID
#define IGD_ENTERPRISE_OID		3902
#endif
#ifndef IGDMIB_OID
#define IGDMIB_OID				1
#endif
#ifndef IGD_OID_PREFIX
#define IGD_OID_PREFIX			1,3,6,1,4,1,IGD_ENTERPRISE_OID,IGDMIB_OID
#endif

/*
 * return 0 - successful, non-zero - fail
 * NOTE - when alarmCode is 0 1 2, alarmLevel, alarmEventTime and alarmTitle don't be supported
 */
void tw_AddTrapMsg(netsnmp_variable_list **vars, traps_info* pTrapInfo)
{
    static oid objId_snmpTrap[] = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 };
    static oid entPhySN_oid[] = { 1, 3, 6, 1, 2, 1, 47, 1, 1, 1, 1, 11 };
     
    char szNodeId[64] = {0}; 
    unsigned long ulEventTime = time(NULL);

    oid tw_snmptrap_preoid[][10] = {
						        {IGD_OID_PREFIX, 2, 1}, //alarm id
						        {IGD_OID_PREFIX, 2, 2}, //event time
						        {IGD_OID_PREFIX, 2, 3}}; //alarm descrition
   
   /* add in the trap definition object */
    snmp_varlist_add_variable(vars,objId_snmpTrap, OID_LENGTH(objId_snmpTrap),
    	           ASN_OBJECT_ID, (u_char *) pTrapInfo->trap_oid,pTrapInfo->trap_oid_len*sizeof(oid));

    /*"InternetGatewayDevice.DeviceInfo."是在文件scalarNodes.c中定义的TR069_ROOT_DeviceInfo*/
    if(CMM_GetStr("InternetGatewayDevice.DeviceInfo.SerialNumber", szNodeId, sizeof(szNodeId), NULL, 0) != CMM_SUCCESS)
    {
        SNMPA_TRACE("Failed to read SerialNumber!\n");
        return;
    }

    /* SerialNumber */
    snmp_varlist_add_variable(vars, entPhySN_oid, OID_LENGTH(entPhySN_oid),ASN_OCTET_STR, (u_char *)szNodeId, strlen(szNodeId));

    /*  alarm id*/
    snmp_varlist_add_variable(vars, tw_snmptrap_preoid[0], OID_LENGTH(tw_snmptrap_preoid[0]),
    	                                           ASN_INTEGER, (u_char *)&pTrapInfo->trap_code, sizeof(pTrapInfo->trap_code));

	/* EventTime */
    snmp_varlist_add_variable(vars, tw_snmptrap_preoid[1], OID_LENGTH(tw_snmptrap_preoid[1]),
    	                                           ASN_TIMETICKS, (u_char *)&ulEventTime, sizeof(ulEventTime));

	/* description msg*/
    snmp_varlist_add_variable(vars, tw_snmptrap_preoid[2], OID_LENGTH(tw_snmptrap_preoid[2]),
    	                                           ASN_OCTET_STR, (u_char *)pTrapInfo->trap_msg, strlen(pTrapInfo->trap_msg));
}

/*
 * return 0 - successful, non-zero - fail
 * NOTE - when alarmCode is 0 1 2, alarmLevel, alarmEventTime and alarmTitle don't be supported
 */
void ZTE_AddTrapPremsg(netsnmp_variable_list **vars, 
                                                 oid *trapoid, unsigned long ulTrapOidLen,
                                                 unsigned long ulAlarmCode, 
                                                 unsigned long ulAlarmLevel, char *pszAlarmTitle, char *pszAlarmInfo)
{
  #ifndef TBS_SNMP_APP
    int idx = 0;
    char szNodeId[64] = {0}; 
    unsigned long ulEventTime = time(NULL);
    oid zte_snmptrap_oid[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};
    oid zte_snmptrap_preoid[][13] = {
        {IGD_OID_PREFIX, 8, 1, 8, 1, 1},
        {IGD_OID_PREFIX, 8, 1, 8, 10, 5},
        {IGD_OID_PREFIX, 8, 1, 8, 10, 3},
        {IGD_OID_PREFIX, 8, 1, 8, 10, 6},
        {IGD_OID_PREFIX, 8, 1, 8, 10, 8},
        {IGD_OID_PREFIX, 8, 1, 8, 10, 9}};
     SNMPA_TRACE("Start...\n"); //wei

    //strlen的参数是NULL时会导致段错误，所以需要对参数判断
    if ((pszAlarmTitle == NULL) || (pszAlarmInfo == NULL))
    {
        SNMPA_TRACE("Error in-parameter!Some parameter can not be NULL\n"); 
        return ;
    }
    
    /*
   * add in the trap definition object
   */
    snmp_varlist_add_variable(vars,
    	zte_snmptrap_oid, OID_LENGTH(zte_snmptrap_oid), /* the snmpTrapOID.0 variable */
    	ASN_OBJECT_ID, (u_char *) trapoid, ulTrapOidLen);

    /*"InternetGatewayDevice.DeviceInfo."是在文件scalarNodes.c中定义的TR069_ROOT_DeviceInfo*/
    if(CMM_GetStr("InternetGatewayDevice.DeviceInfo.SerialNumber", szNodeId, sizeof(szNodeId), NULL, 0) != CMM_SUCCESS)
    {
        SNMPA_TRACE("Failed to read SerialNumber!\n");
        return;
    }

    snmp_varlist_add_variable(vars, zte_snmptrap_preoid[idx], OID_LENGTH(zte_snmptrap_preoid[idx++]),
    	                                           ASN_OCTET_STR, (u_char *)szNodeId, strlen(szNodeId));

    snmp_varlist_add_variable(vars, zte_snmptrap_preoid[idx], OID_LENGTH(zte_snmptrap_preoid[idx++]),
    	                                           ASN_INTEGER, (u_char *)&ulAlarmCode, sizeof(ulAlarmCode));

    /*"第三方AP接入中兴网管接口规范V1.0"中的"2.	传送结果通知规定当alarmCode<3时的靠告警是不需要下列三项的"*/
    if(ulAlarmCode > 3)
    {
        snmp_varlist_add_variable(vars, zte_snmptrap_preoid[idx], OID_LENGTH(zte_snmptrap_preoid[idx++]),
        	                                           ASN_INTEGER, (u_char *)&ulAlarmLevel, sizeof(ulAlarmLevel));

        snmp_varlist_add_variable(vars, zte_snmptrap_preoid[idx], OID_LENGTH(zte_snmptrap_preoid[idx++]),
        	                                           ASN_TIMETICKS, (u_char *)&ulEventTime, sizeof(ulEventTime));

        snmp_varlist_add_variable(vars, zte_snmptrap_preoid[idx], OID_LENGTH(zte_snmptrap_preoid[idx++]),
        	                                           ASN_OCTET_STR, (u_char *)pszAlarmTitle, strlen(pszAlarmTitle));
    }
SNMPA_TRACE("Exit...\n"); //wei
    snmp_varlist_add_variable(vars, zte_snmptrap_preoid[idx], OID_LENGTH(zte_snmptrap_preoid[idx++]),
    	                                           ASN_OCTET_STR, (u_char *)pszAlarmInfo, strlen(pszAlarmInfo));
   #endif
}
//////////////////////END HERE--------------------------------------------------------------------


/*loop to receive message from pc and execute them*/
void snmpa_handleMessage()
{
    ST_MSG *pstMsg = NULL;
    if (MSG_OK == MSG_ReceiveMessage(&pstMsg, SNMPA_RECEIVE_TIME_OUT))
    {
        /* 处理接收到的消息，忽略返回值 */
        snmpa_MsgProcess(pstMsg);
        safe_free_msg(pstMsg);
    }
}
/*-------------------------------------------------------------------------
 * send a alarm when authentication failed
 */
static void send_authFailedAlarm()
{
    netsnmp_variable_list *notification_vars = NULL;
	tw_AddTrapMsg(&notification_vars,&authFailedTrapMsg);
    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

int snmpa_MsgProcess(void *pstMsg_void)
{
    int nRet = TBS_SUCCESS;
    ST_MSG *pstMsg = (ST_MSG*)pstMsg_void;
    if (NULL == pstMsg)
    {
        SNMPA_TRACE("NULL pointer encounterd.\n");
        return TBS_FAILED;
    }

    SNMPA_TRACE("Received Msg From(%#x) (len = %lu type = %u)\n",
        pstMsg->stMsgHead.usSrcMID, pstMsg->stMsgHead.ulBodyLength, pstMsg->stMsgHead.usMsgType);

    switch (pstMsg->stMsgHead.usMsgType)
    {
   #ifndef TBS_SNMP_APP
        case MSG_PC_START_PROC_ACK: /*succeed in processing comline: 1－传送中 trap*/
            SNMPA_HandleProcStart(pstMsg);
            break;
        case MSG_PC_PROC_OUTPUT: 
            SNMPA_MsgProcOutputHandler(pstMsg);
            break;
        case MSG_PC_PROC_STATE: /*process exit*/
            SNMPA_HandleProcState(pstMsg);
            break;
        case MSG_CMM_BACKUP_ACK:
            SNMPA_StartComline(UPLOAD, CONFIG_FILE, gszUploadDestPath);
            break;
   #endif
        case MSG_WAN_CONN_SET_CHNG:
            //process_wanManageIpChange();
			break ;
			
        case MSG_SNMPA_AUTH_FAILED:
			send_authFailedAlarm();
	        break;

        default:
            nRet = TBS_FAILED;
            SNMPA_TRACE("Recieved INGNORED message, Its id is %d.\n", pstMsg->stMsgHead.usMsgType);
            break;
    }

    return nRet;
}

/* 用于处理进程输出匹配消息，解析PC模块发送过来进程输出匹配消息，并调用相应的匹配处理函数 */
void SNMPA_MsgProcOutputHandler(ST_MSG *pstMsg)
{
  #ifndef TBS_SNMP_APP
    char *pcPos = NULL;
    unsigned long ulProcId = 0;
    unsigned char ucCount = 0;
    unsigned char ucMatchNo = 0;
    unsigned char i = 0;
SNMPA_TRACE("Has come into ...\n"); //wei
    if ( NULL == pstMsg )
    {
        SNMPA_TRACE("TBS_NULL_PTR");
        return;
    }

    pcPos = pstMsg->szMsgBody;
    /* 进程ID */
    SNMPA_GET_ULONG(pcPos, ulProcId);
    /* 匹配个数*/
    SNMPA_GET_UCHAR(pcPos, ucCount);
SNMPA_TRACE("ucCount: %d\n", ucCount); //wei
    for (i=0; i<ucCount; i++)
    {
        /* 匹配号*/
        SNMPA_GET_UCHAR(pcPos, ucMatchNo);
        SNMPA_TRACE("ucMatchNo = %u, Output = %s\n", ucMatchNo, pcPos);
        if (ucMatchNo>0 && ucMatchNo<=sizeof(g_astSNMPMatchInfoList) / sizeof(ST_ProcOutMatchInfo))
        {
            if(ucMatchNo-1 == DOWNLOAD_DONE_INDEX) gisDownloadSuccessful = 1; /*download the needed file, flag it*/
            g_astSNMPMatchInfoList[ucMatchNo - 1].pMatchFunc(NULL, pcPos);
            break; /*wei add this for breaking loop, least work to resolve this problem...*/
            /*本模块是单匹配，如果有多个匹配，取第一个就跳出了，
            因为有多个匹配，肯定是出错了，上面定义的匹配条件的顺序
            能保证此时跳出发送的是出错trap*/
        }
        pcPos += safe_strlen(pcPos) + 1;
    }

    return;
   #endif
}

/*download or upload failure trap*/
int  SNMPA_SendDl_UlFailTrap_paraEmpty(void *pInstance, const char *pszOutInfo)
{
    #ifndef TBS_SNMP_APP
    netsnmp_variable_list *vars = NULL;
    char szWarnMessage[256] = {0};

    snprintf(szWarnMessage, sizeof(szWarnMessage), "Bad command syntax:parameter(s) needed!\n");
    if(pszOutInfo)
        strcat(szWarnMessage, pszOutInfo);

    /*告警码:0-传送成功；1－传送中；2－传送失败*/
    ZTE_AddTrapPremsg(&vars, download_upload_oid, sizeof(download_upload_oid), 2, 0, NULL, szWarnMessage); 
    send_v2trap(vars);
    snmp_free_varbind(vars);
	#endif
}

/*download or upload failure trap*/
int  SNMPA_SendDl_UlFailTrap(void *pInstance, const char *pszOutInfo)
{
    #ifndef TBS_SNMP_APP
    netsnmp_variable_list *vars = NULL;
    char szWarnMessage[256] = {0};

    if(gisDownload == DOWNLOAD) snprintf(szWarnMessage, sizeof(szWarnMessage)-sizeof("Download - "), "Download - %s", pszOutInfo);
    else if(gisDownload == UPLOAD) snprintf(szWarnMessage, sizeof(szWarnMessage)-sizeof("Upload - "), "Upload - %s", pszOutInfo);
    else return TBS_FAILED;

    /*告警码:0-传送成功；1－传送中；2－传送失败*/
    ZTE_AddTrapPremsg(&vars, download_upload_oid, sizeof(download_upload_oid), 2, 0, NULL, szWarnMessage); 
    send_v2trap(vars);
    snmp_free_varbind(vars);
   #endif
}

/*download or upload success trap*/
int  SNMPA_SendDl_UlSuccTrap(void *pInstance, const char *pszOutInfo)
{
    #ifndef TBS_SNMP_APP
    netsnmp_variable_list *vars = NULL;
    char szWarnMessage[48] = {0};

    if(gisDownload == DOWNLOAD) strcpy(szWarnMessage, "Download successful! ");
    else if(gisDownload == UPLOAD) strcpy(szWarnMessage, "Upload successful! ");
    else if(safe_strcmp(pszOutInfo, "1") == 0) strcpy(szWarnMessage, "Upgrade successful! "); /*上一次下载成功后的重启trap*/
    else return TBS_FAILED;

    /*告警码:0-传送成功；1－传送中；2－传送失败*/
    ZTE_AddTrapPremsg(&vars, download_upload_oid, sizeof(download_upload_oid), 0, 0, NULL, szWarnMessage); 
    send_v2trap(vars);
    snmp_free_varbind(vars);
	#endif
}

/*downloading or uploading trap*/
int SNMPA_HandleProcStart (ST_MSG* pstMsg)
{
    #ifndef TBS_SNMP_APP
    netsnmp_variable_list *vars = NULL;
    char szWarnMessage[32] = {0};
    unsigned long ulMsgId = 0;

    if(gisDownload == DOWNLOAD)
    {
        ulMsgId = MSG_SNMPA_STATE_DOWNLOAD;
        strcpy(szWarnMessage, "Downloading...");
    }
    else if(gisDownload == UPLOAD)
    {
        ulMsgId = MSG_SNMPA_STATE_UPLOAD;
        strcpy(szWarnMessage, "Uploading...");
    }
    else return TBS_FAILED;

    if(ulMsgId == pstMsg->stMsgHead.ulMsgID)
    {
        if( 0x0 != * (unsigned long *) pstMsg->szMsgBody)
        {
            g_stSNMPAPrivate.enComlineState = clDown;
            SNMPA_TRACE("Start Process comline error !\n");
        }
        else /*succeed in launching comline, then send "ing" trap*/
        {
            SNMPA_TRACE("Start PID = %lu\n", *(unsigned long*)(pstMsg->szMsgBody+ sizeof(unsigned long)));
            g_stSNMPAPrivate.enComlineState = clUp;
            g_stSNMPAPrivate.ulComlinePID = *(unsigned long*)(pstMsg->szMsgBody+ sizeof(unsigned long));

            /*告警码:0-传送成功；1－传送中；2－传送失败*/
            ZTE_AddTrapPremsg(&vars, download_upload_oid, sizeof(download_upload_oid), 1, 0, NULL, szWarnMessage); 
            send_v2trap(vars);
            snmp_free_varbind(vars);
            
            SNMPA_TRACE("Start Process comline success!\n");
        }    
    }
    else SNMPA_TRACE("Received a start message unexpected!\n");

	#endif
    return TBS_SUCCESS;
}


/**********************************************************************
  snmpa执行处理进程退出操作
  功能说明:处理进程退出操作
  输入参数: 
                          pstMsg 接收到退出消息

  输出参数: 
  返 回 值: TBS_SUCCESS表示成功
  修订记录:
           1.作者: weichongx
             日期: 2008-11-14
             内容: 创建函数
**********************************************************************/
int SNMPA_HandleProcState (ST_MSG* pstMsg)
{
    #ifndef TBS_SNMP_APP
    /*判断是否为SNMPA的PID*/
    SNMPA_TRACE(" ProcID %lu quit!\n",  * (unsigned long *)pstMsg->szMsgBody);

    if(g_stSNMPAPrivate.ulComlinePID == *(unsigned long*)pstMsg->szMsgBody)
    {
        g_stSNMPAPrivate.enComlineState = clDown;
        g_stSNMPAPrivate.ulComlinePID = 0;

        SNMPA_TRACE("SNMPA: comline finished!\n"); 
        if(gisDownload==DOWNLOAD || gisDownloadSuccessful==1) /*不需要考虑失败的情况，反正update找不到目标不会出问题，自然死亡*/
        {
            gisDownloadSuccessful = 0; /*reset it*/
            SNMPA_SaveEventsToFlash(TRAP_START); /*save flag to flash when upgrader is done, we need send trap*/
            
            SNMPA_TRACE("SNMPA: comline finished! launch the upgrader now...\n");
            SNMPA_do_upgrade(isFirmwareFile);
        }
    }
    else
    {
    SNMPA_TRACE("unknown ProcID %lu quit!\n",  * (unsigned long *)pstMsg->szMsgBody);
    }

	#endif
    return TBS_SUCCESS;
}

/*
/sbin/comline -o /var/image.img -get http://192.168.1.100/test/fuck.img - for DOWNLOAD
/sbin/comline -put /var/tmp/tr069up -dest http://192.168.1.100/test/tr069 - for UPLOAD
whichFile - two types file, 1 config file; 2 firemware image file
*/
int SNMPA_StartComline(int action, int whichFile, char *szRemoteURL)
{
    #ifndef STB_SNMP_APP
    /*给PC模块发送一个消息，启动comline*/
    int itmp = 0;
    unsigned long ulMsgId = 0;
    ST_PC_PROC_START pProcStart;
    char *args[] = {"/sbin/comline", 0, 0, 0, 0};

    if(g_stSNMPAPrivate.enComlineState == clDown)
    {
        gisDownload = action;
        if(gisDownload == DOWNLOAD) /*Download*/
        {
            args[1] = "-o";
            args[3] = "-get";
            ulMsgId = MSG_SNMPA_STATE_DOWNLOAD;
        }
        else if(gisDownload == UPLOAD) /*Upload*/
        {
            args[1] = "-put";
            args[3] = "-dest";
            ulMsgId = MSG_SNMPA_STATE_UPLOAD;
        }
        else return TBS_FAILED;

        /*local file*/
        isFirmwareFile = whichFile; /*record file type*/
        if(whichFile == IMG_FILE) args[2] = _FIRMWARE_FILE;
        else if(whichFile == CONFIG_FILE) args[2] = _CONFIG_FILE;
        else args[2] = _LOG_FILE;
        args[4] = szRemoteURL; /*remote url*/
        
        SNMPA_TRACE("%s %s %s %s %s\n", args[0], args[1], args[2], args[3], args[4]);
        
        memset(&pProcStart, 0, sizeof(pProcStart));
        pProcStart.usMID = MID_SNMPA;
        pProcStart.ulMsgID = ulMsgId;
        pProcStart.ucNeedSteady = 0;
        pProcStart.ucExecCount = 0x05;
        pProcStart.ppcExecInfo = args;
        pProcStart.ucMonCount = sizeof(apcRegExpress)/sizeof(apcRegExpress[0]);;
        pProcStart.ppcMonVals = apcRegExpress;
        itmp = PC_StartProcess(&pProcStart);
        if(itmp)
        {
            SNMPA_TRACE("Start comline error %d. \n", itmp);
            return TBS_OUT_OF_SRC;
        }
        
        g_stSNMPAPrivate.enComlineState = clWait;
    }
    else
    {
       SNMPA_TRACE("Attempt to start comline while it is running...\n");
    }
	#endif
    return TBS_SUCCESS;
}

/*After done downloading, do upgrade*/
/*
  ssap/upg/upg_frw.c has been modified, in main() method:
  if(!strcmp(cuser, "SNMPA"))
  {
       sMID = MID_SNMPA;
  }
  has been added for recognizing SNMPA. then upg module can send response to me...
 */
int SNMPA_do_upgrade(int mode)
{
    #ifndef STB_SNMP_APP
    char szCmd[SYS_CMD_LEN] = {0};
    ST_MSG *pstMsg;
    int iRet = 0;
    unsigned long ulResult;
    
    switch(mode)
    {
        case 1:
            sprintf(szCmd, "upgrader -c %d -p " _FIRMWARE_FILE " -u SNMPA", IMG_WRITE);
            tbsSystem(szCmd, 1);  
            break;            
        case 2:
            sprintf(szCmd, "upgrader -c %d -p " _CONFIG_FILE " -u SNMPA", CONFIG_WRITE);
            tbsSystem(szCmd, 1);            
            break;
        default:
            syslog(LOG_ERR, "snmpa: error upgrade mode: %lu\n", mode);
            break;
    }

    sleep(2);
    iRet = MSG_ReceiveMessage(&pstMsg, 5); /*scan the message for this module, check the status*/
    switch (iRet)
    {
        case MSG_OK:
            syslog(LOG_NOTICE, "Receive msg success.");
            break;
        case MSG_TIMEOUT:
            syslog(LOG_ERR, "Receive msg failed.");
            return TBS_FAILED;
        default:
            syslog(LOG_ERR, "Receive msg failed.");
            return TBS_FAILED;
    }
    
    /* 收到响应消息 */
    if (pstMsg->stMsgHead.usSrcMID != MID_UPG)
    {
        syslog(LOG_ERR, "message id error");
        return TBS_FAILED;
    }

    ulResult = *(unsigned long *)pstMsg->szMsgBody;
    if (ulResult != 0)
    {
        syslog(LOG_ERR, "upgrade failed");
        safe_free_msg(pstMsg);
        return TBS_FAILED;
    }

    syslog(LOG_NOTICE, "upgrade success");
    safe_free_msg(pstMsg);
	#endif
    return TBS_SUCCESS;
}

/*Because config.xml must be created when upload it*/
int SNMPA_CreateCfgFile(char *szRemoteURL)
{
    int nRet = TBS_SUCCESS;
	#ifndef TBS_SNMP_APP
    ST_MSG *pstGetAddressMsg = NULL;
    SNMPA_TRACE("Start....\n");

    /*Save this feature value, when the following message acked, we use them for upload*/
    strcpy(gszUploadDestPath, szRemoteURL);
    
    pstGetAddressMsg = (ST_MSG*)MSG_CreateMessage(256);
    if (!pstGetAddressMsg)
     return TBS_FAILED;

    pstGetAddressMsg->stMsgHead.usMsgType = MSG_CMM_BACKUP;
    pstGetAddressMsg->stMsgHead.ulMsgID = 1002; /*random value*/
    pstGetAddressMsg->stMsgHead.usDstMID = MID_CMM;
    pstGetAddressMsg->stMsgHead.usSrcMID = MID_SNMPA;

    safe_strncpy(pstGetAddressMsg->szMsgBody, _CONFIG_FILE, safe_strlen(_CONFIG_FILE)+1);
    pstGetAddressMsg->stMsgHead.ulBodyLength = safe_strlen(_CONFIG_FILE);

    /* Sending  message*/
    nRet = MSG_SendMessage(pstGetAddressMsg);
    safe_free_msg(pstGetAddressMsg);

    if(TBS_SUCCESS != nRet)
    {
        SNMPA_TRACE(" send message error!\n");
    }
    
	#endif
    return nRet;
}

/*----------------------------------------------------------
 *Read traps data from flash methods
 -----------------------------------------------------------*/
 /*
  read value from buf by a specified name, value is formated as name\tvalue\n
*/
#define EVENT_PRE "TRAP"               /*保存在flash区的TRAP标识名称的前缀Event_1开始*/
int SNMPA_ReadEventsFromFlash(char *buf, char *name)
{
    #ifndef TBS_SNMP_APP
       char *ptr = NULL;
	char *pos, *pos1;
	
	ptr = buf;
	while((pos = strchr(ptr, '\n')))
	{
		*pos = 0;
		pos++;
		pos1 = strchr(ptr, '\t');
		if(pos1)
		{
			*pos1 = 0;
			pos1++;
			if(!safe_strcmp(ptr, name)) /*Got it*/
			{
                            SNMPA_TRACE("Read event of %s from flash.\n", pos1); 
                            return TBS_SUCCESS; /*GOT ONE, only one for snmpa*/
		       }
		}
		ptr = pos;
	}
    
       return TBS_FAILED;
	#endif
}

/*
 return: the event number represents how much event saved in flash has added to event list
 有数据的话返回tbs_success
*/
int SNMPA_GetTrapEventSavedInFlash(char *buf)
{
    const int SNMPA_FLASH_SIZE = 256 ;
    int nRet = TBS_FAILED;
    if(buf)
    {
	    int res = ReadOrigiData(OBJ_SNMPA, buf, SNMPA_FLASH_SIZE);
	    if(res > 4) /* event name length must be more than 4 */
	    {
	        buf[res] = 0;
	        nRet = SNMPA_ReadEventsFromFlash(buf, EVENT_PRE);
	        WriteOrigiData(OBJ_SNMPA, "", 1, FLAG_TRUNC); //clear all data after deal
	    }
	    
	    free(buf);
    }
    return nRet;
}


/* write an event to flash for next inform after reboot 
 */
int SNMPA_SaveEventsToFlash(char *szEventValue)
{
    char buf[40] = {0};  /*Eventname length + eventvalue length*/

    if(szEventValue)
    {
        int ret = 0;
        snprintf(buf, sizeof(buf), "%s\t%s\n", EVENT_PRE, szEventValue);
        ret = WriteOrigiData(OBJ_SNMPA, buf, safe_strlen(buf), FLAG_TRUNC);
        SNMPA_TRACE("Saved event of %s to flash, return: %d\n", buf, ret);
    }
    return TBS_SUCCESS;
}
void SNMPA_Send_Comm_Trap(traps_info *pstTraps)
{
    netsnmp_variable_list *vars = NULL;

	tw_AddTrapMsg(&vars,pstTraps);
    send_v2trap(vars);
    snmp_free_varbind(vars);
}

void SNMPA_Send_StartTrap()
{
   //////////////
}
