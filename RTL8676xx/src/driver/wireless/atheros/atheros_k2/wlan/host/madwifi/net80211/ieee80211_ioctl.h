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
 * $FreeBSD: src/sys/net80211/ieee80211_ioctl.h,v 1.8 2004/12/31 22:42:38 sam Exp $
 */
#ifndef _NET80211_IEEE80211_IOCTL_H_
#define _NET80211_IEEE80211_IOCTL_H_

/*
 * IEEE 802.11 ioctls.
 */
#include <_ieee80211.h>
#include <ieee80211.h>
#include <ieee80211_crypto.h>

/*
 * Per/node (station) statistics available when operating as an AP.
 */
struct ieee80211_nodestats {
	a_uint32_t	ns_rx_data;		/* rx data frames */
	a_uint32_t	ns_rx_mgmt;		/* rx management frames */
	a_uint32_t	ns_rx_ctrl;		/* rx control frames */
	a_uint32_t	ns_rx_ucast;		/* rx unicast frames */
	a_uint32_t	ns_rx_mcast;		/* rx multi/broadcast frames */
	a_uint64_t	ns_rx_bytes;		/* rx data count (bytes) */
	a_uint64_t	ns_rx_beacons;		/* rx beacon frames */
	a_uint32_t	ns_rx_proberesp;	/* rx probe response frames */

	a_uint32_t	ns_rx_dup;		/* rx discard 'cuz dup */
	a_uint32_t	ns_rx_noprivacy;	/* rx w/ wep but privacy off */
	a_uint32_t	ns_rx_wepfail;		/* rx wep processing failed */
	a_uint32_t	ns_rx_demicfail;	/* rx demic failed */
	a_uint32_t	ns_rx_decap;		/* rx decapsulation failed */
	a_uint32_t	ns_rx_defrag;		/* rx defragmentation failed */
	a_uint32_t	ns_rx_disassoc;		/* rx disassociation */
	a_uint32_t	ns_rx_deauth;		/* rx deauthentication */
    a_uint32_t   ns_rx_action;       /* rx action */
	a_uint32_t	ns_rx_decryptcrc;	/* rx decrypt failed on crc */
	a_uint32_t	ns_rx_unauth;		/* rx on unauthorized port */
	a_uint32_t	ns_rx_unencrypted;	/* rx unecrypted w/ privacy */

	a_uint32_t	ns_tx_data;		/* tx data frames */
	a_uint32_t	ns_tx_mgmt;		/* tx management frames */
	a_uint32_t	ns_tx_ucast;		/* tx unicast frames */
	a_uint32_t	ns_tx_mcast;		/* tx multi/broadcast frames */
	a_uint64_t	ns_tx_bytes;		/* tx data count (bytes) */
	a_uint32_t	ns_tx_probereq;		/* tx probe request frames */
	a_uint32_t	ns_tx_uapsd;		/* tx on uapsd queue */

	a_uint32_t	ns_tx_novlantag;	/* tx discard 'cuz no tag */
	a_uint32_t	ns_tx_vlanmismatch;	/* tx discard 'cuz bad tag */

	a_uint32_t	ns_tx_eosplost;		/* uapsd EOSP retried out */

	a_uint32_t	ns_ps_discard;		/* ps discard 'cuz of age */

	a_uint32_t	ns_uapsd_triggers;	/* uapsd triggers */

	/* MIB-related state */
	a_uint32_t	ns_tx_assoc;		/* [re]associations */
	a_uint32_t	ns_tx_assoc_fail;	/* [re]association failures */
	a_uint32_t	ns_tx_auth;		/* [re]authentications */
	a_uint32_t	ns_tx_auth_fail;	/* [re]authentication failures*/
	a_uint32_t	ns_tx_deauth;		/* deauthentications */
	a_uint32_t	ns_tx_deauth_code;	/* last deauth reason */
	a_uint32_t	ns_tx_disassoc;		/* disassociations */
	a_uint32_t	ns_tx_disassoc_code;	/* last disassociation reason */
	a_uint32_t	ns_psq_drops;		/* power save queue drops */
};

/*
 * Summary statistics.
 */
