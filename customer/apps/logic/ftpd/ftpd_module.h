/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : ftpd_module.h
 文件描述 :
 函数列表 :
 修订记录 :
          1 创建 : 雷達
            日期 : 2008-7-14
            描述 :
**********************************************************************/
#ifndef	__FTPD_MODULE_H__
#define	__FTPD_MODULE_H__
#include "autoconf.h"

#ifdef	CONFIG_APPS_LOGIC_FTPD_CT
/* Node for China-Telecom */
#define	PATH_FTPD	"InternetGatewayDevice.DeviceInfo.X_CT-COM_ServiceManage."
#define	NODE_FTPD_ENABLE	"FtpEnable"
#define	NODE_FTPD_PORT	"FtpPort"
#define	NODE_FTPD_USERNAME	"FtpUserName"
#define	NODE_FTPD_PASSWORD	"FtpPassword"

#elif defined(CONFIG_APPS_LOGIC_FTPD_TBS)
/* Node for T&W TBS */
/* 非标准节点名前缀*/
#define	CUST_NODE_NAME_PREFIX "X_TWSZ-COM_"

#define	PATH_FTPD	"InternetGatewayDevice."CUST_NODE_NAME_PREFIX"FTPD."
/* "InternetGatewayDevice.X_TWSZ-COM_FTPD" */
#define	NODE_FTPD_ENABLE	"Enable"
#define NODE_FTPD_WANACCESS_ENABLE "WANAccessEnable"
#define	NODE_FTPD_PORT	"Port"
#define	NODE_FTPD_STATUS	"Status"

#define	PATH_FTPD_ACCOUNT	PATH_FTPD"Account."
/* InternetGatewayDevice.X_TWSZ-COM_FTPD.Account.{i} */
#define	NODE_FTPD_USERNAME	"UserName"
#define	NODE_FTPD_PASSWORD	"Password"
#define	NODE_FTPD_RIGHT_LIST	"EnableList"
#define	NODE_FTPD_RIGHT_UPLOAD	"EnableUpload"
#define	NODE_FTPD_RIGHT_DOWNLOAD	"EnableDownload"
#endif

#define	FTPD_CMD_MAX	8
#define	FTPD_BIN_NAME	"/sbin/ftpd"
#define	FTPD_USER_DB	"/var/ftpvuser.db"
#define	FTPD_RIGHTS_DB	"/var/ftpvright.db"

#define	FTPD_CRYPT_SALT	"TW"
#define	FTPD_ACCOUNT_GROUP	"ftp"
#define	FTPD_ACCOUNT_HOMEDIR	"/mnt"	/* the same as usb storage mountpoint */

#endif
