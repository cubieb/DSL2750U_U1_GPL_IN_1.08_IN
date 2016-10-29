/*-
 * Copyright (c) 2001 Atsushi Onoe
 * Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/net80211/ieee80211_var.h,v 1.18 2004/12/31 22:42:38 sam Exp $
 */
#ifndef _NET80211_IEEE80211_VAR_H_
#define _NET80211_IEEE80211_VAR_H_

/*
 * Definitions for IEEE 802.11 drivers.
 */
#define IEEE80211_DEBUG
#undef  IEEE80211_DEBUG_REFCNT          /* node refcnt stuff */
#include <asf_bitmap.h>
#include <ieee80211_linux.h>
#include <if_media.h>
#include <if_athproto.h>
#include <_ieee80211.h>
#include <ieee80211.h>
#include <ieee80211_crypto.h>
#include <ieee80211_ioctl.h>       /* for ieee80211_stats */
#include <ieee80211_node.h>
#include <ieee80211_scan.h>
#ifdef ATH_MC_TUNNEL
#include <ieee80211_mcast.h>
#endif
#include <wlan_hdr.h>

/*
 * For carrier platforms, the platform is responsible for providing the
 * implementation for the following APIs.
 */ 
#ifdef __CARRIER_PLATFORM__
#include <ath_carr_pltfrm.h>
#else /* __CARRIER_PLATFORM__ */
/*
 * Built-in implementation for skb allocation.
 */ 
#define ieee80211_alloc_nbuf(_hdl, _size, _res, _align)        adf_nbuf_alloc(_size, _res, _align)
/*
 * Built-in implementation for skb free.
 */ 
#define ieee80211_free_nbuf( _nbuf)          adf_nbuf_free( _nbuf)
/*
 * Built-in implementation for local skb free. Only interesting for platforms
 * that pass skbs between OS instances.
 */ 
#define ieee80211_free_local_nbuf( _nbuf)    ieee80211_free_nbuf( _nbuf)
/*
 * Built-in implementation for copying a remote skb. Only interesting for 
 * platforms that pass skbs between OS instances.
 */ 
#define ieee80211_copy_remote_nbuf(_nbuf)   (_nbuf)
/*
 * Built-in implementation for checking if a skb is cloned. Only interesting 
 * for platforms that pass skbs between OS instances.
 */ 
#define ieee80211_is_nbuf_cloned(_nbuf)     (0)
/*
 * Built-in implementation for dumping skb internals.
 */ 
#define ieee80211_dump_nbuf(_nbuf)            
#endif /* __CARRIER_PLATFORM__ */

/*
 * If Linux can't tell us if irqs are disabled, then they are not.
 */ 
#ifndef irqs_disabled
#define irqs_disabled()     (0)
#endif

#define IEEE80211_TXPOWER_MAX   100 /* .5 dbM (XXX units?) */
#define IEEE80211_TXPOWER_MIN   0   /* kill radio */

#define	IEEE80211_DTIM_MAX	255	/* max DTIM period */
#define IEEE80211_DTIM_MIN  1   /* min DTIM period */
#define IEEE80211_DTIM_DEFAULT  1   /* default DTIM period */

#define	IEEE80211_BINTVAL_MAX	1000	/* max beacon interval (TU's) */
#define IEEE80211_BINTVAL_MIN   20  /* min beacon interval (TU's) */
#define IEEE80211_BINTVAL_DEFAULT 100   /* default beacon interval (TU's) */

#define IEEE80211_BGSCAN_INTVAL_MIN 15  /* min bg scan intvl (secs) */
#define IEEE80211_BGSCAN_INTVAL_DEFAULT (5*60)  /* default bg scan intvl */

#define IEEE80211_BGSCAN_IDLE_MIN   100 /* min idle time (ms) */
#define IEEE80211_BGSCAN_IDLE_DEFAULT   250 /* default idle time (ms) */

#define IEEE80211_COVERAGE_CLASS_MAX    31  /* max coverage class */
#define IEEE80211_REGCLASSIDS_MAX   10  /* max regclass id list */

#define IEEE80211_PS_SLEEP  0x1 /* STA is in power saving mode */
#define IEEE80211_PS_MAX_QUEUE  100  /* maximum saved packets */

#define IEEE80211_XR_BEACON_FACTOR  3   /* factor between xr Beacon interval and normal beacon interval */
#define IEEE80211_XR_DEFAULT_RATE_INDEX 0
#define IEEE80211_XR_FRAG_THRESHOLD 540

#define IEEE80211_SWBMISS_THRESHOLD	2 /* software beacon miss threshold, in TU's */
#define IEEE80211_BMISS_COUNT_MAX	2 /* maximum consecutive bmiss allowed */

#define IEEE80211_MS_TO_TU(x)   (((x) * 1000) / 1024)
#define IEEE80211_TU_TO_MS(x)   (((x) * 1024) / 1000)
#define IEEE80211_TU_TO_JIFFIES(x) (msecs_to_jiffies((((x) * 1024) / 1000)) )

#define IEEE80211_APPIE_MAX        1024
#define MODULE_NAME		   "ath"

#define NODE_INVALID 100

#define IEEE80211_PWRCONSTRAINT_VAL(ic) \
    (((ic)->ic_bsschan->ic_maxregpower > (ic)->ic_curchanmaxpwr) ? \
        (ic)->ic_bsschan->ic_maxregpower - (ic)->ic_curchanmaxpwr : 0)

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

#define IEEE80211_RB_DEF_TIMEOUT      7000
#define IEEE80211_RB_DEF_SKIP_THRESH  5

typedef struct ieee80211_rb {
    a_uint16_t       timeout_ms;               /* timeout -> non_rb  */
    a_uint8_t        skip_thresh;              /* 1-seq skips -> rb  */
} ieee80211_rb_t;

#define IEEE80211_BINTVAL_VALID(_bi) \
        ((IEEE80211_BINTVAL_MIN <= (_bi)) && \
         ((_bi) <= IEEE80211_BINTVAL_MAX))
#define IEEE80211_BINTVAL_SANITISE(_bi) \
        (IEEE80211_BINTVAL_VALID(_bi) ? \
         (_bi) : IEEE80211_BINTVAL_DEFAULT)

/*
 * 802.11 control state is split into a common portion that maps
 * 1-1 to a physical device and one or more "Virtual AP's" (VAP)
 * that are bound to an ieee80211com instance and share a single
 * underlying device.  Each VAP has a corresponding OS device
 * entity through which traffic flows and that applications use
 * for issuing ioctls, etc.
 */

/*
 * Data common to one or more virtual AP's.  State shared by
 * the underlying device and the net80211 layer is exposed here;
 * e.g. device-specific callbacks.
 */
struct ieee80211vap;
#define TARGET_MAX_VAP  4
#define TARGET_MAX_NODE 32

