#ifndef __IFX_ADSL_APP_IOCTL_H
#define __IFX_ADSL_APP_IOCTL_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

/** \file
   This file defines the MIB ioctl interface for DSL CPE API in case of using
   RFC2662 and RFC3440
*/

/*
   take care to avoid excluding any code sections if not used for generating of
   documentation
*/
#ifndef SWIG
#undef DSL_DOC_GENERATION_EXCLUDE_MIB
#undef DSL_DOC_GENERATION_EXCLUDE_WEB
#endif

/** \addtogroup DRV_DSL_CPE_MIB
 @{ */

#ifndef DSL_DOC_GENERATION_EXCLUDE_MIB

/**
   This table includes common attributes describing both ends of the line.
   It is required for all ADSL physical interfaces. ADSL physical interfaces
   are those ifEntries where ifType is equal to adsl(94).

   \param nData*
      The parameter points to a \ref adslLineTableEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslLineTableEntry nData = { 0 };
      ret = ioctl(fd, GET_ADSL_LINE_CODE, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslLineTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.1\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslLineTable(1)\n
      Module:           ADSL-LINE-MIB (RFC2662)\n
      Parent:           adslMibObjects\n
      First child:      adslLineEntry\n
      Next sibling:     adslAtucPhysTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslLineEntry\n
      Composed syntax:  SEQUENCE OF AdslLineEntry\n
      Max access:       not-accessible\n
      Sequences:\n
        1: adslLineCoding - AdslLineCodingType(2 - integer (32 bit))\n
        2: adslLineType - INTEGER(2 - integer (32 bit))\n
        3: adslLineSpecific - VariablePointer(6 - object identifier)\n
        4: adslLineConfProfile - SnmpAdminString(4 - octets)\n
        5: adslLineAlarmConfProfile - SnmpAdminString(4 - octets)\n
*/
#define GET_ADSL_LINE_CODE                (1 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUC. Each row contains the Physical
   Layer Parameters table for that ATUC. ADSL physical interfaces are those
   ifEntries where ifType is equal to adsl(94).

   \param nData*
      The parameter points to a \ref adslAtucPhysEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslAtucPhysEntry nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUC_PHY, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAtucPhysTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.2\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAtucPhysTable(2)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAtucPhysEntry\n
      Prev sibling:     adslLineTable\n
      Next sibling:     adslAturPhysTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAtucPhysEntry\n
      Composed syntax:  SEQUENCE OF AdslAtucPhysEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAtucInvSerialNumber - SnmpAdminString(4 - octets)\n
         2: adslAtucInvVendorID - SnmpAdminString(4 - octets)\n
         3: adslAtucInvVersionNumber - SnmpAdminString(4 - octets)\n
         4: adslAtucCurrSnrMgn - INTEGER(2 - integer (32 bit))\n
         5: adslAtucCurrAtn - Gauge32(66 - gauge (32 bit))\n
         6: adslAtucCurrStatus - BITS(4 - octets)\n
         7: adslAtucCurrOutputPwr - INTEGER(2 - integer (32 bit))\n
         8: adslAtucCurrAttainableRate - Gauge32(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUC_PHY                 (4 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUR. Each row contains the Physical
   Layer Parameters table for that ATUR. ADSL physical interfaces are those
   ifEntries where ifType is equal to adsl(94).

   \param nData*
      The parameter points to a \ref adslAturPhysEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      Name:             adslAturPhysTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.3\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAturPhysTable(3)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAturPhysEntry\n
      Prev sibling:     adslAtucPhysTable\n
      Next sibling:     adslAtucChanTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAturPhysEntry\n
      Composed syntax:  SEQUENCE OF AdslAturPhysEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAturInvSerialNumber - SnmpAdminString(4 - octets)\n
         2: adslAturInvVendorID - SnmpAdminString(4 - octets)\n
         3: adslAturInvVersionNumber - SnmpAdminString(4 - octets)\n
         4: adslAturCurrSnrMgn - INTEGER(2 - integer (32 bit))\n
         5: adslAturCurrAtn - Gauge32(66 - gauge (32 bit))\n
         6: adslAturCurrStatus - BITS(4 - octets)\n
         7: adslAturCurrOutputPwr - INTEGER(2 - integer (32 bit))\n
         8: adslAturCurrAttainableRate - Gauge32(66 - gauge (32 bit))\n

   \code
      adslAturPhysEntry nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUR_PHY, (int) &nData);
      // process received data
   \endcode */
#define GET_ADSL_ATUR_PHY                 (10 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUC channel. ADSL channel interfaces
   are those ifEntries where ifType is equal to adslInterleave(124) or
   adslFast(125).

   \param nData*
      The parameter points to a \ref adslAtucChanInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslAtucChanInfo nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUC_CHAN_INFO, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAtucChanTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.4\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAtucChanTable(4)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAtucChanEntry\n
      Prev sibling:     adslAturPhysTable\n
      Next sibling:     adslAturChanTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAtucChanEntry\n
      Composed syntax:  SEQUENCE OF AdslAtucChanEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAtucChanInterleaveDelay - Gauge32(66 - gauge (32 bit))\n
         2: adslAtucChanCurrTxRate - Gauge32(66 - gauge (32 bit))\n
         3: adslAtucChanPrevTxRate - Gauge32(66 - gauge (32 bit))\n
         4: adslAtucChanCrcBlockLength - Gauge32(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUC_CHAN_INFO           (15 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUR channel. ADSL channel interfaces
   are those ifEntries where ifType is equal to adslInterleave(124) or
   adslFast(125).

   \param nData*
      The parameter points to a \ref adslAturChanInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslAturChanInfo nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUR_CHAN_INFO, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAturChanTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.5\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAturChanTable(5)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAturChanEntry\n
      Prev sibling:     adslAtucChanTable\n
      Next sibling:     adslAtucPerfDataTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAturChanEntry\n
      Composed syntax:  SEQUENCE OF AdslAturChanEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAturChanInterleaveDelay - Gauge32(66 - gauge (32 bit))\n
         2: adslAturChanCurrTxRate - Gauge32(66 - gauge (32 bit))\n
         3: adslAturChanPrevTxRate - Gauge32(66 - gauge (32 bit))\n
         4: adslAturChanCrcBlockLength - Gauge32(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUR_CHAN_INFO           (18 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUC. ADSL physical interfaces are
   those ifEntries where ifType is equal to adsl(94).

   \param nData*
      The parameter points to a \ref atucPerfDataEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      atucPerfDataEntry nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUC_PERF_DATA, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAtucPerfDataTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.6\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAtucPerfDataTable(6)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAtucPerfDataEntry\n
      Prev sibling:     adslAturChanTable\n
      Next sibling:     adslAturPerfDataTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAtucPerfDataEntry\n
      Composed syntax:  SEQUENCE OF AdslAtucPerfDataEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAtucPerfLofs - Counter32(65 - counter (32 bit))\n
         2: adslAtucPerfLoss - Counter32(65 - counter (32 bit))\n
         3: adslAtucPerfLols - Counter32(65 - counter (32 bit))\n
         4: adslAtucPerfLprs - Counter32(65 - counter (32 bit))\n
         5: adslAtucPerfESs - Counter32(65 - counter (32 bit))\n
         6: adslAtucPerfInits - Counter32(65 - counter (32 bit))\n
         7: adslAtucPerfValidIntervals - INTEGER(2 - integer (32 bit))\n
         8: adslAtucPerfInvalidIntervals - INTEGER(2 - integer (32 bit))\n
         9: adslAtucPerfCurr15MinTimeElapsed - AdslPerfTimeElapsed(66 - gauge (32 bit))\n
         10: adslAtucPerfCurr15MinLofs - PerfCurrentCount(66 - gauge (32 bit))\n
         11: adslAtucPerfCurr15MinLoss - PerfCurrentCount(66 - gauge (32 bit))\n
         12: adslAtucPerfCurr15MinLols - PerfCurrentCount(66 - gauge (32 bit))\n
         13: adslAtucPerfCurr15MinLprs - PerfCurrentCount(66 - gauge (32 bit))\n
         14: adslAtucPerfCurr15MinESs - PerfCurrentCount(66 - gauge (32 bit))\n
         15: adslAtucPerfCurr15MinInits - PerfCurrentCount(66 - gauge (32 bit))\n
         16: adslAtucPerfCurr1DayTimeElapsed - AdslPerfTimeElapsed(66 - gauge (32 bit))\n
         17: adslAtucPerfCurr1DayLofs - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         18: adslAtucPerfCurr1DayLoss - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         19: adslAtucPerfCurr1DayLols - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         20: adslAtucPerfCurr1DayLprs - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         21: adslAtucPerfCurr1DayESs - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         22: adslAtucPerfCurr1DayInits - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         23: adslAtucPerfPrev1DayMoniSecs - INTEGER(2 - integer (32 bit))\n
         24: adslAtucPerfPrev1DayLofs - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         25: adslAtucPerfPrev1DayLoss - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         26: adslAtucPerfPrev1DayLols - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         27: adslAtucPerfPrev1DayLprs - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         28: adslAtucPerfPrev1DayESs - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         29: adslAtucPerfPrev1DayInits - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUC_PERF_DATA           (21 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUR. ADSL physical interfaces are
   those ifEntries where ifType is equal to adsl(94).

   \param nData*
      The parameter points to a \ref aturPerfDataEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      aturPerfDataEntry nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUR_PERF_DATA, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAturPerfDataTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.7\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAturPerfDataTable(7)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAturPerfDataEntry\n
      Prev sibling:     adslAtucPerfDataTable\n
      Next sibling:     adslAtucIntervalTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAturPerfDataEntry\n
      Composed syntax:  SEQUENCE OF AdslAturPerfDataEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAturPerfLofs - Counter32(65 - counter (32 bit))\n
         2: adslAturPerfLoss - Counter32(65 - counter (32 bit))\n
         3: adslAturPerfLprs - Counter32(65 - counter (32 bit))\n
         4: adslAturPerfESs - Counter32(65 - counter (32 bit))\n
         5: adslAturPerfValidIntervals - INTEGER(2 - integer (32 bit))\n
         6: adslAturPerfInvalidIntervals - INTEGER(2 - integer (32 bit))\n
         7: adslAturPerfCurr15MinTimeElapsed - AdslPerfTimeElapsed(66 - gauge (32 bit))\n
         8: adslAturPerfCurr15MinLofs - PerfCurrentCount(66 - gauge (32 bit))\n
         9: adslAturPerfCurr15MinLoss - PerfCurrentCount(66 - gauge (32 bit))\n
         10: adslAturPerfCurr15MinLprs - PerfCurrentCount(66 - gauge (32 bit))\n
         11: adslAturPerfCurr15MinESs - PerfCurrentCount(66 - gauge (32 bit))\n
         12: adslAturPerfCurr1DayTimeElapsed - AdslPerfTimeElapsed(66 - gauge (32 bit))\n
         13: adslAturPerfCurr1DayLofs - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         14: adslAturPerfCurr1DayLoss - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         15: adslAturPerfCurr1DayLprs - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         16: adslAturPerfCurr1DayESs - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         17: adslAturPerfPrev1DayMoniSecs - INTEGER(2 - integer (32 bit))\n
         18: adslAturPerfPrev1DayLofs - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         19: adslAturPerfPrev1DayLoss - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         20: adslAturPerfPrev1DayLprs - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         21: adslAturPerfPrev1DayESs - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUR_PERF_DATA           (40 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUC performance data collection
   interval. ADSL physical interfaces are those ifEntries where ifType is equal
   to adsl(94).

   \param nData*
      The parameter points to a \ref adslAtucIntvlInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslAtucIntvlInfo nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUC_INTVL_INFO, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAtucIntervalTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.8\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAtucIntervalTable(8)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAtucIntervalEntry\n
      Prev sibling:     adslAturPerfDataTable\n
      Next sibling:     adslAturIntervalTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAtucIntervalEntry\n
      Composed syntax:  SEQUENCE OF AdslAtucIntervalEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAtucIntervalNumber - INTEGER(2 - integer (32 bit))\n
         2: adslAtucIntervalLofs - PerfIntervalCount(66 - gauge (32 bit))\n
         3: adslAtucIntervalLoss - PerfIntervalCount(66 - gauge (32 bit))\n
         4: adslAtucIntervalLols - PerfIntervalCount(66 - gauge (32 bit))\n
         5: adslAtucIntervalLprs - PerfIntervalCount(66 - gauge (32 bit))\n
         6: adslAtucIntervalESs - PerfIntervalCount(66 - gauge (32 bit))\n
         7: adslAtucIntervalInits - PerfIntervalCount(66 - gauge (32 bit))\n
         8: adslAtucIntervalValidData - TruthValue(2 - integer (32 bit))\n
*/
#define GET_ADSL_ATUC_INTVL_INFO          (60 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUC performance data collection
   interval. ADSL physical interfaces are those ifEntries where ifType is equal
   to adsl(94).

   \param nData*
      The parameter points to a \ref adslAturIntvlInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslAturIntvlInfo nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUR_INTVL_INFO, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAturIntervalTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.9\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAturIntervalTable(9)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAturIntervalEntry\n
      Prev sibling:     adslAtucIntervalTable\n
      Next sibling:     adslAtucChanPerfDataTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAturIntervalEntry\n
      Composed syntax:  SEQUENCE OF AdslAturIntervalEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAturIntervalNumber - INTEGER(2 - integer (32 bit))\n
         2: adslAturIntervalLofs - PerfIntervalCount(66 - gauge (32 bit))\n
         3: adslAturIntervalLoss - PerfIntervalCount(66 - gauge (32 bit))\n
         4: adslAturIntervalLprs - PerfIntervalCount(66 - gauge (32 bit))\n
         5: adslAturIntervalESs - PerfIntervalCount(66 - gauge (32 bit))\n
         6: adslAturIntervalValidData - TruthValue(2 - integer (32 bit))\n
*/
#define GET_ADSL_ATUR_INTVL_INFO          (65 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUC channel. ADSL channel interfaces
   are those ifEntries where ifType is equal to adslInterleave(124) or
   adslFast(125).

   \param nData*
      The parameter points to a \ref atucChannelPerfDataEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      atucChannelPerfDataEntry nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUC_CHAN_PERF_DATA, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAtucChanPerfDataTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.10\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAtucChanPerfDataTable(10)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAtucChanPerfDataEntry\n
      Prev sibling:     adslAturIntervalTable\n
      Next sibling:     adslAturChanPerfDataTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAtucChanPerfDataEntry\n
      Composed syntax:  SEQUENCE OF AdslAtucChanPerfDataEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAtucChanReceivedBlks - Counter32(65 - counter (32 bit))\n
         2: adslAtucChanTransmittedBlks - Counter32(65 - counter (32 bit))\n
         3: adslAtucChanCorrectedBlks - Counter32(65 - counter (32 bit))\n
         4: adslAtucChanUncorrectBlks - Counter32(65 - counter (32 bit))\n
         5: adslAtucChanPerfValidIntervals - INTEGER(2 - integer (32 bit))\n
         6: adslAtucChanPerfInvalidIntervals - INTEGER(2 - integer (32 bit))\n
         7: adslAtucChanPerfCurr15MinTimeElapsed - AdslPerfTimeElapsed(66 - gauge (32 bit))\n
         8: adslAtucChanPerfCurr15MinReceivedBlks - PerfCurrentCount(66 - gauge (32 bit))\n
         9: adslAtucChanPerfCurr15MinTransmittedBlks - PerfCurrentCount(66 - gauge (32 bit))\n
         10: adslAtucChanPerfCurr15MinCorrectedBlks - PerfCurrentCount(66 - gauge (32 bit))\n
         11: adslAtucChanPerfCurr15MinUncorrectBlks - PerfCurrentCount(66 - gauge (32 bit))\n
         12: adslAtucChanPerfCurr1DayTimeElapsed - AdslPerfTimeElapsed(66 - gauge (32 bit))\n
         13: adslAtucChanPerfCurr1DayReceivedBlks - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         14: adslAtucChanPerfCurr1DayTransmittedBlks - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         15: adslAtucChanPerfCurr1DayCorrectedBlks - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         16: adslAtucChanPerfCurr1DayUncorrectBlks - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         17: adslAtucChanPerfPrev1DayMoniSecs - INTEGER(2 - integer (32 bit))\n
         18: adslAtucChanPerfPrev1DayReceivedBlks - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         19: adslAtucChanPerfPrev1DayTransmittedBlks - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         20: adslAtucChanPerfPrev1DayCorrectedBlks - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         21: adslAtucChanPerfPrev1DayUncorrectBlks - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUC_CHAN_PERF_DATA      (70 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUR channel. ADSL channel interfaces
   are those ifEntries where ifType is equal to adslInterleave(124) or
   adslFast(125).

   \param nData*
      The parameter points to a \ref aturChannelPerfDataEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      aturChannelPerfDataEntry nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUR_CHAN_PERF_DATA, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAturChanPerfDataTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.11\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAturChanPerfDataTable(11)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAturChanPerfDataEntry\n
      Prev sibling:     adslAtucChanPerfDataTable\n
      Next sibling:     adslAtucChanIntervalTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAturChanPerfDataEntry\n
      Composed syntax:  SEQUENCE OF AdslAturChanPerfDataEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAturChanReceivedBlks - Counter32(65 - counter (32 bit))\n
         2: adslAturChanTransmittedBlks - Counter32(65 - counter (32 bit))\n
         3: adslAturChanCorrectedBlks - Counter32(65 - counter (32 bit))\n
         4: adslAturChanUncorrectBlks - Counter32(65 - counter (32 bit))\n
         5: adslAturChanPerfValidIntervals - INTEGER(2 - integer (32 bit))\n
         6: adslAturChanPerfInvalidIntervals - INTEGER(2 - integer (32 bit))\n
         7: adslAturChanPerfCurr15MinTimeElapsed - AdslPerfTimeElapsed(66 - gauge (32 bit))\n
         8: adslAturChanPerfCurr15MinReceivedBlks - PerfCurrentCount(66 - gauge (32 bit))\n
         9: adslAturChanPerfCurr15MinTransmittedBlks - PerfCurrentCount(66 - gauge (32 bit))\n
         10: adslAturChanPerfCurr15MinCorrectedBlks - PerfCurrentCount(66 - gauge (32 bit))\n
         11: adslAturChanPerfCurr15MinUncorrectBlks - PerfCurrentCount(66 - gauge (32 bit))\n
         12: adslAturChanPerfCurr1DayTimeElapsed - AdslPerfTimeElapsed(66 - gauge (32 bit))\n
         13: adslAturChanPerfCurr1DayReceivedBlks - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         14: adslAturChanPerfCurr1DayTransmittedBlks - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         15: adslAturChanPerfCurr1DayCorrectedBlks - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         16: adslAturChanPerfCurr1DayUncorrectBlks - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         17: adslAturChanPerfPrev1DayMoniSecs - INTEGER(2 - integer (32 bit))\n
         18: adslAturChanPerfPrev1DayReceivedBlks - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         19: adslAturChanPerfPrev1DayTransmittedBlks - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         20: adslAturChanPerfPrev1DayCorrectedBlks - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         21: adslAturChanPerfPrev1DayUncorrectBlks - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUR_CHAN_PERF_DATA      (90 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUC channel's performance data
   collection interval. ADSL channel interfaces are those ifEntries where
   ifType is equal to adslInterleave(124) or adslFast(125).

   \param nData*
      The parameter points to a \ref adslAtucChanIntvlInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslAtucChanIntvlInfo nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUC_CHAN_INTVL_INFO, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAtucChanIntervalTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.12\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAtucChanIntervalTable(12)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAtucChanIntervalEntry\n
      Prev sibling:     adslAturChanPerfDataTable\n
      Next sibling:     adslAturChanIntervalTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAtucChanIntervalEntry\n
      Composed syntax:  SEQUENCE OF AdslAtucChanIntervalEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAtucChanIntervalNumber - INTEGER(2 - integer (32 bit))\n
         2: adslAtucChanIntervalReceivedBlks - PerfIntervalCount(66 - gauge (32 bit))\n
         3: adslAtucChanIntervalTransmittedBlks - PerfIntervalCount(66 - gauge (32 bit))\n
         4: adslAtucChanIntervalCorrectedBlks - PerfIntervalCount(66 - gauge (32 bit))\n
         5: adslAtucChanIntervalUncorrectBlks - PerfIntervalCount(66 - gauge (32 bit))\n
         6: adslAtucChanIntervalValidData - TruthValue(2 - integer (32 bit))\n
*/
#define GET_ADSL_ATUC_CHAN_INTVL_INFO     (110 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATUR channel's performance data
   collection interval. ADSL channel interfaces are those ifEntries where
   ifType is equal to adslInterleave(124) or adslFast(125).

   \param nData*
      The parameter points to a \ref adslAturChanIntvlInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslAturChanIntvlInfo nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ATUR_CHAN_INTVL_INFO, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAturChanIntervalTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.13\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslAturChanIntervalTable(13)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslAturChanIntervalEntry\n
      Prev sibling:     adslAtucChanIntervalTable\n
      Next sibling:     adslLineConfProfileTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAturChanIntervalEntry\n
      Composed syntax:  SEQUENCE OF AdslAturChanIntervalEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAturChanIntervalNumber - INTEGER(2 - integer (32 bit))\n
         2: adslAturChanIntervalReceivedBlks - PerfIntervalCount(66 - gauge (32 bit))\n
         3: adslAturChanIntervalTransmittedBlks - PerfIntervalCount(66 - gauge (32 bit))\n
         4: adslAturChanIntervalCorrectedBlks - PerfIntervalCount(66 - gauge (32 bit))\n
         5: adslAturChanIntervalUncorrectBlks - PerfIntervalCount(66 - gauge (32 bit))\n
         6: adslAturChanIntervalValidData - TruthValue(2 - integer (32 bit))\n
*/
#define GET_ADSL_ATUR_CHAN_INTVL_INFO     (115 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table contains information on the ADSL line configuration. One entry in
   this table reflects a profile defined by a manager which can be used to
   configure the modem for a physical line

   \param nData*
      The parameter points to a \ref adslLineAlarmConfProfileEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslLineAlarmConfProfileEntry nData = { 0 };
      ret = ioctl(fd, GET_ADSL_ALRM_CONF_PROF, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslLineAlarmConfProfileTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.1.1.15\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslMibObjects(1).adslLineAlarmConfProfileTable(15)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslMibObjects\n
      First child:      adslLineAlarmConfProfileEntry\n
      Prev sibling:     adslLineConfProfileTable\n
      Next sibling:     adslLCSMib\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslLineAlarmConfProfileEntry\n
      Composed syntax:  SEQUENCE OF AdslLineAlarmConfProfileEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslLineAlarmConfProfileName - SnmpAdminString(4 - octets)\n
         2: adslAtucThresh15MinLofs - INTEGER(2 - integer (32 bit))\n
         3: adslAtucThresh15MinLoss - INTEGER(2 - integer (32 bit))\n
         4: adslAtucThresh15MinLols - INTEGER(2 - integer (32 bit))\n
         5: adslAtucThresh15MinLprs - INTEGER(2 - integer (32 bit))\n
         6: adslAtucThresh15MinESs - INTEGER(2 - integer (32 bit))\n
         7: adslAtucThreshFastRateUp - Unsigned32(66 - gauge (32 bit))\n
         8: adslAtucThreshInterleaveRateUp - Unsigned32(66 - gauge (32 bit))\n
         9: adslAtucThreshFastRateDown - Unsigned32(66 - gauge (32 bit))\n
         10: adslAtucThreshInterleaveRateDown - Unsigned32(66 - gauge (32 bit))\n
         11: adslAtucInitFailureTrapEnable - INTEGER(2 - integer (32 bit))\n
         12: adslAturThresh15MinLofs - INTEGER(2 - integer (32 bit))\n
         13: adslAturThresh15MinLoss - INTEGER(2 - integer (32 bit))\n
         14: adslAturThresh15MinLprs - INTEGER(2 - integer (32 bit))\n
         15: adslAturThresh15MinESs - INTEGER(2 - integer (32 bit))\n
         16: adslAturThreshFastRateUp - Unsigned32(66 - gauge (32 bit))\n
         17: adslAturThreshInterleaveRateUp - Unsigned32(66 - gauge (32 bit))\n
         18: adslAturThreshFastRateDown - Unsigned32(66 - gauge (32 bit))\n
         19: adslAturThreshInterleaveRateDown - Unsigned32(66 - gauge (32 bit))\n
         20: adslLineAlarmConfProfileRowStatus - RowStatus(2 - integer (32 bit))\n
*/
#define GET_ADSL_ALRM_CONF_PROF           (120 + IFX_ADSL_IOC_MIB_BASE)
/**
   For a description of this ioctl please refer to \ref GET_ADSL_ALRM_CONF_PROF.
   The only difference is the access type get/set */
#define SET_ADSL_ALRM_CONF_PROF           (121 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns a bitfield indicating which traps has been occured on
   ATU-R side.

   \param nData*
      The parameter points to a value that has to interpreted as a bitfield.
      The meanung of the single bits is defined by \ref adslAturTrapsFlags_t

   \return
      0 if successful and -1 in case of an error/warning

   \code
      unsigned log nData = 0;
      ret = ioctl(fd, ADSL_ATUR_TRAPS, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAturTraps\n
      Type:             OBJECT-IDENTIFIER\n
      OID:              1.3.6.1.2.1.10.94.1.2.2\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslLineMib(1).\n
                        adslTraps(2).adslAturTraps(2)\n
      Module:           ADSL-LINE-MIB\n
      Parent:           adslTraps\n
      First child:      adslAturTrapsx0\n
      Prev sibling:     adslAtucTraps\n
*/
#define ADSL_ATUR_TRAPS                   (135 + IFX_ADSL_IOC_MIB_BASE)


/* RFC-3440 */

#ifdef IFX_ADSL_MIB_RFC3440

/**
   This table is an extension of RFC 2662. It contains ADSL line configuration
   and monitoring information. This includes the ADSL line's capabilities and
   actual ADSL transmission system.

   \param nData*
      The parameter points to a \ref adslLineExtTableEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslLineExtTableEntry nData = 0;
      ret = ioctl(fd, GET_ADSL_ATUC_LINE_EXT, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslLineExtTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.3.1.17\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslExtMIB(3).\n
                        adslExtMibObjects(1).adslLineExtTable(17)\n
      Module:           ADSL-LINE-EXT-MIB\n
      Parent:           adslExtMibObjects\n
      First child:      adslLineExtEntry\n
      Next sibling:     adslAtucPerfDataExtTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslLineExtEntry\n
      Composed syntax:  SEQUENCE OF AdslLineExtEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslLineTransAtucCap - AdslTransmissionModeType(4 - octets)\n
         2: adslLineTransAtucConfig - AdslTransmissionModeType(4 - octets)\n
         3: adslLineTransAtucActual - AdslTransmissionModeType(4 - octets)\n
         4: adslLineGlitePowerState - INTEGER(2 - integer (32 bit))\n
         5: adslLineConfProfileDualLite - SnmpAdminString(4 - octets)\n
*/
#define GET_ADSL_ATUC_LINE_EXT            (201 + IFX_ADSL_IOC_MIB_BASE)
/**
   For a description of this ioctl please refer to \ref GET_ADSL_ATUC_LINE_EXT.
   The only difference is the access type get/set */
#define SET_ADSL_ATUC_LINE_EXT            (203 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table extends adslAtucPerfDataTable [RFC2662] with additional ADSL
   physical line counter information such as unavailable seconds-line and
   severely errored seconds-line.

   \param nData*
      The parameter points to a \ref atucPerfDataExtEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      atucPerfDataExtEntry nData = 0;
      ret = ioctl(fd, GET_ADSL_ATUC_PERF_DATA_EXT, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslLineExtTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.3.1.17\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslExtMIB(3).\n
                        adslExtMibObjects(1).adslLineExtTable(17)\n
      Module:           ADSL-LINE-EXT-MIB\n
      Parent:           adslExtMibObjects\n
      First child:      adslLineExtEntry\n
      Next sibling:     adslAtucPerfDataExtTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslLineExtEntry\n
      Composed syntax:  SEQUENCE OF AdslLineExtEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslLineTransAtucCap - AdslTransmissionModeType(4 - octets)\n
         2: adslLineTransAtucConfig - AdslTransmissionModeType(4 - octets)\n
         3: adslLineTransAtucActual - AdslTransmissionModeType(4 - octets)\n
         4: adslLineGlitePowerState - INTEGER(2 - integer (32 bit))\n
         5: adslLineConfProfileDualLite - SnmpAdminString(4 - octets)\n
*/
#define GET_ADSL_ATUC_PERF_DATA_EXT       (205 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATU-C performance data collection
   interval for ADSL physical interfaces whose IfEntries' ifType is equal to
   adsl(94).

   \param nData*
      The parameter points to a \ref adslAtucInvtlExtInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslAtucInvtlExtInfo nData = 0;
      ret = ioctl(fd, GET_ADSL_ATUC_INTVL_EXT_INFO, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAtucIntervalExtTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.3.1.19\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslExtMIB(3).\n
                        adslExtMibObjects(1).adslAtucIntervalExtTable(19)\n
      Module:           ADSL-LINE-EXT-MIB\n
      Parent:           adslExtMibObjects\n
      First child:      adslAtucIntervalExtEntry\n
      Prev sibling:     adslAtucPerfDataExtTable\n
      Next sibling:     adslAturPerfDataExtTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAtucIntervalExtEntry\n
      Composed syntax:  SEQUENCE OF AdslAtucIntervalExtEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAtucIntervalFastR - PerfIntervalCount(66 - gauge (32 bit))\n
         2: adslAtucIntervalFailedFastR - PerfIntervalCount(66 - gauge (32 bit))\n
         3: adslAtucIntervalSesL - PerfIntervalCount(66 - gauge (32 bit))\n
         4: adslAtucIntervalUasL - PerfIntervalCount(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUC_INTVL_EXT_INFO      (221 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table contains ADSL physical line counters not defined in the
   adslAturPerfDataTable from the ADSL-LINE-MIB [RFC2662].

   \param nData*
      The parameter points to a \ref aturPerfDataExtEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      aturPerfDataExtEntry nData = 0;
      ret = ioctl(fd, GET_ADSL_ATUR_PERF_DATA_EXT, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAturPerfDataExtTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.3.1.20\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslExtMIB(3).\n
                        adslExtMibObjects(1).adslAturPerfDataExtTable(20)\n
      Module:           ADSL-LINE-EXT-MIB\n
      Parent:           adslExtMibObjects\n
      First child:      adslAturPerfDataExtEntry\n
      Prev sibling:     adslAtucIntervalExtTable\n
      Next sibling:     adslAturIntervalExtTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAturPerfDataExtEntry\n
      Composed syntax:  SEQUENCE OF AdslAturPerfDataExtEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAturPerfStatSesL - Counter32(65 - counter (32 bit))\n
         2: adslAturPerfStatUasL - Counter32(65 - counter (32 bit))\n
         3: adslAturPerfCurr15MinSesL - PerfCurrentCount(66 - gauge (32 bit))\n
         4: adslAturPerfCurr15MinUasL - PerfCurrentCount(66 - gauge (32 bit))\n
         5: adslAturPerfCurr1DaySesL - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         6: adslAturPerfCurr1DayUasL - AdslPerfCurrDayCount(66 - gauge (32 bit))\n
         7: adslAturPerfPrev1DaySesL - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
         8: adslAturPerfPrev1DayUasL - AdslPerfPrevDayCount(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUR_PERF_DATA_EXT       (225 + IFX_ADSL_IOC_MIB_BASE)

/**
   This table provides one row for each ATU-R performance data collection
   interval for ADSL physical interfaces whose IfEntries' ifType is equal to
   adsl(94).

   \param nData*
      The parameter points to a \ref aturPerfDataExtEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      aturPerfDataExtEntry nData = 0;
      ret = ioctl(fd, GET_ADSL_ATUR_INTVL_EXT_INFO, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslAturIntervalExtTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.3.1.21\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslExtMIB(3).\n
                        adslExtMibObjects(1).adslAturIntervalExtTable(21)\n
      Module:           ADSL-LINE-EXT-MIB\n
      Parent:           adslExtMibObjects\n
      First child:      adslAturIntervalExtEntry\n
      Prev sibling:     adslAturPerfDataExtTable\n
      Next sibling:     adslConfProfileExtTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslAturIntervalExtEntry\n
      Composed syntax:  SEQUENCE OF AdslAturIntervalExtEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslAturIntervalSesL - PerfIntervalCount(66 - gauge (32 bit))\n
         2: adslAturIntervalUasL - PerfIntervalCount(66 - gauge (32 bit))\n
*/
#define GET_ADSL_ATUR_INTVL_EXT_INFO      (233 + IFX_ADSL_IOC_MIB_BASE)

/**
   The adslConfProfileExtTable extends the ADSL line profile configuration
   information in the adslLineConfProfileTable from the ADSL-LINE-MIB [RFC2662]
   by adding the ability to configure the ADSL physical line mode.

   \param nData*
      The parameter points to a \ref adslLineAlarmConfProfileExtEntry structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslLineAlarmConfProfileExtEntry nData = 0;
      ret = ioctl(fd, GET_ADSL_ALRM_CONF_PROF_EXT, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslConfProfileExtTable\n
      Type:             OBJECT-TYPE\n
      OID:              1.3.6.1.2.1.10.94.3.1.22\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslExtMIB(3).\n
                        adslExtMibObjects(1).adslConfProfileExtTable(22)\n
      Module:           ADSL-LINE-EXT-MIB\n
      Parent:           adslExtMibObjects\n
      First child:      adslConfProfileExtEntry\n
      Prev sibling:     adslAturIntervalExtTable\n
      Next sibling:     adslAlarmConfProfileExtTable\n
      Numerical syntax: Sequence\n
      Base syntax:      SEQUENCE OF AdslConfProfileExtEntry\n
      Composed syntax:  SEQUENCE OF AdslConfProfileExtEntry\n
      Max access:       not-accessible\n
      Sequences:\n
         1: adslConfProfileLineType - INTEGER(2 - integer (32 bit))\n
*/
#define GET_ADSL_ALRM_CONF_PROF_EXT       (235 + IFX_ADSL_IOC_MIB_BASE)
/**
   For a description of this ioctl please refer to \ref GET_ADSL_ALRM_CONF_PROF_EXT.
   The only difference is the access type get/set */
#define SET_ADSL_ALRM_CONF_PROF_EXT       (236 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns a bitfield indicating which traps has been occured on
   ATU-R side.

   \param nData*
      The parameter points to a value that has to interpreted as a bitfield.
      The meanung of the single bits is defined by \ref adslAturTrapsFlags_t

   \return
      0 if successful and -1 in case of an error/warning

   \code
      unsigned log nData = 0;
      ret = ioctl(fd, ADSL_ATUR_EXT_TRAPS, (int) &nData);
      // process received data
   \endcode

   \remarks
      Name:             adslExtTraps\n
      Type:             OBJECT-IDENTIFIER\n
      OID:              1.3.6.1.2.1.10.94.3.1.24\n
      Full path:        iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1).\n
                        transmission(10).adslMIB(94).adslExtMIB(3).\n
                        adslExtMibObjects(1).adslExtTraps(24)\n
      Module:           ADSL-LINE-EXT-MIB\n
      Parent:           adslExtMibObjects\n
      First child:      adslExtAtucTraps\n
      Prev sibling:     adslAlarmConfProfileExtTable\n
*/
#define ADSL_ATUR_EXT_TRAPS               (240 + IFX_ADSL_IOC_MIB_BASE)

#endif /* IFX_ADSL_MIB_RFC3440 */

#endif /* DSL_DOC_GENERATION_EXCLUDE_MIB */

/** @} DRV_DSL_CPE_MIB */


/** \addtogroup DRV_DSL_CPE_WEB
 @{ */

#ifndef DSL_DOC_GENERATION_EXCLUDE_WEB

/* The following constants are added to support the WEB related ADSL Statistics */

/**
   This ioctl returns line status informations.

   \param adslLineStatusInfo*
      This parameter points to a \ref adslLineStatusInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslLineStatusInfo nData;
      memset(&nData, o, sizeof(adslLineStatusInfo));
      ret = ioctl(fd, GET_ADSL_LINE_STATUS, (int) &nData);
      // process received data
   \endcode
*/
#define GET_ADSL_LINE_STATUS              (245 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns line rate status informations.

   \param adslLineRateInfo*
      This parameter points to a \ref adslLineRateInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslLineRateInfo nData;
      memset(&nData, o, sizeof(adslLineRateInfo));
      ret = ioctl(fd, GET_ADSL_LINE_RATE, (int) &nData);
      // process received data
   \endcode
*/
#define GET_ADSL_LINE_RATE                (250 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns common line status informations.

   \param adslLineInfo*
      This parameter points to a \ref adslLineInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslLineInfo nData;
      memset(&nData, o, sizeof(adslLineInfo));
      ret = ioctl(fd, GET_ADSL_LINE_INFO, (int) &nData);
      // process received data
   \endcode
*/
#define GET_ADSL_LINE_INFO                (255 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns Near End Performance statistics.

   \param adslNearEndPerfStats*
      This parameter points to a \ref adslNearEndPerfStats structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslNearEndPerfStats nData;
      memset(&nData, o, sizeof(adslNearEndPerfStats));
      ret = ioctl(fd, GET_ADSL_NEAREND_STATS, (int) &nData);
      // process received data
   \endcode
*/
#define GET_ADSL_NEAREND_STATS            (270 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns Far End Performance statistics.

   \param adslFarEndPerfStats*
      This parameter points to a \ref adslFarEndPerfStats structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslFarEndPerfStats nData;
      memset(&nData, o, sizeof(adslFarEndPerfStats));
      ret = ioctl(fd, GET_ADSL_FAREND_STATS, (int) &nData);
      // process received data
   \endcode
*/
#define GET_ADSL_FAREND_STATS             (290 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns line initialization statistics.

   \param adslInitStats*
      This parameter points to a \ref adslInitStats structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslInitStats nData;
      memset(&nData, o, sizeof(adslInitStats));
      ret = ioctl(fd, GET_ADSL_LINE_INIT_STATS, (int) &nData);
      // process received data
   \endcode
*/
#define GET_ADSL_LINE_INIT_STATS          (150 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns Power Spectrum Density status information.

   \param adslPowerSpectralDensity*
      This parameter points to a \ref adslPowerSpectralDensity structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslPowerSpectralDensity nData;
      memset(&nData, o, sizeof(adslPowerSpectralDensity));
      ret = ioctl(fd, GET_ADSL_POWER_SPECTRAL_DENSITY, (int) &nData);
      // process received data
   \endcode
*/
#define GET_ADSL_POWER_SPECTRAL_DENSITY   (151 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns Sub-carrier related parameters for upstream.

   \param adslATUCSubcarrierInfo*
      This parameter points to a \ref adslATUCSubcarrierInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslATUCSubcarrierInfo nData;
      memset(&nData, o, sizeof(adslATUCSubcarrierInfo));
      ret = ioctl(fd, GET_ADSL_ATUC_SUBCARRIER_STATS, (int) &nData);
      // process received data
   \endcode
*/
#define GET_ADSL_ATUC_SUBCARRIER_STATS    (297 + IFX_ADSL_IOC_MIB_BASE)

/**
   This ioctl returns Sub-carrier related parameters for downstream.

   \param adslATURSubcarrierInfo*
      This parameter points to a \ref adslATUCSubcarrierInfo structure

   \return
      0 if successful and -1 in case of an error/warning

   \code
      adslATURSubcarrierInfo nData;
      memset(&nData, o, sizeof(adslATURSubcarrierInfo));
      ret = ioctl(fd, GET_ADSL_ATUR_SUBCARRIER_STATS, (int) &nData);
      // process received data
   \endcode
*/
#define GET_ADSL_ATUR_SUBCARRIER_STATS    (298 + IFX_ADSL_IOC_MIB_BASE)

#endif /* DSL_DOC_GENERATION_EXCLUDE_WEB */

/** @} DRV_DSL_CPE_WEB */


/*  makeCMV(Opcode, Group, Address, Index, Size, Data) */

/** \addtogroup DRV_DSL_CPE_MIB
 @{ */

/**
 * \deprecated
 * Not necessary anymore, as this is handled by the autoboot daemon in kernel!
 */
#define IFX_ADSL_MIB_LO_ATUC              (295 + IFX_ADSL_IOC_MIB_BASE)
/**
 * \deprecated
 * Not necessary anymore, as this is handled by the autoboot daemon in kernel!
 */
#define IFX_ADSL_MIB_LO_ATUR              (296 + IFX_ADSL_IOC_MIB_BASE)


#ifndef DSL_DOC_GENERATION_EXCLUDE_MIB

/** Definitions for adslLineCode flags */
typedef enum
{
   LINE_CODE_FLAG = 0x1   /* BIT 0th position */
} adslLineTableFlags_t;

/** Definitions for adslAtucPhysTable flags */
typedef enum
{
   ATUC_PHY_SER_NUM_FLAG   = 0x1,   /* BIT 0th position */
   ATUC_PHY_VENDOR_ID_FLAG = 0x2,   /* BIT 1 */
   ATUC_PHY_VER_NUM_FLAG   = 0x4,   /* BIT 2 */
   ATUC_CURR_STAT_FLAG     = 0x8,   /* BIT 3 */
   ATUC_CURR_OUT_PWR_FLAG  = 0x10,  /* BIT 4 */
   ATUC_CURR_ATTR_FLAG     = 0x20   /* BIT 5 */
} adslAtucPhysTableFlags_t;

/** Definitions for adslAturPhysTable flags */
typedef enum
{
   ATUR_PHY_SER_NUM_FLAG   = 0x1,   /* BIT 0th position */
   ATUR_PHY_VENDOR_ID_FLAG = 0x2,   /* BIT 1 */
   ATUR_PHY_VER_NUM_FLAG   = 0x4,   /* BIT 2 */
   ATUR_CURR_STAT_FLAG     = 0x20,  /* BIT 3 */
   ATUR_CURR_OUT_PWR_FLAG  = 0x40,  /* BIT 4 */
   ATUR_CURR_ATTR_FLAG     = 0x80   /* BIT 5 */
} adslAturPhysTableFlags_t;

/** Definitions for adslAtucChanInfo flags */
typedef enum
{
   ATUC_CHAN_INTLV_DELAY_FLAG  = 0x1,   /* BIT 0th position */
   ATUC_CHAN_CURR_TX_RATE_FLAG = 0x2,   /* BIT 1 */
   ATUC_CHAN_PREV_TX_RATE_FLAG = 0x4    /* BIT 2 */
} adslAtucChanInfoFlags_t;

/** Definitions for adslAturChanInfo flags */
typedef enum
{
   ATUR_CHAN_INTLV_DELAY_FLAG  = 0x1,   /* BIT 0th position */
   ATUR_CHAN_CURR_TX_RATE_FLAG = 0x2,   /* BIT 1 */
   ATUR_CHAN_PREV_TX_RATE_FLAG = 0x4,   /* BIT 2 */
   ATUR_CHAN_CRC_BLK_LEN_FLAG  = 0x8    /* BIT 3 */
} adslAturChanInfoFlags_t;

/** Definitions for adslAtucPerfDataTable flags */
typedef enum
{
   ATUC_PERF_LOFS_FLAG               = 0x1,     /* BIT 0th position */
   ATUC_PERF_LOSS_FLAG               = 0x2,     /* BIT 1 */
   ATUC_PERF_ESS_FLAG                = 0x4,     /* BIT 2 */
   ATUC_PERF_INITS_FLAG              = 0x8,     /* BIT 3 */
   ATUC_PERF_VALID_INTVLS_FLAG       = 0x10,    /* BIT 4 */
   ATUC_PERF_INVALID_INTVLS_FLAG     = 0x20,    /* BIT 5 */
   ATUC_PERF_CURR_15MIN_TIME_ELAPSED_FLAG = 0x40, /* BIT 6 */
   ATUC_PERF_CURR_15MIN_LOFS_FLAG    = 0x80,     /* BIT 7 */
   ATUC_PERF_CURR_15MIN_LOSS_FLAG    = 0x100,    /* BIT 8 */
   ATUC_PERF_CURR_15MIN_ESS_FLAG     = 0x200,    /* BIT 9 */
   ATUC_PERF_CURR_15MIN_INIT_FLAG    = 0x400,    /* BIT 10 */
   ATUC_PERF_CURR_1DAY_TIME_ELAPSED_FLAG = 0x800, /* BIT 11 */
   ATUC_PERF_CURR_1DAY_LOFS_FLAG     = 0x1000,   /* BIT 12 */
   ATUC_PERF_CURR_1DAY_LOSS_FLAG     = 0x2000,   /* BIT 13 */
   ATUC_PERF_CURR_1DAY_ESS_FLAG      = 0x4000,   /* BIT 14 */
   ATUC_PERF_CURR_1DAY_INIT_FLAG     = 0x8000,   /* BIT 15 */
   ATUC_PERF_PREV_1DAY_MON_SEC_FLAG  = 0x10000,  /* BIT 16 */
   ATUC_PERF_PREV_1DAY_LOFS_FLAG     = 0x20000,  /* BIT 17 */
   ATUC_PERF_PREV_1DAY_LOSS_FLAG     = 0x40000,  /* BIT 18 */
   ATUC_PERF_PREV_1DAY_ESS_FLAG      = 0x80000,  /* BIT 19 */
   ATUC_PERF_PREV_1DAY_INITS_FLAG    = 0x100000  /* BIT 20 */
} adslAtucPerfDataTableFlags_t;

/** Definitions for adslAturPerfDataTable flags */
typedef enum
{
   ATUR_PERF_LOFS_FLAG               = 0x1,      /* BIT 0th position */
   ATUR_PERF_LOSS_FLAG               = 0x2,      /* BIT 1 */
   ATUR_PERF_LPR_FLAG                = 0x4,      /* BIT 2 */
   ATUR_PERF_ESS_FLAG                = 0x8,      /* BIT 3 */
   ATUR_PERF_VALID_INTVLS_FLAG       = 0x10,     /* BIT 4 */
   ATUR_PERF_INVALID_INTVLS_FLAG     = 0x20,     /* BIT 5 */
   ATUR_PERF_CURR_15MIN_TIME_ELAPSED_FLAG = 0x40, /* BIT 6 */
   ATUR_PERF_CURR_15MIN_LOFS_FLAG    = 0x80,     /* BIT 7 */
   ATUR_PERF_CURR_15MIN_LOSS_FLAG    = 0x100,    /* BIT 8 */
   ATUR_PERF_CURR_15MIN_LPR_FLAG     = 0x200,    /* BIT 9 */
   ATUR_PERF_CURR_15MIN_ESS_FLAG     = 0x400,    /* BIT 10 */
   ATUR_PERF_CURR_1DAY_TIME_ELAPSED_FLAG = 0x800, /* BIT 11 */
   ATUR_PERF_CURR_1DAY_LOFS_FLAG     = 0x1000,   /* BIT 12 */
   ATUR_PERF_CURR_1DAY_LOSS_FLAG     = 0x2000,   /* BIT 13 */
   ATUR_PERF_CURR_1DAY_LPR_FLAG      = 0x4000,   /* BIT 14 */
   ATUR_PERF_CURR_1DAY_ESS_FLAG      = 0x8000,   /* BIT 15 */
   ATUR_PERF_PREV_1DAY_MON_SEC_FLAG  = 0x10000,  /* BIT 16 */
   ATUR_PERF_PREV_1DAY_LOFS_FLAG     = 0x20000,  /* BIT 17 */
   ATUR_PERF_PREV_1DAY_LOSS_FLAG     = 0x40000,  /* BIT 18 */
   ATUR_PERF_PREV_1DAY_LPR_FLAG      = 0x80000,  /* BIT 19 */
   ATUR_PERF_PREV_1DAY_ESS_FLAG      = 0x100000  /* BIT 20 */
} adslAturPerfDataTableFlags_t;

/** Definitions for adslAtucIntvlInfo flags */
typedef enum
{
   ATUC_INTVL_LOF_FLAG      = 0x1,   /* BIT 0th position */
   ATUC_INTVL_LOS_FLAG      = 0x2,   /* BIT 1 */
   ATUC_INTVL_ESS_FLAG      = 0x4,   /* BIT 2 */
   ATUC_INTVL_INIT_FLAG     = 0x8,   /* BIT 3 */
   ATUC_INTVL_VALID_DATA_FLAG = 0x10 /* BIT 4 */
} adslAtucIntvlInfoFlags_t;

/** Definitions for adslAturIntvlInfo flags */
typedef enum
{
   ATUR_INTVL_LOF_FLAG      = 0x1,   /* BIT 0th position */
   ATUR_INTVL_LOS_FLAG      = 0x2,   /* BIT 1 */
   ATUR_INTVL_LPR_FLAG      = 0x4,   /* BIT 2 */
   ATUR_INTVL_ESS_FLAG      = 0x8,   /* BIT 3 */
   ATUR_INTVL_VALID_DATA_FLAG = 0x10 /* BIT 4 */
} adslAturIntvlInfoFlags_t;

/** Definitions for adslAtucChannelPerfDataTable flags */
typedef enum
{
   ATUC_CHAN_RECV_BLK_FLAG                     = 0x01,  /* BIT 0th position */
   ATUC_CHAN_TX_BLK_FLAG                       = 0x02,  /* BIT 1 */
   ATUC_CHAN_CORR_BLK_FLAG                     = 0x04,  /* BIT 2 */
   ATUC_CHAN_UNCORR_BLK_FLAG                   = 0x08,  /* BIT 3 */
   ATUC_CHAN_PERF_VALID_INTVL_FLAG             = 0x10,  /* BIT 4 */
   ATUC_CHAN_PERF_INVALID_INTVL_FLAG           = 0x20,  /* BIT 5 */
   ATUC_CHAN_PERF_CURR_15MIN_TIME_ELAPSED_FLAG = 0x40,  /* BIT 6 */
   ATUC_CHAN_PERF_CURR_15MIN_RECV_BLK_FLAG     = 0x80,  /* BIT 7 */
   ATUC_CHAN_PERF_CURR_15MIN_TX_BLK_FLAG       = 0x100, /* BIT 8 */
   ATUC_CHAN_PERF_CURR_15MIN_CORR_BLK_FLAG     = 0x200, /* BIT 9 */
   ATUC_CHAN_PERF_CURR_15MIN_UNCORR_BLK_FLAG   = 0x400, /* BIT 10 */
   ATUC_CHAN_PERF_CURR_1DAY_TIME_ELAPSED_FLAG  = 0x800, /* BIT 11*/
   ATUC_CHAN_PERF_CURR_1DAY_RECV_BLK_FLAG      = 0x1000, /* BIT 12 */
   ATUC_CHAN_PERF_CURR_1DAY_TX_BLK_FLAG        = 0x2000, /* BIT 13 */
   ATUC_CHAN_PERF_CURR_1DAY_CORR_BLK_FLAG      = 0x4000, /* BIT 14 */
   ATUC_CHAN_PERF_CURR_1DAY_UNCORR_BLK_FLAG    = 0x8000, /* BIT 15 */
   ATUC_CHAN_PERF_PREV_1DAY_MONI_SEC_FLAG      = 0x10000, /* BIT 16 */
   ATUC_CHAN_PERF_PREV_1DAY_RECV_BLK_FLAG      = 0x20000, /* BIT 17 */
   ATUC_CHAN_PERF_PREV_1DAY_TX_BLK_FLAG        = 0x40000, /* BIT 18 */
   ATUC_CHAN_PERF_PREV_1DAY_CORR_BLK_FLAG      = 0x80000, /* BIT 19 */
   ATUC_CHAN_PERF_PREV_1DAY_UNCORR_BLK_FLAG    = 0x100000 /* BIT 20 */
} adslAtucChannelPerfDataTableFlags_t;


/** Definitions for adslAturChannelPerfDataTable flags */
typedef enum
{
   ATUR_CHAN_RECV_BLK_FLAG                     = 0x01,     /* BIT 0th position */
   ATUR_CHAN_TX_BLK_FLAG                       = 0x02,     /* BIT 1 */
   ATUR_CHAN_CORR_BLK_FLAG                     = 0x04,     /* BIT 2 */
   ATUR_CHAN_UNCORR_BLK_FLAG                   = 0x08,     /* BIT 3 */
   ATUR_CHAN_PERF_VALID_INTVL_FLAG             = 0x10,     /* BIT 4 */
   ATUR_CHAN_PERF_INVALID_INTVL_FLAG           = 0x20,     /* BIT 5 */
   ATUR_CHAN_PERF_CURR_15MIN_TIME_ELAPSED_FLAG = 0x40,     /* BIT 6 */
   ATUR_CHAN_PERF_CURR_15MIN_RECV_BLK_FLAG     = 0x80,     /* BIT 7 */
   ATUR_CHAN_PERF_CURR_15MIN_TX_BLK_FLAG       = 0x100,    /* BIT 8 */
   ATUR_CHAN_PERF_CURR_15MIN_CORR_BLK_FLAG     = 0x200,    /* BIT 9 */
   ATUR_CHAN_PERF_CURR_15MIN_UNCORR_BLK_FLAG   = 0x400,    /* BIT 10 */
   ATUR_CHAN_PERF_CURR_1DAY_TIME_ELAPSED_FLAG  = 0x800,    /* BIT 11 */
   ATUR_CHAN_PERF_CURR_1DAY_RECV_BLK_FLAG      = 0x1000,   /* BIT 12 */
   ATUR_CHAN_PERF_CURR_1DAY_TX_BLK_FLAG        = 0x2000,   /* BIT 13 */
   ATUR_CHAN_PERF_CURR_1DAY_CORR_BLK_FLAG      = 0x4000,   /* BIT 14 */
   ATUR_CHAN_PERF_CURR_1DAY_UNCORR_BLK_FLAG    = 0x8000,   /* BIT 15 */
   ATUR_CHAN_PERF_PREV_1DAY_MONI_SEC_FLAG      = 0x10000,  /* BIT 16 */
   ATUR_CHAN_PERF_PREV_1DAY_RECV_BLK_FLAG      = 0x20000,  /* BIT 17 */
   ATUR_CHAN_PERF_PREV_1DAY_TRANS_BLK_FLAG     = 0x40000,  /* BIT 18 */
   ATUR_CHAN_PERF_PREV_1DAY_CORR_BLK_FLAG      = 0x80000,  /* BIT 19 */
   ATUR_CHAN_PERF_PREV_1DAY_UNCORR_BLK_FLAG    = 0x100000  /* BIT 20 */
} adslAturChannelPerfDataTableFlags_t;

/** Definitions for adslAtucChanIntvlInfo flags */
typedef enum
{
   ATUC_CHAN_INTVL_NUM_FLAG          = 0x1,   /* BIT 0th position */
   ATUC_CHAN_INTVL_RECV_BLK_FLAG     = 0x2,   /* BIT 1 */
   ATUC_CHAN_INTVL_TX_BLK_FLAG       = 0x4,   /* BIT 2 */
   ATUC_CHAN_INTVL_CORR_BLK_FLAG     = 0x8,   /* BIT 3 */
   ATUC_CHAN_INTVL_UNCORR_BLK_FLAG   = 0x10,  /* BIT 4 */
   ATUC_CHAN_INTVL_VALID_DATA_FLAG   = 0x20   /* BIT 5 */
} adslAtucChanIntvlInfoFlags_t;

/** Definitions for adslAturChanIntvlInfo flags */
typedef enum
{
   ATUR_CHAN_INTVL_NUM_FLAG          = 0x1,   /* BIT 0th Position */
   ATUR_CHAN_INTVL_RECV_BLK_FLAG     = 0x2,   /* BIT 1 */
   ATUR_CHAN_INTVL_TX_BLK_FLAG       = 0x4,   /* BIT 2 */
   ATUR_CHAN_INTVL_CORR_BLK_FLAG     = 0x8,   /* BIT 3 */
   ATUR_CHAN_INTVL_UNCORR_BLK_FLAG   = 0x10,  /* BIT 4 */
   ATUR_CHAN_INTVL_VALID_DATA_FLAG   = 0x20   /* BIT 5 */
} adslAturChanIntvlInfoFlags_t;

/** Definitions for adslAturLineAlarmConfProfileTable flags */
typedef enum
{
   ATUC_THRESH_15MIN_LOFS_FLAG            = 0x01,   /* BIT 0th position */
   ATUC_THRESH_15MIN_LOSS_FLAG            = 0x02,   /* BIT 1 */
   ATUC_THRESH_15MIN_ESS_FLAG             = 0x04,   /* BIT 2 */
   ATUC_THRESH_FAST_RATEUP_FLAG           = 0x08,   /* BIT 3 */
   ATUC_THRESH_INTERLEAVE_RATEUP_FLAG     = 0x10,   /* BIT 4 */
   ATUC_THRESH_FAST_RATEDOWN_FLAG         = 0x20,   /* BIT 5 */
   ATUC_THRESH_INTERLEAVE_RATEDOWN_FLAG   = 0x40,   /* BIT 6 */
   ATUC_INIT_FAILURE_TRAP_ENABLE_FLAG     = 0x80,   /* BIT 7 */
   ATUR_THRESH_15MIN_LOFS_FLAG            = 0x100,    /* BIT 8 */
   ATUR_THRESH_15MIN_LOSS_FLAG            = 0x200,    /* BIT 9 */
   ATUR_THRESH_15MIN_LPRS_FLAG            = 0x400,    /* BIT 10 */
   ATUR_THRESH_15MIN_ESS_FLAG             = 0x800,    /* BIT 11 */
   ATUR_THRESH_FAST_RATEUP_FLAG           = 0x1000,   /* BIT 12 */
   ATUR_THRESH_INTERLEAVE_RATEUP_FLAG     = 0x2000,   /* BIT 13 */
   ATUR_THRESH_FAST_RATEDOWN_FLAG         = 0x4000,   /* BIT 14 */
   ATUR_THRESH_INTERLEAVE_RATEDOWN_FLAG   = 0x8000,   /* BIT 15 */
   LINE_ALARM_CONF_PROFILE_ROWSTATUS_FLAG = 0x10000   /* BIT 16 */
} adslAturLineAlarmConfProfileTableFlags_t;


/** Definitions for adslAturTraps flags */
typedef enum
{
   ATUC_PERF_LOFS_THRESH_FLAG =  0x1,   /* BIT 0th position */
   ATUC_PERF_LOSS_THRESH_FLAG =  0x2,   /* BIT 1 */
   ATUC_PERF_ESS_THRESH_FLAG  =  0x4,   /* BIT 2 */
   ATUC_RATE_CHANGE_FLAG      =  0x8,   /* BIT 3 */
   ATUR_PERF_LOFS_THRESH_FLAG =  0x10,  /* BIT 4 */
   ATUR_PERF_LOSS_THRESH_FLAG =  0x20,  /* BIT 5 */
   ATUR_PERF_LPRS_THRESH_FLAG =  0x40,  /* BIT 6 */
   ATUR_PERF_ESS_THRESH_FLAG  =  0x80,  /* BIT 7 */
   ATUR_RATE_CHANGE_FLAG      =  0x100  /* BIT 8 */
} adslAturTrapsFlags_t;

/* RFC- 3440 FLAG DEFINITIONS */

#ifdef IFX_ADSL_MIB_RFC3440
/** Definitions for adslAtucLineExtTable flags */
typedef enum
{
   ATUC_LINE_TRANS_CAP_FLAG     = 0x1,      /* BIT 0th position */
   ATUC_LINE_TRANS_CONFIG_FLAG  = 0x2,      /* BIT 1 */
   ATUC_LINE_TRANS_ACTUAL_FLAG  = 0x4,      /* BIT 2 */
   LINE_GLITE_POWER_STATE_FLAG  = 0x8       /* BIT 3 */
} adslAtucLineExtTableFlags_t;

/** Definitions for adslAtucPerfDataExtTable flags */
typedef enum
{
   ATUC_PERF_STAT_FASTR_FLAG              = 0x1,    /* BIT 0th position */
   ATUC_PERF_STAT_FAILED_FASTR_FLAG       = 0x2,    /* BIT 1 */
   ATUC_PERF_STAT_SESL_FLAG               = 0x4,    /* BIT 2 */
   ATUC_PERF_STAT_UASL_FLAG               = 0x8,    /* BIT 3 */
   ATUC_PERF_CURR_15MIN_FASTR_FLAG        = 0x10,   /* BIT 4 */
   ATUC_PERF_CURR_15MIN_FAILED_FASTR_FLAG = 0x20,   /* BIT 5 */
   ATUC_PERF_CURR_15MIN_SESL_FLAG         = 0x40,   /* BIT 6 */
   ATUC_PERF_CURR_15MIN_UASL_FLAG         = 0x80,   /* BIT 7 */
   ATUC_PERF_CURR_1DAY_FASTR_FLAG         = 0x100,  /* BIT 8 */
   ATUC_PERF_CURR_1DAY_FAILED_FASTR_FLAG  = 0x200,  /* BIT 9 */
   ATUC_PERF_CURR_1DAY_SESL_FLAG          = 0x400,  /* BIT 10 */
   ATUC_PERF_CURR_1DAY_UASL_FLAG          = 0x800,  /* BIT 11 */
   ATUC_PERF_PREV_1DAY_FASTR_FLAG         = 0x1000, /* BIT 12 */
   ATUC_PERF_PREV_1DAY_FAILED_FASTR_FLAG  = 0x2000, /* BIT 13 */
   ATUC_PERF_PREV_1DAY_SESL_FLAG          = 0x4000, /* BIT 14 */
   ATUC_PERF_PREV_1DAY_UASL_FLAG          = 0x8000  /* BIT 15 */
} adslAtucPerfDataExtTableFlags_t;

/** Definitions for adslAturPerfDataExtTable flags */
typedef enum
{
   ATUR_PERF_STAT_SESL_FLAG       = 0x1,  /* BIT 0th position */
   ATUR_PERF_STAT_UASL_FLAG       = 0x2,  /* BIT 1 */
   ATUR_PERF_CURR_15MIN_SESL_FLAG = 0x4,  /* BIT 2 */
   ATUR_PERF_CURR_15MIN_UASL_FLAG = 0x8,  /* BIT 3 */
   ATUR_PERF_CURR_1DAY_SESL_FLAG  = 0x10, /* BIT 4 */
   ATUR_PERF_CURR_1DAY_UASL_FLAG  = 0x20, /* BIT 5 */
   ATUR_PERF_PREV_1DAY_SESL_FLAG  = 0x40, /* BIT 6 */
   ATUR_PERF_PREV_1DAY_UASL_FLAG  = 0x80  /* BIT 7 */
} adslAturPerfDataExtTableFlags_t;

/** Definitions for adslAtucIntvlExtInfo flags */
typedef enum
{
   ATUC_INTERVAL_FASTR_FLAG         = 0x1, /* Bit 0 */
   ATUC_INTERVAL_FAILED_FASTR_FLAG  = 0x2, /* Bit 1 */
   ATUC_INTERVAL_SESL_FLAG          = 0x4, /* Bit 2 */
   ATUC_INTERVAL_UASL_FLAG          = 0x8  /* Bit 3 */
} adslAtucIntvlExtInfoFlags_t;

/** Definitions for adslAturIntvExtTable flags */
typedef enum
{
   ATUR_INTERVAL_SESL_FLAG         = 0x1, /* BIT 0th position */
   ATUR_INTERVAL_UASL_FLAG         = 0x2  /* BIT 1 */
} adslAturIntvExtTableFlags_t;

/** Definitions for adslLineAlarmConfProfileExtTable flags */
typedef enum
{
   ATUC_THRESH_15MIN_FAILED_FASTR_FLAG = 0x1, /* BIT 0th position */
   ATUC_THRESH_15MIN_SESL_FLAG         = 0x2, /* BIT 1 */
   ATUC_THRESH_15MIN_UASL_FLAG         = 0x4, /* BIT 2 */
   ATUR_THRESH_15MIN_SESL_FLAG         = 0x8, /* BIT 3 */
   ATUR_THRESH_15MIN_UASL_FLAG         = 0x10 /* BIT 4 */
} adslLineAlarmConfProfileExtTableFlags_t;

/** Definitions for adslAturExtTraps flags */
typedef enum
{
   ATUC_15MIN_FAILED_FASTR_TRAP_FLAG = 0x1,  /* BIT 0th position */
   ATUC_15MIN_SESL_TRAP_FLAG         = 0x2,  /* BIT 1 */
   ATUC_15MIN_UASL_TRAP_FLAG         = 0x4,  /* BIT 2 */
   ATUR_15MIN_SESL_TRAP_FLAG         = 0x8,  /* BIT 3 */
   ATUR_15MIN_UASL_TRAP_FLAG         = 0x10  /* BIT 4 */
} adslAturExtTrapsFlags_t;

#endif /* IFX_ADSL_MIB_RFC3440 */

#endif /* DSL_DOC_GENERATION_EXCLUDE_MIB */

/** @} DRV_DSL_CPE_MIB */


/** \addtogroup DRV_DSL_CPE_WEB
 @{ */

#ifndef DSL_DOC_GENERATION_EXCLUDE_WEB

#define ATUR_SNRMGN_FLAG      0x8
#define ATUR_ATTN_FLAG        0x10

/** Definitions for adslLineStatusInfo flags */
typedef enum
{
   LINE_STAT_MODEM_STATUS_FLAG    = 0x1, /* BIT 0th position */
   LINE_STAT_MODE_SEL_FLAG        = 0x2, /* BIT 1 */
   LINE_STAT_TRELLCOD_ENABLE_FLAG = 0x4, /* BIT 2 */
   LINE_STAT_LATENCY_FLAG         = 0x8  /* BIT 3 */
} adslLineStatusInfoFlags_t;

/** Definitions for adslLineRateInfo flags */
typedef enum
{
   LINE_RATE_DATA_RATEDS_FLAG  = 0x1, /* BIT 0th position */
   LINE_RATE_DATA_RATEUS_FLAG  = 0x2, /* BIT 1 */
   LINE_RATE_ATTNDRDS_FLAG     = 0x4, /* BIT 2 */
   LINE_RATE_ATTNDRUS_FLAG     = 0x8  /* BIT 3 */
} adslLineRateInfoFlags_t;

/** Definitions for adslLineInfo flags */
typedef enum
{
   LINE_INFO_INTLV_DEPTHDS_FLAG = 0x1,   /* BIT 0th position */
   LINE_INFO_INTLV_DEPTHUS_FLAG = 0x2,   /* BIT 1 */
   LINE_INFO_LATNDS_FLAG        = 0x4,   /* BIT 2 */
   LINE_INFO_LATNUS_FLAG        = 0x8,   /* BIT 3 */
   LINE_INFO_SATNDS_FLAG        = 0x10,  /* BIT 4 */
   LINE_INFO_SATNUS_FLAG        = 0x20,  /* BIT 5 */
   LINE_INFO_SNRMNDS_FLAG       = 0x40,  /* BIT 6 */
   LINE_INFO_SNRMNUS_FLAG       = 0x80,  /* BIT 7 */
   LINE_INFO_ACATPDS_FLAG       = 0x100, /* BIT 8 */
   LINE_INFO_ACATPUS_FLAG       = 0x200  /* BIT 9 */
} adslLineInfoFlags_t;

/** Definitions for adslNearEndPerformanceStats flags */
typedef enum
{
   NEAREND_PERF_SUPERFRAME_FLAG = 0x1,    /* BIT 0th position */
   NEAREND_PERF_LOS_FLAG        = 0x2,    /* BIT 1 */
   NEAREND_PERF_LOF_FLAG        = 0x4,    /* BIT 2 */
   NEAREND_PERF_LPR_FLAG        = 0x8,    /* BIT 3 */
   NEAREND_PERF_NCD_FLAG        = 0x10,   /* BIT 4 */
   NEAREND_PERF_LCD_FLAG        = 0x20,   /* BIT 5 */
   NEAREND_PERF_CRC_FLAG        = 0x40,   /* BIT 6 */
   NEAREND_PERF_RSCORR_FLAG     = 0x80,   /* BIT 7 */
   NEAREND_PERF_FECS_FLAG       = 0x100,  /* BIT 8 */
   NEAREND_PERF_ES_FLAG         = 0x200,  /* BIT 9 */
   NEAREND_PERF_SES_FLAG        = 0x400,  /* BIT 10 */
   NEAREND_PERF_LOSS_FLAG       = 0x800,  /* BIT 11 */
   NEAREND_PERF_UAS_FLAG        = 0x1000, /* BIT 12 */
   NEAREND_PERF_HECERR_FLAG     = 0x2000  /* BIT 13 */
} adslNearEndPerfStatsFlags_t;

/** Definitions for adslFarEndPerformanceStats flags */
typedef enum
{
   FAREND_PERF_LOS_FLAG    = 0x1,   /* BIT 0th position */
   FAREND_PERF_LOF_FLAG    = 0x2,   /* BIT 1 */
   FAREND_PERF_LPR_FLAG    = 0x4,   /* BIT 2 */
   FAREND_PERF_NCD_FLAG    = 0x8,   /* BIT 3 */
   FAREND_PERF_LCD_FLAG    = 0x10,  /* BIT 4 */
   FAREND_PERF_CRC_FLAG    = 0x20,  /* BIT 5 */
   FAREND_PERF_RSCORR_FLAG = 0x40,  /* BIT 6 */
   FAREND_PERF_FECS_FLAG   = 0x80,  /* BIT 7 */
   FAREND_PERF_ES_FLAG     = 0x100, /* BIT 8 */
   FAREND_PERF_SES_FLAG    = 0x200, /* BIT 9 */
   FAREND_PERF_LOSS_FLAG   = 0x400, /* BIT 10 */
   FAREND_PERF_UAS_FLAG    = 0x800, /* BIT 11 */
   FAREND_PERF_HECERR_FLAG = 0x1000 /* BIT 12 */
} adslFarEndPerfStatsFlags_t;

/*  603221:tc.chen end */
/* TR-69 related additional parameters - defines */
/** Definitions for adslAturSubcarrierInfo flags */
typedef enum
{
   NEAREND_HLINSC = 0x1,
   NEAREND_HLINPS = 0x2,
   NEAREND_HLOGMT = 0x4,
   NEAREND_HLOGPS = 0x8,
   NEAREND_QLNMT  = 0x10,
   NEAREND_QLNPS  = 0x20,
   NEAREND_SNRMT  = 0x40,
   NEAREND_SNRPS  = 0x80,
   NEAREND_BITPS  = 0x100,
   NEAREND_GAINPS = 0x200
} adslAturSubcarrierInfoFlags_t;

/** Definitions for adslAtucSubcarrierInfo flags */
typedef enum
{
   FAREND_HLINSC = 0x1,
   /*
      As per the feedback from Knut on 21/08/2006, the cmv command of HLINSC
      should be INFO 70 2 */
   FAREND_HLINPS = 0x2,
   FAREND_HLOGMT = 0x4,
   FAREND_HLOGPS = 0x8,
   FAREND_QLNMT  = 0x10,
   FAREND_QLNPS  = 0x20,
   FAREND_SNRMT  = 0x40,
   FAREND_SNRPS  = 0x80,
   FAREND_BITPS  = 0x100,
   FAREND_GAINPS = 0x200
} adslAtucSubcarrierInfoFlags_t;

#endif /* DSL_DOC_GENERATION_EXCLUDE_WEB */

/** @} DRV_DSL_CPE_WEB */


/*  GET_ADSL_POWER_SPECTRAL_DENSITY */

/* Macro Definitions ? FLAG Setting & Testing */

#define SET_FLAG(flags, flag_val)   ((*flags) = ((*flags) | flag_val))
/*  -- This macro sets the flags with the flag_val. Here flags is passed as a pointer */

#define IS_FLAG_SET(flags, test_flag)  (((*flags) & (test_flag)) == (test_flag)? test_flag:0)
/*     -- This macro verifies whether test_flag has been set in flags. Here flags is passed as a pointer */


#define CLR_FLAG(flags, flag_bit)   ((*flags) = (*flags) & (~flag_bit))
/*  -- This macro resets the specified flag_bit in the flags. Here flags is passed as a pointer */


/* DATA STRUCTURES ORGANIZATION */

#define u32 unsigned int
#define u16 unsigned short
#define s16 short
#define u8 unsigned char

/** A type for handling boolean issues. */
typedef enum {
   /** false */
   DSL_FALSE = 0,
   /** true */
   DSL_TRUE = 1
} DSL_boolean_t;


/** \addtogroup DRV_DSL_CPE_MIB
 @{ */

#ifndef DSL_DOC_GENERATION_EXCLUDE_MIB

/*
   Here are the data structures used for accessing mib parameters. The ioctl
   call includes the third parameter as a void pointer. This parameter has to
   be type-casted in the driver code to the corresponding structure depending
   upon the command type. For Ex: consider the ioctl used to get the
   adslLineCode type, ioctl(fd,GET_ADSL_LINE_CODE,void *struct_adslLineTableEntry).
   In the driver code we check on the type of the command,
   i.e GET_ADSL_LINE_CODE and type-cast the void pointer to
   struct adslLineTableEntry type.
*/

/** Helper definition for MIB table entry value */
typedef u32 AdslPerfTimeElapsed;
/** Helper definition for MIB table entry value */
typedef u32 AdslPerfPrevDayCount;
/** Helper definition for MIB table entry value */
typedef u32 PerfCurrentCount;
/** Helper definition for MIB table entry value */
typedef u32 PerfIntervalCount;
/** Helper definition for MIB table entry value */
typedef u32 AdslPerfCurrDayCount;


/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_LINE_CODE */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
      Specifies the ADSL coding type used on this line.
      - Numerical syntax: Integer (32 bit)
      - Base syntax: INTEGER */
   int adslLineCode;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslLineTableFlags_t */
   u8 flags;
} adslLineTableEntry;

#ifdef IFX_ADSL_MIB_RFC3440
/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUC_LINE_EXT and \ref SET_ADSL_ATUC_LINE_EXT */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
      The transmission modes, represented by a bitmask that the ATU-C is capable
      of supporting. The modes available are limited by the design of the
      equipment.
      - Numerical syntax: Octets
      - Base syntax: BITS
      - Reference: Section 7.3.2 ITU G.997.1 */
   u16 adslLineTransAtucCap;
   /**
      The transmission modes, represented by a bitmask, currently enabled by the
      ATU-C. The manager can only set those modes that are supported by the
      ATU-C. An ATU-C's supported modes are provided by AdslLineTransAtucCap.
      - Numerical syntax: Octets
      - Base syntax: BITS
      - Reference: Section 7.3.2 ITU G.997.1 */
   u16 adslLineTransAtucConfig;
   /**
      The actual transmission mode of the ATU-C. During ADSL line initialization,
      the ADSL Transceiver Unit - Remote terminal end (ATU-R) will determine the
      mode used for the link. This value will be limited a single transmission
      mode that is a subset of those modes enabled by the ATU-C and denoted by
      adslLineTransAtucConfig. After an initialization has occurred, its mode is
      saved as the 'Current' mode and is persistence should the link go down.
      This object returns 0 (i.e. BITS with no mode bit set) if the mode is not
      known.
      - Numerical syntax: Octets
      - Base syntax: BITS
      - Reference: Section 7.3.2 ITU G.997.1 */
   u16 adslLineTransAtucActual;
   /**
      The value of this object specifies the power state of this interface.
      L0 is power on, L1 is power on but reduced and L3 is power off. Power
      state cannot be configured by an operator but it can be viewed via the
      ifOperStatus object for the managed ADSL interface. The value of the
      object ifOperStatus is set to down(2) if the ADSL interface is in power
      state L3 and is set to up(1) if the ADSL line interface is in power state
      L0 or L1. If the object adslLineTransAtucActual is set to a G.992.2
      (G.Lite)-type transmission mode, the value of this object will be one of
      the valid power states: L0(2), L1(3), or L3(4).  Otherwise, its value
      will be none(1).
      - Numerical syntax: Integer (32 bit)
      - Base syntax: INTEGER
      - Value list:
         - 1: none(1)
         - 2: l0(2)
         - 3: l1(3)
         - 4: l3(4) */
   int adslLineGlitePowerState;
   /**
   Defines a bitmask to specify which parameters shall be accessed according
   to \ref adslAtucLineExtTableFlags_t */
   u32 flags;
} adslLineExtTableEntry;
#endif /* IFX_ADSL_MIB_RFC3440 */

#ifndef u_char
#define u_char u8
#endif

/**
   Structure used within \ref adslAtucPhysEntry and \ref adslAturPhysEntry to
    store the vendor id information */
typedef struct
{
   u16   country_code;
   u_char   provider_id[4];  /* Ascii characters */
   u_char   revision_info[2];
} adslVendorId_t;

/**
   Structure (union) definition for storing vedor id in CMV and parameter
   format */
typedef union
{
   char vendor_id[16];
   adslVendorId_t vendor_info;
} vendor_id_t;

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUC_PHY */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
      The vendor specific string that identifies the vendor equipment.
      - Numerical syntax: Octets
      - Base syntax: OCTET STRING
      - Size list: 1: 0..32 */
   char serial_no[32];
   /**
      The vendor ID code is a copy of the binary vendor identification field
      defined by the PHY[10] and expressed as readable characters.
      - Numerical syntax: Octets
      - Base syntax: OCTET STRING
      - Size list: 1: 0..16
      - Reference: ANSI T1.413[10] */
   vendor_id_t vendor_id;
   /**
      The vendor specific version number sent by this ATU as part of the
      initialization messages. It is a copy of the binary version number field
      defined by the PHY[10] and expressed as readable characters.
      - Numerical syntax: Octets
      - Base syntax: OCTET STRING
      - Size list: 1: 0..16
      - Reference: ANSI T1.413[10] */
   char version_no[16];
   /**
      Indicates current state of the ATUC line. This is a bit-map of possible
      conditions. The various bit positions are:
      - 0: noDefect            There no defects on the line
      - 1: lossOfFraming       ATUC failure due to not receiving valid frame.
      - 2: lossOfSignal        ATUC failure due to not receiving signal.
      - 3: lossOfPower         ATUC failure due to loss of power.
                               \note the Agent may still function.
      - 4: lossOfSignalQuality Loss of Signal Quality is declared when the Noise
                               Margin falls below the Minimum Noise Margin, or
                               the bit-error-rate exceeds 10^-7.
      - 5: lossOfLink          ATUC failure due to inability to link with ATUR.
      - 6: dataInitFailure     ATUC failure during initialization due to bit
                               errors corrupting startup exchange data.
      - 7: configInitFailure   ATUC failure during initialization due to peer
                               ATU not able to support
                               requested configuration
      - 8: protocolInitFailure ATUC failure during initialization due to
                               incompatible protocol used by the peer ATU.
      - 9: noPeerAtuPresent    ATUC failure during initialization due to no
                               activation sequence detected from peer ATU.
   This is intended to supplement ifOperStatus. */
   u32 status;
   /**
      Measured total output power transmitted by this ATU. This is the
      measurement that was reported during the last activation sequence.
      - Numerical syntax: Integer (32 bit)
      - Base syntax: INTEGER
      - Size list: 1: -310..310
      - Units: tenth dBm */
   int outputPwr;
   /**
      Indicates the maximum currently attainable data rate by the ATU. This
      value will be equal or greater than the current line rate.
      - Numerical syntax: Gauge (32 bit)
      - Base syntax: Gauge32
      - Units: bps */
   u32 attainableRate;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAtucPhysTableFlags_t */
   u8 flags;
} adslAtucPhysEntry;

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUR_PHY */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
      The vendor specific string that identifies the vendor equipment.
      - Numerical syntax: Octets
      - Base syntax: OCTET STRING
      - Size list: 1: 0..32 */
   char serial_no[32];
   /**
      The vendor ID code is a copy of the binary vendor identification field
      defined by the PHY[10] and expressed as readable characters.
      - Numerical syntax: Octets
      - Base syntax: OCTET STRING
      - Size list: 1: 0..16
      - Reference: ANSI T1.413[10] */
   vendor_id_t vendor_id;
   /**
      The vendor specific version number sent by this ATU as part of the
      initialization messages. It is a copy of the binary version number field
      defined by the PHY[10] and expressed as readable characters.
      - Numerical syntax: Octets
      - Base syntax: OCTET STRING
      - Size list: 1: 0..16
      - Reference: ANSI T1.413[10] */
   char version_no[16];
   /**
      Noise Margin as seen by this ATU with respect to its received signal in
      tenth dB.
      - Numerical syntax: Integer (32 bit)
      - Base syntax: INTEGER
      - Size list: 1: -640..640
      - Units: tenth dB */
   int SnrMgn;
   /**
      Measured difference in the total power transmitted by the peer ATU and
      the total power received by this ATU.
      - Numerical syntax: Gauge (32 bit)
      - Base syntax: Gauge32
      - Size list: 1: 0..630
      - Units: tenth dB */
   u32 Attn;
   /**
      Indicates current state of the ATUR line.  This is a bit-map of possible
      conditions. Due to the isolation of the ATUR when line problems occur,
      many state conditions like loss of power, loss of quality signal, and
      initialization errors,  can not be determined. While trouble shooting
      ATUR, also use object, adslAtucCurrStatus. The various bit positions are:
      - 0: noDefect             There no defects on the line
      - 1: lossOfFraming        ATUR failure due to not receiving valid frame
      - 2: lossOfSignal         ATUR failure due to not receiving signal
      - 3: lossOfPower          ATUR failure due to loss of power
      - 4: lossOfSignalQuality  Loss of Signal Quality is declared when the
                                Noise Margin falls below the Minimum Noise
                                Margin, or the bit-error-rate exceeds 10^-7.
      This is intended to supplement ifOperStatus. */
   u32 status;
   /**
      Measured total output power transmitted by this ATU. This is the
      measurement that was reported during the last activation sequence.
      - Numerical syntax: Integer (32 bit)
      - Base syntax: INTEGER
      - Size list: 1: -310..310
      - Units: tenth dBm */
   int outputPwr;
   /**
      Indicates the maximum currently attainable data rate by the ATU. This
      value will be equal or greater than the current line rate.
      - Numerical syntax: Gauge (32 bit)
      - Base syntax: Gauge32
      - Units: bps */
   u32 attainableRate;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAturPhysTableFlags_t */
   u8 flags;
} adslAturPhysEntry;

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUC_CHAN_INFO */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   Interleave Delay for this channel.
   Interleave delay applies only to the interleave channel and defines the
   mapping (relative spacing) between subsequent input bytes at the interleaver
   input and their placement in the bit stream at the interleaver output.
   Larger numbers provide greater separation between consecutive input bytes in
   the output bit stream allowing for improved impulse noise immunity at the
   expense of payload latency.
   In the case where the ifType is Fast(125), use
   noSuchObject.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Units: milli-seconds */
   u32 interleaveDelay;
   /**
   Actual transmit rate on this channel.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax:   Gauge32
   - Units: bps */
   u32 currTxRate;
   /**
   The rate at the time of the last adslAtucRateChangeTrap event. It is also set
   at initialization to prevent a trap being sent.
   Rate changes less than adslAtucThresh(*)RateDown or less than
   adslAtucThresh(*)RateUp will not cause a trap or cause this object to change.
   (*) == Fast or Interleave.
   See AdslLineAlarmConfProfileEntry.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Max access: read-only
   - Units: bps */
   u32 prevTxRate;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAtucChanInfoFlags_t */
   u8 flags;
} adslAtucChanInfo;

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUR_CHAN_INFO */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   Interleave Delay for this channel.
   Interleave delay applies only to the interleave channel and defines the
   mapping (relative spacing) between subsequent input bytes at the interleaver
   input and their placement in the bit stream at the interleaver output.
   Larger numbers provide greater separation between consecutive input bytes in
   the output bit stream allowing for improved impulse noise immunity at
   the expense of payload latency.
   In the case where the ifType is Fast(125), use noSuchObject.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: Gauge32
   - Max access: read-only
   - Units: milli-seconds */
   u32 interleaveDelay;
   /**
   Actual transmit rate on this channel.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Max access: read-only
   - Units: bps */
   u32 currTxRate;
   /**
   The rate at the time of the last adslAturRateChangeTrap event. It is also set
   at initialization to prevent a trap being sent. Rate changes less than
   adslAturThresh(*)RateDown or less than adslAturThresh(*)RateUp will not
   cause a trap or cause this object to change.
   (*) == Fast or Interleave.
   See AdslLineAlarmConfProfileEntry.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Units: bps */
   u32 prevTxRate;
   /**
   Indicates the length of the channel data-block on which the CRC operates.
   Refer to Line Code Specific MIBs, [11] and [12] for more information.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: Gauge32
   - Max access: read-only */
   u32 crcBlkLen;
   /**
   Defines a bitmask to specify which parameters shall be accessed according
   to \ref adslAturChanInfoFlags_t */
   u8 flags;
} adslAturChanInfo;

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUC_PERF_DATA */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   Count of the number of Loss of Framing failures since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAtucPerfLofs;
   /**
   Count of the number of Loss of Signal failures since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAtucPerfLoss;
   /**
   Count of the number of Errored Seconds since agent reset.
   The errored second parameter is a count of one-second intervals containing
   one or more crc anomalies, or one or more los or sef defects
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAtucPerfESs;
   /**
   Count of the line initialization attempts since agent reset. Includes both
   successful and failed attempts.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAtucPerfInits;
   /**
   The number of previous 15-minute intervals in the interval table for which
   data was collected. Given that [n] is the maximum # of intervals supported.
   The value will be [n] unless the measurement was (re-)started within the
   last ([n]*15) minutes, in which case the value will be the number of
   complete 15 minute intervals for which the agent has at least some data.
   In certain cases (e.g., in the case where the agent is a proxy) it is
   possible that some intervals are unavailable. In this case, this interval is
   the maximum interval number for which data is available.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..96 */
   int adslAtucPerfValidIntervals;
   /**
   The number of intervals in the range from 0 to the value of
   adslAtucPerfValidIntervals for which no data is available. This object
   will typically be zero except in cases where the data for some intervals are
   not available (e.g., in proxy situations).
   - Numerical syntax: Integer (32 bit)
   - Base syntax:   INTEGER
   - Composed syntax:  INTEGER
   - Max access: read-only
   - Size list:  1: 0..96 */
   int adslAtucPerfInvalidIntervals;
   /**
   Total elapsed seconds in this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfTimeElapsed
   - Max access: read-only
   - Size list: 1: 0..899
   - Units: seconds */
   AdslPerfTimeElapsed adslAtucPerfCurr15MinTimeElapsed;
   /**
   Count of seconds in the current 15 minute interval when there was Loss of
   Framing.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds */
   PerfCurrentCount adslAtucPerfCurr15MinLofs;
   /**
   Count of seconds in the current 15 minute interval when there was Loss of
   Signal.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds */
   PerfCurrentCount adslAtucPerfCurr15MinLoss;
   /**
   Count of Errored Seconds in the current 15 minute interval.  The errored
   second parameter is a count of one-second intervals containing one or more
   crc anomalies, or one or more los or sef defects.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds */
   PerfCurrentCount adslAtucPerfCurr15MinESs;
   /**
   Count of the line initialization attempts in the current 15 minute interval.
   Includes both successful and failed attempts.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only */
   PerfCurrentCount adslAtucPerfCurr15MinInits;
   /**
   Number of seconds that have elapsed since the beginning of the current 1-day
   interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfTimeElapsed
   - Max access: read-only
   - Size list: 1: 0..86399
   - Units: seconds */
   AdslPerfTimeElapsed  adslAtucPerfCurr1DayTimeElapsed;
   /**
   Count of the number of seconds when there was Loss of Framing during the
   current day as measured by adslAtucPerfCurr1DayTimeElapsed.
   Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Status: current
   - Max access: read-only
   - Units: seconds */
   AdslPerfCurrDayCount adslAtucPerfCurr1DayLofs;
   /**
   Count of the number of seconds when there was Loss of Signal during the
   current day as measured by adslAtucPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfCurrDayCount adslAtucPerfCurr1DayLoss;
   /**
   Count of Errored Seconds during the current day as measured by
   adslAtucPerfCurr1DayTimeElapsed. The errored second parameter is a count of
   one-second intervals containing one or more crc anomalies, or one or more
   los or sef defects.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfCurrDayCount adslAtucPerfCurr1DayESs;
   /**
   Count of the line initialization attempts in the day as measured by
   adslAtucPerfCurr1DayTimeElapsed. Includes both successful and failed attempts.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only */
   AdslPerfCurrDayCount adslAtucPerfCurr1DayInits;
   /**
   The amount of time in the previous 1-day interval over which the performance
   monitoring information is actually counted. This value will be the same as
   the interval duration except in a situation where performance monitoring
   data could not be collected for any reason.
   Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..86400
   - Units: seconds */
   int adslAtucPerfPrev1DayMoniSecs;
   /**
   Count of seconds in the interval when there was Loss of Framing within the
   most recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfPrevDayCount adslAtucPerfPrev1DayLofs;
   /**
   Count of seconds in the interval when there was Loss of Signal within the
   most recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfPrevDayCount adslAtucPerfPrev1DayLoss;
   /**
   Count of Errored Seconds within the most recent previous 1-day period. The
   errored second parameter is a count of one-second intervals containing one
   or more crc anomalies, or one or more los or sef defects.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfPrevDayCount adslAtucPerfPrev1DayESs;
   /**
   Count of the line initialization attempts in the most
   recent previous 1-day period. Includes both successful
   and failed attempts.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only */
   AdslPerfPrevDayCount adslAtucPerfPrev1DayInits;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAtucPerfDataTableFlags_t */
   u32         flags;
} atucPerfDataEntry;

#ifdef IFX_ADSL_MIB_RFC3440
/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUC_PERF_DATA_EXT */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   The value of this object reports the count of the number of fast line bs
   since last agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: line retrains
   - Reference: ITU G.997.1 Section 7.4.15.1 */
   u32 adslAtucPerfStatFastR;
   /**
   The value of this object reports the count of the number of failed fast line
   retrains since last agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: line retrains
   - Reference: ITU G.997.1 Section 7.4.15.2 */
   u32 adslAtucPerfStatFailedFastR;
   /**
   The value of this object reports the count of the number of severely errored
   seconds-line since last agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.1.7 */
   u32 adslAtucPerfStatSesL;
   /**
   The value of this object reports the count of the number of unavailable
   seconds-line since last agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.1.9 */
   u32 adslAtucPerfStatUasL;
   /**
   For the current 15-minute interval, adslAtucPerfCurr15MinFastR reports the
   current number of seconds during which there have been fast retrains.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.4.15.1 */
   u32 adslAtucPerfCurr15MinFastR;
   /**
   For the current 15-minute interval, adslAtucPerfCurr15MinFailedFastR reports
   the current number of seconds during which there have been failed fast
   retrains.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax:   Gauge32
   - Composed syntax:  PerfCurrentCount
   - Max access: read-only
   - Units:   seconds
   - Reference:  ITU G.997.1 Section 7.4.15.2 */
   u32 adslAtucPerfCurr15MinFailedFastR;
   /**
   For the current 15-minute interval, adslAtucPerfCurr15MinSesL reports the
   current number of seconds during which there have been severely errored
   seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax:   Gauge32
   - Composed syntax:  PerfCurrentCount
   - Max access: read-only
   - Units:   seconds
   - Reference:  ITU G.997.1 Section 7.2.1.1.7 */
   u32 adslAtucPerfCurr15MinSesL;
   /**
   For the current 15-minute interval, adslAtucPerfCurr15MinUasL reports the
   current number of seconds during which there have been unavailable
   seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.1.9 */
   u32 adslAtucPerfCurr15MinUasL;
   /**
   For the current day as measured by adslAtucPerfCurr1DayTimeElapsed [RFC2662],
   adslAtucPerfCurr1DayFastR reports the number of seconds during which there
   have been fast retrains.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds
   - Reference:  ITU G.997.1 Section 7.4.15.1 */
   u32 adslAtucPerfCurr1DayFastR;
   /**
   For the current day as measured by adslAtucPerfCurr1DayTimeElapsed [RFC2662],
   adslAtucPerfCurr1DayFailedFastR reports the number of seconds during which
   there have been failed fast retrains.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.4.15.2 */
   u32 adslAtucPerfCurr1DayFailedFastR;
   /**
   For the current day as measured by adslAtucPerfCurr1DayTimeElapsed [RFC2662],
   adslAtucPerfCurr1DaySesL reports the number of seconds during which there
   have been severely errored seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.1.7 */
   u32 adslAtucPerfCurr1DaySesL;
   /**
   For the current day as measured by adslAtucPerfCurr1DayTimeElapsed [RFC2662],
   adslAtucPerfCurr1DayUasL reports the number of seconds during which there
   have been unavailable seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.1.9 */
   u32 adslAtucPerfCurr1DayUasL;
   /**
   For the previous day, adslAtucPerfPrev1DayFastR reports the number of seconds
   during which there were fast retrains.
   Numerical syntax: Gauge (32 bit)
   Base syntax: Gauge32
   Composed syntax: AdslPerfPrevDayCount
   Max access: read-only
   Units: seconds
   Reference: ITU G.997.1 Section 7.4.15.1 */
   u32 adslAtucPerfPrev1DayFastR;
   /**
   For the previous day, adslAtucPerfPrev1DayFailedFastR reports the number
   of seconds during which there were failed fast retrains.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.4.15.2 */
   u32 adslAtucPerfPrev1DayFailedFastR;
   /**
   For the previous day, adslAtucPerfPrev1DaySesL reports the number of seconds
   during which there were severely errored seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.1.7 */
   u32 adslAtucPerfPrev1DaySesL;
   /**
   For the previous day, adslAtucPerfPrev1DayUasL reports the number of seconds
   during which there were unavailable seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.1.9 */
   u32 adslAtucPerfPrev1DayUasL;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAtucPerfDataExtTableFlags_t */
   u32 flags;
} atucPerfDataExtEntry;

#endif /* IFX_ADSL_MIB_RFC3440 */

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUR_PERF_DATA */
typedef struct
{
   /**
      Table index */
   int         ifIndex;
   /**
   Count of the number of Loss of Framing failures since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: seconds */
   u32 adslAturPerfLofs;
   /**
   Count of the number of Loss of Signal failures since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: seconds */
   u32 adslAturPerfLoss;
   /**
   Count of the number of Loss of Power failures since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: seconds */
   u32 adslAturPerfLprs;
   /**
   Count of the number of Errored Seconds since agent reset. The errored
   second parameter is a count of one-second intervals containing one or more
   crc anomalies, or one or more los or sef defects.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: seconds */
   u32 adslAturPerfESs;
   /**
   The number of previous 15-minute intervals in the interval table for which
   data was collected. Given that [n] is the maximum # of intervals supported.
   The value will be [n] unless the measurement was (re-)started within the
   last ([n]*15) minutes, in which case the value will be the number of
   complete 15 minute intervals for which the agent has at least some data.
   In certain cases (e.g., in the case where the agent is a proxy) it is
   possible that some intervals are unavailable.  In this case, this interval
   is the maximum interval number for which data is available.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..96 */
   int adslAturPerfValidIntervals;
   /**
   The number of intervals in the range from 0 to the value of
   adslAturPerfValidIntervals for which no data is available. This object
   will typically be zero except in cases where the data for some intervals are
   not available (e.g., in proxy situations).
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..96 */
   int adslAturPerfInvalidIntervals;
   /**
   Total elapsed seconds in this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfTimeElapsed
   - Max access: read-only
   - Size list: 1: 0..899
   - Units: seconds */
   AdslPerfTimeElapsed  adslAturPerfCurr15MinTimeElapsed;
   /**
   Count of seconds in the current 15 minute interval when there was Loss of
   Framing.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds */
   PerfCurrentCount adslAturPerfCurr15MinLofs;
   /**
   Count of seconds in the current 15 minute interval when there was Loss of
   Signal.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds */
   PerfCurrentCount adslAturPerfCurr15MinLoss;
   /**
   Count of seconds in the current 15 minute interval when there was Loss of
   Power.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds */
   PerfCurrentCount adslAturPerfCurr15MinLprs;
   /**
   Count of Errored Seconds in the current 15 minute interval. The errored
   second parameter is a count of one-second intervals containing one or more
   crc anomalies, or one or more los or sef defects.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds */
   PerfCurrentCount adslAturPerfCurr15MinESs;
   /**
   Number of seconds that have elapsed since the beginning of the current 1-day
   interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfTimeElapsed
   - Max access: read-only
   - Size list: 1: 0..86399
   - Units: seconds */
   AdslPerfTimeElapsed  adslAturPerfCurr1DayTimeElapsed;
   /**
   Count of the number of seconds when there was Loss of Framing during the
   current day as measured by adslAturPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfCurrDayCount adslAturPerfCurr1DayLofs;
   /**
   Count of the number of seconds when there was Loss of Signal during the
   current day as measured by adslAturPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfCurrDayCount adslAturPerfCurr1DayLoss;
   /**
   Count of the number of seconds when there was Loss of Power during the
   current day as measured by adslAturPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfCurrDayCount adslAturPerfCurr1DayLprs;
   /**
   Count of Errored Seconds during the current day as measured by
   adslAturPerfCurr1DayTimeElapsed. The errored second parameter is a count of
   one-second intervals containing one or more crc anomalies, or one or more
   los or sef defects.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfCurrDayCount adslAturPerfCurr1DayESs;
   /**
   The amount of time in the previous 1-day interval over which the performance
   monitoring information is actually counted. This value will be the same as
   the interval duration except in a situation where performance monitoring
   data could not be collected for any reason.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..86400
   - Units: seconds */
   int adslAturPerfPrev1DayMoniSecs;
   /**
   Count of seconds in the interval when there was Loss of Framing within the
   most recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfPrevDayCount adslAturPerfPrev1DayLofs;
   /**
   Count of seconds in the interval when there was Loss of Signal within the
   most recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfPrevDayCount adslAturPerfPrev1DayLoss;
   /**
   Count of seconds in the interval when there was Loss of Power within the most
   recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfPrevDayCount adslAturPerfPrev1DayLprs;
   /**
   Count of Errored Seconds within the most recent previous 1-day period. The
   errored second parameter is a count of one-second intervals containing one
   or more crc anomalies, or one or more los or sef defects.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds */
   AdslPerfPrevDayCount adslAturPerfPrev1DayESs;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAturPerfDataTableFlags_t */
   u32         flags;
} aturPerfDataEntry;

#ifdef IFX_ADSL_MIB_RFC3440
/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUR_PERF_DATA_EXT */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   The value of this object reports the count of severely errored second-line
   since the last agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.1.7 */
   u32 adslAturPerfStatSesL;
   /**
   The value of this object reports the count of unavailable seconds-line since
   the last agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.2.9 */
   u32 adslAturPerfStatUasL;
   /**
   For the current 15-minute interval, adslAturPerfCurr15MinSesL reports the
   current number of seconds during which there have been severely errored
   seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.2.7 */
   u32 adslAturPerfCurr15MinSesL;
   /**
   For the current 15-minute interval, adslAturPerfCurr15MinUasL reports the
   current number of seconds during which there have been available seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.2.9 */
   u32 adslAturPerfCurr15MinUasL;
   /**
   For the current day as measured by adslAturPerfCurr1DayTimeElapsed [RFC2662],
   adslAturPerfCurr1DaySesL reports the number of seconds during which there
   have been severely errored seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.2.7 */
   u32 adslAturPerfCurr1DaySesL;
   /**
   For the current day as measured by adslAturPerfCurr1DayTimeElapsed [RFC2662],
   adslAturPerfCurr1DayUasL reports the number of seconds during which there
   have been unavailable seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.2.9 */
   u32 adslAturPerfCurr1DayUasL;
   /**
   For the previous day, adslAturPerfPrev1DaySesL reports the number of seconds
   during which there were severely errored seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.2.7 */
   u32 adslAturPerfPrev1DaySesL;
   /**
   For the previous day, adslAturPerfPrev1DayUasL reports the number of seconds
   during which there were severely errored seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only
   - Units: seconds
   - Reference: ITU G.997.1 Section 7.2.1.2.9 */
   u32 adslAturPerfPrev1DayUasL;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAturPerfDataExtTableFlags_t */
   u32 flags;
} aturPerfDataExtEntry;
#endif /* IFX_ADSL_MIB_RFC3440 */

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUC_INTVL_INFO */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   Performance Data Interval number 1 is the the most recent previous interval;
   interval 96 is 24 hours ago.  Intervals 2..96 are
   optional.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: not-accessible
   - Size list: 1: 1..96 */
   int IntervalNumber;
   /**
   Count of seconds in the interval when there was Loss of Framing.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   PerfIntervalCount intervalLOF;
   /**
   Count of seconds in the interval when there was Loss of Signal.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   PerfIntervalCount intervalLOS;
   /**
   Count of Errored Seconds in the interval. The errored second parameter is a
   count of one-second intervals containing one or more crc anomalies, or one
   or more los or sef defects.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   PerfIntervalCount intervalES;
   /**
   Count of the line initialization attempts during the interval. Includes both
   successful and failed attempts.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax:   Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only */
   PerfIntervalCount intervalInits;
   /**
   This variable indicates if the data for this interval is valid.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: TruthValue
   - Max access: read-only */
   int intervalValidData;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAtucIntvlInfoFlags_t */
   u8 flags;
} adslAtucIntvlInfo;

#ifdef IFX_ADSL_MIB_RFC3440
/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUC_INTVL_EXT_INFO */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   */
   int IntervalNumber;
   /**
   For the current interval, adslAtucIntervalFastR reports the current number
   of seconds during which there have been fast retrains.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   u32 adslAtucIntervalFastR;
   /**
   For the each interval, adslAtucIntervalFailedFastR reports the number of
   seconds during which there have been failed fast retrains.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   u32 adslAtucIntervalFailedFastR;
   /**
   For the each interval, adslAtucIntervalSesL reports the number of seconds
   during which there have been severely errored seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   u32 adslAtucIntervalSesL;
   /**
   For the each interval, adslAtucIntervalUasL reports the number of seconds
   during which there have been unavailable seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   u32 adslAtucIntervalUasL;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAtucIntvlExtInfoFlags_t */
   u32 flags;
} adslAtucInvtlExtInfo;
#endif /* IFX_ADSL_MIB_RFC3440 */

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUR_INTVL_INFO */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   Performance Data Interval number 1 is the the most recent previous interval;
   interval 96 is 24 hours ago.  Intervals 2..96 are optional.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: not-accessible
   - Size list: 1: 1..96 */
   int IntervalNumber;
   /**
   Count of seconds in the interval when there was Loss of Framing.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   PerfIntervalCount intervalLOF;
   /**
   Count of seconds in the interval when there was Loss of Signal.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   PerfIntervalCount intervalLOS;
   /**
   Count of seconds in the interval when there was Loss of Power.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   PerfIntervalCount intervalLPR;
   /**
   Count of Errored Seconds in the interval. The errored second parameter is a
   count of one-second intervals containing one or more crc anomalies, or one
   or more los or sef defects.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   PerfIntervalCount intervalES;
   /**
   This variable indicates if the data for this interval is valid.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: TruthValue
   - Max access: read-only */
   int intervalValidData;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAturIntvlInfoFlags_t */
   u8 flags;
} adslAturIntvlInfo;

#ifdef IFX_ADSL_MIB_RFC3440
/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUR_INTVL_EXT_INFO */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   */
   int IntervalNumber;
   /**
   For the each interval, adslAturIntervalSesL reports the number of seconds
   during which there have been severely errored seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   u32 adslAturIntervalSesL;
   /**
   For the each interval, adslAturIntervalUasL reports the number of seconds
   during which there have been unavailable seconds-line.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only
   - Units: seconds */
   u32 adslAturIntervalUasL;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAturIntvExtTableFlags_t */
   u32 flags;
} adslAturInvtlExtInfo;
#endif /* IFX_ADSL_MIB_RFC3440 */

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUC_CHAN_PERF_DATA */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   Count of all encoded blocks received on this channel since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAtucChanReceivedBlks;
   /**
   Count of all encoded blocks transmitted on this channel since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAtucChanTransmittedBlks;
   /**
   Count of all blocks received with errors that were corrected since agent
   reset. These blocks are passed on as good data.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAtucChanCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAtucChanUncorrectBlks;
   /**
   The number of previous 15-minute intervals in the interval table for which
   data was collected. Given that [n] is the maximum # of intervals supported.
   The value will be [n] unless the measurement was (re-)started within the
   last ([n]*15) minutes, in which case the value will be the number of
   complete 15 minute intervals for which the agent has at least some data.
   In certain cases (e.g., in the case where the agent is a proxy) it is
   possible that some intervals are unavailable. In this case, this interval is
   the maximum interval number for which data is available.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax:INTEGER
   - Max access: read-only
   - Size list: 1: 0..96 */
   int adslAtucChanPerfValidIntervals;
   /**
   The number of intervals in the range from 0 to the value of
   adslAtucChanPerfValidIntervals for which no data is available. This object
   will typically be zero except in cases where the data for some intervals are
   not available (e.g., in proxy situations).
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..96 */
   int adslAtucChanPerfInvalidIntervals;
   /**
   Total elapsed seconds in this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfTimeElapsed
   - Max access: read-only
   - Size list: 1: 0..899
   - Units: seconds */
   AdslPerfTimeElapsed  adslAtucChanPerfCurr15MinTimeElapsed;
   /**
   Count of all encoded blocks received on this channel within the current
   15 minute interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax:PerfCurrentCount
   - Max access: read-only */
   PerfCurrentCount adslAtucChanPerfCurr15MinReceivedBlks;
   /**
   Count of all encoded blocks transmitted on this channel within the current
   15 minute interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only */
   PerfCurrentCount adslAtucChanPerfCurr15MinTransmittedBlks;
   /**
   Count of all blocks received with errors that were corrected on this channel
   within the current 15 minute interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only */
   PerfCurrentCount adslAtucChanPerfCurr15MinCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors on this channel within
   the current 15 minute interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only
   */
   PerfCurrentCount adslAtucChanPerfCurr15MinUncorrectBlks;
   /**
   Number of seconds that have elapsed since the beginning of the current 1-day
   interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfTimeElapsed
   - Max access: read-only
   - Size list: 1: 0..86399
   - Units: seconds */
   AdslPerfTimeElapsed  adslAtucChanPerfCurr1DayTimeElapsed;
   /**
   Count of all encoded blocks received on this channel during the current day
   as measured by adslAtucChanPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only */
   AdslPerfCurrDayCount adslAtucChanPerfCurr1DayReceivedBlks;
   /**
   Count of all encoded blocks transmitted on this channel during the current
   day as measured by adslAtucChanPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only */
   AdslPerfCurrDayCount adslAtucChanPerfCurr1DayTransmittedBlks;
   /**
   Count of all blocks received with errors that were corrected on this channel
   during the current day as measured by adslAtucChanPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only */
   AdslPerfCurrDayCount adslAtucChanPerfCurr1DayCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors on this channel during
   the current day as measured by adslAtucChanPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only */
   AdslPerfCurrDayCount adslAtucChanPerfCurr1DayUncorrectBlks;
   /**
   The amount of time in the previous 1-day interval over which the performance
   monitoring information is actually counted. This value will be the same as
   the interval duration except in a situation where performance monitoring data
   could not be collected for any reason.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..86400
   - Units: seconds */
   int adslAtucChanPerfPrev1DayMoniSecs;
   /**
   Count of all encoded blocks received on this channel within the most recent
   previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only */
   AdslPerfPrevDayCount adslAtucChanPerfPrev1DayReceivedBlks;
   /**
   Count of all encoded blocks transmitted on this channel within the most
   recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only */
   AdslPerfPrevDayCount adslAtucChanPerfPrev1DayTransmittedBlks;
   /**
   Count of all blocks received with errors that were corrected on this channel
   within the most recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only */
   AdslPerfPrevDayCount adslAtucChanPerfPrev1DayCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors on this channel within
   the most recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only */
   AdslPerfPrevDayCount adslAtucChanPerfPrev1DayUncorrectBlks;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAtucChannelPerfDataTableFlags_t */
   u32 flags;
} atucChannelPerfDataEntry;

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUR_CHAN_PERF_DATA */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   Count of all encoded blocks received on this channel since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAturChanReceivedBlks;
   /**
   Count of all encoded blocks transmitted on this channel since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAturChanTransmittedBlks;
   /**
   Count of all blocks received with errors that were corrected since agent
   reset. These blocks are passed on as good data.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAturChanCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors since agent reset.
   - Numerical syntax: Counter (32 bit)
   - Base syntax: Counter32
   - Composed syntax: Counter32
   - Max access: read-only */
   u32 adslAturChanUncorrectBlks;
   /**
   The number of previous 15-minute intervals in the interval table for which
   data was collected. Given that [n] is the maximum # of intervals supported.
   The value will be [n] unless the measurement was (re-)started within the last
   ([n]*15) minutes, in which case the value will be the number of complete 15
   minute intervals for which the agent has at least some data. In certain cases
   (e.g., in the case where the agent is a proxy) it is possible that some
   intervals are unavailable.  In this case, this interval is the maximum
   interval number for which data is available.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..96 */
   int adslAturChanPerfValidIntervals;
   /**
   The number of intervals in the range from 0 to the value of
   adslAturChanPerfValidIntervals for which no data is available. This object
   will typically be zero except in cases where the data for some intervals are
   not available (e.g., in proxy situations).
   - Numerical syntax:   Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..96 */
   int adslAturChanPerfInvalidIntervals;
   /**
   Total elapsed seconds in this interval. A full interval is 900 seconds.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfTimeElapsed
   - Max access: read-only
   - Size list: 1: 0..899
   - Units: seconds */
   AdslPerfTimeElapsed  adslAturChanPerfCurr15MinTimeElapsed;
   /**
   Count of all encoded blocks received on this channel within the current 15
   minute interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only */
   PerfCurrentCount adslAturChanPerfCurr15MinReceivedBlks;
   /**
   Count of all encoded blocks transmitted on this channel within the current
   15 minute interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   -Max access: read-only */
   PerfCurrentCount adslAturChanPerfCurr15MinTransmittedBlks;
   /**
   Count of all blocks received with errors that were corrected on this channel
   within the current 15 minute interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only */
   PerfCurrentCount adslAturChanPerfCurr15MinCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors on this channel within
   the current 15 minute interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfCurrentCount
   - Max access: read-only */
   PerfCurrentCount adslAturChanPerfCurr15MinUncorrectBlks;
   /**
   Number of seconds that have elapsed since the beginning of the current 1-day
   interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfTimeElapsed
   - Max access: read-only
   - Size list: 1: 0..86399
   - Units: seconds */
   AdslPerfTimeElapsed  adslAturChanPerfCurr1DayTimeElapsed;
   /**
   Count of all encoded blocks received on this channel during the current day
   as measured by adslAturChanPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only */
   AdslPerfCurrDayCount adslAturChanPerfCurr1DayReceivedBlks;
   /**
   Count of all encoded blocks transmitted on this channel during the current
   day as measured by adslAturChanPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only */
   AdslPerfCurrDayCount adslAturChanPerfCurr1DayTransmittedBlks;
   /**
   Count of all blocks received with errors that were corrected on this channel
   during the current day as measured by adslAturChanPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only */
   AdslPerfCurrDayCount adslAturChanPerfCurr1DayCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors on this channel during
   the current day as measured by adslAturChanPerfCurr1DayTimeElapsed.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfCurrDayCount
   - Max access: read-only */
   AdslPerfCurrDayCount adslAturChanPerfCurr1DayUncorrectBlks;
   /**
   The amount of time in the previous 1-day interval over which the performance
   monitoring information is actually counted. This value will be the same as
   the interval duration except in a situation where performance monitoring data
   could not be collected for any reason.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-only
   - Size list: 1: 0..86400
   - Units: seconds */
   int adslAturChanPerfPrev1DayMoniSecs;
   /**
   Count of all encoded blocks received on this channel within the most recent
   previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only */
   AdslPerfPrevDayCount adslAturChanPerfPrev1DayReceivedBlks;
   /**
   Count of all encoded blocks transmitted on this channel within the most
   recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only */
   AdslPerfPrevDayCount adslAturChanPerfPrev1DayTransmittedBlks;
   /**
   Count of all blocks received with errors that were corrected on this channel
   within the most recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only */
   AdslPerfPrevDayCount adslAturChanPerfPrev1DayCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors on this channel within
   the most recent previous 1-day period.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: AdslPerfPrevDayCount
   - Max access: read-only */
   AdslPerfPrevDayCount adslAturChanPerfPrev1DayUncorrectBlks;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAturChannelPerfDataTableFlags_t */
   u32 flags;
} aturChannelPerfDataEntry;

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUC_CHAN_INTVL_INFO */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   Performance Data Interval number 1 is the the most recent previous interval;
   interval 96 is 24 hours ago. Intervals 2..96 are optional.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: not-accessible
   - Size list: 1: 1..96 */
   int IntervalNumber;
   /**
   Count of all encoded blocks received on this channel during this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only */
   PerfIntervalCount chanIntervalRecvdBlks;
   /**
   Count of all encoded blocks transmitted on this channel during this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only */
   PerfIntervalCount chanIntervalXmitBlks;
   /**
   Count of all blocks received with errors that were corrected on this channel
   during this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only */
   PerfIntervalCount chanIntervalCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors on this channel during
   this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only */
   PerfIntervalCount chanIntervalUncorrectBlks;
   /**
   This variable indicates if the data for this interval is valid.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: TruthValue
   - Max access: read-only */
   int intervalValidData;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAtucChanIntvlInfoFlags_t */
   u8 flags;
} adslAtucChanIntvlInfo;

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ATUR_CHAN_INTVL_INFO */
typedef struct
{
   /**
      Table index */
   int ifIndex;
   /**
   Performance Data Interval number 1 is the the most recent previous interval;
   interval 96 is 24 hours ago. Intervals 2..96 are optional.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: not-accessible
   - Size list: 1: 1..96 */
   int IntervalNumber;
   /**
   Count of all encoded blocks received on this channel during this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only */
   PerfIntervalCount chanIntervalRecvdBlks;
   /**
   Count of all encoded blocks transmitted on this channel during this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only */
   PerfIntervalCount chanIntervalXmitBlks;
   /**
   Count of all blocks received with errors that were corrected on this channel
   during this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only */
   PerfIntervalCount chanIntervalCorrectedBlks;
   /**
   Count of all blocks received with uncorrectable errors on this channel during
   this interval.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Gauge32
   - Composed syntax: PerfIntervalCount
   - Max access: read-only */
   PerfIntervalCount chanIntervalUncorrectBlks;
   /**
   This variable indicates if the data for this interval is valid.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: TruthValue
   - Max access: read-only */
   int intervalValidData;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAturChanIntvlInfoFlags_t */
   u8 flags;
} adslAturChanIntvlInfo;

