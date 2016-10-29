#include <linux/config.h>
#include <linux/sched.h>	/* jiffies is defined */
#include <linux/kernel.h>
#include <linux/interrupt.h>


#include "proslic_datatypes.h"
#include "proslic_ctrl.h"
#include "sys_driv_type.h"

#include "spi.h"

#include "rtk_voip.h"


extern rtl_spi_dev_t spi_dev[SPI_DEV_NUM];

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
#ifndef CONFIG_RTK_VOIP_8672_SHARED_SPI    //shlee, remove unnecessary code
extern int32 _rtl_spi_rawWrite( rtl_spi_dev_t* pDev, void* pData, int32 bits );
extern int32 _rtl_spi_rawRead( rtl_spi_dev_t* pDev, void* pData, int32 bits );
#endif
static uInt8 ReadReg (uInt16 portID, uInt8 channel, uInt8 regAddr){

#ifdef CONFIG_RTK_VOIP_MULTIPLE_SI32178
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
	rtl8672_share_spi_read(portID, 1, regCtrl, regAddr,  &reg_val);	
#else
	//printk("(%d, %d)\n", portID, channel);
	_rtl_spi_rawWrite(&spi_dev[portID], &regCtrl, 8);
	_rtl_spi_rawWrite(&spi_dev[portID], &regAddr, 8);
	_rtl_spi_rawRead(&spi_dev[portID], &reg_val, 8);
#endif //#ifdef CONFIG_RTK_VOIP_8672_SHARED_SPI

#endif
	restore_flags(flags);

	return reg_val;
}

static void WriteReg (uInt16 portID, uInt8 channel, uInt8 regAddr, uInt8 data)
{
#ifdef CONFIG_RTK_VOIP_MULTIPLE_SI32178
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
	rtl8672_share_spi_write(portID, 1,regCtrl , regAddr,  data);
#else
	_rtl_spi_rawWrite(&spi_dev[portID], &regCtrl, 8);
	_rtl_spi_rawWrite(&spi_dev[portID], &regAddr, 8);
	_rtl_spi_rawWrite(&spi_dev[portID], &data, 8);
#endif //CONFIG_RTK_VOIP_8672_SHARED_SPI
#endif

	restore_flags(flags);
}

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


static void RAMwait (uInt16 portID,unsigned short channel)
{
	unsigned char regVal; 
	regVal = ReadReg (portID,channel,4);
	while (regVal&0x01)
	{
		regVal = ReadReg (portID,channel,4);
	}//wait for indirect registers

}

static void WriteRam (uInt16 portID, uInt8 channel, uInt16 regAddr, uInt32 data)
{
	unsigned long flags;
	
	save_flags(flags); cli();
	
	if (channel == 0xff)
		RAMwait(portID,0);   
	else
		RAMwait(portID,channel);   
	WriteReg(portID,channel,5,(regAddr>>3)&0xe0); //write upper address bits
	WriteReg(portID,channel,6,(data<<3)&0xff);
	WriteReg(portID,channel,7,(data>>5)&0xff);
	WriteReg(portID,channel,8,(data>>13)&0xff);
	WriteReg(portID,channel,9,(data>>21)&0xff);
	WriteReg(portID,channel,10,regAddr&0xff); //write lower address bits  
		
	restore_flags(flags);
}

static uInt32 ReadRam (uInt16 portID, uInt8 channel, uInt16 regAddr)
{
	unsigned char reg; unsigned long RegVal;
	unsigned long flags;
	
	save_flags(flags); cli();
	
	RAMwait(portID,channel);
	WriteReg(portID,channel,5,(regAddr>>3)&0xe0); //write upper address bits
	WriteReg(portID,channel,10,regAddr&0xff); //write lower address bits
	
	RAMwait(portID,channel);
	
	reg=ReadReg (portID,channel,6);
	RegVal = reg>>3;
	reg=ReadReg(portID,channel,7);
	RegVal |= ((unsigned long)reg)<<5;
	reg=ReadReg(portID,channel,8);
	RegVal |= ((unsigned long)reg)<<13;
	reg=ReadReg(portID,channel,9);
	RegVal |= ((unsigned long)reg)<<21;
	
	restore_flags(flags);
	
	return RegVal;
}

void W_ram(unsigned char chid, unsigned short reg, unsigned int data)
{
	return WriteRam(0, chid, reg, data);
}

unsigned int R_ram(unsigned char chid, unsigned short reg)
{
	return ReadRam(0, chid, reg);
}

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
	init_spi(hSpi->portID);

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
	

	return ReadReg(hSpiGci->portID,channel,regAddr);
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

	WriteReg(hSpiGci->portID,channel,regAddr, data);
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
	data = ReadRam(hSpiGci->portID,channel,ramAddr);
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
	WriteRam(hSpiGci->portID,channel,ramAddr,data);
#endif
	return 0;
}


