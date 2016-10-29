
#include "trace.h"

FILE *LogFileHandle=NULL;

BOOL
BcmOpenLog()
{
    if(LogFileHandle) 
        return TRUE; /*Let the user use the already open handle*/ 

    if(!(LogFileHandle = fopen(LOG_FILE_NAME,"w+")))
    {
        LogFileHandle=NULL;
        return FALSE;
    }

    return TRUE;
}

void 
BcmLogMsg(const char *sFormatString, ...)
{
    va_list va;

    if (!LogFileHandle) return;
    va_start(va, sFormatString);
    vfprintf(LogFileHandle,sFormatString,va);
    va_end(va);
    fflush(LogFileHandle);
}

BOOL
BcmCloseLog()
{
    if (!LogFileHandle) return FALSE;

    if (LogFileHandle)
    {
        fflush(LogFileHandle);
        fclose(LogFileHandle);
        LogFileHandle = NULL;
    }

    return TRUE;
}






