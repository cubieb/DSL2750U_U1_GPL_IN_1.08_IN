//==========================================================================
//
//      dev/ae531xecos.c
//
//      Ethernet driver for Atheros AR531X
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2003 Atheros Communications, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting the copyright
// holders.
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    Atheros Communications, Inc.
// Contributors: Atheros Engineering
// Date:         2003-10-22
// Purpose:      
// Description:  AR531X ethernet hardware driver
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

// Ethernet device driver for Atheros AR531X

#include <common.h>
#include <net.h>
#include "ae531xreg.h"
#include "ae531xecos.h"
#include "ae531xmac.h"
#include "ae531xPhy.h"
#include "hal_cache.h"

#define ETHER_ADDR_LEN 6

typedef struct ae531x_priv_data_s {
    int                     enetUnit;
    unsigned char           enet_addr[ETHER_ADDR_LEN];
    ae531x_MAC_t            MACInfo;
} ae531x_priv_data_t;

#define ETH_MAX_MTU 1536
#define CYGNUM_PHY_TRAILER_SIZE 4
#define ETHER_SMALL 64

#define AE531X_RX_BUF_SIZE \
    (ETH_MAX_MTU + CYGNUM_PHY_TRAILER_SIZE)

#define AE531X_TX_BUF_SIZE \
    (ETH_MAX_MTU)

typedef struct ae531xRxBuf_s {
    struct ae531xRxBuf_s *next;
    char                 data[AE531X_RX_BUF_SIZE];
} ae531xRxBuf_t;
#define AE531X_RX_DATA_BUF_COUNT 2*AE531X_RX_DESC_COUNT_DEFAULT
ae531xRxBuf_t ae531xRxDataBuf[AE531X_RX_DATA_BUF_COUNT];
ae531xRxBuf_t *ae531xRxFreeList = NULL;
ae531xRxBuf_t *ae531xRxRecvdListHead = NULL;
ae531xRxBuf_t *ae531xRxRecvdListTail = NULL;

typedef struct ae531xTxBuf_s {
    struct ae531xTxBuf_s *next;
    unsigned long        key;
    char                 data[AE531X_TX_BUF_SIZE];
} ae531xTxBuf_t;
ae531xTxBuf_t ae531xTxDataBuf[AE531X_TX_DESC_COUNT_DEFAULT];
ae531xTxBuf_t *ae531xTxFreeList = NULL;
unsigned int ae531xTxAvail;

//static void ae531x_TxReap(ae531x_MAC_t *MACInfo, struct eth_drv_sc *sc);
static void ae531x_txbuf_alloc(ae531x_MAC_t *MACInfo, ae531xTxBuf_t **txBptr);
static void ae531x_rxbuf_free(ae531xRxBuf_t *rxDesc);
static void ae531x_TxReap(ae531x_MAC_t *MACInfo);//, struct eth_drv_sc *sc);


char DEFAULT_MAC_ADDRESS[] = { 0x00, 0x03, 0x7f, 0xe0, 0x02, 0xbf };

/*
 * Standard eCos initialization routine.  Call the upper-level ethernet
 * driver and let it know our MAC address.
 */
static int
ae531x_init(struct eth_device *dev, bd_t *bis)
{
    unsigned char enaddr[ETHER_ADDR_LEN];
    char *mac_addr;
    ae531x_priv_data_t *ae531x_priv;
    ae531x_MAC_t *MACInfo;
    int unit;
    
    ae531x_priv = (ae531x_priv_data_t *)dev->priv;
    unit = ae531x_priv->enetUnit;

    /* Get ethernet's MAC address from board configuration data */
 
    memcpy(enaddr, DEFAULT_MAC_ADDRESS, ETHER_ADDR_LEN);
 

    MACInfo = &ae531x_priv->MACInfo;

    /* Initialize MACInfo */
    MACInfo->OSinfo = (void *)ae531x_priv;
    MACInfo->unit = unit;

    if (unit == 0) {
        MACInfo->macBase = (UINT32)(PHYS_TO_K1(AR531X_ENET0)+AE531X_MAC_OFFSET);
        MACInfo->dmaBase = (UINT32)(PHYS_TO_K1(AR531X_ENET0)+AE531X_DMA_OFFSET);
        MACInfo->phyBase = (UINT32)(PHYS_TO_K1(AR531X_ENET0)+AE531X_PHY_OFFSET);
    }

        /* Bring MAC and PHY out of reset */

   ae531x_reset(MACInfo);

    /* Initialize PHY */
    phySetup(MACInfo->unit, MACInfo->phyBase);

    return 1;
}

