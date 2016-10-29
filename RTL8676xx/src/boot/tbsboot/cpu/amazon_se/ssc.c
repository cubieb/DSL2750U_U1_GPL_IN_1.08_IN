/*****************************************************************************
 * AMAZON_SE BootROM
 * Copyright (c) 2005, Infineon Technologies AG, All rights reserved
 * IFAP DC COM SD
 *****************************************************************************/

#include <asm/lib.h>
#include <asm/addrspace.h>
#include "ssc.h"
#include "amazon_se_reg.h"
//#include <asc.h>

extern spi_dev_t spi_sflash;

/* report bug! */
#undef SSC_RX_INT
void SSC1_CS_SET(u8 bit)
{
	*AMAZON_SE_SSC1_WHBGPOSTAT=(1<<(bit+8));
}
void SSC1_CS_CLEAR(u8 bit)
{
	*AMAZON_SE_SSC1_WHBGPOSTAT=(1<<(bit));
}

/***********************************************************************/
/* SPI bus common routines */
/***********************************************************************/

/* Brief: initialize SSC1 module
 * Parameter:	baudrate FPI/clock ~ FPI/clock/2^16
 */
int ssc_init(u32 baudrate)
{
	/* enable SSC1 */
	//*AMAZON_SE_PMU_PM_GEN |= AMAZON_SE_PMU_PM_GEN_EN11;

#if 0 // yclee for testing sflash
	/* P0.7 SPI_CS1, P0.8 SPI_DIN, P0.9 SPI_DOUT, P0.10 SPI_CLK, P0.14 SPI_CS3, P0.15 SPI_CS2 */
	*(AMAZON_SE_GPIO_P0_DIR) = (((*AMAZON_SE_GPIO_P0_DIR)|(0x8780)) & (~(0x0100)));
	*(AMAZON_SE_GPIO_P0_ALTSEL0) = (((*AMAZON_SE_GPIO_P0_ALTSEL0)|(0x8780)) & (~(0x8000)));
	*(AMAZON_SE_GPIO_P0_ALTSEL1) = (((*AMAZON_SE_GPIO_P0_ALTSEL1)|(0x8780)) & (~(0x0780)));
	*(AMAZON_SE_GPIO_P0_OD) = (*AMAZON_SE_GPIO_P0_OD)|(0x8680);
#else
	/* SSC1 Ports */
	/* P0.7 SPI_CS1, P0.8 SPI_DIN, P0.9 SPI_DOUT, P0.10 SPI_CLK */
	*(AMAZON_SE_GPIO_P0_DIR) = (((*AMAZON_SE_GPIO_P0_DIR)|(0x0780)) & (~(0x0100)));
	*(AMAZON_SE_GPIO_P0_ALTSEL0) = (((*AMAZON_SE_GPIO_P0_ALTSEL0)|(0x0780)) & (~(0x0000)));
	*(AMAZON_SE_GPIO_P0_ALTSEL1) = (((*AMAZON_SE_GPIO_P0_ALTSEL1)|(0x0780)) & (~(0x0780)));
	*(AMAZON_SE_GPIO_P0_OD) = (*AMAZON_SE_GPIO_P0_OD)|(0x0680);
#endif

	/* Clock Control Register */
	/* DISS OFF and RMC = 1 */
	//CLC
	//*AMAZON_SE_SSC1_CLC = 0x0100; //(in Danube)
	
	// in AmazonSE, fpi_clk=133MHz and too big for 16-bit BR 
	// change RMC to 4
	*AMAZON_SE_SSC1_CLC = 0x0400;   
	
	/* Disable SSC to get access to the control bits */
	*AMAZON_SE_SSC1_WHBSTATE = 0x1;
	asm("SYNC");

#if 0 // yclee for testing sflash
	/* CS1 CS2 */
	*AMAZON_SE_SSC1_GPOCON = 0x00000300;
	SSC1_CS_SET(0);
	SSC1_CS_SET(1);
#else
	/* CS1 */
	*AMAZON_SE_SSC1_GPOCON = 0x00000100;
	SSC1_CS_SET(0);
#endif

	/*CS3 CS4 CS5 CS6*/
  /* *AMAZON_SE_GPIO_P0_OUT = ( (*AMAZON_SE_GPIO_P0_OUT) | 0x2E00); */
  
	// disable IRQ
	*AMAZON_SE_SSC1_IRNEN = 0x0;

#ifdef SSC_RX_INT
  *AMAZON_SE_SSC1_IRNCR = 0;
#endif
	/* Set the Baudrate */
	/*
	 * BR = (FPI clk / (2 * Baudrate)) - 1
	 * Note: Must be set while SSC is disabled!
	 */
	//*AMAZON_SE_SSC1_BR = AMAZON_SE_SSC_BR_BR_VALUE(((danube_get_fpiclk()>>1) + baudrate/2)/(baudrate) - 1);

	// in AmazonSE, fpi_clk=133MHz and too big for 16-bit BR 
	*AMAZON_SE_SSC1_BR = 0; //by 张喻

	/*set PISEL*/
	/**AMAZON_SE_SSC1_PISEL = 0x0;*/
	/*enable and flush RX/TX FIFO*/
	*AMAZON_SE_SSC1_RXFCON = 0xF03;
	*AMAZON_SE_SSC1_TXFCON = 0xF03;
	/* set CON, TX off , RX off, ENBV=0, BM=7(8 bit valid) HB=1(MSB first), PO=0,PH=1(SPI Mode 0)*/
	*AMAZON_SE_SSC1_CON = 0x00070033;	// 0x00070013;
	asm("SYNC");

	/*Set Master mode and  Enable SSC */
	*AMAZON_SE_SSC1_WHBSTATE = 0xA;
	asm("SYNC");

	return 0;
}

