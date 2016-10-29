#ifndef _MTD_SPI_PROBE_H_
#define _MTD_SPI_PROBE_H_
#include <linux/kernel.h>

#ifdef _LINUX_KERNEL_H
#include <linux/types.h>
#include <linux/mtd/map.h>
#include <linux/mtd/gen_probe.h>

struct spi_chip_info
{
    struct spi_flash_params *flpara;
    void (*destroy)(struct spi_chip_info *chip_info);
    int (*read)(unsigned int from, unsigned int to, size_t size);
    int (*write)(unsigned int from, unsigned int to, size_t size);
    int (*erase)(unsigned int addr);
};

/* Function Prototypes */

void spi_read(unsigned int chip, unsigned int address, unsigned int *data_out);
//void spi_write(unsigned int chip, unsigned int address, unsigned int data_in);

void spi_cp_probe(void);
void spi_burn_image(unsigned int chip, unsigned char *image_addr, unsigned int image_size);
struct spi_chip_info *spi_probe_flash_chip(struct map_info *map, struct chip_probe *cp);

#endif

#define ID_MASK         0xffff
#define SPANSION 0x01
#define SPI          0x02
#define SIZE_64KiB      0x10000
#define SIZE_2MiB       0x200000

struct spi_flash_params
{
    char *name;
    unsigned long mtc_id;     //mtcid
    unsigned long MB_size;    //MB
    unsigned long KB_sec;     //KB
    unsigned long n_secs;     //m
    unsigned long B_pgsize;   //B
};


#endif /* _MTD_SPI_PROBE_H_ */
#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

/* MX25LXX05D-specific commands */
#define CMD_MX25L_WREN     0x06
#define CMD_MX25L_WRDI     0x04
#define CMD_MX25L_WRSR     0x01 /* Write Status Register */
#define CMD_MX25L_RDID     0x9F
#define CMD_MX25L_RDSR     0x05 /* Read Status Register */

#define CMD_MX25L_READ     0x03
#define CMD_MX25L_FASTREAD 0x0B
#define CMD_MX25L_RES      0xAB
#define CMD_MX25L_REMS     0x90
#define CMD_MX25L_DREAD    0x3B

#define CMD_MX25L_SE       0x20 
#define CMD_MX25L_BE       0x52 /* or 0xd8 */
#define CMD_MX25L_CE       0x60 /* or 0xc7 */
#define CMD_MX25L_PP       0x02 /* Page Programming */
#define CMD_MX25L_RDSCUR   0x2B
#define CMD_MX25L_WRSCUR   0x2F
#define CMD_MX25L_ENSO     0xB1
#define CMD_MX25L_EXSO     0xC1
#define CMD_MX25L_DP       0xB9
#define CMD_MX25L_RDP      0xAB

#define CMD_MX25L_REMS2    0xEF
#define CMD_MX25L_CP       0xAD
#define CMD_MX25L_ESRY     0x70
#define CMD_MX25L_DSRY     0x80

#define MX25L_SR_WIP            (1 << 0)        /* Write-in-Progress */
#define MX25L_SR_WEL            (1 << 1)        /* Write-enable-latch */
#define MX25L_SR_SRWD           (1 << 7)        /* Status-register-write-disable */

#define SPI_CS(i)           ((i) << 30)   /* 0: CS0 & CS1   1: CS0   2: CS1   3: NONE */
#define SPI_LENGTH(i)       ((i) << 28)   /* 0 ~ 3 */
#define SPI_READY(i)        ((i) << 27)   /* 0: Busy  1: Ready */
#define SPI_CLK_DIV(i)      ((i) << 29)   /* 0: DIV_2  1: DIV_4  2: DIV_6 ... 7: DIV_16 */
#define SPI_RD_ORDER(i)     ((i) << 28)   /* 0: Little-Endian  1: Big-Endian */
#define SPI_WR_ORDER(i)     ((i) << 27)   /* 0: Little-Endian  1: Big-Endian */
#define SPI_RD_MODE(i)      ((i) << 26)   /* 0: Fast-Mode  1: Normal Mode */
#define SPI_SFSIZE(i)       ((i) << 23)   /* 0 ~ 7. 128KB * (i+1) */
#define SPI_RD_OPT(i)       ((i) << 20)   /* 0: No-Optimization  1: Optimized for Sequential Access */
#define SPI_TCS(i)          ((i) << 22)   /* Bit 26:22, SPI chip deselect time. 0 ~ 31 */

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

#endif /* _SPI_FLASH_H_ */
