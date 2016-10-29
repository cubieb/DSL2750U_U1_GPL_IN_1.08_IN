/**************************************************************
$Workfile:   dapi_parms.h  $
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
$Date: 2008/07/18 11:09:32 $
***************************************************************/

#ifndef _API_PARMS_H_
#define _API_PARMS_H_

#include <api/bmeconfig.h>
#include "dapi_define.h"

/**** Defines ***/

/* variable types */
#ifndef __VAR_DEFINITIONS__ 
#define __VAR_DEFINITIONS__
#define UINT64  unsigned long long int
#define LONG	signed long
#define UINT32  unsigned long
#define SHORT	signed short
#define UINT16  unsigned short
#define UINT8  unsigned char
#define BYTE	unsigned char
#define WORD  unsigned short
#endif /* __VAR_DEFINITIONS__ */

// Notch Band defines
#define STOP_BAN_640K_3BAND   148
#define STOP_BAN_1100K_3BAND  255
#define STOP_BAN_640K_4BAND   74
#define STOP_BAN_1100K_4BAND  128

#define MAX_EOC_DATA_STR_LENGTH 250
#define MAX_NUM_BANDS        (5) 

#define DAPI_MAX_PSD_LEN (85) // Keep this in sync with IPOS_MAX_PSD_LEN

/**** Enumerations ***/

/* Bme States */
typedef enum eBmeState
	{
		INACTIVE = 0,
		SW_DNLD_IN_PROGRESS,
		SW_DNLD_DONE,
		SW_DNLD_VERIFICATION,
		ACTIVE
	} eBmeState;

/* Bme Download Verification State */
typedef enum eBmeDnldVerifyState 
	{
		DW_T_BIT_UNKNOWN_STATE = 0,
		DW_T_BIT_RESET,
		DW_T_BIT_SET,
		DW_T_BIT_CLEARED
	} eBmeDnldVerifyState;

/* EMI Module configuration values */
/* memory type */
typedef enum eBmeMemType 
	{
	MEM_SRAM = 0,	/* SRAM */
	MEM_SDRAM		/* SDRAM */
	} eBmeMemType;

/* sram memory size */
typedef enum eBmeSramMemSize
	{
	SRAM_1MB = 0,	/* SRAM - 1 MB size */
	SRAM_2MB		/* SRAM - 2 MB size */
	} eBmeSramMemSize;

/* sdram memory size */
typedef enum eBmeSdramMemSize 
	{
	SDRAM_16MB = 0,	/* SDRAM - 16 MB size */
	SDRAM_22MB,		/* SDRAM - 22 MB size */
	SDRAM_8MB		/* SDRAM - 8 MB size */
	} eBmeSdramMemSize;

/* CAS Latency for SDRAM */
typedef enum eBmeCasLat
	{
	CAS_LAT_3_CLOCKS = 0,	/* 3 clocks */
	CAS_LAT_2_CLOCKS		/* 2 clocks */
	} eBmeCasLat;

/* RAS to CAS delay for SDRAM */
typedef enum eBmeRCDel
	{
	RC_DEL_2_CLOCKS = 0,	/* 2 clocks */
	RC_DEL_3_CLOCKS			/* 3 clocks */
	} eBmeRCDel;

/* Minimum row precharge time for SDRAM */
typedef enum eBmeRowPre
	{
	ROW_PRE_2_CLOCKS = 0,	/* 2 clocks */
	ROW_PRE_3_CLOCKS		/* 3 clocks */
	} eBmeRowPre;

/* Minimum row active time for SDRAM */
typedef enum eBmeRowAct
	{
	ROW_ACT_6_CLOCKS = 0,	/* 6 clocks */
	ROW_ACT_7_CLOCKS		/* 7 clocks */
	} eBmeRowAct;

/* Minimum row cycle time for SDRAM */
typedef enum eBmeRowCyc
	{
	ROW_CYC_11_CLOCKS = 0,	/* 11 clocks */
	ROW_CYC_10_CLOCKS		/* 10 clocks */
	} eBmeRowCyc;

