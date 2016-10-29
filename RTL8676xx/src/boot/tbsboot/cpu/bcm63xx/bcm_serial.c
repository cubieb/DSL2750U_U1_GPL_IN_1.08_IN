/*
 * COM1 NS16550 support
 * originally from linux source (arch/ppc/boot/ns16550.c)
 * modified to use CFG_ISA_MEM and new defines
 */

#include <config.h>
#include <common.h>

#ifdef BCM63XX_CHIP
#if CONFIG_BCM6338
#define CFG_NS16550_CLK		 18432000
#define CFG_NS16550_COM1	 0xfffe0300

struct UartChannel {
  unsigned char         unused0;
  unsigned char         control;
#define BRGEN           0x80    /* Control register bit defs */
#define TXEN            0x40
#define RXEN            0x20
#define LOOPBK          0x10
#define TXPARITYEN      0x08
#define TXPARITYEVEN    0x04
#define RXPARITYEN      0x02
#define RXPARITYEVEN    0x01

  unsigned char         config;
#define XMITBREAK       0x40
#define BITS5SYM        0x00
#define BITS6SYM        0x10
#define BITS7SYM        0x20
#define BITS8SYM        0x30
#define ONESTOP         0x07
#define TWOSTOP         0x0f
  /* 4-LSBS represent STOP bits/char
   * in 1/8 bit-time intervals.  Zero
   * represents 1/8 stop bit interval.
   * Fifteen represents 2 stop bits.
   */
  unsigned char         fifoctl;
#define RSTTXFIFOS      0x80
#define RSTRXFIFOS      0x40
  /* 5-bit TimeoutCnt is in low bits of this register.
   *  This count represents the number of characters 
   *  idle times before setting receive Irq when below threshold
   */
  unsigned int        baudword;
  /* When divide SysClk/2/(1+baudword) we should get 32*bit-rate
   */

  unsigned char          txf_levl;       /* Read-only fifo depth */
  unsigned char          rxf_levl;       /* Read-only fifo depth */
  unsigned char          fifocfg;        /* Upper 4-bits are TxThresh, Lower are
                                 *      RxThreshold.  Irq can be asserted
                                 *      when rx fifo> thresh, txfifo<thresh
                                 */
  unsigned char          prog_out;       /* Set value of DTR (Bit0), RTS (Bit1)
                                 *  if these bits are also enabled to GPIO_o
                                 */
#define DTREN   0x01
#define RTSEN   0x02

  unsigned char          unused1;
  unsigned char          DeltaIPEdgeNoSense;     /* Low 4-bits, set corr bit to 1 to 
                                         * detect irq on rising AND falling 
                                         * edges for corresponding GPIO_i
                                         * if enabled (edge insensitive)
                                         */

  unsigned char          DeltaIPConfig_Mask;     /* Upper 4 bits: 1 for posedge sense
                                         *      0 for negedge sense if
                                         *      not configured for edge
                                         *      insensitive (see above)
                                         * Lower 4 bits: Mask to enable change
                                         *  detection IRQ for corresponding
                                         *  GPIO_i
                                         */
  unsigned char          DeltaIP_SyncIP;         /* Upper 4 bits show which bits
                                         *  have changed (may set IRQ).
                                         *  read automatically clears bit
                                         * Lower 4 bits are actual status
                                         */

  unsigned short        intMask;                                /* Same Bit defs for Mask and status */
  unsigned short        intStatus;
#define DELTAIP         0x0001
#define TXUNDERR        0x0002
#define TXOVFERR        0x0004
#define TXFIFOTHOLD     0x0008
#define TXREADLATCH     0x0010
#define TXFIFOEMT       0x0020
#define RXUNDERR        0x0040
#define RXOVFERR        0x0080
#define RXTIMEOUT       0x0100
#define RXFIFOFULL      0x0200
#define RXFIFOTHOLD     0x0400
#define RXFIFONE        0x0800
#define RXFRAMERR       0x1000
#define RXPARERR        0x2000
#define RXBRK           0x4000

  unsigned short        unused2;
  unsigned char		RxErr;
  unsigned char	        Data;                   /* Write to TX, Read from RX */
                                        /* bits 11:8 are BRK,PAR,FRM errors */

  unsigned int                unused3;
  unsigned int                unused4;
} __attribute__ ((packed));

#endif
#if CONFIG_BCM6358
#define CFG_NS16550_CLK		 18432000
#define CFG_NS16550_COM1	 0xfffe0100

