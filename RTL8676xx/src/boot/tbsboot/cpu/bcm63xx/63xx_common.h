
#ifndef __BCM63xx_MAP_COMMON_H
#define __BCM63xx_MAP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/*

*/
#if CONFIG_BCM6338 

#define PERF_BASE     0xfffe0000    /* chip control registers */
#define BB_BASE       0xfffe0100    /* bus bridge registers */
#define TIMR_BASE     0xfffe0200    /* timer registers */
#define UART_BASE     0xfffe0300    /* uart registers */
#define GPIO_BASE     0xfffe0400    /* gpio registers */
#define SPI_BASE      0xfffe0c00    /* SPI master controller registers */

#define ADSL_BASE     0xfffe1000	/* ADSL core control registers */
#define ATM_BASE      0xfffe2000	/* ATM SAR control registers */
#define EMAC_DMA_BASE 0xfffe2400    /* EMAC DMA control registers */
#define USB_DMA_BASE  0xfffe2400    /* USB DMA control registers */
#define EMAC1_BASE    0xfffe2800    /* EMAC1 control registers */
#define USB_CTL_BASE  0xfffe3000    /* USB control registers */
#define SDRAM_BASE    0xfffe3100    /* SDRAM control registers */
#define MPI_BASE      0xfffe3160    /* EBI control registers */


/*
#####################################################################
# System PLL Control Register
#####################################################################
*/

#define SOFT_RESET	0x00000001

/*
#####################################################################
# SDRAM Control Registers
#####################################################################
*/
#define SDR_INIT_CTL        0x00
    /* Control Bits */
#define SDR_PFEN1           (1<<16)
#define SDR_PFEN0           (1<<15)
#define SDR_EMPRS           (1<<14)
#define SDR_2_BANKS         (1<<13)
#define SDR_1_BANK          (0<<13)
#define SDR_CS1_EN          (1<<12)
#define SDR_PEND            (1<<11)
#define SDR_32_BIT          (1<<10)
#define SDR_POWER_DOWN      (1<<9)
#define SDR_SELF_REFRESH    (1<<8)
#define SDR_11_COLS         (3<<6)
#define SDR_10_COLS         (2<<6)
#define SDR_9_COLS          (1<<6)
#define SDR_8_COLS          (0<<6)
#define SDR_13_ROWS         (2<<4)
#define SDR_12_ROWS         (1<<4)
#define SDR_11_ROWS         (0<<4)
#define SDR_MASTER_EN       (1<<3)
#define SDR_MRS_CMD         (1<<2)
#define SDR_PRE_CMD         (1<<1)
#define SDR_CBR_CMD         (1<<0)

#define SDR_CFG_REG         0x04
    /* Control Bits */
#define SDR_FULL_PG         0
#define SDR_BURST8          1
#define SDR_BURST4          2
#define SDR_BURST2          3
#define SDR_FAST_MEM        (1<<2)
#define SDR_SLOW_MEM        (0<<2)

#define SDR_REF_CTL         0x0C
    /* Control Bits */
#define SDR_REF_EN          (1<<15)

#define SDR_PRIOR           0x2C
    /* Control Bits */
#define SDR_EN_PRIOR        (1<<31)
#define SDR_ADSL_HPE        (1<<30)
#define SDR_MIPS_HPE        (1<<29)


/*
#####################################################################
# MPI Control Registers
#####################################################################
*/
#define CS0BASE         0x00
#define CS0CNTL         0x04

/*
# CSxBASE settings
#   Size in low 4 bits
#   Base Address for match in upper 24 bits
*/
#define EBI_SIZE_8K         0
#define EBI_SIZE_16K        1
#define EBI_SIZE_32K        2
#define EBI_SIZE_64K        3
#define EBI_SIZE_128K       4
#define EBI_SIZE_256K       5
#define EBI_SIZE_512K       6
#define EBI_SIZE_1M         7
#define EBI_SIZE_2M         8
#define EBI_SIZE_4M         9
#define EBI_SIZE_8M         10
#define EBI_SIZE_16M        11
#define EBI_SIZE_32M        12
#define EBI_SIZE_64M        13
#define EBI_SIZE_128M       14
#define EBI_SIZE_256M       15

