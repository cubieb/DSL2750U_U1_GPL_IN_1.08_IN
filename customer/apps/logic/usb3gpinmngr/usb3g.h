/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: USB3G.h

 文件描述: USB3G模块的头文件

 修订记录:
           1 作者 : 周华海
             日期 : 2010-3-8
             描述 :
**********************************************************************/
#ifndef __TBS_USB3G_H
#define __TBS_USB3G_H

#include <time.h>
#include <fcntl.h>
#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"

#include "autoconf.h"



/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
/* 调试信息宏*/
#define USB3G_DEBUG 1

#ifdef USB3G_DEBUG
#define USB3G_TRACE(fmt, args...)               COMMON_TRACE(MID_USB3G, fmt"\n", ##args)
#define USB3G_FUNC_IN(fmt, args...)             USB3G_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define USB3G_FUNC_OUT()                        USB3G_TRACE("--O %s\n", __FUNCTION__)


#else
#define USB3G_TRACE(str, args...)
#define USB3G_FUNC_IN(fmt, args...)
#define USB3G_FUNC_OUT()

#endif

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define USB3G_SET_NODE_VAL(pszPath, pszVal) \
    { \
        iRet = CFG_SetNodeVal(pszPath, pszVal, NULL); \
        USB3G_RET_RETURN(iRet, "Set node \"%s\" value failed\n", pszPath); \
    }

#define USB3G_LOG_ERR(fmt, args...)                             COMMON_LOG_ERROR(MID_USB3G, fmt, ##args)

#define USB3G_RET_LOG(iRet, strError, args...)                  RET_LOG(MID_USB3G, iRet, strError, ##args)
#define USB3G_POINTER_LOG(pRet, strError, args...)              POINTER_LOG(MID_USB3G, pRet, strError, ##args)

#define USB3G_RET_RETURN(iRet, strError, args...)               RET_RETURN(MID_USB3G, iRet, strError, ##args)
#define USB3G_POINTER_RETURN(pRet, strError, args...)           POINTER_RETURN(MID_USB3G, pRet, strError, ##args)

#define USB3G_RET_GOTO(iRet, strError, gotoTag)                 RET_GOTO(MID_USB3G, iRet, strError, gotoTag)
#define USB3G_POINTER_GOTO(pRet, gotoTag, strError, args...)    POINTER_GOTO(MID_USB3G, pRet, gotoTag ,strError, ##args)

#define USB3G_RET_FALSE(iRet, strError, args...)                RET_FALSE(MID_USB3G, iRet, strError, ##args)
#define USB3G_POINTER_FALSE(pRet, strError, args...)            POINTER_FALSE(MID_USB3G, pRet, strError, ##args)

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"



/************************** 常量定义 ************************/

#define USB3G_MAX_ARGS  	        16                  /* nmbd和smbd最大参数个数 */
#define MAX_CMD_LEN			256
#define MAX_COMM_LEN                  64

#define PIN_DISABLED "0"
#define PIN_LOCKED "2"
#define PIN_READY "1"

/* 正则 */
#define USB3G_REGEX_PREFIX			    "^InternetGatewayDevice\\.X_TWSZ-COM_USB3G\\."

/* 节点路径定义 */
#define USB3G_PATH_PREFIX		        "InternetGatewayDevice.X_TWSZ-COM_USB3G."

#define NODE_USB3GENABLE		         "USB3GEnable"
#define NODE_STATE				  		 "state"
#define NODE_NEEDPINCODE			     "NeedPinCode"
#define NODE_PINACTSTA					 "PinActSta"
#define NODE_PINENABLE					 "PinEnable"
#define NODE_PINCHGENABLE				 "PinChgEnable"
#define NODE_NEEDPUKCODE			     "NeedPukCode"
#define NODE_ENTERPINCODE	             "EnterPinCode"
#define NODE_ENTERPUKCODE	             "EnterPukCode"
#define NODE_REMAINPINTIMES	             "RemainPinTimes"
#define NODE_REMAINCHGPINTIMES	         "RemainChgPinTimes"
#define NODE_REMAINPUKTIMES	             "RemainPukTimes"
#define NODE_OLDPINCODE		             "OldPinCode"
#define NODE_NEWPINCODE		             "NewPinCode"


/* 节点全路径名 */
#define USB3G_PATH_USB3G_ENABLE				USB3G_PATH_PREFIX NODE_USB3GENABLE
#define USB3G_PATH_STATE					USB3G_PATH_PREFIX NODE_STATE
#define USB3G_PATH_NEEDPINCODE				USB3G_PATH_PREFIX NODE_NEEDPINCODE
#define USB3G_PATH_PINACTSTA				USB3G_PATH_PREFIX NODE_PINACTSTA
#define USB3G_PATH_PINENABLE				USB3G_PATH_PREFIX NODE_PINENABLE
#define USB3G_PATH_PINCHGENABLE				USB3G_PATH_PREFIX NODE_PINCHGENABLE
#define USB3G_PATH_PUKCODE					USB3G_PATH_PREFIX NODE_NEEDPUKCODE
#define USB3G_PATH_ENTERPINCODE				USB3G_PATH_PREFIX NODE_ENTERPINCODE
#define USB3G_PATH_ENTERPUKCODE				USB3G_PATH_PREFIX NODE_ENTERPUKCODE
#define USB3G_PATH_REMAINPINTIMES			USB3G_PATH_PREFIX NODE_REMAINPINTIMES
#define USB3G_PATH_REMAINPUKTIMES			USB3G_PATH_PREFIX NODE_REMAINPUKTIMES
#define USB3G_PATH_OLDPINCODE				USB3G_PATH_PREFIX NODE_OLDPINCODE
#define USB3G_PATH_NEWPINCODE				USB3G_PATH_PREFIX NODE_NEWPINCODE

#define MAX_NODE_LENGTH 10

/* 节点枚举值定义 */
#define USB3G_STATUS_STOPPED	    0
#define USB3G_STATUS_STARTED	    1
#define USB3G_STATUS_RESTART	    2

#define MAX_MOUNT    16
/******************************************************************************
 *                                ENUM                                      *
 ******************************************************************************/

/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/
 /*
typedef struct tag_ST_USB3G_Nodes
{    int     bUSB3GEnable;
    char    *szWorkgroup;
    char    *szNetbiosName;
    char    *szNewpassword;
    int     bUSBEnable;
}ST_USB3G_Nodes;
*/

/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init and Destroy */
int USB3G_ModuleInit(void);
int USB3G_ModuleDestroy(void);


#endif //__TBS_USB3G_H