/* Space for Transmit and Receive descriptors */
char ae531xTxDescs[AE531X_QUEUE_ELE_SIZE * AE531X_TX_DESC_COUNT_DEFAULT];
char ae531xRxDescs[AE531X_QUEUE_ELE_SIZE * AE531X_RX_DESC_COUNT_DEFAULT];

static int ae531x_start(struct eth_device *dev, bd_t *bis)
{
    ae531x_priv_data_t *ae531x_priv;
    ae531x_MAC_t *MACInfo;
    unsigned char enet_addr[ETHER_ADDR_LEN];
    int i;

    ae531x_priv = (ae531x_priv_data_t *)dev->priv;
    MACInfo = (ae531x_MAC_t *)&ae531x_priv->MACInfo;

    memcpy(ae531x_priv->enet_addr, bis->bi_enetaddr, ETHER_ADDR_LEN);

    /* Attach interrupt handler */
    /* TBDXXX */

    /* Bring MAC and PHY out of reset */

  // ae531x_reset(MACInfo);

    /* Initialize PHY */
   // phySetup(MACInfo->unit, MACInfo->phyBase);

    /* Start thread to poll for phy link status changes */
    /* TBDXXX -- do in ae531x_poll? */

    /* Setup Transmit Descriptors */
    MACInfo->txDescCount = AE531X_TX_DESC_COUNT_DEFAULT;
    MACInfo->pTxDescs = ae531xTxDescs;

    /* Setup Transmit buffer free list */
    for (i=0; i<AE531X_TX_DESC_COUNT_DEFAULT; i++) {
        ae531xTxDataBuf[i].next = ae531xTxFreeList;
        ae531xTxFreeList = &ae531xTxDataBuf[i];
    }
    ae531xTxAvail = AE531X_TX_DESC_COUNT_DEFAULT;

    /* Setup Receive Descriptors */
    MACInfo->rxDescCount = AE531X_RX_DESC_COUNT_DEFAULT;
    MACInfo->pRxDescs = ae531xRxDescs;

    /* Setup Receive buffer free list */
    for (i=0; i<AE531X_RX_DATA_BUF_COUNT; i++) {
        ae531xRxDataBuf[i].next = ae531xRxFreeList;
        ae531xRxFreeList = &ae531xRxDataBuf[i];
    }

    /* Allocate RX/TX Queues */
    if (ae531x_AllocateQueues(MACInfo) < 0) {
        AE531X_PRINT(AE531X_DEBUG_RESET, ("Queue allocation failed"));
        goto skip_start;
    }

    /* Initialize MAC */
    ae531x_MACReset(MACInfo);

    /* Initialize DMA and descriptors */
    ae531x_DmaReset(MACInfo);

    /* Enable Receive/Transmit */
    ae531x_EnableComm(MACInfo);

    MACInfo->port_is_up = TRUE;

skip_start:
    
    return 0;
}

static void
ae531x_stop(struct eth_device *dev)
{
#if 0
    ae531x_priv_data_t *ae531x_priv;
    ae531x_MAC_t *MACInfo;
    ae531x_priv = (ae531x_priv_data_t *)dev->priv;
    MACInfo = (ae531x_MAC_t *)&ae531x_priv->MACInfo;
   /* Disable Receive/Transmit */
    ae531x_DisableComm(MACInfo);
 #endif
}


