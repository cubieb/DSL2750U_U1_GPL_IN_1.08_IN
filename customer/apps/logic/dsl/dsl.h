/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: dsl.h
 文件描述: dsl 适配模块头文件

 修订记录:
       1 作者 : huangce
         日期 : 2009-1-06
         描述 : 创建
**********************************************************************/


#ifndef _TBS_DSL_H_
#define _TBS_DSL_H_

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"


/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
//#define DSL_DEBUG
/* 调试信息宏*/
#ifdef DSL_DEBUG
#define DSL_TRACE(fmt, args...)         COMMON_TRACE(MID_DSL,fmt"\n", ##args)
#else
#define DSL_TRACE(fmt, args...)         do{}while(0)
#endif

#define DSL_LOG_ERR(fmt, args...)       COMMON_LOG_ERROR(MID_DSL, fmt, ##args)

/* 非标准节点名前缀*/
#define CUST_NODE_NAME_PREFIX           "X_TWSZ-COM_"

/* 生成路径的格式字符串*/
#define PATH_DSL_STATS              "InternetGatewayDevice.WANDevice.%lu.WANDSLInterfaceConfig."
#define PATH_DSL_ENABLE              "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Enable"
#define PATH_DSL_STDSUP             "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.StandardsSupported"

#define PATH_REGEX_DSL_ENABLE "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANDSLInterfaceConfig\\.Enable$"
#define PATH_REGEX_DSL_DOWNBLOCKS "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANDSLInterfaceConfig\\.DownstreamBlocks$"
#define PATH_REGEX_DSL_UPBLOCKS "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANDSLInterfaceConfig\\.UpstreamBlocks$"

#define PATH_DSL_INTE_CFG_PATH_PREFIX              "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig."
#define PATH_DSL_STATS_PREFIX   "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.Total."
#define PATH_DSL_STATS_SHOWTIME_PREFIX   "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.Showtime."


#define DSL_DEVICE_NAME_PATH    "InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.X_TWSZ-COM_DeviceName"


#define MID_PATH_DSL_INTERFACE_CONF     "InternetGatewayDevice.WANDevice.{i}.WANDSLInterfaceConfig"
#define MID_PATH_DSL_INTERFACE_STATS    "InternetGatewayDevice.WANDevice.{i}.WANDSLInterfaceConfig.Stats.Total"
#define MID_PATH_DSL_INTERFACE_STATS_SHOWTIME    "InternetGatewayDevice.WANDevice.{i}.WANDSLInterfaceConfig.Stats.Showtime"


#define ENABLE     "Enable"
#define NODE_STANDARD_SUPPORTED     "StandardsSupported"
#define NODE_PWR_MANAGE_STATE       "PowerManagementState"

#define MAX_DSL_VALUE_LEN 512
#define MAX_DSL_MODE_COUNT 26
#define DSL_MODE_LEN    30
#define STATUS_INFO_LEN     50
#define MAX_MESSAGE_LEN 512

#endif //_TBS_DSL_H_

