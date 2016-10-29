/*-
 * Copyright (c) 2003-2005 Sam Leffler, Errno Consulting
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
 *	$Id: //depot/sw/carriersrc/branches/magpie_host_dev/split_src/host/madwifi/net80211/ieee80211_linux.h#12 $
 */
#ifndef _NET80211_IEEE80211_LINUX_H_
#define _NET80211_IEEE80211_LINUX_H_

#include <adf_os_time.h>

#ifndef ATH_STA_ONLY
void nbuf_queue_drain(adf_nbuf_queue_t *q, adf_net_handle_t anet);

#endif

/*
 * Beacon handler locking definitions.
 */
typedef adf_os_spinlock_t ieee80211com_lock_t;
#define	IEEE80211_LOCK_INIT(_ic, _name)					\
	adf_os_spinlock_init(&(_ic)->ic_comlock)
#define	IEEE80211_LOCK_DESTROY(_ic)
#define	IEEE80211_LOCK(_ic, __ilockflags) do {				\
	adf_os_spin_lock_irq(&(_ic)->ic_comlock, &__ilockflags);
#define	IEEE80211_UNLOCK(_ic, __ilockflags)				\
	adf_os_spin_unlock_irq(&(_ic)->ic_comlock, &__ilockflags);	\
} while (0)
/* NB: beware, spin_is_locked() is unusable for !SMP */
#define	IEEE80211_LOCK_ASSERT(_ic)

#define IEEE80211_VAPS_LOCK_INIT(_ic, _name)		do {		\
	adf_os_spinlock_init(&(_ic)->ic_vapslock);			\
	(_ic)->ic_vapslock_held = 0;					\
}while(0)

#define IEEE80211_VAPS_LOCK_DESTROY(_ic)

#define IEEE80211_VAPS_LOCK(_ic) do {					\
	a_uint32_t __vlockflags=0;					\
        a_uint32_t __vlocked = (_ic)->ic_vapslock_held;			\
	if (!__vlocked) {						\
	    adf_os_spin_lock_irq(&(_ic)->ic_vapslock, &__vlockflags);	\
		(_ic)->ic_vapslock_held=1;				\
	}

#define IEEE80211_VAPS_UNLOCK(_ic)					   \
	if (!__vlocked) {						   \
		adf_os_spin_unlock_irq(&(_ic)->ic_vapslock,&__vlockflags); \
                (_ic)->ic_vapslock_held=0;				   \
	}								   \
} while (0)


/* NB: beware, spin_is_locked() is unusable for !SMP */
#if defined(CONFIG_SMP)
#define IEEE80211_VAPS_LOCK_ASSERT(_ic) \
  adf_os_assert(spin_is_locked(&(_ic)->ic_vapslock))
#else
#define IEEE80211_VAPS_LOCK_ASSERT(_ic)
#endif

/*
 * Beacon locking definitions; piggyback on com lock.
 */
#define	IEEE80211_BEACON_LOCK(_ic, _flag)	   IEEE80211_LOCK(_ic, _flag)
#define	IEEE80211_BEACON_UNLOCK(_ic, _flag)	   IEEE80211_UNLOCK(_ic, _flag)
#define	IEEE80211_BEACON_LOCK_ASSERT(_ic)  IEEE80211_LOCK_ASSERT(_ic)

/*
 * UAPSD locking definitions; piggyback on com lock.
 */
#define	IEEE80211_UAPSD_LOCK(_ic, _flag)	   IEEE80211_LOCK(_ic, _flag)
#define	IEEE80211_UAPSD_UNLOCK(_ic, _flag)	   IEEE80211_UNLOCK(_ic, _flag)
#define	IEEE80211_UAPSD_LOCK_ASSERT(_ic)   IEEE80211_LOCK_ASSERT(_ic)

/*
 * Node locking definitions.
 */
typedef adf_os_spinlock_t ieee80211_node_lock_t;
#define	IEEE80211_NODE_LOCK_INIT(_nt, _name)	adf_os_spinlock_init(&(_nt)->nt_nodelock)
#define	IEEE80211_NODE_LOCK_DESTROY(_nt)
#define	IEEE80211_NODE_LOCK(_nt, _flag) 	adf_os_spin_lock_irq(&(_nt)->nt_nodelock, &_flag)
#define	IEEE80211_NODE_UNLOCK(_nt, _flag)	adf_os_spin_unlock_irq(&(_nt)->nt_nodelock, &_flag)
#define	IEEE80211_NODE_LOCK_BH(_nt)			\
		adf_os_spin_lock_bh(&(_nt)->nt_nodelock); 	
