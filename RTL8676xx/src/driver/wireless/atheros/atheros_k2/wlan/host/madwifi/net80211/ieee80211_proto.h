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
 * $FreeBSD: src/sys/net80211/ieee80211_proto.h,v 1.8 2004/12/31 22:42:38 sam Exp $
 */
#ifndef _NET80211_IEEE80211_PROTO_H_
#define _NET80211_IEEE80211_PROTO_H_

/*
 * 802.11 protocol implementation definitions.
 */
#include <adf_net_wcmd.h>

enum ieee80211_state {
	IEEE80211_S_INIT	= 0,	/* default state */
	IEEE80211_S_SCAN	= 1,	/* scanning */
	IEEE80211_S_JOIN	= 2,	/* join */
	IEEE80211_S_AUTH	= 3,	/* try to authenticate */
	IEEE80211_S_ASSOC	= 4,	/* try to assoc */
	IEEE80211_S_RUN		= 5,	/* associated */
};
#define	IEEE80211_S_MAX		(IEEE80211_S_RUN+1)

#define	IEEE80211_SEND_MGMT(_ni,_type,_arg) \
	((*(_ni)->ni_ic->ic_send_mgmt)(_ni, _type, _arg))

struct ieee80211_action_mgt_args {
	a_uint8_t 	category;
	a_uint8_t	action;
	a_uint32_t	arg1;
	a_uint32_t	arg2;
	a_uint32_t	arg3;
};

extern	const char *ieee80211_mgt_subtype_name[];
extern	const char *ieee80211_ctl_subtype_name[];
extern	const char *ieee80211_state_name[IEEE80211_S_MAX];
extern	const char *ieee80211_wme_acnames[];
extern	const char *ieee80211_phymode_name[];

void	ieee80211_proto_attach(struct ieee80211com *);
void	ieee80211_proto_detach(struct ieee80211com *);
void	ieee80211_proto_vattach(struct ieee80211vap *);
void	ieee80211_proto_vdetach(struct ieee80211vap *);

struct ieee80211_node;
a_int32_t	ieee80211_input(struct ieee80211_node *,
		adf_nbuf_t, a_int32_t, a_uint32_t);
a_int32_t	ieee80211_input_all(struct ieee80211com *,
		adf_nbuf_t, a_int32_t, a_uint32_t);
a_int32_t	ieee80211_setup_rates(struct ieee80211_node *,
		const a_uint8_t *, const a_uint8_t *, a_int32_t);
a_int32_t	ieee80211_setup_ht_rates(struct ieee80211_node *, a_uint8_t *,a_int32_t);
void	ieee80211_setup_basic_ht_rates(struct ieee80211_node *,a_uint8_t *);
void	ieee80211_saveie(a_uint8_t **, const a_uint8_t *);
void	ieee80211_savenie(a_uint8_t **, const a_uint8_t *, size_t);
a_int32_t   ieee80211_parse_wmeie(a_uint8_t *, const struct ieee80211_frame *, 
					  struct ieee80211_node *);
void	ieee80211_saveath(struct ieee80211_node *, a_uint8_t *);
void    ieee80211_parse_htcap(struct ieee80211_node *ni, a_uint8_t *ie);
void	ieee80211_parse_htinfo(struct ieee80211_node *ni, a_uint8_t *ie);
void	ieee80211_recv_mgmt(struct ieee80211_node *, adf_nbuf_t,
		a_int32_t, a_int32_t, a_uint32_t);

