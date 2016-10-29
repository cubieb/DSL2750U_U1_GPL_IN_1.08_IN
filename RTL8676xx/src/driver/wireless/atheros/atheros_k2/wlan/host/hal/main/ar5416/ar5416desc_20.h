#ifndef _ATH_AR5416_DESC__20_H_
#define _ATH_AR5416_DESC__20_H_

/* Owl 2.0 descriptor. */

#include <adf_os_prepack.h>
adf_os_packed_pre struct ar5416_desc_20 {
    a_uint32_t   ds_link;    /* link pointer */
    a_uint32_t   ds_data;    /* data buffer pointer */
    a_uint32_t   ds_ctl0;    /* DMA control 0 */
    a_uint32_t   ds_ctl1;    /* DMA control 1 */
    union {
        struct { /* tx desc has 8 control words(for owl1.0)/12 
                    control words(for owl2.0) + 10 status words */
            a_uint32_t   ctl2;
            a_uint32_t   ctl3;
            a_uint32_t   ctl4;
            a_uint32_t   ctl5;
            a_uint32_t   ctl6;
            a_uint32_t   ctl7;
            a_uint32_t   ctl8;
            a_uint32_t   ctl9;
            a_uint32_t	ctl10;
            a_uint32_t	ctl11;
            a_uint32_t   status0;
            a_uint32_t   status1;
            a_uint32_t   status2;
            a_uint32_t   status3;
            a_uint32_t   status4;
            a_uint32_t   status5;
            a_uint32_t   status6;
            a_uint32_t   status7;
            a_uint32_t   status8;
            a_uint32_t   status9;
        } tx;
        struct { /* rx desc has 2 control words + 9 status words */
            a_uint32_t   status0;
            a_uint32_t   status1;
            a_uint32_t   status2;
            a_uint32_t   status3;
            a_uint32_t   status4;
            a_uint32_t   status5;
            a_uint32_t   status6;
            a_uint32_t   status7;
            a_uint32_t   status8;
        } rx;
    } u;
} adf_os_packed_post;
#include <adf_os_postpack.h>

#define AR5416DESC_20(_ds) ((struct ar5416_desc_20 *)(_ds))
#define AR5416DESC_CONST_20(_ds) ((const struct ar5416_desc_20 *)(_ds))

	/* TX Functions */

extern  HAL_BOOL ar5416UpdateCTSForBursting_20(struct ath_hal *, struct ath_desc *,
         struct ath_desc *,struct ath_desc *, struct ath_desc *,
         a_uint32_t, a_uint32_t);
extern  HAL_BOOL ar5416SetupTxDesc_20(struct ath_hal *ah, struct ath_desc *ds,
        a_uint32_t pktLen, a_uint32_t hdrLen, HAL_PKT_TYPE type, a_uint32_t txPower,
        a_uint32_t txRate0, a_uint32_t txTries0,
        a_uint32_t keyIx, a_uint32_t antMode, a_uint32_t flags,
        a_uint32_t rtsctsRate, a_uint32_t rtsctsDuration,
        a_uint32_t compicvLen, a_uint32_t compivLen, a_uint32_t comp);
extern  HAL_BOOL ar5416SetupXTxDesc_20(struct ath_hal *, struct ath_desc *,
        a_uint32_t txRate1, a_uint32_t txRetries1,
        a_uint32_t txRate2, a_uint32_t txRetries2,
        a_uint32_t txRate3, a_uint32_t txRetries3);
extern  HAL_BOOL ar5416FillTxDesc_20(struct ath_hal *ah, struct ath_desc *ds,
        a_uint32_t segLen, HAL_BOOL firstSeg, HAL_BOOL lastSeg,
        const struct ath_desc *ds0);
extern  HAL_STATUS ar5416ProcTxDesc_20(struct ath_hal *ah, struct ath_desc *);

extern void ar5416IntrReqTxDesc_20(struct ath_hal *ah, struct ath_desc *ds);
extern void ar5416Set11nTxDesc_20(struct ath_hal *ah, struct ath_desc *ds,
       a_uint32_t pktLen, HAL_PKT_TYPE type, a_uint32_t txPower,
       a_uint32_t keyIx, HAL_KEY_TYPE keyType, a_uint32_t flags);
extern void ar5416Set11nRateScenario_20(struct ath_hal *ah, struct ath_desc *ds,
       a_uint32_t durUpdateEn, a_uint32_t rtsctsRate, a_uint32_t rtsctsDuration, HAL_11N_RATE_SERIES series[], 
       a_uint32_t nseries, a_uint32_t flags);
extern void ar5416Set11nAggrFirst_20(struct ath_hal *ah, struct ath_desc *ds,
       a_uint32_t aggrLen, a_uint32_t numDelims);
extern void ar5416Set11nAggrMiddle_20(struct ath_hal *ah, struct ath_desc *ds,
       a_uint32_t numDelims);
extern void ar5416Set11nAggrLast_20(struct ath_hal *ah, struct ath_desc *ds);
extern void ar5416Clr11nAggr_20(struct ath_hal *ah, struct ath_desc *ds);

extern void ar5416Set11nBurstDuration_20(struct ath_hal *ah, struct ath_desc *ds,
       a_uint32_t burstDuration);
extern void ar5416Set11nVirtualMoreFrag_20(struct ath_hal *ah, struct ath_desc *ds,
       a_uint32_t vmf);

        /* RX Functions */

extern  HAL_BOOL ar5416SetupRxDesc_20(struct ath_hal *,
        struct ath_desc *, a_uint32_t size, a_uint32_t flags);
extern  HAL_STATUS ar5416ProcRxDescFast_20(struct ath_hal *ah, 
                                           struct ath_desc *, a_uint32_t,
                                           struct ath_desc *,
                                           struct ath_rx_status *);

#endif
