/**************************************************************
***************************************************************
**
** Host Application, Copyright (C) 2000-2002
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
$Date: 2008/07/18 11:08:38 $
**************************************************************/

#ifndef _OAMPDB_H_
#define _OAMPDB_H_

#include <string.h>

#if (defined(P2CPE) || defined(BME_CPE_20)) && defined(INI_SUPPORT)
#include <api/HostPtmfApi.h>
#endif

#define MAX_PASS_WD_LEN 11
#define MAX_PASS_BUF_LEN  (MAX_PASS_WD_LEN + 1)

#define MAX_PSD_TONES        (28)
#define MAX_PSD_MSG_LENGTH   (85)   // Keep this in sync with
                                    // IPOS_MAX_PSD_LEN & DAPI_MAX_PSD_LEN
                                    // MAX_PSD_MSG_LENGTH >= (MAX_PSD_TONES * 3) + 1


/* Default values for Max PSD level */
#define DEFAULT_MAX_PSD  { 28, { {  138, 255},  {  640, 255}, \
                                 {  640, 255},  { 1100, 255},  \
                                 { 1100, 255},  { 1300, 255},  \
                                 { 1300, 255},  { 2000, 255},  \
                                 { 2000, 255},  { 3000, 255},  \
                                 { 3000, 255},  { 3750, 255},  \
                                 { 3750, 255},  { 4500, 255},  \
                                 { 4500, 255},  { 5100, 255},  \
                                 { 5100, 255},  { 5200, 255},  \
                                 { 5200, 255},  { 7000, 255},  \
                                 { 7000, 255},  { 8500, 255},  \
                                 { 8500, 255},  {12000, 255},  \
                                 {12000, 255},  {16700, 255},  \
                                 {16700, 255},  {17600, 255},  \
                               } \
                         }


typedef struct
{
  unsigned short Freq;     // in KHz
  unsigned char  PsdLevel; // in terms of (-140 + X*0.5)dBm/Hz)
} tPsdTone;

typedef struct
{
  unsigned char   NumPsd;
  tPsdTone PsdTone[MAX_PSD_TONES];
} tMaxPsd;

typedef struct
{
  unsigned char bandPlan;
  unsigned char autoStartWaitTime;
  unsigned char autoStartScanTime;
  unsigned char losIntegrationTime;
  char 			oprPasswd[MAX_PASS_BUF_LEN];
  char 			supPasswd[MAX_PASS_BUF_LEN];
  unsigned int 	iniCrcThreshold; // in unit of 10**-4
  unsigned char utopiaMode;
  unsigned char optionMask;	
  unsigned char	psdMaskLevelIndex;
  long			k0[3];
  long			k1[3];
  unsigned char EnableGhs;
  unsigned char EnetFlowControl;
  unsigned short custId;
  unsigned short keys[3];
  unsigned int vdslCrcThreshold;
  unsigned char ife2_pwr_mode;
  unsigned char enetStatusRptEn;
  unsigned char oamOptionMask;
  unsigned char jIsdnDsuEnable;
  long			k0_hi[3];
  long			k1_hi[3];
  tMaxPsd maxPsd;
} blk2Struct;


extern blk2Struct blk2Shadow;
#ifdef INI_SUPPORT 
#if defined(VTU_R) && (defined(P2CPE) || defined(BME_CPE_20))
extern tsIposPtmfEnetAddrFilterCfg macFilCfgShadow;
#endif
#endif
extern unsigned int numCrcCheckFrames;

/*-------------------------------------------------------------------------*/
#define oampGetBandPlan               (blk2Shadow.bandPlan)
#define oampGetAutoStartWaitTime      (blk2Shadow.autoStartWaitTime)
#define oampGetAutoStartScanTime      (blk2Shadow.autoStartScanTime)
#define oampGetLosIntegrationTime     (blk2Shadow.losIntegrationTime)
                                      
#define oampGetOperatorPassword       (blk2Shadow.oprPasswd)
                                      
#define oampGetSupervisorPassword     (blk2Shadow.supPasswd)
                                      
#define oampGetIniCrcThreshold        (blk2Shadow.iniCrcThreshold)
                                      
#define oampGetUtopiaMode             (blk2Shadow.utopiaMode)
                                      
#define oampGetOptionMask             (blk2Shadow.optionMask)
#define oampGetjIsdnDsuEnable         (blk2Shadow.jIsdnDsuEnable?1:0)
#define oampGetPsdIndex				        (blk2Shadow.psdMaskLevelIndex)
#define oampGetK0_array(i)			      (blk2Shadow.k0[i])
#define oampGetK1_array(i)			      (blk2Shadow.k1[i])
#define oampGetK0_hi_array(i)			      (blk2Shadow.k0_hi[i])
#define oampGetK1_hi_array(i)			      (blk2Shadow.k1_hi[i])

#define oampGetGhsControl             (blk2Shadow.EnableGhs)
                                      
#define oampGetEnetFlowControl        (blk2Shadow.EnetFlowControl)

