/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_API_H
#define _DRV_DSL_CPE_API_H

#ifdef __cplusplus
   extern "C" {
#endif

/*
   For doxygen generation only!
   Take care to avoid excluding any code sections if not used for generating of
   documentation.
*/
#ifndef SWIG
#undef DSL_DOC_GENERATION_EXCLUDE_ADSL_MIB
#endif

#ifdef DSL_INTERN
#ifdef HAVE_CONFIG_H
#include "drv_dsl_cpe_api_config.h"
#define DSL_CPE_API_PACKAGE_VERSION  PACKAGE_VERSION
#else
      #if defined (INCLUDE_DSL_CPE_API_DANUBE)
         #include "drv_dsl_cpe_api_danube_kernel_config.h"
      #elif defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
         #include "drv_dsl_cpe_api_amazon_se_kernel_config.h"
      #endif
   #endif /* HAVE_CONFIG_H */
#endif /*#ifdef DSL_INTERN*/

#include "drv_dsl_cpe_api_types.h"
#include "drv_dsl_cpe_api_error.h"

/*#define HAS_TO_BE_TESTED*/

#define DSL_MAX_TIMEOUT_NUM   2
/** \todo move INCLUDE_FW_REQUEST_SUPPORT definition to the
          configuration step*/
#define INCLUDE_FW_REQUEST_SUPPORT

/** \todo decide whether this mechanism has to be configurable or should be
          always included */
#define INCLUDE_CALLBACK_SUPPORT

/** \file
   DSL CPE API main interface file for user application
*/

/** \defgroup DRV_DSL_CPE_API Infineon DSL CPE API
    Lists the entire modules to the DSL CPE_API.
  @{ */

/**
   \defgroup DRV_DSL_CPE_INIT Initialization Service
   This module does the default initialization of DSL devices including firmware
   download.
*/

/**
   \defgroup DRV_DSL_CPE_COMMON Common Functionality for all Modules
   This module includes all DSL library functionality which does not belong to
   any other module.
*/

/**
   \defgroup DRV_DSL_CPE_G997 Implementation of ITU standard G.997.1
   This module implements the Physical Layer OAM specification G.PLOAM .  */

/**
   \defgroup DRV_DSL_CPE_PM Implementation of Performance Monitoring of ITU standard
   G.997.1.
   This module implements the Performance Monitoring specification G.PLOAM .
*/

/**
   \defgroup DRV_DSL_CPE_EVENT Event Implementation
   Includes all definitions and functions used for the event handling
   (autonomous messages) and a detailed description of event
   implementation/handling.
*/

/**
   \defgroup DRV_DSL_CPE_SAR Implementation of Segmentation and Reassembly API
   This module contains the API of the Segmentation and Reassembly functionality,
   also called interworking between packet- and cell-oriented data interfaces.
*/

/**
   \defgroup DRV_DSL_CPE_ERROR_CODES Error Codes
   Defines all possible error codes returned by the DSL API library
*/

/**
   \defgroup DRV_DSL_CPE_DEBUG Debug Code
   Includes all debug related coding
*/

/**
   \defgroup DRV_DSL_CPE_TYPES Basic Data Types
   Basic data types.
*/

/**
   \defgroup DRV_DSL_CPE_MIB Implementation of xDSL SNMP MIBs
   This module implements the different xDSL MIBs.
*/

/** @} */

/**
   This define specifies to use the exported interface functions instead of
   the ioctl interface. This is only necessary if the access should be done
   from within another driver module within the kernel space.
   The default configuration is to undefine this and use the driver from an
   user space application by ioctl.
   \attention This is included at this place temporarily because it shall be
              done by configure process later on.
*/
/*#define DSL_CPE_API_USE_KERNEL_INTERFACE*/
#undef DSL_CPE_API_USE_KERNEL_INTERFACE
/**
   This define specifies the maximum number of device instances
   \attention This is included at this place temporarily because it shall be
              done by configure process later on.
*/
#define DSL_MAX_DEVICE_INSTANCES 1
/**
   This define specifies the number of event elements in the FIFO
   \attention This is included at this place temporarily because it shall be
              done by configure process later on.
*/
#define DSL_EVENT_FIFO_ELEMENT_COUNT 8

/**
   If number of used bearer channels is not set choose default value of '1'
*/
#ifndef DSL_CHANNELS_PER_LINE
   #define DSL_CHANNELS_PER_LINE 1
      #ifdef _MSC_VER
         #pragma message("WARNING: DSL_CHANNELS_PER_LINE not specified, chossing default '1'")
      #else
         #warning "WARNING: DSL_CHANNELS_PER_LINE not specified, chossing default '1'"
      #endif
#else
   #if (DSL_CHANNELS_PER_LINE > 2)
      #error "ERROR: DSL_CHANNELS_PER_LINE has to be in range of '0..1'"
   #elif (DSL_CHANNELS_PER_LINE > 1)

      #ifdef _MSC_VER
         #pragma message("WARNING: DSL_CHANNELS_PER_LINE=2 activates dual latency which is")
         #pragma message("a feature that is not released!")
      #else
         #warning "WARNING: DSL_CHANNELS_PER_LINE=2 activates dual latency which is"
         #warning "a feature that is not released!"
      #endif
   #endif
#endif

/** \addtogroup DRV_DSL_CPE_G997
 @{ */

/** Maximum length of xTU System Enabling Octets */
#define DSL_G997_NUM_XTSE_OCTETS       8

/** Maximum DRV_DSL_CPE_INIT length of line inventory VendorID */
#define DSL_G997_LI_MAXLEN_VENDOR_ID   8
/** Maximum length of line inventory Version Number */
#define DSL_G997_LI_MAXLEN_VERSION     16
/** Maximum length of line inventory Serial Number */
#define DSL_G997_LI_MAXLEN_SERIAL      32
/** Maximum length of auxiliary inventory information */
#define DSL_G993_LI_MAXLEN_AUX         64
/** @} DRV_DSL_CPE_G997 */

/** \addtogroup DRV_DSL_CPE_COMMON
 @{ */

#define DSL_MAC_ADDRESS_OCTETS   6

/**
   The driver context contains global information.
*/
typedef struct DSL_Context DSL_Context_t;

#ifdef DSL_INTERN
   /* Operating-system related stuff */
   #include "drv_dsl_cpe_os.h"
   #include "drv_dsl_cpe_timeout.h"
#endif

/* define the supported standards */
#ifdef INCLUDE_DSL_CPE_API_DANUBE
   #define INCLUDE_DSL_CPE_API_ADSL_SUPPORT         1
   #define INCLUDE_DSL_CPE_API_VDSL_SUPPORT         0
#endif
#ifdef INCLUDE_DSL_CPE_API_VINAX
   #define INCLUDE_DSL_CPE_API_ADSL_SUPPORT         1
   #define INCLUDE_DSL_CPE_API_VDSL_SUPPORT         1
#endif
#ifdef INCLUDE_DSL_CPE_API_AMAZON_SE
   #define INCLUDE_DSL_CPE_API_ADSL_SUPPORT         1
   #define INCLUDE_DSL_CPE_API_VDSL_SUPPORT         0
#endif

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   /** Defines the maximum number of carrier bands in one direction */
   #define DSL_G997_MAX_NUMBER_OF_BANDS         5
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */


#ifndef DSL_MAX_NSC
   #if defined (INCLUDE_DSL_CPE_API_DANUBE)
      /** maximum number of data elements for subcarrier related data */
      #define DSL_MAX_NSC           512
   #elif defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
      /** maximum number of data elements for subcarrier related data */
      #define DSL_MAX_NSC           512
   #elif defined (INCLUDE_DSL_CPE_API_VINAX)
      /** maximum number of data elements for subcarrier related data */
      #define DSL_MAX_NSC           4096
      /** maximum number of data elements for subcarrier group related data */
      #define VNX_MAX_SCGROUPS      512
   #else
      #error "Device is not defined!"
   #endif
#endif /* #ifndef DSL_MAX_NSC */


/** Maximal length of version information strings */
#define MAX_INFO_STRING_LEN            32

/** @} DRV_DSL_CPE_COMMON */

/** \addtogroup DRV_DSL_CPE_DEBUG
 @{ */

/** Definitions for defining the debug level.
    The smaller the value the less debug output will be printed
*/
typedef enum
{
   /** no output */
   DSL_DBG_NONE = 0x00,
   /** */
   DSL_DBG_PRN = 0x01,
   /** errors will be printed */
   DSL_DBG_ERR = 0x02,
   /** warnings and errors are printed */
   DSL_DBG_WRN = 0x40,
   /** verbose output */
   DSL_DBG_MSG = 0x80,
   /** */
   DSL_DBG_LOCAL = 0xFF
} DSL_debugLevels_t;

/**
   Defines all available debug modules
*/
typedef enum
{
   /** no block selected */
   DSL_DBG_NO_BLOCK = 0,
   /** API function block */
   DSL_DBG_CPE_API = 1,
   /** Internal function block */
   DSL_DBG_G997 = 2,
   /** Performance monitoring */
   DSL_DBG_PM = 3,
   /** MIB specific block */
   DSL_DBG_MIB = 4,
   /** CEOC specific block */
   DSL_DBG_CEOC = 5,
   /** LED specific block */
   DSL_DBG_LED = 6,
   /** SAR specific block */
   DSL_DBG_SAR = 7,
   /** Device specific block */
   DSL_DBG_DEVICE = 8,
   /** Control thread */
   DSL_DBG_AUTOBOOT_THREAD = 9,
   /** Operating system block */
   DSL_DBG_OS = 10,
   /** Callback event handling */
   DSL_DBG_CALLBACK = 11,
   /** Message dump enable. */
   DSL_DBG_MESSAGE_DUMP = 12,
   /** Driver debug level */
   DSL_DBG_LOW_LEVEL_DRIVER = 13,
   /** Last entry for debug blocks - only used as delimiter! */
   DSL_DBG_LAST_BLOCK = 14
} DSL_debugModules_t;

/** @} DRV_DSL_CPE_DEBUG */

#if defined(DSL_INTERN)
/* include DSL CPE API debug header */
#include "drv_dsl_cpe_debug.h"
#endif

/** \addtogroup DRV_DSL_CPE_COMMON
 @{ */

/**
   Empty define used as information direction hint.
   Defines if a parameter has been used as an input parameter */
#ifndef SWIG
#define DSL_IN
#else
#define DSL_IN const
#endif /* SWIG */

/**
   Empty define used as information direction hint
   Defines if a parameter has been used as an output parameter */
#define DSL_OUT

/**
   Empty define used as information direction hint
   Defines if a parameter has been used as a reference which means it is used
   as an input and output parameter */
#define DSL_IN_OUT

/**
   Empty define used as information direction hint
   Defines if a parameter has been used whether as an output or an input
   parameter according to the driver access function as follows
   - ConfigSet -> input
   - ConfigGet -> output
   - StatusGet -> input */
#define DSL_CFG


/** Definition for special value of (bearer) channel if the value is not
    applicable for the actual context handling.
    \remarks Channel definition in API functions is always DSL_uint8_t */
#define DSL_CHANNEL_NA     0xFF


/**
   Defines the direction for which a function call should apply
*/
typedef enum
{
   /** Direction information not applicable */
   DSL_ACCESSDIR_NA = -1,
   /** Upstream access mode. This is the direction xTU-R -> xTU-C.
      It depends on the desired information whether this means a
      near end or far end access. */
   DSL_UPSTREAM = 0,
   /** Downstream access mode. This is the direction xTU-C -> xTU-R.
      It depends on the desired information whether this means a
      near end or far end access. */
   DSL_DOWNSTREAM = 1,
   /* Delimiter only */
   DSL_ACCESSDIR_LAST
} DSL_AccessDir_t;


/**
   Defines the DELT data type for which a function call should apply
*/
typedef enum
{
   /**
      DELT diagnostics data access mode. */
   DSL_DELT_DATA_DIAGNOSTICS = 0,
   /**
      DELT diagnostics data access mode. */
   DSL_DELT_DATA_SHOWTIME = 1,
   /* Delimiter only */
   DSL_DELT_DATA_LAST
} DSL_DeltDataType_t;


/**
   Defines the point of view for which a parameter should apply
*/
typedef enum
{
   /** Direction information not applicable */
   DSL_XTUDIR_NA = -1,
   /** Near-End access mode. This is always the local site. */
   DSL_NEAR_END = 0,
   /** Far-End access mode. This is the foreign connection
      partner of the line. */
   DSL_FAR_END = 1
} DSL_XTUDir_t;


/**
   Defines all possible line init states of the device.
   dependant on the used device some states might not be used.
*/
typedef enum
{
   /* ******************************** */
   /* *** Common line states !     *** */
   /* ******************************** */

   /** Line State is not initialized! */
   DSL_LINESTATE_NOT_INITIALIZED   = 0x00000000,

   /** Line State: EXCEPTION
       corresponds to the VINAX device state FAIL_STATE */
   DSL_LINESTATE_EXCEPTION         = 0x00000001,

   /** Line State: NOT_UPDATED
       internal line state that indicates stopped autoboot thread */
   DSL_LINESTATE_NOT_UPDATED       = 0x00000010,

   /** Line State: IDLE_REQUEST - Interim state between deactivation of line
       and the time this user request is acknowledged by the firmware. */
   DSL_LINESTATE_IDLE_REQUEST      = 0x000000FF,
   /** Line State: IDLE -  corresponds to the
      GEMINAX device state DISABLED and the
      VINAX device state RESET_STATE
      SOCRATES device state DOWN_READY*/
   DSL_LINESTATE_IDLE              = 0x00000100,
   /** Line State: SILENT_REQUEST - Interim state between activation of line
       and the time this user request is acknowledged by the firmware. */
   DSL_LINESTATE_SILENT_REQUEST    = 0x000001FF,
   /** Line State: SILENT -  corresponds to the
       GEMINAX device state IDLE and the
       VINAX device state READY*/
   DSL_LINESTATE_SILENT            = 0x00000200,
   /** Line State: HANDSHAKE
       corresponds to VINAX device state GHS */
   DSL_LINESTATE_HANDSHAKE         = 0x00000300,
   /** Line State: FULL_INIT
       GEMINAX device does not support this state
       corresponds to VINAX device state FULL_INIT */
   DSL_LINESTATE_FULL_INIT         = 0x00000380,
   /** Line State: DISCOVERY
       for VINAX this is a substate of FULL_INIT and is not reported */
   DSL_LINESTATE_DISCOVERY         = 0x00000400,
   /** Line State: TRAINING
       for VINAX this is a substate of FULL_INIT and is not reported */
   DSL_LINESTATE_TRAINING          = 0x00000500,
   /** Line State: ANALYSIS
       for VINAX this is a substate of FULL_INIT and is not reported */
   DSL_LINESTATE_ANALYSIS          = 0x00000600,
   /** Line State: EXCHANGE
       for VINAX this is a substate of FULL_INIT and is not reported */
   DSL_LINESTATE_EXCHANGE          = 0x00000700,
   /** Line State: SHOWTIME
       corresponds to VINAX device state STEADY_STATE_TC_NOSYNC */
   DSL_LINESTATE_SHOWTIME_NO_SYNC  = 0x00000800,
   /** Line State: SHOWTIME_TC_SYNC
       corresponds to VINAX device state STEADY_STATE_TC_SYNC */
   DSL_LINESTATE_SHOWTIME_TC_SYNC  = 0x00000801,
   /** Line State: FASTRETRAIN
       not supported by VINAX */
   DSL_LINESTATE_FASTRETRAIN       = 0x00000900,
   /** Line State: LOWPOWER_L2
       not supported by VINAX */
   DSL_LINESTATE_LOWPOWER_L2       = 0x00000A00,
   /** Line State: DIAGNOSTIC ACTIVE */
   DSL_LINESTATE_LOOPDIAGNOSTIC_ACTIVE   = 0x00000B00,
   /** Line State: DIAGNOSTIC_DATA_EXCHANGE */
   DSL_LINESTATE_LOOPDIAGNOSTIC_DATA_EXCHANGE = 0x00000B10,
   /** This status is used if the DELT data is already available within the
       firmware but has to be updated within the DSL API data elements. If
       the line is within this state the data within a DELT element is NOT
       consistent and shall be NOT read out by the upper layer software.
       \note This state is currently used for the GEMINAX-D device only */
   DSL_LINESTATE_LOOPDIAGNOSTIC_DATA_REQUEST = 0x00000B20,
   /** Line State: DIAGNOSTIC COMPLETE */
   DSL_LINESTATE_LOOPDIAGNOSTIC_COMPLETE = 0x00000C00,
   /** Line State: RESYNC
       not supported by VINAX */
   DSL_LINESTATE_RESYNC            = 0x00000D00,


   /* *********************************************************************** */
   /* *** Line States that may bundle various sates that are not handled  *** */
   /* *** in detail at the moment                                         *** */
   /* *********************************************************************** */

   /** Line State: TEST - common test status may include various test states */
   DSL_LINESTATE_TEST              = 0x01000000,
   /** Line State: any loop activated */
   DSL_LINESTATE_TEST_LOOP         = 0x01000001,

   /** Line State: TEST_REVERB */
   DSL_LINESTATE_TEST_REVERB       = 0x01000010,
   /** Line State: TEST_REVERB */
   DSL_LINESTATE_TEST_MEDLEY       = 0x01000020,
   /** Line State: TEST_SHOWTIME_LOCK */
   DSL_LINESTATE_TEST_SHOWTIME_LOCK= 0x01000030,
   /** Line State: TEST_QUIET */
   DSL_LINESTATE_TEST_QUIET        = 0x01000040,

   /** Line State: LOWPOWER_L3
       not supported by VINAX */
   DSL_LINESTATE_LOWPOWER_L3       = 0x02000000,
   /** Line State: All line states that are not assigned at the moment */
   DSL_LINESTATE_UNKNOWN           = 0x03000000

} DSL_LineStateValue_t;

/**
   This structure handles control and/or management information for common
   driver access.
*/
typedef struct
{
   /**
   Returns the DSL CPE_API driver internal error/return code according to the
   definition of \ref DSL_Error_t */
   DSL_OUT DSL_Error_t nReturn;
} DSL_AccessCtl_t;

/**
   The version information strings.
*/
typedef struct
{
   /**
   DSL CPE_API driver version. This is the version of the DSL Driver related
   layer which provides G.997.1 and ADSL/VDSL MIB functionality.

   Consists of major.minor.build.relchar:
   - major version (major): logical number, incremented for new versions.
   - minor version (minor): logical number, incremented for new minor changes
     in versions.
   - build step (build): logical number, incremented for each new version build.
   - optional release character (relchar): character to distinguish minor
     functional changes
     or special build for one version (starting with "a", "b", "c" etc.) */
   DSL_OUT DSL_char_t DSL_DriverVersionApi[MAX_INFO_STRING_LEN];

   /**
   Firmware version.

   Consists of hw_platform.feature_set.major_ver.minor_ver.rel_indication.annex:

   hardware platform (hw_platform)
   - 0x01: Amazon
   - 0x02: Danube

   feature set (feature_set)
   - indicates the major feature set implementation for firmware (please refer
     to firmware release documentation and/or contact ifx for detailed
     informations.

   major version (major_ver)
   - logical number (incremented for major updates of firmware)

   minor version (minor_ver)
   - logical number (incremented for minor updates of firmware)

   release indication (rel_indication)
   - 0x00: RELEASE_STATUS_RELEASED, The firmware has been tested and released.
   - 0x01: RELEASE_STATUS_PRE_RELEASE, The firmware is undergoing the release
           process; the release has not been completed.

   annex type (annex)
   - 0x01: Annex A
   - 0x02: Annex B */
   DSL_OUT DSL_char_t DSL_ChipSetFWVersion[MAX_INFO_STRING_LEN];

   /**
   Device chip version */
   DSL_OUT DSL_char_t DSL_ChipSetHWVersion[MAX_INFO_STRING_LEN];

   /**
   Chip set type string which specifies the chip name

   The following names are defined
   - Ifx_Danube
   - Ifx_Amazon      (not supported)
   - Ifx_Amazon_SE   (not supported)
   - Ifx_Vinax       (not supported) */
   DSL_OUT DSL_char_t DSL_ChipSetType[MAX_INFO_STRING_LEN];

   /**
   MEI BSP Driver version. This is the version of the low level driver
   which provides basic access to the according device/firmware interface.

   Consists of major.minor.build.rel:
   - major version (major): logical number, incremented for new versions
   - minor version (minor): logical number, incremented for minor changes in versions
   - build step (build): logical number, incremented for each new version build
   - optional release indictor (rel): specifies different Infineon internal indications */
   DSL_OUT DSL_char_t DSL_DriverVersionMeiBsp[MAX_INFO_STRING_LEN];
} DSL_VersionInformationData_t;

/**
   This structure has to be used for ioctl \ref DSL_FIO_VERSION_INFORMATION_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains all necessary version information data */
   DSL_OUT DSL_VersionInformationData_t data;
} DSL_VersionInformation_t;

