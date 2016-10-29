/*
 * COM1 NS16550 support
 * originally from linux source (arch/ppc/boot/ns16550.c)
 * modified to use CFG_ISA_MEM and new defines
 */
#include <config.h>
#include <common.h>
#include "solos_serial.h"

int serial_tsttx(void);

int serial_init (void)
{
	/* The UART has probably already been used by the boot code. Allow it to */
	/* transmit any remaining characters before (re)initialising it here, to */
	/* avoid corrupted terminal output.                                      */

	U32 start;

	/* Wait for tx buffer to drain */
	while ((PPIO(UART_STATUS) & HF_PP_UART_CS_TXEMPTY) == 0) continue;

	/* Wait for two more milliseconds (plenty for one char at 9600bps) */
	start = TIMER_CURRENT_TIME;
	while ((TIMER_CURRENT_TIME - start) < CLOCKFREQ/500) continue;


	PPIO(UART_CONTROL) = 0; /* UART soft reset */

	/* Set the (hardware dependent) baud rate, enable the UART, set low */
	/* pri IRQ, clear the rx overrun flag, disable rx and tx interrupts */
	PPIO(UART_CONTROL) = HELIUM_UART_BAUDRATE | HF_PP_UART_CS_UARTEN |
                         HF_PP_UART_CS_LPIRQ  | HF_PP_UART_CS_RXORUN;
	return (0);
}



void
solos_serial_putc(const char c)
{
        while(serial_tsttx());
        PPIO(UART_TX) = (U32) c;
}


void
serial_putc(const char c)
{

	if (c == '\n')
                solos_serial_putc('\r');
	
	solos_serial_putc(c);
}




void
serial_putc_raw(const char c)
{
	PPIO(UART_TX) = (U32)(c);
}

void
serial_puts(const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

int
serial_getc(void)
{
	char ch = 0;
	/*  Receive a character  */
	
	if( serial_load == 0 )
	{
		while(!serial_tstc())
		{
			return 0;
		}
	}
	else
	{
		while(!serial_tstc());
	}

	ch = PPIO(UART_RX) & 0xff;

	return ch;
}

int
serial_tstc(void)
{
	/*  Check to see if there is a character to receive  */

	return PPIO(UART_STATUS) & HF_PP_UART_CS_RXNOTEMPTY;
}

int serial_tsttx(void)
{
    /*  Check to see if we can transmit a character  */

    return PPIO(UART_STATUS) & HF_PP_UART_CS_TXFULL;
}   

void
serial_setbrg(void)
{
	serial_init();
}





