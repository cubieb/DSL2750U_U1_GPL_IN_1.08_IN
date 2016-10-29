#ifndef _IFX_ADSL_BASIC_H
#define _IFX_ADSL_BASIC_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

/** \file
   This file defines the common DSL management interface functionality for
   the DSL CPE API.
*/

/** \defgroup DRV_DSL_CPE_API Infineon DSL Application Programming Interface for CPE
    Lists the entire modules to the DSL CPE API.
  @{ */

/**
   \defgroup DRV_DSL_CPE_INIT Initialization Service
   This module includes the functionality for the initialization of the DSL
   device including firmware download and controlling the autoboot handling.
*/

/**
   \defgroup DRV_DSL_CPE_COMMON Common Functionality for all Modules
   This module includes all DSL library functionality which does not belong to
   any other module.
*/

/**
   \defgroup DRV_DSL_CPE_LED LED module
   This module includes all functionalities related to LED support
*/

/**
   \defgroup DRV_DSL_CPE_CEOC CEOC module
   This module includes all functionalities related to Clear EOC support
   (required by China MII)
*/

/**
   \defgroup DRV_DSL_CPE_MIB Mib module
   This module includes all functionalities related to SNMP MIB in case of using
   RFC2662 (ADSL line MIB) and RFC3440 (ADSL line extension MIB)
*/

/**
   \defgroup DRV_DSL_CPE_WEB WEB related ADSL Statistics
   This module includes all functionalities related to the Web interface
*/

/**
   \defgroup DRV_DSL_CPE_INTERN Intern definitions
   This module includes internal definitions only!
*/

/**
   \defgroup DRV_DSL_CPE_ERROR_CODES Error Codes
   Defines all possible error codes returned by the DSL API library
*/

/**
   \defgroup DRV_DSL_DEBUG Debug related implementation
   This module defines debug related functionality. This shall be not used in
   a productive version of the software. There are different configure options
   which makes it possible to completly exclude this functionality from the
   software (please refer to the according functions).
*/

/** @} */


/** \addtogroup DRV_DSL_CPE_CFG
 @{ */

#define IFX_ADSL_DEV_MAJOR 106

#define MSG_LENGTH         16   /* x16 bits */
#define YES_REPLY          1
#define NO_REPLY           0


#define IFX_POP_EOC_DONE   0
#define IFX_POP_EOC_FAIL   -1

#define OMB_CLEAREOC_INTERRUPT_CODE (0x00020000)

#if defined(__KERNEL__) || defined (IFX_ADSL_PORT_RTEMS)

#ifdef __LINUX__
#ifdef CONFIG_PROC_FS
typedef struct reg_entry
{
   int *flag;
   char name[30];               /* big enough to hold names */
   char description[100];       /* big enough to hold description */
   unsigned short low_ino;
} reg_entry_t;
#endif
#endif /* __LINUX__ */
/*
 * external variables from ifx_adsl_basic.c
 */
extern MEI_mutex_t mei_sema;
extern int showtime;
extern u32 adsl_mode, adsl_mode_extend;
extern int loop_diagnostics_mode;
extern int loop_diagnostics_completed;
extern wait_queue_head_t wait_queue_loop_diagnostic;



/*
** Native size for the Stratiphy interface is 32-bits. All reads and writes
** MUST be aligned on 32-bit boundaries. Trickery must be invoked to read word and/or
** byte data. Read routines are provided. Write routines are probably a bad idea, as the
** Arc has unrestrained, unseen access to the same memory, so a read-modify-write cycle
** could very well have unintended results.
*/
extern MEI_ERROR meiCMV (
   u16 *,
   int,
   u16 *
);                              /* first arg is CMV to ARC, second to indicate
                                   whether need reply */
extern void makeCMV (
   u8 opcode,
   u8 group,
   u16 address,
   u16 index,
   int size,
   u16 * data,
   u16 * CMVMSG
);

extern int IFX_ADSL_ReadHdlc (
   char *hdlc_pkt,
   int max_hdlc_pkt_len
);

extern int IFX_ADSL_SendHdlc (
   unsigned char *hdlc_pkt,
   int hdlc_pkt_len
);

extern int IFX_ADSL_AdslStart (
   void
);

extern int IFX_ADSL_AdslShowtime (
   void
);

extern int IFX_ADSL_ReadAdslMode (
   void
);

extern int IFX_ADSL_AdslReset (
   void
);

extern ifx_adsl_device_t *IFX_ADSL_GetAdslDevice (
   void
);

extern int IFX_ADSL_SendCmv (
   u8 opcode,
   u8 group,
   u16 address,
   u16 index,
   int size,
   u16 * data,
   u16 * CMVMSG
);

extern int IFX_ADSL_IsModemReady (
   ifx_adsl_device_t * pDev
);

extern int ifx_adsl_autoboot_thread_start (
   void
);

extern int ifx_adsl_autoboot_thread_restart (
   void
);

extern int ifx_adsl_autoboot_thread_stop (
   void
);

extern int mei_mib_ioctl (
   MEI_inode_t * ino,
   MEI_file_t * fil,
   unsigned int command,
   unsigned long lon
);

extern int mei_mib_adsl_link_up (
   void
);

extern int mei_mib_adsl_link_down (
   void
);

extern int ifx_adsl_mib_init (
   void
);

extern void ifx_adsl_mib_cleanup (
   void
);

extern int ifx_copy_to_user (
   char *pDst,
   char *pSrc,
   int iSize
);
extern int ifx_copy_from_user (
   char *pDst,
   char *pSrc,
   int iSize
);

/* Common helper functions */
extern MEI_ERROR IFX_TwosComplement16_IntToHex (
   s16 nsVal16,
   int nBits,
   u16 * nuVal16
);
extern MEI_ERROR IFX_TwosComplement16_HexToInt (
   u16 nuVal16,
   int nBits,
   s16 * nsVal16
);

#endif /* defined(__KERNEL__) || defined (IFX_ADSL_PORT_RTEMS) */

#endif /* _IFX_ADSL_BASIC_H */
