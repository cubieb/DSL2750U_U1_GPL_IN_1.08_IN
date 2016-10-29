		/******************************************************
		 *						      *
		 *	Realtek VoIP-ATA DTMF DECODER HEADER FILE     *
		 *						      *
		 *	Author : thlin@realtek.com.tw		      *
		 *						      *
		 *	Date: 2005-10-11			      *
		 *						      *
		 *	Copyright 2005 Realtek Semiconductor Corp.    *
		 *						      *
		 ******************************************************/

#ifndef DTMF_DEC_H
#define DTMF_DEC_H

#include "../../include/rtk_voip.h"
#include "fsk_det.h"


/*
 ***************************************************************************************************************************
 * Description of sturcture Dtmf_det_out:
 * Assume DTMF key "6" is pressed for several seconds, digit response "6" only one time when DTMF detecter detect it. 
 * However, digitOnOff response result of each detection. I.e. digitOnOff response "6" while DTMF key "6" is still pressed.
 ***************************************************************************************************************************
 */

typedef enum
{
	DTMF_POWER_LEVEL_0DBM = 0,
	DTMF_POWER_LEVEL_MINUS_1DBM,
	DTMF_POWER_LEVEL_MINUS_2DBM,
	DTMF_POWER_LEVEL_MINUS_3DBM,
	DTMF_POWER_LEVEL_MINUS_4DBM,
	DTMF_POWER_LEVEL_MINUS_5DBM,
	DTMF_POWER_LEVEL_MINUS_6DBM,
	DTMF_POWER_LEVEL_MINUS_7DBM,
	DTMF_POWER_LEVEL_MINUS_8DBM,
	DTMF_POWER_LEVEL_MINUS_9DBM,
	DTMF_POWER_LEVEL_MINUS_10DBM,
	DTMF_POWER_LEVEL_MINUS_11DBM,
	DTMF_POWER_LEVEL_MINUS_12DBM,
	DTMF_POWER_LEVEL_MINUS_13DBM,
	DTMF_POWER_LEVEL_MINUS_14DBM,
	DTMF_POWER_LEVEL_MINUS_15DBM,
	DTMF_POWER_LEVEL_MINUS_16DBM,
	DTMF_POWER_LEVEL_MINUS_17DBM,
	DTMF_POWER_LEVEL_MINUS_18DBM,
	DTMF_POWER_LEVEL_MINUS_19DBM,
	DTMF_POWER_LEVEL_MINUS_20DBM,
	DTMF_POWER_LEVEL_MINUS_21DBM,
	DTMF_POWER_LEVEL_MINUS_22DBM,
	DTMF_POWER_LEVEL_MINUS_23DBM,
	DTMF_POWER_LEVEL_MINUS_24DBM,
	DTMF_POWER_LEVEL_MINUS_25DBM,
	DTMF_POWER_LEVEL_MINUS_26DBM,
	DTMF_POWER_LEVEL_MINUS_27DBM,
	DTMF_POWER_LEVEL_MINUS_28DBM,
	DTMF_POWER_LEVEL_MINUS_29DBM,
	DTMF_POWER_LEVEL_MINUS_30DBM,
	DTMF_POWER_LEVEL_MINUS_31DBM,
	DTMF_POWER_LEVEL_MINUS_32DBM,
	DTMF_POWER_LEVEL_MINUS_33DBM,
	DTMF_POWER_LEVEL_MINUS_34DBM,
	DTMF_POWER_LEVEL_MINUS_35DBM,
	DTMF_POWER_LEVEL_MINUS_36DBM,
	DTMF_POWER_LEVEL_MINUS_37DBM,
	DTMF_POWER_LEVEL_MINUS_38DBM,
	DTMF_POWER_LEVEL_MINUS_39DBM,
	DTMF_POWER_LEVEL_MINUS_40DBM

}TdtmfPowerLevel;

