/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_API_ERROR_H
#define _DRV_DSL_API_ERROR_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \addtogroup DRV_DSL_CPE_ERROR_CODES
 @{ */

/**
   Defines all possible error codes.
   Error codes are negative, warning codes are positive and success has the
   value 0.

   \note If there are more than one warnings during processing of one DSL API
         call the warning with the lowest value will be returned
*/
typedef enum
{
   /* *********************************************************************** */
   /* *** Error Codes Start here !                                        *** */
   /* *********************************************************************** */

   /* *********************************************************************** */
   /* *** Error Codes for configuration parameter consistency check       *** */
   /* *********************************************************************** */
   /** parameter out of range */
   DSL_ERR_PARAM_RANGE = -400,

   /* *********************************************************************** */
   /* *** Error Codes for EOC handler                                     *** */
   /* *********************************************************************** */
   /** transmission error */
   DSL_ERR_CEOC_TX_ERR = -300,

   /* *********************************************************************** */
   /* *** Error Codes for modem handling                                  *** */
   /* *********************************************************************** */
   /** Modem is not ready */
   DSL_ERR_MODEM_NOT_READY = -201,

   /* *********************************************************************** */
   /* *** Error Codes for Autoboot handler                                *** */
   /* *********************************************************************** */
   /** Autoboot thread is not started yet */
   DSL_ERR_AUTOBOOT_NOT_STARTED = -102,
   /** Autoboot thread is busy */
   DSL_ERR_AUTOBOOT_BUSY = -101,

   /* *********************************************************************** */
   /* *** Error Codes for IOCTL handler                                   *** */
   /* *********************************************************************** */
   /** An error occured during execution of a low level (MEI BSP) driver
       function */
   DSL_ERR_LOW_LEVEL_DRIVER_ACCESS = -31,
   /** Invalid parameter is passed */
   DSL_ERR_INVALID_PARAMETR = -30,

   /* *********************************************************************** */
   /* *** Common Error Codes                                              *** */
   /* *********************************************************************** */
   /** The DELT data is not available within DSL CPE API.
       Whether the diagnostic complete state was never reached (no successful
       completion of DELT measurement) or the DELT data was already deleted
       by using ioctl \ref DSL_FIO_G997_DELT_FREE_RESOURCES */
   DSL_ERR_DELT_DATA_NOT_AVAILABLE = -30,
   /** The event that should be processed are not active for the current
       instance */
   DSL_ERR_EVENTS_NOT_ACTIVE = -29,
   /** failed to get low level driver handle */
   DSL_ERR_L3_UNKNOWN_FAILURE = -28,
   /** L3 request timed out */
   DSL_ERR_L3_NOT_IN_L0 = -27,
   /** L3 request timed out */
   DSL_ERR_L3_TIMED_OUT = -26,
   /** L3 request rejected */
   DSL_ERR_L3_REJECTED = -25,
   /** failed to get low level driver handle */
   DSL_ERR_LOWLEVEL_DRIVER_HANDLE = -24,
   /** invalid direction */
   DSL_ERR_DIRECTION = -23,
   /** invalid channel number is passed */
   DSL_ERR_CHANNEL_RANGE = -22,
   /** function available only in the Showtime state */
   DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME = -21,
   /** Device has no data for application */
   DSL_ERR_DEVICE_NO_DATA = -20,
   /** Device is busy */
   DSL_ERR_DEVICE_BUSY = -19,
   /** The answer from the device does not return within the specifies timeout */
   DSL_ERR_FUNCTION_WAITING_TIMEOUT = -18,
   /** Last operation is supported if debug is enabled only error */
   DSL_ERR_ONLY_SUPPORTED_WITH_DEBUG_ENABLED = -17,
   /** Semaphore lock error */
   DSL_ERR_SEMAPHORE_GET = -16,
   /** Common error on send message and wait for answer handling */
   DSL_ERR_FUNCTION_WAITING = -15,
   /** Message exchange error */
   DSL_ERR_MSG_EXCHANGE = -14,
   /** Not implemented error */
   DSL_ERR_NOT_IMPLEMENTED = -13,
   /** Internal error */
   DSL_ERR_INTERNAL = -12,
   /** Feature or functionality not suppoted by device */
   DSL_ERR_NOT_SUPPORTED_BY_DEVICE = -11,
   /** Feature or functionality not supported by firmware */
   DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE = -10,
   /** Feature or functionality not supported by DSL API */
   DSL_ERR_NOT_SUPPORTED = -9,
   /** function returned with timeout */
   DSL_ERR_TIMEOUT = -8,
   /** invalid pointer */
   DSL_ERR_POINTER = -7,
   /** invalid memory */
   DSL_ERR_MEMORY = -6,
   /** file open failed */
   DSL_ERR_FILE_OPEN = -5,
   /** file write failed */
   DSL_ERR_FILE_WRITE = -4,
   /** file reading failed */
   DSL_ERR_FILE_READ = -3,
   /** file close failed */
   DSL_ERR_FILE_CLOSE = -2,
   /** Common error */
   DSL_ERROR = -1,

   /** Success */
   DSL_SUCCESS = 0,

   /* *********************************************************************** */
   /* *** Warning Codes Start here !                                      *** */
   /* *********************************************************************** */

   /* *********************************************************************** */
   /* *** Common Warning Codes                                            *** */
   /* *********************************************************************** */
   /** One or more parameters are truncated to min./max or next possible value */
   DSL_WRN_CONFIG_PARAM_TRUNCATED = 1,
   /** DSL CPE API already initialized*/
   DSL_WRN_ALREADY_INITIALIZED = 2,
   /** This warning occurs if the firmware did not accept the last message.
      This may occur if the message is unknown or not allowed in the current
      state. */
   DSL_WRN_FIRMWARE_MSG_DENIED = 9,
   /** This warning occurs if no data available from the device. */
   DSL_WRN_DEVICE_NO_DATA = 10,
   /** The requested functionality is not supported due to build configuration.
       Please refer to the documentation for "Configure options for the DSL CPE
       API Driver" */
   DSL_WRN_NOT_SUPPORTED_DUE_TO_BUILD_CONFIG = 13,
   /** The performed API interface access is not allowed within current autoboot
       state. */
   DSL_WRN_NOT_ALLOWED_IN_CURRENT_STATE = 14,
   /** This warning occurs if there was a request of status information but not
       all returned values includes updated data.
       For example the ioctl \ref DSL_FIO_G997_LINE_STATUS_GET includes six
       parameters that are returned and three of it are requested from far end
       side via overhead channel. If this is not possible because of a not
       responding CO this warning is returned and the according value will have
       its special value.
       The higher layer application shall check all returned values according
       to its special value if this warning is returned. */
   DSL_WRN_INCOMPLETE_RETURN_VALUES = 15,
   /* *********************************************************************** */
   /* *** PM related warning Codes                                        *** */
   /* *********************************************************************** */
   /** Performance Monitor thread was not able to receive showtime related
       counter (TR-98) */
   DSL_WRN_PM_NO_SHOWTIME_DATA = 100,

   /* *********************************************************************** */
   /* *** SNMP/EOC related warning Codes                                 *** */
   /* *********************************************************************** */
   /** CEOC Rx SNMP fifo of DSL CPE API is empty or firmware does not provide
       any data with interrupt. */
   DSL_WRN_SNMP_NO_DATA = 200,
   /** Currently the only protocol that is handled by the DSL CPE API is
       SNMP (0x814C) */
   DSL_WRN_EOC_UNSUPPORTED_PROTOCOLL_ID = 201,

   /* Last warning code marker */
   DSL_WRN_LAST
} DSL_Error_t;

/** @} DRV_DSL_CPE_ERROR_CODES */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_API_ERROR_H */
