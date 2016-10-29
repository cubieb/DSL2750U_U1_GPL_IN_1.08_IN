#ifndef __FLASH_LAYOUT_KERNEL_H__
#define __FLASH_LAYOUT_KERNEL_H__
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/compatmac.h>
#include <asm/uaccess.h>
#include <autoconf.h>
#include <flash_layout.h>


#ifdef CONFIG_FLASH_DEBUG
#define fldebug(fmt, args...)		printk(fmt, ##args)
#else
#define fldebug(fmt, args...)       
#endif
#define IMG_FILE "/var/image.img"

extern struct semaphore item_sem;

#define CFG_MAX_FLASH_SECT 256
#define		BACKUP_IMG 0x01
#define     LEGACY_IMG 0x02

#ifdef CONFIG_DUAL_IMAGE
#ifndef CONFIG_DOUBLE_BACKUP
#define CONFIG_DOUBLE_BACKUP
#endif
#endif

typedef struct flash_info{
	ulong	size;			/* total bank size in bytes		*/
	ushort	sector_count;		/* number of erase units		*/
	ulong	flash_id;		/* combined device & manufacturer code	*/
	ulong	start[CFG_MAX_FLASH_SECT];   /* physical sector start addresses */
	unsigned char	protect[CFG_MAX_FLASH_SECT]; /* sector protection status	*/

	unsigned char	portwidth;		/* the width of the port		*/
	unsigned char	chipwidth;		/* the width of the chip		*/
	ushort	buffer_size;		/* # of bytes in write buffer		*/
	ulong	erase_blk_tout;		/* maximum block erase timeout		*/
	ulong	write_tout;		/* maximum write timeout		*/
	ulong	buffer_write_tout;	/* maximum buffer write timeout		*/
	ushort	vendor;			/* the primary vendor id		*/
	ushort	cmd_reset;		/* Vendor specific reset command	*/
	ushort	interface;		/* used for x8/x16 adjustments		*/
	ushort	legacy_unlock;		/* support Intel legacy (un)locking	*/
}flash_info_t;


struct mtd_part {
	struct mtd_info mtd;
	struct mtd_info *master;
	u_int32_t offset;
	int index;
	struct list_head list;
	int registered;
};

extern int kernel_item_get( void *data , char *item_name , size_t *len);
extern int kernel_item_save( void *data , char *item_name , size_t len);
extern int crc16(unsigned char *buf, int len, unsigned short *checksum);
extern void flash_info_get(struct flash_info *flash_info_low, struct mtd_info *mtd_master);
extern int is_sysdata(void *sys_data);
extern int kernel_sysdata_get(void *syscfg);
extern int kernel_sysdata_save(void *syscfg);
extern int kernel_image_update(int sync);
extern int kernel_erase(struct mtd_info *mtd_master,flash_info_t *finfo,unsigned int addr,unsigned int len);
extern int kernel_write(struct mtd_info *mtd_master,unsigned char * src,unsigned int addr,unsigned int len);

#ifdef CONFIG_TBS_WATCHDOG
extern void watchdog_close( void );
extern void watchdog_open( void );
#endif

#endif
