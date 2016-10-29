/*
 * COM1 NS16550 support
 * originally from linux source (arch/ppc/boot/ns16550.c)
 * modified to use CFG_ISA_MEM and new defines
 */

#include <config.h>
#include <common.h>
#include"atheros_serial.h"
#include"ar7100_soc.h"

#define CFG_NS16550_CLK		 150000000   /*当CPU为倍频到300_300_150时*/
#define CFG_NS16550_COM1	 0xb8020000   /*串口控制器的地址*/
#define BAUDRATE 115200

//-----------------------------------------------------------------------------
// Define the serial registers. The AR7100 is equipped with a 16550C
// serial chip.
#define SER_16550_RBR 0x00*4   // receiver buffer register, read, dlab = 0
#define SER_16550_THR 0x00*4   // transmitter holding register, write, dlab = 0
#define SER_16550_DLL 0x00*4   // divisor latch (LS), read/write, dlab = 1
#define SER_16550_IER 0x01*4   // interrupt enable reg, read/write, dlab = 0
#define SER_16550_DLM 0x01*4   // divisor latch (MS), read/write, dlab = 1
#define SER_16550_IIR 0x02*4   // interrupt identification reg, read, dlab = 0
#define SER_16550_FCR 0x02*4   // fifo control register, write, dlab = 0
#define SER_16550_AFR 0x02*4   // alternate function reg, read/write, dlab = 1
#define SER_16550_LCR 0x03*4   // line control register, read/write
#define SER_16550_MCR 0x04*4   // modem control register, read/write
#define SER_16550_LSR 0x05*4   // line status register, read
#define SER_16550_MSR 0x06*4   // modem status register, read
#define SER_16550_SCR 0x07*4   // scratch pad register

// The interrupt enable register bits.
#define SIO_IER_ERDAI   0x01            // enable received data available irq
#define SIO_IER_ETHREI  0x02            // enable THR empty interrupt
#define SIO_IER_ELSI    0x04            // enable receiver line status irq
#define SIO_IER_EMSI    0x08            // enable modem status interrupt

// The interrupt identification register bits.
#define SIO_IIR_IP      0x01            // 0 if interrupt pending
#define SIO_IIR_ID_MASK 0x0e            // mask for interrupt ID bits

// The line status register bits.
#define SIO_LSR_DR      0x01            // data ready
#define SIO_LSR_OE      0x02            // overrun error
#define SIO_LSR_PE      0x04            // parity error
#define SIO_LSR_FE      0x08            // framing error
#define SIO_LSR_BI      0x10            // break interrupt
#define SIO_LSR_THRE    0x20            // transmitter holding register empty
#define SIO_LSR_TEMT    0x40            // transmitter register empty
#define SIO_LSR_ERR     0x80            // any error condition

// The modem status register bits.
#define SIO_MSR_DCTS  0x01              // delta clear to send
#define SIO_MSR_DDSR  0x02              // delta data set ready
#define SIO_MSR_TERI  0x04              // trailing edge ring indicator
#define SIO_MSR_DDCD  0x08              // delta data carrier detect
#define SIO_MSR_CTS   0x10              // clear to send
#define SIO_MSR_DSR   0x20              // data set ready
#define SIO_MSR_RI    0x40              // ring indicator
#define SIO_MSR_DCD   0x80              // data carrier detect

// The line control register bits.
#define SIO_LCR_WLS0   0x01             // word length select bit 0
#define SIO_LCR_WLS1   0x02             // word length select bit 1
#define SIO_LCR_STB    0x04             // number of stop bits
#define SIO_LCR_PEN    0x08             // parity enable
#define SIO_LCR_EPS    0x10             // even parity select
#define SIO_LCR_SP     0x20             // stick parity
#define SIO_LCR_SB     0x40             // set break
#define SIO_LCR_DLAB   0x80             // divisor latch access bit

// The FIFO control register
#define SIO_FCR_FCR0   0x01             // enable xmit and rcvr fifos
#define SIO_FCR_FCR1   0x02             // clear RCVR FIFO
#define SIO_FCR_FCR2   0x04             // clear XMIT FIFO

/////////////////////////////////////////
// Interrupt Enable Register
#define IER_RCV 0x01
#define IER_XMT 0x02
#define IER_LS  0x04
#define IER_MS  0x08

