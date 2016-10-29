/*****************************************************************************
 * DANUBE BootROM
 * Copyright (c) 2005, Infineon Technologies AG, All rights reserved
 * IFAP DC COM SD
 *****************************************************************************/

#ifndef __SFLASH_H__
#define __SFLASH_H__

/******************************************************************************/

/******************************************************************************/
/* Common declarations                                                        */
/******************************************************************************/

/* M25P64 Serial Flash Memory Instruction Set */
#define SFLASH_WREN	0x06	/* Write Enable */
#define SFLASH_WRDI	0x04	/* Write Disable */
#define SFLASH_RDID	0x9F
#define SFLASH_RDSR	0x05	/* Read Status Register */
#define SFLASH_WRSR	0x01	/* Write Status Register */
#define SFLASH_READ	0x03
#define DFLASH_READ	0xE8
#define SFLASH_FAST_READ	0x0B
#define SFLASH_PP		0x02	/* Page Program */
#define SFLASH_SE		0xD8	/* Sector Erase */
#define SFLASH_BE		0xC7	/* Bulk Erase */

/* Status Register bits */
#define SFLASH_SR_SRWD	1 << 7
#define SFLASH_SR_WEL	1 << 1
#define SFLASH_SR_WIP	1 << 0
#define SFLASH_SR_BP_MASK		7 << 2
#define SFLASH_SR_BP(value)	(7 & value) << 2
#define SFLASH_SR_BP_MAX		7

#define SFLASH_PP_TIMEOUT		(5 * CFG_HZ) >> 10	/* To divide by a 1000 -> shift 10 bits to the right */
#define SFLASH_WRSR_TIMEOUT	(15 * CFG_HZ) >> 10
#define SFLASH_SE_TIMEOUT		(3 * CFG_HZ)
#define SFLASH_BE_TIMEOUT		(160 * CFG_HZ)
#define SFLASH_BE_SUB_TIMEOUT	(40 * CFG_HZ)	/* 160 is too long for the get_timer u32 return value */

 /* Macros: Serial flash operation */
/**********************************/
#define SFLASH_WRITE_ENABLE		SFLASH_CHIPSELECT_0; \
				ssc_write(SFLASH_WREN); \
				SFLASH_CHIPSELECT_1;

#define SFLASH_WRITE_DISABLE	SFLASH_CHIPSELECT_0; \
				ssc_write(SFLASH_WRDI); \
				SFLASH_CHIPSELECT_1;

#define SFLASH_WRITE_ADDR(value)	ssc_write((value & 0xFF0000) >> 16); \
				ssc_write((value & 0x00FF00) >> 8); \
				ssc_write(value & 0x0000FF);

#define SFLASH_get_sect_number(value)	(value & 0xFF0000) >> 16


/******************************************************************************/


/******************************************************************************/
/* Function Declarations                                                      */
/******************************************************************************/

/* Return values for sflash functions */
#define SFLASH_OK		         0x00000000
#define SFLASH_ERR            0x80000000
#define SFLASH_TIMEOUT		   (SFLASH_ERR | 0x00000001)
 
int sflash_init(void);
int sflash_config(u32 *config);
//int sflash_read_block(u32 *addr, int length,int offset);
int sflash_read_block(u32 *addr, int length, u32 offset);
int sflash_terminate(void);

extern spi_dev_t spi_sflash;
#endif