static int ae531x_send(struct eth_device *dev, volatile void *packet, int length)
{
    ae531x_priv_data_t *ae531x_priv;
    ae531x_MAC_t *MACInfo;
    UINT32 ctrlen;
    VIRT_ADDR txDesc;
    ae531xTxBuf_t *TxBuf;
    unsigned char *TxBufData, *TxBufDataStart;
    int i;
    volatile UINT32 regval;

    ae531x_priv = (ae531x_priv_data_t *)dev->priv;
    MACInfo = (ae531x_MAC_t *)&ae531x_priv->MACInfo;

    /* Check if this port is up, else toss packet */
    if (!MACInfo->port_is_up) 
    {
        goto dropFrame;
    }

    if (ae531x_IsInResetMode(MACInfo)) 
    {
        goto dropFrame;
    }

     ae531x_TxReap(MACInfo);

    txDesc = MACInfo->txQueue.curDescAddr;
    if (txDesc == MACInfo->txQueue.reapDescAddr) 
    {
        goto dropFrame;
    }


    AE531X_CONSUME_DESC((&MACInfo->txQueue));
    ae531xTxAvail--;

    /* Allocate a transmit data buffer */
    ae531x_txbuf_alloc(MACInfo, &TxBuf);
    AE531X_DESC_SWPTR_SET(txDesc, TxBuf);

    TxBufDataStart = TxBufData = TxBuf->data;

    if (length< 60)
    {
        length= 60;
    }
   memcpy(TxBufData, packet, length);

    TxBufData += length;

    /* Update the descriptor */
    AE531X_DESC_BUFPTR_SET(txDesc, virt_to_bus(TxBufDataStart));
    ctrlen = AE531X_DESC_CTRLEN_GET(txDesc);
    ctrlen = (ctrlen & (DescEndOfRing)) |
                            DescTxFirst |
                             DescTxLast |
                        DescTxIntEnable;

    ctrlen |= ((length << DescSize1Shift) & DescSize1Mask);

    AE531X_DESC_CTRLEN_SET(txDesc, ctrlen);

    AE531X_DESC_STATUS_SET(txDesc, DescOwnByDma);

    /* Alert DMA engine to resume Tx */
    ae531x_WriteDmaReg(MACInfo, DmaTxPollDemand, 0);
    sysWbFlush();


dropFrame:
    return length;
}

