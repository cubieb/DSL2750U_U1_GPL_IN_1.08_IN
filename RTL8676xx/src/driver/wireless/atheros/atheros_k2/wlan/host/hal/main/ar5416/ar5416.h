/*
 * Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
 * Copyright (c) 2002-2005 Atheros Communications, Inc.
 * All rights reserved.
 *
 * $Id: //depot/sw/carriersrc/branches/magpie_host_dev/split_src/host/hal/main/ar5416/ar5416.h#15 $
 */
#ifndef _ATH_AR5416_H_
#define _ATH_AR5416_H_

#include "ah_eeprom.h"
#include "ah_devid.h"
#include <ar5416desc.h>
#ifdef K2
#include "ar5416eep.h"  /* For Eeprom definitions */
#endif

#define AR5416_MAGIC            0x19641014

#define AR_SREV_HOWL(ah) ((AH_PRIVATE(ah)->ah_macVersion) == AR_SREV_VERSION_HOWL)
#define IS_5416_HOWL AR_SREV_HOWL

/* DCU Transmit Filter macros */
#define CALC_MMR(dcu, idx) \
    ( (4 * dcu) + (idx < 32 ? 0 : (idx < 64 ? 1 : (idx < 96 ? 2 : 3))) )
#define TXBLK_FROM_MMR(mmr) \
    (AR_D_TXBLK_BASE + ((mmr & 0x1f) << 6) + ((mmr & 0x20) >> 3))
#define CALC_TXBLK_ADDR(dcu, idx)   (TXBLK_FROM_MMR(CALC_MMR(dcu, idx)))
#define CALC_TXBLK_VALUE(idx)       (1 << (idx & 0x1f))

/* MAC register values */


#ifdef AR5416_INT_MITIGATION
#define INIT_INTERRUPT_MASK \
    ( AR_IMR_TXERR  | AR_IMR_RXORN | AR_IMR_RXERR  | AR_IMR_TXURN | \
      AR_IMR_TXMINTR | AR_IMR_RXMINTR | AR_IMR_TXINTM | AR_IMR_RXINTM)
#else
#define INIT_INTERRUPT_MASK \
    ( AR_IMR_TXERR  | AR_IMR_TXOK | AR_IMR_RXORN | \
      AR_IMR_RXERR  | AR_IMR_RXOK | AR_IMR_TXURN | )
#endif

#define INIT_BEACON_CONTROL \
    ((INIT_RESET_TSF << 24)  | (INIT_BEACON_EN << 23) | \
      (INIT_TIM_OFFSET << 16) | INIT_BEACON_PERIOD)

#define INIT_CONFIG_STATUS  0x00000000
#define INIT_RSSI_THR       0x00000700  /* Missed beacon counter initialized to 0x7 (max is 0xff) */
#define INIT_IQCAL_LOG_COUNT_MAX    0x2
#define INIT_BCON_CNTRL_REG 0x00000000

/*
 * Various fifo fill before Tx start, in 64-byte units
 * i.e. put the frame in the air while still DMAing
 */
#define MIN_TX_FIFO_THRESHOLD   0x1
#define MAX_TX_FIFO_THRESHOLD   (( 4096 / 64) - 1)
#define INIT_TX_FIFO_THRESHOLD  MIN_TX_FIFO_THRESHOLD

#define IS_SPUR_CHAN(_chan) \
    ( (((_chan)->channel % 32) != 0) && \
        ((((_chan)->channel % 32) < 10) || (((_chan)->channel % 32) > 22)) )

/*
 * Gain support.
 */
#define NUM_CORNER_FIX_BITS_2133    7
#define CCK_OFDM_GAIN_DELTA         15
#define MERLIN_TX_GAIN_TABLE_SIZE   22

enum GAIN_PARAMS {
    GP_TXCLIP,
    GP_PD90,
    GP_PD84,
    GP_GSEL
};

enum GAIN_PARAMS_2133 {
    GP_MIXGAIN_OVR,
    GP_PWD_138,
    GP_PWD_137,
    GP_PWD_136,
    GP_PWD_132,
    GP_PWD_131,
    GP_PWD_130,
};

enum {
    HAL_RESET_POWER_ON,
    HAL_RESET_WARM,
    HAL_RESET_COLD,
};

typedef struct _gainOptStep {
    a_int16_t paramVal[NUM_CORNER_FIX_BITS_2133];
    a_int32_t stepGain;
    a_uint8_t  stepName[16];
} GAIN_OPTIMIZATION_STEP;

typedef struct {
    a_uint32_t   numStepsInLadder;
    a_uint32_t   defaultStepNum;
    GAIN_OPTIMIZATION_STEP optStep[10];
} GAIN_OPTIMIZATION_LADDER;

typedef struct {
    a_uint32_t   currStepNum;
    a_uint32_t   currGain;
    a_uint32_t   targetGain;
    a_uint32_t   loTrig;
    a_uint32_t   hiTrig;
    a_uint32_t   gainFCorrection;
    a_uint32_t   active;
    const GAIN_OPTIMIZATION_STEP *currStep;
} GAIN_VALUES;

typedef struct {
    a_uint16_t   synth_center;
    a_uint16_t   ctl_center;
    a_uint16_t   ext_center;
} CHAN_CENTERS;

/* RF HAL structures */
typedef struct RfHalFuncs {
    void      (*rfDetach)(struct ath_hal *ah);
    void      (*writeRegs)(struct ath_hal *,
            a_uint32_t modeIndex, a_uint32_t freqIndex, a_int32_t regWrites);
    HAL_BOOL  (*setChannel)(struct ath_hal *, HAL_CHANNEL_INTERNAL *, HAL_HT *);
    HAL_BOOL  (*setRfRegs)(struct ath_hal *,
              HAL_CHANNEL_INTERNAL *, a_uint16_t modesIndex);
    HAL_BOOL  (*getChipPowerLim)(struct ath_hal *ah, HAL_CHANNEL *chans,
                       a_uint32_t nchancs);
    void      (*decreaseChainPower)(struct ath_hal *ah, HAL_CHANNEL *chans);                       
} RF_HAL_FUNCS;

/*
 * Per-channel ANI state private to the driver.
 */
struct ar5416AniState {
    HAL_CHANNEL c;
    a_uint8_t    noiseImmunityLevel;
    a_uint8_t    spurImmunityLevel;
    a_uint8_t    firstepLevel;
    a_uint8_t    ofdmWeakSigDetectOff;
    a_uint8_t    cckWeakSigThreshold;

    /* Thresholds */
    a_uint32_t   listenTime;
    a_uint32_t   ofdmTrigHigh;
    a_uint32_t   ofdmTrigLow;
    a_int32_t     cckTrigHigh;
    a_int32_t     cckTrigLow;
    a_int32_t     rssiThrLow;
    a_int32_t     rssiThrHigh;

    a_uint32_t   noiseFloor; /* The current noise floor */
    a_uint32_t   txFrameCount;   /* Last txFrameCount */
    a_uint32_t   rxFrameCount;   /* Last rx Frame count */
    a_uint32_t   cycleCount; /* Last cycleCount (can detect wrap-around) */
    a_uint32_t   ofdmPhyErrCount;/* OFDM err count since last reset */
    a_uint32_t   cckPhyErrCount; /* CCK err count since last reset */
    a_uint32_t   ofdmPhyErrBase; /* Base value for ofdm err counter */
    a_uint32_t   cckPhyErrBase;  /* Base value for cck err counters */
    a_int16_t     pktRssi[2]; /* Average rssi of pkts for 2 antennas */
    a_int16_t     ofdmErrRssi[2]; /* Average rssi of ofdm phy errs for 2 ant */
    a_int16_t     cckErrRssi[2];  /* Average rssi of cck phy errs for 2 ant */
};

#define HAL_PROCESS_ANI     0x00000001  /* ANI state setup */
#define HAL_RADAR_EN        0x80000000  /* Radar detect is capable */
#define HAL_AR_EN       0x40000000  /* AR detect is capable */

