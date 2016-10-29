/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_PM_CORE_H
#define _DRV_DSL_CPE_PM_CORE_H

#ifdef __cplusplus
   extern "C" {
#endif

#include "drv_dsl_cpe_api.h"

/** \file
   This file specifies the internal functions that are used for the Performance
   Monitoring specific implementation of the ioctl interface.
   It is intendet to be used within the DSL CPE_API driver ONLY.
*/

/** \addtogroup DRV_DSL_CPE_PM
 @{ */

#if defined(INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS)
   #if !defined(INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS)
      #error "PM module can't support Channel thresholds while the Channel Counters are disabled!"
   #endif
#endif

#if defined(INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS)
   #if !defined(INCLUDE_DSL_CPE_PM_LINE_COUNTERS)
      #error "PM module can't support Line thresholds while the Line Counters are disabled!"
   #endif
#endif

#if defined(INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS)
   #if !defined(INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS)
      #error "PM module can't support Data Path thresholds while the Data Path Counters are disabled!"
   #endif
#endif

#if !defined(INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS) && \
    !defined(INCLUDE_DSL_CPE_PM_LINE_COUNTERS) && \
    !defined(INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS) && \
     defined(INCLUDE_DSL_PM)
   #error "None of the PM module counters are enabled while the PM module is enabled!"
#endif

#if !defined(INCLUDE_DSL_CPE_PM_HISTORY) && !defined(INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS) && \
    !defined(INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS) && defined(INCLUDE_DSL_PM)
   #error "Please enable PM History or PM Showtime or Total counters in the enabled PM module"
#endif

#define DSL_PM_COUNTER_ES_MAX_VALUE               ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_LOFS_MAX_VALUE             ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_LOSS_MAX_VALUE             ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_SES_MAX_VALUE              ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_UAS_MAX_VALUE              ((DSL_uint32_t)0xFFFFFFFF)

#define DSL_PM_COUNTER_FEC_MAX_VALUE              ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_CODEVIOLATIONS_MAX_VALUE   ((DSL_uint32_t)0xFFFFFFFF)

#define DSL_PM_COUNTER_CRCP_MAX_VALUE             ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_CRCPP_MAX_VALUE            ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_CVP_MAX_VALUE              ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_CVPP_MAX_VALUE             ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_HEC_MAX_VALUE              ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_IBE_MAX_VALUE              ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_TOTALCELL_MAX_VALUE        ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_UTOTALCELL_MAX_VALUE       ((DSL_uint32_t)0xFFFFFFFF)

#define DSL_PM_COUNTER_FFINIT_MAX_VALUE           ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_FSINIT_MAX_VALUE           ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_FINIT_MAX_VALUE            ((DSL_uint32_t)0xFFFFFFFF)
#define DSL_PM_COUNTER_SINIT_MAX_VALUE            ((DSL_uint32_t)0xFFFFFFFF)

/** PM module thread name*/
#define DSL_PM_NE_TASK_NAME   "cpe_pm_ne"
#define DSL_PM_FE_TASK_NAME   "cpe_pm_fe"

/** PM module thread minimum poll time (msec)*/
#define DSL_PM_COUNTER_MIN_POLLING_CYCLE   (500)

/** PM module thread default poll time (msec)*/
#define DSL_PM_COUNTER_POLLING_CYCLE   (1000)

#define DSL_PM_COUNTER_FE_POLLING_FACTOR   (10)

/** number of seconds in the 15 minutes interval. */
#define DSL_PM_15MIN   (900)

#if ((DSL_PM_15MIN * 1000) < DSL_PM_COUNTER_POLLING_CYCLE) || \
      ((DSL_PM_15MIN * 1000) < \
      (DSL_PM_COUNTER_FE_POLLING_FACTOR * DSL_PM_COUNTER_POLLING_CYCLE))
#error PM counters polling time is incorrect, please fix it
#endif

/** number of 15 min intervals per day */
#define DSL_PM_15MIN_PER_DAY   (96)

/** number of msec in the 1 day interval. */
#define DSL_PM_1DAY   (DSL_PM_15MIN*DSL_PM_15MIN_PER_DAY)

#define DSL_PM_MSEC   (1000)

#define DSL_PM_TIME_GET(timevalstruct) do_gettimeofday(timevalstruct)

/** Helper macro for PM context retrieving from \ref DSL_Context_t structure. */
#define DSL_PM_CONTEXT(X)   ((DSL_PM_Context*)((X)->PM))

#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
   #define DSL_PM_PTR_CHANNEL_COUNTERS(ptr, dir) \
      (dir==DSL_NEAR_END ? ptr->data_ne : ptr.data_fe)

/**
   Macro to get access to the current channel counters in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_COUNTERS_CURR(channel,dir) \
      (dir==DSL_NEAR_END?\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.recCurr.data_ne[channel]:\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.recCurr.data_fe[channel])

/**
   Macro to get access to the absolute channel counters in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_COUNTERS_ABS(channel,dir) \
      (dir==DSL_NEAR_END?\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.recAbs.data_ne[channel]:\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.recAbs.data_fe[channel])

/**
   Macro to get access to 15 min channel counters in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_COUNTERS_15MIN(idx,channel,dir) \
      (dir==DSL_NEAR_END?\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.rec15min[idx].data_ne[channel]:\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.rec15min[idx].data_fe[channel])

/**
   Macro to get access to 1 day channel counters in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_COUNTERS_1DAY(idx,channel,dir) \
      (dir==DSL_NEAR_END?\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.rec1day[idx].data_ne[channel]:\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.rec1day[idx].data_fe[channel])

/**
   Macro to get access to Showtime channel counters in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_COUNTERS_SHOWTIME(idx,channel,dir) \
      (dir==DSL_NEAR_END?\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.recShowtime[idx].data_ne[channel]:\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.recShowtime[idx].data_fe[channel])

/**
   Macro to get access to total channel counters in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_COUNTERS_TOTAL(channel,dir) \
      (dir==DSL_NEAR_END?\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.recTotal.data_ne[channel]:\
      &DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.recTotal.data_fe[channel])

/**
      Macro to get access to the 15-min channel history data in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_HISTORY_15MIN() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.hist15min)

/**
      Macro to get access to the 1-day channel history data in the PM context
   */
   #define DSL_PM_PTR_CHANNEL_HISTORY_1DAY() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.hist1day)

/**
      Macro to get access to the Showtime channel history data in the PM context
   */
   #define DSL_PM_PTR_CHANNEL_HISTORY_SHOWTIME() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.histShowtime)


/**
      Macro to get access to the channel threshold indication data in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_THRESHOLD_IND(dir) \
      dir==DSL_NEAR_END?\
      (DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.thresholdInd.ind_ne):\
      (DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.thresholdInd.ind_fe)

/**
      Macro to get access to the channel 15-min interval thresholds data in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_THRESHOLD_15MIN(dir) \
      dir==DSL_NEAR_END?\
      (DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.thresholds15min.data_ne):\
      (DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.thresholds15min.data_fe)

/**
      Macro to get access to the channel 1-day interval thresholds data in the PM context
*/
   #define DSL_PM_PTR_CHANNEL_THRESHOLD_1DAY(dir) \
      dir==DSL_NEAR_END?\
      (DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.thresholds1day.data_ne):\
      (DSL_PM_CONTEXT(pContext)->pCounters->channelCounters.thresholds1day.data_fe)

#endif /** #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
   #define DSL_PM_PTR_DATAPATH_COUNTERS(ptr, dir) \
      (dir==DSL_NEAR_END ? ptr->data_ne : ptr.data_fe)

/**
   Macro to get access to the current channel counters in the PM context
*/
   #define DSL_PM_PTR_DATAPATH_COUNTERS_CURR(channel,dir) \
      (dir==DSL_NEAR_END?\
      &DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.recCurr.data_ne[channel]:\
      &DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.recCurr.data_fe[channel])

/**
   Macro to get access to the absolute channel counters in the PM context
*/
   #define DSL_PM_PTR_DATAPATH_COUNTERS_ABS(channel,dir) \
      (dir==DSL_NEAR_END?\
      &DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.recAbs.data_ne[channel]:\
      &DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.recAbs.data_fe[channel])

/**
      Macro to get access to 15 min data path counters in the PM context
*/
   #define DSL_PM_PTR_DATAPATH_COUNTERS_15MIN(idx,channel,dir) \
      dir==DSL_NEAR_END?\
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.rec15min[idx].data_ne[channel]):\
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.rec15min[idx].data_fe[channel])

   /**
      Macro to get access to 1 day data path counters in the PM context
   */
   #define DSL_PM_PTR_DATAPATH_COUNTERS_1DAY(idx,channel,dir) \
      dir==DSL_NEAR_END?\
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.rec1day[idx].data_ne[channel]):\
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.rec1day[idx].data_fe[channel])

   /**
      Macro to get access to Showtime data path counters in the PM context
   */
   #define DSL_PM_PTR_DATAPATH_COUNTERS_SHOWTIME(idx,channel,dir) \
      dir==DSL_NEAR_END?\
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.recShowtime[idx].data_ne[channel]):\
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.recShowtime[idx].data_fe[channel])

