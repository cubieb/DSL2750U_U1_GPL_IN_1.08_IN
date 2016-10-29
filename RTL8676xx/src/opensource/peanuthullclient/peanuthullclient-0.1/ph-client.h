/*****************************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : ph-client.h
 文件描述 : 花生壳客户端(peanuthullc)进程入口文件的信息定义

 函数列表 :

 修订记录 :
            创建 :  Kevin
            日期 :  2009-11-18
            描述 :

******************************************************************************/

#ifndef __PH_CLIENT_H
#define __PH_CLIENT_H

#include "stdio.h"

/******************************************************************************
                        *                                 宏定义                                  *
 ******************************************************************************/
 /*花生壳客户端信息(For D-link)*/
 #define PHCLIENT_ID                     9857               /*0X2681*/
 #define PHCLIENT_KEY                   503842356       /*0X1E080634*/
 #define PHCLIENT_VERSION            38726             /*0X9746*/
 #define PHCLIENT_VERSION_STR     "9.7.4.6"          /*VERSION:38726*/
 #define PHCLIENT_INFO		0x26819746	/* 客户端信息是4个字节，前两位为嵌入式的客户号，后两位为客户端版本号*/
 
 /*报文的换行标识*/
#define	NEWLINE_R_N	"\015\012"	//\r\n

/*各种数组的最大长度*/
 #define MAX_STRING_LEN            256    /*一般数组最大长度*/
 #define MAX_AUTH_LEN               16
 #define MAX_PASSPORT_LEN        32      /*护照及密码的最大长度*/
 #define MAX_MD5PSW_LEN          128
 #define MAX_BUF_LEN                 4096
 
 /*花生壳服务器信息*/
 #define PH_SERVER_URL                         "hphwebservice.oray.net"   /*WebService address*/
 #define HTTP_PROTOCOL_PORT                "80"
 #define TCP_PROTOCOL_PORT_6060         "6060"
 #define UDP_PROTOCOL_PORT_6060         "6060"
 /*报文字段*/
 #define PH_SERVER_IP                            "PHServer"    /*服务器IP地址*/
 #define PH_USER_TYPE                           "UserType"    /*用户类型*/

 /*空格*/
 #define CHAR_BLANK                          32         

 /*花生壳服务器回应码*/
 #define PH_RESPOND_LEN                 3      /*服务器回应码为3位*/
 #define PH_TCP_OK                          "220"
 #define PH_AUTH_ROUTE_OK            "334"
 #define PH_AUTH_OK                       "250" 
 #define PH_AUTH_FAIL                     "535"
 #define PH_REGISTER_FAIL               "507"       /*no name registered*/
 #define PH_UPDATE_OK                    "50"


 /*花生壳客户端命令*/
 #define PHC_AUTH_ROUTE                        "AUTH ROUTER"
 #define PHC_CNFM                                   "CNFM"
 #define PHC_QUIT                                    "QUIT"
 #define PHC_UPDATE                                10
 #define PHC_LOGOUT                                11
  #define PH_UPDATE_FAIL                         1000
 #define PH_LOGOUT_OK                            51

/*各用户类型及心跳包间隔*/
#define STANDARD_USER                      0
#define ADVANCED_USER                     1
#define STANDARD_HEARTBEAT            60
#define ADVANCED_HEARTBEAT           30

/******************************************************************************
                        *                                 枚举类型定义                                    *
 ******************************************************************************/
/*进程状态*/
typedef enum
{
    PROC_UPDATE_FAIL = -3,
    PROC_AUTH_FAIL = -2,
    PROC_LOGIN_FAIL = -1,
    PROC_NORMAL = 0,          /* NORMAL*/
} PROC_STATUS;


/******************************************************************************
                        *                                结构体定义                                  *
 ******************************************************************************/
/*花生壳客户端更新信息结构体*/
typedef struct
{
    long lSessionID;
    long lOptionCode;
    long lSerialNum;
    long lCheckSum;
    long lAppend;
}ST_PH_UDP_SEND_DATE;

typedef struct
{
    ST_PH_UDP_SEND_DATE stUDPDate;
    long lIP;
}ST_PH_UDP_REC_DATE;


 /*****************************************************************************
                        *                            函数申明                          *
 ******************************************************************************/
static void Usage(char *psProName);
static int PH_init(void);
static int PH_login(const char *pcsPassport, const char *pcsPassword);
static int GetUserInfo(char *psBuf, const char* pcsPassport, const char *pcsPassword);
static int GetValueFromXML(const char *pcsMsg, const char *pcsKeyword, char *psBuf, int iBufLen);

#endif /* __PH_CLIENT_H */
