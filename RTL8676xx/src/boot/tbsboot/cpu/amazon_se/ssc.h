/*****************************************************************************
 * DANUBE BootROM
 * Copyright (c) 2005, Infineon Technologies AG, All rights reserved
 * IFAP DC COM SD
 *****************************************************************************/

#ifndef __SSC_H__
#define __SSC_H__

#define DATA_WIDTH	8

// in Danube FIFO size is 8
#define RXFIFO_SIZE             	4
#define TXFIFO_SIZE             	4

#define SPI_MAGIC			0xAA55
#define SPI_ADDR_CYC_MAX   0x0A
#define SSC_ERR            0x80000000
#define SSC_TIMEOUT		   (SSC_ERR | 0x00000001)

#ifdef ON_EMULATOR
#ifdef ON_VENUS
#define EEPROM_BAUDRATE	100000
#define SFLASH_BAUDRATE	300000
#else
/*ON_IKOS*/
#define EEPROM_BAUDRATE	10000
#define SFLASH_BAUDRATE	10000
#endif
#else
#define EEPROM_BAUDRATE	1000000
//#define SFLASH_BAUDRATE	15000000
#define SFLASH_BAUDRATE	30000000
#endif

/* SPI BUS configuration */
typedef struct 
{
	u32 baudrate;
} ssc_cfg_t;

/* non-volatile device attached to SPI BUS  */
typedef struct 
{
	u8 cs;
	u8 read_cmd;
	u8 addr_cycles;
} spi_dev_t;

/* SPI bus common routines */
int ssc_init(u32 baudrate);
int ssc_close(void);
int ssc_write(unsigned char uc_value);
int ssc_read_only(u8 *buf,u32 len);
int ssc_read(unsigned char *uc_value);
void ssc_tx_only(void);
void ssc_rx_only(void);
void ssc_tx_rx(void);
/* SPI based non-volatile storage device common routines */
//int spi_probe(spi_dev_t * dev);
int spi_probe(spi_dev_t * dev, u32);
int spi_read(spi_dev_t * dev,u8 *buf, u32 len,u32 offset);
void SSC1_CS_SET(u8 bit);
void SSC1_CS_CLEAR(u8 bit);
void ssc_sync(void);
#endif 
