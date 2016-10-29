/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_API_PM_H
#define _DRV_DSL_CPE_API_PM_H

#ifdef __cplusplus
   extern "C" {
#endif

#include "drv_dsl_cpe_api.h"

/** \file
   Performance Monitoring interface
*/

/** \addtogroup DRV_DSL_CPE_PM
 @{ */

#ifndef DSL_PM_CHANNEL_15MIN_RECORDS_NUM
/** history size - 15 minuntes channel records */
#define DSL_PM_CHANNEL_15MIN_RECORDS_NUM     2
#endif

#ifndef DSL_PM_CHANNEL_1DAY_RECORDS_NUM
/** history size - one day channel records */
#define DSL_PM_CHANNEL_1DAY_RECORDS_NUM      2
#endif

#ifndef DSL_PM_LINE_15MIN_RECORDS_NUM
/** history size - 15 minutes line records */
#define DSL_PM_LINE_15MIN_RECORDS_NUM        2
#endif

#ifndef DSL_PM_LINE_1DAY_RECORDS_NUM
/** history size - one day line records */
#define DSL_PM_LINE_1DAY_RECORDS_NUM         2
#endif

#ifndef DSL_PM_DATAPATH_15MIN_RECORDS_NUM
/** history size - 15 minutes data path records */
#define DSL_PM_DATAPATH_15MIN_RECORDS_NUM    2
#endif

#ifndef DSL_PM_DATAPATH_1DAY_RECORDS_NUM
/** history size - one day data path records */
#define DSL_PM_DATAPATH_1DAY_RECORDS_NUM     2
#endif

/* $$PM_Showtime: Following three definitions has to be used within
   PM implementation for TR-98 showtime counters */
#ifndef DSL_PM_LINE_SHOWTIME_RECORDS_NUM
/** showtime interval numbers per line */
#define DSL_PM_LINE_SHOWTIME_RECORDS_NUM        2
#endif
#ifndef DSL_PM_CHANNEL_SHOWTIME_RECORDS_NUM
/** showtime interval numbers per channel */
#define DSL_PM_CHANNEL_SHOWTIME_RECORDS_NUM     2
#endif
#ifndef DSL_PM_DATAPATH_SHOWTIME_RECORDS_NUM
/** showtime interval numbers per data path */
#define DSL_PM_DATAPATH_SHOWTIME_RECORDS_NUM    2
#endif

#ifndef DSL_PM_DEFAULT_SYNC_MODE
/**
   Default PM synchronization mode
   This define can be set during compile time to change the default behaviour.
*/
#ifndef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   /** Here the sync mode should be fixed!!! */
   #define DSL_PM_DEFAULT_SYNC_MODE             DSL_PM_SYNC_MODE_SYS_TIME
#else
   #define DSL_PM_DEFAULT_SYNC_MODE             DSL_PM_SYNC_MODE_SYS_TIME
#endif /** #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif

/* ************************************************************************** */
/* *  xDSL Line Endpoint interface                                          * */
/* ************************************************************************** */

/**
   Structure used to return additional information for total counters.
*/
typedef struct
{
   /**
   Elapsed time in the interval */
   DSL_OUT DSL_uint32_t nElapsedTime;
} DSL_PM_TotalData_t;

/**
   Structure used to return additional information about a PM interval.
*/
typedef struct
{
   /**
   Elapsed time in the interval */
   DSL_OUT DSL_uint32_t nElapsedTime;
   /**
   Interval number */
   DSL_OUT DSL_uint8_t nNumber;
   /**
   Interval validity flag */
   DSL_OUT DSL_boolean_t bValid;
} DSL_PM_IntervalData_t;

/**
   History statistics structure for valid/invalid counters
*/
typedef struct
{
   /**
   Number of previous intervals */
   DSL_OUT DSL_uint32_t nPrevIvs;
   /**
   Number of previous invalid intervals */
   DSL_OUT DSL_uint32_t nPrevInvalidIvs;
} DSL_PM_HistoryStatsData_t;

/**
   History statistics structure for line specific data usage
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_LINE_INIT_HISTORY_STATS_15MIN_GET
   - \ref DSL_FIO_PM_LINE_INIT_HISTORY_STATS_1DAY_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains PM history information */
   DSL_OUT DSL_PM_HistoryStatsData_t data;
} DSL_PM_HistoryStats_t;

