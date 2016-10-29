/* ###########################################################################
#
#      cf_boot for Bandit Family: Ethernet driver
#      ==========================================
# ##########################################################################*/

#include <common.h>
#include <net.h>
//#include <malloc.h>
#include "solosw_mac.h"

/* Define to print debug information */
//#define DEBUG_ETH

/*
 *  Macro definitions
 */

/* Debugging macros */
#ifdef DEBUG_ETH
#define DBG_ARGS(_x, _y...) printf("%s: " _x, __FUNCTION__, _y)
#define DBG(_x) printf("%s: "_x, __FUNCTION__)
#else
#define DBG_ARGS(_x, _y...)
#define DBG(_x)
#endif

/* Macros to access registers */

/* TBD - get the base address more sensibly */
#define BF_ETHER_ADDR(_r) (volatile U32 *)( BF_VMAC_0 + offsetof(BF_VMAC_REGS, _r))
#define BF_ETHER_READ(_r) *BF_ETHER_ADDR(_r)
#define BF_ETHER_WRITE(_r, _v) *BF_ETHER_ADDR(_r) = _v


/* General macros */
#define BUFF_SIZE       1536
#define ADDR_MASK       (0x1fffffff)

/*
 *  Type definitions
 */

typedef struct sBF_EtherDesc
{
    U32 info;
    U8* ptr;
} tBF_EtherDesc;

/* Local variables */
static tBF_EtherDesc *rx_desc = NULL;
static tBF_EtherDesc *tx_desc = NULL;
static BYTE *rx_buf = NULL;

/*
 * Function prototypes
 */
static void phy_init(void);
static void phy_term(void);
static BYTE *solosw_eth_init(BYTE *mem, BYTE *macaddr);
static BYTE *solosw_eth_rx(int *len);
static int  solosw_eth_tx(BYTE *buffer, int len);
static void eth_enable(void);
static void eth_free(BYTE*);
static void solosw_eth_term(void);

int solosw_initialize(bd_t *bis);
static int solosw_probe(struct eth_device *dev, bd_t *bis);
static int solosw_recv(struct eth_device *dev);
static int solosw_send(struct eth_device *dev, volatile void *packet, int length);
static void solosw_halt(struct eth_device *dev);



/* 
 * Driver routines
 */


/*
 *  solosw_eth_init
 *
 *  Initialise Ethernet block, and descriptors
 */
