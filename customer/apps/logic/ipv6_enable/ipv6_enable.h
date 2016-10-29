/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: dnsipv6.h

 文件描述: dns模块的头文件

 修订记录:
           1 作者: 胡煜
             日期: 2009-12-18
             描述: 创建
           2 作者: 陈凯
             日期: 2011-09-22
             描述: 重构
**********************************************************************/
#ifndef _DNS_IP6_H_
#define _DNS_IP6_H_

#include "tbsutil.h"

#define CALL_FUCTION_FAILED "Call function \"%s\" failed\n"

#define DNS_RET_RETURN(iRet, strError, args...) RET_RETURN(MID_DNS, iRet, strError, ##args)
#define DNS_POINTER_RETURN(pRet, strError, args...) POINTER_RETURN(MID_DNS, pRet, strError, ##args)
/*
 * ==========================
 */

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef IPV6ENABLE_DEBUG
    #define IPV6ENABLE_TRACE(fmt, args...) TBS_TRACE(MID_IPV6ENABLE, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define IPV6ENABLE_LOG_ERR(fmt, args...) TBS_TRACE(MID_IPV6ENABLE, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
    #define IPV6ENABLE_TRACE(fmt, args...)   do { ; } while(0)
    #define IPV6ENABLE_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_IPV6ENABLE, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define IPV6ENABLE_TRACE_INTO_FUNC IPV6ENABLE_TRACE("##In## %s\n", __func__)
#define IPV6ENABLE_TRACE_OUT_FUNC IPV6ENABLE_TRACE("##Out## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define Check_PARAMETER_FAILED  "Check parameter \"%s\" failed\n"
#define IPV6ENABLE_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            IPV6ENABLE_LOG_ERR(fmt, ##args); \
            goto gotoTag; \
        }

#define IPV6ENABLE_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_IPV6ENABLE, Ret, fmt, ##args)

#define IPV6ENABLE_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_IPV6ENABLE, Pointer, gotoTag, fmt, ##args)

#define IPV6ENABLE_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            IPV6ENABLE_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define IPV6ENABLE_CHECK_NULL_PTR(ptr)    IPV6ENABLE_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")

/*
 * ==================================================
 */
/* IPv6 */

#define MAX_IPv6_LEN_DNS 40
#define MAX_PROC_NAME_LEN 16
#define DEV_NAME_LEN 20

#define IPV6ENABLE_PATH_PREFIX            "InternetGatewayDevice.X_TWSZ-COM_IPv6Enable."

#define NODE_IPv6Enable "Enable"
#define IPV6_ENABLE_BASE "^InternetGatewayDevice\\.X_TWSZ-COM_IPv6Enable\\."

#define IPV6ENABLE_NODE_REGEX(node) IPV6_ENABLE_BASE node


/*
int DNS_IPv6_IsAddrExisted(const char *pszAddress, const char *pszFileName);
int DNS_IPv6_RewriteResolv();
int DNS_DefaultWriteConf(char *pszDevice, char *pszData);
int DNS_IPv6_WriteResConf(const char *pszDnsList, const char *pszData);
int DNS_IPv6_WriteHostsConf(char *pszPath, void *pFile);
int DNS_IPv6_WriteToConf(char *pszPath, void *pData);
int DNS_IPv6_WriteConf();
int DNS_MsgIP6WANChgHandler(ST_MSG *pstMsg);
int DNS_Ip6ServersApply(const char *pszPath, const char *pszValue);
*/

#endif

