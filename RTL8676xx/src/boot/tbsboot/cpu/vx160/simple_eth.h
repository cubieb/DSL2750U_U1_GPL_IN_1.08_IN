/* =============================================================================
 *
 * Copyright (C) 2004 Analog Devices Inc. All rights reserved
 *
 * The information and source code contained herein is the exclusive property
 * of Analog Devices and may not be disclosed, examined or reproduced in whole
 * or in part without the explicit written authorization from Analog Devices.
 *
 * ===========================================================================*/

//======================================================================
// File Name	:	$RCSfile: simple_eth.h,v $
// Description	:	Include file for simple ethernet AP code
// CVS ID	:	$Id: simple_eth.h,v 1.1 2006/09/26 08:06:40 dballa Exp $
//====================================================================== 

#ifndef _SAETH_H_
#define _SAETH_H_

/* Define the number of received buffers */
#define SA_RX_BUFS      12

/* Define the structure of the TX and RX buffers. The RX buffer area is a
 * set of SA_RX_BUFS contiguous versions of this structure
 */
struct simpleEthBuffer_s
{
    unsigned short      length;
    unsigned char       pkt[2048 - 2];
};

/* Bits defined in the EVENT_IN register. Other bits should not be used */
#define SA_DO_TX_BIT    0       // Set to send buffer in AP_GP_REGISTER1/2

/* Bits defined in the INTERRUPT_STATUs register */
#define SA_TX_ERR_BIT           15      // Error during TX
#define SA_RX_INT_ERR_BIT       14      // Internal error during Rx
#define SA_RX_EXT_ERR_BIT       13      // Error in received data
#define SA_RX_BUF_0_BIT         0       // 

#endif /* _SAETH_H_ */

