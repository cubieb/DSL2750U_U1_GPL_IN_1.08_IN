#ifndef __SDRAM_LAYOUT_H__
#define __SDRAM_LAYOUT_H__

/*********************************************************
**********************************************************/

#if defined(CONFIG_C1000)
#define BOOTARGS_OFFSET         (0x800000+0x100)         /* bootargs for Linux kernel */
#else
#define BOOTARGS_OFFSET         (0x1000)                 /* bootargs for Linux kernel */
#endif

#if defined(SOLOSW)
#define KERNEL_OFFSET           0x18000
#elif defined(ATHEROS)
#define KERNEL_OFFSET           0x60000
#elif defined(AR2317)
#define KERNEL_OFFSET           0x60000
#elif defined(CONFIG_VX160)
#define KERNEL_OFFSET           0x10000
#elif defined(CONFIG_VX180)
#define KERNEL_OFFSET           0x10000
#elif defined(CONFIG_RT3052)
#define KERNEL_OFFSET           0x10000
#elif defined(CONFIG_C1000)
#define KERNEL_OFFSET           0x808000
#elif defined(CONFIG_RTL8672)
#define KERNEL_OFFSET           0x10000
#elif defined(CONFIG_RTL8672VE)
#define KERNEL_OFFSET           0x10000
#else 
#define KERNEL_OFFSET           0x10000
#endif
#define KERNEL_TMP_OFFSET       0x600000                     /*  For spiflash vmlinux.bin.gz sdram unlzma */

/*********************************************************
**********************************************************/
#define BOOT_RUNTIME_SIZE       0x400000
#define BOOT_STAGE2_OFFSET      0                            /* 1024KB */
#define BOOT_STAGE2_SIZE        0x100000

#define ITEM_MAP_OFFSET         0x100000                     /* 64KB */
#define ITEM_MAP_SIZE           0x10000

#define SYSTEMCFG_OFFSET        0x110000                     /* 16KB */
#define SYSTEMCFG_SIZE          0x4000

#define ITEM_CACHE_OFFSET       0x114000                     /* 64KB */
#define ITEM_CACHE_SIZE         0x10000

#define GLOBAL_DATA_OFFSET      0x124000                     /* 16KB */
#define GLOBAL_DATA_SIZE        0x4000

#define HEAP_OFFSET             0x128000                     /* 1024KB */
#define HEAP_SIZE               0x100000

/*********************************************************
**********************************************************/
#define STACK_OFFSET            0x80000                         /* 1024KB */
#define STACK_SIZE              0x100000

#endif

