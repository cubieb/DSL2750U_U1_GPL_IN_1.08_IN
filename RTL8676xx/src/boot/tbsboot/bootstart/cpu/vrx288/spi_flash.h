
#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

/* Controller-specific definitions: */

/* CONFIG_HARD_SPI triggers SPI bus initialization in PowerPC */
#ifdef CONFIG_MPC8XXX_SPI
# ifndef CONFIG_HARD_SPI
#  define CONFIG_HARD_SPI
# endif
#endif

/***********************************************************************/
/*  Module      :  SSC1 register address and bits                      */
/***********************************************************************/
#define SPI_CLC             ((volatile u32*)(IFX_SPI_BASE+0x0000))
#define SPI_PISEL           ((volatile u32*)(IFX_SPI_BASE+0x0004))
#define SPI_ID              ((volatile u32*)(IFX_SPI_BASE+0x0008))
#define SPI_CON             ((volatile u32*)(IFX_SPI_BASE+0x0010))
#define SPI_STAT            ((volatile u32*)(IFX_SPI_BASE+0x0014))
#define SPI_WHBSTATE        ((volatile u32*)(IFX_SPI_BASE+0x0018))
#define SPI_TB              ((volatile u32*)(IFX_SPI_BASE+0x0020))
#define SPI_RB              ((volatile u32*)(IFX_SPI_BASE+0x0024))
#define SPI_RXFCON          ((volatile u32*)(IFX_SPI_BASE+0x0030))
#define SPI_TXFCON          ((volatile u32*)(IFX_SPI_BASE+0x0034))
#define SPI_FSTAT           ((volatile u32*)(IFX_SPI_BASE+0x0038))
#define SPI_BRT             ((volatile u32*)(IFX_SPI_BASE+0x0040))
#define SPI_BRSTAT          ((volatile u32*)(IFX_SPI_BASE+0x0044))
#define SPI_SFCON           ((volatile u32*)(IFX_SPI_BASE+0x0060))
#define SPI_SFSTAT          ((volatile u32*)(IFX_SPI_BASE+0x0064))
#define SPI_GPOCON          ((volatile u32*)(IFX_SPI_BASE+0x0070))
#define SPI_GPOSTAT         ((volatile u32*)(IFX_SPI_BASE+0x0074))
#define SPI_FGPO            ((volatile u32*)(IFX_SPI_BASE+0x0078))
#define SPI_RXREQ           ((volatile u32*)(IFX_SPI_BASE+0x0080))
#define SPI_RXCNT           ((volatile u32*)(IFX_SPI_BASE+0x0084))
#define SPI_DMACON          ((volatile u32*)(IFX_SPI_BASE+0x00ec))
#define SPI_IRNEN           ((volatile u32*)(IFX_SPI_BASE+0x00f4))
#define SPI_IRNICR          ((volatile u32*)(IFX_SPI_BASE+0x00f8))
#define SPI_IRNCR           ((volatile u32*)(IFX_SPI_BASE+0x000c))

/*control bits in SPI_CON*/
#define SPI_CON_RUEN            (1 << 12)
#define SPI_CON_TUEN            (1 << 11)
#define SPI_CON_AEN             (1 << 10)
#define SPI_CON_REN             (1 << 9)
#define SPI_CON_TEN             (1 << 8)
#define SPI_CON_LB              (1 << 7)
#define SPI_CON_PO              (1 << 6)
#define SPI_CON_PH              (1 << 5)
#define SPI_CON_HB              (1 << 4)
#define SPI_CON_BM(value)       (((( 1 << 5) - 1) & (value)) << 16)
#define SPI_CON_RX_OFF          (1 << 1)
#define SPI_CON_TX_OFF          (1 << 0)


