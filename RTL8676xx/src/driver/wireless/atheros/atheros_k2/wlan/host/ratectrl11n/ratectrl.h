/*
 * Copyright (c) 2000-2002 Atheros Communications, Inc., All Rights Reserved
 *
 * Definitions for core driver
 * This is a common header file for all platforms and operating systems.
 */
#ifndef _RATECTRL_H_
#define _RATECTRL_H_

#include <adf_os_types.h>
#include <adf_os_dma.h>
#include <adf_os_timer.h>
#include <adf_os_lock.h>
#include <adf_os_io.h>
#include <adf_os_mem.h>
#include <adf_os_util.h>
#include <adf_os_stdtypes.h>
#include <adf_os_defer.h>
#include <adf_os_atomic.h>
#include <adf_nbuf.h>
#include <adf_net.h>
#include <adf_net_types.h>

#include <if_media.h>
#include <ieee80211_var.h>

#include "if_athrate.h"
#include "if_athvar.h"

/*
 * Set configuration parameters here; they cover multiple files.
 */
#define TURBO_PRIME     1
#define ATHEROS_DEBUG   1

#define TRUE_NON_UAPSD  TRUE
#define TRUE_UAPSD      0x2
#define TRUE_ALL        (TRUE_UAPSD | TRUE_NON_UAPSD)

#define RC_UAPSD_RATE(_table, _idx) \
    ((_table)->info[_idx].valid & TRUE_UAPSD)
#define RC_NODE_IS_UAPSD(_node)     \
    ((_node)->an_node.ni_flags & IEEE80211_NODE_UAPSD)

#if 0 //shliu: 1006
/*
 * Compatibility shims.  We leverage the work already done for the hal.
 */
typedef u_int8_t        a_uint8_t;
typedef int8_t          a_int8_t;
typedef u_int16_t       a_uint16_t;
typedef int16_t         a_int16_t;
typedef u_int32_t       a_uint32_t;
typedef int32_t         a_int32_t;
typedef int             a_bool_t;
#endif

#define FALSE   0

/* For legacy validity */

#define TRUE    1

typedef a_int8_t          A_RSSI;
typedef a_int32_t         A_RSSI32;

typedef a_uint8_t        WLAN_PHY;

#ifndef INLINE
#define INLINE           adf_os_inline
#endif

#ifndef A_MIN
#define A_MIN(a,b)      ((a)<(b)?(a):(b))
#endif

#ifndef A_MAX
#define A_MAX(a,b)      ((a)>(b)?(a):(b))
#endif

/*
 * Use the hal os glue code to get ms time; we supply
 * a null arg because we know it's not needed.
 */
#define A_MS_TICKGET()  OS_GETUPTIME(NULL)
#define A_MEM_ZERO(p,s) OS_MEMZERO(p,s)

#define WLAN_PHY_OFDM   IEEE80211_T_OFDM
#define WLAN_PHY_TURBO  IEEE80211_T_TURBO
#define WLAN_PHY_CCK    IEEE80211_T_CCK
#define WLAN_PHY_XR (IEEE80211_T_TURBO+1)

enum {
    WLAN_RC_PHY_CCK,
    WLAN_RC_PHY_OFDM,
    WLAN_RC_PHY_TURBO,
    WLAN_RC_PHY_XR,
    WLAN_RC_PHY_HT_20_SS,
    WLAN_RC_PHY_HT_20_DS,
    WLAN_RC_PHY_HT_40_SS,
    WLAN_RC_PHY_HT_40_DS,
    WLAN_RC_PHY_HT_20_SS_HGI,
    WLAN_RC_PHY_HT_20_DS_HGI,
    WLAN_RC_PHY_HT_40_SS_HGI,
    WLAN_RC_PHY_HT_40_DS_HGI,
    WLAN_RC_PHY_MAX
};

#define IS_CHAN_TURBO(_c)   (((_c)->channelFlags & CHANNEL_TURBO) != 0)
#define IS_CHAN_2GHZ(_c)    (((_c)->channelFlags & CHANNEL_2GHZ) != 0)

