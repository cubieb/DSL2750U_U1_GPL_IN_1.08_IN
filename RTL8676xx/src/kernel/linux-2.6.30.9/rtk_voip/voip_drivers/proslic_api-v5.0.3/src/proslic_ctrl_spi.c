#include <linux/config.h>
#include <linux/sched.h>	/* jiffies is defined */
#include <linux/kernel.h>
#include <linux/interrupt.h>


#include "si_voice_datatypes.h"
#include "si_voice_ctrl.h"
#include "sys_driv_type.h"
#include "spi/spi.h"
#include "rtk_voip.h"

#ifndef CONFIG_RTK_VOIP_8672_SHARED_SPI
//extern rtl_spi_dev_t spi_dev[SPI_DEV_NUM];
#endif

#if 0
////////////////////////////////////////////////////////////////////////////////
static void byteToSpi(uInt16 portID, uInt8 byte)
{
	unsigned long flags;		
	save_flags(flags); cli();
	
	_rtl_spi_rawWrite(&spi_dev[portID], &byte, 8);

	restore_flags(flags);
}// byteToSpi

//*************************************************************
static uInt8 spiToByte(uInt16 portID)
{
	uInt8 byte;
	unsigned long flags;		
	save_flags(flags); cli();

	_rtl_spi_rawRead(&spi_dev[portID], &byte, 8);
	
	restore_flags(flags);

	return byte;	
}// spiToByte
#endif

#define CNUM_TO_CID_QUAD(channelNumber)   (((channelNumber<<4)&0x10)|((channelNumber<<2)&0x8)|((channelNumber>>2)&0x2)|((channelNumber>>4)&0x1)|(channelNumber&0x4))
#ifdef CONFIG_RTK_VOIP_8672_SHARED_SPI
extern void rtl8672_share_spi_write(unsigned int ch, unsigned int uses_daisy, unsigned int contorl, unsigned int address,unsigned int data);
extern unsigned int  rtl8672_share_spi_read(unsigned int ch, unsigned int uses_daisy, unsigned int control, unsigned int address,unsigned char *data);
#else
extern int32 _rtl_spi_rawWrite( rtl_spi_dev_t* pDev, void* pData, int32 bits );
extern int32 _rtl_spi_rawRead( rtl_spi_dev_t* pDev, void* pData, int32 bits );
#endif
static uInt8 ReadReg_dev (rtl_spi_dev_t* pDev, uInt8 channel, uInt8 regAddr){

#if 0//def CONFIG_RTK_VOIP_MULTIPLE_SI32178
	channel = channel/SLIC_CH_NUM;
	//printk("portID = %d, ch=%d\n", portID, channel);
#endif
	uInt8 regCtrl = CNUM_TO_CID_QUAD(channel)|0x60;
	uInt8 reg_val;
	unsigned long flags;
	
	save_flags(flags); cli();
#if 0
	byteToSpi(portID,regCtrl);
	byteToSpi(portID,regAddr);
	reg_val =  spiToByte(portID);
#else

#ifdef CONFIG_RTK_VOIP_8672_SHARED_SPI
	uInt16 portID = 0; // for daisy chain
	rtl8672_share_spi_read(portID, 1, regCtrl, regAddr,  &reg_val);	
#else
	//printk("(%d, %d)\n", portID, channel);
 	// .8138 (.2474)
 	// .6031 (.2130)	inline + dmem (gpio)
 	// .6300 (.2555)	inline
 	// .5799 (.2039)	inline + dmem (gpio/desc)
 	// .4697 (.2323)	inline + no debug 
	_rtl_spi_rawWrite(pDev, &regCtrl, 8);
	_rtl_spi_rawWrite(pDev, &regAddr, 8);
	// .5400 (.1401)
	// .5018 (.1468)	inline + dmem 
	// .4963 (.1508)	inline
	// .4920 (.1774)	inline + dmem (gpio/desc)
	// .3847 (.1772)	inline + no debug 
	_rtl_spi_rawRead(pDev, &reg_val, 8);
#endif //#ifdef CONFIG_RTK_VOIP_8672_SHARED_SPI

#endif
	restore_flags(flags);

	return reg_val;
}

