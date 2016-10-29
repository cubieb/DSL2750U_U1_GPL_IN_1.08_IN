#ifndef _XDSL_FMC_H
#define _XDSL_FMC_H

#include "bme.h"

#define  TX_CTL_MSG_PARAM_SIZE  10  // size in bytes of params field 
				    
typedef  struct { 
     u8  chNum;    						// channel number: 0 to 31
     u8  newState; 						// if msb bit (bit 7) is 1, then change state at next frame, 
		   								// if msb bit is 0, then change state at given Frame number
		   								// list of possible states is described later
         								// Change state and do command at super-frame and frame number below 
         								// unless the msb bit of newState indicates that the state has to be 
         								// changed in next frame       
	 u16 fullFrmNum;	   				// in initialization, this is a 16-bit frame counter
		   								// in showtime, msb byte has the super-frame number:0 to 255
		   								// and the lsb byte has the frame number: 0 to 68
     u16  cmdFlags;						// indicate which commands to be done:
     u8  params[TX_CTL_MSG_PARAM_SIZE]; // parameters - depends on newState & cmdFlags.
} BmeTxCtlMsg_t;						// total 16 bytes


//-----------------------------------------------------------------------------
// 	states specified by newState field can be: 
//  	these get converted to appropriate mapper mode (encoded in MapperMode 
//  	field of mapper's ext cmd block). 
//  	The first few states are same as mapper mode, & have the state number 
//  	to be same as the mapper mode number. 
//------------------------------------------------------------------------------
#define  TX_SHOWTIME_STATE        	0 
#define  TX_TONE_INP_STATE        	1 
#define  TX_PSEUDO_RAND_STATE     	2 
#define  TX_IFFT_INP_STATE        	3 
#define  TX_INIT_DATA_STATE       	4 
#define  TX_GEN_PR_SEQ_STATE      	5 
#define  TX_VDSL_ROT_STATE        	6

//---------------------------------------------------------------------------
//   next few states are used only by framer - CoreCpu uses this to 
//   indicate to framer, the desired mode of operation. FrmCpu converts
//   this to one of the above states plus other bit-settings in the ext
//   cmd of the mapper. 
//----------------------------------------------------------------------------

// ---- used in ADSL only ---- 
#define  TX_REVERB_STATE          	8 
#define  TX_SEGUE_STATE           	9 
#define  TX_MEDLEY_STATE         	10 
 
// ---- used in VDSL only ---- 
#define  TX_OP_TRAIN_STATE          11 
#define  TX_OP_SYNCHRO_TRAIN_STATE  12 
#define  TX_OP_MEDLEY_STATE         13 
#define  TX_OP_SYNCHRO_MEDLEY_STATE 14 

#define  TX_DISABLED_STATE       	15  // disabled or not yet configured 

// a TxCtlMsg can indicate that current state is to be preserved. 
#define  TX_KEEP_CUR_STATE       	16  // always the 2nd. last # in the enum 
#define  TX_ERROR_STATE		 		17  // always the last # in the enum 

// next few states are used only for testing mapper operation in C-Model
// these states are converted by test program to reqd mapper ext cmds
// with one of the valid mapper modes and/or enabling various mapper 
// bypass options. 
#define  TX_SYNC_SYM_STATE       	32

//-------------------------------------------------------------------
// commands specified by cmdFlags field are: 

//--------- Any-state commands ------------------
#define  FMC_CMD_INTERUPT_CORE        		0x8000  // bit 15 
    // - when this message processing is done by FrmCpu (and the first 
    //    4 bytes are set to 0xff), send an interrupt to core cpu. 
    //    This can be used for cases where CoreCpu needs to send 
    //    many messages one after another.

#define  FMC_CMD_RAISE_DMA_EVENT      		0x4000  // bit 14 
    // - raise dma events as specified in params[1]
    //   params[1].bit0: raise dma event after Mapper has finished this symbol
    //   params[1].bit1: raise dma event after this symbol is written to Zfifo 

#define  FMC_CMD_SEND_EXT_PKT_HDR         	0x2000  // bit 13 
    // - send tx-path extended command - is used for debugging purposes, 
    //   where CoreCpu provides the full cmd to be sent. The cmd can be upto 
    //   32-bytes and is given in a separate mem area which can hold upto 
    //   say 16 commands (define MAX_EXT_CMDS_IN_FMEM as 16). Then the command 
    //   to be used is specified by last byte of params (params[9]), with the 
    //   value ranging from 0 to 15. If this bit is set, then none of the 
    //   other bits can be set except for the interrupt core bit (bit 15). 

//--------- init-state commands ------------------
#define  FMC_CMD_SEND_ZFIFO_EXT_HDR       	0x0001  // bit 0 
    // - appends zfifo ext cmd hdr to end of current pkt hdr,  
    //   And automatically removes the ext hdr at next symbol
    //   The 2 bytes of zfifo ext pkt hdr are in params[6] & params[7] 

#define  FMC_CMD_SET_TX_VBUS_ON           	0x0003  // bit 1 
    // - appends zfifo ext cmd hdr to end of current pkt hdr,  
    //   And automatically removes the ext hdr at next symbol
    //   The 2 bytes of zfifo ext pkt hdr to turn on vbus must be 
    //   in params[6] & params[7] 

#define  FMC_CMD_ZFIFO_MASK               	0x0003  
    // mask to select cmds which need zfifo ext pkt hdr 

  // skip z-fifo not needed as we can now send full z-fifo ext cmd
 // #define  FMC_CMD_SKIP_ZFIFO             0x0004  // bit 2 
    // - skip Z-fifo read ptr (for vdsl synchronization)
    //   number of bits to skip is given as part of zfifo ext pkt 
    //   hdr in params[2] & params[3] 

