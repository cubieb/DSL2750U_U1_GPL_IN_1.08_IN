/*========================================================================
**
** $Header: /cvshome/linux/fusiv_src/user/dsld/ife6/diags/fast_track/include/frmhal.h,v 1.1 2008/07/18 11:09:30 ksrao Exp $
**             
**      frmhal.h - include file for frmcpu's hal layer defines.  
**	  This file will be included in both C as well as Asm src.
**	  When included in C src, the pre-proc var C_CODE must be defined. 
**	  When C_CODE is not defined, only pre-proc statements must be included 
**	  in this file. And no statements which will require C compiler 
**	  (typedefs extern var declaration, etc) should be included here. 
**
**	Ver: 1.0;  Release date: 1/23/2001;   Author: Avadhani Shridhar. 
**                 Initial version. 
**
**	Copyright (c) 2000,2001 Ikanos Communication
**  This Ikanos Communication Inc. proprietary information must be 
**  maintained in the strictest of confidence and may not be copied, 
**  disclosed,or used without the express prior written consent of an  
**  officer of Ikanos Communication Inc.
**=======================================================================*/
#ifndef FRMHAL_H 
#define FRMHAL_H 

   // include frm accelerator related defines 
#include "frmacl.h"

/*----------------------------------------------------------------------
   FMC: Framer MicroController (aka FrmCpu) Interface defines 
-----------------------------------------------------------------------*/

#define MAX_NUM_CHANNELS     4 

//----------------- Structs for FrmCpu interface ---------------------- 
// ---- BmeTxCtlMsg_t;

#define  TX_CTL_MSG_PARAM_SIZE  10  // size in bytes of params field 
				    //   in tx ctl msg below.

//  The offsets of each field in the BmeTxCtlMsg_t struct from the start of the 
//  struct are defined for use in asm src, with 'FMC_TX_MSG_' prefix, after
//  the end of the struct. 
//

#if C_CODE 

typedef  struct { 
     u8  chNum;    // channel number: 0 to 31

     u8  newState; // if msb bit (bit 7) is 1, then change state at next frame, 
		   // if msb bit is 0, then change state at given Frame number
		   // list of possible states is described later
		   // if bit 6 is 1, then chg state & reset Frame number to 0 


         // Change state and do command at super-frame and frame number below 
         //   unless the msb bit of newState indicates that the state has to be 
         //   changed in next frame 
     u16 fullFrmNum; 
		   // in initialization, this is a 16-bit frame counter
		   // in showtime, msb byte has the super-frame number:0 to 255
		   //     and the lsb byte has the frame number: 0 to 68

     u16  cmdFlags;// indicate which commands to be done:

     u8  params[TX_CTL_MSG_PARAM_SIZE]; // parameters - depends on newState & cmdFlags.

} BmeTxCtlMsg_t;

#endif 

  // ---- define offsets of struct members for use in fmc asm language code
#define FMC_TX_MSG_chNum_OFST       0 
#define FMC_TX_MSG_newState_OFST    1 
#define FMC_TX_MSG_fullFrmNum_OFST  2 
#define FMC_TX_MSG_cmdFlags_OFST    4 
#define FMC_TX_MSG_params0_OFST     6 
#define FMC_TX_MSG_params1_OFST     7 
#define FMC_TX_MSG_params2_OFST     8 
#define FMC_TX_MSG_params3_OFST     9 
#define FMC_TX_MSG_params4_OFST    10 
#define FMC_TX_MSG_params5_OFST    11 
#define FMC_TX_MSG_params6_OFST    12 
#define FMC_TX_MSG_params7_OFST    13 
#define FMC_TX_MSG_params8_OFST    14 
#define FMC_TX_MSG_params9_OFST    15 

#define FMC_TX_CTL_PARAM_SIZE      10 

#define FMC_TX_MSG_SIZE		   16
#define FMC_TX_MSG_WORD_SIZE	    8

#define TX_MSG_PER_CHNL		    2
#define FMC_TX_MSG_SIZE_PER_CH	   32   // 2 * 16 = 32 bytes 
#define FMC_SHFT_TX_MSG_PER_CH	    5	// Right shift 5 = 32 (2*txCtlMsgSize(16))
#define FMC_SHFT_2_TX_MSG_SIZE	    5	// Right shift 5 = 32 (2*txCtlMsgSize(16))

//-------------------------------------------------------------------
// states specified by newState field can be: 

  //  these get converted to appropriate mapper mode (encoded in MapperMode 
  //  field of mapper's ext cmd block). 

  //  The first few states are same as mapper mode, & have the state number 
  //  to be same as the mapper mode number. 
#define  TX_SHOWTIME_STATE        0 
#define  TX_TONE_INP_STATE        1 
#define  TX_PSEUDO_RAND_STATE     2 
#define  TX_IFFT_INP_STATE        3 
#define  TX_INIT_DATA_STATE       4 
#define  TX_GEN_PR_SEQ_STATE      5 
#define  TX_VDSL_ROT_STATE        6

  //   next few states are used only by framer - CoreCpu uses this to 
  //   indicate to framer, the desired mode of operation. FrmCpu converts
  //   this to one of the above states plus other bit-settings in the ext
  //   cmd of the mapper. 
       // ---- used in ADSL only ---- 
#define  TX_REVERB_STATE          8 
#define  TX_SEGUE_STATE           9 
#define  TX_MEDLEY_STATE         10 
 
       // ---- used in VDSL only ---- 
#define  TX_OP_TRAIN_STATE           11 
#define  TX_OP_SYNCHRO_TRAIN_STATE   12 
#define  TX_OP_MEDLEY_STATE          13 
#define  TX_OP_SYNCHRO_MEDLEY_STATE  14 

  //   unused states 
// #define  TX_ONE_BIT_STATE        ?? 
// #define  TX_ONE_BYTE_STATE       ?? 
// #define  TX_DPSK_BIT_STATE       ?? 
#define  TX_DISABLED_STATE       15  // disabled or not yet configured 
 
  // a TxCtlMsg can indicate that current state is to be preserved. 
