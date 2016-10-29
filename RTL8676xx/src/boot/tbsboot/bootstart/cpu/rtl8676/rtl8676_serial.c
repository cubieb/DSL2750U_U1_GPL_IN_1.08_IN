#include <config.h>
#include <common.h>
#include <command.h>
#include "../../../cpu/rtl8676/rtl8676_uart.h"


int serial_init(void)
{
	unsigned short divisor;
	int i;

	#ifdef CONFIG_USE_UART1
	i = *(volatile unsigned int *)0xB8000300 & (~0xc000); //disable UART0
	i |= 0x300; //enable UART1
	*(volatile unsigned int *)0xB8000300 = i;
	#else
	i = *(volatile unsigned int *)0xB8000300 & (~0x300);  //disable UART1
	i |= 0xC000;  //enable UART0
	*(volatile unsigned int *)0xB8000300 = i;
	#endif
	/* DLAB Set, 8 bit word, no parity, 1 stop bit */
	UART_LCR = 0x83;
	/* Set Baud Rate to CONFIG_BAUDRATE bps */
	divisor = (200000000 / ( 16 * CONFIG_BAUDRATE)) - 1;
	UART_DLM = (divisor >> 8) & 0x00FF;
	UART_DLL = divisor & 0x00FF;
	/* Clear DLAB Bit */
	UART_LCR = 0x03;
	UART_FCR = 0x06;
	for(i=0;i<1000;i++);
	UART_FCR = 0x01;
	UART_IER = 0x00;
	UART_MCR = 0x03;

	return 0;
}


#if 1

void serial_putc(const char c)
{
	if(c=='\n')
		serial_putc('\r');
	/* While !THRE, loop */
	while(0 == (UART_LSR & 0x20));
	UART_THR = c;
}

void serial_puts(const char *s)
{
	int i = 0;

	while(0 != s[i])
		{
		serial_putc(s[i++]);
		}
}

int serial_getc(void)
{

		while(1 != (UART_LSR & 0x01));
	return(UART_RBR);
}

int serial_tstc(void)
{
    return (UART_LSR & 0x01);
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	void (*reboot)(void) = (void *)CONFIG_TEXT_BASE_BOOTSTART;
	reboot();

	return 0;
}
#else

void s1_putc(const char c)
{
	if(c=='\n')
		s1_putc('\r');
	/* While !THRE, loop */
	while(0 == (UART_LSR & 0x20));
	UART_THR = c;
}

void s1_puts(const char *s)
{
	int i = 0;

	while(0 != s[i])
		{
		s1_putc(s[i++]);
		}
}

void initUart(unsigned int baud_rate)
{
    unsigned short divisor;

    /* DLAB Set, 8 bit word, no parity, 1 stop bit */
    *((char *)UART_LCR) = 0x83;

    /* Set Baud Rate to 9600 bps */
    divisor = (25000000 / ( 16 * baud_rate )) - 1;
    *((char *)UART_DLM) = (divisor >> 8) & 0x00FF;
    *((char *)UART_DLL) = divisor & 0x00FF;

    /* Clear DLAB Bit */
    *((char *)UART_LCR) = 0x03;
    
    *((char *)UART_MCR) = 0x00;
}

void uputc( char c )
{

	if(c=='\n')
		uputc('\r');
    /* While !THRE, loop */
    while ( 0 == (*((volatile char *)UART_LSR) & 0x20) );

    *((char *)UART_THR) = c;
}

void uputs( char *string )
{
    int index = 0;

    while ( 0 != string[ index ] )
    {
		uputc( string[ index++ ] );
    }
    return;
}

void C_Entry(void)
{


	const int calibrationDbgSize = 256;
	unsigned char calibrationDBG[calibrationDbgSize];
    	initUart( 115200 );		
		uputs("Start to decompress!\n\r");
	memcpy(calibrationDBG, 0xA0600000, calibrationDbgSize);	//copy ddr calibration information from sram to dram
	*((unsigned long*)0xb8001300)=0x0;
	*((unsigned long*)0xb8001310)=0x0;
	*((unsigned long*)0xb8001320)=0x0;
	*((unsigned long*)0xb8001330)=0x0;	
	*((unsigned long*)0xb8004000)=0x0;
	*((unsigned long*)0xb8004010)=0x0;
	*((unsigned long*)0xb8004020)=0x0;
	*((unsigned long*)0xb8004030)=0x0;
	memcpy(0xA0600000, calibrationDBG, calibrationDbgSize);	//copy ddr calibration information to 0xA0600000 on dram
	/* initialize UART for debug message*/	

	




	serial_init();
	s1_putc('\r');
	s1_putc('\n');
	s1_puts("Hello, world\n");
	while(1);
}
#endif
