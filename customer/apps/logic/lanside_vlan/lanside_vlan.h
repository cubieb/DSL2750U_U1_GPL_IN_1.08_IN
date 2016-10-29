/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称 : lanside_vlan.h
 文件描述 : LAN端VLAN 的头文件

 修订记录 :
          1 创建 : 陈灼
            日期 : 2008-8-28
            描述 :
          2 修改 : 陈灼
            日期 : 2008-10-15
            描述 : 适配模块优化
          3 修改 : 徐晓东
            日期 : 2009-06-10
            描述 : 进行代码重构,添加代码注释说明
**********************************************************************/
#ifndef __LSVLAN_H__
#define __LSVLAN_H__

#include "warnlog.h"
#include "tbstype.h"

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef LSVLAN_DEBUG
    #define LSVLAN_TRACE(fmt, args...) COMMON_TRACE(MID_LSVLAN, fmt, ##args)
    #define LSVLAN_LOG_ERROR(fmt, args...) LSVLAN_TRACE(fmt, ##args)
#else
    #define LSVLAN_TRACE(fmt, args...)  do { ; } while(0)
    #define LSVLAN_LOG_ERROR(fmt, args...) COMMON_LOG_ERROR(MID_LSVLAN, fmt, ##args)
#endif

#define LSVLAN_TRACE_INTO_FUNC  LSVLAN_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define LSVLAN_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            LSVLAN_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define LSVLAN_RET_RETURN(Ret, fmt, args...)  \
    RET_RETURN(MID_LSVLAN, Ret, fmt, ##args)

#define LSVLAN_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
    POINTER_GOTO(MID_LSVLAN, Pointer, gotoTag, fmt, ##args)

#define LSVLAN_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            LSVLAN_TRACE(fmt, ##args); \
            return Ret; \
        }

/***************************** 节点路径定义 ****************************/
#define LSVLAN_PATH_PREFIX     "InternetGatewayDevice.Layer2Bridging."
#define LSVLAN_PATH_BRIDGE     LSVLAN_PATH_PREFIX "Bridge."
#define LSVLAN_PATH_FILTER     LSVLAN_PATH_PREFIX "Filter."
#define LSVLAN_PATH_AVIBLEIF   LSVLAN_PATH_PREFIX "AvailableInterface."
#define ETHWAN_ENABLE		   "InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig.Enable"
#define ETHWAN_PORT		   	   "InternetGatewayDevice.WANDevice.3.WANCommonInterfaceConfig.X_TWSZ-COM_DeviceName"

#define LSVLAN_NODE_BRKEY      "BridgeKey"
#define LSVLAN_NODE_BREN       "BridgeEnable"
#define LSVLAN_NODE_BRNAME     "BridgeName"
#define LSVLAN_NODE_BRVLAN     "VLANID"
#define LSVLAN_NODE_BRLANDEV   "X_TWSZ-COM_AssociatedLANDevice"
#define LSVLAN_NODE_BRSTPEN    "X_TWSZ-COM_STP"
#define LSVLAN_NODE_BRETHPORTNUM "NumOfAssociatedEthPort"

#define LSVLAN_NODE_FILTERKEY    "FilterKey"
#define LSVLAN_NODE_FILTEREN     "FilterEnable"
#define LSVLAN_NODE_FILTERBRREF  "FilterBridgeReference"
#define LSVLAN_NODE_FILTERIF     "FilterInterface"
#define LSVLAN_NODE_VLANTAGGED   "AdmitOnlyVLANTagged"

#define LSVLAN_NODE_AVIBLEIFKEY  "AvailableInterfaceKey"
#define LSVLAN_NODE_IFTYPE       "InterfaceType"
#define LSVLAN_NODE_IFREF        "InterfaceReference"

#define KEY_DEV_NAME       "X_TWSZ-COM_DeviceName"
#define KEY_WLAN_DEV       "WLANConfiguration"
#define KEY_ETHLAN_DEV     "LANEthernetInterfaceConfig"
#define KEY_WAN_CONN_DEV   "WANConnectionDevice"
#define KEY_HOST_CM "LANHostConfigManagement"
#define KEY_LAN_DEV_IPROUTERS "IPRouters"

/*****************************************************************
 *                   节点路径匹配正则表达式                      *
 *****************************************************************/
#define LSVLAN_BASE_PATH_REGEX "^InternetGatewayDevice\\.Layer2Bridging"

#define LSVLAN_NODE_BRSTPEN_REGEX  LSVLAN_BASE_PATH_REGEX"\\."LSVLAN_NODE_BRSTPEN"$"

#define LSVLAN_BRIDGE_NODES_REGEX  LSVLAN_BASE_PATH_REGEX"\\.Bridge\\.[0-9]+\\..*"
#define LSVLAN_BR_NODE_REGEX(node) LSVLAN_BASE_PATH_REGEX"\\.Bridge\\.[0-9]+\\." node "$"

#define LSVLAN_FILTER_NODES_REGEX  LSVLAN_BASE_PATH_REGEX"\\.Filter\\.[0-9]+\\..*"
#define LSVLAN_FL_NODE_REGEX(node) LSVLAN_BASE_PATH_REGEX"\\.Filter\\.[0-9]+\\." node "$"

#define LSVLAN_BR_ADD_REGEX LSVLAN_BASE_PATH_REGEX"\\.Bridge\\.$"
#define LSVLAN_FL_ADD_REGEX LSVLAN_BASE_PATH_REGEX"\\.Filter\\.$"
#define LSVLAN_BR_DEL_REGEX LSVLAN_BASE_PATH_REGEX"\\.Bridge\\.[0-9]+\\.$"
#define LSVLAN_FL_DEL_REGEX LSVLAN_BASE_PATH_REGEX"\\.Filter\\.[0-9]+\\.$"

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define LSVLAN_MAX_CMD_LEN 256

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
/* 生效操作方式 */
enum {
    LSVLAN_APPLY_NON,
    LSVLAN_APPLY_ADD,
    LSVLAN_APPLY_DEL
};

int LSVLAN_CheckBridgeNum(void);
/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/
#endif /* __LSVLAN_H__ */

