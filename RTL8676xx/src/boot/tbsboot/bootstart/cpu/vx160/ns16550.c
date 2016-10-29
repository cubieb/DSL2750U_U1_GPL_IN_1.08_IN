/*
 * COM1 NS16550 support
 * originally from linux source (arch/ppc/boot/ns16550.c)
 * modified to use CFG_ISA_MEM and new defines
 */

#include <config.h>

#ifdef CFG_NS16550

#include "ns16550.h"

#define LCRVAL LCR_8N1					/* 8 data, 1 stop, no parity */
#define MCRVAL (MCR_DTR | MCR_RTS)			/* RTS/DTR */
#define FCRVAL (FCR_FIFO_EN | FCR_RXSR | FCR_TXSR)	/* Clear & enable FIFOs */

void NS16550_init (NS16550_t com_port, int baud_divisor)
{
	com_port->ier = 0x00;
#ifdef CONFIG_OMAP
	com_port->mdr1 = 0x7;	/* mode select reset TL16C750*/
#endif
	com_port->lcr = LCR_BKSE | LCRVAL;
	com_port->dll = baud_divisor & 0xff;
	com_port->dlm = (baud_divisor >> 8) & 0xff;
	com_port->lcr = LCRVAL;
	com_port->mcr = MCRVAL;
	com_port->fcr = FCRVAL;
#if defined(CONFIG_OMAP)
#if defined(CONFIG_APTIX)
	com_port->mdr1 = 3;	/* /13 mode so Aptix 6MHz can hit 115200 */
#else
	com_port->mdr1 = 0;	/* /16 is proper to hit 115200 with 48MHz */
#endif
#endif
}

void NS16550_reinit (NS16550_t com_port, int baud_divisor)
{
	com_port->ier = 0x00;
	com_port->lcr = LCR_BKSE;
	com_port->dll = baud_divisor & 0xff;
	com_port->dlm = (baud_divisor >> 8) & 0xff;
	com_port->lcr = LCRVAL;
	com_port->mcr = MCRVAL;
	com_port->fcr = FCRVAL;
}

void NS16550_putc (NS16550_t com_port, char c)
{
	while ((com_port->lsr & LSR_THRE) == 0);
	com_port->thr = c;
}

char NS16550_getc (NS16550_t com_port)
{
	while ((com_port->lsr & LSR_DR) == 0) {
#ifdef CONFIG_USB_TTY
		extern void usbtty_poll(void);
		usbtty_poll();
#endif
	}
	return (com_port->rbr);
}

int NS16550_tstc (NS16550_t com_port)
{
	return ((com_port->lsr & LSR_DR) != 0);
}

#endif



/*********************************************************
 *
 *   Adapter for TBS,by ZhangYu
 *
 ********************************************************/

static NS16550_t *serial_port = (NS16550_t)CFG_NS16550_COM1;


int serial_init (void)
{
	#define MODE_X_DIV	16
	#define BAUDRATE	57600

	int clock_divisor;


	clock_divisor = CFG_NS16550_CLK / MODE_X_DIV / BAUDRATE;

	NS16550_init(serial_port, clock_divisor);

	return (0);
}


int serial_tstc(void)
{
	return NS16550_tstc(serial_port);
}


int serial_getc(void)
{
	return NS16550_getc(serial_port);
}

void serial_putc(const char c)
{
	if (c == '\n')
		NS16550_putc(serial_port,'\r');

	NS16550_putc(serial_port, c);
}

void serial_puts(const char *s)
{
	while (*s) 
	{
		serial_putc (*s++);
	}
}