// Line Control Register
#define LCR_WL5 0x00    // Word length
#define LCR_WL6 0x01
#define LCR_WL7 0x02
#define LCR_WL8 0x03
#define LCR_SB1 0x00    // Number of stop bits
#define LCR_SB1_5 0x04  // 1.5 -> only valid with 5 bit words
#define LCR_SB2 0x04
#define LCR_PN  0x00    // Parity mode - none
#define LCR_PE  0x0C    // Parity mode - even
#define LCR_PO  0x08    // Parity mode - odd
#define LCR_PM  0x28    // Forced "mark" parity
#define LCR_PS  0x38    // Forced "space" parity
#define LCR_DL  0x80    // Enable baud rate latch

// Line Status Register
#define LSR_RSR 0x01
#define LSR_THE 0x20

// Modem Control Register
#define MCR_DTR 0x01
#define MCR_RTS 0x02
#define MCR_INT 0x08   // Enable interrupts
#define MCR_AFE 0x20

// Interrupt status register
#define ISR_None             0x01
#define ISR_Rx_Line_Status   0x06
#define ISR_Rx_Avail         0x04
#define ISR_Rx_Char_Timeout  0x0C
#define ISR_Tx_Empty         0x02
#define IRS_Modem_Status     0x00

// FIFO control register
#define FCR_ENABLE     0x01
#define FCR_CLEAR_RCVR 0x02
#define FCR_CLEAR_XMIT 0x04

//-----------------------------------------------------------------------------
typedef struct {
    unsigned char* base;
    unsigned int msec_timeout;
    int isr_vector;
} channel_data_t;

static channel_data_t channels[1] = {
    { (unsigned char*)CFG_NS16550_COM1, 1000, 0},
};


unsigned int
hal_ar7100_sys_frequency(void)
{
    u32 ar7100_cpu_freq, pll, pll_div, cpu_div, ahb_div, freq;
    static u32 ar7100_ahb_freq=0;
      
    if (ar7100_ahb_freq)
        return ar7100_ahb_freq;

    pll = ar7100_reg_rd(AR7100_PLL_CONFIG);

    pll_div  = ((pll >> PLL_DIV_SHIFT) & PLL_DIV_MASK) + 1;
    freq     = pll_div * 40000000;
    cpu_div  = ((pll >> CPU_DIV_SHIFT) & CPU_DIV_MASK) + 1;
    ahb_div  = (((pll >> AHB_DIV_SHIFT) & AHB_DIV_MASK) + 1)*2;

    ar7100_cpu_freq = freq/cpu_div;
    ar7100_ahb_freq = ar7100_cpu_freq/ahb_div;

    return ar7100_ahb_freq;
}


//-----------------------------------------------------------------------------
// Set the baud rate

static void
cyg_hal_plf_serial_set_baud(unsigned char * port, unsigned short baud_divisor)
{
    unsigned char _lcr;

    HAL_READ_UINT32(port+SER_16550_LCR, _lcr);
    _lcr |= LCR_DL;
    HAL_WRITE_UINT32(port+SER_16550_LCR, _lcr);

    HAL_WRITE_UINT32(port+SER_16550_DLM,(baud_divisor >> 8) & 0xff);
    HAL_WRITE_UINT32(port+SER_16550_DLL, baud_divisor & 0xff);

    _lcr &= ~LCR_DL;
    HAL_WRITE_UINT32(port+SER_16550_LCR, _lcr);
}

//-----------------------------------------------------------------------------
// The minimal init, get and put functions. All by polling.