#if 0
static uInt8 ReadReg (uInt16 portID, uInt8 channel, uInt8 regAddr)
{
	return ReadReg_dev( &spi_dev[portID], channel, regAddr );
}
#endif

static void WriteReg_dev (rtl_spi_dev_t* pDev, uInt8 channel, uInt8 regAddr, uInt8 data)
{
#if 0//def CONFIG_RTK_VOIP_MULTIPLE_SI32178
	channel = channel/SLIC_CH_NUM;
#endif
	uInt8 regCtrl = CNUM_TO_CID_QUAD(channel)|0x20;
	unsigned long flags;

	save_flags(flags); cli();
	if (channel == 0xff)
		regCtrl = 0x20 | 0x80;
#if 0
	byteToSpi(portID,regCtrl);
	byteToSpi(portID,regAddr);
	byteToSpi(portID,data);
#else

#ifdef CONFIG_RTK_VOIP_8672_SHARED_SPI
	uInt16 portID = 0; // for daisy chain
	rtl8672_share_spi_write(portID, 1,regCtrl , regAddr,  data);
#else
	_rtl_spi_rawWrite(pDev, &regCtrl, 8);
	_rtl_spi_rawWrite(pDev, &regAddr, 8);
	_rtl_spi_rawWrite(pDev, &data, 8);
#endif //CONFIG_RTK_VOIP_8672_SHARED_SPI
#endif

	restore_flags(flags);
}

#if 0
static void WriteReg (uInt16 portID, uInt8 channel, uInt8 regAddr, uInt8 data)
{
	WriteReg_dev( &spi_dev[portID], channel, regAddr, data );
}
#endif

unsigned char R_reg_dev(rtl_spi_dev_t *pdev, unsigned char chid, unsigned char regaddr)
{
	return ReadReg_dev( pdev, chid, regaddr );
}

#if 0	// chmap don't use this 
unsigned char R_reg(unsigned char chid, unsigned char regaddr)
{
#ifdef CONFIG_RTK_VOIP_MULTIPLE_SI32178
	uInt16 portID;
	
	switch (chid)
	{
		/* 1st Si32178 */
		case 0:
		case (0+SLIC_CH_NUM):		
			portID = 0;
			break;
			
#if defined (CONFIG_RTK_VOIP_SI32178_NUM_2) || defined (CONFIG_RTK_VOIP_SI32178_NUM_3) || defined (CONFIG_RTK_VOIP_SI32178_NUM_4)
		/* 2nd Si32178 */
		case 1:
		case (1+SLIC_CH_NUM):
			portID = 1;
			break;
#endif

#if defined (CONFIG_RTK_VOIP_SI32178_NUM_3) || defined (CONFIG_RTK_VOIP_SI32178_NUM_4)
		/* 3rd Si32178 */
		case 2:
		case (2+SLIC_CH_NUM):
			portID = 2;
			break;
#endif

#ifdef CONFIG_RTK_VOIP_SI32178_NUM_4		
		/* 4th Si32178 */
		case 3:
		case (3+SLIC_CH_NUM):
			portID = 3;
			break;
#endif
			
		default:
			PRINT_R("Not support chid%d: %s, line-%d\n", chid, __FUNCTION__, __LINE__);
			break;
	}
	return ReadReg(portID, chid, regaddr);
#else
	return ReadReg(0, chid, regaddr);
#endif
}
#endif

void W_reg_dev(rtl_spi_dev_t *pdev, unsigned char chid, unsigned char regaddr, unsigned char data)
{

	WriteReg_dev( pdev, chid, regaddr, data );
}