void ssc_sync(void)
{
	while(((*(volatile unsigned int *)AMAZON_SE_SSC1_STATE)&AMAZON_SE_SSC_STATE_BSY)==AMAZON_SE_SSC_STATE_BSY);
}

void ssc_tx_only(void){
	ssc_sync();
	/* change to TX only mode */
	*AMAZON_SE_SSC1_CON = ((*AMAZON_SE_SSC1_CON) & (~AMAZON_SE_SSC_CON_TX_OFF)) | AMAZON_SE_SSC_CON_RX_OFF;
}
void ssc_rx_only(void){
	ssc_sync();
	/* change to RX only mode */
	*AMAZON_SE_SSC1_CON = ((*AMAZON_SE_SSC1_CON) & (~AMAZON_SE_SSC_CON_RX_OFF)) | AMAZON_SE_SSC_CON_TX_OFF;
	/* flush RX */
	*AMAZON_SE_SSC1_RXFCON = (*AMAZON_SE_SSC1_RXFCON) | AMAZON_SE_SSC_RXFCON_RXFLU;
}


void ssc_tx_rx(void){
	ssc_sync();
	/* change to TXRX */
	*AMAZON_SE_SSC1_CON = ((*AMAZON_SE_SSC1_CON) & (~(AMAZON_SE_SSC_CON_RX_OFF|AMAZON_SE_SSC_CON_TX_OFF)));
	/* flush RX */
	*AMAZON_SE_SSC1_RXFCON = (*AMAZON_SE_SSC1_RXFCON) | AMAZON_SE_SSC_RXFCON_RXFLU;
}

int ssc_write(unsigned char uc_value)
{
  ssc_sync();
	*AMAZON_SE_SSC1_TB = (u32)uc_value;
	asm("SYNC");
	return 0;
}

int ssc_read(unsigned char *uc_value)
{
	while((AMAZON_SE_SSC_FSTAT_RXFFL(*AMAZON_SE_SSC1_FSTAT)) == 0);
   //printf ("%d bytes in FSTAT .. and value = %x\n",(AMAZON_SE_SSC_FSTAT_RXFFL(*AMAZON_SE_SSC1_FSTAT)), *AMAZON_SE_SSC1_RB);
	*uc_value=(unsigned char)(*AMAZON_SE_SSC1_RB);
	asm("SYNC");
	return 0;
}

/* Brief:	set SSC to read-only mode and read data
 * Parameter:
 *		buf:	pointer to the read buffer
 *		len:	length of byte
 * Return:
 *		number of data read
 */
int ssc_read_only(u8 *buf,u32 len)
{
	int i,eff;
	u32 eff_word;
	union{
		u32 val_u32;
		u8	val_u8[4];
	}tmp;

	while(1){
		eff = len>=4 ? 4:len;
		eff_word = 1;
		
		*AMAZON_SE_SSC1_RXREQ = eff;

		/* poll RIR */
#ifdef SSC_RX_INT
		while(((*AMAZON_SE_SSC1_IRNICR)&AMAZON_SE_SSC_IRN_RIR)==0);
		/* clear RIR */
		*AMAZON_SE_SSC1_IRNCR = 0;
#endif

		while(AMAZON_SE_SSC_FSTAT_RXFFL(*(volatile unsigned int *)AMAZON_SE_SSC1_FSTAT) != eff_word);



	    tmp.val_u32= *(volatile unsigned int *)AMAZON_SE_SSC1_RB;

		for(i=0;i<eff;i++){
			if(eff < 4){
				buf[i] = tmp.val_u8[i+(4-eff)];	/* 由于芯片使用低位优先的填充方式，当读取的数据不够4个字节时，必须跳过前面的空字节数(4-eff) */
			}
			else
			buf[i] = tmp.val_u8[i];
		}
		
		len-=eff;
		buf+=eff;

		if (len <= 0){
			break;
		}

	}

	return len;
}


