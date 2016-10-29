/*-
 * Copyright (c) 2002-2005 Sam Leffler, Errno Consulting, Atheros
 * Communications, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the following conditions are met:
 * 1. The materials contained herein are unmodified and are used
 *    unmodified.
 * 2. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following NO
 *    ''WARRANTY'' disclaimer below (''Disclaimer''), without
 *    modification.
 * 3. Redistributions in binary form must reproduce at minimum a
 *    disclaimer similar to the Disclaimer below and any redistribution
 *    must be conditioned upon including a substantially similar
 *    Disclaimer requirement for further binary redistribution.
 * 4. Neither the names of the above-listed copyright holders nor the
 *    names of any contributors may be used to endorse or promote
 *    product derived from this software without specific prior written
 *    permission.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT,
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE
 * FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES.
 *
 * $Id: //depot/sw/carriersrc/branches/magpie_host_dev/split_src/host/hal/main/ah.h#13 $
 */

#ifndef _ATH_AH_H_
#define _ATH_AH_H_
/*
 * Atheros Hardware Access Layer
 *
 * Clients of the HAL call ath_hal_attach to obtain a reference to an ath_hal
 * structure for use with the device.  Hardware-related operations that
 * follow must call back into the HAL through interface, supplying the
 * reference as the first parameter.
 */
#include <ah_osdep.h>
#include <ah_desc.h>

/*
 * __ahdecl is analogous to _cdecl; it defines the calling
 * convention used within the HAL.  For most systems this
 * can just default to be empty and the compiler will (should)
 * use _cdecl.  For systems where _cdecl is not compatible this
 * must be defined.  See linux/ah_osdep.h for an example.
 */
#ifndef __ahdecl
#define __ahdecl
#endif

/*
 * Status codes that may be returned by the HAL.  Note that
 * interfaces that return a status code set it only when an
 * error occurs--i.e. you cannot check it for success.
 */
typedef enum {
    HAL_OK      = 0,    /* No error */
    HAL_ENXIO   = 1,    /* No hardware present */
    HAL_ENOMEM  = 2,    /* Memory allocation failed */
    HAL_EIO     = 3,    /* Hardware didn't respond as expected */
    HAL_EEMAGIC = 4,    /* EEPROM magic number invalid */
    HAL_EEVERSION   = 5,    /* EEPROM version invalid */
    HAL_EELOCKED    = 6,    /* EEPROM unreadable */
    HAL_EEBADSUM    = 7,    /* EEPROM checksum invalid */
    HAL_EEREAD  = 8,    /* EEPROM read problem */
    HAL_EEBADMAC    = 9,    /* EEPROM mac address invalid */
    HAL_EESIZE  = 10,   /* EEPROM size not supported */
    HAL_EEWRITE = 11,   /* Attempt to change write-locked EEPROM */
    HAL_EINVAL  = 12,   /* Invalid parameter to function */
    HAL_ENOTSUPP    = 13,   /* Hardware revision not supported */
    HAL_ESELFTEST   = 14,   /* Hardware self-test failed */
    HAL_EINPROGRESS = 15,   /* Operation incomplete */
    HAL_FULL_RESET  = 16,   /* Full reset done */
} HAL_STATUS;

typedef enum {
    AH_FALSE = 0,       /* NB: lots of code assumes false is zero */
    AH_TRUE  = 1,
} HAL_BOOL;

typedef enum {
    HAL_CAP_REG_DMN     = 0,    /* current regulatory domain */
    HAL_CAP_CIPHER      = 1,    /* hardware supports cipher */
    HAL_CAP_TKIP_MIC    = 2,    /* handle TKIP MIC in hardware */
    HAL_CAP_TKIP_SPLIT  = 3,    /* hardware TKIP uses split keys */
    HAL_CAP_PHYCOUNTERS = 4,    /* hardware PHY error counters */
    HAL_CAP_DIVERSITY   = 5,    /* hardware supports fast diversity */
    HAL_CAP_KEYCACHE_SIZE   = 6,    /* number of entries in key cache */
    HAL_CAP_NUM_TXQUEUES    = 7,    /* number of hardware xmit queues */
    HAL_CAP_VEOL        = 9,    /* hardware supports virtual EOL */
    HAL_CAP_PSPOLL      = 10,   /* hardware has working PS-Poll support */
    HAL_CAP_DIAG        = 11,   /* hardware diagnostic support */
    HAL_CAP_COMPRESSION = 12,   /* hardware supports compression */
    HAL_CAP_BURST       = 13,   /* hardware supports packet bursting */
    HAL_CAP_FASTFRAME   = 14,   /* hardware supoprts fast frames */
    HAL_CAP_TXPOW       = 15,   /* global tx power limit  */
    HAL_CAP_TPC         = 16,   /* per-packet tx power control  */
    HAL_CAP_PHYDIAG     = 17,   /* hardware phy error diagnostic */
    HAL_CAP_BSSIDMASK   = 18,   /* hardware supports bssid mask */
    HAL_CAP_MCAST_KEYSRCH   = 19,   /* hardware has multicast key search */
    HAL_CAP_TSF_ADJUST  = 20,   /* hardware has beacon tsf adjust */
    HAL_CAP_XR          = 21,   /* hardware has XR support  */
    HAL_CAP_WME_TKIPMIC = 22,   /* hardware can support TKIP MIC when WMM is turned on */
    HAL_CAP_CHAN_HALFRATE   = 23,   /* hardware can support half rate channels */
    HAL_CAP_CHAN_QUARTERRATE = 24,  /* hardware can support quarter rate channels */
    HAL_CAP_HT       	= 25,  /* hardware can support HT */
    HAL_CAP_GTT      	= 26,  /* hardware supports global transmit timeout */
    HAL_CAP_FAST_CC 	= 27, /* hardware can support fast channel change */
    HAL_CAP_NUM_MR_RETRIES = 28, /* number of multirate retries supported by HW */
    HAL_CAP_TX_CHAINMASK = 29, /* number of tx chains */
    HAL_CAP_RX_CHAINMASK = 30, /* number of rx chains */
    HAL_CAP_DFS_DMN	= 31, /* DFS domain for current cc */
    HAL_CAP_EXT_CHAN_DFS = 32, /* DFS support for extension channel */
    HAL_CAP_COMBINED_RADAR_RSSI = 33, /* Is combined RSSI for radar accurate */

    HAL_CAP_RTS_AGGR_LIMIT = 36, /* aggregation limit with RTS */
    HAL_CAP_OWL       = 37,   /* hardware is an Owl device */
    HAL_CAP_CST               = 38,   /* hardware supports carrier sense timeout interrupt */
    HAL_CAP_RIFS_RX           = 39,   /* hardware supports RIFS receive */
    HAL_CAP_RIFS_TX	          = 40,   /* hardware supports RIFS transmit */
    HAL_CAP_FORCE_PPM         = 41,   /* Force PPM */

    HAL_CAP_RFSILENT	= 42,   /* hardware has rfsilent support  */
    HAL_CAP_TPC_ACK		= 43,   /* ack txpower with per-packet tpc */
    HAL_CAP_TPC_CTS		= 44,   /* cts txpower with per-packet tpc */
    HAL_CAP_11D		= 45,   /* 11d beacon support for changing cc */
    HAL_CAP_BB_RIFS_HANG      = 46,   /* BB may hang due to RIFS */
    HAL_CAP_RIFS_RX_ENABLED   = 47,   /* RIFS RX currently enabled */
    HAL_CAP_BB_DFS_HANG       = 48,   /* BB may hang due to DFS */
    HAL_CAP_NUM_GPIO_PINS = 49, /* Number of GPIO pins */
    HAL_CAP_PCIE_PS           = 50,   /* pci express power save */
    HAL_CAP_4KB_SPLIT_TRANS   = 51,   /* hardware is capable of splitting PCIe transanctions on 4KB boundaries */
    HAL_CAP_MBSSID_AGGR_SUPPORT    = 52,   /* Support for mBSSID Aggregation */
    HAL_CAP_TKIP_WEP_HT_RATE_SUPPORT    = 53,   /* Support Tkip/WEP at HT rates */
    HAL_CAP_REG_FLAG = 54, /* Regulatory domain flags */
    HAL_CAP_DS                = 55,   /* hardware support double stream: HB93 1x2 only support single stream */
} HAL_CAPABILITY_TYPE;

