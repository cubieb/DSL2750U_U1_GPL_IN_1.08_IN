/*-
 * Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * $FreeBSD: src/sys/dev/ath/if_athioctl.h,v 1.5 2003/12/28 07:00:32 sam Exp $
 */

/*
 * Ioctl-related defintions for the Atheros Wireless LAN controller driver.
 */
#ifndef _DEV_ATH_ATHIOCTL_H
#define _DEV_ATH_ATHIOCTL_H

#include <wlan_hdr.h>
/*
 * 11n tx/rx stats
 */
struct ath_11n_stats_host {
    a_uint32_t   tx_pkts;            /* total tx data packets */
    a_uint32_t   tx_checks;          /* tx drops in wrong state */
    a_uint32_t   tx_drops;           /* tx drops due to qdepth limit */
    a_uint32_t   tx_queue;           /* tx pkts when h/w queue is busy */
    a_uint32_t   tx_normnobufs;      /* tx no desc for legacy packets */
    a_uint32_t   tx_schednone;       /* tx schedule pkt queue empty */
    a_uint32_t   txaggr_noskbs;      /* tx no skbs for aggr encapsualtion */
    a_uint32_t   txaggr_nobufs;      /* tx no desc for aggr */
    a_uint32_t   txaggr_badkeys;     /* tx enc key setup failures */
    a_uint32_t   rx_pkts;            /* rx pkts */
    a_uint32_t   rx_aggr;            /* rx aggregated packets */
    a_uint32_t   rx_aggrbadver;      /* rx pkts with bad version */
    a_uint32_t   rx_bars;            /* rx bars */
    a_uint32_t   rx_nonqos;          /* rx non qos-data frames */
    a_uint32_t   rx_seqreset;        /* rx sequence resets */
    a_uint32_t   rx_oldseq;          /* rx old packets */
    a_uint32_t   rx_bareset;         /* rx block ack window reset */
    a_uint32_t   rx_baresetpkts;     /* rx pts indicated due to baw resets */
    a_uint32_t   rx_dup;             /* rx duplicate pkts */
    a_uint32_t   rx_baadvance;       /* rx block ack window advanced */
    a_uint32_t   rx_recvcomp;        /* rx pkt completions */
    a_uint32_t   rx_bardiscard;      /* rx bar discarded */
    a_uint32_t   rx_barcomps;        /* rx pkts unblocked on bar reception */
    a_uint32_t   rx_barrecvs;        /* rx pkt completions on bar reception */
    a_uint32_t   rx_skipped;         /* rx pkt sequences skipped on timeout */
    a_uint32_t   rx_comp_to;         /* rx indications due to timeout */
    a_uint32_t   wd_tx_active;       /* watchdog: tx is active */
    a_uint32_t   wd_tx_inactive;     /* watchdog: tx is not active */
    a_uint32_t   wd_tx_hung;         /* watchdog: tx is hung */
    a_uint32_t   wd_spurious;        /* watchdog: spurious tx hang */
    a_uint32_t   tx_requeue;         /* filter & requeue on 20/40 transitions */
    a_uint32_t   tx_drain_tid;       /* draining tid buf queue on error */
    a_uint32_t   tx_drain_bufs;      /* buffers drained from pending tid queue */
    a_uint32_t   tx_tidpaused;       /* pausing tx on tid */
    a_uint32_t   tx_tidresumed;      /* resuming tx on tid */
    a_uint32_t   tx_unaggr_filtered; /* unaggregated tx pkts filtered */
    a_uint32_t   tx_filtered;        /* total sub-frames filtered */
    a_uint32_t   rx_rb_on;           /* total rb on-s */
    a_uint32_t   rx_rb_off;          /* total rb off-s */    
};

