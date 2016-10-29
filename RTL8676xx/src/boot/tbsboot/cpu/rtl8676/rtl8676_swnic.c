#include <common.h>
#include <error.h>
#include <malloc.h>
#include "rtl8676_phy.h"
#include "rtl8676_asicregs.h"
#include "rtl8676_swnic.h"
#include "rtl8676_swcore.h"
#include "rtl8676_swvlan.h"


#define NUM_RX_PKTHDR_DESC          16 //80 //16
#define NUM_RX_MBUF_DESC            16
#define NUM_TX_PKTHDR_DESC          16
#define MBUF_LEN                    2048

#define UNCACHED_MALLOC(x)  (void *) (0xa0000000 | (u32) malloc(x))


int switch_debug_msg=0;
//char eth0_mac[6]; //by xiachaoren 2011-10-27
/* refer to rtl865xc_swNic.c & rtl865xc_swNic.h
 */
#define RTL865X_SWNIC_RXRING_MAX_PKTDESC	6
#ifdef CONFIG_RTL8196D
#define RTL865X_SWNIC_TXRING_MAX_PKTDESC	4
#else
#define RTL865X_SWNIC_TXRING_MAX_PKTDESC	2
#endif

#define USED_TX_RING		0	// must >= 0 and < RTL865X_SWNIC_TXRING_MAX_PKTDESC

/* RX Ring */
static u32*	rxPkthdrRing[RTL865X_SWNIC_RXRING_MAX_PKTDESC];					/* Point to the starting address of RX pkt Hdr Ring */
static u32	rxPkthdrRingCnt[RTL865X_SWNIC_RXRING_MAX_PKTDESC];				/* Total pkt count for each Rx descriptor Ring */
//static u32	rxPkthdrRingIndex[RTL865X_SWNIC_RXRING_MAX_PKTDESC];			/* Current Index for each Rx descriptor Ring */

/* TX Ring */
static u32*	txPkthdrRing[RTL865X_SWNIC_TXRING_MAX_PKTDESC];				/* Point to the starting address of TX pkt Hdr Ring */
static u32	txPkthdrRingCnt[RTL865X_SWNIC_TXRING_MAX_PKTDESC];			/* Total pkt count for each Tx descriptor Ring */
//static u32	txPkthdrRingFreeIndex[RTL865X_SWNIC_TXRING_MAX_PKTDESC];	/* Point to the entry can be set to SEND packet */

//#define txPktHdrRingFull(idx)	(((txPkthdrRingFreeIndex[idx] + 1) & (txPkthdrRingMaxIndex[idx])) == (txPkthdrRingDoneIndex[idx]))

/* Mbuf */
u32* rxMbufRing=NULL;                                                     /* Point to the starting address of MBUF Ring */
u32	rxMbufRingCnt;													/* Total MBUF count */

static u32  size_of_cluster;

/* descriptor ring tracing pointers */
static int   currRxPkthdrDescIndex;      /* Rx pkthdr descriptor to be handled by CPU */
static int   currRxMbufDescIndex;        /* Rx mbuf descriptor to be handled by CPU */
static int   currTxPkthdrDescIndex;      /* Tx pkthdr descriptor to be handled by CPU */
static int txPktDoneDescIndex;

/* debug counters */
static int   rxPktCounter;
static int   txPktCounter;

#if 0
#define ARPTAB_SIZ 16
//--------------------------------------------------------------------------
struct arptab_s
    {
    u32  port_list;
    u8   arp_mac_addr[6];  /* hardware address */
    u8   valid;
    u8   reserved;
    };
//--------------------------------------------------------------------------
static struct arptab_s arptab[ARPTAB_SIZ];
static u32 arptab_next_available;
#endif

#define     BUF_FREE            0x00   /* Buffer is Free  */
#define     BUF_USED            0x80   /* Buffer is occupied */
#define     BUF_ASICHOLD        0x80   /* Buffer is hold by ASIC */
#define     BUF_DRIVERHOLD      0xc0   /* Buffer is hold by driver */

