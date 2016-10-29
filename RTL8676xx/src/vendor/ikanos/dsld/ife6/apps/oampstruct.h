/**************************************************************
$Workfile:   oampstruct.h  $
***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communications, Fremont, CA
** All rights reserved. 
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
$Revision: 1.1 $
$Date: 2008/07/18 11:08:36 $
***************************************************************/

#ifndef OAMPSTRUCT_H 
#define OAMPSTRUCT_H

#include "perfhandler.h"
#include "oampdef.h"


/* oamp Port Structure	*/
typedef	struct
{
	eoamp_port_state	portState;
	ipos_port_profile	*pPortProfile;
	tPerformance		Performance;
	uPortStruct 		portStatus;
	ipos_port_counters  Counters;
    ipos_qos_counters QosCounters;
	ipos_port_nesnrattn	SnrAttn;
    ipos_pm_counters PmCounters;
    ipos_pm_counters PmCountersAtPortStart;
    ipos_pm_counters PmCounters24Hrs;
    eoamp_link_down cpeLinkDownReason;
}	oamp_port;



/* oamp BME structure	*/

typedef	struct
{

	int	bmeNum;
	int	numPort;
	int	bmeHasCrashed;
	int	bmeErrorCode;

	eoamp_bme_state	bmeState;
	eoamp_cmd_state	cmdState;

	oamp_port	oampPort[MAX_PORTS_PER_BME+1];

}	oamp_bme;


#endif /* OAMPSTRUCT_H */





