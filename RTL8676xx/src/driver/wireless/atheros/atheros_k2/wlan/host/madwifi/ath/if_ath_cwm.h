/*
 * Copyright (c) 2005 Atheros Communications, Inc.
 * All rights reserved.
 *
 */
#ifndef _ATH_CWM_H_
#define _ATH_CWM_H_

/*
 * Channel Width Management (CWM)
 */
 

/*
 * External Definitions 
 */
#define ATH_CWM_EXTCH_BUSY_THRESHOLD  30  /* Extension Channel Busy Threshold (0-100%) */


/*
 * External Structures 
 */



/*
 * External Function Prototypes
 */

struct ath_softc;

a_int32_t  ath_cwm_attach(struct ath_softc *sc);
void ath_cwm_detach(struct ath_softc *sc);
void ath_cwm_init(struct ath_softc *sc);
void ath_cwm_stop(struct ath_softc *sc);
void ath_cwm_newstate(struct ieee80211vap *vap, a_uint8_t state);
a_int32_t  ath_cwm_ioctl(struct ath_softc *sc, a_int32_t cmd, a_uint32_t * data);
void ath_cwm_newchwidth(struct ieee80211_node *ni);
a_uint32_t ath_cwm_getextbusy(struct ath_softc *sc);
void ath_cwm_txtimeout(struct ath_softc *sc);
void ath_cwm_gethwstate(struct ath_softc *sc, HAL_HT_CWM *cwm);
a_int32_t  ath_cwm_ht40allowed(struct ath_softc *sc);
void ath_cwm_setlocalextoff(struct ieee80211vap *vap);

#endif /* _ATH_CWM_H_ */