//--------------------------------------------------------------------------
/* mbuf header associated with each cluster 
*/
struct mBuf
{
    struct mBuf *m_next;
    struct pktHdr *m_pkthdr;            /* Points to the pkthdr structure */
    u16    m_len;                    /* data bytes used in this cluster */
#if defined(CONFIG_RTL865XC) || defined(CONFIG_RTL8676)
    u16    m_flags;                  /* mbuf flags; see below */
#else
    char      m_flags;                  /* mbuf flags; see below */
#endif
#define MBUF_FREE            BUF_FREE   /* Free. Not occupied. should be on free list   */
#define MBUF_USED            BUF_USED   /* Buffer is occupied */
#define MBUF_EXT             0x10       /* has associated with an external cluster, this is always set. */
#define MBUF_PKTHDR          0x08       /* is the 1st mbuf of this packet */
#define MBUF_EOR             0x04       /* is the last mbuf of this packet. Set only by ASIC*/
    u8     *m_data;                  /*  location of data in the cluster */
    u8     *m_extbuf;                /* start of buffer*/
    u16    m_extsize;                /* sizeof the cluster */
    char      m_reserved[2];            /* padding */
};
//--------------------------------------------------------------------------
/* pkthdr records packet specific information. Each pkthdr is exactly 32 bytes.
 first 20 bytes are for ASIC, the rest 12 bytes are for driver and software usage.
*/
struct pktHdr
{
    union
    {
        struct pktHdr *pkthdr_next;     /*  next pkthdr in free list */
        struct mBuf *mbuf_first;        /*  1st mbuf of this pkt */
    }PKTHDRNXT;
#define ph_nextfree         PKTHDRNXT.pkthdr_next
#define ph_mbuf             PKTHDRNXT.mbuf_first
    u16    ph_len;                   /*   total packet length */
    u16    ph_reserved1: 1;           /* reserved */
    u16    ph_queueId: 3;            /* bit 2~0: Queue ID */
    u16    ph_extPortList: 4;        /* dest extension port list. must be 0 for TX */
    u16    ph_reserved2: 3;          /* reserved */
    u16    ph_hwFwd: 1;              /* hwFwd - copy from HSA bit 200 */
    u16    ph_isOriginal: 1;         /* isOriginal - DP included cpu port or more than one ext port */
    u16    ph_l2Trans: 1;            /* l2Trans - copy from HSA bit 129 */
    u16    ph_srcExtPortNum: 2;      /* Both in RX & TX. Source extension port number. */

    u16    ph_type: 3;
#define PKTHDR_ETHERNET      0
#define PKTHDR_IP            2
#define PKTHDR_ICMP          3
#define PKTHDR_IGMP          4
#define PKTHDR_TCP           5
#define PKTHDR_UDP           6
    u16    ph_vlanTagged: 1;         /* the tag status after ALE */
    u16    ph_LLCTagged: 1;          /* the tag status after ALE */
    u16    ph_pppeTagged: 1;         /* the tag status after ALE */
    u16    ph_pppoeIdx: 3;
    u16    ph_linkID: 7;             /* for WLAN WDS multiple tunnel */
    u16    ph_reason;                /* indicates wht the packet is received by CPU */

    u16    ph_flags;                 /*  NEW:Packet header status bits */
#define PKTHDR_FREE          (BUF_FREE << 8)        /* Free. Not occupied. should be on free list   */
#define PKTHDR_USED          (BUF_USED << 8)
#define PKTHDR_ASICHOLD      (BUF_ASICHOLD<<8)      /* Hold by ASIC */
#define PKTHDR_DRIVERHOLD    (BUF_DRIVERHOLD<<8)    /* Hold by driver */
#define PKTHDR_CPU_OWNED     0x4000
#define PKT_INCOMING         0x1000     /* Incoming: packet is incoming */
#define PKT_OUTGOING         0x0800     /*  Outgoing: packet is outgoing */
#define PKT_BCAST            0x0100     /*send/received as link-level broadcast  */
#define PKT_MCAST            0x0080     /*send/received as link-level multicast   */
#define PKTHDR_PPPOE_AUTOADD    0x0004  /* PPPoE header auto-add */
#define CSUM_TCPUDP_OK       0x0001     /*Incoming:TCP or UDP cksum checked */
#define CSUM_IP_OK           0x0002     /* Incoming: IP header cksum has checked */
#define CSUM_TCPUDP          0x0001     /*Outgoing:TCP or UDP cksum offload to ASIC*/
#define CSUM_IP              0x0002     /* Outgoing: IP header cksum offload to ASIC*/

   u8      ph_orgtos;                /* RX: original TOS of IP header's value before remarking, TX: undefined */
   u8      ph_portlist;              /* RX: source port number, TX: destination portmask */

