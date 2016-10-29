/* Include files */
#include <common.h>
#include <net.h>
#include <command.h>
//#include <malloc.h>
#include "seth.h"

/* Simple ethernet AP code definition */
#include "simple_eth.h"

/* EMAC support */
#include "liu.h"

extern void * memset(void * s,int c,size_t count);

/* Configuration */

#define ETH_PORT_BASE   0xb9110000              // ETH Port base address

/* Useful internal macros */
#define ETH_CONTROL     *(volatile uint16_t *)(ETH_PORT_BASE + 0x300)
#define ETH_INT_STATUS  *(volatile uint16_t *)(ETH_PORT_BASE + 0x308)
#define ETH_GP_1        *(volatile uint16_t *)(ETH_PORT_BASE + 0x310)
#define ETH_GP_2        *(volatile uint16_t *)(ETH_PORT_BASE + 0x314)
#define ETH_EVENT_IN    *(volatile uint16_t *)(ETH_PORT_BASE + 0x318)

#define ETH_PMEM_BASE   ((volatile uint16_t *)(ETH_PORT_BASE + 0x4000))


#define _ETH_REG(x)     *(volatile uint32_t *)(ETH_PORT_BASE + EMOFF(x))

// Define a local EMAC-register offset macro
#define EMOFF(reg) (unsigned int)(&((LIU_REGISTERS *)0)->reg)
#define VAL(bit)                (1 << (bit ## _BIT))

/* Receive buffers */
static volatile struct simpleEthBuffer_s rxBufs[SA_RX_BUFS];
//static volatile struct simpleEthBuffer_s *rxBufs;
static struct simpleEthBuffer_s * volatile pRxBufs;

static unsigned int     rxNextBuf = 0;
static char isInitialized = 0;
/* Transmit buffer */
static volatile struct simpleEthBuffer_s txBuf;

/* ===========================================================================
 * Function name:       sethLoadAp()
 * Input Parameters:    none
 * Output Parameters:   none
 * Result:              nothing
 * Description:         Load the simple ethernet code into the AP for the
 * ===========================================================================
 */
static void
sethLoadAp(void)
{
    extern unsigned char        SIMPLE_codeSection[];
    extern unsigned int         SIMPLE_codeSectionLength;

    unsigned int                offset;

    /* Load the AP */
    for (       offset = 0 ; 
                offset < SIMPLE_codeSectionLength / sizeof(uint16_t);
                ++offset)
    {
        uint16_t        instr;

        instr =                 (SIMPLE_codeSection[offset * 2] << 8)                                           | SIMPLE_codeSection[offset * 2 + 1];
        ETH_PMEM_BASE[offset] = instr;
    }
}

/* ===========================================================================
 * Function name:       sethInitEmac()
 * Input Parameters:    none
 * Output Parameters:   none
 * Result:              nothing
 * Description:         Initialize the EMAC, but do not enable either Tx or
 *                      Rx operation
 * ===========================================================================
 */
static void
sethInitEmac(void)
{
    /* Setup the transmit configuration, but do not enable transmission
     */
    _ETH_REG(tx_ctrl) =         VAL(LIU_EnComp)         // Completion int
                                | VAL(LIU_EnUnder)      // Underrun int
                                | VAL(LIU_EnLateColl)   // Late collision
                                | VAL(LIU_EnLCarr)      // Lost carrier
                                | VAL(LIU_EnExdefer);   // Excess defers
                    

    /* Setup the receive config. Enable most errors but do not enable
     * reception
     */
    _ETH_REG(rx_ctrl) =         VAL(LIU_ShortEn)        // Short frames
                                | VAL(LIU_EnCRCErr)     // CRC err int
                                | VAL(LIU_EnOver)       // Overflow int
                                | VAL(LIU_EnLongErr)    // Long packet int
                                | VAL(LIU_IgnoreLen)    // Ignore 802.3 len
                                | VAL(LIU_EnGood);      // Good pkt int

    _ETH_REG(mac_ctrl) =        VAL(LIU_FullDup);       // Assume full duplex

    /* Set up the ARC to allow reception of all broadcast packets.
     * Allow unicast packets that match the ARC filter criteria
     */
    _ETH_REG(arc_ctl) = VAL(LIU_BroadAcc) | VAL(LIU_CmpEn);
} 