BYTE *solosw_eth_init(BYTE *mem, BYTE *macaddr)
{
    U32 tmp;

    /*
     *  Firstly, check the hardware
     */

    DBG_ARGS("Eth base address at 0x%08x\n", BF_ETHER_ADDR(ID));

    /* Get ID from MAC block, and check it is correct */
    tmp = BF_ETHER_READ(ID);

    DBG_ARGS("ID 0x%x\n", tmp);

    if (tmp != BF_VMAC_ID_VALUE )
    {
        printf("%s: Read back %08x from ID. Wanted %08x\n", __FUNCTION__, tmp, BF_VMAC_ID_VALUE );
        return mem;
    }

    /* Work around for bits not cleared at reset in the hardware */
    tmp = BF_ETHER_READ(XTRACTRL );
    tmp &= ~BF_VMAC_XTRACTRL_RESETMASK;
    BF_ETHER_WRITE(XTRACTRL, tmp );

    /*
     *  Set up descriptors and buffers
     */

    /* Tx descriptor */
    mem -= sizeof(tBF_EtherDesc);
    tx_desc = (tBF_EtherDesc*)mem;
    tx_desc->info = BF_VMAC_TXINFO_CPU_FIRST | BF_VMAC_TXINFO_LAST;
    tx_desc->ptr = 0;

    /* Rx descriptor */
    mem -= sizeof(tBF_EtherDesc);
    rx_desc =(tBF_EtherDesc*) mem;

    /* Rx buffer */
    mem -= BUFF_SIZE;
    rx_buf = mem;
    rx_desc->ptr = (U8*)((U32)rx_buf & ADDR_MASK);
    rx_desc->info = BUFF_SIZE | BF_VMAC_RXINFO_OWN;



    /*
     * Set up the Ethernet hardware
     */
    /* Set up MAC */
    BF_ETHER_WRITE( CONTROL,
                    BF_VMAC_CONTROL_ENBFULL  |            /* Use full duplex */
                    BF_VMAC_CONTROL_PROM     |            /* Test - use promiscous */
                    1 << BF_VMAC_CONTROL_TXBDTLEN_SHIFT | /* One tx descriptor */
                    1 << BF_VMAC_CONTROL_RXBDTLEN_SHIFT );/* One rx descriptor */

    /* Set the poll rate */
    BF_ETHER_WRITE(POLLRATE, BF_VMAC_POLLRATE_MASK ); /* Maximum time? */

    /* Set up descriptor ring pointers */
    BF_ETHER_WRITE(TXRINGPTR, (U32)tx_desc & ADDR_MASK );
    BF_ETHER_WRITE(RXRINGPTR, (U32)rx_desc & ADDR_MASK );

    
    /* Set up MAC address */
    tmp =   (macaddr[3] << 24)
          | (macaddr[2] << 16)
          | (macaddr[1] <<  8)
          | (macaddr[0] <<  0);

    BF_ETHER_WRITE(ADDRL, tmp );

    tmp =   (macaddr[5] << 8)
          | (macaddr[4] << 0);

    BF_ETHER_WRITE( ADDRH, tmp );
    
    /* Clear the multicast filter */
    BF_ETHER_WRITE(LAFL, 0 );
    BF_ETHER_WRITE(LAFH, 0 );

    /* Set to PHY mode */
    *(volatile U32 *)(SOLOS_DEVCONF_BASE + SOLOS_DEVCONF_SOLOSW) = 0x01f00102;


    /* Reset the MAC */
    solosw_eth_term(); 
    eth_enable();


#ifdef CONFIG_SWITCH_RTL8306

    unsigned int request;
    unsigned int status;
    int phy = 6;
    int reg = 22;
    unsigned int value = 0x873F;   /* Referrence RTL8306 datasheet page 76,set reg22.15=1  */

    request = SOLOS_ETHER_MDIO_OP_WRITE | SOLOS_ETHER_MDIO_SFD | SOLOS_ETHER_MDIO_TA    /* Set up a write */
        | (phy << SOLOS_ETHER_MDIO_PHY_SHIFT) | (reg <<
                             SOLOS_ETHER_MDIO_REG_SHIFT)
        | value;

    *(volatile U32 *)(BF_VMAC_0 + BF_VMAC_MDIO) = request;

    do{
        status = *(volatile U32 *)(BF_VMAC_0 + BF_VMAC_STAT);
    }while(!(status & BF_VMAC_STAT_MDIO));

    *(volatile U32 *)(BF_VMAC_0 + BF_VMAC_STAT) = BF_VMAC_STAT_MDIO;

    udelay(1*CFG_HZ);


#endif


    return mem;

}

/*
 *  eth_enable
 *
 *  Enable the Ethernet block
 */
void eth_enable(void)
{
    U32 tmp;


    /*
     *  Enable MAC
     */


    tmp = BF_ETHER_READ(CONTROL);
    tmp |= ( BF_VMAC_CONTROL_ENABLE | BF_VMAC_CONTROL_TXRUN | BF_VMAC_CONTROL_RXRUN );
    BF_ETHER_WRITE(CONTROL, tmp);


    udelay(1*CFG_HZ);
}

/*
 *  solosw_eth_rx
 *
 *  Poll for a received Ethernet packet.
 *  Returns pointer to buffer, and sets len if there has been a
 *  successfully received packet.
 *  Another packet cannot be received until this buffer has been
 *  freed with eth_free
 */
