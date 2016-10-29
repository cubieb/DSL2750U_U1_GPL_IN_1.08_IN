#ifndef __HIF_PCI_H
#define __HIF_PCI_H

#include  "hif_pci_zdma.h"

#define HIF_PCI_MAX_DEVS        4
#define MAX_TXDESC_SHIFT        7
#define MAX_RXDESC_SHIFT        8
#define HIF_PCI_MAX_TX_DESC     (1 << MAX_TXDESC_SHIFT)
#define HIF_PCI_MAX_RX_DESC     (1 << MAX_RXDESC_SHIFT)
#define MAX_NBUF_SIZE           1664

#endif
