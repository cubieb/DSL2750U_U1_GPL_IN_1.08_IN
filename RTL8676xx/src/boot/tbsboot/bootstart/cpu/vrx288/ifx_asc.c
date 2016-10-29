/*****************************************************************************
 * DANUBE BootROM
 * Copyright (c) 2005, Infineon Technologies AG, All rights reserved
 * IFAP DC COM SD
 *****************************************************************************/

#include <config.h>
#include "vr9.h"
#include <asm/addrspace.h>
#include "ifx_asc.h"


#define ASC_FIFO_PRESENT
#define SET_BIT(reg, mask)                  reg |= (mask)
#define CLEAR_BIT(reg, mask)                reg &= (~mask)
#define CLEAR_BITS(reg, mask)               CLEAR_BIT(reg, mask)
#define SET_BITS(reg, mask)                 SET_BIT(reg, mask)
#define SET_BITFIELD(reg, mask, off, val)   {reg &= (~mask); reg |= (val << off);}


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed   long s32;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef volatile unsigned short vuint;


/*TODO: undefine this !!!*/
#undef DEBUG_ASC_RAW
#ifdef DEBUG_ASC_RAW
#define DEBUG_ASC_RAW_RX_BUF		0xA0800000
#define DEBUG_ASC_RAW_TX_BUF		0xA0900000
#endif



typedef struct{
  u32 fdv; /* 0~511 fractional divider value*/
  u32 reload; /* 13 bit reload value*/
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
	u32 uiReloadValue, fdv;
	u8 cgu_pos;
	volatile IFX_Asc_t *pAsc = (IFX_Asc_t *)VR9_ASC1;
	
	const ifx_asc_baud_reg_t g_danube_asc_baud[] = {
    { 151, 9 },
    { 151, 9 },
    { 486, 42 },
    { 151, 19 },
    { 151, 19 },
    { 151, 19 },
    { 243, 42 },
    { 486, 102 },
    { 419, 110 },
    { 475, 150 },
    { 151, 9 },
    { 48, 4 },
    { 151, 23 },
    { 24, 4 },
    { 151, 9 },
    { 151, 9 },
};

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

	//f_ASC = amazon_s_get_fpiclk();
#ifdef CONFIG_ON_VENUS
     /*use for Venus testing --- to be remove for realchip */

	 fdv = 419;
	 uiReloadValue=1;
			 
#elif CONFIG_ON_PALLADIUM
   fdv = 345;
	 uiReloadValue = 0;

#else

     cgu_pos = (u8)(((u32)(*BSP_IF_CLK ) >> 25) & 0x0F);
   
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



	/* enable error signals &  Receiver enable  */
	SET_BIT(pAsc->asc_whbstate, ASCWHBSTATE_SETREN|ASCCON_FEN|ASCCON_TOEN|ASCCON_ROEN);

	return 0;
}


void serial_putc (const char c)
{
	u32 txFl = 0;
	volatile IFX_Asc_t *pAsc = (IFX_Asc_t *)VR9_ASC1;
	
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
#if 0 //by xiachaoren: not used
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
    if ( (pAsc->asc_fstat & ASCFSTAT_RXFFLMASK) == 0 )
    {
        res = 0;
    }

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
#endif
