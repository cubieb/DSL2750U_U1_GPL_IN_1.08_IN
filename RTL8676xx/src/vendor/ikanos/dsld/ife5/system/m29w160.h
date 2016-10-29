#ifndef __M29W160_H__
#define __M29W160_H__

/* =============================================================================
 *
 * Copyright (C) 2001 Analog Devices Inc. All rights reserved
 *
 * The information and source code contained herein is the exclusive property
 * of Analog Devices and may not be disclosed, examined or reproduced in whole
 * or in part without the explicit written authorization from Analog Devices.
 *
 * ===========================================================================*/

/* =============================================================================
 *
 * File name    : $RCSfile: m29w160.h,v $
 * Author       : $Author: ksrao $
 * Version      : $Revision: 1.1 $
 * Description  : Header file for 29W160 support code
 *
 * ===========================================================================*/

typedef unsigned long   flash_offset_t;         /* Offsets into areas/flash */
typedef unsigned long   flash_length_t;         /* Length of operations etc */

/* The following functionins are supplied by the 29w160 support code. These
 * support erase and write of the device in 8 and 16 bit modes
 */
extern int
m29W160Erase16(void *pBase, flash_offset_t offset, flash_length_t length);

extern int
m29W160Write16(         void *pBase,
                        flash_offset_t offset,
                        void *pData,
                        flash_length_t length);

#endif /* __M29W160_H__ */
