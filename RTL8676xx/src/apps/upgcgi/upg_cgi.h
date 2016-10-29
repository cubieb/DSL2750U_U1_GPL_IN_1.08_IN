/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : upg_cgi.h
 文件描述 : upgcgi 模块头文件

 函数列表 :

 修订记录 :
          1 创建 : 李浩
            日期 : 2008-04-10
            描述 : 创建文件

          2 创建 : 匡素文
            日期 : 2008-07-03
            描述 : 完善注释

**********************************************************************/

#ifndef _UPGCGI_H
#define _UPGCGI_H

#include "common.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/

/* 调试开关 */
//#define UPGCGI_DEBUG   1

#ifdef UPGCGI_DEBUG
#define UPGCGI_LogTrace(fmt, args...)      UPGCGI_Log("%s:%4d %20s: " fmt, __FILE__, __LINE__, __func__, ##args)
#else
#define UPGCGI_LogTrace(fmt, args...)      {do { ; } while(0);}
#endif

/* 文件名定义 */
#define FIRM_FILE_NAME 	        "/var/image.img"
#define AUTH_AS_FILE_NAME       "/var/Wlan_Auth_AS.cer"
#define AUTH_AP_FILE_NAME       "/var/Wlan_Auth_AP.cer"
#define AUTH_TR069_FILE_NAME    "/var/CA_root.pem"
#define AUTH_TR069_CA_PATH      "InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAFilePath"
#define AUTH_TR069_CA_NAME      "InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAname"
#define AUTH_TR069_CA_FILE      "InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAFile"

#define CFG_NODE_LEN            256
#define CFG_NODE_VALUE_LEN            3*2048

#define LOGFILE     		    "/var/upgcgi_log.txt"
#define UPGRADINGPAGE		    "/usr/www/html/page/upgrading.html"
#define RESTARTINGPAGE          "/usr/www/html/page/restarting.html"
#define ERROR_RESTARTINGPAGE          "/usr/www/html/page/error_restarting.html"
#define CERTFILE                "/usr/www/html/page/certificate_ca.html"
#define FACTORY_MODE_PAGE            "/usr/www/factory_mode.html"

/* 配置文件名 */
#define UPGCGI_CONFIG_FILENAME      "config.xml"
/* 日志文件名 */
#define UPGCGI_SYSEVENT_FILENAME    "sysevent.txt"

/*防火墙日志文件名*/
#define UPGCGI_FIREWALL_FILENAME "firewalllog.log"
/* 日志文件路径 */
#define UPGCGI_SYSEVENT_PATH        "/var/log/" UPGCGI_SYSEVENT_FILENAME
/*防火墙日志路径*/
#define UPGCGI_FIREWALL_PATH       "/var/log/"UPGCGI_FIREWALL_FILENAME


/* 认证模块相关定义 */
#define WEB_SSID_TOKEN          "sessionid"
#define WEB_SPLIT_CHAR_SET      "&;"

/* webshell的密钥 */
#define UPGCGI_KEY              "twmode"

/* -c action */
#define IMG_WRITE			    "1"
#define CONFIG_READ		        "2"
#define CONFIG_WRITE		    "3"
#define CONFIG_BAKUP		    "4"

/* 大小定义 */
#define PAGE_SIZE			    1024*10
#define CONFIG_SIZE			    1024

#define MAX_LINE			    80
#define MAX_PATH			    1024
#define MAX_MSG_SIZE		    256

/* 可变参数定义 */
typedef char *va_list;

/******************************************************************************
*                                ENUM
******************************************************************************/
/* 文件类型 */
enum
{
  	TYPE_BINARY,
	TYPE_TEXT
};

/* web数据编码方式 */
enum
{
    CONTENT_TYPE_NONE,
	CONTENT_TYPE_URLENCODE,
  	CONTENT_TYPE_FORMDATA
};

