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
 * $FreeBSD: src/sys/net80211/ieee80211_node.h,v 1.12 2004/12/31 22:42:38 sam Exp $
 */
#ifndef _NET80211_IEEE80211_NODE_H_
#define _NET80211_IEEE80211_NODE_H_

#include <ieee80211_ioctl.h>		/* for ieee80211_nodestats */
#include <ieee80211_proto.h>		/* for proto macros on node */
//#include <sys/queue.h>

/*
 * Each ieee80211com instance has a single timer that fires once a
 * second.  This is used to initiate various work depending on the
 * state of the instance: scanning (passive or active), ``transition''
 * (waiting for a response to a management frame when operating
 * as a station), and node inactivity processing (when operating
 * as an AP).  For inactivity processing each node has a timeout
 * set in it's ni_inact field that is decremented on each timeout
 * and the node is reclaimed when the counter goes to zero.  We
 * use different inactivity timeout values depending on whether
 * the node is associated and authorized (either by 802.1x or
 * open/shared key authentication) or associated but yet to be
 * authorized.  The latter timeout is shorter to more aggressively
 * reclaim nodes that leave part way through the 802.1x exchange.
 */
#define	IEEE80211_INACT_WAIT	15		/* inactivity interval (secs) */
#define	IEEE80211_INACT_INIT	(30/IEEE80211_INACT_WAIT)	/* initial */
#define	IEEE80211_INACT_AUTH	(180/IEEE80211_INACT_WAIT)	/* associated but not authorized */
#define	IEEE80211_INACT_RUN	(300/IEEE80211_INACT_WAIT)	/* authorized */
#define	IEEE80211_INACT_PROBE	(30/IEEE80211_INACT_WAIT)	/* probe */
#define	IEEE80211_INACT_SCAN	(300/IEEE80211_INACT_WAIT)	/* scanned */
#define IEEE80211_INACT_NONERP  10             /* Aging period for non-erp bss (Secs) */
#define IEEE80211_INACT_HT	10	       /* # sec's to age out HT protection */

#define	IEEE80211_TRANS_WAIT 	5		/* mgt frame tx timer (secs) */

#define	IEEE80211_NODE_HASHSIZE	32
/* simple hash is enough for variation of macaddr */
#define	IEEE80211_NODE_HASH(addr)	\
	(((const a_uint8_t *)(addr))[IEEE80211_ADDR_LEN - 1] % \
		IEEE80211_NODE_HASHSIZE)

struct ieee80211_rsnparms {
	a_uint8_t	rsn_mcastcipher;	/* mcast/group cipher */
	a_uint8_t	rsn_mcastkeylen;	/* mcast key length */
	a_uint8_t	rsn_ucastcipherset;	/* unicast cipher set */
	a_uint8_t	rsn_ucastcipher;	/* selected unicast cipher */
	a_uint8_t	rsn_ucastkeylen;	/* unicast key length */
	a_uint8_t	rsn_keymgmtset;		/* key mangement algorithms */
	a_uint8_t	rsn_keymgmt;		/* selected key mgmt algo */
	a_uint16_t	rsn_caps;		/* capabilities */
};

struct ieee80211_node_table;
struct ieee80211com;
struct ieee80211vap;

/*
 * Node specific information.  Note that drivers are expected
 * to derive from this structure to add device-specific per-node
 * state.  This is done by overriding the ic_node_* methods in
 * the ieee80211com structure.
 */
//todo TARGET RELATED STUFF TO BE MOVED TO A COMMON FILE 

struct ieee80211_key_target {
    a_int32_t dummy ;
};


