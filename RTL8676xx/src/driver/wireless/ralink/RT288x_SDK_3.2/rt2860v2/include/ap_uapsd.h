/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    ap_uapsd.h

    Abstract:
    WMM UAPSD Module

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Sample Lin  11-07-2006      created
*/

#ifndef MODULE_WMM_UAPSD

#define UAPSD_EXTERN    		extern

/* Public Marco list */
/* init some parameters in packet structure for QoS Null frame;
   purpose: is for management frame tx done use */
#define UAPSD_MR_QOS_NULL_HANDLE(pAd, pData, pPacket) 								\
    {   PHEADER_802_11 header_p = (PHEADER_802_11)pData; 							\
		MAC_TABLE_ENTRY *entry_p;													\
        if (header_p->FC.SubType == SUBTYPE_QOS_NULL) {								\
            RTMP_SET_PACKET_QOS_NULL(pPacket);										\
			entry_p = MacTableLookup(pAd, header_p->Addr1);							\
			if (entry_p != NULL) { RTMP_SET_PACKET_WCID(pPacket, entry_p->Aid); } }	\
		else { RTMP_SET_PACKET_NON_QOS_NULL(pPacket); } }

/* init MAC entry UAPSD parameters;
   purpose: initialize UAPSD PS queue and control parameters */
#define UAPSD_MR_ENTRY_INIT(pEntry) \
    {   USHORT ac_id; \
        for(ac_id=0; ac_id<WMM_NUM_OF_AC; ac_id++) \
            InitializeQueueHeader(&pEntry->UAPSDQueue[ac_id]); \
        pEntry->UAPSDTxNum = 0; \
        pEntry->pUAPSDEOSPFrame = NULL; \
        pEntry->bAPSDFlagSPStart = 0; \
        pEntry->bAPSDFlagEOSPOK = 0; \
        pEntry->MaxSPLength = 0; }

/* reset MAC entry UAPSD parameters;
   purpose: clean all UAPSD PS queue; release the EOSP frame if exists;
            reset control parameters */
#define UAPSD_MR_ENTRY_RESET(pEntry) \
    {   MAC_TABLE_ENTRY *sta_p; \
        ULONG ac_id; \
        sta_p = pEntry; \
        /* clear all U-APSD queues */ \
        for(ac_id=0; ac_id<WMM_NUM_OF_AC; ac_id++) \
            APCleanupPsQueue(pAd, &sta_p->UAPSDQueue[ac_id]); \
        /* clear EOSP frame */ \
        sta_p->UAPSDTxNum = 0; \
        if (sta_p->pUAPSDEOSPFrame != NULL) { \
            RELEASE_NDIS_PACKET(pAd, \
                                QUEUE_ENTRY_TO_PACKET(sta_p->pUAPSDEOSPFrame), \
                                NDIS_STATUS_FAILURE); \
            sta_p->pUAPSDEOSPFrame = NULL; } \
        sta_p->bAPSDFlagSPStart = 0; \
        sta_p->bAPSDFlagEOSPOK = 0; }

/* enable or disable UAPSD flag in WMM element in beacon frame;
   purpose: set UAPSD enable/disable bit */
#define UAPSD_MR_IE_FILL(qos_ctrl_field, pAd) \
        qos_ctrl_field |= (pAd->CommonCfg.bAPSDCapable) ? 0x80 : 0x00;

/* check if we do NOT need to control TIM bit for the station;
   note: we control TIM bit only when all AC are UAPSD AC */
#define UAPSD_MR_IS_NOT_TIM_BIT_NEEDED_HANDLED(pMacEntry, QueIdx) \
        (CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_APSD_CAPABLE) && \
         (!pMacEntry->bAPSDCapablePerAC[QID_AC_VO] || \
          !pMacEntry->bAPSDCapablePerAC[QID_AC_VI] || \
          !pMacEntry->bAPSDCapablePerAC[QID_AC_BE] || \
          !pMacEntry->bAPSDCapablePerAC[QID_AC_BK]) && \
         pMacEntry->bAPSDCapablePerAC[QueIdx])

/* check if the AC is UAPSD AC */
#define UAPSD_MR_IS_UAPSD_AC(pMacEntry, ac_id) \
        (CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_APSD_CAPABLE) && \
         ((0 <= ac_id) && (ac_id < WMM_NUM_OF_AC)) && /* 0 ~ 3 */ \
         pMacEntry->bAPSDCapablePerAC[ac_id])

/* check if all AC are UAPSD AC */
#define UAPSD_MR_IS_ALL_AC_UAPSD(isActive, pMacEntry) \
        ((isActive == FALSE) && (pMacEntry->bAPSDAllAC == 1))

/* suspend SP */
#define UAPSD_MR_SP_SUSPEND(pAd) \
		pAd->bAPSDFlagSPSuspend = 1;

/* resume SP */
#define UAPSD_MR_SP_RESUME(pAd) \
		pAd->bAPSDFlagSPSuspend = 0;

/* mark PS poll frame sent in mix mode */
/* Note:
	(1) When SP is not started, try to mark a flag to record if the legacy ps
		packet is handled in statistics handler;
	(2) When SP is started, increase the UAPSD count number for the legacy PS. */
