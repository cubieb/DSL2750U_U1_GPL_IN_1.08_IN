#ifndef _IFX_ADSL_MIB_H
#define _IFX_ADSL_MIB_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

/** \file
   This file implements the MIB interface definitions for the DSL CPE API in
   case of using RFC2662 and RFC3440
*/


#if defined(__KERNEL__) || defined (IFX_ADSL_PORT_RTEMS)


#define MIB_INTERVAL    10000    /* msec */

/*  Number of intervals */
#define INTERVAL_NUM             192 /* two days */
typedef struct ifx_adsl_mib{
   struct list_head list;
   struct timeval start_time; /* start of current interval */

   int AtucPerfLof;
   int AtucPerfLos;
   int AtucPerfEs;
   int AtucPerfInit;

   int AturPerfLof;
   int AturPerfLos;
   int AturPerfLpr;
   int AturPerfEs;

   int AturChanPerfRxBlk;
   int AturChanPerfTxBlk;
   int AturChanPerfCorrBlk;
   int AturChanPerfUncorrBlk;

   /* RFC-3440 */
   u32 AtucPerfStatFastR;
   u32 AtucPerfStatFailedFastR;
   u32 AtucPerfStatSesL;
   u32 AtucPerfStatUasL;
   u32 AturPerfStatSesL;
   u32 AturPerfStatUasL;
}ifx_adsl_mib;

typedef struct adslChanPrevTxRate{
   u32 adslAtucChanPrevTxRate;
   u32 adslAturChanPrevTxRate;
}adslChanPrevTxRate;

typedef struct adslPhysCurrStatus{
   u32 adslAtucCurrStatus;
   u32 adslAturCurrStatus;
}adslPhysCurrStatus;

typedef struct ChanType{
   int interleave;
   int fast;
   int bearchannel0;
   int bearchannel1;
}ChanType;

typedef struct mib_previous_read{
   u16 ATUC_PERF_ESS;
   u16 ATUR_PERF_ESS;
   u32 ATUR_CHAN_RECV_BLK;
   u16 ATUR_CHAN_CORR_BLK_INTL;
   u16 ATUR_CHAN_CORR_BLK_FAST;
   u16 ATUR_CHAN_UNCORR_BLK_INTL;
   u16 ATUR_CHAN_UNCORR_BLK_FAST;
   u16 ATUC_PERF_STAT_FASTR;
   u16 ATUC_PERF_STAT_FAILED_FASTR;
   u16 ATUC_PERF_STAT_SESL;
   u16 ATUC_PERF_STAT_UASL;
   u16 ATUR_PERF_STAT_SESL;
}mib_previous_read;

typedef struct mib_flags_pretime{
   struct timeval ATUC_PERF_LOSS_PTIME;
   struct timeval ATUC_PERF_LOFS_PTIME;
   struct timeval ATUR_PERF_LOSS_PTIME;
   struct timeval ATUR_PERF_LOFS_PTIME;
   struct timeval ATUR_PERF_LPR_PTIME;
}mib_flags_pretime;

#endif

#endif /* _IFX_ADSL_MIB_H */

