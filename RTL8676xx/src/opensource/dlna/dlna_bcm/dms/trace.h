#ifndef _TACE_H_
#define _TACE_H_

#include <stdarg.h>
#include <stdio.h>
#include "bcm_types.h"

#ifdef WIN32
#define LOG_FILE_NAME    "c:\\temp\\BcmMServer.log"
#else
#define LOG_FILE_NAME    "BcmMServer.log"
#endif
/*API To Expose logging feature*/
BOOL
BcmOpenLog();

void 
BcmLogMsg(const char *sFormatString, ...);

BOOL
BcmCloseLog();






#endif





