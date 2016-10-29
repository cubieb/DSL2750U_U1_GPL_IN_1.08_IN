#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__


typedef struct 
{
    unsigned int device_id;
    unsigned short int ext_device_id;
}spi_flash_id;

#define RTL8672_SPI_PAGE_SIZE		256
#define RTL8672_FLASH_CHIP			0

/*
 * Macro Definition
 */
#define SPI_CS(i)           ((i) << 30)   /* 0: CS0 & CS1   1: CS0   2: CS1   3: NONE */
#define SPI_LENGTH(i)       ((i) << 28)   /* 0 ~ 3 */
#define SPI_READY(i)        ((i) << 27)   /* 0: Busy  1: Ready */

#define SPI_CLK_DIV(i)      ((i) << 29)   /* 0: DIV_2  1: DIV_4  2: DIV_6 ... 7: DIV_16 */
#define SPI_RD_ORDER(i)     ((i) << 28)   /* 0: Little-Endian  1: Big-Endian */
#define SPI_WR_ORDER(i)     ((i) << 27)   /* 0: Little-Endian  1: Big-Endian */
#define SPI_RD_MODE(i)      ((i) << 26)   /* 0: Fast-Mode  1: Normal Mode */
#define SPI_SFSIZE(i)       ((i) << 23)   /* 0 ~ 7. 128KB * (i+1) */
#define SPI_TCS(i)          ((i) << 19)   /* 0 ~ 15 */
#define SPI_RD_OPT(i)       ((i) << 18)   /* 0: No-Optimization  1: Optimized for Sequential Access */


#define LENGTH(i)       SPI_LENGTH(i)
#define CS(i)           SPI_CS(i)
#define RD_ORDER(i)     SPI_RD_ORDER(i)
#define WR_ORDER(i)     SPI_WR_ORDER(i)
#define READY(i)        SPI_READY(i)
#define CLK_DIV(i)      SPI_CLK_DIV(i)
#define RD_MODE(i)      SPI_RD_MODE(i)
#define SFSIZE(i)       SPI_SFSIZE(i)
#define TCS(i)          SPI_TCS(i)
#define RD_OPT(i)       SPI_RD_OPT(i)

//tylo, for ic ver. detect
#define SCCR	0xb8003200

#define IC8672 	0
#define IC8671B 	1
#define IC8671B_costdown 	2

static void spi_ready(void);
static void spi_pio_init(void);
static void checkICver(void);
static void read_id(spi_flash_id * flash_id);
static void spi_erase_sector(int sector);
static void spi_unprotect_block_other(unsigned int chip);
static void rtl8672_spi_write_page(unsigned int addr, unsigned char *data, int len);
static int flash_program_buf(void* addr, void* data, int len);

#endif

