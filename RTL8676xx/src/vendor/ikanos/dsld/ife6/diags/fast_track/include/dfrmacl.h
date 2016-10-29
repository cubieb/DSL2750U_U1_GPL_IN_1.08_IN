
#ifndef DFRMACL_H 
#define DFRMACL_H 

/*========================================================================
**            
**      dfrmacl.h - include file for deframer accelerator related defines
**
**	  This file will be included in both C as well as Asm src.
**	  When included in C src, the pre-proc var C_CODE must be defined. 
**	  When C_CODE is not defined, only pre-proc statements must be included 
**	  in this file. And no statements which will require C compiler 
**	  (typedefs extern var declaration, etc) should be included here. 
**
**	Copyright (c) 2000,2001 Ikanos Communication
**  This Ikanos Communication Inc. proprietary information must be 
**  maintained in the strictest of confidence and may not be copied, 
**  disclosed,or used without the express prior written consent of an  
**  officer of Ikanos Communication Inc.
**=======================================================================*/


//------------- defs for addresses & ops --------------------------------- 

  //---- addresses of Fifos in deframer acl cmds (bit 15 must be 1) ---- 
#define  BF_FIFO_ADR  		0x8900 
#define  BI_FIFO_ADR  		0x8a00 
#define  C_FIFO_ADR  		0x8b00 
#define  RSDECBUFFER_ADR  	0x8c00 


  //---- Group0 deframer ops ---- 
#define  DFOP_CrcOn					0x0010 /* 0=data, 1= pkt hdr: no crc etc. */ 
#define  DFOP_ScrOn					0x0020 /* if 1, scrambler is on */ 
#define  DFOP_BypassAtmSwap			0x0040 /* if 1, rs enc is on */ 
#define  DFOP_IdleOff				0x0080 /* if 1, idle cell insertion is OFF */ 

#define  DFOP_AtmHecScrOff			0x0100 /* if 1, atm hec & scrambling is OFF */ 
#define  DFOP_DeFrameDone			0x0200 /* if 1, then this has last byte of */ 
#define  DFOP_SetChState			0x0400 /* if 1, then this has last byte of */ 
#define  DFOP_SaveChState			0x0800 /* if 1, bypass the 'byte swapper' at  */ 

#define  DFOP_DeFrmSignalDone0		0x1000 /*   */ 
#define  DFOP_DeFrmSignalDone1		0x2000 /*   */

#define  DFOPS_AtmHecScrOff_BypassAtmSwap	0x0140 

  //---- Group1 rx ops ---- 
#define  DFOP_DeMapperRead			0x4001	/*  if 1, then the nbytes field is sent */
											/*  to interleaver to indicate num */
											/*  bytes to read from interleaver mem */ 
											/*  note: this does not transfer data */

#define  DFOP_RdIlvDummyBytes		0x4006  /*  bits 2,3 indicate dummy bytes to be */ 
											/*  deleted during read from intrlv mem */

#define  DFOP_RsDecFromBfifo		0x4010	/*  if 1, then Set framerState for given ch */
											/*  & for given path to values stored in  */ 
											/*  FrmCpu's data memory */
#define  DFOP_RsDecFromBfifo_off	0x4010
#define  DFOP_RsDecFromBfifo_on		0x4030
#define  DFOP_RsDecOn				0x4020 
#define  DFOP_SetRsChkBytes			0x4040	/*  if 1, then Save framerState for given ch */
											/*  & for given path, from FrmAcl & save */ 
											/*  them in FrmCpu's memory */ 

  //---- Group2 deframer ops ---- 
#define  DFOP_GetPktHdr				0x8001	/*  0=data, 1= pkt hdr: no crc etc. */ 


//------------- macros to chk if an op is set ---------------------------- 

#define  isBF_FIFO_ADR(x)   (((x) & 0xff00) == BF_FIFO_ADR) 
#define  isBI_FIFO_ADR(x)   (((x) & 0xff00) == BI_FIFO_ADR) 
#define  isRSDECBUFFER_ADR(x)   (((x) & 0xff00) == RSDECBUFFER_ADR) 
#define  isC_FIFO_ADR(x)   (((x) & 0xff00) == C_FIFO_ADR) 

  //---- Group0 deframer ops ----
