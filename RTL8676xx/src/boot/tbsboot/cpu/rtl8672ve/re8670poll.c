/* re8670pool.c - RealTek re8670 Fast Ethernet interface header */
/* History:
 *  2007/03/06  SH  Added Dcache write invalidate for CPU LX4181
 *
*/
#include <common.h>
#include <net.h>
#include <command.h>
#include "re8670poll.h"
#include "iob.h"

#define NULL	0
static void Lan_Stop(struct eth_device *dev);

unsigned char descriptor_tx[TX_DESC_NUM*16+256];
unsigned char descriptor_rx[RX_DESC_NUM*16+256];

NIC_FD_T *pTxBDPtr;
NIC_FD_T *pRxBDPtr;
unsigned int	txBDhead = 0;	// index for system to release buffer
unsigned int	txBDtail = 0;	// index for system to set buf to BD
unsigned int	rxBDtail = 0;	// index for system to set buf to BD

char *Lan_Receive(void)
{
	char *pBuf, *pData;

	EISR = 0xffff;  //reset RDU flag to start rx again

	if(pRxBDPtr[rxBDtail].StsLen & OWN_BIT)
		return NULL;

	pData = (char*)pRxBDPtr[rxBDtail].DataPtr;
	pBuf = IOBGetHdr(pData);
	IOB_PKT_LEN(pBuf) = (pRxBDPtr[rxBDtail].StsLen & 0xfff) - 4;

#if 0
	{
	int i;
	unsigned char *sbuf = pData;
		printf("p=%x, l=%d\n\r", sbuf, IOB_PKT_LEN(pBuf));
		for(i=0;i<IOB_PKT_LEN(pBuf); i++)
		{
			if((i%16)==0)
				printf("\n\r");
			printf("%02x ", sbuf[i]);
		}
		printf("\n\r");
	}
#endif

	pRxBDPtr[rxBDtail].StsLen &= ~0xfff;
	pRxBDPtr[rxBDtail].StsLen |= 2048;
	
	pRxBDPtr[rxBDtail].StsLen |= OWN_BIT;
	EthrntRxCPU_Des_Num = rxBDtail;
	rxBDtail++;
	rxBDtail %= RX_DESC_NUM;

	return (pBuf);
}


static int rtl_eth_rx(struct eth_device *dev)
{
	void * pPkt;
	int len;
	IOB *p_iob;

	if ( p_iob = Lan_Receive() )
	{
		pPkt = IOB_PKT_PTR(p_iob);
		len = IOB_PKT_LEN(p_iob);	
	}
	
	for (;;) 
	{
		if ( p_iob = Lan_Receive() )
		{
			pPkt = IOB_PKT_PTR(p_iob);
			len = IOB_PKT_LEN(p_iob);	
		}
		
		/* Pass the packet up to the protocol layers. */
		/*       NetReceive(NetRxPackets[rxIdx], length - 4); */
		/*       NetReceive(NetRxPackets[i], length); */
		NetReceive (pPkt,len);

		if ( len < 0x22e ) 
		{			
			break;	/* nothing received - leave for() loop */
		}
	}
	return len;
}	


int Lan_Transmit(struct eth_device *dev , volatile void * buff, int length)
{
	pTxBDPtr[txBDtail].DataPtr = (unsigned int)buff | UNCACHE_MASK;
	if(length < 60)
		length = 60;
	pTxBDPtr[txBDtail].StsLen &= ~0xfff;
	pTxBDPtr[txBDtail].StsLen |= length;

	// trigger to send
	pTxBDPtr[txBDtail].StsLen |= OWN_BIT|FS_BIT|LS_BIT|(1<<23);

	//tylo, for 8672 fpga, cache write-back
	__asm__ volatile(
		"mtc0 $0,$20\n\t"
		"nop\n\t"
		"nop\n\t"
		"li $8,512\n\t"
		"mtc0 $8,$20\n\t"
		"nop\n\t"
		"nop\n\t"
		"mtc0 $0,$20\n\t"
		"nop"
		: /* no output */
		: /* no input */
			);

	IO_CMD |= (1<<0);
	delay_msec(1);

	while(pTxBDPtr[txBDtail].StsLen & OWN_BIT)
	{
		
		delay_msec(1);
	}

	/* advance one */
	txBDtail++;
	txBDtail %= TX_DESC_NUM;

	return 0;
}


static inline void Eth_W32(unsigned long addr, unsigned int val) 
{
	*(volatile unsigned int *)(regbase + addr) = val;
	delay_msec(5);		
}

static inline unsigned int Eth_R32(unsigned long addr) 
{
	return *(volatile unsigned int *)(regbase + addr);
}