/**
   Defines the the possible types of firmware requests.
*/
typedef enum
{
   /**
   No dedicated information available for firmware request type.
   \note This values should be never given with event by the driver itself.
         If set, it normally indicates a problem within the driver. */
   DSL_FW_REQUEST_NA = 0,
   /**
   Firmware request is related to an autoboot reboot processing that will start
   in ADSL after successfull firmware download.
   \note For Amazon-SE and Danube this is should be the only value that is
         signaled. */
   DSL_FW_REQUEST_ADSL = 1,
   /**
   Firmware request is related to an autoboot reboot processing that will start
   in VDSL after successfull firmware download.
   \note This value is only available within Vinax versions of DSL CPE API. */
   DSL_FW_REQUEST_VDSL = 2,
   /**
   Delimiter only! */
   DSL_FW_REQUEST_LAST = 3
} DSL_FirmwareRequestType_t;

/**
   Structure to return the current type of firmware request.
   It defines which kind of firmware is needed for the next reboot sequence.
*/
typedef struct
{
   /**
   Current line state. */
   DSL_OUT DSL_FirmwareRequestType_t nFirmwareRequestType;
} DSL_FirmwareRequestData_t;

/**
   Structure to return the current line state (according to appropriate standard,
   e.g. G.992.3)
*/
typedef struct
{
   /**
   Current line state. */
   DSL_OUT DSL_LineStateValue_t nLineState;
} DSL_LineStateData_t;