struct ieee80211_stats {
	a_uint32_t	is_rx_badversion;	/* rx frame with bad version */
	a_uint32_t	is_rx_tooshort;		/* rx frame too short */
	a_uint32_t	is_rx_wrongbss;		/* rx from wrong bssid */
	a_uint32_t	is_rx_dup;		/* rx discard 'cuz dup */
	a_uint32_t	is_rx_wrongdir;		/* rx w/ wrong direction */
	a_uint32_t	is_rx_mcastecho;	/* rx discard 'cuz mcast echo */
	a_uint32_t	is_rx_notassoc;		/* rx discard 'cuz sta !assoc */
	a_uint32_t	is_rx_noprivacy;	/* rx w/ wep but privacy off */
	a_uint32_t	is_rx_unencrypted;	/* rx w/o wep and privacy on */
	a_uint32_t	is_rx_wepfail;		/* rx wep processing failed */
	a_uint32_t	is_rx_decap;		/* rx decapsulation failed */
	a_uint32_t	is_rx_mgtdiscard;	/* rx discard mgt frames */
	a_uint32_t	is_rx_ctl;		/* rx discard ctrl frames */
	a_uint32_t	is_rx_beacon;		/* rx beacon frames */
	a_uint32_t	is_rx_rstoobig;		/* rx rate set truncated */
	a_uint32_t	is_rx_elem_missing;	/* rx required element missing*/
	a_uint32_t	is_rx_elem_toobig;	/* rx element too big */
	a_uint32_t	is_rx_elem_toosmall;	/* rx element too small */
	a_uint32_t	is_rx_elem_unknown;	/* rx element unknown */
	a_uint32_t	is_rx_badchan;		/* rx frame w/ invalid chan */
	a_uint32_t	is_rx_chanmismatch;	/* rx frame chan mismatch */
	a_uint32_t	is_rx_nodealloc;	/* rx frame dropped */
	a_uint32_t	is_rx_ssidmismatch;	/* rx frame ssid mismatch  */
	a_uint32_t	is_rx_auth_unsupported;	/* rx w/ unsupported auth alg */
	a_uint32_t	is_rx_auth_fail;	/* rx sta auth failure */
	a_uint32_t	is_rx_auth_countermeasures;/* rx auth discard 'cuz CM */
	a_uint32_t	is_rx_assoc_bss;	/* rx assoc from wrong bssid */
	a_uint32_t	is_rx_assoc_notauth;	/* rx assoc w/o auth */
	a_uint32_t	is_rx_assoc_capmismatch;/* rx assoc w/ cap mismatch */
	a_uint32_t	is_rx_assoc_norate;	/* rx assoc w/ no rate match */
	a_uint32_t	is_rx_assoc_badwpaie;	/* rx assoc w/ bad WPA IE */
	a_uint32_t	is_rx_deauth;		/* rx deauthentication */
	a_uint32_t	is_rx_disassoc;		/* rx disassociation */
    a_uint32_t   is_rx_action;       /* rx action mgt */
	a_uint32_t	is_rx_badsubtype;	/* rx frame w/ unknown subtype*/
	a_uint32_t	is_rx_nobuf;		/* rx failed for lack of buf */
	a_uint32_t	is_rx_decryptcrc;	/* rx decrypt failed on crc */
	a_uint32_t	is_rx_ahdemo_mgt;	/* rx discard ahdemo mgt frame*/
	a_uint32_t	is_rx_bad_auth;		/* rx bad auth request */
	a_uint32_t	is_rx_unauth;		/* rx on unauthorized port */
	a_uint32_t	is_rx_badkeyid;		/* rx w/ incorrect keyid */
	a_uint32_t	is_rx_ccmpreplay;	/* rx seq# violation (CCMP) */
	a_uint32_t	is_rx_ccmpformat;	/* rx format bad (CCMP) */
	a_uint32_t	is_rx_ccmpmic;		/* rx MIC check failed (CCMP) */
	a_uint32_t	is_rx_tkipreplay;	/* rx seq# violation (TKIP) */
	a_uint32_t	is_rx_tkipformat;	/* rx format bad (TKIP) */
	a_uint32_t	is_rx_tkipmic;		/* rx MIC check failed (TKIP) */
	a_uint32_t	is_rx_tkipicv;		/* rx ICV check failed (TKIP) */
	a_uint32_t	is_rx_badcipher;	/* rx failed 'cuz key type */
	a_uint32_t	is_rx_nocipherctx;	/* rx failed 'cuz key !setup */
	a_uint32_t	is_rx_acl;		/* rx discard 'cuz acl policy */
	a_uint32_t	is_rx_ffcnt;		/* rx fast frames */
	a_uint32_t	is_rx_badathtnl;   	/* driver key alloc failed */
	a_uint32_t	is_tx_nobuf;		/* tx failed for lack of buf */
	a_uint32_t	is_tx_nonode;		/* tx failed for no node */
	a_uint32_t	is_tx_unknownmgt;	/* tx of unknown mgt frame */
	a_uint32_t	is_tx_badcipher;	/* tx failed 'cuz key type */
	a_uint32_t	is_tx_nodefkey;		/* tx failed 'cuz no defkey */
	a_uint32_t	is_tx_noheadroom;	/* tx failed 'cuz no space */
	a_uint32_t	is_tx_ffokcnt;		/* tx fast frames sent success */
	a_uint32_t	is_tx_fferrcnt;		/* tx fast frames sent success */
	a_uint32_t	is_scan_active;		/* active scans started */
	a_uint32_t	is_scan_passive;	/* passive scans started */
	a_uint32_t	is_node_timeout;	/* nodes timed out inactivity */
	a_uint32_t	is_crypto_nomem;	/* no memory for crypto ctx */
	a_uint32_t	is_crypto_tkip;		/* tkip crypto done in s/w */
	a_uint32_t	is_crypto_tkipenmic;	/* tkip en-MIC done in s/w */
	a_uint32_t	is_crypto_tkipdemic;	/* tkip de-MIC done in s/w */
	a_uint32_t	is_crypto_tkipcm;	/* tkip counter measures */
	a_uint32_t	is_crypto_ccmp;		/* ccmp crypto done in s/w */
	a_uint32_t	is_crypto_wep;		/* wep crypto done in s/w */
	a_uint32_t	is_crypto_setkey_cipher;/* cipher rejected key */
	a_uint32_t	is_crypto_setkey_nokey;	/* no key index for setkey */
	a_uint32_t	is_crypto_delkey;	/* driver key delete failed */
	a_uint32_t	is_crypto_badcipher;	/* unknown cipher */
	a_uint32_t	is_crypto_nocipher;	/* cipher not available */
	a_uint32_t	is_crypto_attachfail;	/* cipher attach failed */
	a_uint32_t	is_crypto_swfallback;	/* cipher fallback to s/w */
	a_uint32_t	is_crypto_keyfail;	/* driver key alloc failed */
	a_uint32_t	is_crypto_enmicfail;	/* en-MIC failed */
	a_uint32_t	is_ibss_capmismatch;	/* merge failed-cap mismatch */
	a_uint32_t	is_ibss_norate;		/* merge failed-rate mismatch */
	a_uint32_t	is_ps_unassoc;		/* ps-poll for unassoc. sta */
	a_uint32_t	is_ps_badaid;		/* ps-poll w/ incorrect aid */
	a_uint32_t	is_ps_qempty;		/* ps-poll w/ nothing to send */
};