#define PKTLOG_RATE_CTL_FIND(_sc, log_data, flags)   ath_log_rcfind(_sc, log_data, flags);
#define PKTLOG_RATE_CTL_UPDATE(_sc, log_data, flags) ath_log_rcupdate(_sc, log_data, flags);
#define ASSERT(condition)

#define WIRELESS_MODE_11a       IEEE80211_MODE_11A
#define WIRELESS_MODE_11b       IEEE80211_MODE_11B
#define WIRELESS_MODE_11g       IEEE80211_MODE_11G
#define WIRELESS_MODE_TURBO     IEEE80211_MODE_TURBO_A  /* NB: diff reduction */
#define WIRELESS_MODE_108a      IEEE80211_MODE_TURBO_A
#define WIRELESS_MODE_108g      IEEE80211_MODE_TURBO_G
#define WIRELESS_MODE_11NA      IEEE80211_MODE_11NA
#define WIRELESS_MODE_11NG      IEEE80211_MODE_11NG
#define WIRELESS_MODE_XR        IEEE80211_MODE_MAX
#define WIRELESS_MODE_N         IEEE80211_MODE_MAX+1
#define WIRELESS_MODE_MAX       IEEE80211_MODE_MAX+2

#define RX_FLIP_THRESHOLD       3       /* XXX */
#define MAX_TX_RATE_TBL         64
#define MAX_TX_RATE_PHY     48

/*
 * State structures for new rate adaptation code
 *
 * NOTE: Modifying these structures will impact
 * the Perl script that parses packet logging data.
 * See the packet logging module for more information.
 */
typedef struct TxRateCrtlState_s {
    A_RSSI rssiThres;           /* required rssi for this rate (dB) */
    a_uint8_t per;                /* recent estimate of packet error rate (%) */
} TxRateCtrlState;

typedef struct TxRateCtrl_s {
    TxRateCtrlState state[MAX_TX_RATE_TBL];                         /* state for each rate */
    A_RSSI   rssiLast;            /* last ack rssi */
    A_RSSI   rssiLastLkup;        /* last ack rssi used for lookup */
    A_RSSI   rssiLastPrev;        /* previous last ack rssi */
    A_RSSI   rssiLastPrev2;       /* 2nd previous last ack rssi */
    A_RSSI32 rssiSumCnt;          /* count of rssiSum for averaging */
    A_RSSI32 rssiSumRate;         /* rate that we are averaging */
    A_RSSI32 rssiSum;             /* running sum of rssi for averaging */
    a_uint32_t validTxRateMask;     /* mask of valid rates */
    a_uint8_t  rateTableSize;       /* rate table size */
    a_uint8_t  rateMax;             /* max rate that has recently worked */
    a_uint8_t  probeRate;           /* rate we are probing at */
    a_int8_t   antFlipCnt;          /* number of consec times retry=2,3 */
    a_uint8_t  misc[16];            /* miscellaneous state */
    a_uint32_t rssiTime;            /* msec timestamp for last ack rssi */
    a_uint32_t rssiDownTime;        /* msec timestamp for last down step */
    a_uint32_t probeTime;           /* msec timestamp for last probe */
//    a_uint8_t rate;                 /* last rate we returned */
    a_uint8_t  hwMaxRetryRate;      /* rate of last max retry fail */
    a_uint8_t  hwMaxRetryPktCnt;    /* num packets since we got HW max retry error */
    a_uint8_t  antProbeCnt;         /* number of packets since ant probe */
    a_uint8_t  recommendedPrimeState;   /* 0 = regular (11a/11g); 1 = Turbo (Turbo A/108g) */
    a_uint8_t  switchCount;        /* num consec frames sent at rate past the mode switch threshold */
    a_uint8_t  maxValidRate;       /* maximum number of valid rate */
    a_uint8_t  maxValidTurboRate;  /* maximum number of valid turbo rate */
    a_uint8_t  validRateIndex[MAX_TX_RATE_TBL];    /* rc Index is valid for this Sib */
    a_uint32_t perDownTime;         /* msec timstamp for last PER down step */

    /* 11n state */
    a_uint8_t  validPhyRateCount[WLAN_RC_PHY_MAX]; /* valid rate count */
    a_uint8_t  validPhyRateIndex[WLAN_RC_PHY_MAX][MAX_TX_RATE_TBL]; /* index */
    a_uint8_t  rcPhyState;
    a_uint8_t  rcPhyMode;
    a_uint8_t  rateMaxPhy;          /* Phy index for the max rate */
    a_uint8_t  rateMaxIndex;        /* index for the max within the phy */
} TX_RATE_CTRL;

