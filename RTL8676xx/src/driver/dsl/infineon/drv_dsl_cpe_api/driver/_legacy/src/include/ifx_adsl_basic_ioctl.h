#ifndef _IFX_ADSL_APP_H
#define _IFX_ADSL_APP_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

/** \file
   This file defines the common ioctl interface for DSL CPE API
*/

/** \addtogroup DRV_DSL_CPE_ERROR_CODES
 @{ */

/** error code definitions */
typedef enum mei_error
{
   MEI_SUCCESS = 0,
   MEI_FAILURE = -1,
   MEI_MAILBOX_FULL = -2,
   MEI_MAILBOX_EMPTY = -3,
   MEI_MAILBOX_TIMEOUT = -4,
} MEI_ERROR;

/** @} DRV_DSL_CPE_ERROR_CODES */

/** \addtogroup DRV_DSL_CPE_INIT
 @{ */

/**
   This enumeration type contains the possible ADSL mode selections. */
typedef enum {
   /**
      Depending on firmware-image:
       - Annex A
         - ANSI T.1413,
         - G.992.1 Annex A (ADSL)
         - G.992.3 Annex A (ADSL2)
         - G.992.3 Annex L (ADSL2)
         - G.992.5 Annex A (ADSL2+)
       - Annex B
         - G.992.1 Annex B (ADSL)
         - G.992.3 Annex B (ADSL2)
         - G.992.5 Annex B (ADSL2+) */
   AUTOBOOT_DSL_MODE_ALL,
   /**
      - G.992.1 Annex A (ADSL) */
   AUTOBOOT_DSL_MODE_992_1A,
   /**
      - G.992.1 Annex B (ADSL) */
   AUTOBOOT_DSL_MODE_992_1B,
   /**
      - G.992.3 Annex A (ADSL2) */
   AUTOBOOT_DSL_MODE_992_3A,
   /**
      - G.992.3 Annex B (ADSL2) */
   AUTOBOOT_DSL_MODE_992_3B,
   /**
      - G.992.3 Annex A (ADSL2)
      - G.992.5 Annex A (ADSL2+) */
   AUTOBOOT_DSL_MODE_992_5A,
   /**
      - G.992.3 Annex B (ADSL2)
      - G.992.5 Annex B (ADSL2+) */
   AUTOBOOT_DSL_MODE_992_5B,
   /**
      - G.992.3 Annex I (ADSL2) */
   AUTOBOOT_DSL_MODE_992_3I,
   /**
      - G.992.3 Annex J(ADSL2) */
   AUTOBOOT_DSL_MODE_992_3J,
   /**
      - G.992.3 Annex M (ADSL2) */
   AUTOBOOT_DSL_MODE_992_3M,
   /**
      - G.992.5 Annex I (ADSL2+) */
   AUTOBOOT_DSL_MODE_992_5I,
   /**
      - G.992.5 Annex J (ADSL2+) */
   AUTOBOOT_DSL_MODE_992_5J,
   /**
      - G.992.5 Annex M (ADSL2+) */
   AUTOBOOT_DSL_MODE_992_5M,
   /**
      All Annex M modes:
      - G.992.3 Annex M (ADSL2)
      - G.992.5 Annex M (ADSL2+) */
   AUTOBOOT_DSL_MODE_M_ALL,
   /**
      All Annex B modes:
      - G.992.1 Annex B (ADSL)
      - G.992.3 Annex B (ADSL2)
      - G.992.5 Annex B (ADSL2+) */
   AUTOBOOT_DSL_MODE_B_ALL,
   /**
      All Annex B & M modes:
      - G.992.1 Annex B (ADSL)
      - G.992.3 Annex B (ADSL2)
      - G.992.3 Annex M (ADSL2)
      - G.992.5 Annex B (ADSL2+)
      - G.992.5 Annex M (ADSL2+) */
   AUTOBOOT_DSL_MODE_M_B_ALL
} autoboot_adsl_mode_t;

/** Defines the maximum number of DSL modes */
#define AUTOBOOT_DSL_MODE_MAX (AUTOBOOT_DSL_MODE_M_B_ALL + 1)