/**
   History statistics structure for line and direction specific data usage
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_LINE_SEC_HISTORY_STATS_15MIN_GET
   - \ref DSL_FIO_PM_LINE_SEC_HISTORY_STATS_1DAY_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains direction specific PM history information */
   DSL_OUT DSL_PM_HistoryStatsData_t data;
} DSL_PM_HistoryStatsDir_t;

/**
   History statistics structure for line and direction specific data usage
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_CHANNEL_HISTORY_STATS_15MIN_GET
   - \ref DSL_FIO_PM_CHANNEL_HISTORY_STATS_1DAY_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains direction and channel specific PM history information */
   DSL_PM_HistoryStatsData_t data;
} DSL_PM_HistoryStatsChDir_t;

/**
   Line perfomance second counters (see chapter 7.2.1.1 and 7.2.1.2 of G.997.1)
*/
typedef struct
{
   /**
   Errored Seconds.
   This parameter is a count of 1 second intervals with one or
   more CRC 8 anomalies summed over all received bearer channels,
   or one or more LOS defects, or one or more SEF defects, or one or
   more LPR defects. */
   DSL_OUT DSL_uint32_t nES;
   /**
   Severely Errored Seconds.
   This parameter is a count of 1 second intervals with 18 or more
   CRC 8 anomalies summed over all received bearer channels, or one or
   more LOS defects, or one or more SEF defects, or one or more LPR defects.
   If a common CRC is applied over multiple bearer channels, then each related
   CRC-8 anomaly shall be counted only once for the whole set of bearer channels
   over which the CRC is applied. */
   DSL_OUT DSL_uint32_t nSES;
   /**
   Loss of Signal Seconds.
   This parameter is a count of 1 second intervals containing one or more LOS
   defects. */
   DSL_OUT DSL_uint32_t nLOSS;
   /**
   Unavailable Seconds (see chapter 7.2.7.13 of G.997.1.)
   This parameter is a count of 1 second intervals for which the ADSL line is
   unavailable. The ADSL line becomes unavailable at the onset of 10 contiguous
   SES Ls. The 10 SESs are included in unavailable time. Once unavailable, the
   ADSL line becomes available at the onset of 10 contiguous seconds with no
   SESs. The 10 seconds with no SESs are excluded from unavailable time. Some
   parameter counts are inhibited during unavailability */
   DSL_OUT DSL_uint32_t nUAS;
   /**
   Loss of Frame Seconds.
   This parameter is a count of 1 second intervals for which the ADSL line is in
   Loss of frame (LOF) state.
   An LOF failure is declared after 2.5 + 0.5 s of contiguous SEF defect, except
   when an LOS defect or failure is present (see LOS definition above).
   An LOF failure is cleared when LOS failure is declared, or after 10 + 0.5 s
   of no SEF defect. */
   DSL_OUT DSL_uint32_t nLOFS;
} DSL_PM_LineSecData_t;

/**
   Line perfomance second counters.
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET
   - \ref DSL_FIO_PM_LINE_SEC_COUNTERS_1DAY_GET
   - \ref DSL_FIO_PM_LINE_SEC_COUNTERS_SHOWTIME_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Number of history interval (0 - current interval)
   In case of requesting showtime counter values
   (\ref DSL_FIO_PM_LINE_SEC_COUNTERS_SHOWTIME_GET) accumulated data is
   returned as follows
   - 0 (current interval): counter values since most recent DSL showtime
   - 1 : counter values since second most recent DSL showtime */
   DSL_IN DSL_uint32_t nHistoryInterval;
   /**
   Structure that contains additional information about the selected PM
   interval */
   DSL_OUT DSL_PM_IntervalData_t interval;
   /**
   Structure that contains line specific PM history counter data */
   DSL_OUT DSL_PM_LineSecData_t data;
} DSL_PM_LineSecCounters_t;