/**
   Macro to get access to total channel counters in the PM context
*/
   #define DSL_PM_PTR_DATAPATH_COUNTERS_TOTAL(channel,dir) \
      dir==DSL_NEAR_END?\
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.recTotal.data_ne[channel]):\
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.recTotal.data_fe[channel])

/**
      Macro to get access to the 15-min data path history data in the PM context
*/
   #define DSL_PM_PTR_DATAPATH_HISTORY_15MIN() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.hist15min)

/**
      Macro to get access to the 1-day data path history data in the PM context
*/
   #define DSL_PM_PTR_DATAPATH_HISTORY_1DAY() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.hist1day)

/**
      Macro to get access to the 1-day data path history data in the PM context
*/
   #define DSL_PM_PTR_DATAPATH_HISTORY_SHOWTIME() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.histShowtime)

   /**
      Macro to get access to the data path threshold indication data in the PM context
   */
   #define DSL_PM_PTR_DATAPATH_THRESHOLD_IND(dir) \
      dir==DSL_NEAR_END?\
      (DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.thresholdInd.ind_ne):\
      (DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.thresholdInd.ind_fe)

   /**
      Macro to get access to the data path 15-min interval thresholds data in the PM context
   */
   #define DSL_PM_PTR_DATAPATH_THRESHOLD_15MIN(dir) \
      dir==DSL_NEAR_END?\
      (DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.thresholds15min.data_ne):\
      (DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.thresholds15min.data_fe)

   /**
      Macro to get access to the data path 1-day interval thresholds data in the PM context
   */
   #define DSL_PM_PTR_DATAPATH_THRESHOLD_1DAY(dir) \
      dir==DSL_NEAR_END?\
      (DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.thresholds1day.data_ne):\
      (DSL_PM_CONTEXT(pContext)->pCounters->dataPathCounters.thresholds1day.data_fe)
