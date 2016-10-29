/* 
 * SAR: Simple Address Resolution Protocol Implementation
 * Written by Geoffrey Cooper, September 27, 1983
 * 
 * This package implements a very simple version of the Plummer Address
 * Resolution Protocol (RFC 826).  It allows clients to resolve Internet
 * addresses into Ethernet addresses, and knows how to respond to an
 * address resolution request (when the transmit buffer is free).
 * 
 * Routines:
 * 
 *  sar_CheckPacket( pb ) => 1, if ARP packet and processed, 0 otherwise
 *  sar_MapIn2Eth( ina, ethap ) => 1 if did it, 0 if couldn't.
 *
 * Copyright (C) 1983, 1986 IMAGEN Corporation
 *  "This code may be duplicated in whole or in part provided that [1] there
 *   is no commercial gain involved in the duplication, and [2] that this
 *   copyright notice is preserved on all copies.  Any other duplication
 *   requires written notice of the author."
 * 
 */


 #include <types.h>
//#include <psbl/stddef.h>
#include "tinytcp.h"
//#include <psbl/debug.h>
//#include <psbl/string.h>

extern int t_get(void);

int sar_CheckPacket(arp_Header *ap)
  {
  register arp_Header *op;

  if ( ap->hwType != wfix(arp_TypeEther) || /* have ethernet hardware, */
       ap->protType != wfix(0x800) ||       /* and internet software, */
       ap->opcode != wfix(ARP_REQUEST) ||   /* and be a resolution req. */
       ap->dstIPAddr != lfix(sin_lclINAddr) /* for my addr. */
     )
    return ( 0 );                  /* .... or we ignore it. */
#if CPMAC_DEBUG
  sys_printf("ARP REQUEST IS VALID\n");	
#endif
  /* format response. */
  op = (arp_Header *)sed_FormatPacket(ap->srcEthAddr, 0x806);
  op->hwType = wfix(arp_TypeEther);
  op->protType = wfix(0x800);
  op->hwProtAddrLen = wfix((sizeof(eth_HwAddress) << 8) + sizeof(in_HwAddress));
  op->opcode = wfix(ARP_REPLY);
  op->srcIPAddr = lfix(sin_lclINAddr);
	memcpy(op->srcEthAddr, sed_lclEthAddr, sizeof(eth_HwAddress));
  ap->dstIPAddr = op->srcIPAddr;
	memcpy(op->dstEthAddr, ap->srcEthAddr, sizeof(eth_HwAddress));

  sed_Send(sizeof(arp_Header));
#if CPMAC_DEBUG 
  sys_printf("ARP RESPONSE SENT\n");	
#endif
  return ( 1 );
  }

/* 
 * Do an address resolution bit.
 */

int sar_MapIn2Eth(longword ina, eth_HwAddress *ethap)
  {
  register arp_Header *op;
  extern in_HwAddress sin_lclINAddr;
  longword endTime;
  longword rxMitTime;

  endTime = clock_ValueRough() + 2000;
  while ( endTime > clock_ValueRough() )
    {
    op = (arp_Header *)sed_FormatPacket(&sed_ethBcastAddr[0], 0x806);
    op->hwType = wfix(arp_TypeEther);
    op->protType = wfix(0x800);
    op->hwProtAddrLen = wfix((sizeof(eth_HwAddress) << 8) + sizeof(in_HwAddress));
    op->opcode = wfix(ARP_REQUEST);
    op->srcIPAddr = lfix(sin_lclINAddr);
	memcpy(op->srcEthAddr, sed_lclEthAddr, sizeof(eth_HwAddress));
    if ((ina&net_IpMask)!=(sin_lclINAddr&net_IpMask))
      ina=net_Gateway;

    op->dstIPAddr = lfix(ina);

    /* ...and send the packet */
    sed_Send( sizeof(arp_Header) );

    rxMitTime = clock_ValueRough() + 500;
    while ( rxMitTime > clock_ValueRough() )
      {
      op = (arp_Header *)sed_IsPacket();
      if ( op )
        {
        if ((sed_CheckPacket(op, 0x806) == 1) &&
            (op->protType == wfix(0x800)) &&
            (op->srcIPAddr == lfix(ina)) &&
            (op->opcode == wfix(ARP_REPLY)) )
          {
          memcpy(ethap, op->srcEthAddr, sizeof(eth_HwAddress));
          return ( 1 );
          }
        }
      }
    }
  return ( 0 );
  }
