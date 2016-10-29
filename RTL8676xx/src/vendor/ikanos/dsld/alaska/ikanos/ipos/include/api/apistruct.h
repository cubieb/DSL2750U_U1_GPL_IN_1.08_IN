
#ifndef IPOSSTRUCT
#define IPOSSTRUCT

/**************************************************************
$Workfile:   apistruct.h  $
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

/***************************************************************
**
** This C header file containes the IPOS API data structure definitions  
**
***************************************************************/

   
/* ipos_port_id: Used to identify the port in the BME */   

typedef struct
{

	int	portNum;			/* the port number:  0 to IPOS_MAX_PORT */
	int bmeNum;				/* the BME number:   0 to IPOS_MAX_BME  */

} ipos_port_id;


/* ipos_bme_load: Used for downloading the firmware to BME	*/

typedef struct
{
	
	U32	*bmeBaseAddr;		/* BME base address	*/
	U32	destAddr;			/* destination address within the BME	*/
	U8	*pFirmware;			/* point to the buffer storing the BME firmware */
    int	firmwareLen;		/* firmware Length	*/

} ipos_bme_load;


/* ipos BME provision	*/

#define IPOS_MAX_PSD_LEN   (85)
#define MAX_NUM_BANDS     (5)

typedef struct
{

	U16	vendorIdT1E1;		/* vendor ID for T1E1, not used for ITU */
	U8	vendorIdITU[8]; 	/* vendor ID for ITU, not used not used for T1E1 */
	U8  revisionNum[16];		/* revision number	*/
	U8	serialNum[32];	   	/* serial number	*/
	U8	utopiaOption;		/* bit0:ATM_LEVEL_2, bit1: ATM extend mode
												 bit3: ATM_remove_hec, bit7: Packet Transport mode */
	U8 	utopiaDataSize;		/* default: 53, range: 5-255	*/
	U8	optionMask;			/* bit0: NTR_DISABLE, bit1: IMUX_ENABLE	*/
	U8	bandPlan;       /* System band plan                     */
  	U8	numPort;        /* Max Number of Ports                  */
	U8	psdMaskLevelIndx;
	S32	k0[3];
	S32	k1[3];
	U16	custId;
	U16	keys[3];
    U8  ife2PwrMode;
    U8  OamOptionMask; /* bit0: Select PM Counters,  bit1: PM Failure Alert Enable */  
        S32     k0_hi[3];
        S32     k1_hi[3];
	U8 MaxPsd[IPOS_MAX_PSD_LEN];
}ipos_bme_provision;

typedef struct {
	U32 buf;
	int len;
} ipos_op_cmd;

/* ipos port provision 	*/