#if 0	// chmap don't use this 
void W_reg(unsigned char chid, unsigned char regaddr, unsigned char data)
{
#ifdef CONFIG_RTK_VOIP_MULTIPLE_SI32178
	uInt16 portID;
	
	switch (chid)
	{
		/* 1st Si32178 */
		case 0:
		case (0+SLIC_CH_NUM):		
			portID = 0;
			break;

#if defined (CONFIG_RTK_VOIP_SI32178_NUM_2)  || defined (CONFIG_RTK_VOIP_SI32178_NUM_3) || defined (CONFIG_RTK_VOIP_SI32178_NUM_4)
		/* 2nd Si32178 */
		case 1:
		case (1+SLIC_CH_NUM):
			portID = 1;
			break;
#endif
	
#if defined (CONFIG_RTK_VOIP_SI32178_NUM_3) || defined (CONFIG_RTK_VOIP_SI32178_NUM_4)
		/* 3rd Si32178 */
		case 2:
		case (2+SLIC_CH_NUM):
			portID = 2;
			break;
#endif

#ifdef CONFIG_RTK_VOIP_SI32178_NUM_4	
		/* 4th Si32178 */
		case 3:
		case (3+SLIC_CH_NUM):
			portID = 3;
			break;
#endif
			
		default:
			PRINT_R("Not support chid%d: %s, line-%d\n", chid, __FUNCTION__, __LINE__);
			break;
	}
	
	WriteReg(portID, chid, regaddr, data);
#else
	WriteReg(0, chid, regaddr, data);
#endif
}
#endif

#if 0
static void WriteRam16Bits (uInt16 portID, uInt8 channel, uInt16 regAddr, uInt16 data){
	
	if ((regAddr>>3)&0xe0)
		WriteReg(portID,channel,5,(regAddr>>3)&0xe0); //write upper address bits
	if (channel == 0xff)
		byteToSpi(portID,0x80);  // Write the control byte
	else
		byteToSpi(portID,CNUM_TO_CID_QUAD(channel));  // Write the control byte
	byteToSpi(portID,regAddr);                                 // Write the RAM address
	byteToSpi(portID,data >> 8);                               // Write the MSB of data
	byteToSpi(portID,data & 0xFF);                             // Write the LSB of data

	if ((regAddr>>3)&0xe0)
		WriteReg(portID,channel,5,0); //clear upper address bits
}

static uInt16 ReadRam16Bits (uInt16 portID, uInt8 channel, uInt16 regAddr){
	uInt16 data;
	if ((regAddr>>3)&0xe0)
		WriteReg(portID,channel,5,(regAddr>>3)&0xe0); //write upper address bits

	byteToSpi(portID,0x40 | CNUM_TO_CID_QUAD(channel));  // Write the control byte
	byteToSpi(portID,regAddr);                                 // Write the RAM address
	data = spiToByte(portID) << 8;                             // High byte RAM data
	data |= spiToByte(portID);  

	if ((regAddr>>3)&0xe0)
		WriteReg(portID,channel,5,0); //clear upper address bits
	return data;
}
#endif


static void RAMwait_dev (rtl_spi_dev_t* pDev,unsigned short channel)
{
	unsigned char regVal; 
	regVal = ReadReg_dev (pDev,channel,4);
	while (regVal&0x01)
	{
		regVal = ReadReg_dev (pDev,channel,4);
	}//wait for indirect registers

}

#if 0
static void RAMwait (uInt16 portID,unsigned short channel)
{
	RAMwait_dev( &spi_dev[portID], channel );
}
#endif

static void WriteRam_dev (rtl_spi_dev_t* pDev, uInt8 channel, uInt16 regAddr, uInt32 data)
{
	unsigned long flags;
	
	save_flags(flags); cli();
	
	if (channel == 0xff)
		RAMwait_dev(pDev,0);   
	else
		RAMwait_dev(pDev,channel);   
	WriteReg_dev(pDev,channel,5,(regAddr>>3)&0xe0); //write upper address bits
	WriteReg_dev(pDev,channel,6,(data<<3)&0xff);
	WriteReg_dev(pDev,channel,7,(data>>5)&0xff);
	WriteReg_dev(pDev,channel,8,(data>>13)&0xff);
	WriteReg_dev(pDev,channel,9,(data>>21)&0xff);
	WriteReg_dev(pDev,channel,10,regAddr&0xff); //write lower address bits  
		
	restore_flags(flags);
}