   u16     ph_vlanId_resv: 1;
   u16     ph_txPriority: 3;
   u16     ph_vlanId: 12;
   u16     ph_flags2;
#ifdef CONFIG_RTL8196D
   //char        ph_reserved[3];            /* padding */

//   u8      ph_ptp_flags;	// only valid @NIC-TX :
	u8      ph_ptpResv:1;
	u8      ph_ptpMsgType:4;	// message type.
	u8      ph_ptpVer:2;	// PTP version, 0: 1588v1; 1: 1588v2 or 802.1as; others: reserved.
	u8      ph_ptpPkt:1;	// 1: PTP
	char        ph_reserved[3];            /* padding */
#endif
};
//--------------------------------------------------------------------------



//#pragma ghs section text=".iram"
/*************************************************************************
*   FUNCTION                                                              
*       swNic_intHandler                                         
*                                                                         
*   DESCRIPTION                                                           
*       This function is the handler of NIC interrupts
*                                                                         
*   INPUTS                                                                
*       intPending      Pending interrupt sources.
*                                                                         
*   OUTPUTS                                                               
*       None
*************************************************************************/
void swNic_intHandler(u32 intPending) {return;}


//-------------------------------------------------------------------------
void dump_rx_desc_own_bit(void) 
{ 
	int idx; 
	printf("---rx desc-------------------- \n");
	printf("---rxPktCounter= %d\n", rxPktCounter);
	
	for (idx=0; idx<rxPkthdrRingCnt[0]; idx++)
	{
		printf(".%d-%d.", (rxPkthdrRing[0][idx] & 3), (rxMbufRing[idx] & 3)); 
	} 
	printf("(%d %d)\n", currRxPkthdrDescIndex, currRxMbufDescIndex); 
} 

//--------------------------------------------------------------------
void dump_tx_desc_own_bit(void) 
{ 
	int idx; 
	printf("---tx desc-------------------- \n");  
	printf("---txPktCounter= %d\n", txPktCounter);

	for (idx=0; idx<txPkthdrRingCnt[USED_TX_RING]; idx++) 
	{
		printf(".%d.", (txPkthdrRing[USED_TX_RING][idx] & 3)); 
	}     
	printf("(%d %d)\n", currTxPkthdrDescIndex, txPktDoneDescIndex); 
} 
//------------------------------------------------------------------------

void dcache_flush(void)
{
	__asm__ volatile(
		".set  push\n\t"
		".set  noreorder\n\t"
		"mfc0    $8, $20\n\t"
		"ori     $8, 0x200\n\t"
		"xori    $9, $8, 0x200\n\t"
		"mtc0    $9, $20\n\t"
		"mtc0    $8, $20\n\t"
		".set pop\n"
		"nop"
		: /* no output */
		: /* no input */
		);
}