#define oampGetVdslCrcThreshold        (blk2Shadow.vdslCrcThreshold)
#define oampGetIFE2PwrMode             (blk2Shadow.ife2_pwr_mode)
#define oampGetRemEnetStatRptEn        (blk2Shadow.enetStatusRptEn)
#define oampGetOamOptionMask           (blk2Shadow.oamOptionMask)
#ifdef CO_30_DIAGS
#define oampGetDiagsOption             ((oampGetOamOptionMask >> 4)&0x3)
#endif
#define oampGetMaxPsd                  (blk2Shadow.maxPsd)
/*-------------------------------------------------------------------------*/

#define oampSetBandPlan(a)            blk2Shadow.bandPlan = a; \
                                      nvramUpdateBlk(2)

#define oampSetAutoStartWaitTime(a)   blk2Shadow.autoStartWaitTime = a; \
                                      nvramUpdateBlk(2)

#define oampSetAutoStartScanTime(a)   blk2Shadow.autoStartScanTime = a; \
                                      nvramUpdateBlk(2)

#define oampSetLosIntegrationTime(a)  blk2Shadow.losIntegrationTime = a; \
                                      nvramUpdateBlk(2)

#define oampSetOperatorPassword(a)    strncpy(blk2Shadow.oprPasswd, a, MAX_PASS_BUF_LEN); \
                                      blk2Shadow.oprPasswd[MAX_PASS_BUF_LEN - 1] = '\0';  \
                                      nvramUpdateBlk(2)

#define oampSetSupervisorPassword(a)  strncpy(blk2Shadow.supPasswd, a, MAX_PASS_BUF_LEN); \
                                      blk2Shadow.supPasswd[MAX_PASS_BUF_LEN - 1] = '\0';  \
                                      nvramUpdateBlk(2)

#define oampSetIniCrcThreshold(a)     blk2Shadow.iniCrcThreshold = a; \
                                      nvramUpdateBlk(2)

#define oampSetUtopiaMode(a)          blk2Shadow.utopiaMode = a; \
                                      nvramUpdateBlk(2)

#define oampSetOptionMask(a)          blk2Shadow.optionMask = a; \
                                      nvramUpdateBlk(2)
									  
#define oampSetPsdLevel(a)			      blk2Shadow.psdMaskLevelIndex=a; \
									                    nvramUpdateBlk(2)						  

#define oampSetGhsControl(a)          blk2Shadow.EnableGhs=a; \
									                    nvramUpdateBlk(2)						  

#define oampSetEnetFlowControl(a)     blk2Shadow.EnetFlowControl=a; \
									                    nvramUpdateBlk(2)

#define oampGetCustID()			  	  (blk2Shadow.custId)

#define oampSetCustID(a)			  blk2Shadow.custId = a; \
														nvramUpdateBlk(2)

#define oampGetKeys(keyID)			  blk2Shadow.keys[keyID]

#define oampSetKeys(val, keyId)		  blk2Shadow.keys[keyId] =  val; \
														nvramUpdateBlk(2)

#define oampSetAllKeys(v0, v1, v2)	  blk2Shadow.keys[0] =  v0; blk2Shadow.keys[1] =  v1; blk2Shadow.keys[2] =  v2; \
														nvramUpdateBlk(2)																																		  

#define oampSetVdslCrcThreshold(a)     blk2Shadow.vdslCrcThreshold = a; \
                                      nvramUpdateBlk(2)
                                      
#define oampSetIFE2PwrMode(a)          blk2Shadow.ife2_pwr_mode = a; \
                                       nvramUpdateBlk(2) 
#define oampSetRemEnetStatusRptEn(a)          blk2Shadow.enetStatusRptEn = a; \
                                       nvramUpdateBlk(2) 

#ifdef CO_30_DIAGS
#define oampSetDiagsOption(a)         blk2Shadow.oamOptionMask &= 0xCF;   \
                                      blk2Shadow.oamOptionMask |= (a & 0x3) << 4; \
                                      nvramUpdateBlk(2)

#define oampSetOamOptionMask(a)       blk2Shadow.oamOptionMask &= 0x70; \
                                      blk2Shadow.oamOptionMask |= (a & 0x8F); \
                                      nvramUpdateBlk(2)
#else                                      
#define oampSetOamOptionMask(a)       blk2Shadow.oamOptionMask = a; \
                                      nvramUpdateBlk(2)
#endif

#define oampSetjIsdnDsuEnable(a)      blk2Shadow.jIsdnDsuEnable = a; \
                                      nvramUpdateBlk(2)

#define oampSetMaxPsd(a)              memcpy(&blk2Shadow.maxPsd, a, sizeof(tMaxPsd));\
                                      nvramUpdateBlk(2)
									  
/*-------------------------------------------------------------------------*/
extern void nvramUpdateBlk (int blkNum);

extern void initOAMPDB (void);
extern void oampSetK_array(float *k0, float *k1, float *k0_hi, float *k1_hi);
extern void oampPrintK_array();
#endif