void ethernet_patch() 
{
	// andrew, fix int. phy issue.
	Eth_W32(0x5c, 0x841f0003);
	Eth_W32(0x5c, 0x84093871);
	Eth_W32(0x5c, 0x841f0000);
	
		
	/*		
	Eth_W32(0x5c, 0x841f0003);
	Eth_W32(0x5c, 0x04090000);
	printf("\r\nr %x\r\n", Eth_R32(0x5c));
	Eth_W32(0x5c, 0x841f0000);
	*/

	//led control
	Eth_W32(0x70, 0x00047777);

	//for RTL8306N
	Eth_W32(0x5c, 0x84002100);
}

static int rtl_eth_init2 (struct eth_device *dev, bd_t * bis)
{
	unsigned long mac_reg0, mac_reg1;
	int i;

	initIOB();

	Lan_Stop(dev);

	pTxBDPtr = (NIC_FD_T *)((((unsigned int)(descriptor_tx+0xff))& 0xffffff00)|UNCACHE_MASK);
	pRxBDPtr = (NIC_FD_T *)((((unsigned int)(descriptor_rx+0xff))& 0xffffff00)|UNCACHE_MASK);

	/* setup descriptor */
	RxFDP = pRxBDPtr;
	RxCDO = 0;
	TxFDP1 = pTxBDPtr;
	TxCDO1 = 0;	

	// init xmt BD
	for(i=0;i<TX_DESC_NUM;i++)
	{
		pTxBDPtr[i].StsLen = 0;
		pTxBDPtr[i].VLan = 0;
		pTxBDPtr[i].DataPtr = 0;
	}
	pTxBDPtr[TX_DESC_NUM-1].StsLen |= EOR_BIT;

	for(i=0;i<RX_DESC_NUM;i++)
	{
		char *pBuf;
		if ( (pBuf = getIOB()) == 0 ) 
		{
			return -1;
		}

		pRxBDPtr[i].StsLen = 2048+ OWN_BIT + RX_DESC_BUFFER_SIZE;
		pRxBDPtr[i].VLan = 0;
		pRxBDPtr[i].DataPtr = (unsigned long) IOB_PKT_PTR(pBuf);
	}

	pRxBDPtr[RX_DESC_NUM-1].StsLen |= EOR_BIT;

	// set MAC address

	mac_reg0 = (bis->bi_enetaddr[0] << 24) | (bis->bi_enetaddr[1] << 16) | (bis->bi_enetaddr[2] << 8) | (bis->bi_enetaddr[3] << 0);
	mac_reg1 = (bis->bi_enetaddr[4]<< 24) | (bis->bi_enetaddr[5] << 16);
	//memcpy(&mac_reg0, mac, 4);
	//memcpy(&mac_reg1, &mac[4], 2);
	NIC_ID0 = mac_reg0;
	NIC_ID1 = mac_reg1;	
			
	/* RCR, don't accept error packet */
	RCR = NoErrAccept;

	/* TCR: IFG, Mode */
	TCR = (unsigned long)(TCR_IFG<<TCR_IFG_OFFSET)|(TCR_NORMAL<<TCR_MODE_OFFSET);

	Rx_Pse_Des_Thres = RX_FC_THRESHOLD;

	/* Rx descriptor Size */	
	EthrntRxCPU_Des_Num = RX_DESC_NUM-1;

	RxRingSize = 0x00;	// 16 descriptor

	/* Flow Control */
	MSR &= ~(TXFCE | RXFCE);

	/* Ethernet IO CMD */
	IO_CMD = CMD_CONFIG;

	txBDtail = 0;	// index for system to set buf to BD
	rxBDtail = 0;	// index for system to set buf to BD

	return 1;
}

//11/09/05' hrchen, disable NIC
static void Lan_Stop(struct eth_device *dev)
{
   	CR = 0x01;	 /* Reset */		
	while ( CR & 0x1 );		
	CR = RX_DISALBE;	 /* receive checksum */

	/* Interrupt Register, ISR, IMR */
	EIMR = 0;
	EISR = 0xffff;
}

int rtl_eth_init(bd_t *bis)
{
	struct eth_device* dev;
	
	dev = (struct eth_device *) malloc (sizeof (*dev));
	if (dev == NULL) 
	{
		printf ("rtl_eth_init: "
		"Cannot allocate eth_device\n");
		return (-1);
	}
	memset(dev, 0, sizeof(*dev));

	strcpy(dev->name,"rtl8672");		
	dev->priv = NULL;
	dev->init = rtl_eth_init2;
	dev->halt = Lan_Stop;
	dev->send = Lan_Transmit;
	dev->recv = rtl_eth_rx;

	eth_register(dev);
	printf("Register rtl8672eth device.\n");

	regbase=0xb8018000;
	ethernet_patch() ;	

	return 1;
}