struct ieee80211com {
    void *host_wmi_handle;
    adf_net_handle_t    ic_dev;    /* associated device */
    adf_os_device_t     ic_osdev;
    ieee80211com_lock_t ic_comlock; /* state update lock */
    ieee80211com_lock_t ic_vapslock; /* vap state machine lock */
	a_bool_t			ic_vapslock_held;
    asf_tailq_head(ic_vaplist, ieee80211vap) ic_vaps; /* list of vap instances */
    enum ieee80211_phytype  ic_phytype; /* XXX wrong for multi-mode */
    enum ieee80211_opmode   ic_opmode;  /* operation mode */
    struct ifmedia      ic_media;   /* interface media config */
    a_uint8_t        ic_myaddr[IEEE80211_ADDR_LEN];
    u_int8_t         ic_ether_dongle_flag;
    a_uint8_t        ic_sta_netdev_addr[IEEE80211_ADDR_LEN];    
    adf_os_timer_t   ic_inact;   /* mgmt/inactivity timer */
    a_uint8_t        target_node_bitmap[TARGET_MAX_NODE];
    a_uint8_t        target_vap_bitmap[TARGET_MAX_VAP];
    a_bool_t         cleanup_in_node_table_reset;
    a_uint8_t        ic_node_cnt;
    a_uint32_t       ic_time_nonerp_present;
    a_uint32_t       ic_time_noht_present; /* time since last HT IE in beacon */
    a_uint32_t       ic_flags;   /* state flags */
    a_uint32_t       ic_flags_ext;   /* extension of state flags */
    a_uint32_t       ic_caps;    /* capabilities */
    a_uint8_t        ic_ath_cap; /* Atheros adv. capablities */
    a_uint8_t	     ic_broadcast[IEEE80211_ADDR_LEN];
    a_uint32_t       ic_ath_extcap; /* Owl specific flags */
    a_uint8_t        ic_promisc; /* vap's needing promisc mode */
    a_uint8_t        ic_allmulti;    /* vap's needing all multicast*/
    a_uint8_t        ic_nopened; /* vap's been opened */
    a_uint8_t        ic_nrunning;    /* vap's marked RUNNING */
    struct ieee80211_rateset ic_sup_rates[IEEE80211_MODE_MAX];
    struct ieee80211_rateset ic_sup_xr_rates;
    struct ieee80211_rateset ic_sup_half_rates;
    struct ieee80211_rateset ic_sup_quarter_rates;
    struct ieee80211_rateset ic_sup_ht_rates[IEEE80211_MODE_MAX];
    a_uint16_t       ic_modecaps;    /* set of mode capabilities */
    a_uint16_t       ic_curmode; /* current mode */
    a_uint16_t       ic_lintval; /* beacon interval */
    a_uint16_t       ic_holdover;    /* PM hold over duration */
    a_uint16_t       ic_bmisstimeout;/* beacon miss threshold (ms) */
    a_uint16_t       ic_txpowlimit;  /* global tx power limit */
    a_uint8_t        ic_coverageclass; /* coverage class */
    a_uint16_t	    ic_chanbwflag;    /* channel bandwidth (half/quater/full) */ 

    /* 11n Capabilities */
    a_uint16_t       ic_htflags; /* HT state flags */
    a_uint16_t       ic_htcap;   /* HT capabilities */
    a_uint16_t       ic_htextcap;/* HT extended capabilities */ 
    a_uint8_t        ic_maxampdu;    /* maximum rx A-MPDU factor */
    a_uint8_t        ic_mpdudensity; /* MPDU density */

    /* 11n Information */
    struct ieee80211_cwm    ic_cwm;     /* Channel Width Management */

    /*
     * Channel state:
     *
     * ic_channels is the set of available channels for the device;
     *    it is setup by the driver
     * ic_nchans is the number of valid entries in ic_channels
     * ic_chan_avail is a bit vector of these channels used to check
     *    whether a channel is available w/o searching the channel table.
     * ic_chan_active is a (potentially) constrained subset of
     *    ic_chan_avail that reflects any mode setting or user-specified
     *    limit on the set of channels to use/scan
     * ic_curchan is the current channel the device is set to; it may
     *    be different from ic_bsschan when we are off-channel scanning
     *    or otherwise doing background work
     * ic_bsschan is the channel selected for operation; it may
     *    be undefined (IEEE80211_CHAN_ANYC)
     * ic_prevchan is a cached ``previous channel'' used to optimize
     *    lookups when switching back+forth between two channels
     *    (e.g. for dynamic turbo)
     */
    a_uint32_t         ic_nchans;  /* # entries in ic_channels */
    struct ieee80211_channel ic_channels[IEEE80211_CHAN_MAX+1];
    a_uint8_t        ic_chan_avail[IEEE80211_CHAN_BYTES];
    a_uint8_t        ic_chan_active[IEEE80211_CHAN_BYTES];
    struct ieee80211_channel *ic_curchan;   /* current channel */
    struct ieee80211_channel *ic_bsschan;   /* bss channel */
    struct ieee80211_channel *ic_prevchan;  /* previous channel */

    /* regulatory class ids */
    a_uint32_t           ic_nregclass;  /* # entries in ic_regclassids */
    a_uint8_t        ic_regclassids[IEEE80211_REGCLASSIDS_MAX];
    a_uint8_t        ic_isdfsregdomain; /* operating in DFS domain ? */
    /* scan-related state */
    struct ieee80211_scan_state *ic_scan;   /* scan state */
    enum ieee80211_roamingmode ic_roaming;  /* roaming mode */
    adf_time_t       ic_lastdata;    /* time of last data frame */
    adf_time_t       ic_lastscan;    /* time last scan completed */
    a_uint32_t       ic_nextscandelay; /* time between set of channel scans */

    /* NB: this is the union of all vap stations/neighbors */
    struct ieee80211_node_table ic_sta; /* stations/neighbors */

    /* XXX multi-bss: split out common/vap parts? */
    struct ieee80211_wme_state ic_wme;  /* WME/WMM state */

    /* XXX multi-bss: can per-vap be done/make sense? */
    enum ieee80211_protmode ic_protmode;    /* 802.11g protection mode */
    a_uint16_t       ic_nonerpsta;   /* # non-ERP stations */
    a_uint16_t       ic_longslotsta; /* # long slot time stations */
    a_uint16_t       ic_sta_assoc;   /* stations associated */
    a_uint16_t       ic_dt_sta_assoc;/* dturbo capable stations */
    a_uint16_t       ic_xr_sta_assoc;/* XR stations associated */
    a_uint16_t       ic_non_ht_sta;  /* # of non-HT stations */
    a_uint16_t       ic_ht20_sta;    /* # of HT20 stations */
    a_uint8_t        ic_ht_prot_sm; /* HT protection mode state */