typedef	struct
{

	
	U8	InterleavedChannel;			/* utopia address for interleaved channel: 0-63 */ 
	U8	FastChannel;				/* utopia address for fast channel: 0-63 */
	U8	LineType;					/* line type: VDSL_AUTO_DETECT	for DEFAULT */
	U8	DslMode;					/* 1 = No channel exists
										2 = Fast channel exists only
										3 = Interleaved/Slow channel only
										4 = Either fast or interleaved/slow channels exist,
											but only one at a time.
										5 = Both fast and interleaved/slow channel exist.
									*/
	U8	RateAdaptationMode;			/* 0 = Fixed
										1 = Startup	*/
	U8	InterleavedPercentage;		/* Percentage of data through interleave path. The range
										is from 1 to 100.	*/
	U32 UpstreamMaxRateFast;		/* Data rate in kbps. The range is 0 to 25 Mbps, in steps
										of 64 kbps (e.g., 0 = 0 kbps, 1 = 64 kbps, 2 = 128 kbps,
										etc.).		*/
	U32 UpstreamMinRateFast;		/* Data rate in kbps. The range is 0 to 25 Mbps, in steps
										of 64 kbps (e.g., 0 = 0 kbps, 1 = 64 kbps, 2 = 128 kbps,
										etc.).	*/
	U32 UpstreamMaxRateSlow;		/* Data rate in kbps. The range is 0 to 25 Mbps, in steps
										of 64 kbps (e.g., 0 = 0 kbps, 1 = 64 kbps, 2 = 128 kbps,
										etc.). */
	U32 UpstreamMinRateSlow;		/* Data rate in kbps. The range is 0 to 25 Mbps, in steps
										of 64 kbps (e.g., 0 = 0 kbps, 1 = 64 kbps, 2 = 128 kbps,
										etc.). */ 
	U8	UpstreamMaxInterDelay;		/* Maximum upstream interleave delay. User defined. */
	U32 DownstreamMaxRateFast;		/* Data rate in kbps. The range is 0 to 25 Mbps, in steps
										of 64 kbps (e.g., 0 = 0 kbps, 1 = 64 kbps, 2 = 128 kbps,
										etc.). */
	U32 DownstreamMinRateFast;		/* Data rate in kbps. The range is 0 to 25 Mbps, in steps
										of 64 kbps (e.g., 0 = 0 kbps, 1 = 64 kbps, 2 = 128 kbps,
										etc.). */
	U32 DownstreamMaxRateSlow;		/* 	Data rate in kbps. The range is 0 to 25 Mbps, in steps
										of 64 kbps (e.g., 0 = 0 kbps, 1 = 64 kbps, 2 = 128 kbps,
										etc.). */
	U32 DownstreamMinRateSlow;		/* Data rate in kbps. The range is 0 to 25 Mbps, in steps
										of 64 kbps (e.g., 0 = 0 kbps, 1 = 64 kbps, 2 = 128 kbps,
										etc.).	*/
	U8	DownstreamMaxInterDelay;	/* Maximum downstream interleave delay. User defined.*/
	U8	NumParityFast;				/* Number of parity bytes (0 - 16) */
	U8	NumParitySlow;				/* Number of parity bytes (0 - 16) */
/*	U8	NumCodeWord;	*/		
	U16 UsTargetNoiseMargin;				/* Range 0-31.0dB, increments of 0.5dB (e.g., 0 = 0dB, 1
										= 0.5dB, ...)	*/
	U16 DsTargetNoiseMargin;			/* Range 0-31.0dB, increments of 0.5dB (e.g., 0 = 0dB, 1
										= 0.5dB, ...)	*/
	U16 MinNoiseMargin;				/* Range 0-31.0dB, increments of 0.5dB (e.g., 0 = 0dB, 1
										= 0.5dB, ...)	*/
/*	U16 UpNoiseMargin;
	U16 UpNoiseInterval;
	U16 DownNoiseMargin;
	U16 DownNoiseInterval;*/
	U8 BandPlan;					/* specific band plan: e.g. BP998_3B_8_5M,BP998_4B_12M,
									BP997_3B_7_1M,BP997_4B_7_1M BP998_2B_3_8M  */
	U8 NumRFIBand;					/* numbner of RFI band: 0 to 5	*/
	U16 Band1Start;					/* Starting tone index for RFI band 1.*/
	U16 Band1Stop;					/* stop tone index for RFI band 2	*/
	U16 Band2Start;
	U16 Band2Stop;
	U16 Band3Start;
	U16 Band3Stop;
	U16 Band4Start;
	U16 Band4Stop;
	U16 Band5Start;
	U16 Band5Stop;

  U8 RFIBand1;
  U8 RFIBand2;
  U8 RFIBand3;
  U8 RFIBand4;
  U8 RFIBand5;
    U8 UsMinProtection;
    U8 DsMinProtection;
#ifdef CPE_40
    U8 framingmode;
#endif
	U8  disableMicroCut;
}	ipos_port_provision;	
														 


/**********************************/
/* Port specific API Parameters */
/**********************************/



/* Port Profile Structure */
typedef struct ipos_port_profile {

	int PortProfileNum;							/* the profile identifier	*/
	
	ipos_port_provision  PortProvParms;			/* the port provision parameters	*/

	struct 
 	{
		U8	PortStartParm;
	
	} PortStartResetParms;						/* the parameters to start the port */

	struct 
	{
		U8	PortStopParm;
	} PortStopParms;							/* parameters to stop the port	*/

	struct 
	{
		U8	bandModifier;
	} BandMod;

	struct
	{
	U8 PortOption;
	U8 Reserved1;
	U8 Reserved2;
	U8 Reserved3;
	} OptionParms;

    struct
    {
        U8  pwrMode;
        U8  txPathFilterMode;
        U8  rxPathFilterMode;
    } IFE_BRD_CFG;
    
}ipos_port_profile;



