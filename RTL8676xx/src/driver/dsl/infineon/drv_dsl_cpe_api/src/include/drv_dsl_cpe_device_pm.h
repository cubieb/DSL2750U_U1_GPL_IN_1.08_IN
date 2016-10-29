/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_DEVICE_PM_H
#define _DRV_DSL_CPE_DEVICE_PM_H

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef SWIG


/**
   This function initializes PM module device specific parameters

   \param pContext   Pointer to dsl library context structure, [I/O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_PM_DEV_Start(DSL_Context_t *pContext);

#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
/**
   This function gets current Channel Counters
   directly from the device

   \param pContext   Pointer to dsl library context structure, [I/O]
   \param nChannel   Channel for access, [I]
   \param nDirection Specifies direction, [O]
   \param pCounters  Pointer to Channel Counters, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_PM_DEV_ChannelCountersGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_XTUDir_t nDirection,
   DSL_PM_ChannelData_t *pCounters);

/**
   This function writes saved Channel Counters
   directly to the device

   \param pContext   Pointer to dsl library context structure, [I/O]
   \param nChannel   Channel for access, [I]
   \param nDirection Specifies direction, [O]
   \param pCounters  Pointer to Channel Counters, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_PM_DEV_ChannelCountersSet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_XTUDir_t nDirection,
   DSL_PM_ChannelData_t *pCounters);
#endif /** #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
/**
   This function gets current Data Path Counters
   directly from the device

   \param pContext   Pointer to dsl library context structure, [I/O]
   \param nChannel   Specifies channel to proceed, [I]
   \param nDirection Specifies direction, [O]
   \param pCounters  Pointer to Channel Counters, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_PM_DEV_DataPathCountersGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_XTUDir_t nDirection,
   DSL_PM_DataPathData_t *pCounters);

/**
   This function sets saved Data Path Counters
   directly to the device

   \param pContext   Pointer to dsl library context structure, [I/O]
   \param nChannel   Specifies channel to proceed, [I]
   \param nDirection Specifies direction, [O]
   \param pCounters  Pointer to Channel Counters, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_PM_DEV_DataPathCountersSet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_XTUDir_t nDirection,
   DSL_PM_DataPathData_t *pCounters);
#endif /** #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/

/**
   This function gets current Line Second Counters
   directly from the device

   \param pContext   Pointer to dsl library context structure, [I/O]
   \param nDirection Specifies direction, [O]
   \param pCounters  Pointer to Line Counters, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_PM_DEV_LineSecCountersGet(
   DSL_Context_t *pContext,
   DSL_XTUDir_t nDirection,
   DSL_PM_LineSecData_t *pCounters);

/**
   This function sets saved Line Second Counters
   directly to the device

   \param pContext   Pointer to dsl library context structure, [I/O]
   \param nDirection Specifies direction, [O]
   \param pCounters  Pointer to Line Counters, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_PM_DEV_LineSecCountersSet(
   DSL_Context_t *pContext,
   DSL_XTUDir_t nDirection,
   DSL_PM_LineSecData_t *pCounters);

#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
/**
   This function gets current Line Init Counters
   directly from the device

   \param pContext   Pointer to dsl library context structure, [I/O]
   \param pCounters  Pointer to Line init Counters, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_PM_DEV_LineInitCountersGet(
   DSL_Context_t *pContext,
   DSL_PM_LineInitData_t *pCounters);

/**
   This function restores Line Init Counters
   in the FW

   \param pContext   Pointer to dsl library context structure, [I/O]
   \param pCounters  Pointer to Line init Counters, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_PM_DEV_LineInitCountersSet(
   DSL_Context_t *pContext,
   DSL_PM_LineInitData_t *pCounters);
#endif /** #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/

#endif /* #ifndef SWIG*/

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_DEVICE_PM_H */
