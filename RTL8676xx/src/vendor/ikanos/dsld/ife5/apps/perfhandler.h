 
/************************************************************************************************ 
		Workfile:   perfhandler.h   
************************************************************************************************* 
 
 FIRMWARE,	Copyright (C) 2000-2002 Ikanos Communication, Fremont, CA 
			All rights reserved. 
 
			The information and contents of this file are the proprietary information of Ikanos  
			Communication and may not be disclosed or used without the formal written approval of 
			Ikanos Communication. 
************************************************************************************************* 
$Revision: 1.1 $ 
$Date: 2008/07/18 11:08:38 $ 
************************************************************************************************/ 
 
 
#ifndef _PERFHANDLER_H_ 
#define _PERFHANDLER_H_ 
 
 
/* Include fiels */ 
#include <time.h> 
#include <dapi/dapi_parms.h>
 
#define MAX_CRC_CNT_PER_INTVL   ((400 * RPT_INTVL_SEC) - 5)
#define LOS_DELTA_THRESHOLD (15)

#define FRAMES_PER_SEC (4000)
#define ADSL12_FRAMES_PER_SUPERFRAME (68)
#define ADSL2P_FRAMES_PER_SUPERFRAME (68)
#define VDSL1_FRAMES_PER_SUPERFRAME (10)
#define VDSL2_FRAMES_PER_SUPERFRAME (256)

#define ADSL12_SUPERFRAMES_PER_SEC ((FRAMES_PER_SEC + (ADSL12_FRAMES_PER_SUPERFRAME >> 1)) / ADSL12_FRAMES_PER_SUPERFRAME)
#define ADSL2P_SUPERFRAMES_PER_SEC ((FRAMES_PER_SEC + (ADSL2P_FRAMES_PER_SUPERFRAME >> 1)) / ADSL2P_FRAMES_PER_SUPERFRAME)
#define VDSL1_SUPERFRAMES_PER_SEC ((FRAMES_PER_SEC + (VDSL1_FRAMES_PER_SUPERFRAME >> 1)) / VDSL1_FRAMES_PER_SUPERFRAME)
#define VDSL2_SUPERFRAMES_PER_SEC ((FRAMES_PER_SEC + (VDSL2_FRAMES_PER_SUPERFRAME >> 1)) / VDSL2_FRAMES_PER_SUPERFRAME)
 
#define MAX_15MIN_HISTORY_REGISTERS	16 
#define	MAX_24HOUR_HISTORY_REGISTERS	7 
							   
 
#define BYTE	unsigned char 
 
#define ENABLE_PERIODIC_REPORTS		0x00 
#define DISABLE_PERIODIC_REPORTS	0x01 
#define ENABLE_ALARM_COUNTERS		0x02 
#define ENABLE_SYSTEM_STATUS		0x04 
#define MAX_COUNT_BYTE				0xff 
#define MAX_COUNT_16BIT				0xffff 
#define MAX_COUNT_32BIT				0xffffffff 
#define ALARMS_FREQUENCY			500			/* Alarm counters will be reported by BME every 500ms */ 
#define BOUNDARY_15MINUTE			(15*60)		/* Number of Seconds in 15 Minutes */ 
#define BOUNDARY_24HOUR				(24l*60l*60l)	/* Number of Seconds in 24 hours */ 
#define TIME_ERROR					-1 
#define PORT_STABILITY_TIME			10 			/* In Seconds */ 
#define MAX_CRC_ERRORS_FOR_ES		1 
#define MAX_CRC_ERRORS_FOR_SES		18 
#define MAX_SES_FOR_UAS				10 
#define MAX_COUNT					0xffffffff	/* Maximum count that can be hold in counter registers */ 
#define ALARM_COUNT_REPORT			0x2 
 
#define LAST_TR1_HISTORY_REGISTER	MAX_15MIN_HISTORY_REGISTERS 
#define LAST_TR2_HISTORY_REGISTER	MAX_24HOUR_HISTORY_REGISTERS 
 
