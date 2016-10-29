
#include "../ipos/include/api/apitype.h"
#include "../ipos/include/api/apistruct.h"

/***********************************************************************/
/*              PORT RELATED CONSTANT DEFINITIONS                      */ 
/***********************************************************************/
#define PORT_L3_CONFIGURED_STATE    0x01 /*uENABLE*/
#define PORT_L3_ACTIVATE_STATE      0x02 /*uACTIVATING*/
#define PORT_L0_STATE               0x2d /*uSHOWTIME*/
#define PORT_UDISABLE				0x0f /*uDISABLE*/

#ifndef TICKS_PER_SECOND
#define TICKS_PER_SECOND 500
#endif
#define MAX_SOCKET_MSG_SIZE 35000  /* This is header(8) + data*/


#define ERROR (-1)
#define CO5_SERVER_PORT      8800
#define CPE5_SERVER_PORT     8801
#define THOUSAND 1000

#define IDS_CMD_IDS_INIT       1
#define IDS_CMD_BME_PROVISION  10
#define IDS_CMD_PORT_PROVISION 11
#define IDS_CMD_PORT_START     12
#define IDS_CMD_PORT_STOP      13
#define IDS_CMD_PORT_STATUS    14
#define IDS_CMD_PORT_PM_STATUS 15
#define IDS_CMD_PORT_NC_STATUS 16
#define IDS_CMD_PORT_NPM_STATUS 17

#define IDS_CMD_678_6_DATA 51
#define IDS_CMD_678_7_DATA 52
#define IDS_CMD_678_EVENTLOG_DATA 53

#define IDS_CMD_PORT_ERROR_STATUS 20

#define IDS_CMD_PORT_LOADING_STATUS 21



#define IDS_CMD_PORT_OPTIONALCMD_TX_DATA 44
#define IDS_CMD_PORT_OPTIONALCMD_RX_DATA 45




#define IDS_CMD_AFE_IFE_DATA 54
#define IDS_CMD_678_3X1_DATA 55
#define IDS_CMD_PORT_EVENT_LOG 56




#define IDS_CMD_PORT_CLR_COUNTER 57
#define IDS_CMD_FILE_UPLOAD    50

#define IDS_CMD_678_FFT_OUT_DATA 100   
#define IDS_CMD_678_RAW_TO_VERIFY_UPBO_DATA 101   
	
#define IDS_CMD_678_FREQ_AVG_DATA 25
#define IDS_CMD_678_PWR_TO_CFAQ_DATA 26 
#define IDS_CMD_678_RAW_TO_VERIFY_UPBO_VTUO_DATA 27
#define IDS_CMD_678_FEQ1_FTRAINING_DATA 28  
#define IDS_CMD_678_FEQ1_CTRAINING_DATA 29   
#define IDS_CMD_678_FEQ2_FTRAINING_DATA 30   
#define IDS_CMD_678_FEQ2_CTRAINING_DATA 31   
#define IDS_CMD_678_TRN_SNR_DATA  32  
#define IDS_CMD_678_SNR_EST1_DATA 33   
#define IDS_CMD_678_SNR_EST2_DATA 34 
#define IDS_CMD_678_SNR_EST3_DATA 35  
#define IDS_CMD_678_SNR_EST4_DATA 36
#define IDS_CMD_678_SHOW_SNR_DATA         37 
#define IDS_CMD_678_RX_BIT_TABLE_DATA     38
#define IDS_CMD_678_TX_BIT_TABLE_DATA     39
#define IDS_CMD_678_SHOW_FEQ_FINE_DATA    40  
#define IDS_CMD_678_SHOW_FEQ_COARSE_DATA  41   
#define IDS_CMD_678_TOTAL_TX_GIPOWER_DATA 42
#define IDS_CMD_678_TOTAL_RX_GIPOWER_DATA 43


#define IDS_CMD_MAINT_SELT_QLN_DATA 46
#define IDS_CMD_ECHO_MEASUREMENT_DATA 47