/* Return values for BME commands */
typedef enum eBmeRv
	{
	ACK_SUCCESS			=0x01,
	NACK_BUSY			=0x82,
  NACK_DEACTIVATED =0x83,
	NACK_ERROR			=0x84,		
	NACK_NOT_IMPLEMENTED=0x85,
	NACK_TIMEOUT		=0x86,
	NACK_INVALID_DEST	=0x88,
	NACK_INVALID_OPCODE	=0x89,
	NACK_INVALID_LENGTH	=0x8A,
	NACK_INVALID_DATA	=0x8B,
	NACK_INVALID_STATE = 0x8C,
	NACK_DB_ERROR = 0x8D,
	NACK_ACTIVATE_FAIL	=0x8E,
	NO_COMMAND_SENT		=0x10,
	DMA_NOT_DONE		=0x11,
	DMA_DONE			=0x12,
	PENDING_BME_RV		=0x13,
	DMA_WRITE_DONE		=0x14
	} eBmeRv;

/* Return values for Host APIs */
typedef enum eApiRv
	{
	IN_PROGRESS = 1,	/* Message has been successfully sent. Response from BME is 
							expected later.*/
	FAIL,				/* API execution fails */
	PARMS_UNDEFINED,	/* Some API parameters undefined */
	TxCmdTIMEOUT,		/* DoTxCmd fails, timeout */
	COMPLETE,			/* API has been executed successfully. If any BME messages 
							were sent, the transmission was successful and the action was 
							performed successfully on the BME*/
	CMD_TIMER_EXPIRE,	/* Maximum response time specified for the API command expired.
							No response from BME. Time out the command at the 
							APPLICATION level */
	AUTONOMOUS,			/* Autonomous message from BME, no ApiRv related to message*/
	TxCmdFAIL
    } eApiRv;



/**** Data Structures ***/



/**********************************/
/* Port specific API Parameters */
/**********************************/



/* Port Profile Structure */
typedef struct tPortProfile {

	int PortProfileNum;

	struct tPortProvision
		{
		BYTE	InterleavedChannel;
		BYTE	FastChannel; 
		BYTE	LineType;
		BYTE	DslMode;
		BYTE	RateAdaptationMode;
		BYTE	InterleavedPercentage;  	
		unsigned long UpstreamMaxRateFast;
		unsigned long UpstreamMinRateFast;
		unsigned long UpstreamMaxRateSlow;  
		unsigned long UpstreamMinRateSlow;   
		BYTE	UpstreamMaxInterDelay;	
		unsigned long DownstreamMaxRateFast;   
		unsigned long DownstreamMinRateFast;   
		unsigned long DownstreamMaxRateSlow;
		unsigned long DownstreamMinRateSlow;
		BYTE	DownstreamMaxInterDelay;
		BYTE	NumParityFast;
		BYTE	NumParitySlow;
	/*	BYTE	NumCodeWord;	*/		
		WORD UsTargetNoiseMargin;
		WORD DsTargetNoiseMargin;
		WORD MinNoiseMargin;
/*		WORD UpNoiseMargin;
		WORD UpNoiseInterval;
		WORD DownNoiseMargin;
		WORD DownNoiseInterval;*/
        BYTE BandPlan;
        BYTE NumRFIBand;
        WORD Band1Start;
        WORD Band1Stop;
        WORD Band2Start;
        WORD Band2Stop;
        WORD Band3Start;
        WORD Band3Stop;
        WORD Band4Start;
        WORD Band4Stop;
        WORD Band5Start;
        WORD Band5Stop;
//JK- rfi		
		BYTE RFIBand1;
		BYTE RFIBand2;
		BYTE RFIBand3;
		BYTE RFIBand4;
		BYTE RFIBand5;
        BYTE UsMinProtection;
        BYTE DsMinProtection;
#ifdef CPE_40
        BYTE framingmode;
#endif            
		BYTE DisableMicroCut;
		} PortProvParms;

	struct tPortStartReset
		{
		BYTE	PortStartParm;
		} PortStartResetParms;

	struct tPortStop
		{
		BYTE	PortStopParm;
		} PortStopParms;
	struct 
		{
		BYTE	bandModifier;
		} BandMod;

	struct
		{
		BYTE PortOption;
		BYTE Reserved1;
		BYTE Reserved2;
		BYTE Reserved3;
		} OptionParms; 

    struct
        {
            BYTE  pwrMode;
            BYTE  txPathFilterMode;
            BYTE  rxPathFilterMode;
        } IFE_BRD_CFG;
	} tPortProfile;