    /*
     * Spectrum Management.
     */
    a_uint16_t       ic_country_code;
    a_int32_t             ic_country_outdoor;
    struct ieee80211_ie_country ic_country_ie; /* country info element */
    /*
     *  current channel max power, used to compute Power Constraint IE.
     *
     *  NB: local power constraint depends on the channel, but assuming it must
     *     be detected dynamically, we cannot maintain a table (i.e., will not
     *     know value until change to channel and detect).
     */
    a_uint8_t        ic_bangradar;
    a_uint8_t        ic_curchanmaxpwr;
    a_uint8_t        ic_chanchange_tbtt;
    a_uint8_t        ic_chanchange_chan;

    /* 
     * Global chain select mask for tx and rx.
     */
    a_uint8_t       ic_tx_chainmask_legacy;
    a_uint8_t 	   ic_tx_chainmask;
    a_uint8_t	   ic_rx_chainmask;

    /* Set RTS rate */
    a_uint8_t	   ic_rtscts_ratecode;

    /* virtual ap create/delete */
    struct ieee80211vap     *(*ic_vap_create)(struct ieee80211com *,
                    const char *name, a_int32_t unit,
                    a_int32_t opmode, a_int32_t flags);
    void            (*ic_vap_delete)(struct ieee80211vap *);
    /* send/recv 802.11 management frame */
    a_int32_t         (*ic_send_mgmt)(struct ieee80211_node *,
                    a_int32_t, a_int32_t);
    void            (*ic_recv_mgmt)(struct ieee80211_node *,
                    adf_nbuf_t, a_int32_t, a_int32_t, a_uint32_t);
#ifdef ATH_FORCE_PPM
    /* Force PPM tracking workaround */
    void            (*ic_force_ppm_notify)(struct ieee80211_node *, a_int32_t);
#endif
    /* send management frame to driver, like hardstart */
    a_int32_t         (*ic_mgtstart)(struct ieee80211com *,
                    adf_nbuf_t);
    /* reset device state after 802.11 parameter/state change */
    a_status_t         (*ic_init)(adf_drv_handle_t);
    a_int32_t         (*ic_reset)(adf_drv_handle_t);
    /* update device state for 802.11 slot time change */
    void            (*ic_updateslot)(adf_drv_handle_t);
    /* new station association callback/notification */
    void            (*ic_newassoc)(struct ieee80211_node *, a_int32_t);
    /* sta change mac address, used for ether-dongle feature */
    int         (*ic_sta_change_mac_addr)(struct ieee80211com * , a_uint8_t*);

    /* node state management */
    struct ieee80211_node*  (*ic_node_alloc)(struct ieee80211_node_table *,struct ieee80211vap *);
    void            (*ic_node_free)(struct ieee80211_node *);
    void            (*ic_node_cleanup)(struct ieee80211_node *);
    void            (*ic_add_node_target)(struct ieee80211com *, struct ieee80211_node_target *);
    void            (*ic_add_vap_target)(struct ieee80211com *, struct ieee80211vap_target *);
    void            (*ic_delete_vap_target)(struct ieee80211com *, struct ieee80211vap *);
    void            (*ic_delete_node_target)(struct ieee80211com *, struct ieee80211_node *);
    
    a_int8_t		    	(*ic_node_getrssi)(const struct ieee80211_node *);
    a_uint8_t        (*ic_node_move_data)(const struct ieee80211_node *);

    /* scanning support */
    void            (*ic_scan_start)(struct ieee80211com *);
    void            (*ic_scan_end)(struct ieee80211com *);
    void            (*ic_set_channel)(struct ieee80211com *);

	/* u-apsd support */
	//void			(*ic_uapsd_flush)(struct ieee80211_node *);

    /* set coverage class */
    void            (*ic_set_coverageclass)(struct ieee80211com *);

    /* mhz to ieee conversion */
    a_uint32_t           (*ic_mhz2ieee) (struct ieee80211com *, a_uint32_t freq, a_uint32_t flags);

	void			(*ic_athcapsetup) (struct ieee80211com *, a_int32_t);

	/* beacon update */
	void            (*ic_beacon_update)(struct ieee80211_node *, a_int32_t rssi);

	/* Ad-Hoc aggregation addba clear */
	void            (*ic_adhoc_aggr_clear)(struct ieee80211com *, struct ieee80211_node *);
    /*
     * 11n A-MPDU and A-MSDU support
     */
    a_int32_t             ic_ampdu_limit;     /* A-MPDU length limit   */
    a_int32_t             ic_ampdu_density;   /* A-MPDU density        */
    a_uint8_t             ic_ampdu_subframes; /* A-MPDU subframe limit */ 
    a_int32_t             ic_amsdu_limit;     /* A-MSDU length limit   */

    /* 
     * ADDBA support 
     */
    a_uint8_t        ic_addba_mode; /* ADDBA mode auto or manual */
    void            (*ic_addba_requestsetup)(struct ieee80211com *,
					     struct ieee80211_node *,
					     a_uint8_t 				tidno,
					     struct ieee80211_ba_parameterset 	*baparamset,
					     a_uint16_t                		*batimeout,
                                             struct ieee80211_ba_seqctrl        *basequencectrl,
                                             a_uint16_t                          buffersize);

    void	    (*ic_addba_requestprocess)(struct ieee80211com *,
					       struct ieee80211_node *,
					       a_uint8_t                		dialogtoken,
					       struct ieee80211_ba_parameterset	*baparamset,
					       a_uint16_t                	batimeout,
					       struct ieee80211_ba_seqctrl	basequencectrl);
    void	    (*ic_addba_responsesetup)(struct ieee80211com *,
					      struct ieee80211_node *,
					      a_uint8_t 				tidno,
					      a_uint8_t                		*dialogtoken,
					      a_uint16_t                		*statuscode,
					      struct ieee80211_ba_parameterset  *baparamset,
					      a_uint16_t                		*batimeout);
    void            (*ic_addba_responseprocess)(struct ieee80211com *,
					     struct ieee80211_node *,
					     a_uint16_t				statuscode,
					     struct ieee80211_ba_parameterset 	*baparamset,
					     a_uint16_t                		batimeout);
    void            (*ic_delba_process)(struct ieee80211com *,
					     struct ieee80211_node *,
					     struct ieee80211_delba_parameterset *delbaparamset,
					     a_uint16_t                		reasoncode);
    a_int32_t             (*ic_addba_send)(struct ieee80211com *ic,
                                             struct ieee80211_node *ni,
                                             a_uint8_t tid,
                                             a_uint16_t buffersize);
    void            (*ic_delba_send)(struct ieee80211com *ic,
                                             struct ieee80211_node *ni,
                                             a_uint8_t tid,
                                             a_uint8_t initiator,
                                             a_uint16_t reasoncode);
    void            (*ic_addba_status)(struct ieee80211com *ic,
                                             struct ieee80211_node *ni,
                                             a_uint8_t tid,
                                             a_uint16_t *status);
    void            (*ic_addba_setresponse)(struct ieee80211com *ic,
                                             struct ieee80211_node *ni,
                                             a_uint8_t tid,
                                             a_uint16_t statuscode);
    void            (*ic_addba_clearresponse)(struct ieee80211com *ic,
                                             struct ieee80211_node *ni);
    ieee80211_rb_t  ic_rb;
    void            (*ic_set_rb)(struct ieee80211com *ic, a_int32_t enable);                                             
#ifdef ATH_MC_TUNNEL
	struct MC_SNOOP_LIST mc_snoop_list;
#endif
};

