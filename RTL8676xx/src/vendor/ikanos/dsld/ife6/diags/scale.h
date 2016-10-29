#ifndef SCALE_H 
#define SCALE_H 


/*========================================================================
**            
**      scale.h - top level include file for BME Simulator 
**      	has scale factors & precisions of fix-pt nunbers 
**      	and generic masks of diff widths 
**
**	Ver: 1.0;  Release date: 6/10/2000;   Author: Avadhani Shridhar. 
**                 Initial implementation. 
**
**	Copyright (c) 2000, Velocity Communication
**  This Velocity Communication Inc. proprietary information of must be 
**  maintained in the strictest of confidence and may not be copied, 
**  disclosed,or used without the express prior written consent of an  
**  officer of Velocity Communication Inc.
**=======================================================================*/

#define  _1b   0x1 
#define  _2b   0x3 
#define  _3b   0x7 
#define  _4b   0xf 
#define  _5b   0x1f 
#define  _6b   0x3f 
#define  _7b   0x7f 
#define  _8b   0xff 
#define  _9b   0x1ff 
#define  _10b  0x3ff 
#define  _11b  0x7ff 
#define  _12b  0xfff 
#define  _13b  0x1fff 
#define  _14b  0x3fff 
#define  _15b  0x7fff 
#define  _16b  0xffff 
#define  _17b  0x1ffff 
#define  _18b  0x3ffff 
#define  _19b  0x7ffff 
#define  _20b  0xfffff 
#define  _21b  0x1fffff 
#define  _22b  0x3fffff 
#define  _23b  0x7fffff 
#define  _24b  0xffffff 
#define  _25b  0x1ffffff 
#define  _26b  0x3ffffff 
#define  _27b  0x7ffffff 
#define  _28b  0xfffffff 
#define  _29b  0x1fffffff 
#define  _30b  0x3fffffff 
#define  _31b  0x7fffffff 
#define  _32b  0xffffffff 

#define  _ZPFXTA_MASK    0x3ffffffc 
#define  _VIF_RWCR_MASK  0xc000ffff 
#define  _RNI_PHY_MASK   0x80000000 
#define  _YFI_TOFT_MASK  0xbfffffff 

/*=============== SCALING in BME Sim Ver PMD3 ===========================
ATU-C Transmit:
 *  Gain Scaler output is 16 bits in Q1.15 format.
 *  IDFT does not do any scaling, producing 16 bit output in Q1.15 format.

ATU-R Receive:
 *  TEQ is in bypass mode, so does not do any scaling. 
 *  DFT (N-point) must be scaled by N to produce fix-pt in (-1 < n <= 1) range.
    But this will lose too much precision to avoid a rare case of overflow.
    So, we scale DFT output by smaller number and saturate to avoid overflow.
    scale factor can be 512 down to 64 ? 
    NOTE: need to add saturation code to avoid overflow of DFT output. 

=======================================================================*/


#define  GDMT_GSCALE_FIX_WIDTH		16 
#define  GDMT_GSCALE_BYPASS_SHIFT	24  

#define  GDMT_IDFT_FIX_WIDTH		16 
#define  GDMT_IDFTscaleFactor	 	 1 
#define  GDMT_IDFTscaleShift		 0 

#define  GDMT_DFT_FIX_WIDTH		20 
#define  GDMT_DFTscaleFactor		128 
#define  GDMT_DFTscaleShift		 7 


/*----- Global Vars used to hold scale factros & precisions -----*/ 
extern  int  DFT_FIX_WIDTH;  /* float output quantized to this precision */
extern  int  DFTscaleFactor; /* scale down output of dft by this */
extern  int  DFTscaleShift;  /* scale factor as num of bits to shift */ 

extern  int  IDFT_FIX_WIDTH;  
extern  int  IDFTscaleFactor;

extern  ub   GSCALE_FIX_WIDTH; 
extern  ub   GSCALE_BYPASS_SHIFT; /* scale up by this much to convert 8-bit
				  // consteln map output to 16-bit ifft inp */


#endif 

