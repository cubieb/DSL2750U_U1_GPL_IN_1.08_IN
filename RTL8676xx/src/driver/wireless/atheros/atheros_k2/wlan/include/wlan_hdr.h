#ifndef _WLAN_HDR_H
#define _WLAN_HDR_H

/* Please make sure the size of ALL headers is on word alignment */

#define	M_LINK0		0x01			/* frame needs WEP encryption */
#define	M_FF		0x02			/* fast frame */
#define	M_PWR_SAV	0x04			/* bypass power save handling */
#define M_UAPSD		0x08			/* frame flagged for u-apsd handling */

#define RX_STATS_SIZE 9
#define UNALIGNED_ETHERBUFFER_WORKAROUND
struct  rx_frame_header {
    a_uint32_t rx_stats[RX_STATS_SIZE];
};

#define    ATH_DATA_TYPE_AGGR       0x1 
#define    ATH_DATA_TYPE_NON_AGGR   0x2

#define    ATH_SHORT_PREAMBLE       0x1
#define    ATH_DH_FLAGS_PRIVACY		0x2
#define    ATH_DH_FLAGS_MOREDATA    0x4

typedef struct _mgt_header {
    a_uint8_t   ni_index;
    a_uint8_t   vap_index;
    a_uint8_t   tidno;
    a_uint8_t   flags;
    a_int8_t    keytype;
    a_int8_t    keyix;
    a_int16_t   byte_reserve;
} ath_mgt_hdr_t;


typedef struct _beacon_header {
    a_uint8_t   vap_index;   
    a_uint8_t   len_changed;    
    a_uint16_t  reserved;
}ath_beacon_hdr_t;

#define	M_LINK0		0x01			/* frame needs WEP encryption */
#define M_UAPSD		0x08			/* frame flagged for u-apsd handling */

typedef struct __data_header {

#ifdef UNALIGNED_ETHERBUFFER_WORKAROUND
    a_uint32_t  align_info ; /* workaround for unaligned frames */
#endif

    a_uint8_t   datatype;
    a_uint8_t   ni_index;
    a_uint8_t   vap_index;
    a_uint8_t   tidno;
    a_uint32_t  flags;  
    a_int8_t    keytype;
    a_int8_t    keyix;
    a_uint8_t   keyid;
    a_int8_t    byte_reserve;
    a_uint8_t   reserved[24];       /*padding till config pipe fixes reserve*/
} ath_data_hdr_t;

#define RX_HEADER_SPACE     HTC_HDR_LENGTH + sizeof(struct rx_frame_header)

struct ieee80211com_target {
    a_uint32_t    ic_flags;       
    a_uint32_t    ic_flags_ext;         /* extension of state flags */
    a_uint32_t    ic_ampdu_limit ;      /* A-AMPDU length limit*/
    a_uint8_t     ic_ampdu_subframes;   /* A AMPDU subfrmae limit */
    a_uint8_t 	  ic_tx_chainmask;
    a_uint8_t     ic_tx_chainmask_legacy;
    a_uint8_t	  ic_rtscts_ratecode;		 
    a_uint8_t	  ic_protmode;		 
};

#if defined(PROJECT_K2) || defined(K2)

#if defined(K2_SINGLE_VAP)
#define ATH_NODE_MAX 16      /* max no. of nodes */
#define ATH_VAP_MAX  1       /* max no. of vaps */
#else
#define ATH_NODE_MAX 14      /* max no. of nodes */
#define ATH_VAP_MAX  4       /* max no. of vaps */
#endif

#else
#define ATH_NODE_MAX 30       /* max no. of nodes */
#define ATH_VAP_MAX  4       /* max no. of vaps */
#endif