/***********************************************
*	
*  structures for periodic report 
*
************************************************/


typedef struct
{
    U16 etype;
    U8  class_num;
} etype2class;
typedef struct {
    U8 CfgEthPri2Class[8];
    etype2class CfgEtype2Class[8];
    U8 CfgIpv4Tos2Class[256];
    U8 CfgIpv4DefaultClass;

} ipos_bme_Protocol2ClassMap;

typedef struct {

    U8 ClassifierEnable;
    U8 CfgPPPoEDecode;
    U8 CfgSNAPDecode;
    U8 CfgEthernet2Decode;
    U8 CfgIpv4Decode;
    U8 CfgIpv6Decode;
    U8 CfgPortEthPriEnable;
    U8 CfgPortETypeEnable;
    U8 CfgPortIpv4TosEnable;
    U8 CfgPortIpv6TcEnable;
    U8 CfgPortDefaultClass;
    
} ipos_port_protocoltype_config;

typedef struct {
    U8 queue_num; 
    U8 drop_criteria; /* NEVER_DROP or ALWAYS_DROP */
} iposClass2QueueParms;


typedef struct
{
    U16  TxHWM;
    U16  TxLWM;
    U8  FlowCtrlEnable;
    U8  queue_enable;
    U16 WRRMinQuantHigh;
    U16 WRRMinQuantLow;
    U16 WRRExtQuantHigh;
    U16 WRRExtQuantLow;
} queue_parms;

typedef struct
{
    U8   scheduler_type; /* Strict Priority/WRR/EFM-Premption */
    U32  WRRMinUpdateTime;
    U32  WRRExtUpdateTime;
}  scheduler_parms;

typedef struct 
{
    U8 drop_crc_err_frames;
    U8 drop_long_frames;
    U16 MaxFrameSize;
    U16 MinFrameSize;
    scheduler_parms sched_cfg[2];/* Per Bearer scheduler config */
    queue_parms  queue_cfg[4];

}   ipos_queue_config;



/***********************************************
*	
*  structures for periodic report 
*
************************************************/


typedef struct	
{

	U8		ReportsEnable;		/* bit 2:system status, bit1 alarm counter bit 0: disable */
	U16 	AlarmCountFreq;		/* Frequency of Alarm counter report in 100 ms */
	U16 	SystemStatusFreq;	/* Frequency of System Status report in 100 ms */

} ipos_report_config;

typedef	struct
{
	
	U8	ReportEnable;			/* bit 1 port status; bit 0: disable	*/

}ipos_update_config;

/***********************************************
*	
*  structures for callback functions of	Status and autonomous 
*
************************************************/

/* brief port status	*/
typedef	struct
{

	U8	portStatus;	/*	Bit[5:3]: DSL FSM state
					001 = Activation/Acknowledgement
					010 = Training
					011 = Channel Analysis
					100 = Exchange
					101 = Showtime
					Bit[2]: Showtime
					Bit[1]: Port Activating
					Bit[0]: Port Enabled
					*/
    U8  currentFramingMode; /*  definition matches the same field in ipos_hde_cfg
                      0x01:  ATM mode
                      0x10:  64-65 mode (EFM)
                      0x80:  HDLC mode
                    */

}ipos_port_status_brief;