typedef enum
{
   DSL_FW_LOAD_SUCCESS = 0,
   DSL_FW_LOAD_ERROR = 1
} DSL_FwError_t;

typedef struct
{
   /* Error code*/
   DSL_FwError_t nError;
   /* Indicates which FW request was handled*/
   DSL_FirmwareRequestType_t nFwType;
} DSL_FwDownloadStatusData_t;

/**
   This structure has to be used for ioctl \ref DSL_FIO_LINE_STATE_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains all necessary status data */
   DSL_OUT DSL_LineStateData_t data;
} DSL_LineState_t;

/**
   Structure to write (SET) or read (GET) the configuration or status for the
   lines features.
*/
typedef struct
{
   /**
   Trellis config/status value. */
   DSL_CFG DSL_boolean_t bTrellisEnable;
   /**
   Bitswap config/status value. */
   DSL_CFG DSL_boolean_t bBitswapEnable;
} DSL_LineFeatureData_t;

/**
   This structure has to be used for ioctl
   - \ref DSL_FIO_LINE_FEATURE_CONFIG_SET
   - \ref DSL_FIO_LINE_FEATURE_CONFIG_GET
   - \ref DSL_FIO_LINE_FEATURE_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which direction (upstream/downstream) the function will
   apply */
   DSL_IN DSL_AccessDir_t nDirection;
   /**
   Structure that contains all necessary configuration data */
   DSL_CFG DSL_LineFeatureData_t data;
} DSL_LineFeature_t;

/**
   Structure to return the framing parameter status values.
*/
typedef struct
{
   /**
   Ratio of Number of Mux Data Frames to Sync Octet (TP)
   The ratio of the number of mux data frames to the number of sync octets.
   Valid values are 1 .. 64.
   G993.2: The number of MDFs in an OH sub-frame;
   Tp = k Mp, where k is an integer. The value of Tp shall not exceed 64.
   This parameter is valid only for G.992.3/4/5 and G993.2. */
   DSL_OUT DSL_uint16_t nTp;
   /**
   Frames Per Codeword (S / MP)
   For 992.1 and 992.2, SM denotes the number of data-frames per DMTsymbol.
   This corresponds to parameter S. For the fast path, this
   parameter is always 1; for the interleaved path values from 0...16 are
   possible (0 for S = 1/2 only).
   For 992.3/4/5, SM denotes the number of mux data frames per
   codeword in the particular latency path P. This corresponds to
   parameter MP.
   This parameter is not valid for G.993.1.
   G993.2: The number of MDFs in an RS codeword. Only values of 1, 2, 4, 8,
   and 16 shall be supported.*/
   DSL_OUT DSL_uint16_t nMp;
   /**
   Each latency path that is enabled carries an overhead channel to be
   transported in the sync octets. Generally, each overhead channel can contain
   a CRC portion, a bit oriented portion, and a message oriented portion over a
   repeating sequence of sync octets of length SEQp.
   This parameter is only defined for ADSL2 (G.992.3/4), ADSL2+ (G.992.5) and
   VDSL2 (G993.2).
   and might be used for implementation(s)regarding RFC2662
   (for example: adslAtucChanReceivedBlks). */
   DSL_OUT DSL_uint16_t nSEQ;
   /**
   For G.992.1 and G.992.2 it represents the number of payload bytes per frame
   (raw frame size without overhead, without Reed-Solomon check bytes).
   According to table 6.3/G.992.1, the total data rate including RS-coding
   overhead for the fast path is (1 + Frame_Size + RS)  32 kbit/s
   For G.992.3, G.992.4, G.992.5 it represents the number of payload octets for
   this particular bearer channel and latency path. Corresponds to parameter
   BPN in G.992.3/4/5.
   For G993.2 it shows the number of octets from bearer channel n per MDF.
   The range of values is from 0 to 254. When Gp/Tp is not an integer, the
   number of octets from the bearer channel #0 varies between Bp0 and Bp0+1.
   In order to obtain the number of payload bytes per mux
   data frame, the parameter from all contributing bearer channels has to be
   added. */
   DSL_OUT DSL_uint16_t nBP;
   /**
   Number of Octets in Msg. Based Portion of Overhead Structure (MSGC)
   This parameter corresponds to MSGC in G.992.3 and G.992.4. It is
   undefined for G.992.1 and G.992.2 */
   DSL_OUT DSL_uint16_t nMSGC;
   /**
   This parameter defines the latency path which carries the message oriented
   portion of the overhead channel. This parameter has to be ignored for
   G.992.1 and G.992.2.
   The valid values are 0, 1, 2, and 3. */
   DSL_OUT DSL_uint16_t nMSGLP;
} DSL_FramingParameterStatusData_t;

/**
   Structure to return the actual status of various framing parameter
   This structure has to be used for ioctl
   \ref DSL_FIO_FRAMING_PARAMETER_STATUS_GET
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
   Specifies for which direction (upstream/downstream) the function will
   apply */
   DSL_IN DSL_AccessDir_t nDirection;
   /**
   Structure that contains all necessary status data */
   DSL_OUT DSL_FramingParameterStatusData_t data;
} DSL_FramingParameterStatus_t;

/**
   Defines the testmodes that might be used for testing purpose.
*/
typedef enum
{
   /**
   Used as command: Disable all Test Modes.
   Used for status request: No testmode is currently active
   \attention When using this command to switch off any testmode that has been
              activated before the autoboot state machine will be reset and the
              firmware will be restarted! */
   DSL_TESTMODE_DISABLE = 0,
   /**
   The testmode 'showtime lock' avoids that the autoboot handling will reboot
   the firmware in special conditions such as
   - LOS has been occurred
   - Current SNR margin is below defined minimum value from CO */
   DSL_TESTMODE_SHOWTIME_LOCK = 1,
   /**
   The testmode 'quiet' is used to completely switch off the TX signals for
   NearEnd and to avoid reboot as defined for testmode 'showtime lock' */
   DSL_TESTMODE_QUIET = 2,
   /**
   The testmode 'training lock' is a special mode that will be used by debug
   tools. It includes lock functionality for
   - showtime state as defined within \ref DSL_TESTMODE_SHOWTIME_LOCK
   - training state - line state (\ref DSL_LineStateValue_t) >= 0x380. */
   DSL_TESTMODE_TRAINING_LOCK = 3,
   /**
   The testmode 'lock' will "freeze" the autoboot state machine at any state
   that is currently active. In opposite to the other lock test modes the moment
   when this command is executed directly influences the behavior. */
   DSL_TESTMODE_LOCK = 4,
   /**
   This command can be used for unlocking any testmode.
   Compared to the command \ref DSL_TESTMODE_DISABLE this will NOT reset the
   autoboot state machine and does NOT restart the firmware. Instead of the
   autoboot state will be 'synchronized' with the current firmware state.
   \note This is a command only. If requesting the testmode state after
         successful execution of this command \ref DSL_TESTMODE_DISABLE will be
         returned. */
   DSL_TESTMODE_UNLOCK = 5,
   DSL_TESTMODE_LAST
} DSL_TestModeControlSet_t;

/**
   Structure used for configuration of a testmode.
*/
typedef struct
{
   /**
   Testmode to activate */
   DSL_IN DSL_TestModeControlSet_t nTestMode;
} DSL_TestModeControlData_t;

/**
   Structure used for configuration of a testmode.
   This structure has to be used for ioctl \ref DSL_FIO_TEST_MODE_CONTROL_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Testmode to activate */
   DSL_IN DSL_TestModeControlData_t data;
} DSL_TestModeControl_t;

/**
   Structure used to return the currently active testmode.
*/
typedef struct
{
   /**
   Testmode to activate */
   DSL_IN DSL_TestModeControlSet_t nTestMode;
} DSL_TestModeStatusData_t;

/**
   Structure used for getting the current testmode state.
   This structure has to be used for ioctl \ref DSL_FIO_TEST_MODE_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Returns the testmode state that is currently active. */
   DSL_OUT DSL_TestModeStatusData_t data;
} DSL_TestModeStatus_t;

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   Structure to retrieve Auxiliary Line Inventory information according
   to G.993.2
*/
typedef struct
{
   /**
   Size of the buffer \ref pData in bytes.
   Pass here the size of the buffer to the function.
   In return the function will indicate in this parameter the number of
   bytes retrieved and written to the buffer successfully. */
   DSL_IN_OUT DSL_uint32_t nLength;
   /**
   Auxiliary Inventory bytes.
   Though G.993.2 does not define an upper limit of this field, it is
   limited to \ref DSL_G993_LI_MAXLEN_AUX in DSL-API for the near end. */
   DSL_OUT DSL_uint8_t pData[DSL_G993_LI_MAXLEN_AUX];
} DSL_AuxLineInventoryData_t;

/**
   Structure to retrieve Auxiliary Line Inventory information according
   to G.993.2
   This structure has to be used for ioctl \ref DSL_FIO_AUX_LINE_INVENTORY_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which direction (upstream/downstream) the function will
   apply */
   DSL_IN DSL_AccessDir_t nDirection;
   /**
   Structure that contains all necessary auxiliary version information data */
   DSL_IN_OUT DSL_AuxLineInventoryData_t data;
} DSL_AuxLineInventory_t;
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This structure contains user defined line parameters and results
   for the loop length calculation.
*/
typedef struct
{
   /**
   Line constant in 0.1 [m/dB] for User Line type 0 */
   DSL_IN DSL_uint32_t nUserLine0;
   /**
   Line constant in 0.1 [m/dB] for User Line type 1 */
   DSL_IN DSL_uint32_t nUserLine1;
   /**
   Current loop length in meters for
   - Line type 0.4 mm
   - Linear approximation is 40.0 m/dB */
   DSL_OUT DSL_uint32_t nLength04;
   /**
   Current loop length in meters for
   - Line type 0.5 mm
   - Linear approximation is 50.0 m/dB */
   DSL_OUT DSL_uint32_t nLength05;
   /**
   Current loop length in meters for
   - User Line type 0 */
   DSL_OUT DSL_uint32_t nLengthUser0;
   /**
   Current loop length in meters for
   - User Line type 1 */
   DSL_OUT DSL_uint32_t nLengthUser1;
} DSL_LoopLengthStatusData_t;