#define IPOS_MAX_PSD_LEN   (85)

#define PACK_STRUCT __attribute__((packed)) __attribute__ ((aligned(1)))
//#pragma align 1

typedef struct
{

	U16	vendorIdT1E1;		/* vendor ID for T1E1, not used for ITU */
	U8	vendorIdITU[8]; 	/* vendor ID for ITU, not used not used for T1E1 */
	U32     revisionNum;		/* revision number	*/
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
}PACK_STRUCT ids_bme_provision;




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

    U8 framingmode;


} PACK_STRUCT ids_port_provision;	
														 


/**********************************/
/* Port specific API Parameters */
/**********************************/



/* Port Profile Structure */
typedef struct ids_port_profile {

	int PortProfileNum;							/* the profile identifier	*/
	
	ids_port_provision  PortProvParms;			/* the port provision parameters	*/

	struct 
 	{
		U8	PortStartParm;
	
	} PACK_STRUCT PortStartResetParms;						/* the parameters to start the port */

	struct 
	{
		U8	PortStopParm;
	} PACK_STRUCT PortStopParms;							/* parameters to stop the port	*/

	struct 
	{
		U8	bandModifier;
	} PACK_STRUCT BandMod;

	struct
	{
	U8 PortOption;
	U8 Reserved1;
	U8 Reserved2;
	U8 Reserved3;
	} PACK_STRUCT OptionParms;

    struct
    {
        U8  pwrMode;
        U8  txPathFilterMode;
        U8  rxPathFilterMode;
    } PACK_STRUCT IFE_BRD_CFG;
    
} PACK_STRUCT ids_port_profile;


typedef struct _IDS_MESSAGE_HEADER {
	unsigned char bCmd;  /* command : Bme prov, BME */
	unsigned char bBme;
	unsigned char bPort;
	unsigned char bStatus;   /* status of return response */
	unsigned short nLen;   /* AY?? order to be changed later */   /* length of incoming or out going info */
	unsigned short nmsgID;    /* length of incoming or out going info */

} IDS_MESSAGE_HEADER;


#define MAX_VERSION_LEN 32
#define IDS_CO   1
#define IDS_CPE  2

typedef struct _IDS_INIT {
	unsigned char bNumBMEs;  /* command : Bme prov, BME */
	unsigned char bNumPorts;
	unsigned char bType;     /* 1 - CO or 2 - HFCPE or 4 for Vx180 */
	unsigned char bLen;   /* status of return response */
	unsigned char bIPOSVersion[MAX_VERSION_LEN];    

} IDS_INIT;


typedef struct _IDS_MESSAGE {
	IDS_MESSAGE_HEADER idsMHeader;
		//	unsigned char nInfo[1000];     /* input or output data */
		unsigned char nInfo[MAX_SOCKET_MSG_SIZE];     /* input or output data */ /*pandit*/
//Keeping in mind 4k data *8

} IDS_MESSAGE;


typedef struct _IDS_MSG_INIT {
	IDS_MESSAGE_HEADER idsMHeader;
	IDS_INIT           idsInitInfo;
} IDS_MSG_INIT;

#if 0  // not cpe
typedef struct _IDS_MSG_NC {
	IDS_MESSAGE_HEADER idsMHeader;
	ipos_network_counters idsNC;
} IDS_MSG_NC;
#endif

typedef struct _IDS_MSG_BME_PROV {
	IDS_MESSAGE_HEADER idsMHeader;
	//ipos_bme_provision ids_bme_prov;
	ids_bme_provision ids_bme_prov; 

} IDS_MSG_BME_PROV;

typedef struct _IDS_MSG_PORT_PROV {
	IDS_MESSAGE_HEADER idsMHeader;
	//ipos_port_profile  ids_port_prov;
	ids_port_profile  ids_port_prov;

} IDS_MSG_PORT_PROV;