a_status_t	ieee80211_hardstart(adf_drv_handle_t, adf_nbuf_t);
a_status_t ieee80211_cmd(adf_drv_handle_t drv_hdl, adf_net_cmd_t cmd,adf_net_cmd_data_t *data);
a_status_t	ieee80211_wcmd(adf_drv_handle_t drv_hdl, adf_net_wcmd_type_t cmd, adf_net_wcmd_data_t *data);
a_int32_t		ieee80211_send_nulldata(struct ieee80211_node *);
a_int32_t		ieee80211_send_mgmt(struct ieee80211_node *, a_int32_t, a_int32_t);
a_int32_t		ieee80211_send_probereq(struct ieee80211_node *,
		 a_uint8_t sa[IEEE80211_ADDR_LEN],
		 a_uint8_t da[IEEE80211_ADDR_LEN],
		 a_uint8_t bssid[IEEE80211_ADDR_LEN],
		a_uint8_t *ssid, size_t ssidlen,
		void *optie, size_t optielen);
void	ieee80211_ht_prot(struct ieee80211com *ic);
void	ieee80211_reset_ht(struct ieee80211com *ic);
void	ieee80211_set_shortslottime(struct ieee80211com *, a_int32_t onoff);
a_int32_t	ieee80211_iserp_rateset(struct ieee80211com *,
		struct ieee80211_rateset *);
void	ieee80211_set11gbasicrates(struct ieee80211_rateset *,
		enum ieee80211_phymode);
void	ieee80211_setpuregbasicrates(struct ieee80211_rateset *);
enum ieee80211_phymode ieee80211_get11gbasicrates(struct ieee80211_rateset *);
void ieee80211_send_pspoll(struct ieee80211_node *);
void	ieee80211_check_mic(struct ieee80211_node *, adf_nbuf_t);

#ifndef ATH_STA_ONLY
a_int32_t		ieee80211_send_qosnulldata(struct ieee80211_node *, a_int32_t);
a_status_t ieee80211_encap(struct ieee80211_node *, adf_nbuf_queue_t *, a_int32_t *);
#else
#define ieee80211_send_qosnulldata(_ni, _ac)
#endif

/*
 * Return the size of the 802.11 header for a management or data frame.
 */
static __inline a_int32_t
ieee80211_hdrsize(const void *data)
{
	const struct ieee80211_frame *wh = data;
	a_int32_t size = sizeof(struct ieee80211_frame);

	/* NB: we don't handle control frames */
	adf_os_assert((wh->i_fc[0]&IEEE80211_FC0_TYPE_MASK) != IEEE80211_FC0_TYPE_CTL);
	if ((wh->i_fc[1] & IEEE80211_FC1_DIR_MASK) == IEEE80211_FC1_DIR_DSTODS)
		size += IEEE80211_ADDR_LEN;
	if (IEEE80211_QOS_HAS_SEQ(wh))
		size += sizeof(a_uint16_t);
	return size;
}

/*
 * Return the size of the 802.11 header for a management or data frame.
 */
static __inline a_int32_t
ieee80211_hdrsize_padding(const void *data)
{
	const struct ieee80211_frame *wh = data;
	a_int32_t size = sizeof(struct ieee80211_frame);
	a_int32_t is4addr = (wh->i_fc[1] & IEEE80211_FC1_DIR_MASK) == IEEE80211_FC1_DIR_DSTODS;
	a_int32_t is_qos = IEEE80211_QOS_HAS_SEQ(wh);

	/* NB: we don't handle control frames */
	adf_os_assert((wh->i_fc[0]&IEEE80211_FC0_TYPE_MASK) != IEEE80211_FC0_TYPE_CTL);
	if (is4addr)
		size += IEEE80211_ADDR_LEN;
	if (is_qos)
		size += sizeof(a_uint16_t);
	if (is4addr && is_qos)
		size += sizeof(a_uint16_t);
	return size;
}

/*
 * Like ieee80211_hdrsize, but handles any type of frame.
 */
static __inline a_int32_t
ieee80211_anyhdrsize(const void *data)
{
	const struct ieee80211_frame *wh = data;

	if ((wh->i_fc[0]&IEEE80211_FC0_TYPE_MASK) == IEEE80211_FC0_TYPE_CTL) {
		switch (wh->i_fc[0] & IEEE80211_FC0_SUBTYPE_MASK) {
		case IEEE80211_FC0_SUBTYPE_CTS:
		case IEEE80211_FC0_SUBTYPE_ACK:
			return sizeof(struct ieee80211_frame_ack);
		}
		return sizeof(struct ieee80211_frame_min);
	} else
		return ieee80211_hdrsize(data);
}

