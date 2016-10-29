

#ifndef __CONFIG_H
#define __CONFIG_H

#include "sdram_layout.h"

#define __BIG_ENDIAN 		4321
#define __BYTE_ORDER    	__BIG_ENDIAN
#define AMAZON_SE  //for kernel entry

#define CONFIG_MIPS32		1	/* MIPS 4Kc CPU core	*/
#define CONFIG_AMAZON_SE	1

/****************** MARK by Zhang Yu **********
#define CFI_FLASH 		1
#define CFG_AMAZON_SE_FLASH_CFI_DRIVER	1
#define CFG_FLASH_CFI		1
************************************************/

#define SPI_FLASH		1 
#define CFG_AMAZON_SE_FLASH_SPI_DRIVER	1

#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(256)	/* max number of sectors on one chip */
#define CFG_FLASH_BASE		0xb0000000 //PHYS_FLASH_1
#define CFG_FLASH_16BIT         1
#define CFG_FLASH_LAYOUT /*完成flash_layout后定义*/



/*dram param define*/
#define CONFIG_NR_DRAM_BANKS	1
#define CONFIG_DRAM_BASE		0x80000000
#define CFG_SDRAM_UNCACHE_BASE	0xa0000000
#define CFG_SDRAM_SIZE 		0x1000000
#define CFG_UBOOT_LEN 		(0x40000) //bootloader+bootloader_ex+sp
#define CFG_MEMTEST_START	0x80000000
#define CFG_MEMTEST_END		0xa0800000

#define CONFIG_LOADADDR  ( CONFIG_DRAM_BASE+ KERNEL_OFFSET )

#define CONFIG_CONS_INDEX	1
#define CONFIG_BOOTDELAY	1	/* autoboot after 1 seconds	*/

#define CONFIG_BAUDRATE		115200

#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

#define CONFIG_TIMESTAMP		/* Print image info with timestamp */

#define CONFIG_PREBOOT	"echo;" \
	"echo Type \"boot\" for the network boot using DHCP, TFTP and NFS;" \
	"echo Type \"run netboot_initrd\" for the network boot with initrd;" \
	"echo Type \"run flash_nfs\" to mount root filesystem over NFS;" \
	"echo Type \"run flash_local\" to mount local root filesystem;" \
	"echo"


#include <cmd_confdefs.h>
/*
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP				/* undef to save memory	     */
#define CFG_PROMPT		"AMAZON_SE# "		/* Monitor Command Prompt    */
#define CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS             16              /* max number of command args*/


/* boot args for kernel*/
#define CONFIG_CMDLINE_TAG	   1	     /* enable passing of ATAGs	 */
//#define CONFIG_BOOTARGS		"console=ttyS0,115200  root=31:2 mtdparts=Amazon_se_Bank_0:"
#define CONFIG_BOOTARGS		"console=ttyS0,115200  root=31:2 mtdparts=amazon_se-spi:"


#define CFG_BOOTPARAMS_LEN	128*1024
#define CPU_CLOCK_RATE		266666666	/* 266 MHz clock for the MIPS core */

#define CFG_HZ			(CPU_CLOCK_RATE / 2)

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(20 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */

#define CFG_ENV_SIZE            0x20000

/* The following #defines are needed to get flash environment right */
#define CFG_MONITOR_BASE	TEXT_BASE

#define CFG_MONITOR_LEN		(192 << 10)

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CFG_CACHE_SIZE		8192
#define CFG_CACHE_LINES		16
#define CFG_CACHE_WAYS		2
#define CFG_CACHE_SETS		256

#define CFG_ICACHE_SIZE		CFG_CACHE_SIZE
#define CFG_DCACHE_SIZE		CFG_CACHE_SIZE
#define CFG_CACHELINE_SIZE	CFG_CACHE_LINES


#endif	/* __CONFIG_H */