#ifndef IDSCPE
typedef struct _IDS_MSG_PORT_RFI_PROV {
	IDS_MESSAGE_HEADER idsMHeader;
	ipos_port_rfi_cfg  ids_port_rfi_cfg;

} IDS_MSG_PORT_RFI_PROV;

typedef struct _IDS_MSG_PORT_DPBO_PROV {
	IDS_MESSAGE_HEADER idsMHeader;
	ipos_dpbo_config   ids_dpbo_config;

} IDS_MSG_PORT_DPBO_PROV;
#endif //IDSCPE


typedef struct _IDS_MSG_STATUS {
	IDS_MESSAGE_HEADER idsMHeader;
//	ipos_port_status_brief status_brief;
    U32 portStatus; /*It is U8, but to sync with IDS, it is explicatly declared as U32 for byte alignment*/
	ipos_port_status status;
	unsigned char    bBMEVersion[MAX_VERSION_LEN];    

} IDS_MSG_STATUS;


typedef struct _IDS_MSG_PM_STATUS {
	IDS_MESSAGE_HEADER idsMHeader;
	ipos_port_nesnrattn nesnrattn;
	ipos_pm_counters pmstatus;

} IDS_MSG_PM_STATUS;

typedef struct _IDS_MSG_NPM_STATUS {
	IDS_MESSAGE_HEADER idsMHeader;
	ipos_port_counters npmstatus;

} IDS_MSG_NPM_STATUS;

typedef struct _IDS_ERROR_CODE {
	U32 bmeErrorCode;   // same as ipos_port_error_status
	U32 UsAttainableRate;
	U32 DsAttainableRate;
	U16 lastStateXmittedTx;
	U16 lastStateXmittedRx;
	U32 bmeEC;  // Error code for iposbmeprovision
    U32 portEC; // error code for portprovision
    U32 portNIEC; // error code for portNIprovision
    U32 portPSDEC;
    U32 portRFIEC;
    U32 StartEC;
    U32 StopEC;
    U32 BriefEC;
    U32 ExtEC;
    U32 PMEC;
} IDS_ERROR_CODE;


/* Store all iPOS API calls */
typedef struct _IDS_MSG_ERROR_STATUS {
	IDS_MESSAGE_HEADER idsMHeader;
	IDS_ERROR_CODE ErrorCode;

} IDS_MSG_ERROR_STATUS;

	#if 0
	typedef struct _IDS_LOADING_CODE {
		U16 Type;
		U16 Command;
		U16 Start;
		U16 End;
	} IDS_LOADING_CODE;
	#endif
	
	typedef struct _IDS_LOADING_CODE {
		U8 Type;
		U8 Command;
		U16 Start;
		U16 End;
	} IDS_LOADING_CODE;
	
	typedef struct _IDS_MSG_LOADING_STATUS {
		IDS_MESSAGE_HEADER idsMHeader;
		IDS_LOADING_CODE   loading;
	
	} IDS_MSG_LOADING_STATUS;
	
	#if 0
	typedef struct _IDS_MSG_BIT_STATUS {
		IDS_MESSAGE_HEADER idsMHeader;
		U8 BitLoading[6000];
	
	} IDS_MSG_BIT_STATUS;
	#endif
	
	typedef struct _IDS_MSG_TONE_DATA {
		IDS_MESSAGE_HEADER idsMHeader;
		U16 Tone[4096];  // 1024 * 8
	
	} IDS_MSG_TONE_DATA;
	
