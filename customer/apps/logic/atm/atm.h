/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: atm.h
 文件描述: 实现通过使用br2684ctl配置pvc
 修订记录:
        1. 作者: liannan
           日期: 2009-1-4
           内容: 创建文件
**********************************************************************/

#ifndef _ATM_H
#define _ATM_H

/* 日志打印宏 */
#ifdef ATM_DEBUG
#define ATM_TRACE(fmt, args...)  COMMON_TRACE(MID_ATM, fmt "\n", ##args)
#define CMD_PRINT 1   /* 调用tbsSystem时打印命令 */
#else
#define ATM_TRACE(fmt, args...)
#define CMD_PRINT 0
#endif

#define ATM_TRACE_SUCCESS(fmt, args...)  ATM_TRACE(GREEN INFO_SUCCESS NONE fmt, ##args)
#define ATM_TRACE_ERROR(fmt, args...)    ATM_TRACE(LIGHT_RED INFO_ERROR NONE fmt, ##args)

#define ATM_TRACE_INTO_FUNC ATM_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define ATM_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            ATM_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define ATM_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_ATM, Ret, fmt, ##args)

#define ATM_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_ATM, Pointer, gotoTag, fmt, ##args)

#define ATM_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            ATM_TRACE(fmt, ##args); \
            return Ret; \
        }

#define ATM_CHECK_NULL_PTR(ptr)   ATM_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
/*********************************************************************/
#define MAX_WAN_CONN 8
#define MAX_ATM_EXEC_COUNT 16

#define NODE_DSL_LINK    "WANDSLLinkConfig"
#define NODE_ATM_ENABLE  "Enable"
#define NODE_LINK_STATUS "LinkStatus"
#define NODE_LINK_TYPE   "LinkType"
#define NODE_DEST_ADDR   "DestinationAddress"
#define NODE_ATM_ENCAP   "ATMEncapsulation"
#define NODE_ATM_QOS     "ATMQoS"
#define NODE_PEAK_CELL_RATE "ATMPeakCellRate"
#define NODE_MAX_BURST_SIZE "ATMMaximumBurstSize"
#define NODE_SUS_CELL_RATE  "ATMSustainableCellRate"
#define NODE_CELL_DELAY_VAR "X_TWSZ-COM_ATMCellDelayVarition"
#define NODE_MIN_CELL_RATE  "X_TWSZ-COM_ATMMinimumCellRate"
#define NODE_CONN_DEV_NAME  "X_TWSZ-COM_DeviceName"

#define NODE_VLAN_ID "X_TWSZ-COM_VLANID"
#define NODE_VLAN_PRIORITY "X_TWSZ-COM_VLANPriority"
#define NODE_VLAN_UNTAG "X_TWSZ-COM_VLANUntag"

#define WAN_PATH_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+"
#define ATM_PATH_REGEX WAN_PATH_REGEX"\\.WANDSLLinkConfig$"

#define WAN_CONNDEV_PATH_PREFIX "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}."

#define ATM_PATH_PREFIX WAN_CONNDEV_PATH_PREFIX NODE_DSL_LINK"."
#define WAN_CONN_DEV_PATH "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice."
#define WAN_CONN_DEV_PATH_EX "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu"
#define ATM_PATH "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.WANDSLLinkConfig."
#define COMMON_INTERFACE_PATH    "InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig."

#define LINK_UP        "Up"
#define LINK_DOWN      "Down"
#define LINK_UNAVIBLE  "Unavailable"

enum EN_ATM_LINK_TYPE {
    EN_ATM_EOA = 0,
    EN_ATM_IPOA,
    EN_ATM_PPPOA,
    EN_ATM_NON,
};

#endif

