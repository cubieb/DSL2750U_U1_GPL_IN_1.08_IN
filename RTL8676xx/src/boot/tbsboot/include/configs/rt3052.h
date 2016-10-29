

#ifndef __CONFIG_H
#define __CONFIG_H

#include "sdram_layout.h"

#define __LITTLE_ENDIAN 		1234
#define __BYTE_ORDER    	__LITTLE_ENDIAN
#define RT3052  //for kernel entry

#define CONFIG_MIPS32		1	/* MIPS 4Kc CPU core	*/
#define CONFIG_RT3052	1

#define CFI_FLASH 		1
#define CFG_FLASH_CFI_DRIVER_RT3052	1
#define CFG_FLASH_CFI		1

#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(256)	/* max number of sectors on one chip */
#define CONFIG_FLASH_BASE		0xbf000000 //PHYS_FLASH_1
//#define CFG_FLASH_16BIT         1
#define CFG_FLASH_LAYOUT /*完成flash_layout后定义*/



/*dram param define*/
#define CONFIG_NR_DRAM_BANKS	1
//#define CONFIG_DRAM_BASE		0x80000000
#define CONFIG_DRAM_BASE		0x80000000
#define CFG_SDRAM_SIZE 		0x2000000
#define CFG_UBOOT_LEN 		(0x40000) //bootloader+bootloader_ex+sp
#define CFG_MEMTEST_START	0x80000000
#define CFG_MEMTEST_END		0xa0800000

#define CONFIG_LOADADDR  ( CONFIG_DRAM_BASE + KERNEL_OFFSET )

/* cpu rate */
#define PALMCHIP_CLOCK_DIVISOR 16
#define FPGA_BOARD_CLOCK_DIVISOR 32

#define CPU_CLOCK_RATE	384000000	/* 384 MHz clock for the MIPS core */
#define CPU_TCLOCK_RATE 	16588800	/* 16.5888 MHz for TClock */

/* serial rate */
#define CONFIG_CONS_INDEX	1
#define CONFIG_BOOTDELAY	1	/* autoboot after 1 seconds	*/

#define CONFIG_BAUDRATE		115200	

#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

#define CONFIG_TIMESTAMP		/* Print image info with timestamp */


#include <cmd_confdefs.h>
/*
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP				/* undef to save memory	     */
#define CFG_PROMPT		"RT3052# "		/* Monitor Command Prompt    */
#define CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS             16              /* max number of command args*/


/* boot args for kernel*/
#define CONFIG_CMDLINE_TAG	   1	     /* enable passing of ATAGs	 */
#define CONFIG_BOOTARGS		"console=ttyS1,115200 root=31:2 mtdparts=physmap-flash.0:"


#define CFG_BOOTPARAMS_LEN	128*1024

#define CFG_HZ			CPU_CLOCK_RATE/2

/* timeout values are in ticks */
//#define CFG_FLASH_ERASE_TOUT	(20 * CFG_HZ) /* Timeout for Flash Erase */
//#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(15UL * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(5 * CFG_HZ) /* Timeout for Flash Write */
#define CFG_ETH_AN_TOUT	(5 * CFG_HZ) /* Timeout for Flash Write */
#define CFG_ETH_LINK_UP_TOUT	(5 * CFG_HZ) /* Timeout for Flash Write */
#define CFG_FLASH_STATE_DISPLAY_TOUT  (2 * CFG_HZ) /* Timeout for Flash Write */



#define CFG_ENV_SIZE            0x20000

#define CFG_MONITOR_BASE	TEXT_BASE


#define CFG_MONITOR_LEN		(192 << 10)

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CFG_DCACHE_SIZE		(16*1024)
#define CFG_ICACHE_SIZE		(16*1024)
#define CFG_CACHELINE_SIZE	16

/* net */
#define CONFIG_NET_MULTI
#define CFG_RX_ETH_BUFFER		60
#define CONFIG_RT2880_ETH		1	/* Enable built-in 10/100 Ethernet */

/*
 * System Controller	(0x00300000)
 *   Offset
 *   0x10  -- SYSCFG		System Configuration Register
 *   0x34  -- RSTCTRL		Reset Control Register
 *   0x38  -- RSTSTAT		Reset Status Register 
 *   0x60  -- GPIOMODE		GPIO Mode Control Register 
 */
#define RT2880_SYS_CNTL_BASE			(RALINK_SYSCTL_BASE)
#define RT2880_SYSCFG_REG			(RT2880_SYS_CNTL_BASE+0x10)
#define RT2880_RSTCTRL_REG			(RT2880_SYS_CNTL_BASE+0x34)
#define RT2880_RSTSTAT_REG			(RT2880_SYS_CNTL_BASE+0x38)
#define RT2880_GPIOMODE_REG			(RT2880_SYS_CNTL_BASE+0x60)

#define RT2880_PRGIO_ADDR       (RALINK_SYSCTL_BASE + 0x600) // Programmable I/O
#define RT2880_REG_PIOINT       (RT2880_PRGIO_ADDR + 0)
#define RT2880_REG_PIOEDGE      (RT2880_PRGIO_ADDR + 0x04)
#define RT2880_REG_PIORENA      (RT2880_PRGIO_ADDR + 0x08)
#define RT2880_REG_PIOFENA      (RT2880_PRGIO_ADDR + 0x0C)
#define RT2880_REG_PIODATA      (RT2880_PRGIO_ADDR + 0x20)
#define RT2880_REG_PIODIR       (RT2880_PRGIO_ADDR + 0x24)

#define RT2882_REG(x)		(*((volatile u32 *)(x)))	

#define RT2880_WDRST            (1<<1)
#define RT2880_SWSYSRST         (1<<2)
#define RT2880_SWCPURST         (1<<3)

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
