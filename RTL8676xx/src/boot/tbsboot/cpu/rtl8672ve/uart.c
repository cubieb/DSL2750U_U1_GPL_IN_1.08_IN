/*#include "loader.h"*/
//#include "board.h"
#include <config.h>
#include <common.h>
#include "uart.h"

void initUart(unsigned int baud_rate)
{
    unsigned short divisor;
	int i;
	
    /* DLAB Set, 8 bit word, no parity, 1 stop bit */
    UART_LCR = 0x83;

    /* Set Baud Rate to 9600 bps */
    divisor = (CLOCKGEN_FREQ / ( 16 * baud_rate )) - 1;
    UART_DLM = (divisor >> 8) & 0x00FF;
    UART_DLL = divisor & 0x00FF;

    /* Clear DLAB Bit */
    UART_LCR = 0x03;

	UART_FCR = 0x06;
	for(i=0;i<1000;i++);
	UART_FCR = 0x01;
    
    UART_IER = 0x00;
    UART_MCR = 0x03;
    
}


void
serial_putc(const char c)
{
	if(c=='\n')
		serial_putc('\r');
    /* While !THRE, loop */
    while ( 0 == (UART_LSR & 0x20) );

    UART_THR = c;

}

void
serial_putc_raw(const char c)
{

}

void
serial_puts(const char *s)
{
    int index = 0;

    while ( 0 != s[ index ] )
    {
		serial_putc( s[ index++ ] );
    }
    return;
}

int
serial_getc(void)
{
	if( serial_load == 0 )
	{
		while( 1 != (UART_LSR & 0x01) )
		{
			return 0;
		}
	}
	else
	{
		while( 1 != (UART_LSR & 0x01) );
	}
	
	return( UART_RBR );
}

int
serial_tstc(void)
{
    return (UART_LSR & 0x01 );
}

void
serial_setbrg(void)
{
	
}

int serial_init(void)
{
	initUart(CONFIG_BAUDRATE);

	return 0;
}