/* extendef port status	*/
typedef	struct
{
	
	U32 	DsLineRate;		/* Provides the downstream data 
							rate on the line, in increments	of 64 kbps */
	U32		UsLineRate;		/* Upstream line rate. 64k incrementation */
	U32		FastDsPayloadRate;	/* fast channel down stream payload rate. */
	U32 	SlowDsPayloadRate;	/* slow channel down stream payload rate */
	U32		FastUsPayloadRate;	/* fast channel upstream payload rate	*/
	U32 	SlowUsPayloadRate;	/* slow channel upstream payload rate   */
	U8		FastDsNumParityBytes;
	U8		SlowDsNumParityBytes;
	U8		FastUsNumParityBytes;
	U8		SlowUsNumParityBytes;
	U16		DsDelay;
	U16		UsDelay;
	U16		TransmitPower;
  U16   DsTrainingMargin;
  U16   DsAttainableRate;
  U16   DsAttainableLineRate;
  U16   SlowDsLineProtection;
  U16   UsTrainingMargin;
  U16   UsAttainableRate;
  U16   UsAttainableLineRate;
  U16   SlowUsLineProtection;
	U8		NeItuVendorId[8];
	U8		FeItuVendorId[8];
	U16		FeTransmitPower;
  U16   LoopLength;
  U16   ghsNearEndLoopLength;
  U16   ghsFarEndLoopLength;
	U16		SlowUsFFTProtection;
 	U16		SlowDsFFTProtection;
	U8		FastDsCodeWordSize;
	U8		SlowDsCodeWordSize;
	U8		FastUsCodeWordSize;
	U8		SlowUsCodeWordSize;
	U8		DsInterleaverDepth;
	U8		UsInterleaverDepth;
	U8		DsInterleaverBlkLen;
	U8		UsInterleaverBlkLen;
	U8		currentFramingMode;
    
    U8      bandType;              /* Bandplan type: 998, 997, China, etc */
    U8      numOfBandsUsDs;        /* Number of Upstream & Downstream bands */
    U32 LineType;
}ipos_port_status;


/* near end snr and attn	*/
typedef	struct
{
	S32	SnrMargin;
	U32	Attn;
	S32	SnrAvgMargin;
	S32	SnrAvg;
     S32     Snrm[MAX_NUM_BANDS];
     S32     Satn[MAX_NUM_BANDS];
     S32     Latn[MAX_NUM_BANDS];
}ipos_port_nesnrattn;

typedef struct 
{
	U8		tag;
	U8		buf[128];
} ipos_opCmd_status;

/* system status	*/
typedef	struct
{
	U8	Enable_1_8;			/* port enable status bits for port 1 to 8	*/
	U8	Enable_9_16;		/* port enable status bits for port 9 to 16	*/
	U8	Enable_17_24;		/* port enable status bits for port 17to 24	*/
	U8	Enable_25_32;		/* port enable status bits for port 25to 32	*/
	U8	Activate_1_8;		/* port activation status bits for port 1 to 8	*/
	U8	Activate_9_16;		/* port activation status bits for port 9 to 16	*/
	U8	Activate_17_24;		/* port activation status bits for port 17 to 24 */
	U8	Activate_25_32;		/* port activation status bits for port 25 to 32 */
	U8	LinkUp_1_8;			/* port link status bits for port 1 to 8	*/
	U8	LinkUp_9_16;		/* port link status bits for port 9 to 16	*/
	U8	LinkUp_17_24;		/* port link status bits for port 17 to 24	*/
	U8	LinkUp_25_32;		/* port link status bits for port 25 to 32	*/
	U8    temp;

}ipos_system_status;


typedef	struct
{
	U8 VersionStr[250];		/* the string for BME and sytem version	*/
	U8 VersionLen;			/* teh length of the version string */
}ipos_bme_ver;



/* port counters structures	*/

