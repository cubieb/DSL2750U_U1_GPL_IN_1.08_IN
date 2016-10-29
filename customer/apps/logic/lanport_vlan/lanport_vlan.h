/**********************************************************************
 Copyright (c), 1991-2011, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: lanport_vlan.h
 文件描述: LAN端口vlan头文件

 修订记录:
        1. 作者: 韩尚鹏
           日期: 2011-09-19
           内容: 创建文件

**********************************************************************/
#ifndef _LANPORT_VLAN_H_
#define _LANPORT_VLAN_H_

#include "warnlog.h"
#include "tbstype.h"

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
//#define LANPORT_VLAN_DEBUG
#ifdef LANPORT_VLAN_DEBUG
    #define LPVLAN_TRACE(fmt, args...) COMMON_TRACE(MID_LPVLAN, fmt"\n", ##args)
    #define LPVLAN_LOG_ERR(fmt, args...) COMMON_TRACE(MID_LPVLAN, fmt, ##args)
#else
    #define LPVLAN_TRACE(fmt, args...) 
    #define LPVLAN_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_LPVLAN, fmt, ##args)
#endif

#define LPVLAN_RET_GOTO(iRet, strError, gotoTag)   RET_GOTO(MID_LPVLAN, iRet, strError, gotoTag)
#define LPVLAN_TRACE_INTO_FUNC  LPVLAN_TRACE("##In## %s\n", __func__)

#define LPVLAN_RET_RETURN(Ret, fmt, args...)  \
    RET_RETURN(MID_LPVLAN, Ret, fmt, ##args)
    
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define LPVLAN_NODE_ENABLE              "Enable"
#define LPVLAN_NODE_PORT_PATH           "PortPath"
#define LPVLAN_NODE_LINK_TYPE           "LinkType"
#define LPVLAN_NODE_PVID                "PVID"
#define LPVLAN_NODE_VLAN_ID_LIST        "VLANIDList"
#define LPVLAN_NODE_UNTAG_VLAN_ID_LIST  "UntagVLANIDList"


#define LPVLAN_BASE_PATH "InternetGatewayDevice.X_TWSZ-COM_LanPortVLAN."
#define LPVLAN_VLAN_PATH "InternetGatewayDevice.X_TWSZ-COM_LanPortVLAN.VLAN."

/*
正则定义
*/
#define LPVLAN_REGEX_PREFIX                "^InternetGatewayDevice\\.X_TWSZ-COM_ACL\\."

#define LPVLAN_VLAN_PATH_REGEX    "^InternetGatewayDevice\\.X_TWSZ-COM_LanPortVLAN\\.VLAN\\.[0-9]+\\."

#define LPVLAN_REGEX_VLAN(node)           LPVLAN_VLAN_PATH_REGEX node "$"


#define LPVLAN_VLAN_ADD_REGEX    "^InternetGatewayDevice\\.X_TWSZ-COM_LanPortVLAN\\.VLAN\\.$"
#define LPVLAN_VLAN_DEL_REGEX    "^InternetGatewayDevice\\.X_TWSZ-COM_LanPortVLAN\\.VLAN\\.[0-9]+\\.$"


#define LPVLAN_VALN_SHOW(p) LPVLAN_TRACE("******Vlan CFG show********\n");\
        LPVLAN_TRACE("      Enable = %s\n", (p)->pszEnable);\
        LPVLAN_TRACE("      Linktype = %s\n", (p)->pszLinkType);\
        LPVLAN_TRACE("      PortPath = %s\n", (p)->pszPortPath);\
        LPVLAN_TRACE("      PVID = %s\n", (p)->pszPVID);\
        LPVLAN_TRACE("      VIDList = %s\n", (p)->pszVLANIDList);\
        LPVLAN_TRACE("      UntagVIDList = %s\n", (p)->pszUntagVLANIDList);

/******************************************************************************
*                                STRUCT
******************************************************************************/
#define MAX_VLAN_ID 4094

#define MAX_DEVNAME_LEN   20
#define MAX_PVID_STR_LEN  5 //1~4094
#define MAX_VID_LST_LEN   160
#define MAX_VLAN_NUM_IN_LST 32
#define MAX_VLAN_NUM_TOTAL (MAX_VLAN_NUM_IN_LST*2 + 1) //vlan list + untag vlan list + pvid
#define MAX_VLAN_STR_LEN (MAX_VID_LST_LEN * 2 + MAX_PVID_STR_LEN)//vlan list + untag vlan list + pvid

typedef struct{
    char *pszEnable;
    char *pszPortPath;
    char *pszLinkType;
    char *pszPVID;
    char *pszVLANIDList;
    char *pszUntagVLANIDList;
}ST_PORT_VLAN;

/******************************************************************************
*                                FUNCTION
******************************************************************************/


/* Init && Destroy */
int LPVLAN_ModuleInit(void);
int LPVLAN_ModuleDestroy(void);

#endif