/*
 * Max size of optional information elements.  We artificially
 * constrain this; it's limited only by the max frame size (and
 * the max parameter size of the wireless extensions).
 */
#define	IEEE80211_MAX_OPT_IE	256
#define	IEEE80211_MAX_WSC_IE	256

/*
 * WPA/RSN get/set key request.  Specify the key/cipher
 * type and whether the key is to be used for sending and/or
 * receiving.  The key index should be set only when working
 * with global keys (use IEEE80211_KEYIX_NONE for ``no index'').
 * Otherwise a unicast/pairwise key is specified by the bssid
 * (on a station) or mac address (on an ap).  They key length
 * must include any MIC key data; otherwise it should be no
 more than IEEE80211_KEYBUF_SIZE.
 */
struct ieee80211req_key {
	a_uint8_t	ik_type;	/* key/cipher type */
	a_uint8_t	ik_pad;
	a_uint16_t	ik_keyix;	/* key index */
	a_uint8_t	ik_keylen;	/* key length in bytes */
	a_uint8_t	ik_flags;
/* NB: IEEE80211_KEY_XMIT and IEEE80211_KEY_RECV defined elsewhere */
#define	IEEE80211_KEY_DEFAULT	0x80	/* default xmit key */
	a_uint8_t	ik_macaddr[IEEE80211_ADDR_LEN];
	a_uint64_t	ik_keyrsc;	/* key receive sequence counter */
	a_uint64_t	ik_keytsc;	/* key transmit sequence counter */
	a_uint8_t	ik_keydata[IEEE80211_KEYBUF_SIZE+IEEE80211_MICBUF_SIZE];
};

/*
 * Delete a key either by index or address.  Set the index
 * to IEEE80211_KEYIX_NONE when deleting a unicast key.
 */
struct ieee80211req_del_key {
	a_uint8_t	idk_keyix;	/* key index */
	a_uint8_t	idk_macaddr[IEEE80211_ADDR_LEN];
};

/*
 * MLME state manipulation request.  IEEE80211_MLME_ASSOC
 * only makes sense when operating as a station.  The other
 * requests can be used when operating as a station or an
 * ap (to effect a station).
 */
struct ieee80211req_mlme {
	a_uint8_t	im_op;		/* operation to perform */
#define	IEEE80211_MLME_ASSOC		1	/* associate station */
#define	IEEE80211_MLME_DISASSOC		2	/* disassociate station */
#define	IEEE80211_MLME_DEAUTH		3	/* deauthenticate station */
#define	IEEE80211_MLME_AUTHORIZE	4	/* authorize station */
#define	IEEE80211_MLME_UNAUTHORIZE	5	/* unauthorize station */
	a_uint16_t	im_reason;	/* 802.11 reason code */
	a_uint8_t	im_macaddr[IEEE80211_ADDR_LEN];
};

/* 
 * MAC ACL operations.
 */
enum {
	IEEE80211_MACCMD_POLICY_OPEN	= 0,	/* set policy: no ACL's */
	IEEE80211_MACCMD_POLICY_ALLOW	= 1,	/* set policy: allow traffic */
	IEEE80211_MACCMD_POLICY_DENY	= 2,	/* set policy: deny traffic */
	IEEE80211_MACCMD_FLUSH		= 3,	/* flush ACL database */
	IEEE80211_MACCMD_DETACH		= 4,	/* detach ACL policy */
};

/*
 * Set the active channel list.  Note this list is
 * intersected with the available channel list in
 * calculating the set of channels actually used in
 * scanning.
 */
struct ieee80211req_chanlist {
	a_uint8_t	ic_channels[IEEE80211_CHAN_BYTES];
};

/*
 * Get the active channel list info.
 */
struct ieee80211req_chaninfo {
	a_uint32_t	ic_nchans;
	struct ieee80211_channel ic_chans[IEEE80211_CHAN_MAX];
};

/*
 * Retrieve the WPA/RSN information element for an associated station.
 */
struct ieee80211req_wpaie {
	a_uint8_t	wpa_macaddr[IEEE80211_ADDR_LEN];
	a_uint8_t	wpa_ie[IEEE80211_MAX_OPT_IE];
};

/*
 * Retrieve the WSC information element for an associated station.
 */
struct ieee80211req_wscie {
	a_uint8_t	wsc_macaddr[IEEE80211_ADDR_LEN];
	a_uint8_t	wsc_ie[IEEE80211_MAX_WSC_IE];
};


/*
 * Retrieve per-node statistics.
 */
struct ieee80211req_sta_stats {
	union {
		/* NB: explicitly force 64-bit alignment */
		a_uint8_t	macaddr[IEEE80211_ADDR_LEN];
		a_uint64_t	pad;
	} is_u;
	struct ieee80211_nodestats is_stats;
};

/*
 * Station information block; the mac address is used
 * to retrieve other data like stats, unicast key, etc.
 */