struct ieee80211_node {
	struct ieee80211vap	*ni_vap;
	struct ieee80211com	*ni_ic;
	struct ieee80211_node_table *ni_table;
	asf_tailq_entry(ieee80211_node)	ni_list;
	asf_list_entry(ieee80211_node)	ni_hash;
	adf_os_atomic_t		ni_refcnt;
	a_int32_t		ni_scangen;	/* gen# for timeout scan */
	a_uint8_t		ni_authmode;	/* authentication algorithm */
	a_uint16_t		ni_flags;	/* special-purpose state */
#define	IEEE80211_NODE_AUTH	0x0001		/* authorized for data */
#define	IEEE80211_NODE_QOS	0x0002		/* QoS enabled */
#define	IEEE80211_NODE_ERP	0x0004		/* ERP enabled */
#define	IEEE80211_NODE_HT	0x0008		/* HT enabled */

/* NB: this must have the same value as IEEE80211_FC1_PWR_MGT */
#define	IEEE80211_NODE_PWR_MGT	0x0010		/* power save mode enabled */
#define	IEEE80211_NODE_AREF	0x0020		/* authentication ref held */
#define IEEE80211_NODE_UAPSD	0x0040		/* U-APSD power save enabled */
#define IEEE80211_NODE_UAPSD_TRIG 0x0080	/* U-APSD triggerable state */
#define IEEE80211_NODE_UAPSD_SP	0x0100		/* U-APSD SP in progress */
#define	IEEE80211_NODE_ATH	0x0200		/* Atheros Owl or follow-on device */
#define	IEEE80211_NODE_OWL_WORKAROUND	0x0400	/* Owl WDS workaround needed*/
#define	IEEE80211_NODE_WDS	0x0800		/* WDS link */
	a_uint8_t		ni_ath_flags;	/* Atheros feature flags */
	/* NB: These must have the same values as IEEE80211_ATHC_* */
#define IEEE80211_NODE_TURBOP	0x0001		/* Turbo prime enable */
#define IEEE80211_NODE_COMP	0x0002		/* Compresssion enable */
#define IEEE80211_NODE_FF	0x0004          /* Fast Frame capable */
#define IEEE80211_NODE_XR	0x0008		/* Atheros WME enable */
#define IEEE80211_NODE_AR	0x0010		/* AR capable */
#define IEEE80211_NODE_BOOST	0x0080 
#define IEEE80211_NODE_PS_CHANGED	0x0200	/* PS state change */ 
	a_uint16_t               ni_ath_defkeyindex; /* Atheros def key index */
#define IEEE80211_INVAL_DEFKEY	0x7FFF
    a_uint16_t		ni_associd;	/* assoc response */
	a_uint16_t		ni_txpower;	/* current transmit power */
	a_uint16_t		ni_vlan;	/* vlan tag */
	a_uint8_t		*ni_challenge;	/* shared-key challenge */
	a_uint8_t		*ni_wpa_ie;	/* captured WPA ie */
	a_uint8_t		*ni_rsn_ie;	/* captured RSN ie */
	a_uint8_t		*ni_wme_ie;	/* captured WME ie */
	a_uint8_t		*ni_ath_ie;	/* captured Atheros ie */
	a_uint8_t		*ni_wsc_ie;	/* captured WSC ie */
	a_uint16_t		ni_txseqs[17];	/* tx seq per-tid */
	a_uint16_t		ni_rxseqs[17];	/* rx seq previous per-tid*/
	adf_time_t		ni_rxfragstamp;	/* time stamp of last rx frag */
	adf_nbuf_t		ni_rxfrag[3];	/* rx frag reassembly */
	struct ieee80211_rsnparms ni_rsn;	/* RSN/WPA parameters */
	struct ieee80211_key	ni_ucastkey;	/* unicast key */
	a_int16_t			ni_rxkeyoff;    /* Receive key offset */

	/* 11n Capabilities */
	a_uint16_t 		ni_htcap;	/* HT capabilities */
	a_uint16_t		ni_maxampdu;	/* maximum rx A-MPDU length */
	a_uint8_t		ni_mpdudensity;	/* MPDU density */

	/* 11n information */
    enum ieee80211_cwm_width  ni_chwidth;	/* recommended tx channel width */
    a_uint8_t       ni_extoffset;
	a_uint8_t		ni_newchwidth; 	/* channel width changed */
	a_uint8_t		ni_updaterates; /* update rate table on SM power save */