struct ath_stats_host {
    a_uint32_t   _ast_watchdog;   /* device reset by watchdog */
    a_uint64_t   _ast_tx_packets; /* packet sent on the interface */
    a_uint32_t   _ast_tx_discard; /* frames discarded prior to assoc */
    a_uint32_t   _ast_tx_invalid; /* frames discarded 'cuz device gone */
    a_uint32_t   _ast_tx_qstop;   /* tx queue stopped 'cuz full */
    a_uint32_t   _ast_tx_nonode;  /* tx failed 'cuz no node */    
    a_uint32_t   _ast_tx_badrate; /* tx failed 'cuz bogus xmit rate */
    a_uint32_t   _ast_tx_noack;   /* tx frames with no ack marked */    
    a_uint32_t   _ast_tx_cts; /* tx frames with cts enabled */
    a_uint32_t   _ast_tx_shortpre;/* tx frames with short preamble */
    a_uint32_t   _ast_rx_crcerr;  /* rx failed 'cuz of bad CRC */
    a_uint32_t   _ast_rx_fifoerr; /* rx failed 'cuz of FIFO overrun */
    a_uint32_t   _ast_rx_badcrypt;/* rx failed 'cuz decryption */
    a_uint32_t   _ast_rx_badmic;  /* rx failed 'cuz MIC failure */
    a_uint32_t   _ast_rx_phyerr;  /* rx PHY error summary count */
    a_uint32_t   _ast_rx_phy[64]; /* rx PHY error per-code counts */
    a_uint32_t   _ast_rx_tooshort;/* rx discarded 'cuz frame too short */
    a_uint32_t   _ast_rx_toobig;  /* rx discarded 'cuz frame too large */    
    a_uint64_t   _ast_rx_packets; /* packet recv on the interface */
    a_uint32_t   _ast_rx_mgt; /* management frames received */
    a_uint32_t   _ast_rx_ctl; /* control frames received */
    a_int8_t      _ast_rx_rssi_combined;/* rx rssi from histogram [combined]*/
    a_int8_t      _ast_rx_rssi_ctl0; /* rx rssi from histogram [ctl, chain 0] */
    a_int8_t      _ast_rx_rssi_ctl1; /* rx rssi from histogram [ctl, chain 1] */
    a_int8_t      _ast_rx_rssi_ctl2; /* rx rssi from histogram [ctl, chain 2] */
    a_int8_t      _ast_rx_rssi_ext0; /* rx rssi from histogram [ext, chain 0] */
    a_int8_t      _ast_rx_rssi_ext1; /* rx rssi from histogram [ext, chain 1] */
    a_int8_t      _ast_rx_rssi_ext2; /* rx rssi from histogram [ext, chain 2] */
    a_uint32_t   _ast_be_nobuf;   /* no skbuff available for beacon */
    a_uint32_t   _ast_per_cal;    /* periodic calibration calls */
    a_uint32_t   _ast_per_calfail;/* periodic calibration failed */
    a_uint32_t   _ast_per_rfgain; /* periodic calibration rfgain reset */
    a_uint32_t   _ast_rate_calls; /* rate control checks */
    a_uint32_t   _ast_rate_raise; /* rate control raised xmit rate */
    a_uint32_t   _ast_rate_drop;  /* rate control dropped xmit rate */
    a_uint32_t   _ast_ant_txswitch;/* tx antenna switches */
    a_uint32_t   _ast_ant_rx[8];  /* rx frames with antenna */ 
    a_uint32_t   _ast_suspend;    /* driver suspend calls */
    a_uint32_t   _ast_resume;     /* driver resume calls  */
    a_uint32_t   _ast_shutdown;   /* driver shutdown calls  */
    a_uint32_t   _ast_init;       /* driver init calls  */
    a_uint32_t   _ast_stop;       /* driver stop calls  */
    a_uint32_t   _ast_reset;      /* driver resets      */
    a_uint32_t   _ast_nodealloc;  /* nodes allocated    */
    a_uint32_t   _ast_nodefree;   /* nodes deleted      */
    a_uint32_t   _ast_keyalloc;   /* keys allocateds    */
    a_uint32_t   _ast_keydelete;   /* keys deleted    */
    a_uint32_t   _ast_bstuck;     /* beacon stuck       */
    a_uint32_t   _ast_startrecv;  /* start recv         */
    a_uint32_t   _ast_flushrecv;  /* flush recv         */
    a_uint32_t   _ast_chanchange; /* channel changes    */
    a_uint32_t   _ast_fastcc;     /* Number of fast channel changes */
    a_uint32_t   _ast_fastcc_errs;/* Number of failed fast channel changes */
    a_uint32_t   _ast_chanset;    /* channel sets       */
    a_uint32_t   _ast_cwm_mac;    /* CWM - mac mode switch */
    a_uint32_t   _ast_cwm_phy;    /* CWM - phy mode switch */
    a_uint32_t   _ast_cwm_requeue;/* CWM - requeue dest node packets */
    a_uint32_t   _ast_rx_delim_pre_crcerr; /* pre-delimiter crc errors */
    a_uint32_t   _ast_rx_delim_post_crcerr; /* post-delimiter crc errors */
    a_uint32_t   _ast_rx_decrypt_busyerr; /* decrypt busy errors */
    struct ath_11n_stats_host ast_11n_stats; /* 11n statistics */
    
};

struct ath_stats {
    struct ath_stats_host   ast_stats_hst;  /* host statistics */
    struct ath_stats_target ast_stats_tgt; /* target statistics */
};