#define	IEEE80211_NODE_UNLOCK_BH(_nt)			\
		adf_os_spin_unlock_bh(&(_nt)->nt_nodelock);	
/* NB: beware, *_is_locked() are boguly defined for UP+!PREEMPT */
#if (defined(CONFIG_SMP) || defined(CONFIG_PREEMPT)) && defined(rwlock_is_locked)
#define	IEEE80211_NODE_LOCK_ASSERT(_nt) \
  adf_os_assert(rwlock_is_locked(&(_nt)->nt_nodelock))
#else
#define	IEEE80211_NODE_LOCK_ASSERT(_nt)
#endif

/*
 * Node table scangen locking definitions.
 */
typedef adf_os_spinlock_t ieee80211_scan_lock_t;
#define	IEEE80211_SCAN_LOCK_INIT(_nt, _name) adf_os_spinlock_init(&(_nt)->nt_scanlock)
#define	IEEE80211_SCAN_LOCK_DESTROY(_nt)
#define	IEEE80211_SCAN_LOCK_BH(_nt)	do {		\
	  adf_os_spin_lock_bh(&(_nt)->nt_scanlock);	\
  } while (0)
#define	IEEE80211_SCAN_UNLOCK_BH(_nt)	do {		\
	  adf_os_spin_unlock_bh(&(_nt)->nt_scanlock);	\
  } while (0)
/* NB: beware, spin_is_locked() is unusable for !SMP */
#if defined(CONFIG_SMP)
#define	IEEE80211_SCAN_LOCK_ASSERT(_nt) \
  adf_os_assert(spin_is_locked(&(_nt)->nt_scanlock))
#else
#define	IEEE80211_SCAN_LOCK_ASSERT(_nt)
#endif

/*
 * 802.1x MAC ACL database locking definitions.
 */
typedef adf_os_spinlock_t acl_lock_t;
#define	ACL_LOCK_INIT(_as, _name)	adf_os_spinlock_init(&(_as)->as_lock)
#define	ACL_LOCK_DESTROY(_as)
#define	ACL_LOCK(_as)						\
  adf_os_spin_lock_bh(&(_as)->as_lock);	
#define	ACL_UNLOCK(_as)					        \
  adf_os_spin_unlock_bh(&(_as)->as_lock);	

/* NB: beware, spin_is_locked() is unusable for !SMP */
#if defined(CONFIG_SMP)
/* #define	ACL_LOCK_ASSERT(_as) \ */
/* 	KASSERT(spin_is_locked(&(_as)->as_lock), ("ACL not locked!")) */
/* #else */
#define	ACL_LOCK_ASSERT(_as)
#endif

/*
 * Per-node power-save queue definitions.  Beware of control
 * flow with IEEE80211_NODE_SAVEQ_LOCK/IEEE80211_NODE_SAVEQ_UNLOCK.
 */
#define	IEEE80211_NODE_SAVEQ_INIT(_ni, _name) do {		\
	adf_nbuf_queue_init(&(_ni)->ni_savedq);			\
} while (0)
#define	IEEE80211_NODE_SAVEQ_DESTROY(_ni)
#define	IEEE80211_NODE_SAVEQ_QLEN(_ni)	adf_nbuf_queue_len(&(_ni)->ni_savedq)

#define	IEEE80211_NODE_SAVEQ_LOCK(_ni) do {			\
	a_uint32_t __sqlockflags;				\
	adf_os_spin_lock_irq(&(_ni)->ni_savedq_lock, &__sqlockflags);

#define	IEEE80211_NODE_SAVEQ_UNLOCK(_ni)			\
	adf_os_spin_unlock_irq(&(_ni)->ni_savedq_lock, &__sqlockflags);\
} while (0)


