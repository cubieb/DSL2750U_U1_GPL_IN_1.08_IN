/*
 * (C) Copyright 2006
 * Mindspeed Technologies
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>
//#include <asm/io.h>
#include "hardware.h"
#include "bsp_1000.h"


DECLARE_GLOBAL_DATA_PTR;

#if !defined(CONFIG_UART0) && !defined(CONFIG_UART1)
#error must define one of CONFIG_UART0 or CONFIG_UART1
#endif


typedef struct tagUART16550REGS {
	volatile u32 data;	// Receive/Transmit data register
	volatile u32 ier;	// Interrupt Enable register
	volatile u32 iir_fcr;// Interrupt Identity register / FIFO Control register
	volatile u32 lcr;	// Line Control register
	volatile u32 mcr;	// Modem Control register
	volatile u32 lsr;	// Line Status register
	volatile u32 msr;	// Modem Status register
	volatile u32 sr;		// Scratch register
} UART16550REGS, *PUART16550REGS;

typedef struct tagUART16550DLREGS {
	volatile u32 dll;	// Divisor Latch (Low)
	volatile u32 dlh;	// Divisor Latch (High)
} UART16550DLREGS, *PUART16550DLREGS;

#define LCR_CHAR_LEN_5		0x00	  /* default */
#define LCR_CHAR_LEN_6		0x01
#define LCR_CHAR_LEN_7		0x02
#define LCR_CHAR_LEN_8		0x03
#define LCR_ONE_STOP		0x00	  /* One stop bit! - default */
#define LCR_TWO_STOP		0x04	  /* Two stop bit! */
#define LCR_PEN				0x08	  /* Parity Enable */
#define LCR_PARITY_NONE		0x00
#define LCR_EPS				0x10	  /* Even Parity Select */
#define LCR_PS				0x20	  /* Enable Parity  Stuff */
#define LCR_SBRK				0x40  /* Start Break */
#define LCR_PSB				0x80	  /* Parity Stuff Bit */
#define LCR_DLAB				0x80  /* UART 16550 Divisor Latch Assess */

#define LSR_FIFOE		(1 << 7)	/* FIFO Error Status */
#define LSR_TEMT			(1 << 6)	/* Transmitter Empty */
#define LSR_TDRQ			(1 << 5)	/* Transmit Data Request */
#define LSR_BI			(1 << 4)	/* Break Interrupt */
#define LSR_FE			(1 << 3)	/* Framing Error */
#define LSR_PE			(1 << 2)	/* Parity Error */
#define LSR_OE			(1 << 1)	/* Overrun Error */
#define LSR_DR			(1 << 0)	/* Data Ready */


#define IER_DMAE			(1 << 7)	/* DMA Requests Enable */
#define IER_UUE			(1 << 6)	/* UART Unit Enable */
#define IER_NRZE			(1 << 5)	/* NRZ coding Enable */
#define IER_RTIOE			(1 << 4)	/* Receiver Time Out Interrupt Enable */
#define IER_MIE			(1 << 3)	/* Modem Interrupt Enable */
#define IER_RLSE			(1 << 2)	/* Receiver Line Status Interrupt Enable */
#define IER_TIE			(1 << 1)	/* Transmit Data request Interrupt Enable */
#define IER_RAVIE			(1 << 0)	/* Receiver Data Available Interrupt Enable */

#define IIR_FIFOES1		(1 << 7)	/* FIFO Mode Enable Status */
#define IIR_FIFOES0		(1 << 6)	/* FIFO Mode Enable Status */
#define IIR_TOD			(1 << 3)	/* Time Out Detected */
#define IIR_IID2			(1 << 2)	/* Interrupt Source Encoded */
#define IIR_IID1			(1 << 1)	/* Interrupt Source Encoded */
#define IIR_IP				(1 << 0)	/* Interrupt Pending (active low) */

/* UART 16550 FIFO Control Register */
#define FCR_FIFOEN			0x01
#define FCR_RCVRRES			0x02
#define FCR_XMITRES			0x04

/* Interrupt Enable Register */
// UART 16550
#define IER_RXTH			0x01	// Enable Received Data Available Interrupt
#define IER_TXTH			0x02	// Enable Transmitter Empty Interrupt

#ifdef CONFIG_UART0
PUART16550REGS uart = (PUART16550REGS) UART0_BASEADDR;
PUART16550DLREGS uart_dl = (PUART16550DLREGS) UART0_BASEADDR;
#if defined(CONFIG_COMCERTO_100)
static void EnableUART0(void)
{
	u32 temp;

	do {
		temp = *(volatile u32 *)GPIO_IOCTRL_REG | GPIO_IOCTRL_UART0; 	// enable UART0
		*(volatile u32 *)GPIO_LOCK_REG = 0x55555555; 				// remove lock
		*(volatile u32 *)GPIO_IOCTRL_REG = temp;				// write to ctrl reg
	} while (*(volatile u32 *)GPIO_IOCTRL_REG != temp);
}
#endif
#else
PUART16550REGS uart = (PUART16550REGS) UART1_BASEADDR;
PUART16550DLREGS uart_dl = (PUART16550DLREGS) UART1_BASEADDR;
#if defined(CONFIG_COMCERTO_100)
static void EnableUART1(void)
{
	u32 temp;

	do {
		temp = *(volatile u32 *)GPIO_IOCTRL_REG | GPIO_IOCTRL_UART1; 	// enable UART0
		*(volatile u32 *)GPIO_LOCK_REG = 0x55555555; 				// remove lock
		*(volatile u32 *)GPIO_IOCTRL_REG = temp;				// write to ctrl reg
	} while (*(volatile u32 *)GPIO_IOCTRL_REG != temp);
}
#else
static void EnableUART1(void)
{
	*(volatile u32 *)GPIO_PIN_SELECT_REG |= GPIO_PIN_SELECT_UART1; 		// enable UART1
}
#endif
#endif

void serial_setbrg (void)
{
	int baudrate;
	unsigned int reg = 0;
	unsigned int clk = HAL_get_amba_clk();

	baudrate = CONFIG_BAUDRATE;

	reg = clk / (baudrate * 16);
	uart->lcr |= LCR_DLAB; // Enable Divisor Latch access
	uart_dl->dll = reg & 0xFF; // Set Divisor Latch Low
	uart_dl->dlh = (reg >> 8) & 0xFF; // Set Divisor Latch high
	uart->lcr &= ~LCR_DLAB; // Disable Divisor Latch access
}

int serial_init (void)
{
#ifdef CONFIG_UART1
	EnableUART1();
#elif defined(CONFIG_COMCERTO_100)
	EnableUART0();
#endif

	serial_setbrg ();
	
	uart->lcr = LCR_ONE_STOP | LCR_CHAR_LEN_8;
	uart->iir_fcr = FCR_XMITRES | FCR_RCVRRES | FCR_FIFOEN; // Reset, FIFO
	uart->ier = IER_UUE;		/* Enable FFUART */

	return (0);
}

void serial_putc (const char c)
{
	/* wait for room in the tx FIFO on FFUART */
	while ((uart->lsr & LSR_TEMT)==0) ;
	uart->data = c;
	if (c == '\n')
		serial_putc ('\r');
}

void serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

int serial_getc (void)
{
	while ((uart->lsr & LSR_DR) == 0);
		return (char)uart->data & 0xff;	
}

int serial_tstc (void)
{
	return uart->lsr & LSR_DR;
}