/*************************************************************************
*   FUNCTION                                                              
*       swNic_receive                                         
*                                                                         
*   DESCRIPTION                                                           
*       This function reads one packet from rx descriptors, and return the 
*       previous read one to the switch core. This mechanism is based on 
*       the assumption that packets are read only when the handling 
*       previous read one is done.
*                                                                         
*   INPUTS                                                                
*       None
*                                                                         
*   OUTPUTS                                                               
*       None
*************************************************************************/
int swNic_receive(void** input, u32* pLen)
{
    struct pktHdr * pPkthdr;
    int pkthdr_index;
    int mbuf_index;
    static int firstTime = 1;
    unsigned char *data;
    int	ret = -1;
//	extern char eth0_mac[6];
		
    /* Check OWN bit of descriptors */
    if((rxPkthdrRing[0][currRxPkthdrDescIndex] & DESC_OWNED_BIT) == DESC_RISC_OWNED)
    	{	/* Fetch pkthdr */
        pPkthdr = (struct pktHdr *)(rxPkthdrRing[0][currRxPkthdrDescIndex] & (~(DESC_OWNED_BIT | DESC_WRAP)));
        /* Increment counter */
        rxPktCounter++;
		data = pPkthdr->ph_mbuf->m_data;
		
#if 0
		if((data[0]&0x1)||!memcmp(data, eth0_mac, 6))
			{/* Output packet */
			*input = data;
			*pLen = pPkthdr->ph_len - 4;
			ret = 0;
			}
		if(_start_dump == 1)
			{
			printf("rx pkthdr=%x mbuf=%x mddata=%x\n", pPkthdr, pPkthdr->ph_mbuf, pPkthdr->ph_mbuf->m_data);
			if (*pLen > 64)
				;//ddump(*input, 64);
			else
				;//ddump(*input, *pLen);
			}
#else
		/* Output packet */
		*input = data;
		*pLen = pPkthdr->ph_len - 4;
		ret = 0;
		etdebug("rx pkthdr=%x mbuf=%x mddata=%x\n", pPkthdr, pPkthdr->ph_mbuf, pPkthdr->ph_mbuf->m_data);

#endif

#if 1
	if(!firstTime)
		{         /* Calculate previous pkthdr and mbuf index */
		pkthdr_index = currRxPkthdrDescIndex;
		if(--pkthdr_index < 0)
			pkthdr_index = rxPkthdrRingCnt[0] - 1;

#ifdef CONFIG_RTL8196D
		// for rx descriptor runout
		pPkthdr = (struct pktHdr *) (rxPkthdrRing[0][pkthdr_index] & ~(DESC_OWNED_BIT | DESC_WRAP));
		mbuf_index = ((u32)(pPkthdr->ph_mbuf) - (rxMbufRing[0] & ~(DESC_OWNED_BIT | DESC_WRAP))) / (sizeof(struct mBuf));
#else
        mbuf_index = currRxMbufDescIndex;
		if ( --mbuf_index < 0 )
			mbuf_index = rxPkthdrRingCnt[0] - 1;
#endif
		/* flush dcache*/
		dcache_flush();
		/* Reset OWN bit */
		rxPkthdrRing[0][pkthdr_index] |= DESC_SWCORE_OWNED;
		rxMbufRing[mbuf_index] |= DESC_SWCORE_OWNED;
		}
	else
		{
		firstTime = 0;
		}
#endif	
	/* Increment index */
	if(++currRxPkthdrDescIndex == rxPkthdrRingCnt[0])
		currRxPkthdrDescIndex = 0;
	if(++currRxMbufDescIndex == rxMbufRingCnt)
		currRxMbufDescIndex = 0;
	if(REG32(CPUIISR) & PKTHDR_DESC_RUNOUT_IP_ALL)
		{/* Enable and clear interrupt for continue reception */
		REG32(CPUIIMR) |= PKTHDR_DESC_RUNOUT_IE_ALL;
		REG32(CPUIISR) = PKTHDR_DESC_RUNOUT_IP_ALL;
		}
	}

	return ret;
}



u8 pktbuf[2048];

/*************************************************************************
*   FUNCTION                                                              
*       swNic_send                                         
*                                                                         
*   DESCRIPTION                                                           
*       This function writes one packet to tx descriptors, and waits until 
*       the packet is successfully sent.
*                                                                         
*   INPUTS                                                                
*       None
*                                                                         
*   OUTPUTS                                                               
*       None
*************************************************************************/
int swNic_send(volatile void *output, int len)
{
	struct pktHdr * pPkthdr;
	u8* pktbuf_alligned;

	int next_index;
	if ((currTxPkthdrDescIndex+1) == txPkthdrRingCnt[USED_TX_RING])
		next_index = 0;
	else
		next_index = currTxPkthdrDescIndex+1;
	if (next_index == txPktDoneDescIndex) {
		printf("Tx Desc full!\n");
		return -1;
	}		

#ifdef _FAST_TX
	pktbuf_alligned = (u8 *)output;
#else
	pktbuf_alligned = (u8 *) (( (u32) pktbuf & 0xfffffffc) | 0xa0000000);
	/* Copy Packet Content */
	memcpy(pktbuf_alligned, (const void *)output, len);
#endif

#if 0
//	if(switch_debug_msg==1)
	if(_start_dump==1)
	{
		printf("\r\ntx\r\n");
		if (len > 64)
			;//ddump(output, 64);
		else
			;//ddump(output, len);
	}
#endif

    /* Fetch packet header from Tx ring */
    pPkthdr = (struct pktHdr *) ((int) txPkthdrRing[USED_TX_RING][currTxPkthdrDescIndex] 
                                                & ~(DESC_OWNED_BIT | DESC_WRAP));

    /* Pad small packets and add CRC */
	if ( len < 60 )
		pPkthdr->ph_len = 64;
	else
		pPkthdr->ph_len = len + 4;
	pPkthdr->ph_mbuf->m_len       = pPkthdr->ph_len;
	pPkthdr->ph_mbuf->m_extsize = pPkthdr->ph_len;

    /* Set cluster pointer to buffer */
	pPkthdr->ph_mbuf->m_data    = pktbuf_alligned;
	pPkthdr->ph_mbuf->m_extbuf = pktbuf_alligned;


#ifdef CONFIG_RTL8196D
	{
	unsigned char *p = (unsigned char *)output;
	if (p[12] == 0x88 && p[13] == 0xF7) {
		pPkthdr->ph_ptpPkt = 1;
		pPkthdr->ph_ptpVer = 1; // p[15] & 0xF; // for tx pkt: phtdkr->ph_ptpVer = 1, but packet content must be 2
		pPkthdr->ph_ptpMsgType = p[14] & 0xF;
		
		printf(" PTP pkt: %d %d\n",pPkthdr->ph_ptpVer, pPkthdr->ph_ptpMsgType);		
	}
	else {
		pPkthdr->ph_ptpPkt = 0;
	}
	}
#endif

    /* Set destination port */
	pPkthdr->ph_portlist = ALL_PORT_MASK;
	/* flush dcache*/
	dcache_flush();		
    /* Give descriptor to switch core */
    txPkthdrRing[USED_TX_RING][currTxPkthdrDescIndex] |= DESC_SWCORE_OWNED;

    /* Set TXFD bit to start send */
    REG32(CPUICR) |= TXFD;
    txPktCounter++;

	currTxPkthdrDescIndex = next_index;
    return 0;
}