/** List of line states for autoboot handling */
typedef enum {
   /** Init state. Line is in initialization phase. */
   AUTOBOOT_INIT,
   /** Training state. Line is in handshake/training phase. */
   AUTOBOOT_TRAIN,
   /** Showtime state. Line is in showtime, data trafic possible. */
   AUTOBOOT_SHOWTIME,
   /** Diagnostic state. Line is in diagnostic mode. */
   AUTOBOOT_DIAGNOSTIC,
   /** Restart state. Line is going to be restarted. (This is an intermediate state!) */
   AUTOBOOT_RESTART
} autoboot_line_states_t;

/** @} DRV_DSL_CPE_INIT */


/** \addtogroup DRV_DSL_DEBUG
 @{ */

/**
   Structure is used for debug access only.
   Refer to configure option INCLUDE_ADSL_WINHOST_DEBUG */
typedef struct struct_meireg{
   /*
      Specifies that address for debug access */
   unsigned long iAddress;
   /*
      Specifies the pointer to the data that has to be written or returns a
      pointer to the data that has been read out*/
   unsigned long iData;
}meireg;

/** Defines the buffer size for register debug access */
#define MEIDEBUG_BUFFER_SIZES 512
/**
   Structure used for debug access only. */
typedef struct struct_meidebug{
   /*
      Specifies that address for debug access */
   unsigned long iAddress;
   /*
      Specifies the pointer to the data that has to be written or returns a
      pointer to the data that has been read out*/
   unsigned long iCount;
   /*
      Specifies a buffer to which the data is copied in case od read access or
      to which the data has to be written before access to write the values */
   unsigned long buffer[MEIDEBUG_BUFFER_SIZES];
}meidebug;

/** @} DRV_DSL_DEBUG */


/** \addtogroup DRV_DSL_CPE_API
 @{ */

/**
   Defines the base number for ioctls that are related to the ADSL MIB (SNMP)
   RFC2662 and RFC3440 */
#define IFX_ADSL_IOC_MIB_BASE           0
/**
   Defines the base number for common ioctls */
#define IFX_ADSL_IOC_BASIC_BASE      3000
/**
   Defines the base number for ioctls that are related to Clear EOC handling */
#define IFX_ADSL_IOC_CEOC_BASE       4000
/**
   Defines the base number for ioctls that are related to autoboot handling */
#define IFX_ADSL_IOC_AUTOBOOT_BASE   5000
/**
   Defines the base number for ioctls that are related to BSP related hanling */
#define IFX_ADSL_IOC_BSP_BASE	     6000
/**
   Defines the max. number of ioctl */
#define IFX_ADSL_IOC_END            10000

/** @} DRV_DSL_CPE_API */