/*control bits in SPI_STAT*/
#define SPI_STAT_EN         (1 << 0)
#define SPI_STAT_MS         (1 << 1)
#define SPI_STAT_BSY        (1 << 13)
#define SPI_STAT_RUE        (1 << 12)
#define SPI_STAT_TUE        (1 << 11)
#define SPI_STAT_AE         (1 << 10)
#define SPI_STAT_RE         (1 << 9)
#define SPI_STAT_TE         (1 << 8)
#define SPI_STAT_BC(value)  (((( 1 << 5) - 1) & (value)) << 16)

/***SSC Receive FIFO Control Register***/
#define SPI_RXFCON_RXFITL(value) (((( 1 << 6) - 1) & (value)) << 8)
#define SPI_RXFCON_RXTMEN    (1 << 2)
#define SPI_RXFCON_RXFLU     (1 << 1)
#define SPI_RXFCON_RXFEN     (1 << 0)
/***SSC Transmit FIFO Control Register***/
#define SPI_TXFCON_RXFITL(value) (((( 1 << 6) - 1) & (value)) << 8)
#define SPI_TXFCON_TXTMEN    (1 << 2)
#define SPI_TXFCON_TXFLU     (1 << 1)
#define SPI_TXFCON_TXFEN     (1 << 0)

/***SSC FIFO Status Register***/
#define SPI_FSTAT_TXFFL(value)   (((( 1 << 6) - 1) & (value)) << 8)
#define SPI_FSTAT_RXFFL(value)   (((( 1 << 6) - 1) & (value)) << 0)



#define DATA_WIDTH	         8
#define RXFIFO_SIZE          8
#define TXFIFO_SIZE          8

#define SFLASH_BAUDRATE	     2000000


/* Common parameters */

/* MX25LXX05D-specific commands */
#define CMD_MX25L_WREN     0x06
#define CMD_MX25L_WRDI     0x04
#define CMD_MX25L_WRSR     0x01 /* Write Status Register */
#define CMD_MX25L_RDID     0x9f
#define CMD_MX25L_RDSR     0x05 /* Read Status Register */

#define CMD_MX25L_READ     0x03
#define CMD_MX25L_FASTREAD 0x0B
#define CMD_MX25L_RES      0xAB
#define CMD_MX25L_REMS     0x90
#define	CMD_MX25L_DREAD	   0x3B

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




#define CMD_MX25L_REMS2    0xef
#define CMD_MX25L_CP       0xad
#define CMD_MX25L_ESRY     0x70
#define CMD_MX25L_DSRY     0x80

#define MX25L_SR_WIP		(1 << 0)        /* Write-in-Progress */
#define	MX25L_SR_WEL		(1 << 1)		/* Write-enable-latch */
#define MX25L_SR_SRWD		(1 << 7)		/* Status-register-write-disable */

/* SPI mode flags */
#define	SPI_CPHA	0x01			/* clock phase */
#define	SPI_CPOL	0x02			/* clock polarity */
#define	SPI_MODE_0	(0|0)			/* (original MicroWire) */
#define	SPI_MODE_1	(0|SPI_CPHA)
#define	SPI_MODE_2	(SPI_CPOL|0)
#define	SPI_MODE_3	(SPI_CPOL|SPI_CPHA)
#define	SPI_CS_HIGH	0x04			/* CS active high */
#define	SPI_LSB_FIRST	0x08			/* per-word bits-on-wire */
#define	SPI_3WIRE	0x10			/* SI/SO signals shared */
#define	SPI_LOOP	0x20			/* loopback mode */

/* SPI transfer flags */
#define SPI_WRITE	0x01			/* Write SPI */
#define SPI_READ	0x02			/* Read SPI */

enum {
    IFX_SSC_CS1 = 0, 
    IFX_SSC_CS2,
    IFX_SSC_CS3,
    IFX_SSC_CS4,
    IFX_SSC_CS5,
    IFX_SSC_CS6,
    IFX_SSC_CS7,
};


extern ulong get_timer (ulong base);
extern int memcmp(const void * cs, const void * ct, size_t count);

#endif	/* _SPI_FLASH_H_ */