#endif /** #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/


#define DSL_PM_PTR_LINE_SEC_COUNTERS(ptr,dir) \
   (dir==DSL_NEAR_END ? &(ptr->sec_ne) : &(ptr->sec_fe))

/**
      Macro to get access to the absolute line sec counters in the PM context
*/
#define DSL_PM_PTR_LINE_SEC_COUNTERS_ABS(dir) \
   (dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recAbs.sec_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recAbs.sec_fe))


/**
      Macro to get access to the current line sec counters in the PM context
*/
#define DSL_PM_PTR_LINE_SEC_COUNTERS_CURR(dir) \
   (dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recCurr.sec_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recCurr.sec_fe))

/**
      Macro to get access to 15 min line sec counters in the PM context
*/
#define DSL_PM_PTR_LINE_SEC_COUNTERS_15MIN(idx,dir) \
   (dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.rec15min[idx].sec_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.rec15min[idx].sec_fe))


/**
      Macro to get access to 1 day line sec counters in the PM context
*/
#define DSL_PM_PTR_LINE_SEC_COUNTERS_1DAY(idx,dir) \
   (dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.rec1day[idx].sec_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.rec1day[idx].sec_fe))

/**
      Macro to get access to Showtime line sec counters in the PM context
*/
#define DSL_PM_PTR_LINE_SEC_COUNTERS_SHOWTIME(idx,dir) \
   (dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recShowtime[idx].sec_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recShowtime[idx].sec_fe))

/**
   Macro to get access to total line sec counters in the PM context
*/
#define DSL_PM_PTR_LINE_SEC_COUNTERS_TOTAL(dir) \
   dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recTotal.sec_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recTotal.sec_fe)

/**
   Macro to get access to line sec aux data in the PM context
*/
#define DSL_PM_PTR_LINE_SEC_AUX_DATA(dir) \
  (dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.auxData.sec_data_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.auxData.sec_data_fe))

/**
   Macro to get access to the 15-min line history data in the PM context
*/
#define DSL_PM_PTR_LINE_HISTORY_15MIN() \
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.hist15min)