#define PORT_ENABLED		0x01  
#define PORT_ACTIVATED 		0X02 
#define LINK_UP				0X04 
#define PORT_IN_SHOWTIME	0x07 
 
/* TOCHECK : for getting the number of ticks in a second */ 
#define TICKS_PER_SECOND	CLOCKS_PER_SEC 
 
typedef enum eReportType 
{ 
	NE_15MIN = 0,		/* 15-Minute accumulation period for near end */ 
	FE_15MIN, 
	NE_24HOUR, 		 
	FE_24HOUR		/* 24-Hour accumulation period for the far end */	 
}eReportType; 
 
typedef  enum eParaType 
{ 
	 CV_SLOW_15MIN=0, 
	 CV_FAST_15MIN, 
	 CV_SLOW_24HOUR, 
	 CV_FAST_24HOUR, 
	 FEC_SLOW_15MIN, 
	 FEC_FAST_15MIN, 
	 FEC_SLOW_24HOUR, 
	 FEC_FAST_24HOUR, 
	 HECV_SLOW_5MIN, 
	 HECV_FAST_15MIN, 
	 HECV_SLOW_24HOUR, 
	 HECV_FAST_24HOUR, 
	 ERR_SEC_15MIN, 
	 ERR_SEC_24HOUR, 
	 SEV_ERR_SEC_15MIN, 
	 SEV_ERR_SEC_24HOUR, 
	 UASEC_15MIN, 
	 UASEC_24HOUR 
}eParaType; 
 
/* Line State: Line will be declared unavailable at the onset of 10 contiguos SE Seconds */ 
typedef enum eLINE_STATE 
{ 
	UNAVAILABLE = 0,									 
	AVAILABLE 
}eLINE_STATE; 
 
 
typedef enum eFAIL_STATE 
{ 
	FAIL_INACTIVE=0,				/* Healthy State */ 
	DEFECT_START,					/* Set once the defect starts */ 
	FAIL_ACTIVE,					/* Set once the failue has been declared */ 
	DEFECT_STOP					/* Set once the line recoving to normal state */ 
}eFAIL_STATE; 
 
typedef struct FailStatus16_tag 
{ 
	U16 previous_value;			/* Previous count value got from BME */ 
	eFAIL_STATE fail_state;			/* Present failure state */ 
	time_t defect_start_time;		/* Time at which defect starts on the line */ 
	time_t defect_stop_time;		/* Time at which the line started coming to Normal state */ 
}FailStatus16_T; 
 
typedef struct FailStatus_tag 
{ 
	BYTE previous_value;			/* Previous count value got from BME */ 
	eFAIL_STATE fail_state;			/* Present failure state */ 
	time_t defect_start_time;		/* Time at which defect starts on the line */ 
	time_t defect_stop_time;		/* Time at which the line started coming to Normal state */ 
}FailStatus_T; 
 
typedef struct Failures_tag 
{ 
	FailStatus_T Los;				/* Loss of Signal Count */ 
	FailStatus16_T Lof;				/* Loss of Frame Count*/ 
	FailStatus_T Crc;				/* line CRC */ 
	FailStatus_T NCDSlow;			/* No Cell Delineation for the slow path */ 
	FailStatus_T NCDFast;			/* No Cell Delineation for the fast path */ 
	FailStatus_T LCDSlow;			/* Loss of Cell Delineation for the slow path */ 
	FailStatus_T LCDFast;			/* Loss of Cell Delineation for the fast path */  
	FailStatus_T LPR;				/* Loss of power, only for Far End */ 
/*	BYTE PortAlarm;*/				/* Set to '1', if any failure has been reported */ 
    FailStatus_T SixPt;             /* Six PT failure and over raid */
}Failures_T; 
 
/*	Each History register circular buffer will have read and write pointers. it wraps up at the end of the  
	boundary. The oldest value stored will be discarded everytime it gets the latest value	*/ 
 