struct ieee80211req_sta_info {
	a_uint16_t	isi_len;		/* length (mult of 4) */
	a_uint16_t	isi_freq;		/* MHz */
        a_uint32_t   isi_flags;      /* channel flags */
	a_uint16_t	isi_state;		/* state flags */
	a_uint8_t	isi_authmode;		/* authentication algorithm */
	a_int8_t	    	isi_rssi;
	a_uint16_t	isi_capinfo;		/* capabilities */
	a_uint8_t	isi_athflags;		/* Atheros capabilities */
	a_uint8_t	isi_erp;		/* ERP element */
	a_uint8_t	isi_macaddr[IEEE80211_ADDR_LEN];
	a_uint8_t	isi_nrates;
						/* negotiated rates */
	a_uint8_t	isi_rates[IEEE80211_RATE_MAXSIZE];
	a_uint8_t	isi_txrate;		/* index to isi_rates[] */
	a_uint32_t	isi_txrateKbps;		/* rate in Kbps , for 11n */
	a_uint16_t	isi_ie_len;		/* IE length */
	a_uint16_t	isi_associd;		/* assoc response */
	a_uint16_t	isi_txpower;		/* current tx power */
	a_uint16_t	isi_vlan;		/* vlan tag */
	a_uint16_t	isi_txseqs[17];		/* seq to be transmitted */
	a_uint16_t	isi_rxseqs[17];		/* seq previous for qos frames*/
	a_uint16_t	isi_inact;		/* inactivity timer */
	a_uint8_t	isi_uapsd;		/* UAPSD queues */
	a_uint8_t	isi_opmode;		/* sta operating mode */
	a_uint8_t	isi_cipher;
    adf_time_t  isi_assoc_time;         /* sta association time */


        a_uint16_t   isi_htcap;      /* HT capabilities */
	/* XXX frag state? */
	/* variable length IE data */
};

enum {
	IEEE80211_STA_OPMODE_NORMAL,
	IEEE80211_STA_OPMODE_XR
};

/*
 * Retrieve per-station information; to retrieve all
 * specify a mac address of ff:ff:ff:ff:ff:ff.
 */
struct ieee80211req_sta_req {
	union {
		/* NB: explicitly force 64-bit alignment */
		a_uint8_t	macaddr[IEEE80211_ADDR_LEN];
		a_uint64_t	pad;
	} is_u;
	struct ieee80211req_sta_info info[1];	/* variable length */
};

/*
 * Get/set per-station tx power cap.
 */
struct ieee80211req_sta_txpow {
	a_uint8_t	it_macaddr[IEEE80211_ADDR_LEN];
	a_uint8_t	it_txpow;
};

/*
 * WME parameters are set and return using i_val and i_len.
 * i_val holds the value itself.  i_len specifies the AC
 * and, as appropriate, then high bit specifies whether the
 * operation is to be applied to the BSS or ourself.
 */
#define	IEEE80211_WMEPARAM_SELF	0x0000		/* parameter applies to self */
#define	IEEE80211_WMEPARAM_BSS	0x8000		/* parameter applies to BSS */
#define	IEEE80211_WMEPARAM_VAL	0x7fff		/* parameter value */

/*
 * Scan result data returned for IEEE80211_IOC_SCAN_RESULTS.
 */
struct ieee80211req_scan_result {
    a_uint16_t      isr_len;                /* length (mult of 4) */
    a_uint16_t      isr_freq;               /* MHz */
    a_uint16_t      isr_flags;              /* channel flags */
    a_uint8_t       isr_noise;
    a_uint8_t       isr_rssi;
    a_uint8_t       isr_intval;             /* beacon interval */
    a_uint16_t      isr_capinfo;            /* capabilities */
    a_uint8_t       isr_erp;                /* ERP element */
    a_uint8_t       isr_bssid[IEEE80211_ADDR_LEN];
    a_uint8_t       isr_nrates;
    a_uint8_t       isr_rates[IEEE80211_RATE_MAXSIZE];
    a_uint8_t       isr_ssid_len;           /* SSID length */
    a_uint8_t       isr_ie_len;             /* IE length */
    a_uint8_t       isr_pad[5];
    /* variable length SSID followed by IE data */
};

#ifdef __FreeBSD__
/*
 * FreeBSD-style ioctls.
 */
/* the first member must be matched with struct ifreq */
struct ieee80211req {
	char		i_name[ADF_NET_WCMD_NAME_SIZE];	/* if_name, e.g. "wi0" */
	a_uint16_t	i_type;			/* req type */
	a_uint16_t		i_val;			/* Index or simple value */
	a_uint16_t		i_len;			/* Index or simple value */
	void		*i_data;		/* Extra data */
};
#define	SIOCS80211		 _IOW('i', 234, struct ieee80211req)
#define	SIOCG80211		_IOWR('i', 235, struct ieee80211req)
//#define	SIOCG80211STATS		_IOWR('i', 236, struct ifreq)
//#define	SIOC80211IFCREATE	_IOWR('i', 237, struct ifreq)
//#define	SIOC80211IFDESTROY	 _IOW('i', 238, struct ifreq)