#if 0
static void WriteRam (uInt16 portID, uInt8 channel, uInt16 regAddr, uInt32 data)
{
	WriteRam_dev( &spi_dev[portID], channel, regAddr, data );
}
#endif

static uInt32 ReadRam_dev (rtl_spi_dev_t* pDev, uInt8 channel, uInt16 regAddr)
{
	unsigned char reg; unsigned long RegVal;
	unsigned long flags;
	
	save_flags(flags); cli();
	
	RAMwait_dev(pDev,channel);
	WriteReg_dev(pDev,channel,5,(regAddr>>3)&0xe0); //write upper address bits
	WriteReg_dev(pDev,channel,10,regAddr&0xff); //write lower address bits
	
	RAMwait_dev(pDev,channel);
	
	reg=ReadReg_dev(pDev,channel,6);
	RegVal = reg>>3;
	reg=ReadReg_dev(pDev,channel,7);
	RegVal |= ((unsigned long)reg)<<5;
	reg=ReadReg_dev(pDev,channel,8);
	RegVal |= ((unsigned long)reg)<<13;
	reg=ReadReg_dev(pDev,channel,9);
	RegVal |= ((unsigned long)reg)<<21;
	
	restore_flags(flags);
	
	return RegVal;
}

#if 0
static uInt32 ReadRam (uInt16 portID, uInt8 channel, uInt16 regAddr)
{
	return ReadRam_dev( &spi_dev[portID], channel, regAddr );
}
#endif

void W_ram_dev(rtl_spi_dev_t* pDev, unsigned char chid, unsigned short reg, unsigned int data)
{
	return WriteRam_dev(pDev, chid, reg, data);
}

#if 0	// chmap don't use this 
void W_ram(unsigned char chid, unsigned short reg, unsigned int data)
{
#ifdef CONFIG_RTK_VOIP_MULTIPLE_SI32178
	uInt16 portID;
	
	switch (chid)
	{
		/* 1st Si32178 */
		case 0:
		case (0+SLIC_CH_NUM):		
			portID = 0;
			break;
			
#if defined (CONFIG_RTK_VOIP_SI32178_NUM_2) || defined (CONFIG_RTK_VOIP_SI32178_NUM_3) || defined (CONFIG_RTK_VOIP_SI32178_NUM_4)
		/* 2nd Si32178 */
		case 1:
		case (1+SLIC_CH_NUM):
			portID = 1;
			break;
#endif

#if defined (CONFIG_RTK_VOIP_SI32178_NUM_3) || defined (CONFIG_RTK_VOIP_SI32178_NUM_4)
		/* 3rd Si32178 */
		case 2:
		case (2+SLIC_CH_NUM):
			portID = 2;
			break;
#endif

#ifdef CONFIG_RTK_VOIP_SI32178_NUM_4		
		/* 4th Si32178 */
		case 3:
		case (3+SLIC_CH_NUM):
			portID = 3;
			break;
#endif
			
		default:
			PRINT_R("Not support chid%d: %s, line-%d\n", chid, __FUNCTION__, __LINE__);
			break;
	}
	return WriteRam(portID, chid, reg, data);
#else
	return WriteRam(0, chid, reg, data);
#endif
}
#endif 

unsigned int R_ram_dev(rtl_spi_dev_t* pdev, unsigned char chid, unsigned short reg)
{
	return ReadRam_dev(pdev, chid, reg);
}

