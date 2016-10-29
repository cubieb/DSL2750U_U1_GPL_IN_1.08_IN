

#ifndef __CONFIG_H
#define __CONFIG_H

#define BCM63XX_CHIP

#define __BIG_ENDIAN 4321
#define __BYTE_ORDER    __BIG_ENDIAN

#define CONFIG_MIPS32		1	/* MIPS 4Kc CPU core	*/
#define CONFIG_BCM6338		1	/* on a BCM6338 Board	*/

/*flash param define*/
#define CFG_FLASH_CFI
#define CFG_FLASH_CFI_DRIVER
//#define CFG_FLASH_PROTECTION  	/*hardware protect*/
#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(128)	/* max number of sectors on one chip */
#define CONFIG_FLASH_BASE		0xbe000000 //PHYS_FLASH_1
#define CFG_FLASH_16BIT         1
#define PHYS_FLASH_1		0xbfc00000 /* Flash Bank #1 */

/*dram param define*/
#define CONFIG_NR_DRAM_BANKS	1
#define CONFIG_DRAM_BASE		0x80000000
#define CFG_MEMTEST_START	0x80000000
#define CFG_MEMTEST_END		0x80800000
#define CFG_LOAD_ADDR		0x80400000	/* default load address */

#ifndef CPU_CLOCK_RATE
#define CPU_CLOCK_RATE	240000000	/* 200 MHz clock for the MIPS core */
#endif
#define CPU_TCLOCK_RATE 16588800	/* 16.5888 MHz for TClock */

#define CONFIG_CONS_INDEX	1
//#define CONFIG_BOOTDELAY	5	/* autoboot after 5 seconds	*/

#define CONFIG_BAUDRATE		115200

#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

#define CONFIG_TIMESTAMP		/* Print image info with timestamp */

#define CONFIG_PREBOOT	"echo;" \
	"echo Type \"boot\" for the network boot using DHCP, TFTP and NFS;" \
	"echo Type \"run netboot_initrd\" for the network boot with initrd;" \
	"echo Type \"run flash_nfs\" to mount root filesystem over NFS;" \
	"echo Type \"run flash_local\" to mount local root filesystem;" \
	"echo"

#undef	CONFIG_BOOTARGS

#define CONFIG_EXTRA_ENV_SETTINGS					\
	"netboot=dhcp;tftp;run netargs; bootm\0"			\
	"nfsargs=setenv bootargs root=/dev/nfs ip=dhcp\0"		\
	"localargs=setenv bootargs root=1F02 ip=dhcp\0"			\
	"addmisc=setenv bootargs ${bootargs} "				\
		"console=ttyS0,${baudrate} "				\
		"read-only=readonly\0"					\
	"netargs=run nfsargs addmisc\0"					\
	"flash_nfs=run nfsargs addmisc;"				\
		"bootm ${kernel_addr}\0"				\
	"flash_local=run localargs addmisc;"				\
		"bootm ${kernel_addr}\0"				\
	"netboot_initrd=dhcp;tftp;tftp 80600000 initrd;"		\
		"setenv bootargs root=/dev/ram ramdisk_size=8192 ip=dhcp;"\
		"run addmisc;"						\
		"bootm 80400000 80600000\0"				\
	"rootpath=/export/miniroot-mipsel\0"				\
	"autoload=no\0"							\
	"kernel_addr=BFC60000\0"					\
	"ramdisk_addr=B0100000\0"					\
	"u-boot=u-boot.bin\0"						\
	"bootfile=uImage\0"						\
	"load=dhcp;tftp 80400000 ${u-boot}\0"				\
	"load_kernel=dhcp;tftp 80400000 ${bootfile}\0"			\
	"update_uboot=run load;"					\
		"protect off BFC00000 BFC3FFFF;"			\
		"erase BFC00000 BFC3FFFF;"				\
		"cp.b 80400000 BFC00000 ${filesize}\0"			\
	"update_kernel=run load_kernel;"				\
		"erase BFC60000 BFD5FFFF;"				\
		"cp.b 80400000 BFC60000 ${filesize}\0"			\
	"initenv=erase bfc40000 bfc5ffff\0"				\
	""
/*#define CONFIG_BOOTCOMMAND	"run flash_local" */
#define CONFIG_BOOTCOMMAND	"run netboot"
#include <cmd_confdefs.h>
/*
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP				/* undef to save memory	     */
#define CFG_PROMPT		"# "		/* Monitor Command Prompt    */
#define CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS             16              /* max number of command args*/


#define CFG_MALLOC_LEN		128*1024

#define CFG_BOOTPARAMS_LEN	128*1024

#define CFG_HZ			(CPU_TCLOCK_RATE/4)

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(20 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */

#define CFG_INIT_SP_OFFSET      0x400000
#define CFG_ENV_SIZE            0x20000

/* The following #defines are needed to get flash environment right */
#define CFG_MONITOR_BASE	TEXT_BASE
#define CFG_MONITOR_LEN		(192 << 10)

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CFG_DCACHE_SIZE		8192
#define CFG_ICACHE_SIZE		16384
#define CFG_CACHELINE_SIZE	16

#endif	/* __CONFIG_H */