/***********************************************************************/
/* SPI based non-volatile storage device common routines */
/***********************************************************************/

/* Brief:	probe non-volatile device conected to SPI bus (EEPROM,FLASH)
 * Parameter:	chip select
 * Return:
 *	<0 fails
 *	=0 ???
 *	number of address byte
 */
int spi_probe(spi_dev_t * dev, u32 offset)
{
	u8 tmp1,cnt;

	ssc_tx_rx();

	SSC1_CS_CLEAR(dev->cs);

   //printf ("dev->read_cmd = %x\n", dev->read_cmd);
	ssc_write(dev->read_cmd);

	/* dummy read */
	ssc_read(&tmp1);

#if 1
	for (cnt = 0; cnt < SPI_ADDR_CYC_MAX; cnt++)
	{
		ssc_write(0x00);
      //ssc_sync();
	   //asm("SYNC");
	   //*AMAZON_SE_SSC1_TB = (u32)offset;
	   //asm("SYNC");
		ssc_read(&tmp1);
      //printf ("tmp1 = %x\n", tmp1);
		if ( (tmp1 !=0 ) && (tmp1 != 0xFF) )
			break;
	}
	SSC1_CS_SET(dev->cs);

	if (cnt == SPI_ADDR_CYC_MAX)
		return -1;
	else {
		dev->addr_cycles=cnt;
		return cnt;
	}
#else
	for (cnt = 0; cnt < SPI_ADDR_CYC_MAX; cnt++)
	{
		u32 tmp2;
		u16 magic_word=SPI_MAGIC;
		tmp2=tmp1;
		ssc_write(0x00);
		ssc_read(&tmp1);
		if ( (tmp2 == (magic_word>>8)) && (tmp1 == (magic_word&0xFF)) )
			break;
	}
	SSC1_CS_SET(dev->cs);

	if (cnt == SPI_ADDR_CYC_MAX)
		return -1;
	else {
		dev->addr_cycles = cnt-3;
		return cnt-3;
	}
#endif
}

/* Brief:	continuously read data
 * Parameter:
 *		buf:	pointer to the read buffer
 *		len:	length of byte
 * Return:
 *		number of data read
 *	<0	error
 * Description
 *	-for the first time, spi_probe is called to figure out the address byte and then the 0x00000000 address is written to the device
 *	-follow up read has no CMD
 *	-leave the SSC bus in read-only mode to speed up follow-up read
 *	-leave the chip select valid to speed up follow-up read, meaning caller need to invalidate the CS once done!!
 */

int spi_read(spi_dev_t * dev,u8 *buf, u32 len,u32 offset)
{
   //printf("\nentered spi_read\n");
	/* Probe for address cycles and assert chip select for first time
	   (will be de-asserted in terminate function) */
	/*if (dev->addr_cycles == SPI_ADDR_CYC_MAX)*/ /* Commented by Manamohan */
   
   //printf("\nspi_sflash.addr_cycles = %x\n", spi_sflash.addr_cycles);
   //printf("\ndev->addr_cycles = %x\n", dev->addr_cycles);

	if (dev->addr_cycles == SPI_ADDR_CYC_MAX)
	{
		if ( spi_probe(dev, offset) < 0){
         printf("\nNo serial flash found..\n");
			return -1;
		}

		SSC1_CS_CLEAR(dev->cs);
		ssc_tx_only();
      
		ssc_write(dev->read_cmd);
      
		/* Write start address 0x00000000 */
		while (dev->addr_cycles-- > 0) {
			//ssc_write(0x00);
         //printf("\nEntered the while true part to set the address\n");
			//ssc_write(offset); /* Added by Manamohan */
	      asm("SYNC");
         ssc_sync();
	      *AMAZON_SE_SSC1_TB = offset;
	      asm("SYNC");
		}
		ssc_rx_only();
	}

	ssc_read_only(buf,len);
	return len;
}