/* per-node state */
struct atheros_node {
    TX_RATE_CTRL txRateCtrl;    /* rate control state proper */
    a_uint32_t lastRateKbps;      /* last rate in Kb/s */
    a_uint8_t rixMap[MAX_TX_RATE_TBL]; /* map of rate ix -> negotiated
                                   rate set ix */
    a_uint8_t htrixMap[MAX_TX_RATE_TBL]; /* map of ht rate ix -> negotiated
                                   rate set ix */
    a_uint32_t htcap;            /* ht capabilites */
    a_uint8_t antTx;              /* current transmit antenna */
};

#define ATH_NODE_ATHEROS(an)    ((struct atheros_node *)&an[1])
#define ATH_VAP_ATHEROS(vap)    ((struct atheros_vap *)&((struct ath_vap *)vap)[1])

/*
 * Rate Table structure for various modes - 'b', 'a', 'g', 'xr';
 * order of fields in info structure is important because hardcoded
 * structures are initialized within the hal for these
 */
#define RATE_TABLE_SIZE             32
typedef struct {
    int         rateCount;
    a_uint8_t     rateCodeToIndex[RATE_TABLE_SIZE]; /* backward mapping */
    struct {
        int    valid;            /* Valid for use in rate control */
        WLAN_PHY  phy;              /* CCK/OFDM/TURBO/XR */
        a_uint16_t  rateKbps;         /* Rate in Kbits per second */
        a_uint16_t  userRateKbps;     /* User rate in KBits per second */
        a_uint8_t   rateCode;         /* rate that goes into hw descriptors */
        a_uint8_t   shortPreamble;    /* Mask for enabling short preamble in rate code for CCK */
        a_uint8_t   dot11Rate;        /* Value that goes into supported rates info element of MLME */
        a_uint8_t   controlRate;      /* Index of next lower basic rate, used for duration computation */
        A_RSSI    rssiAckValidMin;  /* Rate control related information */
        A_RSSI    rssiAckDeltaMin;  /* Rate control related information */
    a_uint16_t  lpAckDuration;    /* long preamble ACK duration */
    a_uint16_t  spAckDuration;    /* short preamble ACK duration*/
        a_uint32_t  max4msFrameLen;   /* Maximum frame length(bytes) for 4ms tx duration */
        struct {
            a_uint32_t  word4Retries;
            a_uint32_t  word5Rates;
        } normalSched;
        struct {
            a_uint32_t  word4Retries;
            a_uint32_t  word5Rates;
        } shortSched;
        struct {
            a_uint32_t  word4Retries;
            a_uint32_t  word5Rates;
        } probeSched;
        struct {
            a_uint32_t  word4Retries;
            a_uint32_t  word5Rates;
        } probeShortSched;
        struct {
            a_uint32_t  word4Retries;
            a_uint32_t  word5Rates;
        } uapsd_normalSched;
        struct {
            a_uint32_t  word4Retries;
            a_uint32_t  word5Rates;
        } uapsd_shortSched;
#ifdef ATH_REMOVE_5G_RATE_TABLE
    #ifdef ATH_REMOVE_TURBO_RATE_TABLE
        #ifdef ATH_REMOVE_XR_RATE_TABLE
    } info[12];
        #else
    } info[32];
        #endif
    #else
    } info[32];
    #endif
#else
    } info[32];
