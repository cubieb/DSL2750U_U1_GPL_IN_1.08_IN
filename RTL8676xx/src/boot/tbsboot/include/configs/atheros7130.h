

#ifndef __CONFIG_H
#define __CONFIG_H

#include "sdram_layout.h"

#define __BIG_ENDIAN 		4321
#define __BYTE_ORDER    	__BIG_ENDIAN
#define ATHEROS  //for kernel entry

#define CONFIG_MIPS32		1	/* MIPS 4Kc CPU core	*/
#define CONFIG_ATHEROS7130	1
#define SPI_FLASH 0

#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(256)	/* max number of sectors on one chip */
#define CONFIG_FLASH_BASE		0xbf000000 //PHYS_FLASH_1
#define CFG_FLASH_16BIT         1
#define CFG_FLASH_LAYOUT /*完成flash_layout后定义*/



/*dram param define*/
#define CONFIG_NR_DRAM_BANKS	1
#define CONFIG_DRAM_BASE		0x80000000
#define CFG_SDRAM_SIZE 		0x2000000
#define CFG_UBOOT_LEN 		(0x40000) //bootloader+bootloader_ex+sp
#define CFG_MEMTEST_START	0x80000000
#define CFG_MEMTEST_END		0xa0800000

#define CONFIG_LOADADDR  ( CONFIG_DRAM_BASE + KERNEL_OFFSET )

#define CPU_CLOCK_RATE		240000000	/* 200 MHz clock for the MIPS core */
#define CPU_TCLOCK_RATE 	16588800	/* 16.5888 MHz for TClock */

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
#define CFG_PROMPT		"ar7130# "		/* Monitor Command Prompt    */
#define CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS             16              /* max number of command args*/


/* boot args for kernel*/
#define CONFIG_CMDLINE_TAG	   1	     /* enable passing of ATAGs	 */
#define CONFIG_BOOTARGS		"console=ttyS0,115200  root=31:2 mtdparts=ar7100-nor0:"


#define CFG_BOOTPARAMS_LEN	128*1024

#ifdef CONFIG_PLL_800_400_400

#define CFG_HZ	400000000

#elif CONFIG_PLL_600_400_300

#define CFG_HZ	300000000

#elif CONFIG_PLL_600_400_150

#define CFG_HZ	300000000

#elif CONFIG_PLL_400_400_200

#define CFG_HZ	200000000

#elif CONFIG_PLL_333_333_166

#define CFG_HZ	166000000

#elif CONFIG_PLL_266_266_133

#define CFG_HZ	133000000

#elif CONFIG_PLL_266_266_66
 
#define CFG_HZ	133000000

#elif CONFIG_PLL_200_200_100

#define CFG_HZ	100000000

#else  /* CONFIG_PLL_300_300_150 */

#define CFG_HZ	150000000  //CPU_CLOCK_RATE/2
    	
#endif


/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(20 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */

#define CFG_ENV_SIZE            0x20000

/* The following #defines are needed to get flash environment right */
#define CFG_MONITOR_BASE	TEXT_BASE
//#define CFG_MONITOR_BASE 0xa0000000


#define CFG_MONITOR_LEN		(192 << 10)

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CFG_DCACHE_SIZE		0x8000
#define CFG_ICACHE_SIZE		0x10000
#define CFG_CACHELINE_SIZE	32

/* GPIO registers */
#if 0
#define GPIO_0	( 1 << 0 )
#define GPIO_1	( 1 << 1 )
#define GPIO_2	( 1 << 2 )
#define GPIO_3	( 1 << 3 )
#define GPIO_4	( 1 << 4 )
#define GPIO_5	( 1 << 5 )
#define GPIO_6	( 1 << 6 )
#define GPIO_7	( 1 << 7 )
#define GPIO_8	( 1 << 8 )
#define GPIO_9	( 1 << 9 )
#define GPIO_10	( 1 << 10 )
#define GPIO_11	( 1 << 11 )

#define WPS_KEY			GPIO_1
#define RESET_KEY		GPIO_8
#define JUMPSTART_KEY	GPIO_3

#define USB_LED			GPIO_2
#define JUMPSTART_LED	GPIO_4
#define WPS_LED			GPIO_5

#define ETH_MDC			GPIO_6
#define ETH_MDIO		GPIO_7
#endif

#define GPIO_OE			0xb8040000
#define GPIO_IN			0xb8040004
#define GPIO_OUT		0xb8040008
#define GPIO_SET		0xb804000c
#define GPIO_CLEAR		0xb8040010


#endif	/* __CONFIG_H */
