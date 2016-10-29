/*****************************************************************************
//
//  Copyright (c) 2006  Broadcom Corporation
//  All Rights Reserved
//  No portions of this material may be reproduced in any form without the
//  written permission of:
//          Broadcom Corporation
//          16215 Alton Parkway
//          Irvine, California 92619
//  All information contained in this document is Broadcom Corporation
//  company private, proprietary, and trade secret.
//
******************************************************************************
//
//  Filename:       igd.h
//
******************************************************************************/

#ifndef _igd_h_
#define _igd_h_

typedef struct _if_stats {
    unsigned long rx_packets;	/* total packets received       */
    unsigned long tx_packets;	/* total packets transmitted    */
    unsigned long rx_bytes;	/* total bytes received         */
    unsigned long tx_bytes;	/* total bytes transmitted      */
    unsigned long rx_errors;	/* bad packets received         */
    unsigned long tx_errors;	/* packet transmit problems     */
    unsigned long rx_dropped;	/* no space in linux buffers    */
    unsigned long tx_dropped;	/* no space available in linux  */
    unsigned long rx_multicast;	/* multicast packets received   */
    unsigned long rx_compressed;
    unsigned long tx_compressed;
    unsigned long collisions;

    /* detailed rx_errors: */
    unsigned long rx_length_errors;
    unsigned long rx_over_errors;	/* receiver ring buff overflow  */
    unsigned long rx_crc_errors;	/* recved pkt with crc error    */
    unsigned long rx_frame_errors;	/* recv'd frame alignment error */
    unsigned long rx_fifo_errors;	/* recv'r fifo overrun          */
    unsigned long rx_missed_errors;	/* receiver missed packet     */
    /* detailed tx_errors */
    unsigned long tx_aborted_errors;
    unsigned long tx_carrier_errors;
    unsigned long tx_fifo_errors;
    unsigned long tx_heartbeat_errors;
    unsigned long tx_window_errors;

} if_stats_t;

typedef struct _WANDevicePrivateData {
    char ifname[20];
} WANDevicePrivateData, *PWANDevicePrivateData;

typedef struct _LANDevicePrivateData {
    char ifname[20];
} LANDevicePrivateData, *PLANDevicePrivateData;


struct Layer3Forwarding {
    struct Service *default_svc;
};

extern void osl_igd_disable(void);
extern void osl_igd_enable(void);
extern char *igd_pri_wan_var(char *prefix, int len, char *var);
extern int LANDevice_Init(PDevice pdev, device_state_t state, va_list ap);
extern int WANDevice_Init(PDevice pdev, device_state_t state, va_list ap);
struct Param *findActionParamByRelatedVar(PAction ac, int relatedVar);
int OutputCharValueToAC(PAction ac, int varIndex, char *value);


#if defined(linux)
#define sys_restart() kill_after(1, SIGHUP, 3)
#define sys_reboot() kill_after(1, SIGTERM, 3)

#endif /* linux */

#endif /* _igd_h_ */
