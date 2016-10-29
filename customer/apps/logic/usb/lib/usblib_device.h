/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : usblib_device.h
 文件描述 :


 函数列表 :


 修订记录 :
          1 创建 : 雷達
            日期 : 2008-5-12
            描述 :

**********************************************************************/
#ifndef	__USBLIB_DEVICE_H__
#define	__USBLIB_DEVICE_H__

#include <string.h>
#include <dirent.h>	/* struct dirent */
#include <sys/stat.h>
#include <sys/vfs.h>
#include "usblib_disk.h"	/* for TDiskInfoContainer */

/* mountpoint content */
typedef struct mountpoint{
	char *block;	/* partition device name. not include path. like: sda1, sdb1 */
	char *mountpoint;	/* mount-point. not include path. like: usb1_1 */
	int flgmounted;	/* flag shows mount-point is mounted or not */
	struct statfs fsstat;	/* file system stat */
}TstMountPointInfo;

/* device's content */
typedef struct device_storage_info{
	char *name;		/* storage device name */

	/* disk info container */
	TDiskInfoContainer *pstDiskInfoCtn;		/* a pointer to the struct containing disk's info (including: disk/part size, fsid etc.) */
	/* mount point */
	int nmountpoing;	/* num of how many sd* mounted  */
	TstMountPointInfo *pstMountPoint;	/* a pointer, for mount-point info */
}TstStorageInfo;

#define	MNTPOINT_INFO_SIZE	(sizeof(TstMountPointInfo))
#define	T_MNTPNT_INF_P(pmntpntInfo)		((TstMountPointInfo *)(pmntpntInfo))
#define	MNTPNT_INF_BLOCK(pmntpntInfo)		(T_MNTPNT_INF_P(pmntpntInfo)->block)
#define	MNTPNT_INF_MNTPNT(pmntpntInfo)		(T_MNTPNT_INF_P(pmntpntInfo)->mountpoint)
#define	MNTPNT_INF_FLGMNTED(pmntpntInfo)		(T_MNTPNT_INF_P(pmntpntInfo)->flgmounted)
#define	MNTPNT_INF_FSSTAT(pmntpntInfo)		(T_MNTPNT_INF_P(pmntpntInfo)->fsstat)

#define	STORAGEINFO_SIZE	(sizeof(TstStorageInfo))
#define	T_STOR_INF_P(pstorInfo)			((TstStorageInfo *)(pstorInfo))
#define	STOR_INF_NAME(pstorInfo)		(T_STOR_INF_P(pstorInfo)->name)
#define	STOR_INF_DSKINF(pstorInfo)		(T_STOR_INF_P(pstorInfo)->pstDiskInfoCtn)
#define	STOR_INF_MNTPNTSUM(pstorInfo)		(T_STOR_INF_P(pstorInfo)->nmountpoing)
#define	STOR_INF_MNTPNTINF(pstorInfo)		(T_STOR_INF_P(pstorInfo)->pstMountPoint)

/* not interest */
#define	IS_INF_ENTRY_INVALID(pfileEntry)	('.' == ((struct dirent *)(pfileEntry))->d_name[0])

#ifdef	CONFIG_SYSFS
#define	SYSFS_PATH		"/sys"

#ifdef CONFIG_APPS_LOGIC_TF_GUI
typedef enum{
	SYSFS_DEV_STOR_VENDOR,
	SYSFS_DEV_STOR_MODEL,
	SYSFS_DEV_STOR_SERIAL,
	SYSFS_DEV_STOR_TYPE,
	SYSFS_DEV_STOR_VERSION
}TenmSysfs_inf_type;
#else
typedef enum{
	SYSFS_DEV_STOR_VENDOR,
	SYSFS_DEV_STOR_MODEL
}TenmSysfs_inf_type;
#endif

/* sdname: sdname without path */
/* if succeed, return a copy of the string, else return NULL */
extern char *storInfo_get_sysfs_info(char *sdname, TenmSysfs_inf_type type);
extern int seek_mountpoint_info(char *sdname, int hStorInfo);
#else
#define	storInfo_get_sysfs_info(a...)	(NULL)
#endif	/* CONFIG_SYSFS */

/* Storage Info Entry functions */
extern int storageInfoEntry_open(void);
extern int storageInfoEntry_get(int hDirEntry);
extern void storageInfoEntry_close(int hDirEntry);
/* Storage Info Functions */
extern int storageInfo_get(int hInfoEntry);
extern void storageInfo_free(int hStorInf);

#endif