/**
   Macro to get access to the 1-day line history data in the PM context
*/
#define DSL_PM_PTR_LINE_HISTORY_1DAY() \
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.hist1day)

/**
   Macro to get access to the Showtime line history data in the PM context
*/
#define DSL_PM_PTR_LINE_HISTORY_SHOWTIME() \
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.histShowtime)

/**
   Macro to get access to the line threshold indication data in the PM context
*/
#define DSL_PM_PTR_LINE_THRESHOLD_IND(dir) \
   dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.thresholdInd.ind_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.thresholdInd.ind_fe)

   /**
      Macro to get access to the line 15-min interval thresholds data in the PM context
   */
#define DSL_PM_PTR_LINE_SEC_THRESHOLD_15MIN(dir) \
   dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.thresholds15min.sec_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.thresholds15min.sec_fe)

/**
   Macro to get access to the line 1-day interval thresholds data in the PM context
*/
#define DSL_PM_PTR_LINE_SEC_THRESHOLD_1DAY(dir) \
   dir==DSL_NEAR_END?\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.thresholds1day.sec_ne):\
   &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.thresholds1day.sec_fe)


#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
   #define DSL_PM_PTR_LINE_INIT_COUNTERS(ptr) &(ptr->init)

   /**
      Macro to get access to current line init counters in the PM context
   */
   #define DSL_PM_PTR_LINE_INIT_COUNTERS_CURR() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recCurr.init)

   /**
      Macro to get access to absolute line init counters in the PM context
   */
   #define DSL_PM_PTR_LINE_INIT_COUNTERS_ABS() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recAbs.init)

   /**
      Macro to get access to 15 min line init counters in the PM context
   */
   #define DSL_PM_PTR_LINE_INIT_COUNTERS_15MIN(idx) \
      &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.rec15min[idx].init)

   /**
      Macro to get access to 1 day line init counters in the PM context
   */
   #define DSL_PM_PTR_LINE_INIT_COUNTERS_1DAY(idx) \
      &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.rec1day[idx].init)

   /**
      Macro to get access to Showtime line init counters in the PM context
   */
   #define DSL_PM_PTR_LINE_INIT_COUNTERS_SHOWTIME(idx) \
      &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recShowtime[idx].init)

   /**
      Macro to get access to total line init counters in the PM context
   */
   #define DSL_PM_PTR_LINE_INIT_COUNTERS_TOTAL() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.recTotal.init)

   /**
      Macro to get access to the line 15-min interval thresholds data in the PM context
   */
   #define DSL_PM_PTR_LINE_INIT_THRESHOLD_15MIN() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.thresholds15min.init)

   /**
      Macro to get access to the line 1-day interval thresholds data in the PM context
   */
   #define DSL_PM_PTR_LINE_INIT_THRESHOLD_1DAY() \
      &(DSL_PM_CONTEXT(pContext)->pCounters->lineCounters.thresholds1day.init)
#endif /** #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/

/** PM History control structure */
typedef struct
{
   /**
   Maximum number of items in the history. */
   DSL_uint32_t     historySize;
   /**
   Current item offset. */
   DSL_uint32_t     curItem;
   /**
   Current number of items in the history. */
   DSL_uint32_t     itemsNum;
} DSL_pmHistory_t;

/**
   Line Counters data structure. Includes Second and Init counters for the
   Far-End and Near-End directions
*/
typedef struct
{
   /** Far-End data*/
   DSL_PM_LineSecData_t  sec_fe;
   /** Near-End data*/
   DSL_PM_LineSecData_t  sec_ne;
#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
   /** Near-End data*/
   DSL_PM_LineInitData_t init;
#endif
} DSL_pmLineData_t;

typedef struct
{
   DSL_uint32_t nLOFBegin;
   DSL_uint32_t nLOFIntern;
   DSL_uint32_t nLOFCurr;
   DSL_uint32_t nUASBegin;
   DSL_uint32_t nUASIntern;
   DSL_uint32_t nUASCurr;
} DSL_pmLineSecAuxData_t;

typedef struct
{
   DSL_pmLineSecAuxData_t sec_data_ne;
   DSL_pmLineSecAuxData_t sec_data_fe;
} DSL_pmLineAuxData_t;

/**
   Line counters threshold crossing indication structure.
*/
typedef struct
{
   /**
   15 minutes data path counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_LineThresholdCrossing_t).*/
   DSL_uint32_t   n15Min;
   /**
   1 day data path counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_LineThresholdCrossing_t).*/
   DSL_uint32_t   n1Day;
} DSL_pmLineThresholdCrossingData_t;

