

#ifndef __CONFIG_H
#define __CONFIG_H

#include "sdram_layout.h"
#include "../../../../autoconf.h"

#define SUPPORT_8676_PLATFORM	1
#define	CPU_LX4181  			1//for FPGA
#define	SDRAM_AUTO_DETECT		1
#define	PCIe_module				1
#define Flash_AA21_GPA5  		1 // GPIO interface as PCI
#define __BIG_ENDIAN 		4321
#define __BYTE_ORDER    	__BIG_ENDIAN
//#define RTL8676  //for kernel entry

#ifdef CONFIG_RTL8676_SPI_FLASH
#define SPI_FLASH 		0
#define BOARD_PARAM_FLASHSIZE   0xfffe8000 //2M  0xfffc8000 //4M 0xfffe8000      //16M
#else
#define CFI_FLASH               1
#define CFG_FLASH_CFI_DRIVER_RTL8672 1
#define CFG_FLASH_CFI           1
#endif

#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(512)	/* max number of sectors on one chip */
#define CFG_FLASH_16BIT         1
#define CFG_FLASH_LAYOUT /*完成flash_layout后定义*/

#define CONFIG_NR_DRAM_BANKS	1

#define CFG_MEMTEST_START	0x80000000
#define CFG_MEMTEST_END		0xa0800000

#define CONFIG_LOADADDR  (CONFIG_DRAM_BASE + KERNEL_OFFSET)

/* System Bus Clock configuration, undefined SYS_CLOCK_175MHZ is set to 200MHz */
#define SYS_CLOCK_175MHZ
/* SDRAM Clock configuration, undefined 116Mhz and 166Mhz is set to 133Mhz */
#define CONFIG_RAM_166
#define	LX_210_DDR166	1
#define	SDRAM_CLOCK		166

#define MCR0_VAL	0x54480000
#define MCR1_VAL	0x1818FFFF
#define MCR2_VAL	0x00000CC9


/* System Clock */
#ifdef CONFIG_RLE0412
#define BSP_SCCR	0xB8000200
#else //CONFIG_RLE0412
#define BSP_SCCR	0xB8003200
#define BSP_PLL_CTRL	(BSP_SCCR + 0x04)
#define BSP_PLL2_CTRL	(BSP_SCCR + 0x08)
#define BSP_ANA1_CTRL	(BSP_SCCR + 0x14)
#define BSP_PLL3_CTRL	(BSP_SCCR + 0x18)
#define BSP_LDO_CTRL	(BSP_SCCR + 0x20)
#endif //CONFIG_RLE0412

#ifdef CONFIG_RLE0412
#define BSP_PCI_MISC				0xB8000400
#else
#define BSP_PCI_MISC				0xB8003400
#endif

/* Switch Core*/
#define SWTBL_BASE                          0xBB000000
#define SWCORE_BASE                         0xBB800000
#define HSB_BASE                            0xBB806280
#define HSA_BASE                            0xBB806200
#define SYSTEM_BASE	                        0xB8000000

/* Memory Controller */
#define BSP_MCR                             (SYSTEM_BASE+0x1000)      /* 0xB8001000 Memory Control Register */
#define BSP_DCR                             (BSP_MCR+0x04)      /* DRAM Configuration Register */

/* Global interrupt control registers */
#define GICR_BASE                           (SYSTEM_BASE+0x3000)      /* 0xB8003000 */
#define GIMR                                (0x000 + GICR_BASE)       /* Global interrupt mask */
#define GISR                                (0x004 + GICR_BASE)       /* Global interrupt status */
#define IRR                                 (0x008 + GICR_BASE)       /* Interrupt routing */
#define IRR0                                (0x008 + GICR_BASE)       /* Interrupt routing */
#define IRR1                                (0x00C + GICR_BASE)       /* Interrupt routing */
#define IRR2                                (0x010 + GICR_BASE)       /* Interrupt routing */
#define IRR3                                (0x014 + GICR_BASE)       /* Interrupt routing */
/* Timer control registers 
*/
#define TIMER_BASE                          (SYSTEM_BASE+0x3100)       /* 0xB8003100 */
#define TC0DATA                             (0x000 + TIMER_BASE)       /* Timer/Counter 0 data */
#define TC1DATA                             (0x004 + TIMER_BASE)       /* Timer/Counter 1 data */
#define TC0CNT                              (0x008 + TIMER_BASE)       /* Timer/Counter 0 count */
#define TC1CNT                              (0x00C + TIMER_BASE)       /* Timer/Counter 1 count */
#define TCCNR                               (0x010 + TIMER_BASE)       /* Timer/Counter control */
#define TCIR                                (0x014 + TIMER_BASE)       /* Timer/Counter intertupt */
#define CDBR                                (0x018 + TIMER_BASE)       /* Clock division base */
#define WDTCNR                              (0x01C + TIMER_BASE)       /* Watchdog timer control */



/* System interface */
/*internal phy*/
//#define SICR_VAL	0xB7053A19    //for external phy & PCI
#define SICR_VAL	0xB7053819    //for internal phy & PCI

/* Lexra Bus Arbitor */
#define CPUC_VAL	0x0C0100FF    //for device bus weighting
#define SCCR					0xB8003200		/* System Clock Control Register */

#define NAND_CTRL_BASE  0xB801A000
#define Boot_Select		0xB8000304

#define CONFIG_CONS_INDEX	1
#define CONFIG_BOOTDELAY	3	/* autoboot after 3 seconds	*/
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
#define CFG_PROMPT		"RTL8676# "		/* Monitor Command Prompt    */
#define CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS             80              /* max number of command args*/


/* boot args for kernel*/
#define CONFIG_CMDLINE_TAG	   1	     /* enable passing of ATAGs	 */

#ifdef CONFIG_KERNEL_INITCALL_DEBUG
	#ifdef SPI_FLASH
	#define CONFIG_BOOTARGS		"console=ttyS0,%d mem=%dM initcall_debug=1 root=31:2 mtdparts=rtl8676_spiflash:"
	#else
	#define CONFIG_BOOTARGS		"console=ttyS0,%d mem=%dM initcall_debug=1 root=31:2 mtdparts=physmap-flash.0:"
	#endif
#else
	#ifdef SPI_FLASH
	#define CONFIG_BOOTARGS		"console=ttyS0,%d mem=%dM root=31:2 mtdparts=rtl8676_spiflash:"
	#else
	#define CONFIG_BOOTARGS		"console=ttyS0,%d mem=%dM root=31:2 mtdparts=physmap-flash.0:"
	#endif
#endif

#define CFG_BOOTPARAMS_LEN	128*1024

#define CFG_HZ	1000 //CLOCKGEN_FREQ  

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(20 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */
#define CFG_ENV_SIZE            0x20000
/* The following #defines are needed to get flash environment right */
#define CFG_MONITOR_BASE	CONFIG_TEXT_BASE
#define CFG_MONITOR_LEN		(192 << 10)

#endif	/* __CONFIG_H */