#define DO_ANI(ah) \
    ((AH5416(ah)->ah_procPhyErr & HAL_PROCESS_ANI))

struct ar5416Stats {
    a_uint32_t   ast_ani_niup;   /* ANI increased noise immunity */
    a_uint32_t   ast_ani_nidown; /* ANI decreased noise immunity */
    a_uint32_t   ast_ani_spurup; /* ANI increased spur immunity */
    a_uint32_t   ast_ani_spurdown;/* ANI descreased spur immunity */
    a_uint32_t   ast_ani_ofdmon; /* ANI OFDM weak signal detect on */
    a_uint32_t   ast_ani_ofdmoff;/* ANI OFDM weak signal detect off */
    a_uint32_t   ast_ani_cckhigh;/* ANI CCK weak signal threshold high */
    a_uint32_t   ast_ani_ccklow; /* ANI CCK weak signal threshold low */
    a_uint32_t   ast_ani_stepup; /* ANI increased first step level */
    a_uint32_t   ast_ani_stepdown;/* ANI decreased first step level */
    a_uint32_t   ast_ani_ofdmerrs;/* ANI cumulative ofdm phy err count */
    a_uint32_t   ast_ani_cckerrs;/* ANI cumulative cck phy err count */
    a_uint32_t   ast_ani_reset;  /* ANI parameters zero'd for non-STA */
    a_uint32_t   ast_ani_lzero;  /* ANI listen time forced to zero */
    a_uint32_t   ast_ani_lneg;   /* ANI listen time calculated < 0 */
    HAL_MIB_STATS   ast_mibstats;   /* MIB counter stats */
    HAL_NODE_STATS  ast_nodestats;  /* Latest rssi stats from driver */
};

struct ar5416RadReader {
    a_uint16_t   rd_index;
    a_uint16_t   rd_expSeq;
    a_uint32_t   rd_resetVal;
    a_uint8_t    rd_start;
};

struct ar5416RadWriter {
    a_uint16_t   wr_index;
    a_uint16_t   wr_seq;
};

struct ar5416RadarEvent {
    a_uint32_t   re_ts;      /* 32 bit time stamp */
    a_uint8_t    re_rssi;    /* rssi of radar event */
    a_uint8_t    re_dur;     /* duration of radar pulse */
    a_uint8_t    re_chanIndex;   /* Channel of event */
};

struct ar5416RadarQElem {
    a_uint32_t   rq_seqNum;
    a_uint32_t   rq_busy;        /* 32 bit to insure atomic read/write */
    struct ar5416RadarEvent rq_event;   /* Radar event */
};

struct ar5416RadarQInfo {
    a_uint16_t   ri_qsize;       /* q size */
    a_uint16_t   ri_seqSize;     /* Size of sequence ring */
    struct ar5416RadReader ri_reader;   /* State for the q reader */
    struct ar5416RadWriter ri_writer;   /* state for the q writer */
};

#define HAL_MAX_ACK_RADAR_DUR   511
#define HAL_MAX_NUM_PEAKS   3
#define HAL_ARQ_SIZE        4096        /* 8K AR events for buffer size */
#define HAL_ARQ_SEQSIZE     4097        /* Sequence counter wrap for AR */
#define HAL_RADARQ_SIZE     1024        /* 1K radar events for buffer size */
#define HAL_RADARQ_SEQSIZE  1025        /* Sequence counter wrap for radar */
#define HAL_NUMRADAR_STATES 64      /* Number of radar channels we keep state for */

struct ar5416ArState {
    a_uint16_t   ar_prevTimeStamp;
    a_uint32_t   ar_prevWidth;
    a_uint32_t   ar_phyErrCount[HAL_MAX_ACK_RADAR_DUR];
    a_uint32_t   ar_ackSum;
    a_uint16_t   ar_peakList[HAL_MAX_NUM_PEAKS];
    a_uint32_t   ar_packetThreshold; /* Thresh to determine traffic load */
    a_uint32_t   ar_parThreshold;    /* Thresh to determine peak */
    a_uint32_t   ar_radarRssi;       /* Rssi threshold for AR event */
};

struct ar5416RadarState {
    HAL_CHANNEL_INTERNAL *rs_chan;      /* Channel info */
    a_uint8_t    rs_chanIndex;       /* Channel index in radar structure */
    a_uint32_t   rs_numRadarEvents;  /* Number of radar events */
    a_int32_t     rs_firpwr;      /* Thresh to check radar sig is gone */
    a_uint32_t   rs_radarRssi;       /* Thresh to start radar det (dB) */
    a_uint32_t   rs_height;      /* Thresh for pulse height (dB)*/
    a_uint32_t   rs_pulseRssi;       /* Thresh to check if pulse is gone (dB) */
    a_uint32_t   rs_inband;      /* Thresh to check if pusle is inband (0.5 dB) */
};

#define AR5416_OPFLAGS_11A           0x01   /* if set, allow 11a */
#define AR5416_OPFLAGS_11G           0x02   /* if set, allow 11g */
#define AR5416_OPFLAGS_N_5G_HT40     0x04   /* if set, disable 5G HT40 */
#define AR5416_OPFLAGS_N_2G_HT40     0x08   /* if set, disable 2G HT40 */
#define AR5416_OPFLAGS_N_5G_HT20     0x10   /* if set, disable 5G HT20 */
#define AR5416_OPFLAGS_N_2G_HT20     0x20   /* if set, disable 2G HT20 */

#ifndef K2
/* RF silent fields in EEPROM */
#define EEP_RFSILENT_ENABLED        1
#define EEP_RFSILENT_POLARITY       0x0002
#define EEP_RFSILENT_POLARITY_S     1
#define EEP_RFSILENT_GPIO_SEL       0x001c
#define EEP_RFSILENT_GPIO_SEL_S     2

#define AR5416_EEP_NO_BACK_VER       0x1
#define AR5416_EEP_VER               0xE
#define AR5416_EEP_VER_MINOR_MASK    0x0FFF
#define AR5416_EEP_MINOR_VER_2       0x2
#define AR5416_EEP_MINOR_VER_3       0x3
#endif

#ifdef MAGPIE_MERLIN
#define AR5416_EEP_MINOR_VER_7       0x7
#define AR5416_EEP_MINOR_VER_9       0x9
#define AR5416_EEP_MINOR_VER_16      0x10
#define AR5416_EEP_MINOR_VER_17      0x11
#define AR5416_EEP_MINOR_VER_19      0x13

/* Delta from which to start power to pdadc table */

#define AR5416_PWR_TABLE_OFFSET  -5


#define AR5416_1_CHAINMASK		1
#define AR5416_2LOHI_CHAINMASK          5	
#define AR5416_2LOMID_CHAINMASK         3	
#define AR5416_3_CHAINMASK		7

/* Rx gain type values */
#define AR5416_EEP_RXGAIN_23dB_BACKOFF     0
#define AR5416_EEP_RXGAIN_13dB_BACKOFF     1
#define AR5416_EEP_RXGAIN_ORIG             2

/* Tx gain type values */
#define AR5416_EEP_TXGAIN_ORIGINAL         0
#define AR5416_EEP_TXGAIN_HIGH_POWER       1

#endif


#ifndef K2
// 16-bit offset location start of calibration struct
#define AR5416_EEP_START_LOC         256
#define AR5416_NUM_5G_CAL_PIERS      8
#define AR5416_NUM_2G_CAL_PIERS      4
#define AR5416_NUM_5G_20_TARGET_POWERS  8
#define AR5416_NUM_5G_40_TARGET_POWERS  8
#define AR5416_NUM_2G_CCK_TARGET_POWERS 3
#define AR5416_NUM_2G_20_TARGET_POWERS  4
#define AR5416_NUM_2G_40_TARGET_POWERS  4
#define AR5416_NUM_CTLS              24
#define AR5416_NUM_BAND_EDGES        8
#define AR5416_NUM_PD_GAINS          4
#define AR5416_PD_GAINS_IN_MASK      4
#define AR5416_PD_GAIN_ICEPTS        5
#define AR5416_EEPROM_MODAL_SPURS    5
#define AR5416_MAX_RATE_POWER        63
#define AR5416_NUM_PDADC_VALUES      128
#define AR5416_NUM_RATES             16
#define AR5416_BCHAN_UNUSED          0xFF
#define AR5416_MAX_PWR_RANGE_IN_HALF_DB 64
#define AR5416_EEPMISC_BIG_ENDIAN    0x01
#define FREQ2FBIN(x,y) ((y) ? ((x) - 2300) : (((x) - 4800) / 5))
#endif
#define AR5416_MAX_CHAINS            3
#ifndef K2
#define AR5416_ANT_16S               25

