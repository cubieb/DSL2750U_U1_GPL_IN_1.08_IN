/*-
 * Copyright (c) 2005 Sam Leffler, Errno Consulting
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
#ifndef _NET80211_IEEE80211_SCAN_H_
#define _NET80211_IEEE80211_SCAN_H_

#define	IEEE80211_SCAN_MAX	IEEE80211_CHAN_MAX

struct ieee80211_scanner;

struct ieee80211_scan_ssid {
	a_int32_t	 len;				/* length in bytes */
	a_uint8_t ssid[IEEE80211_NWID_LEN];	/* ssid contents */
};
#define	IEEE80211_SCAN_MAX_SSID	1

struct ieee80211_scan_state {
	struct ieee80211vap *ss_vap;
	const struct ieee80211_scanner *ss_ops;	/* policy hookup, see below */
	void		*ss_priv;		/* scanner private state */
	a_uint16_t	ss_flags;
#define	IEEE80211_SCAN_NOPICK	0x0001		/* scan only, no selection */
#define	IEEE80211_SCAN_ACTIVE	0x0002		/* active scan (probe req) */
#define	IEEE80211_SCAN_PICK1ST	0x0004		/* ``hey sailor'' mode */
#define	IEEE80211_SCAN_BGSCAN	0x0008		/* bg scan, exit ps at end */
#define	IEEE80211_SCAN_ONCE	0x0010		/* do one complete pass */
#define	IEEE80211_SCAN_GOTPICK	0x1000		/* got candidate, can stop */
#define	IEEE80211_SCAN_USECACHE	0x2000		/* Must use a result from the cache */
	a_uint8_t	ss_nssid;		/* # ssid's to probe/match */
	struct ieee80211_scan_ssid ss_ssid[IEEE80211_SCAN_MAX_SSID];
						/* ssid's to probe/match */
						/* ordered channel set */
	struct ieee80211_channel *ss_chans[IEEE80211_SCAN_MAX];
	a_uint16_t	ss_next;		/* ix of next chan to scan */
	a_uint16_t	ss_last;		/* ix+1 of last chan to scan */
	adf_time_t	ss_mindwell;		/* min dwell on channel */
	adf_time_t	ss_maxdwell;		/* max dwell on channel */
};

/*
 * The upper 16 bits of the flags word is used to communicate
 * information to the scanning code that is NOT recorded in
 * ss_flags.  It might be better to split this stuff out into
 * a separate variable to avoid confusion.
 */
#define	IEEE80211_SCAN_FLUSH	0x10000		/* flush candidate table */
#define	IEEE80211_SCAN_NOSSID	0x20000		/* don't update ssid list */

struct ieee80211com;
void	ieee80211_scan_attach(struct ieee80211com *);
void	ieee80211_scan_detach(struct ieee80211com *);
void	ieee80211_scan_vattach(struct ieee80211vap *);
void	ieee80211_scan_vdetach(struct ieee80211vap *);

void	ieee80211_scan_dump_channels(const struct ieee80211_scan_state *);

#define	IEEE80211_SCAN_FOREVER	0x7fffffff
a_int32_t	ieee80211_start_scan(struct ieee80211vap *, a_int32_t flags, adf_time_t duration,
		a_uint32_t nssid, const struct ieee80211_scan_ssid ssids[]);
a_int32_t	ieee80211_check_scan(struct ieee80211vap *, a_int32_t flags, adf_time_t duration,
			     a_uint32_t nssid, const struct ieee80211_scan_ssid ssids[],
			     a_int32_t (*action)(struct ieee80211vap *, struct ieee80211_scan_entry *));
a_int32_t	ieee80211_bg_scan(struct ieee80211vap *);
void	ieee80211_cancel_scan(struct ieee80211vap *);

#ifndef ATH_STA_ONLY
a_int32_t	ieee80211_scan_dfs_action(struct ieee80211vap *, struct ieee80211_scan_entry *);
#endif

struct ieee80211_scanparams;
void	ieee80211_add_scan(struct ieee80211vap *,
		 struct ieee80211_scanparams *,
		 struct ieee80211_frame *,
		a_int32_t subtype, a_int32_t rssi, a_int32_t rstamp);
void	ieee80211_scan_timeout(struct ieee80211com *);

void	ieee80211_scan_assoc_success(struct ieee80211com *,
		a_uint8_t mac[IEEE80211_ADDR_LEN]);
enum {
	IEEE80211_SCAN_FAIL_TIMEOUT	= 1,	/* no response to mgmt frame */
	IEEE80211_SCAN_FAIL_STATUS	= 2	/* negative response to " " */
};
void	ieee80211_scan_assoc_fail(struct ieee80211com *,
		a_uint8_t mac[IEEE80211_ADDR_LEN], a_int32_t reason);
void	ieee80211_scan_flush(struct ieee80211com *);

struct ieee80211_scan_entry;
typedef a_int32_t ieee80211_scan_iter_func(void *,
		struct ieee80211_scan_entry *);
a_int32_t	ieee80211_scan_iterate(struct ieee80211com *,
		ieee80211_scan_iter_func, void *);

/*
 * Parameters supplied when adding/updating an entry in a
 * scan cache.  Pointer variables should be set to NULL
 * if no data is available.  Pointer references can be to
 * local data; any information that is saved will be copied.
 * All multi-byte values must be in host byte order.
 */
