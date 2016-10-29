/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: samba.h

 文件描述: samba模块的头文件

 修订记录:
           1 作者 : 张志勇
             日期 : 2010-2-22
             描述 :
**********************************************************************/
#ifndef __TBS_SAMBA_H
#define __TBS_SAMBA_H

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

#ifdef SAMBA_DEBUG
#define SAMBA_TRACE(fmt, args...)               COMMON_TRACE(MID_SAMBA, fmt"\n", ##args)
#define SAMBA_FUNC_IN(fmt, args...)             SAMBA_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define SAMBA_FUNC_OUT()                        SAMBA_TRACE("--O %s\n", __FUNCTION__)


#else
#define SAMBA_TRACE(str, args...)
#define SAMBA_FUNC_IN(fmt, args...)
#define SAMBA_FUNC_OUT()

/* 模块关闭调试时关掉内存调试 */
#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free
#endif
#ifdef calloc
#undef calloc
#endif
#ifdef realloc
#undef realloc
#endif

#endif

#define SAMBA_LOG_ERR(fmt, args...)                             COMMON_LOG_ERROR(MID_SAMBA, fmt, ##args)

#define SAMBA_RET_LOG(iRet, strError, args...)                  RET_LOG(MID_SAMBA, iRet, strError, ##args)
#define SAMBA_POINTER_LOG(pRet, strError, args...)              POINTER_LOG(MID_SAMBA, pRet, strError, ##args)

#define SAMBA_RET_RETURN(iRet, strError, args...)               RET_RETURN(MID_SAMBA, iRet, strError, ##args)
#define SAMBA_POINTER_RETURN(pRet, strError, args...)           POINTER_RETURN(MID_SAMBA, pRet, strError, ##args)

#define SAMBA_RET_GOTO(iRet, strError, gotoTag)                 RET_GOTO(MID_SAMBA, iRet, strError, gotoTag)
#define SAMBA_POINTER_GOTO(pRet, gotoTag, strError, args...)    POINTER_GOTO(MID_SAMBA, pRet, gotoTag ,strError, ##args)

#define SAMBA_RET_FALSE(iRet, strError, args...)                RET_FALSE(MID_SAMBA, iRet, strError, ##args)
#define SAMBA_POINTER_FALSE(pRet, strError, args...)            POINTER_FALSE(MID_SAMBA, pRet, strError, ##args)

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"



/************************** 常量定义 ************************/
#define SAMBA_NMBD_NAME			    "/sbin/nmbd"
#define SAMBA_SMBD_NAME			    "/sbin/smbd"
#define SAMBA_PASSWD_FILE			    "/var/smbpasswd"
#define SAMBA_PRVATE_PATH			    "/tmp/smbvar/"
#define SAMBA_CONF_FILE		           "/var/smb.conf"
#define USB_RUN_INFO                              "/var/run/usb/sda"
#define SAMBA_MAX_ARGS  	        16                  /* nmbd和smbd最大参数个数 */
#define MAX_CMD_LEN			256
#define MAX_COMM_LEN                  64
#define MAX_SAMBA_PATH_LEN  128
#define MAX_USER_NUM        64
#define MAX_USER_NAME_LEN   65
#define MAX_PASSWORD_LEN   65

const char NMBDPID[] = "/var/tmp/smbvar/nmbd-smb.conf.pid";
const char SMBDPID[]= "/var/tmp/smbvar/smbd-smb.conf.pid";

/* 节点路径定义 */
#define SAMBA_PATH_PREFIX		        "InternetGatewayDevice.X_TELEFONICA-ES_CIFS."

#define NODE_SambaEnable		        "Enable"
#define NODE_Workgroup				    "WorkgroupName"
#define NODE_NetbiosName		        "NetbiosName"
#define NODE_Newpassword		        "Newpassword"
#define NODE_USBEnable		            "USBEnable"
#define NODE_AnonymousEnable		    "AnonymousEnable"

#define SAMBA_NODE_DIRECTORY            "Directory"
#define SAMBA_NODE_NAME                 "Name"
#define SAMBA_NODE_DESCRIPTION          "Description"
#define SAMBA_NODE_READONLY             "ReadOnly"
#define SAMBA_NODE_ENABLED              "Enabled"
#define SAMBA_NODE_PASSWORD             "Password"

/* 节点全路径名 */
#define SAMBA_PATH_SAMBA_ENABLE	        SAMBA_PATH_PREFIX NODE_SambaEnable
#define SAMBA_PATH_USB_ENABLE           SAMBA_PATH_PREFIX NODE_USBEnable
#define SAMBA_PATH_ANONYMOUS_ENABLE     SAMBA_PATH_PREFIX NODE_AnonymousEnable
#define SAMBA_NODE_CIFS_PREFIX          SAMBA_PATH_PREFIX "CIFS."
#define SAMBA_NODE_USER_PREFIX          SAMBA_PATH_PREFIX "User."

/* 正则 */
#define SAMBA_PREFIX_REGEX           "^InternetGatewayDevice\\.X_TELEFONICA-ES_CIFS\\."
#define SAMBA_NODE_PREFIX_REGEX      SAMBA_PREFIX_REGEX "(CIFS|User)\\.[0-9]+\\."
#define SAMBA_NODE_CIFS_PREFIX_REGEX SAMBA_PREFIX_REGEX "CIFS\\.[0-9]+\\."
#define SAMBA_NODE_USER_PREFIX_REGEX SAMBA_PREFIX_REGEX "User\\.[0-9]+\\."
#define SAMBA_DEL_REGEX              SAMBA_PREFIX_REGEX "(CIFS|User)\\.[0-9]+\\.$"

/* 节点枚举值定义 */
#define SAMBA_STATUS_STOPPED	    0
#define SAMBA_STATUS_STARTED	    1
#define SAMBA_STATUS_RESTART	    2

#define MAX_MOUNT    16
/******************************************************************************
 *                                ENUM                                      *
 ******************************************************************************/

/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/
 /*
typedef struct tag_ST_SAMBA_Nodes
{    int     bSambaEnable;
    char    *szWorkgroup;
    char    *szNetbiosName;
    char    *szNewpassword;
    int     bUSBEnable;
}ST_SAMBA_Nodes;
*/

typedef struct {
	bool bEnabled;
	char szUsername[MAX_USER_NAME_LEN];
	char szPassword[MAX_PASSWORD_LEN];
}ST_SAMBA_User;

typedef struct usb_mount
{
    int used;
    char usb_name[MAX_COMM_LEN];
}USB_MOUNT;

/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init and Destroy */
int TF_SAMBA_ModuleInit(void);
int TF_SAMBA_ModuleDestroy(void);


#endif //__TBS_SAMBA_H