/**
   This structure contains user defined line parameters and results
   for the loop length calculation.
   This structure has to be used for ioctl \ref DSL_FIO_LOOP_LENGTH_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains all necessary loop length status data */
   DSL_IN_OUT DSL_LoopLengthStatusData_t data;
} DSL_LoopLengthStatus_t;
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   Structure used for configuration access of the assignment from the logical
   DSL line to the used UTOPIA/POS PHY Address.
 */
typedef struct
{
   /**
   UTOPIA/POS PHY Address number */
   DSL_CFG DSL_uint8_t nPhyAdr;
} DSL_PhyAddressConfigData_t;

/**
   Structure used for configuration access of the assignment from the logical
   DSL line to the used UTOPIA/POS PHY Address.
   This structure has to be used for ioctl
   - \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_SET
   - \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_GET
   \if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   - \ref DSL_FIO_POSPHY_ADDRESS_CONFIG_SET
   - \ref DSL_FIO_POSPHY_ADDRESS_CONFIG_GET
   \endif
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
   Structure that contains configuration data */
   DSL_CFG DSL_PhyAddressConfigData_t data;
} DSL_PhyAddressConfig_t;

/**
   Definitions for possible Customer Interfaces
*/
typedef enum
{
   /**
   select the UTOPIA - System Interface
   \note This configuration/status is only valid for Vinax */
   DSL_SYSTEMIF_UNKNOWN = 0,
   /**
   select the UTOPIA - System Interface
   \note This configuration/status is only valid for Vinax */
   DSL_SYSTEMIF_UTOPIA = 1,
   /**
   select the POSPHY - System Interface
   \note This configuration/status is only valid for Vinax */
   DSL_SYSTEMIF_POSPHY = 2,
   /**
   select the MII - System Interface
   \note For Danube this configuration/status is the only valid one */
   DSL_SYSTEMIF_MII = 3,
   /**
   System Interface was not yet configured
   \note This configuration/status is only valid for Vinax */
   DSL_SYSTEMIF_UNDEFINED = 4,
   /*
   Delimiter */
   DSL_SYSTEMIF_LAST
} DSL_SystemInterfaceSelection_t;

/**
   Definitions for possible TC-Layers
*/
typedef enum
{
   /**
   Indicates that the TC-layer configuration is unknown at the current system
   state. This initialization value in only valid for status request (not for
   configuration) and should be only set after first startup of DSL CPE API
   and before first full init line state is passed. */
   DSL_TC_UNKNOWN = 0,
   /**
   select the TC-layer - ATM */
   DSL_TC_ATM = 1,
   /**
   select the TC-layer - EFM (PTM) */
   DSL_TC_EFM = 2,
   /**
   select the TC-layer - HDLC
   \note This configuration is only valid for Vinax */
   DSL_TC_HDLC = 3,
   /**
   select the TC-layer - Autonegotiation
   \note This configuration is only valid for Danube so far
   \note In case of a status request this value should be never returned */
   DSL_TC_AUTO = 4,
   /*
   Delimiter */
   DSL_TC_LAST
} DSL_TcLayerSelection_t;

/**
   Bitfield definitions for possible operation modes for TC-Layer in case of
   selecting EFM as current used TC-Layer selection.
   \note In case of writing or reading the current configuration setting any
         combination might be used. In case of status request only one bit is
         used.
*/
typedef enum
{
   /* Cleaned. */
   DSL_EMF_TC_CLEANED = 0x00000000,
   /**
   64/65-octet encapsulation supported
   \note For Vinax this is the only valid configuration option
   \note For Danube downstream this is the only valid configuration option */
   DSL_EMF_TC_NORMAL = 0x00000001,
   /**
   64/65-octet encapsulation with pre-emption
   \note This configuration is only valid for upstream configuration of Danube */
   DSL_EMF_TC_PRE_EMPTION = 0x00000002,
   /**
   64/65-octet encapsulation with short packets
   \note This configuration is for future usage and not supported at the moment */
   DSL_EMF_TC_SHORT_PACKETS = 0x00000004
} DSL_BF_EfmTcConfig_t;

/**
   Structure used for System Interface configuration.
*/
typedef struct
{
   /**
   Specifies the TC-Layer */
   DSL_CFG DSL_TcLayerSelection_t nTcLayer;
   /**
   Specifies the operation mode for upstream direction in case of using of EFM
   TC-layer */
   DSL_CFG DSL_BF_EfmTcConfig_t nEfmTcConfigUs;
   /**
   Specifies the operation mode for downstream direction in case of using of EFM
   TC-layer */
   DSL_CFG DSL_BF_EfmTcConfig_t nEfmTcConfigDs;
   /**
   Specifies the System Interface
   \note For the Danube version of the DSL CPE API the only possible mode
         is DSL_SYSTEMIF_MII. */
   DSL_CFG DSL_SystemInterfaceSelection_t nSystemIf;
} DSL_SystemInterfaceConfigData_t;

/**
   Structure used for System Interface configuration
   This structure has to be used for ioctl
   - \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET
   - \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains system interface configuration settings */
   DSL_CFG DSL_SystemInterfaceConfigData_t data;
} DSL_SystemInterfaceConfig_t;

/**
   Structure used for System Interface configuration
   This structure has to be used for ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_STATUS_GET
   and event
   \ref DSL_EVENT_S_SYSTEM_INTERFACE_STATUS
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains system interface configuration settings */
   DSL_CFG DSL_SystemInterfaceConfigData_t data;
} DSL_SystemInterfaceStatus_t;

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   Band Plan Selection
   Several bandplans from different standards are listed here.
   The bandplan defines the usage of tones grouped to bands
   for upstream and downstream transmission. Number width and position
   of these bands as well as the PSD shape over the spectrum is defined
   by the bandplan.
*/
typedef enum
{
   /** ITU G993.2 Annex A Table A-2 Mask 1 */
   DSL_BANDPLAN_ITU_ANNEX_A_M1_EU32                = 0,
   /** ITU G993.2 Annex A Table A-2 Mask 9 */
   DSL_BANDPLAN_ITU_ANNEX_A_M9_EU64                = 1,
   /** ITU G993.2 Annex A Table A-4 Mask 1 */
   DSL_BANDPLAN_ITU_ANNEX_A_M1_ADLU32              = 2,
   /** ITU G993.2 Annex A Table A-4 Mask 9 */
   DSL_BANDPLAN_ITU_ANNEX_A_M9_ADLU64              = 3,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-1 997-M1c-A-7 */
   DSL_BANDPLAN_ITU_ANNEX_B_B01                    = 4,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-2 997-M1x-M-8 */
   DSL_BANDPLAN_ITU_ANNEX_B_B02                    = 5,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-3 997-M1x-M */
   DSL_BANDPLAN_ITU_ANNEX_B_B03                    = 6,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-4 997-M2x-M-8 */
   DSL_BANDPLAN_ITU_ANNEX_B_B04                    = 7,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-5 997-M2x-A */
   DSL_BANDPLAN_ITU_ANNEX_B_B05                    = 8,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-6 997-M2x-M */
   DSL_BANDPLAN_ITU_ANNEX_B_B06                    = 9,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-1 998-M1x-A */
   DSL_BANDPLAN_ITU_ANNEX_B_B07                    = 10,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-2 998-M1x-B */
   DSL_BANDPLAN_ITU_ANNEX_B_B08                    = 11,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-3 998-M1x-NUS0 */
   DSL_BANDPLAN_ITU_ANNEX_B_B09                    = 12,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-4 998-M2x-A */
   DSL_BANDPLAN_ITU_ANNEX_B_B10                    = 13,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-5 998-M2x-M */
   DSL_BANDPLAN_ITU_ANNEX_B_B11                    = 14,
   /** ITU G993.2 Annex B
       998-M2x-M 17MHz */
   DSL_BANDPLAN_ITU_ANNEX_B_B11_17                 = 15,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-6 998-M2x-B */
   DSL_BANDPLAN_ITU_ANNEX_B_B12                    = 16,
   /** ITU G993.2 Annex B
       998-M2x-B 17MHz */
   DSL_BANDPLAN_ITU_ANNEX_B_B12_17                 = 17,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-7 998-M2x-NUS0 */
   DSL_BANDPLAN_ITU_ANNEX_B_B13                    = 18,
   /** ITU G993.2 Annex B
       998-M2x-NUS0 17MHz */
   DSL_BANDPLAN_ITU_ANNEX_B_B13_17                 = 19,
   /** ITU G993.2 Annex C
        */
   DSL_BANDPLAN_ITU_ANNEX_C                        = 20,
   /** ITU G993.2 Annex C  8kHz spacing */
   DSL_BANDPLAN_ITU_ANNEX_C_8K                     = 21,
   /** ITU G993.2 Annex B  997E30 M2x NUS0
      Table B-2, B-3, B-4:  B7-10 997E30 M2x NUS0*/
   DSL_BANDPLAN_ITU_ANNEX_B_997E30_M2X_NUS0        = 22,
   /** ITU G993.2 Annex C, starting at 1.1MHz
         */
   DSL_BANDPLAN_ITU_ANNEX_C_1M1                    = 23,
   /** ITU G993.2 Annex C  8kHz spacing, starting at 1.1MHz
        */
   DSL_BANDPLAN_ITU_ANNEX_C_8K_1M1                 = 24,
   /** ITU G993.2 Annex B
       998E17-M2x-A */
   DSL_BANDPLAN_ITU_ANNEX_B_998E17_M2X_A           = 25,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-8 998E17-M2x-NUS0 */
   DSL_BANDPLAN_ITU_ANNEX_B_998E17_M2X_NUS0        = 26,
   /** ANFP Cal0 long */
   DSL_BANDPLAN_ANFP_CAL0_LONG                     = 27,
   /** ANFP Cal0 medium */
   DSL_BANDPLAN_ANFP_CAL0_MEDIUM                   = 28,
   DSL_BANDPLAN_LAST
} DSL_BandPlanType_t;

/**
   Profile Selection
   You may select among the 8 VDSL2 profiles
   defined in G993.2 chapter 6.1
   The profile defines some transmission parameters like
   maximum transmit power, subcarrier spacing ...
*/
typedef enum
{
   /** Profile 8a */
   DSL_PROFILE_8A = 0,
   /** Profile 8b */
   DSL_PROFILE_8B = 1,
   /** Profile 8c */
   DSL_PROFILE_8C = 2,
   /** Profile 8d */
   DSL_PROFILE_8D = 3,
   /** Profile 12a */
   DSL_PROFILE_12A = 4,
   /** Profile 12b */
   DSL_PROFILE_12B = 5,
   /** Profile 17a */
   DSL_PROFILE_17A = 6,
   /** Bandplan 30a */
   DSL_PROFILE_30A = 7,
   /** Profile 17b */
   DSL_PROFILE_17B = 8,

   DSL_PROFILE_LAST = 9
} DSL_ProfileType_t;