typedef struct UartChannel {
  unsigned char          unused0;
  unsigned char           control;
#define BRGEN           0x80    /* Control register bit defs */
#define TXEN            0x40
#define RXEN            0x20
#define LOOPBK          0x10
#define TXPARITYEN      0x08
#define TXPARITYEVEN    0x04
#define RXPARITYEN      0x02
#define RXPARITYEVEN    0x01

  unsigned char           config;
#define XMITBREAK       0x40
#define BITS5SYM        0x00
#define BITS6SYM        0x10
#define BITS7SYM        0x20
#define BITS8SYM        0x30
#define ONESTOP         0x07
#define TWOSTOP         0x0f
  /* 4-LSBS represent STOP bits/char
   * in 1/8 bit-time intervals.  Zero
   * represents 1/8 stop bit interval.
   * Fifteen represents 2 stop bits.
   */
  unsigned char           fifoctl;
#define RSTTXFIFOS      0x80
#define RSTRXFIFOS      0x40
  /* 5-bit TimeoutCnt is in low bits of this register.
   *  This count represents the number of characters 
   *  idle times before setting receive Irq when below threshold
   */
  unsigned int        baudword;
  /* When divide SysClk/2/(1+baudword) we should get 32*bit-rate
   */

  unsigned char           txf_levl;       /* Read-only fifo depth */
  unsigned char           rxf_levl;       /* Read-only fifo depth */
  unsigned char           fifocfg;        /* Upper 4-bits are TxThresh, Lower are
                                 *      RxThreshold.  Irq can be asserted
                                 *      when rx fifo> thresh, txfifo<thresh
                                 */
  unsigned char           prog_out;       /* Set value of DTR (Bit0), RTS (Bit1)
                                 *  if these bits are also enabled to GPIO_o
                                 */
#define	DTREN	0x01
#define	RTSEN	0x02

  unsigned char           unused1;
  unsigned char           DeltaIPEdgeNoSense;     /* Low 4-bits, set corr bit to 1 to 
                                         * detect irq on rising AND falling 
                                         * edges for corresponding GPIO_i
                                         * if enabled (edge insensitive)
                                         */
  unsigned char           DeltaIPConfig_Mask;     /* Upper 4 bits: 1 for posedge sense
                                         *      0 for negedge sense if
                                         *      not configured for edge
                                         *      insensitive (see above)
                                         * Lower 4 bits: Mask to enable change
                                         *  detection IRQ for corresponding
                                         *  GPIO_i
                                         */
  unsigned char           DeltaIP_SyncIP;         /* Upper 4 bits show which bits
                                         *  have changed (may set IRQ).
                                         *  read automatically clears bit
                                         * Lower 4 bits are actual status
                                         */

  unsigned short        intMask;				/* Same Bit defs for Mask and status */
  unsigned short        intStatus;
#define DELTAIP         0x0001
#define TXUNDERR        0x0002
#define TXOVFERR        0x0004
#define TXFIFOTHOLD     0x0008
#define TXREADLATCH     0x0010
#define TXFIFOEMT       0x0020
#define RXUNDERR        0x0040
#define RXOVFERR        0x0080
#define RXTIMEOUT       0x0100
#define RXFIFOFULL      0x0200
#define RXFIFOTHOLD     0x0400
#define RXFIFONE        0x0800
#define RXFRAMERR       0x1000
#define RXPARERR        0x2000
#define RXBRK           0x4000

  unsigned short        unused2;
  unsigned short        Data;                   /* Write to TX, Read from RX */
                                        /* bits 11:8 are BRK,PAR,FRM errors */

  unsigned int		unused3;
  unsigned int		unused4;
} Uart;
#endif




typedef volatile struct UartChannel *NS16550_t;


void NS16550_init (NS16550_t com_port, int baud_divisor)
{
	com_port->baudword = 13;
	com_port->control = BRGEN | TXEN | RXEN;
	com_port->config = BITS8SYM | ONESTOP;
	com_port->fifoctl = RSTTXFIFOS | RSTRXFIFOS;
	com_port->intMask  = 0;
}

void NS16550_reinit (NS16550_t com_port, int baud_divisor)
{
	NS16550_init (com_port, baud_divisor);
}


void NS16550_putc (NS16550_t com_port, char c)
{
	unsigned int status;

	do {
		status = com_port->intStatus & TXFIFOEMT;
	} while( !status );

	com_port->Data = c;
}

char NS16550_getc (NS16550_t com_port)
{
	unsigned int status;
	do {
                status = com_port->intStatus & RXFIFONE;
        } while( !status );	

	return (com_port->Data);
}

int NS16550_tstc (NS16550_t com_port)
{
	return ((com_port->intStatus & RXFIFONE) != 0);
}



DECLARE_GLOBAL_DATA_PTR;

static NS16550_t serial_ports[1] = {

	(NS16550_t)CFG_NS16550_COM1,

};

#define PORT	serial_ports[port-1]
/*
static int calc_divisor (NS16550_t port)
{
#define MODE_X_DIV 16

	return (CFG_NS16550_CLK / MODE_X_DIV / gd->baudrate);

}
*/

int serial_init (void)
{
//	int clock_divisor;

//	clock_divisor = calc_divisor(serial_ports[0]);
	NS16550_init(serial_ports[0], 0);//clock_divisor);
	return (0);
}


void
_serial_putc(const char c,const int port)
{
	if (c == '\n')
		NS16550_putc(PORT, '\r');

	NS16550_putc(PORT, c);
}

void
_serial_putc_raw(const char c,const int port)
{
	NS16550_putc(PORT, c);
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
	return NS16550_getc(PORT);
}

int
_serial_tstc(const int port)
{
	return NS16550_tstc(PORT);
}

void
_serial_setbrg (const int port)
{
//	int clock_divisor;

//	clock_divisor = calc_divisor(PORT);
	NS16550_reinit(PORT,0);// clock_divisor);
}


void
serial_putc(const char c)
{
	_serial_putc(c,CONFIG_CONS_INDEX);
}

void
serial_putc_raw(const char c)
{
	_serial_putc_raw(c,CONFIG_CONS_INDEX);
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
	return _serial_tstc(CONFIG_CONS_INDEX);
}

void
serial_setbrg(void)
{
	_serial_setbrg(CONFIG_CONS_INDEX);
}


#endif
