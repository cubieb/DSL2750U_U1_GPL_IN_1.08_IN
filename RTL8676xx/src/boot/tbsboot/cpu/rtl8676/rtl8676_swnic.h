

#ifndef _SWNIC_H
#define _SWNIC_H

#include <types.h>

#if defined(CONFIG_RTL865XC) || defined(CONFIG_RTL8676)
/* --------------------------------------------------------------------
 * ROUTINE NAME - swNic_init
 * --------------------------------------------------------------------
 * FUNCTION: This service initializes the switch NIC.
 * INPUT   : 
        userNeedRxPkthdrRingCnt[RTL865X_SWNIC_RXRING_MAX_PKTDESC]: Number of Rx pkthdr descriptors. of each ring
        userNeedRxMbufRingCnt: Number of Rx mbuf descriptors.
        userNeedTxPkthdrRingCnt[RTL865X_SWNIC_TXRING_MAX_PKTDESC]: Number of Tx pkthdr descriptors. of each ring
        clusterSize: Size of a mbuf cluster.
 * OUTPUT  : None.
 * RETURN  : Upon successful completion, the function returns ENOERR. 
        Otherwise, 
		EINVAL: Invalid argument.
 * NOTE    : None.
 * -------------------------------------------------------------------*/
int swNic_init(u32 userNeedRxPkthdrRingCnt[],
                 u32 userNeedRxMbufRingCnt,
                 u32 userNeedTxPkthdrRingCnt[],
                 u32 clusterSize);
#endif /* CONFIG_RTL865XC */



/* --------------------------------------------------------------------
 * ROUTINE NAME - swNic_intHandler
 * --------------------------------------------------------------------
 * FUNCTION: This function is the NIC interrupt handler.
 * INPUT   :
		intPending: Pending interrupts.
 * OUTPUT  : None.
 * RETURN  : None.
 * NOTE    : None.
 * -------------------------------------------------------------------*/
void swNic_intHandler(u32 intPending);
void swInit(volatile unsigned char* mac);
int swNic_receive(void** input, u32* pLen);
int swNic_send(volatile void * output, int len);
void swNic_txDone(void);
int is_link_change(void);

#endif /* _SWNIC_H */