/**
   Starts the modem after firmware download. Performs an internal low level
   driver function in the following sequence:
   - IFX_ADSL_IOC_RESET
   - IFX_ADSL_IOC_RUN

   \param none

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   If autoboot handling is included within driver build the functionality shall
   be not used because it is handled automatically. Please refer to\n
     Configure option: --enable-adsl-autoboot\n
     Used internal define: INCLUDE_ADSL_AUTOBOOT

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_START);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define IFX_ADSL_IOC_START          ( 0 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Runs the arc.

   \param none

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   If autoboot handling is included within driver build the functionality shall
   be not used because it is handled automatically. Please refer to\n
     Configure option: --enable-adsl-autoboot\n
     Used internal define: INCLUDE_ADSL_AUTOBOOT

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_RUN);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define IFX_ADSL_IOC_RUN            ( 1 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Resets the arc.
   Implementation is similar to ioctl \ref IFX_ADSL_IOC_REBOOT.

   \param none

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   If autoboot handling is included within driver build the functionality shall
   be not used because it is handled automatically. Please refer to\n
     Configure option: --enable-adsl-autoboot\n
     Used internal define: INCLUDE_ADSL_AUTOBOOT

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_RESET);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define IFX_ADSL_IOC_RESET          ( 3 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Reboot the arc.
   Implementation is similar to ioctl \ref IFX_ADSL_IOC_RUN.

   \param none

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   If autoboot handling is included within driver build the functionality shall
   be not used because it is handled automatically. Please refer to\n
     Configure option: --enable-adsl-autoboot\n
     Used internal define: INCLUDE_ADSL_AUTOBOOT

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_REBOOT);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define IFX_ADSL_IOC_REBOOT         ( 4 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Halts the arc.
   The following procedure will be done
   - Resets arc
   - Halts arc

   \param none

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   If autoboot handling is included within driver build the functionality shall
   be not used because it is handled automatically. Please refer to\n
     Configure option: --enable-adsl-autoboot\n
     Used internal define: INCLUDE_ADSL_AUTOBOOT

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_HALT);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define IFX_ADSL_IOC_HALT           ( 5 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Sends a winhost message directly to the low level driver.
   \attention This is debug functionality only! Do nit use it for normal
              operation because this might influence the handling that has been
              implemented within the other ioctls.
   \attention It is strictly recommended to use the low level MEI BSP Driver
              interface directly to operate on CMVS.

   \param char* TxMessage
      Defines the CMV that shall be send to the firmware.

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   This ioctl is only available in case of including the winhost debug
   functionality within driver build. Please refer to\n
     Configure option: --enable-adsl-winhost-debug\n
     Used internal define: INCLUDE_ADSL_WINHOST_DEBUG

   \code
      char[] TxMessage = { 0xXXXX, 0xXXXX, ... };
      ret = ioctl(fd, IFX_ADSL_IOC_CMV_WINHOST, &TxMessage[0]);
   \endcode

   \ingroup DRV_DSL_DEBUG */
#define IFX_ADSL_IOC_CMV_WINHOST    (6 + IFX_ADSL_IOC_BASIC_BASE)

/**
   This function performs a debug read access.

   \param meireg* This parameter points to a \ref meireg structure

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   This ioctl is only available in case of including the winhost debug
   functionality within driver build. Please refer to\n
     Configure option: --enable-adsl-winhost-debug\n
     Used internal define: INCLUDE_ADSL_WINHOST_DEBUG

   \code
      meireg nReg = { 0 };
      nReg.iAddress = 0xX;
      ret = ioctl(fd, IFX_ADSL_IOC_CMV_READ, &nReg);
      // process returned value
      nValue = nReg.iData;
   \endcode

   \ingroup DRV_DSL_DEBUG */
#define IFX_ADSL_IOC_CMV_READ       (7 + IFX_ADSL_IOC_BASIC_BASE)

/**
   This function performs a debug write access.

   \param meireg* This parameter points to a \ref meireg structure

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   This ioctl is only available in case of including the winhost debug
   functionality within driver build. Please refer to\n
     Configure option: --enable-adsl-winhost-debug\n
     Used internal define: INCLUDE_ADSL_WINHOST_DEBUG

   \code
      meireg nReg = { 0 };
      nReg.iAddress = 0xX;
      nReg.iData = 0xX;
      ret = ioctl(fd, IFX_ADSL_IOC_CMV_WRITE, &nReg);
   \endcode

   \ingroup DRV_DSL_DEBUG */
#define IFX_ADSL_IOC_CMV_WRITE      (8 + IFX_ADSL_IOC_BASIC_BASE)

/**
   This function return the base address of the Danube device.

   \param nBaseAddress* This parameter returns the base address

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   This ioctl is only available in case of including the winhost debug
   functionality within driver build. Please refer to\n
     Configure option: --enable-adsl-winhost-debug\n
     Used internal define: INCLUDE_ADSL_WINHOST_DEBUG

   \code
      unsigned long nBaseAddress = 0;
      ret = ioctl(fd, IFX_ADSL_IOC_GET_BASE_ADDRESS, &nBaseAddress)
   \endcode

   \ingroup DRV_DSL_DEBUG */
#define IFX_ADSL_IOC_GET_BASE_ADDRESS ( 9 + IFX_ADSL_IOC_BASIC_BASE)