#define  TX_KEEP_CUR_STATE       16  // always the 2nd. last # in the enum 
#define  TX_ERROR_STATE		 17  // always the last # in the enum 

  // next few states are used only for testing mapper operation in C-Model
  // these states are converted by test program to reqd mapper ext cmds
  // with one of the valid mapper modes and/or enabling various mapper 
  // bypass options. 
#define  TX_SYNC_SYM_STATE       32 


//-------------------------------------------------------------------
// commands specified by cmdFlags field are: 

//--------- Any-state commands ------------------
 // #define  FMC_CMD_INTERUPT_CORE        0x8000  // bit 15 
    // - when this message processing is done by FrmCpu (and the first 
    //    4 bytes are set to 0xff), send an interrupt to core cpu. 
    //    This can be used for cases where CoreCpu needs to send 
    //    many messages one after another.

 // #define  FMC_CMD_RAISE_DMA_EVENT      0x4000  // bit 14 
    // - raise dma events as specified in params[1]
    //   params[1].bit0: raise dma event after Mapper has finished this symbol
    //   params[1].bit1: raise dma event after this symbol is written to Zfifo 

#define  FMC_CMD_SEND_EXT_PKT_HDR         0x2000  // bit 13 
    // - send tx-path extended command - is used for debugging purposes, 
    //   where CoreCpu provides the full cmd to be sent. The cmd can be upto 
    //   32-bytes and is given in a separate mem area which can hold upto 
    //   say 16 commands (define MAX_EXT_CMDS_IN_FMEM as 16). Then the command 
    //   to be used is specified by last byte of params (params[9]), with the 
    //   value ranging from 0 to 15. If this bit is set, then none of the 
    //   other bits can be set except for the interrupt core bit (bit 15). 

//--------- init-state commands ------------------
#define  FMC_CMD_SEND_ZFIFO_EXT_HDR       0x0001  // bit 0 
    // - appends zfifo ext cmd hdr to end of current pkt hdr,  
    //   And automatically removes the ext hdr at next symbol
    //   The 2 bytes of zfifo ext pkt hdr are in params[6] & params[7] 

#define  FMC_CMD_SET_TX_VBUS_ON           0x0003  // bit 1 
    // - appends zfifo ext cmd hdr to end of current pkt hdr,  
    //   And automatically removes the ext hdr at next symbol
    //   The 2 bytes of zfifo ext pkt hdr to turn on vbus must be 
    //   in params[6] & params[7] 

#define  FMC_CMD_ZFIFO_MASK               0x0003  
    // mask to select cmds which need zfifo ext pkt hdr 

  // skip z-fifo not needed as we can now send full z-fifo ext cmd
 // #define  FMC_CMD_SKIP_ZFIFO             0x0004  // bit 2 
    // - skip Z-fifo read ptr (for vdsl synchronization)
    //   number of bits to skip is given as part of zfifo ext pkt 
    //   hdr in params[2] & params[3] 

#define  FMC_CMD_DO_BIT_SWAP              0x0004  // bit 2 
    // - bit swap: to new frm config, & new bit-table
    //   params[5] has the new bit-table offset, and 
    //   ch's aoc buf has the new framer-config params. 

#define  FMC_MASK_NEED_MAPPER_EXT_HDR     0x0007
    // - used to check if we neeed to have mapper ext cmd part

#define  FMC_CMD_SET_CYC_PFX_ON           0x0008  // bit 3 
    // - sets cyclic_prefix on. All frames from this onwards have cyclic 
    //   extension on. 
    //   ?? Cmd to turn off cyclic extn is Not needed ?? 

#define  FMC_CMD_RESET_FRMNUM             0x0010  // bit 4 
    // - resets frm num after moving to this state. 

#define  FMC_MASK_REMOVE_MAPPER_EXT_HDR   0x0020  // bit 5
    // - used in zfifoFlag to remember that we have to remove mapper
    //   ext cmd part after the current symbol

#define  FMC_MASK_REMOVE_EXT_HDRS         0x0027
    // mask to select cmds which need to remove ext pkt hdr at end of symbol

    
 // #define  FMC_CMD_SET_IFFT_SHIFT       0x0008  // bit 3 
    // - sets ifft shift value. The shift amount (4-bits) is in bits 0 to 3 
    //   of the first byte of params[] part (params[0]). 

 // wrt afe reg not needed as we can use pio 
 // #define  FMC_CMD_WRT_AFE_REG          0x0200  // bit 09 
    // - write afe reg:  write given 32-bit value to given 
    //       32-bit address  = 8 bytes . The 8 bytes for Afe command are 
    //   written to a fixed place for each channel in 256 byte buffer. 
    //   the byte offset in this buffer, divided by 8, is given in params[1]
    //   later, we can optimize to reduce the size of this buffer to hold 
    //   a max of say 8 such afe reg-values - this should be enough since 
    //   we will rarely have to write afe regs through packet header. 

//--------- Showtime-state commands ------------------
 // eoc & aoc msgs now sent by CoreCpu by writing to circ buffer 
 // #define  FMC_CMD_SEND_EOC_MSG         0x0001  // bit 0 
    // - send eoc msg
    //   eoc msgs are written to a fixed place for each channel. This is 
    //   referred to as eoc msg # 0. One message per channel should be  
    //   enough, since we have to get acknowledgement for an eoc msg before 
    //   sending the next one. 
    //   If new msg is to be sent to FrmCpu while the current one is being 
    //   transmitted, then it is written to overflow eoc msg area which can 
    //   hold upto say 8 msgs (referred to as eoc msg # 1 to 8). 
    //   The # of times to send the msg, is given in msb 4 bits of param[2],
    //   and the eoc msg # is given in lsb 4 bits of params[2].
    //   The size (number of bytes) of eoc msg is given in params[3]. 

 // #define  FMC_CMD_SEND_AOC_MSG         0x0002  // bit 1 
    // - send aoc msg 5 times back-to-back
    //   params are encoded same as for eoc msgs, but in params[4] byte. 
    //   No need for overflow aoc msg area ? since we can always wait 
    //   for current aoc msg to end, before sending new one: in this case, 
    //   the aoc msg# is always 0. 
    //   The # of times to send the msg, is given in msb 4 bits of param[4],
    //   and the aoc msg # is given in lsb 4 bits of params[4].
    //   The size (number of bytes) of aoc msg is given in params[5]. 


