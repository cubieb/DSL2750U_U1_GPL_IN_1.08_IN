/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : usblib_mountinf.h
 文件描述 : Hotplug标志处理函数


 函数列表 :


 修订记录 :
          1 创建 : 雷達
            日期 : 2008-5-12
            描述 :

**********************************************************************/
#ifndef	__USBLIB_MOUNTINF_H__
#define	__USBLIB_MOUNTINF_H__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	/* open, create */
#include <unistd.h> /* read */

/************************************************
mountpoint info file struct
# file begin
line1:	%d		#sum of mountpoint
line2:	%s\t%s\t	%d	# sdname \t usb%d_%d \t mounted
linen:	%s\t%s\t	%d	# same as upper
# file end.
************************************************/

#define	MNTPNT_NAME_BUFSIZE	(64)	/* mount point full path size */
#define	MOUNTPOINT_FMT0	"0"	/* no mountpoing */
#define	MOUNTPOINT_FMT1	"usb%d_%d"	/* default format */
#define	MOUNTPOINT_FMT2	"%s"	/* customer string */
#define	INFO_LINE_MOUNTED_FMT(fmt)	"%s\t"fmt"\t%d"	//"%s\tusb%d_%d\t%d\n"

#define	USB_MNT_INF_LNBUF_SIZE	(64)

#define	INFO_SNPRINTF(linebuf, buflen, blockname, idxdisk, idxpart, flgmounted)	\
		do {	\
			if (flgmounted)	\
				snprintf((linebuf), buflen, INFO_LINE_MOUNTED_FMT(MOUNTPOINT_FMT1), \
					(blockname), (idxdisk), (idxpart), (flgmounted));	\
			else	\
				snprintf((linebuf), buflen, INFO_LINE_MOUNTED_FMT(MOUNTPOINT_FMT0), 	\
					(blockname), (flgmounted) );	\
		}while(0)

#define	INFO_SSCANF(buf, blockname, mountpoint, flgmounted)	\
		sscanf((buf), INFO_LINE_MOUNTED_FMT(MOUNTPOINT_FMT2), 	\
			(blockname), (mountpoint), &(flgmounted))

#define	USB_MOUNTSUM_FMT	"%03d"	/* mount sum: xxx */
#define	USB_MOUNTSUM_SIZE	(4)	/* equal strlen($mountnum)+1 */
/*
*   replace the mount-sum line in file with new line.
*   must be the same length.
*   -1 for error;
*   0 for success.
*/
int usb_mountsum_set(char *filepath, int num)
{
    char buf[8] = {0};
    size_t nbytes = 0;
    int fd = 0;
    int ret = 0;

    fd = open(filepath, O_RDWR); // O_SYNC |
    if (fd < 0){
        fd = open(filepath, O_CREAT | O_TRUNC | O_WRONLY);  // | O_SYNC);
        if (fd < 0){
            /* lumme, file can't be created?! */
            perror("open");
            return -1;
        }
    }
    nbytes = snprintf(buf, sizeof(buf)-1, USB_MOUNTSUM_FMT"\n", num);
    if (nbytes > 0)
        ret = write(fd, buf, nbytes);
    close(fd);
    if (ret > 0)
        return 0;
    else
        return -1;
}

/*****************************************
 * negative for failed; see ERRORS in errno.
 * >=0 for successe.
 ******************************************/
int usb_mountsum_get(char *filepath)
{
    int fd = 0;
    char mountsumbuf[USB_MOUNTSUM_SIZE] = {0};
    int ret = 0;

    fd = open(filepath, O_RDONLY); /*O_SYNC |  */
    if (fd < 0){
        return (-1);
    }

    /* read number only */
    ret = read(fd, mountsumbuf, USB_MOUNTSUM_SIZE-1);
    if (ret > 0)
        ret = atoi(mountsumbuf);
    else
        ret = -1;

    close(fd);
    return ret;
}

#endif /* __USBLIB_MOUNTINF_H__ */