/* CSxCNTL settings */
#define EBI_ENABLE          0x00000001  /* .. enable this range */
#define EBI_WAIT_STATES     0x0000000e  /* .. mask for wait states */
#define ZEROWT              0x00000000  /* ..  0 WS */
#define ONEWT               0x00000002  /* ..  1 WS */
#define TWOWT               0x00000004  /* ..  2 WS */
#define THREEWT             0x00000006  /* ..  3 WS */
#define FOURWT              0x00000008  /* ..  4 WS */
#define FIVEWT              0x0000000a  /* ..  5 WS */
#define SIXWT               0x0000000c  /* ..  6 WS */
#define SEVENWT             0x0000000e  /* ..  7 WS */
#define EBI_WORD_WIDE       0x00000010  /* .. 16-bit peripheral, else 8 */
#define EBI_POLARITY        0x00000040  /* .. set to invert chip select polarity */
#define EBI_TS_TA_MODE      0x00000080  /* .. use TS/TA mode */
#define EBI_TS_SEL          0x00000100  /* .. drive tsize, not bs_b */
#define EBI_FIFO            0x00000200  /* .. enable fifo */
#define EBI_RE              0x00000400  /* .. Reverse Endian */

/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define UART0CONTROL     0x01
#define UART0CONFIG      0x02
#define UART0RXTIMEOUT   0x03
#define UART0BAUD        0x04
#define UART0FIFOCFG     0x0a
#define UART0INTMASK     0x10
#define UART0INTSTAT     0x12
#define UART0DATA        0x17

#define BRGEN            0x80   /* Control register bit defs */
#define TXEN             0x40
#define RXEN             0x20
#define LOOPBK           0x10
#define TXPARITYEN       0x08
#define TXPARITYEVEN     0x04
#define RXPARITYEN       0x02
#define RXPARITYEVEN     0x01

#define XMITBREAK        0x40   /* Config register */
#define BITS5SYM         0x00
#define BITS6SYM         0x10
#define BITS7SYM         0x20
#define BITS8SYM         0x30
#define ONESTOP          0x07
#define TWOSTOP          0x0f

#define RSTTXFIFOS       0x80   /* Rx Timeout register */
#define RSTRXFIFOS       0x40

#define TX4              0x40   /* FIFO config register */
#define RX4              0x04

#define DELTAIP          0x0001 /* Interrupt Status and Mask registers */
#define TXUNDERR         0x0002
#define TXOVFERR         0x0004
#define TXFIFOTHOLD      0x0008
#define TXREADLATCH      0x0010
#define TXFIFOEMT        0x0020
#define RXUNDERR         0x0040
#define RXOVFERR         0x0080
#define RXTIMEOUT        0x0100
#define RXFIFOFULL       0x0200
#define RXFIFOTHOLD      0x0400
#define RXFIFONE         0x0800
#define RXFRAMERR        0x1000
#define RXPARERR         0x2000
#define RXBRK            0x4000
          

#endif

/*

*/
#if CONFIG_BCM6348

#define PERF_BASE     0xfffe0000    /* chip control registers */
#define BB_BASE       0xfffe0100    /* bus bridge registers */
#define TIMR_BASE     0xfffe0200    /* timer registers */
#define UART_BASE     0xfffe0300    /* uart registers */
#define GPIO_BASE     0xfffe0400    /* gpio registers */
#define SPI_BASE      0xfffe0c00    /* SPI master controller registers */

#define USB_CTL_BASE  0xfffe1000    /* USB control registers */
#define USB_DMA_BASE  0xfffe1400    /* USB DMA control registers */
#define USB_OHCI_BASE 0xfffe1b00    /* USB host registers */
#define USBH_CFG_BASE 0xfffe1c00    /* USB host non-OHCI registers */

#define MPI_BASE      0xfffe2000    /* MPI control registers */
#define SDRAM_BASE    0xfffe2300    /* SDRAM control registers */

