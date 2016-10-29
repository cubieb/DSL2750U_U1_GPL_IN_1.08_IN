/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: ip6aac.h
 文件描述: ip6aac模块的头文件

 修订记录:
        1. 作者: XuXiaodong
           日期: 2009-12-28
           内容: 创建文件
**********************************************************************/
#ifndef _IP6AAC_H_
#define _IP6AAC_H_

#ifdef IP6AAC_DEBUG
    #define IP6AAC_TRACE(fmt, args...) COMMON_TRACE(MID_IP6AAC, fmt, ##args)
    #define IP6AAC_LOG_ERR(fmt, args...) COMMON_TRACE(MID_IP6AAC, fmt, ##args)
#else
    #define IP6AAC_TRACE(fmt, args...)
    #define IP6AAC_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_IP6AAC, fmt, ##args)
#endif

#define IP6AAC_TRACE_INTO_FUNC IP6AAC_TRACE("##In## %s\n", __func__)

#define IP6AAC_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            IP6AAC_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define IP6AAC_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_IP6AAC, Ret, fmt, ##args)

#define IP6AAC_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_IP6AAC, Pointer, gotoTag, fmt, ##args)

#define IP6AAC_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            IP6AAC_TRACE(fmt, ##args); \
            return Ret; \
        }



#define IP6AAC_ASSERT(expr) \
    do{ \
        if(!(expr))\
        {\
            IP6AAC_TRACE("Assert \"%s\" failed\n", #expr); \
            exit(-1); \
        } \
    }while(0)

#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

#define IAPD_FLAG_MASK     0x000F
#define IANA_ENABLE_FLAG   0x0010
#define DNS_ENABLE_FLAG    0x0020
#define NTP_ENABLE_FLAG    0x0040
#define RA_MANAGED_FLAG    0x0100
#define RA_OTHERCONF_FLAG  0x0200
#define RA_STATE_FLAG      0x0400  /*该标志表示是否收到了MSG_RA_UPDATE或MSG_RS_FAILED消息*/
#define CONF_UPDATE_FLAG   0x1000
#define TMP_SESSION_FLAG   0x2000

#define PREFIX_STAT_VALID    0x01
#define PREFIX_STAT_STALE    0x02
#define PREFIX_STAT_INVALID  0x03

typedef struct tag_ST_PREFIX_INFO {
    unsigned long ulIAID;
    struct in6_addr stPrefix;
    unsigned char ucPrefixLen;
    unsigned char ucStatus;
    unsigned char ucPad[2];
    unsigned long ulPltime;
    unsigned long ulVltime;
    TBS_ST_LIST_HEAD lhPrefix;
} ST_PREFIX_INFO;

#define MAX_DUID_LEN 44
typedef struct tag_ST_IPV6_SESSION_INFO {
    unsigned long ulSessionID;
    char szDevName[MAX_IF_NAME_LEN];
    char szDUID[MAX_DUID_LEN];
    unsigned short usFlag;
    unsigned char ucStateMachine;
    unsigned char ucPad;
    char *pszDnsInfo;
    char *pszNtpInfo;
    TBS_ST_LIST_HEAD lhPrefix;
    TBS_ST_LIST_HEAD lhSession;
	unsigned char ucAddrType;
	unsigned char ucOnceFlag;
} ST_IPV6_SEESION_INFO;

typedef struct tag_ST_SESSION_CONF_INFO {
    unsigned long ulSessionID;
    char szDevName[MAX_IF_NAME_LEN];
    char szDUID[MAX_DUID_LEN];
    unsigned char ucIANAConf;
    unsigned char ucIAPDConf;
    unsigned char ucDNSConf;
    unsigned char ucNTPConf;
} ST_SESSION_CONF_INFO;

#define SIGF_TERM 0x1
#define SIGF_HUP  0x2

#define FIND_SESSION_BY_ID  0x1
#define FIND_SESSION_BY_DEV 0x2


#define CONF_FILE_PATH   "/var/dhcpv6/"
#define PID_FILE_PATH    "/var/run/"
#define MAX_FILE_NAME_LEN  64

#define MAX_DNS_INFO_LEN (3*MAX_IP6_ADDR_LEN)
#define MAX_NTP_INFO_LEN (3*MAX_IP6_ADDR_LEN)

enum {
    NOTIFY_IAPD_UPDATE,
    NOTIFY_DNS_UPDATE,
    NOTIFY_NTP_UPDATE,
    NOTIFY_REQUEST_FAILED
};

#endif /*_IP6AAC_H_*/

