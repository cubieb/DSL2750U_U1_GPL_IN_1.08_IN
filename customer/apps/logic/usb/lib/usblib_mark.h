/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : usblib_mark.h
 文件描述 : Hotplug标志处理函数


 函数列表 :


 修订记录 :
          1 创建 : 雷達
            日期 : 2008-5-12
            描述 :

**********************************************************************/
#ifndef	__USBLIB_HOTPLUG_H__
#define	__USBLIB_HOTPLUG_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef	SUPPORT_USB_SDNEWS_MARK
#define	SDNEWS_PATH		"/var/sdnews"	/* if a new usb sd plug-in or pull-out will be record the action */
/* usb scsi-disk hotplug: add or remove will call */
#define	CREATE_SDNEWS()	do { int fd = creat(SDNEWS_PATH, O_CREAT); if(fd > 0) close(fd); } while(0)
#define	ERASE_SDNEWS()	remove(SDNEWS_PATH)

static inline int is_sdnews_exist(void)
{
	int fd = open(SDNEWS_PATH, O_RDONLY);
	if(fd > 0)
		close(fd);
	return (0 < fd);
}
#else
#define	CREATE_SDNEWS()
#define	ERASE_SDNEWS()
#define	is_sdnews_exist()	(0)
#endif

#endif
