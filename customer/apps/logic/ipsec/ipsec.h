/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 
 文件名称: ipsec.h

 文件描述: IPSEC模块头文件

 修订记录:
             作者 : kevin
             日期 : 2010-07-02
             描述 : 创建
**********************************************************************/
#ifndef __TBS_IPSEC_H__
#define __TBS_IPSEC_H__
/*********************************************************************
*****************************包含头文件*******************************
**********************************************************************/
#include "common.h"
#include <netinet/in.h>	   
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "autoconf.h"

/*********************************************************************
*****************************结构类型定义*****************************
**********************************************************************/
typedef struct 
{
    char *pszConName;   
    char *pszTunnelMode;
    char *pszRemoteGW;
    char *pszLocalIPAddr;
    char *pszLocalIPMask; 
    char *pszRemoteIPAddr;
    char *pszRemoteIPMask;
    char *pszKeyExchangeMethod;
    char *pszPreShardKey;
    char *pszPerfectForwardEnable;
    char *pszMode;
    char *pszEncryAlg1;
    char *pszIntegAlg1;
    char *pszDHGroup;
    char *pszKeyLifeTime1;
    char *pszEncryAlg2;
    char *pszIntegAlg2;
    char *pszPFSGroup;
    char *pszKeyLifeTime2;
    char *pszEncryAlgManual;
    char *pszEncryKey;
    char *pszIntegAlgManual;
    char *pszIntegKey;
    char *pszSPI;
    char *pszInterface;
}ST_IPSEC_LIST;
/*********************************************************************
*****************************宏变量定义*******************************
**********************************************************************/
//调试宏信息
#define IPSEC_DEBUG

#ifdef IPSEC_DEBUG
#define IPSEC_TRACE(fmt, args...)                     COMMON_TRACE(MID_IPSEC, fmt"\n", ##args)
#define IPSEC_RET_GOTO(iRet, strError, gotoTag)       RET_GOTO(MID_IPSEC, iRet, strError, gotoTag)
#define IPSEC_FUN_IN()                                IPSEC_TRACE("Function=%s IN",__FUNCTION__)
#define IPSEC_FUN_OUT()                               IPSEC_TRACE("Function=%s OUT",__FUNCTION__)
#else
#define IPSEC_TRACE(fmt, args...)
#define IPSEC_RET_GOTO(iRet, strError, gotoTag) 
#define IPSEC_FUN_IN()
#define IPSEC_FUN_OUT()
#endif


