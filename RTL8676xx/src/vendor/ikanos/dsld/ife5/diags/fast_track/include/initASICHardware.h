/**************************************************************
***************************************************************
**
** BME Firmware, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA 
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of 
** Ikanos Communication
**
***************************************************************
$Revision: 1.1 $
$Date: 2008/07/18 11:09:30 $
**************************************************************/

#ifndef _INITAH_H_
#define _INITAH_H_
						 
extern int	shadowSNRContextTable[40]; 
extern int	shadowDeMapperContextTable[40]; 
extern int	shadowMapperContextTable[40]; 
extern int	shadowXFIFOFastContextTable[40];
extern int	shadowXFIFOIntlContextTable[40];
extern int	shadowYFIFOContextTable[40];
extern int	shadowZFIFOContextTable[40];
extern int	shadowAFIFOContextTable[40];
extern int	shadowBFIFOIntlContextTable[40];
extern int	shadowBFIFOFastContextTable[40];
extern int	shadowCFIFO0ContextTable[40];
extern int	shadowCFIFO1ContextTable[40];
						 
						 
extern void initASICHardware ();
extern void enable_vbus(int vbus_num);
#endif

