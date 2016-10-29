#ifndef _IFX_ADSL_AUTOBOOT_H
#define _IFX_ADSL_AUTOBOOT_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

/** \file
   This file defines the autoboot interface description for DSL CPE API
*/

/*
   external variables from ifx_adsl_autoboot.c
*/
extern wait_queue_head_t   wait_queue_autoboot;
extern int autoboot_shutdown;
extern struct completion autoboot_thread_exit;
extern autoboot_line_states_t autoboot_linestate;
extern autoboot_adsl_mode_t autoboot_adsl_mode;
extern int autoboot_showtime_lock;

#endif /*_IFX_ADSL_AUTOBOOT_H*/