struct ieee80211vap_target 
{
    a_uint8_t               iv_vapindex;
    a_uint8_t               iv_des_bssid[IEEE80211_ADDR_LEN];
    enum ieee80211_opmode   iv_opmode;             
    a_uint8_t               iv_myaddr[IEEE80211_ADDR_LEN];
    a_uint8_t               iv_ni_bssid[IEEE80211_ADDR_LEN];
    a_uint32_t              iv_flags;                   
    a_uint32_t              iv_flags_ext;              
    a_uint16_t              iv_ps_sta;               
    a_uint16_t              iv_rtsthreshold;
    a_uint8_t               iv_ath_cap;
    a_uint8_t               iv_node ;
    a_int8_t                iv_mcast_rate;
    a_uint8_t               iv_nodeindex;
    struct ieee80211_node_target *iv_bss;
};


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

struct ieee80211_node_target
{
    a_uint16_t        ni_associd;	/* assoc response */
    a_uint16_t        ni_txpower;	/* current transmit power */
    //struct ieee80211_key_target	ni_ucastkey;	/* unicast key */
    a_uint8_t       ni_macaddr[IEEE80211_ADDR_LEN];
    a_uint8_t       ni_bssid[IEEE80211_ADDR_LEN];
    a_uint8_t       ni_nodeindex;
    a_uint8_t       ni_vapindex;
    a_uint8_t       ni_vapnode;
    a_uint16_t      ni_flags;	/* special-purpose state */
		/* 11n Capabilities */
    a_uint16_t 		ni_htcap;	/* HT capabilities */
    a_uint8_t       ni_valid ;
    a_uint16_t      ni_capinfo;	
    struct ieee80211com_target  *ni_ic;
    struct ieee80211vap_target	*ni_vap;
    a_uint16_t                   ni_txseqmgmt;    
    a_uint8_t                    ni_is_vapnode;	 
    a_uint16_t                   ni_maxampdu;	/* maximum rx A-MPDU length */
    a_uint64_t      ni_ucast_keytsc;                  /* unicast key transmit sequence counter */
    a_uint64_t      ni_mcast_keytsc;                  /* multicast key transmit sequence counter */
#ifdef MAGPIE_UAPSD
    a_uint8_t                    ni_uapsd;  /* U-APSD per-node WMM STA Qos Info field */
    a_uint8_t ni_uapsd_maxsp; /* maxsp from flags above */
    struct ieee80211_node_tgt_uapsd ni_uapsd_pvt;
#endif
};


/*
 * 11n tx/rx stats
 */