#define IEEE80211_IOC_SSID		1
#define IEEE80211_IOC_NUMSSIDS		2
#define IEEE80211_IOC_WEP		3
#define 	IEEE80211_WEP_NOSUP	-1
#define 	IEEE80211_WEP_OFF	0
#define 	IEEE80211_WEP_ON	1
#define 	IEEE80211_WEP_MIXED	2
#define IEEE80211_IOC_WEPKEY		4
#define IEEE80211_IOC_NUMWEPKEYS	5
#define IEEE80211_IOC_WEPTXKEY		6
#define IEEE80211_IOC_AUTHMODE		7
#define IEEE80211_IOC_STATIONNAME	8
#define IEEE80211_IOC_CHANNEL		9
#define IEEE80211_IOC_POWERSAVE		10
#define 	IEEE80211_POWERSAVE_NOSUP	-1
#define 	IEEE80211_POWERSAVE_OFF		0
#define 	IEEE80211_POWERSAVE_CAM		1
#define 	IEEE80211_POWERSAVE_PSP		2
#define 	IEEE80211_POWERSAVE_PSP_CAM	3
#define 	IEEE80211_POWERSAVE_ON		IEEE80211_POWERSAVE_CAM
#define IEEE80211_IOC_POWERSAVESLEEP	11
#define	IEEE80211_IOC_RTSTHRESHOLD	12
#define IEEE80211_IOC_PROTMODE		13
#define 	IEEE80211_PROTMODE_OFF		0
#define 	IEEE80211_PROTMODE_CTS		1
#define 	IEEE80211_PROTMODE_RTSCTS	2
#define	IEEE80211_IOC_TXPOWER		14	/* global tx power limit */
#define	IEEE80211_IOC_BSSID		15
#define	IEEE80211_IOC_ROAMING		16	/* roaming mode */
#define	IEEE80211_IOC_PRIVACY		17	/* privacy invoked */
#define	IEEE80211_IOC_DROPUNENCRYPTED	18	/* discard unencrypted frames */
#define	IEEE80211_IOC_WPAKEY		19
#define	IEEE80211_IOC_DELKEY		20
#define	IEEE80211_IOC_MLME		21
#define	IEEE80211_IOC_OPTIE		22	/* optional info. element */
#define	IEEE80211_IOC_SCAN_REQ		23
#define	IEEE80211_IOC_SCAN_RESULTS	24
#define	IEEE80211_IOC_COUNTERMEASURES	25	/* WPA/TKIP countermeasures */
#define	IEEE80211_IOC_WPA		26	/* WPA mode (0,1,2) */
#define	IEEE80211_IOC_CHANLIST		27	/* channel list */
#define	IEEE80211_IOC_WME		28	/* WME mode (on, off) */
#define	IEEE80211_IOC_HIDESSID		29	/* hide SSID mode (on, off) */
#define IEEE80211_IOC_APBRIDGE      30 /* AP inter-sta bridging */
#define	IEEE80211_IOC_MCASTCIPHER	31	/* multicast/default cipher */
#define	IEEE80211_IOC_MCASTKEYLEN	32	/* multicast key length */
#define	IEEE80211_IOC_UCASTCIPHERS	33	/* unicast cipher suites */
#define	IEEE80211_IOC_UCASTCIPHER	34	/* unicast cipher */
#define	IEEE80211_IOC_UCASTKEYLEN	35	/* unicast key length */
#define	IEEE80211_IOC_DRIVER_CAPS	36	/* driver capabilities */
#define	IEEE80211_IOC_KEYMGTALGS	37	/* key management algorithms */
#define	IEEE80211_IOC_RSNCAPS		38	/* RSN capabilities */
#define	IEEE80211_IOC_WPAIE		39	/* WPA information element */
#define	IEEE80211_IOC_STA_STATS		40	/* per-station statistics */
#define	IEEE80211_IOC_MACCMD		41	/* MAC ACL operation */
#define	IEEE80211_IOC_TXPOWMAX		43	/* max tx power for channel */
#define	IEEE80211_IOC_STA_TXPOW		44	/* per-station tx power limit */
#define	IEEE80211_IOC_STA_INFO		45	/* station/neighbor info */
#define	IEEE80211_IOC_WME_CWMIN		46	/* WME: ECWmin */
#define	IEEE80211_IOC_WME_CWMAX		47	/* WME: ECWmax */
#define	IEEE80211_IOC_WME_AIFS		48	/* WME: AIFSN */
#define	IEEE80211_IOC_WME_TXOPLIMIT	49	/* WME: txops limit */
#define	IEEE80211_IOC_WME_ACM		50	/* WME: ACM (bss only) */
#define	IEEE80211_IOC_WME_ACKPOLICY	51	/* WME: ACK policy (!bss only)*/
#define	IEEE80211_IOC_DTIM_PERIOD	52	/* DTIM period (beacons) */
#define	IEEE80211_IOC_BEACON_INTERVAL	53	/* beacon interval (ms) */
#define	IEEE80211_IOC_ADDMAC		54	/* add sta to MAC ACL table */
#define	IEEE80211_IOC_DELMAC		55	/* del sta from MAC ACL table */
#define	IEEE80211_IOC_FF		56	/* ATH fast frames (on, off) */
#define	IEEE80211_IOC_TURBOP		57	/* ATH turbo' (on, off) */
#define IEEE80211_IOC_APPIEBUF          58      /* IE in the management frame */
#define IEEE80211_IOC_FILTERFRAME       59      /* management frame filter*/

/*
 * Scan result data returned for IEEE80211_IOC_SCAN_RESULTS.
 */
struct ieee80211req_scan_result {
	a_uint16_t	isr_len;		/* length (mult of 4) */
	a_uint16_t	isr_freq;		/* MHz */
	a_uint32_t   isr_flags;      /* channel flags */
	a_uint8_t	isr_noise;
	a_uint8_t	isr_rssi;
	a_uint8_t	isr_intval;		/* beacon interval */
	a_uint16_t	isr_capinfo;		/* capabilities */
	a_uint8_t	isr_erp;		/* ERP element */
	a_uint8_t	isr_bssid[IEEE80211_ADDR_LEN];
	a_uint8_t	isr_nrates;
	a_uint8_t	isr_rates[IEEE80211_RATE_MAXSIZE];
	a_uint8_t	isr_ssid_len;		/* SSID length */
	a_uint8_t	isr_ie_len;		/* IE length */
	a_uint8_t	isr_pad[5];
	/* variable length SSID followed by IE data */
};

