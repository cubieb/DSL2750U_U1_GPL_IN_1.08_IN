#ifndef	BSPH
#define BSPH

/**************************************************************
$Workfile:   bsp.h  $
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
$Date: 2008/07/18 11:09:33 $
***************************************************************/
#include <api/apitype.h>

#ifdef CPE_ATM
#define PORT_PROFILE_BASE 0x50081000
#define PORT_PROFILE_1	  0x50080210
#define PORT_PROFILE_2	  (PORT_PROFILE_1+TOTAL_PORTPARMS)
#define PORT_PROFILE_3	  (PORT_PROFILE_2+TOTAL_PORTPARMS)
#define PORT_PROFILE_0 	  PORT_PROFILE_3
#define NVRAM_BLK_START   0x50080000 

#elif defined(RTEMS_VX180)

#define PORT_PROFILE_BASE 0xBFFC1000
#define PORT_PROFILE_1	  0xBFFC0210
#define PORT_PROFILE_2	  (PORT_PROFILE_1+TOTAL_PORTPARMS)
#define PORT_PROFILE_3	  (PORT_PROFILE_2+TOTAL_PORTPARMS)
#define PORT_PROFILE_0 	  PORT_PROFILE_3
#define NVRAM_BLK_START   0xBFFC0000 

#else  /* CPE_ATM */

#define PORT_PROFILE_BASE 0xFFF81000
#define PORT_PROFILE_1	  0xFFF80210
#define PORT_PROFILE_2	  (PORT_PROFILE_1+TOTAL_PORTPARMS)
#define PORT_PROFILE_3	  (PORT_PROFILE_2+TOTAL_PORTPARMS)

#define PORT_PROFILE_0 	  PORT_PROFILE_3
	  

#define NVRAM_BLK_START   0xFFF80000 

#endif /* CPE_ATM */

/* Flash Functions	*/
extern U32 FlashWrite(volatile U8 *pDest, U8 *pSource,	 U32 length);

extern U32 FlashEraseSector(volatile U8 *pSector);


/* the system information for BME_CPE_20: stored in flash */

#ifdef	BME_CPE_20

#define	AZTEC_SRAM		0
#define	AZTEC_SDRAM16	1
#define AZTEC_SDRAM32	2


typedef	struct
{
	U8	bmeInfo;		/* bme Info: not used  */
	U8	afeInfo;		/* afe info: not used  */
	U8	eiuInfo;		/* eiu info: not used  */
	U8	memInfo;		/* mem info: BME_CPE_20_SRAM, BME_CPE_20_SDRAM16, BME_CPE_20_SDRAM32 */
}	sysInfo;

#endif


/*  Caution:  the definition of LOOPS_PER_uS changes from processor to processor  */

#define LOOPS_PER_uS 5

#define uSdelay( uS )						\
{								\
	U32	iter;						\
								\
	for (iter=0;iter< (uS*LOOPS_PER_uS);++iter)		\
	  ;							\
}


#endif			