/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ALRM_CONF_PROF and \ref SET_ADSL_ALRM_CONF_PROF */
typedef struct
{
   /**
   This object is used by the line alarm configuration table in order to
   identify a row of this table.
   When `dynamic' profiles are implemented, the profile name is user specified.
   Also, the system will always provide a default profile whose name is `DEFVAL'.
   When `static' profiles are implemented, there is an one-to-one relationship
   between each line and its profile.  In which case, the profile name will
   need to algorithmicly represent the Line's ifIndex. Therefore, the profile's
   name is a decimalized string of the ifIndex that is fixed-length (i.e., 10)
   with leading zero(s).  For example, the profile name for ifIndex which
   equals '15' will be '0000000015'.
   - Numerical syntax: Octets
   - Base syntax: OCTET STRING
   - Composed syntax: SnmpAdminString
   - Max access: not-accessible
   - Size list: 1: 1..32 */
   unsigned char adslLineAlarmConfProfileName[32];
   /**
   The number of Loss of Frame Seconds encountered by an ADSL interface within
   any given 15 minutes performance data collection period, which causes the
   SNMP agent to send an adslAtucPerfLofsThreshTrap.
   One trap will be sent per interval per interface. A value of `0' will
   disable the trap.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-create
   - Size list: 1: 0..900
   - Units: seconds */
   int adslAtucThresh15MinLofs;
   /**
   The number of Loss of Signal Seconds encountered by an ADSL interface within
   any given 15 minutes performance data collection period, which causes the
   SNMP agent to send an adslAtucPerfLossThreshTrap.
   One trap will be sent per interval per interface. A value of `0' will disable
   the trap.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-create
   - Size list: 1: 0..900
   - Units: seconds */
   int adslAtucThresh15MinLoss;
   /**
   The number of Errored Seconds encountered by an ADSL interface within any
   given 15 minutes performance data collection period, which causes the SNMP
   agent to send an adslAtucPerfESsThreshTrap.
   One trap will be sent per interval per interface. A value of `0' will disable
   the trap.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-create
   - Size list: 1: 0..900
   - Units: seconds */
   int adslAtucThresh15MinESs;
   /**
   Applies to `Fast' channels only. Configured change in rate causing an
   adslAtucRateChangeTrap. A trap is produced when:
   ChanCurrTxRate >= ChanPrevTxRate plus the value of this object.
   A value of `0' will disable the trap.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Unsigned32
   - Composed syntax: Unsigned32
   - Max access: read-create
   - Units: bps */
   u32 adslAtucThreshFastRateUp;
   /**
   Applies to `Interleave' channels only. Configured change in rate causing an
   adslAtucRateChangeTrap.  A trap is produced when:
   ChanCurrTxRate >= ChanPrevTxRate plus the value of this object.
   A value of `0' will disable the trap.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Unsigned32
   - Composed syntax: Unsigned32
   - Max access: read-create
   - Units: bps */
   u32 adslAtucThreshInterleaveRateUp;
   /**
   Applies to `Fast' channels only. Configured change in rate causing an
   adslAtucRateChangeTrap.  A trap is produced when:
   ChanCurrTxRate <= ChanPrevTxRate minus the value of
   this object. A value of `0' will disable the trap.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Unsigned32
   - Composed syntax: Unsigned32
   - Max access: read-create
   - Units: bps */
   u32 adslAtucThreshFastRateDown;
   /**
   Applies to `Interleave' channels only. Configured change in rate causing an
   adslAtucRateChangeTrap.  A trap is produced when:
   ChanCurrTxRate <= ChanPrevTxRate minus the value of this object.
   A value of `0' will disable the trap.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Unsigned32
   - Composed syntax: Unsigned32
   - Max access: read-create
   - Units: bps */
   u32 adslAtucThreshInterleaveRateDown;
   /**
   Enables and disables the InitFailureTrap. This object is defaulted disable(2).
   - Numerical syntax: Integer (32 bit)
   - Base syntax:   INTEGER
   - Composed syntax:  INTEGER
   - Max access: read-create
   - Value list:
     - 1: enable(1)
     - 2: disable(2)
   - Default values: 1: disable (name) */
   int adslAtucInitFailureTrapEnable;
   /**
   The number of Loss of Frame Seconds encountered by an ADSL interface within
   any given 15 minutes performance data collection period, which causes the
   SNMP agent to send an adslAturPerfLofsThreshTrap.
   One trap will be sent per interval per interface. A value of `0' will disable
   the trap.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-create
   - Size list:  1: 0..900
   - Units: seconds */
   int adslAturThresh15MinLofs;
   /**
   The number of Loss of Signal Seconds encountered by an ADSL interface within
   any given 15 minutes performance data collection period, which causes the
   SNMP agent to send an adslAturPerfLossThreshTrap.
   One trap will be sent per interval per interface. A value of `0' will disable
   the trap.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-create
   - Size list: 1: 0..900
   - Units: seconds */
   int adslAturThresh15MinLoss;
   /**
   The number of Loss of Power Seconds encountered by an ADSL interface within
   any given 15 minutes performance data collection period, which causes the
   SNMP agent to send an adslAturPerfLprsThreshTrap.
   One trap will be sent per interval per interface. A value of `0' will disable
   the trap.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-create
   - Size list: 1: 0..900
   - Units: seconds */
   int adslAturThresh15MinLprs;
   /**
   The number of Errored Seconds encountered by an ADSL interface within any
   given 15 minutes performance data collection period, which causes the SNMP
   agent to send an adslAturPerfESsThreshTrap.
   One trap will be sent per interval per interface. A value of `0' will disable
   the trap.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: INTEGER
   - Max access: read-create
   - Size list: 1: 0..900
   - Units: seconds */
   int adslAturThresh15MinESs;
   /**
   Applies to `Fast' channels only. Configured change in rate causing an
   adslAturRateChangeTrap. A trap is produced when:
   ChanCurrTxRate >= ChanPrevTxRate plus the value of this object. A value of
   `0' will disable the trap.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Unsigned32
   - Composed syntax: Unsigned32
   - Max access: read-create
   - Units: bps */
   u32 adslAturThreshFastRateUp;
   /**
   Applies to `Interleave' channels only. configured change in rate causing an
   adslAturRateChangeTrap.  A trap is produced when:
   ChanCurrTxRate >= ChanPrevTxRate plus the value of this object. A value of
   `0' will disable the trap.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Unsigned32
   - Composed syntax: Unsigned32
   - Max access: read-create
   - Units: bps */
   u32 adslAturThreshInterleaveRateUp;
   /**
   Applies to `Fast' channels only. Configured change in rate causing an
   adslAturRateChangeTrap.  A trap is produced when:
   ChanCurrTxRate <= ChanPrevTxRate minus the value of this object. A value of
   `0' will disable the trap.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Unsigned32
   - Composed syntax: Unsigned32
   - Max access: read-create
   - Units: bps */
   u32 adslAturThreshFastRateDown;
   /**
   Applies to `Interleave' channels only. Configured change in rate causing an
   adslAturRateChangeTrap.  A trap is produced when:
   ChanCurrTxRate <= ChanPrevTxRate minus the value of this object. A value of
   `0' will disable the trap.
   - Numerical syntax: Gauge (32 bit)
   - Base syntax: Unsigned32
   - Composed syntax: Unsigned32
   - Max access: read-create
   - Units: bps */
   u32 adslAturThreshInterleaveRateDown;
   /**
   This object is used to create a new row or modify or delete an existing row
   in this table.
   A profile activated by setting this object to `active'.  When `active' is
   set, the system will validate the profile.
   Before a profile can be deleted or taken out of service, (by setting this
   object to `destroy' or `outOfService') it must be first unreferenced from
   all associated lines.
   If the implementator of this MIB has chosen not to implement
   `dynamic assignment' of profiles, this object's MIN-ACCESS is read-only and
   its value is always to be `active'.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: INTEGER
   - Composed syntax: RowStatus
   - Max access: read-create */
   int adslLineAlarmConfProfileRowStatus;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslAturLineAlarmConfProfileTableFlags_t */
   u32 flags;
} adslLineAlarmConfProfileEntry;