#endif /* __FreeBSD__ */

// #ifdef __linux__
/*
 * Wireless Extensions API, private ioctl interfaces.
 *
 * NB: Even-numbered ioctl numbers have set semantics and are privileged!
 *     (regardless of the incorrect comment in wireless.h!)
 */
// #include <linux/if.h>
#define	IEEE80211_IOCTL_SETPARAM	(SIOCIWFIRSTPRIV+0)
#define	IEEE80211_IOCTL_GETPARAM	(SIOCIWFIRSTPRIV+1)
#define	IEEE80211_IOCTL_SETKEY		(SIOCIWFIRSTPRIV+2)
#define	IEEE80211_IOCTL_SETWMMPARAMS	(SIOCIWFIRSTPRIV+3)
#define	IEEE80211_IOCTL_DELKEY		(SIOCIWFIRSTPRIV+4)
#define	IEEE80211_IOCTL_GETWMMPARAMS	(SIOCIWFIRSTPRIV+5)
#define	IEEE80211_IOCTL_SETMLME		(SIOCIWFIRSTPRIV+6)
#define	IEEE80211_IOCTL_GETCHANINFO	(SIOCIWFIRSTPRIV+7)
#define	IEEE80211_IOCTL_SETOPTIE	(SIOCIWFIRSTPRIV+8)
#define	IEEE80211_IOCTL_GETOPTIE	(SIOCIWFIRSTPRIV+9)
#define	IEEE80211_IOCTL_ADDMAC		(SIOCIWFIRSTPRIV+10)
#define	IEEE80211_IOCTL_DELMAC		(SIOCIWFIRSTPRIV+12)
#define	IEEE80211_IOCTL_GETCHANLIST	(SIOCIWFIRSTPRIV+13)
#define	IEEE80211_IOCTL_SETCHANLIST	(SIOCIWFIRSTPRIV+14)
#define	IEEE80211_IOCTL_CHANSWITCH	(SIOCIWFIRSTPRIV+16)
#define	IEEE80211_IOCTL_SETMODE		(SIOCIWFIRSTPRIV+17)
#define	IEEE80211_IOCTL_GETMODE		(SIOCIWFIRSTPRIV+18)
#define IEEE80211_IOCTL_GET_APPIEBUF    (SIOCIWFIRSTPRIV+19)
#define IEEE80211_IOCTL_SET_APPIEBUF    (SIOCIWFIRSTPRIV+20)
#define IEEE80211_IOCTL_FILTERFRAME     (SIOCIWFIRSTPRIV+22) 
#define IEEE80211_IOCTL_SENDADDBA       (SIOCIWFIRSTPRIV+24)
#define IEEE80211_IOCTL_GETADDBASTATUS  (SIOCIWFIRSTPRIV+25)
#define IEEE80211_IOCTL_SENDDELBA       (SIOCIWFIRSTPRIV+26)
#define IEEE80211_IOCTL_SET_ADDBARESP   (SIOCIWFIRSTPRIV+28)