void swNic_txDone(void)
{
	struct pktHdr * pPkthdr;
	
	while (txPktDoneDescIndex != currTxPkthdrDescIndex) {		
	    if ( (*(volatile u32 *)&txPkthdrRing[USED_TX_RING][txPktDoneDescIndex] 
                    & DESC_OWNED_BIT) == DESC_RISC_OWNED ) 
            {										
		    pPkthdr = (struct pktHdr *) ((int) txPkthdrRing[USED_TX_RING][txPktDoneDescIndex] 
                                                & ~(DESC_OWNED_BIT | DESC_WRAP));

			if (++txPktDoneDescIndex == txPkthdrRingCnt[USED_TX_RING])
				txPktDoneDescIndex = 0;
		}
		else
			break;
	}
}

#ifdef	CONFIG_RTL865X_MODEL_TEST_FT2
int swNic_send_portmbr(void * output, u32 len, u32 portmbr)
{
    struct pktHdr * pPkthdr;
    u8 pktbuf[2048];
    u8* pktbuf_alligned = (u8*) (( (u32) pktbuf & 0xfffffffc) | 0xa0000000);

    /* Copy Packet Content */
    memcpy(pktbuf_alligned, output, len);


    /* Fetch packet header from Tx ring */
    pPkthdr = (struct pktHdr *) ((int) txPkthdrRing[0][currTxPkthdrDescIndex] 
                                                & ~(DESC_OWNED_BIT | DESC_WRAP));

    /* Pad small packets and add CRC */
    if ( len < 60 )
        pPkthdr->ph_len = 64;
    else
        pPkthdr->ph_len = len + 4;

    pPkthdr->ph_mbuf->m_len = pPkthdr->ph_len;
    pPkthdr->ph_mbuf->m_extsize = pPkthdr->ph_len;

    /* Set cluster pointer to buffer */
    pPkthdr->ph_mbuf->m_data = pktbuf_alligned;
    pPkthdr->ph_mbuf->m_extbuf = pktbuf_alligned;

    /* Set destination port */
    pPkthdr->ph_portlist = portmbr;

    /* Give descriptor to switch core */
    txPkthdrRing[0][currTxPkthdrDescIndex] |= DESC_SWCORE_OWNED;

    /* Set TXFD bit to start send */
    REG32(CPUICR) |= TXFD;
    
    /* Wait until packet is successfully sent */
    while ( (*(volatile u32 *)&txPkthdrRing[0][currTxPkthdrDescIndex] 
                    & DESC_OWNED_BIT) == DESC_SWCORE_OWNED );
    
    txPktCounter++;
    
    if ( ++currTxPkthdrDescIndex == txPkthdrRingCnt[0] )
        currTxPkthdrDescIndex = 0;

    return 0;
}
#endif



