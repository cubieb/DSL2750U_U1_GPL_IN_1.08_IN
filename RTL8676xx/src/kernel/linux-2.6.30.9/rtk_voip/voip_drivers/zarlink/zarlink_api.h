
#ifndef __ZARLINK_API_H__
#define __ZARLINK_API_H__

#include "common/zarlinkCommon.h"

extern int ZarlinkInit(int pcm_mode);

void ZarlinkSetFxsPcmMode(RTKLineObj *pLine, int pcm_mode);
void ZarlinkSetFxsAcProfileByBand(RTKLineObj *pLine, int pcm_mode);
void ZarlinkSetRingCadenceProfile(RTKLineObj *pLine, uint8 ring_cad);
void ZarlinkSetRingCadence(RTKLineObj *pLine, unsigned short on_msec, unsigned short off_msec);
void ZarlinkSetRingFreqAmp(RTKLineObj *pLine, uint8 profile);
void ZarlinkFxsRing(RTKLineObj *pLine, uint8 enable);
void ZarlinkSendNTTCAR(RTKLineObj *pLine);
unsigned char ZarlinkSendNTTCAR_Check(RTKLineObj *pLine, unsigned long time_out);
unsigned char ZarlinkGetFxsHookStatus(RTKLineObj *pLine, int from_polling_timer);
unsigned char ZarlinkGetLineState(RTKLineObj *pLine);
void ZarlinkSetLineState(RTKLineObj *pLine, VpLineStateType state);
void ZarlinkSetOHT(RTKLineObj *pLine, uint8 reversal);
void ZarlinkSetLineOpen(RTKLineObj *pLine);
void ZarlinkSetPcmTxOnly(RTKLineObj *pLine, int enable);
void ZarlinkSetPcmRxOnly(RTKLineObj *pLine, int enable);
void ZarlinkSetImpedenceCountry(RTKLineObj *pLine, uint8 country);
void ZarlinkSetImpedence(RTKLineObj *pLine, uint16 preset);
unsigned char  ZarlinkCheckFxsRing(RTKLineObj *pLine);

/********** DAA Function **********/
BOOL ZarlinkGetFxoLineStatus(RTKLineObj *pLine, uint8 category);
unsigned char ZarlinkGetFxoHookStatus(RTKLineObj *pLine);
void ZarlinkSetFxoLineState(RTKLineObj *pLine, VpLineStateType lineState);
void ZarlinkSetFxoLineOnHook(RTKLineObj *pLine);
void ZarlinkSetFxoLineOffHook(RTKLineObj *pLine);
void ZarlinkSetFxoLineOHT(RTKLineObj *pLine);
void ZarlinkSetFxoRingValidation(RTKLineObj *pLine);

/* Debug function */
void ZarlinkDumpDevReg(RTKDevObj *pDev);
void ZarlinkRWDevReg(RTKLineObj *pLine, unsigned int reg, unsigned char *len, unsigned char *regdata);

#endif /* __ZARLINK_API_H__*/