/* Parameter structure for a port */
typedef struct tPortParms 
	{
	
	tPortProfile *pPtrPortProfile;
	tPortProfile bmeRxPortProfile;

	/* Status Request Commands */

	struct tPortStatusBrief	
		{
		/* Return parameters from BME */
		BYTE	Actual;
		BYTE	FramingMode;
		} PortStatusBriefParms;

	struct tExpectedPortStatus
		{
		BYTE	Expected;
		} ExpectedPortStatus;

	struct tGetCounters
		{
		/* Return parameters from BME */
		unsigned long	NeFEC_S;
		unsigned long	NeFEC_F;
		WORD	NeCRC_S;
		WORD	NeCRC_F;
		BYTE	Reserved1[4];
		BYTE	NeLOS;
		WORD	NeSEF;
		WORD	NeLOM;
		BYTE	NeNCD_S;
		BYTE	NeNCD_F;
		BYTE	NeOCD_S;
		BYTE	NeOCD_F;
		BYTE	NeHEC_S;
		BYTE	NeHEC_F;
		BYTE	Reserved3[7];
		BYTE	NeBER_S;
		BYTE	NeBER_F;
                unsigned long NeFECUnCrr_S;
                unsigned long NeFECUnCrr_F;
                unsigned long numUsrCell_S;
                unsigned long numUsrCell_F;
                unsigned long numIdlCell_S;
                unsigned long numIdlCell_F;
                unsigned long numTxBlk_S;
                unsigned long numTxBlk_F;
                unsigned long numRxBlk_S;
                unsigned long numRxBlk_F;
		WORD	FeFEC_S;
		WORD	FeFEC_F;
		WORD	FeCRC_S;
		WORD	FeCRC_F;
		WORD	FeFLOS;
		WORD	FeFRDI;
		WORD	FeOCD_S;
		WORD	FeOCD_F;
		WORD	FeHEC_S;
		WORD	FeHEC_F;
		WORD	FeLPR;
		WORD	FeLOM;
		WORD	FePO;

    unsigned long  FeFecEs_S;             /* Far-end Forward Error Correction Errored Seconds (FEC ES) Bearer 1 count    */
    unsigned long  FeEs_S;                  /* Far-end Errored Seconds (ES) Bearer 1 count    */
    unsigned long  FeSEs_S;                /* Far-end Severely Errored Seconds (ES) Bearer 1 count    */
    unsigned long  FeLosEs_S;             /* Far-end LOS Errored Seconds (LOS ES) Bearer 1 count    */
    unsigned long  FeUnAvlEs_S;         /* Far-end Un Available Errored Seconds (UNAVL ES) Bearer 1 count    */
    unsigned long  FeBER_S;                /* Near-end Bit Error Rate (BER) Bearer 1 count             */
    
        WORD    sixPtLineHold;
		UINT8  uCutReportFlag;
		} GetCounterParms;

  struct tPmAlertParms
    {
    WORD NeFailures;
    WORD FeFailures;
    WORD NeTCA1;
    WORD FeTCA1;
    WORD NeTCA2;
    WORD FeTCA2;
	UINT8  uCutReportFlag;
    } PmAlertParms;

  struct tPmCounterParms
    {

    UINT8  PmPeriodId;
    UINT8  PortStartFlag;

    /* PM Alert Status */
    UINT16 NeFailures;
    UINT16 FeFailures;
    UINT16 NeTCA1;
    UINT16 FeTCA1;
    UINT16 NeTCA2;
    UINT16 FeTCA2;

    /* Near End Failure Counts */
    UINT16	NeLOS;   //	Near-end Loss Of Signal failure count
    UINT16  NeLOF;   // Near-end Loss of Frame failure count
    UINT16	NeLOM;   // Near-end Loss of Margin failure count
    UINT16	NeESE;   //	Near-end Excessive Severe Error failure count
    UINT16	NeNCD_S; //	Near-end No Cell Delineation (NCD) failure Slow count
    UINT16	NeLCD_S; //	Near-end Loss of Cell Delineation (LCD) failure Slow count
    UINT16	NeNCD_F; //	Near-end No Cell Delineation (NCD) failure Fast count
    UINT16	NeLCD_F; //	Near-end Loss of Cell Delineation (LCD) failure Fast count

    /* Far End Failure Counts */
    UINT16	FeFLOS;  //	Far-end Loss Of Signal (LOS) failure count
    UINT16	FeFRDI;	 // Far-end Remote Defect Indication (RDI) failure count
    UINT16	FeLPR;   //	Far-end Loss of Power failure count
    UINT16	FeLOM;   //	Far-end Loss of Margin failure count
    UINT16	FeESE;   //	Far-end Excessive Severe Error failure count
    UINT16	FeNCD_S; //	Far-end No Cell Delineation (NCD) failure Slow count
    UINT16	FeLCD_S; //	Far-end Loss of Cell Delineation (LCD) failure Slow count
    UINT16	FeNCD_F; //	Far-end No Cell Delineation (NCD) failure Fast count
    UINT16	FeLCD_F; //	Far-end Loss of Cell Delineation(LCD)  failure Fast count
    UINT16  FePO;    // Far-end Power off failure count

    /* Counts of Seconds with Errors */
    UINT16	NeSES;   //	Near-end Severe Errored second count
    UINT16	NeES;  	 // Near-end Errored second count
    UINT16	NeFECS;  //	Near-end FEC second count
    UINT16	NeLOSS;	 // Near-end Loss of Signal second count
    UINT16  NeUnavl;  // Near-end Unavailable second count
    UINT16	FeSES;   //	Far-end Severe Errored second count
    UINT16	FeES;    //	Fear-end Errored second count
    UINT16	FeFECS;  //	Far-end FEC second count
    UINT16	FeLOSS;  //	Far-end Loss of Signal second count
    UINT16  FeUnavl;  // Far-end Unavailable second count

    /* Near-end Anomaly Counts */
    UINT32  NeFEC_S; // Near-end Forward Error Correction (FEC) Slow Count
    UINT32  NeFEC_F; // Near-end Forward Error Correction (FEC) Fast Count
    UINT32	NeCRC_S; // Near-end Cyclic Redundancy Check (CRC) Slow count
    UINT32	NeCRC_F; // Near-end Cyclic Redundancy Check (CRC) Fast count
    UINT32	NeHEC_S; //	Near-end Header Error Control (HEC) Slow count
    UINT32	NeHEC_F; //	Near-end Header Error Control (HEC) Fast count

    /* Far-end Anomaly Counts */
    UINT32	FeFEC_S; //	Far-end Forward Error Correction (FEC) Slow count
    UINT32	FeFEC_F; //	Far-end Forward Error Correction (FEC) Fast count
    UINT32	FeBE_S;	 // Far-end Block Error (BE) Slow count
    UINT32	FeBE_F;  //	Far-end Block Error (BE) Fast count
    UINT32	FeHEC_S; //	Far-end Header Error Control (HEC) Slow count
    UINT32	FeHEC_F; //	Far-end Header Error Control (HEC) Fast count

    UINT32	numUSRcell_S;  //	Number of User Cells (USR) Slow count
    UINT32	numUSRcell_F;  //	Number of User Cells (USR) Fast count
    UINT32	numIDLEcell_S; //	Number of Idle Cells (IDLE) Slow count
    UINT32	numIDLEcell_F; //	Number of Idle Cells (IDLE) Fast count

    UINT32 numIDLEcellBe_S;  //Number of Idle Cells Bit Error Bearer 1 count
    UINT32 numIDLEcellBe_F;  //Number of Idle Cells Bit Error Bearer 0 count

    UINT32 numTxBlk_S;    // Number of TX Block Slow  count
    UINT32 numTxBlk_F;    // Number of Tx Block Fast  count
    UINT32 numRxBlk_S;    // Number of Rx Block Slow  count
    UINT32 numRxBlk_F;    // Number of Rx Block Fast  count

    UINT32 NeFECUnCrr_S;  // Near end slow FEC uncorrected count
    UINT32 NeFECUnCrr_F;  // Near end Fast FEC uncorrected count
    UINT16  sixPtLineHold;

    UINT16 numReInit;      // Number of re-initialization count
    UINT16 numInitFailure; // Number of initialization failures 
    UINT32 NeSEF;         // Near-end Severely Errored Frame (SEF) "raw" count
    } PmCounterParms;

  struct tQosCounterParms
  {
      UINT32 ClassCounters[8];
      UINT32 Ipv4PacketCount;
      UINT32 Ipv4TotalByteCount;
  } QosCounterParms;

  struct tPmConfigParms
  {

    UINT16	LOS_Threshold;
    UINT16	LOF_Threshold;
    UINT16	LPR_Threshold;
    UINT16	LOM_Threshold;
    UINT16	ESE_Threshold;
    UINT16	NCD_S_Threshold;
    UINT16	LCD_S_Threshold;
    UINT16	NCD_F_Threshold;
    UINT16	LCD_F_Threshold;

    UINT16	FeLOS_Threshold;
    UINT16	FeLOF_Threshold;
    UINT16	FeLPR_Threshold;
    UINT16	FeLOM_Threshold;
    UINT16	FeESE_Threshold;
    UINT16	FeNCD_S_Threshold;
    UINT16	FeLCD_S_Threshold;
    UINT16	FeNCD_F_Threshold;
    UINT16	FeLCD_F_Threshold;
    UINT16  FePO_Threshold;

    UINT16	FEC_Second_Threshold;
    UINT16	NeES_Second_Threshold;
    UINT16	NeSES_Second_Threshold;
    UINT16	NeLOS_Second_Threshold;
    UINT16  NeUnavl_Second_Threshold;
    UINT16	FeFEC_Second_Threshold;
    UINT16	FeES_Second_Threshold;
    UINT16	FeSE_Second_Threshold;
    UINT16	FeLOS_Second_Threshold;
    UINT16  FeUnavl_Second_Threshold;

    UINT16	FEC_S_Threshold;
    UINT16	FEC_F_Threshold;
    UINT16	CRC_S_Threshold;
    UINT16	CRC_F_Threshold;
    UINT16	HEC_S_Threshold;
    UINT16	HEC_F_Threshold;
    UINT16	FeFEC_S_Threshold;
    UINT16	FeFEC_F_Threshold;
    UINT16	FeCRC_S_Threshold;
    UINT16	FeCRC_F_Threshold;
    UINT16	FeHEC_S_Threshold;
    UINT16	FeHEC_F_Threshold;


  } PmConfigParms;

  struct tPmTriggerParms {
    UINT16 NeFailures;
    UINT16 FeFailures;
    UINT16 NeTCA1;
    UINT16 FeTCA1;
    UINT16 NeTCA2;
    UINT16 FeTCA2;
  } PmTriggerParms;

	struct tFeGetCounters
		{
		unsigned long	FeFEC_S;
		unsigned long	FeFEC_F;
		WORD			FeCRC_S;
		WORD			FeCRC_F;	
		BYTE	FeFLOS;

		} FeGetCounterParms;

	struct tPeriodicReportEnable
		{
		BYTE	ReportsEnable;
		WORD 	AlarmCountFreq;		/* Frequency of Alarm counter report in 100 ms */
		WORD 	SystemStatusFreq;	/* Frequency of System Status report in 100 ms */
		} PerReportParms;

	struct tUpdatedReportEnable
		{
		BYTE	ReportsEnable;

		} UpdatedReportParms;

	struct tPortStatusExtended
		{
		UINT32 	DsLineRate;
		UINT32	UsLineRate;
		UINT32	FastDsPayloadRate;
		UINT32 	SlowDsPayloadRate;
		UINT32	FastUsPayloadRate;
		UINT32 	SlowUsPayloadRate;
		BYTE	FastDsNumParityBytes;
		BYTE	SlowDsNumParityBytes;
		BYTE	FastUsNumParityBytes;
		BYTE	SlowUsNumParityBytes;
		UINT16	DsDelay;
		UINT16	UsDelay;
		UINT16	TransmitPower;
		UINT16	DsTrainingMargin;
		UINT16  DsAttainableRate;
		UINT16  DsAttainableLineRate;
		UINT16	SlowDsLineProtection;
		UINT16	UsTrainingMargin;
		UINT16	UsAttainableRate;
		UINT16	UsAttainableLineRate;
		UINT16  SlowUsLineProtection;
		BYTE	NeItuVendorId[8];
		BYTE	FeItuVendorId[8];
		UINT16	FeTransmitPower;
        UINT16  LoopLength; 
        UINT16  ghsEndLoopLength; 
        UINT16  ghsFarEndLoopLength; 
		UINT16	SlowUsFFTProtection;
		UINT16	SlowDsFFTProtection;
		BYTE	FastDsCodeWordSize;
		BYTE	SlowDsCodeWordSize;
		BYTE	FastUsCodeWordSize;
		BYTE	SlowUsCodeWordSize;
		BYTE	DsInterleaverDepth;
		BYTE	UsInterleaverDepth;
		BYTE	DsInterleaverBlkLen;
		BYTE	UsInterleaverBlkLen;
		BYTE	currentFramingMode;
        
        BYTE    bandType;              /* Bandplan type: 998, 997, China, etc */
        BYTE    numOfBandsUsDs;        /* Number of Upstream & Downstream bands */
        UINT32 LineType;
		} PortStatusExtParms;

	struct tDeviceVersions
		{
		/* Return parameters from BME */
		char VersionStr[250];
		BYTE VersionLen;

		} DeviceVerParms;

	/* EOC Commands */

	struct tEocControl	
		{
		BYTE	EocMode;
		} EocControlParms;

	struct tEocSetData
		{
		BYTE	EocOpcode;
		BYTE 	EocRegisterData[128];
		} EocSetDataParms;

    struct tEocDataReceive
        {
        BYTE    EocOpcode;
        BYTE    EocData[128];
        } EocDataRecvParms;

	struct tEocDataReply
		{
		BYTE	EocOpcode;
		BYTE	EocLength; 
		BYTE 	EocData[128];
		} EocDataReply;

	struct tEocSendDataStream	
		{
		BYTE	EocLength; 	
		BYTE	DataStream[MAX_EOC_DATA_STR_LENGTH];
		} EocSendDataStrParms;

	struct tEocGetData
		{
		BYTE	EocOpcode;
		/* Return value */
		BYTE RtnEocOpcode;
		BYTE	EocRegisterData[128];
		} EocGetDataParms;

    /* Autonomous Eoc structs */
    struct tEocRelayDataStream	
        {
	BYTE	len;
        BYTE	DataStream[250];
        } EocRelayDataStrParms;
    
    struct tEocMsgReceive	
        {
        BYTE	EocOpcode; 	
        BYTE	DataStream[128];
        } EocMsgRecvParms;
    
	/* Debug Commands */
	struct tSetLoopbackParms
		{
		BYTE LoopbackMode;

		} SetLpbkParms;

	struct tSendSignals
		{
		BYTE SigType;
		BYTE NumTones;
		UINT16 Tone1;
		UINT16 Tone2;
		UINT16 Tone3;
		UINT16 SweepInterval;
		UINT16 SigOptions;
		} SendSigParms;

	struct tDetectSig
		{
		BYTE SigType;
        BYTE DetectSigType;
		} DetectSigParms;

	struct tGetSetlftestResults
		{
			int dummy; /* Some compiler can not handle empty struct */
		} SelftestResultsParms;
    
    	struct tGetNeSnrAttn	
		{
		long	SnrMargin;
        	long    Attn;
		long    AvgSnrMargin;
		long    AvgSnr;
          long    Snrm[MAX_NUM_BANDS];
          long    Satn[MAX_NUM_BANDS];
          long    Latn[MAX_NUM_BANDS];
		} NeSnrAttn;
	struct tOpCmdResults	
		{
			BYTE subopCode;
			BYTE buf[128];
		} opCmdResults;

	struct tBitSwapReq
		{
		BYTE	BitSwapOpcode;
		BYTE 	BitSwapMsg[30];
		} BitSwapReqParms;
#ifdef BER_PER_TONE
	struct tAllZeroReq
		{
		BYTE 	cmd_options[8];
		} AllZeroReqParms;
#endif
    struct tQosParms
        {
        BYTE classifier_enable;
        BYTE cfgPPPoEDecode;
        BYTE cfgSNAPDecode;
        BYTE cfgEthernet2Decode;
        BYTE cfgIpv4Decode;
        BYTE cfgIpv6Decode;
        BYTE cfgPortEthPriEnable;
        BYTE cfgPortETypeEnable;
        BYTE cfgPortIpv4TosEnable;
        BYTE cfgPortIpv6TcEnable;
        BYTE cfgPortdefaultclass;
        struct tclass2queueparms
          {
            BYTE queue_num;
            BYTE drop_criteria;
          } cfgclass2queuemap[8];
        BYTE drop_crc_err_frames;
        BYTE drop_long_frames;
        UINT16 MaxFrameSize;
        UINT16 MinFrameSize;
        struct tscheduler_parms
          {
            BYTE scheduler_type;
            UINT32 WRRMinUpdateTime;
            UINT32 WRRExtUpdateTime;
          } cfgscheduler[2];

        struct tqueue_parms
          {
            UINT16   TxHWM;
            UINT16   TxLWM;
            BYTE   FlowCtrlEnable;
            BYTE   QueueEnable;
            UINT16 WRRMinQuantHigh;
            UINT16 WRRMinQuantLow;
            UINT16 WRRExtQuantHigh;
            UINT16 WRRExtQuantLow;
          } cfgqueue[4];

        } QosUsParms;

	} tPortParms;