#define ADSL_BASE     0xfffe3000	/* ADSL core control registers */
#define ATM_BASE      0xfffe4000	/* ATM SAR control registers */
#define UBUS_BASE     0xfffe5000	/* UBUS status registers */
#define EMAC1_BASE    0xfffe6000    /* EMAC1 control registers */
#define EMAC2_BASE    0xfffe6800    /* EMAC2 control registers */
#define EMAC_DMA_BASE 0xfffe7000    /* EMAC DMA control registers */

/*
#####################################################################
# System PLL Control Register
#####################################################################
*/
#define SYSPLLCFG       0x08

#define M_MPI_MASK      0x00000018      // 4:3
#define M_MPI_SHFT      3
#define M_MPI_50MHZ     0
#define M_MPI_40MHZ     1
#define M_MPI_33MHZ     2
#define M_MPI_25MHZ     3

#define M_UTO_MASK      0x00000002      // 1:1
#define M_UTO_SHFT      1

#define SOFT_RESET	0x00000001

#define PLL_STRAP_VALUE  0x34

/*
#####################################################################
# SDRAM Control Registers
#####################################################################
*/
#define SDR_INIT_CTL        0x00
    /* Control Bits */
#define SDR_PFEN1           (1<<16)
#define SDR_PFEN0           (1<<15)
#define SDR_EMPRS           (1<<14)
#define SDR_2_BANKS         (1<<13)
#define SDR_1_BANK          (0<<13)
#define SDR_CS1_EN          (1<<12)
#define SDR_PEND            (1<<11)
#define SDR_32_BIT          (1<<10)
#define SDR_POWER_DOWN      (1<<9)
#define SDR_SELF_REFRESH    (1<<8)
#define SDR_11_COLS         (3<<6)
#define SDR_10_COLS         (2<<6)
#define SDR_9_COLS          (1<<6)
#define SDR_8_COLS          (0<<6)
#define SDR_13_ROWS         (2<<4)
#define SDR_12_ROWS         (1<<4)
#define SDR_11_ROWS         (0<<4)
#define SDR_MASTER_EN       (1<<3)
#define SDR_MRS_CMD         (1<<2)
#define SDR_PRE_CMD         (1<<1)
#define SDR_CBR_CMD         (1<<0)

#define SDR_CFG_REG         0x04
    /* Control Bits */
#define SDR_FULL_PG         0
#define SDR_BURST8          1
#define SDR_BURST4          2
#define SDR_BURST2          3
#define SDR_FAST_MEM        (1<<2)
#define SDR_SLOW_MEM        (0<<2)

#define SDR_REF_CTL         0x0C
    /* Control Bits */
#define SDR_REF_EN          (1<<15)

#define SDR_PRIOR           0x2C
    /* Control Bits */
#define SDR_EN_PRIOR        (1<<31)
#define SDR_ADSL_HPE        (1<<30)
#define SDR_MIPS_HPE        (1<<29)


/*
#####################################################################
# MPI Control Registers
#####################################################################
*/
#define CS0BASE         0x00
#define CS0CNTL         0x04

/*
# CSxBASE settings
#   Size in low 4 bits
#   Base Address for match in upper 24 bits
*/
#define EBI_SIZE_8K         0
#define EBI_SIZE_16K        1
#define EBI_SIZE_32K        2
#define EBI_SIZE_64K        3
#define EBI_SIZE_128K       4
#define EBI_SIZE_256K       5
#define EBI_SIZE_512K       6
#define EBI_SIZE_1M         7
#define EBI_SIZE_2M         8
#define EBI_SIZE_4M         9
#define EBI_SIZE_8M         10
#define EBI_SIZE_16M        11
#define EBI_SIZE_32M        12
#define EBI_SIZE_64M        13
#define EBI_SIZE_128M       14
#define EBI_SIZE_256M       15