//  FrmTxStatus_t  -- Tx Path Status struct contains:  
//   config parameters are set in FrmCpu's memory by CoreCpu 
//   status vars are maintained by FrmCpu (in FrmCpu's memory) 
// 
//  The offsets of each field in the TxStatus struct from the start of the 
//  struct are defined for use in asm src, with 'FMC_' prefix, after
//  the end of the struct. 
//
//  some showtime status struct members are redefined for init modes 
//  and are indicated by FMC_I_ prefix in offset defines. 
//

#if (C_CODE || defined(BME_BUILD))

typedef struct { 
    // ---- common fields -------------------------------
  u8   chNum; 		   // ch num 
  u8   chState; 	   // ch's current state 

  u16  frmFnAdr;  	   // Adr of fn which is to be called to do framing 
		   	   // diff fns used for init modes & for each framing
		   	   // mode in Showtime. 

    // ---- init mode fields -------------------------------
  u16  I_frmPf;            // bytes/symbol sent to mapper

  u8   I_synchroDuration;  // if 0: no synchro in OP-train/medley
			   // else: synchro duration (in OP-train 
			   // or medley), num of symbols of each 
			   // synchro portion (all 0 or all FF)
 
  u8   I_synchroCtr;       // # sym done in current synchro portion
 
  u8   I_Xdata0;           // first data byte read from X-fifo 
  // u8 itlv138FrmNum is overwritten on Xdata0 in showtime 

  u8   I_Xdata1;           // 2nd data byte read from X-fifo 
  // u8 itlvNiFrmNum is overwritten on Xdata1 in showtime 

  u8   I_XfifoFlag;        // 0: send data from ch's init data buf
							// 1: send data from ch's X-fifo-fast
  u8   subFrmNum;          //	for repeat framer features

  u8	frmIB; 
  u8	frmNTR; 
  u8	frmEOC; 
  u8	frmVOC; 

    // ---- showtime mode fields -------------------------------
   u16  frmPf;    	   // fast payload/frame: usually Kf-1 

   u8  	frmNf;    

   u8  	frmRf;    

   u16	frmUf; 

   u8	fastDummyZ; 

   u8	fastDummyRS; 

   u8	fastDzCnt;

   u8	fastDrsCnt;	

   u8	CowordPerSym; 

   u8	fastNextState; 

   u8	fastStateDataOfst; 

   u8	fastResidueCoword; 

   u8   fastCRC;     // holds last fast byte sent 

   //// u8   fastReserved; // for aligning to word boundary 

   u16  frmPi;    // ilv payload/frame: usually Ki-1 

   u8	frmNi;    // Ni=Ki+Ri 

   u8  	frmRi;  

   u16	frmUi; 
   //  u8  	frmPi2;    // 2nd codeword: ilv payload/frame: when S =1/2 

   u8	itlvDummyZ; 	
   //   u8  	frmDummys; // holds FrmDummy1 in lsb bit, and FrmDummy2 in msb 4 bits.

   u8	itlvDummyRS; 
   //   u8  	frmSi;    // Si >= 1: is same as S >= 1
		  // Si =  0: means S=1/2. 
		  // Si = -1: means no ilv part, Pi=0 

   u8	itlvDzCnt;
   u8	itlvDrsCnt;

   u8	itlvNextState; 
   u8	itlvResidueCoword; 	
   u16	itlvStateDataOfst; 

   u8   itlvCRC;     // holds last sync byte (ilv path's fast byte) sent 

   // u8   frmSymRsCtr;  // sym# in current RS codeword, reset value = 0 
   //   u8	bitsRemain;  - adsl only ? 

    // ---- common fields -------------------------------

   u8   zfifoFlag;    // whether zfifo ext pkt hdr to be sent in next symbol 

   u16 	fullFrmNum;   // Frm#, reset value = 0 
		// in initialization, this is a 16-bit frame counter
		// in showtime, msb byte has the super-frame number: 0 to 255
		//     and the lsb byte has the frame number: 0 to 68

   u8	fast138FrmNum;		// for fast path 			 
   u8	fastNfFrmNum;

   u8	itlvSupFrmNum;		// for interleaver			 
   u8	itlvFrmNum;

   u16 	lastStateChgFrmNum; // frm# of last state change, reset value = 0 

   u16  zfifoUsed;    // bytes in zfifo at start of current symbol.
   u16  zfifoThres;   // threshold to reach when we can idle
   u16  zfifoGained;  // bytes gained per symbol in zfifo:
		      //  is init to bytes written to zfifo per sym; 
		      //  then when Vbus is enabled, is changed to 
		      //    bytes_written - bytes_drained per symbol from zfifo
   u16  zfifoDrained; // bytes drained (read) per symbol from zfifo by Vbus 

} FrmTxStatus_t; 

#endif 

  // ---- define offsets of struct members for use in fmc asm language code
  // ---- showtime status struct members used for init modes only 

#define FMC_chNum_OFST        		0 
#define FMC_chState_OFST      		1  

#define FMC_frmFnAdr_OFST               2 

#define FMC_I_frmPf_OFST                4  
#define FMC_I_frmPfLowByte_OFST         5  

#define FMC_I_synchroDuration_OFST      6 
#define FMC_I_synchroCtr_OFST           7  

