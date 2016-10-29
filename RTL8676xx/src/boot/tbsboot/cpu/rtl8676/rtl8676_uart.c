#include <config.h>
#include <common.h>
#include "rtl8676_uart.h"

extern unsigned int CLOCKGEN_FREQ;


void unmap_sram(void)
{
	const int calibrationDbgSize = 256;
	unsigned char calibrationDBG[calibrationDbgSize];

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
	
}
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
	divisor = (CLOCKGEN_FREQ / ( 16 * gd->bi_baudrate)) - 1;
	UART_DLM = (divisor >> 8) & 0x00FF;
	UART_DLL = divisor & 0x00FF;
	/* Clear DLAB Bit */
	UART_LCR = 0x03;
	UART_FCR = 0x06;
	for(i=0;i<1000;i++);
	UART_FCR = 0x01;
	UART_IER = 0x00;
	UART_MCR = 0x03;
	
	unmap_sram();     //patch for Bugid:38426 
	
	return 0;
}

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
	if(0 == serial_transfer)
		{
		if(1 != (UART_LSR & 0x01))
			{
			return 0;
			}
		}
	else
		{
		while(1 != (UART_LSR & 0x01));
		}

	return(UART_RBR);
}

int serial_tstc(void)
{
    return (UART_LSR & 0x01);
}