typedef struct
{
   /** Near-End data*/
   DSL_pmLineThresholdCrossingData_t ind_ne;
   /** Far-End data*/
   DSL_pmLineThresholdCrossingData_t ind_fe;
} DSL_pmLineThresholdCrossing_t;

/**
   PM Line Counters Data
*/
typedef struct
{
#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
   /** History intervals information for 15-min*/
   DSL_pmHistory_t hist15min;
   /** History intervals information for 1-day*/
   DSL_pmHistory_t hist1day;
   /** 15-min interval records. Contains DSL_PM_LINE_15MIN_RECORDS_NUM history
   records and one current record*/
   DSL_pmLineData_t rec15min[DSL_PM_LINE_15MIN_RECORDS_NUM + 1];
   /** 1-day interval records. Contains DSL_PM_LINE_1DAY_RECORDS_NUM history
   records and one current record*/
   DSL_pmLineData_t rec1day[DSL_PM_LINE_1DAY_RECORDS_NUM + 1];
#endif /** #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/
   /** Absolute values*/
   DSL_pmLineData_t recAbs;
#endif /* #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/
   /** Current data record. Contains values requested from the FW in the last polling cycle*/
   DSL_pmLineData_t recCurr;
#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
   /** History intervals information for the Showtime intervals*/
   DSL_pmHistory_t histShowtime;
   /** Showtime interval records. Contains DSL_PM_LINE_SHOWTIME_RECORDS_NUM history
   records and one current record*/
   DSL_pmLineData_t recShowtime[DSL_PM_LINE_SHOWTIME_RECORDS_NUM + 1];
   /** Showtime timeframes history statistics*/
   DSL_uint32_t nShowtimeTimeHist[DSL_PM_LINE_SHOWTIME_RECORDS_NUM + 1];
#endif /** #ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/
#endif /* #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/
   /** Total data record*/
   DSL_pmLineData_t recTotal;
#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   /** 15-min interval thresholds*/
   DSL_pmLineData_t thresholds15min;
   /** 1-day interval thresholds*/
   DSL_pmLineData_t thresholds1day;
   /** Thresholds indication bitmask*/
   DSL_pmLineThresholdCrossing_t thresholdInd;
#endif /** #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /** #ifdef INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS*/
#endif /* #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/
   /** Line Aux data*/
   DSL_pmLineAuxData_t auxData;
} DSL_PM_LineCountersData_t;


#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
/**
   Channel Counters data structure. Includes data for both Far-End and Near-End
   directions
*/
typedef struct
{
   /** Far-End data*/
   DSL_PM_ChannelData_t data_fe[DSL_CHANNELS_PER_LINE];
   /** Near-End data*/
   DSL_PM_ChannelData_t data_ne[DSL_CHANNELS_PER_LINE];
} DSL_pmChannelData_t;

/**
   Channel counters threshold crossing indication structure.
*/
typedef struct
{
   /**
   15 minutes data path counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_ChannelThresholdCrossing_t).*/
   DSL_uint32_t   n15Min;
   /**
   1 day data path counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_ChannelThresholdCrossing_t).*/
   DSL_uint32_t   n1Day;
} DSL_pmChannelThresholdCrossingData_t;

typedef struct
{
   /** Near-End data*/
   DSL_pmChannelThresholdCrossingData_t ind_ne[DSL_CHANNELS_PER_LINE];
   /** Far-End data*/
   DSL_pmChannelThresholdCrossingData_t ind_fe[DSL_CHANNELS_PER_LINE];
} DSL_pmChannelThresholdCrossing_t;