#define AR5416_NUM_ANT_CHAIN_FIELDS     7
#define AR5416_NUM_ANT_COMMON_FIELDS    4
#define AR5416_SIZE_ANT_CHAIN_FIELD     3
#define AR5416_SIZE_ANT_COMMON_FIELD    4
#define AR5416_ANT_CHAIN_MASK           0x7
#define AR5416_ANT_COMMON_MASK          0xf
#define AR5416_CHAIN_0_IDX              0
#define AR5416_CHAIN_1_IDX              1
#define AR5416_CHAIN_2_IDX              2

#define AR5416_LEGACY_CHAINMASK		1

typedef enum {
    EEP_NFTHRESH_5,
    EEP_NFTHRESH_2,
    EEP_MAC_MSW,
    EEP_MAC_MID,
    EEP_MAC_LSW,
    EEP_REG_0,
    EEP_REG_1,
    EEP_OP_CAP,
    EEP_OP_MODE,
    EEP_RF_SILENT,
    EEP_OB_5,
    EEP_DB_5,
    EEP_OB_2,
    EEP_DB_2,
    EEP_MINOR_REV,
    EEP_TX_MASK,
    EEP_RX_MASK,
#ifdef MAGPIE_MERLIN
    EEP_FSTCLK_5G,
    EEP_RXGAIN_TYPE,
    EEP_OL_PWRCTRL,
    EEP_TXGAIN_TYPE,
    EEP_RC_CHAIN_MASK,
#endif
} EEPROM_PARAM;

typedef enum Ar5416_Rates {
    rate6mb,  rate9mb,  rate12mb, rate18mb,
    rate24mb, rate36mb, rate48mb, rate54mb,
    rate1l,   rate2l,   rate2s,   rate5_5l,
    rate5_5s, rate11l,  rate11s,  rateXr,
    rateHt20_0, rateHt20_1, rateHt20_2, rateHt20_3,
    rateHt20_4, rateHt20_5, rateHt20_6, rateHt20_7,
    rateHt40_0, rateHt40_1, rateHt40_2, rateHt40_3,
    rateHt40_4, rateHt40_5, rateHt40_6, rateHt40_7,
    rateDupCck, rateDupOfdm, rateExtCck, rateExtOfdm,
    Ar5416RateSize
} AR5416_RATES;

#include <adf_os_prepack.h>
typedef adf_os_packed_pre struct BaseEepHeader {
    a_uint16_t  length;
    a_uint16_t  checksum;
    a_uint16_t  version;
    a_uint8_t   opCapFlags;
    a_uint8_t   eepMisc;
    a_uint16_t  regDmn[2];
    a_uint8_t   macAddr[6];
    a_uint8_t   rxMask;
    a_uint8_t   txMask;
    a_uint16_t  rfSilent;
    a_uint16_t  blueToothOptions;
    a_uint16_t  deviceCap;
    a_uint32_t  binBuildNumber;
    a_uint8_t   deviceType;
#ifdef MAGPIE_MERLIN
   a_uint8_t   pwdclkind;
    a_uint8_t   fastClk5g;
    a_uint8_t   divChain;  
    a_uint8_t   rxGainType;
    a_uint8_t   dacHiPwrMode;    // #bits used: 1, bit0: 1: use the DAC high power mode, 0: don't use the DAC high power mode (1 for MB91 only)
    a_uint8_t   openLoopPwrCntl; // #bits used: 1, bit0: 1: use open loop power control, 0: use closed loop power control
    a_uint8_t   dacLpMode;       // #bits used: bit0.  
    a_uint8_t   txGainType;      // #bits used: all, indicates high power tx gain table support. 0: original 1: high power
    a_uint8_t   rcChainMask;     // bits used: bit0, This flag will be "1" if the card is an HB93 1x2, otherwise, it will be "0".
    a_uint8_t   futureBase[24];
#else
     a_uint8_t   futureBase[33];
#endif
} adf_os_packed_post BASE_EEP_HEADER; // 64 B

typedef adf_os_packed_pre struct spurChanStruct {
    a_uint16_t spurChan;
    a_uint8_t  spurRangeLow;
    a_uint8_t  spurRangeHigh;
} adf_os_packed_post SPUR_CHAN;

typedef adf_os_packed_pre struct ModalEepHeader {
    a_uint32_t  antCtrlChain[AR5416_MAX_CHAINS];       // 12
    a_uint32_t  antCtrlCommon;                         // 4
    a_uint8_t   antennaGainCh[AR5416_MAX_CHAINS];      // 3
    a_uint8_t   switchSettling;                        // 1
    a_uint8_t   txRxAttenCh[AR5416_MAX_CHAINS];        // 3
    a_uint8_t   rxTxMarginCh[AR5416_MAX_CHAINS];       // 3
    a_uint8_t   adcDesiredSize;                        // 1
    a_uint8_t   pgaDesiredSize;                        // 1
    a_uint8_t   xlnaGainCh[AR5416_MAX_CHAINS];         // 3
    a_uint8_t   txEndToXpaOff;                         // 1
    a_uint8_t   txEndToRxOn;                           // 1
    a_uint8_t   txFrameToXpaOn;                        // 1
    a_uint8_t   thresh62;                              // 1
    a_uint8_t   noiseFloorThreshCh[AR5416_MAX_CHAINS]; // 3
    a_uint8_t   xpdGain;                               // 1
    a_uint8_t   xpd;                                   // 1
    a_uint8_t   iqCalICh[AR5416_MAX_CHAINS];           // 1
    a_uint8_t   iqCalQCh[AR5416_MAX_CHAINS];           // 1
    a_uint8_t   pdGainOverlap;                         // 1
    a_uint8_t   ob;                                    // 1
    a_uint8_t   db;                                    // 1
    a_uint8_t   xpaBiasLvl;                            // 1
    a_uint8_t   pwrDecreaseFor2Chain;                  // 1
    a_uint8_t   pwrDecreaseFor3Chain;                  // 1 -> 48 B
    a_uint8_t   txFrameToDataStart;                    // 1
    a_uint8_t   txFrameToPaOn;                         // 1
    a_uint8_t   ht40PowerIncForPdadc;                  // 1
    a_uint8_t   bswAtten[AR5416_MAX_CHAINS];           // 3
    a_uint8_t   bswMargin[AR5416_MAX_CHAINS];          // 3
    a_uint8_t   swSettleHt40;                          // 1
#ifdef MAGPIE_MERLIN
    a_uint8_t   xatten2Db[AR5416_MAX_CHAINS];          // 3 -> New for AR9280 (0xa20c/b20c 11:6)
    a_uint8_t   xatten2Margin[AR5416_MAX_CHAINS];      // 3 -> New for AR9280 (0xa20c/b20c 21:17)
    a_uint8_t   ob_ch1;                                // 1 -> ob and db become chain specific from AR9280
    a_uint8_t   db_ch1;                                // 1 
    a_uint8_t   lna_cntl_sp          : 1,              // 1 Spare bit
               force_xpaon          : 1,              // Force XPA bit for 5G mode
               local_bias           : 1,              // enable local bias
               femBandSelectUsed    : 1,              //
               xlnabufin            : 1,              //
               xlnaisel             : 2,              //
               xlnabufmode          : 1;              //
    a_uint8_t   miscBits;                              // 2 // bit0, bit1: bb_tx_dac_scale_cck
    /* a_uint8_t   futureModalMerlin;                     // 1 */
    a_uint16_t  xpaBiasLvlFreq[3];                     // 3
    a_uint8_t   futureModal[6];                        // 6 , old value:22
#else
    a_uint8_t   futureModal[22];                       // 22
#endif
    SPUR_CHAN spurChans[AR5416_EEPROM_MODAL_SPURS];   // 20 B
} adf_os_packed_post MODAL_EEP_HEADER;                          // == 100 B