/*
 * "States" for setting the LED.  These correspond to
 * the possible 802.11 operational states and there may
 * be a many-to-one mapping between these states and the
 * actual hardware states for the LED's (i.e. the hardware
 * may have fewer states).
 */
typedef enum {
    HAL_LED_INIT    = 0,
    HAL_LED_SCAN    = 1,
    HAL_LED_AUTH    = 2,
    HAL_LED_ASSOC   = 3,
    HAL_LED_RUN = 4
} HAL_LED_STATE;

/*
 * Transmit queue types/numbers.  These are used to tag
 * each transmit queue in the hardware and to identify a set
 * of transmit queues for operations such as start/stop dma.
 */
typedef enum {
    HAL_TX_QUEUE_INACTIVE   = 0,        /* queue is inactive/unused */
    HAL_TX_QUEUE_DATA   = 1,        /* data xmit q's */
    HAL_TX_QUEUE_BEACON = 2,        /* beacon xmit q */
    HAL_TX_QUEUE_CAB    = 3,        /* "crap after beacon" xmit q */
    HAL_TX_QUEUE_PSPOLL = 4,        /* power-save poll xmit q */
    HAL_TX_QUEUE_UAPSD  = 5,        /* For WMM-UAPSD xmit q */
} HAL_TX_QUEUE;









#define HAL_NUM_TX_QUEUES   10      /* max possible # of queues */

/*
 * Transmit queue subtype.  These map directly to
 * WME Access Categories (except for UPSD).  Refer
 * to Table 5 of the WME spec.
 */
typedef enum {
    HAL_WME_AC_BK   = 0,            /* background access category */
    HAL_WME_AC_BE   = 1,            /* best effort access category*/
    HAL_WME_AC_VI   = 2,            /* video access category */
    HAL_WME_AC_VO   = 3,            /* voice access category */
    HAL_WME_UPSD    = 4,            /* uplink power save */
    HAL_XR_DATA = 5,            /* uplink power save */
} HAL_TX_QUEUE_SUBTYPE;

/*
 * Transmit queue flags that control various
 * operational parameters.
 */
typedef enum {
    TXQ_FLAG_TXOKINT_ENABLE     = 0x0001,    /* enable TXOK interrupt */
    TXQ_FLAG_TXERRINT_ENABLE    = 0x0001,    /* enable TXERR interrupt */
    TXQ_FLAG_TXDESCINT_ENABLE   = 0x0002,    /* enable TXDESC interrupt */
    TXQ_FLAG_TXEOLINT_ENABLE    = 0x0004,    /* enable TXEOL interrupt */
    TXQ_FLAG_TXURNINT_ENABLE    = 0x0008,    /* enable TXURN interrupt */
    TXQ_FLAG_BACKOFF_DISABLE    = 0x0010,    /* disable Post Backoff  */
    TXQ_FLAG_COMPRESSION_ENABLE = 0x0020,    /* compression enabled */
    TXQ_FLAG_RDYTIME_EXP_POLICY_ENABLE = 0x0040, /* enable ready time
                            expiry policy */
    TXQ_FLAG_FRAG_BURST_BACKOFF_ENABLE = 0x0080, /* enable backoff while
                            sending fragment burst*/
} HAL_TX_QUEUE_FLAGS;

typedef struct {
    a_uint32_t   tqi_ver;        /* hal TXQ version */
    HAL_TX_QUEUE_SUBTYPE tqi_subtype;   /* subtype if applicable */
    HAL_TX_QUEUE_FLAGS tqi_qflags;      /* flags (see above) */
    a_uint32_t   tqi_priority;       /* (not used) */
    a_uint32_t   tqi_aifs;       /* aifs */
    a_uint32_t   tqi_cwmin;      /* cwMin */
    a_uint32_t   tqi_cwmax;      /* cwMax */
    a_uint16_t   tqi_shretry;        /* rts retry limit */
    a_uint16_t   tqi_lgretry;        /* long retry limit (not used)*/
    a_uint32_t   tqi_cbrPeriod;
    a_uint32_t   tqi_cbrOverflowLimit;
    a_uint32_t   tqi_burstTime;
    a_uint32_t   tqi_readyTime;
    a_uint32_t   tqi_compBuf;    /* compression buffer phys addr */
} HAL_TXQ_INFO;

/* token to use for aifs, cwmin, cwmax */
#define HAL_TXQ_USEDEFAULT  ((a_uint32_t) -1)

/* compression definitions */
#define HAL_COMP_BUF_MAX_SIZE           9216            /* 9K */
#define HAL_COMP_BUF_ALIGN_SIZE         512
#define HAL_DECOMP_MASK_SIZE        128


/*
 * Transmit packet types.  This belongs in ah_desc.h, but
 * is here so we can give a proper type to various parameters
 * (and not require everyone include the file).
 *
 * NB: These values are intentionally assigned for
 *     direct use when setting up h/w descriptors.
 */
typedef enum {
    HAL_PKT_TYPE_NORMAL = 0,
    HAL_PKT_TYPE_ATIM   = 1,
    HAL_PKT_TYPE_PSPOLL = 2,
    HAL_PKT_TYPE_BEACON = 3,
    HAL_PKT_TYPE_PROBE_RESP = 4,
    HAL_PKT_TYPE_CHIRP  = 5,
    HAL_PKT_TYPE_GRP_POLL = 6,
} HAL_PKT_TYPE;

/* Rx Filter Frame Types */
typedef enum {
    HAL_RX_FILTER_UCAST = 0x00000001,   /* Allow unicast frames */
    HAL_RX_FILTER_MCAST = 0x00000002,   /* Allow multicast frames */
    HAL_RX_FILTER_BCAST = 0x00000004,   /* Allow broadcast frames */
    HAL_RX_FILTER_CONTROL   = 0x00000008,   /* Allow control frames */
    HAL_RX_FILTER_BEACON    = 0x00000010,   /* Allow beacon frames */
    HAL_RX_FILTER_PROM  = 0x00000020,   /* Promiscuous mode */
    HAL_RX_FILTER_XRPOLL    = 0x00000040,   /* Allow XR poll frmae */
    HAL_RX_FILTER_PROBEREQ  = 0x00000080,   /* Allow probe request frames */
    HAL_RX_FILTER_PHYERR    = 0x00000100,   /* Allow phy errors */

#ifdef MAGPIE_MERLIN
    HAL_RX_FILTER_PHYRADAR =  0x00002000, /* Allow phy radar errors*/ 
    HAL_RX_FILTER_PSPOLL    = 0x00004000,   /* Allow PSPOLL frames */
    /* 
     ** PHY "Pseudo bits" should be in the upper 16 bits since the lower 
     ** 16 bits actually correspond to register 0x803c bits 
     */ 
#else
    HAL_RX_FILTER_PHYRADAR  = 0x00000200,   /* Allow phy radar errors*/
#endif
} HAL_RX_FILTER;