BYTE *solosw_eth_rx(int *len)
{
    U32 tmp;

    /* Poll for an Rx interrupt */
    tmp = BF_ETHER_READ(STAT);
    if (tmp & BF_VMAC_STAT_RXINT)
    {
        /* Rx int fired */
        DBG("Rx int\n");

        /* Clear the rx interrupt */
       	BF_ETHER_WRITE(STAT, BF_VMAC_STAT_RXINT);


        /* Check that we own the buffer, and that if is valid */
        tmp = rx_desc->info;

        if ((tmp & BF_VMAC_RXINFO_OWN) || (rx_desc->ptr == NULL) )
        {
            /* Wait a us, and try again */
            udelay(1);
            tmp = rx_desc->info;
            if ((tmp & BF_VMAC_RXINFO_OWN) || (rx_desc->ptr == NULL) )
            {
                /* Still a problem */
                printf("Rx interrupt, but we don't own buffer, or not valid (info=0x%x, ptr=0x%08x)\n", tmp, rx_desc->ptr);

                return 0;
            }
        }

        /* Check the status */
        if (tmp & BF_VMAC_RXINFO_BUFF)
        {
            /* Error */
            printf("Rx error\n");
            eth_free(rx_buf);
            return 0;
        }

        /* Get the length */
        tmp = tmp & BF_VMAC_RXINFO_RX_LEN;
        /* Remove 4 byte FCS */
        if (tmp > 4)
        {
            tmp -= 4;
        }
        *len = tmp;  /* Pass the length back */
	
	
        /* Return the buffer */
        return rx_buf;
            
    }


    /* No recevied packet - return null */
    return NULL;
}


/*
 *  eth_free
 *
 *  Free a receive buffer, ready for it to be used to receive another packet
 */
void  eth_free(BYTE *buffer)
{    
    DBG_ARGS("buf=0x%x\n", buffer);

    /* We only have one buffer, so this really should be the same as the receive buffer */
    rx_buf = buffer;

    /* Set the descriptor up to receive into this buffer */
    rx_desc->ptr = (U8*)((U32)rx_buf & ADDR_MASK);
    rx_desc->info = BUFF_SIZE | BF_VMAC_RXINFO_OWN;
}

/*
 *  solosw_eth_tx
 *
 *  Transmit the buffer passed in.  Return when it has successully been transmitted
 */
int solosw_eth_tx(BYTE *buffer, int len)
{
    U32 tmp;


    /* Is the descriptor owned by us? */
    if (tx_desc->info & BF_VMAC_TXINFO_CPU_OWN)
    {
        /* No, so we can't transmit. */
        DBG("Tx desc not owned by us!\n");
        return 1;
    }

    /* Is buffer 0 */
    if (tx_desc->ptr != NULL)
    {
        /* No, so we can't transmit */
        DBG_ARGS("Tx buf not 0 (0x%x\n", tx_desc->ptr);
        return 1;
    }

    /* Put buffer into descriptor */


    tx_desc->ptr = (U8*) ((U32)buffer & ADDR_MASK);

    /* Length needs to be rounded upto 60 */
    if (len < 60)
    {
        len = 60;
    }

    /* Put the length into the descriptor */
    tx_desc->info = BF_VMAC_TXINFO_CPU_FIRST
                                      | BF_VMAC_TXINFO_LAST
                                      | BF_VMAC_TXINFO_CPU_OWN
                                      | len;


    /* Kick the MAC */
    BF_ETHER_WRITE( STAT, BF_VMAC_STAT_TXPOLL);

    DBG("txd packet\n");

    /* Wait for interrupt to say it's complete */
    do
    {
        tmp = BF_ETHER_READ(STAT);
        DBG_ARGS("stat = 0x%x\n", tmp);
    } while (! (tmp &  BF_VMAC_STAT_TXINT));
    
    /* Interrupt fired - clear it */
    BF_ETHER_WRITE(STAT, BF_VMAC_STAT_TXINT);

    /* Check that we own the buffer now */
    tmp = tx_desc->info;

    if (! (tmp & BF_VMAC_TXINFO_OWN))
    {
        /* Check status */
        DBG_ARGS("info = 0x%x\n", tmp);
        /* TBD - we should check that it was actually successful! */

        /* Clear descriptor */
        tx_desc->ptr = NULL;
    }
    else
    {
        /* Interrupt fired, but we don't seem to own the descriptor */
        DBG_ARGS("Int fired, but we don't own descriptor 0x%x\n", tmp);
        return 1;
    }
   

    DBG("tx complete\n");

    return 0;
}


/*
 *  solosw_eth_term
 *
 *  Finished with the Ethernet - disable the Ethernet block
 */