#ifdef IFX_ADSL_MIB_RFC3440
/**
   Structure that defines all parameters that are used in ioctl
   \ref GET_ADSL_ALRM_CONF_PROF_EXT and \ref SET_ADSL_ALRM_CONF_PROF_EXT */
typedef struct
{
   /**
   */
   u8 adslLineAlarmConfProfileExtName[32];
   /**
   The first time the value of the corresponding instance of
   adslAtucPerfCurr15MinFailedFastR reaches or exceeds this value within a given
   15-minute performance data collection period, an adslAtucFailedFastRThreshTrap
   notification will be generated. The value '0' will disable the notification.
   The default value of this object is '0'.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: Integer32
   - Composed syntax: Integer32
   - Max access: read-create
   - Size list: 1: 0..900
   - Default values: 1: 0 (int)")
   - Units: seconds */
   u32 adslAtucThreshold15MinFailedFastR;
   /**
   The first time the value of the corresponding instance of adslAtucPerf15MinSesL
   reaches or exceeds this value within a given 15-minute performance data
   collection period, an adslAtucSesLThreshTrap notification will be generated.
   The value '0' will disable the notification.  The default value of this
   object is '0'.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: Integer32
   - Composed syntax: Integer32
   - Max access: read-create
   - Size list: 1: 0..900
   - Default values: 1: 0 (int)")
   - Units: seconds */
   u32 adslAtucThreshold15MinSesL;
   /**
   The first time the value of the corresponding instance of adslAtucPerf15MinUasL
   reaches or exceeds this value within a given 15-minute performance data
   collection period, an adslAtucUasLThreshTrap notification will be generated.
   The value '0' will disable the notification.  The default value of this
   object is '0'.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: Integer32
   - Composed syntax: Integer32
   - Max access: read-create
   - Size list: 1: 0..900
   - Default values: 1: 0 (int)")
   - Units: seconds */
   u32 adslAtucThreshold15MinUasL;
   /**
   The first time the value of the corresponding instance of adslAturPerf15MinSesL
   reaches or exceeds this value within a given 15-minute performance data
   collection period, an adslAturSesLThreshTrap notification will be generated.
   The value '0' will disable the notification.  The default value of this
   object is '0'.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: Integer32
   - Composed syntax: Integer32
   - Max access: read-create
   - Size list: 1: 0..900
   - Default values: 1: 0 (int)")
   - Units: seconds */
   u32 adslAturThreshold15MinSesL;
   /**
   The first time the value of the corresponding instance of adslAturPerf15MinUasL
   reaches or exceeds this value within a given 15-minute performance data
   collection period, an adslAturUasLThreshTrap notification will be generated.
   The value '0' will disable the notification.  The default value of this
   object is '0'.
   - Numerical syntax: Integer (32 bit)
   - Base syntax: Integer32
   - Composed syntax: Integer32
   - Max access: read-create
   - Size list: 1: 0..900
   - Default values: 1: 0 (int)")
   - Units: seconds */
   u32 adslAturThreshold15MinUasL;
   /**
   Defines a bitmask to specify which parameters shall be accessed according
   to \ref adslLineAlarmConfProfileExtTableFlags_t */
   u32 flags;
} adslLineAlarmConfProfileExtEntry;
#endif /* IFX_ADSL_MIB_RFC3440 */
/* TRAPS */