/* CSxCNTL settings */
#define EBI_ENABLE          0x00000001  /* .. enable this range */
#define EBI_WAIT_STATES     0x0000000e  /* .. mask for wait states */
#define ZEROWT              0x00000000  /* ..  0 WS */
#define ONEWT               0x00000002  /* ..  1 WS */
#define TWOWT               0x00000004  /* ..  2 WS */
#define THREEWT             0x00000006  /* ..  3 WS */
#define FOURWT              0x00000008  /* ..  4 WS */
#define FIVEWT              0x0000000a  /* ..  5 WS */
#define SIXWT               0x0000000c  /* ..  6 WS */
#define SEVENWT             0x0000000e  /* ..  7 WS */
#define EBI_WORD_WIDE       0x00000010  /* .. 16-bit peripheral, else 8 */
#define EBI_POLARITY        0x00000040  /* .. set to invert chip select polarity */
#define EBI_TS_TA_MODE      0x00000080  /* .. use TS/TA mode */
#define EBI_TS_SEL          0x00000100  /* .. drive tsize, not bs_b */
#define EBI_FIFO            0x00000200  /* .. enable fifo */
#define EBI_RE              0x00000400  /* .. Reverse Endian */

/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define UART0CONTROL     0x01
#define UART0CONFIG      0x02
#define UART0RXTIMEOUT   0x03
#define UART0BAUD        0x04
#define UART0FIFOCFG     0x0a
#define UART0INTMASK     0x10
#define UART0INTSTAT     0x12
#define UART0DATA        0x17

#define BRGEN            0x80   /* Control register bit defs */
#define TXEN             0x40
#define RXEN             0x20
#define LOOPBK           0x10
#define TXPARITYEN       0x08
#define TXPARITYEVEN     0x04
#define RXPARITYEN       0x02
#define RXPARITYEVEN     0x01

#define XMITBREAK        0x40   /* Config register */
#define BITS5SYM         0x00
#define BITS6SYM         0x10
#define BITS7SYM         0x20
#define BITS8SYM         0x30
#define ONESTOP          0x07
#define TWOSTOP          0x0f

#define RSTTXFIFOS       0x80   /* Rx Timeout register */
#define RSTRXFIFOS       0x40

#define TX4              0x40   /* FIFO config register */
#define RX4              0x04

#define DELTAIP          0x0001 /* Interrupt Status and Mask registers */
#define TXUNDERR         0x0002
#define TXOVFERR         0x0004
#define TXFIFOTHOLD      0x0008
#define TXREADLATCH      0x0010
#define TXFIFOEMT        0x0020
#define RXUNDERR         0x0040
#define RXOVFERR         0x0080
#define RXTIMEOUT        0x0100
#define RXFIFOFULL       0x0200
#define RXFIFOTHOLD      0x0400
#define RXFIFONE         0x0800
#define RXFRAMERR        0x1000
#define RXPARERR         0x2000
#define RXBRK            0x4000
#endif 

/*

*/
#if CONFIG_BCM6358

#define PERF_BASE            0xfffe0000  /* chip control registers */
#define TIMR_BASE            0xfffe0040  /* timer registers */
#define GPIO_BASE            0xfffe0080  /* gpio registers */
#define UART_BASE            0xfffe0100  /* uart registers */
#define SPI_BASE             0xfffe0800  /* SPI master controller registers */
#define MPI_BASE             0xfffe1000  /* MPI control registers */
#define MEMC_BASE            0xfffe1200  /* Memory control registers */
#define DDR_BASE             0xfffe12a0  /* DDR IO Buf Control registers */
#define USB_EHCI_BASE        0xfffe1300  /* USB host registers */
#define USB_OHCI_BASE        0xfffe1400  /* USB host registers */
#define USBH_CFG_BASE        0xfffe1500
#define PCM_BASE             0xfffe1600  /* PCM control registers */
#define PCM_IUDMA_BASE       0xfffe1800  /* PCM UIDMA register base */
#define ATM_BASE             0xfffe2000  /* ATM SAR control registers */
#define ADSL_BASE            0xfffe3000  /* ADSL core control registers */
#define EMAC1_BASE           0xfffe4000  /* EMAC1 control registers */
#define EMAC2_BASE           0xfffe4800  /* EMAC2 control registers */
#define USB_CTL_BASE         0xfffe5400  /* USB control registers */
#define EMAC_DMA_BASE        0xfffe5000  /* EMAC DMA control registers */
#define USB_DMA_BASE         0xfffe5000  /* USB DMA control registers */