/**
   Line perfomance second counters (see chapter 7.2.1.1 and 7.2.1.2 of G.997.1)
   This structure has to be used for ioctl
   \ref DSL_FIO_PM_LINE_SEC_COUNTERS_TOTAL_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains additional info for total PM counter */
   DSL_OUT DSL_PM_TotalData_t total;
   /**
   Structure that contains line specific PM history counter data */
   DSL_OUT DSL_PM_LineSecData_t data;
} DSL_PM_LineSecCountersTotal_t;

/**
   Line perfomance second counters thresholds
   (see chapter 7.2.1.1 and 7.2.1.2 of G.997.1)
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_SET
   - \ref DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_SET
   - \ref DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_GET
   - \ref DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains line specific PM threshold configuration */
   DSL_CFG DSL_PM_LineSecData_t data;
} DSL_PM_LineSecThreshold_t;

/**
   Line perfomance initialization counters
   (see chapter 7.2.1.3 of G.997.1)
*/
typedef struct
{
   /**
   Full Initializations.
   This parameter is a count of the total number of full initializations
   attempted on the line (successful and failed) during the accumulation period.
   Parameter procedures shall be as defined in chapter 7.2.7 of G.997.1. */
   DSL_OUT DSL_uint32_t nFullInits;
   /**
   Failed Full Initializations.
   This performance parameter is a count of the total number of failed full
   initializations during the accumulation period. A failed full initialization
   is when showtime is not reached at the end of the full initialization
   procedure, e.g., when:
      - A CRC error is detected.
      - A time out occurs.
      - Unexpected message content is received.
   Parameter procedures shall be as defined in in chapter 7.2.7 of G.997.1. */
   DSL_OUT DSL_uint32_t nFailedFullInits;
   /**
   Short Initializations.
   This parameter is a count of the total number of fast retrains or short
   initializations attempted on the line (successful and failed) during the
   accumulation period. Parameter procedures shall be as defined in chapter
   7.2.7 of G.997.1.
   Fast Retrain is defined in Recommendation G.992.2.
   Short Initialization is defined in Recommendation G.992.3 and G.992.4. */
   DSL_OUT DSL_uint32_t nShortInits;
   /**
   Failed Short Initializations.
   This performance parameter is a count of the total number of failed fast
   retrains or short initializations during the accumulation period. A failed
   fast retrain or short initialization is when showtime is not reached at the
   end of the fast retrain or short initialization procedure, e.g., when:
      - A CRC error is detected.
      - A time out occurs.
      - A fast retrain profile is unknown.
   Parameter procedures shall be as defined in chapter 7.2.7 of G.997.1.*/
   DSL_OUT DSL_uint32_t nFailedShortInits;
} DSL_PM_LineInitData_t;

/**
   Line perfomance initialization counters.
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_LINE_INIT_COUNTERS_15MIN_GET
   - \ref DSL_FIO_PM_LINE_INIT_COUNTERS_1DAY_GET
   - \ref DSL_FIO_PM_LINE_INIT_COUNTERS_SHOWTIME_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Number of history interval (0 - current interval).
   In case of requesting showtime counter values
   (\ref DSL_FIO_PM_LINE_INIT_COUNTERS_SHOWTIME_GET) accumulated data is
   returned as follows
   - 0 (current interval): counter values since most recent DSL showtime
   - 1 : counter values since second most recent DSL showtime */
   DSL_IN DSL_uint32_t nHistoryInterval;
   /**
   Structure that contains additional information about the selected PM
   interval */
   DSL_OUT DSL_PM_IntervalData_t interval;
   /**
   Structure that contains line specific PM initialization counter data */
   DSL_OUT DSL_PM_LineInitData_t data;
} DSL_PM_LineInitCounters_t;