struct ieee80211_scanparams {
	a_uint16_t	capinfo;	/* 802.11 capabilities */
	a_uint16_t	fhdwell;	/* FHSS dwell interval */
	a_uint8_t	chan;		/* */
	a_uint8_t	bchan;
	a_uint8_t	fhindex;
	a_uint8_t	erp;
	a_uint16_t	bintval;
	a_uint8_t	timoff;
	a_uint8_t	*tim;
	a_uint8_t	*tstamp;
	a_uint8_t	*country;
	a_uint8_t	*ssid;
	a_uint8_t	*rates;
	a_uint8_t	*xrates;
	a_uint8_t	*doth;
    a_uint8_t    *doth_pwrcnstr;
	a_uint8_t	*wpa;
	a_uint8_t	*wps;
	a_uint8_t	*rsn;
	a_uint8_t	*wme;
	a_uint8_t	*htcap;
	a_uint8_t	*htinfo;
	a_uint8_t	*ath;
	a_uint8_t	*athextcap;
};

/*
 * Scan cache entry format used when exporting data from a policy
 * module; this data may be represented some other way internally.
 */
struct ieee80211_scan_entry {
	a_uint8_t	se_macaddr[IEEE80211_ADDR_LEN];
	a_uint8_t	se_bssid[IEEE80211_ADDR_LEN];
	a_uint8_t	se_ssid[2+IEEE80211_NWID_LEN];
	a_uint8_t	se_rates[2+IEEE80211_RATE_MAXSIZE];
	a_uint8_t	se_xrates[2+IEEE80211_RATE_MAXSIZE];
    a_uint8_t    se_doth_pwrcnstr[3];
	a_uint32_t	se_rstamp;	/* recv timestamp */
	union {
		a_uint8_t	data[8];
		a_uint64_t	tsf;
	} se_tstamp;			/* from last rcv'd beacon */
	a_uint16_t	se_intval;	/* beacon interval (host byte order) */
	a_uint16_t	se_capinfo;	/* capabilities (host byte order) */
	struct ieee80211_channel *se_chan;/* channel where sta found */
	a_uint16_t	se_timoff;	/* byte offset to TIM ie */
	a_uint16_t	se_fhdwell;	/* FH only (host byte order) */
	a_uint8_t	se_fhindex;	/* FH only */
	a_uint8_t	se_erp;		/* ERP from beacon/probe resp */
	a_int8_t	se_rssi;	/* avg'd recv ssi */
	a_uint8_t	se_dtimperiod;	/* DTIM period */
	a_uint8_t	*se_wpa_ie;	/* captured WPA ie */
	a_uint8_t	*se_wps_ie;	/* captured WPS ie */
	a_uint8_t	*se_rsn_ie;	/* captured RSN ie */
	a_uint8_t	*se_wme_ie;	/* captured WME ie */
	a_uint8_t	*se_htcap_ie;	/* captured HT cap ie */
	a_uint8_t	*se_htinfo_ie;  /* captured HT info ie */
	a_uint8_t	*se_ath_ie;	/* captured Atheros ie */
    a_uint8_t    *se_country_ie; /* country IE */
	adf_time_t	se_age;		/* age of entry (0 on create) */
};

/*
 * Template for an in-kernel scan policy module.
 * Modules register with the scanning code and are
 * typically loaded as needed.
 */
struct ieee80211_scanner {
	const char *scan_name;		/* printable name */
	a_int32_t	(*scan_attach)(struct ieee80211_scan_state *);
	a_int32_t	(*scan_detach)(struct ieee80211_scan_state *);
	a_int32_t	(*scan_start)(struct ieee80211_scan_state *,
			struct ieee80211vap *);
	a_int32_t	(*scan_restart)(struct ieee80211_scan_state *,
			struct ieee80211vap *);
	a_int32_t	(*scan_cancel)(struct ieee80211_scan_state *,
			struct ieee80211vap *);
	a_int32_t	(*scan_end)(struct ieee80211_scan_state *,
			struct ieee80211vap *,
			a_int32_t (*action)(struct ieee80211vap *,
				      struct ieee80211_scan_entry *));
	a_int32_t	(*scan_flush)(struct ieee80211_scan_state *);
	/* add an entry to the cache */
	a_int32_t	(*scan_add)(struct ieee80211_scan_state *,
			    struct ieee80211_scanparams *,
			    struct ieee80211_frame *,
			a_int32_t subtype, a_int32_t rssi, a_int32_t rstamp);
	/* age and/or purge entries in the cache */
	void	(*scan_age)(struct ieee80211_scan_state *);
	/* note that association failed for an entry */
	void	(*scan_assoc_fail)(struct ieee80211_scan_state *,
			a_uint8_t macaddr[IEEE80211_ADDR_LEN],
			a_int32_t reason);
	/* note that association succeed for an entry */
	void	(*scan_assoc_success)(struct ieee80211_scan_state *,
			a_uint8_t macaddr[IEEE80211_ADDR_LEN]);
	/* iterate over entries in the scan cache */
	a_int32_t	(*scan_iterate)(struct ieee80211_scan_state *,
			ieee80211_scan_iter_func *, void *);
	/* default action to take when found scan match */
	a_int32_t	(*scan_default)(struct ieee80211vap *,
				struct ieee80211_scan_entry *);
};
void	ieee80211_scanner_register(enum ieee80211_opmode,
		const struct ieee80211_scanner *);
void	ieee80211_scanner_unregister(enum ieee80211_opmode,
		const struct ieee80211_scanner *);
void	ieee80211_scanner_unregister_all(const struct ieee80211_scanner *);
const struct ieee80211_scanner *ieee80211_scanner_get(enum ieee80211_opmode);
#endif /* _NET80211_IEEE80211_SCAN_H_ */

