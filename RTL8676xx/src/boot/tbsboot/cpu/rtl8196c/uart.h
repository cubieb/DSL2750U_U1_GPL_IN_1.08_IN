
#ifndef _UART_H
#define _UART_H

#ifdef __cplusplus
extern "C" {
#endif
/* for simulation codes */
#define REG32(addr) (*((volatile u32*)(addr)))
#define rtl_outb(reg,val)	(*(volatile unsigned char *)(reg) = val)

//UART REG
#define UART_BASE        0xB8000000
#define UART_RBR_REG	(0x2000+UART_BASE)
#define UART_THR_REG	(0x2000+UART_BASE)
#define UART_DLL_REG	(0x2000+UART_BASE)
#define	UART_IER_REG	(0x2004+UART_BASE)	
#define	UART_DLM_REG	(0x2004+UART_BASE)
#define	UART_IIR_REG	(0x2008+UART_BASE)
#define	UART_FCR_REG	(0x2008+UART_BASE)
#define UART_LCR_REG	(0x200c+UART_BASE)
#define	UART_MCR_REG	(0x2010+UART_BASE)
#define	UART_LSR_REG	(0x2014+UART_BASE)
#define	UART_MSR_REG	(0x2018+UART_BASE)
#define	UART_SCR_REG	(0x201c+UART_BASE)


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
