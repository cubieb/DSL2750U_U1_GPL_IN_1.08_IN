#include <common.h>
#include <net.h>
#include <command.h>
#include <malloc.h>
#include <led.h>
#include "rtl8676_phy.h"
#include "rtl8676_swmain.h"
#include "rtl8676_swnic.h"
#include "rtl8676_swiob.h"
#include "rtl8676_asicregs.h"

static void check_link_status(struct eth_device *dev)
{
	if(dev)
		{
		if(is_link_change())
			{
			if(ETH_STATE_UP != dev->state)
				{
				SetLed(led_lan_green, LED_BOOT_ON);
				dev->state = ETH_STATE_UP;
				}
			else
				{
				SetLed(led_lan_green, LED_BOOT_OFF);				
				dev->state = ETH_STATE_DOWN;
				}
			}
		}
}

static int Lan_Transmit(struct eth_device *dev, volatile void *buff, int length)
{
	if(dev)
		{
		check_link_status(dev);
		}
	swNic_send(buff, length);
	
	return 0;
}

static int Lan_Initialize(struct eth_device *dev)
{
	REG32(PCRP0) |= 1;
	REG32(PCRP1) |= 1;
	REG32(PCRP2) |= 1;
	REG32(PCRP3) |= 1;
	REG32(PCRP4) |= 1;
	mdelay(1);
	initIOB();
	swInit(gd->bi_enetaddr);	
	return 0;
}

static void Lan_Stop(struct eth_device *dev)
{
	REG32(PCRP0) &= ~1;
	REG32(PCRP1) &= ~1;
	REG32(PCRP2) &= ~1;
	REG32(PCRP3) &= ~1;
	REG32(PCRP4) &= ~1;
	mdelay(1);
}


static int rtl8676_eth_rx(struct eth_device *dev)
{
	void *pPkt;
	u32 len = 0;
	int ret_val;
#ifdef CONFIG_BOOTLOADER_ETH_DEBUG	
	int i, 
	unsigned char *sbuf;
#endif
	if(dev)
		{
		check_link_status(dev);
		}
	ret_val = swNic_receive(&pPkt,&len);
	swNic_txDone();
    if(0 == ret_val)
   	  	{
   	  	etdebug(fmt, args...)
#ifdef CONFIG_BOOTLOADER_ETH_DEBUG  	// for debug only
   	  	printf("p=%x, length=%d\n\r", (unsigned char *)pPkt, len);
		sbuf = (unsigned char *)pPkt;
		for(i=0;i<len; i++)
			{
			if((i%16)==0)
				printf("\n\r");
			printf("%02x ", sbuf[i]);
			}
		printf("\n\r");
#endif		
		NetReceive(pPkt,len);
		}

	return len;
}	


int rtl_eth_init(void)
{
	struct eth_device *dev;

	dev = (struct eth_device *)malloc(sizeof(struct eth_device));
	if(NULL == dev)
		{
		printf("Can not allocate memory for eth_device!\n");
		return -1;
		}
	debug("Eth address is %#x\n", (unsigned int)dev);
	memset(dev, 0x00, sizeof(struct eth_device));
	strcpy(dev->name,"rtl8676sw");
	dev->priv = NULL;
	dev->init = Lan_Initialize;
	dev->halt = Lan_Stop;
	dev->send = Lan_Transmit;
	dev->recv = rtl8676_eth_rx;
	dev->chklk = check_link_status;
	eth_register(dev);
	return 1;
}

