/** \file Ve880_api.c
 * 
 *
 * This file contains the major api for upper application
 * 
 *
 * Copyright (c) 2010, Realtek Semiconductor, Inc.
 *
 */
#include "ve880_api.h"
#include "Ve_profile.h"
#ifdef CONFIG_RTK_VOIP_DRIVERS_SLIC_ZARLINK_ON_NEW_ARCH
#include "snd_define.h"
#else
#include "Slic_api.h" 	/* for COUNTRY_USA ... */
#endif

#undef DEBUG_API 

#if defined(DEBUG_API)
#define DEBUG_API_PRINT() printk("%s(%d) line #%d\n",__FUNCTION__,__LINE__,pLine->ch_id);
#else
#define DEBUG_API_PRINT()
#endif

VpStatusType Ve880SetRingCadenceProfile(RTKLineObj *pLine, uint8 ring_cad)
{
	VpStatusType status;
	VpProfilePtrType ring_cadence = DEF_LE880_RING_CAD_PROFILE;

	DEBUG_API_PRINT();

	switch (ring_cad)
	{
		case 0:
			ring_cadence = LE880_RING_CAD_STD;
			PRINT_MSG("set LE880_RING_CAD_STD\n");
			break;
		case 1:
			ring_cadence = LE880_RING_CAD_SHORT;
			PRINT_MSG("set LE880_RING_CAD_SHORT\n");
			break;
		default:
			ring_cadence = LE880_RING_CAD_STD;
			PRINT_MSG("set LE880_RING_CAD_STD\n");
			break;
	}

	status = VpInitRing( pLine->pLineCtx, 
						 ring_cadence,
						 VP_PTABLE_NULL);

	if ( status == VP_STATUS_SUCCESS )
		pLine->pRing_cad_profile = ring_cadence;

	return status;
}

VpStatusType Ve880SetImpedenceCountry(RTKLineObj *pLine, uint8 country)
{
	VpStatusType status;
	VpProfilePtrType AC_profile;

	DEBUG_API_PRINT();

	PRINT_MSG(" <<<<<<<<< %s Country %d >>>>>>>>>\n",__FUNCTION__, country);

#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
	if (pLine->codec_type == VP_OPTION_WIDEBAND) 
		PRINT_MSG("line#%d VP_OPTION_WIDEBAND\n",pLine->ch_id);
	else
#endif
		PRINT_MSG("line#%d VP_OPTION_NARROWBAND\n",pLine->ch_id);

	switch(country)
	{
		case COUNTRY_AUSTRALIA:	
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = LE880_AC_FXS_RF14_AU;
			else
#endif
				AC_profile = LE880_AC_FXS_RF14_AU;
			break;

		case COUNTRY_BE:	/* Belgium*/
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = LE880_AC_FXS_RF14_BE;
			else
#endif
				AC_profile = LE880_AC_FXS_RF14_BE;
			break;
			
		case COUNTRY_CN:	/* China  */
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = LE880_AC_FXS_RF14_CN;
			else
#endif
				AC_profile = LE880_AC_FXS_RF14_CN;
			break;
		
		case COUNTRY_GR:	/* German */
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = LE880_AC_FXS_RF14_DE;
			else
#endif
				AC_profile = LE880_AC_FXS_RF14_DE;
			break;

		case COUNTRY_FL:	/* Finland*/
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = LE880_AC_FXS_RF14_FI;
			else
#endif
				AC_profile = LE880_AC_FXS_RF14_FI;
			break;
			
		case COUNTRY_FR:	/* France */
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = LE880_AC_FXS_RF14_FR;
			else
#endif
				AC_profile = LE880_AC_FXS_RF14_FR;
			break;
			
		case COUNTRY_IT:	/* Italy  */
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = LE880_AC_FXS_RF14_IT;
			else
#endif
				AC_profile = LE880_AC_FXS_RF14_IT;
			break;			

		case COUNTRY_JP:	/* Japan  */
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = LE880_AC_FXS_RF14_JP;
			else
#endif
				AC_profile = LE880_AC_FXS_RF14_JP;
			break;

		case COUNTRY_SE:	/* Sweden */
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = LE880_AC_FXS_RF14_SE;
			else
#endif
				AC_profile = LE880_AC_FXS_RF14_SE;
			break;

		case COUNTRY_HK:
		case COUNTRY_TW:	
		case COUNTRY_UK:
		case COUNTRY_USA:
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = DEF_LE880_AC_PROFILE;
			else
#endif
				AC_profile = DEF_LE880_AC_PROFILE;
			PRINT_MSG("Set to default SLIC impedance 600 ohm.\n");
			break;

		default:
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
			if (pLine->codec_type == VP_OPTION_WIDEBAND)
				AC_profile = DEF_LE880_AC_PROFILE;
			else
#endif
				AC_profile = DEF_LE880_AC_PROFILE;
			PRINT_Y("country wasn't defined. \
						Set to default SLIC impedance 600 ohm.\n");
			break;
	}

	status = VpConfigLine( pLine->pLineCtx, 
						   AC_profile,
						   VP_PTABLE_NULL,
						   VP_PTABLE_NULL );

	if ( status == VP_STATUS_SUCCESS ) {
		pLine->pAC_profile = AC_profile;
		pLine->AC_country  = country;
	}
	
	return status;
}

VpStatusType Ve880SetFxsAcProfileByBand(RTKLineObj *pLine, int pcm_mode)
{
    VpStatusType status;

	DEBUG_API_PRINT();

	status = Ve880SetImpedenceCountry(pLine, pLine->AC_country);
	return status;
    
}

/*****************************************************************/
#if 0
VpProfilePtrType Ve880RingProfile(uint8 profileId)
{
    VpProfilePtrType ring_profile = VP_PTABLE_NULL;                                                        
	PRINT_Y("%s(%d) Not support yet!\n",__FUNCTION__,profileId);
	return ring_profile;
}
#endif

#if 0
VpProfilePtrType Ve880AcProfile(uint8 profileId)
{
    VpProfilePtrType AC_profile = DEF_LE880_AC_PROFILE;

	PRINT_Y("%s(%d) Not support yet!\n",__FUNCTION__,profileId);
    return AC_profile;
}   
#endif

#ifdef DEBUG_API
int Ve880_ver(int deviceId)
{
	#if 1 // read revision
    unsigned char res[14]={0};
    int i;
    uint8 reg, len;
        
    reg = 0x73;
    len= 2;
    VpMpiCmd(deviceId, 0x3, reg, len, res);
    printk("Revision: ");
        
    for (i=0; i<len; i++)
        printk("\nbyte%d = 0x%x", i, res[i]);
    printk("\n");
	#endif

	return 0;
}
#endif