/**
   Line perfomance initialization counters
   (see chapter 7.2.1.3 of G.997.1)
   This structure has to be used for ioctl
   \ref DSL_FIO_PM_LINE_INIT_COUNTERS_TOTAL_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains additional info for total PM counter */
   DSL_OUT DSL_PM_TotalData_t total;
   /**
   Structure that contains line specific PM initialization counter data */
   DSL_OUT DSL_PM_LineInitData_t data;
} DSL_PM_LineInitCountersTotal_t;

/**
   Line perfomance initialization threshold (see chapter 7.2.1.3 of G.997.1)
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_SET
   - \ref DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_SET
   - \ref DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_GET
   - \ref DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains line specific PM initialization threshold
   configuration data */
   DSL_OUT DSL_PM_LineInitData_t data;
} DSL_PM_LineInitThreshold_t;

/**
   Line threshold crossing bitfields
   \remarks The enums corresponds to bit field definitions.
*/
typedef enum
{
   /**
   Empty entry */
   DSL_PM_LINETHRESHCROSS_EMPTY   = 0x00000000,
   /**
   FECS counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_FECS    = 0x00000001,
   /**
   ES counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_ES      = 0x00000002,
   /**
   SES counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_SES     = 0x00000004,
   /**
   LOSS counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_LOSS    = 0x00000008,
   /**
   UAS counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_UAS     = 0x00000010,
   /**
   Full initializations counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_FI      = 0x00000020,
   /**
   Failed full initializations counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_FIFAIL  = 0x00000040,
   /**
   Short initializations counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_SI      = 0x00000080,
   /**
   Failed short initializations counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_SIFAIL  = 0x00000100,
   /**
   LOFS counter threshold crossing. */
   DSL_PM_LINETHRESHCROSS_LOFS    = 0x00000200,
} DSL_PM_BF_LineThresholdCrossing_t;


/**
   Line counters threshold crossing indication structure.
*/
typedef struct
{
   /**
   Number of seconds elapsed in the current 15 min interval. */
   DSL_uint16_t   nCurr15MinTime;
   /**
   Number of seconds elapsed in the current 1 day interval. */
   DSL_uint32_t   nCurr1DayTime;
   /**
   15 minutes line counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_LineThresholdCrossing_t).*/
   DSL_uint32_t   n15Min;
   /**
   1 day line counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_LineThresholdCrossing_t).*/
   DSL_uint32_t   n1Day;
} DSL_PM_LineThresholdCrossing_t;

/* ************************************************************************** */
/* *  xDSL Channel Endpoint interface                                       * */
/* ************************************************************************** */

/**
   Channel perfomance counters (see chapter 7.2.2.1 and 7.2.2.2 of G.997.1)
*/
typedef struct
{
   /**
   Code Violations.
   This parameter is a count of CRC 8 anomalies (the number of incorrect CRC)
   occurring in the bearer channel during the accumulation period. This parameter
   is subject to inhibiting - see chapter 7.2.7.13 of G.997.1.
   If the CRC is applied over multiple bearer channels, then each related CRC 8
   anomaly shall increment each of the counters related to the individual bearer channels. */
   DSL_OUT DSL_uint32_t nCodeViolations;
   /**
   Forward Error Corrections.
   This parameter is a count of FEC anomalies (the number of corrected code words)
   occurring in the bearer channel during the accumulation period. This parameter is
   subject to inhibiting - see chapter 7.2.7.13 of G.997.1.
   If FEC is applied over multiple bearer channels, then each related FEC anomaly
   shall increment each of the counters related to the individual bearer channels.*/
   DSL_OUT DSL_uint32_t nFEC;
} DSL_PM_ChannelData_t;

/**
   Channel perfomance counters (see chapter 7.2.2.1 and 7.2.2.2 of G.997.1)
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET
   - \ref DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET
   - \ref DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Number of history interval (0 - current interval)
   In case of requesting showtime counter values
   (\ref DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET) accumulated data is
   returned as follows
   - 0 (current interval): counter values since most recent DSL showtime
   - 1 : counter values since second most recent DSL showtime */
   DSL_IN DSL_uint32_t nHistoryInterval;
   /**
   Structure that contains additional information about the selected PM
   interval */
   DSL_OUT DSL_PM_IntervalData_t interval;
   /**
   Structure that contains channel specific PM counter data */
   DSL_OUT DSL_PM_ChannelData_t data;
} DSL_PM_ChannelCounters_t;