/*
 * Manual ADDBA support
 */
enum {
    ADDBA_SEND   = 0,
    ADDBA_STATUS = 1,
    DELBA_SEND   = 2,
    ADDBA_RESP   = 3,
    ADDBA_CLR_RESP = 4,
};

enum {
    ADDBA_MODE_AUTO   = 0,
    ADDBA_MODE_MANUAL = 1,
};

struct ieee80211_addba_delba_request {
    struct ieee80211com *ic;
    a_uint8_t action;
    a_uint8_t tid;
    a_uint16_t status;
    a_uint16_t aid;
    a_uint32_t arg1;
    a_uint32_t arg2;
};

struct vlan_group;
struct eapolcom;
struct ieee80211_aclator;

struct ieee80211_app_ie_t { 
    a_uint32_t length;
    a_uint8_t *ie;
}; 

typedef struct adf_net_stats_s
{
	unsigned long	rx_packets;		/* total packets received	*/
	unsigned long	tx_packets;		/* total packets transmitted	*/
	unsigned long	rx_bytes;		/* total bytes received 	*/
	unsigned long	tx_bytes;		/* total bytes transmitted	*/
	unsigned long	rx_errors;		/* bad packets received		*/
	unsigned long	tx_errors;		/* packet transmit problems	*/
	unsigned long	rx_dropped;		/* no space in linux buffers	*/
	unsigned long	tx_dropped;		/* no space available in linux	*/
	unsigned long	multicast;		/* multicast packets received	*/
	unsigned long	collisions;

	/* detailed rx_errors: */
	unsigned long	rx_length_errors;
	unsigned long	rx_over_errors;		/* receiver ring buff overflow	*/
	unsigned long	rx_crc_errors;		/* recved pkt with crc error	*/
	unsigned long	rx_frame_errors;	/* recv'd frame alignment error */
	unsigned long	rx_fifo_errors;		/* recv'r fifo overrun		*/
	unsigned long	rx_missed_errors;	/* receiver missed packet	*/

	/* detailed tx_errors */
	unsigned long	tx_aborted_errors;
	unsigned long	tx_carrier_errors;
	unsigned long	tx_fifo_errors;
	unsigned long	tx_heartbeat_errors;
	unsigned long	tx_window_errors;
	
	/* for cslip etc */
	unsigned long	rx_compressed;
	unsigned long	tx_compressed;
} adf_net_stats_t;

struct ieee80211vap {
    adf_net_handle_t   iv_dev;    /* associated device */
    adf_net_stats_t    iv_devstats;    /* interface statistics */
    struct ifmedia      iv_media;   /* interface media config */
    a_uint8_t		iv_vapindex;
    a_uint32_t		mtu;	    /* max transmission unit */
    adf_time_t		last_rx;
  // delete #ifdef CONFIG_NET_WIRELESS
    adf_net_wcmd_vapstats_t    iv_iwstats; /* wireless statistics block */
  // delete #endif
#ifdef CONFIG_SYSCTL
    struct ctl_table_header *iv_sysctl_header;
    struct ctl_table    *iv_sysctls;
#endif
    struct vlan_group   *iv_vlgrp;  /* vlan group state */

    asf_tailq_entry(ieee80211vap) iv_next;  /* list of vap instances */
    a_uint32_t           iv_unit;    /* virtual AP unit */
    struct ieee80211com *iv_ic;     /* back ptr to common state */
    a_uint32_t       iv_debug;   /* debug msg flags */
    struct ieee80211_stats  iv_stats;   /* statistics */

    a_int32_t             (*iv_newstate)(struct ieee80211vap *,
                    enum ieee80211_state, a_int32_t);
    a_uint8_t        iv_myaddr[IEEE80211_ADDR_LEN];
    a_uint32_t       iv_flags;   /* state flags */
    a_uint32_t       iv_flags_ext;   /* extension of state flags */
    a_uint32_t       iv_caps;    /* capabilities */
    a_uint8_t        iv_ath_cap; /* Atheros adv. capablities */
    enum ieee80211_opmode   iv_opmode;  /* operation mode */
    enum ieee80211_state    iv_state;   /* state machine state */
    adf_os_timer_t          iv_mgtsend; /* mgmt frame response timer */
                        /* inactivity timer settings */
    a_int32_t         iv_inact_init;  /* setting for new station */
    a_int32_t         iv_inact_auth;  /* auth but not assoc setting */
    a_int32_t         iv_inact_run;   /* authorized setting */
    a_int32_t         iv_inact_probe; /* inactive probe time */

    a_int32_t         iv_des_nssid;   /* # desired ssids */
    struct ieee80211_scan_ssid iv_des_ssid[1];/* desired ssid table */
    a_uint8_t        iv_des_bssid[IEEE80211_ADDR_LEN];
    struct ieee80211_channel *iv_des_chan;  /* desired channel */
    a_uint16_t       iv_des_mode;    /* desired mode */
    a_int32_t         iv_nicknamelen; /* XXX junk */
    a_uint8_t        iv_nickname[IEEE80211_NWID_LEN];
    adf_time_t           iv_bgscanidle;  /* bg scan idle threshold */
    adf_time_t           iv_bgscanintvl; /* bg scan min interval */
    adf_time_t           iv_scanvalid;   /* scan cache valid threshold */
    struct ieee80211_roam   iv_roam;    /* sta-mode roaming state */