#define UAPSD_MR_MIX_PS_POLL_RCV(pAd, pMacEntry)			\
		if (pMacEntry->bAPSDFlagSpRoughUse == 0) {			\
			if (pMacEntry->bAPSDFlagSPStart == 0) {			\
				if (pMacEntry->bAPSDFlagLegacySent == 1)	\
					NICUpdateFifoStaCounters(pAd);			\
				pMacEntry->bAPSDFlagLegacySent = 1;			\
			} else {										\
				pMacEntry->UAPSDTxNum ++; } }

#else

#define UAPSD_EXTERN
#define UAPSD_QOS_NULL_QUE_ID	0x7f

#ifdef RT2860
/* In RT2870, FIFO counter is for all stations, not for per-entry,
	so we can not use accurate method in RT2870 */

/* Note for SP ACCURATE Mechanism:
	1. When traffic is busy for the PS station
		Statistics FIFO counter maybe overflow before we read it, so UAPSD
		counting mechanism will not accurately.

		Solution:
		We need to avoid the worse case so we suggest a maximum interval for
		a SP that the interval between last frame from QAP and data frame from
		QSTA is larger than UAPSD_EPT_SP_INT.

	2. When traffic use CCK/1Mbps from QAP
		Statistics FIFO will not count the packet. There are 2 cases:
		(1) We force to downgrage ARP response & DHCP packet to 1Mbps;
		(2) After rate switch mechanism, tx rate is fixed to 1Mbps.

		Solution:
		Use old DMA UAPSD mechanism.

	3. When part of AC uses legacy PS mode
		Statistics count will inclue packet statistics for legacy PS packets
		so we can not know which one is UAPSD, which one is legacy.

		Solution:
		Cound the legacy PS packet.

	4. Check FIFO statistics count in Rx Done function
		We can not to check TX FIFO statistics count in Rx Done function or
		the real packet tx/rx sequence will be disarranged.

		Solution:
		Suspend SP handle before rx done and resume SP handle after rx done.
*/
#define UAPSD_SP_ACCURATE		/* use more accurate method to send EOSP */
#endif // RT2860 //

#define UAPSD_EPT_SP_INT		(100000/(1000000/OS_HZ)) /* 100ms */

#endif // MODULE_WMM_UAPSD //


/* max UAPSD buffer queue size */
#define MAX_PACKETS_IN_UAPSD_QUEUE	16  /* for each AC = 16*4 = 64 */


/* Public function list */
UAPSD_EXTERN VOID UAPSD_Init(
    IN  PRTMP_ADAPTER       ad_p);

UAPSD_EXTERN VOID UAPSD_Release(
    IN  PRTMP_ADAPTER       ad_p);

UAPSD_EXTERN VOID UAPSD_SP_Close(
    IN  PRTMP_ADAPTER       ad_p,
	IN	MAC_TABLE_ENTRY		*entry_p);

UAPSD_EXTERN VOID UAPSD_AllPacketDeliver(
    IN  PRTMP_ADAPTER       ad_p,
    IN  MAC_TABLE_ENTRY     *entry_p);

UAPSD_EXTERN VOID UAPSD_AssocParse(
    IN  PRTMP_ADAPTER       ad_p,
    IN  MAC_TABLE_ENTRY     *entry_p,
    IN  UCHAR               *elm_p);

UAPSD_EXTERN VOID UAPSD_PacketEnqueue(
    IN  PRTMP_ADAPTER       ad_p,
    IN  MAC_TABLE_ENTRY     *entry_p,
    IN  PNDIS_PACKET        packet_p,
    IN  UINT32              ac_id);

UAPSD_EXTERN VOID UAPSD_QoSNullTxDoneHandle(
    IN  PRTMP_ADAPTER       ad_p,
    IN  PNDIS_PACKET        packet_p,
    IN  UCHAR               *dst_mac_p);

UAPSD_EXTERN VOID UAPSD_QueueMaintenance(
    IN  PRTMP_ADAPTER       ad_p,
    IN  MAC_TABLE_ENTRY     *entry_p);

UAPSD_EXTERN VOID UAPSD_SP_AUE_Handle(
	IN RTMP_ADAPTER			*ad_p,
    IN MAC_TABLE_ENTRY		*entry_p,
	IN UINT8				flg_success);

UAPSD_EXTERN VOID UAPSD_SP_CloseInRVDone(
    IN  PRTMP_ADAPTER       ad_p);

UAPSD_EXTERN VOID UAPSD_SP_PacketCheck(
    IN  PRTMP_ADAPTER       ad_p,
    IN  PNDIS_PACKET        packet_p,
    IN  UCHAR               *dst_mac_p);

UAPSD_EXTERN VOID UAPSD_TriggerFrameHandle(
    IN  PRTMP_ADAPTER       ad_p,
    IN  MAC_TABLE_ENTRY     *entry_p,
    IN  UCHAR               up_of_frame);

#ifdef RT2870
UAPSD_EXTERN VOID UAPSD_TagFrame(
	IN RTMP_ADAPTER		*ad_p,
	IN NDIS_PACKET 		*pkt_p, 
	IN UCHAR			Wcid,
	IN UINT32			offset);

UAPSD_EXTERN VOID UAPSD_UnTagFrame(
	IN RTMP_ADAPTER 	*ad_p,
	IN UCHAR			que_idx,
	IN UINT32			bulkStartPos,
	IN UINT32			bulkEnPos);
#endif // RT2870 //

/* End of ap_uapsd.h */
