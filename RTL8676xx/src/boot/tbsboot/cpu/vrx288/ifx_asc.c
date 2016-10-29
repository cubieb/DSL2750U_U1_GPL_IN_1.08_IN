/*****************************************************************************
 * DANUBE BootROM
 * Copyright (c) 2005, Infineon Technologies AG, All rights reserved
 * IFAP DC COM SD
 *****************************************************************************/
#include <common.h>
#include <config.h>
#include <types.h>
#include <asm/addrspace.h>
#include "../../bootstart/cpu/vrx288/ifx_asc.h"
#include "../../bootstart/cpu/vrx288/vr9.h"

#define ASC_FIFO_PRESENT
#define SET_BIT(reg, mask)                  reg |= (mask)
#define CLEAR_BIT(reg, mask)                reg &= (~mask)
#define CLEAR_BITS(reg, mask)               CLEAR_BIT(reg, mask)
#define SET_BITS(reg, mask)                 SET_BIT(reg, mask)
#define SET_BITFIELD(reg, mask, off, val)   {reg &= (~mask); reg |= (val << off);}

void serial_setbrg (void);

/*TODO: undefine this !!!*/
#undef DEBUG_ASC_RAW
#ifdef DEBUG_ASC_RAW
#define DEBUG_ASC_RAW_RX_BUF		0xA0800000
#define DEBUG_ASC_RAW_TX_BUF		0xA0900000
#endif

static volatile IFX_Asc_t *pAsc = (IFX_Asc_t *)VR9_ASC1;

typedef struct{
  u32 fdv; /* 0~511 fractional divider value*/
  u32 reload; /* 13 bit reload value*/
} ifx_asc_baud_reg_t;



u8 compute_cgu_pos(void)
{
    u32 if_clk = (*BSP_IF_CLK );

    return (u8)((if_clk >> 25) & 0x0F); // cgu_pos = if_clk[28:25]
        
}

const ifx_asc_baud_reg_t g_danube_asc_baud[] = 
{
    // following settings are generated using asc_fdv_reload.c
    // gcc asc_fdv_reload.c -o asc.exe

    //FPI_CLK = 62500000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.856250
    { 151, 9 },

    //FPI_CLK = 62500000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.856250
    { 151, 9 },

    //FPI_CLK = 83500000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.146802
    { 486, 42 },

    //FPI_CLK = 125000000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.856250
    { 151, 19 },

    //FPI_CLK = 125000000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.856250
    { 151, 19 },

    //FPI_CLK = 125000000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.856250
    { 151, 19 },

    //FPI_CLK = 167000000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.146802
    { 243, 42 },

    //FPI_CLK = 200000000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.549757
    { 486, 102 },

    //FPI_CLK = 250000000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.016329
    { 419, 110 },

    //FPI_CLK = 300000000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 1.195364
    { 475, 150 },

    //FPI_CLK = 62500000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.856250
    { 151, 9 },

    //FPI_CLK = 98300000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 4.687500
    { 48, 4 },

    //FPI_CLK = 150000000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.856771
    { 151, 23 },

    //FPI_CLK = 196600000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 4.687500
    { 24, 4 },

    //FPI_CLK = 62500000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.856250
    { 151, 9 },

    //FPI_CLK = 62500000
    //TARGET_BAUDRATE = 115200
    //min_deviation = 3.856250
    { 151, 9 },

};

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
	SET_BITFIELD(pAsc->asc_txfcon, ASCRXFCON_RXFITLMASK,
			ASCRXFCON_RXFITLOFF, DANUBEASC_RXFIFO_FL);
	/* enable RXFIFO */
	SET_BIT(pAsc->asc_rxfcon, ASCRXFCON_RXFEN);

	/* set baud rate */
	serial_setbrg();

	/* enable error signals &  Receiver enable  */
	SET_BIT(pAsc->asc_whbstate, ASCWHBSTATE_SETREN|ASCCON_FEN|ASCCON_TOEN|ASCCON_ROEN);

	return 0;
}

void serial_setbrg (void)
{
	u32 uiReloadValue, fdv;
//	u32 f_ASC;
	u8 cgu_pos;

	//f_ASC = amazon_s_get_fpiclk();
#ifdef CONFIG_ON_VENUS
     /*use for Venus testing --- to be remove for realchip */

	 fdv = 419;
	 uiReloadValue=1;
			 
#elif CONFIG_ON_PALLADIUM
   fdv = 345;
	 uiReloadValue = 0;

#else
     cgu_pos = compute_cgu_pos();
   
     fdv = g_danube_asc_baud[cgu_pos].fdv;
	   uiReloadValue = g_danube_asc_baud[cgu_pos].reload;
	
	 
#endif
	/* Disable Baud Rate Generator; BG should only be written when R=0 */
	CLEAR_BIT(pAsc->asc_con, ASCCON_R);

	/* Enable Fractional Divider */
	SET_BIT(pAsc->asc_con, ASCCON_FDE); /* FDE = 1 */

	/* Set fractional divider value */
	//pAsc->asc_fdv = fdv & ASCFDV_VALUE_MASK;
    pAsc->asc_fdv = fdv & 0x000001ff;
	
	/* Set reload value in BG */
	pAsc->asc_bg = uiReloadValue & 0x00001fff;

	/* Enable Baud Rate Generator */
	SET_BIT(pAsc->asc_con, ASCCON_R);           /* R = 1 */
}


void serial_putc (const char c)
{
	u32 txFl = 0;
#ifdef DEBUG_ASC_RAW
	static u8 * debug = (u8 *) DEBUG_ASC_RAW_TX_BUF;
	*debug++=c;
#endif
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

int asc_inb(int timeout)
{
	u32 symbol_mask;
	char c;
	while ((pAsc->asc_fstat & ASCFSTAT_RXFFLMASK) == 0 ) {
	}
	symbol_mask = ((ASC_OPTIONS & ASCOPT_CSIZE) == ASCOPT_CS7) ? (0x7f) : (0xff);
	c = (char)(pAsc->asc_rbuf & symbol_mask);
	return (c);
}

int serial_getc (void)
{
	char c;
	while ((pAsc->asc_fstat & ASCFSTAT_RXFFLMASK) == 0 );
	c = (char)(pAsc->asc_rbuf & 0xff);

#ifdef 	DEBUG_ASC_RAW
	static u8* debug=(u8*)(DEBUG_ASC_RAW_RX_BUF);
	*debug++=c;
#endif
	return c;
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
#if 0
    else if ( pAsc->asc_con & ASCCON_FE )
    {
        SET_BIT(pAsc->asc_whbcon, ASCWHBCON_CLRFE);
        res = 0;
    }
    else if ( pAsc->asc_con & ASCCON_PE )
    {
        SET_BIT(pAsc->asc_whbcon, ASCWHBCON_CLRPE);
        res = 0;
    }
    else if ( pAsc->asc_con & ASCCON_OE )
    {
        SET_BIT(pAsc->asc_whbcon, ASCWHBCON_CLROE);
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
