/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: ipmacfilter.h

 文件描述: ipmacfilter模块头文件

 修订记录:

        1. 作者: HuangMengxiang

           日期: 2010-01-20

           内容: 创建文件

**********************************************************************/

#ifndef __LOGIC_IPMACFILTER_H__
#define  __LOGIC_IPMACFILTER_H__
#include "warnlog.h"

//#define IPMACFILTER_DEBUG                     //for debug

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define  IPMACFILTER_NO_PRINT_CMD                                      0
#define  IPMACFILTER_IP_MAX_LENGTH                         		  32
#define  IPMACFILTER_MAC_MAX_LENGTH   			    		  32
#define  IPMACFILTER_INTERFACE_NAME_MAX_LENGTH           32
#define  IPMACFILTER_INTERFACE_MAX_NUM                            10
#define  IPMACFILTER_TIME_INTERVAL                                      3
#define  IPMACFILTER_ONE_LINE_MAX_LENGTH				  128
#define  IPMACFILTER_MAX_CMD_LEN                                             256
#define  LAN_ARP_MAX                                                              10  

#define  	IPMACFILTER_ARP_FILE_PATH                                    "/proc/net/arp"
#define   IPMACFILTER_MACLIST_NAME					 "IPMACFILTER_MACLIST"
#define   IPMACFILTER_IPLIST_NAME                                 "IPMACFILTER_IPLIST"
#define   IPMACFILTER_PORTS_EXCLUDE                                    "vport+"

#define IPMACFILTER_WIRELESS_PORTS_EXCLUDE                   "wlan+"
#define LAN_ARP_PATH_PREFIX   "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_ARPTable.Arptable."
#define LAN_ARP_PAHT_NUMBER   "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_ARPTable.LANARPTableNumberOfEntries"
#define LAN_ARP_IPNAME "IPAddress"
#define LAN_ARP_MACNAME "MACAddress"
#define INITIPMACTABLE(table) do { \
	memset(table,0,sizeof(table)); \
}while(0)
/******************************************************************************
*                                 STRUCTS                                      *
******************************************************************************/
typedef struct tag_ST_LOGIC_IPMACFILTER_IPMAC{
	char szIP[IPMACFILTER_IP_MAX_LENGTH];
	char szMac[IPMACFILTER_MAC_MAX_LENGTH];
	char szInterface[IPMACFILTER_INTERFACE_NAME_MAX_LENGTH];
}ST_LOGIC_IPMACFILTER_IPMAC;

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef IPMACFILTER_DEBUG
    #define IPMACFILTER_TRACE(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define IPMACFILTER_LOG_ERR(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define IPMACFILTER_TRACE(fmt, args...)   do { ; } while(0)
    #define IPMACFILTER_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_MACFILTER, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define IPMACFILTER_TRACE_INTO_FUNC IPMACFILTER_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define IPMACFILTER_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            IPMACFILTER_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define IPMACFILTER_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_MACFILTER, Ret, fmt, ##args)

#define IPMACFILTER_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_MACFILTER, Pointer, gotoTag, fmt, ##args)

#define IPMACFILTER_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            IPMACFILTER_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define IPMACFILTER_CHECK_NULL_PTR(ptr)    IPMACFILTER_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

/******************************************************************************
*                                FUNCTION
******************************************************************************/

/* Init && Destroy */
int IPMACFILTER_ModuleRegister();
int IPMACFILTER_ModuleInit(void);
int IPMACFILTER_ModuleDestroy (void);

#endif