typedef enum {
    HAL_PM_UNDEFINED    = 0,
    HAL_PM_AWAKE        = 1,
    HAL_PM_FULL_SLEEP   = 2,
    HAL_PM_NETWORK_SLEEP    = 3
} HAL_POWER_MODE;



/*
 * NOTE WELL:
 * These are mapped to take advantage of the common locations for many of
 * the bits on all of the currently supported MAC chips. This is to make
 * the ISR as efficient as possible, while still abstracting HW differences.
 * When new hardware breaks this commonality this enumerated type, as well
 * as the HAL functions using it, must be modified. All values are directly
 * mapped unless commented otherwise.
 */
typedef enum {
    HAL_INT_RX  = 0x00000001,   /* Non-common mapping */
    HAL_INT_RXDESC  = 0x00000002,
    HAL_INT_RXNOFRM = 0x00000008,
    HAL_INT_RXEOL   = 0x00000010,
    HAL_INT_RXORN   = 0x00000020,
    HAL_INT_TX  = 0x00000040,   /* Non-common mapping */
    HAL_INT_TXDESC  = 0x00000080,
    HAL_INT_TXURN   = 0x00000800,
    HAL_INT_MIB = 0x00001000,
    HAL_INT_RXPHY   = 0x00004000,
    HAL_INT_RXKCM   = 0x00008000,
    HAL_INT_SWBA    = 0x00010000,
    HAL_INT_BMISS   = 0x00040000,
    HAL_INT_BNR     = 0x00100000,   /* Non-common mapping */
    HAL_INT_GPIO    = 0x01000000,
    HAL_INT_CST     = 0x02000000,   /* Non-common mapping */
    HAL_INT_GTT     = 0x20000000,   /* Non-common mapping */
    HAL_INT_FATAL   = 0x40000000,   /* Non-common mapping */
    HAL_INT_GLOBAL  = 0x80000000,   /* Set/clear IER */
    HAL_INT_GENTIMER =0x10000000,   /* Non-common mapping */ 

    /* Interrupt bits that map directly to ISR/IMR bits */
    HAL_INT_COMMON  = HAL_INT_RXNOFRM
            | HAL_INT_RXDESC
            | HAL_INT_RXEOL
            | HAL_INT_RXORN
            | HAL_INT_TXURN
            | HAL_INT_TXDESC
            | HAL_INT_MIB
            | HAL_INT_RXPHY
            | HAL_INT_RXKCM
            | HAL_INT_SWBA
            | HAL_INT_BMISS
            | HAL_INT_GPIO,
    HAL_INT_NOCARD  = 0xffffffff    /* To signal the card was removed */
} HAL_INT;



typedef enum {
    HAL_RFGAIN_INACTIVE     = 0,
    HAL_RFGAIN_READ_REQUESTED   = 1,
    HAL_RFGAIN_NEED_CHANGE      = 2
} HAL_RFGAIN;

/*
 * Channels are specified by frequency.
 */
typedef struct {
    a_uint16_t   channel;    /* setting in Mhz */
    a_uint32_t   channelFlags;   /* see below */
    a_uint8_t    privFlags;
    a_int8_t      maxRegTxPower;  /* max regulatory tx power in dBm */
    a_int8_t      maxTxPower; /* max true tx power in 0.25 dBm */
    a_int8_t      minTxPower; /* min true tx power in 0.25 dBm */
    a_uint8_t    regClassId; /* regulatory class id of this channel */
} HAL_CHANNEL;

/* channelFlags */
#define CHANNEL_CW_INT  0x0002  /* CW interference detected on channel */
#define CHANNEL_TURBO   0x0010  /* Turbo Channel */
#define CHANNEL_CCK 0x0020  /* CCK channel */
#define CHANNEL_OFDM    0x0040  /* OFDM channel */
#define CHANNEL_2GHZ    0x0080  /* 2 GHz spectrum channel. */
#define CHANNEL_5GHZ    0x0100  /* 5 GHz spectrum channel */
#define CHANNEL_PASSIVE 0x0200  /* Only passive scan allowed in the channel */
#define CHANNEL_DYN 0x0400  /* dynamic CCK-OFDM channel */
#define CHANNEL_XR  0x0800  /* XR channel */
#define CHANNEL_STURBO  0x2000  /* Static turbo, no 11a-only usage */
#define CHANNEL_HALF    0x4000  /* Half rate channel */
#define CHANNEL_QUARTER 0x8000  /* Quarter rate channel */
#define CHANNEL_HT20    0x10000 /* HT20 channel */
#define CHANNEL_HT40    0x20000 /* HT40 channel */
#define CHANNEL_HT40U 	0x40000 /* control channel can be upper channel */
#define CHANNEL_HT40L 	0x80000 /* control channel can be lower channel */

/* privFlags */
#define CHANNEL_INTERFERENCE    0x01 /* Software use: channel interference
                        used for as AR as well as RADAR
                        interference detection */
#define CHANNEL_DFS     0x02 /* DFS required on channel */
#define CHANNEL_4MS_LIMIT	0x04 /* 4msec packet limit on this channel */
#define CHANNEL_DFS_CLEAR   0x08 /* if channel has been checked for DFS */

#define CHANNEL_A   (CHANNEL_5GHZ|CHANNEL_OFDM)
#define CHANNEL_B   (CHANNEL_2GHZ|CHANNEL_CCK)
#define CHANNEL_PUREG   (CHANNEL_2GHZ|CHANNEL_OFDM)
#ifdef notdef
#define CHANNEL_G   (CHANNEL_2GHZ|CHANNEL_DYN)
#else
#define CHANNEL_G   (CHANNEL_2GHZ|CHANNEL_OFDM)
#endif
#define CHANNEL_T   (CHANNEL_5GHZ|CHANNEL_OFDM|CHANNEL_TURBO)
#define CHANNEL_ST  (CHANNEL_T|CHANNEL_STURBO)
#define CHANNEL_108G    (CHANNEL_2GHZ|CHANNEL_OFDM|CHANNEL_TURBO)
#define CHANNEL_108A    CHANNEL_T
#define CHANNEL_X   (CHANNEL_5GHZ|CHANNEL_OFDM|CHANNEL_XR)

#define CHANNEL_G_HT20  (CHANNEL_2GHZ|CHANNEL_HT20)
#define CHANNEL_A_HT20  (CHANNEL_5GHZ|CHANNEL_HT20)
#define CHANNEL_G_HT40  (CHANNEL_2GHZ|CHANNEL_HT20|CHANNEL_HT40)
#define CHANNEL_A_HT40  (CHANNEL_5GHZ|CHANNEL_HT20|CHANNEL_HT40)
#define CHANNEL_ALL \
    (CHANNEL_OFDM|CHANNEL_CCK| CHANNEL_2GHZ | CHANNEL_5GHZ | CHANNEL_TURBO | CHANNEL_HT20 | CHANNEL_HT40)
