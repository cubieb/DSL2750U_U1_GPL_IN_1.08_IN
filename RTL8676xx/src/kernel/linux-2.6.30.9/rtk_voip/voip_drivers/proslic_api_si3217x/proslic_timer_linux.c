#include <linux/sched.h>
#include <linux/delay.h>
#include "sys_driv_type.h"


/*
** Function: SYSTEM_Delay
*/
int time_DelayWrapper (void *hTimer, int timeInMs){
	mdelay(timeInMs);
	return 0;
}


/*
** Function: SYSTEM_TimeElapsed
*/
int time_TimeElapsedWrapper (void *hTimer, timeStamp *startTime, int *timeInMs){
	unsigned long diff = jiffies - startTime->time;
	*timeInMs = (int)(diff * 10);
	return 0;
}

/*
** Function: SYSTEM_GetTime
*/
int time_GetTimeWrapper (void *hTimer, timeStamp *time){
	time->time = jiffies;
	return 0;
}