#define	IEEE80211_NODE_SAVEQ_DEQUEUE(_ni, _skb, _qlen) do {	\
	IEEE80211_NODE_SAVEQ_LOCK(_ni);				\
	_skb = adf_nbuf_queue_remove(&(_ni)->ni_savedq);	\
	(_qlen) = adf_nbuf_queue_len(&(_ni)->ni_savedq);	\
	IEEE80211_NODE_SAVEQ_UNLOCK(_ni);			\
} while (0)
#define	_IEEE80211_NODE_SAVEQ_ENQUEUE(_ni, _skb, _qlen, _age) do {\
        if (adf_nbuf_is_queue_empty(&(_ni)->ni_savedq) {	\
	      adf_nbuf_queue_init(&(_ni)->ni_savedq);		\
	      adf_nbuf_queue_add(&(_ni)->ni_savedq, _skb);	\
	} else {						\
	      adf_nbuf_queue_add(&(_ni)->ni_savedq, _skb);	\
	    }							\
	M_AGE_SET(_skb, _age);					\
	(_qlen) = adf_nbuf_queue_len(&(_ni)->ni_savedq);	\
} while (0)

/*
 * Transmitted frames have the following information
 * held in the sk_buff control buffer.  This is used to
 * communicate various inter-procedural state that needs
 * to be associated with the frame for the duration of
 * it's existence.
 *
 * NB: sizeof(cb) == 48 and the vlan code grabs the first
 *     8 bytes so we reserve/avoid it.
 */
struct ieee80211_cb {
	struct ieee80211_node	*ni;
	a_uint32_t		flags;
#define	M_LINK0		0x01			/* frame needs WEP encryption */
#define	M_FF		0x02			/* fast frame */
#define	M_PWR_SAV	0x04			/* bypass power save handling */
#define M_UAPSD		0x08			/* frame flagged for u-apsd handling */
	struct sk_buff		*next;		/* fast frame sk_buf chain */
	a_uint8_t		u_tid;      /* user priority from vlan/ip tos   */
	a_uint16_t		magic;      /* check flags should clean or not */
        a_uint32_t		priority;
	a_uint32_t		csum;
};

#define M_FLAG_SET(_anbuf, _flag) do {				\
        a_uint8_t *_temp;					\
        _temp = adf_nbuf_get_priv(_anbuf);			\
	(((struct ieee80211_cb *)_temp)->flags |= (_flag));	\
	} while(0)

#define	M_FLAG_CLR(_anbuf, _flag) do {				\
        a_uint8_t *_temp;					\
        _temp = adf_nbuf_get_priv(_anbuf);			\
	(((struct ieee80211_cb *)_temp)->flags &= ~(_flag));	\
	} while(0)

#define	M_FLAG_GET(_anbuf, _flag, _val)  do {			\
        a_uint8_t *_temp;					\
        _temp = adf_nbuf_get_priv(_anbuf);			\
	_val = (((struct ieee80211_cb *)_temp)->flags & (_flag));	\
	} while(0)



#define M_FLAG_KEEP_ONLY(_anbuf, _flag) do {			\
        a_uint8_t *_temp;					\
        _temp = adf_nbuf_get_priv(_anbuf);			\
	(((struct ieee80211_cb *)_temp)->flags &= (_flag));	\
	} while(0)

#define	M_PWR_SAV_SET(skb) M_FLAG_SET((skb), M_PWR_SAV)
#define	M_PWR_SAV_CLR(skb) M_FLAG_CLR((skb), M_PWR_SAV)
#define	M_PWR_SAV_GET(skb, _val) M_FLAG_GET((skb), M_PWR_SAV, _val)

/*
 * Skbufs on the power save queue are tagged with an age and
 * timed out.  We reuse the hardware checksum field in the
 * mbuf packet header to store this data.
 * XXX use private cb area
 */

#define M_AGE_SET(_anbuf, _val)					\
	(((struct ieee80211_cb *)adf_nbuf_get_priv(_anbuf))->csum = _val)

#define M_AGE_GET(_anbuf)					\
	(((struct ieee80211_cb *)adf_nbuf_get_priv(_anbuf))->csum)

#define M_AGE_SUB(_anbuf, _adj)					\
	(((struct ieee80211_cb *)adf_nbuf_get_priv(_anbuf))->csum -= _adj)


struct ieee80211com;
struct ieee80211vap;