//#pragma ghs section text=default

/*************************************************************************
*   FUNCTION DESCRIPTION                                                           
*       This function initializes descriptors and data structures.
*                                                                         
*   INPUTS                                                                
*       userNeedRxPkthdrRingCnt[RTL865X_SWNIC_RXRING_MAX_PKTDESC] :
*          Number of Rx pkthdr descriptors of each ring.
*       userNeedRxMbufRingCnt :
*          Number of Tx mbuf descriptors.
*       userNeedTxPkthdrRingCnt[RTL865X_SWNIC_TXRING_MAX_PKTDESC] :
*          Number of Tx pkthdr descriptors of each ring.
*       clusterSize :
*          Size of cluster.
*                                                                         
*   OUTPUTS                                                               
*       Status.
*************************************************************************/

int swNic_init(u32 userNeedRxPkthdrRingCnt[RTL865X_SWNIC_RXRING_MAX_PKTDESC],
                 u32 userNeedRxMbufRingCnt,
                 u32 userNeedTxPkthdrRingCnt[RTL865X_SWNIC_TXRING_MAX_PKTDESC],
                 u32 clusterSize)
{
    u32 i, j, k;
	u32 totalRxPkthdrRingCnt = 0, totalTxPkthdrRingCnt = 0;
    struct pktHdr *pPkthdrList;
    struct mBuf *pMbufList;
    u8 * pClusterList;
    struct pktHdr * pPkthdr;
    struct mBuf * pMbuf;

    /* Cluster size is always 2048 */
    size_of_cluster = 2048;

    /* Allocate Rx descriptors of rings */
    for(i = 0; i < RTL865X_SWNIC_RXRING_MAX_PKTDESC; i++)
		{
		rxPkthdrRingCnt[i] = userNeedRxPkthdrRingCnt[i];
		if(rxPkthdrRingCnt[i] == 0)
			{
			continue;
			}
		rxPkthdrRing[i] = (u32 *)UNCACHED_MALLOC(rxPkthdrRingCnt[i] * sizeof(u32));
		totalRxPkthdrRingCnt += rxPkthdrRingCnt[i];
		}
	
	if(totalRxPkthdrRingCnt == 0)
		{
		return EINVAL;
		}
    /* Allocate Tx descriptors of rings */
    for(i = 0; i < RTL865X_SWNIC_TXRING_MAX_PKTDESC; i++)
		{
		txPkthdrRingCnt[i] = userNeedTxPkthdrRingCnt[i];
		if(txPkthdrRingCnt[i] == 0)
			{
			continue;
			}
		txPkthdrRing[i] = (u32 *) UNCACHED_MALLOC(txPkthdrRingCnt[i] * sizeof(u32));
		totalTxPkthdrRingCnt += txPkthdrRingCnt[i];
		}
	if(totalTxPkthdrRingCnt == 0)
		{
		return EINVAL;
		}
    /* Allocate MBuf descriptors of rings */
	rxMbufRingCnt = userNeedRxMbufRingCnt;
	if (userNeedRxMbufRingCnt == 0)
		{
		return EINVAL;
		}
	rxMbufRing = (u32 *) UNCACHED_MALLOC(userNeedRxMbufRingCnt * sizeof(u32));
    /* Allocate pkthdr */
    pPkthdrList = (struct pktHdr *)UNCACHED_MALLOC((totalRxPkthdrRingCnt + totalTxPkthdrRingCnt) * sizeof(struct pktHdr));                    
    /* Allocate mbufs */
    pMbufList = (struct mBuf *) UNCACHED_MALLOC((rxMbufRingCnt + totalTxPkthdrRingCnt) * sizeof(struct mBuf));                    
    /* Allocate clusters */
    pClusterList = (u8 *) UNCACHED_MALLOC(rxMbufRingCnt * size_of_cluster + 8 - 1+2*rxMbufRingCnt);
    pClusterList = (u8*)(((u32) pClusterList + 8 - 1) & ~(8 - 1));
    /* Initialize interrupt statistics counter */
    rxPktCounter = txPktCounter = 0;
    /* Initialize index of Tx pkthdr descriptor */
    currTxPkthdrDescIndex = 0;
    txPktDoneDescIndex=0;
    /* Initialize Tx packet header descriptors */
    for(i = 0; i < RTL865X_SWNIC_TXRING_MAX_PKTDESC; i++)
		{
		for(j = 0; j < txPkthdrRingCnt[i]; j++)
			{/* Dequeue pkthdr and mbuf */
			pPkthdr = pPkthdrList++;
			pMbuf = pMbufList++;
			memset((void *) pPkthdr, 0, sizeof(struct pktHdr));
			memset((void *) pMbuf, 0, sizeof(struct mBuf));
			pPkthdr->ph_mbuf = pMbuf;
			pPkthdr->ph_len = 0;
			pPkthdr->ph_flags = PKTHDR_USED | PKT_OUTGOING;
			pPkthdr->ph_type = PKTHDR_ETHERNET;
			pPkthdr->ph_portlist = 0;
			pMbuf->m_next = NULL;
			pMbuf->m_pkthdr = pPkthdr;
			pMbuf->m_flags = MBUF_USED | MBUF_EXT | MBUF_PKTHDR | MBUF_EOR;
			pMbuf->m_data = NULL;
			pMbuf->m_extbuf = NULL;
			pMbuf->m_extsize = 0;
			txPkthdrRing[i][j] = (int) pPkthdr | DESC_RISC_OWNED;
			}

		/* Set wrap bit of the last descriptor */
		if(txPkthdrRingCnt[i] != 0)
			{
			txPkthdrRing[i][txPkthdrRingCnt[i] - 1] |= DESC_WRAP;
			}
		}
    /* Fill Tx packet header FDP */
    REG32(CPUTPDCR0) = (u32) txPkthdrRing[0];
    REG32(CPUTPDCR1) = (u32) txPkthdrRing[1];
#ifdef CONFIG_RTL8196D
    REG32(CPUTPDCR2) = (u32) txPkthdrRing[2];
    REG32(CPUTPDCR3) = (u32) txPkthdrRing[3];
#endif
    /* Initialize index of current Rx pkthdr descriptor */
    currRxPkthdrDescIndex = 0;
    /* Initialize index of current Rx Mbuf descriptor */
    currRxMbufDescIndex = 0;
    /* Initialize Rx packet header descriptors */
	k = 0;
    for(i = 0; i < RTL865X_SWNIC_RXRING_MAX_PKTDESC; i++)
		{
		for(j = 0; j < rxPkthdrRingCnt[i]; j++)
			{/* Dequeue pkthdr and mbuf */
			pPkthdr = pPkthdrList++;
			pMbuf = pMbufList++;
			memset((void *) pPkthdr, 0, sizeof(struct pktHdr));
			memset((void *) pMbuf, 0, sizeof(struct mBuf));
			/* Setup pkthdr and mbuf */
			pPkthdr->ph_mbuf = pMbuf;
			pPkthdr->ph_len = 0;
			pPkthdr->ph_flags = PKTHDR_USED | PKT_INCOMING;
			pPkthdr->ph_type = PKTHDR_ETHERNET;
			pPkthdr->ph_portlist = 0;
			pMbuf->m_next = NULL;
			pMbuf->m_pkthdr = pPkthdr;
			pMbuf->m_len = 0;
			pMbuf->m_flags = MBUF_USED | MBUF_EXT | MBUF_PKTHDR | MBUF_EOR;
			pMbuf->m_data = NULL;
			pMbuf->m_extsize = size_of_cluster;
			/*offset 2 bytes for 4 bytes align of ip packet*/
			pMbuf->m_data = pMbuf->m_extbuf = (pClusterList+2);
			pClusterList += size_of_cluster;			
			/* Setup descriptors */
			rxPkthdrRing[i][j] = (int) pPkthdr | DESC_SWCORE_OWNED;
			rxMbufRing[k++] = (int) pMbuf | DESC_SWCORE_OWNED;
			}
		/* Set wrap bit of the last descriptor */
		if(rxPkthdrRingCnt[i] != 0)
			{
			rxPkthdrRing[i][rxPkthdrRingCnt[i] - 1] |= DESC_WRAP;
			}
        /* Fill Rx packet header FDP */
        REG32(CPURPDCR(i)) = (u32) rxPkthdrRing[i];
		}

	/* flush dcache*/
	dcache_flush();	
	rxMbufRing[rxMbufRingCnt - 1] |= DESC_WRAP;
    REG32(CPURMDCR0) = (u32) rxMbufRing;
// for debug
#if 0
    /* Initialize ARP table */
    memset((void *) arptab, 0, ARPTAB_SIZ * sizeof(struct arptab_s));
    arptab_next_available = 0;
#endif

	//dprintf("addr=%x, val=%x\r\n",(CPUIIMR),REG32(CPUIIMR));
    /* Enable runout interrupts */
    //REG32(CPUIIMR) |= RX_ERR_IE_ALL | TX_ERR_IE_ALL | PKTHDR_DESC_RUNOUT_IE_ALL;  //8651c
	//REG32(CPUIIMR) = 0xffffffff; //RX_DONE_IE_ALL;  //   0xffffffff;  //wei test irq
	
	//*(volatile unsigned int*)(0xb8010028)=0xffffffff;	
	//dprintf("eth0 CPUIIMR status=%x\r\n", *(volatile unsigned int*)(0xb8010028));   //ISR	
	
    /* Enable Rx & Tx. Config bus burst size and mbuf size. */
    //REG32(CPUICR) = TXCMD | RXCMD | BUSBURST_256WORDS | icr_mbufsize;
	REG32(CPUICR) = TXCMD | RXCMD | BUSBURST_32WORDS | MBUF_2048BYTES; //wei test irq
	REG32(CPUIIMR) = RX_DONE_IE_ALL|TX_DONE_IE_ALL; 

//	*(volatile unsigned int*)(0xbb804004)=0x96181441;	// enable Giga port 8211B LED

	//dprintf("eth0 CPUIIMR status=%x\r\n", *(volatile unsigned int*)(0xb8010028));   //ISR
	
    return 0;
}


