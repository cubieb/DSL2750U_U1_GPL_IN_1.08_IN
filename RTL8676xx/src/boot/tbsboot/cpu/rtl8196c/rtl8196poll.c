/* re8670pool.c - RealTek re8670 Fast Ethernet interface header */
/* History:
 *  2007/03/06  SH  Added Dcache write invalidate for CPU LX4181
 *
*/
#include <common.h>
#include <net.h>
#include <command.h>
#include "rtl8196x/asicregs.h"  
#include "rtl8196x/rtl8196x.h"
#include "rtl8196x/rtl_types.h"
#include "rtl8196x/loader.h"
#include "rtl8196x/swCore.h"
#include "rtl8196x/swNic_poll.h"
#include "rtl8196x/vlanTable.h"
#include "rtl8196poll.h"
#include "iob.h"

//#define NULL	0

unsigned char descriptor_tx[TX_DESC_NUM*16+256];
unsigned char descriptor_rx[RX_DESC_NUM*16+256];

NIC_FD_T *pTxBDPtr;
NIC_FD_T *pRxBDPtr;
unsigned int	txBDhead = 0;	// index for system to release buffer
unsigned int	txBDtail = 0;	// index for system to set buf to BD
unsigned int	rxBDtail = 0;	// index for system to set buf to BD
extern int32 swCore_netifCreate(uint32 idx, rtl_netif_param_t * param);

void dump_pack(char *pBuf,int len)
{
	int i;
	unsigned char *sbuf ;
 	if(pBuf == NULL)
		return;
	sbuf = pBuf;
	printf("p=%x, l=%d\n\r",sbuf,len);
	for(i=0;i<len; i++)
	{
		if((i%16)==0)
			printf("\n\r");
		printf("%02x ", sbuf[i]);
	}
	printf("\n\r");
}

static int rtl_eth_rx(struct eth_device *dev)
{
	char *pPkt;
	int len;
	
	while(1)
		{
		len=0;
		if(swNic_receive((void *)&pPkt,&len)==0) /* Pass the packet up to the protocol layers. */
			{
 			NetReceive (pPkt,len);
			}
		swNic_txDone();

		if(len < 0x22e) /* nothing received - leave for() loop */
			break;	
		}

	return len;
}	


int Lan_Transmit(struct eth_device *dev , volatile void * buff, int length)
{
#if 0
	dump_pack(buff,length);
#endif
	swNic_send(buff,length);
	return 0;
}

void ethernet_patch(void) 
{
	if (REG32(0xb8000000) == 0x80000002)
	Setting_RTL8196C_PHY_REV_B();
}
void Lan_Stop(struct eth_device *dev)
{
	//FullAndSemiReset();
	//REG32(CPUICR) &= ~(TXCMD | RXCMD);
}

static int rtl_eth_init2 (struct eth_device *dev, bd_t * bis)
{
	//avoid download bin checksum error
	uint32 rx[6] = {4, 0, 0, 0, 0, 0};
	uint32 tx[2] = {4, 2};
	rtl_vlan_param_t vp;
	int32 ret=-1;
	rtl_netif_param_t np;
	
	//Int32 val;
	if (swCore_init()) {  	
		printf("\nSwitch core initialization failed!\n");        
		return 0;
	}


	/* Initialize NIC module */
	if (swNic_init(rx, 4, tx, MBUF_LEN)) {
		printf("\nSwitch nic initialization failed!\n");            
		return 0;
	}

	/* Create Netif */
	memset((void *) &np,0, sizeof(rtl_netif_param_t));
	np.vid = 8;
	np.valid = 1;
	np.enableRoute = 0;
	np.inAclEnd = 0;
	np.inAclStart = 0;
	np.outAclEnd = 0;
	np.outAclStart = 0;
	memcpy(&np.gMac, &bis->bi_enetaddr[0], 6);
	np.macAddrNumber = 1;
	np.mtu = 1500;
	ret = swCore_netifCreate(0, &np);
	if (ret != 0) {
		printf( "Creating intif fails:%d\n", ret );
		return 0;
	}

	/* Create vlan */
	memset((void *) &vp,0, sizeof(rtl_vlan_param_t));
	vp.egressUntag = ALL_PORT_MASK;
	vp.memberPort = ALL_PORT_MASK;
	ret = swCore_vlanCreate(8, &vp);
	if (ret != 0) {       
		printf( "Creating vlan fails:%d\n", ret );
       return 0;
	}

	return 1;
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

	strcpy(dev->name,"rt8196c");		
	dev->priv = NULL;
	dev->init = rtl_eth_init2;
	dev->halt = Lan_Stop;
	dev->send = Lan_Transmit;
	dev->recv = rtl_eth_rx;

	eth_register(dev);
	printf("Register rtl8196ceth device.\n");
	ethernet_patch();
	return 0;
}



