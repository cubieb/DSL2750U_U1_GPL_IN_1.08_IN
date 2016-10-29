/* ###########################################################################
# (c) Copyright Conexant Systems, Inc. 2004
#
# Conexant Systems, Inc. Confidential and Proprietary
#
# The following software source code ("Software") is strictly confidential and
# is proprietary to Conexant Systems, Inc. and its subsidiaries (collectively,
# "Conexant"). It may only be read, used, copied, adapted, modified or 
# otherwise dealt with by you if you have entered into a license agreement 
# with Conexant and then subject to the terms of that agreement and any other 
# applicable confidentiality agreement between you and Conexant.
#
# If you are in any doubt as to whether you are entitled to access, read,
# use, copy, adapt, modify or otherwise deal with the Software or whether you
# are entitled to disclose the Software to any other person you should
# contact Conexant.  If you have not entered into a license agreement
# with Conexant granting access to this Software you should forthwith return
# all media, copies and printed listings containing the Software to Conexant.
#
# Conexant reserves the right to take legal action against you should you
# breach the above provisions.
#
# If you are unsure, or to report violations, please contact
# licensee.support@conexant.com
# ##########################################################################*/

#ifndef ARM1026_H
#define ARM1026_H

/******************************************************************************
*
*       Definitions for ARM1026 Coprocessor Registers
*       =============================================
*
******************************************************************************/

/* TBD: Currently this header file only contains extra definitions required which
   are not already in arm920.h.
*/

/* Auxilliary control register */
#define ARM1026_CP15_CTRL_AUX  1

#define ARM1026_CP15_xTCM_REGION   cr9
#define ARM1026_CP15_xTCM_REGION2   cr1
#define ARM1026_CP15_xTCM_REGION_DTCM  0
#define ARM1026_CP15_xTCM_REGION_ITCM  1


/* Control bit-definitions - cr1 */
#define ARM1026_CP15_CTRL_VICENA       (0x01000000)  /* VIC enable                */
#define ARM1026_CP15_CTRL_LOADPCT_DIS  (0x00008000)  /* Load PC thumb disable     */
#define ARM1026_CP15_CTRL_ROUNDROBIN   (0x00004000)  /* Round robin replacement   */
#define ARM1026_CP15_CTRL_VECTSELECT   (0x00002000)  /* Alternate vectors select  */
#define ARM1026_CP15_CTRL_ICACHEENA    (0x00001000)  /* Instruction cache enable  */
#define ARM1026_CP15_CTRL_BRPREDICT    (0x00000800)  /* Branch predict enable     */
#define ARM1026_CP15_CTRL_ROMPROT      (0x00000200)  /* ROM protection            */
#define ARM1026_CP15_CTRL_SYSPROT      (0x00000100)  /* System protection         */
#define ARM1026_CP15_CTRL_ENDIAN       (0x00000080)  /* Endian                    */
#define ARM1026_CP15_CTRL_DCACHEENA    (0x00000004)  /* Data cache enable         */
#define ARM1026_CP15_CTRL_AFAULTENA    (0x00000002)  /* Alignment fault enable    */
#define ARM1026_CP15_CTRL_MMUENA       (0x00000001)  /* MMU enable                */


/* Auxilliary control register bit-definitions - cr1, cr0, 1 */
#define ARM1026_CP15_AUX_CTRL_MXUCFG   (0x1)

/* xTCM region registers bit-definitions - cr9, cr1 */
#define ARM1026_CP15_xTCM_REGION_BASE       (0xfffff000)
#define ARM1026_CP15_xTCM_REGION_SIZE_MASK  (0x3c)
#define ARM1026_CP15_xTCM_REGION_SIZE_SHIFT (2)
#define ARM1026_CP15_xTCM_REGION_ENABLED    (0x1)


#endif /* ARM1026_H */