/**
   Channel perfomance thresholds (see chapter 7.2.2.1 and 7.2.2.2 of G.997.1)
   This structure has to be used for ioctl
   \ref DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET
 */
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains additional info for total PM counter */
   DSL_OUT DSL_PM_TotalData_t total;
   /**
   Structure that contains channel specific PM counter data */
   DSL_OUT DSL_PM_ChannelData_t data;
} DSL_PM_ChannelCountersTotal_t;

/**
   Line perfomance initialization threshold
   (see chapter 7.2.2.1 and 7.2.2.2 of G.997.1)
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_SET
   - \ref DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_SET
   - \ref DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_GET
   - \ref DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains line specific PM initialization threshold
   configuration data */
   DSL_OUT DSL_PM_ChannelData_t data;
} DSL_PM_ChannelThreshold_t;

/**
   Channel threshold crossing bitfields
   \remarks The enums corresponds to bit field definitions.
*/
typedef enum
{
   /**
   Empty threshold crossing. */
   DSL_PM_CHANNELTHRESHCROSS_EMPTY = 0x00000000,
   /**
   FEC counter threshold crossing. */
   DSL_PM_CHANNELTHRESHCROSS_FEC   = 0x00000001,
   /**
   Code violations counter threshold crossing. */
   DSL_PM_CHANNELTHRESHCROSS_CV    = 0x00000002
} DSL_PM_BF_ChannelThresholdCrossing_t;

/**
   Channel counters threshold crossing indication structure.
*/
typedef struct
{
   /**
   Number of seconds elapsed in the current 15 min interval. */
   DSL_uint16_t   nCurr15MinTime;
   /**
   Number of seconds elapsed in the current 1 day interval. */
   DSL_uint32_t   nCurr1DayTime;
   /**
   15 minutes channel counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_ChannelThresholdCrossing_t).*/
   DSL_uint32_t   n15Min;
   /**
   1 day channel counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_ChannelThresholdCrossing_t).*/
   DSL_uint32_t   n1Day;
} DSL_PM_ChannelThresholdCrossing_t;

/* ************************************************************************** */
/* *  xDSL Data-Path Endpoint interface                                     * */
/* ************************************************************************** */