/*
 * Template for an in-kernel authenticator.  Authenticators
 * register with the protocol code and are typically loaded
 * as separate modules as needed.
 */
struct ieee80211_authenticator {
	const char *ia_name;		/* printable name */
	a_int32_t	(*ia_attach)(struct ieee80211vap *);
	void	(*ia_detach)(struct ieee80211vap *);
	void	(*ia_node_join)(struct ieee80211_node *);
	void	(*ia_node_leave)(struct ieee80211_node *);
};
void	ieee80211_authenticator_register(a_int32_t type,
		const struct ieee80211_authenticator *);
void	ieee80211_authenticator_unregister(a_int32_t type);
const struct ieee80211_authenticator *ieee80211_authenticator_get(a_int32_t auth);

struct eapolcom;
/*
 * Template for an in-kernel authenticator backend.  Backends
 * register with the protocol code and are typically loaded
 * as separate modules as needed.
 */
struct ieee80211_authenticator_backend {
	const char *iab_name;		/* printable name */
	a_int32_t	(*iab_attach)(struct eapolcom *);
	void	(*iab_detach)(struct eapolcom *);
};
void	ieee80211_authenticator_backend_register(
		const struct ieee80211_authenticator_backend *);
void	ieee80211_authenticator_backend_unregister(
		const struct ieee80211_authenticator_backend *);
const struct ieee80211_authenticator_backend *
	ieee80211_authenticator_backend_get(const char *name);

/*
 * Template for an MAC ACL policy module.  Such modules
 * register with the protocol code and are passed the sender's
 * address of each received frame for validation.
 */
struct ieee80211_aclator {
	const char *iac_name;		/* printable name */
	a_int32_t	(*iac_attach)(struct ieee80211vap *);
	void	(*iac_detach)(struct ieee80211vap *);
	a_int32_t	(*iac_check)(struct ieee80211vap *,
			     a_uint8_t mac[IEEE80211_ADDR_LEN]);
	a_int32_t	(*iac_add)(struct ieee80211vap *,
			   a_uint8_t mac[IEEE80211_ADDR_LEN]);
	a_int32_t	(*iac_remove)(struct ieee80211vap *,
			   a_uint8_t mac[IEEE80211_ADDR_LEN]);
	a_int32_t	(*iac_flush)(struct ieee80211vap *);
    a_int32_t   (*iac_clear_blk_list)(struct ieee80211vap *);
    a_int32_t   (*iac_get_blk_list)(struct ieee80211vap *, u_int8_t *); 
	a_int32_t	(*iac_setpolicy)(struct ieee80211vap *, a_int32_t);
	a_int32_t	(*iac_getpolicy)(struct ieee80211vap *);
};
void	ieee80211_aclator_register(const struct ieee80211_aclator *);
void	ieee80211_aclator_unregister(const struct ieee80211_aclator *);
const struct ieee80211_aclator *ieee80211_aclator_get(const char *name);

/* flags for ieee80211_fix_rate() */
#define	IEEE80211_F_DOSORT	0x00000001	/* sort rate list */
#define	IEEE80211_F_DOFRATE	0x00000002	/* use fixed rate */
#define	IEEE80211_F_DOXSECT	0x00000004	/* intersection of rates  */
#define	IEEE80211_F_DOBRS	0x00000008	/* check for basic rates */

void	ieee80211_sort_rate(struct ieee80211_rateset *);
void    ieee80211_xsect_rate(struct ieee80211_rateset *, 
		struct ieee80211_rateset *, struct ieee80211_rateset *);
a_int32_t     ieee80211_brs_rate_check(struct ieee80211_rateset *, 
		struct ieee80211_rateset *);