/**
   PM Channel Counters Data
*/
typedef struct
{
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
   /** History intervals information for 15-min*/
   DSL_pmHistory_t hist15min;
   /** History intervals information for 1-day*/
   DSL_pmHistory_t hist1day;
   /** 15-min interval records. Contains DSL_PM_CHANNEL_15MIN_RECORDS_NUM history
   records and one current record*/
   DSL_pmChannelData_t rec15min[DSL_PM_CHANNEL_15MIN_RECORDS_NUM + 1];
   /** 1-day interval records. Contains DSL_PM_CHANNEL_1DAY_RECORDS_NUM history
   records and one current record*/
   DSL_pmChannelData_t rec1day[DSL_PM_CHANNEL_1DAY_RECORDS_NUM + 1];
#endif /** #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/
   /** Absolute values*/
   DSL_pmChannelData_t recAbs;
   /** Current data record. Contains values requested from the FW in the last polling cycle*/
   DSL_pmChannelData_t recCurr;
#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
   /** History intervals information for the Showtime intervals*/
   DSL_pmHistory_t histShowtime;
   /** Showtime interval records. Contains DSL_PM_CHANNEL_SHOWTIME_RECORDS_NUM history
   records and one current record*/
   DSL_pmChannelData_t recShowtime[DSL_PM_CHANNEL_SHOWTIME_RECORDS_NUM + 1];
   /** Showtime timeframes history statistics*/
   DSL_uint32_t nShowtimeTimeHist[DSL_PM_CHANNEL_SHOWTIME_RECORDS_NUM + 1];
#endif /** #ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/
   /** Total data record*/
   DSL_pmChannelData_t recTotal;
#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   /** 15-min interval thresholds*/
   DSL_pmChannelData_t thresholds15min;
   /** 1-day interval thresholds*/
   DSL_pmChannelData_t thresholds1day;
   /** Thresholds indication bitmask*/
   DSL_pmChannelThresholdCrossing_t thresholdInd;
#endif /** #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /** #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS*/
} DSL_PM_ChannelCountersData_t;
#endif /** #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
/**
   Data Path Counters data structure. Includes data for both Far-End and Near-End
   directions
*/
typedef struct
{
   /** Far-End data*/
   DSL_PM_DataPathData_t data_fe[DSL_CHANNELS_PER_LINE];
   /** Near-End data*/
   DSL_PM_DataPathData_t data_ne[DSL_CHANNELS_PER_LINE];
} DSL_pmDataPathData_t;

/**
   Data path counters threshold crossing indication structure.
*/
typedef struct
{
   /**
   15 minutes data path counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_DataPathThresholdCrossing_t).*/
   DSL_uint32_t   n15Min;
   /**
   1 day data path counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_DataPathThresholdCrossing_t).*/
   DSL_uint32_t   n1Day;
} DSL_pmDataPathThresholdCrossingData_t;

typedef struct
{
   /** Near-End data*/
   DSL_pmDataPathThresholdCrossingData_t ind_ne[DSL_CHANNELS_PER_LINE];
   /** Far-End data*/
   DSL_pmDataPathThresholdCrossingData_t ind_fe[DSL_CHANNELS_PER_LINE];
} DSL_pmDataPathThresholdCrossing_t;

/**
   PM Data Path Counters Data
*/
typedef struct
{
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
   /** History intervals information for 15-min*/
   DSL_pmHistory_t hist15min;
   /** History intervals information for 1-day*/
   DSL_pmHistory_t hist1day;
   /** 15-min interval records. Contains DSL_PM_DATAPATH_15MIN_RECORDS_NUM history
   records and one current record*/
   DSL_pmDataPathData_t rec15min[DSL_PM_DATAPATH_15MIN_RECORDS_NUM + 1];
   /** 1-day interval records. Contains DSL_PM_DATAPATH_1DAY_RECORDS_NUM history
   records and one current record*/
   DSL_pmDataPathData_t rec1day[DSL_PM_DATAPATH_1DAY_RECORDS_NUM + 1];
#endif /** #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/
   /** Absolute values*/
   DSL_pmDataPathData_t recAbs;
   /** Current data record. Contains values requested from the FW in the last polling cycle*/
   DSL_pmDataPathData_t recCurr;
#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
   /** History intervals information for the Showtime intervals*/
   DSL_pmHistory_t histShowtime;
   /** Showtime interval records. Contains DSL_PM_DATAPATH_SHOWTIME_RECORDS_NUM history
   records and one current record*/
   DSL_pmDataPathData_t recShowtime[DSL_PM_DATAPATH_SHOWTIME_RECORDS_NUM + 1];
   /** Showtime timeframes history statistics*/
   DSL_uint32_t nShowtimeTimeHist[DSL_PM_DATAPATH_SHOWTIME_RECORDS_NUM + 1];
#endif /** #ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/
   /** Total data record*/
   DSL_pmDataPathData_t recTotal;
#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   /** 15-min interval thresholds*/
   DSL_pmDataPathData_t thresholds15min;
   /** 1-day interval thresholds*/
   DSL_pmDataPathData_t thresholds1day;
   /** Thresholds indication bitmask*/
   DSL_pmDataPathThresholdCrossing_t thresholdInd;
#endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /** #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS*/
} DSL_PM_DataPathCountersData_t;
#endif /** #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/

