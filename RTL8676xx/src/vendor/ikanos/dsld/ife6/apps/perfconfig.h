
/************************************************************************************************
	Workfile:   userconfig.h  
*************************************************************************************************

 FIRMWARE,	Copyright (C) 2000-2002 Ikanos Communication, Fremont, CA
			All rights reserved.

			The information and contents of this file are the proprietary information of Ikanos 
			Communication and may not be disclosed or used without the formal written approval of
			Ikanos Communication
*************************************************************************************************
$Revision: 1.1 $
$Date: 2008/07/18 11:08:36 $
************************************************************************************************/

#ifndef _USERCONFIG_H_
#define _USERCONFIG_H_

#include <api/apitype.h>

/* Include fiels */

				
#define MAX_15MIN_HISTORY_REGISTERS		16
#define MAX_24HOUR_HISTORY_REGISTERS		7


/*	Default Threshold values (in seconds) set according to the standards for declaring a Failure after the occurence of a defect 
	User is not supposed to change these values. */

#define DEFAULT_TS1			6/*2.5*/		/* Time (in seconds) after which contiguous LOS defect could be declared as a LOS failure */
#define DEFAULT_TS2			2/*10*/		/* Time (in seconds) after which LOS failure could be cleared  with NO LOS defect */
#define DEFAULT_TF1			10/*2.5*/		/* Time (in seconds) after which contiguous LOF defect could be declared as a LOF failure */
#define DEFAULT_TF2			2/*10*/		/* Time (in seconds) after which LOF failure could be cleared  with NO LOF defect */
#define DEFAULT_TN1			2.5		/* Time (in seconds) after which contiguous No cell delineation primitive  could be declared as a failure */
#define DEFAULT_TN2			10		/* Time (in seconds) after which NCD failure could be cleared with NO NCD primitive  */
#define DEFAULT_TL1			2.5		/* Time (in seconds) after which contiguous Loss of cell delineation primitive  could be declared as a failure */
#define DEFAULT_TL2			10		/* Time (in seconds) after which LCD failure could be cleared with NO LCD primitive  */
#define DEFAULT_MAX_CRC_ERROR			100
#define DEFAULT_MAX_FEC_COUNT			100
#define DEFAULT_MAX_HEC_VIOLATION_COUNT		100
#define DEFAULT_MAX_ERR_SECONDS			100
#define DEFAULT_MAX_SEV_ERR_SECONDS		 10
#define DEFAULT_MAX_UNAVAILABLE_SECONDS   5


/* These are user defined values which the can be changed to a set of new values */
#define TS1							DEFAULT_TS1	
#define TS2							DEFAULT_TS2	
#define TF1							DEFAULT_TF1	
#define TF2							DEFAULT_TF2	
#define TN1							DEFAULT_TN1	
#define TN2							DEFAULT_TN2	
#define TL1							DEFAULT_TL1	
#define TL2							DEFAULT_TL2	
#define MAX_CRC_ERROR				DEFAULT_MAX_CRC_ERROR
#define MAX_FEC_COUNT				DEFAULT_MAX_FEC_COUNT
#define MAX_HEC_VIOLATION_COUNT		DEFAULT_MAX_HEC_VIOLATION_COUNT
#define MAX_ERR_SECONDS				DEFAULT_MAX_ERR_SECONDS
#define MAX_SEV_ERR_SECONDS			DEFAULT_MAX_SEV_ERR_SECONDS
#define MAX_UNAVAILABLE_SECONDS		DEFAULT_MAX_UNAVAILABLE_SECONDS

/* Used for tracking errored seconds */

typedef enum eErroredSecondsType
{
	eSecondsWithLOS,
	eSecondsWithLOF,
	eSecondsWithSEF,
	eSecondsWithCRC_F,
	eSecondsWithCRC_S,
	eSecondsNotSupported
} eErroredSecondsType;

extern void incrementErroredSeconds( int lineNum, eErroredSecondsType eEST );
extern U32 getErroredSeconds( int lineNum, eErroredSecondsType eEST );

#endif  /* _USERCONFIG_H_ */