/* ===========================================================================
 * Function name:       sethSetAddr()
 * Input Parameters:    enetAddr        6 byte ethernet addr for this port
 * Output Parameters:   none
 * Result:              nothing
 * Description:         Initialize the unicast address of this port
 * ===========================================================================
 */
void
sethSetAddr(unsigned char addr[6])
{
    /* The ARC entry is six bytes. This is written into the ARC
     * as two, 32 bit words
     */
    _ETH_REG(arc_addr) = 0x78;
    _ETH_REG(arc_data) =        (addr[0] << 24)
                                | (addr[1] << 16)
                                | (addr[2] << 8)
                                | (addr[3] << 0);

    _ETH_REG(arc_addr) = 0x7C;
    _ETH_REG(arc_data) =        (addr[4] << 24)
                                | (addr[5] << 16);

    /* Enable the ARC entry just written */
    _ETH_REG(arc_en) |= (1 << 20);
}

/* ===========================================================================
 * Function name:       sethInit()
 * Input Parameters:    none
 * Output Parameters:   none
 * Result:              nothing
 * Description:         Initialize the ethernet port, AP, and driver.
 * ===========================================================================
 */
void
sethInit(void)
{
//    static char isInitialized = 0;

    if ( ! isInitialized)
    {
        /* First initialization */
        
        /* Initialize the EMAC */
        sethInitEmac();

        /* Load the AP code into the AP */
        sethLoadAp();

        /* Set the receive buffer address and start the AP */
        ETH_GP_1 = ((uint32_t)rxBufs >>16) & 0x1fff;
        ETH_GP_2 = (uint32_t)rxBufs & 0xffff;

        ETH_CONTROL = 0x4000;

        /* Enable MAC receive and transmit functions */
        _ETH_REG(tx_ctrl) |= VAL(LIU_TxEn);
        _ETH_REG(rx_ctrl) |= VAL(LIU_RxEn);

        /* Setup pointer to rx bufs in K1SEG space to avoid cache issues
         */
        pRxBufs = (void *)((unsigned int)rxBufs | 0x20000000);


        /* Remeber we have been initialized */
        isInitialized = 1;
    }
}

/* ===========================================================================
 * Function name:       sethPoll()
 * Input Parameters:    none
 * Output Parameters:   none
 * Result:              0 = no packets waiting, other = packet(s) waiting
 * Description:         Check to see if ethernet packets have been received
 * ===========================================================================
 */
int
sethPoll(void)
{
    /* Check the flag in the interrupt register that indicates whether
     * the next receive buffer has been filled
     */
    return ETH_INT_STATUS & (1 << rxNextBuf);
}

/* ===========================================================================
 * Function name:       sethReceive()
 * Input Parameters:    pBuf    Pointer to buffer to hold received packet
 * Output Parameters:   none
 * Result:              Packet length, or zero if no packet waiting
 * Description:         Collect a packet that has been received by the AP
 * ===========================================================================
 */
unsigned int
sethReceive(unsigned char *pBuf)
{
    unsigned short      length;
    int                 i;

    /* Make sure there is a packet to receive. Return 0 if there isn't
     */
    if (sethPoll() == 0)
        return 0;


    /* Copy the recieved packet contents into the supplied buffer */
    length = pRxBufs[rxNextBuf].length;
    memcpy(pBuf, pRxBufs[rxNextBuf].pkt, length);

    /* Release the buffer back to the ethernet AP. One wrinkle is that we
     * need to "hit" the status bit until it actually turns off. It seems
     * that simultaneous write to the status register from both the CP and
     * AP causes the CP access to be lost
     *
     * One problem here is that to do this we need to disable interrupts but
     * we can't use operating system services to do it (as we're OS
     * independent).
     *
     * Assembler to the rescue!
     */
    
    // Get the current interrupt state and disable interrupts
#define _GET_SR(x)    asm volatile ("mfc0 %0, $12; nop" : "=r" (x))
#define _SET_SR(x)    asm volatile ("mtc0 %0, $12; nop" : : "r" (x))

    _GET_SR(i);
    _SET_SR(i & ~0x0f00);
        
    do
    {
        ETH_INT_STATUS = (1 << rxNextBuf);
    } while (ETH_INT_STATUS & (1 << rxNextBuf));

    // Restore interrupts
    _SET_SR(i);

    rxNextBuf = (rxNextBuf + 1) % SA_RX_BUFS;

    /* Return length to caller */
    return length;
}