#ifdef FAT_CODE
/*************************************************************************
*   FUNCTION                                                              
*       swNic_resetDescriptors                                         
*                                                                         
*   DESCRIPTION                                                           
*       This function resets descriptors.
*                                                                         
*   INPUTS                                                                
*       None.
*                                                                         
*   OUTPUTS                                                               
*       None.
*************************************************************************/
void swNic_resetDescriptors(void)
{
    /* Disable Tx/Rx and reset all descriptors */
    REG32(CPUICR) &= ~(TXCMD | RXCMD);
}
#endif//FAT_CODE

void swInit(volatile unsigned char* mac)
{
	struct vlan_param vp;
	int ret;
	struct netif_param np;
	u32 rx[6] = {NUM_RX_PKTHDR_DESC, 0, 0, 0, 0, 0};

#ifdef CONFIG_RTL8196D
//        u32 tx[4] = {4, 2, 2, 2};
	u32 tx[4] = {4, 4, 4, 4};
#else
	u32 tx[2] = {4, 2};
#endif
//	memcpy(eth0_mac, mac, 6);
	ret = swCore_init(mac);
	if(0 != ret)
		{  	
		printf("\nSwitch core initialization failed!\n");        
		return;
		}	
	ret = swNic_init(rx, NUM_RX_PKTHDR_DESC, tx, MBUF_LEN);	
	if(0 != ret)
		{        /* Initialize NIC module */
		printf("\nSwitch nic initialization failed!\n");            
		return;
		}
	/* Create Netif */
	memset((void *) &np, 0, sizeof(struct netif_param));
	np.vid = 8;
	np.valid = 1;
	np.enableRoute = 1;
	np.inAclEnd = 1;
	np.inAclStart = 0;
	np.outAclEnd = 0;
	np.outAclStart = 0;
	memcpy(&np.gMac, (const void *)mac, 6);
	np.macAddrNumber = 1;
	np.mtu = 1500;
	ret = swCore_netifCreate(0, &np); 
	if(0 != ret)
		{
		printf("Creating netif fails:%d\n", ret);
		return;
        }
	/* Create vlan */
	memset((void *)&vp, 0, sizeof(struct vlan_param));
	vp.egressUntag = ALL_PORT_MASK;
	vp.memberPort = ALL_PORT_MASK;
	ret = swCore_vlanCreate(8, &vp);
	if(0 != ret)
		{       
		printf("Creating vlan fails:%d\n", ret);
        }
	REG32(CPUIIMR) |= LINK_CHANGE_IE;
}

int is_link_change(void)
{
	int ret_val = 0;
	u32 status;

	status = REG32(CPUIISR);
	REG32(CPUIISR) = status;
	status &= REG32(CPUIIMR);
	
	if(status & LINK_CHANGE_IE)
		{		
		ret_val = 1;
		}
	
	return ret_val;
}