#endif /* DSL_DOC_GENERATION_EXCLUDE_MIB */

/** @} DRV_DSL_CPE_MIB */


/** \addtogroup DRV_DSL_CPE_WEB
 @{ */

#ifndef DSL_DOC_GENERATION_EXCLUDE_WEB

/**
   Data Sturctures that includes ADSL Line Status information.
*/
typedef struct
{
   /**
   Returns the current modem line status.
   The following values are possible
   - 0:  RESET - Prior to link initiation.
   - 1:  READY - Sending G.hs or T.1413 tones and waiting for CO response.
   - 2:  FAIL  - Showtime not reached due to failure in handshake or initialization.
   - 3:  IDLE  - Reached after completion of Loop Diagnostics link.
   - 5:  GHS   -  CO Tones detected
   - 6:  FULL INITIALIZATION
   - 7:  SHOWTIME (both Rx and Tx direction)
   - 8:  RETRAIN (G.992.2 Fast Retrain)
   - 9:  DIAGNOSTIC MODE
   - 10: SHORT INITIALIZATION
   - 11: T.1413 ACTIVATION :CO Tones detected */
   int adslModemStatus;
   /**
   Returns the currently selected ADSL transmission mode
   The following values are possible
   - 0x00000001: ANSI T.1413
   - 0x00000004: G992_1_A
   - 0x00000008: G992_1_B
   - 0x00000100: G992_3_A
   - 0x00000200: G992_3_B
   - 0x00000400: G992_3_I
   - 0x00000800: G992_3_J
   - 0x00001100: G992_3_L
   - 0x00002000: G992_3_M
   - 0x00004000: G992_5_B
   - 0x00008000: G992_5_A
   - 0x00010000: G992_5_I
   - 0x00020000: G992_5_J
   - 0x00040000: G992_5_M */
   u32 adslModeSelected;
   /**
   Not implemented yet. */
   int adslAtucThresh15MinESs;
   /**
   Returns the status about Trellis-Coded Modulation as follows
   - 0: Disable (ADSL1), Disable for Rx only (ADSL2)
   - 1: Enable (Default) */
   int adslTrellisCodeEnable;
   /**
   Returns the status about Latency Path and Bearer Channel as folows
   - Bit 0:\n
       ADSL1: Interleave Path\n
       ADSL2/2+: LP0\n
       0 = Not active\n
       1 = Active\n
   - Bit 1:\n
       ADSL1: Fast Path\n
       ADSL2/2+: LP1\n
       0 = Not active\n
       1 = Active\
   - Bit 8:\n
       Bearer Channel 0 State\n
       0 = Not active\n
       1 = Active\n
   - Bit 9:\n
       Bearer Channel 1 State\n
       0 = Not active\n
       1 = Active */
   int adslLatency;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslLineStatusInfoFlags_t */
   u8 flags;
} adslLineStatusInfo;