/**
   Structure to read the current band plan configuration.
*/
typedef struct
{
   /** Selection of the Bandplan */
   DSL_OUT DSL_BandPlanType_t nBandPlan;
   /** Selection of the Profile */
   DSL_OUT DSL_ProfileType_t nProfile;
} DSL_BandPlanConfigData_t;

/**
   Structure to read the current band plan configuration.
   This structure has to be used for ioctl \ref DSL_FIO_BAND_PLAN_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains all necessary version information data */
   DSL_OUT DSL_BandPlanConfigData_t data;
} DSL_BandPlanStatus_t;

/**
   Matrix of all possible combinations of
   profile from \ref DSL_ProfileType_t and bandplan from \ref DSL_BandPlanType_t.
   Each supported combination is represented by DSL_TRUE.
*/
typedef struct
{
   /** Bandplan vs profile support matrix */
   DSL_OUT DSL_boolean_t bSupported[DSL_BANDPLAN_LAST][DSL_PROFILE_LAST];
} DSL_BandPlanSupportData_t;

/**
   This function returns all VDSL2 bandplan/profile combinations supported by
   the device.
   This structure has to be used for ioctl \ref DSL_FIO_BAND_PLAN_SUPPORT_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains all necessary version information data */
   DSL_OUT DSL_BandPlanSupportData_t data;
} DSL_BandPlanSupport_t;

/**
   EFM Speed */
typedef enum
{
   /**
      Speed is 100 Mbps */
   DSL_EFM_SPEED_100 = 0,
   /**
      Speed is 10 Mbps */
   DSL_EFM_SPEED_10 = 1,
   /**
      Delimiter only! */
   DSL_EFM_SPEED_LAST = 2
} DSL_EFM_Speed_t;

/**
   EFM Duplex Mode */
typedef enum
{
   /**
      EFM Duplex mode is: Full Duplex */
   DSL_EFM_DUPLEX_FULL = 0,
   /**
      EFM Duplex mode is: Half Duplex */
   DSL_EFM_DUPLEX_HALF = 1,
   /**
      Delimiter only! */
   DSL_EFM_DUPLEX_LAST = 2
} DSL_EFM_Duplex_t;

/**
   Flow Control by pause frames on the MII
*/
typedef enum
{
   /**
      Flow Control is off */
   DSL_EFM_FLOWCTRL_OFF    = 0,
   /**
      Flow Control is on */
   DSL_EFM_FLOWCTRL_ON   = 1,
   /**
      Delimiter only! */
   DSL_EFM_FLOWCTRL_LAST
} DSL_EFM_FlowCtrl_t;

/**
   This type defines the autonegotiation mode for the MAC in the device.
   If the autonegotiation is enabled, then the device will poll via MDIO
   register access the status of an attached ethernet phy and configure
   the speed and the duplex mode of the MAC accordingly.
   This only represents the status of the MAC and not of an attached
   ethernet phy. The autonegotiation function in the ethernet phy has
   to be enabled with a separate MDIO access.
*/
typedef enum
{
  /**
     Autonegotiation is disabled.
  */
  DSL_EFM_AUTONEG_OFF = 0,
  /**
     Autonegotiation is enabled
  */
  DSL_EFM_AUTONEG_ON = 1,
   /**
      Delimiter only! */
  DSL_EFM_AUTONEG_LAST
} DSL_EFM_AutoNegotiation_t;

/**
   This struct represents the MAC configuration settings. This only represents
   the settings of the MAC in the device and not of a possibly attached
   ethernet phy.
*/
typedef struct{
  /**
     Speed setting of the MAC interface. This can be 10MBit/s or 100MBit/s
  */
  DSL_CFG DSL_EFM_Speed_t nEfmSpeed;
  /**
     Duplex mode of the MAC interface. This can be Full Duplex or Half Duplex.
  */
  DSL_CFG DSL_EFM_Duplex_t nEfmDuplex;
  /**
     Flow Control mode of the MAC interface. This can be on or off. When flow
     control is activated in full duplex mode, then PAUSE frame according to
     IEEE 802.3 Annex 31B are used for flow control. In half-duplex mode a
     jam pattern is generated.
  */
  DSL_CFG DSL_EFM_FlowCtrl_t nFlowControl;
  /**
     Autonegotiation mode of the MAC interface. This can be enabled or disabled.
     When enabled, the device will poll via MDIO register access the speed and
     duplex state of an attached ethernet phy. Note that the ethernet
     autonegotiation in the ethernet phy has to be activated separately via a
     MDIO access.
     \note VINAX supports this parameter for hostless CPE only, means
           for far end from CO side.
  */
  DSL_CFG DSL_EFM_AutoNegotiation_t nAutoNegotiation;
} DSL_EFM_MacConfigData_t;

/**
   This structure has to be used for ioctl
   - \ref DSL_FIO_EFM_MAC_CONFIG_SET
   - \ref DSL_FIO_EFM_MAC_CONFIG_GET
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
   Structure that contains configuration data */
   DSL_CFG DSL_EFM_MacConfigData_t data;
} DSL_EFM_MacConfig_t;


/**
   This structure is used to get total data path counters of the
   system side.
*/
typedef struct
{
   /**
   Transmitted Units (Cells or Frames)
   This Value refers to aFramesTransmittedOK
   of IEEE 802.3 chapter 30.3.1.1.2

   APPROPRIATE SYNTAX:
      Generalized nonresettable counter. This counter has a
      maximum increment rate of 16 000 counts
      per second at 10 Mb/s

   BEHAVIOUR DEFINED AS:
      A count of frames that are successfully transmitted.
      This counter is incremented when the
      TransmitStatus is reported as transmitOK. The actual
      update occurs in the LayerMgmtTransmitCounters procedure
      (5.2.4.2).; */
   DSL_uint32_t UnitsTx;
   /**
   Received Units (Cells or Frames)
   This value refers to of IEEE 802.3 chapter 30.3.1.1.5 aFramesReceivedOK

   APPROPRIATE SYNTAX:
      Generalized nonresettable counter. This counter has a maximum
      increment rate of 16 000 counts per second at 10 Mb/s

   BEHAVIOUR DEFINED AS:
      A count of frames that are successfully received (receiveOK).
      This does not include frames
      received with frame-too-long, FCS, length or alignment errors,
      or frames lost due to internal MAC
      sublayer error. This counter is incremented when the
      ReceiveStatus is reported as receiveOK. The
      actual update occurs in the LayerMgmtReceiveCounters procedure
      (5.2.4.3).; */
   DSL_uint32_t UnitsRx;
   /**
   This value refers to of IEEE 802.3 chapter 5.2.2.1.6
   aFrameCheckSequenceErrors

   APPROPRIATE SYNTAX:
      Generalized nonresetable counter. This counter has a maximum increment
      rate of 16 000 counts per second.

   BEHAVIOUR DEFINED AS:
         A count of frames that are an integral number of octets in length
         and do not pass the FCS check. This counter is incremented when
         the ReceiveStatus is reported as frameCheckError.
         The actual update occurs in the LayerMgmtReceiveCounters procedure
         (5.2.4.3). */
   DSL_uint32_t FrameCheckSequenceErrors;
   /**
   This value refers to of IEEE 802.3 chapter 5.2.2.1.7
   aAlignmentErrors

   APPROPRIATE SYNTAX:
      Generalized nonresetable counter. This counter has a maximum increment
      rate of 16 000 counts per second.

   BEHAVIOUR DEFINED AS:
      A count of frames that are not an integral number of octets in length
      and do not pass the FCS check. This counter is incremented when the
      ReceiveStatus is reported as alignmentError. The actual update occurs in
      the LayerMgmtReceiveCounters procedure (5.2.4.3). */
   DSL_uint32_t AlignmentErrors;
   /**
   Transmitted Octets
   This value refers to of IEEE 802.3 chapter 30.3.1.1.8 aOctetsTransmittedOK

   APPROPRIATE SYNTAX:
      Generalized nonresettable counter. This counter
      has a maximum increment rate of 1 230 000
      counts per second at 10 Mb/s

   BEHAVIOUR DEFINED AS:
      A count of data and padding octets of frames
      that are successfully transmitted. This counter is
      incremented when the TransmitStatus is reported
      as transmitOK. The actual update occurs in the
      LayerMgmtTransmitCounters procedure (5.2.4.2).; */
   DSL_uint32_t OctetsTx;
   /**
   Received Octets
   This value refers to of IEEE 802.3 chapter 30.3.1.1.14 aOctetsReceivedOK

   APPROPRIATE SYNTAX:
      Generalized nonresettable counter. This counter
      has a maximum increment rate of 1 230 000
      counts per second at 10 Mb/s

   BEHAVIOUR DEFINED AS:
      A count of data and padding octets in frames
      that are successfully received. This does not include
      octets in frames received with frame-too-long, FCS,
      length or alignment errors, or frames lost due
      to internal MAC sublayer error. This counter is
      incremented when the result of a reception is
      reported as a receiveOK status. The actual
      update occurs in the LayerMgmtReceiveCounters
      procedure (5.2.4.3).; */
   DSL_uint32_t OctetsRx;
   /**
   Units with Tx Errors
   This value refers to of IEEE 802.3 chapter 30.3.1.1.12
   aFramesLostDueToIntMACXmitError

   APPROPRIATE SYNTAX:
      Generalized nonresettable counter. This counter has a maximum increment
      rate of 75 000 counts per second at 10 Mb/s

   BEHAVIOUR DEFINED AS:
      A count of frames that would otherwise be transmitted by the station,
      but could not be sent due to an internal MAC sublayer transmit error.
      If this counter is incremented, then none of the other counters in this
      section are incremented. The exact meaning and mechanism for
      incrementing this counter is implementation dependent.; */
   DSL_uint32_t UnitsTxErrors;
   /**
   Units with Rx Errors
   This value refers to of IEEE 802.3 chapter 30.3.1.1.15
   aFramesLostDueToIntMACRcvError

   APPROPRIATE SYNTAX:
      Generalized nonresettable counter. This counter has a maximum increment
      rate of 16 000 counts per second at 10 Mb/s

   BEHAVIOUR DEFINED AS:
      A count of frames that would otherwise be received by the station, but
      could not be accepted due to an internal MAC sublayer receive error.
      If this counter is incremented, then none of the other counters in this
      section are incremented. The exact meaning and mechanism for
      incrementing this counter is implementation dependent.; */
   DSL_uint32_t UnitsRxErrors;
   /**
   Pause Frames Tx
   This value refers to of IEEE 802.3 chapter 30.3.4.2
   aPAUSEMACCtrlFramesTransmitted

   APPROPRIATE SYNTAX:
      Generalized nonresettable counter. This counter has a maximum increment
      rate of 16 000 counts per second at 10 Mb/s

   BEHAVIOUR DEFINED AS:
      A count of PAUSE frames passed to the MAC sublayer for transmission.
      This counter is incremented when a MA_CONTROL.request primitive is
      generated within the MAC Control sublayer with an opcode indicating the
      PAUSE operation.; */
   DSL_uint32_t PauseFramesTx;
   /**
   Pause Frames Rx
   This value refers to of IEEE 802.3 chapter 30.3.4.3
   aPAUSEMACCtrlFramesReceived

   APPROPRIATE SYNTAX:
      Generalized nonresettable counter. This counter has a maximum increment
      rate of 16 000 counts per second at 10 Mb/s

   BEHAVIOUR DEFINED AS:
      A count of MAC Control frames passed by the MAC sublayer to the MAC
      Control sublayer. This counter is incremented when a ReceiveFrame
      function call returns a valid frame with: (1) a lengthOrType field
      value equal to the reserved Type for 802.3_MAC_Control as specified
      in 31.4.1.3, and (2) an opcode indicating the PAUSE operation.; */
   DSL_uint32_t PauseFramesRx;
   /**
   Frame too long errors
   This value refers to of IEEE 802.3 chapter 30.3.1.1.25 aFrameTooLongErrors

   APPROPRIATE SYNTAX:
      Generalized nonresettable counter. This counter has a maximum increment
      rate of 815 counts per second at 10 Mb/s

   BEHAVIOUR DEFINED AS:
      A count of frames received that exceed the maximum permitted frame
      size. This counter is incremented when the status of a frame reception
      is frameTooLong. The actual update occurs in the
      LayerMgmtReceiveCounters procedure (5.2.4.3).
      NOTE: Implementations may use either maxUntaggedFrameSize or
      (maxUntaggedFrameSize + qTagPre.xSize) (see 4.2.7.1 and 4.4.2) for the
      maximum permitted frame size, either as a constant or as a function of
      whether the frame received is a basic or tagged frame (see 3.2 and 3.5).
      In implementations that treat this as a constant it is recommended that
      the larger value be used. The use of any value other than the larger
      value in this case may result in the counter being incremented by valid
      tagged frames.; */
   DSL_uint32_t FramesTooLong;
   /**
   Frame too short errors
   This value refers to counter etherStatsUndersizePkts from RFC2819

   DESCRIPTION:
      "The total number of packets received that were
      less than 64 octets long (excluding framing bits,
      but including FCS octets) and were otherwise well
      formed." */
   DSL_uint32_t FramesTooShort;
} DSL_LinePathCounterTotalData_t;