#define CHANNEL_ALL_NOTURBO     (CHANNEL_ALL &~ CHANNEL_TURBO)

#define HAL_ANTENNA_MIN_MODE  0
#define HAL_ANTENNA_FIXED_A   1
#define HAL_ANTENNA_FIXED_B   2
#define HAL_ANTENNA_MAX_MODE  3

typedef struct {
    a_uint32_t   ackrcv_bad;
    a_uint32_t   rts_bad;
    a_uint32_t   rts_good;
    a_uint32_t   fcs_bad;
    a_uint32_t   beacons;
} HAL_MIB_STATS;

typedef a_uint16_t HAL_CTRY_CODE;        /* country code */
typedef a_uint16_t HAL_REG_DOMAIN;       /* regulatory domain code */

enum {
    CTRY_DEBUG  = 0x1ff,        /* debug country code */
    CTRY_DEFAULT    = 0         /* default country code */
};

/*
** Added in support of bug 27002, Japan midband
** support
*/

typedef enum {
        REG_EXT_JAPAN_MIDBAND          = 1,
        REG_EXT_FCC_DFS_HT40           = 2,
        REG_EXT_JAPAN_NONDFS_HT40      = 3,
        REG_EXT_JAPAN_DFS_HT40         = 4
} REG_EXT_BITMAP;       

enum {
    HAL_MODE_11A    = 0x001,        /* 11a channels */
    HAL_MODE_TURBO  = 0x002,        /* 11a turbo-only channels */
    HAL_MODE_11B    = 0x004,        /* 11b channels */
    HAL_MODE_PUREG  = 0x008,        /* 11g channels (OFDM only) */
#ifdef notdef
    HAL_MODE_11G    = 0x010,        /* 11g channels (OFDM/CCK) */
#else
    HAL_MODE_11G    = 0x008,        /* XXX historical */
#endif
    HAL_MODE_108G   = 0x020,        /* 11a+Turbo channels */
    HAL_MODE_108A   = 0x040,        /* 11g+Turbo channels */
    HAL_MODE_XR     = 0x100,        /* XR channels */
    HAL_MODE_11A_HALF_RATE = 0x200,     /* 11A half rate channels */
    HAL_MODE_11A_QUARTER_RATE = 0x400,  /* 11A quarter rate channels */
    HAL_MODE_11NG   = 0x4000,           /* 11ng channels */
    HAL_MODE_11NA   = 0x8000,           /* 11na channels */
    HAL_MODE_ALL    = 0xffff
};

/* 11n */
typedef enum {
    HAL_HT_MACMODE_20   = 0,        /* 20 MHz operation */
    HAL_HT_MACMODE_2040     = 1,        /* 20/40 MHz operation */
} HAL_HT_MACMODE;

typedef enum {
    HAL_HT_PHYMODE_20   = 0,        /* 20 MHz operation */
    HAL_HT_PHYMODE_2040     = 1,        /* 20/40 MHz operation */
} HAL_HT_PHYMODE;

typedef enum {
    HAL_HT_EXTPROTSPACING_20 = 0,       /* 20 MHz spacing */
    HAL_HT_EXTPROTSPACING_25 = 1,       /* 25 MHz spacing */
} HAL_HT_EXTPROTSPACING;

typedef struct {
    HAL_HT_MACMODE      ht_macmode;     /* MAC - 20/40 mode */
    HAL_HT_PHYMODE      ht_phymode;     /* PHY - 20/40 mode */
    a_int8_t              ht_extoff;      /* ext channel offset */
    HAL_HT_EXTPROTSPACING   ht_extprotspacing;  /* ext channel protection spacing */
} HAL_HT_CWM;

typedef struct {
    a_uint8_t            ht_txchainmask; /* tx chain mask    */
    a_uint8_t            ht_rxchainmask; /* rx chain mask    */
} HAL_HT_MISC;

typedef struct {
    HAL_HT_CWM  cwm;
    HAL_HT_MISC misc;
} HAL_HT;

typedef enum {
    HAL_RX_CLEAR_CTL_LOW    = 0x1,    /* force control channel to appear busy */
    HAL_RX_CLEAR_EXT_LOW    = 0x2,    /* force extension channel to appear busy */
} HAL_HT_RXCLEAR;

typedef struct {
    a_int32_t     rateCount;      /* NB: for proper padding */
    a_uint8_t    rateCodeToIndex[32];    /* back mapping */
    struct {
        a_uint8_t    valid;      /* valid for rate control use */
        a_uint8_t    phy;        /* CCK/OFDM/XR */
#ifdef MAGPIE_MERLIN
        a_int16_t     txPower;    /* Final power per rate in 1/2 dBm units */
        a_int16_t     txPower2Chains;    /* Final power per rate in 1/2 dBm units for 2 Chain operation */
        a_int16_t     txPower3Chains;    /* Final power per rate in 1/2 dBm units for 3 Chain operation*/
#endif
        a_uint32_t   rateKbps;   /* transfer rate in kbs */
        a_uint8_t    rateCode;   /* rate for h/w descriptors */
        a_uint8_t    shortPreamble;  /* mask for enabling short
                         * preamble in CCK rate code */
        a_uint8_t    dot11Rate;  /* value for supported rates
                         * info element of MLME */
        a_uint8_t    controlRate;    /* index of next lower basic
                         * rate; used for dur. calcs */
        a_uint16_t   lpAckDuration;  /* long preamble ACK duration */
        a_uint16_t   spAckDuration;  /* short preamble ACK duration*/
    } info[32];
} HAL_RATE_TABLE;

typedef struct {
    a_uint32_t       rs_count;       /* number of valid entries */
    a_uint8_t    rs_rates[32];       /* rates */
} HAL_RATE_SET;

#define HAL_RATESERIES_RTS_CTS    0x0001  /* use rts/cts w/this series */
#define HAL_RATESERIES_2040       0x0002  /* use ext channel for series */
#define HAL_RATESERIES_HALFGI     0x0004  /* use half-gi for series */
typedef struct {
    a_uint32_t   Tries;
    a_uint32_t   Rate;
    a_uint32_t   PktDuration;
    a_uint32_t   ChSel;
    a_uint32_t   RateFlags;
#ifdef MAGPIE_MERLIN
    a_uint32_t   RateIndex;
    a_uint32_t   TxPowerCap;     /* in 1/2 dBm units */
#endif
} HAL_11N_RATE_SERIES;

enum {
    HAL_MAC_TO_MAC_EMU = 1,
    HAL_MAC_BB_EMU,
    HAL_TRUE_CHIP,
};

typedef enum {
    HAL_ANT_VARIABLE = 0,           /* variable by programming */
    HAL_ANT_FIXED_A  = 1,           /* fixed to 11a frequencies */
    HAL_ANT_FIXED_B  = 2,           /* fixed to 11b frequencies */
} HAL_ANT_SETTING;

typedef enum {
    HAL_M_STA   = 1,            /* infrastructure station */
    HAL_M_IBSS  = 0,            /* IBSS (adhoc) station */
    HAL_M_HOSTAP    = 6,            /* Software Access Point */
    HAL_M_MONITOR   = 8         /* Monitor mode */
} HAL_OPMODE;

typedef struct {
    a_uint8_t    kv_type;        /* one of HAL_CIPHER */
    a_uint8_t    kv_pad;
    a_uint16_t   kv_len;         /* length in bits */
    a_uint8_t    kv_val[16];     /* enough for 128-bit keys */
    a_uint8_t    kv_mic[8];      /* TKIP MIC key */
    a_uint8_t    kv_txmic[8];
} HAL_KEYVAL;