/**
   Data Sturctures that includes ADSL Line Rate Status information.
*/
typedef struct
{
   /**
   Returns the current line rate for downstream direction in bps.
   The value is rounded down to a multiple of 1kbps. */
   u32  adslDataRateds;
   /**
   Returns the current line rate for upstream direction in bps.
   The value is rounded down to a multiple of 1kbps. */
   u32  adslDataRateus;
   /**
   Returns the Attainable Net Datarate for downsteam direction in bps.
   (Section 7.5.1.12 of G997.1) */
   u32  adslATTNDRds;
   /**
   Returns the Attainable Net Datarate for upstream direction in bps.
   (Section 7.5.1.13 of G997.1) */
   u32  adslATTNDRus;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslLineRateInfoFlags_t */
   u8   flags;
} adslLineRateInfo;

/**
   Data Sturctures that includes common line specific ADSL Status information.
*/
typedef struct
{
   /**
   Returns the current interleaving depth for downstream direction.
   The value is defined in number of Octets that are used for interleaving.
   According to the current data path configuration the values are returned
   as follows
   - ADSL1: interleaved, ADSL2/2+: LP0
   - ADSL1: fast, ADSL2/2+: LP1 */
   u32 adslInterleaveDepthds;
   /**
   Returns the current interleaving depth for upstream direction.
   The value is defined in number of Octets that are used for interleaving.
   According to the current data path configuration the values are returned
   as follows
   - ADSL1: interleaved, ADSL2/2+: LP0
   - ADSL1: fast, ADSL2/2+: LP1 */
   u32 adslInterleaveDepthus;
   /**
   Returns the current Line Attenuation for downstream direction.
   LATNds (unit: dB*10) (Section 7.5.1.6 of G997.1) */
   u32 adslLATNds;
   /**
   Returns the current Line Attenuation for upstream direction.
   LATNus (unit: dB*10) (Section 7.5.1.7 of G997.1) */
   u32 adslLATNus;
   /**
   Returns the current Signal Attenuation for downstream direction.
   SATNds (unit: dB*10) (Section 7.5.1.8 of G997.1) */
   u32 adslSATNds;
   /**
   Returns the current Signal Attenuation for upstream direction.
   SATNus (unit: dB*10) (Section 7.5.1.9 of G997.1) */
   u32 adslSATNus;
   /**
   Returns the current Signal to Noise Ratio Marging for downstream direction.
   SNRMds (unit: dB*10) (Section 7.5.1.10 of G997.1) */
   int adslSNRMds;
   /**
   Returns the current Signal to Noise Ratio Marging for upstream direction.
   SNRMus (unit: dB*10) (Section 7.5.1.11 of G997.1) */
   int adslSNRMus;
   /**
   Returns the Actual Agregate Transmit Power for downstream direction.
   ACTATPds (unit: dBm*10) (Section 7.5.1.16 of G997.1) */
   int adslACATPds;
   /**
   Returns the Actual Agregate Transmit Power for upstream direction.
   ACTATPus (unit: dBm*10) (Section 7.5.1.17 of G997.1) */
   int adslACATPus;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslLineInfoFlags_t */
   u32  flags;
} adslLineInfo;

