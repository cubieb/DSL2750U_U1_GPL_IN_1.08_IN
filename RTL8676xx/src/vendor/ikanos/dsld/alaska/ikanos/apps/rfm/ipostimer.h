/**************************************************************
$Workfile:   ipostimer.h  $
***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA 
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**  
$Revision: 1.1 $
$Date: 2008/07/18 11:08:52 $
**************************************************************/


#ifndef IPOSTIMER_H
#define IPOSTIMER_H


/* definition of IPOS_MAX_TMRS changed in response to TT ID 1057 */

#define IPOS_MAX_TMRS		(64*IPOS_MAX_BME)     // Maximum 64 timers per BME
#define IPOS_TMR_RESOLUTION     300   // 300 milliseconds

#define IPOS_TMR_PASSIVE	0
#define IPOS_TMR_ACTIVE 	1

typedef	void (* tmrExp)(U32 arg1, U32 arg2);

struct ipos_tmr_struct {
  U16  timecount ;
  U16  elapsedtime ;
  U8   status ;
  U32  user_arg1 ;
  U32  user_arg2 ;
  tmrExp ptrCbFunc;
  struct ipos_tmr_struct *next ;
} ;

typedef struct ipos_tmr_struct ipos_tmr_info ;

int   iposTmrInit( void ) ;
int   iposCreateTmr(ipos_tmr_info **tmrHandle) ;
int   iposFreeTmr(ipos_tmr_info *tmrHandle) ;
int   iposSetTmr(ipos_tmr_info *tmr, U16 timecount, tmrExp pFunc, U32 arg1, U32 arg2) ;
int   iposResetTmr(ipos_tmr_info *tmr) ;
void  print_lists( void ) ;


#endif
