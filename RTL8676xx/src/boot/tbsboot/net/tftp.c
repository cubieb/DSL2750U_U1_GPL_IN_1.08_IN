/*
 *	Copyright 1994, 1995, 2000 Neil Russell.
 *	(See License)
 *	Copyright 2000, 2001 DENX Software Engineering, Wolfgang Denk, wd@denx.de
 */

#include <common.h>
#include <command.h>
#include <net.h>
#include <malloc.h>
#include "tftp.h"

#if (CONFIG_COMMANDS & CFG_CMD_NET)

extern ulong get_timer (ulong base);

#define WELL_KNOWN_PORT	69		/* Well known TFTP port #		*/
#define TIMEOUT		2		/* Seconds to timeout for a lost pkt	*/
#ifndef	CONFIG_NET_RETRY_COUNT
# define TIMEOUT_COUNT	5		/* # of timeouts before giving up  */
#else
# define TIMEOUT_COUNT  (CONFIG_NET_RETRY_COUNT * 2)
#endif
#define HASHES_PER_LINE	65		/* Number of "loading" hashes per line */ /* (for checking the image size)	*/

/* TFTP operations. */
#define TFTP_RRQ	1
#define TFTP_WRQ	2
#define TFTP_DATA	3
#define TFTP_ACK	4
#define TFTP_ERROR	5
#define TFTP_OACK	6

#define STATE_RRQ	1
#define STATE_DATA	2
#define STATE_TOO_LARGE	3
#define STATE_BAD_MAGIC	4
#define STATE_OACK	5
#define TFTP_BLOCK_SIZE		512		    /* default TFTP block size	*/
#define TFTP_SEQUENCE_SIZE	((ulong)(1<<16))    /* sequence number is 16 bit */
#define DEFAULT_NAME_LEN	(8 + 4 + 1)

static unsigned int	TftpServerPort;		/* The UDP port at their end		*/
static unsigned int	TftpOurPort;		/* The UDP port at our end		*/
static unsigned long TftpBlock;		/* packet sequence number		*/
static unsigned long TftpLastBlock;		/* last packet sequence number received */
static unsigned long TftpBlockWrap;		/* count of sequence number wraparounds */
static unsigned long TftpBlockWrapOffset;	/* memory offset due to wrapping	*/
static int TftpState;
static int TftpTimeoutCount;
static char default_filename[DEFAULT_NAME_LEN];
static char *tftp_filename;

#ifdef CFG_DIRECT_FLASH_TFTP
extern flash_info_t flash_info[];
#endif

static __inline__ void store_block (unsigned block, uchar * src, unsigned len)
{
	ulong offset = block * TFTP_BLOCK_SIZE + TftpBlockWrapOffset;
	ulong newsize = offset + len;
#ifdef CFG_DIRECT_FLASH_TFTP
	int i, rc = 0;

	for(i=0; i<CFG_MAX_FLASH_BANKS; i++) 
		{ /* start address in flash? */
		if(load_addr + offset >= flash_info[i].start[0])
			{
			rc = 1;
			break;
			}
		}
	if(rc)
		{ /* Flash is destination for this packet */
		rc = flash_write ((char *)src, (ulong)(load_addr+offset), len);
		if(rc) 
			{
			flash_perror (rc);
			NetState = NETLOOP_FAIL;
			return;
			}
		}
	else
#endif /* CFG_DIRECT_FLASH_TFTP */
		(void)memcpy((void *)(load_addr + offset), src, len);
	if(NetBootFileXferSize < newsize)
		NetBootFileXferSize = newsize;
}

