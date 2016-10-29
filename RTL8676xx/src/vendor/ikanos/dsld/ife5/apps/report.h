/**************************************************************
$Workfile:   report.h  $
***************************************************************
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
$Revision: 1.1 $
$Date: 2008/07/18 11:08:38 $
**************************************************************/                    

			 


/*******************************************************************

	Thsi header file includes the definitions and  function prototypes for 
		1. print: port brief status
		2. print: extended port status
		3. print: the port counter
		4. print: the port NE SNR ATTN
		5. print: API status
			
	
*********************************************************************/
	
#define	  	OAMP_IN_PROGRESS	0			/* oamp function status	*/
#define 	OAMP_COMPLETE		1			/* oamp function status	*/
#define 	OAMP_FAILED			2			/* oamp funcion status	*/
#define 	OAMP_RESTART   		3			/* oamp funcion status	*/

							
typedef struct
{
	U16		SlowUsFFTProtection;
	U16		SlowDsFFTProtection;
	U8		FastDsNumParityBytes;
	U8		SlowDsNumParityBytes;
	U8		FastUsNumParityBytes;
	U8		SlowUsNumParityBytes;
	U8		FastDsCodeWordSize;
	U8		SlowDsCodeWordSize;
	U8		FastUsCodeWordSize;
	U8		SlowUsCodeWordSize;
	U8		DsInterleaverDepth;
	U8		UsInterleaverDepth;
	U8		DsInterleaverBlkLen;
	U8		UsInterleaverBlkLen;
	U8		dataIsValid;
	U32		LineType;

} ShowtimeParams;
							
									  

typedef enum {
  FLR_LOS = 0,
  FLR_LOF,
  FLR_LPR,
  FLR_LOM,
  FLR_ESE,
  FLR_NCD_S,
  FLR_LCD_S,
  FLR_NCD_F,
  FLR_LCD_F,
  FLR_FE_LOS,
  FLR_FE_RDI,
  FLR_FE_LPR,
  FLR_FE_LOM,
  FLR_FE_ESE,
  FLR_FE_NCD_S,
  FLR_FE_LCD_S,
  FLR_FE_NCD_F,
  FLR_FE_LCD_F,
  FLR_FE_PO,
} ePmIndex;

 /* Mapping of indices of failures to the corresponding bit-indices
   * in the 32-bit failure alert word (Failures element of tPmAlert).
   * Should be in sync with the IPOS document */
typedef struct 
 {
   char fstring[20];
 } failureBitString;



extern void	uiLogPortStatusBrief(ipos_port_id pid, ipos_port_status_brief	*pPortStatus);

/* bandRate=(64*94)/100 for BP3_998_4.otherwise: bandRate=64	*/

extern void	uiLogPortStatus(ipos_port_id pid, ipos_port_status	*pPortStatus, int bandRate);   

extern void	uiLogPortCounters(ipos_port_id pid, ipos_port_counters	*pPortCounters, send_ack_info_t *psend_ack_info);

extern void	uiLogPortNeSnr(ipos_port_id pid, ipos_port_nesnrattn  *pPortNeSnr);

extern void	uiLog(ipos_port_id pid, char *funcName,U8 cmdtag, int status, int errCode);

extern void	uiLogDeviceVer(int bmeNum, int	len, char *string);
extern void displayPerfCntrs(ipos_port_id Pid, send_ack_info_t *psend_ack_info);

extern void displayPmCounters(ipos_port_id Pid, send_ack_info_t *psend_ack_info);
extern ShowtimeParams	*getShowtimeParamsRef( ipos_port_id pid );
extern void		 displayShowtimeParams( ipos_port_id pid );
extern void		 storeShowtimeParams( ipos_port_id pid, const ipos_port_status *pPortStatus );
extern void		 invalidateShowtimeParams( ipos_port_id pid );
extern void uiLogPortPmCounters(ipos_port_id pid, ipos_pm_counters *pPmCntr, send_ack_info_t *psend_ack_info);