	/* hardware */
	a_uint32_t		ni_rstamp;	/* recv timestamp */
	a_int8_t		ni_rssi;	/* recv ssi */

	/* header */
	a_uint8_t		ni_macaddr[IEEE80211_ADDR_LEN];
	a_uint8_t		ni_bssid[IEEE80211_ADDR_LEN];

	/* beacon, probe response */
	union {
		a_uint8_t	data[8];
		a_uint64_t	tsf;
	} ni_tstamp;				/* from last rcv'd beacon */
	a_uint16_t			ni_intval;	/* beacon interval */
	a_uint16_t			ni_capinfo;	/* capabilities */
	a_uint8_t			ni_esslen;
	a_uint8_t			ni_essid[IEEE80211_NWID_LEN];
	struct ieee80211_rateset	ni_rates;	/* negotiated rate set */
	struct ieee80211_rateset	ni_htrates;	/* negotiated ht rate set */
	struct ieee80211_channel       *ni_chan;
	a_uint16_t			ni_fhdwell;	/* FH only */
	a_uint8_t			ni_fhindex;	/* FH only */
	a_uint8_t			ni_erp;		/* ERP from beacon/probe resp */
	a_uint16_t			ni_timoff;	/* byte offset to TIM ie */

	/* others */
	adf_nbuf_queue_t		ni_savedq;	/* packets queued for pspoll */
	adf_os_spinlock_t		ni_savedq_lock;	/* saved queue lock */
	a_int16_t				ni_inact;	/* inactivity mark count */
	a_int16_t				ni_inact_reload;/* inactivity reload value */
	a_int32_t				ni_txrate;	/* index to ni_rates[] */
	a_uint32_t			ni_txrateKbps;
	struct ieee80211_nodestats	ni_stats;	/* per-node statistics */
	struct ieee80211vap	       *ni_prev_vap;  /* previously associated vap */
	a_uint8_t			ni_uapsd;	/* U-APSD per-node flags matching WMM STA Qos Info field */
	a_uint8_t			ni_uapsd_maxsp; /* maxsp from flags above */
	a_uint16_t			ni_uapsd_trigseq[WME_NUM_AC]; /* trigger suppression on retry */
	a_uint16_t			ni_pschangeseq;
    adf_time_t			ni_assoctime;   /* sta association time */
    a_uint8_t           ni_nodeindex;
    a_uint8_t       ni_is_vapnode;
};


#define	IEEE80211_NODE_AID(ni)	IEEE80211_AID(ni->ni_associd)

#define	IEEE80211_NODE_STAT(ni,stat)	(ni->ni_stats.ns_##stat++)
#define	IEEE80211_NODE_STAT_ADD(ni,stat,v)	(ni->ni_stats.ns_##stat += v)
#define	IEEE80211_NODE_STAT_SET(ni,stat,v)	(ni->ni_stats.ns_##stat = v)

#define WME_UAPSD_AC_CAN_TRIGGER(_ac, _ni) ( \
		((ni)->ni_flags & IEEE80211_NODE_UAPSD_TRIG) && WME_UAPSD_AC_ENABLED((_ac), (_ni)->ni_uapsd) )
#define WME_UAPSD_NODE_MAXQDEPTH	8
#define IEEE80211_NODE_UAPSD_USETIM(_ni) (((_ni)->ni_uapsd & 0xF) == 0xF )
#define WME_UAPSD_NODE_INVALIDSEQ	0xffff
#define WME_UAPSD_NODE_TRIGSEQINIT(_ni)	(adf_os_mem_set(&(_ni)->ni_uapsd_trigseq[0], 0xff, sizeof((_ni)->ni_uapsd_trigseq)))

static __inline struct ieee80211_node *
ieee80211_ref_node(struct ieee80211_node *ni)
{
	ieee80211_node_incref(ni);
	return ni;
}