a_int32_t     ieee80211_fixed_rate_check(struct ieee80211_node *, 
		struct ieee80211_rateset *);
a_int32_t 	ieee80211_fixed_htrate_check(struct ieee80211_node *, 
		struct ieee80211_rateset *);

struct wmeParams {
	a_uint8_t	wmep_acm;		/* ACM parameter */
	a_uint8_t	wmep_aifsn;		/* AIFSN parameters */
	a_uint8_t	wmep_logcwmin;		/* cwmin in exponential form */
	a_uint8_t	wmep_logcwmax;		/* cwmax in exponential form */
	a_uint16_t	wmep_txopLimit;		/* txopLimit */
	a_uint8_t        wmep_noackPolicy;       /* No-Ack Policy: 0=ack, 1=no-ack */
};

#define IEEE80211_EXPONENT_TO_VALUE(_exp)  (1 << (a_uint32_t)(_exp)) - 1
#define IEEE80211_TXOP_TO_US(_txop)  (a_uint32_t)((_txop) << 5)
#define IEEE80211_US_TO_TXOP(_us)  (a_uint16_t)(((a_uint32_t)(_us)) >> 5)

struct chanAccParams{
	/* XXX: is there any reason to have multiple instances of cap_info_count??? */
	a_uint8_t		cap_info_count;		 	/* ver. of the current param set */
	struct wmeParams	cap_wmeParams[WME_NUM_AC];	/*WME params for each access class */ 
};

struct ieee80211_wme_state {
	a_uint32_t	wme_flags;
#define	WME_F_AGGRMODE	0x00000001	/* STATUS: WME agressive mode */

	a_uint32_t	wme_hipri_traffic;	/* VI/VO frames in beacon interval */
	a_uint32_t	wme_hipri_switch_thresh;/* agressive mode switch thresh */
	a_uint32_t	wme_hipri_switch_hysteresis;/* agressive mode switch hysteresis */

	struct chanAccParams	wme_wmeChanParams;	/* configured WME parameters applied to itself*/
	struct chanAccParams	wme_wmeBssChanParams;	/* configured WME parameters broadcasted to STAs*/
	struct chanAccParams	wme_chanParams;		/* channel parameters applied to itself*/
	struct chanAccParams	wme_bssChanParams;	/* channel parameters broadcasted to STAs*/
	a_uint8_t                wme_nonAggressiveMode;   /* don't use aggressive params and use WME params */

	/* update hardware tx params after wme state change */
	a_int32_t	(*wme_update)(struct ieee80211com *);
};

void	ieee80211_wme_initparams(struct ieee80211vap *);
void	ieee80211_wme_initparams_locked(struct ieee80211vap *);
void	ieee80211_wme_updateparams(struct ieee80211vap *);
void	ieee80211_wme_updateparams_locked(struct ieee80211vap *);

a_status_t ieee80211_open(adf_drv_handle_t);
a_status_t ieee80211_init(adf_drv_handle_t, a_int32_t force);
void	ieee80211_start_running(struct ieee80211com *);
void   ieee80211_stop(adf_drv_handle_t);
void	ieee80211_stop_running(struct ieee80211com *);
void	ieee80211_beacon_miss(struct ieee80211com *);
#ifdef ATH_SUPERG_DYNTURBO
void	ieee80211_dturbo_switch(struct ieee80211com *, a_int32_t newflags);
#endif
a_int32_t ieee80211_new_state(struct ieee80211vap *,enum ieee80211_state, a_int32_t);
void	ieee80211_print_essid(const a_uint8_t *, a_int32_t);
void	ieee80211_dump_pkt(struct ieee80211com *,
		adf_nbuf_t nbuf, a_int32_t, a_int32_t);
adf_nbuf_t ieee80211_getcfframe(struct ieee80211vap *vap, a_int32_t type);

/*
 * Beacon frames constructed by ieee80211_beacon_alloc
 * have the following structure filled in so drivers
 * can update the frame later w/ minimal overhead.
 */