#define  isDFOP_SetChState(x)		((x) == DFOP_SetChState) 
#define  isDFOP_SaveChState(x)		((x&0xc800) == DFOP_SaveChState) 
#define  isDFOP_WrtToCfifo(x)		(((x & 0xc000) == 0) && (x & 0x03f0))
#define  isDFOP_CrcOn(x)		(((x >> 4)&0x01) == 1) 
#define  isDFOP_ScrOn(x)		(((x >> 5)&0x01) == 1) 
#define  isDFOP_BypassAtmSwap(x)	(((x >> 6)&0x01) == 1) 
#define  isDFOP_IdleInsertOff(x)	(((x >> 7)&0x01) == 1) 
#define  isDFOP_AtmHecScrOff(x)		(((x >> 8)&0x01) == 1) 


  //---- Group1 deframer ops ----
#define  isDFOP_DeMapperRead(x)   (((x) & DFOP_DeMapperRead) == DFOP_DeMapperRead) 
#define  isDFOP_RsDecFromBfifo(x) (((x) & DFOP_RsDecFromBfifo) == DFOP_RsDecFromBfifo) 
#define  isDFOP_RsDecOn(x)  	  (((x) & DFOP_RsDecOn) == DFOP_RsDecOn) 
#define  isDFOP_SetRsChkBytes(x)  (((x) & DFOP_SetRsChkBytes) == DFOP_SetRsChkBytes) 


  //---- Group2 deframer ops ----
#define  isDFOP_GetPktHdr(x)   (((x) & DFOP_GetPktHdr) == DFOP_GetPktHdr) 


//------------- defs for dfrm acl state fields ---------------------------- 

// DeframerAcl keeps its state of each path (fast, intrlv) for each channel in 
//  the data memory of Dmc. The offsets below indicates the offsets within 
//  the block of memory used to hold a state. 
// 
// ToDo: Update struct for DeframerAcl state to match final hardware states 
//	 so that microcode can correctly set initial values for scr & crc
//	 states, & reset to 0 the diff error counters. Update the related
//	 OFST defs also. 
//
// Note: the struct DeframerAcl_t defined below is simplify Xtensa code 
//  	 for accessing fields of deframer acl state. It is different from 
//	 the struct used in ASIC C model (DfmAclState_t) which keeps the 
//	 deframer acl state in a format which speeds up C model code. 

#if C_CODE 

typedef struct {  /* Deframer-Accelarator command struct */ 
		  /*  see below for details */
	u8	rs_corrected_cum;
	u8	rs_uncorrected_cum;
        u8  	atm_scr_state[6];
	u8  	crc_state;
	u8  	scr_state[3];
	u8  	cellsize;
    	u8  	atm_ocd_errors_cum;
    	u8  	atm_hec_errors_cum;
    	u8  	atm_hec_sync_data[6];

	u8	atm_bit_position_state_info;
	u8	atm_data_cell_counter;
	u8	atm_delta_alpha_count;
	u8	atm_idle_and_syncfound;
	u8      atm_idle_count;

} DmcAclState_t; 

#endif 

#define DFM_RS_CORRECTED_CUM_OFST  0
#define DFM_RS_UNCORRECTED_CUM_OFST  1
#define DFM_ATM_DESCR_STATE_OFST  2
#define DFM_ATM_DESCR_STATE_SIZE  6
#define DFM_ST_CRC_OFST  8  // offset of crc byte in a save_state block. 
#define DFM_DSL_DESCR_STATE_OFST  9
#define DFM_DSL_DESCR_STATE_SIZE  3
#define DFM_ST_CELLSIZE_OFST  12
// #define DFM_ST_ATM_BIT_POS_STATE_INFO_OFST  13
#define DFM_HEC_ERROR_CUM_OFST  14  // offset of cellSize byte in a save_state block. 
 
#define DFM_ATM_HUNT_DATA_OFST  15
#define DFM_ATM_HUNT_DATA__SIZE  6

#define DFM_ATM_PREV_DATA_OFST  20
#define DFM_ATM_BIT_POSITION_OFST  21
#define DFM_ATM_STATE_INFO_OFST  22
#define DFM_ATM_PSYNC_CNT_OFST  23
#define DFM_ATM_DELTA_CNT_OFST  24
#define DFM_ATM_ALPHA_CNT_OFST  25
#define DFM_ATM_SYNC_CNT_OFST  26
#define DFM_ATM_VBYTE_CNT_OFST  27
#define DFM_ATM_IDLE_OR_REAL_OFST  28


// reset values for fields in Dfrm Acl State 
#define DFM_CELLSIZE_RESET_VAL   53     // 0x35  hex 
#define DFM_CRC_RESET_VAL        0x00  

#define DFM_SCR_BYTE0_RESET_VAL  0xff  
#define DFM_SCR_BYTE1_RESET_VAL  0xff  
#define DFM_SCR_BYTE2_RESET_VAL  0xff  


#endif 