/** Data path perfomance counters (see chapter 7.2.4.1 and 7.2.4.2 of G.997.1) */
typedef struct
{
   /**
   HEC violation count.
   The HEC_violation_count performance parameter is a count of the number of
   occurrences of a HEC anomaly in the ATM Data Path.*/
   DSL_uint32_t nHEC;
   /**
   Delineated total cell count.
   The delineated_total_cell_count performance parameter is a count of
   the total number of cells passed through the cell delineation and HEC function
   process operating on the ATM Data Path while in the SYNC state.*/
   DSL_uint32_t nTotalCells;
   /**
   User total cell count.
   The User_total_cell_count performance parameter is a count of the
   total number of cells in the ATM Data Path delivered at the V-C (for ATU C) or
   T-R (for ATU R) interface.*/
   DSL_uint32_t nUserTotalCells;
   /**
   Idle Cell Bit Error Count.
   The idle_bit_error_count performance parameter in a count of the number of bit
   errors in the idle cell payload received in the ATM Data Path.
   The idle cell payload is defined in Recommendations I.361 and I.432.*/
   DSL_uint32_t nIBE;

   /* PTM counters according to G997.1 chapter 7.2.5 */
   /**
   CRC error count
   On CO: NE/FE - mandatory ; on CPE: NE mandatory / FE optional
   NEAR-END: The CRC P performance parameter is a count of the number of
   occurrences of a CRC n anomaly in the PTM Data Path at the near-end.
   FAR-END:  The far end CRC PFE performance parameter is a count at the
   far-end of the number of occurrences of a CRC n anomaly (as observed
   by the far-end) in the PTM Data Path. */
   DSL_uint32_t nCRC_P;
   /**
   CRCp error count
   On CO: NE/FE - mandatory ; on CPE: NE mandatory / FE optional
   NEAR-END: The CRCP P performance parameter is a count of the number of
   occurrences of a CRC np anomaly in the PTM Data Path at the near-end.
   FAR-END:  The far end CRCP PFE performance parameter is a count at the
   far-end of the number of occurrences of a CRC np anomaly (as observed
   by the far-end) in the PTM Data Path. */
   DSL_uint32_t nCRCP_P;
   /**
   Coding Violations Count
   On CO: NE/FE - mandatory ; on CPE: NE mandatory / FE optional
   NEAR-END: The CV P performance parameter is a count of the number of
   occurrences of a cv n anomaly in the PTM Data Path at the near-end.
   FAR-END:  The far end CV PFE performance parameter is a count at the
   far-end of the number of occurrences of a cv n anomaly (as observed
   by the far-end) in the PTM Data Path. */
   DSL_uint32_t nCV_P;
   /**
   Coding Violations P Count
   On CO: NE/FE - mandatory ; on CPE: NE mandatory / FE optional
   NEAR-END: The CVP P performance parameter is a count of the number of
   occurrences of a cv np anomaly in the PTM Data Path at the near-end.
   FAR-END:  The far end CVP PFE performance parameter is a count of the
   number of occurrences of a cv np anomaly (as observed by the far-end)
   in the PTM Data Path. */
   DSL_uint32_t nCVP_P;
} DSL_PM_DataPathData_t;

/**
   Data path perfomance counters (see chapter 7.2.4.1 and 7.2.4.2 of G.997.1)
   This structure has to be used for ioctl
   - \ref DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET
   - \ref DSL_FIO_PM_DATA_PATH_COUNTERS_1DAY_GET
   - \ref DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Number of history interval (0 - current interval)
   In case of requesting showtime counter values
   (\ref DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET) accumulated data is
   returned as follows
   - 0 (current interval): counter values since most recent DSL showtime
   - 1 : counter values since second most recent DSL showtime */
   DSL_IN DSL_uint32_t nHistoryInterval;
   /**
   Structure that contains additional information about the selected PM
   interval */
   DSL_OUT DSL_PM_IntervalData_t interval;
   /**
   Structure that contains data path specific PM counter data */
   DSL_OUT DSL_PM_DataPathData_t data;
} DSL_PM_DataPathCounters_t;

/**
   Data path perfomance counters (see chapter 7.2.4.1 and 7.2.4.2 of G.997.1)
   This structure has to be used for ioctl
   \ref DSL_FIO_PM_DATA_PATH_COUNTERS_TOTAL_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains additional info for total PM counter */
   DSL_OUT DSL_PM_TotalData_t total;
   /**
   Structure that contains data path specific PM counter data */
   DSL_OUT DSL_PM_DataPathData_t data;
} DSL_PM_DataPathCountersTotal_t;

/** Data path perfomance counters
    (see chapter 7.2.4.1 and 7.2.4.2 of G.997.1) */
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains data path specific PM threshold configuration data */
   DSL_OUT DSL_PM_DataPathData_t data;
} DSL_PM_DataPathThreshold_t;

