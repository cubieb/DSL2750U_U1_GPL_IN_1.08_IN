

#ifndef __CONFIG_H
#define __CONFIG_H

#include "sdram_layout.h"

#define __BIG_ENDIAN 		4321
#define __BYTE_ORDER    	__BIG_ENDIAN
#define RTL8196C	 //for kernel entry

#define CONFIG_MIPS32		1	/* MIPS 4Kc CPU core	*/
#define CONFIG_RTL8196C		1
#define CONFIG_LX4380		1

#ifdef CONFIG_RTL819X_SPI_FLASH
#define SPI_FLASH 				1
//#define CONFIG_FLASH_BASE          0xbfc00000 //PHYS_FLASH_1
#define CONFIG_FLASH_BASE          0xbd000000 //PHYS_FLASH_1
#else
#define CFI_FLASH               1
#define CFG_FLASH_CFI_DRIVER_RTL8672 1
#define CFG_FLASH_CFI           1
#define CONFIG_FLASH_BASE          0xbd000000 //PHYS_FLASH_1
#endif

#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(512)	/* max number of sectors on one chip */
#define CFG_FLASH_16BIT         1
#define CFG_FLASH_LAYOUT /*完成flash_layout后定义*/

/*dram param define*/
#ifndef CONFIG_DRAM_SIZE
#define CONFIG_DRAM_SIZE        32
#endif

#define CONFIG_NR_DRAM_BANKS	1
#define CONFIG_DRAM_BASE		0x80000000
#define CFG_SDRAM_SIZE 		( ( 1 << 20 ) * CONFIG_DRAM_SIZE )
#define CFG_UBOOT_LEN 		(0x40000) //bootloader+bootloader_ex+sp
#define CFG_MEMTEST_START	0x80000000
#define CFG_MEMTEST_END		0xa0800000
#define CONFIG_LOADADDR  ( CONFIG_DRAM_BASE + KERNEL_OFFSET )

//#define CPU_CLOCK_RATE		240000000	/* 200 MHz clock for the MIPS core */
//#define CPU_TCLOCK_RATE 	16588800	/* 16.5888 MHz for TClock */

#define SUPPORT_8672_PLATFORM

/* System Bus Clock configuration, undefined SYS_CLOCK_175MHZ is set to 200MHz */
//#define SYS_CLOCK_175MHZ

/* SDRAM Clock configuration, undefined 116Mhz and 166Mhz is set to 133Mhz */
#define SDRAM_CLOCK_166MHZ

/* CPU (OCP) Clock Configuration, undefined 340Mhz is set as 400Mhz */
//#define CPU_CLOCK_340MHZ

#ifndef SYS_CLOCK_175MHZ
#define CLOCKGEN_FREQ	200000000
#else
#define CLOCKGEN_FREQ	175000000
#endif

#define MCR1_VAL	0x1818ffff
#define MCR2_VAL	0x00000CC9


/* System interface */
/*internal phy*/
//#define SICR_VAL	0xb7053a19    //for external phy & PCI
#define SICR_VAL	0xb7053819    //for internal phy & PCI

/* Lexra Bus Arbitor */
#define CPUC_VAL	0x0c0100FF    //for device bus weighting

#define SCCR		0xb8003200

#define TC_BASE		0xB8003100
#define TC0DATA		(TC_BASE+0x00)
#define TC1DATA		(TC_BASE+0x04)
#define TC0CNT		(TC_BASE+0x08)
#define TC1CNT		(TC_BASE+0x0C)
#define TCCNR		(TC_BASE+0x10)
#define TCIR			(TC_BASE+0x14)
#define CDBR			(TC_BASE+0x18)
#define WDTCNT		(TC_BASE+0x1C)
#define BSTMOUT		(TC_BASE+0x20)

#define WDTE_OFFSET		24              /* Watchdog enable */
#define WDSTOP_PATTERN	0xA5            /* Watchdog stop pattern */
#define WDTCLR				(1 << 23)       /* Watchdog timer clear */
#define OVSEL_13			0               /* Overflow select count 2^13 */
#define WDTKICK				(WDTCLR|OVSEL_13)  /* command for kick watchdog */


#define CONFIG_CONS_INDEX	1
#define CONFIG_BOOTDELAY	2	/* autoboot after 2 seconds	*/

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
#define CFG_PROMPT		"RTL8196# "		/* Monitor Command Prompt    */
#define CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS             16              /* max number of command args*/


/* boot args for kernel*/
#define CONFIG_CMDLINE_TAG	   1	     /* enable passing of ATAGs	 */

#ifdef SPI_FLASH
#define CONFIG_BOOTARGS		"console=ttyS0,115200  root=31:2 mtdparts=spi_flash:"
#else
#define CONFIG_BOOTARGS		"console=ttyS0,115200  root=31:2 mtdparts=physmap-flash.0:"
#endif


#define CFG_BOOTPARAMS_LEN	128*1024

#define CFG_HZ	1000 //CLOCKGEN_FREQ  

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
#define CFG_DCACHE_SIZE		0x8000
#define CFG_ICACHE_SIZE		0x10000
#define CFG_CACHELINE_SIZE	32

#endif	/* __CONFIG_H */

