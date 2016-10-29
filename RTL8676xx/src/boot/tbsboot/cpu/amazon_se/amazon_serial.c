/*
 * COM1 NS16550 support
 * originally from linux source (arch/ppc/boot/ns16550.c)
 * modified to use CFG_ISA_MEM and new defines
 */

#include <config.h>
#include <common.h>
#include "amazon_se_reg.h"
#include "amazon_serial.h"

#define ASC_FIFO_PRESENT
#define SET_BIT(reg, mask)                  reg |= (mask)
#define CLEAR_BIT(reg, mask)                reg &= (~mask)
#define CLEAR_BITS(reg, mask)               CLEAR_BIT(reg, mask)
#define SET_BITS(reg, mask)                 SET_BIT(reg, mask)
#define SET_BITFIELD(reg, mask, off, val)   {reg &= (~mask); reg |= (val << off);}

static volatile DanubeAsc_t *pAsc = (DanubeAsc_t *)AMAZON_SE_ASC1;

typedef struct{
  u16 fdv; /* 0~511 fractional divider value*/
  u16 reload; /* 13 bit reload value*/
} ifx_asc_baud_reg_t;


/******************************************************************************
*
* asc_init - initialize a Danube ASC channel
*
* This routine initializes the number of data bits, parity
* and set the selected baud rate. Interrupts are disabled.
* Set the modem control signals if the option is selected.
*
* RETURNS: N/A
*/

int serial_init (void)
{
	/* and we have to set CLC register*/
	CLEAR_BIT(pAsc->asc_clc, ASCCLC_DISS);
	SET_BITFIELD(pAsc->asc_clc, ASCCLC_RMCMASK, ASCCLC_RMCOFFSET, 0x0001);

	/* initialy we are in async mode */
	pAsc->asc_con = ASCCON_M_8ASYNC;

	/* select input port */
	pAsc->asc_pisel = (CONSOLE_TTY & 0x1);

	/* TXFIFO's filling level */
	SET_BITFIELD(pAsc->asc_txfcon, ASCTXFCON_TXFITLMASK,
			ASCTXFCON_TXFITLOFF, DANUBEASC_TXFIFO_FL);
	/* enable TXFIFO */
	SET_BIT(pAsc->asc_txfcon, ASCTXFCON_TXFEN);

	/* RXFIFO's filling level */
	SET_BITFIELD(pAsc->asc_rxfcon, ASCRXFCON_RXFITLMASK,
			ASCRXFCON_RXFITLOFF, DANUBEASC_RXFIFO_FL);
	/* enable RXFIFO */
	SET_BIT(pAsc->asc_rxfcon, ASCRXFCON_RXFEN);

	/* set baud rate */
	serial_setbrg();

	/* enable error signals &  Receiver enable  */
	SET_BIT(pAsc->asc_whbstate, ASCWHBSTATE_SETREN);

	return 0;
}

void serial_setbrg (void)
{
	u32 uiReloadValue, fdv;

#if defined(ON_IKOS)
	/*1200 @77K */
	//fdv=472;
	//uiReloadValue=5;
	
	/* 2400bps @76800Hz */
	fdv=433;
	uiReloadValue=1;
#else
	/*venus & tapeout */
   
        /* 115200 @133MHz */
	fdv=453;
	uiReloadValue=63;

#endif	//ON_IKOS

	/* Disable Baud Rate Generator; BG should only be written when R=0 */
	CLEAR_BIT(pAsc->asc_con, ASCCON_R);

	/* Enable Fractional Divider */
	SET_BIT(pAsc->asc_con, ASCCON_FDE); /* FDE = 1 */

	/* Set fractional divider value */
	pAsc->asc_fdv = fdv & ASCFDV_VALUE_MASK;

	/* Set reload value in BG */
	pAsc->asc_bg = uiReloadValue;

	/* Enable Baud Rate Generator */
	SET_BIT(pAsc->asc_con, ASCCON_R);           /* R = 1 */
}


void serial_putc (const char c)
{
	u32 txFl = 0;
	if (c == '\n')
		serial_putc ('\r');
	/* check do we have a free space in the TX FIFO */
	/* get current filling level */
	do
	{
		txFl = ( pAsc->asc_fstat & ASCFSTAT_TXFFLMASK ) >> ASCFSTAT_TXFFLOFF;
	}
	while ( txFl == DANUBEASC_TXFIFO_FULL );

	pAsc->asc_tbuf = c; /* write char to Transmit Buffer Register */

	/* check for errors */
	if ( pAsc->asc_state & ASCSTATE_TOE )
	{
		SET_BIT(pAsc->asc_whbstate, ASCWHBSTATE_CLRTOE);
		return;
	}
}

void serial_puts (const char *s)
{
	while (*s)
	{
		serial_putc (*s++);
	}
}

int serial_getc (void)
{
	char c;

	if( serial_load == 0 )
	{
		while ((pAsc->asc_fstat & ASCFSTAT_RXFFLMASK) == 0 )
		{
			return 0;
		}
	}
	else
	{
		while ((pAsc->asc_fstat & ASCFSTAT_RXFFLMASK) == 0 );
	}
	
	c = (char)(pAsc->asc_rbuf & 0xff);

	return c;
}

void asc_clear_input(void)
{
	volatile char c;
	volatile u32 cnt = 0xFFFF;

    while (cnt)    
    {
        cnt = 0xFFFF;
	
	while ((pAsc->asc_fstat & ASCFSTAT_RXFFLMASK) == 0 )
	{
	    cnt--;
	    if (cnt == 0)
	    {
	        break;
	    }
	}

        if (cnt)
 	    c = (char)(pAsc->asc_rbuf & 0xff);
    }
}

int serial_tstc (void)
{
         int res = 1;
#ifdef ASC_FIFO_PRESENT
    if ( (pAsc->asc_fstat & ASCFSTAT_RXFFLMASK) == 0 )
    {
        res = 0;
    }
#else
    if (!(*(volatile unsigned long*)(SFPI_INTCON_BASEADDR + FBS_ISR) &
			    					FBS_ISR_AR))

    {
        res = 0;
    }
#endif
	 return res;
}

int serial_start(void)
{
   return 1;
}

int serial_stop(void)
{
   return 1;
}

void
_serial_putc_raw(const char c,const int port)
{

}

void
_serial_setbrg (const int port)
{

}

void
serial_putc_raw(const char c)
{

}