#ifdef MAGPIE_MERLIN
typedef adf_os_packed_pre struct calDataPerFreqOpLoop {
    a_uint8_t pwrPdg[2][5]; /* power measurement                  */
    a_uint8_t vpdPdg[2][5]; /* pdadc voltage at power measurement */
    a_uint8_t pcdac[2][5];  /* pcdac used for power measurement   */
    a_uint8_t empty[2][5];  /* future use */
} adf_os_packed_post CAL_DATA_PER_FREQ_OP_LOOP;
#endif

typedef adf_os_packed_pre struct calDataPerFreq {
    a_uint8_t pwrPdg[AR5416_NUM_PD_GAINS][AR5416_PD_GAIN_ICEPTS];
    a_uint8_t vpdPdg[AR5416_NUM_PD_GAINS][AR5416_PD_GAIN_ICEPTS];
} adf_os_packed_post CAL_DATA_PER_FREQ;

typedef adf_os_packed_pre struct CalTargetPowerLegacy {
    a_uint8_t  bChannel;
    a_uint8_t  tPow2x[4];
} adf_os_packed_post CAL_TARGET_POWER_LEG;

typedef adf_os_packed_pre struct CalTargetPowerHt {
    a_uint8_t  bChannel;
    a_uint8_t  tPow2x[8];
} adf_os_packed_post CAL_TARGET_POWER_HT;

#if AH_BYTE_ORDER == AH_BIG_ENDIAN
typedef adf_os_packed_pre struct CalCtlEdges {
    a_uint8_t  bChannel;
    a_uint8_t  flag   :2,
             tPower :6;
} adf_os_packed_post CAL_CTL_EDGES;
#else
typedef adf_os_packed_pre struct CalCtlEdges {
    a_uint8_t  bChannel;
    a_uint8_t  tPower :6,
             flag   :2;
} adf_os_packed_post CAL_CTL_EDGES;
#endif

typedef adf_os_packed_pre struct CalCtlData {
    CAL_CTL_EDGES  ctlEdges[AR5416_MAX_CHAINS][AR5416_NUM_BAND_EDGES];
} adf_os_packed_post CAL_CTL_DATA;