#define FMC_I_Xdata0_OFST               8 
#define	FMC_itlv138FrmNum_OFST			8			// share the memory
#define FMC_I_Xdata1_OFST               9
#define FMC_itlvNiFrmNum_OFST			9			// share the memory
#define FMC_I_XfifoFlag_OFST           10 
#define FMC_subFrmNum		           11

#define FMC_IB_OFST					   12
#define FMC_NTR_OFST				   13	
#define FMC_EOC_OFST				   14	 
#define	FMC_VOC_OFST				   15	

#define FMC_frmPf_OFST			16  
#define FMC_frmNf_OFST			18 
#define FMC_frmRf_OFST			19 
#define FMC_frmUf_OFST			20
#define FMC_fastDummyZ_OFST		22 
#define FMC_fastDummyRS_OFST		23
#define	FMC_fastDzCnt_OFST			24
#define	FMC_fastDrsCnt_OFST			25
#define FMC_CowordPerSym_OFST		26 
#define FMC_fastNextState_OFST		27 
#define FMC_fastStateDataOfst_OFST	28 
#define FMC_fastResidueCoword_OFST	30 
#define FMC_fastCRC_OFST			31 

#define FMC_frmPi_OFST			32 
#define FMC_frmNi_OFST			34 
#define FMC_frmRi_OFST			35 
#define FMC_frmUi_OFST			36
#define FMC_itlvDummyZ_OFST		38		
#define FMC_itlvDummyRS_OFST		39	
#define FMC_itlvDzCnt_OFST			40
#define FMC_itlvDrsCnt_OFST			41  

#define FMC_itlvNextState_OFST		42		
#define FMC_itlvResidueCoword_OFST	43 
#define FMC_itlvStateDataOfst_OFST	44
#define FMC_itlvCRC_OFST		46 
#define FMC_zfifoFlag_OFST		47 

#define FMC_fullFrmNum_OFST		48  
#define FMC_fastSupFrmNum_OFST		48  
#define FMC_fastFrmNum_OFST			49
#define	FMC_fast138FrmNum_OFST		50
#define FMC_fastNfFrmNum_OFST		51  

#define FMC_itlvSupFrmNum_OFST		52  
#define FMC_itlvFrmNum_OFST			53
#define FMC_lastStateChgFrmNum_OFST	54 

	
#define FMC_zfifoUsed_OFST		56  
#define FMC_zfifoThres_OFST		58  
#define FMC_zfifoGained_OFST		60  
#define FMC_zfifoDrained_OFST		62  



#if old_ADSL_offsets_needed 

  // ---- offsets of struct members used in old adsl code 
#define FMC_TX_STATUS_chNum_OFST       0 
#define FMC_TX_STATUS_chState_OFST     1  
#define FMC_TX_STATUS_frmModes_OFST    2 
#define FMC_TX_STATUS_frmPf_OFST       3 
#define FMC_TX_STATUS_frmSi_OFST       4 
#define FMC_TX_STATUS_frmPi_OFST       5 
#define FMC_TX_STATUS_frmNi_OFST       6 
#define FMC_TX_STATUS_frmDummys_OFST   7 
#define FMC_TX_STATUS_frmRfRi_OFST     9 
#define FMC_TX_STATUS_prsState_OFST    10 
#define FMC_TX_STATUS_frmFnAdr_OFST    12 
#define FMC_TX_STATUS_cfgReserved_OFST 14 
#define FMC_TX_STATUS_chStd_OFST        16 
#define FMC_TX_STATUS_frmSymRsCtr_OFST  17 
#define FMC_TX_STATUS_fastByte_OFST     18 
#define FMC_TX_STATUS_bitsRemain_OFST   18 
#define FMC_TX_STATUS_syncByte_OFST     19 
#define FMC_TX_STATUS_curDataByte_OFST  19 
#define FMC_TX_STATUS_fastAclCmdOp_OFST 20 
#define FMC_TX_STATUS_ilvAclCmdOp_OFST  22  
#define FMC_TX_STATUS_fullFrmNum_OFST   24  
#define FMC_TX_STATUS_supFrmNum_OFST    24  
#define FMC_TX_STATUS_frmNum_OFST       25  
#define FMC_TX_STATUS_lastStateChgFrmNum_OFST  26 
#define FMC_TX_STATUS_mapInpSize_OFST          28 
#define FMC_TX_STATUS_statusReserved_OFST      30 

#endif 


   // initial states & masks for G.DMT ATU-C's pseudo random generator 
   // pr seq defs for vdsl 
#define  PRS_VDSL_ATUCR_INIT_STATE  0xffe0
#define  PRS_VDSL_ATUCR_MASK        0x00a0
#define  PRS_VDSL_ATUCR_STATE_BITS  0xffe0
#define  PRS_VDSL_ATUCR_SIZE         4096   // bits per symbol

   // pr seq defs for adsl 
#define  PRS_GDMT_ATUC_INIT_STATE  0x0780 
#define  PRS_GDMT_ATUC_MASK        0x1080 
#define  PRS_GDMT_ATUC_STATE_BITS  0xff80 
#define  PRS_GDMT_ATUC_SIZE         512    // bits per symbol 

  // # of bits to skip after each medley
#define  FMC_VDSL_MEDLEY_SKIP_BITS    6 

#define  FMC_IDLE_WRITTEN    8832 // bytes written/symbol to zfifo 
#define  FMC_IDLE_THRESHOLD 23936 // threshold to reach when we can idle
				  //  set to fifoSize - 'bytes written per sym'
#define  FMC_IDLE_DRAINED    8192 // bytes drained/symbol from zfifo by Vbus
#define  FMC_IDLE_GAINED      640 // bytes gained/symbol in zfifo after Vbus
				  //   starts reading from zfifo

#ifndef BME_BUILD
  // defs related to framing microcode 
//#define FAST_PATH		0x00
//#define ITLV_PATH		0x01

