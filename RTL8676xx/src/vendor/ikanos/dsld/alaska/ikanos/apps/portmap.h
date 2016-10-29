#ifndef		PORTMAPH
#define		PORTMAPH
/**************************************************************
$Workfile:   portmap.h  $
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
$Date: 2008/07/18 11:08:39 $
***************************************************************/

/*************************************************************

	this header file defines the definitions, structures, and functions prototypes
	for portmapping functions

	The numbering scheme used in the application:
		
		bmenum:   	The bme index  						1-MAX_NUM_OF_BMES
		portnum:  	The port index within the BME:	 	1-MAX_PORTS_PER_BME
		
		linenum:	The global line index: 				1-MAX_NUM_OF_PORTS 		

	Note: INI port index start from 0					 = lineNum-1.																
	
*************************************************************/



typedef	struct
{

	int	lineNum;						/* the global line index: -1: end of table   */
	int	iniNum;							/* the global INI index		*/
	int	uaSlow;							/* utopia address for slow channel	*/
	int	uaFast;							/* utopia address for fast channel 	*/
	ipos_port_id	pid;				/* the port identifier: bmeNum and portNum */
	U32				bmBandPlan;			/* bit map for supported bandplan within the map
											
											1: the mapping is supported for the bandplan
											0: the mapping is not supported for the bandplan
											*/
							
} sys_port_mapping;



/************************************************************ 

	set the port mapping: input: bmeNum, bandPlan for the BME 
	output: IPOS_SUCCESS or IPOS_ERROR_CODE  

************************************************************/

extern int	setPortMap(int bmeNum, int bandPlan);	   		/* set the port mapping	*/

extern int		getBmePortId( int	lineNum, ipos_port_id *pid); /* lineNum-> portId */
extern int		getLineNum(ipos_port_id, int *lineNum);			/* portId->lineNum   */

extern int		getIniPhy(int lineNUm, int	*iniNum);  			/* lineNum-> INI phyNum */

extern int		getUtopiaSlow(int lineNum, int *uaSlow);		/* lineNum->utopia slow channel */
extern int		getUtopiaFast(int lineNum, int *uaFast);		/* lineNum->utopia fast channel */ 

extern int		getIniMap(ipos_port_id pid, int *IniIndex, int *IniSlow, int *IniFast);

extern void setUtopiaAddr(void);


#endif


