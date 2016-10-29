#ifndef __OSATIMERP_H__
#define __OSATIMERP_H__

#define FREE    (BOOL)1
#define ALLOC   (BOOL)2
#define MAX_TIMERS 100

typedef struct phtimer
{
   BOOL bMode;   /* indicates if the entry is free or not */
   INT32 lTimerId;   /* unique timer id returned by timerStart fn */
   INT32 lTimeVal;   /* incremental time(wrt the previous entry) to fire */
   VOID (*timerHandler)(VOID *);   /* fn to fire */
   VOID *timerArg;   /* argument to handler fn */
   struct phtimer *timerNext;   /* to next entry in runq or freeq */
} phTimer_t;

void * __timer_thread(void *dummy);
static VOID timer_function_handler();


#endif