typedef enum {
    HAL_KEY_TYPE_CLEAR,
    HAL_KEY_TYPE_WEP,
    HAL_KEY_TYPE_AES,
    HAL_KEY_TYPE_TKIP,
} HAL_KEY_TYPE;

typedef enum {
    HAL_CIPHER_WEP      = 0,
    HAL_CIPHER_AES_OCB  = 1,
    HAL_CIPHER_AES_CCM  = 2,
    HAL_CIPHER_CKIP     = 3,
    HAL_CIPHER_TKIP     = 4,
    HAL_CIPHER_CLR      = 5,        /* no encryption */

    HAL_CIPHER_MIC      = 127       /* TKIP-MIC, not a cipher */
} HAL_CIPHER;

enum {
    HAL_SLOT_TIME_6	 = 6,
    HAL_SLOT_TIME_9  = 9,
    HAL_SLOT_TIME_20 = 20,
};

typedef enum {
    HAL_FREQ_BAND_5GHZ          = 0,
    HAL_FREQ_BAND_2GHZ          = 1,
} HAL_FREQ_BAND;
/*
 * Per-station beacon timer state.  Note that the specified
 * beacon interval (given in TU's) can also include flags
 * to force a TSF reset and to enable the beacon xmit logic.
 * If bs_cfpmaxduration is non-zero the hardware is setup to
 * coexist with a PCF-capable AP.
 */
typedef struct {
    a_uint32_t   bs_nexttbtt;        /* next beacon in TU */
    a_uint32_t   bs_nextdtim;        /* next DTIM in TU */
    a_uint32_t   bs_intval;      /* beacon interval+flags */
#define HAL_BEACON_PERIOD   0x0000ffff  /* beacon interval period */
#define HAL_BEACON_ENA      0x00800000  /* beacon xmit enable */
#define HAL_BEACON_RESET_TSF    0x01000000  /* clear TSF */
    a_uint32_t   bs_dtimperiod;
    a_uint16_t   bs_cfpperiod;       /* CFP period in TU */
    a_uint16_t   bs_cfpmaxduration;  /* max CFP duration in TU */
    a_uint32_t   bs_cfpnext;     /* next CFP in TU */
    a_uint16_t   bs_timoffset;       /* byte offset to TIM bitmap */
    a_uint16_t   bs_bmissthreshold;  /* beacon miss threshold */
    a_uint32_t   bs_sleepduration;   /* max sleep duration */
} HAL_BEACON_STATE;

/*
 * Per-node statistics maintained by the driver for use in
 * optimizing signal quality and other operational aspects.
 */
typedef struct {
    a_uint32_t   ns_avgbrssi;    /* average beacon rssi */
    a_uint32_t   ns_avgrssi; /* average data rssi */
    a_uint32_t   ns_avgtxrssi;   /* average tx rssi */
} HAL_NODE_STATS;

typedef struct {
    a_uint8_t txctl_offset;
    a_uint8_t txctl_numwords;
    a_uint8_t txstatus_offset;
    a_uint8_t txstatus_numwords;

    a_uint8_t rxctl_offset;
    a_uint8_t rxctl_numwords;
    a_uint8_t rxstatus_offset;
    a_uint8_t rxstatus_numwords;

    a_uint8_t macRevision;
} HAL_DESC_INFO;

#define HAL_RSSI_EP_MULTIPLIER  (1<<7)  /* pow2 to optimize out * and / */
#define HAL_RATE_EP_MULTIPLIER  (1<<7)  /* pow2 to optimize out * and / */

/*
 * GPIO Output mux can select from a number of different signals as input.
 * The current implementation uses 5 of these input signals:
 *     - An output value specified by the caller;
 *     - The Attention LED signal provided by the PCIE chip;
 *     - The Power     LED signal provided by the PCIE chip;
 *     - The Network LED pin controlled by the chip's MAC;
 *     - The Power   LED pin controlled by the chip's MAC.
 */
typedef enum {
    HAL_GPIO_OUTPUT_MUX_AS_OUTPUT,
    HAL_GPIO_OUTPUT_MUX_AS_PCIE_ATTENTION_LED,
    HAL_GPIO_OUTPUT_MUX_AS_PCIE_POWER_LED,
    HAL_GPIO_OUTPUT_MUX_AS_MAC_NETWORK_LED,
    HAL_GPIO_OUTPUT_MUX_AS_MAC_POWER_LED,

    HAL_GPIO_OUTPUT_MUX_NUM_ENTRIES    // always keep this type last; it must map to the number of entries in this enumeration
} HAL_GPIO_OUTPUT_MUX_TYPE;

typedef struct {
    a_int32_t     pe_firpwr;	/* FIR pwr out threshold */
    a_int32_t     pe_rrssi;	/* Radar rssi thresh */
    a_int32_t     pe_height;	/* Pulse height thresh */
    a_int32_t     pe_prssi;	/* Pulse rssi thresh */
    a_int32_t     pe_inband;	/* Inband thresh */
    /* The following params are only for AR5413 and later */
    a_uint32_t   pe_relpwr;	/* Relative power threshold in 0.5dB steps */
    a_uint32_t   pe_relstep;	/* Pulse Relative step threshold in 0.5dB steps */
    a_uint32_t   pe_maxlen;	/* Max length of radar sign in 0.8us units */
    HAL_BOOL    pe_usefir128;	/* Use the average in-band power measured over 128 cycles */
    HAL_BOOL    pe_blockradar;	/* Enable to block radar check if pkt detect is done via OFDM
                                 * weak signal detect or pkt is detected immediately after tx
                                 * to rx transition */
    HAL_BOOL    pe_enmaxrssi;	/* Enable to use the max rssi instead of the last rssi during
                                 *fine gain changes for radar detection */
} HAL_PHYERR_PARAM;

#define	HAL_PHYERR_PARAM_NOVAL	65535
#define	HAL_PHYERR_PARAM_ENABLE	0x8000	/* Enable/Disable if applicable */

struct ath_desc;
struct ath_rx_status;

typedef struct {
	a_uint32_t	rp_numpulses;	/* Num of pulses in radar burst */
	a_uint32_t	rp_pulsedur;	/* Duration of each pulse in usecs */
	a_uint32_t	rp_pulsefreq;	/* Frequency of pulses in burst */
	a_uint32_t	rp_pulsevar;	/* Time variation of pulse duration for
					   matched filter (single-sided) in usecs */
	a_uint32_t	rp_threshold;	/* Threshold for MF output to indicate
					   radar match */
	a_uint32_t	rp_mindur;	/* Min pulse duration to be considered for
					   this pulse type */
	a_uint32_t	rp_maxdur;	/* Max pusle duration to be considered for
					   this pulse type */
	a_uint32_t	rp_rssithresh;	/* Minimum rssi to be considered a radar pulse */
	a_uint32_t	rp_meanoffset;	/* Offset for timing adjustment */
	a_uint32_t	rp_rssimargin;  /* rssi threshold margin. In Turbo Mode HW reports rssi 3dBm */
					/* lower than in non TURBO mode. This will be used to offset that diff.*/
	a_uint32_t	rp_pulseid;	/* Unique ID for identifying filter */

} HAL_DFS_PULSE;