/*
#####################################################################
# System PLL Control Register
#####################################################################
*/
#define SYSPLLCFG       0x08

#define M_MPI_MASK      0x00000018      // 4:3
#define M_MPI_SHFT      3
#define M_MPI_50MHZ     0
#define M_MPI_40MHZ     1
#define M_MPI_33MHZ     2
#define M_MPI_25MHZ     3

#define M_UTO_MASK      0x00000002      // 1:1
#define M_UTO_SHFT      1

#define SOFT_RESET      0x00000001

/*
#####################################################################
# Memory Control Registers
#####################################################################
*/
#define MEMC_CONTROL             0x0
#define MEMC_CONFIG              0x4
#define MEMC_REF_PD_CONTROL      0x8
#define MEMC_BIST_STATUS         0xc
#define MEMC_M_EM_BUF           0x10
#define MEMC_BANK_CLS_TIM       0x14
#define MEMC_PRIOR_INV_TIM      0x18
#define MEMC_DRAM_TIM           0x1c
#define MEMC_INT_STATUS         0x20
#define MEMC_INT_MASK           0x24
#define MEMC_INT_INFO           0x28
#define MEMC_BARRIER            0x50
#define MEMC_CORE_ID            0x54

#define DDR_REV_ID               0x0
#define DDR_PAD_SSTL_MODE        0x4
#define DDR_CMD_PAD_CNTL         0x8
#define DDR_DQ_PAD_CNTL          0xc
#define DDR_DQS_PAD_CNTL        0x10    
#define DDR_CLK_PAD_CNTL0       0x14
#define DDR_MIPSDDR_PLL_CONFIG  0x18
#define DDR_PLL_DESKEW_REG      0x1c

#define DDR_MIPS_PHASE_CNTL     0x20
#define DDR_DDR1_2_PHASE_CNTL0  0x24
#define DDR_DDR3_4_PHASE_CNTL0  0x28    
#define DDR_VCDL_PHASE_CNTL     0x2c
#define DDR_MISC                0x30
#define DDR_SPARE0              0x34
#define DDR_SPARE1              0x38
#define DDR_SPARE2              0x3c
#define DDR_CLBIST              0x40    
#define DDR_LBIST_CRC           0x44


// Some bit/field definitions for the MEMC_CONTROL register.
#define MEMC_DQSGATEEN          (1<<16)
#define MEMC_MIPS1HIPREN        (1<<11)
#define MEMC_MIPS0HIPREN        (1<<10)
#define MEMC_HIPRRTYQEN         (1<<9)

#define MEMC_11BIT_ROW          (0<<6)
#define MEMC_12BIT_ROW          (1<<6)
#define MEMC_13BIT_ROW          (2<<6)
#define MEMC_14BIT_ROW          (3<<6)

#define MEMC_8BIT_COL           (0<<3)
#define MEMC_9BIT_COL           (1<<3)
#define MEMC_10BIT_COL          (2<<3)
#define MEMC_11BIT_COL          (3<<3)

#define MEMC_SELPRIORITY        (1<<2)

#define MEMC_32BIT_BUS          (0<<1)
#define MEMC_16BIT_BUS          (1<<1)

#define MEMC_MEMTYPE_SDR        (0<<0)
#define MEMC_MEMTYPE_DDR        (1<<0)

/*
#####################################################################
# MPI Control Registers
#####################################################################
*/
#define CS0BASE         0x00
#define CS0CNTL         0x04

/*
# CSxBASE settings
#   Size in low 4 bits
#   Base Address for match in upper 24 bits
*/
#define EBI_SIZE_8K         0
#define EBI_SIZE_16K        1
#define EBI_SIZE_32K        2
#define EBI_SIZE_64K        3
#define EBI_SIZE_128K       4
#define EBI_SIZE_256K       5
#define EBI_SIZE_512K       6
#define EBI_SIZE_1M         7
#define EBI_SIZE_2M         8
#define EBI_SIZE_4M         9
#define EBI_SIZE_8M         10
#define EBI_SIZE_16M        11
#define EBI_SIZE_32M        12
#define EBI_SIZE_64M        13
#define EBI_SIZE_128M       14
#define EBI_SIZE_256M       15