    a_uint32_t       *iv_aid_bitmap; /* association id map */
    a_uint16_t       iv_max_aid;
    a_uint16_t       iv_sta_assoc;   /* stations associated */
    a_uint16_t       iv_ps_sta;  /* stations in power save */
    a_uint16_t       iv_ps_pending;  /* ps sta's w/ pending frames */
    a_uint8_t        *iv_tim_bitmap; /* power-save stations w/ data*/
    a_uint8_t       iv_tim_len; /* ic_tim_bitmap size (bytes) */
    a_uint8_t        iv_dtim_period; /* DTIM period */
    a_uint8_t        iv_dtim_count;  /* DTIM count from last bcn */
                        /* set/unset aid pwrsav state */
    void            (*iv_set_tim)(struct ieee80211_node *, a_int32_t);
    a_uint8_t	    iv_uapsdinfo;	/* sta mode QoS Info flags */
    struct ieee80211_node   *iv_bss;    /* information for this node */
    struct ieee80211_fixed_rate   iv_fixed_rate;  /* 802.11 rate or -1 */
    a_uint16_t       iv_rtsthreshold;
    a_uint16_t       iv_fragthreshold;
    a_uint16_t       iv_txmin;   /* min tx retry count */
    a_uint16_t       iv_txmax;   /* max tx retry count */
    a_uint16_t       iv_txlifetime;  /* tx lifetime */
    a_int32_t             iv_inact_timer; /* inactivity timer wait */
    void            *iv_opt_ie; /* user-specified IE's */
    a_uint16_t       iv_opt_ie_len;  /* length of ni_opt_ie */
    a_uint16_t       iv_def_txkey;   /* default/group tx key index */
    struct ieee80211_key    iv_nw_keys[IEEE80211_WEP_NKID];
    a_uint16_t         (*iv_key_alloc)(struct ieee80211vap *,
                    const struct ieee80211_key *);
    a_int32_t         (*iv_key_delete)(struct ieee80211vap *,
                    const struct ieee80211_key *,
                    struct ieee80211_node *);
    a_int32_t   (*iv_key_set)(struct ieee80211vap *,
                     struct ieee80211_key *,
                    const a_uint8_t mac[IEEE80211_ADDR_LEN]);
    void            (*iv_key_update_begin)(struct ieee80211vap *);
    void            (*iv_key_update_end)(struct ieee80211vap *);
#ifdef ATH_SUPERG_COMP
    void            (*iv_comp_set)(struct ieee80211vap *,
                    struct ieee80211_node *, a_int32_t);
#endif
    const struct ieee80211_authenticator *iv_auth;/* authenticator glue */
    void            *iv_ec;     /* private auth state */
    struct ieee80211vap *iv_xrvap; /* pointer to XR VAP , if XR is enabled */
    a_uint16_t       iv_xrbcnwait;   /* SWBA count incremented until it reaches XR_BECON_FACTOR */
    adf_os_timer_t  iv_xrvapstart;  /* timer to start xr */
    a_uint8_t        iv_chanchange_count; /* 11h counter for channel change */
    a_int32_t             iv_mcast_rate; /* Multicast rate (Kbps) */

    const struct ieee80211_aclator *iv_acl;	/* aclator glue */
    void			*iv_as;		/* private aclator state */

    adf_os_timer_t	iv_swbmiss;	/* software beacon miss timer */
    a_uint16_t		iv_swbmiss_period; /* software beacon miss timer period */
    a_uint8_t		iv_bmiss_count;
	
    a_int32_t             iv_mcast_fixedrate; /* fixed rate for Multicast */
    struct ieee80211_app_ie_t  app_ie[IEEE80211_APPIE_NUM_OF_FRAME];
    a_uint32_t       app_filter;
    a_uint16_t       sequence;
    a_int32_t    iv_disable_htrates; /* flag to disable HT rates for WEP and TKIP */
    a_uint32_t       iv_vapvalid;
};

#define IEEE80211_ADDR_EQ(a1,a2)    (adf_os_mem_cmp(a1,a2,IEEE80211_ADDR_LEN) == 0)
#define IEEE80211_ADDR_COPY(dst,src)    adf_os_mem_copy(dst, src, IEEE80211_ADDR_LEN)

/* ic_flags */
#define IEEE80211_F_FF          0x00000001  /* CONF: ATH FF enabled */
#define IEEE80211_F_TURBOP      0x00000002  /* CONF: ATH Turbo enabled*/
#define IEEE80211_F_PROMISC     0x00000004  /* STATUS: promiscuous mode */
#define IEEE80211_F_ALLMULTI    0x00000008  /* STATUS: all multicast mode */
/* NB: this is intentionally setup to be IEEE80211_CAPINFO_PRIVACY */
#define IEEE80211_F_PRIVACY     0x00000010  /* CONF: privacy enabled */
#define IEEE80211_F_PUREG       0x00000020  /* CONF: 11g w/o 11b sta's */
#define IEEE80211_F_XRUPDATE    0x00000040  /* CONF: update beacon XR element*/
#define IEEE80211_F_SCAN        0x00000080  /* STATUS: scanning */
#define IEEE80211_F_XR          0x00000100  /* CONF: operate in XR mode */
#define IEEE80211_F_SIBSS       0x00000200  /* STATUS: start IBSS */
/* NB: this is intentionally setup to be IEEE80211_CAPINFO_SHORT_SLOTTIME */
#define IEEE80211_F_SHSLOT      0x00000400  /* STATUS: use short slot time*/
#define IEEE80211_F_PMGTON      0x00000800  /* CONF: Power mgmt enable */
#define IEEE80211_F_DESBSSID    0x00001000  /* CONF: des_bssid is set */
#define IEEE80211_F_WME         0x00002000  /* CONF: enable WME use */
#define IEEE80211_F_BGSCAN      0x00004000  /* CONF: bg scan enabled */
#define IEEE80211_F_SWRETRY     0x00008000  /* CONF: sw tx retry enabled */
#define IEEE80211_F_TXPOW_FIXED 0x00010000  /* TX Power: fixed rate */
#define IEEE80211_F_IBSSON      0x00020000  /* CONF: IBSS creation enable */
#define IEEE80211_F_SHPREAMBLE  0x00040000  /* STATUS: use short preamble */
#define IEEE80211_F_DATAPAD     0x00080000  /* CONF: do alignment pad */
#define IEEE80211_F_USEPROT     0x00100000  /* STATUS: protection enabled */
#define IEEE80211_F_USEBARKER   0x00200000  /* STATUS: use barker preamble*/
#define IEEE80211_F_TIMUPDATE   0x00400000  /* STATUS: update beacon tim */
#define IEEE80211_F_WPA1        0x00800000  /* CONF: WPA enabled */
#define IEEE80211_F_WPA2        0x01000000  /* CONF: WPA2 enabled */
#define IEEE80211_F_WPA         0x01800000  /* CONF: WPA/WPA2 enabled */
#define IEEE80211_F_DROPUNENC   0x02000000  /* CONF: drop unencrypted */
#define IEEE80211_F_COUNTERM    0x04000000  /* CONF: TKIP countermeasures */
#define IEEE80211_F_HIDESSID    0x08000000  /* CONF: hide SSID in beacon */
#define IEEE80211_F_NOBRIDGE    0x10000000  /* CONF: disable internal bridge */
#define IEEE80211_F_WMEUPDATE   0x20000000  /* STATUS: update beacon wme */
#define IEEE80211_F_DOTH        0x40000000  /* enable 11.h */
#define IEEE80211_F_CHANSWITCH  0x80000000  /* force chanswitch */

