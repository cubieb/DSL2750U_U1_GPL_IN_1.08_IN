#include <config.h>
#include <common.h>
#include "uart.h"

void initUart(unsigned int baud_rate)
{
	unsigned long dl;
	unsigned long dll;     
	unsigned long dlm;       
           
  	REG32( UART_LCR_REG)=0x03000000;		//Line Control Register  8,n,1
  			
  	REG32( UART_FCR_REG)=0xc7000000;		//FIFO Ccontrol Register
  	REG32( UART_IER_REG)=0x00000000;
  	dl = (200000000 /16)/baud_rate-1;
  	*(volatile unsigned long *)(0xa1000000) = dl ; 
  	dll = dl & 0xff;
  	dlm = dl / 0x100;
  	REG32( UART_LCR_REG)=0x83000000;		//Divisor latch access bit=1
  	REG32( UART_DLL_REG)=dll*0x1000000;
   	REG32( UART_DLM_REG)=dlm*0x1000000; 
   	REG32( UART_LCR_REG)=0x83000000& 0x7fffffff;	//Divisor latch access bit=0
	//printf("\nUART1 output test ok\n");
    
}

#define rtl_inb(reg)		(*(volatile unsigned char *)(reg))

void
serial_putc(const char c)
{
	int i;
   i=0;
#if 1
    while (1)
	{
		i++;
		if (i >=3210)
			break;
		
		if 	(rtl_inb(UART_LSR_REG) & 0x60)
			break;	
	}
#endif

	//for(i=0; i<0xff00;i++);
 	rtl_outb(UART_THR_REG, c);  
		  
	if (c == 0x0a)
		rtl_outb(UART_THR_REG, 0x0d);  
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
	int i;
    if( serial_load == 0 )
	{
		while( 1 != (rtl_inb(UART_LSR_REG) & 0x1) )
		{
			return 0;
		}
	}
	else
	{
		while( 1 != (rtl_inb(UART_LSR_REG) & 0x1) );
	}
	
	i=rtl_inb(UART_RBR_REG);
	return (i & 0xff);
}
	
int
serial_tstc(void)
{
    return (rtl_inb(UART_LSR_REG) & 0x01 );
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
