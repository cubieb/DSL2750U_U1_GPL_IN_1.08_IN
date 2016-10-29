  
/******************************************************************************
  Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
  
  文件名称: wps.h
  文件描述: 
  函数列表: //列出主要函数
  修订记录:
         1. 作者: Hansel He < heyiming@twsz.com>
            日期: 2008-8-11
            内容: 创建文件
 ******************************************************************************/


#ifndef __WPS_H__
#define __WPS_H__

#include "autoconf.h"  //for CONFIG_WLAN_WPS definitions
#include "wps_com.h"
#include "scalarvar.h"

/****************************************************************************** 
 *                                 MACRO                                      * 
 ******************************************************************************/
#define  WPS_SUPPORT_UNCONFIGURE   /* 是否实现了对未配置VAP的支持*/

#define  WPS_PIN_LEN			        8		/* PIN length */
#define  WPS_PIN_BUF_LEN                16      
#define  WPS_MSG_BUF_LEN                256

/* WPS node definitions */
#define	 WLAN_WPS_PATH					"InternetGatewayDevice.X_TWSZ-COM_WLAN.WPS"
#define  WLAN_WPS_ENABLE_PATH			WLAN_WPS_PATH".Enable"
#define  WLAN_WPS_VAP_IDX_PATH			WLAN_WPS_PATH".VAPIndex"
#define  WLAN_WPS_DEV_PIN_PATH			WLAN_WPS_PATH".DevicePIN"
#define  WLAN_WPS_DEFAULT_PIN_PATH		WLAN_WPS_PATH".DefaultPIN"
#define  WLAN_WPS_STATUS_VAR_PATH		WLAN_WPS_PATH".StatusVar"
#define  WLAN_WPS_START_WPS_PATH		WLAN_WPS_STATUS_VAR_PATH".StartWPSSession"
#define  WLAN_WPS_SESSION_STATUS_PATH	WLAN_WPS_STATUS_VAR_PATH".WPSSessionStatus"
#define  WLAN_WPS_CHANGE_PIN_PATH		WLAN_WPS_STATUS_VAR_PATH".ChangePIN"
#define  WLAN_WPS_STATION_PIN_PATH		WLAN_WPS_STATUS_VAR_PATH".StationPIN"
#define  WLAN_WPS_RESET_UNCONF_PATH		WLAN_WPS_STATUS_VAR_PATH".ResetUnconfigured"

/* WPS node constant definitions */
#define  WPS_CHANGE_PIN_DONE            0   /* change pin done */
#define  WPS_CHANGE_PIN_DEFAULT         1   /* restore default pin */
#define  WPS_CHANGE_PIN_NEW             2   /* generate new pin */ 

#define  WPS_SESSION_DONE               0   /* WPS session done */
#define  WPS_START_SESSION_PBC          1   /* start PBC mode WPS session */
#define  WPS_START_SESSION_PIN          2   /* start PIN mode WPS session */

/* message definitions */
#define  WPS_MSG_EVENT_ACK					"WSC_EVENT_ACK"
#define  WPS_MSG_SUCCESS					"WPS_SUCCESS"
#define  WPS_MSG_FAILURE					"WPS_FAILURE"
#define  WPS_MSG_REPORT_PIN					"WPS_REPORT_PIN"
#define  WPS_MSG_REPORT_STATUS				"WPS_REPORT_STATUS"

#define  WPS_CMD_START_PBC_FMT				"pin %s"
#define  WPS_CMD_QUIT_WSC					"WPS_QUIT"
#define  WPS_CMD_SET_PIN					"WPS_SET_PIN"
#define  WPS_CMD_GET_PIN					"WPS_GET_PIN"
#define  WPS_CMD_NEW_PIN					"WPS_NEW_PIN"
#define  WPS_CMD_GET_CONF                   "WPS_GET_CONF"

#define  WPS_QUIT_WSC_ACK					"WPS_QUIT_ACK"
#define  WPS_SET_PIN_ACK					"WPS_SET_PIN_ACK"
#define  WPS_GET_PIN_ACK					"WPS_GET_PIN_ACK"
#define  WPS_NEW_PIN_ACK					"WPS_NEW_PIN_ACK"
#define  WPS_GET_CONF_ACK                   "WPS_GET_CONF_ACK"


#define  WPS_ARG_DELIMITER                  "="
#define  WPS_CMD_DELIMITER                  ";"

#define  WPS_TIMER_INTERVAL                 10 /* seconds to time out */

#define  WPS_DEL_VAP_MSG                    "VapIndex"

/* timer name */
#define WPS_TIMER_NAME_CHANGE_PIN           "WPS_CHANGE_PIN"   /* PIN is changing */
#define WPS_TIMER_NAME_START_WPS            "WPS_START_WPS"    /* WPS session is going */
#define WPS_CHANGE_PIN_TIME_OUT             10   /* sec */
#define WPS_START_WPS_TIME_OUT              30   /* sec */

/****************************************************************************** 
 *                                STRUCT                                      * 
 ******************************************************************************/
typedef struct tag_ST_WPS_StatusVar{ 
	unsigned int start_wps;
	unsigned int wps_status;
	unsigned int change_pin;
	char sta_pin[WPS_PIN_BUF_LEN];
	char reset_unconf;
}ST_WPS_StatusVar;


/* Configure Tree Node for WPS */
typedef struct tag_ST_WPS_NodeVar{
	int enable;
	unsigned int vap_idx;
	char dev_pin[WPS_PIN_BUF_LEN];
	char default_pin[WPS_PIN_BUF_LEN];
    ST_WPS_StatusVar status_var;
}ST_WPS_NodeVar;

typedef enum tag_EM_WPS_ERRNO{
    WPS_E_SUCCESS=0,    /* no error */
    WPS_E_GENERAL,      /* general failue */ 
    WPS_E_SYSTEM,       /* system error */
    WPS_E_INVALID,      /* invalid parameter */
    WPS_E_UNKNOWN       /* unknown error */    
}EM_WPS_ERRNO;

typedef enum tag_EM_WPS_STATUS{
    WPS_STATUS_NONE=0,
    WPS_STATUS_INPROGRESS,
    WPS_STATUS_SUCCESS,
    WPS_STATUS_OVERLAP,
    WPS_STATUS_ERROR
}EM_WPS_STATUS;

/****************************************************************************** 
 *                                 GLOABLE VARIABLE                           * 
 ******************************************************************************/
extern ST_WPS_NodeVar g_NodeVar;

/****************************************************************************** 
 *                               FUNCTION                                     * 
 ******************************************************************************/
int WPS_StartWPSSession(ST_ScalarVal *pstScalarVal);
int WPS_EnableWPS(ST_ScalarVal *pstScalarVal);
int WPS_ChangePIN(ST_ScalarVal *pstScalarVal);

BOOL WPS_CheckPIN(ST_ScalarVal *pstScalarVal, char *pszValue);
BOOL WPS_CheckVapIndex(ST_ScalarVal *pstScalarVal, char *pszValue);

BOOL WPS_CanUseWPS(unsigned int vap_idx);
ST_WPS_NodeVar * WPS_GetNodeVar(void);

int WPS_CallBack(int action);
int WPS_CheckWlanSetting(void);
    
int WPS_ModuleInit(void);
int WPS_ModuleUninit(void);
/****************************************************************************** 
 *                                 END                                        * 
 ******************************************************************************/


#endif /* __WPS_H__ */