/* ic_flags_ext */
#define IEEE80211_FEXT_WDS          0x00000001  /* CONF: 4 addr allowed */
#define IEEE80211_FEXT_COUNTRYIE    0x00000002 /* CONF: enable country IE */
#define IEEE80211_FEXT_SCAN_PENDING 0x00000004 /* STATE: scan pending */
#define	IEEE80211_FEXT_BGSCAN	    0x00000008	/* STATE: enable full bgscan completion */
#define IEEE80211_FEXT_UAPSD	    0x00000010	/* CONF: enable U-APSD */
#define IEEE80211_FEXT_SLEEP	    0x00000020	/* STATUS: sleeping */
#define IEEE80211_FEXT_EOSPDROP	    0x00000040	/* drop uapsd EOSP frames for test */
#define	IEEE80211_FEXT_MARKDFS	    0x00000080	/* Enable marking of dfs interfernce */
#define IEEE80211_FEXT_REGCLASS	    0x00000100	/* CONF: send regclassids in country ie */
#define	IEEE80211_FEXT_MARKDFS      0x00000080	/* Enable marking of dfs interfernce */
#define IEEE80211_FEXT_ERPUPDATE    0x00000200	/* STATUS: update ERP element */
#define IEEE80211_FEXT_SWBMISS      0x00000400 	/* CONF: use software beacon timer */
#define IEEE80211_FEXT_BLKDFSCHAN   0x00000800 	/* CONF: block the use of DFS channels */
#define IEEE80211_FEXT_APPIE_UPDATE 0x00001000 /* STATE: beacon APP IE updated */
#define IEEE80211_FAST_CC           0x00002000  /* CONF: Fast channel change */
#define IEEE80211_C_AMPDU           0x00004000  /* CONF: A-MPDU supported */
#define IEEE80211_C_AMSDU           0x00008000  /* CONF: A-MSDU supported */
#define IEEE80211_C_HTPROT          0x00010000  /* CONF: HT traffic protected */
#define IEEE80211_C_RESET           0x00020000  /* CONF: Reset once */
#define IEEE80211_F_NONHT_AP        0x00040000  /* STATUS: HT traffic protected */
#define IEEE80211_FEXT_HTUPDATE     0x00080000      /* STATUS: update HT element */
#define IEEE80211_C_WDS_AUTODETECT  0x00100000 /* CONF: WDS auto Detect/DELBA */
#define IEEE80211_C_RB              0x00200000  /* CONF: RB control */
#define IEEE80211_C_RB_DETECT       0x00400000  /* CONF: RB auto detection */
#define IEEE80211_C_NO_HTIE         0x00800000 /* CONF: No HT IE sending/parsing */
#define IEEE80211_C_ETHER_DONGLE    0x01000000 /* CONF: flag for the support for ether-dongle*/

#define IEEE80211_ETHER_DONGLE_ENABLE(_ic)        ((_ic)->ic_flags_ext |= IEEE80211_C_ETHER_DONGLE)
#define IEEE80211_ETHER_DONGLE_DISABLE(_ic)       ((_ic)->ic_flags_ext &= ~IEEE80211_C_ETHER_DONGLE)
#define IEEE80211_ETHER_DONGLE_IS_ENABLED(_ic)    ((_ic)->ic_flags_ext & IEEE80211_C_ETHER_DONGLE)
#define IEEE80211_ETHER_DONGLE_F_LEARNING     0x01

#define IEEE80211_COM_GOTOSLEEP(_ic)		((_ic)->ic_flags_ext |= IEEE80211_FEXT_GOTOSLEEP)
#define IEEE80211_COM_WAKEUP(_ic)		((_ic)->ic_flags_ext &= ~IEEE80211_FEXT_SLEEP)
#define IEEE80211_COM_IS_SLEEPING(_ic)		((_ic)->ic_flags_ext & IEEE80211_FEXT_SLEEP)

#define IEEE80211_VAP_UAPSD_ENABLE(_v)	((_v)->iv_flags_ext |= IEEE80211_FEXT_UAPSD)
#define IEEE80211_VAP_UAPSD_DISABLE(_v)	((_v)->iv_flags_ext &= ~IEEE80211_FEXT_UAPSD)
#define IEEE80211_VAP_UAPSD_ENABLED(_v)	((_v)->iv_flags_ext & IEEE80211_FEXT_UAPSD)
#define IEEE80211_VAP_GOTOSLEEP(_v)	((_v)->iv_flags_ext |= IEEE80211_FEXT_SLEEP)
#define IEEE80211_VAP_WAKEUP(_v)	((_v)->iv_flags_ext &= ~IEEE80211_FEXT_SLEEP)
#define IEEE80211_VAP_IS_SLEEPING(_v)	((_v)->iv_flags_ext & IEEE80211_FEXT_SLEEP)
#define IEEE80211_VAP_EOSPDROP_ENABLE(_v)  ((_v)->iv_flags_ext |= IEEE80211_FEXT_EOSPDROP)
#define IEEE80211_VAP_EOSPDROP_DISABLE(_v) ((_v)->iv_flags_ext &= ~IEEE80211_FEXT_EOSPDROP)
#define IEEE80211_VAP_EOSPDROP_ENABLED(_v) ((_v)->iv_flags_ext & IEEE80211_FEXT_EOSPDROP)

