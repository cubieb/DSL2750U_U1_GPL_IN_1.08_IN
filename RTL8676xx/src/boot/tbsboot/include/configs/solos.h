#ifndef __CONFIG_H
#define __CONFIG_H

#include "../../blp_common/sdram_layout.h"

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define __LITTLE_ENDIAN            1234
#define __BYTE_ORDER            __LITTLE_ENDIAN
#define SOLOSW  //for kernel entry
#define FLASH_LAYOUT
/*
 * High Level Configuration Options
 * (easy to change)
 */

#define CONFIG_ARM926EJS	   1	     /* This is an arm926ejs CPU core  */
#define CONFIG_SOLOSW		   1	     /* in a solow_w core    */
#define CFI_FLASH 0

/*
 * Input clock of PLL
 * The OMAP730 Perseus 2 has 13MHz input clock
 */

#define CONFIG_SYS_CLK_FREQ	   13000000

#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG	   1	     /* enable passing of ATAGs	 */
/* #define CONFIG_SETUP_MEMORY_TAGS   1  */

#define CFG_GBL_DATA_SIZE	   128	     /* size in bytes reserved for initial data */

/*
 * Hardware drivers
 */

#define CONFIG_DRIVER_LAN91C96
#define CONFIG_LAN91C96_BASE	   0x04000300
#define CONFIG_LAN91C96_EXT_PHY
/*
 * Flash driver
 */
#define CFG_FLASH_CFI_DRIVER
#define CFG_FLASH_CFI

/*
 * NS16550 Configuration
 */

#define CFG_NS16550
#define CFG_NS16550_SERIAL
#define CFG_NS16550_REG_SIZE	   (1)
#define CFG_NS16550_CLK		   (48000000)	  /* can be 12M/32Khz or 48Mhz */
#define CFG_NS16550_COM1	   0xfffb0000	  /* uart1, bluetooth uart   * on perseus */

/*
 * select serial console configuration
 */

#define CONFIG_SERIAL1		   1	     /* we use SERIAL 1 on OMAP730 Perseus 2 */

#define CONFIG_CONS_INDEX	   1
#define CONFIG_BAUDRATE		   38400
#define CFG_BAUDRATE_TABLE	   { 9600, 19200, 38400, 57600, 115200 }

#define CONFIG_COMMANDS		   (CONFIG_CMD_DFL | CFG_CMD_DHCP)
#define CONFIG_BOOTP_MASK	   CONFIG_BOOTP_DEFAULT

/*
 * This must be included AFTER the definition of CONFIG_COMMANDS (if any)
 */

#include <cmd_confdefs.h>

#define CONFIG_BOOTDELAY	   1   /* autoboot after 1 seconds     */
#define CONFIG_BOOTARGS		   "console=ttyS0 root=31:2 mem=15M mtdparts=physmap-flash.0:"

#define CONFIG_LOADADDR		   ( CONFIG_DRAM_BASE + KERNEL_OFFSET )  /* Required */

#define CONFIG_SWITCH_RTL8306      1

#define CONFIG_ETHADDR
#define CONFIG_NETMASK		   255.255.255.0
#define CONFIG_IPADDR		   192.168.0.23
#define CONFIG_SERVERIP		   192.150.0.100
#define CONFIG_BOOTFILE		   "uImage"  /* File to load */

#if defined (CONFIG_COMMANDS) && defined (CFG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	   115200    /* Speed to run kgdb serial port */
#define CONFIG_KGDB_SER_INDEX	   1	     /* Which serial port to use */
#endif

/*
 * Miscellaneous configurable options
 */

#define CFG_LONGHELP				       /* undef to save memory	   */
#define CFG_PROMPT		   "SOLOSW # "     /* Monitor Command Prompt   */
#define CFG_CBSIZE		   256		       /* Console I/O Buffer Size  */
/* Print Buffer Size */
#define CFG_PBSIZE		   (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)
#define CFG_MAXARGS		   16		       /* max number of command args   */
#define CFG_BARGSIZE		   CFG_CBSIZE	       /* Boot Argument Buffer Size    */

#define CFG_MEMTEST_START	   0x10000000	       /* memtest works on */
#define CFG_MEMTEST_END		   0x12000000	       /* 32 MB in DRAM	   */

#undef CFG_CLKS_IN_HZ		     /* everything, incl board info, in Hz */

#define CFG_HZ			   0x800000

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */

#define CONFIG_STACKSIZE	   (128*1024)	  /* regular stack */

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */

#define CONFIG_NR_DRAM_BANKS	   1		  /* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		   0x0	  /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	   0x02000000	  /* 32 MB */
#define CONFIG_DRAM_BASE		   PHYS_SDRAM_1

#define PHYS_FLASH_1		   0x38000000

#define CONFIG_FLASH_BASE		   PHYS_FLASH_1
#define CFG_SDRAM_SIZE		0x1000000

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

#define CFG_MAX_FLASH_BANKS	   1		  /* max number of memory banks */
#define PHYS_FLASH_SIZE		   0x02000000	  /* 32MB */
#define CFG_MAX_FLASH_SECT	   (259)	  /* max number of sectors on one chip */
/* addr of environment */
#define CFG_ENV_ADDR		   (CONFIG_FLASH_BASE + 0x020000)

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	    (20*CFG_HZ)	  /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	    (20*CFG_HZ)	  /* Timeout for Flash Write */

#define CFG_ENV_IS_IN_FLASH	   1
#define CFG_ENV_SIZE		   0x20000	  /* Total Size of Environment Sector */
#define CFG_ENV_OFFSET		   0x20000	  /* environment starts here  */

#endif	  /* ! __CONFIG_H */