/*** BME specific Parameters ***/

/* Definition of Port structure */

typedef struct tPort
	{
	eApiRv	CmdStatus;
	eCommands	Cmd;
	UINT32		CmdMaxRespTime; 
	tPortParms PortParms;
//	tPerformance Performance;
	/* Failures */
    BYTE PortStopDelay;
	} tPort;

typedef struct tPortId
	{
	int BmeNum;
	int PortNum;
	} tPortId;

/* EMI Configuration Parameters */
typedef struct tEmiConfig
	{
	eBmeMemType 		memType;
	eBmeSramMemSize		sramMemSize;
	eBmeSdramMemSize	sdramMemSize;
	eBmeCasLat			casLat;
	eBmeRCDel			rcDel;
	eBmeRowPre			rowPre;
	eBmeRowAct			rowAct;
	eBmeRowCyc			rowCyc;
	unsigned char		refPeriod;
	} tEmiConfig;

typedef struct tUpdateCounterReq
{
        UINT32 value;
        UINT16 index;
} tUpdateCounterReq;		/* tracks ipos_update_counter_req in apistruct.h */

/**********************************/
/* System specific API Parameters */
/**********************************/

typedef struct tBmeParms
	{
	struct tBmeStart
		{
			int dummy; /* Some compiler can not handle empty struct */
		} BmeStartParms;

	struct tBmeReset
		{
			int dummy; /* Some compiler can not handle empty struct */
		} BmeResetParms;

    struct tSystemStatus
		{
		/* Return paramters from BME */
		BYTE	Enable_1_8;
		BYTE	Enable_9_16;
		BYTE	Enable_17_24;
		BYTE	Enable_25_32;
		BYTE	Activate_1_8;
		BYTE	Activate_9_16;
		BYTE	Activate_17_24;
		BYTE	Activate_25_32;
		BYTE	LinkUp_1_8;
		BYTE	LinkUp_9_16;
		BYTE	LinkUp_17_24;
		BYTE	LinkUp_25_32;
        BYTE    temp;
		} SystemStatusParms;

    struct tSystemProv
    {
        WORD    VendorIdT1E1;
        BYTE    VendorIdItu[8];
        BYTE  RevisionNum[16];
        BYTE    SerialNumber[32];
        BYTE    AtmOption;
        BYTE    AtmCellSize;
        BYTE    OptionMask;
        BYTE    BandPlan;
        BYTE    NumPort;
		BYTE	psdMaskLevelIndx;
		unsigned long k0[3];
		unsigned long k1[3];
		unsigned short		custId;
		unsigned short		keys[3];
        BYTE    ife2PwrMode;
        BYTE    OamOptionMask;
                /* K0 & K1 for higher US bands (4, 5, 6) */
		unsigned long k0_hi[3];
		unsigned long k1_hi[3];
        BYTE    MaxPsd[DAPI_MAX_PSD_LEN];
        WORD    iposVer;  // Should be last element of structure.
    } SysProvParms;

    struct tSystemProv bmeRxSysProvParms;

    struct tDebugParms
    {
      UINT32 Parm1;
      UINT32 Parm2;
      UINT32 Parm3;
      UINT32 Parm4;
      UINT32 Parm5;
    } DebugParms;
		
	struct tOptionalCmd
	{
		unsigned int 	buf;
		int				length;
	} OptionalCmd;
    struct tBmeQosParms
    {
        BYTE cfgethpri2class[8];
        BYTE cfgipv4defaultclass;
        BYTE cfgipv6defaultclass;
        BYTE cfgipv4tos2class[128];
        struct teype_parm
          {
            UINT16 etype;
            BYTE   class_num;
          } cfgetype2class[8];
        BYTE BmeQosType;
    } SysQosParms;
} tBmeParms;