/**
   PM counters data. Encapsulates all required counters data information
*/
typedef struct
{
   /** PM Line Counters data*/
   DSL_PM_LineCountersData_t lineCounters;
#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
   /** PM Channel Counters data*/
   DSL_PM_ChannelCountersData_t channelCounters;
#endif
#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
   /** PM Data Path Counters data*/
   DSL_PM_DataPathCountersData_t dataPathCounters;
#endif
} DSL_PM_CountersData_t;

typedef struct
{
   /** PM module thread activity flag*/
   DSL_boolean_t bRun;
   /** Pm module thread poll time, msec*/
   DSL_uint32_t nThreadPollTime;
   /** PM module */
   DSL_waitQueueHead_t pmWaitQueue;
   /** PM module*/
   DSL_completion_t pmThreadExit;
} DSL_PM_Thread_t;

typedef struct
{
   /** PM module initialization flag*/
   DSL_boolean_t bInit;
   /** Common PM module mutex*/
   DSL_mutex_t pmMutex;
   /** PM module Near-End mutex*/
   DSL_mutex_t pmNeMutex;
   /** PM module Far-End mutex*/
   DSL_mutex_t pmFeMutex;
   /** Pm module ...*/
   DSL_PM_Thread_t pmThreadNe;
   /** Pm module ...*/
   DSL_PM_Thread_t pmThreadFe;
   /** PM module lock indication flag*/
   DSL_boolean_t bPmLock;
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
   /** Current synchronization mode. */
   DSL_PM_SyncModeType_t syncMode;
   /** 15 minutes elapsed flag*/
   DSL_boolean_t b15minElapsed;
   /** External 15 min elapsed flag. Used with the external sync mode*/
   DSL_boolean_t bExternal15minElapsed;
   /** 1 day elapsed flag*/
   DSL_boolean_t b1dayElapsed;
   /** External 1 day elapsed flag. Used with the external sync mode*/
   DSL_boolean_t bExternal1dayElapsed;
   /** Synchronization time to be used, (msec) */
   DSL_uint32_t nCurr15MinTime;
   /** Synchronization time to be used, (msec)*/
   DSL_uint32_t nCurr1DayTime;
   /** Number of seconds in the 15 minutes interval, default is 900. */
   DSL_uint32_t nPm15Min;
   /** Number of seconds in the 1 day interval*/
   DSL_uint32_t nPm1Day;
#endif /** #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/
#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
   /** Showtime elapsed flag*/
   DSL_boolean_t bShowtimeProcessingStart;
   /** Showtime reached flag*/
   DSL_boolean_t bShowtimeInvTrigger;
   /** Showtime synchronization time to be used, (msec) */
   DSL_uint32_t nCurrShowtimeTime;
   /** Actual Line state*/
   DSL_LineStateValue_t nLineState;
#endif /** #ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/
   /** Last time checked*/
   DSL_TimeVal_t nLastTimeCheck;
   /** PM Module Startup time*/
   DSL_uint32_t nPmStartTime;
   /** Far-End Request cycle*/
   DSL_uint32_t nFeRequestCycle;
   /** Far-End counters reset flag*/
   DSL_boolean_t bFeTr1Reset;
   /** PM tick interval in milli seconds, default is 1000 msec, mimimum is 500 msec.
       Each tick the PM database is updated through reading of the counters out
       of the firmware. */
   DSL_uint32_t nPmTick;
   /** PM module counters*/
   DSL_PM_CountersData_t *pCounters;
} DSL_PM_Context;

/** PM module counters data*/
extern DSL_PM_CountersData_t g_Counters;

/**
   PM module main control thread, Near-End Processing
*/
DSL_int_t DSL_PM_ThreadNe(DSL_void_t *pCtx);

/**
   PM module Far-End thread
*/
DSL_int_t DSL_PM_ThreadFe(DSL_void_t *pCtx);

/**
   Function to Lock/Unlock PM module direction specific mutex
*/
DSL_Error_t DSL_PM_DirectionMutexControl(
   DSL_Context_t *pContext,
   const DSL_XTUDir_t nDirection,
   DSL_boolean_t bLock);

/**
  Lock PM module processing
*/
DSL_Error_t DSL_PM_Lock(DSL_Context_t *pContext);