static __inline void
ieee80211_unref_node(struct ieee80211_node **ni)
{
	ieee80211_node_decref(*ni);
	*ni = NULL;			/* guard against use */
}

void	ieee80211_node_attach(struct ieee80211com *);
void	ieee80211_node_detach(struct ieee80211com *);
void	ieee80211_node_vattach(struct ieee80211vap *);
void	ieee80211_node_latevattach(struct ieee80211vap *);
void	ieee80211_node_vdetach(struct ieee80211vap *);

static __inline a_int32_t
ieee80211_node_is_authorized(const struct ieee80211_node *ni)
{
	return (ni->ni_flags & IEEE80211_NODE_AUTH);
}

void	ieee80211_node_authorize(struct ieee80211_node *);
void	ieee80211_node_unauthorize(struct ieee80211_node *);

void	ieee80211_create_ibss(struct ieee80211vap*, struct ieee80211_channel *, a_uint8_t *);
void	ieee80211_reset_bss(struct ieee80211vap *);
a_int32_t	ieee80211_ibss_merge(struct ieee80211_node *);
struct ieee80211_scan_entry;
a_int32_t	ieee80211_sta_join(struct ieee80211vap *,
		struct ieee80211_scan_entry *);
void	ieee80211_sta_leave(struct ieee80211_node *, a_int32_t);

#define WDS_AGING_TIME  600   /* 10 minutes */ 
#define WDS_AGING_COUNT 2 
#define WDS_AGING_TIMER_VAL (WDS_AGING_TIME/2)

struct ieee80211_wds_addr {
	asf_list_entry(ieee80211_wds_addr)	wds_hash;
	a_uint8_t	wds_macaddr[IEEE80211_ADDR_LEN];
	struct ieee80211_node	*wds_ni;
#define IEEE80211_NODE_F_WDS_BEHIND   0x00001
#define IEEE80211_NODE_F_WDS_REMOTE   0x00002
#define IEEE80211_NODE_F_WDS_START    0x00010 /* Set for first time, when node
                                                 is added (for TDLS)*/
	u_int32_t   flags;
	a_uint16_t   wds_agingcount;
};
	

/*
 * Table of ieee80211_node instances.  Each ieee80211com
 * has at least one for holding the scan candidates.
 * When operating as an access point or in ibss mode there
 * is a second table for associated stations or neighbors.
 */
struct ieee80211_node_table {
	struct ieee80211com	               *nt_ic;		/* back reference */
	ieee80211_node_lock_t			nt_nodelock;	/* on node table */
	asf_tailq_head(ni_list, ieee80211_node)		nt_node;	/* information of all nodes */
	asf_list_head(ni_nash, ieee80211_node)		nt_hash[IEEE80211_NODE_HASHSIZE];
	asf_list_head(ni_wds_hash, ieee80211_wds_addr)	nt_wds_hash[IEEE80211_NODE_HASHSIZE];
	const char		               *nt_name;	/* for debugging */
	ieee80211_scan_lock_t			nt_scanlock;	/* on nt_scangen */
	a_uint32_t		                nt_scangen;	/* gen# for timeout scan */
	a_int32_t		                nt_inact_init;	/* initial node inact setting */
	adf_os_timer_t				nt_wds_aging_timer;	/* timer to age out wds entries */
};

struct ieee80211_node *ieee80211_alloc_node(struct ieee80211_node_table *,
		struct ieee80211vap *, a_uint8_t *);
struct ieee80211_node *ieee80211_tmp_node(struct ieee80211vap *,
		 a_uint8_t *);
struct ieee80211_node *ieee80211_dup_bss(struct ieee80211vap *,
		a_uint8_t *);
void ieee80211_node_reset(struct ieee80211_node *ni,
		 struct ieee80211vap *vap);

void ieee80211_free_node_forced(struct ieee80211_node *);

#ifdef IEEE80211_DEBUG_REFCNT
void	ieee80211_free_node_debug(struct ieee80211_node *,
		const char *func, a_int32_t line);