/**
   Data path threshold crossing bitfields
   \remarks The enums corresponds to bit field definitions.
*/
typedef enum
{
   /**
   Empty threshold crossing. */
   DSL_PM_DATAPATHTHRESHCROSS_EMPTY       = 0x00000000,
    /**
   HEC counter threshold crossing. */
   DSL_PM_DATAPATHTHRESHCROSS_HEC         = 0x00000001,
   /**
   Total cells counter threshold crossing. */
   DSL_PM_DATAPATHTHRESHCROSS_TOTALCELL   = 0x00000002,
   /**
   User cells counter threshold crossing. */
   DSL_PM_DATAPATHTHRESHCROSS_USERCELL    = 0x00000004,
   /**
   Idle Cell Bit Error counter threshold crossing. */
   DSL_PM_DATAPATHTHRESHCROSS_IBE         = 0x00000008,
   /**
   Count of non preemptive packets with CRC error in the
   bearer channel threshold crossing. */
   DSL_PM_DATAPATHTHRESHCROSS_CRC_P       = 0x00000010,
   /**
   Count of preemptive packets with CRC error in the bearer
   channel threshold crossing. */
   DSL_PM_DATAPATHTHRESHCROSS_CRCP_P      = 0x00000020,
   /**
   Count of non preemptive packets with coding violation
   in the bearer channel threshold crossing. */
   DSL_PM_DATAPATHTHRESHCROSS_CV_P        = 0x00000040,
   /**
   Count of preemptive packets with coding violation in
   the bearer channel threshold crossing.  */
   DSL_PM_DATAPATHTHRESHCROSS_CVP_P       = 0x00000080
} DSL_PM_BF_DataPathThresholdCrossing_t;


/**
   Data path counters threshold crossing indication structure.
*/
typedef struct
{
   /**
   Number of seconds elapsed in the current 15 min interval. */
   DSL_uint16_t   nCurr15MinTime;
   /**
   Number of seconds elapsed in the current 1 day interval. */
   DSL_uint32_t   nCurr1DayTime;
   /**
   15 minutes data path counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_DataPathThresholdCrossing_t).*/
   DSL_uint32_t   n15Min;
   /**
   1 day data path counters threshold crossing indication bitmask
   (refer to \ref DSL_PM_BF_DataPathThresholdCrossing_t).*/
   DSL_uint32_t   n1Day;
} DSL_PM_DataPathThresholdCrossing_t;


/* ************************************************************************** */
/* *  Common function interface                                             * */
/* ************************************************************************** */

/**
   Definitions for PM reset functionalities
*/
typedef enum
{
   /**
   Reset current values only */
   DSL_PM_RESET_CURRENT = 0,
   /*
   Reset history values only */
   DSL_PM_RESET_HISTORY = 1,
   /*
   Reset the total counter values only */
   DSL_PM_RESET_TOTAL = 2,
   /**
   Reset all PM values including current, history and total */
   DSL_PM_RESET_ALL = 3
} DSL_PM_ResetTypes_t;

/**
   Defines structure for reset of PM counters
*/
typedef struct
{
   /**
   Number of previous invalid intervals */
   DSL_IN DSL_PM_ResetTypes_t nResetType;
} DSL_PM_ResetData_t;

/**
   Defines structure for reset of PM counters
   This structure has to be used for ioctl
   \ref DSL_FIO_PM_RESET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains control information */
   DSL_IN DSL_PM_ResetData_t data;
} DSL_PM_Reset_t;

/**
   Defines structure for performing external trigger for elapsed intervals
*/
typedef struct
{
   /**
   Specifies which type of interval has been elapsed
   - DSL_FALSE: Only current 15 min interval has been elapsed
   - DSL_TRUE: Current 15 min and 1 day interval has been elapsed */
   DSL_IN DSL_boolean_t bOneDayElapsed;
} DSL_PM_ElapsedExtTriggerData_t;

/**
   Defines structure for performing external trigger for elapsed intervals
   This structure has to be used for ioctl
   \ref DSL_FIO_PM_15MIN_ELAPSED_EXT_TRIGGER
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains control information */
   DSL_IN DSL_PM_ElapsedExtTriggerData_t data;
} DSL_PM_ElapsedExtTrigger_t;

