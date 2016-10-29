
#ifndef __DEVCONFIG_H__
#define __DEVCONFIG_H__

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

#define BASE_PATH_DEVCONFIG       "^InternetGatewayDevice\\.DeviceConfig\\."
#define PATH_DEVCONFIG            "InternetGatewayDevice.DeviceConfig."

#define DEVCONFIG_NODE_PERSISTENDATA       	BASE_PATH_DEVCONFIG "PersistentData" "$"
#define DEVCONFIG_NODE_CONFIGFILE       			BASE_PATH_DEVCONFIG "ConfigFile" "$"

//#define DEVCONFIG_DEBUG 1
#ifdef  DEVCONFIG_DEBUG
#define DEVCONFIG_TRACE(fmt, args...)        TBS_TRACE(MID_DEVCONFIG, fmt, ##args)
#else
#define DEVCONFIG_TRACE(fmt, args...)
#endif

int DEVCONFIG_ModuleInit();
int DEVCONFIG_ModuleDestroy();


#endif