void
cyg_hal_plf_serial_init_channel(void* __ch_data)
{
    unsigned char * port;
    unsigned char _lcr;

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
    if (__ch_data == 0)
      __ch_data = (void*)&channels[0];

    port = ((channel_data_t*)__ch_data)->base;

    /*
     * undocumented. confirm, why write to GPIO for uart?
     */
    ar7100_reg_wr(AR7100_GPIO_OE, 0xcff);
    ar7100_reg_wr(AR7100_GPIO_OUT, 0x3b);

    HAL_WRITE_UINT32(0xb8040028, 0x100);

    // Disable port interrupts while changing hardware
    HAL_WRITE_UINT32(port+SER_16550_IER, 0);

    // Set databits, stopbits and parity.
    _lcr = LCR_WL8 | LCR_SB1 | LCR_PN;
    HAL_WRITE_UINT32(port+SER_16550_LCR, _lcr);

    // Set baud rate.
    //cyg_hal_plf_serial_set_baud(port, hal_ar7100_sys_frequency()/ (16 *BAUDRATE));
  cyg_hal_plf_serial_set_baud(port, CFG_NS16550_CLK/ (16 *BAUDRATE));

    // Enable and clear FIFO
    HAL_WRITE_UINT32(port+SER_16550_FCR, (FCR_ENABLE | FCR_CLEAR_RCVR | FCR_CLEAR_XMIT));

#ifdef NOTANYMORE
    // enable RTS to keep host side happy. Also allow interrupts
    HAL_WRITE_UINT32( port+SER_16550_MCR, MCR_DTR | MCR_RTS | MCR_INT);
#endif

    // Don't allow interrupts.
    HAL_WRITE_UINT32(port+SER_16550_IER, 0);
}

static int
cyg_hal_plf_serial_getc_nonblock(void* port, unsigned char* ch)
{
     unsigned char _lsr;

    HAL_READ_UINT32(port+SER_16550_LSR, _lsr);
    if ((_lsr & SIO_LSR_DR) == 0)
        return 0;

    HAL_READ_UINT32(port+SER_16550_RBR, *ch);

    return 1;
}

static int
cyg_hal_plf_serial_rx_test(void* port)
{
     unsigned char _lsr;

    HAL_READ_UINT32(port+SER_16550_LSR, _lsr);
    if ((_lsr & SIO_LSR_DR) == 0)
        return 0;

    return 1;
}


void
cyg_hal_plf_serial_putc(void* port, unsigned char __ch)
{
    unsigned char _lsr;

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
 
    CYGARC_HAL_SAVE_GP();

    do {
        HAL_READ_UINT32(port+SER_16550_LSR, _lsr);
    } while ((_lsr & SIO_LSR_THRE) == 0);

    // Now, the transmit buffer is empty
    HAL_WRITE_UINT32(port+SER_16550_THR, __ch);
    CYGARC_HAL_RESTORE_GP();
}

unsigned char
cyg_hal_plf_serial_getc(void* port)
{
     unsigned char ch;
    CYGARC_HAL_SAVE_GP();

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
   
    while(!cyg_hal_plf_serial_getc_nonblock(port, &ch));

    CYGARC_HAL_RESTORE_GP();
    return ch;
}

static void
cyg_hal_plf_serial_init(void)
{
    cyg_hal_plf_serial_init_channel((void*)&channels[0]);
}

 int
cyg_hal_plf_comms_init(void)
{
    static int initialized = 0;

    if (initialized)
        return 0;

    initialized = 1;

    cyg_hal_plf_serial_init();
	return 0;
}

//-----------------------------------------------------------------------------
// end of ser16c550c.c

int serial_init (void)
{
	cyg_hal_plf_serial_init();
	return (0);
}

void
_serial_putc(const char c,const int port)
{
	unsigned char *p ;
	p = ( unsigned char * ) CFG_NS16550_COM1;
	if (c == '\n')
		cyg_hal_plf_serial_putc(p,  '\r');

	cyg_hal_plf_serial_putc(p,  c);
}

void
_serial_putc_raw(const char c,const int port)
{

}

void
_serial_puts (const char *s,const int port)
{
	while (*s) {
		_serial_putc (*s++,port);
	}
}


int
_serial_getc(const int port)
{	
	unsigned char *p;
	p = ( unsigned char * )  CFG_NS16550_COM1;
	return cyg_hal_plf_serial_getc(p);
}

int
_serial_tstc(const int port)
{
	return 0;
}

void
_serial_setbrg (const int port)
{

}


void
serial_putc(const char c)
{
	_serial_putc(c,CONFIG_CONS_INDEX);
}

void
serial_putc_raw(const char c)
{

}

void
serial_puts(const char *s)
{
	_serial_puts(s,CONFIG_CONS_INDEX);
}

int
serial_getc(void)
{
	return _serial_getc(CONFIG_CONS_INDEX);
}

int
serial_tstc(void)
{
	unsigned char *p;
	p = ( unsigned char * )  CFG_NS16550_COM1;
	return cyg_hal_plf_serial_rx_test(p);	
}

void
serial_setbrg(void)
{
	
}