static void TftpSend(void)
{
	volatile uchar *	pkt;
	volatile uchar *	xp;
	int			len = 0;
	volatile ushort *s;

	/* We will always be sending some sort of packet, so cobble together the packet headers now. */
	pkt = NetTxPacket + NetEthHdrSize() + IP_HDR_SIZE;
	switch (TftpState) 
		{
	case STATE_RRQ:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_RRQ);
		pkt = (uchar *)s;
		strcpy ((char *)pkt, tftp_filename);
		pkt += strlen(tftp_filename) + 1;
		strcpy ((char *)pkt, "octet");
		pkt += 5 /*strlen("octet")*/ + 1;
		strcpy ((char *)pkt, "timeout");
		pkt += 7 /*strlen("timeout")*/ + 1;
		sprintf((char *)pkt, "%d", TIMEOUT);
		etdebug("send option \"timeout %s\"\n", (char *)pkt);
		pkt += strlen((char *)pkt) + 1;
		len = pkt - xp;
		break;

	case STATE_DATA:
	case STATE_OACK:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_ACK);
		*s++ = htons(TftpBlock);
		pkt = (uchar *)s;
		len = pkt - xp;
		break;

	case STATE_TOO_LARGE:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_ERROR);
		*s++ = htons(3);
		pkt = (uchar *)s;
		strcpy ((char *)pkt, "File too large");
		pkt += 14 /*strlen("File too large")*/ + 1;
		len = pkt - xp;
		break;

	case STATE_BAD_MAGIC:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_ERROR);
		*s++ = htons(2);
		pkt = (uchar *)s;
		strcpy ((char *)pkt, "File has bad magic");
		pkt += 18 /*strlen("File has bad magic")*/ + 1;
		len = pkt - xp;
		break;
		}
	NetSendUDPPacket(NetServerEther, NetServerIP, TftpServerPort, TftpOurPort, len);
}

static void TftpTimeout_Handler(void)
{
	if(++TftpTimeoutCount > TIMEOUT_COUNT) 
		{
		printf("\nTimeout, trying again\n");
		NetStartAgain ();
		}
	else
		{
		printf("T ");
		NetSetTimeout (TIMEOUT * CFG_HZ, TftpTimeout_Handler);
		TftpSend();
		}
}

static void TftpHandler(uchar *pkt, unsigned dest, unsigned src, unsigned len)
{
	ushort proto;
	ushort *s;

	if(dest != TftpOurPort) 
		{
		printf("Unknow Packet:dest = %d  TftpOurPort= %d \n", dest,TftpOurPort);
		return;
		}
	if(TftpState != STATE_RRQ && src != TftpServerPort) 
		{
		printf("Error Packet:TftpState = %d  src =%d TftpServerPort= %d \n",TftpState,src,TftpServerPort);
		return;
		}

	if(len < 2) 
		return;
	len -= 2; /* warning: don't use increment (++) in ntohs() macros!! */
	s = (ushort *)pkt;
	proto = *s++;
	etdebug("TFTP proto=0x%04x\n", ntohs(proto));
	pkt = (uchar *)s;
	switch (ntohs(proto)) 
		{
		case TFTP_RRQ:
		case TFTP_WRQ:
		case TFTP_ACK:
			break;
		default:
			break;

		case TFTP_OACK:
			etdebug("Got OACK: %s %s\n", pkt, pkt+strlen(pkt)+1);
			TftpState = STATE_OACK;
			TftpServerPort = src;
			TftpSend (); /* Send ACK */
			break;
		case TFTP_DATA:
			TftpTimeoutCount = 0;
			if (len < 2)
				return;
			len -= 2;
			TftpBlock = ntohs(*(ushort *)pkt);
			/* RFC1350 specifies that the first data packet will have sequence number 1. If we receive a sequence number of 0 this means that there was a wrap around of the (16 bit) counter. */
			if (TftpBlock == 0) 
				{
				TftpBlockWrap++;
				TftpBlockWrapOffset += TFTP_BLOCK_SIZE * TFTP_SEQUENCE_SIZE;
				printf ("\n\t %lu MB received\n\t ", TftpBlockWrapOffset>>20);
				}
			else 
				{
				if (((TftpBlock - 1) % 10) == 0)
					printf("#");
				else if ((TftpBlock % (10 * HASHES_PER_LINE)) == 0)
					printf("\n\t ");
				}

			if (TftpState == STATE_RRQ)
				etdebug("Server did not acknowledge timeout option!\n");
			if (TftpState == STATE_RRQ || TftpState == STATE_OACK) 
				{/* first block received */
				TftpState = STATE_DATA;
				TftpServerPort = src;
				TftpLastBlock = 0;
				TftpBlockWrap = 0;
				TftpBlockWrapOffset = 0;
				if (TftpBlock != 1) 
					{	/* Assertion */
					printf("\nTFTP error: First block is not block 1 (%ld)\n Starting again\n\n", TftpBlock);
					NetStartAgain ();
					break;
					}
				}
			if (TftpBlock == TftpLastBlock)
				{ /* Same block again; ignore it. */
				break;
				}
			TftpLastBlock = TftpBlock;
			NetSetTimeout (TIMEOUT * CFG_HZ, TftpTimeout_Handler);
			store_block (TftpBlock - 1, pkt + 2, len);
			/* Acknoledge the block just received, which will prompt the server for the next one. */
			TftpSend ();
			if (len < TFTP_BLOCK_SIZE) 
				{/* We received the whole thing.  Try to run it. */
				printf("\nDone\n");
				NetState = NETLOOP_SUCCESS;
				NetStartAgain ();
				}
			break;

		case TFTP_ERROR:
			printf ("\nTFTP error: '%s' (%d)\n", pkt + 2, ntohs(*(ushort *)pkt));
			printf("Starting again\n\n");
			NetStartAgain ();
			break;
	}
}