typedef struct ParameterStatus_tag 
{ 
	BYTE previous_value; 
	BYTE inhibited; 
	BYTE fail_onesecond_flag;		/* Set in the one second routine whenever there is a failure */ 
	BYTE overflow_flag; 
	UINT32 current_reg;	 
	UINT32 old_count; 
}ParameterStatus_T; 
 
typedef struct PerfParameters_tag 
{ 
	/* Line Related Performance Parameters */ 
	ParameterStatus_T	CvSlow15Min;			/* Code Violation Counter for slow data stream for 15 minute accumulation period */ 
	ParameterStatus_T	CvFast15Min;			 
	ParameterStatus_T	CvSlow24Hour;			/* Code Violation Counter for slow data stream for 24 hour accumulation period*/ 
	ParameterStatus_T	CvFast24Hour;			 
 
	ParameterStatus_T	FecSlow15Min;			 
	ParameterStatus_T	FecFast15Min;			/* FEC Counter for fast data stream for 15 minute accumulation period*/ 
	ParameterStatus_T	FecSlow24Hour;			 
	ParameterStatus_T	FecFast24Hour;			/* FEC Counter for fast data stream for 24 hour accumulation period*/ 
 
	ParameterStatus_T	ErrSecond15Min;			/* Errored Seconds for 15 minute accumulation period */			 
	ParameterStatus_T	ErrSecond24Hour; 
	 
	ParameterStatus_T	SevErrSecond15Min;		/* Severe Errored Seconds for 15 minute accumulation period */ 
	ParameterStatus_T	SevErrSecond24Hour; 
	 
	ParameterStatus_T	UASecond15Min;						 
	ParameterStatus_T	UASecond24Hour;			/* Unavailable Seconds for 24 hour accumulation period */	 
	 
	ParameterStatus_T	FecSecond15Min;			/* FEC Seconds count for 15 minute accumulation period */			 
	ParameterStatus_T	FecSecond24Hour; 
 
	/* ATM path Related Performance Parameters */ 
	ParameterStatus_T	HecvSlow15Min;			/* HEC violation counter for slow data stream for 15 minute accumulation period*/ 
	ParameterStatus_T	HecvFast15Min;			 
	ParameterStatus_T	HecvSlow24Hour;			/* HEC violation counter for slow data stream for 24 hour accumulation period*/ 
	ParameterStatus_T	HecvFast24Hour;			 
	time_t StartTime;					/* Start time for waiting till the end of Stability time */ 
	BYTE FirstTime;						/* Set during the initialisantion. And is reset after the Port comes to Show Time */ 
}PerfParameters_T; 
 
/*	Structure declarations for reporting failures to Management Unit.  
	'1' Indicates a Failure, '0' Indicates healthy state			*/ 
typedef struct FailReport_tag 
{ 
	BYTE LossOfSignal;			/* Loss of Signal failure */ 
	BYTE LossOfFrame;			/* Loss of Frame failure  */ 
	BYTE NCDSlow;			/* No Cell Delineation for the slow path */ 
	BYTE NCDFast;			/* No Cell Delineation for the fast path */ 
	BYTE LCDSlow;			/* Loss of Cell Delineation for the slow path */ 
	BYTE LCDFast;			/* Loss of Cell Delineation for the fast path */  
	BYTE LossOfPower;		/* Loss of power at the far end , its not used for near end */ 
	BYTE SendReport;		/* Should be reset once the failures are reported */		 
	BYTE PortAlarm;			/* This is set if any of the above is set or cleared 
					    Should be rest after recognizing the failure */ 
 
	struct tm report_time; 
}FailReport_T; 
 
typedef struct TRparameters_tag 
{ 
	BYTE ReportStart;				/* Flag to indicate the start of the report */ 
	BYTE SendReport;				/* Set is when a report is ready for sending to Management Unit */ 
	BYTE StackReg;					/* Set to indicate the stacking of current register into history register */ 
	time_t StartTime;				/* Calendar Time in seconds : Report Staring Time */ 
}TRparameters_T; 
 