struct ieee80211_node *ieee80211_find_node_debug(
		struct ieee80211_node_table *, const a_uint8_t *,
		const char *func, a_int32_t line);
struct ieee80211_node * ieee80211_find_rxnode_debug(
		struct ieee80211com *, const struct ieee80211_frame_min *,
		const char *func, a_int32_t line);
struct ieee80211_node *ieee80211_find_txnode_debug(
		struct ieee80211vap *, const a_uint8_t *,
		const char *func, a_int32_t line);
#define	ieee80211_free_node(ni) \
	ieee80211_free_node_debug(ni, __func__, __LINE__)
#define	ieee80211_find_node(nt, mac) \
	ieee80211_find_node_debug(nt, mac, __func__, __LINE__)
#define	ieee80211_find_rxnode(nt, wh) \
	ieee80211_find_rxnode_debug(nt, wh, __func__, __LINE__)
#define	ieee80211_find_txnode(nt, mac) \
	ieee80211_find_txnode_debug(nt, mac, __func__, __LINE__)
#else
void	ieee80211_free_node(struct ieee80211_node *);
#ifndef ATH_STA_ONLY
a_int32_t 	ieee80211_node_removeall_wds(
		struct ieee80211_node_table *, struct ieee80211_node *);
#endif /* !ATH_STA_ONLY */
struct ieee80211_node *ieee80211_find_node(
		struct ieee80211_node_table *, a_uint8_t *);
struct ieee80211_node * ieee80211_find_rxnode(
		struct ieee80211com *, struct ieee80211_frame_min *);
struct ieee80211_node *ieee80211_find_txnode(
		struct ieee80211vap *,  a_uint8_t *);
#endif
#ifndef ATH_STA_ONLY
struct ieee80211_node *ieee80211_find_wds_node(struct ieee80211_node_table *,
					        a_uint8_t *);
struct ieee80211_node * ieee80211_is_wds_local_node(
			struct ieee80211_node_table *nt,a_uint8_t *macaddr);
a_int32_t	ieee80211_add_wds_addr(struct ieee80211_node_table *,
			       struct ieee80211_node *, a_uint8_t *, u_int32_t);
void	ieee80211_remove_wds_addr(struct ieee80211_node_table *,
							   a_uint8_t *);
void	ieee80211_del_wds_node(struct ieee80211_node_table *,
			       struct ieee80211_node *);

void	ieee80211_node_join(struct ieee80211_node *, a_int32_t);
#else /* !ATH_STA_ONLY */
#define ieee80211_find_wds_node(_nt , _macaddr)             (NULL)
#define ieee80211_node_removeall_wds(_nt, _ni)              
#define	ieee80211_add_wds_addr(_nt , _ni, _macaddr, _flags)         
#define	ieee80211_remove_wds_addr(_nt, _macaddr)
#define ieee80211_del_wds_node(_nt, _ni)
#define	ieee80211_node_join(_ni, _resp)
#endif /* !ATH_STA_ONLY */

typedef void ieee80211_iter_func(void *, struct ieee80211_node *);
void	ieee80211_iterate_nodes(struct ieee80211_node_table *,
		ieee80211_iter_func *, void *);

void	ieee80211_dump_node(struct ieee80211_node_table *,
		struct ieee80211_node *);
void	ieee80211_dump_nodes(struct ieee80211_node_table *);

struct ieee80211_node *ieee80211_fakeup_adhoc_node(struct ieee80211vap *vap,
		 a_uint8_t macaddr[]);
struct ieee80211_scanparams;
struct ieee80211_node *ieee80211_add_neighbor(struct ieee80211vap *,
		struct ieee80211_frame *,
		const struct ieee80211_scanparams *);
void	ieee80211_node_leave(struct ieee80211_node *);
a_uint8_t ieee80211_getrssi(struct ieee80211com *);
#endif /* _NET80211_IEEE80211_NODE_H_ */