struct ieee80211_beacon_offsets {
	a_uint16_t	*bo_caps;		/* capabilities */
	a_uint8_t	*bo_tim;		/* start of atim/dtim */
	a_uint8_t	*bo_wme;		/* start of WME parameters */
	a_uint8_t	*bo_tim_trailer;	/* start of fixed-size tim trailer */
	a_uint16_t	bo_tim_len;		/* atim/dtim length in bytes */
	a_uint16_t	bo_tim_trailerlen;	/* trailer length in bytes */
	a_uint8_t	*bo_chanswitch;		/* where channel switch IE will go */
	a_uint8_t	*bo_extchanswitch;	/* where extended channel switch IE will go */
	a_uint8_t	*bo_ath_caps;		/* where ath caps is */
	a_uint8_t	*bo_xr;			/* start of xr element */
	a_uint8_t	*bo_erp;		/* start of ERP element */
    a_uint8_t    *bo_appie_buf;  /* start of APP IE buf */
    a_uint16_t   bo_appie_buf_len;
	a_uint8_t	*bo_wsc;		/* start of WSC element */
	a_uint8_t	*bo_htinfo;		/* start of HT Info element */
	a_uint16_t	bo_chanswitch_trailerlen;
	a_uint16_t	bo_extchanswitch_trailerlen;
};

#ifndef ATH_STA_ONLY
adf_nbuf_t ieee80211_beacon_alloc(struct ieee80211_node *,
		struct ieee80211_beacon_offsets *);
a_int32_t	ieee80211_beacon_update(struct ieee80211_node *,
		struct ieee80211_beacon_offsets *, adf_nbuf_t, a_int32_t mcast);
#else
#define ieee80211_beacon_alloc(_ni, _bo)
#define	ieee80211_beacon_update(_ni, _bo, _nbuf, _mcast)
#endif /* ATH_STA_ONLY */

/* XXX exposed 'cuz of beacon code botch */
a_uint8_t *ieee80211_add_rates(a_uint8_t *, struct ieee80211_rateset *);
a_uint8_t *ieee80211_add_xrates(a_uint8_t *, struct ieee80211_rateset *);
a_uint8_t *ieee80211_add_wpa(a_uint8_t *, struct ieee80211vap *);
a_uint8_t *ieee80211_add_erp(a_uint8_t *, struct ieee80211com *);
a_uint8_t *ieee80211_add_athAdvCap(a_uint8_t *, a_uint8_t, a_uint16_t);
a_uint8_t *ieee80211_add_athextcap(a_uint8_t *, a_uint32_t);
a_uint8_t *ieee80211_add_xr_param(a_uint8_t *, struct ieee80211vap *);
a_uint8_t *ieee80211_add_wme(a_uint8_t *, struct ieee80211_node *);
a_uint8_t *ieee80211_add_wme_param(a_uint8_t *, struct ieee80211_wme_state *,
								  a_int32_t uapsd_enable);
a_uint8_t *ieee80211_add_country(a_uint8_t *, struct ieee80211com *);
a_uint8_t *ieee80211_add_htcap(a_uint8_t *, struct ieee80211_node *);
a_uint8_t *ieee80211_add_htinfo(a_uint8_t *, struct ieee80211_node *);

a_uint8_t *ieee80211_add_wsc(a_uint8_t *frm, a_uint8_t);
/*
 * Notification methods called from the 802.11 state machine.
 * Note that while these are defined here, their implementation
 * is OS-specific.
 */
void	ieee80211_notify_node_join(struct ieee80211_node *, a_int32_t newassoc);
void	ieee80211_notify_node_leave(struct ieee80211_node *, a_int32_t);
void	ieee80211_notify_scan_done(struct ieee80211vap *);
void	ieee80211_notify_push_button(struct ieee80211vap *);

#endif /* _NET80211_IEEE80211_PROTO_H_ */