/* Definition of BME structure */

typedef struct tBmeStruct
	{
	eBmeState	BmeState;
	eBmeDnldVerifyState	BmeDnldVerifyState;
	tBmeParms	BmeParms;
	tPort		Port[IPOS_MAX_PORT_PER_BME+1];	/* 1-based index */
	tEmiConfig 	EMIConfig; /* EMI Configuration values */
        BYTE hostResync;

	int BmeNum;

	/* For now, one BME message buffer allocated for each BME */
	UINT32 BMEbaseAddr;
	UINT32 srcAddr;			/* bmeWr/RdMemory is used if these values non-zero */
	UINT32 destAddr;
	UINT32 dpramBaseRxAddr; /* if writing to DPRAM set these values, secAddr and destAddr =0 bmeWr/RdMessage is used */
	UINT32 dpramBaseTxAddr;
	UINT32 BMEResetLineAddr;
	UINT32 BMECodeDnldAddr;	/* BME software code download address */
	BYTE *dataPtr;
	BYTE BmeMsg[MAX_BME_MESSAGE]; /* Used for Code download only .to store the data read from FIFO */
	BYTE TxBmeMsg[MAX_BME_MESSAGE];
	BYTE RxBmeMsg[MAX_BME_MESSAGE];
	int dataLen;
	UINT32 BMECodeSize; /* BME software code download size */
	
	void	*pBmeIntrFcn;	/* User-defined function to handle BME interrupts */

	int FifoFlag;
	BYTE fifoStatus;
	BYTE msgRdyPending;
#ifdef CPE_40
	BYTE hicStatus;		/* returned in bmeRdStatus - CO_30 change */
#endif
	} tBmeStruct;



#endif