/**
   Data Sturctures that includes performance statistics for NearEnd.
*/
typedef struct
{
   /**
   Returns the total number of superframes since agent reset. */
   u32 adslSuperFrames;
   /**
   Returns the current status for Near End Loss Of Signal failure.
   LOS Failure (Section 7.1.1.1 of G997.1).
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslneLOS;
   /**
   Returns the current status for Near End Loss Of Framing failure.
   LOF Failure (Section 7.1.1.2 of G997.1).
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslneLOF;
   /**
   Returns the current status for Near End Loss Of Power failure.
   LPR Failure (Section 7.1.1.3 of G997.1).
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslneLPR;
   /**
   Returns the current status for Near End No Cell Delination failure.
   NCD Failure LP0/LP1 (Section 7.1.4.1.1 of G997.1).
   There are two bits used for failure signalling as follows
   - Bit 0: LP0
   - Bit 1: LP1
   Each bit can have one of the following values
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslneNCD;
   /**
   Returns the current status for Near End Loss of Cell Delination failure.
   LCD Failure LP0  (Section 7.1.4.1.2 of G997.1).
   There are two bits used for failure signalling as follows
   - Bit 0: LP0
   - Bit 1: LP1
   Each bit can have one of the following values
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslneLCD;
   /**
   Returns the Near End CRC Count (Section 7.2.2.1.1 of G997.1).
   According to the current data path configuration the values are returned
   as follows
   - ADSL1: interleaved, ADSL2/2+: LP0
   - ADSL1: fast, ADSL2/2+: LP1 */
   u32 adslneCRC;
   /**
   Returns the Near End RS Correction Count (Section 7.2.2.1.2 of G997.1)
   According to the current data path configuration the values are returned
   as follows
   - ADSL1: interleaved, ADSL2/2+: LP0
   - ADSL1: fast, ADSL2/2+: LP1 */
   u32 adslneRSCorr;
   /**
   Returns the Near End Forward Error Correction Seconds (FECS-L)
   (Section 7.2.1.1.1 of G997.1) */
   u32 adslneFECS;
   /**
   Returns the Near End Errored Second (ES-L) (Section 7.2.1.1.2 of G997.1) */
   u32 adslneES;
   /**
   Returns the Near End Severely Errored Seconds (SES-L)
   (Section 7.2.1.1.3 of G997.1) */
   u32 adslneSES;
   /**
   Returns the Near End Loss of Signal Seconds (LOSS-L)
   (Section 7.2.1.1.4 of G997.1) */
   u32 adslneLOSS;
   /**
   Returns the Near End Unavailabe Seconds (UAS-L)
   (Section 7.2.1.1.4 of G997.1) */
   u32 adslneUAS;
   /**
   Returns the Near End Rx HEC Error Count (Section 7.2.4.1.1 of G997.1)
   According to the current data path configuration the values are returned
   for Bearer Channel 0 or 1. */
   u32 adslneHECErrors;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslNearEndPerfStatsFlags_t */
   u32  flags;
} adslNearEndPerfStats;

