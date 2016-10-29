
/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cfg_comm.h
 文件描述 : cfg模块内部全局公用部分的头文件
 修订记录 :
          1 创建 : 陈跃东
            日期 : 2009-07-27
            描述 :

**********************************************************************/

#include "cfg_api.h"
#include <errno.h>

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


#define STATIC static


/*********************** 错误报告 ************************/

#ifdef _CFG_DEBUG


#define CFG_ERR(ErrCode, arg...) \
    printf("\n[CFG]: %s %d errno: %04u%04u %s", __FILE__, __LINE__, \
        TBS_ERR_INTERNAL(ErrCode), TBS_ERR_STAND(ErrCode), strerror(errno)); \
    printf(" para: " arg); \
    printf("\n")

#define CFG_LOG(arg...) \
    printf("\n[CFG]: %s %d ",  __FILE__, __LINE__); \
    printf(" log: " arg); \
    printf("\n")

#define CFG_ASSERT(exp) \
{ \
    if (!(exp)) \
    { \
        CFG_ERR(CFG_FAIL, "ASSERT FAILED"); \
        exit(-1); \
    } \
}

#define CFG_TRACE(fmt, args...)  printf("" fmt, ##args)
#else

#define CFG_ERR(ErrCode, arg...) (void)0

#define CFG_LOG(arg...) (void)0

#define CFG_ASSERT(exp) (void)0

#define CFG_TRACE(fmt, args...) (void)0
#endif

#define XML_CTL_HEAD "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
#define XML_CTL_HEAD_LEN (strlen("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"))
#define WLAN_BASIC_PATH          "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1"
#define NODE_WLAN_SSID			"SSID"
#define NODE_WLAN_BEACON_TYPE			"BeaconType"
#define BEACON_TYPE_NONE					"None"
#define BEACON_TYPE_BASIC					"Basic"
#define BEACON_TYPE_WPA						"WPA"
#define BEACON_TYPE_11i						"11i"
#define BEACON_TYPE_WPA2_MIXED 				"WPAand11i"
#define NODE_WLAN_WEPKEY				"WEPKey"
#define NODE_WLAN_PSK_EXPRESSION		"X_TWSZ-COM_PSKExpression"
#define NODE_WLAN_WPA_KEY_PASSPHRASE	"KeyPassphrase"
#define NODE_WLAN_WPA_PSK				"PreSharedKey"
#define MAX_PATH_LEN                    257


#define CFG_COMM_ERR_PROC(err, arg...) \
if (CFG_OK != ret) \
{ \
    CFG_ERR(ret, "" arg); \
    return ret; \
}

#define CFG_GOTO_ERR_PROC(ret, tag, arg...) \
if (CFG_OK != ret) \
{ \
    CFG_ERR(ret, "" arg); \
    goto tag; \
}



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