/**
   This structure is used to get total data path counters of the
   system side of the device and has to be used for ioctl
   \ref DSL_FIO_LINE_PATH_COUNTER_TOTAL_GET
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
   Structure that contains configuration data */
   DSL_CFG DSL_LinePathCounterTotalData_t data;
} DSL_LinePathCounterTotal_t;

#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   Structure for showtime event logging data upload.
*/
typedef struct
{
   /**
   Pointer to logging data. */
   DSL_OUT DSL_uint8_t *pData;
   /**
   Size of logging data.
   This value is only valid if pData pointer is NOT equal DSL_NULL */
   DSL_IN DSL_uint32_t nDataSize;
   /**
   Offset of current chunk within complete logging data.
   This values has to be used in case of using chunks for logging data upload
   only.
   A value equal to 0 means that chunk are NOT used.
   A value >0 and smaller nDataSize indicates a chunk. In this case the ioctl
   has to be called multiple times until nDataOffset is equal to nDataSize
   which means the complete data has been transferred. */
   DSL_IN DSL_uint32_t nDataOffset;
} DSL_ShowtimeLoggingData_t;

/**
   Structure for showtime event logging data upload.
   This structure has to be used for ioctl
   \ref DSL_FIO_SHOWTIME_LOGGING_DATA_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains showtime event logging data */
   DSL_OUT DSL_ShowtimeLoggingData_t data;
} DSL_ShowtimeLogging_t;

#ifdef DSL_INTERN
/**
   Specifies all possible timeout event types.
*/
typedef enum
{
   /**
      Reserved event timeout identifier shall be NOT used! */
   DSL_TIMEOUTEVENT_RESERVED = 0,
   /**
      Event timeout identifier for initialization of API */
   DSL_TIMEOUTEVENT_API_INIT = 1,
   /**
      Event timeout identifier for far end information retrieval */
   DSL_TIMEOUTEVENT_FE_STATUS = 2
} DSL_TimeoutEvent_t;

/**
   Defines all possible (ADSL) latency path configurations.
*/
typedef enum
{
   /**
   Zero has been chosen to indicate 'not initialized' after memset of context
   structure after startup for example */
   DSL_LATENCY_UNKNOWN = 0,
   /**
   Latency path is not used */
   DSL_LATENCY_DISABLED = 1,
   /**
   Latency path is interleaved (ADSL1) or LP0 for ADSL2/2+ */
   DSL_LATENCY_IP_LP0 = 2,
   /**
   Latency path is fast (ADSL1) or LP1 for ADSL2/2+ */
   DSL_LATENCY_FP_LP1 = 3,
   DSL_LATENCY_LAST
} DSL_LatencyPath_t;
#endif


/** @} DRV_DSL_CPE_COMMON */

/** \addtogroup DRV_DSL_CPE_EVENT
 @{ */


/**
   Defines all possible classes of callback event types according to G.997.1
   standard (see figure 7-2).

   Meaning of group specifiers
   - prefix DSL_EVENT_  indicates a common event type specifier
   - prefix DSL_EVENT_I_ indicates an event for indication or alarm
   - prefix DSL_EVENT_S_ indicates a, event for status

*/
typedef enum
{
   /**
   alias for first callback event */
   #define DSL_EVENT_FIRST   DSL_EVENT_I_FIRST

   /* *********************************************************************** */
   /* *** FAULT MONITORING EVENTS                                         *** */
   /* *********************************************************************** */
   /**
   alias for first callback indication */
   #define DSL_EVENT_I_FIRST   DSL_EVENT_I_LINE_FAILURES
   /**
   (0) Direction (Near/Far End) specific line failure(s) occurred
   (e.g. LOS, LOF, LPR)
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_LineFailures_t */
   DSL_EVENT_I_LINE_FAILURES = 0,
   /**
   (1) Direction (Near/Far End) specific data path failure(s) occurred
   (e.g. NCD, LCD)
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_DataPathFailures_t */
   DSL_EVENT_I_DATA_PATH_FAILURES,
   /**
   (2) Direction (Near/Far End) specific line threshold value exceeded */
   DSL_EVENT_I_LINE_THRESHOLD_CROSSING,
   /**
   (3) Direction (Near/Far End) specific channel threshold value exceeded
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_ChannelDataRateThreshold_t */
   DSL_EVENT_I_CHANNEL_THRESHOLD_CROSSING,
   /**
   (4) Direction (Near/Far End) specific data path threshold value exceeded */
   DSL_EVENT_I_DATA_PATH_THRESHOLD_CROSSING,
   /**
   (5) Data rate up/down-shift threshold crossing
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_DataRateShiftThresholdCrossingData_t */
   DSL_EVENT_I_CHANNEL_DATARATE_SHIFT_THRESHOLD_CROSSING,
   /**
   alias for last callback indication */
   #define DSL_EVENT_I_LAST    DSL_EVENT_S_FIRST

   /* *********************************************************************** */
   /* *** TEST, DIAGNOSTIC AND STATUS EVENTS                              *** */
   /* *********************************************************************** */
   /**
   alias for first callback status event */
   #define DSL_EVENT_S_FIRST   DSL_EVENT_S_LINIT_FAILURE
   /**
   (6) Line initialization failure (LINIT)
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_LineInit_t */
   DSL_EVENT_S_LINIT_FAILURE,
   /**
   (7) Line state change event
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_LineState_t */
   DSL_EVENT_S_LINE_STATE,
   /**
   (8) Line power management state change event
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_PowerManagementStatus_t */
   DSL_EVENT_S_LINE_POWERMANAGEMENT_STATE,
   /**
   (9) Data rate change event
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_ChannelStatus_t */
   DSL_EVENT_S_CHANNEL_DATARATE,
   /**
   (10) Firmware error event */
   DSL_EVENT_S_FIRMWARE_ERROR,
   /**
   (11) DSL API initialization ready event
   \attention It is strongly recommended to check for this event before
   calling any configuration functions, because the DMS does not guarantee
   a proper functionality before. */
   DSL_EVENT_S_INIT_READY,
   /**
   (12) FarEnd line inventory available */
   DSL_EVENT_S_FE_INVENTORY_AVAILABLE,
   /**
   (13) System status event
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_SystemStatusEvent_t */
   DSL_EVENT_S_SYSTEM_STATUS,
   /**
   (14) Performance Monitoring synchronization (PM sync) event
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_CB_DATA_PmSync_t */
   DSL_EVENT_S_PM_SYNC,
   /**
   (15) Line transmission status
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_LineTransmissionStatus_t */
   DSL_EVENT_S_LINE_TRANSMISSION_STATUS,
   /**
   (16) Showtime Event Logging
   InfoEvent which does not include any additional data beside event
   information itself.
   To get detailed data the ioctl \ref DSL_FIO_SHOWTIME_LOGGING_DATA_GET has
   to be used. */
   DSL_EVENT_S_SHOWTIME_LOGGING,
   /**
   (17) FW binary request Event
   InfoEvent which does not include any additional data beside event
   information itself.
   Event informs the upper layer application to provide the FW binary
   via the ioctl \ref DSL_FIO_AUTOBOOT_LOAD_FIRMWARE */
   DSL_EVENT_S_FIRMWARE_REQUEST,
   /**
   (18) SNMP Message available Event
   This is an InfoEvent which does not include any additional data beside event
   information itself up to now.
   Event informs the upper layer application that SNMP message has to be read
   out from driver using the ioctl \ref DSL_FIO_G997_SNMP_MESSAGE_RECEIVE
   \attention For future versions of the driver it might be necessary to extend
   the event by returning the SNMP data directly within this event. But this
   is not planned up to now. */
   DSL_EVENT_S_SNMP_MESSAGE_AVAILABLE,
   /**
   (19) System Interface Status
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_SystemInterfaceStatus_t */
   DSL_EVENT_S_SYSTEM_INTERFACE_STATUS,
   /**
   (20) FW download status event
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_FwDownloadStatusData_t */
   DSL_EVENT_S_FIRMWARE_DOWNLOAD_STATUS,
   /**
   alias for last callback status event */
#define DSL_EVENT_S_LAST    DSL_EVENT_LAST
   /**
   alias for last callback event */
   DSL_EVENT_LAST
} DSL_EventType_t;


#include "drv_dsl_cpe_api_g997.h"

#if defined(INCLUDE_DSL_PM)
   #include "drv_dsl_cpe_api_pm.h"
#endif

