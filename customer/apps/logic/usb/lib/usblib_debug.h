#ifndef	__HOTPLUG_LIB_DEBUG_H__
#define	__HOTPLUG_LIB_DEBUG_H__

#ifdef	ENABLE_HOTPLUG_DEBUG
#include <assert.h>
#define	HOTPLUG_ASSERT	assert

extern void debuginfo(char *func, int line, char type, char *msg, int intmsg);
#define	DBINF_STR	1
#define	DBINF_INT	2
#define	DEBUG_INF(msg)	debuginfo((char *)__FUNCTION__, __LINE__, DBINF_STR, msg, 0)
#define	DEBUG_INF2(intmsg)	debuginfo((char *)__FUNCTION__, __LINE__, DBINF_INT, NULL, intmsg)
#else
#define	NOP	(void)0
#define	HOTPLUG_ASSERT(s)	NOP

#define	DEBUG_INF(msg)		NOP
#define	DEBUG_INF2(msg)	NOP
#endif

#endif /* __HOTPLUG_LIB_DEBUG_H__ */
