#include <common.h>
#include <net.h>
#include <tcp.h>
#include <malloc.h>

#if (CONFIG_COMMANDS & CFG_CMD_NET)

#define UPDATE_ACK_TIMEOUT	2
#ifdef __BIG_ENDIAN
#define UPDATE_SERVER_IP		0xc0a801fd
#define UPDATE_SERVER_PORT	5300
#else
#define UPDATE_SERVER_IP		0xfd01a8c0
#define UPDATE_SERVER_PORT	0x14b4		//5300
#endif

static int GotServerReply;
static int UpdateOurPort;
static unsigned short UpdateUdpId;
extern unsigned char *NetArpWaitPacketMAC;
extern unsigned char *NetArpWaitTxPacket;
extern IPaddr_t NetArpWaitPacketIP;
extern IPaddr_t NetArpWaitReplyIP;
extern volatile uchar PktBuf[(PKTBUFSRX+1) * PKTSIZE_ALIGN + PKTALIGN];
extern int NetArpWaitTxPacketSize;
extern unsigned char NetArpWaitPacketBuf[PKTSIZE_ALIGN + PKTALIGN];

void massive_upgrade_request_Handler(unsigned char *pkt, unsigned int dst_port, unsigned int src_port, unsigned int len)
{
	unsigned short id = 0;
	IPaddr_t ipaddr;
	unsigned char data[16];
	
	if(dst_port != UpdateOurPort)
		{
		printf("Unknow Packet:dst = %d  UpdateOurPort= %d \n", dst_port, UpdateOurPort);
		return;
		}
	memcpy(data, pkt, sizeof(data));

#ifdef __BIG_ENDIAN
	id = (data[0] << 8) | data[1];
#else
	id = (data[1] <<8) | data[0];
#endif

	if(id != UpdateUdpId)
		{
		printf("Unknow Packet:ID = %d  UpdateUdpId= %d \n", id, UpdateUdpId);
		return;
		}
	if((data[2] != 192) || (data[3] != 168) || (data[4] != 1))
		{
		return;
		}
	
#ifdef __BIG_ENDIAN
	ipaddr = (data[2] << 24 ) | (data[3] << 16) | (data[4] << 8) | data[5];
#else
	ipaddr = (data[5] << 24) | (data[4] << 16) | (data[3] << 8) | data[2];
#endif

	gd->bi_ip_addr = ipaddr;
	GotServerReply = 1;	
}


int check_udp_massive_upgrade(void)
{
	volatile uchar *pkt, *xp;
	int i, len = 0;
	volatile ushort *s;
	ulong time;
	unsigned int crc;
	struct sys_cfg *syscfg = (struct sys_cfg *)(gd->syscfg_addr);

	GotServerReply = -1;
	NetCopyIP(&NetOurIP, (unsigned long *)&(gd->bi_ip_addr));
	memcpy(NetOurEther, (unsigned char *)&(gd->bi_enetaddr[0]), 6);
	NetOurSubnetMask= getenv_netmask();
	UpdateOurPort = (1024 + (get_timer(0) % 3072)) & 0xFFFF;
	UpdateUdpId= get_timer(0) & 0xFFFF;
	NetServerIP = UPDATE_SERVER_IP;
	memset(NetServerEther, 0, 6);
	if(1 == gd->syscfg_ver)
		{
		if(0 == syscfg->boot_img_flag)
			{
			crc = syscfg->first_image_checksum;
			}
		else
			{
			crc = syscfg->second_image_checksum;
			}
		}
	else
		{
		crc = (unsigned int)(get_timer(0));
		}
	NetArpWaitPacketMAC = NULL;
	NetArpWaitTxPacket = NULL;
	NetArpWaitPacketIP = 0;
	NetArpWaitReplyIP = 0;
	NetArpWaitTxPacket = NULL;
	NetTxPacket = NULL;
	if(!NetTxPacket)
		{		
		NetTxPacket = &PktBuf[0] + (PKTALIGN - 1);
		NetTxPacket -= (ulong)NetTxPacket % PKTALIGN;
		for(i = 0; i < PKTBUFSRX; i++)
			{
			NetRxPackets[i] = NetTxPacket + (i+1)*PKTSIZE_ALIGN;
			}
		}

	if(!NetArpWaitTxPacket)
		{
		NetArpWaitTxPacket = &NetArpWaitPacketBuf[0] + (PKTALIGN - 1);
		NetArpWaitTxPacket -= (ulong)NetArpWaitTxPacket % PKTALIGN;
		NetArpWaitTxPacketSize = 0;
		}

	pkt = NetTxPacket + NetEthHdrSize() + IP_HDR_SIZE;
	xp = pkt;
	s = (ushort *)pkt;
	*s++ = UpdateUdpId;
	pkt = (uchar *)s;
	strcpy ((char *)pkt, "Reqestchangeip");
	pkt += 14;	
	memcpy((void *)pkt, &crc, 4);
	pkt += 4;
	pkt += strlen((char *)pkt) ;
	len = pkt - xp;
	printf("Request for massive upgrade...");
	NetSendUDPPacket(NetServerEther, NetServerIP, UPDATE_SERVER_PORT, UpdateOurPort, len);
	NetSetHandler(massive_upgrade_request_Handler);
	time = get_timer(0);	
	while(GotServerReply <= 0)
		{
		eth_rx();
		/* Abort if ctrl-c was pressed. */
		if(ctrlc())
			{
			eth_halt();
			printf("Abort\n");
			GotServerReply = -1;
			break;
			}
		ArpTimeoutCheck();
		if((get_timer(0) - time) > UPDATE_ACK_TIMEOUT * CFG_HZ)
			{
			printf("Timed out!\n");
			break;
			}
		}
	NetStartAgain();

	return GotServerReply;
}

#endif