void TftpStart(void)
{
#ifdef CONFIG_TFTP_PORT
	char *ep;             /* Environment pointer */
#endif

	if (BootFile[0] == '\0') 
		{
		sprintf(default_filename, "%02lX%02lX%02lX%02lX.img", NetOurIP & 0xFF, (NetOurIP >>  8) & 0xFF,	(NetOurIP >> 16) & 0xFF, (NetOurIP >> 24) & 0xFF);
		tftp_filename = default_filename;
		printf ("*** Warning: no boot file name; using '%s'\n",	tftp_filename);
		}
	else
		tftp_filename = BootFile;
#if defined(CONFIG_NET_MULTI)
	printf ("Using %s device\n", eth_get_name());
#endif
	printf("TFTP from server ");
	print_IPaddr(NetServerIP);
	printf("; our IP address is ");	
	print_IPaddr(NetOurIP);
	/* Check if we need to send across this subnet */
	if(NetOurGatewayIP && NetOurSubnetMask) 
		{
	    if((NetOurIP & NetOurSubnetMask) != (NetServerIP & NetOurSubnetMask)) 
			{
			printf("; sending through gateway ");
			print_IPaddr (NetOurGatewayIP) ;
	    	}
		}
	printf("\nFilename: %s\n", tftp_filename);
	if(NetBootFileSize) 
		{
		printf(" Size is 0x%x Bytes = ", NetBootFileSize<<9);
		print_size(NetBootFileSize<<9, "");
		}
	printf ("\nLoad address: 0x%lx\nLoading: *\b", load_addr);
	NetSetTimeout (TIMEOUT * CFG_HZ, TftpTimeout_Handler);
	NetSetHandler (TftpHandler);
	TftpServerPort = WELL_KNOWN_PORT;
	TftpTimeoutCount = 0;
	TftpState = STATE_RRQ;
	/* Use a pseudo-random port unless a specific port is set */
	TftpOurPort = (1024 + (get_timer(0) % 3072)) & 0xFFFF;
#ifdef CONFIG_TFTP_PORT
	if ((ep = getenv("tftpdstp")) != NULL)
		TftpServerPort = simple_strtol(ep, NULL, 10);
	if ((ep = getenv("tftpsrcp")) != NULL)
		TftpOurPort= simple_strtol(ep, NULL, 10);
#endif
	TftpBlock = 0;
	/* zero out server ether in case the server ip has changed */
	memset(NetServerEther, 0, 6);
	TftpSend();
}

#endif /* CFG_CMD_NET */