/**
   When the link is up and has entered showtime, this command informs the low
   level MEI BSP Driver that showtime is reached.

   \param none

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   If autoboot handling is included within driver build the functionality shall
   be not used because it is handled automatically. Please refer to\n
     Configure option: --enable-adsl-autoboot\n
     Used internal define: INCLUDE_ADSL_AUTOBOOT

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_SHOWTIME)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define IFX_ADSL_IOC_SHOWTIME          (10 + IFX_ADSL_IOC_BASIC_BASE)

/**
   This function enables or disables the MIPS interrupts of the DFE.

   \param bEnable*
      If set to '0' interrupts are disabled, if set to '1' interrupts are
      enabled

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   This ioctl is only available in case of including the winhost debug
   functionality within driver build. Please refer to\n
     Configure option: --enable-adsl-winhost-debug\n
     Used internal define: INCLUDE_ADSL_WINHOST_DEBUG

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_REMOTE, 0)
   \endcode

   \ingroup DRV_DSL_DEBUG */
#define IFX_ADSL_IOC_REMOTE            (11 + IFX_ADSL_IOC_BASIC_BASE)

/**
   ARC Debug Memory Access for reading.
   This function is used for direct, address-based access to ARC memory.

   \param meidebug* This parameter points to a \ref meidebug structure

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   This ioctl is only available in case of including the winhost debug
   functionality within driver build. Please refer to\n
     Configure option: --enable-adsl-winhost-debug\n
     Used internal define: INCLUDE_ADSL_WINHOST_DEBUG

   \code
      meidebug nData = { 0 };
      nData.iAddress = 0xX;
      nData.iCount = 3;
      ret = ioctl(fd, IFX_ADSL_IOC_READDEBUG, &nData);
      // process returned values
      nValue[x] = nData.buffer[x];
   \endcode

   \ingroup DRV_DSL_DEBUG */
#define IFX_ADSL_IOC_READDEBUG         (12 + IFX_ADSL_IOC_BASIC_BASE)

/**
   ARC Debug Memory Access for writing.
   This function is used for direct, address-based access to ARC memory.

   \param meidebug* This parameter points to a \ref meidebug structure

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   This ioctl is only available in case of including the winhost debug
   functionality within driver build. Please refer to\n
     Configure option: --enable-adsl-winhost-debug\n
     Used internal define: INCLUDE_ADSL_WINHOST_DEBUG

   The maximum number of data within buffer is defined by
   \ref MEIDEBUG_BUFFER_SIZES

   \code
      meidebug nData = { 0 };
      nData.iAddress = 0xX;
      nData.iCount = 3;
      nData.buffer[x] = 0xX;
      ret = ioctl(fd, IFX_ADSL_IOC_WRITEDEBUG, &nData);
   \endcode

   \ingroup DRV_DSL_DEBUG */
#define IFX_ADSL_IOC_WRITEDEBUG        (13 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Download boot code to ARC.
   This handling will be done in context of \ref IFX_ADSL_IOC_RUN automatically.

   \param none

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   If autoboot handling is included within driver build the functionality shall
   be not used because it is handled automatically. Please refer to\n
     Configure option: --enable-adsl-autoboot\n
     Used internal define: INCLUDE_ADSL_AUTOBOOT

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_DOWNLOAD);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define IFX_ADSL_IOC_DOWNLOAD          (15 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Enable Jtag debugger interface.
   This function setups mips gpio to enable jtag debugger.

   \param none

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_JTAG_ENABLE);
   \endcode

   \ingroup DRV_DSL_DEBUG */