/* CSxCNTL settings */
#define EBI_ENABLE          0x00000001  /* .. enable this range */
#define EBI_WAIT_STATES     0x0000000e  /* .. mask for wait states */
#define ZEROWT              0x00000000  /* ..  0 WS */
#define ONEWT               0x00000002  /* ..  1 WS */
#define TWOWT               0x00000004  /* ..  2 WS */
#define THREEWT             0x00000006  /* ..  3 WS */
#define FOURWT              0x00000008  /* ..  4 WS */
#define FIVEWT              0x0000000a  /* ..  5 WS */
#define SIXWT               0x0000000c  /* ..  6 WS */
#define SEVENWT             0x0000000e  /* ..  7 WS */
#define EBI_WORD_WIDE       0x00000010  /* .. 16-bit peripheral, else 8 */
#define EBI_POLARITY        0x00000040  /* .. set to invert chip select polarity */
#define EBI_TS_TA_MODE      0x00000080  /* .. use TS/TA mode */
#define EBI_TS_SEL          0x00000100  /* .. drive tsize, not bs_b */
#define EBI_FIFO            0x00000200  /* .. enable fifo */
#define EBI_RE              0x00000400  /* .. Reverse Endian */

/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define UART0CONTROL     0x01
#define UART0CONFIG      0x02
#define UART0RXTIMEOUT   0x03
#define UART0BAUD        0x04
#define UART0FIFOCFG     0x0a
#define UART0INTMASK     0x10
#define UART0INTSTAT     0x12
#define UART0DATA        0x17

#define BRGEN            0x80   /* Control register bit defs */
#define TXEN             0x40
#define RXEN             0x20
#define LOOPBK           0x10
#define TXPARITYEN       0x08
#define TXPARITYEVEN     0x04
#define RXPARITYEN       0x02
#define RXPARITYEVEN     0x01

#define XMITBREAK        0x40   /* Config register */
#define BITS5SYM         0x00
#define BITS6SYM         0x10
#define BITS7SYM         0x20
#define BITS8SYM         0x30
#define ONESTOP          0x07
#define TWOSTOP          0x0f

#define RSTTXFIFOS       0x80   /* Rx Timeout register */
#define RSTRXFIFOS       0x40

#define TX4              0x40   /* FIFO config register */
#define RX4              0x04

#define DELTAIP          0x0001 /* Interrupt Status and Mask registers */
#define TXUNDERR         0x0002
#define TXOVFERR         0x0004
#define TXFIFOTHOLD      0x0008
#define TXREADLATCH      0x0010
#define TXFIFOEMT        0x0020
#define RXUNDERR         0x0040
#define RXOVFERR         0x0080
#define RXTIMEOUT        0x0100
#define RXFIFOFULL       0x0200
#define RXFIFOTHOLD      0x0400
#define RXFIFONE         0x0800
#define RXFRAMERR        0x1000
#define RXPARERR         0x2000
#define RXBRK            0x4000

/*
#************************************************************************
#* Coprocessor 0 Register Names
#************************************************************************
*/
#define C0_BCM_CONFIG          $22

/*
# Select 1
#  Bit  31:   unused
#  Bits 30:25 MMU Size (Num TLB entries-1)
#  Bits 24:22 ICache sets/way (2^n * 64)
#  Bits 21:19 ICache Line size (2^(n+1) bytes) 0=No Icache
#  Bits 18:16 ICache Associativity (n+1) way                    
#  Bits 15:13 DCache sets/way (2^n * 64)
#  Bits 12:10 DCache Line size (2^(n+1) bytes) 0=No Dcache
#  Bits 9:7   DCache Associativity (n+1) way                    
#  Bits 6:4   unused
#  Bit  3:    1=At least 1 watch register
#  Bit  2:    1=MIPS16 code compression implemented
#  Bit  1:    1=EJTAG implemented                   
#  Bit  0:    1=FPU implemented                   
*/
#define CP0_CFG_ISMSK      (0x7 << 22)
#define CP0_CFG_ISSHF      22
#define CP0_CFG_ILMSK      (0x7 << 19)
#define CP0_CFG_ILSHF      19
#define CP0_CFG_IAMSK      (0x7 << 16)
#define CP0_CFG_IASHF      16
#define CP0_CFG_DSMSK      (0x7 << 13)
#define CP0_CFG_DSSHF      13
#define CP0_CFG_DLMSK      (0x7 << 10)
#define CP0_CFG_DLSHF      10
#define CP0_CFG_DAMSK      (0x7 << 7)
#define CP0_CFG_DASHF      7