typedef struct History15Min_tag 
{ 
	BYTE overflow_flag;	/* This is set when the current register contents overflow */ 
	UINT32 current_reg;	/* Holds the current 15 Min accumulated value */ 
	UINT32 history_reg[MAX_15MIN_HISTORY_REGISTERS]; 
	BYTE read_index; 
	BYTE write_index; 
}HistoryReg15Min_T; 
 
typedef struct History24Hour_tag 
{ 
	BYTE overflow_flag;	/* This is set when the current register contents overflow */ 
	UINT32 current_reg;	/* Holds the current 24 hour accumulated value */ 
	UINT32 history_reg[MAX_24HOUR_HISTORY_REGISTERS]; 
	BYTE read_index;	 
	BYTE write_index; 
}HistoryReg24Hour_T; 
 
typedef struct ThresholdReport15Min_tag 
{ 
	HistoryReg15Min_T ErrSeconds; 
	HistoryReg15Min_T SevErrSeconds; 
	HistoryReg15Min_T UASeconds; 
	BYTE InvalidFlag;		 		 
	struct tm report_time;			/* The time of occurrence of the report */ 
}ThresholdReport15Min_T; 
 
typedef struct ThresholdReport24Hour_tag 
{ 
	HistoryReg24Hour_T ErrSeconds; 
	HistoryReg24Hour_T SevErrSeconds; 
	HistoryReg24Hour_T UASeconds; 
	BYTE InvalidFlag;		 		 
	struct tm report_time;			/* The time of occurrence of the report */ 
}ThresholdReport24Hour_T; 
 
/* Performance Monitoring structures for a port */ 
typedef struct tPerformance  
{ 
	Failures_T NeFails; 
	Failures_T FeFails; 
	 
	/* Fail Report to Management Unit */ 
	FailReport_T NeFailReport; 
	FailReport_T FeFailReport; 
	 
	/* Performance Parameters */ 
	PerfParameters_T NePerfPara; 
	PerfParameters_T FePerfPara; 
 
	/* Threshold Report Parameters */ 
	TRparameters_T NeTRpara15Min; 
	TRparameters_T NeTRpara24Hour; 
	 
	/* Threshold Report Parameters */ 
	TRparameters_T FeTRpara15Min; 
	TRparameters_T FeTRpara24Hour; 
 
	/* 15 Minute Threshold Reports */ 
	ThresholdReport15Min_T Ne15MinTR; 
	ThresholdReport15Min_T Fe15MinTR; 
 
	/* 24Hour Threshold Reports */ 
	ThresholdReport24Hour_T Ne24HourTR; 
	ThresholdReport24Hour_T Fe24HourTR; 
 
	eLINE_STATE NeLineState, FeLineState; 
 
}tPerformance; 
 
// Used for calculating CRC threshold  
typedef struct 
{ 
	unsigned char status; 
	unsigned char bmeNum; 
	unsigned char portNum; 
  unsigned char startCnt; 
  unsigned char pollCmdSnd; 
  unsigned char losFailure; 
  unsigned short iniCrcTCrossCnt[2]; 
  unsigned short iniCrcCnt[2]; 
  unsigned int expIniRxFramePerSec[2]; 
  unsigned int tickCount[2]; 
  unsigned int iniRxFrameCnt[2]; 
  unsigned short pRxFrame[2]; 
  unsigned short pCrcCnt[2]; 
  unsigned short pDropByteCnt[2]; 
	int FastChan; 
	int SlowChan; 
	int LedActivate; 
  unsigned short linkHealthCnt; 
  unsigned short uCutReportFlagOld;
} uPortStruct; 

extern void resetPerfPara (ipos_port_id pid); 
 
#endif	/* _PERFHANDLER_H_ */ 