#define IFX_ADSL_IOC_JTAG_ENABLE       (16 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Enables or disables the loop diagnostic mode (DELT).
   In case of former state differs from new one a restart of the autoboot will
   be done automatically.

   \param bEnable
      Select '0' to disable and '1' to enable loop diagnostic mode

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none.

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_SET_LOOP_DIAGNOSTICS_MODE, 1);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define IFX_ADSL_IOC_SET_LOOP_DIAGNOSTICS_MODE  (19 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Reads the current status of loop diagnostic mode (DELT).

   \param bIsEnabled*
      According to the returned value the status of DELT is as follows
      - '0' indicate that DELT is currently disabled
      - '1' indicate that DELT is currently enabled

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none.

   \code
      unsigned long bIsEnabled = 0;
      ret = ioctl(fd, IFX_ADSL_IOC_GET_LOOP_DIAGNOSTICS_MODE, &bIsEnabled);
      // process returned value
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define IFX_ADSL_IOC_GET_LOOP_DIAGNOSTICS_MODE  (20 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Forces the loop diagnostic mode to be completed even without any result.

   \param none

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_LOOP_DIAGNOSTIC_MODE_COMPLETE);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define IFX_ADSL_IOC_LOOP_DIAGNOSTIC_MODE_COMPLETE     (21 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Checks if there is currently a diagnostic mode running.

   \param bIsCompleted*
      According to the returned value the status of DELT is as follows
      - '0' indicate that DELT is still runnning
      - '1' indicate that DELT is completed

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none

   \code
      unsigned long bIsCompleted = 0;
      ret = ioctl(fd, IFX_ADSL_IOC_IS_LOOP_DIAGNOSTICS_MODE_COMPLETE, &bIsCompleted);
      // process returned value
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define IFX_ADSL_IOC_IS_LOOP_DIAGNOSTICS_MODE_COMPLETE (22 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Get current Power Management Mode Status.

   \param DSL_PowerMode_t*
      According to the returned value the Power Management Status is as defined
      within \ref DSL_PowerMode_t

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none

   \code
      DSL_PowerMode_t nMode = 0;
      ret = ioctl(fd, IFX_ADSL_IOC_GET_POWER_MANAGEMENT_MODE, &nMode);
      // process returned value
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define IFX_ADSL_IOC_GET_POWER_MANAGEMENT_MODE (23 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Enables or disables the L3 Power Mode.

   \param bEnable*
      According to the defined value the processing for L3 Power Mode is as
      follows:
      - '0' disable L3 Power Mode
      - '1' enable L2 Power Mode

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_SET_L3_POWER_MODE, 1);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define IFX_ADSL_IOC_SET_L3_POWER_MODE       (24 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Read current dual latency configuration.

   \param bIsEnable*
      According to the defined value the processing for L3 Power Mode is as
      follows:
      - '0' Dual Latency is currently disabled
      - '1' Dual Latency is currently enabled

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none

   \code
      unsigned long bIsEnable = 0;
      ret = ioctl(fd, IFX_ADSL_IOC_GET_DUAL_LATENCY, &bIsEnable);
      // process returned value
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define IFX_ADSL_IOC_GET_DUAL_LATENCY        (25 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Enables or disables the dual latency configuration.

   \param nMode*
      The mode has to be specified according to definition of
      \ref DSL_DualLatency_t

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none

   \code
      ret = ioctl(fd, IFX_ADSL_IOC_SET_DUAL_LATENCY, DUAL_LATENCY_US_DS_ENABLE);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define IFX_ADSL_IOC_SET_DUAL_LATENCY        (26 + IFX_ADSL_IOC_BASIC_BASE)

/**
   Enable/Disable autoboot mode.
   When the autoboot mode is disabled, the driver will excute some cmv
   commands for led control and dual latency when DSL startup.

   \param bIsEnable*
      According to the defined value the ahandling will be done as follows:
      - '0' Disable autoboot
      - '1' Enable autoboot

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
      none

   \code
      ret = ioctl(fd, AUTOBOOT_ENABLE_SET, 1);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define AUTOBOOT_ENABLE_SET                  (27+ IFX_ADSL_IOC_BASIC_BASE)

/** \addtogroup DRV_DSL_CPE_COMMON
 @{ */

/** Definitions of Power Management modes */
typedef enum
{
   /** No link available, unknown mode */
   DSL_POWERMODE_NO_LINK = -1,
   /** Power Mode is L0 */
   DSL_POWERMODE_L0 = 0,
   /** Power Mode is L2 */
   DSL_POWERMODE_L2 = 2,
   /** Power Mode is L3 */
   DSL_POWERMODE_L3 = 3,
   DSL_POWERMODE_LAST
} DSL_PowerMode_t;


/** Definitions of Dual Latency modes */
typedef enum
{
   /** Dual Latency is disabled */
   DUAL_LATENCY_US_DS_DISABLE = 0,
   /** Dual Latency is enabled for upstream */
   DUAL_LATENCY_US_ENABLE = 1,
   /** Dual Latency is enabled for downstream */
   DUAL_LATENCY_DS_ENABLE = 2,
   /** Dual Latency is enabled for upstream and downstream */
   DUAL_LATENCY_US_DS_ENABLE = 3,
   DUAL_LATENCY__LAST
} DSL_DualLatency_t;

/** @} DRV_DSL_CPE_COMMON */


/** \addtogroup DRV_DSL_CPE_INTERN
 @{ */

#define ME_HDLC_IDLE 0
#define ME_HDLC_INVALID_MSG 1
#define ME_HDLC_MSG_QUEUED 2
#define ME_HDLC_MSG_SENT 3
#define ME_HDLC_RESP_RCVD 4
#define ME_HDLC_RESP_TIMEOUT 5
#define ME_HDLC_RX_BUF_OVERFLOW 6
#define ME_HDLC_UNRESOLVED 1
#define ME_HDLC_RESOLVED 2

/** @} DRV_DSL_CPE_INTERN */

/**
   Selecting the accepted ADSL modes in the firmware

   \deprecated
   interim solution to provide old autoboot interface, may change for final SW API

   \param autoboot_adsl_mode_t
   The parameter is a value of the type \ref autoboot_adsl_mode_t

   \retval 0 if successful
   \retval -1 in case of an error

   \remarks
   If the firmware does not support the selected mode, a fallback to "all" is done
   This setting should be done before starting the firmware or another
   restart of the firmware with \ref IFX_ADSL_IOC_START should  be triggered

   \ingroup DRV_DSL_CPE_INIT */
#define AUTOBOOT_ADSL_MODE_SET            (0 + IFX_ADSL_IOC_AUTOBOOT_BASE)

/**
   Possibility for applications to read out the line state
   This is done by mapping the firmware states to the ADSL standard.
   \param autoboot_line_states_t*
      The parameter is a pointer to \ref autoboot_line_states_t value.

   \retval 0 if successful
   \retval -1 in case of an error

   \ingroup DRV_DSL_CPE_INIT */
#define AUTOBOOT_LINE_STATE_GET           (1 + IFX_ADSL_IOC_AUTOBOOT_BASE)

/**
   Give the possibility to enable/disable the autoboot task.

   \remarks This is mainly intended for debugging/testing purpose; therefore the
   task is enabled by default.

   \param int  The parameter has the following values:

   \arg 0: disabled
   \arg 1: enabled

   \retval 0 if successful
   \retval -1 in case of an error

   \ingroup DRV_DSL_CPE_INIT */
#define AUTOBOOT_CONTROL_SET           (2 + IFX_ADSL_IOC_AUTOBOOT_BASE)

/**
   Controls the showtime behavior of the autoboot task in showtime.
   If this setting is enabled, the line stay in showtime, even if no data
   traffic is posssible anymore because of LOS or other error signals.

   \remarks This is mainly intended for maesuring/testing purpose.

   \param int  The parameter has the following values:

   \arg 0: disabled
   \arg 1: enabled

   \retval 0 if successful
   \retval -1 in case of an error

   \ingroup DRV_DSL_CPE_INIT */
#define AUTOBOOT_SHOWTIME_LOCK_SET        (3 + IFX_ADSL_IOC_AUTOBOOT_BASE)


#define IFX_ADSL_IOC_CEOC_SEND            (0 + IFX_ADSL_IOC_CEOC_BASE)

#ifdef MODULE
#include <ifx_adsl_mib_ioctl_old.h>
#else
#include <ifx/dsl/ifx_adsl_mib_ioctl_old.h>
#endif

#ifdef __KERNEL__
int IFX_ADSL_Ioctls(MEI_inode_t * ino, MEI_file_t * fil, unsigned int command, unsigned long lon);
#endif

#endif /* _IFX_ADSL_APP_H */
