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
$Date: 2008/07/18 11:08:34 $
**************************************************************/

#ifndef _OAMPDB_H_
#define _OAMPDB_H_

#include <string.h>

#define MAX_PASS_WD_LEN 11
#define MAX_PASS_BUF_LEN  (MAX_PASS_WD_LEN + 1)

typedef struct
{
  unsigned char bandPlan;
  unsigned char autoStartWaitTime;
  unsigned char autoStartScanTime;
  unsigned char losIntegrationTime;
  char oprPasswd[MAX_PASS_BUF_LEN];
  char supPasswd[MAX_PASS_BUF_LEN];
} b2_01;

typedef struct
{
  unsigned char bandPlan;
  unsigned char autoStartWaitTime;
  unsigned char autoStartScanTime;
  unsigned char losIntegrationTime;
  char oprPasswd[MAX_PASS_BUF_LEN];
  char supPasswd[MAX_PASS_BUF_LEN];
  unsigned int iniCrcThreshold; // in unit of 10**-4
} blk2Struct;

extern blk2Struct blk2Shadow;
extern unsigned int numCrcCheckFrames;

#define oampGetBandPlan           (blk2Shadow.bandPlan)
#define oampGetAutoStartWaitTime  (blk2Shadow.autoStartWaitTime)
#define oampGetAutoStartScanTime  (blk2Shadow.autoStartScanTime)
#define oampGetLosIntegrationTime (blk2Shadow.losIntegrationTime)
#define oampGetOperatorPassword   (blk2Shadow.oprPasswd)
#define oampGetSupervisorPassword (blk2Shadow.supPasswd)
#define oampGetIniCrcThreshold    (blk2Shadow.iniCrcThreshold)
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

extern void nvramUpdateBlk (int blkNum);
extern void initOAMPDB (void);

#endif