/**
   Data Sturctures that includes performance statistics for FarEnd.
*/
typedef struct
{
   /**
   Returns the current status for Far End Loss Of Signal failure.
   LOS Failure (Section 7.1.2.1 of G997.1).
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslfeLOS;
   /**
   Returns the current status for Far End Loss Of Framing failure.
   LOF Failure (Section 7.1.2.2 of G997.1).
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslfeLOF;
   /**
   Returns the current status for Far End Loss Of Power failure.
   LPR Failure (Section 7.1.2.3 of G997.1).
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslfeLPR;
   /**
   Returns the current status for Far End No Cell Delination failure.
   NCD Failure LP0/LP1 (Section 7.1.4.2.1 of G997.1).
   There are two bits used for failure signalling as follows
   - Bit 0: LP0
   - Bit 1: LP1
   Each bit can have one of the following values
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslfeNCD;
   /**
   Returns the current status for Far End Loss of Cell Delination failure.
   LCD Failure LP0  (Section 7.1.4.2.2 of G997.1).
   There are two bits used for failure signalling as follows
   - Bit 0: LP0
   - Bit 1: LP1
   Each bit can have one of the following values
   - 0: normal operation (no failure)
   - 1: failure */
   u32 adslfeLCD;
   /**
   Returns the Far End CRC Count (Section 7.2.2.2.1 of G997.1).
   According to the current data path configuration the values are returned
   as follows
   - ADSL1: interleaved, ADSL2/2+: LP0
   - ADSL1: fast, ADSL2/2+: LP1 */
   u32 adslfeCRC;
   /**
   Returns the Far End RS Correction Count (Section 7.2.2.2.2 of G997.1)
   According to the current data path configuration the values are returned
   as follows
   - ADSL1: interleaved, ADSL2/2+: LP0
   - ADSL1: fast, ADSL2/2+: LP1 */
   u32 adslfeRSCorr;
   /**
   Returns the Far End Forward Error Correction Seconds (FECS-L)
   (Section 7.2.1.2.1 of G997.1) */
   u32 adslfeFECS;
   /**
   Returns the Far End Errored Second (ES-L) (Section 7.2.1.2.2 of G997.1) */
   u32 adslfeES;
   /**
   Returns the Far End Severely Errored Seconds (SES-L)
   (Section 7.2.1.2.3 of G997.1) */
   u32 adslfeSES;
   /**
   Returns the Far End Loss of Signal Seconds (LOSS-L)
   (Section 7.2.1.2.4 of G997.1) */
   u32 adslfeLOSS;
   /**
   Returns the Far End Unavailabe Seconds (UAS-L)
   (Section 7.2.1.2.4 of G997.1) */
   u32 adslfeUAS;
   /**
   Returns the Far End HEC Error Count (Section 7.2.4.2.1 of G997.1).
   According to the current data path configuration the values are returned
   for Bearer Channel 0 or 1. */
   u32 adslfeHECErrors;
   /**
      Defines a bitmask to specify which parameters shall be accessed according
      to \ref adslFarEndPerfStatsFlags_t */
   u32  flags;
} adslFarEndPerfStats;

