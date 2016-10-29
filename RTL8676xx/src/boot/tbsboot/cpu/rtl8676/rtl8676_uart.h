#ifndef _UART_H
#define _UART_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_USE_UART0)
#define	UART_BASE 0xb8002000
#elif defined(CONFIG_USE_UART1)
#define	UART_BASE 0xB8002100
#endif

#define UART_RBR     *((volatile char *)UART_BASE)
#define UART_THR     *((volatile char *)UART_BASE)    /* Tx Holding */
#define UART_DLL     *((volatile char *)UART_BASE)
#define UART_IER     *((volatile char *)UART_BASE+0x04)    /* Interrupt Enable */
#define UART_DLM     *((volatile char *)UART_BASE+0x04)
#define UART_IIR     *((volatile char *)UART_BASE+0x08)
#define UART_FCR     *((volatile char *)UART_BASE+0x08)    /* FIFO Control */
#define UART_LCR     *((volatile char *)UART_BASE+0x0C)    /* Line Control */
#define UART_MCR     *((volatile char *)UART_BASE+0x10)    /* Modem Control */
#define UART_LSR     *((volatile char *)UART_BASE+0x14)    /* Line Status */
#define UART_MSR     *((volatile char *)UART_BASE+0x18)    /* Modem Status */
#define UART_SCR     *((volatile char *)UART_BASE+0x1C)    /* Scratchpad */
#ifdef __cplusplus
}
#endif

#endif /* for the #define uart.h */					    
