#ifndef RTL8196C_GPIO_H
#define RTL8196C_GPIO_H
/*port A pin*/
#define GPIO_PA7	0x00000080
#define GPIO_PA6	0x00000040
#define GPIO_PA5	0x00000020
#define GPIO_PA4	0x00000010
#define GPIO_PA3	0x00000008
#define GPIO_PA2	0x00000004
#define GPIO_PA1	0x00000002
#define GPIO_PA0	0x00000001
/*port B pin*/
#define GPIO_PB7	0x00008000
#define GPIO_PB6	0x00004000
#define GPIO_PB5	0x00002000
#define GPIO_PB4	0x00001000
#define GPIO_PB3	0x00000800
#define GPIO_PB2	0x00000400
#define GPIO_PB1	0x00000200
#define GPIO_PB0	0x00000100
/*port C pin*/
#define GPIO_PC7	0x00800000
#define GPIO_PC6	0x00400000
#define GPIO_PC5	0x00200000
#define GPIO_PC4	0x00100000
#define GPIO_PC3	0x00080000
#define GPIO_PC2	0x00040000
#define GPIO_PC1	0x00020000
#define GPIO_PC0	0x00010000
/*port D pin*/
#define GPIO_PD7	0x80000000
#define GPIO_PD6	0x40000000
#define GPIO_PD5	0x20000000
#define GPIO_PD4	0x10000000
#define GPIO_PD3	0x08000000
#define GPIO_PD2	0x04000000
#define GPIO_PD1	0x02000000
#define GPIO_PD0	0x01000000

/* 
 *GPIO control registers 
*/
#define GPIOCR_BASE 0xB8003500
/*Port A,B,C,D*/
#define GPIO_PABCD_CNR		(GPIOCR_BASE+0x00)	/*Port A,B,C,D control register*/
#define GPIO_PABCD_PTYPE	(GPIOCR_BASE+0x04)	/*Port A,B,C,D peripheral type control register*/
#define GPIO_PABCD_DIR		(GPIOCR_BASE+0x08)	/*Port A,B,C,D direction */
#define GPIO_PABCD_DAT		(GPIOCR_BASE+0x0C)	/*Port A,B,C,D data register*/
#define GPIO_PABCD_ISR		(GPIOCR_BASE+0x10)	/*Port A,B,C,D interrupt status register*/
#define GPIO_PAB_IMR		(GPIOCR_BASE+0x14)	/*Port A,B interrupt mask register*/
#define GPIO_PCD_IMR		(GPIOCR_BASE+0x18)	/*Port C,D interrupt mask register*/

/*Port A*/
#define GPIO_PADIR	(GPIOCR_BASE+0x00L)	/*Port A direction register*/
#define GPIO_PADAT	(GPIOCR_BASE+0x04L)	/*Port A data register*/
#define GPIO_PAISR	(GPIOCR_BASE+0x08L)	/*Port A interrupt status register*/
#define GPIO_PAIMR	(GPIOCR_BASE+0x0CL)	/*Port A interrupt mask register*/
/*Port B*/
#define GPIO_PBDIR	(GPIOCR_BASE+0x10L)	/*Port B direction register*/
#define GPIO_PBDAT	(GPIOCR_BASE+0x14L)	/*Port B data register*/
#define GPIO_PBISR	(GPIOCR_BASE+0x18L)	/*Port B interrupt status register*/
#define GPIO_PBIMR	(GPIOCR_BASE+0x1CL)	/*Port B interrupt mask register*/

#define PIN_MUX_SEL	0xB8000040

#define REG_SHARED_PIN_CFG	0x00300000  /* 0x003C0000 Only for USB LED */

#define JTAG_TRST	GPIO_PA3  /* Pin 96 */
#define JTAG_TMS	GPIO_PA4  /* Pin 97 */
#define JTAG_TDO	GPIO_PA6  /* Pin 98 */
#define JTAG_TDI	GPIO_PA5  /* Pin 99 */


#define	GPIO_WPS_BUTTON		JTAG_TRST
#define GPIO_RESET_BUTTON	JTAG_TDI
#define	GPIO_WPS_LED		JTAG_TMS
#define GPIO_INET_LED		JTAG_TDO

#define REG_READ(addr)			*(volatile u32 *)(addr)
#define REG_WRITE(addr, value)	*(u32 *)(addr) = (volatile u32)(value)


#endif