typedef struct _IDS_MSG_EVENT_LOG {
	IDS_MESSAGE_HEADER idsMHeader;
	U8 log[10000];  

} IDS_MSG_EVENT_LOG;


	typedef struct _IDS_MSG_678_DATA {
		IDS_MESSAGE_HEADER idsMHeader;
		U8 log[5000];  
	
	} IDS_MSG_678_DATA;
	
	typedef struct _IDS_MSG_678_5DATA {
		IDS_MESSAGE_HEADER idsMHeader;
		U32 log[4096];  
	
	} IDS_MSG_678_5DATA;

    	typedef struct _IDS_MSG_ECHO_DATA {
		IDS_MESSAGE_HEADER idsMHeader;
		U8 log[16384];  
	
	} IDS_MSG_ECHO_DATA;

	 typedef struct _IDS_MSG_678_AFE_DATA {
		IDS_MESSAGE_HEADER idsMHeader;
		U8 log[5000];  
	
	} IDS_MSG_678_AFE_DATA;

 typedef struct
{
    U8 SigType;
    U8 NumTones;
    U16 Tone1;
    U16 Tone2;
    U16 Tone3;
    U16 SweepInterval;
    U16 SigOptions;

}PACK_STRUCT ids_test_signals;
#if 1 //DNG: should remove this ipos structure
//once new ipos version is ported to Vx180
typedef struct
{
    U32 bmeErrorCode;
    U32 UsAttainableRate; // 0
    U32 DsAttainableRate; // 1
    U16 lastStateXmittedTx; // 2
    U16 lastStateXmittedRx; // 3
#if 1    
    U32 CurrentOpMode; // 4
    U32 NEOpModeCapabilities; // 5
    U32 FEOpModeCapabilities; // 6
    S16 USSNRMarginTraining; // 7
    S16 DSSNRMarginTraining; // 8
    U16 USLoopAttnTraining; // 9
    U16 DSLoopAttnTraining; // 10
    U16 USSignalAttnTraining; // 11
    U16 DSSignalAttnTraining; // 12
    S16 USACTATPTraining; // 13
    S16 DSACTATPTraining;// 14
    S16 USActualPsdTraining; // 15
    S16 DSActualPsdTraining; // 16
    U8  USInterleaveDelayTraining; // 17
    U8  DSInterleaveDelayTraining; // 18
    U8  USINPTraining; // 19
    U8  DSINPTraining; // 20
    U32 UsAttainableLineRate; // 21 
    U32 DsAttainableLineRate; // 22
#endif    
} ipos_port_error_status;
#endif //DNG
extern int  IDS_Port_Status(ipos_port_id pid, IDS_MSG_STATUS *pmsg);
extern int  IDS_BME_Provision(ipos_port_id pid, IDS_MSG_BME_PROV *pmsg);
extern int  IDS_Port_Provision(ipos_port_id pid, IDS_MSG_PORT_PROV *pmsg);
extern int  IDS_Port_Start(ipos_port_id pid);
extern int  IDS_Port_Stop(ipos_port_id pid);
extern int  IDS_Port_PM_Status(ipos_port_id pid, IDS_MSG_PM_STATUS *pmsg);
extern int  IDS_Port_NPM_Status(ipos_port_id pid, IDS_MSG_NPM_STATUS *pmsg);

extern int IDS_Init(ipos_port_id pid, IDS_MSG_INIT *pmsg);

extern int IDS_BME_Reset_Blk(ipos_port_id pid );
extern int IDS_Clear_BME_Counters(ipos_port_id pid);

//extern int IDS_GetNC(ipos_port_id pid, IDS_MSG_NC *pmsg);
extern int IDS_Error(ipos_port_id pid, IDS_MSG_ERROR_STATUS *pmsg);
extern int IDS_Loading(ipos_port_id pid, IDS_LOADING_CODE *pids_loading_config, IDS_MSG_TONE_DATA *pmsg);
	
//extern int IDS_BitLoading(ipos_port_id pid, unsigned char cmd, IDS_MSG_BIT_STATUS *pmsg);
extern int IDS_678Data(ipos_port_id pid,  IDS_MSG_678_DATA *pmsg);
extern int IDS_678_5Data(ipos_port_id pid,  IDS_MSG_678_5DATA *pmsg);
extern int IDS_EventLog(ipos_port_id pid,IDS_MSG_EVENT_LOG *pmsg);