typedef adf_os_packed_pre struct eepMap {
    BASE_EEP_HEADER      baseEepHeader;         // 64 B
    a_uint8_t             custData[64];                   // 64 B
    MODAL_EEP_HEADER     modalHeader[2];        // 200 B
    a_uint8_t             calFreqPier5G[AR5416_NUM_5G_CAL_PIERS];
    a_uint8_t             calFreqPier2G[AR5416_NUM_2G_CAL_PIERS];
    CAL_DATA_PER_FREQ    calPierData5G[AR5416_MAX_CHAINS][AR5416_NUM_5G_CAL_PIERS];
    CAL_DATA_PER_FREQ    calPierData2G[AR5416_MAX_CHAINS][AR5416_NUM_2G_CAL_PIERS];
    CAL_TARGET_POWER_LEG calTargetPower5G[AR5416_NUM_5G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower5GHT20[AR5416_NUM_5G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower5GHT40[AR5416_NUM_5G_40_TARGET_POWERS];
    CAL_TARGET_POWER_LEG calTargetPowerCck[AR5416_NUM_2G_CCK_TARGET_POWERS];
    CAL_TARGET_POWER_LEG calTargetPower2G[AR5416_NUM_2G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower2GHT20[AR5416_NUM_2G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower2GHT40[AR5416_NUM_2G_40_TARGET_POWERS];
    a_uint8_t             ctlIndex[AR5416_NUM_CTLS];
    CAL_CTL_DATA         ctlData[AR5416_NUM_CTLS];
    a_uint8_t             padding;
} adf_os_packed_post ar5416_eeprom_t;  // EEP_MAP
#include <adf_os_prepack.h>
#endif

/* Support for multiple INIs */
struct ar5416IniArray {
	a_uint32_t *ia_array;
	a_uint32_t ia_rows;
	a_uint32_t ia_columns;
};
#define INIT_INI_ARRAY(iniarray, array, rows, columns) do {             \
	(iniarray)->ia_array = (a_uint32_t *)(array);	\
	(iniarray)->ia_rows = (rows);		\
	(iniarray)->ia_columns = (columns);	\
} while (0)
#define INI_RA(iniarray, row, column) (((iniarray)->ia_array)[(row) * ((iniarray)->ia_columns) + (column)])

struct ath_hal_5416 {
    struct ath_hal_private  ah_priv;    /* base class */

    /*
     * Information retrieved from EEPROM.
     */
    ar5416_eeprom_t  ah_eeprom;

    GAIN_VALUES ah_gainValues;

    a_uint8_t    ah_macaddr[IEEE80211_ADDR_LEN];
    a_uint8_t    ah_bssid[IEEE80211_ADDR_LEN];
    a_uint8_t    ah_bssidmask[IEEE80211_ADDR_LEN];

    a_int16_t     ah_curchanRadIndex; /* cur. channel radar index */

    /*
     * Runtime state.
     */
    a_uint32_t   ah_maskReg;     /* copy of AR_IMR */
    struct ar5416Stats ah_stats;        /* various statistics */
    RF_HAL_FUNCS    ah_rfHal;
    a_uint32_t   ah_txDescMask;      /* mask for TXDESC */
    a_uint32_t   ah_txOkInterruptMask;
    a_uint32_t   ah_txErrInterruptMask;
    a_uint32_t   ah_txDescInterruptMask;
    a_uint32_t   ah_txEolInterruptMask;
    a_uint32_t   ah_txUrnInterruptMask;
    HAL_TX_QUEUE_INFO ah_txq[HAL_NUM_TX_QUEUES];
    HAL_POWER_MODE  ah_powerMode;
    a_uint32_t   ah_atimWindow;
    HAL_ANT_SETTING ah_diversityControl;    /* antenna setting */
    a_uint16_t   ah_antennaSwitchSwap;       /* Controls mapping of OID request */
    enum {
        IQ_CAL_INACTIVE,
        IQ_CAL_RUNNING,
        IQ_CAL_DONE
    } ah_bIQCalibration;            /* IQ calibrate state */
    a_uint32_t   ah_totalPowerMeasI[AR5416_MAX_CHAINS];
    a_uint32_t   ah_totalPowerMeasQ[AR5416_MAX_CHAINS];
    a_int32_t     ah_totalIqCorrMeas[AR5416_MAX_CHAINS];
    a_uint16_t   ah_iqCalSamples;
    a_uint32_t   ah_tx6PowerInHalfDbm;   /* power output for 6Mb tx */
    a_uint32_t   ah_staId1Defaults;  /* STA_ID1 default settings */
    a_uint32_t   ah_miscMode;        /* MISC_MODE settings */
    HAL_BOOL    ah_tpcEnabled;      /* per-packet tpc enabled */
    a_uint32_t   ah_beaconInterval;  /* XXX */
    enum {
        AUTO_32KHZ,     /* use it if 32kHz crystal present */
        USE_32KHZ,      /* do it regardless */
        DONT_USE_32KHZ,     /* don't use it regardless */
    } ah_enable32kHzClock;          /* whether to sleep at 32kHz */
    a_uint32_t   *ah_analogBank0Data;/* RF register banks */
    a_uint32_t   *ah_analogBank1Data;
    a_uint32_t   *ah_analogBank2Data;
    a_uint32_t   *ah_analogBank3Data;
    a_uint32_t   *ah_analogBank6Data;
    a_uint32_t   *ah_analogBank6TPCData;
    a_uint32_t   *ah_analogBank7Data;

    a_uint32_t	*ah_addacOwl21;		/* temporary register arrays */
    a_uint32_t   *ah_bank6Temp;
    
    a_uint32_t   ah_ofdmTxPower;
    a_int16_t     ah_txPowerIndexOffset;

    a_uint32_t       ah_slottime;        /* user-specified slot time */
    a_uint32_t       ah_acktimeout;      /* user-specified ack timeout */
    a_uint32_t       ah_ctstimeout;      /* user-specified cts timeout */
    a_uint32_t       ah_globaltxtimeout; /* user-specified global tx timeout */
    /*
     * XXX
     * 11g-specific stuff; belongs in the driver.
     */
    a_uint8_t    ah_gBeaconRate;     /* fixed rate for G beacons */
    /*
     * RF Silent handling; setup according to the EEPROM.
     */
    a_uint32_t   ah_gpioSelect;      /* GPIO pin to use */
    a_uint32_t   ah_polarity;        /* polarity to disable RF */
    a_uint32_t   ah_gpioBit;     /* after init, prev value */
    HAL_BOOL    ah_eepEnabled;      /* EEPROM bit for capability */
    /*
     * ANI & Radar support.
     */
    a_uint32_t   ah_procPhyErr;      /* Process Phy errs */
    HAL_BOOL    ah_hasHwPhyCounters;    /* Hardware has phy counters */
    a_uint32_t   ah_aniPeriod;       /* ani update list period */
    struct ar5416AniState   *ah_curani; /* cached last reference */
    struct ar5416AniState   ah_ani[64]; /* per-channel state */
    struct ar5416RadarState ah_radar[HAL_NUMRADAR_STATES];  /* Per-Channel Radar detector state */
    struct ar5416RadarQElem *ah_radarq; /* radar event queue */
    struct ar5416RadarQInfo ah_radarqInfo;  /* radar event q read/write state */
    struct ar5416ArState    ah_ar;      /* AR detector state */
    struct ar5416RadarQElem *ah_arq;    /* AR event queue */
    struct ar5416RadarQInfo ah_arqInfo; /* AR event q read/write state */

    /*
     * Ani tables that change between the 5416 and 5312.
     * These get set at attach time.
     * XXX don't belong here
     * XXX need better explanation
     */
        a_int32_t     ah_totalSizeDesired[5];
        a_int32_t     ah_coarseHigh[5];
        a_int32_t     ah_coarseLow[5];
        a_int32_t     ah_firpwr[5];

    /*
     * Transmit power state.  Note these are maintained
     * here so they can be retrieved by diagnostic tools.
     */
    /* This is used to store transmit power settings configured dynamically through 
     * the athconf power utility. If ah_dynconf is set, these values will
     * overide the eeprom settings.
     */
    a_uint16_t   ah_ratesArray[Ar5416RateSize];
    HAL_BOOL    ah_dynconf; 
     /*
     * Tx queue interrupt state.
     */
    a_uint32_t   ah_intrTxqs;

    /*
     * Extension Channel Rx Clear State
     */
    a_uint32_t   ah_cycleCount;
    a_uint32_t   ah_ctlBusy;
    a_uint32_t   ah_extBusy;

    /* HT CWM state */
    HAL_HT_CWM  ah_htcwm;
    #ifdef K2
    a_uint8_t    ah_txchainmask; /* tx chain mask */
    a_uint8_t    ah_rxchainmask; /* rx chain mask */
    #endif

    a_int32_t         ah_hwp;
    void        *ah_cal_mem;
    HAL_BOOL    ah_emu_eeprom;
    HAL_BOOL    rifs_enabled;
    a_uint32_t   rifs_reg[11];
#ifdef MAGPIE_MERLIN
    a_uint32_t   rifs_sec_cnt;
    struct ar5416IniArray ah_iniPcieSerdes;
    struct ar5416IniArray ah_iniModesAdditional;
    struct ar5416IniArray ah_iniModesRxgain;

    /* open-loop power control */
    a_uint32_t originalGain[22];
    a_int32_t   initPDADC;
    a_int32_t   PDADCdelta;
#endif
    /*
     * Support for ar5416 multiple INIs
     */
    struct ar5416IniArray ah_iniModes;
    struct ar5416IniArray ah_iniCommon;
    struct ar5416IniArray ah_iniBank0;
    struct ar5416IniArray ah_iniBB_RfGain;
    struct ar5416IniArray ah_iniBank1;
    struct ar5416IniArray ah_iniBank2;
    struct ar5416IniArray ah_iniBank3;
    struct ar5416IniArray ah_iniBank6;
    struct ar5416IniArray ah_iniBank6TPC;
    struct ar5416IniArray ah_iniBank7;
    struct ar5416IniArray ah_iniAddac;
    struct ar5416IniArray ah_iniModesTxgain;

    #ifdef K2
	/* To indicate EEPROM mapping used */
	HAL_5416_EEP_MAP ah_eep_map;
    #endif
};
#define AH5416(_ah) ((struct ath_hal_5416 *)(_ah))

#define IS_5416_EMU(ah) \
    ((AH_PRIVATE(ah)->ah_devid == AR5416_DEVID_EMU) || \
     (AH_PRIVATE(ah)->ah_devid == AR5416_DEVID_EMU_PCIE))

#define ar5416RfDetach(ah) do {             \
    if (AH5416(ah)->ah_rfHal.rfDetach != AH_NULL)   \
        AH5416(ah)->ah_rfHal.rfDetach(ah);  \
} while (0)

#define ar5416GetRfBank(ah, b) \
    AH5416(ah)->ah_rfHal.getRfBank(ah, b)

#define ar5416EepDataInFlash(_ah) \
    (!AH_PRIVATE(_ah)->ah_flags & AH_USE_EEPROM)

#ifdef MAGPIE_MERLIN
#define IS_5GHZ_FAST_CLOCK_EN(_ah, _c)  (IS_CHAN_5GHZ(_c) && \
                                         ((AH_PRIVATE(_ah))->ah_config.ath_hal_fastClockEnable) && \
                                         ((ar5416EepromGet(AH5416(_ah), EEP_MINOR_REV) <= AR5416_EEP_MINOR_VER_16) || \
                                         (ar5416EepromGet(AH5416(_ah), EEP_FSTCLK_5G))))

#define AR5416_PWR2XPERRATE_ABSTOREL(_ahpriv, pwr2x) \
        ((pwr2x) - 2 * ((_ahpriv)->ah_txPowerTableBase)) 

#define AR5416_PWR2XPERRATE_RELTOABS(_ahpriv, pwr2x) \
        ((pwr2x) +  2 * ((_ahpriv)->ah_txPowerTableBase)) 

#define AR5416_RATES_OFDM_OFFSET    0
#define AR5416_RATES_CCK_OFFSET     8
#define AR5416_RATES_HT20_OFFSET    16
#define AR5416_RATES_HT40_OFFSET    24

#endif

/*
 * WAR for bug 6773.  OS_DELAY() does a PIO READ on the PCI bus which allows
 * other cards' DMA reads to complete in the middle of our reset.
 */
#define WAR_6773(x) do {                \
        if ((++(x) % 64) == 0)          \
                OS_DELAY(1);            \
} while (0)

/*   
 *  There are two methods to fix PCI write unreliable to analog register
 *    space (0x7800- 0x7898):
 *    1. Use analog long shift register.
 *    2. Use delay.
 *    but, the new value can not be written correcly by using method 1. So
 *      we select method 2 to work around this issue. (Reported Bug:32840)
 */
#define WAR_32840(_ah, _reg) do {                      \
        if ((AR_SREV_MERLIN(_ah)) && ((_reg) >= 0x00007800 && (_reg) <= 0x00007898) )  \
            adf_os_udelay(100);                                 \
} while (0)


#ifdef MAGPIE_SINGLE_CPU_CASE
#define REG_WRITE_ARRAY(iniarray, column, regWr) do {                   \
        a_uint32_t r;                                                          \
        for (r = 0; r < ((iniarray)->ia_rows); r++) {    \
                OS_REG_WRITE(ah, INI_RA((iniarray), (r), 0), INI_RA((iniarray), r, (column)));\
                WAR_6773(regWr);                                        \
        }                                                               \
} while (0)