typedef	struct
{
	/* Return parameters from BME */
	U32		NeFEC_S;				/* Near-end Forward Error Correction (FEC) Slow count	*/
	U32		NeFEC_F;				/* Near-end Forward Error Correction (FEC) Fast count	*/
	U16		NeCRC_S;				/* Near-end Cyclic Redundancy Check (CRC) Slow count	*/
	U16		NeCRC_F;				/* Near-end Cyclic Redundancy Check (CRC) Fast count	*/
	U8		Reserved1[4];		/* Reserved */
	U8		NeLOS;					/* Near-end Loss Of Signal (LOS) count					*/
	U16		NeSEF;					/* Near-end Severely Errored Frame (SEF) count			*/
	U16		NeLOM;          		/* Near-end Loss of Margin */
	U8		NeNCD_S;				/* Near-end No Cell Delineation (NCD) Slow count		*/
	U8		NeNCD_F;				/* Near-end No Cell Delineation (NCD) Fast count		*/
	U8		NeOCD_S;				/* Near-end Out of Cell Delineation (OCD) Slow count	*/
	U8		NeOCD_F;				/* Near-end Out of Cell Delineation (OCD) Fast count	*/
	U8		NeHEC_S;				/* Near-end Header Error Control (HEC) Slow count		*/
	U8		NeHEC_F;				/* Near-end Header Error Control (HEC) Fast count		*/
	U8		Reserved3[7];		/* Reserved */
	U8		NeBER_S;				/* Near-end Bit Error Rate (BER) Slow count				*/
	U8		NeBER_F;				/* Near-end Bit Error Rate (BER) Fast count				*/
	U32 	NeFECUnCrr_S;			
	U32 	NeFECUnCrr_F;			
	U32 	numUsrCell_S;			/* Number of User Cells (USR) Slow count				*/
	U32 	numUsrCell_F;			/* Number of User Cells (USR) Fast count				*/
	U32 	numIdlCell_S;			/* Number of Idle Cells (IDLE) Slow count				*/
	U32 	numIdlCell_F;			/* Number of Idle Cells (IDLE) Fast count				*/
	U32 	numTxBlk_S;				/* Number of TX Block Slow	count						*/
	U32 	numTxBlk_F;				/* Number of Tx Block Fast	count						*/
	U32 	numRxBlk_S;				/* Number of Rx Block Slow	count						*/
	U32 	numRxBlk_F;				/* Number of Rx Block Fast	count						*/

	U16		FeFEC_S;				/* Far-end Forward Error Correction (FEC) Slow count	*/
	U16		FeFEC_F;				/* Far-end Forward Error Correction (FEC) Fast count	*/
	U16		FeCRC_S;				/* Far-end Block Error (BE) Slow count					*/
	U16		FeCRC_F;				/* Far-end Block Error (BE) Fast count					*/
	U16		FeLOS;					/* Far-end Loss Of Signal (LOS) count					*/
	U16		FeRDI;					/* Far-end Remote Defect Indication (RDI) count			*/
	U16		FeOCD_S;				/* Far-end Out of Cell Delineation (OCD) Slow count		*/
	U16		FeOCD_F;				/* Far-end Out of Cell Delineation(OCD) Fast count		*/
	U16		FeHEC_S;				/* Far-end Header Error Control (HEC) Slow count		*/
	U16		FeHEC_F;				/* Far-end Header Error Control (HEC) Fast count		*/
	U16		FeLPR;					/* Far-end Loss of Power (LPR) count					*/
	U16		FeLOM;					/* Far-end Loss of Margin (LOM) count					*/
	U16		FePO;					  /* Far-end Power-Off count					*/

    U32     FeFecEs_S;             /* Far-end Forward Error Correction Errored Seconds (FEC ES) Bearer 1 count    */
    U32     FeEs_S;                  /* Far-end Errored Seconds (ES) Bearer 1 count    */
    U32     FeSEs_S;                /* Far-end Severely Errored Seconds (ES) Bearer 1 count    */
    U32     FeLosEs_S;             /* Far-end LOS Errored Seconds (LOS ES) Bearer 1 count    */
    U32     FeUnAvlEs_S;         /* Far-end Un Available Errored Seconds (UNAVL ES) Bearer 1 count    */
    U32     FeBER_S;                /* Near-end Bit Error Rate (BER) Bearer 1 count             */

    U16     sixPtLineHold;
	U8 		uCutReportFlag;
}ipos_port_counters;


/* PM Alert structure */
typedef struct
{

  U16 NeFailures;
  U16 FeFailures;
  U16 NeTCA1;
  U16 FeTCA1;
  U16 NeTCA2;
  U16 FeTCA2;
  U8 uCutReportFlag;
}ipos_pm_alert;


/* PM Trigger structure */
typedef ipos_pm_alert ipos_pm_trigger;