/**
  Unlock PM module processing
*/
DSL_Error_t DSL_PM_UnLock(DSL_Context_t *pContext);

#if defined(INCLUDE_DSL_CPE_PM_HISTORY) || defined(INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS)
/**
   PM module hostory create fucntion
*/
DSL_void_t DSL_PM_HistoryCreate(
   DSL_pmHistory_t *hist,
   DSL_uint32_t historySize);

/** Function to delete PM history intervals*/
DSL_Error_t DSL_PM_HistoryDelete(
   DSL_Context_t *pContext,
   DSL_pmHistory_t *hist);

/** Function to update PM history interval*/
DSL_Error_t DSL_PM_HistoryCurItemUpdate(
   DSL_Context_t *pContext,
   DSL_pmHistory_t *hist);

/** Function to get current history fill level*/
DSL_Error_t DSL_PM_HistoryFillLevelGet(
   DSL_Context_t *pContext,
   DSL_pmHistory_t *hist,
   DSL_uint32_t *pFillLevel);

/** Function to get history interval Index*/
DSL_Error_t DSL_PM_HistoryItemIdxGet(
   DSL_Context_t *pContext,
   DSL_pmHistory_t *hist,
   DSL_uint32_t histInterval,
   DSL_int_t *pIdx);

/** Function to update all PM module history values*/
DSL_Error_t DSL_PM_HistoryUpdate(DSL_Context_t *pContext);

/** Function to get the PM module history statistics*/
DSL_Error_t DSL_PM_HistoryStatsGet(
   DSL_Context_t *pContext,
   DSL_pmHistory_t *pHist,
   DSL_PM_HistoryStatsData_t *pStats);
#endif /** #if defined(INCLUDE_DSL_CPE_PM_HISTORY) || defined(INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS)*/

/** Function to update all PM module counters*/
DSL_Error_t DSL_PM_CountersUpdate(
   DSL_Context_t *pContext,
   const DSL_XTUDir_t nDirection);

#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
DSL_Error_t DSL_PM_LineCountersReset(
   DSL_Context_t *pContext,
   DSL_PM_ResetTypes_t ResetType);
#endif /* #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/

#if defined(INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS)
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
DSL_Error_t DSL_PM_LineSecThresholdsUpdate(
   DSL_PM_LineSecData_t *pCounters,
   DSL_PM_LineSecData_t *pThreshsOld,
   DSL_PM_LineSecData_t *pThreshsNew,
   DSL_uint32_t *pInd);


DSL_Error_t DSL_PM_LineInitThresholdsUpdate(
   DSL_PM_LineInitData_t *pCounters,
   DSL_PM_LineInitData_t *pThreshsOld,
   DSL_PM_LineInitData_t *pThreshsNew,
   DSL_uint32_t *pInd);
#endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif

#if defined(INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS)
DSL_Error_t DSL_PM_ChannelCountersReset(
   DSL_Context_t *pContext,
   DSL_PM_ResetTypes_t ResetType);
#endif

#if defined(INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS)
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
DSL_Error_t DSL_PM_ChannelThresholdsUpdate(
   DSL_PM_ChannelData_t *pCounters,
   DSL_PM_ChannelData_t *pThreshsOld,
   DSL_PM_ChannelData_t *pThreshsNew,
   DSL_uint32_t *pInd);
#endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif

#if defined(INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS)
DSL_Error_t DSL_PM_DataPathCountersReset(
   DSL_Context_t *pContext,
   DSL_PM_ResetTypes_t ResetType);
#endif

#if defined(INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS)
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
DSL_Error_t DSL_PM_DataPathThresholdsUpdate(
   DSL_PM_DataPathData_t *pCounters,
   DSL_PM_DataPathData_t *pThreshsOld,
   DSL_PM_DataPathData_t *pThreshsNew,
   DSL_uint32_t *pInd);
#endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif

/** Function to check the PM module availability*/
DSL_boolean_t DSL_PM_IsPmReady(
   DSL_Context_t *pContext);

/** Function to save all PM module counters before restarting the FW*/
DSL_Error_t DSL_PM_CountersSave(DSL_Context_t *pContext);

/** Function to restore all PM module counters in the FW after restart*/
DSL_Error_t DSL_PM_CountersRestore(
   DSL_Context_t *pContext,
   DSL_uint32_t fwUnavailableTime);


/** @} DRV_DSL_CPE_PM */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_PM_CORE_H */