#define Z_DUMMY			0x3A
#define RS_DUMMY		0xD3
#define HDLC_FLAG		0x7E

#define	ADSL_SUPER_FMC_SIZE		69
#define VDSL_SUPER_FMC_SIZE		11
#define VDSL_H_FMC_SIZE			138	// used to be 139 ? 
#endif

  // sync action types 
#define  FMC_FB_NO_SYNC_ACTION       0x0C   // fast byte: no sync 
#define  FMC_SB_NO_SYNC_ACTION       0x0C   // sync byte: no sync 

  // states in fmc mcode 
#define	Send_Fast_1st_Byte_State	0x00
#define	Send_Fast_EOC_State		0x01
#define	Send_Fast_Xfifo_Data_State	0x02
#define	Send_Fast_Dummy_Z_State		0x03
#define	Send_Fast_Dummy_RS_State	0x04
#define	Send_Itlv_1st_Byte_State	0x05
#define	Send_Itlv_VOC_State		0x06
#define	Send_Itlv_EOC_State		0x07
#define	Send_Itlv_Xfifo_Data_State	0x08	
#define	Send_Itlv_Dummy_Z_State		0x09
#define	Send_Itlv_Dummy_RS_State	0x0A

  // misc 
#define OP_TRAIN_SWAP2BITS_IN_MAPPER  1  

#define FMC_RESET_IB_BITS_MSB16 	0x0000 
#define FMC_RESET_IB_BITS_LSB16  	0x0000  


//----------------------------------------------------------------------
//  defs for Framer Cpu's  AclStatus reg, known as synrg
//----------------------------------------------------------------------
// framer accelerator based on accelerator cmds, sets bits in synrg

// when FrmSignalDone0 bit is set in FrmAcl cmd, after the cmd is done,
//  bit 2 in synrg is set
#define  FMC_SYNRG_FRM_DONE0_BIT        0x04


//----------------------------------------------------------------------
//  defs for accessing Framer Cpu's data memory 
//----------------------------------------------------------------------

// Accessing Framer Cpu's data memory is through the following macros, 
// so that the code can be easily ported among the foll. 3 cases: 
// 1. CoreCpu Firmware:
//	Use PIO address of Fmem & use address as offsets from that. 
//	Enabled by defining CORE_CPU_FIRMWARE before including this file. 
// 2. C-Model (FramerCpu microcode model):
//	Model Framer Cpu's data mem as array. 
//	Define access functions which read/write to array 
//	Enabled by defining C_MODEL_MCODE before including this file. 
// 3. C-Model (Tensilica firmware model) - for testing mcode in rtl 
//	simulation by gen tx ctl msgs. 
//	Define access functions which read/write to array and Print out 
//	PIO read/writes or FMEM sections or FMEM_MSG sections. 
//	Enabled by defining C_MODEL_CORE_CPU before including this file. 


#define FMC_MEM_SIZE  16384 /* 16KB */ 

#if C_MODEL_MCODE || C_MODEL_CORE_CPU 

#include <stdio.h> 

extern  u8  FrmCpuMem[FMC_MEM_SIZE];   // array which models fmem
#define FMC_MEM_START   ((u8 *)FrmCpuMem) // start of 'fmem' array 
FILE   *dmp_cmd_fp; 

#define isFmcValidAdr(adr) ((adr) < FMC_MEM_SIZE) 

#elif CORE_CPU_FIRMWARE 

#define FMC_MEM_START      ((u8 *)0xCE010000) // where fmem is mapped in PIO space 

#define isFmcValidAdr(adr) ( (((adr)-FMC_MEM_START) >= 0) && \
			      (((adr)-FMC_MEM_START) < FMC_MEM_SIZE) )

#endif 

#define isWordAdr(adr)      (((adr) & 0x1) == 0)

#if C_MODEL_MCODE || C_MODEL_CORE_CPU 

   // use fns to rd/wrt to array & then print PIO etc to test out file 
extern FmcRd_u8(u16 adr, u8 *val_p); 
extern FmcRd_u16(u16 adr, u16 *val_p); 
extern FmcRd_u32(u16 adr, u32 *val_p); 
extern FmcWrt_u8(u16 adr, u8 val); 
extern FmcWrt_u16(u16 adr, u16 val); 
extern FmcWrt_u32(u16 adr, u32 val); 

#endif 

#if 0  // if needed for speed, use macros: 
   // use macros 

// #define FmcRd_u8( u16 adr)  (*(u8  *)(FMC_MEM_START+(adr)))  
#define FmcRd_u16(adr, val_p)  \
			(*(u16 *)(*val_p) = *(u16 *)(FMC_MEM_START+(adr))) 
#define FmcRd_u32(adr, val_p)  \
			(*(u32 *)(*val_p) = *(u32 *)(FMC_MEM_START+(adr))) 

// #define FmcWrt_u8( u16 adr, u8  val)  (*(u8  *)(FMC_MEM_START+(adr)) = (val)) 
#define FmcWrt_u16(adr, val)  (*(u16 *)(FMC_MEM_START+(adr)) = (val)) 
#define FmcWrt_u32(adr, val)  (*(u32 *)(FMC_MEM_START+(adr)) = (val)) 

#endif 



//----------------------------------------------------------------------
//  defs for addresses & offsets in FrmCpu's data memory. 
//----------------------------------------------------------------------

// #define FMC_MEM_SIZE  16384 /* 16KB */ 


//----------------------------------------------------------------------
#if 0 
		New (Ver 1) MemMap of FMC 

 Variable   			16 ch   32 ch  StartAdr	 