#define ast_watchdog	ast_stats_hst._ast_watchdog
#define ast_tx_packets  ast_stats_hst._ast_tx_packets 
#define ast_tx_discard  ast_stats_hst._ast_tx_discard /* frames discarded prior to assoc */
#define ast_tx_invalid  ast_stats_hst._ast_tx_invalid /* frames discarded 'cuz device gone */
#define ast_tx_qstop    ast_stats_hst._ast_tx_qstop   /* tx queue stopped 'cuz full */
#define ast_tx_nonode   ast_stats_hst._ast_tx_nonode  /* tx failed 'cuz no node */    
#define ast_tx_badrate  ast_stats_hst._ast_tx_badrate /* tx failed 'cuz bogus xmit rate */
#define ast_tx_noack    ast_stats_hst._ast_tx_noack   /* tx frames with no ack marked */    
#define ast_tx_cts      ast_stats_hst._ast_tx_cts /* tx frames with cts enabled */
#define ast_tx_shortpre ast_stats_hst._ast_tx_shortpre /* tx frames with short preamble */
#define ast_rx_crcerr   ast_stats_hst._ast_rx_crcerr  /* rx failed 'cuz of bad CRC */
#define ast_rx_fifoerr  ast_stats_hst._ast_rx_fifoerr /* rx failed 'cuz of FIFO overrun */
#define ast_rx_badcrypt ast_stats_hst._ast_rx_badcrypt/* rx failed 'cuz decryption */
#define ast_rx_badmic   ast_stats_hst._ast_rx_badmic  /* rx failed 'cuz MIC failure */
#define ast_rx_phyerr   ast_stats_hst._ast_rx_phyerr  /* rx PHY error summary count */
#define ast_rx_phy      ast_stats_hst._ast_rx_phy /* rx PHY error per-code counts */
#define ast_rx_tooshort ast_stats_hst._ast_rx_tooshort/* rx discarded 'cuz frame too short */
#define ast_rx_toobig   ast_stats_hst._ast_rx_toobig  /* rx discarded 'cuz frame too large */    
#define ast_rx_packets  ast_stats_hst._ast_rx_packets /* packet recv on the interface */
#define ast_rx_mgt      ast_stats_hst._ast_rx_mgt /* management frames received */
#define ast_rx_ctl      ast_stats_hst._ast_rx_ctl /* control frames received */
#define ast_rx_rssi_combined     ast_stats_hst._ast_rx_rssi_combined /* rx rssi from histogram [combined]*/
#define ast_rx_rssi_ctl0         ast_stats_hst._ast_rx_rssi_ctl0 /* rx rssi from histogram [ctl, chain 0] */
#define ast_rx_rssi_ctl1         ast_stats_hst._ast_rx_rssi_ctl1 /* rx rssi from histogram [ctl, chain 1] */
#define ast_rx_rssi_ctl2         ast_stats_hst._ast_rx_rssi_ctl2 /* rx rssi from histogram [ctl, chain 2] */
#define ast_rx_rssi_ext0         ast_stats_hst._ast_rx_rssi_ext0 /* rx rssi from histogram [ext, chain 0] */
#define ast_rx_rssi_ext1         ast_stats_hst._ast_rx_rssi_ext1 /* rx rssi from histogram [ext, chain 1] */
#define ast_rx_rssi_ext2         ast_stats_hst._ast_rx_rssi_ext2 /* rx rssi from histogram [ext, chain 2] */
#define ast_be_nobuf     ast_stats_hst._ast_be_nobuf   /* no skbuff available for beacon */
#define ast_per_cal      ast_stats_hst._ast_per_cal    /* periodic calibration calls */
#define ast_per_calfail  ast_stats_hst._ast_per_calfail/* periodic calibration failed */
#define ast_per_rfgain   ast_stats_hst._ast_per_rfgain /* periodic calibration rfgain reset */
#define ast_rate_calls   ast_stats_hst._ast_rate_calls /* rate control checks */
#define ast_rate_raise   ast_stats_hst._ast_rate_raise /* rate control raised xmit rate */
#define ast_rate_drop    ast_stats_hst._ast_rate_drop  /* rate control dropped xmit rate */
#define ast_ant_txswitch ast_stats_hst._ast_ant_txswitch/* tx antenna switches */
#define ast_ant_rx       ast_stats_hst._ast_ant_rx  /* rx frames with antenna */ 
#define ast_suspend      ast_stats_hst._ast_suspend    /* driver suspend calls */
#define ast_resume       ast_stats_hst._ast_resume     /* driver resume calls  */
#define ast_shutdown     ast_stats_hst._ast_shutdown   /* driver shutdown calls  */
#define ast_init         ast_stats_hst._ast_init       /* driver init calls  */
#define ast_stop         ast_stats_hst._ast_stop       /* driver stop calls  */
#define ast_reset        ast_stats_hst._ast_reset      /* driver resets      */
#define ast_nodealloc    ast_stats_hst._ast_nodealloc  /* nodes allocated    */
#define ast_nodefree     ast_stats_hst._ast_nodefree   /* nodes deleted      */
#define ast_keyalloc     ast_stats_hst._ast_keyalloc   /* keys allocateds    */
#define ast_keydelete    ast_stats_hst._ast_keydelete   /* keys allocateds    */
#define ast_bstuck       ast_stats_hst._ast_bstuck     /* beacon stuck       */
#define ast_startrecv    ast_stats_hst._ast_startrecv  /* start recv         */
#define ast_flushrecv    ast_stats_hst._ast_flushrecv  /* flush recv         */
#define ast_chanchange   ast_stats_hst._ast_chanchange /* channel changes    */
#define ast_fastcc       ast_stats_hst._ast_fastcc     /* Number of fast channel changes */
#define ast_fastcc_errs  ast_stats_hst._ast_fastcc_errs /* Number of failed fast channel changes */
#define ast_chanset      ast_stats_hst._ast_chanset    /* channel sets       */
#define ast_cwm_mac      ast_stats_hst._ast_cwm_mac    /* CWM - mac mode switch */
#define ast_cwm_phy      ast_stats_hst._ast_cwm_phy    /* CWM - phy mode switch */
#define ast_cwm_requeue  ast_stats_hst._ast_cwm_requeue/* CWM - requeue dest node packets */
#define ast_rx_delim_pre_crcerr      ast_stats_hst._ast_rx_delim_pre_crcerr /* pre-delimiter crc errors */
#define ast_rx_delim_post_crcerr     ast_stats_hst._ast_rx_delim_post_crcerr /* post-delimiter crc errors */
#define ast_rx_decrypt_busyerr       ast_stats_hst._ast_rx_decrypt_busyerr /* decrypt busy errors */	