#define IPSEC_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (NULL == Pointer)    \
        { \
            IPSEC_TRACE(fmt, ##args); \
            return Ret; \
        }
#define IPSEC_CHECK_NULL_PTR(ptr)   IPSEC_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

//文件路径宏定义
#define RACOON_CONF  "/var/racoon.conf"
#define SETKEY_CONF  "/var/setkey.conf"
#define PSKTXT_TXT  "/var/psk.txt"
#define RACOON_PROC  "/usr/bin/racoon"
#define SETKEY_PROC  "/usr/bin/setkey"


#define DEVICE_REGEX_WAN_CONN_IP                 "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define DEVICE_REGEX_WAN_CONN_PPP               "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"

//IPSEC模块路径前缀
#define IPSEC_PATH_PREFIX                   "InternetGatewayDevice.X_TWSZ-COM_IPSEC."
//IPSEC模块IPSECEnable路径
#define IPSEC_PATH_ENABLE                   "InternetGatewayDevice.X_TWSZ-COM_IPSEC.IPSECEnable"
//IPSEC模块IPSEC实例路径
#define IPSEC_PATH_IPSEC                    "InternetGatewayDevice.X_TWSZ-COM_IPSEC.IPSEC"
//IPSEC模块IPSEC实例的基本节点
#define CONNECNAME                           "ConnectionName"
#define TUNNELMODE                           "TunnelMode"
#define REMOTEGW                             "RemoteGW"
#define LOCALIPADDR                          "LocalIPAddr"
#define LOCALIPMASK                          "LocalIPSubnetMask"
#define REMOTEIPADDR                         "RemoteIPAddr"
#define REMOTEIPMASK                         "RemoteIPSubnetMask"
#define KEYEXCHMETHOD                        "KeyExchangeMethod"
#define SHARDKEY                             "PreSharedKey"
#define ENABLEFORWARDSEC                     "PerfectForwardSecrecyEnable"
#define MODE                                 "Mode"
#define ENCRYALGAUTO1                        "EncryptionAlgorithm_Auto1"
#define INTERALGAUTO1                        "IntegrityAlgorithm_Auto1"
#define DHGROUP                              "DHGroup"
#define KEYLIFETIME1                         "KeyLifeTime1"
#define ENCRYALGAUTO2                        "EncryptionAlgorithm_Auto2"
#define INTERALGAUTO2                        "IntegrityAlgorithm_Auto2"
#define PFSGROUP                             "PFSGroup"
#define KEYLIFETIME2                         "KeyLifeTime2"
#define ENCRYALGMANUAL                       "EncryptionAlgorithm_Manual"
#define ENCRYKEY                             "EncryptionKey"
#define INTERALGMANUAL                       "IntegrityAlgorithm_Manual"
#define INTERKEY                             "IntegrityKey"
#define SPI                                  "SPI"
#define INTERFACE                            "Interface"


// 节点路径正则表达式
//IPSEC模块路径前缀
#define IPSEC_REGEX_PREFIX                   "^InternetGatewayDevice\\.X_TWSZ-COM_IPSEC\\."
//IPSEC模块IPSECEnable节点
#define IPSEC_REGEX_ENABLE                    IPSEC_REGEX_PREFIX "IPSECEnable" "$"
//IPSEC模块IPSEC实例路径
#define IPSEC_REGEX_ADD_PATH                  IPSEC_REGEX_PREFIX "IPSEC." "$"
//IPSEC模块实例的基本路径
#define IPSEC_REGEX_BASIC_PATH                IPSEC_REGEX_PREFIX "IPSEC\\.[0-9]+\\."
//IPSEC模块实例节点
#define IPSEC_REGEX_DEL_NODE                  IPSEC_REGEX_BASIC_PATH "$"
//IPSEC模块设置实例节点
#define IPSEC_REGEX_NODE_CONNECNAME       IPSEC_REGEX_BASIC_PATH CONNECNAME "$"
#define IPSEC_REGEX_NODE_TUNNELMODE       IPSEC_REGEX_BASIC_PATH TUNNELMODE "$"
#define IPSEC_REGEX_NODE_REMOTEGW         IPSEC_REGEX_BASIC_PATH REMOTEGW "$"
#define IPSEC_REGEX_NODE_LOCALIPADDR      IPSEC_REGEX_BASIC_PATH LOCALIPADDR "$"
#define IPSEC_REGEX_NODE_LOCALIPMASK     IPSEC_REGEX_BASIC_PATH LOCALIPMASK "$"
#define IPSEC_REGEX_NODE_REMOTEIPADDR     IPSEC_REGEX_BASIC_PATH REMOTEIPADDR "$"
#define IPSEC_REGEX_NODE_REMOTEIPMASK     IPSEC_REGEX_BASIC_PATH REMOTEIPMASK "$"
#define IPSEC_REGEX_NODE_KEYEXCHMETHOD    IPSEC_REGEX_BASIC_PATH KEYEXCHMETHOD "$"
#define IPSEC_REGEX_NODE_SHARDKEY         IPSEC_REGEX_BASIC_PATH SHARDKEY "$"
#define IPSEC_REGEX_NODE_ENABLEFORWARDSEC IPSEC_REGEX_BASIC_PATH ENABLEFORWARDSEC "$"
#define IPSEC_REGEX_NODE_MODE             IPSEC_REGEX_BASIC_PATH MODE "$"
#define IPSEC_REGEX_NODE_ENCRYALGAUTO1    IPSEC_REGEX_BASIC_PATH ENCRYALGAUTO1 "$"
#define IPSEC_REGEX_NODE_INTERALGAUTO1    IPSEC_REGEX_BASIC_PATH INTERALGAUTO1 "$"
#define IPSEC_REGEX_NODE_DHGROUP          IPSEC_REGEX_BASIC_PATH DHGROUP "$"
#define IPSEC_REGEX_NODE_KEYLIFETIME1     IPSEC_REGEX_BASIC_PATH KEYLIFETIME1 "$"
#define IPSEC_REGEX_NODE_ENCRYALGAUTO2    IPSEC_REGEX_BASIC_PATH ENCRYALGAUTO2 "$"
#define IPSEC_REGEX_NODE_INTERALGAUTO2    IPSEC_REGEX_BASIC_PATH INTERALGAUTO2 "$"
#define IPSEC_REGEX_NODE_PFSGROUP         IPSEC_REGEX_BASIC_PATH PFSGROUP "$"
#define IPSEC_REGEX_NODE_KEYLIFETIME2    IPSEC_REGEX_BASIC_PATH KEYLIFETIME2 "$"
#define IPSEC_REGEX_NODE_ENCRYALGMANUAL   IPSEC_REGEX_BASIC_PATH ENCRYALGMANUAL "$"
#define IPSEC_REGEX_NODE_ENCRYKEY         IPSEC_REGEX_BASIC_PATH ENCRYKEY "$"
#define IPSEC_REGEX_NODE_INTERALGMANUAL   IPSEC_REGEX_BASIC_PATH INTERALGMANUAL "$"
#define IPSEC_REGEX_NODE_INTERKEY         IPSEC_REGEX_BASIC_PATH INTERKEY "$"
#define IPSEC_REGEX_NODE_SPI              IPSEC_REGEX_BASIC_PATH SPI "$"
#define IPSEC_REGEX_NODE_INTERFACE        IPSEC_REGEX_BASIC_PATH INTERFACE "$"
/***********************************************************************
**************************函数原型**********************************
************************************************************************/
int IPSEC_ModuleInit();

#endif