---------- 			-----	-----  --------
GlobalVars:	992 		 992	992	0x0000 
Per Channel Memory: 
  IBrdIndex:	1 x 1  =  1 	 16	 32	0x03E0 
  AclState:	2 * 32 = 64 	1KB	2KB	0x0400 
  PktHdr:	1 x 32 = 32 	512	1KB	0x0C00
  CtlMsg: 	2 x 16 = 32 	512	1KB	0x1000 
  EOCbuf:	1 x 64 = 64 	1KB 	2KB	0x1400
  VOCbuf/InitData:1x64 = 64 	1KB 	2KB	0x1C00
  Status:	1 x 64 = 64 	1KB	2KB	0x2400
  IBbits:	1 x 8  =  8 	128	256	0x2C00
  EOCrdIndex:	1 x 2  =  2 	 32	 64	0x2D00 
  EOCwrtIndex:	1 x 2  =  2 	 32	 64	0x2D40 
  VOCrdIndex:	1 x 2  =  2 	 32	 64	0x2D80 
  VOCwrtIndex:	1 x 2  =  2 	 32	 64	0x2DC0 
  CfgData:	1 x 32 = 32 	512	1KB	0x2E00
AclCmdBuf:	1KB		1KB	1KB	0x3200 
Unused:		1.5K		1.5K	1.5K	0x3600 
Scratch(tmp):	512		512	512	0x3C00 
Stack:		512		512	512	0x3E00 

#endif 
//----------------------------------------------------------------------

/*-------------------------------------------------------------------------
FrmCpu memory allocation: A fixed memory allocation is used to simplify 
 coding.

* 0 to 4K-1: FrmAcl states, current pkt hdrs 
   @0, 896 bytes: scratch 
   @896, 128 bytes: ?? New TxConfig settings sent from CoreCpu. ??
   @1K, 2KB: FrmAcl states area: 2 states * 32 bytes/state * 32 channels 
   @3K, 1KB: Current Pkt Hdr area: 1 ext_hdr * 32 bytes/ext_hdr * 32 channels 
**-----------------------------------------------------------------------*/

/*------- DEMO VER SHOWTIME CODE's PreComputed Acl Cmds Buf ------------- 

  demo ver of showtime code reuses EOC buf area to keep pre-computed 
  acl cmds. this simple framing scheme does not support EOC msgs & so 
  can overwrite the EOC buf. 

**-----------------------------------------------------------------------*/

#define  FMC_ADR_SHOWTIME_ACL_CMDS      0x1400   // adr 5K - overwrite EOC buf
#define  FMC_SHOWTIME_ACL_CMD_BUF_SIZE  256      // alloc 512 bytes/ch
#define  FMC_SHFT1_SHOWTIME_ACL_CMDS      7      // total shift by 7+1=8
#define  FMC_SHFT2_SHOWTIME_ACL_CMDS      1
  

// #define  FMC_ADR_TX_CONFIGS  0x0200   // adr 512 
#define  FMC_ADR_ACL_STATES  0x0400   // adr 1K 
#define  FMC_ADR_PKT_HDRS    0x0C00   // adr 3K 


#define FMC_ACL_STATE_SIZE        32 
#define FMC_ACL_STATE_WORD_SIZE   16 
#define FMC_ACL_STATE_PER_CH_SIZE 64 
#define FMC_SHFT_ACL_STATE_PER_CH  6  // left shift amount to mult by 64 

#define FMC_PKT_HDR_SIZE          32  // incl max ext cmds 
#define FMC_PKT_HDR_WORD_SIZE     16 
// #define FMC_PKT_HDR_CPY_WORD_SIZE  4 
// #define FMC_PKT_HDR_CPY_BYTE_SIZE  8 
#define FMC_SHFT_PKT_HDR           5  // left shift amount to mult by 32 

    // returns adrs of channel's fast path frm acl state in fcpu's memory.
#define  FmcAclStateAdr( ch_num)   \
    (FMC_ADR_ACL_STATES + (ch_num << FMC_SHFT_ACL_STATE_PER_CH)) 

    // returns adrs of channel's pkt hdr area in fcpu's memory.
#define  FmcPktHdrAdr( ch_num)   \
    (FMC_ADR_PKT_HDRS + (ch_num << FMC_SHFT_PKT_HDR)) 

  // scratch mem (adr: 0 to 511) use: 

// global pointers
// NOTE: ToDo:  fmcdata.s defines data locations which overlap with 
//   defines below. 
#define FMC_ADR_softSymSignal 	  0x0102 
#define FMC_ADR_frmAclCmdAdr      0x010c

#define FMC_FAST_ACL_CMD_OP_ADDR    252   // this is for test number
#define FMC_ITLV_ACL_CMD_OP_ADDR	    254   

      // dflt pkt hdr & status struct for each state 
#define FMC_ADR_ToneInpDfltPktHdr   256 
#define FMC_ADR_ToneInpDfltStatus   288  // =256+32

#define FMC_ADR_InitDataDfltPktHdr  304  // =288+16 
#define FMC_ADR_InitDataDfltStatus  336  // =304+32

#define FMC_ADR_OpTrainDfltPktHdr   352  // =336+16 

  // dflt status of vsl mapper related states must be kept one after 
#define FMC_ADR_OpTrainDfltStatus   384  // =352+32

// #define FMC_ADR_OpSynchroTrainDfltStatus   400  // =384+16
// #define FMC_ADR_OpMedleyDfltStatus         416  // =400+16
// #define FMC_ADR_OpSynchroMedleyDfltStatus  432  // =416+16

#define FMC_ADR_ShowtimeDfltPktHdr  400  // =384+16 
#define FMC_ADR_ShowtimeDfltStatus  432  // =400+32

#define FMC_ADR_NotConfigDfltStatus 448  // =432+16