#else

#define REG_WRITE_ARRAY(iniarray, column, regWr) do {                   \
        a_uint32_t r;                                                          \
        for (r = 0; r < ((iniarray)->ia_rows); r++) {    \
                OS_REG_WRITE(ah, INI_RA((iniarray), (r), 0), INI_RA((iniarray), r, (column)));\
                WAR_6773(regWr);                                        \
                /* WAR_32840(ah, INI_RA((iniarray), (r), 0));*/         \
         adf_os_udelay(100);                               \
        }                                                   \
} while (0)
#endif

extern  HAL_BOOL ar2133RfAttach(struct ath_hal *, HAL_STATUS *);

struct ath_hal;

extern  a_uint32_t ar5416RadioAttach(struct ath_hal *ah);
extern  struct ath_hal_5416 * ar5416NewState(HAL_SOFTC sc,
        adf_os_device_t dev, a_uint32_t flags, HAL_STATUS *status);
extern  struct ath_hal * ar5416Attach(HAL_SOFTC sc,
        adf_os_device_t dev, a_uint32_t flags, HAL_STATUS *status);
extern  void ar5416Detach(struct ath_hal *ah);
extern  HAL_BOOL ar5416ChipTest(struct ath_hal *ah);
extern  HAL_BOOL ar5416GetChannelEdges(struct ath_hal *ah,
                a_uint32_t flags, a_uint16_t *low, a_uint16_t *high);
extern  HAL_BOOL ar5416FillCapabilityInfo(struct ath_hal *ah);

extern  void ar5416GetBssIdMask(struct ath_hal *ah, a_uint8_t *mac);
extern  HAL_BOOL ar5416SetBssIdMask(struct ath_hal *, const a_uint8_t *);
extern  void ar5416SetPCUConfig(struct ath_hal *);
extern void ar5416SetCoverageClass(struct ath_hal *ah, a_uint8_t coverageclass, a_int32_t now);

#ifndef ATH_STA_ONLY
extern  void ar5416BeaconInit(struct ath_hal *ah,
        a_uint32_t next_beacon, a_uint32_t beacon_period);
extern  HAL_BOOL ar5416WaitForBeaconDone(struct ath_hal *, HAL_BUS_ADDR baddr);
extern  HAL_BOOL ar5416SetRegulatoryDomain(struct ath_hal *ah,
        a_uint16_t regDomain, HAL_STATUS *stats);
extern  HAL_BOOL ar5416GetRfKill(struct ath_hal *ah);
extern  void ar5416EnableRfKill(struct ath_hal *);
extern  void ar5416SetLedState(struct ath_hal *ah, HAL_LED_STATE state);
extern  void ar5416SetBasicRate(struct ath_hal *ah, HAL_RATE_SET *pSet);
extern  a_uint32_t ar5416GetRandomSeed(struct ath_hal *ah);
extern  HAL_BOOL ar5416DetectCardPresent(struct ath_hal *ah);
extern  void ar5416UpdateMibCounters(struct ath_hal *ah, HAL_MIB_STATS* stats);
extern  HAL_BOOL ar5416IsJapanChannelSpreadSupported(struct ath_hal *ah);
extern  a_uint32_t ar5416GetCurRssi(struct ath_hal *ah);
extern HAL_BOOL ar5416SetDecompMask(struct ath_hal *ah, a_uint16_t keyidx, a_int32_t en);
extern  HAL_BOOL ar5416IsSleepAfterBeaconBroken(struct ath_hal *ah);
extern  a_uint32_t ar5416GetEifsDur(struct ath_hal *);
extern  HAL_BOOL ar5416SetSlotTime(struct ath_hal *, a_uint32_t);
extern  a_uint32_t ar5416GetSlotTime(struct ath_hal *);
extern  HAL_BOOL ar5416SetAckTimeout(struct ath_hal *, a_uint32_t);
extern  a_uint32_t ar5416GetAckTimeout(struct ath_hal *);
extern  HAL_BOOL ar5416SetEifsMask(struct ath_hal *, a_uint32_t);
extern  a_uint32_t ar5416GetEifsMask(struct ath_hal *);
extern  HAL_BOOL ar5416SetEifsDur(struct ath_hal *, a_uint32_t);
extern  HAL_BOOL ar5416SetCTSTimeout(struct ath_hal *, a_uint32_t);
extern  a_uint32_t ar5416GetCTSTimeout(struct ath_hal *);
extern  HAL_BOOL ar5416GetDiagState(struct ath_hal *ah, a_int32_t request,
        const void *args, a_uint32_t argsize,
        void **result, a_uint32_t *resultsize);
extern  void ar5416DmaRegDump(struct ath_hal *);
extern  a_uint32_t ar5416GetCycleCounts(struct ath_hal *, a_uint32_t *,
        a_uint32_t *, a_uint32_t *);
#else
#define ar5416BeaconInit                NULL
#define ar5416WaitForBeaconDone         NULL
#define ar5416SetRegulatoryDomain       NULL
#define ar5416GetRfKill(_ah)            0
#define ar5416EnableRfKill(_ah)
#define ar5416SetLedState               NULL
#define ar5416DetectCardPresent(_ah)    1
#define ar5416UpdateMibCounters         NULL
#define ar5416SetDecompMask             NULL
#define ar5416SetSlotTime(_ah, _us)
#define ar5416GetSlotTime               NULL
#define ar5416SetAckTimeout(_ah, _us)
#define ar5416GetAckTimeout             NULL
#define ar5416SetEifsMask               NULL
#define ar5416GetEifsMask               NULL
#define ar5416SetEifsDur                NULL
#define ar5416GetEifsDur                NULL
#define ar5416SetCTSTimeout(_ah, _us)
#define ar5416GetCTSTimeout             NULL
#define ar5416GetDiagState              NULL
#define ar5416DmaRegDump                NULL
#define ar5416GetCycleCounts            NULL
#endif /* ATH_STA_ONLY */

extern  void ar5416ResetStaBeaconTimers(struct ath_hal *ah);
extern  void ar5416SetStaBeaconTimers(struct ath_hal *ah,
        const HAL_BEACON_STATE *);

extern  HAL_BOOL ar5416IsInterruptPending(struct ath_hal *ah);
extern  HAL_BOOL ar5416GetPendingInterrupts(struct ath_hal *ah, HAL_INT *);
extern  HAL_INT ar5416GetInterrupts(struct ath_hal *ah);
extern  HAL_INT ar5416SetInterrupts(struct ath_hal *ah, HAL_INT ints);

extern  a_uint32_t ar5416GetKeyCacheSize(struct ath_hal *);
extern  HAL_BOOL ar5416IsKeyCacheEntryValid(struct ath_hal *, a_uint16_t entry);
extern  HAL_BOOL ar5416ResetKeyCacheEntry(struct ath_hal *ah, a_uint16_t entry);
extern  HAL_BOOL ar5416SetKeyCacheEntryMac(struct ath_hal *,
            a_uint16_t entry, const a_uint8_t *mac);
extern  HAL_BOOL ar5416SetKeyCacheEntry(struct ath_hal *ah, a_uint16_t entry,
                       const HAL_KEYVAL *k, const a_uint8_t *mac, a_int32_t xorKey);

extern  void ar5416GetMacAddress(struct ath_hal *ah, a_uint8_t *mac);
extern  HAL_BOOL ar5416SetMacAddress(struct ath_hal *ah, const a_uint8_t *);
extern  HAL_STATUS ar5416EepromAttach(struct ath_hal *);
extern  a_uint32_t ar5416EepromGet(struct ath_hal_5416 *ahp, EEPROM_PARAM param);
extern  HAL_STATUS ar5416EepromSetTransmitPower(struct ath_hal *ah, HAL_HT *ht,
                     ar5416_eeprom_t *pEepData, HAL_CHANNEL_INTERNAL *chan,
                     a_uint16_t cfgCtl, a_uint16_t twiceAntennaReduction,
                     a_uint16_t twiceMaxRegulatoryPower, a_uint16_t powerLimit);