/* ic_caps */
#define IEEE80211_C_WEP     0x00000001  /* CAPABILITY: WEP available */
#define IEEE80211_C_TKIP    0x00000002  /* CAPABILITY: TKIP available */
#define IEEE80211_C_AES     0x00000004  /* CAPABILITY: AES OCB avail */
#define IEEE80211_C_AES_CCM 0x00000008  /* CAPABILITY: AES CCM avail */
#define IEEE80211_C_CKIP    0x00000020  /* CAPABILITY: CKIP available */
#define IEEE80211_C_FF      0x00000040  /* CAPABILITY: ATH FF avail */
#define IEEE80211_C_TURBOP  0x00000080  /* CAPABILITY: ATH Turbo avail*/
#define IEEE80211_C_IBSS    0x00000100  /* CAPABILITY: IBSS available */
#define IEEE80211_C_PMGT    0x00000200  /* CAPABILITY: Power mgmt */
#define IEEE80211_C_HOSTAP  0x00000400  /* CAPABILITY: HOSTAP avail */
#define IEEE80211_C_AHDEMO  0x00000800  /* CAPABILITY: Old Adhoc Demo */
#define IEEE80211_C_SWRETRY 0x00001000  /* CAPABILITY: sw tx retry */
#define IEEE80211_C_TXPMGT  0x00002000  /* CAPABILITY: tx power mgmt */
#define IEEE80211_C_SHSLOT  0x00004000  /* CAPABILITY: short slottime */
#define IEEE80211_C_SHPREAMBLE  0x00008000  /* CAPABILITY: short preamble */
#define IEEE80211_C_MONITOR 0x00010000  /* CAPABILITY: monitor mode */
#define IEEE80211_C_TKIPMIC 0x00020000  /* CAPABILITY: TKIP MIC avail */
#define IEEE80211_C_WPA1    0x00800000  /* CAPABILITY: WPA1 avail */
#define IEEE80211_C_WPA2    0x01000000  /* CAPABILITY: WPA2 avail */
#define IEEE80211_C_WPA     0x01800000  /* CAPABILITY: WPA1+WPA2 avail*/
#define IEEE80211_C_BURST   0x02000000  /* CAPABILITY: frame bursting */
#define IEEE80211_C_WME     0x04000000  /* CAPABILITY: WME avail */
#define IEEE80211_C_WDS     0x08000000  /* CAPABILITY: 4-addr support */
#define IEEE80211_C_WME_TKIPMIC 0x10000000  /* CAPABILITY: TKIP MIC for QoS frame */
#define IEEE80211_C_BGSCAN  0x20000000  /* CAPABILITY: bg scanning */
#define	IEEE80211_C_UAPSD	0x40000000	/* CAPABILITY: UAPSD */
#define IEEE80211_C_FASTCC	0x80000000	/* CAPABILITY: fast channel change */

/* XXX protection/barker? */

#define IEEE80211_C_CRYPTO  0x0000002f  /* CAPABILITY: crypto alg's */

/* Atheros ABOLT definitions */
#define IEEE80211_ABOLT_TURBO_G     0x01    /* Legacy Turbo G */
#define IEEE80211_ABOLT_TURBO_PRIME 0x02    /* Turbo Prime */
#define IEEE80211_ABOLT_COMPRESSION 0x04    /* Compression */
#define IEEE80211_ABOLT_FAST_FRAME  0x08    /* Fast Frames */
#define IEEE80211_ABOLT_BURST       0x10    /* Bursting */
#define IEEE80211_ABOLT_WME_ELE     0x20    /* WME based cwmin/max/burst tuning */
#define IEEE80211_ABOLT_XR      0x40    /* XR */
#define IEEE80211_ABOLT_AR      0x80    /* AR switches out based on adjaced non-turbo traffic */

/* Atheros Advanced Capabilities ABOLT definition */
#define IEEE80211_ABOLT_ADVCAP  (IEEE80211_ABOLT_TURBO_PRIME | \
                 IEEE80211_ABOLT_COMPRESSION | \
                 IEEE80211_ABOLT_FAST_FRAME | \
                 IEEE80211_ABOLT_XR | \
                 IEEE80211_ABOLT_AR | \
                 IEEE80211_ABOLT_BURST | \
                 IEEE80211_ABOLT_WME_ELE)

/* check if a capability was negotiated for use */
#define IEEE80211_ATH_CAP(vap, ni, bit) \
    ((ni)->ni_ath_flags & (vap)->iv_ath_cap & (bit))

/* flags to VAP create function */
#define IEEE80211_VAP_XR        0x10000 /* create a XR VAP without registering net device with OS*/

/* HT flags */
#define IEEE80211_HTF_SHORTGI          0x0001


a_int32_t ieee80211_ifattach(struct ieee80211com *);
void    ieee80211_ifdetach(struct ieee80211com *);
a_int32_t ieee80211_vap_setup(struct ieee80211com *, struct ieee80211vap *,
        const char *name, a_int32_t unit, a_int32_t opmode, a_int32_t flags);
a_status_t ieee80211_vap_attach(struct ieee80211vap *,
        ifm_change_cb_t, ifm_stat_cb_t);
void    ieee80211_vap_detach(struct ieee80211vap *);
#ifndef ATH_STA_ONLY
void    ieee80211_mark_dfs(struct ieee80211com *, struct ieee80211_channel *);
void    ieee80211_dfs_test_return(struct ieee80211com *, a_uint8_t);
#endif
void    ieee80211_announce(struct ieee80211com *);
void    ieee80211_announce_channels(struct ieee80211com *);
a_int32_t ieee80211_media_change(adf_drv_handle_t);
void    ieee80211_media_status(void *, struct intfmediareq *);
a_int32_t ieee80211_rate2media(struct ieee80211com*, a_int32_t, enum ieee80211_phymode);
a_int32_t ieee80211_media2rate(a_int32_t);
a_int32_t ieee80211_htrate2media(struct ieee80211com*, a_int32_t, enum ieee80211_phymode);
a_int32_t ieee80211_htmedia2rate(a_int32_t);
#ifndef ATH_STA_ONLY
a_uint32_t   ieee80211_mhz2ieee(a_uint32_t, a_uint32_t);
#endif
a_uint32_t   ieee80211_chan2ieee(struct ieee80211com *,
        const struct ieee80211_channel *);
a_uint32_t   ieee80211_ieee2mhz(a_uint32_t, a_uint32_t);
struct ieee80211_channel *ieee80211_find_channel(struct ieee80211com *,
        a_int32_t freq, a_uint32_t flags);
a_int32_t ieee80211_setmode(struct ieee80211com *, enum ieee80211_phymode);
void    ieee80211_reset_erp(struct ieee80211com *, enum ieee80211_phymode);
enum ieee80211_phymode ieee80211_chan2mode(const struct ieee80211_channel *);
void    ieee80211_build_countryie(struct ieee80211com *);
a_int32_t     ieee80211_media_setup(struct ieee80211com *, struct ifmedia *,
                a_uint32_t, ifm_change_cb_t, ifm_stat_cb_t);
a_int32_t     ieee80211_ioctl_getstats(adf_drv_handle_t drv_hdl, adf_net_wcmd_devstats_t *arg);

struct ieee80211_channel *
findchannel(struct ieee80211com *ic, int ieee, int mode);
/*
 * Key update synchronization methods.  XXX should not be visible.
 */
static __inline void
ieee80211_key_update_begin(struct ieee80211vap *vap)
{
    vap->iv_key_update_begin(vap);
}
static __inline void
ieee80211_key_update_end(struct ieee80211vap *vap)
{
    vap->iv_key_update_end(vap);
}

/*
 * XXX these need to be here for IEEE80211_F_DATAPAD
 */

/*
 * Return the space occupied by the 802.11 header and any
 * padding required by the driver.  This works for a
 * management or data frame.
 */