/* 响应消息的检查结果 */
enum en_RESPONSE_STATE
{
    EN_RESPONSE_SUCCESS,    /* 结果OK */
    EN_RESPONSE_ERROR,      /* 结果错误 */
    EN_RESPONSE_TIMEOUT,    /* 等待超时 */
    EN_RESPONSE_UNREPECT,   /* 非预期消息 */

    EN_RESPONSE_BUTT
};

/* 错误码 */
enum
{
    BEGIN_STATUS = -1,

    UPG_SUCCESS = 0,            //0
    UPG_GENERAL_ERROR,
    INIT_MODULE_ERROR,
    DESTROY_MODULE_ERROR,
    REQUEST_METHOD_ERROR,
    CONTENT_TYPE_ERROR,         //5
    CONTENT_LENGTH_ERROR,
    INVALID_VARIABLE_ERROR,
    NO_VARIABLE_ERROR,
    MIN_CONTENT_LENGTH_ERROR,
    MAX_CONTENT_LENGTH_ERROR,   //10
    FOPEN_LOG_ERROR,
    FOPEN_FW_ERROR,
    OPERATE_FW_ERROR,
    FILE_TYPE_ERROR,
    FILE_SIZE_ERROR,            //15
    FILE_PRODUCT_ERROR,
    FILE_CRC_ERROR,
    FILE_UPLOAD_ERROR,
    AUTH_ERROR,
    END_STATUS                  //20
};

/******************************************************************************
*                                STRUCT
******************************************************************************/



/******************************************************************************
*                                FUNCTION
******************************************************************************/
#define UPGCGI_SUCCEED(ret) (ret == UPG_SUCCESS)
#define UPGCGI_FAILED(ret)  (ret != UPG_SUCCESS)
#define MIN(a,b)            ((a)<(b))?(a):(b)


/* in upg_parse.c */
BOOL get_line_from_post(char *sPtr, int maxLen);
void get_var_name_from_post(char *sPtr, int maxLen);
void get_file_name_from_post(char *sPtr, int maxLen);
void get_value_from_post(char *sPtr, int maxLen, const char *pVarName);
int  get_content_type(char *sPtr, int maxLen);
void remove_trailing_CRLF(char *sPtr, int length);
void remove_CRLF_and_save_buffer(char *sPtr, long *nSize, char *pSavePtr);
unsigned char Hex2Uchar(const char *szHex);
void UrlUnEsc(char *szUrl);


void UPGCGI_RemoveAllFile();
void UPGCGI_FlushBuffer(void);
void UPGCGI_Header(int type, const char *pszFile);
void UPGCGI_SendMsgPage(char *data);
void UPGCGI_SendMsgText(char *data);
void UPGCGI_SendHtmlPage(char * pagename);
void UPGCGI_SendReturnPage(char *data);
void UPGCGI_SendClosePage(char *data);


int UPGCGI_WebShell();
int UPGCGI_ModifyMac();
int UPGCGI_UploadAuthFile();
int UPGCGI_UploadTR069AuthFile();

int UPGCGI_SendSetMsg(int nValNum, char aszName[][CFG_NODE_LEN], char aszVal[][CFG_NODE_VALUE_LEN]);
int UPGCGI_ProcessAckMsg();

int UPGCGI_UploadSingleFile(char * filename, unsigned long minSizeLimit , unsigned long maxSizeLimit);
int UPGCGI_DownloadSysEvtLogFile(BOOL bDownOrShow);
int UPGCGI_DownloadFile(void);
int UPGCGI_DoConfigUpgrade(void);
int UPGCGI_DoFirmwareUpgrade(void);
void handle_read_timeout( int sig );
void UPGCGI_DispPostContent();
void UPGCGI_Log(char *szFmt, ...);
int UPGCGI_CheckAuth(char *pszSessionID);
int UPGCGI_SetCertFile();
int UPGCGI_RedirhtmlPage(int flag);


/******************************************************************************
*                                VARIABLES
******************************************************************************/
extern long nPostDataCount;
extern char sDivider[];

#endif /* _UPGCGI_H */

