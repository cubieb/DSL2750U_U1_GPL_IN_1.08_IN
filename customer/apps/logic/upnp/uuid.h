#ifndef _UUID_H
#define _UUID_H

#include "warnlog.h"

#ifndef TBS_SUCCESS
#define TBS_SUCCESS      0
#endif

#ifndef TBS_FAILED
#define TBS_FAILED       -1
#endif

/* »’÷æ¥Ú”°∫Í */
#ifdef UUID_DEBUG
#define UUID_TRACE(fmt, args...)		COMMON_TRACE(MID_UPNP, fmt "\n", ##args)
#else
#define UUID_TRACE(fmt, args...)
#endif

struct ST_UPNP_UUID
{
   unsigned long    time_low;
   unsigned short   time_mid;
   unsigned short    time_hi_and_version;
   unsigned short  clock_seq;
   char            node[6];
};

int GetMacAddr(char* pcMacAddr);
int IfConfigRead(const char* ifname, const char *pszPrefix, const char* itemname, const char* seperator,char* value, size_t size);
char* uuidstr_create();

#endif