typedef enum
{
	DTMF_TWIST_4DB = 0x33333333,		//0x80000000*0.4	= 0x19990000 (-4dB)   
	DTMF_TWIST_6DB = 0x20000000,            //0x80000000*0.25	= 0x19990000 (-6dB)   
	DTMF_TWIST_8DB = 0x14467381,            //0x80000000*0.1584	= 0x14490000 (-8dB)   
	DTMF_TWIST_10DB = 0x0CCCCCCC,           //0x80000000*0.1	= 0x14490000 (-10dB)  
	DTMF_TWIST_12DB	= 0x0810624D,           //0x80000000*0.063	= 0x14490000 (-12dB)  
	DTMF_TWIST_14DB	= 0x04FDF3B6,           //0x80000000*0.039	= 0x9990000 (-14dB)   
	DTMF_TWIST_16DB	= 0x0333333	        //0x80000000*0.025	= 0x3330000 (-16dB)   

}TdtmfTwistLevel;


/* larger percent, more strict DTMF detection */
typedef enum
{
	DTMF_90_PERCENT = 115,
	DTMF_80_PERCENT = 102,
	DTMF_70_PERCENT = 90,
	DTMF_60_PERCENT = 77,
	DTMF_50_PERCENT	= 65,
	DTMF_40_PERCENT	= 52,
	DTMF_30_PERCENT	= 40,
	DTMF_20_PERCENT	= 27,
	DTMF_10_PERCENT	= 15

}TdtmfRowColPercent;


/* larger multiple, more strict DTMF detection */
typedef enum
{
	DTMF_51_MULTIPLE,
	DTMF_42_MULTIPLE,
	DTMF_32_MULTIPLE,
	DTMF_28_MULTIPLE,
	DTMF_25_MULTIPLE,
	DTMF_21_MULTIPLE,
	DTMF_16_MULTIPLE,
	DTMF_15_MULTIPLE,
	DTMF_14_MULTIPLE,
	DTMF_13_MULTIPLE,
	DTMF_11_MULTIPLE


}TdtmfRowColPowerMultiple;

typedef struct
{
	char digit;	 
	char digitOnOff; 

}Dtmf_det_out;

typedef struct
{
	unsigned short fax_v21_data[512];
	unsigned short fax_v21_windex;
}
TstVoipFaxV21;//For fax v21 data
#define FAX_V21_HEADER	0x5501
#define FAX_V21_GEP	0x5502
#define FAX_V21_NO_SIGNAL	0x5503


extern TstVoipFaxV21 stVoipFaxV21[];

void init_fax_v21(unsigned int chid);
void exit_fax_v21(unsigned int chid);
void fax_v21_time_power_ths_set(int power);

/* dtmf_dec.c function prototype */

Dtmf_det_out dtmf_dec(unsigned char *adr, uint32 page_size, unsigned char CH, long dtmf_pwr_level);
void dtmf_start(unsigned char CH);
void dtmf_stop(unsigned char CH);
//void dtmf_cid_det(unsigned char *adr, uint32 page_size, unsigned char daa_chid, long level);
TstVoipciddet dtmf_cid_check(unsigned char daa_chid, char digit);
void dmtf_cid_det_init(int chid);

/* Set the dtmf det threshld */
void dtmf_det_threshold_set(int chid, TdtmfPowerLevel threshold);

/* Get the current dtmf det threshld(dBm) */
TdtmfPowerLevel dtmf_det_threshold_dBm_get(int chid);

/* Get the dtmf det threshld value(hex) from dBm */
long dtmf_det_threshold_dBm2Hex(TdtmfPowerLevel threshold);

/*  lower dtmf detect level : */
void dtmf_lower_level(void);

/*  lower dtmf caller id detect level : */
void dtmf_cid_lower_level(int chid, int mode);

/* dtmf det twist setting */
void dtmf_det_twist_set(unsigned int chid,unsigned int fortwist, unsigned int revtwist);

/* For max row, col power percentage setting */
void dtmf_det_hilo_percentage_set(unsigned int chid, unsigned int row_stage1, unsigned int col_stage1, unsigned int row_stage2, unsigned int col_stage2);

/* For the setting of multiple of time power which "max (row+col) power" sould be */
void dtmf_det_time_power_multiple(unsigned int chid, unsigned int multiple);

#endif