struct ath_11n_stats_target {
    a_uint32_t   tx_tgt;	     /* tx data pkts recieved on target */  
    a_uint32_t   tx_nframes;         /* no. of frames aggregated */
    a_uint32_t   tx_comps;           /* tx completions */
    a_uint32_t   tx_qnull;           /* txq empty occurences */
    a_uint32_t   tx_compunaggr;      /* tx unaggregated frame completions */ 
    a_uint32_t   tx_compaggr;        /* tx aggregated completions */
    a_uint32_t   tx_retries;         /* tx retries of sub frames */
    a_uint32_t   txaggr_single;      /* tx frames not aggregated */
    a_uint32_t   txaggr_compgood;    /* tx aggr good completions */
    a_uint32_t   txaggr_compretries; /* tx aggr unacked subframes */
    a_uint32_t   txaggr_prepends;    /* tx aggr old frames requeued */
    a_uint32_t   txaggr_data_urun;   /* data underrun for an aggregate */
    a_uint32_t   txaggr_delim_urun;  /* delimiter underrun for an aggr */
    a_uint32_t   txaggr_errlast;     /* tx aggr: last sub-frame failed */
    a_uint32_t   txaggr_longretries; /* tx aggr h/w long retries */
    a_uint32_t   tx_drain_txq;       /* draining tx queue on error */
    a_uint32_t   tx_minqdepth;       /* tx when h/w queue depth is low */
    a_uint32_t   tx_stopfiltered;    /* tx pkts filtered for requeueing */
    a_uint32_t   tx_noskbs;          /* tx no skbs for encapsulations */
    a_uint32_t   tx_nobufs;          /* tx no descriptors */
    a_uint32_t   tx_badsetups;       /* tx key setup failures */
    a_uint32_t   tx_bars;            /* tx bars sent */
    a_uint32_t   txbar_xretry;       /* tx bars excessively retried */
    a_uint32_t   txbar_compretries;  /* tx bars retried */
    a_uint32_t   txbar_errlast;      /* tx bars last frame failed */
    a_uint32_t   txunaggr_xretry;    /* tx unaggregated excessive retries */
    a_uint32_t   tx_bawadv;          /* tx block ack window advanced */
    a_uint32_t   tx_bawretries;      /* tx block ack window retries */
    a_uint32_t   tx_bawnorm;         /* tx block ack window additions */
    a_uint32_t   tx_bawupdates;      /* tx block ack window updates */
    a_uint32_t   tx_bawupdtadv;      /* tx block ack window advances */
    a_uint32_t   tx_xretries;        /* tx excessive retries of aggr */
    a_uint32_t   txaggr_schedwindow; /* tx no frame scheduled: baw lim */
    a_uint32_t   txaggr_compxretry;  /* tx aggr excessive retries */
    a_uint32_t   txunaggr_compretries; /* tx non-aggr unacked subframes */
    a_uint32_t   txaggr_filtered;    /* filtered aggr packet */
    a_uint32_t   txaggr_fifo;        /* fifo underrun of aggregate */
    a_uint32_t   txaggr_xtxop;       /* txop exceeded for an aggregate */
    a_uint32_t   txaggr_desc_cfgerr; /* aggr descriptor config error */
    a_uint32_t   txunaggr_errlast;   /* tx non-aggr: last frame failed */
    a_uint32_t   txaggr_shortretries;/* tx aggr h/w short retries */
    a_uint32_t   txaggr_timer_exp;   /* tx aggr : tx timer expired */
    a_uint32_t   txaggr_babug;       /* tx aggr : BA bug */
    a_uint32_t   tx_aggr_filtered;   /* aggregated tx pkts filtered */
    a_uint32_t   tx_tgt_drain_bufs;  /* draining target buffers */
};

#define NTRENTS	        128	
struct ath_ampdu_trc_entry {
    a_uint16_t   tre_seqst;  	    /* starting sequence of aggr */
    a_uint16_t   tre_baseqst;  	    /* starting sequence of ba */
    a_uint32_t   tre_npkts;          /* packets in aggregate */
    a_uint32_t   tre_aggrlen;        /* aggregation length */
    a_uint32_t   tre_bamap0;         /* block ack bitmap word 0 */
    a_uint32_t   tre_bamap1;         /* block ack bitmap word 1 */
};
struct ath_ampdu_trc {
    a_uint32_t   tr_head;
    a_uint32_t   tr_tail;
    struct ath_ampdu_trc_entry tr_ents[NTRENTS];
};

#define AMPDU_TRCINIT(_sc) do {                \
    (_sc)->sc_stats.ast_ampdu_trc.tr_head = 0; \
    (_sc)->sc_stats.ast_ampdu_trc.tr_tail = 0; \
} while(0)

#define AMPDU_TRCINC(_pt) (_pt) = (((_pt) + 1) % NTRENTS)
#define AMPDU_TRCADD(_sc, _npkts, _al, _seqst, _seqstba, _bamap0, _bamap1) do {         \
    if (!(_sc)->noampdutrc) {                                                           \
    struct ath_ampdu_trc_entry *__e;                                                    \
    __e = &(_sc)->sc_stats.ast_ampdu_trc.tr_ents[(_sc)->sc_stats.ast_ampdu_trc.tr_tail];\
    __e->tre_seqst = (_seqst);								\
    __e->tre_baseqst = (_seqstba);								\
    __e->tre_npkts = (_npkts);                                                          \
    __e->tre_aggrlen = (_al);                                                           \
    __e->tre_bamap0 = (_bamap0);                                                        \
    __e->tre_bamap1 = (_bamap1);                                                        \
    AMPDU_TRCINC((_sc)->sc_stats.ast_ampdu_trc.tr_tail);                                \
    if ((_sc)->sc_stats.ast_ampdu_trc.tr_head == (_sc)->sc_stats.ast_ampdu_trc.tr_tail) \
        AMPDU_TRCINC((_sc)->sc_stats.ast_ampdu_trc.tr_head);                            \
    }                                                                                   \
} while (0)

