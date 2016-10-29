/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: firewalllog.h

 文件描述: firewalllog模块头文件

 修订记录:

        1. 作者: HuangMengxiang

           日期: 2010-01-23

           内容: 创建文件

**********************************************************************/

#ifndef __LOGIC_FIREWALLLOG_H__
#define  __LOGIC_FIREWALLLOG_H__
#include "warnlog.h"

//#define FIREWALLLOG_DEBUG                     //for debug




/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define IPT_LOG_INFO_START_POINT          12
//#define IPT_LOG_INFO_DATA_SIZE              12
#define IPT_LOG_WRITERPOINT_ADDR         6
#define IPT_LOG_READPOINT_ADDR             4
#define IPT_LOG_WPASSR_ADDR                  8

#define DEVMAXSIZE                                     16


#define SOCK_T                                                                             int
#define  FIREWALLLOG_TIME_INTERVAL                                      10
#define  IPT_BASE_CTL									    64                                     // the same in ip_tables.h
#define IPT_GO_GET_LOGINFOR                                                    (IPT_BASE_CTL + 4)            // add for log
#define FIREWALLLOG_LOGINFO_FILE_PATH                               "/var/log/firewalllog.log"
#define FIREWALLLOG_LOGINFO_FORMAT                                    "   TimeStamp		  SourceIP-DestIP       Protocol    port    target"
#define FIREWALLLOG_LOGINFO_ITEM_MAXLENGTH                   120
#define FIREWALLLOG_LOGINFO_ITEM_MAX                                120
#define FIREWALLLOG_LOGINFO_ITEM_FIRST                              0
#define FIRWALLLOG_IPADDR_MAX_LENGTH                                32
#define FIREWALLLOG_PROTOCOL_MAX_LENGTH                          8
#define FIREWALLLOG_ETH_MAX_LENGTH                                    16
#define FIREWALLLOG_LOGINFO_MESSAGE_MAX   \
	ST_LOGINFO_SIZE*FIREWALLLOG_LOGINFO_ITEM_MAX + sizeof(unsigned long) + 1

/******************************************************************************
*                                 STRUCTS                                      *
******************************************************************************/

typedef struct tag_ST_LOGINFO{                                             // log info struct
    unsigned long jiffies;
    u_int32_t saddr;
    u_int32_t daddr;
    unsigned int protocol;
    char szDev[DEVMAXSIZE];
}ST_LOGINFO;

#define ST_LOGINFO_SIZE sizeof(ST_LOGINFO)

typedef struct tag_ST_LOGINFO_STR{
	
}ST_LOGINFO_STR;

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef FIREWALLLOG_DEBUG
    #define FIREWALLLOG_TRACE(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define FIREWALLLOG_LOG_ERR(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define FIREWALLLOG_TRACE(fmt, args...)   do { ; } while(0)
    #define FIREWALLLOG_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_MACFILTER, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define FIREWALLLOG_TRACE_INTO_FUNC FIREWALLLOG_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define FIREWALLLOG_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            FIREWALLLOG_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define FIREWALLLOG_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_MACFILTER, Ret, fmt, ##args)

#define FIREWALLLOG_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_MACFILTER, Pointer, gotoTag, fmt, ##args)

#define FIREWALLLOG_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            FIREWALLLOG_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define FIREWALLLOG_CHECK_NULL_PTR(ptr)    FIREWALLLOG_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"


#endif