void  solosw_eth_term(void)
{
    U32 tmp;

    //phy_term();

    tmp = BF_ETHER_READ(CONTROL);

    /* Clear enable bits */
    tmp &= ~(BF_VMAC_CONTROL_ENABLE | BF_VMAC_CONTROL_TXRUN | BF_VMAC_CONTROL_RXRUN);

    BF_ETHER_WRITE(CONTROL, tmp);

    udelay(1*CFG_HZ);
}


/*
 *  phy_init
 *
 *  Set up a phy, if anything is required
 */
static void phy_init(void)
{

#ifdef BF_NETBOOT_PHY_GPIO_RESET

 
   /* Assume that the GPIO is already correctly configured (OUTPUT or INVOUTPUT) */
    *(volatile U32 *)((&REGIO(GPIO, CONTROL1))) = 0xc12ccccc;
    *(volatile U32 *)((&REGIO(GPIO, CONTROL2))) = 0x11111113;
    *(volatile U32 *)((&REGIO(GPIO, INPUT1))) = 0x0003ec20;


    /* Write a 1 to the appropriate sett register */
    *(volatile U32 *)((&REGIO(GPIO, SET1)) + (BF_NETBOOT_PHY_GPIO_RESET >> 5)) |= 1 << (BF_NETBOOT_PHY_GPIO_RESET & 31);
	
    udelay(1*CFG_HZ);

    /* Need to drive a GPIO to take an external phy out of reset */
    printf("Phy reset line on GPIO %d\n", BF_NETBOOT_PHY_GPIO_RESET);

#endif



}

/*
 *  phy_term
 *
 *  Shut down a phy, if anything is required
 */
static void phy_term(void)
{
#ifdef BF_NETBOOT_PHY_GPIO_RESET
    /* Take the GPIO used to reset low again */
    /* Write a 1 to the appropriate clear register */
    *(volatile U32 *)((&REGIO(GPIO, CLEAR1)) + (BF_NETBOOT_PHY_GPIO_RESET >> 5)) |= 1 << (BF_NETBOOT_PHY_GPIO_RESET & 31);

    udelay(1*CFG_HZ);
#endif
}



/**************************************************************

	Adapter driver for u-boot

***************************************************************/

int solosw_initialize(bd_t *bis)
{
	struct eth_device *dev;

	debug ("Register solos_w device.\n");

        dev = (struct eth_device *)malloc(sizeof(struct eth_device));

	dev->init = solosw_probe;
	dev->halt = solosw_halt;
	dev->send = solosw_send;
	dev->recv = solosw_recv;


	eth_register(dev);
	
	/* RTL8305 need to be rest twice */
	phy_init();  
        phy_term();
	phy_init();

	return 0;
}



static int solosw_probe(struct eth_device *dev, bd_t *bis)
{

	BYTE *buf;
	
	buf = (BYTE*)malloc(2*sizeof(tBF_EtherDesc) + BUFF_SIZE);

	buf += 2*sizeof(tBF_EtherDesc) + BUFF_SIZE;
	
	if(buf == solosw_eth_init(buf,bis->bi_enetaddr))
	{
		free(buf - 2*sizeof(tBF_EtherDesc) - BUFF_SIZE);

		printf("probe solos_w network device fail.\n");
		return 0;
	}

	eth_enable();

	printf("probe solos_w network device successfully.\n");

	return 1;
}



static int solosw_recv(struct eth_device *dev)
{
	int length;
	BYTE *buf;
//	int i;

	buf = solosw_eth_rx(&length);

	if(buf != 0 || buf != NULL)
	{
		
#if 0
		for(i=buf; i < buf+ length; i++)
		{
			printf("%2x ",*(unsigned char*)i);
			if((i%16) == 0)
				printf("\n");
		}
#endif
		NetReceive(buf, length);
		eth_free(buf);
		return length;
	}
	

	return 0;
}

static int solosw_send(struct eth_device *dev, volatile void *packet, int length)
{

	if(solosw_eth_tx((BYTE*)packet,length))
	{
		return 0;    /* fail to send */
	}

	return length;       /* send successfully */

}


static void solosw_halt(struct eth_device *dev)
{
	solosw_eth_term();
}