static int ae531x_poll(struct eth_device *dev)
{
    ae531xRxBuf_t *rxBuf, *newRxBuf;
    char *rxBufData;
    int unused_length;
    VIRT_ADDR   rxDesc;
    int length;
    ae531x_MAC_t *MACInfo;
    unsigned int cmdsts;
    ae531x_priv_data_t *ae531x_priv;

    ae531x_priv = (ae531x_priv_data_t *)dev->priv;
    MACInfo = (ae531x_MAC_t *)&ae531x_priv->MACInfo;

    do {
        for(;;) {
            rxDesc = MACInfo->rxQueue.curDescAddr;
            cmdsts = AE531X_DESC_STATUS_GET(KSEG1ADDR(rxDesc));

            if (cmdsts & DescOwnByDma) {
                /* There's nothing left to process in the RX ring */
                goto rx_all_done;
            }

            AE531X_PRINT(AE531X_DEBUG_RX,
                  ("consume rxDesc %p with cmdsts=0x%x\n",
                   (void *)rxDesc, cmdsts));
   
            AE531X_CONSUME_DESC((&MACInfo->rxQueue));
    
            A_DATA_CACHE_INVAL(rxDesc, AE531X_DESC_SIZE);

            /*  Process a packet */
            length = AE531X_DESC_STATUS_RX_SIZE(cmdsts) - ETH_CRC_LEN;
            if ( (cmdsts & (DescRxFirst |DescRxLast | DescRxErrors)) ==
                           (DescRxFirst | DescRxLast) ) {
                /* Descriptor status indicates "NO errors" */

                rxBuf = AE531X_DESC_SWPTR_GET(rxDesc);
                /*
                 * Allocate a replacement buffer
                 * We want to get another buffer ready for Rx ASAP.
                 */
                newRxBuf = ae531x_rxbuf_alloc(MACInfo, &rxBufData, &unused_length);
                if(newRxBuf == NULL ) {
                    /*
                     * Give this descriptor back to the DMA engine,
                     * and drop the received packet.
                     */
                    AE531X_PRINT(AE531X_DEBUG_ERROR,
                              ("Can't allocate new rx buf\n"));
                } else {
		   //diag_printf("rxBufData=%p\n",rxBufData);
                    AE531X_DESC_BUFPTR_SET(rxDesc, virt_to_bus(rxBufData));
                    AE531X_DESC_SWPTR_SET(rxDesc, newRxBuf);
                }

                AE531X_DESC_STATUS_SET(rxDesc, DescOwnByDma);
                rxDesc = NULL; /* sanity -- cannot use rxDesc now */
                sysWbFlush();
    
                if (newRxBuf == NULL) {
                    goto no_rx_bufs;
                } else {
                    /* Sync data cache w.r.t. DMA */
                    A_DATA_CACHE_INVAL(rxBuf->data, length);
        
                    /* Send the data up the stack */
                    AE531X_PRINT(AE531X_DEBUG_RX,
                              ("Send data up stack: rxBuf=%p data=%p length=%d\n",
                               (void *)rxBuf, (void *)rxBuf->data, length));

                    {
                        unsigned long oldIntrState;

                        rxBuf->next = NULL;
                        intDisable(oldIntrState);
                        if (ae531xRxRecvdListHead) {
                            ae531xRxRecvdListTail->next = rxBuf;
                            ae531xRxRecvdListTail = rxBuf;
                        } else {
                            ae531xRxRecvdListHead = rxBuf;
                            ae531xRxRecvdListTail = rxBuf;
                        }
                        intEnable(oldIntrState);
                    }
                     NetReceive(rxBuf->data, length);
			    /* Free driver Rx buffer */
    			ae531x_rxbuf_free(rxBuf);
                }
            } else {
                AE531X_PRINT(AE531X_DEBUG_ERROR,
                          ("Bad receive.  rxDesc=%p  cmdsts=0x%8.8x\n",
                           (void *)rxDesc, cmdsts));
            }
        }
    } while (ae531x_ReadDmaReg(MACInfo, DmaStatus) & DmaIntRxCompleted);

rx_all_done:
    ae531x_SetDmaReg(MACInfo, DmaIntrEnb,
                     DmaIeRxCompleted | DmaIeRxNoBuffer);
    ae531x_WriteDmaReg(MACInfo, DmaRxPollDemand, 0);

no_rx_bufs:

    return;
}

UINT8 *
macAddrGet(ae531x_MAC_t *MACInfo)
{
    ae531x_priv_data_t *priv_data;

    priv_data = (ae531x_priv_data_t *)MACInfo->OSinfo;

    return priv_data->enet_addr;
}

void *
ae531x_rxbuf_alloc(ae531x_MAC_t *MACInfo, char **rxBptr, int *rxBSize)
{
    ae531xRxBuf_t *rxbuf;
    unsigned long oldIntrState;

    intDisable(oldIntrState);
    if (ae531xRxFreeList) {
        rxbuf = ae531xRxFreeList;
	//DEBUG_PRINTF("ae531xRxFreeList=%p\n",rxbuf);
        ae531xRxFreeList = ae531xRxFreeList->next;
        intEnable(oldIntrState);

        *rxBptr = (char *)rxbuf->data;
        *rxBSize = sizeof(rxbuf->data);
        return (void *)rxbuf;
    }
    intEnable(oldIntrState);

    DEBUG_PRINTF("ae531x_rxbuf_alloc failed!\n");
    *rxBptr = NULL;

    return (void *)NULL;
}

