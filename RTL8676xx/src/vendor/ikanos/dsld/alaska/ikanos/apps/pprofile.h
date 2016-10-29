/**************************************************************
$Workfile:   pprofile.h  $
***************************************************************
**							    										
**	Filename    : pprofile.h	  		    									
**	Description : c header file defining the port profile management	 					
**	Author	    : jeya					    									
**	Date	    : 26 March 2001					 								
**	Notes	    :		  	   		    										
**  References  : host api document prepared by Tom.					    											 	
**	Revision History :				    										
**							    												
****************************************************************/

/***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
***************************************************************/

#ifndef __pprofile_h__
#define __pprofile_h__

/* #includes */

/* #defines */
#define MAX_PORT_PROFILES 		3
#define MAX_CHANNEL_NUMBER		63 
#define MAX_LINE_TYPES			8
#define MAX_DSL_MODES			5
#define UPDATE_IN_FLASH         1   
#define DO_NOT_UPDATE           -1

#define PROFILE_CHKSTR "multi prof\0"
/* enum var type - profile management return values */
typedef enum eProfileMgmtRv {
		PROFILE_MGMT_REQ_SUCCESS = 0x0,
		WRONG_PROFILE_NUMBER,
		WRONG_PARAM_VALUE
	} eProfileMgmtRv;

/* enum var type - port provision paramater type */
typedef enum ePortProvisionParmType {
		INTERLEAVED_CHANNEL 	=0x0,
		FAST_CHANNEL        	=0x1,
		LINE_TYPE		=0x2,
		DSL_MODE		=0x3,
		RATE_ADAPTATION_MODE	=0x4,
		INTERLEAVED_PERCENTAGE	=0x5,
		UPSTREAM_MAX_RATE_FAST	=0x6,
		UPSTREAM_MIN_RATE_FAST	=0x7,
		UPSTREAM_MAX_RATE_SLOW	=0x8,
		UPSTREAM_MIN_RATE_SLOW	=0x9,
		UPSTREAM_MAX_INTER_DELAY=0xa,
		DOWNSTREAM_MAX_RATE_FAST=0xb,
		DOWNSTREAM_MIN_RATE_FAST=0xc,
		DOWNSTREAM_MAX_RATE_SLOW=0xd,
		DOWNSTREAM_MIN_RATE_SLOW=0xe,
		DOWNSTREAM_MAX_INTER_DELAY=0xf,
		NUM_PARITY_FAST		=0x10,
		NUM_PARITY_SLOW		=0x11,
		US_TARGET_NOISE_MARGIN	=0x12,
		DS_TARGET_NOISE_MARGIN	=0x13,
		MIN_NOISE_MARGIN	=0x14,
       	BAND_PLAN		=0x15,
       	NUM_RFI_BAND		=0x16,
       	BAND_1_START		=0x17,
       	BAND_1_STOP		=0x18,
       	BAND_2_START		=0x19,
       	BAND_2_STOP		=0x1a,
       	BAND_3_START		=0x1b,
       	BAND_3_STOP		=0x1c,
       	BAND_4_START		=0x1d,
       	BAND_4_STOP		=0x1e,
       	BAND_5_START		=0x1f,
       	BAND_5_STOP		=0x20,
		RFI_BANDS		=0x21,
		PORT_OPTION		=0x22,
	    TONE_CONFIG=0x23,
        UPSTREAM_MIN_PROTECTION=0x24,
        DOWNSTREAM_MIN_PROTECTION=0x25,
#ifdef CPE_40
        NETWORK_FRAMING_MODE=0x26,
#endif
        DISABLE_MICRO_CUT_DS=0x27,
	} ePortProvisionParmType;

typedef enum ePortStartParmType {
		PORT_START_PARAM = 0x0	
	} ePortStartParmType;

typedef enum ePortStopParmType {
		PORT_STOP_PARAM = 0x0	
	} ePortStopParmType;

struct line_type_descr
{
    U32 type;
    char *name;
};

extern struct line_type_descr ui_line_type[]; 


/* Profile management function prototypes	*/

/* function responding to TL command to set the profile parameters	*/
eProfileMgmtRv setPortParameter (U8 bProfileNum, U8 bPortCmd, U8 bParamType, U32 *cPtrParamVal);

/* Get the defult port protile hardwired in the code */
ipos_port_profile *getDefaultPortProfile (void);

/* Get the port profile indentified by the the profile number	*/
ipos_port_profile *getPortProfile (U8 bProfileNum);

/* set the default profile for all port profiles */
void setDefaultPortProfile 		(void);

/* set the cpe default profile to port profile number 2	*/
void setCpeDefaultPortProfile 	(int profilenum);

/* set the CO default profile to port profile number 1  */
void setCoDefaultPortProfile 	(int profilenum);

/* set user definded port profile to port profile number bProfileNum */
eProfileMgmtRv setUserDefinedPortProfile (U8 bProfileNum);

/* display the port profile with bProfileNum */
void getPortProfileDisplay (U8 bProfileNum);
/* display the RFI band Setting */
void displayRFIBandSetting (U8 band, char *binString);

/* save all three port profiles into flash */
extern void updateFlashProf(void);

/* set port profile for menu based UI	*/
extern int setPortProfile(U8 *ptrTlStr);

extern ipos_port_profile sPortProfiles[];
extern int ProfileMapping [];
extern void readProfileFromFlash (U8 ProfileNum);
extern void savePreviousProfiles (int version);
extern void writeProfileToFlash (void);
extern int memcpyProfile (int SrcProfileNum, int DestProfileNum);

#endif /* __pprofile_h__*/

/* END_OF_FILE */
