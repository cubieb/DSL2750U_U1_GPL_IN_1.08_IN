/*****************************************************************************
 * DANUBE BootROM
 * Copyright (c) 2005, Infineon Technologies AG, All rights reserved
 * COM NRW CSE VoIP
 *****************************************************************************/

#ifndef __LIB_H
#define __LIB_H

/******************************************************************************
 * Global types
 ******************************************************************************/
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed   long s32;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef volatile unsigned short vuint;

/* for simulation codes */
#define REG32(addr) (*((volatile u32*)(addr)))

/******************************************************************************
 * Global macros
 ******************************************************************************/
#define WRITEU32(add, value) *((u32*)addr) = value;
/* #define U8TOU16(addr) (*(addr) << 8 | *((addr)+1)) */
/* #define U8TOU32(addr) (*((addr)+3) << 24 | *((addr)+2) << 16 | *((addr)+1) << 8 | *((addr)+0)) */
/* #define U8TOU32(addr) (*((addr)+0) << 24 | *((addr)+1) << 16 | *((addr)+2) << 8 | *((addr)+3)) */
#define U8TOU32(addr) (*((u32 *)(addr)))

/******************************************************************************
 * Global functions
 ******************************************************************************/
u8* memcpy(u8* dest, const u8* source, int count);
u8* icachecpy(u8* dest, const u8* source, int count);
void cache_wrt(void); 
u32 get_timer(u32 base);
u32 danube_get_fpiclk (void);
u32 danube_get_cpuclk (void);
void htoa(u32 hex, u8 *buf);
void decrypt(u32 addr, int n);
void print_u32(u32 data);
void print_u8(u8 data);
#ifdef USE_DDR_SDRAM
#warning "DDR is included"
void config_ddr(void);
#endif
#if 0
void jump_start(u32 addr);
#endif

#endif /* __LIB_H */