/**
   Defines structure for performing a reset of the elapsed time
   This structure has to be used for ioctl
   \ref DSL_FIO_PM_ELAPSED_TIME_RESET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
} DSL_PM_ElapsedTimeReset_t;

/** PM synchronization modes */
typedef enum
{
   /**
   Free synchronization.
   PM will be synchronized to its startup time. After start of the PM no further
   synchronisation to an external clock source is done. */
   DSL_PM_SYNC_MODE_FREE,
   /**
   System time synchronization.
   PM will be synchronized to the system time. The time base is derived from the
   DSL_SysTimeGet function (OS specific). */
   DSL_PM_SYNC_MODE_SYS_TIME,
   /**
   External synchronization.
   PM will be synchronized to the external time network time.
   The host application should call the the function DSL_PM_15MinElapsedExtTrigger
   each 15 minutes. In addition the bOneDayElapsed parameter should be set
   accordingly. */
   DSL_PM_SYNC_MODE_EXTERNAL
} DSL_PM_SyncModeType_t;

/**
   Defines structure for configuring the PM sync mode
*/
typedef struct
{
   /**
   Specifies which type PM sync mode shall be used */
   DSL_IN DSL_PM_SyncModeType_t nMode;
} DSL_PM_SyncModeData_t;

/**
   Defines structure for configuring the PM sync mode
   This structure has to be used for ioctl
   \ref DSL_FIO_PM_SYNC_MODE_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains control information */
   DSL_IN DSL_PM_SyncModeData_t data;
} DSL_PM_SyncMode_t;

/**
   Configuration structure for the burnin mode of the PM module.
*/
typedef struct
{
   /** PM tick interval in milli seconds, default is 1000 msec, mimimum is 500 msec.
       Each tick the PM database is updated through reading of the counters out
       of the firmware. */
   DSL_uint32_t    nPmTick;
   /** Number of seconds in the 15 minutes interval, default is 900.
       By decreasing this values the 15 minutes interval can be shorten. */
   DSL_uint32_t    nPm15Min;
   /** Number of 15 min intervals per day, default is 96 .
       By decreasing this values the 15 minutes per day interval can be shorten. */
   DSL_uint32_t    nPm15MinPerDay;
} DSL_PM_BurninModeType_t;

/**
   Defines structure for configuring the PM burn in mode
*/
typedef struct
{
   /**
   Specifies whether to switch burnin mode on or off
   - DSL_FALSE: Burnin mode off (normal operation). In this case fixed, internal
     configuration is used which is equal to the default which are specified
     within \ref DSL_PM_BurninModeType_t
   - DSL_TRUE: Burnin mode on (debug mode). In this case the specified user
     values from nMode configuration settings are used. */
   DSL_IN DSL_boolean_t bActivate;
   /**
   Specifies the burn in mode configurations settings. These values are only
   active if the burn in mode is activated (bActivate = DSL_TRUE) */
   DSL_IN DSL_PM_BurninModeType_t nMode;
} DSL_PM_BurninModeData_t;

/**
   Defines structure for configuring the PM burn in mode
   This structure has to be used for ioctl
   \ref DSL_FIO_PM_BURNIN_MODE_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains control information */
   DSL_IN DSL_PM_BurninModeData_t data;
} DSL_PM_BurninMode_t;

#if defined(DSL_PM_DEBUG_MODE_ENABLE) && (DSL_PM_DEBUG_MODE_ENABLE > 0)
/**
   Configuration structure for the PM dump functions.
*/
typedef struct
{
   /**
   Specifies whether to print only the PM memory and
   data structure statistic (DSL_TRUE) or to print also the
   content of the counters (DSL_FALSE). If set to DSL_FALSE
   the output might be very extensive in accordance to the
   configured number of devices. */
   DSL_boolean_t bPrintMemStatOnly;
} DSL_PM_DumpConfig_t;

/**
   Defines structure for performing PM dump functionality
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies whether switch burnin mode on or off
   - DSL_FALSE: Burnin mode off (normal operation)
   - DSL_TRUE: urnin mode on (debug mode) */
   DSL_IN DSL_PM_DumpConfig_t dumpConfig;
} DSL_PM_Dump_t;

#endif /* defined(DSL_PM_DEBUG_MODE_ENABLE) && (DSL_PM_DEBUG_MODE_ENABLE > 0) */


/** @} DRV_DSL_CPE_PM */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_API_PM_H */
