#ifndef _IFX_ADSL_LED_H
#define _IFX_ADSL_LED_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

/** \file
   This file implements interface definitions for LED handling of DSL CPE API
*/

extern int IFX_ADSL_LED_ModuleInit(void);
extern int IFX_ADSL_LED_Init(void);
extern int IFX_ADSL_LED_ModuleCleanup(void);
extern int IFX_ATM_LED_Callback_Register( int (*adsl_led_cb)(void));
extern int IFX_ATM_LED_Callback_Unregister( int (*adsl_led_cb)(void));
#endif /*_IFX_ADSL_LED_H*/