/**
   Data Sturctures that includes subcarrier related status information for
   ATU-R (CPE, in this case NE).

   The number of tones (and indices) dependends on the ADSL mode as follows
   - ADSL1 (G.992.1, G.992.2): 0..255
   - ADSL2+ (G.992.3, G.992.4, G.992.5): 0..511
   According to this definition there are unused values in case of ADSL1
   for indices 256..511.

   There is one special case for HLINps values which consists of 2 Octets
   per subcarrier. This results in unused indices 512..1023 for ADSL1.

   \todo Check description above and implementation!!!
*/
typedef struct
{
   /**
   Table index */
   int ifindex;
   /**
   Returns HLINSCds (Section 7.5.1.18.1 of G997.1).
   This value is available in Loop Diagnostics mode only.*/
   u16 HLINSCds;
   /**
   Returns the HLINpsds (Section 7.5.1.18.2 of G997.1)
   This value is available in Loop Diagnostics mode only.
   Even index = real part; Odd Index = imaginary part for each tone */
   u16 HLINpsds[1024];/*  */
   /**
   Returns the HLOGMTds (Section 7.5.1.18.3 of G997.1)
   This value is available in Loop Diagnostics mode only. */
   u16 HLOGMTds;
   /**
   Returns the HLOGpsds (Section 7.5.1.18.4 of G997.1).
   This value is available in Loop Diagnostics mode only. */
   u16 HLOGpsds[512];
   /**
   Returns the QLNMTds (Section 7.5.1.19.1 of G997.1)
   This value is available in Loop Diagnostics mode only. */
   u16 QLNMTds;
   /**
   Returns the Quiet Line Noise for downstream direction
   QLNpsds (Section 7.5.1.19.2 of G997.1)
   This value is available in Loop Diagnostics mode only. */
   u16 QLNpsds[512];
   /**
   Returns the SNRMTds (Section 7.5.1.20.1 of G997.1)
   This value is available in Loop Diagnostics mode only. */
   u16 SNRMTds;
   /**
   According to the curent line state the following values will be returned
   (Section 7.5.1.20.2 of G997.1)
   - Medley SNR in case of DELT is currently active
   - Showtime SNR values in all other cases */
   u16 SNRpsds[512];
   /**
   Returns the Bit Allocation Table for downstream direction
   (Section 7.5.1.21.1 of G997.1) */
   u16 BITpsds[512];
   /**
   Returns the Fine Gains for downstream direction
   (unit: dBm*10), (Section 7.5.1.21.3 of G997.1). */
   s16 GAINpsds[512];
   /**
   Defines a bitmask to specify which parameters shall be accessed according
   to \ref adslAturSubcarrierInfoFlags_t */
   u16   flags;
} adslATURSubcarrierInfo;

/**
   Data Sturctures that includes subcarrier related status information for
   ATU-C (CO, in this case FE).

   The number of tones (and indices) dependends on the ADSL Annex type
   as follows
   - Annex A: 0..31
   - Annex B: 0..63
   According to this definition there are unused values in case of ADSL1
   for indices 32..63

   There is one special case for HLINps values which consists of 2 Octets
   per subcarrier. This results in unused indices 64..127 for ADSL1.

   \todo Check description above and implementation!!!
*/
typedef struct
{
   /**
   Table index */
   int ifindex;
   /**
   Returns HLINSCus (Section 7.5.1.18.5 of G997.1).
   This value is available in Loop Diagnostics mode only.*/
   u16 HLINSCus;
   /**
   Returns the HLINpsus (Section 7.5.1.18.6 of G997.1)
   This value is available in Loop Diagnostics mode only.
   Even index = real part; Odd Index = imaginary part for each tone */
   u16 HLINpsus[128];
   /**
   Returns the HLOGMTus (Section 7.5.1.18.7 of G997.1)
   This value is available in Loop Diagnostics mode only. */
   u16 HLOGMTus;
   /**
   Returns the HLOGpsus (Section 7.5.1.18.8 of G997.1).
   This value is available in Loop Diagnostics mode only. */
   u16 HLOGpsus[64];
   /**
   Returns the QLNMTds (Section 7.5.1.19.3 of G997.1)
   This value is available in Loop Diagnostics mode only. */
   u16 QLNMTus;
   /**
   Returns the Quiet Line Noise for upstream direction
   QLNpsus (Section 7.5.1.19.4 of G997.1)
   This value is available in Loop Diagnostics mode only. */
   u16 QLNpsus[64];
   /**
   Returns the SNRMTus (Section 7.5.1.20.3 of G997.1)
   This value is available in Loop Diagnostics mode only. */
   u16 SNRMTus;
   /**
   Returns the SNRpsus (Section 7.5.1.20.4 of G997.1).
   This value is available in Loop Diagnostics mode only. */
   u16 SNRpsus[64];
   /**
   Returns the Bit Allocation Table for upstream direction
   (Section 7.5.1.21.2 of G997.1) */
   u16 BITpsus[64];
   /**
   Returns the Fine Gains for upstream direction
   (unit: dBm*10), (Section 7.5.1.21.4 of G997.1). */
   s16 GAINpsus[64];
   /**
   Defines a bitmask to specify which parameters shall be accessed according
   to \ref adslAtucSubcarrierInfoFlags_t */
   u16   flags;
} adslATUCSubcarrierInfo;

#ifndef u_int16
#define u_int16 u16
#endif

/**
   Data Sturctures that includes information about initialization statistics.
   \attention This functionality is not implemented yet!
*/
typedef struct
{
   u_int16 FullInitializationCount;
   u_int16 FailedFullInitializationCount;
   u_int16 LINIT_Errors;
   u_int16 Init_Timeouts;
} adslInitStats;

/**
   Data Sturctures that includes information about PSD values.
   \attention This functionality is not completely implemented yet!
*/
typedef struct
{
   /**
   Returns the Actual Power Spectrum Densitiy value for downstream direction */
   int ACTPSDds;
   /**
   Returns the Actual Power Spectrum Densitiy value for upstream direction */
   int ACTPSDus;
} adslPowerSpectralDensity;

#endif /* DSL_DOC_GENERATION_EXCLUDE_WEB */

/** @} DRV_DSL_CPE_WEB */


/**
   encapsulate all ioctl command arguments
   for example: ioctl(int fd, ADSL_ATUR_TRAPS, void  *uint16_flags)
*/
typedef union s_structpts
{
   adslLineTableEntry * adslLineTableEntry_pt;
   adslAtucPhysEntry * adslAtucPhysEntry_pt;
   adslAturPhysEntry * adslAturPhysEntry_pt;
   adslAtucChanInfo * adslAtucChanInfo_pt;
   adslAturChanInfo * adslAturChanInfo_pt;
   atucPerfDataEntry * atucPerfDataEntry_pt;
   aturPerfDataEntry * aturPerfDataEntry_pt;
   adslAtucIntvlInfo * adslAtucIntvlInfo_pt;
   adslAturIntvlInfo * adslAturIntvlInfo_pt;
   atucChannelPerfDataEntry * atucChannelPerfDataEntry_pt;
   aturChannelPerfDataEntry * aturChannelPerfDataEntry_pt;
   adslAtucChanIntvlInfo * adslAtucChanIntvlInfo_pt;
   adslAturChanIntvlInfo * adslAturChanIntvlInfo_pt;
   adslLineAlarmConfProfileEntry * adslLineAlarmConfProfileEntry_pt;

   /*  RFC 3440 */
   #ifdef IFX_ADSL_MIB_RFC3440
   adslLineExtTableEntry * adslLineExtTableEntry_pt;
   atucPerfDataExtEntry * atucPerfDataExtEntry_pt;
   adslAtucInvtlExtInfo * adslAtucInvtlExtInfo_pt;
   aturPerfDataExtEntry * aturPerfDataExtEntry_pt;
   adslAturInvtlExtInfo * adslAturInvtlExtInfo_pt;
   adslLineAlarmConfProfileExtEntry * adslLineAlarmConfProfileExtEntry_pt;
   #endif

   adslLineStatusInfo   * adslLineStatusInfo_pt;
   adslLineRateInfo  * adslLineRateInfo_pt;
   adslLineInfo      * adslLineInfo_pt;
   adslNearEndPerfStats * adslNearEndPerfStats_pt;
   adslFarEndPerfStats  * adslFarEndPerfStats_pt;
   adslATUCSubcarrierInfo  * adslATUCSubcarrierInfo_pt;
   adslATURSubcarrierInfo  * adslATURSubcarrierInfo_pt;
   adslPowerSpectralDensity * adslPowerSpectralDensity_pt;
} structpts;

#endif /* ] __IFX_ADSL_APP_IOCTL_H */