typedef struct {
	a_uint32_t	b5_threshold;		/* Number of bin5 pulses to indicate detection */
	a_uint32_t	b5_mindur;		/* Min duration for a bin5 pulse */
	a_uint32_t	b5_maxdur;		/* Max duration for a bin5 pulse */
	a_uint32_t	b5_timewindow;		/* Window over which to count bin5 pulses */
	a_uint32_t	b5_rssithresh;		/* Min rssi to be considered a pulse */
	a_uint32_t	b5_rssimargin;		/* rssi threshold margin. In Turbo Mode HW reports rssi 3dB */
} HAL_DFS_BIN5PULSE;

/*
 * Hardware Access Layer (HAL) API.
 *
 * Clients of the HAL call ath_hal_attach to obtain a reference to an
 * ath_hal structure for use with the device.  Hardware-related operations
 * that follow must call back into the HAL through interface, supplying
 * the reference as the first parameter.  Note that before using the
 * reference returned by ath_hal_attach the caller should verify the
 * ABI version number.
 */ 

struct ath_hal {
    a_uint32_t  *hal_wmi_handle;
    a_uint32_t   ah_magic;   /* consistency check magic number */
    a_uint32_t   ah_abi;     /* HAL ABI version */
#define HAL_ABI_VERSION 0x05062000  /* YYMMDDnn */
    a_uint16_t   ah_devid;   /* PCI device ID */
    a_uint16_t   ah_subvendorid; /* PCI subvendor ID */
    HAL_SOFTC   ah_sc;      /* back pointer to driver/os state */
    adf_os_device_t ah_dev;
    HAL_BUS_HANDLE  ah_sh;
    HAL_CTRY_CODE   ah_countryCode;

    a_uint32_t   ah_macVersion;  /* MAC version id */
    a_uint16_t   ah_macRev;  /* MAC revision */
    a_uint16_t   ah_phyRev;  /* PHY revision */
    /* NB: when only one radio is present the rev is in 5Ghz */
    a_uint16_t   ah_analog5GhzRev;/* 5GHz radio revision */
    a_uint16_t   ah_analog2GhzRev;/* 2GHz radio revision */
    a_uint8_t    ah_decompMask[HAL_DECOMP_MASK_SIZE]; /* decomp mask array */
    a_uint8_t    ah_nfCalHistEnable;
    a_uint16_t   ah_nfCalHistSize;	
      
    const HAL_RATE_TABLE *__ahdecl(*ah_getRateTable)(struct ath_hal *, a_uint32_t mode);
    void      __ahdecl(*ah_detach)(struct ath_hal*);

    /* Reset functions */
    HAL_BOOL  __ahdecl(*ah_reset)(struct ath_hal *, HAL_OPMODE,
                HAL_CHANNEL *, HAL_BOOL bChannelChange,
                HAL_STATUS *status);
    HAL_BOOL  __ahdecl(*ah_phyDisable)(struct ath_hal *);
    void      __ahdecl(*ah_setPCUConfig)(struct ath_hal *);
    HAL_BOOL  __ahdecl(*ah_perCalibration)(struct ath_hal*, HAL_CHANNEL *, HAL_BOOL *);
    HAL_BOOL  __ahdecl(*ah_setTxPowerLimit)(struct ath_hal *, a_uint32_t);

    void      __ahdecl(*ah_arEnable)(struct ath_hal *);
    void      __ahdecl(*ah_arDisable)(struct ath_hal *);
    void      __ahdecl(*ah_arReset)(struct ath_hal *);

    /* DFS functions */
    void      __ahdecl(*ah_checkDfs)(struct ath_hal*, HAL_CHANNEL *);
    void      __ahdecl(*ah_dfsFound)(struct ath_hal*, HAL_CHANNEL *, a_uint64_t);
    void      __ahdecl(*ah_enableDfs)(struct ath_hal*, HAL_PHYERR_PARAM *);
    void      __ahdecl(*ah_getDfsThresh)(struct ath_hal*, HAL_PHYERR_PARAM *);
    HAL_BOOL  __ahdecl(*ah_radarWait)(struct ath_hal *, HAL_CHANNEL *);
    HAL_DFS_PULSE* __ahdecl(*ah_getDfsRadar)(struct ath_hal *, a_uint32_t, a_int32_t *,
                    HAL_DFS_BIN5PULSE **, a_int32_t *, HAL_PHYERR_PARAM *);
	

    /* Transmit functions */
    a_int32_t   __ahdecl(*ah_setupTxQueue)(struct ath_hal *, HAL_TX_QUEUE,
                const HAL_TXQ_INFO *qInfo);
    HAL_BOOL  __ahdecl(*ah_setTxQueueProps)(struct ath_hal *, a_int32_t q,
                const HAL_TXQ_INFO *qInfo);
    HAL_BOOL  __ahdecl(*ah_getTxQueueProps)(struct ath_hal *, a_int32_t q,
                HAL_TXQ_INFO *qInfo);
    HAL_BOOL  __ahdecl(*ah_releaseTxQueue)(struct ath_hal *ah, a_uint32_t q);
    HAL_BOOL  __ahdecl(*ah_resetTxQueue)(struct ath_hal *ah, a_uint32_t q);
    a_uint32_t __ahdecl(*ah_numTxPending)(struct ath_hal *, a_uint32_t q);
    HAL_BOOL  __ahdecl(*ah_abortTxDma)(struct ath_hal *);

    HAL_BOOL   __ahdecl(*ah_setGlobalTxTimeout)(struct ath_hal*, a_uint32_t);
    a_uint32_t      __ahdecl(*ah_getGlobalTxTimeout)(struct ath_hal*);
 
    /* Receive Functions */
    void      __ahdecl(*ah_enableReceive)(struct ath_hal*);
    HAL_BOOL  __ahdecl(*ah_stopDmaReceive)(struct ath_hal*);
    void      __ahdecl(*ah_startPcuReceive)(struct ath_hal*);
    void      __ahdecl(*ah_stopPcuReceive)(struct ath_hal*);
    void      __ahdecl(*ah_abortPcuReceive)(struct ath_hal*);
    void      __ahdecl(*ah_setMulticastFilter)(struct ath_hal*,
                a_uint32_t filter0, a_uint32_t filter1);
    HAL_BOOL  __ahdecl(*ah_setMulticastFilterIndex)(struct ath_hal*,
                a_uint32_t index);
    HAL_BOOL  __ahdecl(*ah_clrMulticastFilterIndex)(struct ath_hal*,
                a_uint32_t index);
    a_uint32_t __ahdecl(*ah_getRxFilter)(struct ath_hal*);
    void      __ahdecl(*ah_setRxFilter)(struct ath_hal*, a_uint32_t);
    void      __ahdecl(*ah_rxMonitor)(struct ath_hal *,
                const HAL_NODE_STATS *, HAL_CHANNEL *);
    void      __ahdecl(*ah_procMibEvent)(struct ath_hal *,
                const HAL_NODE_STATS *);