/*
#************************************************************************
#* Coprocessor 0 Broadcom Config Register Bits
#************************************************************************
*/
#define CP0_BCM_CFG_ICSHEN          (0x1 << 31)
#define CP0_BCM_CFG_DCSHEN          (0x1 << 30)
#define CP0_BCM_CFG_BTHD            (0x1 << 21)
#define CP0_BCM_CFG_NBK             (0x1 << 17)

/*
#************************************************************************
#* Coprocessor 0 CMT Interrupt Register
#************************************************************************
*/
#define CP0_CMT_XIR_4               (0x1 << 31)
#define CP0_CMT_XIR_3               (0x1 << 30)
#define CP0_CMT_XIR_2               (0x1 << 29)
#define CP0_CMT_XIR_1               (0x1 << 28)
#define CP0_CMT_XIR_0               (0x1 << 27)
#define CP0_CMT_SIR_1               (0x1 << 16)
#define CP0_CMT_SIR_0               (0x1 << 15)
#define CP0_CMT_NMIR_TP1            (0x1 << 1)
#define CP0_CMT_NMIR_TP0            (0x1 << 0)

/*
#************************************************************************
#* Coprocessor 0 CMT Control Register
#************************************************************************
*/
#define CP0_CMT_DSU_TP1             (0x1 << 30)
#define CP0_CMT_TPS3                (0x1 << 19)
#define CP0_CMT_TPS2                (0x1 << 18)
#define CP0_CMT_TPS1                (0x1 << 17)
#define CP0_CMT_TPS0                (0x1 << 16)
#define CP0_CMT_PRIO_TP1            (0x1 << 5)
#define CP0_CMT_PRIO_TP0            (0x1 << 4)
#define CP0_CMT_RSTSE               (0x1 << 0)

/*
#************************************************************************
#* Coprocessor 0 CMT Local Register
#************************************************************************
*/
#define CP0_CMT_TPID                (0x1 << 31)

/*
#************************************************************************
#* MIPS Registers
#************************************************************************
*/

#define MIPS_BASE_BOOT  0xbfa00000
#define MIPS_BASE       0xff400000

#define MIPS_RAC_CR0    0x00        // RAC Configuration Register
#define MIPS_RAC_CR1    0x08        // RAC Configuration Register 1
#define RAC_FLH         (1 << 8)
#define RAC_DPF         (1 << 6)
#define RAC_NCH         (1 << 5)
#define RAC_C_INV       (1 << 4)
#define RAC_PF_D        (1 << 3)
#define RAC_PF_I        (1 << 2)
#define RAC_D           (1 << 1)
#define RAC_I           (1 << 0)

#define MIPS_RAC_ARR    0x04        // RAC Address Range Register
#define RAC_UPB_SHFT    16
#define RAC_LWB_SHFT    0

#define MIPS_LMB_CR     0x1C        // LMB Control Register
#define LMB_EN          (1 << 0)
#define DISABLE_UCL_FIX (1 << 23)
#define DISABLE_UCS_FIX (1 << 24)
#define DISABLE_UC_FIX  (1 << 25)

#define MIPS_SBR        0x20        // System Base Register

#define MIPS_TP0_ALT_BV 0x30000
#define MIPS_TP1_ALT_BV 0x38000
#define ENABLE_ALT_BV   (1 << 19)
#endif

#ifdef __cplusplus
}
#endif

#endif
