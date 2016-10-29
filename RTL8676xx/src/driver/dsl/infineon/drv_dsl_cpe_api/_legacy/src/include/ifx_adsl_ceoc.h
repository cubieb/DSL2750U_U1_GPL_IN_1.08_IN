#ifndef _IFX_ADSL_CEOC_H
#define _IFX_ADSL_CEOC_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

/** \file
   This file defines the shared interface definitions from MEI BSP Driver
*/

extern MEI_ERROR IFX_ADSL_CEOC_IRQHandler(ifx_adsl_device_t *dev);
extern int IFX_ADSL_CEOC_Ioctl(MEI_inode_t * ino, MEI_file_t * fil, unsigned int command, unsigned long lon);
extern int IFX_ADSL_CEOC_ModuleInit(void);
extern int IFX_ADSL_CEOC_ModuleCleanup(void);
extern int IFX_ADSL_CEOC_Init(void);
#endif /*_IFX_ADSL_CEOC_H*/