a_int32_t	ieee80211_load_module(const char *);

/*
 * Node reference counting definitions.
 *
 * ieee80211_node_initref	initialize the reference count to 1
 * ieee80211_node_incref	add a reference
 * ieee80211_node_decref	remove a reference
 * ieee80211_node_dectestref	remove a reference and return 1 if this
 *				is the last reference, otherwise 0
 * ieee80211_node_refcnt	reference count for printing (only)
 */
#define ieee80211_node_initref(_ni) \
	adf_os_atomic_init(&(_ni)->ni_refcnt) /* revisit */
#define ieee80211_node_incref(_ni) \
	adf_os_atomic_inc(&(_ni)->ni_refcnt)
#define	ieee80211_node_decref(_ni) \
	adf_os_atomic_dec(&(_ni)->ni_refcnt)
#define	ieee80211_node_dectestref(_ni) \
	adf_os_atomic_dec_and_test(&(_ni)->ni_refcnt)    /* TODO: fix the os-dependent function*/
#define	ieee80211_node_refcnt(_ni)	(_ni)->ni_refcnt.counter

//#define	le16toh(_x)	adf_os_le16_to_cpu(_x)
//#define	htole16(_x)	adf_os_cpu_to_le16(_x)
//#define	le32toh(_x)	adf_os_le32_to_cpu(_x)
//#define	htole32(_x)	adf_os_cpu_to_le32(_x)
//#define	be32toh(_x)	adf_os_be32_to_cpu(_x)
//#define	htobe32(_x)	adf_os_cpu_to_be32(_x)

/*
 * Linux has no equivalents to malloc types so null these out.
 */
/* #define	MALLOC_DEFINE(type, shortdesc, longdesc) */
/* #define	MALLOC_DECLARE(type) */

/*
 * flags to malloc.
 */
#define	M_NOWAIT	0x0001		/* do not block */
#define	M_WAITOK	0x0002		/* ok to block */
#define	M_ZERO		0x0100		/* bzero the allocation */

static __inline void *
ieee80211_malloc(size_t size, a_int32_t flags)
{
	void *p = adf_os_mem_alloc(size);
	if (p && (flags & M_ZERO))
		adf_os_mem_set(p, 0, size);
	return p;
}

#undef MALLOC
#define	MALLOC(_ptr, cast, _size, _type, _flags) \
	((_ptr) = (cast)ieee80211_malloc(_size, _flags))

#undef FREE
#define	FREE(addr, type)	adf_os_mem_free((addr))


/*
 * This unlikely to be popular but it dramatically reduces diffs.
 */
// #define	printf	adf_os_print
struct ieee80211com;
extern	void adf_drv_print(adf_net_handle_t, const char *, ...);
extern	const char *ath_ether_sprintf(const a_uint8_t *);

/*
 * Queue write-arounds and support routines.
 */
extern	adf_nbuf_t ieee80211_getmgtframe(a_uint8_t **frm, a_uint32_t pktlen, adf_net_handle_t anet);
#ifdef IF_ENQUEUE
#undef IF_ENQUEUE
#define	IF_ENQUEUE(_q,_skb)	adf_nbuf_queue_add(_q,_skb)
#endif

#ifdef IF_DEQUEUE
#undef IF_DEQUEUE
#define	IF_DEQUEUE(_q,_skb)	(_skb = adf_nbuf_queue_remove(_q))
#endif

#ifdef _IF_QLEN
#undef _IF_QLEN
#define	_IF_QLEN(_q)		adf_nbuf_queue_len(_q)
#endif

#ifdef IF_DRAIN
#undef IF_DRAIN
#define	IF_DRAIN(_q)		nbuf_queue_drain(_q)
#endif

extern	void skb_queue_drain(adf_nbuf_queue_t *head);

static __inline a_uint64_t
ieee80211_get_jiffies_64(void)
{
  return (a_uint64_t) adf_os_getuptime();		/* XXX not right */
}

#ifndef offset_in_page
#define	offset_in_page(p) ((unsigned long) (p) & ~PAGE_MASK)
#endif

#ifndef module_put_and_exit
#define module_put_and_exit(code) do {	\
	_MOD_DEC_USE(THIS_MODULE);	\
	do_exit(code);			\
} while (0)
#endif