#define AMPDU_TRCEND(_sc) (_sc)->noampdutrc = 1


struct ath_stats_target {
    a_uint32_t   ast_hardware;   /* fatal hardware error interrupts */
    a_uint32_t   ast_bmiss;  /* beacon miss interrupts */
    a_uint32_t   ast_rxorn;  /* rx overrun interrupts */
    a_uint32_t   ast_rxeol;  /* rx eol interrupts */
    a_uint32_t   ast_txurn;  /* tx underrun interrupts */
    a_uint32_t   ast_txto;   /* tx timeout interrupts */
    a_uint32_t   ast_cst;    /* carrier sense timeout interrupts */
    a_uint32_t   ast_mib;    /* mib interrupts */
    a_uint32_t   ast_tx_mgmt;    /* management frames transmitted */
    a_uint32_t   ast_tx_encap;   /* tx encapsulation failed */
    a_uint32_t   ast_tx_nobuf;   /* tx failed 'cuz no tx buffer (data) */
    a_uint32_t   ast_tx_nobufmgt;/* tx failed 'cuz no tx buffer (mgmt)*/
    a_uint32_t   ast_tx_xretries;/* tx failed 'cuz too many retries */
    a_uint32_t   ast_tx_fifoerr; /* tx failed 'cuz FIFO underrun */
    a_uint32_t   ast_tx_filtered;/* tx failed 'cuz xmit filtered */
    a_uint32_t   ast_tx_timer_exp;/* tx timer expired */
    a_uint32_t   ast_tx_shortretry;/* tx on-chip retries (short) */
    a_uint32_t   ast_tx_longretry;/* tx on-chip retries (long) */
    a_uint32_t   ast_tx_rts; /* tx frames with rts enabled */
    a_uint32_t   ast_tx_altrate; /* tx frames with alternate rate */
    a_uint32_t   ast_tx_protect; /* tx frames with protection */
    a_uint32_t   ast_rx_orn; /* rx failed 'cuz of desc overrun */
    a_uint32_t   ast_rx_nobuf;   /* rx setup failed 'cuz no skbuff */
    a_int8_t      ast_tx_rssi_combined;/* tx rssi of last ack [combined] */
    a_int8_t      ast_tx_rssi_ctl0;    /* tx rssi of last ack [ctl,chain0] */
    a_int8_t      ast_tx_rssi_ctl1;    /* tx rssi of last ack [ctl,chain1] */
    a_int8_t      ast_tx_rssi_ctl2;    /* tx rssi of last ack [ctl,chain2] */
    a_int8_t      ast_tx_rssi_ext0;    /* tx rssi of last ack [ext,chain0] */
    a_int8_t      ast_tx_rssi_ext1;    /* tx rssi of last ack [ext,chain1] */
    a_int8_t      ast_tx_rssi_ext2;    /* tx rssi of last ack [ext,chain2] */
    a_uint32_t   ast_be_xmit;        /* beacon xmit */
    a_uint32_t   ast_ant_defswitch;/* rx/default antenna switches */
    a_uint32_t   ast_ant_tx[8];  /* tx frames with antenna */
    a_uint32_t   ast_draintxq;   /* drain tx queue     */
    a_uint32_t   ast_stopdma;    /* stop tx queue dma  */
    a_uint32_t   ast_stoprecv;   /* stop recv          */
    struct ath_11n_stats_target ast_11n_stats_tgt; /* 11n statistics */
    /*struct ath_ampdu_trc ast_ampdu_trc;*/ /* ampdu trc */
};

struct ath_aggr_info {
    a_uint8_t nodeindex;
    a_uint8_t tidno;
    a_uint8_t aggr_enable;
    a_uint8_t padding;
};    

struct wmi_data_delba {
    a_uint8_t  ni_nodeindex;
    a_uint8_t  tidno;
    a_uint8_t  initiator;
    a_uint8_t reasoncode;
};

#endif 