extern  HAL_BOOL ar5416EepromSetBoardValues(struct ath_hal *, HAL_HT *, HAL_CHANNEL_INTERNAL *);
extern  HAL_BOOL ar5416EepromRead(struct ath_hal *, a_uint32_t off, a_uint16_t *data);
extern  HAL_BOOL ar5416FlashRead(struct ath_hal *, a_uint32_t off, a_uint16_t *data);
extern  HAL_BOOL ar5416EepromWrite(struct ath_hal *, a_uint32_t off, a_uint16_t data);
extern  HAL_BOOL ar5416FlashWrite(struct ath_hal *, a_uint32_t off, a_uint16_t data);
extern  a_uint32_t ar5416GetWirelessModes(struct ath_hal *ah);
extern  HAL_BOOL ar5416GpioCfgInput(struct ath_hal *, a_uint32_t gpio);
extern  HAL_BOOL ar5416GpioSet(struct ath_hal *, a_uint32_t gpio, a_uint32_t val);
extern  a_uint32_t ar5416GpioGet(struct ath_hal *ah, a_uint32_t gpio);
extern  void ar5416GpioSetIntr(struct ath_hal *ah, a_uint32_t, a_uint32_t ilevel);
extern  void ar5416WriteAssocid(struct ath_hal *ah, const a_uint8_t *bssid,
        a_uint16_t assocId);
#ifdef ATH_FORCE_PPM
extern  a_uint32_t ar5416PpmGetRssiDump(struct ath_hal *);
extern  a_uint32_t ar5416PpmArmTrigger(struct ath_hal *);
extern  a_int32_t ar5416PpmGetTrigger(struct ath_hal *);
extern  a_uint32_t ar5416PpmForce(struct ath_hal *);
extern  void ar5416PpmUnForce(struct ath_hal *);
extern  a_uint32_t ar5416PpmGetReg(struct ath_hal *, a_int32_t);
#endif /* ATH_FORCE_PPM */
extern  void ar5416TimerStart(struct ath_hal *ah,
        a_uint32_t timer_next, a_uint32_t timer_period);
extern  void ar5416TimerStop(struct ath_hal *ah);
extern  a_uint32_t ar5416GetTsf32(struct ath_hal *ah);
extern  a_uint64_t ar5416GetTsf64(struct ath_hal *ah);
extern  void ar5416ResetTsf(struct ath_hal *ah);
extern  a_uint8_t ar5416GetDefAntenna(struct ath_hal *ah);
extern  void ar5416SetDefAntenna(struct ath_hal *ah, a_uint32_t antenna);
extern  HAL_STATUS ar5416GetCapability(struct ath_hal *, HAL_CAPABILITY_TYPE,
        a_uint32_t, a_uint32_t *);
extern  HAL_BOOL ar5416SetCapability(struct ath_hal *, HAL_CAPABILITY_TYPE,
        a_uint32_t, a_uint32_t, HAL_STATUS *);
extern void ar5416GetDescInfo(struct ath_hal *ah, HAL_DESC_INFO *desc_info);
extern  a_uint32_t ar5416Get11nExtBusy(struct ath_hal *ah);
extern  void ar5416Set11nMac2040(struct ath_hal *ah, HAL_HT_MACMODE mode);
extern  HAL_HT_RXCLEAR ar5416Get11nRxClear(struct ath_hal *ah);
extern  void ar5416Set11nRxClear(struct ath_hal *ah, HAL_HT_RXCLEAR rxclear);
extern  HAL_BOOL ar5416SetPowerMode(struct ath_hal *ah, HAL_POWER_MODE mode,
        a_int32_t setChip);
extern  HAL_POWER_MODE ar5416GetPowerMode(struct ath_hal *ah);
extern HAL_BOOL ar5416SetPowerModeAwake(struct ath_hal *ah, a_int32_t setChip);

extern  HAL_BOOL ar5416Reset(struct ath_hal *ah, HAL_OPMODE opmode,
        HAL_CHANNEL *chan, HAL_BOOL bChannelChange, HAL_STATUS *status);
extern  HAL_BOOL ar5416SetResetReg(struct ath_hal *ah, a_uint32_t type,
                                   HAL_CHANNEL *chan);
extern  void ar5416SetOperatingMode(struct ath_hal *ah, a_int32_t opmode);
extern  HAL_BOOL ar5416PhyDisable(struct ath_hal *ah);
extern  HAL_BOOL ar5416Disable(struct ath_hal *ah);
extern  HAL_BOOL ar5416ChipReset(struct ath_hal *ah, HAL_CHANNEL *);
extern  HAL_BOOL ar5416PerCalibration(struct ath_hal *ah,  HAL_CHANNEL *chan, HAL_BOOL *isIQdone);
extern  HAL_BOOL ar5416PerCalibration11n(struct ath_hal *ah,  HAL_CHANNEL *chan,
        HAL_HT *ht, HAL_BOOL shortcal);
extern  void ar5416GetNoiseFloor(struct ath_hal *ah, a_uint16_t nfarray[]);
extern  HAL_BOOL ar5416SetTxPowerLimit(struct ath_hal *ah, a_uint32_t limit);
extern  const HAL_RATE_TABLE *ar5416GetRateTable(struct ath_hal *, a_uint32_t mode);
  

#ifdef MAGPIE_MERLIN
extern  a_int16_t ar5416GetRateTxPower(struct ath_hal *ah, a_uint32_t mode,
                                     a_uint8_t rate_index, a_uint8_t chainmask);
extern  void ar5416InitRateTxPower(struct ath_hal *ah, a_uint32_t mode, HAL_HT *ht, 
                                   a_int32_t ht40Correction, a_int16_t powerPerRate[], 
                                   a_uint8_t chainmask);
extern  void ar5416DumpRateTxPower(struct ath_hal *ah, a_uint32_t mode);
extern void ar5416MarkPhyInactive(struct ath_hal *ah);
extern  HAL_BOOL ar5416GpioCfgOutput(struct ath_hal *, a_uint32_t gpio, HAL_GPIO_OUTPUT_MUX_TYPE signalType);
#else
extern  HAL_BOOL ar5416GpioCfgOutput(struct ath_hal *, a_uint32_t gpio);
#endif

extern  void ar5416EnableMIBCounters(struct ath_hal *);
extern  void ar5416DisableMIBCounters(struct ath_hal *);
extern  void ar5416AniAttach(struct ath_hal *);
extern  void ar5416AniDetach(struct ath_hal *);
extern  struct ar5416AniState *ar5416AniGetCurrentState(struct ath_hal *);
extern  struct ar5416Stats *ar5416AniGetCurrentStats(struct ath_hal *);
extern  HAL_BOOL ar5416AniControl(struct ath_hal *, HAL_ANI_CMD cmd, a_int32_t param);
struct ath_rx_status;
extern  void ar5416AniPhyErrReport(struct ath_hal *ah,
        const struct ath_rx_status *rs);
extern  void ar5416ProcessMibIntr(struct ath_hal *, const HAL_NODE_STATS *);
extern  void ar5416AniArPoll(struct ath_hal *, const HAL_NODE_STATS *,
                 HAL_CHANNEL *);
extern  void ar5416AniReset(struct ath_hal *);

extern  void ar5416ResetAR(struct ath_hal *ah);
extern  void ar5416ArEnable(struct ath_hal *ah);
extern  HAL_STATUS ar5416RadarAttach(struct ath_hal *ah);
extern  void ar5416RadarDetach(struct ath_hal *ah);
extern  void ar5416RadarEnable(struct ath_hal *ah);
extern  void ar5416ArDisable(struct ath_hal *ah);
extern  void ar5416RadarDisable(struct ath_hal *ah);
extern  HAL_BOOL ar5416DeQueueRadarEvent(struct ar5416RadarQElem *q,
        struct ar5416RadarQInfo *qInfo, struct ar5416RadarEvent *radarEvent,
        HAL_BOOL *flush);