/* PM Config structure */
typedef struct {

  U16	LOS_Threshold;
  U16	LOF_Threshold;
  U16	LPR_Threshold;
  U16	LOM_Threshold;
  U16	ESE_Threshold;
  U16	NCD_S_Threshold;
  U16	LCD_S_Threshold;
  U16	NCD_F_Threshold;
  U16	LCD_F_Threshold;

  U16	FeLOS_Threshold;
  U16	FeLOF_Threshold;
  U16	FeLPR_Threshold;
  U16	FeLOM_Threshold;
  U16	FeESE_Threshold;
  U16	FeNCD_S_Threshold;
  U16	FeLCD_S_Threshold;
  U16	FeNCD_F_Threshold;
  U16	FeLCD_F_Threshold;
    U16 FePO_Threshold;

  U16	FEC_Second_Threshold;
  U16	NeES_Second_Threshold;
  U16	NeSES_Second_Threshold;
  U16	NeLOS_Second_Threshold;
    U16  NeUnavl_Second_Threshold;
  U16	FeFEC_Second_Threshold;
  U16	FeES_Second_Threshold;
  U16	FeSE_Second_Threshold;
  U16	FeLOS_Second_Threshold;
    U16 FeUnavl_Second_Threshold;

  U16	FEC_S_Threshold;
  U16	FEC_F_Threshold;
  U16	CRC_S_Threshold;
  U16	CRC_F_Threshold;
  U16	HEC_S_Threshold;
  U16	HEC_F_Threshold;
  U16	FeFEC_S_Threshold;
  U16	FeFEC_F_Threshold;
  U16	FeCRC_S_Threshold;
  U16	FeCRC_F_Threshold;
  U16	FeHEC_S_Threshold;
  U16	FeHEC_F_Threshold;

} ipos_pm_config;

