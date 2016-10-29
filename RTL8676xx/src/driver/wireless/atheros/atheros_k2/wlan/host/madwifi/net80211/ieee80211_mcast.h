/*-
 * Copyright (c) 2003-2007 Atheros Communications, Inc.
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
 *	$Id: //depot/sw/carriersrc/branches/magpie_host_dev/split_src/host/madwifi/net80211/ieee80211_mcast.h#5 $
 */
#ifndef _NET80211_IEEE80211_MCAST_H_
#define _NET80211_IEEE80211_MCAST_H_

#ifdef ATH_MC_TUNNEL

#ifdef CONFIG_NET_WIRELESS
#include <linux/wireless.h>
#endif

/*
 * If Linux can't tell us if irqs are disabled, then they are not.
 */ 
#ifndef irqs_disabled
#define irqs_disabled()     (0)
#endif

/*
 * Table locking definitions.
 */
typedef rwlock_t ieee80211_snoop_lock_t;
#define	IEEE80211_SNOOP_LOCK_INIT(_st, _name)	\
	rwlock_init(&(_st)->msl_lock)
#define	IEEE80211_SNOOP_LOCK_DESTROY(_st)
#define	IEEE80211_SNOOP_LOCK(_st)				\
	write_lock(&(_st)->msl_lock)
#define	IEEE80211_SNOOP_UNLOCK(_st)				\
	write_unlock(&(_st)->msl_lock)
#define	IEEE80211_SNOOP_LOCK_BH(_st)			\
	if (irqs_disabled()) {						\
		write_lock(&(_st)->msl_lock);			\
	} else {									\
		write_lock_bh(&(_st)->msl_lock); 		\
	}
#define	IEEE80211_SNOOP_UNLOCK_BH(_st)			\
	if (irqs_disabled()) {						\
		write_unlock(&(_st)->msl_lock);			\
	} else {									\
		write_unlock_bh(&(_st)->msl_lock);		\
	}

#define MAX_SNOOP_ENTRIES	4	/* max number of destinations to tunnel for */

/* TODO: Demo uses single combo list, not optimized */
/* list entry */
struct MC_GROUP_LIST_ENTRY {
    a_uint8_t							mgl_group_addr[IEEE80211_ADDR_LEN];
    a_uint8_t							mgl_group_member[IEEE80211_ADDR_LEN];
    unsigned long						mgl_timestamp;
	struct ieee80211_node				*mgl_ni;
	asf_tailq_entry(MC_GROUP_LIST_ENTRY)	mgl_list;
};

/* global (for demo only) struct to manage snoop */
struct MC_SNOOP_LIST {
	a_uint16_t							msl_group_list_count;
	a_uint16_t							msl_misc;
    ieee80211_snoop_lock_t				msl_lock;	/* lock snoop table */
    asf_tailq_head(MSL_HEAD_TYPE, MC_GROUP_LIST_ENTRY)	msl_node;	/* head of list of all snoop entries */
};

#define IGMP_SNOOP_CMD_OTHER 0
#define IGMP_SNOOP_CMD_JOIN  1
#define IGMP_SNOOP_CMD_LEAVE 2

extern a_int32_t ath_mc_snoop_enable;
extern a_int32_t ath_mc_snoop_debug;
a_int32_t mcSnoopListUpdate(struct MC_SNOOP_LIST *, a_int32_t, a_uint8_t *, a_uint8_t *,
	struct ieee80211_node *ni);
a_int32_t mcSnoopListGetGroup(struct MC_SNOOP_LIST *, a_uint8_t *, a_uint8_t *);
void mcSnoopListInit(struct ieee80211com *ic);
void mcSnoopDeliverData(struct ieee80211_node *ni, adf_nbuf_t);
void mcSnoopWDSNodeCleanup(struct ieee80211_node *ni);

#endif /* ATH_MC_TUNNEL */

#endif /* _NET80211_IEEE80211_MCAST_H_ */