    /* Misc Functions */
    HAL_STATUS __ahdecl(*ah_getCapability)(struct ath_hal *,
                HAL_CAPABILITY_TYPE, a_uint32_t capability,
                a_uint32_t *result);
    HAL_BOOL   __ahdecl(*ah_setCapability)(struct ath_hal *,
                HAL_CAPABILITY_TYPE, a_uint32_t capability,
                a_uint32_t setting, HAL_STATUS *);
    HAL_BOOL   __ahdecl(*ah_getDiagState)(struct ath_hal *, a_int32_t request,
                const void *args, a_uint32_t argsize,
                void **result, a_uint32_t *resultsize);
    void      __ahdecl(*ah_getMacAddress)(struct ath_hal *, a_uint8_t *);
    HAL_BOOL  __ahdecl(*ah_setMacAddress)(struct ath_hal *, const a_uint8_t*);
    void      __ahdecl(*ah_getBssIdMask)(struct ath_hal *, a_uint8_t *);
    HAL_BOOL  __ahdecl(*ah_setBssIdMask)(struct ath_hal *, const a_uint8_t*);
    HAL_BOOL  __ahdecl(*ah_setRegulatoryDomain)(struct ath_hal*,
                a_uint16_t, HAL_STATUS *);
    void      __ahdecl(*ah_setLedState)(struct ath_hal*, HAL_LED_STATE);
    void      __ahdecl(*ah_writeAssocid)(struct ath_hal*,
                const a_uint8_t *bssid, a_uint16_t assocId);
    HAL_BOOL  __ahdecl(*ah_gpioCfgOutput)(struct ath_hal *, a_uint32_t gpio);
    HAL_BOOL  __ahdecl(*ah_gpioCfgInput)(struct ath_hal *, a_uint32_t gpio);
    a_uint32_t __ahdecl(*ah_gpioGet)(struct ath_hal *, a_uint32_t gpio);
    HAL_BOOL  __ahdecl(*ah_gpioSet)(struct ath_hal *,
                a_uint32_t gpio, a_uint32_t val);
    void      __ahdecl(*ah_gpioSetIntr)(struct ath_hal*, a_uint32_t, a_uint32_t);
    a_uint32_t __ahdecl(*ah_getTsf32)(struct ath_hal*);
    a_uint64_t __ahdecl(*ah_getTsf64)(struct ath_hal*);
    void      __ahdecl(*ah_resetTsf)(struct ath_hal*);
    HAL_BOOL  __ahdecl(*ah_detectCardPresent)(struct ath_hal*);
    void      __ahdecl(*ah_updateMibCounters)(struct ath_hal*,
                HAL_MIB_STATS*);
    HAL_RFGAIN __ahdecl(*ah_getRfGain)(struct ath_hal*);
    a_uint8_t     __ahdecl(*ah_getDefAntenna)(struct ath_hal*);
    void      __ahdecl(*ah_setDefAntenna)(struct ath_hal*, a_uint32_t);
    HAL_BOOL  __ahdecl(*ah_setSlotTime)(struct ath_hal*, a_uint32_t);
    a_uint32_t     __ahdecl(*ah_getSlotTime)(struct ath_hal*);
    HAL_BOOL  __ahdecl(*ah_setAckTimeout)(struct ath_hal*, a_uint32_t);
    a_uint32_t     __ahdecl(*ah_getAckTimeout)(struct ath_hal*);
    HAL_BOOL  __ahdecl(*ah_setCTSTimeout)(struct ath_hal*, a_uint32_t);
    a_uint32_t     __ahdecl(*ah_getCTSTimeout)(struct ath_hal*);
    HAL_BOOL  __ahdecl(*ah_setDecompMask)(struct ath_hal*, a_uint16_t, a_int32_t);
    void      __ahdecl(*ah_setCoverageClass)(struct ath_hal*, a_uint8_t, a_int32_t);
    HAL_BOOL  __ahdecl(*ah_setEifsMask)(struct ath_hal*, a_uint32_t);
    a_uint32_t     __ahdecl(*ah_getEifsMask)(struct ath_hal*);
    HAL_BOOL  __ahdecl(*ah_setEifsDur)(struct ath_hal*, a_uint32_t);
    a_uint32_t     __ahdecl(*ah_getEifsDur)(struct ath_hal*);
    void      __ahdecl(*ah_getDescInfo)(struct ath_hal*, HAL_DESC_INFO *);

    /* Key Cache Functions */
    a_uint32_t __ahdecl(*ah_getKeyCacheSize)(struct ath_hal*);
    HAL_BOOL  __ahdecl(*ah_resetKeyCacheEntry)(struct ath_hal*, a_uint16_t);
    HAL_BOOL  __ahdecl(*ah_isKeyCacheEntryValid)(struct ath_hal *,
                a_uint16_t);
    HAL_BOOL  __ahdecl(*ah_setKeyCacheEntry)(struct ath_hal*,
                a_uint16_t, const HAL_KEYVAL *,
                const a_uint8_t *, a_int32_t);
    HAL_BOOL  __ahdecl(*ah_setKeyCacheEntryMac)(struct ath_hal*,
                a_uint16_t, const a_uint8_t *);

    /* Power Management Functions */
    HAL_BOOL  __ahdecl(*ah_setPowerMode)(struct ath_hal*,
                HAL_POWER_MODE mode, a_int32_t setChip);
    HAL_POWER_MODE __ahdecl(*ah_getPowerMode)(struct ath_hal*);

    /* Beacon Management Functions */
    void      __ahdecl(*ah_beaconInit)(struct ath_hal *,
                a_uint32_t nexttbtt, a_uint32_t intval);
    void      __ahdecl(*ah_setStationBeaconTimers)(struct ath_hal*,
                const HAL_BEACON_STATE *);
    void      __ahdecl(*ah_resetStationBeaconTimers)(struct ath_hal*);
    HAL_BOOL  __ahdecl(*ah_waitForBeaconDone)(struct ath_hal *,
                HAL_BUS_ADDR);



    /* 11n Functions */
    HAL_BOOL  __ahdecl(*ah_reset11n)(struct ath_hal *ah, HAL_OPMODE opmode,
                HAL_CHANNEL *chan, HAL_HT *ht, HAL_BOOL bChannelChange,
                HAL_STATUS *status);
    void      __ahdecl(*ah_set11nTxDesc)(struct ath_hal *ah,
                struct ath_desc *ds, a_uint32_t pktLen, HAL_PKT_TYPE type,
                a_uint32_t txPower, a_uint32_t keyIx, HAL_KEY_TYPE keyType,
                a_uint32_t flags);
    void      __ahdecl(*ah_set11nRateScenario)(struct ath_hal *ah,
                struct ath_desc *ds, a_uint32_t durUpdateEn, a_uint32_t rtsctsRate, a_uint32_t rtsctsDuration,
                HAL_11N_RATE_SERIES series[], a_uint32_t nseries, a_uint32_t flags);
    void      __ahdecl(*ah_set11nAggrFirst)(struct ath_hal *ah,
                struct ath_desc *ds, a_uint32_t aggrLen, a_uint32_t numDelims);
    void      __ahdecl(*ah_set11nAggrMiddle)(struct ath_hal *ah,
                struct ath_desc *ds, a_uint32_t numDelims);
    void      __ahdecl(*ah_set11nAggrLast)(struct ath_hal *ah,
                struct ath_desc *ds);
    void      __ahdecl(*ah_clr11nAggr)(struct ath_hal *ah,
                struct ath_desc *ds);
    void      __ahdecl(*ah_set11nRifsBurstMiddle)(struct ath_hal *ah,
                struct ath_desc *ds);
    void      __ahdecl(*ah_set11nRifsBurstLast)(struct ath_hal *ah,
                struct ath_desc *ds);
    void      __ahdecl(*ah_clr11nRifsBurst)(struct ath_hal *ah,
                struct ath_desc *ds);
    void      __ahdecl(*ah_set11nAggrRifsBurst)(struct ath_hal *ah,
                struct ath_desc *ds);
    void      __ahdecl(*ah_set11nBurstDuration)(struct ath_hal *ah,
                struct ath_desc *ds, a_uint32_t burstDuration);
    void      __ahdecl(*ah_set11nVirtualMoreFrag)(struct ath_hal *ah,
                struct ath_desc *ds, a_uint32_t vmf);
    a_uint32_t __ahdecl(*ah_get11nExtBusy)(struct ath_hal *ah);
    void      __ahdecl(*ah_set11nMac2040)(struct ath_hal *ah, HAL_HT_MACMODE);
    HAL_HT_RXCLEAR __ahdecl(*ah_get11nRxClear)(struct ath_hal *ah);
    void      __ahdecl(*ah_set11nRxClear)(struct ath_hal *ah, HAL_HT_RXCLEAR rxclear);
    a_int32_t       __ahdecl(*ah_get11nHwPlatform)(struct ath_hal *ah);
    a_uint32_t __ahdecl(*ah_getCycleCounts)(struct ath_hal *, a_uint32_t *,
                       a_uint32_t *, a_uint32_t *);
    void      __ahdecl(*ah_dmaRegDump)(struct ath_hal *);
    HAL_BOOL  __ahdecl(*ah_perCalibration11n)(struct ath_hal*, HAL_CHANNEL *, 
                                            HAL_HT *ht, HAL_BOOL shortcal);
    
