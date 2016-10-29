/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************
 
    Module Name:
    vr_ikans.h
 
    Abstract:
    Only for IKANOS Vx160 or Vx180 platform.
 
    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Sample Lin	01-28-2008    Created

 */
 #include <linux/netdevice.h>
#include <netpro/apprehdr.h>

#ifndef MODULE_IKANOS
#define IKANOS_EXTERN	extern
#else
#define IKANOS_EXTERN
#endif // MODULE_IKANOS //


IKANOS_EXTERN void VR_IKANOS_FP_Init(UINT8 BssNum, UINT8 *pApMac);

IKANOS_EXTERN INT32 IKANOS_DataFramesTx(struct sk_buff *pSkb,
										struct net_device *pNetDev);

IKANOS_EXTERN void IKANOS_DataFrameRx(PRTMP_ADAPTER pAd,
										void *pRxParam,
										struct sk_buff *pSkb,
										UINT32 Length);

IKANOS_EXTERN INT32 IKANOS_Reg_Fastpath(PRTMP_ADAPTER pAd,struct sk_buff *pSkb,
										struct net_device *pNetDev,int index);

static  INT32 IKANOS_WlanDataFramesTx(IN void *_pAdBuf,IN struct net_device	*pNetDev);

#define rt_carr_net80211_init() \
	do {	 \
                wlan_ralink_fastpath_tx_ptr = &IKANOS_WlanDataFramesTx; \
	     } while (0)

extern void (*wlan_ralink_fastpath_tx_ptr) (apPreHeader_t *, struct net_device *);

#define FAST_PATH_PACKET 1
#define WDS_PACKET 2
#ifndef MIN_NET_DEVICE_FOR_WDS
#define MIN_NET_DEVICE_FOR_WDS			0x10
#endif

/* End of vr_ikans.h */