/* PM Report Structure */
typedef struct
{

  U8  PmPeriodId;
    U8  PortStartFlag;
  /* PM Alert Status */
  U16 NeFailures;
  U16 FeFailures;
  U16 NeTCA1;
  U16 FeTCA1;
  U16 NeTCA2;
  U16 FeTCA2;

  /* Near End Failure Counts */
	U16	NeLOS;   //	Near-end Loss Of Signal failure count
    U16 NeLOF;   // Near-end Loss of Frame (LOF) failure count
	U16	NeLOM;   // Near-end Loss of Margin failure count
	U16	NeESE;   //	Near-end Excessive Severe Error failure count
	U16	NeNCD_S; //	Near-end No Cell Delineation (NCD) failure Slow count
	U16	NeLCD_S; //	Near-end Loss of Cell Delineation (LCD) failure Slow count
	U16	NeNCD_F; //	Near-end No Cell Delineation (NCD) failure Fast count
	U16	NeLCD_F; //	Near-end Loss of Cell Delineation (LCD) failure Fast count

  /* Far End Failure Counts */
	U16	FeLOS;  //	Far-end Loss Of Signal (LOS) failure count
	U16	FeRDI;	 // Far-end Remote Defect Indication (RDI) failure count
	U16	FeLPR;   //	Far-end Loss of Power failure count
	U16	FeLOM;   //	Far-end Loss of Margin failure count
	U16	FeESE;   //	Far-end Excessive Severe Error failure count
	U16	FeNCD_S; //	Far-end No Cell Delineation (NCD) failure Slow count
	U16	FeLCD_S; //	Far-end Loss of Cell Delineation (LCD) failure Slow count
	U16	FeNCD_F; //	Far-end No Cell Delineation (NCD) failure Fast count
	U16	FeLCD_F; //	Far-end Loss of Cell Delineation(LCD)  failure Fast count
    U16 FePO;    // Far-end Power off Failure count

  /* Counts of Seconds with Errors */
	U16	NeSES;   //	Near-end Severe Errored second count
	U16	NeES;  	 // Near-end Errored second count
	U16	NeFECS;  //	Near-end FEC second count
	U16	NeLOSS;	 // Near-end Loss of Signal second count
    U16  NeUnavl;  // Near-end Unavailable second count
	U16	FeSES;   //	Far-end Severe Errored second count
	U16	FeES;    //	Fear-end Errored second count
	U16	FeFECS;  //	Far-end FEC second count
	U16	FeLOSS;  //	Far-end Loss of Signal second count
    U16  FeUnavl;  // Far-end Unavailable second count

  /* Near-end Anomaly Counts */
	U32 NeFEC_S; // Near-end Forward Error Correction (FEC) Slow Count
	U32 NeFEC_F; // Near-end Forward Error Correction (FEC) Fast Count
	U32	NeCRC_S; // Near-end Cyclic Redundancy Check (CRC) Slow count
	U32	NeCRC_F; // Near-end Cyclic Redundancy Check (CRC) Fast count
	U32	NeHEC_S; //	Near-end Header Error Control (HEC) Slow count
	U32	NeHEC_F; //	Near-end Header Error Control (HEC) Fast count

  /* Far-end Anomaly Counts */
	U32	FeFEC_S; //	Far-end Forward Error Correction (FEC) Slow count
	U32	FeFEC_F; //	Far-end Forward Error Correction (FEC) Fast count
	U32	FeCRC_S;	 // Far-end Block Error (BE) Slow count
	U32	FeCRC_F;  //	Far-end Block Error (BE) Fast count
	U32	FeHEC_S; //	Far-end Header Error Control (HEC) Slow count
	U32	FeHEC_F; //	Far-end Header Error Control (HEC) Fast count

	U32	numUSRcell_S;  //	Number of User Cells (USR) Slow count
	U32	numUSRcell_F;  //	Number of User Cells (USR) Fast count
  U32	numIDLEcell_S; //	Number of Idle Cells (IDLE) Slow count
  U32	numIDLEcell_F; //	Number of Idle Cells (IDLE) Fast count

    U32 numIDLEcellBe_S;  //Number of Idle Cells Bit Error Bearer 1 count
    U32 numIDLEcellBe_F;  //Number of Idle Cells Bit Error Bearer 0 count

    U32 numTxBlk_S;    // Number of TX Block Slow  count
    U32 numTxBlk_F;    // Number of Tx Block Fast  count
    U32 numRxBlk_S;    // Number of Rx Block Slow  count
    U32 numRxBlk_F;    // Number of Rx Block Fast  count

    U32 NeFECUnCrr_S;  // Near end slow FEC uncorrected count
    U32 NeFECUnCrr_F;  // Near end Fast FEC uncorrected count
    U16  sixPtLineHold;

    U16 numReInit;      // Number of re-initialization count
    U16 numInitFailure; // Number of initialization failures 
    U32 NeSEF;          // Near-end Severely Errored Frame (SEF) "raw" count

} ipos_pm_counters;

typedef struct
{
    U32 ClassCounters[8];
    U32 Ipv4PacketCount;
    U32 Ipv4TotalByteCount;
} ipos_qos_counters;
/* far-end counters structures	*/

typedef struct
{
    U8 BmeQosType;
} ipos_bme_qos_type;

typedef struct
{

	U32		FeFEC_S;
	U32		FeFEC_F;
	U16		FeCRC_S;
	U16		FeCRC_F;	
	U8		FeFLOS;

}ipos_feport_counters;

typedef struct
{
    U32  value;
    U16  index;
} ipos_update_counter_req;


typedef	struct
{

	U8	memType;		/* IPOS_BME_MEM_SRAM, IPOS_BME_MEM_SDRAM16, IPOS_BME_MEM_SDRAM32	*/
	U8	memSize;		/* 1,2,4,8,16,32 meg	*/
	U8	clk;			/* IPOS_BME_MEM_CLKHALF, IPOS_BME_MEM_CLKFULL			*/
	U8	casDelay;		/* cas latency: 2 or 3		*/
	U8	rcDelay;		/* ras to cas delay: 2 or 3	*/
	U8	rPrecharge;		/* row Precharge time 2 or 3	*/
	U8	rActive;		/* row active time: 6 or 7	*/
	U8	rCycle;			/* row cycle time	10 or 11	*/

}ipos_emi_config;

typedef struct
{
	U8 SigType;
	U8 NumTones;
	U16 Tone1;
	U16 Tone2;
	U16 Tone3;
	U16 SweepInterval;
	U16 SigOptions;

}ipos_test_signals;

#endif

