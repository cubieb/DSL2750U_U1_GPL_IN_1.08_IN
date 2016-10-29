#ifndef	APPCONFIGH
#define APPCONFIGH

/**************************************************************
$Workfile:   appconfig.h  $
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



/*************************************************************
	
	UI configurations
	
*************************************************************/	
#include <api/bmeconfig.h>

#define SUPERVISOR_MODE			1	  		/* supervisor mode for the UI	*/
				  
/* CONSOLE DEFS			*/				  
#define CONSOLE_TIMEOUT		6000

/* UUI Prompt		*/

#ifdef		VTU_O
#define 	PROMPT	"\nco> "
#else		
#define		PROMPT	"\ncpe> "
#endif


/*************************************************************
	
	system configurations
	
*************************************************************/	
/* BME MAXes	*/
				  
#define	MAX_NUM_OF_BMES		IPOS_MAX_BME				/* maximum number of BME	*/
#define	MAX_PORTS_PER_BME	IPOS_MAX_PORT_PER_BME		 		/* MAX number of ports per BME	*/
#define MAX_NUM_OF_PORTS  IPOS_MAX_PORTS		   		/* MAX number of total ports	*/

							   
/* miscellennous definitions */

#define	LITTLE_ENDIAN_FORMAT	1			/* little endian mode	*/
#define BIG_ENDIAN_FORMAT		2			/* big endian mode		*/


/*************************************************************

	OAMP Task allocation 


**************************************************************/
#define		MAX_GENERIC_OAMP	1			/* one for all bme nonblocking functions */

#define		MAX_NUM_OF_OAMP_TASKS	MAX_GENERIC_OAMP+MAX_NUM_OF_BMES	

#define		TID_OAMPFUNC		0			/* use oamp task [1] for nonblocking  oamp functions */
#define		TID_BLKFUNC			1			/* use oamp task [2] for blocking functions such as
												bmeReset and bmeRestart		*/




#endif				  