/**
   encapsulate all event data/status structures
*/
typedef union
{
   DSL_LineStateData_t                           lineStateData;
   DSL_FirmwareRequestData_t                     fwRequestData;
   DSL_SystemInterfaceConfigData_t               systemInterfaceStatusData;
   DSL_G997_ChannelStatusData_t                  channelStatusData;
   DSL_G997_LineInit_t                           lineInitStatusData;
   DSL_G997_DataPathFailuresData_t               dataPathFailuresData;
   DSL_G997_LineFailuresData_t                   lineFailuresData;
   DSL_G997_DataRateShiftThresholdCrossingData_t dataRateThresholdCrossing;
   DSL_G997_PowerManagementStatusData_t          powerMgmtStatusData;
   DSL_FwDownloadStatusData_t                    fwDownloadStatus;
#if defined(INCLUDE_DSL_PM)
   #if defined(INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS)
   DSL_PM_ChannelThresholdCrossing_t             channelThresholdCrossing;
   #endif
   DSL_PM_LineThresholdCrossing_t                lineThresholdCrossing;
   #if defined(INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS)
   DSL_PM_DataPathThresholdCrossing_t            dataPathThresholdCrossing;
   #endif
#endif
} DSL_EventData_Union_t;


/**
   Defines all possible classes of resource activation types.
   Each resource activation type that is listed, belongs to an internal memory
   resource (FIFO) which is allocated on activation respectively freed on
   deactivation.
*/
typedef enum
{
   /* Cleaned. */
   DSL_RESOURCE_ACTIVATION_CLEANED = 0x00000000,
   /**
   Activates/deactivates SNMP resource buffer.
   Creates a FIFO on activation which includes a number of elements that is
   defined by DSL_CEOC_RX_FIFO_ELEMENT_COUNT (number of max. stored events).
   Each elements size is defined by size of structure DSL_CEOC_Message_t. */
   DSL_RESOURCE_ACTIVATION_SNMP = 0x00000001
} DSL_BF_ResourceActivationType_t;

/**
   Structure that is used to initialize the event handling of the DSL CPE API
   driver. Within this context all necessary FIFO memory will be allocated.
*/
typedef struct
{
   /**
   Specifies to activate or deactivate the event handling.
   Creates a FIFO on activation that includes a number of elements that is
   defined by \ref DSL_EVENT_FIFO_ELEMENT_COUNT (number of max. stored events).
   Each elements size is defined by size of union DSL_EventData_Union_t.
   \attention After activation of events for the current instance, the initial
              state for all following events is masked (not signaled)!
              To unmask all events that are intended to be handled by opening
              instance please use the following functions for status events
              - \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET
              and for indication events
              - \ref DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_SET
              - \ref DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_SET
   \attention Indication events for Line, Channel- and Datapath Threshold
              Crossing as well as Channel Datarate Shift Threshold Crossing can
              be controled by setting the appropriate theshold value. */
   DSL_boolean_t bEventActivation;
   /**
   Specifies the resource activation mask. */
   DSL_BF_ResourceActivationType_t nResourceActivationMask;
} DSL_InstanceControlData_t;

/**
   Structure that is used to configure instance specific event and resource
   management settings.
   This structure has to be used for ioctl \ref DSL_FIO_INSTANCE_CONTROL_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status event mask data */
   DSL_IN DSL_InstanceControlData_t data;
} DSL_InstanceControl_t;

/**
   Structure that is used to get information about current configuration of
   instance specific event and resource management settings.
   This structure has to be used for ioctl \ref DSL_FIO_INSTANCE_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status event mask data */
   DSL_OUT DSL_InstanceControlData_t data;
} DSL_InstanceStatus_t;

/**
   Structure that is used to inform the upper layer software about an event
   that has been occurred.
*/
typedef struct
{
   /**
   Specifies for which (bearer) channel the function will apply.
   This parameter is used for channel specific events only. If the parameter is
   not used it is set to \ref DSL_CHANNEL_NA . */
   DSL_OUT DSL_uint8_t nChannel;
   /**
   Specifies for which direction (upstream/downstream) the function will
   apply.
   This parameter is only used for direction specific call backs. If the
   parameter is not used it is set to \ref DSL_ACCESSDIR_NA . */
   DSL_OUT DSL_AccessDir_t nAccessDir;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply.
   If the parameter is not used it is set to \ref DSL_XTUDIR_NA . */
   DSL_OUT DSL_XTUDir_t nXtuDir;
   /**
   Specifies the event type (reason of event). Regrading to the setting of this
   value the data which is included in the following union has different
   meanings. Please refer to the description of the union to get information
   about the meaning of the included data.
   For some event (InfoEvents) there is only this event type set. The union
   does not include any data in this case because the upper layer software is
   responsible to read the according ioctl to retrieve according data if
   necessary. */
   DSL_OUT DSL_EventType_t nEventType;
   /**
   Data according to \ref DSL_EventData_Union_t */
   DSL_OUT DSL_EventData_Union_t *pData;
} DSL_EventStatusData_t;

/**
   Structure that is used to inform the upper layer software about an event
   that has been occurred.
   This structure has to be used for ioctl \ref DSL_FIO_EVENT_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status event mask data */
   DSL_OUT DSL_EventStatusData_t data;
} DSL_EventStatus_t;


/**
   Structure that is used to read or write configuration for status event
   handling events. Events might be masked (not signaled) or unmasked
   (signaled).
*/
typedef struct
{
   /**
   Specifies the status events that should be handled. The value shall only be
   in the range of DSL_EVENT_S_FIRST...DSL_EVENT_S_LAST because this structure
   is used to handle status events only. */
   DSL_IN DSL_EventType_t nEventType;
   /**
   Specifies or returns the state of the event according to access type
   (get/set).
   - DSL_TRUE : Event masked (not signaled)
   - DSL_FALSE: Event unmasked (signled) */
   DSL_CFG DSL_boolean_t bMask;
} DSL_EventStatusMaskData_t;

/**
   Structure that is used to read (GET) or write (SET) configuration for status
   event handling events. Events might be masked (not signaled) or unmasked
   (signaled).
   This structure has to be used for ioctl
   - \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET
   - \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status event mask data */
   DSL_CFG DSL_EventStatusMaskData_t data;
} DSL_EventStatusMask_t;

/** @} DRV_DSL_CPE_EVENT */

/**
   Defines all possible Annex types as stored in the firmware
*/
typedef enum
{
   DSL_FW_ANNEX_NA = -1,
   DSL_FW_ANNEX_UNKNOWN = 0,
   DSL_FW_ANNEX_A = 1,
   DSL_FW_ANNEX_B = 2
} DSL_FirmwareAnnex_t;

/** \addtogroup DRV_DSL_CPE_INIT
 @{ */

/**
   Device specific initialization data.
*/
typedef struct DSL_DeviceConfig DSL_DeviceConfig_t;

#if defined (INCLUDE_DSL_CPE_API_DANUBE) || defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
   #include "drv_dsl_cpe_danube_ctx.h"
#endif

#ifdef INCLUDE_DSL_CPE_API_VINAX
   #include "drv_dsl_cpe_api_sar.h"
   #include "drv_dsl_cpe_vinax_ctx.h"
#endif

/**
   Structure for Device Low level configuration.
   \if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   This structure has to be used for ioctl
   - \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_SET
   - \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_GET
   \endif
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains low level configuration data */
   DSL_DeviceLowLevelConfig_t data;
} DSL_LowLevelConfiguration_t;

/**
   Structure for firmware download handling of DSL CPE_API Driver.

   It is possible to do the firmware download by using chunks of a defined size.
   In this case...
   - the data includes only a chunk part of the complete firmware binary
   - the "load firmware" functionality has to be called multiple times
   - The offset has to be used for consecutive function calls

   \attention For a detailed description of the parameters that specifies the
              firmware binaries please refer to the structure \ref DSL_Init_t
              which includes parameters that have the same meaning.
*/
typedef struct
{
   /**
   Pointer to firmware binary. */
   DSL_IN DSL_uint8_t *pFirmware;
   /**
   Size of firmware binary.
   This value is only valid if pFirmware pointer is NOT equal DSL_NULL */
   DSL_IN DSL_uint32_t nFirmwareSize;
   /**
   Pointer to 2nd firmware binary. */
   DSL_IN DSL_uint8_t *pFirmware2;
   /**
   Size of the 2nd firmware binary.
   This value is only valid if pFirmware2 pointer is NOT equal DSL_NULL*/
   DSL_IN DSL_uint32_t nFirmwareSize2;
   /**
   Offset within complete firmware binary.
   This values has to be used in case of using chunks for firmware download.
   In this case it has to be zero for the first call and
   (chunk size * index number of call) for consecutive calls. */
   DSL_IN DSL_uint32_t nFirmwareOffset;
   /**
   Last firmware chunk flag.
   This value should be always true in case of download whole firmware binary.
   In case of using chunks it should be true for last download call */
   DSL_IN DSL_boolean_t bLastChunk;
} DSL_AutobootLoadFirmwareData_t;

/**
   Structure for firmware download handling of DSL CPE_API Driver.
   This structure has to be used for ioctl \ref DSL_FIO_AUTOBOOT_LOAD_FIRMWARE
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains firmware data */
   DSL_IN DSL_AutobootLoadFirmwareData_t data;
} DSL_AutobootLoadFirmware_t;


/**
   Defines the control functionalities of the autoboot handling
*/
typedef enum
{
   /**
   Stops the autoboot handling. */
   DSL_AUTOBOOT_CTRL_STOP = 0,
   /**
   Starts the autoboot handling. */
   DSL_AUTOBOOT_CTRL_START = 1,
   /**
   Restarts the firmware and the DSL CPE API for usage of updated configurations. */
   DSL_AUTOBOOT_CTRL_RESTART = 2,
   /* Continues at any wait state of the autoboot handling.
      \note This control trigger is only used if autoboot status is
      DSL_AUTOBOOT_STATUS_xxx_WAIT
      therwise it will be ignored and a special 
      warning code DSL_WRN_NOT_ALLOWED_IN_CURRENT_STATE will be returned.
      \note Currently there is only one wait on link activation used.*/
   DSL_AUTOBOOT_CTRL_CONTINUE = 3
} DSL_AutobootCtrlSet_t;

/**
   Structure used for controlling the autoboot functionality.
   \remark This handling is necessary in any case if not supported by firmware
           automatically.
*/
typedef struct
{
   /**
   Command for autoboot handling. */
   DSL_IN DSL_AutobootCtrlSet_t nCommand;
} DSL_AutobootControlData_t;

/**
   Structure used for controlling the autoboot functionality.
   This structure has to be used for ioctl \ref DSL_FIO_INIT
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains autoboot control data */
   DSL_IN DSL_AutobootControlData_t data;
} DSL_AutobootControl_t;