/* ===========================================================================
 * Function name:       sethTransmit()
 * Input Parameters:    length  Packet length
 *                      pBuf    Pointer to buffer to hold received packet
 * Output Parameters:   none
 * Result:              0 is sucessfully sent, error otherwise
 * Description:         Queue a packet for transmission. The packet is
 *                      copied so there is no need to retain the buffer
 *                      after this call returns
 * ===========================================================================
 */
unsigned int
sethTransmit(unsigned int length, unsigned char *pBuf)
{

    /* Wait for any previous packet transmission to complete. If we need to
     * wait then use a loop that does not saturate the bus
     */
    while (ETH_EVENT_IN & 0x1)
    {
        volatile unsigned int   count;

        for (count = 0 ; count < 100 ; ++count)
            ;
    }

    /* Make a copy of the packet to any changes after we return will not
     * effect the packet. Make sure the packet is a legal length
     */
    if (length > sizeof(txBuf.pkt))
        return 1;

    txBuf.length = length;
    memcpy((void*)txBuf.pkt, (void*)pBuf, length);

    /* Initiate the transmission. To minimize latency we don't wait
     * for the transfer to complete
     */
    ETH_GP_1 = (((uint32_t)&txBuf) >> 16) & 0x1fff;
    ETH_GP_2 = ((uint32_t)&txBuf) & 0xffff;
    ETH_EVENT_IN = 0x1;

    /* Indicate transmission queued OK */
    return 0;
}

static void voxEmac_halt(struct eth_device *dev)
{
//	ETH_CONTROL = 0x8000;
}

static int voxEmac_send(struct eth_device *dev, volatile void *ptr, int len)
{
	sethTransmit(len, (char*)ptr);
	return len;
}

static int voxEmac_rx(struct eth_device *dev)
{
	int length;
	char tmp[2048];
	
	for (;;) {
		length = sethReceive(tmp);
		if (!length) {
			length = -1;
			break;	/* nothing received - leave for() loop */
		}
		
		/* Pass the packet up to the protocol layers. */
		/*       NetReceive(NetRxPackets[rxIdx], length - 4); */
		/*       NetReceive(NetRxPackets[i], length); */
		NetReceive (tmp,length);
	}

	return length;
}	

static int voxEmac_init2 (struct eth_device *dev, bd_t * bis)
{ 
	int length;
	char tmp[2048];

	sethInit();
	sethSetAddr(dev->enetaddr);
	for (;;) {
		length = sethReceive(tmp);
		if (!length) {
			length = -1;
			break;	/* nothing received - leave for() loop */
		}
	}
	return 1;
}

int voxEmac_init(bd_t *bis)
{
	struct eth_device* dev;
/*
	rxBufs = malloc(SA_RX_BUFS * 2048);
	if(!rxBufs) {
		printf("Emac ERROR : memory not allocated\n");
		return;
	}
*/
	dev = (struct eth_device *) malloc (sizeof (*dev));
	if (dev == NULL) {
		printf ("voxEmac_init: "
			"Cannot allocate eth_device\n");
		return (-1);
	}
	memset(dev, 0, sizeof(*dev));

//	sethInit();

	strcpy(dev->name,"voxEmac");		
	dev->priv = NULL;
	dev->init = voxEmac_init2;
	dev->halt = voxEmac_halt;
	dev->send = voxEmac_send;
	dev->recv = voxEmac_rx;

	eth_register(dev);
	printf("Register vx160 eth device.\n");

	return 1;
}
