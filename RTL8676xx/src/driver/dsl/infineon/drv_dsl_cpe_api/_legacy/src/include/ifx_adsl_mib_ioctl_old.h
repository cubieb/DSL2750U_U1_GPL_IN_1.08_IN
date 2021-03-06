#ifndef _IFX_ADSL_APP_IOCTL_OLD_H
#define _IFX_ADSL_APP_IOCTL_OLD_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

/** \file
   This file only exists for backward compatibility. It includes the definitions
   that maps old ioctl names (before DSL CPE API V1.0.0) to the new ones.
*/

#define DANUBE_MEI_START            IFX_ADSL_IOC_START

#define DANUBE_MEI_RESET            IFX_ADSL_IOC_RESET
#define DANUBE_MEI_REBOOT           IFX_ADSL_IOC_REBOOT
#define DANUBE_MEI_HALT             IFX_ADSL_IOC_HALT
#define DANUBE_MEI_CMV_WINHOST      IFX_ADSL_IOC_CMV_WINHOST
#define DANUBE_MEI_CMV_READ         IFX_ADSL_IOC_CMV_READ
#define DANUBE_MEI_CMV_WRITE        IFX_ADSL_IOC_CMV_WRITE

#define DANUBE_MEI_GET_BASE_ADDRESS IFX_ADSL_IOC_GET_BASE_ADDRESS
/**
   When the link is up and has entered showtime, call this command to inform
   driver that it is showtime now.

   \deprecated
   Not necessary anymore, as this is handled by the autoboot daemon in kernel!
 */
#define DANUBE_MEI_SHOWTIME         IFX_ADSL_IOC_SHOWTIME
#define DANUBE_MEI_REMOTE           IFX_ADSL_IOC_REMOTE
#define DANUBE_MEI_READDEBUG        IFX_ADSL_IOC_READDEBUG
#define DANUBE_MEI_WRITEDEBUG       IFX_ADSL_IOC_WRITEDEBUG
#define DANUBE_MEI_DOWNLOAD         IFX_ADSL_IOC_DOWNLOAD
#define DANUBE_MEI_JTAG_ENABLE      IFX_ADSL_IOC_JTAG_ENABLE
#define DANUBE_MEI_RUN              IFX_ADSL_IOC_RUN

#define SET_ADSL_LOOP_DIAGNOSTICS_MODE IFX_ADSL_IOC_SET_LOOP_DIAGNOSTICS_MODE
#define GET_ADSL_LOOP_DIAGNOSTICS_MODE IFX_ADSL_IOC_GET_LOOP_DIAGNOSTICS_MODE

#define LOOP_DIAGNOSTIC_MODE_COMPLETE  IFX_ADSL_IOC_LOOP_DIAGNOSTIC_MODE_COMPLETE
#define IS_ADSL_LOOP_DIAGNOSTICS_MODE_COMPLETE IFX_ADSL_IOC_IS_LOOP_DIAGNOSTICS_MODE_COMPLETE


#define GET_POWER_MANAGEMENT_MODE   IFX_ADSL_IOC_GET_POWER_MANAGEMENT_MODE
/* Set L3 Power Mode /disable L3 power mode */
#define SET_L3_POWER_MODE           IFX_ADSL_IOC_SET_L3_POWER_MODE

/* get current dual latency configuration */
#define GET_ADSL_DUAL_LATENCY       IFX_ADSL_IOC_GET_DUAL_LATENCY
/* enable/disable dual latency path */
#define SET_ADSL_DUAL_LATENCY       IFX_ADSL_IOC_SET_DUAL_LATENCY

#define DANUBE_MIB_LO_ATUC          IFX_ADSL_MIB_LO_ATUC
#define DANUBE_MIB_LO_ATUR          IFX_ADSL_MIB_LO_ATUR

#endif /* _IFX_ADSL_APP_IOCTL_OLD_H */
