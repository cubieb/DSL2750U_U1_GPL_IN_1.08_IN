

#ifndef __CONFIG_H
#define __CONFIG_H

#include "sdram_layout.h"

#define __BIG_ENDIAN 		4321
#define __BYTE_ORDER    	__BIG_ENDIAN
#define VRX288				//for kernel entry

#define CONFIG_MIPS32		1	/* MIPS 4Kc CPU core	*/
#define CONFIG_VRX288		1
//#define	CONFIG_BOOT_FROM_SPI		1
//#define	CONFIG_BOOT_FROM_NOR	1

//#define CFI_FLASH 			1
//#define	CFG_FLASH_CFI_DRIVER		1
//#define CFG_VRX288_FLASH_CFI_DRIVER	1
//#define CFG_FLASH_CFI		1
//#define	UBOOT_SECOND_OFFSET			0x5400 //3800 
//#define	CONFIG_VR9_GPHY_FW_OFFSET	0xc800 //0xac00 0xe800
#define	CFG_ALIGN_LENTH			0x400
#define	CFG_GPHY_FW_ADDR		0xa0200000

#define SPI_FLASH				1 
#define CONFIG_ENV_SPI_CS 		3
#define CONFIG_ENV_SPI_BUS		0
#define	CONFIG_ENV_SPI_MAX_HZ	1000000
#define	CONFIG_ENV_SPI_MODE		0
#define CFG_VRX288_FLASH_SPI_DRIVER	1
#define CONFIG_SPI_FLASH_MXIC 			1

#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(128)	/* max number of sectors on one chip */
//#define CFG_FLASH_16BIT         1
//#define CFG_FLASH_LAYOUT /*完成flash_layout后定义*/

#define PHYS_FLASH_1		0xB0000000 /* Flash Bank #1 */
#define PHYS_FLASH_2		0xB4000000 /* Flash Bank #2 */
#define IFX_SPI_BASE        0xBE100800
#define CONFIG_FLASH_BASE		0xE0000000

/* Swith ports settings */
#define CONFIG_VR9_SW_PORT_0 1
#define CONFIG_VR9_SW_PORT_1 1
#define CONFIG_VR9_SW_PORT_2 1
#define CONFIG_VR9_SW_PORT_4 1
#define CONFIG_VR9_SW_PORT_5a 1
#define CONFIG_VR9_SW_PORT0_MIIMODE 4
#define CONFIG_VR9_SW_PORT1_MIIMODE 4
#define CONFIG_VR9_SW_PORT2_MIIMODE 1
#define CONFIG_VR9_SW_PORT4_MIIMODE 1
#define CONFIG_VR9_SW_PORT5a_MIIMODE 4
#define CONFIG_VR9_SW_PORT0_MIIRATE 4
#define CONFIG_VR9_SW_PORT1_MIIRATE 4
#define CONFIG_VR9_SW_PORT2_MIIRATE 4
#define CONFIG_VR9_SW_PORT4_MIIRATE 4
#define CONFIG_VR9_SW_PORT5a_MIIRATE 4
#define CONFIG_VR9_SW_PORT0_MIIRATE_AUTO 1
#define CONFIG_VR9_SW_PORT1_MIIRATE_AUTO 1
#define CONFIG_VR9_SW_PORT0_RGMII_MAC 1
#define CONFIG_VR9_SW_PORT1_RGMII_MAC 1
#define CONFIG_VR9_SW_PORT5a_RGMII 1
#define CONFIG_VR9_SW_PORT2_GMII 1
#define CONFIG_VR9_SW_PORT4_GMII 1
#define CONFIG_VR9_CRYSTAL_36M 1

/* lock cache for C program stack */
/* points to ROM */
/* stack size is 16K */
#define LOCK_DCACHE_ADDR       	0x9FC00000
#define LOCK_DCACHE_SIZE       	0x1000

/*dram param define*/
#define CONFIG_NR_DRAM_BANKS	1
//#define CONFIG_IFX_MEMORY_SIZE 	31
//#define IFX_CONFIG_MEMORY_SIZE 	CONFIG_IFX_MEMORY_SIZE
#define CONFIG_DRAM_BASE		0x80000000
#define CFG_SDRAM_UNCACHE_BASE	0xa0000000
#define CONFIG_DRAM_SIZE        32
#define CFG_SDRAM_SIZE 		( ( 1 << 20 ) * CONFIG_DRAM_SIZE )
#define CFG_CACHE_LOCK_SIZE  LOCK_DCACHE_SIZE
#define	CONFIG_TUNE_DDR			1
#define	SFDDR					1

//#define IFX_CFG_FLASH_DDR_CFG_START_ADDR      0x0000FFE8
//#define IFX_CFG_FLASH_DDR_CFG_SIZE            24 
//#define IFX_CFG_FLASH_DDR_CFG_END_ADDR        0x0000ffff

#define CFG_MEMTEST_START	0x80000000
#define CFG_MEMTEST_END		0xa0100000
#define CONFIG_LOADADDR		0x80800000
#define CFG_INIT_SP_OFFSET      0x600000
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
#define CFG_PROMPT		"[VRX288]# "		/* Monitor Command Prompt    */
#define CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define CFG_PBSIZE 		(CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS     16              /* max number of command args*/


/* boot args for kernel*/
#define CONFIG_CMDLINE_TAG	   1	     /* enable passing of ATAGs	 */

#ifdef SPI_FLASH
#define CONFIG_BOOTARGS         "init=/sbin/init console=ttyS0,115200  root=31:2 mtdparts=ifx_sflash:"
#else
#define CONFIG_BOOTARGS         "init=/sbin/init console=ttyS0,115200  root=31:2 mtdparts=physmap-flash.0:"
#endif

#define CFG_BOOTPARAMS_LEN	256*1024
#define	CONFIG_VR9_DDR2
#define	CONFIG_VR9_CPU_500M_RAM_250M	1
#define CPU_CLOCK_RATE		500000000	/* 500 MHz clock for the MIPS core */
#define RAM_CLOCK_RATE      250000000   /* 250 MHz clock for RAM           */
#define F_SSC_CLK     		CPU_CLOCK_RATE

#define CFG_HZ				(CPU_CLOCK_RATE / 2)
/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(20 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(20 * CFG_HZ) /* Timeout for Flash Write */
#define CFG_ENV_SIZE            0x20000

/* The following #defines are needed to get flash environment right */
#define CFG_MONITOR_BASE	CONFIG_TEXT_BASE

#define CFG_MONITOR_LEN		(192 << 10)

/*-----------------------------------------------------------------------
 * Cache settings
 */
#define CFG_CACHE_SIZE   16384
#define CFG_CACHE_LINES  32
#define CFG_CACHE_WAYS   4
#define CFG_CACHE_SETS   128

#define CFG_ICACHE_SIZE   CFG_CACHE_SIZE
#define CFG_DCACHE_SIZE   CFG_CACHE_SIZE
#define CFG_CACHELINE_SIZE  CFG_CACHE_LINES

/* Swith ports settings */

#define EXCEPTION_BASE  	0x200
#define CFG_RX_ETH_BUFFER 				32
#define	CONFIG_VR9_GPHY_FW_ADDR	0xb0010000
#define	CONFIG_VR9_GPHY_FW_LEN 0x5214
#define CONFIG_NETMASK		   255.255.255.0

#endif	/* __CONFIG_H */
