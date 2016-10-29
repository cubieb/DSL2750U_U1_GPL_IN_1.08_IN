
#ifndef _UART_H
#define _UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* include files */


//tylo, for 8672 fpga
#define  UART_BASE_ADDRESS 0xb8002000
//#define  UART_BASE_ADDRESS    0xb9c00000

#define UART_RBR     *((volatile char *)UART_BASE_ADDRESS+0x00)
#define UART_THR     *((volatile char *)UART_BASE_ADDRESS+0x00)    /* Tx Holding */
#define UART_DLL     *((volatile char *)UART_BASE_ADDRESS+0x00)
#define UART_IER     *((volatile char *)UART_BASE_ADDRESS+0x04)    /* Interrupt Enable */
#define UART_DLM     *((volatile char *)UART_BASE_ADDRESS+0x04)
#define UART_IIR     *((volatile char *)UART_BASE_ADDRESS+0x08)
#define UART_FCR     *((volatile char *)UART_BASE_ADDRESS+0x08)    /* FIFO Control */
#define UART_LCR     *((volatile char *)UART_BASE_ADDRESS+0x0c)    /* Line Control */
#define UART_MCR     *((volatile char *)UART_BASE_ADDRESS+0x10)    /* Modem Control */
#define UART_LSR     *((volatile char *)UART_BASE_ADDRESS+0x14)    /* Line Status */
#define UART_MSR     *((volatile char *)UART_BASE_ADDRESS+0x18)    /* Modem Status */
#define UART_SCR     *((volatile char *)UART_BASE_ADDRESS+0x1c)    /* Scratchpad */

#define  UART_BAUD_2400       (CLOCKGEN_FREQ/16/2400)
#define  UART_BAUD_4800       (CLOCKGEN_FREQ/16/4800)
#define  UART_BAUD_9600       (CLOCKGEN_FREQ/16/9600)
#define  UART_BAUD_19200      (CLOCKGEN_FREQ/16/19200)
#define  UART_BAUD_38400      (CLOCKGEN_FREQ/16/38400)
#define  UART_BAUD_57600      (CLOCKGEN_FREQ/16/57600)
#define  UART_BAUD_115200     (CLOCKGEN_FREQ/16/115200)



#ifdef __cplusplus
}
#endif   
#if 0
void initUart( unsigned int );
void puts( char * );
void putc( char );
void puthex(unsigned long);
char getc( void );
char pollcc( void );
int getcc( void );
#endif
#endif /* for the #define uart.h */					    