extern  struct ar5416RadarState *ar5416GetRadarChanState(struct ath_hal *ah,
                             a_uint8_t *index);
extern  void ar5416ResetRadar(struct ath_hal *ah);
extern  void ar5416ResetAr(struct ath_hal *ah);
extern  void ar5416ProcessArEvent(struct ath_hal *ah, HAL_CHANNEL *chan);
extern  HAL_BOOL ar5416Reset11n(struct ath_hal *ah, HAL_OPMODE opmode,
        HAL_CHANNEL *chan, HAL_HT *ht, HAL_BOOL bChannelChange, HAL_STATUS *status);
extern HAL_RFGAIN ar5416GetRfgain(struct ath_hal *ah);
extern a_int32_t ar5416Get11nHwPlatform(struct ath_hal *ah);
extern void ar5416GetChannelCenters(struct ath_hal *ah, HAL_HT *ht,
                                    HAL_CHANNEL_INTERNAL *chan,
                                    CHAN_CENTERS *centers);
extern a_uint16_t ar5416GetCtlCenter(struct ath_hal *ah, HAL_HT *ht,
                                        HAL_CHANNEL_INTERNAL *chan);
extern a_uint16_t ar5416GetExtCenter(struct ath_hal *ah, HAL_HT *ht,
                                        HAL_CHANNEL_INTERNAL *chan);
extern void ar5416CheckDfs(struct ath_hal *ah, HAL_CHANNEL *chan);
extern void ar5416DfsFound(struct ath_hal *ah, HAL_CHANNEL *chan, a_uint64_t nolTime);
extern void ar5416EnableDfs(struct ath_hal *ah, HAL_PHYERR_PARAM *pe);
extern void ar5416GetDfsThresh(struct ath_hal *ah, HAL_PHYERR_PARAM *pe);
extern HAL_BOOL ar5416RadarWait(struct ath_hal *ah, HAL_CHANNEL *chan);
extern HAL_DFS_PULSE * ar5416GetDfsRadars(struct ath_hal *ah,
                                          a_uint32_t dfsdomain,
                                          a_int32_t *numradars,
                                          HAL_DFS_BIN5PULSE **bin5pulses,
                                          a_int32_t *numb5radars,
                                          HAL_PHYERR_PARAM *pe);
                                          
extern HAL_BOOL ar5416SetRxRifs(struct ath_hal *ah, HAL_BOOL enable);
extern HAL_BOOL ar5416GetRxRifs(struct ath_hal *ah);
#ifdef K2
/* EEPROM interface functions */
/* Common Interface functions */
extern  HAL_STATUS ar5416EepromGetAntCfg(struct ath_hal_5416 *ahp, HAL_CHANNEL_INTERNAL *chan,
                                     a_uint8_t index, a_uint16_t *config);



/* EEPROM Default MAP - interface functions */
extern a_uint32_t ar5416EepromDefGet(struct ath_hal_5416 *ahp, EEPROM_PARAM param);
extern  HAL_BOOL ar5416EepromDefSetBoardValues(struct ath_hal *, HAL_HT *ht, HAL_CHANNEL_INTERNAL *);
extern  HAL_STATUS ar5416EepromDefSetTransmitPower(struct ath_hal *ah, HAL_HT *ht,
                     ar5416_eeprom_def_t *pEepData, HAL_CHANNEL_INTERNAL *chan,
                     a_uint16_t cfgCtl, a_uint16_t twiceAntennaReduction,
                     a_uint16_t twiceMaxRegulatoryPower, a_uint16_t powerLimit);
extern  HAL_STATUS ar5416EepromDefGetAntCfg(struct ath_hal_5416 *ahp, HAL_CHANNEL_INTERNAL *chan,
                                     a_uint8_t index, a_uint16_t *config);
extern HAL_STATUS ar5416CheckEepromDef(struct ath_hal *ah);
extern HAL_BOOL ar5416FillEepromDef(struct ath_hal *ah);
extern HAL_BOOL ar5416FillEmuEepromDef(struct ath_hal *ah);
extern a_uint32_t ar5416EepromDefINIFixup(struct ath_hal *ah,ar5416_eeprom_def_t *pEepData, a_uint32_t reg, a_uint32_t value);

/* EEPROM  4KBits MAP - interface functions */
extern a_uint32_t ar5416Eeprom4kGet(struct ath_hal_5416 *ahp, EEPROM_PARAM param);
extern  HAL_BOOL ar5416Eeprom4kSetBoardValues(struct ath_hal *, HAL_HT *ht, HAL_CHANNEL_INTERNAL *);
extern  HAL_STATUS ar5416Eeprom4kSetTransmitPower(struct ath_hal *ah, HAL_HT *ht,
                     ar5416_eeprom_4k_t *pEepData, HAL_CHANNEL_INTERNAL *chan,
                     a_uint16_t cfgCtl, a_uint16_t twiceAntennaReduction,
                     a_uint16_t twiceMaxRegulatoryPower, a_uint16_t powerLimit);
extern  HAL_STATUS ar5416Eeprom4kGetAntCfg(struct ath_hal_5416 *ahp, HAL_CHANNEL_INTERNAL *chan,
                                     a_uint8_t index, a_uint16_t *config);
extern HAL_STATUS ar5416CheckEeprom4k(struct ath_hal *ah);
extern HAL_BOOL ar5416FillEeprom4k(struct ath_hal *ah);
extern HAL_BOOL ar5416FillEmuEeprom4k(struct ath_hal *ah);
extern a_uint32_t ar5416Eeprom4kINIFixup(struct ath_hal *ah,ar5416_eeprom_4k_t *pEepData, a_uint32_t reg, a_uint32_t value);

/* Common EEPROM Help function */
extern void ar5416GetTargetPowers(struct ath_hal *ah, HAL_HT *ht,
                     HAL_CHANNEL_INTERNAL *chan, CAL_TARGET_POWER_HT *powInfo,
                     a_uint16_t numChannels, CAL_TARGET_POWER_HT *pNewPower,
                     a_uint16_t numRates, HAL_BOOL isHt40Target);
extern void ar5416GetTargetPowersLeg(struct ath_hal *ah, HAL_HT *ht,
                    HAL_CHANNEL_INTERNAL *chan, CAL_TARGET_POWER_LEG *powInfo,
                    a_uint16_t numChannels, CAL_TARGET_POWER_LEG *pNewPower,
                    a_uint16_t numRates, HAL_BOOL isExtTarget);
#endif

#ifdef AR5416_EMULATION
/* XXX - AR5416 Emulation only. Remove when emulation complete */
extern a_uint32_t ar5416GetEmu(struct ath_hal *ah);
extern void ar5416SetEmu(struct ath_hal *ah, a_uint32_t value);

extern void ar5416InitMacTrace(struct ath_hal *ah);
extern void ar5416StopMacTrace(struct ath_hal *ah);
#endif
/* Functions used for cal value setting */
extern void ar5416SetNominalUserNFVal( struct ath_hal *ah, int16_t val);
extern int16_t ar5416GetNominalUserNFVal( struct ath_hal *ah);
extern void ar5416SetMinUserNFVal( struct ath_hal *ah, int16_t val);
extern int16_t ar5416GetMinUserNFVal( struct ath_hal *ah);
extern void ar5416SetMaxUserNFVal( struct ath_hal *ah, int16_t val);
extern int16_t ar5416GetMaxUserNFVal( struct ath_hal *ah);
extern void ar5416SetNfDeltaVal( struct ath_hal *ah, int16_t val );
extern int16_t ar5416GetNfDeltaVal( struct ath_hal *ah );
extern void ar5416SetNfWeightVal( struct ath_hal *ah, int16_t val );
extern int16_t ar5416GetNfWeightVal( struct ath_hal *ah);
extern int16_t ar5416GetMicCCAPwr( struct ath_hal *ah);
#endif  /* _ATH_AR5416_H_ */