    HAL_BOOL  __ahdecl(*ah_set11nRxRifs)(struct ath_hal *ah, HAL_BOOL enable);
    HAL_BOOL  __ahdecl(*ah_get11nRxRifs)(struct ath_hal *ah);
   
    a_uint32_t __ahdecl(*ah_ppmGetRssiDump)(struct ath_hal *);
    a_uint32_t __ahdecl(*ah_ppmArmTrigger)(struct ath_hal *);
    a_int32_t       __ahdecl(*ah_ppmGetTrigger)(struct ath_hal *);
    a_uint32_t __ahdecl(*ah_ppmForce)(struct ath_hal *);
    void      __ahdecl(*ah_ppmUnForce)(struct ath_hal *);
    a_uint32_t __ahdecl(*ah_ppmGetForceState)(struct ath_hal *);
    a_uint32_t __ahdecl(*ah_ppmGetReg)(struct ath_hal *ah, a_int32_t reg);

    void      __ahdecl(*ah_timerStart)(struct ath_hal *,
                a_uint32_t timer_next, a_uint32_t timer_period);
    void      __ahdecl(*ah_timerStop)(struct ath_hal *);

    /* Noise floor related variables */
    int16_t     noisefloor; /* Recently measured noise floor value */
    int16_t     nominal_nf; /* Nominal noise floor value */
    int16_t     nf_delta;   /* Noise floor delta, i.e. swing that is considered ok */
    int16_t     nf_weight;  /* Weightage given to noise floor. Used in EACS */
    int16_t     ah_minUserNFVal;
    int16_t     ah_maxUserNFVal;
    /* Noise floor functions */
    void      __ahdecl(*ah_setNominalUserNFVal)(struct ath_hal *, int16_t );
    int16_t   __ahdecl(*ah_getNominalUserNFVal)(struct ath_hal *);
    void      __ahdecl(*ah_setMinUserNFVal)(struct ath_hal *, int16_t );
    int16_t   __ahdecl(*ah_getMinUserNFVal)(struct ath_hal *);
    void      __ahdecl(*ah_setMaxUserNFVal)(struct ath_hal *, int16_t );
    int16_t   __ahdecl(*ah_getMaxUserNFVal)(struct ath_hal *);
    void      __ahdecl(*ah_setNfDeltaVal)( struct ath_hal *ah, int16_t val );
    int16_t   __ahdecl(*ah_getNfDeltaVal)( struct ath_hal *ah );
    void      __ahdecl(*ah_setNfWeightVal)( struct ath_hal *ah, int16_t val );
    int16_t   __ahdecl(*ah_getNfWeightVal)( struct ath_hal *ah );
    int16_t   __ahdecl(*ah_arGetNoiseFloorVal)( struct ath_hal *ah);
};

/*
 * Check the PCI vendor ID and device ID against Atheros' values
 * and return a printable description for any Atheros hardware.
 * AH_NULL is returned if the ID's do not describe Atheros hardware.
 */
extern  const char *__ahdecl ath_hal_probe(a_uint16_t vendorid, a_uint16_t devid);

/*
 * Attach the HAL for use with the specified device.  The device is
 * defined by the PCI device ID.  The caller provides an opaque pointer
 * to an upper-layer data structure (HAL_SOFTC) that is stored in the
 * HAL state block for later use.  Hardware register accesses are done
 * using the specified bus tag and handle.  On successful return a
 * reference to a state block is returned that must be supplied in all
 * subsequent HAL calls.  Storage associated with this reference is
 * dynamically allocated and must be freed by calling the ah_detach
 * method when the client is done.  If the attach operation fails a
 * null (AH_NULL) reference will be returned and a status code will
 * be returned if the status parameter is non-zero.
 */
extern  struct ath_hal * __ahdecl ath_hal_attach(HAL_SOFTC,
        adf_os_device_t dev, a_uint32_t flags, HAL_STATUS* status);

/*
 * Return a list of channels available for use with the hardware.
 * The list is based on what the hardware is capable of, the specified
 * country code, the modeSelect mask, and whether or not outdoor
 * channels are to be permitted.
 *
 * The channel list is returned in the supplied array.  maxchans
 * defines the maximum size of this array.  nchans contains the actual
 * number of channels returned.  If a problem occurred or there were
 * no channels that met the criteria then AH_FALSE is returned.
 */
extern  HAL_BOOL __ahdecl ath_hal_init_channels(struct ath_hal *,
        HAL_CHANNEL *chans, a_uint32_t maxchans, a_uint32_t *nchans,
        a_uint8_t *regclassids, a_uint32_t maxregids, a_uint32_t *nregids,
        HAL_CTRY_CODE cc, a_uint16_t modeSelect,
        HAL_BOOL enableOutdoor, HAL_BOOL enableExtendedChannels, a_int32_t ht_mode);

/*
 * Return bit mask of wireless modes supported by the hardware.
 */
extern  a_uint32_t __ahdecl ath_hal_getwirelessmodes(struct ath_hal*, HAL_CTRY_CODE);

/*
 * Return rate table for specified mode (11a, 11b, 11g, etc).
 */
extern  const HAL_RATE_TABLE * __ahdecl ath_hal_getratetable(struct ath_hal *,
        a_uint32_t mode);


/*
 * Return if device is public safety.
 */
extern HAL_BOOL __ahdecl ath_hal_ispublicsafetysku(struct ath_hal *);

/*
 * Convert between IEEE channel number and channel frequency
 * using the specified channel flags; e.g. CHANNEL_2GHZ.
 */
extern  a_uint8_t __ahdecl ath_hal_mhz2ieee(struct ath_hal *, a_uint32_t mhz, a_uint32_t flags);

/*
 * Return a version string for the HAL release.
 */
extern  char ath_hal_version[];
/*
 * Return a NULL-terminated array of build/configuration options.
 */
extern  const char* ath_hal_buildopts[];
#endif /* _ATH_AH_H_ */