struct ath_diag {
    char    ad_name[ADF_NET_WCMD_NAME_SIZE];  /* if name, e.g. "ath0" */
    a_uint16_t ad_id;
#define ATH_DIAG_DYN    0x8000      /* allocate buffer in caller */
#define ATH_DIAG_IN 0x4000      /* copy in parameters */
#define ATH_DIAG_OUT    0x0000      /* copy out results (always) */
#define ATH_DIAG_ID 0x0fff
    a_uint16_t ad_in_size;       /* pack to fit, yech */
    char * ad_in_data;
    char * ad_out_data;
    a_uint32_t   ad_out_size;

};

struct ath_cwmdbg {
    a_uint32_t   dc_cmd;
    a_uint32_t   dc_arg;
};

#define ATH_DBGCWM_CMD_EVENT    0   /* send event */
#define ATH_DBGCWM_CMD_CTL  1  /* set control channel busy indicator */
#define ATH_DBGCWM_CMD_EXT  2  /* set extension channel busy indicator */
#define ATH_DBGCWM_CMD_VCTL 3  /* set virtual control channel busy indicator */
#define ATH_DBGCWM_CMD_VEXT 4  /* set virtual extension channel busy indicator*/

struct ath_cwminfo {
    /* current state */
    a_uint32_t   ci_chwidth; /* channel width */
    a_uint32_t   ci_macmode; /* MAC mode */
    a_uint32_t   ci_phymode; /* PHY mode */
    a_uint32_t	ci_extbusyper; /* extension busy (percent) */
};

#ifdef __linux__
#define SIOCGATHSTATS       (SIOCDEVPRIVATE+0)
#define SIOCGATHDIAG        (SIOCDEVPRIVATE+1)
#define SIOCGATHCWMINFO     (SIOCDEVPRIVATE+2)
#define SIOCGATHCWMDBG      (SIOCDEVPRIVATE+3)
#define SIOCGATHSTATSCLR    (SIOCDEVPRIVATE+4)
#define SIOCGATHPHYERR      (SIOCDEVPRIVATE+5)
#else
#ifdef TODO // shim will define this?
#define SIOCGATHSTATS       _IOWR('i', 137, struct ifreq)
#define SIOCGATHDIAG        _IOWR('i', 138, struct ath_diag)
#define SIOCGATHCWMINFO     _IOWR('i', 139, struct ath_cwminfo)
#define SIOCGATHCWMDBG 	    _IOWR('i', 140, struct ath_cwmdbg)
#define SIOCGATHSTATSCLR    _IOWR('i', 141, struct ifreq)
#else
#define SIOCGATHSTATS       137
#define SIOCGATHDIAG        138
#define SIOCGATHCWMINFO     139
#define SIOCGATHCWMDBG 	    140
#define SIOCGATHSTATSCLR    141
#endif
#endif
#endif /* _DEV_ATH_ATHIOCTL_H */