/*
 * Linux uses __BIG_ENDIAN and __LITTLE_ENDIAN while BSD uses _foo
 * and an explicit _BYTE_ORDER.  Sorry, BSD got there first--define
 * things in the BSD way...
 */
#define	_LITTLE_ENDIAN	1234	/* LSB first: i386, vax */
#define	_BIG_ENDIAN	4321	/* MSB first: 68000, ibm, net */
#if defined(ADF_LITTLE_ENDIAN_MACHINE)
#define	_BYTE_ORDER	_LITTLE_ENDIAN
#elif defined(ADF_BIG_ENDIAN_MACHINE)
#define	_BYTE_ORDER	_BIG_ENDIAN
#else
#error "Please fix asm/byteorder.h"
#endif

void	ieee80211_sysctl_vattach(struct ieee80211vap *);
void	ieee80211_sysctl_vdetach(struct ieee80211vap *);

static adf_os_inline void ieee80211_mod_timer(adf_os_timer_t *timer, a_uint32_t delay)
{
    adf_os_timer_cancel(timer);
    adf_os_timer_start(timer, delay);
}


/*
 * If Linux can't tell us if irqs are disabled, then they are not.
 */ 
#ifndef irqs_disabled
#define irqs_disabled()     (0)
#endif

void	ieee80211_ioctl_vattach(struct ieee80211vap *);
void	ieee80211_ioctl_vdetach(struct ieee80211vap *);
struct intfreq;
a_int32_t	ieee80211_ioctl_create_vap(struct ieee80211com *, adf_net_wcmd_vapinfo_t *);
a_int32_t     ieee80211_change_mtu(adf_drv_handle_t hdl, adf_net_wcmd_mtu_t mtu);
a_status_t ieee80211_ioctl_getmode(adf_drv_handle_t hdl, adf_net_wcmd_phymode_t *info);
a_int32_t ieee80211_ioctl_getparam(adf_drv_handle_t hdl, adf_net_wcmd_param_t *param);
a_int32_t ieee80211_ioctl_chanswitch(adf_drv_handle_t hdl, adf_net_wcmd_chansw_t *param);




/*
 * For packet capture, define the same physical layer packet header 
 * structure as used in the wlan-ng driver 
 */
typedef struct {
	a_uint32_t did;
	a_uint16_t status;
	a_uint16_t len;
	a_uint32_t data;
} p80211item_uint32_t;
typedef struct {
	a_uint32_t msgcode;
	a_uint32_t msglen;
#define WLAN_DEVNAMELEN_MAX 16
	a_uint8_t devname[WLAN_DEVNAMELEN_MAX];
	p80211item_uint32_t hosttime;
	p80211item_uint32_t mactime;
	p80211item_uint32_t channel;
	p80211item_uint32_t rssi;
	p80211item_uint32_t sq;
	p80211item_uint32_t signal;
	p80211item_uint32_t noise;
	p80211item_uint32_t rate;
	p80211item_uint32_t istx;
	p80211item_uint32_t frmlen;
} wlan_ng_prism2_header;

typedef struct {
    a_uint16_t sa_family;      				/* address family, AF_xxx       */
    a_int8_t   sa_data[ADF_NET_WCMD_ADDR_LEN];    		/* 14 bytes of protocol address */ 	
} ieee80211_sockaddr_data_t;

typedef struct 
{
  void         *pointer;        /* Pointer to the data  (in user space) */
  a_uint16_t    length;         /* number of fields or size in bytes */
  a_uint16_t    flags;          /* Optional params */
} ieee80211_point_t;

typedef union {
  ieee80211_sockaddr_data_t addr;
  ieee80211_sockaddr_data_t ap_addr;
  ieee80211_point_t data;
} ieee80211_wevent_data_t;
/*
 * Dispatch an skb to monitor-mode vap's.  The skb is assumed
 * to have space at the front to push a wlan_ng_prims2_header.
 */
void	ieee80211_input_monitor(struct ieee80211com *, adf_nbuf_t,
		a_uint32_t mactime, a_uint32_t rssi, a_uint32_t signal,
		a_uint32_t rate);

#endif /* _NET80211_IEEE80211_LINUX_H_ */
