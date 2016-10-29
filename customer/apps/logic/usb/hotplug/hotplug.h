/**********************************************************************
Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
文件名称 : hotplug.h
文件描述 : Hotplug definitions
修订记录 :
         1 创建 : 雷達
           日期 : 2008-5-12
           描述 :

**********************************************************************/
#ifndef _HOTPLUG_H_
#define _HOTPLUG_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>

#include <sys/wait.h>
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */

#define USB_HOTPLUG_LOG_ENABLE      0

#ifdef USB_HOTPLUG_LOG_ENABLE
#define HT_LOG(args...)  HT_Log("" args)
#else
#define HT_LOG(args...)
#endif



#define	CMD_BUF_SIZE	(256)
#define	CMD_MAX_DATALEN	(CMD_BUF_SIZE - 1)
#define HT_PATH_LENGTH  256
#define PRODUCT_NAME    20

#define WAN_PPP_FMT "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.%u."
#ifdef	UT_USB_MASS
#define	USB_SD_NODEFILE_DIR		"/dev/"
#define	USB_SD_MNT_DIR		"/mnt/"
#define	USB_SD_MNTINFO_DIR	"/var/run/usb/"
#define	UMOUNT_FAIL_LIST  "/var/run/usb/umount_fail_list"

#else
/* usb hotplug class: usb sd(scsi-disk) define */
#define	USB_SD_NODEFILE_DIR		"/dev/usb/sd/"	/* the path that where scsi-disk node file stored */
#define	USB_SD_MNT_DIR		"/mnt/"		/* the path that where the scsi-disk's mountpoint was in */
#define	USB_SD_MNTINFO_DIR	"/var/run/usb/"	/* the path that mounting-info-file store */
#define	UMOUNT_FAIL_LIST  "/var/run/usb/umount_fail_list"
#endif

#include "tbsutil.h"
#define	SYSTEM(cmd)	tbsSystem(cmd, 0)

void HT_Log(char *szFmt, ...);
void HT_Log1(char *szFmt, ...);
extern int hotplug_storage_block(void);
extern int hotplug_storage_mount(char *pdev_fullname, char *pdev_major, char *pdev_minor);
extern int hotplug_storage_umount(char *pdev_fullname);
extern int hotplug_usb_protest(void);
/* usb printer helper. add by radar. 2007.06.15 */
extern int hotplug_usb_lp(void);
extern int HT_MDStatusChg(void);

extern int hotplug_usb_device(void);
extern int HT_MDProbe(void);
extern void HT_Log(char *szFmt, ...);
extern short HT_MyCopy(char *dest, char *src, char stopchar, short maxlen);

#endif