#endif
    a_uint32_t    probeInterval;        /* interval for ratectrl to probe for
                     other rates */
    a_uint32_t    rssiReduceInterval;   /* interval for ratectrl to reduce RSSI */
    a_uint8_t     regularToTurboThresh; /* upperbound on regular (11a or 11g)
                     mode's rate before switching to turbo*/
    a_uint8_t     turboToRegularThresh; /* lowerbound on turbo mode's rate before
                     switching to regular */
    a_uint8_t     pktCountThresh;       /* mode switch recommendation criterion:
                     number of consecutive packets sent at
                     rate beyond the rate threshold */
    a_uint8_t     initialRateMax;       /* the initial rateMax value used in
                     rcSibUpdate() */
    a_uint8_t     numTurboRates;        /* number of Turbo rates in the rateTable */
    a_uint8_t     xrToRegularThresh;    /* threshold to switch to Normal mode */
} RATE_TABLE;

/* per-vap state */
struct atheros_vap {
        const void        *rateTable;
};
/* per-device state */
struct atheros_softc {
        struct ath_ratectrl     arc;
        /* phy tables that contain rate control data */
        void        *hwRateTable[WIRELESS_MODE_MAX];
        int                     fixedrix;       /* -1 or index of fixed rate */
        int                     fixedAutoMaxrix; /* fixed auto max rate index */
};

/* preferred Mode when rates overlap */
typedef struct {
    a_uint8_t prefMode;           /* Mode of the rates preferred over overlapping rates of other specifed mode */
    a_uint8_t ovlMode;            /* Mode with Overlapping rates */
} RATE_OVL_POLICY;

#ifdef ATHEROS_DEBUG
#undef DPRINTF
#define DPRINTF(sc, _fmt, ...) do {                             \
        if (sc->sc_debug & 0x10)                                \
                ath_hal_printf(NULL, _fmt, __VA_ARGS__);        \
} while (0)

extern  void ath_hal_printf(struct ath_hal *, const char*, ...);
#else
#define DPRINTF(sc, _fmt, ...)
#endif

/*
 *  Update the SIB's rate control information
 *
 *  This should be called when the supported rates change
 *  (e.g. SME operation, wireless mode change)
 *
 *  It will determine which rates are valid for use.
 */
void
rcSibUpdate(struct ath_softc *, struct ath_node *, a_bool_t keepState);

/*
 *  This routine is called to initialize the rate control parameters
 *  in the SIB. It is called initially during system initialization
 *  or when a station is associated with the AP.
 */
void    rcSibInit(struct ath_softc *, struct ath_node *);

/*
 * Determines and returns the new Tx rate index.
 */ 
a_uint16_t    rcRateFind(struct ath_softc *, struct atheros_node *,
            a_uint32_t frameLen,const  RATE_TABLE *pRateTable);

/*
 * This routine is called by the Tx interrupt service routine to give
 * the status of previous frames.
 */
void    rcUpdate(struct ath_softc *sc, struct ath_node *an,
        a_bool_t Xretries, int txRate, int retries, A_RSSI rssiAck,
        a_uint8_t curTxAnt,const RATE_TABLE *pRateTable);


#if 0//shliu: not support ar5212
void    atheros_setuptable(RATE_TABLE *rt);
void    ar5211SetupRateTables(void);
void    ar5211AttachRateTables(struct atheros_softc *sc);
void    ar5212SetupRateTables(void);
void    ar5212AttachRateTables(struct atheros_softc *sc);
void    ar5212SetFullRateTable(struct atheros_softc *sc);
void    ar5212SetHalfRateTable(struct atheros_softc *sc);
void    ar5212SetQuarterRateTable(struct atheros_softc *sc);
#endif
void    ar5416SetupRateTables(void);
void    ar5416AttachRateTables(struct atheros_softc *sc);
void    ar5416SetFullRateTable(struct atheros_softc *sc);
void    ar5416SetHalfRateTable(struct atheros_softc *sc);
void    ar5416SetQuarterRateTable(struct atheros_softc *sc);
#endif /* _RATECTRL_H_ */
