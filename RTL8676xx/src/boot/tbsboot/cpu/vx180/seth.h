/* ===========================================================================
 *
 * Copyright (C) 2004 Analog Devices Inc. All rights reserved
 *
 * The information and source code contained herein is the exclusive property
 * of Analog Devices and may not be disclosed, examined or reproduced in whole
 * or in part without the explicit written authorization from Analog Devices.
 *
 * ===========================================================================
 */

/* ===========================================================================
 *
 * File name    : $RCSfile: seth.h,v $
 * Author       : $Author: sedara $
 * Version      : $Revision: 1.1 $
 * Description  : Simple polled-mode ethernet driver for stand alone use.
 *
 *                Only the existance of simple 'C' library functions is
 *                assumed.
 * ===========================================================================
 */

#ifndef _SETH_H_
#define _SETH_H_

/* ===========================================================================
 * Function name:       sethInit()
 * Input Parameters:    none
 * Output Parameters:   none
 * Result:              nothing
 * Description:         Initialize the ethernet port, AP, and driver.
 * ===========================================================================
 */
void sethInit(void);

/* ===========================================================================
 * Function name:       sethSetAddr()
 * Input Parameters:    enetAddr        6 byte ethernet addr for this port
 * Output Parameters:   none
 * Result:              nothing
 * Description:         Initialize the unicast address of this port
 * ===========================================================================
 */
void sethSetAddr(unsigned char addr[6]);

/* ===========================================================================
 * Function name:       sethPoll()
 * Input Parameters:    none
 * Output Parameters:   none
 * Result:              0 = no packets waiting, other = packet(s) waiting
 * Description:         Check to see if ethernet packets have been received
 * ===========================================================================
 */
int sethPoll(void);

/* ===========================================================================
 * Function name:       sethReceive()
 * Input Parameters:    pBuf    Pointer to buffer to hold received packet
 * Output Parameters:   none
 * Result:              Packet length, or zero if no packet waiting
 * Description:         Collect a packet that has been received by the AP
 * ===========================================================================
 */
unsigned int sethReceive(unsigned char *pBuf);

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
unsigned int sethTransmit(unsigned int length, unsigned char *pBuf);

#endif /* _SETH_H_ */