void
ae531x_rxbuf_free(ae531xRxBuf_t *rxBuf)
{
    unsigned long oldIntrState;

    intDisable(oldIntrState);
    if (rxBuf) {
        rxBuf->next = ae531xRxFreeList;
        ae531xRxFreeList = rxBuf;
    }
    intEnable(oldIntrState);
}


static void
ae531x_txbuf_alloc(ae531x_MAC_t *MACInfo, ae531xTxBuf_t **txBptr)
{
    ae531xTxBuf_t *txbuf;
    unsigned long oldIntrState;

    intDisable(oldIntrState);
    if (ae531xTxFreeList) {
        txbuf = ae531xTxFreeList;
        ae531xTxFreeList = ae531xTxFreeList->next;
        intEnable(oldIntrState);

        *txBptr = txbuf;
        return;
    }
    intEnable(oldIntrState);

    DEBUG_PRINTF("ae531x_txbuf_alloc failed!\n");
    *txBptr = NULL;

    return;
}

void
ae531x_txbuf_free(ae531xTxBuf_t *txBuf)
{
    unsigned long oldIntrState;

    intDisable(oldIntrState);
    if (txBuf) {
        txBuf->next = ae531xTxFreeList;
        ae531xTxFreeList = txBuf;
    }
    intEnable(oldIntrState);
}

ae531x_priv_data_t ae531x_priv_data0 = {enetUnit: 0};

int ar2317_initialize(bd_t *bis)
{
	struct eth_device *dev;

	printf ("Register ar2317 driver.\n");
	
	dev = (struct eth_device *)malloc(sizeof(struct eth_device));

	if(dev == NULL)
	{
		printf("Allocate memory for eth device fail.\n");
		return NULL;
	}
	dev->init = ae531x_start;
	dev->halt = ae531x_stop;
	dev->send = ae531x_send;
	dev->recv = ae531x_poll;
	dev->priv = &ae531x_priv_data0;

	ae531x_init(dev, bis);
	
	eth_register(dev);

       return 0;
}

int aeUselessReap = 0;
static void
ae531x_TxReap(ae531x_MAC_t *MACInfo)
{
    AE531X_QUEUE      *txq;
    VIRT_ADDR         txDesc;
    UINT32            cmdsts;
    int               reaped;
    ae531xTxBuf_t     *TxBuf;
    unsigned long     key;

    txq = &MACInfo->txQueue;
    reaped = 0;

    // XXXTODO: bug - won't be able to reap once run out of descriptors
    while (1) {
        txDesc = AE531X_QUEUE_ELE_NEXT_GET(txq, txq->reapDescAddr);
        if (txDesc == txq->curDescAddr) {
            break;
        }

        cmdsts = AE531X_DESC_STATUS_GET(KSEG1ADDR(txDesc));
        if (cmdsts & DescOwnByDma) {
            break;
        }


        TxBuf = (ae531xTxBuf_t *)AE531X_DESC_SWPTR_GET(txDesc);
        AE531X_PRINT(AE531X_DEBUG_TX_REAP, ("TXREAP: cur=%p  reap=%p  txDesc=%p TxBuf=%p\n",
                     txq->curDescAddr, txq->reapDescAddr, txDesc, TxBuf));
        key = TxBuf->key;

        /* Release transmit buffer associated with completed transmit */
        ae531x_txbuf_free(TxBuf);
        AE531X_DESC_SWPTR_SET(txDesc, (void *)0x1bad0dad);
        ae531xTxAvail++;

        txq->reapDescAddr = txDesc;
        reaped++;
    }

    if (reaped > 0) {
        AE531X_PRINT(AE531X_DEBUG_TX_REAP,
                 ("reaped=%d    TxAvail=%d\n", reaped, ae531xTxAvail));
    } else {
        aeUselessReap++;
    }
}