#if 0	// chmap don't use this 
unsigned int R_ram(unsigned char chid, unsigned short reg)
{

#ifdef CONFIG_RTK_VOIP_MULTIPLE_SI32178
	uInt16 portID;
	
	switch (chid)
	{
		/* 1st Si32178 */
		case 0:
		case (0+SLIC_CH_NUM):		
			portID = 0;
			break;
			
#if defined (CONFIG_RTK_VOIP_SI32178_NUM_2) || defined (CONFIG_RTK_VOIP_SI32178_NUM_3) || defined (CONFIG_RTK_VOIP_SI32178_NUM_4)
		/* 2nd Si32178 */
		case 1:
		case (1+SLIC_CH_NUM):
			portID = 1;
			break;
#endif

#if defined (CONFIG_RTK_VOIP_SI32178_NUM_3) || defined (CONFIG_RTK_VOIP_SI32178_NUM_4)
		/* 3rd Si32178 */
		case 2:
		case (2+SLIC_CH_NUM):
			portID = 2;
			break;
#endif

#ifdef CONFIG_RTK_VOIP_SI32178_NUM_4		
		/* 4th Si32178 */
		case 3:
		case (3+SLIC_CH_NUM):
			portID = 3;
			break;
#endif
			
		default:
			PRINT_R("Not support chid%d: %s, line-%d\n", chid, __FUNCTION__, __LINE__);
			break;
	}
	return ReadRam(portID, chid, reg);
#else
	return ReadRam(0, chid, reg);
#endif
}
#endif

/*
** Function: SPI_Init
**
** Description: 
** Initializes the SPI interface
**
** Input Parameters: 
** none
**
** Return:
** none
*/
int SPI_Init (ctrl_S *hSpi){
	// chmap don't need this 
	//init_spi(hSpi->portID);

	return 0;
}

/*
** Function: spiGci_ResetWrapper
**
** Description: 
** Sets the reset pin of the ProSLIC
*/
int ctrl_ResetWrapper (ctrl_S *hSpiGci, int status){

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM865xC
	_rtl865xC_setGpioDataBit(PIN_RESET1, ~status);
#else
	//"Not implemented yet!";
#endif
	
	return 0;
}

/*
** SPI/GCI register read 
**
** Description: 
** Reads a single ProSLIC register
**
** Input Parameters: 
** channel: ProSLIC channel to read from
** regAddr: Address of register to read
** return data: data to read from register
**
** Return:
** none
*/
uInt8 ctrl_ReadRegisterWrapper (ctrl_S *hSpiGci, uInt8 channel, uInt8 regAddr){
	// 1.6549 (.5240)
	// 1.4985 (.4939)	inline + dmem
	// 1.3695 (.5622)	inline
	// 1.4759 (.5439)	inline + dmem (gpio/desc)
	return ReadReg_dev(&hSpiGci->spi_dev,channel,regAddr);
}


/*
** Function: spiGci_WriteRegisterWrapper 
**
** Description: 
** Writes a single ProSLIC register
**
** Input Parameters: 
** channel: ProSLIC channel to write to
** address: Address of register to write
** data: data to write to register
**
** Return:
** none
*/
int ctrl_WriteRegisterWrapper (ctrl_S *hSpiGci, uInt8 channel, uInt8 regAddr, uInt8 data){

	WriteReg_dev(&hSpiGci->spi_dev,channel,regAddr, data);
	return 0;
}


/*
** Function: SPI_ReadRAMWrapper
**
** Description: 
** Reads a single ProSLIC RAM location
**
** Input Parameters: 
** channel: ProSLIC channel to read from
** address: Address of RAM location to read
** pData: data to read from RAM location
**
** Return:
** none
*/
ramData ctrl_ReadRAMWrapper (ctrl_S *hSpiGci, uInt8 channel, uInt16 ramAddr){
	ramData data;
#ifdef ALLBITS
	data = ReadRamAllBits(hSpiGci->portID,channel,ramAddr);
#else
	data = ReadRam_dev(&hSpiGci->spi_dev,channel,ramAddr);
#endif
	return data;
}


/*
** Function: SPI_WriteRAMWrapper
**
** Description: 
** Writes a single ProSLIC RAM location
**
** Input Parameters: 
** channel: ProSLIC channel to write to
** address: Address of RAM location to write
** data: data to write to RAM location
**
** Return:
** none
*/
int ctrl_WriteRAMWrapper (ctrl_S *hSpiGci, uInt8 channel, uInt16 ramAddr, ramData data){
#ifdef ALLBITS
	WriteRamAllBits(hSpiGci->portID,channel,ramAddr,data);
#else
	WriteRam_dev(&hSpiGci->spi_dev,channel,ramAddr,data);
#endif
	return 0;
}