/**
   Defines the control functionalities of the autoboot handling
*/
typedef enum
{
   /**
   Autoboot handling is currently stopped. */
   DSL_AUTOBOOT_STATUS_STOPPED = 0,
   /**
   Autoboot handling is currently started. */
   DSL_AUTOBOOT_STATUS_STARTING = 1,
   /**
   Autobooot handling is currently running. */
   DSL_AUTOBOOT_STATUS_RUNNING = 2,
   /**
   Autobooot handling is currently waiting for the FW. */
   DSL_AUTOBOOT_STATUS_FW_WAIT = 3,
   /**
   Autobooot handling is currently waiting for writing the configurations */
   DSL_AUTOBOOT_STATUS_CONFIG_WRITE_WAIT = 4,
   /**
   Autobooot handling is currently waiting for the link activation */
   DSL_AUTOBOOT_STATUS_LINK_ACTIVATE_WAIT = 5

   /* Might be extended if necessary */
} DSL_AutobootStatGet_t;

/**
   Structure used for controlling the autoboot functionality.
   \remark This handling is necessary in any case if not supported by firmware
           automatically.
*/
typedef struct
{
   /**
   Command for autoboot handling. */
   DSL_IN_OUT DSL_AutobootStatGet_t nStatus;
   /**
   This value is only valid in case of nStatus is equal to
   \ref DSL_AUTOBOOT_STATUS_FW_WAIT. In this case it specifies the desired
   mode for the next startup of autoboot handling.
   Otherwise the value shall be always equal to \ref DSL_FW_REQUEST_NA */
   DSL_IN_OUT DSL_FirmwareRequestType_t nFirmwareRequestType;
} DSL_AutobootStatusData_t;

/**
   Structure used for controlling the autoboot functionality.
   This structure has to be used for ioctl \ref DSL_FIO_INIT
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains autoboot control data */
   DSL_IN_OUT DSL_AutobootStatusData_t data;
} DSL_AutobootStatus_t;

/**
   Structure used for controlling of special features within the autoboot
   functionality.
*/
typedef struct
{
   /**
   Activates a special handling that uses an additional state within autoboot
   handling to wait after firmware download and BEFORE basic configuration and
   line/link activation.
   This handling is intended to be used for extended autoboot script handling
   and makes it possible to write additional configuration by using DSL CPE API
   ioctls that updates the internal database within DSL CPE API that will be
   directly used for current activation after continuing.
   If this option is activated (DSL_TRUE) the autoboot waits after firmware
   download and BEFORE basic configuration (autoboot state
   \ref DSL_AUTOBOOT_STATUS_CONFIG_WRITE_WAIT) and has to be resumed using
   autoboot control \ref DSL_AUTOBOOT_CTRL_CONTINUE. 
   In case of working on event basis the line state 0x100
   (DSL_LINESTATE_IDLE) can be used to identify the "config write wait"          <=== Add comment for detection of autoboot status!!!
   autoboot state.
   \note Because of the possibility of multiple usage for "idle" line state
         (signaling) for different wait points within autoboot handling the
         reason for the "idle" line state indication should be requested by
         using  */
   DSL_IN_OUT DSL_boolean_t bWaitBeforeConfigWrite;
   /**
   Activates a special handling that uses an additional state within autoboot
   handling to wait AFTER firmware download and basic configuration (does not
   activate the line/link)  automatically).
   This handling is intended to be used for extended autoboot script handling
   and makes it possible to write additional device configuration by using
   tunnel messages (ioctl \ref DSL_FIO_DBG_DEVICE_MESSAGE_SEND) before the
   line/link activation.
   If this option is activated (DSL_TRUE) the autoboot waits AFTER firmware
   download and basic configuration (autoboot state
   \ref DSL_AUTOBOOT_STATUS_LINK_ACTIVATE_WAIT) and has to be resumed using
   autoboot control \ref DSL_AUTOBOOT_CTRL_CONTINUE. 
   In case of working on event basis the line state 0x100
   (DSL_LINESTATE_IDLE) can be used to identify the "link activate wait"
   autoboot state. */
   DSL_IN_OUT DSL_boolean_t bWaitBeforeLinkActivation;
} DSL_AutobootStateMachineOptions_t;

/**
   Structure for configuration of autoboot state machine behavior.
*/
typedef struct
{
   /**
      Special options to configure the behavior of the autoboot state
      machine handling. */
   DSL_IN_OUT DSL_AutobootStateMachineOptions_t nStateMachineOptions;
} DSL_AutobootConfigData_t;

/**
   Structure for configuration of autoboot state machine behavior.
   This structure has to be used for ioctl
   - \ref DSL_FIO_AUTOBOOT_CONFIG_GET
   - \ref DSL_FIO_AUTOBOOT_CONFIG_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains autoboot configuration data */
   DSL_IN_OUT DSL_AutobootConfigData_t data;
} DSL_AutobootConfig_t;

/**
   Structure for initialization of DSL CPE_API Driver.

   \note It is possible to set the firmware pointer to DSL_NULL. In this case
         the following procedure has to be used separately
         - the firmware has to be downloaded later by using ioctl
           \ref DSL_FIO_AUTOBOOT_LOAD_FIRMWARE
         - the autoboot handling has to be started afterwards by using ioctl
           \ref DSL_FIO_AUTOBOOT_CONTROL_SET
         The advantage of this procedure is that the firmware can be downloaded
         using chunks of a defined size by using "load firmware" functionality
         multiple times. This reduces the amount of memory that has to be
         allocated for firmware download functionality.
*/
typedef struct
{
   /**
   Pointer to firmware binary.
   According to the device for which the DSL CPE API driver has been compiled
   this pointer is defined as follows
   - Danube: The firmware binary that will be provided with this pointer is the
     one and only firmware that will be used
   - Vinax: The firmware binary that will be provided has to have VDSL
     functionality included if DSL CPE API driver shall support VDSL
     transmission modes. */
   DSL_IN DSL_uint8_t *pFirmware;
   /**
   Size of firmware binary.
   This value is only valid if pFirmware pointer is NOT equal DSL_NULL */
   DSL_IN DSL_uint32_t nFirmwareSize;
   /**
   Pointer to 2nd firmware binary.
   According to the device for which the DSL CPE API driver has been compiled
   this pointer is defined as follows
   - Danube: This parameter is NOT used and has to be set to DSL_NULL
   - Vinax: The firmware binary that will be provided has to have ADSL
     functionality included if DSL CPE API driver shall support ADSL
     transmission modes.
   \note For future versions of Vinax firmware which includes both VDSL and
         ADSL functionality (as "real" standalone functionality) this parameter
         will be not necessary anymore and can be set to DSL_NULL. */
   DSL_IN DSL_uint8_t *pFirmware2;
   /**
   Size of the 2nd firmware binary.
   This value is only valid if pFirmware2 pointer is NOT equal DSL_NULL*/
   DSL_IN DSL_uint32_t nFirmwareSize2;
   /**
   Defines the transmission modes that shall be activated within autoboot
   handling after firmware download */
   DSL_IN DSL_G997_XTUSystemEnablingData_t nXtseCfg;
   /**
   Defines the near end line inventory information that will be programmed
   within initialization procedure between firmware download and start of
   autoboot handling */
   DSL_IN DSL_G997_LineInventoryNeData_t *pInventory;
   /**
   Defines the startup mode of the autoboot handling.
   In special cases it might be necessary that the autoboot handling will be
   not started automatically. In this case the value DSL_AUTOBOOT_CTRL_STOP (0)
   has to be used and the upper layer application is responsible to start the
   autoboot handling later on.
   All other values are starting the autoboot handling automatically */
   DSL_IN DSL_AutobootCtrlSet_t nAutobootStartupMode;
   DSL_IN DSL_AutobootConfigData_t nAutobootConfig;
   /** Testmode Control*/
   DSL_IN DSL_TestModeControlSet_t nTestModeControl;
   /** Line Activation Configuration options*/
   DSL_IN DSL_G997_LineActivateData_t nLineActivateConfig;
   /**
   Defines device configuration parameters*/
   DSL_IN DSL_DeviceConfig_t nDeviceCfg;
   /**
   \todo Event handling needs special implementation for usage within ioctl
         interface (wakeup queue) and in case of using direct function calls
         within kernel space (callback registration).
         For the first implementation this feature might be not necessary */
   /*const DSL_Event_Callback_t pEventCallback,*/
} DSL_InitData_t;

/**
   Structure for initialization of DSL CPE_API Driver.
   This structure has to be used for ioctl \ref DSL_FIO_INIT
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains initialization data */
   DSL_IN DSL_InitData_t data;
} DSL_Init_t;

/** @} DRV_DSL_CPE_INIT */


/** \addtogroup DRV_DSL_CPE_DEBUG
 @{ */

/**
   Structure used to read (GET) or write (SET) the configuration of a debug
   module level. This is used to activate/deactivate dedicated debug printout or
   get the current configuration status.
*/
typedef struct
{
   /**
   Specifies for which debug module the level should be get/set */
   DSL_IN DSL_debugModules_t nDbgModule;
   /**
   Specifies/returns the debug level. This value has to be one of the defined
   debug levels (\ref DSL_debugLevels_t).*/
   DSL_CFG DSL_debugLevels_t nDbgLevel;
} DSL_DBG_ModuleLevelData_t;

/**
   Structure used to read (GET) or write (SET) the configuration of a debug
   module level.
   This structure has to be used for ioctl
   - \ref DSL_FIO_DBG_MODULE_LEVEL_SET
   - \ref DSL_FIO_DBG_MODULE_LEVEL_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains debug configuration data */
   DSL_CFG DSL_DBG_ModuleLevelData_t data;
} DSL_DBG_ModuleLevel_t;

/**
   Structure used for sending plain byte data and low level device messages
*/
typedef struct
{
   /**
   Message data */
   DSL_IN_OUT DSL_uint8_t *pMsg;
   /**
   Size of message included in pMsg. The size of the message has to be given
   in bytes! */
   DSL_IN DSL_uint16_t nSizeTx;
   /**
   Maximal Size of pMsg buffer. This value will be checked if a possible answer
   to the command can be stored in the buffer. */
   DSL_IN_OUT DSL_uint16_t nSizeRx;
} DSL_DeviceMessageData_t;

/**
   Structure used for sending plain byte data and low level device messages
   This structure has to be used for ioctl
   - \ref DSL_FIO_DBG_DEVICE_MESSAGE_SEND
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains device message data */
   DSL_IN_OUT DSL_DeviceMessageData_t data;
} DSL_DeviceMessage_t;

#ifdef DSL_INTERN
   /* lower level device interface*/
   #include "drv_dsl_cpe_device.h"

   #if defined(INCLUDE_DSL_PM)
      #include "drv_dsl_cpe_pm_core.h"
   #endif
#endif


/** @} DRV_DSL_CPE_DEBUG */

/** \addtogroup DRV_DSL_DEVICE
 @{ */

/** @} DRV_DSL_DEVICE */


/** \addtogroup DRV_DSL_API
 @{ */

/** @} DRV_DSL_API */

#if defined(SWIG) || defined(DSL_INTERN)
#include "drv_dsl_cpe_intern.h"
#include "drv_dsl_cpe_intern_g997.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_API_H */
