/*
 *  TBS ioctl 命令字定义
 *
 *
 *  说明:TBS私有ioctl使用使用G字段生成的命令字
 *
 *       by Zhang Yu
 *
 *
 *	添加TBS条目的获取和保存IOCTL号
 *	2008-10-27 by xuanguanglei
 *
 */

#ifndef __LINUX_TBS_IOCTL_H_INCLUDED
#define __LINUX_TBS_IOCTL_H_INCLUDED


#define TBS_IOCTL_MAGIC	 'G'

/* ioctl led命令字定义 */

#define TBS_IOCTL_LED_SET       _IOWR(TBS_IOCTL_MAGIC, 0, struct tbs_ioctl_led_parms)
#define TBS_IOCTL_LED_GET       _IOWR(TBS_IOCTL_MAGIC, 1, struct tbs_ioctl_led_parms)
#define TBS_IOCTL_ITEM_GET      _IOWR(TBS_IOCTL_MAGIC, 2, item_t)
#define TBS_IOCTL_ITEM_SAVE     _IOWR(TBS_IOCTL_MAGIC, 3, item_t)
#define TBS_IOCTL_MAC_READ      _IOWR(TBS_IOCTL_MAGIC, 4, mac_t)
#define TBS_IOCTL_IMAGE_SYNC    _IOWR(TBS_IOCTL_MAGIC, 5, item_t) 

#endif /* __LINUX_TBS_IOCTL_H_INCLUDED */