static __inline a_int32_t
ieee80211_hdrspace(struct ieee80211com *ic, const void *data)
{
    a_int32_t size = ieee80211_hdrsize(data);
    if (ic->ic_flags & IEEE80211_F_DATAPAD)
        size = asf_roundup(size, sizeof(a_uint32_t));
    return size;
}

/*
 * XXX these need to be here for IEEE80211_F_DATAPAD
 */

/*
 * Return the space occupied by the 802.11 header and any
 * padding required by the driver.  This works for a
 * management or data frame.
 */
static __inline a_int32_t
ieee80211_hdrspace_padding(struct ieee80211com *ic, const void *data)
{
    a_int32_t size = ieee80211_hdrsize_padding(data);
    if (ic->ic_flags & IEEE80211_F_DATAPAD)
        size = asf_roundup(size, sizeof(a_uint32_t));
    return size;
}

/*
 * Like ieee80211_hdrspace, but handles any type of frame.
 */
static __inline a_int32_t
ieee80211_anyhdrspace(struct ieee80211com *ic, const void *data)
{
    a_int32_t size = ieee80211_anyhdrsize(data);
    if (ic->ic_flags & IEEE80211_F_DATAPAD)
        size = asf_roundup(size, sizeof(a_uint32_t));
    return size;
}

#define IEEE80211_MSG_11N   0x80000000  /* 11n mode debug */
#define IEEE80211_MSG_DEBUG 0x40000000  /* IFF_DEBUG equivalent */
#define IEEE80211_MSG_DUMPPKTS  0x20000000  /* IFF_LINK2 equivalant */
#define IEEE80211_MSG_CRYPTO    0x10000000  /* crypto work */
#define IEEE80211_MSG_INPUT 0x08000000  /* input handling */
#define IEEE80211_MSG_XRATE 0x04000000  /* rate set handling */
#define IEEE80211_MSG_ELEMID    0x02000000  /* element id parsing */
#define IEEE80211_MSG_NODE  0x01000000  /* node handling */
#define IEEE80211_MSG_ASSOC 0x00800000  /* association handling */
#define IEEE80211_MSG_AUTH  0x00400000  /* authentication handling */
#define IEEE80211_MSG_SCAN  0x00200000  /* scanning */
#define IEEE80211_MSG_OUTPUT    0x00100000  /* output handling */
#define IEEE80211_MSG_STATE 0x00080000  /* state machine */
#define IEEE80211_MSG_POWER 0x00040000  /* power save handling */
#define IEEE80211_MSG_DOT1X 0x00020000  /* 802.1x authenticator */
#define IEEE80211_MSG_DOT1XSM   0x00010000  /* 802.1x state machine */
#define IEEE80211_MSG_RADIUS    0x00008000  /* 802.1x radius client */
#define IEEE80211_MSG_RADDUMP   0x00004000  /* dump 802.1x radius packets */
#define IEEE80211_MSG_RADKEYS   0x00002000  /* dump 802.1x keys */
#define IEEE80211_MSG_WPA   0x00001000  /* WPA/RSN protocol */
#define IEEE80211_MSG_ACL   0x00000800  /* ACL handling */
#define IEEE80211_MSG_WME   0x00000400  /* WME protocol */
#define IEEE80211_MSG_SUPG  0x00000200  /* SUPERG */
#define IEEE80211_MSG_DOTH  0x00000100  /* 11.h */
#define IEEE80211_MSG_INACT 0x00000080  /* inactivity handling */
#define IEEE80211_MSG_ROAM  0x00000040  /* sta-mode roaming */
#define IEEE80211_MSG_ACTION    0x00000020  /* action management frames */

#define IEEE80211_MSG_ANY   0xffffffff  /* anything */

#ifdef IEEE80211_DEBUG
#define ieee80211_msg(_vap, _m) ((_vap)->iv_debug & (_m))
#define IEEE80211_DPRINTF(_vap, _m, _fmt, ...) do {         \
    if (ieee80211_msg(_vap, _m))                    \
        ieee80211_note(_vap, _fmt, __VA_ARGS__);        \
} while (0)
#define IEEE80211_NOTE(_vap, _m, _ni, _fmt, ...) do {           \
    if (ieee80211_msg(_vap, _m))                    \
        ieee80211_note_mac(_vap, (_ni)->ni_macaddr, _fmt, __VA_ARGS__);\
} while (0)
#define IEEE80211_NOTE_MAC(_vap, _m, _mac, _fmt, ...) do {      \
    if (ieee80211_msg(_vap, _m))                    \
        ieee80211_note_mac(_vap, _mac, _fmt, __VA_ARGS__);  \
} while (0)
#define IEEE80211_NOTE_FRAME(_vap, _m, _wh, _fmt, ...) do {     \
    if (ieee80211_msg(_vap, _m))                    \
        ieee80211_note_frame(_vap, _wh, _fmt, __VA_ARGS__); \
} while (0)
void    ieee80211_note(struct ieee80211vap *vap, const char *fmt, ...);
void    ieee80211_note_mac(struct ieee80211vap *vap,
        const a_uint8_t mac[IEEE80211_ADDR_LEN], const char *fmt, ...);
void    ieee80211_note_frame(struct ieee80211vap *vap,
        const struct ieee80211_frame *wh, const char *fmt, ...);
#define ieee80211_msg_debug(_vap) \
    ieee80211_msg(_vap, IEEE80211_MSG_DEBUG)
#define ieee80211_msg_dumppkts(_vap) \
    ieee80211_msg(_vap, IEEE80211_MSG_DUMPPKTS)
#define ieee80211_msg_input(_vap) \
    ieee80211_msg(_vap, IEEE80211_MSG_INPUT)
#define ieee80211_msg_radius(_vap) \
    ieee80211_msg(_vap, IEEE80211_MSG_RADIUS)
#define ieee80211_msg_dumpradius(_vap) \
    ieee80211_msg(_vap, IEEE80211_MSG_RADDUMP)
#define ieee80211_msg_dumpradkeys(_vap) \
    ieee80211_msg(_vap, IEEE80211_MSG_RADKEYS)
#define ieee80211_msg_scan(_vap) \
    ieee80211_msg(_vap, IEEE80211_MSG_SCAN)
#define ieee80211_msg_assoc(_vap) \
    ieee80211_msg(_vap, IEEE80211_MSG_ASSOC)
#else
#define IEEE80211_DPRINTF(_vap, _m, _fmt, ...)
#define IEEE80211_NOTE(_vap, _m, _ni, _fmt, ...)
#define IEEE80211_NOTE_FRAME(_vap, _m, _wh, _fmt, ...)
#define IEEE80211_NOTE_MAC(_vap, _m, _mac, _fmt, ...)
#define ieee80211_msg_dumppkts(_vap)    0
#define ieee80211_msg(_vap, _m)     0
#endif

#endif /* _NET80211_IEEE80211_VAR_H_ */
