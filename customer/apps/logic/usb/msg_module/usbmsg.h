/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称:usbmsg.h
 文件描述:usb Mass Storage 设备信息处理模块头文件
 修订记录:
          1 创建 : 雷達
            日期 : 2008-5-12
            描述 :

          2 修改 : 张喻
            日期 : 2009-3-26
            描述 : 添加USB快速恢复开关节点

**********************************************************************/

#ifndef __USBMSG_H__
#define __USBMSG_H__


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

#define CFG_USB_OBJ             "object"

/* node path */
#define PATH_USBDEV             "InternetGatewayDevice.X_TWSZ-COM_USBDevice"
#define PATH_USBDEV_MASSDEV_SUM "InternetGatewayDevice.X_TWSZ-COM_USBDevice.StorageSum"
#define PATH_USBDEV_MASS        "InternetGatewayDevice.X_TWSZ-COM_USBDevice.Storage"

/* CFG Restore */
#define PATH_USBDEV_CFG_RESTORE_ENABLE  "InternetGatewayDevice.DeviceInfo.X_TWSZ-COM_Restore.Enable"


/* 检查配置备份文件是否存在相关宏定义 */
/* Device Name */
#define PATH_USBDEV_CFG_DEVICE_NAME     "InternetGatewayDevice.DeviceInfo.ModelName"

#define USB_MASS_MOUNT_PATH             "/mnt"
#define USB_MASS_CFG_RESTORE_DIR        "e8_Config_Backup"


/* Object Storage SubObject*/
/* InternetGatewayDevice.USBDevice.Storage.{i}. */
#define NODE_USB_MASS_VENDOR        "Vendor"
#define NODE_USB_MASS_MODEL         "Model"
#ifdef CONFIG_APPS_LOGIC_TF_GUI
#define NODE_USB_MASS_SERIAL        "serial"
#define NODE_USB_MASS_TYPE          "type"
#define NODE_USB_MASS_VERSION       "version"
#endif
#define NODE_USB_MASS_DISKSIZEMB    "DiskSizeMB"
#define NODE_USB_MASS_PARTITIONSUM  "PartitionSum"
/* Object Partitions */
#define NODE_USB_MASS_PARTITIONS    "Partitions"
/* Partitions SubObject */
/* InternetGatewayDevice.USBDevice.Storage.{i}.Partitions.{*}. */
#define NODE_USB_MASS_PARTS_SDNAME      "SDName"
#define NODE_USB_MASS_PARTS_BOOT        "Boot"
#define NODE_USB_MASS_PARTS_PARTUSEDKB  "PartUsedKB"
#define NODE_USB_MASS_PARTS_PARTSIZEMB  "PartSizeMB"
#define NODE_USB_MASS_PARTS_SYSID       "SysID"
#define NODE_USB_MASS_PARTS_FILESYSTEM  "FileSystem"
#define NODE_USB_MASS_PARTS_PERCENTUSED "PercentUsed"
#define NODE_USB_MASS_PARTS_MOUNTPOINT  "MountPoint"
#define NODE_USB_MASS_PARTS_MOUNTED     "Mounted"
#define NODE_USB_MASS_PARTS_CFGFILE_EXIST  "CfgBackupFileExist"


/******************************************************************************
*                                 END                                        *
******************************************************************************/

#endif /* __USBMSG_H__ */