/*-------------------------------------------------------------------------
* 4K to 8K-1: CoreCpu to FrmCpu TxCtlMsg, EOC,AOC message, etc. 
   @4K, 1KB: TxCtlMsg area: 2 msgs * 16 bytes/msg * 32 channels 
   @5K, 0.5KB: EOC msg area:  1 msg * 32 bytes/msg * 16 channels
   @5.5K, 0.5KB: CtlMsgData area: 1 msg * 32 bytes/msg * 16 channels
   @6K, 0.5KB: AOC or VOCmsg area:     1 msg * 32 bytes/msg * 16 channels
   @6.5K, 0.5KB: InitData area: 1 msg * 32 bytes/msg * 16 channels
   @7K, 192: IB bits area:  2 sets of IB bits * 3 bytes/set * 32 channels
   @7K+256, 256: AFE RegWrt area:  1 set of adr+val * 4 bytes/set * 32 channels
//  @7K+512, 256: EOC overflow:  8 msg * 32 bytes/msg: common to all channels 
   @7K+512, 256: Total space
		32: EOC read index:   1 read index(2 bytes) * 16 channels
		32: VOC read index:   1 read index(2 bytes) * 16 channels
		32: EOC write index:  1 write index(2 bytes) * 16 channels
		32: VOC write index:  1 write index(2 bytes) * 16 channels
		32: IB  write index:  1 write index(1 byte) * 16 channels

**-----------------------------------------------------------------------*/
#define FMC_ADR_IB_READ_INDEX               0x03E0	  
#define  FMC_ADR_TX_CTL_MSGS            	0x1000		// adr 4K
#define  FMC_ADR_EOC_MSGS                   0x1400		// adr 5K
#define  FMC_SHFT_ADR_EOC_MSGS          	0x05		// 32 byte shift
#define  FMC_ADR_VOC_MSGS                   0x1C00		
#define  FMC_SHFT_ADR_VOC_MSGS          	0x05		// 32 byte shift
#define  FMC_ADR_INIT_DATA_BUFS				0x1C00
#define  FMC_ADR_AOC_MSGS					0x1C00		
#define  FMC_SHFT_ADR_AOC_MSGS          	0x05		// 32 byte shift
#define  FMC_ADR_IB_BITS                    0x2C00
#define  FMC_SHFT_ADR_IB_BITS           	0x03		// 8 byte shift

#define FMC_READ_WRITE_INDEX_OFST           64			// 32 EOC + 32 VOC/AOC 
#define FMC_ADR_EOC_READ_INDEX              0x2D00  
#define FMC_ADR_VOC_READ_INDEX              0x2D80		
#define FMC_ADR_AOC_READ_INDEX              0x2D80		
#define FMC_ADR_EOC_WRITE_INDEX             0x2D40  
#define FMC_ADR_VOC_WRITE_INDEX             0x2DC0		
		
#define  FMC_ADR_CFG_DATA_BUFS				0x2E00		
	
#define FMC_EOC_RING_BUF_WRAP              	0x3F	// wrap after 64 bytes
#define FMC_VOC_RING_BUF_WRAP              	0x3F	// wrap after 64 bytes
#define FMC_AOC_RING_BUF_WRAP              	0x3F	// wrap after 64 bytes
#define FMC_IB_RING_BUF_WRAP               	0x07	// wrap after  8 bytes

#define FMC_TX_CTL_MSG_SIZE         16 
#define FMC_TX_CTL_MSG_PER_CH_SIZE  32  // 2 msgs per ch. 
#define FMC_SHFT_TX_CTL_MSG_PER_CH   5  // left shift amount to mult by 32 

#define FMC_AOC_MSG_PER_CH_SIZE     32  // 1 msg per ch. 
#define FMC_SHFT_AOC_MSG_PER_CH	     5  // left shift amount to mult by 32 

#define FMC_CFG_DATA_SIZE           32  
#define FMC_CFG_DATA_WORD_SIZE      16  
#define FMC_SHFT_CFG_DATA_PER_CH     5  // left shift amount to mult by 32 

#define FMC_INIT_DATA_SIZE          32  
#define FMC_INIT_DATA_WORD_SIZE     16  
#define FMC_SHFT_INIT_DATA_PER_CH    5  // left shift amount to mult by 32 

#define FMC_SHFT_EOC_READ_INDEX_PER_CH   1  // 2 bytes per index 
#define FMC_SHFT_EOC_WRITE_INDEX_PER_CH  1  // 2 bytes per index 
#define FMC_SHFT_VOC_READ_INDEX_PER_CH   1  // 2 bytes per index 
#define FMC_SHFT_VOC_WRITE_INDEX_PER_CH  1  // 2 bytes per index 

    // returns adrs of channel's first TxCtlMsg structures in fcpu's memory. 
#define  FmcTxCtlMsgAdr( ch_num)   \
     (FMC_ADR_TX_CTL_MSGS + ((ch_num) << FMC_SHFT_TX_CTL_MSG_PER_CH)) 

    // returns adrs of channel's aoc msg area in fcpu's memory. 
#define  FmcAocMsgAdr( ch_num)   \
     (FMC_ADR_AOC_MSGS + ((ch_num) << FMC_SHFT_AOC_MSG_PER_CH)) 

    // returns adrs of channel's msg data buffer area in fcpu's memory. 
#define  FmcCfgDataAdr( ch_num)   \
     (FMC_ADR_CFG_DATA_BUFS + ((ch_num) << FMC_SHFT_CFG_DATA_PER_CH)) 

    // returns adrs of channel's init data buffers area in fcpu's memory. 
#define  FmcInitDataAdr( ch_num)   \
     (FMC_ADR_INIT_DATA_BUFS + ((ch_num) << FMC_SHFT_INIT_DATA_PER_CH)) 

    // returns adrs of channel's EOC msg buffers area in fcpu's memory. 
#define  FmcEocMsgAdr( ch_num)   \
     (FMC_ADR_EOC_MSGS + ((ch_num) << FMC_SHFT_ADR_EOC_MSGS)) 

    // returns adrs of channel's VOC msg buffers area in fcpu's memory. 
#define  FmcVocMsgAdr( ch_num)   \
     (FMC_ADR_VOC_MSGS + ((ch_num) << FMC_SHFT_ADR_VOC_MSGS)) 

    // returns adrs of channel's IB bits data buffers area in fcpu's memory. 