enum {
	IEEE80211_WMMPARAMS_CWMIN       = 1,
	IEEE80211_WMMPARAMS_CWMAX       = 2,
	IEEE80211_WMMPARAMS_AIFS       	= 3,
	IEEE80211_WMMPARAMS_TXOPLIMIT	= 4,
	IEEE80211_WMMPARAMS_ACM		= 5,
	IEEE80211_WMMPARAMS_NOACKPOLICY	= 6,	
};
enum {
	IEEE80211_PARAM_TURBO		= 1,	/* turbo mode */
	IEEE80211_PARAM_MODE		= 2,	/* phy mode (11a, 11b, etc.) */
	IEEE80211_PARAM_AUTHMODE	= 3,	/* authentication mode */
	IEEE80211_PARAM_PROTMODE	= 4,	/* 802.11g protection */
	IEEE80211_PARAM_MCASTCIPHER	= 5,	/* multicast/default cipher */
	IEEE80211_PARAM_MCASTKEYLEN	= 6,	/* multicast key length */
	IEEE80211_PARAM_UCASTCIPHERS	= 7,	/* unicast cipher suites */
	IEEE80211_PARAM_UCASTCIPHER	= 8,	/* unicast cipher */
	IEEE80211_PARAM_UCASTKEYLEN	= 9,	/* unicast key length */
	IEEE80211_PARAM_WPA		= 10,	/* WPA mode (0,1,2) */
	IEEE80211_PARAM_ROAMING		= 12,	/* roaming mode */
	IEEE80211_PARAM_PRIVACY		= 13,	/* privacy invoked */
	IEEE80211_PARAM_COUNTERMEASURES	= 14,	/* WPA/TKIP countermeasures */
	IEEE80211_PARAM_DROPUNENCRYPTED	= 15,	/* discard unencrypted frames */
	IEEE80211_PARAM_DRIVER_CAPS	= 16,	/* driver capabilities */
	IEEE80211_PARAM_MACCMD		= 17,	/* MAC ACL operation */
	IEEE80211_PARAM_WMM		= 18,	/* WMM mode (on, off) */
	IEEE80211_PARAM_HIDESSID	= 19,	/* hide SSID mode (on, off) */
    IEEE80211_PARAM_APBRIDGE    = 20,   /* AP inter-sta bridging */
	IEEE80211_PARAM_KEYMGTALGS	= 21,	/* key management algorithms */
	IEEE80211_PARAM_RSNCAPS		= 22,	/* RSN capabilities */
	IEEE80211_PARAM_INACT		= 23,	/* station inactivity timeout */
	IEEE80211_PARAM_INACT_AUTH	= 24,	/* station auth inact timeout */
	IEEE80211_PARAM_INACT_INIT	= 25,	/* station init inact timeout */
	IEEE80211_PARAM_ABOLT		= 26,	/* Atheros Adv. Capabilities */
	IEEE80211_PARAM_DTIM_PERIOD	= 28,	/* DTIM period (beacons) */
	IEEE80211_PARAM_BEACON_INTERVAL	= 29,	/* beacon interval (ms) */
	IEEE80211_PARAM_DOTH		= 30,	/* 11.h is on/off */
	IEEE80211_PARAM_PWRTARGET	= 31,	/* Current Channel Pwr Constraint */
	IEEE80211_PARAM_GENREASSOC	= 32,	/* Generate a reassociation request */
	IEEE80211_PARAM_COMPRESSION	= 33,	/* compression */
	IEEE80211_PARAM_FF		= 34,	/* fast frames support  */
	IEEE80211_PARAM_XR		= 35,	/* XR support */
	IEEE80211_PARAM_BURST		= 36,	/* burst mode */
	IEEE80211_PARAM_PUREG		= 37,	/* pure 11g (no 11b stations) */
	IEEE80211_PARAM_AR		= 38,	/* AR support */
	IEEE80211_PARAM_WDS		= 39,	/* Enable 4 address processing */
	IEEE80211_PARAM_BGSCAN		= 40,	/* bg scanning (on, off) */
	IEEE80211_PARAM_BGSCAN_IDLE	= 41,	/* bg scan idle threshold */
	IEEE80211_PARAM_BGSCAN_INTERVAL	= 42,	/* bg scan interval */
	IEEE80211_PARAM_MCAST_RATE	= 43,	/* Multicast Tx Rate */
	IEEE80211_PARAM_COVERAGE_CLASS	= 44,	/* coverage class */
	IEEE80211_PARAM_COUNTRY_IE	= 45,	/* enable country IE */
	IEEE80211_PARAM_SCANVALID	= 46,	/* scan cache valid threshold */
	IEEE80211_PARAM_ROAM_RSSI_11A	= 47,	/* rssi threshold in 11a */
	IEEE80211_PARAM_ROAM_RSSI_11B	= 48,	/* rssi threshold in 11b */
	IEEE80211_PARAM_ROAM_RSSI_11G	= 49,	/* rssi threshold in 11g */
	IEEE80211_PARAM_ROAM_RATE_11A	= 50,	/* tx rate threshold in 11a */
	IEEE80211_PARAM_ROAM_RATE_11B	= 51,	/* tx rate threshold in 11b */
	IEEE80211_PARAM_ROAM_RATE_11G	= 52,	/* tx rate threshold in 11g */
	IEEE80211_PARAM_UAPSDINFO	= 53,	/* value for qos info field */
	IEEE80211_PARAM_SLEEP		= 54,	/* force sleep/wake */
	IEEE80211_PARAM_QOSNULL		= 55,	/* force sleep/wake */
	IEEE80211_PARAM_PSPOLL		= 56,	/* force ps-poll generation (sta only) */
	IEEE80211_PARAM_EOSPDROP	= 57,	/* force uapsd EOSP drop (ap only) */
	IEEE80211_PARAM_MARKDFS		= 58,	/* mark a dfs interference channel when found */
	IEEE80211_PARAM_REGCLASS	= 59,	/* enable regclass ids in country IE */
	IEEE80211_PARAM_CHANBW		= 60,	/* set chan bandwidth preference */
	IEEE80211_PARAM_WMM_AGGRMODE    = 61,   /* set WMM Aggressive Mode */
	IEEE80211_PARAM_SHORTPREAMBLE 	= 62, 	/* enable/disable short Preamble */
	IEEE80211_PARAM_BLOCKDFSCHAN 	= 63, 	/* enable/disable use of DFS channels */
    IEEE80211_PARAM_CWM_MODE    = 64,   /* CWM mode */
    IEEE80211_PARAM_CWM_EXTOFFSET   = 65,   /* CWM extension channel offset */
    IEEE80211_PARAM_CWM_EXTPROTMODE = 66,   /* CWM extension channel protection mode */
    IEEE80211_PARAM_CWM_EXTPROTSPACING = 67,/* CWM extension channel protection spacing */
    IEEE80211_PARAM_CWM_ENABLE 		= 68,/* CWM state machine enabled */
    IEEE80211_PARAM_CWM_EXTBUSYTHRESHOLD = 69,/* CWM extension channel busy threshold */
    IEEE80211_PARAM_CWM_CHWIDTH = 70,	      /* CWM STATE: current channel width */
    IEEE80211_PARAM_SHORT_GI    = 71,   /* half GI */
    IEEE80211_PARAM_FAST_CC 	= 72,   /* fast channel change */

    /*
     * 11n A-MPDU, A-MSDU support
     */
    IEEE80211_PARAM_AMPDU         = 73,     /* 11n a-mpdu support */
    IEEE80211_PARAM_AMPDU_LIMIT   = 74,     /* a-mpdu length limit */
    IEEE80211_PARAM_AMPDU_DENSITY = 75,     /* a-mpdu density */
    IEEE80211_PARAM_AMPDU_SUBFRAMES = 76,   /* a-mpdu subframe limit */
    IEEE80211_PARAM_AMSDU         = 77,     /* a-msdu support */
    IEEE80211_PARAM_AMSDU_LIMIT   = 78,     /* a-msdu length limit */