#define  FMC_CMD_DO_BIT_SWAP              	0x0004  // bit 2 
    // - bit swap: to new frm config, & new bit-table
    //   params[5] has the new bit-table offset, and 
    //   ch's aoc buf has the new framer-config params. 

#define  FMC_CMD_EXT_HDR_MASK             	0x0007  
    // mask to select cmds which need to remove ext pkt hdr at end of symbol

#define  FMC_CMD_SET_CYC_PFX_ON           	0x0008  // bit 3 
    // - sets cyclic_prefix on. All frames from this onwards have cyclic 
    //   extension on. 
    //   ?? Cmd to turn off cyclic extn is Not needed ?? 

#define  FMC_CMD_RESET_FRMNUM             	0x0010  // bit 4 
    // - resets frm num after moving to this state. 
    
#define  FMC_CMD_SET_IFFT_SHIFT       		0x0008  // bit 3 
    // - sets ifft shift value. The shift amount (4-bits) is in bits 0 to 3 
    //   of the first byte of params[] part (params[0]). 

 // wrt afe reg not needed as we can use pio 
#define  FMC_CMD_WRT_AFE_REG          		0x0200  // bit 09 
    // - write afe reg:  write given 32-bit value to given 
    //       32-bit address  = 8 bytes . The 8 bytes for Afe command are 
    //   written to a fixed place for each channel in 256 byte buffer. 
    //   the byte offset in this buffer, divided by 8, is given in params[1]
    //   later, we can optimize to reduce the size of this buffer to hold 
    //   a max of say 8 such afe reg-values - this should be enough since 
    //   we will rarely have to write afe regs through packet header. 

//--------- Showtime-state commands ------------------
 // eoc & aoc msgs now sent by CoreCpu by writing to circ buffer 
#define  FMC_CMD_SEND_EOC_MSG         		0x0001  // bit 0 
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

#define  FMC_CMD_SEND_AOC_MSG         		0x0002  // bit 1 
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

//---------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------
typedef struct { 
	// ---- init mode fields ---- 
  	u8   chNum; 		   	// ch num 
  	u8   chState; 	   		// ch's current state 
  	u16  frmFnAdr;  	   	// Adr of fn which is to be called to do framing 
		   	   				// diff fns used for init modes & for each framing
		   	   				// mode in Showtime. 
							// 4 bytes
  	u16  I_frmPf;           // bytes/symbol sent to mapper
  	u8   I_synchroDuration; // if 0: no synchro in OP-train/medley
			   				// else: synchro duration (in OP-train 
			   				// or medley), num of symbols of each 
			   				// synchro portion (all 0 or all FF) 
  	u8   I_synchroCtr;      // # sym done in current synchro portion 
							// 8 bytes
  	u8   I_Xdata0;          // first data byte read from X-fifo 
  	u8   I_Xdata1;          // 2nd data byte read from X-fifo 
  	u16  I_prsState;    	// pseudo-rand seq gen state 
							// 12 bytes
  	u8   I_XfifoFlag;       // 0: send data from ch's init data buf
			   				// 1: send data from ch's X-fifo-fast
  	u8   I_unused1;         // unused byte 
  	u8   I_unused2;         // unused byte 
  	u8   I_unused3;         // unused byte
  							// 16 bytes 
 	// ---- showtime fields ---- 
   	u16  frmPf;    	   		// fast payload/frame: usually Kf-1 
   	u8   frmNf;    
   	u8   frmRf;    
							// 20 bytes
   	u8	 frmUf; 
   	u8	 fastDummyZ; 
   	u8	 fastDummyRS; 
   	u8	 CowordPerSym; 
							// 24 bytes
   	u8	 fastNextState; 
   	u8	 fastStateDataOfst; 
   	u8	 fastResidueCoword; 
   	u8   fastByte;     		// holds last fast byte sent 
							// 28 bytes
   	u16  frmPi;    			// ilv payload/frame: usually Ki-1 
   	u8	 frmNi;    			// Ni=Ki+Ri 
   	u8   frmRi;  
							// 32 bytes
   	u8	 frmUi; 
   	u8	 itlvDummyZ; 	
   	u8	 itlvDummyRS; 
   	u8	 itlvNextState; 
							// 36 bytes
   	u8	 itlvStateDataOfst; 
   	u8	 itlvResidueCoword; 	
   	u8   frmSymRsCtr;  		// sym# in current RS codeword, reset value = 0 
   	u8   syncByte;     		// holds last sync byte (ilv path's fast byte) sent 
							// 40 bytes
 	u16  fullFrmNum;   		// Frm#, reset value = 0 
							// in initialization, this is a 16-bit frame counter
							// in showtime, msb byte has the super-frame number: 0 to 255
							// and the lsb byte has the frame number: 0 to 68
   	u16  lastStateChgFrmNum;// frm# of last state change, reset value = 0 
							// 40 bytes
} FrmTxStatus_t; 

extern FrmTxStatus_t *frm_status[4];
extern u16 CycExtnSize;

typedef enum {
	IDLE = 0,
	START_OP_TRAINING,  
	START_SHOWTIME, 
	START_OP_SYNCHRO_TRAIN,  
	START_OP_MEDLEY,  
	START_OP_SYNCHRO_MEDLEY,
	START_DISABLE
} FMC_CMD_TYPES;

// extern void framerCpu(int ch_num, FMC_CMD_TYPES cmdtype, u32 *arg);
extern int  XtFmcChgStateWithCmd(u8 chNum, u8 newState, u16 atFrmNum, u16 cmdFlags, 
			u16  CycExtnSize, u8 newBitTabOfst, u16 bytesPerFrm);

void framerCpu(int ch_num, FMC_CMD_TYPES cmdtype, int atFrmNum, int bytesPerFrm);
#endif