#define  FmcIBbitsAdr( ch_num)   \
     (FMC_ADR_IB_BITS + ((ch_num) << FMC_SHFT_ADR_IB_BITS)) 

    // returns adrs of channel's EOC read index in fcpu's memory. 
#define  FmcEocRdIndexAdr( ch_num)   \
     (FMC_ADR_EOC_READ_INDEX + ((ch_num) << FMC_SHFT_EOC_READ_INDEX_PER_CH)) 

    // returns adrs of channel's EOC write index in fcpu's memory. 
#define  FmcEocWrtIndexAdr( ch_num)   \
     (FMC_ADR_EOC_WRITE_INDEX + ((ch_num) << FMC_SHFT_EOC_WRITE_INDEX_PER_CH)) 

    // returns adrs of channel's VOC read index in fcpu's memory. 
#define  FmcVocRdIndexAdr( ch_num)   \
     (FMC_ADR_VOC_READ_INDEX + ((ch_num) << FMC_SHFT_VOC_READ_INDEX_PER_CH)) 

    // returns adrs of channel's VOC write index in fcpu's memory. 
#define  FmcVocWrtIndexAdr( ch_num)   \
     (FMC_ADR_VOC_WRITE_INDEX + ((ch_num) << FMC_SHFT_VOC_WRITE_INDEX_PER_CH)) 


/*-------------------------------------------------------------------------
* 8K to 11K-1: Channel Tx Configs area
//   @8K, 1KB: Channel TxCfg+TxStatus area: 16+16=32 bytes * 32 channels
   @8K, 1KB: Channel TxCfg+TxStatus area: 64 bytes * 16 channels (VDSL) 
   @9K, 512: Global TxCfg area: 40 bytes per std
   @9K+512, 1.5KB: reserved for data transfer between CoreCpu & FrmCpu 
     @10K, 1KB: FrmAcl Cmd trace buffer for debugging. 
**-----------------------------------------------------------------------*/

// #define  FMC_ADR_CH_CONFIGS    0x2000   // adr 8K 
#define  FMC_ADR_TX_STATUS_AND_CONFIGS		0x2400   // adr 8K 
//#define  FMC_ADR_GLB_CONFIGS			0x2400   // adr 9K 

// #define  FMC_TX_CONFIG_SIZE			16 
// #define  FMC_TX_CONFIG_WORD_SIZE		8 
// #define  FMC_SHFT_TX_CONFIG			4  // left shift amount to mult by 16 

#define FMC_TX_INIT_STATUS_SIZE			16 // for initializing
#define FMC_TX_INIT_STATUS_WORD_SIZE		8 
#define FMC_SHFT_TX_INIT_STATUS			4  // left shift amount to mult by 16 
#define FMC_TX_FULL_STATUS_SIZE			64 
#define FMC_TX_FULL_STATUS_WORD_SIZE		32 
#define FMC_SHFT_TX_FULL_STATUS			6  // left shift amount to mult by 64 
#define FMC_TX_STATUS_AND_CONFIG_SIZE		64 
#define FMC_SHFT_TX_STATUS_AND_CONFIG		6  // left shift amount to mult by 64 

    // returns adrs of channel's config & status structures in fcpu's memory.
#define  FmcCfgAndStatusAdr( ch_num)   \
    (FMC_ADR_TX_STATUS_AND_CONFIGS + (ch_num << FMC_SHFT_TX_STATUS_AND_CONFIG))

   // NOW, Status struct adr is same as CfgAndStatusAdr 
#define  FmcStatusAdr( ch_num)   FmcCfgAndStatusAdr(ch_num)  

#define  FmcFullFrmNumAdr(ch_num) (FmcStatusAdr(ch_num) + FMC_fullFrmNum_OFST) 

   // adr of showtime config parameter part of ch's Status struct 
#define  FmcShowtimeCfgAdr( ch_num) (FmcStatusAdr(ch_num) +  FMC_frmPf_OFST) 

#define  FMC_SHOWTIME_CFG_WORD_SIZE 16 // is equal to
			      // (FMC_frmSymRsCtr_OFST - FMC_frmPf_OFST)/2); 

// We keep a txDbgMsgBuf at location	9K+512
#define  FMC_ADR_DBG_MSG_BUF				0x2600   // adr 9K + 512
#define  TX_WRITE_PTR						GLOBAL_VAR_BASE_ADR	

    // For Debug purposes, we keep a small circular buffer of FrmAcl commands 
    // sent. Use 1K from 10 for this purpose. 

#define  FMC_ADR_ACL_CMD_BUF_START			0x3200  // adr 12.8K 
#define  FMC_ADR_ACL_CMD_BUF_END			0x35FF  // 

#define  FMC_ADR_TX_ACL_CMD_BUFS			0x3200  // start from adr 10K 
#define  FMC_TX_ACL_CMD_BUF_SIZE			128   // each buf is 128 bytes 
#define  FMC_SHFT_TX_ACL_CMD_BUF			7   // shift amount to mult/div by 128 
#define  FMC_NUM_TX_ACL_CMD_BUF				4   // 4 such bufs. 
#define  FMC_MASK_NUM_TX_ACL_CMD_BUF		3   // mask to limit num bufs to 4 (0 to 3). 


/*-------------------------------------------------------------------------
* 11K to 15K-1: reserved for data transfer from X-fifo. 
	Can be used, if processing to be done before sending data out

* 15K to 16K-1: 
   @15K, 512 bytes: Scratch mem 
   @15K+512, 512: bytes stack area 

-------------------------------------------------------------------------*/

#define  FMC_ADR_SCRATCH       0x3C00   // adr 15K 

          // stack = 252 bytes from 15K+512 to 16K-4
#define  FMC_ADR_STK_END       0x3E00   // adr 15K+512
#define  FMC_ADR_STK_BEGIN     0x3FFC   // 16K - 4

#define  FMC_ADR_STACK         0x3E00   // adr 15K+512 

#endif