    IEEE80211_PARAM_COUNTRYCODE   = 79,     /* Get country code */
    IEEE80211_PARAM_TX_CHAINMASK  = 80,     /* Tx chain mask */
    IEEE80211_PARAM_RX_CHAINMASK  = 81,     /* Rx chain mask */
    IEEE80211_PARAM_RTSCTS_RATECODE = 82,   /* RTS Rate code */
    IEEE80211_PARAM_HT_PROTECTION = 83,     /* Protect traffic in HT mode */
    IEEE80211_PARAM_RESET_ONCE    = 84,     /* Force a reset */
    IEEE80211_PARAM_SETADDBAOPER  = 85,     /* Set ADDBA mode */
    IEEE80211_PARAM_TX_CHAINMASK_LEGACY = 86, /* Tx chain mask for legacy clients */
    IEEE80211_PARAM_11N_RATE  = 87,         /* Set ADDBA mode */
    IEEE80211_PARAM_11N_RETRIES = 88,       /* Tx chain mask for legacy clients */
    IEEE80211_PARAM_WDS_AUTODETECT = 89,    /* Configurable Auto Detect/Delba for WDS mode */
    IEEE80211_PARAM_RB = 90,              /* Switch in/out of RB */
    /*
     * RB Detection knobs.
     */ 
    IEEE80211_PARAM_RB_DETECT = 91,       /* Do RB detection */
    IEEE80211_PARAM_RB_SKIP_THRESHOLD = 92, /* seqno-skip-by-1s to detect */
    IEEE80211_PARAM_RB_TIMEOUT = 93,      /* (in ms) to restore non-RB */
    IEEE80211_PARAM_NO_HTIE       = 94,    /* Control whether HT IEs are sent out or parsed */
    IEEE80211_PARAM_MAXSTA 		= 95, /* Config max allowable staions for each VAP */
    IEEE80211_PARAM_ETHER_DONGLE = 96, /* support for ether-dongle */
};

#define	SIOCG80211STATS		(SIOCDEVPRIVATE+2)
/* NB: require in+out parameters so cannot use wireless extensions, yech */
#define	IEEE80211_IOCTL_GETKEY		(SIOCDEVPRIVATE+3)
#define	IEEE80211_IOCTL_GETWPAIE	(SIOCDEVPRIVATE+4)
#define	IEEE80211_IOCTL_STA_STATS	(SIOCDEVPRIVATE+5)
#define	IEEE80211_IOCTL_STA_INFO	(SIOCDEVPRIVATE+6)
#define	SIOC80211IFCREATE		(SIOCDEVPRIVATE+7)
#define	SIOC80211IFDESTROY	 	(SIOCDEVPRIVATE+8)
#define	IEEE80211_IOCTL_SCAN_RESULTS	(SIOCDEVPRIVATE+9)
#define	IEEE80211_IOCTL_GETWSCIE	(SIOCDEVPRIVATE+10)
struct ieee80211_clone_params {
	char		icp_name[ADF_NET_WCMD_NAME_SIZE];	/* device name */
	a_uint16_t	icp_opmode;		/* operating mode */
	a_uint16_t	icp_flags;		/* see below */
#define	IEEE80211_CLONE_BSSID	0x0001		/* allocate unique mac/bssid */
#define	IEEE80211_NO_STABEACONS	0x0002		/* Do not setup the station beacon timers */
};

/* added APPIEBUF related definations */
enum{
    IEEE80211_APPIE_FRAME_BEACON     = 0,
    IEEE80211_APPIE_FRAME_PROBE_REQ  = 1,   
    IEEE80211_APPIE_FRAME_PROBE_RESP = 2,   
    IEEE80211_APPIE_FRAME_ASSOC_REQ  = 3,   
    IEEE80211_APPIE_FRAME_ASSOC_RESP = 4,   
    IEEE80211_APPIE_NUM_OF_FRAME     = 5   
};
struct ieee80211req_getset_appiebuf {
    a_uint32_t app_frmtype; /*management frame type for which buffer is added*/
    a_uint32_t app_buflen;  /*application supplied buffer length */
    a_uint8_t  app_buf[];	/* Variable array accessed thru this pointer as app_buf[0]*/
};

/* the following definations are used by application to set filter 
 * for receiving management frames */
enum {
     IEEE80211_FILTER_TYPE_BEACON      =   0x1,  
     IEEE80211_FILTER_TYPE_PROBE_REQ   =   0x2,   
     IEEE80211_FILTER_TYPE_PROBE_RESP  =   0x4,   
     IEEE80211_FILTER_TYPE_ASSOC_REQ   =   0x8,   
     IEEE80211_FILTER_TYPE_ASSOC_RESP  =   0x10,   
     IEEE80211_FILTER_TYPE_AUTH        =   0x20,   
     IEEE80211_FILTER_TYPE_DEAUTH      =   0x40,   
     IEEE80211_FILTER_TYPE_DISASSOC    =   0x80,   
     IEEE80211_FILTER_TYPE_ALL         =   0xFF  /* used to check the valid filter bits */  
}; 

struct ieee80211req_set_filter {
      a_uint32_t app_filterype; /* management frame filter type */
};

//#endif /* __linux__ */

#endif /* _NET80211_IEEE80211_IOCTL_H_ */
