#ifndef __TM_H__
#define __TM_H__

#include "tbstype.h"
#include "tbsmsg.h"
#include <unistd.h>

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


typedef   long  TM_RET;

void TM_SetPrintFlag(unsigned char ucPrint);

TM_RET TM_PollTesterMsg(unsigned long ulTimeOut);

TM_RET TM_SendMsgToTester(ST_MSG *pstMsg);

TM_RET TM_TesterSideInit(void);






TM_RET TM_PollCPEMsg(unsigned long ulTimeOut);

TM_RET TM_SendMsgToCPE(ST_MSG *pstMsg);

TM_RET TM_CPESideInit(void);













#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __TM_H__ */


