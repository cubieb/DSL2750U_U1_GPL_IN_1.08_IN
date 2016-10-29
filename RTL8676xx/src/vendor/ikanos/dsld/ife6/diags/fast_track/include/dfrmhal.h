#ifdef USE_INI
/*========================================================================
**
** $Header: /cvshome/linux/fusiv_src/user/dsld/ife6/diags/fast_track/include/dfrmhal.h,v 1.1 2008/07/18 11:09:30 ksrao Exp $
**             
**      dfrmhal.h - include file for Dfrmcpu's hal layer defines.  
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
#ifndef DFRMHAL_H 
#define DFRMHAL_H 

#ifndef BME_BUILD
#include "dfrmacl.h"
#endif

/*----------------------------------------------------------------------
   DMC: DeFramer MicroController (aka DfrmCpu) Interface defines 
-----------------------------------------------------------------------*/


//----------------- Structs for DfrmCpu interface ---------------------- 
// ---- BmeDmcCtlMsg_t;

#define  DMC_CTL_MSG_PARAM_SIZE  10  // size in bytes of params field 
				     //   in dmc ctl msg below.

//  The offsets of each field in the BmeDmcCtlMsg_t struct from the start of 
//  the struct are defined for use in asm src, with 'FMC_RX_MSG_' prefix, after
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

     u8  params[DMC_CTL_MSG_PARAM_SIZE]; // parameters - depends on newState & cmdFlags.

} BmeDmcCtlMsg_t;

#endif

  // ---- define offsets of struct members for use in asm language code
#define DMC_CTL_MSG_chNum_OFST       0 
#define DMC_CTL_MSG_newState_OFST    1 
#define DMC_CTL_MSG_fullFrmNum_OFST  2 
#define DMC_CTL_MSG_cmdFlags_OFST    4 
#define DMC_CTL_MSG_params0_OFST     6 
#define DMC_CTL_MSG_params1_OFST     7 
#define DMC_CTL_MSG_params2_OFST     8 
#define DMC_CTL_MSG_params3_OFST     9 
#define DMC_CTL_MSG_params4_OFST    10 
#define DMC_CTL_MSG_params5_OFST    11 
#define DMC_CTL_MSG_params6_OFST    12 
#define DMC_CTL_MSG_params7_OFST    13 
#define DMC_CTL_MSG_params8_OFST    14 
#define DMC_CTL_MSG_params9_OFST    15 

#define DMC_CTL_MSG_SIZE	    16
#define DMC_CTL_MSG_WORD_SIZE	    8

// #define RX_MSG_PER_CHNL		    2
#define DMC_CTL_MSG_PER_CHNL	    2
#define DMC_SHFT_2_CTL_MSG_SIZE	    5	// Right shift 5 = 32 (2*rxCtlMsgSize(16))

//-------------------------------------------------------------------
// states specified by newState field can be: 

// ToDo: Check which states need to be sent from Tensilica, since the states 
//	 are indicated to dmc in pkt hdr sent by rmc 

#define  DMC_SHOWTIME_STATE        2  // Showtime 

#define  DMC_INIT_STATE            1  // Init

#define  DMC_DISABLED_STATE       15  // disabled or not yet configured
// #define  DMC_IDLE_STATE	 15  // ?? Idle ?? not needed  


  // a CtlMsg can indicate that current state is to be preserved.
#define  DMC_CTLMSG_KEEP_CUR_STATE       16  


//-------------------------------------------------------------------
// commands specified by cmdFlags field are: 

// ToDo: Check which cmds need to be sent from Tensilica. 
//

//--------- Any-state commands ------------------
#define  DMC_CMD_INTERUPT_CORE        0x8000  // bit 15 
    // - when this message processing is done by DfrmCpu (and the first 
    //    4 bytes are set to 0xff), send an interrupt to core cpu. 
    //    This can be used for cases where CoreCpu needs to send 
    //    many messages one after another.

#define  DMC_CMD_RAISE_DMA_EVENT      0x4000  // bit 14 
    // - raise dma events as specified in params[1]
    //   params[1].bit0: raise dma event after DeMapper has finished this symbol
    //   params[1].bit1: raise dma event after init data moved to B-Itlv Fifo
    //   params[1].bit2: raise dma event after SNR is done 

#define  DMC_CMD_SEND_EXT_CMD         0x2000  // bit 13 
    // - send dfrm acl commands - is used for debugging purposes, 
    //   where CoreCpu provides the full cmd to be sent. The cmd can be upto 
    //   32-bytes and is given in a separate mem area which can hold upto 
    //   say 16 commands (define MAX_EXT_CMDS_IN_FMEM as 16). Then the command 
    //   to be used is specified by last byte of params (params[9]), with the 
    //   value ranging from 0 to 15. If this bit is set, then none of the 
    //   other bits can be set except for the interrupt core bit (bit 15). 

//--------- init-state commands ------------------

//--------- Showtime-state commands ------------------

#define  DMC_CMD_DO_BIT_SWAP          0x0004  // bit 2 
    // - bit swap: to new frm config, & new bit-table
    //   params[6] has the new bit-table offset, and 
    //   params[7] has the new framer-config offset. 


//--------- commands in Pkt Hdr (deframer ext cmd field) -----------

// Ext Cmd field in Pkt Header for Deframer 
//   RxCpu sends commands to Deframer by setting bits in the deframer's 
//   ext cmd field of the packet header. Foll commands are supported.

#define  DMC_EXTCMD_SNR_AVG_READY	0x04  
#define  DMC_EXTCMD_UPDATE_CONFIG 	0x08  
#define  DMC_EXTCMD_INIT_DATA_READY	0x10  

#define  DMC_EXTCMD_CH_STATE_MASK	0xE0 
#define  DMC_CH_INIT_STATE         	0x20
#define  DMC_CH_SHOWTIME_STATE		0x40

   // when no dfrm ext cmd part in hdr, the value below is used as 
   // the default: state = showtime & no other bits set 
#define  DMC_DFLT_SHOWTIME_HDR		0x4000 // (DMC_CH_SHOWTIME_STATE << 8)
#define  DMC_DFLT_INIT_HDR              0x3000 //


//---- deframer acl related defines - copied from framer.h 
//     ToDo: use single src for these defines in both mcode & c-model

/* defs for addresses in Fifo: bit 15 must be 1 */
// #define  BF_FIFO_ADR				0x8900 
// #define  BI_FIFO_ADR				0x8a00 

// #define  DFOP_GetPktHdr			0x8001 
// #define  DFOP_SetChState			0x0400 
// #define  DFOP_SaveChState		0x0800 
// #define  DFOP_DeMapperRead		0x4001 
// #define  DFOP_DeFrameDone		0x0200


//--------- DMA Events raised by DMC microcode ---------------
  // these values are defined as the dfrm acl opcode that raises the dma event
#define  DMC_DMA_EVENT_SNR       0x4080  
#define  DMC_DMA_EVENT_INIT_DATA 0x5180

// Deframer Status struct contains : 
//   Config Parameters to be set in DfrmCpu's memory for each channel. 
//       Some are determined at config time, some during initialization. 
//   Status Variables, updated by Dmc microcode, to keep track of dmc status
// 	in handshake/initialization. 
//
//  The offsets of each field in the DmcStatus struct from the start of the 
//  struct are defined for use in asm src, with 'DMC_' prefix. 
//

// ToDo: Move Status struct fields such that u16 fields start on even address, 
//	 Delete fields not used, and assign correct offset values. 


#if (C_CODE || defined(BME_BUILD))

typedef struct { 
    // ---- common fields -------------------------------
  u8  	chNum; 
  u8 	chState;   // current state of the channel

  u16 	FrmFnAdr;  // Adr of fn which is to be called to do framing for this 
		   // ch. diff fns used for init modes & for each framing
		   // mode in Showtime. 

    // ---- init mode fields -------------------------------

    // ---- showtime mode fields -------------------------------
      		  // ---- size of diff fields in dsl frame. ---- 
  u16  	frmPf;    // fast payload/frame

  u8	frmNf;    // Nf=Kf+Rf 

  u8	frmRf;    

  u16	frmUf;

  u8	fastDummyZ;	

  u8	fastDummyRS;	

  u8	fastNextState;	

  u8	fastResidueCoword;		

  u16	fastStateDataOfst;	

  u8    fastByte1;  // holds last fast byte received 
  u8    fastByte2;  // 

  u16  	frmPi;     // ilv payload/frame

  u8	frmNi;     // Ni=Ki+Ri 

  u8  	frmRi;   

  u16	frmUi;

  u8	itlvDummyZ;	

  u8	itlvDummyRS;	

  u8	itlvNextState;	

  u8	itlvResidueCoword;		

  u16	itlvStateDataOfst;	

  u16 	fastAclCmdOp;   // frmAcl cmd's rxop field for sending fast data 
			// from X-fifo to Y-fifo. 

  u16 	ilvAclCmdOp;    // frmAcl cmd's rxop field for sending ilv data 
			// from X-fifo to Y-fifo: incl dummy byte rx ops.  

  u8    syncByte1;     // holds last sync byte (ilv path's fast byte) received 
  u8    syncByte2;     // 

  u8 	nFrmInDlv;

  u8 	ilvD;

  u8    frmSymRsCtr;    // sym# in current RS codeword, reset value = 0 
  u8    itlvReserved;   // for aligning to word boundary 

    // ---- common fields -------------------------------

  u16 	fullFrmNum;   // Frm#, reset value = 0 
		// in initialization, this is a 16-bit frame counter
		// in showtime, msb byte has the super-frame number: 0 to 255
		//     and the lsb byte has the frame number: 0 to 68

  u16 	IfullFrmNum;   // Frm#, reset value = 0 

  u8  	FDummyZFrmNum;   // Frm#, reset value = 0 
  u8  	FDummyRSFrmNum;  // Frm#, reset value = 0 
  u8  	IDummyZFrmNum;   // Frm#, reset value = 0 
  u8  	IDummyRSFrmNum;  // Frm#, reset value = 0 

  u16 	lastStateChgFrmNum; // frm# of last state change, reset value = 0 

  u16	f_port;  // port num of fast path 
  u16	i_port;  // port num of itlv path 

  // 
  // ToDo: next 3 not really needed in microocode ? 
  //       need to delete these struct members & related OFST defines 
  u8	crc_on;

  u8	scr_on;	

  u8	atm_bypass;		

//---------- Status report registor ----------//
  u8	fastCrcErrorCnt;

  u8	itlvCrcErrorCnt;

} DmcStatus_t; 

#endif 


#define DMC_chNum_OFST			0 
#define DMC_chState_OFST		1 
#define DMC_FrmFnAdr_OFST		2 

//------------------------------------
#define DMC_frmPf_OFST			4

#define DMC_frmNf_OFST			6 
#define DMC_frmRf_OFST			7 
#define DMC_frmUf_OFST			8 

#define DMC_fastDummyZ_OFST		10		
#define DMC_fastDummyRS_OFST		11		
#define DMC_fastNextState_OFST		12
#define DMC_fastResidueCoword_OFST	13		
#define DMC_fastStateDataOfst_OFST	14 
		
#define DMC_fastByte1_OFST       	16 
#define DMC_fastByte2_OFST       	17 

//------------------------------------
#define DMC_frmPi_OFST           	18 // 16 

#define DMC_frmNi_OFST           	20 // 18 
#define DMC_frmRi_OFST           	21 // 19 
#define DMC_frmUi_OFST				22 // 20

#define DMC_itlvDummyZ_OFST			24 // 22		
#define DMC_itlvDummyRS_OFST		25 // 23		
#define DMC_itlvNextState_OFST		26 // 24		
#define DMC_itlvResidueCoword_OFST	27 // 25
#define DMC_itlvStateDataOfst_OFST	28 // 26

#define DMC_fastAclCmdOp_OFST   	30 // 32
 
#define DMC_ilvAclCmdOp_OFST    	32 // 34
		
#define DMC_syncByte1_OFST       	34 // 27 
#define DMC_syncByte2_OFST       	35 // 27 

#define DMC_nFrmInDlv_OFST			36 // 28 
#define DMC_ilvD_OFST				37 // 29 
#define DMC_frmSymRsCtr_OFST    	38 // 30
#define DMC_itlvReserved_OFST		39 // 31

#define DMC_fullFrmNum_OFST     	40 // 36  
#define DMC_FsuperFrmNum_OFST    	40 // 36  
#define DMC_FFrmNum_OFST         	41 // 37
#define DMC_IsuperFrmNum_OFST		42
#define DMC_IFrmNum_OFST			43
#define DMC_FDummyZFrmNum_OFST		44
#define DMC_FDummyRSFrmNum_OFST		45
#define DMC_IDummyZFrmNum_OFST		46
#define DMC_IDummyRSFrmNum_OFST		47
 
#define DMC_lastStateChgFrmNum_OFST	48 // 38 

#define	DMC_f_port_OFST				50 // 40

#define	DMC_i_port_OFST				52 // 41

#define DMC_crc_on_OFST				54 // 42
#define DMC_scr_on_OFST				55 // 43
#define DMC_atm_bypass_OFST			56 // 44

#define DMC_fastCrcErrorCnt_OFST	57 // 45
#define DMC_itlvCrcErrorCnt_OFST	58 // 46

#ifdef BME_BUILD
#define DMC_SHOWTIME_CFG_WORD_SIZE  ((DMC_fullFrmNum_OFST - DMC_frmPf_OFST)/2)
#endif

// ToDo: Update struct for DeframerAcl state to match final hardware states 
//	 so that microcode can correctly set initial values for scr & crc
//	 states, & reset to 0 the diff error counters. 

// when pkt hdr is rec'd, this bit in synrg is set
#define  DMC_SYNRG_HDR_RECD_BIT  0x2 

// when DeFrmSignalDone0 is set, after the cmd is done, bit 4 in synrg is set
#define  DMC_SYNRG_DFRM_DONE0_BIT 0x10


//----------------------------------------------------------------------
//  defs for accessing DeFramer Cpu's data memory 
//----------------------------------------------------------------------

// Accessing DeFramer Cpu's data memory is through the following macros, 
// so that the code can be easily ported among the foll. 3 cases: 
// 1. CoreCpu Firmware:
//	Use PIO address of Fmem & use address as offsets from that. 
//	Enabled by defining CORE_CPU_FIRMWARE before including this file. 
// 2. C-Model (DFrmCpu microcode model): 
//	Model Framer Cpu's data mem as array. 
//	Define access functions which read/write to array 
//	Enabled by defining C_MODEL_DMC_MCODE before including this file. 
// 3. DeFramerCpu Firmware:
//	Use given address as final address & access using asm instructions. 
//	

#define DMC_CPU_MEM_SIZE  16384 /* 16KB */ 


#if  C_MODEL_DMC_MCODE 

extern  u8  DMCmem[DMC_CPU_MEM_SIZE]; 

#define DMC_MEM_START      ((u8 *)DMCmem) // start of array which models fmem

#define isDmcValidAdr(adr) ((adr) < DMC_CPU_MEM_SIZE)


#elif  CORE_CPU_FIRMWARE 

#define DMC_MEM_START      ((u8 *)0x12340000) // where fmem is mapped in PIO space 

#define isDmcValidAdr(adr) ( (((adr)-DMC_MEM_START) >= 0) && \
                              (((adr)-DMC_MEM_START) < DMC_CPU_MEM_SIZE) )

#endif 

#define isWordAdr(adr)      (((adr) & 0x1) == 0)

#if  C_MODEL_DMC_MCODE 

   // use fns to rd/wrt to array & then print PIO etc to test out file
extern DmcRd_u8(u16 adr, u8 *val_p);
extern DmcRd_u16(u16 adr, u16 *val_p);
extern DmcRd_u32(u16 adr, u32 *val_p);
extern DmcWrt_u8(u16 adr, u8 val);
extern DmcWrt_u16(u16 adr, u16 val);
extern DmcWrt_u32(u16 adr, u32 val);

#endif 

#if 0  // if needed for speed, use macros: 

#define DMCFmemRdByte( u16 adr)  (*(u8  *)(DMC_MEM_START+(adr)))  
#define DMCFmemRdWord( u16 adr)  (*(u16 *)(DMC_MEM_START+(adr)))  

#define DMCFmemWrtByte( u16 adr, u8  val)  (*(u8  *)(DMC_MEM_START+(adr)) = (val)) 
#define DMCFmemWrtWord( u16 adr, u16 val)  (*(u16 *)(DMC_MEM_START+(adr)) = (val)) 

#endif 


//----------------------------------------------------------------------
//  defs for addresses & offsets in DfrmCpu's data memory. 
//----------------------------------------------------------------------

/*-------------------------------------------------------------------------
DfrmCpu memory allocation: A fixed memory allocation is used to simplify 
 coding.

* 0 to 4K-1: FrmAcl states, current pkt hdrs 
   @0, 512 bytes: scratch 
   // @512, 512 bytes: Current state of the channel (counters etc): 16 bytes/ch
   //      includes FullFrmNum of last state change, & current FullFrmNum. 
   @512, 512 bytes: New RxConfig settings sent from CoreCpu. 
   @1K, 2KB: FrmAcl states area: 2 states * 32 bytes/state * 32 channels 
   @3K, 1KB: Current Pkt Hdr area: 1 ext_hdr * 32 bytes/ext_hdr * 32 channels 
**-----------------------------------------------------------------------*/

#define  DMC_ADR_GLOBAL_VARS		0x0100   // adr 256 
// #define  DMC_ADR_RX_CONFIGS			0x0200   // adr 512 
//  RX_CONFIGS not used any more 
#define  DMC_ADR_ACL_STATES			0x0400   // adr 1K 
#define  DMC_ADR_PKT_HDRS			0x0C00   // adr 3K 

#define  DMC_RX_CONFIG_SIZE			16 
#define  DMC_RX_CONFIG_SHFT			4		// left shift amount to mult by 16 

// #define  DMC_ADR_RX_STATUS		0x0200	// adr 512 
// #define  DMC_SIZE        16 
// #define  DMC_SHFT_RX_STATUS		4		// left shift amount to mult by 16 

#define  DMC_ACL_STATE_SIZE			32 
#define  DMC_ACL_STATE_PER_CH_SIZE	64 
#define  DMC_ACL_STATE_PER_CH_SHFT	6		// left shift amount to mult by 64 

#define  DMC_PKT_HDR_WORD_SIZE		16 
#define  DMC_PKT_HDR_BYTE_SIZE		32 
#define  DMC_PKT_HDR_CPY_WORD_SIZE  4 
#define  DMC_PKT_HDR_CPY_BYTE_SIZE  8 
#define  DMC_PKT_HDR_SHFT			5		// left shift amount to mult by 32 

#define  DMC_MAX_PKT_HDR_SIZE		16		// currently, max pkt hdr at dmc is 16 

// scratch mem (adr: 0 to 511) use: 
// cmds to frmAcl are written to mem, & then sent to by OUT instr to 
// the accelerator. We need space for only one command (8 bytes). 
#define  DMC_ADR_TMP_ACL_CMD		0x01F8   // adr 504 

/*-------------------------------------------------------------------------
* 4K to 8K-1: CoreCpu to FrmCpu RxCtlMsg, EOC,AOC message, etc. 
   @4K, 1KB: RxCtlMsg area: 2 msgs * 16 bytes/msg * 32 channels 
   @5K, 1KB: EOC msg area:  1 msg * 32 words/msg * 16 channels
   @6K, 1KB: AOC msg area:  1 msg * 32 words/msg * 16 channels
   @6K, 1KB: VOC msg area:  1 msg * 32 words/msg * 16 channels
   @7K, 256: IB bits area:  2 sets of IB bits * (3+1reserv) bytes/set * 32 channels
   @7K+256, 256: AFE RegWrt area:  1 set of adr+val * 4 bytes/set * 32 channels
//   @7K+512, 256: EOC overflow:  8 msg * 32 bytes/msg: common to all channels 
   @7K+512, 256: Total space
			32: EOC write index:  1 write  index * 32 channels
			32: VOC write index:  1 write  index * 32 channels
			32: IB  write index:  1 write  index * 32 channels
			32: EOC read index:   1 read index * 32 channels
			32: VOC read index:   1 read index * 32 channels
			32: IB  read index:	  1 read index * 32 channels
**-----------------------------------------------------------------------*/

#define  DMC_ADR_RX_CTL_MSGS		0x1000	// adr 4K 
#define  DMC_ADR_EOC_MSGS			0x1400	// adr 5K 
#define  DMC_EOC_MSGS_SHFT			0x06	// 64 byte shift 
#define  DMC_ADR_AOC_MSGS			0x1800	// adr 6K 
#define  DMC_AOC_MSGS_SHFT			0x06	// 64 byte shift
#define  DMC_ADR_VOC_MSGS			0x1800	// adr 6K 
#define  DMC_VOC_MSGS_SHFT			0x06	// 64 byte shift
#define  DMC_ADR_IB_BITS			0x1C00	// adr 7K 
#define  DMC_IB_BITS_SHFT			0x03	// 2Pinpon * 4 byte shift. 

#define  DMC_RX_CTL_MSG_SIZE         16 
#define  DMC_RX_CTL_MSG_PER_CH_SIZE  32		// 2 msgs per ch. 
#define  DMC_RX_CTL_MSG_PER_CH_SHFT   5		// left shift amount to mult by 32 

#define  READ_WRITE_INDEX_OFST		96		// 32 EOC + 32 VOC/AOC + 32 IB 
#define  EOC_WRITE_INDEX			0x0E00	// adr 3K+512
#define  VOC_WRITE_INDEX			0x0E20	// adr 3K+512+32
#define  AOC_WRITE_INDEX			0x0E20	// adr 3K+512+32
#define  IB_WRITE_INDEX				0x0E40	// adr 3K+512+32+32

#define  EOC_RING_BUF_WRAP			0x3F
#define  VOC_RING_BUF_WRAP			0x3F
#define  AOC_RING_BUF_WRAP			0x3F
#define  IB_RING_BUF_WRAP			0x07

/*-------------------------------------------------------------------------
* 8K to 11K-1: Channel Rx Configs area
   // @8K, 1KB: Channel RxCfg area: 2 rx_cfg * 32 bytes/rx_cfg * 16 channels (aas)
   @8K, 1KB: Channel RxCfg+RxStatus area: 16+16=32 bytes * 32 channels
   @9K, 512: Global RxCfg area: 40 bytes per std
//   @9K+512, 1.5KB: reserved for data transfer between CoreCpu & FrmCpu 
   @9K+512, 512: deFram2CoreCpuInfo area: 2 * 32 bytes * 16 channels   
   @10K, 1KB: FrmAcl Cmd trace buffer for debugging. 
**-----------------------------------------------------------------------*/

#define  DMC_ADR_RX_STATUS_CONFIG			0x1E00   // adr 7.5K 
// #define  DMC_ADR_GLB_CONFIGS				0x2400   // adr 9K 
//  GLB_CONFIGS not used any more - so reassigned to pilot tone buf 

// #define  DMC_ADR_DBG_MSG_BUF				0x2600   // adr 9K + 512 rxDbgMsgBuf
//  DBG_MSG_BUF not used any more - so reassigned to pilot tone buf 


// For Debug purposes, we keep a small circular buffer of FrmAcl commands 
// sent. Use 1K from 10 for this purpose. 

#define  DMC_ADR_RX_ACL_CMD_BUFS			0x2800  // start from adr 10K 

 
// new
#define  DMC_RX_CONFIG_SIZE					16 
#define  DMC_RX_CONFIG_WORD_SIZE			8 
#define  DMC_RX_CONFIG_SHFT					4  // left shift amount to mult by 16

#define  DMC_RX_STATUS_SIZE					16 
#define  DMC_RX_STATUS_WORD_SIZE			8 
#define  DMC_RX_STATUS_SHFT					4  // left shift amount to mult by 16 
#define  DMC_RX_STATUS_CONFIG_SIZE			64 // aas
#define  DMC_RX_STATUS_CONFIG_SHFT			6  // left shift amount to mult by 64 aas
#define  RX_WRITE_PTR						DMC_ADR_GLOBAL_VARS

#define  DMC_RX_ACL_CMD_BUF_SIZE		256 // size of each buf 
#define  DMC_RX_ACL_CMD_BUF_SHFT1		7   // shift amount to mult/div by 128 
#define  DMC_RX_ACL_CMD_BUF_SHFT2		1   // shift amount to mult/div by 2 
#define  DMC_RX_ACL_CMD_BUF_NUM			5   // 4 bufs + 1 overflow buf. 
#define  DMC_RX_ACL_CMD_BUF_NUM_MASK		3   // mask to limit num bufs to 4 (0 to 3).


/*-------------------------------------------------------------------------
* 11K to 15K-1: reserved for data transfer from X-fifo. 
	Can be used, if processing to be done before sending data out

* 15K to 16K-1: 
   @15K, 512 bytes: Scratch mem 
   @15K+512, 512: bytes stack area 

-------------------------------------------------------------------------*/

   // config data for real showtime testing 
#define  DMC_ADR_CFG_DATA_BUFS		0x2E00   // adr 11.5 K 
#define  DMC_CFG_DATA_SHFT		  6 	 // each ch has 64 byte cfg 
#define  DMC_CFG_DATA_WORD_SIZE 	 32 	 // 64 bytes 

    // returns adrs of channel's status struct in dcpu's memory.
#define  DmcStatusAdr( ch_num)   \
     (DMC_ADR_RX_STATUS_CONFIG + ((ch_num) << DMC_RX_STATUS_CONFIG_SHFT))

    // returns adrs of channel's msg data buffer area in dcpu's memory.
#define  DmcCfgDataAdr( ch_num)   \
     (DMC_ADR_CFG_DATA_BUFS + ((ch_num) << DMC_CFG_DATA_SHFT))

    // returns adrs of channel's EOC msg buffers area in dcpu's memory.
#define  DmcEocMsgAdr( ch_num)   \
     (DMC_ADR_EOC_MSGS + ((ch_num) << DMC_EOC_MSGS_SHFT))

    // returns adrs of channel's VOC msg buffers area in dcpu's memory.
#define  DmcVocMsgAdr( ch_num)   \
     (DMC_ADR_VOC_MSGS + ((ch_num) << DMC_VOC_MSGS_SHFT))

    // returns adrs of channel's IB bits data buffers area in dcpu's memory.
#define  DmcIBbitsAdr( ch_num)   \
     (DMC_ADR_IB_BITS + ((ch_num) << DMC_IB_BITS_SHFT))

   // BEGIN:  TEMP SHOWTIME TESTING MEM DEFINES 
   //   use mem from 12.5K to 13.5K-1 (512 bytes/ch) to hold canned dfm acl 
   //       cmds for showtime state 
#define  DMC_ADR_SHOWTIME_ACL_CMDS      0x3200   // adr 12.5K 
#define  DMC_SHOWTIME_ACL_CMD_BUF_SIZE  512      // alloc 512 bytes/ch 
#define  DMC_SHFT1_SHOWTIME_ACL_CMDS      5      // total shift by 5+4=9 
#define  DMC_SHFT2_SHOWTIME_ACL_CMDS      4      
   
#define  DMC_ADR_TST_SENT_DATA 		0x1000   // adr 4K to 5K - overwrites 
						 //    dmc ctl msg (not used ?)

#define  DMC_ADR_TST_RECD_DATA 		0x1400   // adr 5K to 6K - overwrites 
						 //    eoc msg area 

#define  DMC_ADR_TST_ERR_COUNT_ARRAY    0x1800   // adr 6K to 8K - overwrites 
						 //    voc msg, IB bits, and 
						 //    eoc, voc rd/wrt ptrs 

#define  DMC_ADR_TST_2plusERR_ARRAY     0x1802   // 2 + err count array 

#define  DMC_TST_IGNORE_ERR_FRM_NUM 	10       // ignore errors in this
						 // many initial frames 

   // END:  TEMP SHOWTIME TESTING MEM DEFINES 

#define  DMC_ADR_SCRATCH       		0x3C00   // adr 15K 
	  // stack = 508 bytes from 15K+512 to 16K-4
#define  DMC_ADR_STK_END       		0x3E00   // adr 15K+512 
#define  DMC_ADR_STK_BEGIN     		0x3FFC   // 16K - 4 


// globals 

#define  DMC_ADR_PKT_HDR_READ_IN  0x3C00      // size= 32 bytes 
#define  DMC_ADR_2nd_PKT_HDR_READ_IN  0x3C0B  // 2nd pkt hdr read in for 
				// pilot tone bug workaround, to be where the 
				// pilot tone would be in single pkt hdr case,
				// i.e, at offset= 11 bytes, size=16 bytes

   // fields in above pkt hdr are defined below 
#define  DMC_ADR_DATASIZE_PKT_HDR_READ_IN 0x3C00  // ofst= 0 
#define  DMC_ADR_EXTFLAGS_PKT_HDR_READ_IN 0x3C02  // ofst= 2 
#define  DMC_ADR_EXTSIZE_PKT_HDR_READ_IN  0x3C03  // ofst= 3
#define  DMC_ADR_CHNUM_PKT_HDR_READ_IN    0x3C04  // ofst= 4 
#define  DMC_ADR_RXOPS_PKT_HDR_READ_IN    0x3C05  // ofst= 5 
#define  DMC_ADR_EXTCMD_PKT_HDR_READ_IN   0x3C06  // ofst= 6 

#define  DMC_ADR_PTONE_CHNUM      0x3C20  // save pilot tone for this ch

#define  DMC_ADR_PTONE_WRT_PTR    0x3C24  // wrt ptr in ptone circ buffer 
#define  DMC_ADR_DmcHdr           0x3C26  // dmc's part of cur pkt hdr 
#define  DMC_ADR_TotalPktsRecd    0x3C28  // total pkts rec'd 
#define  DMC_ADR_DfrmAclCmdAdr    0x3C2A  // wrt ptr in dfrm acl cmd buf 
#define  DMC_ADR_CurChNum         0x3C2C  // ch being processed 
#define  DMC_ADR_LastIdleCtr      0x3C2E  // idle count in wait for prev pktHdr
#define  DMC_ADR_AclStateAdrSaved 0x3C30  // tmp var to save acl state adr
#define  DMC_ADR_SecPktHdrFlag    0x3C32  // if 1, then getting 2nd pkt hdr
                                          //   (i.e, 1st 16 bytes already got)
#define  DMC_ADR_PtoneFrmNumFlag  0x3c34  // if 1, mcode is saving frm num after each 
				 	  //   ptone value,thus using 6 bytes/symbol 


#define  DMC_ADR_PTONE_BUF_BEGIN  0x2200  // ptone buf = 1.5K bytes 
#define  DMC_ADR_PTONE_BUF_END    0x2800  

#define  DMC_DECIMATED_PILOT_CHNUM  16    // when using decimated input, pilot 
					  // tone sent for the discarded 
					  // symbols on this channel. 

#endif

/*-------------------------------------------------------------------------
Undefined defines that need to be renamed and cataloged in one of the
existing sections

--------------------------------------------------------------------------*/
#ifndef BME_BUILD
#define  FAST_PATH				0x0000
#define  ITLV_PATH				0x0001
#define  VDSL_SUPER_DMC_SIZE	0x000A
#endif

#if 0 
		New (Ver 1) MemMap of DMC - Not Yet Implemented 

 Variable   			16 ch   32 ch  StartAdr	 
---------- 			-----	-----  --------
GlobalVars:	512 		 512	512	0x0000 
PtoneBuf:	256 		 256	256	0x0200 
Unused:		224		 224	224	0x0300 
Per Channel Memory: 
  IBwrtIndex:	1 x 1  =  1 	 16	 32	0x03E0 
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

------------------------------------------------------------
#endif 

#else
/*========================================================================
**
** $Header: /cvshome/linux/fusiv_src/user/dsld/ife6/diags/fast_track/include/dfrmhal.h,v 1.1 2008/07/18 11:09:30 ksrao Exp $
**             
**      dfrmhal.h - include file for Dfrmcpu's hal layer defines.  
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
#ifndef DFRMHAL_H 
#define DFRMHAL_H 

#include "dfrmacl.h" 

/*----------------------------------------------------------------------
   DMC: DeFramer MicroController (aka DfrmCpu) Interface defines 
-----------------------------------------------------------------------*/


//----------------- Structs for DfrmCpu interface ---------------------- 
// ---- BmeDmcCtlMsg_t;

#define  DMC_CTL_MSG_PARAM_SIZE  10  // size in bytes of params field 
				     //   in dmc ctl msg below.

//  The offsets of each field in the BmeDmcCtlMsg_t struct from the start of 
//  the struct are defined for use in asm src, with 'FMC_RX_MSG_' prefix, after
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

     u8  params[DMC_CTL_MSG_PARAM_SIZE]; // parameters - depends on newState & cmdFlags.

} BmeDmcCtlMsg_t;

#endif

  // ---- define offsets of struct members for use in asm language code
#define DMC_CTL_MSG_chNum_OFST       0 
#define DMC_CTL_MSG_newState_OFST    1 
#define DMC_CTL_MSG_fullFrmNum_OFST  2 
#define DMC_CTL_MSG_cmdFlags_OFST    4 
#define DMC_CTL_MSG_params0_OFST     6 
#define DMC_CTL_MSG_params1_OFST     7 
#define DMC_CTL_MSG_params2_OFST     8 
#define DMC_CTL_MSG_params3_OFST     9 
#define DMC_CTL_MSG_params4_OFST    10 
#define DMC_CTL_MSG_params5_OFST    11 
#define DMC_CTL_MSG_params6_OFST    12 
#define DMC_CTL_MSG_params7_OFST    13 
#define DMC_CTL_MSG_params8_OFST    14 
#define DMC_CTL_MSG_params9_OFST    15 

#define DMC_CTL_MSG_SIZE	    16
#define DMC_CTL_MSG_WORD_SIZE	    8

// #define RX_MSG_PER_CHNL		    2
#define DMC_CTL_MSG_PER_CHNL	    2
#define DMC_SHFT_2_CTL_MSG_SIZE	    5	// Right shift 5 = 32 (2*rxCtlMsgSize(16))

//-------------------------------------------------------------------
// states specified by newState field can be: 

// ToDo: Check which states need to be sent from Tensilica, since the states 
//	 are indicated to dmc in pkt hdr sent by rmc 

#define  DMC_SHOWTIME_STATE        2  // Showtime 

#define  DMC_INIT_STATE            1  // Init

#define  DMC_DISABLED_STATE       15  // disabled or not yet configured
// #define  DMC_IDLE_STATE	 15  // ?? Idle ?? not needed  


  // a CtlMsg can indicate that current state is to be preserved.
#define  DMC_CTLMSG_KEEP_CUR_STATE       16  


//-------------------------------------------------------------------
// commands specified by cmdFlags field are: 

// ToDo: Check which cmds need to be sent from Tensilica. 
//

//--------- Any-state commands ------------------
#define  DMC_CMD_INTERUPT_CORE        0x8000  // bit 15 
    // - when this message processing is done by DfrmCpu (and the first 
    //    4 bytes are set to 0xff), send an interrupt to core cpu. 
    //    This can be used for cases where CoreCpu needs to send 
    //    many messages one after another.

#define  DMC_CMD_RAISE_DMA_EVENT      0x4000  // bit 14 
    // - raise dma events as specified in params[1]
    //   params[1].bit0: raise dma event after DeMapper has finished this symbol
    //   params[1].bit1: raise dma event after init data moved to B-Itlv Fifo
    //   params[1].bit2: raise dma event after SNR is done 

#define  DMC_CMD_SEND_EXT_CMD         0x2000  // bit 13 
    // - send dfrm acl commands - is used for debugging purposes, 
    //   where CoreCpu provides the full cmd to be sent. The cmd can be upto 
    //   32-bytes and is given in a separate mem area which can hold upto 
    //   say 16 commands (define MAX_EXT_CMDS_IN_FMEM as 16). Then the command 
    //   to be used is specified by last byte of params (params[9]), with the 
    //   value ranging from 0 to 15. If this bit is set, then none of the 
    //   other bits can be set except for the interrupt core bit (bit 15). 

//--------- init-state commands ------------------

//--------- Showtime-state commands ------------------

#define  DMC_CMD_DO_BIT_SWAP          0x0004  // bit 2 
    // - bit swap: to new frm config, & new bit-table
    //   params[6] has the new bit-table offset, and 
    //   params[7] has the new framer-config offset. 


//--------- commands in Pkt Hdr (deframer ext cmd field) -----------

// Ext Cmd field in Pkt Header for Deframer 
//   RxCpu sends commands to Deframer by setting bits in the deframer's 
//   ext cmd field of the packet header. Foll commands are supported.

#define  DMC_EXTCMD_SNR_AVG_READY	0x04  
#define  DMC_EXTCMD_UPDATE_CONFIG 	0x08  
#define  DMC_EXTCMD_INIT_DATA_READY	0x10  

#define  DMC_EXTCMD_CH_STATE_MASK	0xE0 
#define  DMC_CH_INIT_STATE         	0x20
#define  DMC_CH_SHOWTIME_STATE		0x40

   // when no dfrm ext cmd part in hdr, the value below is used as 
   // the default: state = showtime & no other bits set 
#define  DMC_DFLT_SHOWTIME_HDR		0x4000 // (DMC_CH_SHOWTIME_STATE << 8)

//---- deframer acl related defines - copied from framer.h 
//     ToDo: use single src for these defines in both mcode & c-model

/* defs for addresses in Fifo: bit 15 must be 1 */
// #define  BF_FIFO_ADR				0x8900 
// #define  BI_FIFO_ADR				0x8a00 

// #define  DFOP_GetPktHdr			0x8001 
// #define  DFOP_SetChState			0x0400 
// #define  DFOP_SaveChState		0x0800 
// #define  DFOP_DeMapperRead		0x4001 
// #define  DFOP_DeFrameDone		0x0200


//--------- DMA Events raised by DMC microcode ---------------
  // these values are defined as the dfrm acl opcode that raises the dma event
#define  DMC_DMA_EVENT_SNR       0x4080  
#define  DMC_DMA_EVENT_INIT_DATA 0x5180

// Deframer Status struct contains : 
//   Config Parameters to be set in DfrmCpu's memory for each channel. 
//       Some are determined at config time, some during initialization. 
//   Status Variables, updated by Dmc microcode, to keep track of dmc status
// 	in handshake/initialization. 
//
//  The offsets of each field in the DmcStatus struct from the start of the 
//  struct are defined for use in asm src, with 'DMC_' prefix. 
//

// ToDo: Move Status struct fields such that u16 fields start on even address, 
//	 Delete fields not used, and assign correct offset values. 


#if C_CODE 

typedef struct { 
    // ---- common fields -------------------------------
  u8  	chNum; 
  u8 	chState;   // current state of the channel

  u16 	FrmFnAdr;  // Adr of fn which is to be called to do framing for this 
		   // ch. diff fns used for init modes & for each framing
		   // mode in Showtime. 

    // ---- init mode fields -------------------------------

    // ---- showtime mode fields -------------------------------
      		  // ---- size of diff fields in dsl frame. ---- 
  u16  	frmPf;    // fast payload/frame

  u8	frmNf;    // Nf=Kf+Rf 

  u8	frmRf;    

  u16	frmUf;

  u8	fastDummyZ;	

  u8	fastDummyRS;	

  u8	fastNextState;	

  u8	fastResidueCoword;		

  u16	fastStateDataOfst;	

  u8    fastByte1;  // holds last fast byte received 
  u8    fastByte2;  // 

  u16  	frmPi;     // ilv payload/frame

  u8	frmNi;     // Ni=Ki+Ri 

  u8  	frmRi;   

  u16	frmUi;

  u8	itlvDummyZ;	

  u8	itlvDummyRS;	

  u8	itlvNextState;	

  u8	itlvResidueCoword;		

  u16	itlvStateDataOfst;	

  u16 	fastAclCmdOp;   // frmAcl cmd's rxop field for sending fast data 
			// from X-fifo to Y-fifo. 

  u16 	ilvAclCmdOp;    // frmAcl cmd's rxop field for sending ilv data 
			// from X-fifo to Y-fifo: incl dummy byte rx ops.  

  u8    syncByte1;     // holds last sync byte (ilv path's fast byte) received 
  u8    syncByte2;     // 

  u8 	nFrmInDlv;

  u8 	ilvD;

  u8    frmSymRsCtr;    // sym# in current RS codeword, reset value = 0 
  u8    itlvReserved;   // for aligning to word boundary 

    // ---- common fields -------------------------------

  u16 	fullFrmNum;   // Frm#, reset value = 0 
		// in initialization, this is a 16-bit frame counter
		// in showtime, msb byte has the super-frame number: 0 to 255
		//     and the lsb byte has the frame number: 0 to 68

  u16 	IfullFrmNum;   // Frm#, reset value = 0 

  u8  	FDummyZFrmNum;   // Frm#, reset value = 0 
  u8  	FDummyRSFrmNum;  // Frm#, reset value = 0 
  u8  	IDummyZFrmNum;   // Frm#, reset value = 0 
  u8  	IDummyRSFrmNum;  // Frm#, reset value = 0 

  u16 	lastStateChgFrmNum; // frm# of last state change, reset value = 0 

  u16	f_port;  // port num of fast path 
  u16	i_port;  // port num of itlv path 

  // 
  // ToDo: next 3 not really needed in microocode ? 
  //       need to delete these struct members & related OFST defines 
  u8	crc_on;

  u8	scr_on;	

  u8	atm_bypass;		

//---------- Status report registor ----------//
  u8	fastCrcErrorCnt;

  u8	itlvCrcErrorCnt;

} DmcStatus_t; 

#endif 


#define DMC_chNum_OFST			0 
#define DMC_chState_OFST		1 
#define DMC_FrmFnAdr_OFST		2 

//------------------------------------
#define DMC_frmPf_OFST			4

#define DMC_frmNf_OFST			6 
#define DMC_frmRf_OFST			7 
#define DMC_frmUf_OFST			8 

#define DMC_fastDummyZ_OFST		10		
#define DMC_fastDummyRS_OFST		11		
#define DMC_fastNextState_OFST		12
#define DMC_fastResidueCoword_OFST	13		
#define DMC_fastStateDataOfst_OFST	14 
		
#define DMC_fastByte1_OFST       	16 
#define DMC_fastByte2_OFST       	17 

//------------------------------------
#define DMC_frmPi_OFST           	18 // 16 

#define DMC_frmNi_OFST           	20 // 18 
#define DMC_frmRi_OFST           	21 // 19 
#define DMC_frmUi_OFST			22 // 20

#define DMC_itlvDummyZ_OFST		24 // 22		
#define DMC_itlvDummyRS_OFST		25 // 23		
#define DMC_itlvNextState_OFST		26 // 24		
#define DMC_itlvResidueCoword_OFST	27 // 25
#define DMC_itlvStateDataOfst_OFST	28 // 26

#define DMC_fastAclCmdOp_OFST   	30 // 32
 
#define DMC_ilvAclCmdOp_OFST    	32 // 34
		
#define DMC_syncByte1_OFST       	34 // 27 
#define DMC_syncByte2_OFST       	35 // 27 

#define DMC_nFrmInDlv_OFST		36 // 28 
#define DMC_ilvD_OFST			37 // 29 
#define DMC_frmSymRsCtr_OFST    	38 // 30
#define DMC_itlvReserved_OFST		39 // 31

#define DMC_fullFrmNum_OFST     	40 // 36  
#define DMC_FsuperFrmNum_OFST    		40 // 36  
#define DMC_FFrmNum_OFST         		41 // 37
#define DMC_IsuperFrmNum_OFST		42
#define DMC_IFrmNum_OFST		43
#define DMC_FDummyZFrmNum_OFST		44
#define DMC_FDummyRSFrmNum_OFST		45
#define DMC_IDummyZFrmNum_OFST		46
#define DMC_IDummyRSFrmNum_OFST		47
 
#define DMC_lastStateChgFrmNum_OFST	48 // 38 

#define	DMC_f_port_OFST			50 // 40

#define	DMC_i_port_OFST			52 // 41

#define DMC_crc_on_OFST			54 // 42
#define DMC_scr_on_OFST			55 // 43
#define DMC_atm_bypass_OFST		56 // 44

#define DMC_fastCrcErrorCnt_OFST	57 // 45
#define DMC_itlvCrcErrorCnt_OFST	58 // 46



// ToDo: Update struct for DeframerAcl state to match final hardware states 
//	 so that microcode can correctly set initial values for scr & crc
//	 states, & reset to 0 the diff error counters. 

// when pkt hdr is rec'd, this bit in synrg is set
#define  DMC_SYNRG_HDR_RECD_BIT   0x2 

// when DeFrmSignalDone0 is set, after the cmd is done, bit 4 in synrg is set
#define  DMC_SYNRG_DFRM_DONE0_BIT 0x10 

//----------------------------------------------------------------------
//  defs for accessing DeFramer Cpu's data memory 
//----------------------------------------------------------------------

// Accessing DeFramer Cpu's data memory is through the following macros, 
// so that the code can be easily ported among the foll. 3 cases: 
// 1. CoreCpu Firmware:
//	Use PIO address of Fmem & use address as offsets from that. 
//	Enabled by defining CORE_CPU_FIRMWARE before including this file. 
// 2. C-Model (DFrmCpu microcode model): 
//	Model Framer Cpu's data mem as array. 
//	Define access functions which read/write to array 
//	Enabled by defining C_MODEL_DMC_MCODE before including this file. 
// 3. DeFramerCpu Firmware:
//	Use given address as final address & access using asm instructions. 
//	

#define DMC_CPU_MEM_SIZE  16384 /* 16KB */ 


#if  C_MODEL_DMC_MCODE 

extern  u8  DMCmem[DMC_CPU_MEM_SIZE]; 

#define DMC_MEM_START      ((u8 *)DMCmem) // start of array which models fmem

#define isDmcValidAdr(adr) ((adr) < DMC_CPU_MEM_SIZE)


#elif  CORE_CPU_FIRMWARE 

#define DMC_MEM_START      ((u8 *)0x12340000) // where fmem is mapped in PIO space 

#define isDmcValidAdr(adr) ( (((adr)-DMC_MEM_START) >= 0) && \
                              (((adr)-DMC_MEM_START) < DMC_CPU_MEM_SIZE) )

#endif 

#define isWordAdr(adr)      (((adr) & 0x1) == 0)

#if  C_MODEL_DMC_MCODE 

   // use fns to rd/wrt to array & then print PIO etc to test out file
extern DmcRd_u8(u16 adr, u8 *val_p);
extern DmcRd_u16(u16 adr, u16 *val_p);
extern DmcRd_u32(u16 adr, u32 *val_p);
extern DmcWrt_u8(u16 adr, u8 val);
extern DmcWrt_u16(u16 adr, u16 val);
extern DmcWrt_u32(u16 adr, u32 val);

#endif 

#if 0  // if needed for speed, use macros: 

#define DMCFmemRdByte( u16 adr)  (*(u8  *)(DMC_MEM_START+(adr)))  
#define DMCFmemRdWord( u16 adr)  (*(u16 *)(DMC_MEM_START+(adr)))  

#define DMCFmemWrtByte( u16 adr, u8  val)  (*(u8  *)(DMC_MEM_START+(adr)) = (val)) 
#define DMCFmemWrtWord( u16 adr, u16 val)  (*(u16 *)(DMC_MEM_START+(adr)) = (val)) 

#endif 


//----------------------------------------------------------------------
//  defs for addresses & offsets in DfrmCpu's data memory. 
//----------------------------------------------------------------------

/*-------------------------------------------------------------------------
DfrmCpu memory allocation: A fixed memory allocation is used to simplify 
 coding.

* 0 to 4K-1: FrmAcl states, current pkt hdrs 
   @0, 512 bytes: scratch 
   // @512, 512 bytes: Current state of the channel (counters etc): 16 bytes/ch
   //      includes FullFrmNum of last state change, & current FullFrmNum. 
   @512, 512 bytes: New RxConfig settings sent from CoreCpu. 
   @1K, 2KB: FrmAcl states area: 2 states * 32 bytes/state * 32 channels 
   @3K, 1KB: Current Pkt Hdr area: 1 ext_hdr * 32 bytes/ext_hdr * 32 channels 
**-----------------------------------------------------------------------*/

#define  DMC_ADR_GLOBAL_VARS		0x0100   // adr 256 
#define  DMC_ADR_RX_CONFIGS			0x0200   // adr 512 
#define  DMC_ADR_ACL_STATES			0x0400   // adr 1K 
#define  DMC_ADR_PKT_HDRS			0x0C00   // adr 3K 

#define  DMC_RX_CONFIG_SIZE			16 
#define  DMC_RX_CONFIG_SHFT			4		// left shift amount to mult by 16 

// #define  DMC_ADR_RX_STATUS		0x0200	// adr 512 
// #define  DMC_SIZE        16 
// #define  DMC_SHFT_RX_STATUS		4		// left shift amount to mult by 16 

#define  DMC_ACL_STATE_SIZE			32 
#define  DMC_ACL_STATE_PER_CH_SIZE	64 
#define  DMC_ACL_STATE_PER_CH_SHFT	6		// left shift amount to mult by 64 

#define  DMC_PKT_HDR_WORD_SIZE		16 
#define  DMC_PKT_HDR_BYTE_SIZE		32 
#define  DMC_PKT_HDR_CPY_WORD_SIZE  4 
#define  DMC_PKT_HDR_CPY_BYTE_SIZE  8 
#define  DMC_PKT_HDR_SHFT			5		// left shift amount to mult by 32 

#define  DMC_MAX_PKT_HDR_SIZE		16		// currently, max pkt hdr at dmc is 16 

// scratch mem (adr: 0 to 511) use: 
// cmds to frmAcl are written to mem, & then sent to by OUT instr to 
// the accelerator. We need space for only one command (8 bytes). 
#define  DMC_ADR_TMP_ACL_CMD		0x01F8   // adr 504 

/*-------------------------------------------------------------------------
* 4K to 8K-1: CoreCpu to FrmCpu RxCtlMsg, EOC,AOC message, etc. 
   @4K, 1KB: RxCtlMsg area: 2 msgs * 16 bytes/msg * 32 channels 
   @5K, 1KB: EOC msg area:  1 msg * 32 bytes/msg * 32 channels
   @6K, 1KB: AOC msg area:  1 msg * 32 bytes/msg * 32 channels
   @6K, 1KB: VOC msg area:  1 msg * 32 bytes/msg * 32 channels
   @7K, 256: IB bits area:  2 sets of IB bits * (3+1reserv) bytes/set * 32 channels
   @7K+256, 256: AFE RegWrt area:  1 set of adr+val * 4 bytes/set * 32 channels
//   @7K+512, 256: EOC overflow:  8 msg * 32 bytes/msg: common to all channels 
   @7K+512, 256: Total space
			32: EOC write index:  1 write  index * 32 channels
			32: VOC write index:  1 write  index * 32 channels
			32: IB  write index:  1 write  index * 32 channels
			32: EOC read index:   1 read index * 32 channels
			32: VOC read index:   1 read index * 32 channels
			32: IB  read index:	  1 read index * 32 channels
**-----------------------------------------------------------------------*/

#define  DMC_ADR_RX_CTL_MSGS		0x1000	// adr 4K 
#define  DMC_ADR_EOC_MSGS			0x1400	// adr 5K 
#define  DMC_EOC_MSGS_SHFT			0x05	// 32 byte shift 
#define  DMC_ADR_AOC_MSGS			0x1800	// adr 6K 
#define  DMC_AOC_MSGS_SHFT			0x05	// 32 byte shift
#define  DMC_ADR_VOC_MSGS			0x1800	// adr 6K 
#define  DMC_VOC_MSGS_SHFT			0x05	// 32 byte shift
#define  DMC_ADR_IB_BITS			0x1C00	// adr 7K 
#define  DMC_IB_BITS_SHFT			0x03	// 2Pinpon * 4 byte shift. 

#define  DMC_RX_CTL_MSG_SIZE         16 
#define  DMC_RX_CTL_MSG_PER_CH_SIZE  32		// 2 msgs per ch. 
#define  DMC_RX_CTL_MSG_PER_CH_SHFT   5		// left shift amount to mult by 32 

#define  READ_WRITE_INDEX_OFST		96		// 32 EOC + 32 VOC/AOC + 32 IB 
#define  EOC_WRITE_INDEX			0x1E00	// adr 7K+512
#define  VOC_WRITE_INDEX			0x1E20	// adr 7K+512+32
#define  AOC_WRITE_INDEX			0x1E20	// adr 7K+512+32
#define  IB_WRITE_INDEX				0x1E40	// adr 7K+512+32+32

#define  EOC_RING_BUF_WRAP			0x1F
#define  VOC_RING_BUF_WRAP			0x1F
#define  AOC_RING_BUF_WRAP			0x1F
#define  IB_RING_BUF_WRAP			0x07

/*-------------------------------------------------------------------------
* 8K to 11K-1: Channel Rx Configs area
   // @8K, 1KB: Channel RxCfg area: 2 rx_cfg * 32 bytes/rx_cfg * 16 channels (aas)
   @8K, 1KB: Channel RxCfg+RxStatus area: 16+16=32 bytes * 32 channels
   @9K, 512: Global RxCfg area: 40 bytes per std
//   @9K+512, 1.5KB: reserved for data transfer between CoreCpu & FrmCpu 
   @9K+512, 512: deFram2CoreCpuInfo area: 2 * 32 bytes * 16 channels   
   @10K, 1KB: FrmAcl Cmd trace buffer for debugging. 
**-----------------------------------------------------------------------*/

#define  DMC_ADR_RX_STATUS_CONFIG			0x2000   // adr 8K 
#define  DMC_ADR_GLB_CONFIGS				0x2400   // adr 9K 
#define  DMC_ADR_DBG_MSG_BUF				0x2600   // adr 9K + 512 rxDbgMsgBuf


// For Debug purposes, we keep a small circular buffer of FrmAcl commands 
// sent. Use 1K from 10 for this purpose. 

#define  DMC_ADR_RX_ACL_CMD_BUFS			0x2800  // start from adr 10K 

 
// new
#define  DMC_RX_CONFIG_SIZE					16 
#define  DMC_RX_CONFIG_WORD_SIZE			8 
#define  DMC_RX_CONFIG_SHFT					4  // left shift amount to mult by 16

#define  DMC_RX_STATUS_SIZE					16 
#define  DMC_RX_STATUS_WORD_SIZE			8 
#define  DMC_RX_STATUS_SHFT					4  // left shift amount to mult by 16 
#define  DMC_RX_STATUS_CONFIG_SIZE			64 // aas
#define  DMC_RX_STATUS_CONFIG_SHFT			6  // left shift amount to mult by 64 aas
#define  RX_WRITE_PTR						DMC_ADR_GLOBAL_VARS

#define  DMC_RX_ACL_CMD_BUF_SIZE		256 // size of each buf 
#define  DMC_RX_ACL_CMD_BUF_SHFT1		7   // shift amount to mult/div by 128 
#define  DMC_RX_ACL_CMD_BUF_SHFT2		1   // shift amount to mult/div by 2 
#define  DMC_RX_ACL_CMD_BUF_NUM			5   // 4 bufs + 1 overflow buf. 
#define  DMC_RX_ACL_CMD_BUF_NUM_MASK		3   // mask to limit num bufs to 4 (0 to 3).


/*-------------------------------------------------------------------------
* 11K to 15K-1: reserved for data transfer from X-fifo. 
	Can be used, if processing to be done before sending data out

* 15K to 16K-1: 
   @15K, 512 bytes: Scratch mem 
   @15K+512, 512: bytes stack area 

-------------------------------------------------------------------------*/

   // config data for real showtime testing 
#define  DMC_ADR_CFG_DATA_BUFS		0x2E00   // adr 11.5 K 
#define  DMC_CFG_DATA_SHFT		  6 	 // each ch has 64 byte cfg 
#define  DMC_CFG_DATA_WORD_SIZE 	 32 	 // 64 bytes 

    // returns adrs of channel's status struct in dcpu's memory.
#define  DmcStatusAdr( ch_num)   \
     (DMC_ADR_RX_STATUS_CONFIG + ((ch_num) << DMC_RX_STATUS_CONFIG_SHFT))

    // returns adrs of channel's msg data buffer area in dcpu's memory.
#define  DmcCfgDataAdr( ch_num)   \
     (DMC_ADR_CFG_DATA_BUFS + ((ch_num) << DMC_CFG_DATA_SHFT))

    // returns adrs of channel's EOC msg buffers area in dcpu's memory.
#define  DmcEocMsgAdr( ch_num)   \
     (DMC_ADR_EOC_MSGS + ((ch_num) << DMC_EOC_MSGS_SHFT))

    // returns adrs of channel's VOC msg buffers area in dcpu's memory.
#define  DmcVocMsgAdr( ch_num)   \
     (DMC_ADR_VOC_MSGS + ((ch_num) << DMC_VOC_MSGS_SHFT))

    // returns adrs of channel's IB bits data buffers area in dcpu's memory.
#define  DmcIBbitsAdr( ch_num)   \
     (DMC_ADR_IB_BITS + ((ch_num) << DMC_IB_BITS_SHFT))

   // BEGIN:  TEMP SHOWTIME TESTING MEM DEFINES 
   //   use mem from 12.5K to 13.5K-1 (512 bytes/ch) to hold canned dfm acl 
   //       cmds for showtime state 
#define  DMC_ADR_SHOWTIME_ACL_CMDS      0x3200   // adr 12.5K 
#define  DMC_SHOWTIME_ACL_CMD_BUF_SIZE  512      // alloc 512 bytes/ch 
#define  DMC_SHFT1_SHOWTIME_ACL_CMDS      5      // total shift by 5+4=9 
#define  DMC_SHFT2_SHOWTIME_ACL_CMDS      4      
   
#define  DMC_ADR_TST_SENT_DATA 		0x1000   // adr 4K to 5K - overwrites 
						 //    dmc ctl msg (not used ?)

#define  DMC_ADR_TST_RECD_DATA 		0x1400   // adr 5K to 6K - overwrites 
						 //    eoc msg area 

#define  DMC_ADR_TST_ERR_COUNT_ARRAY    0x1800   // adr 6K to 8K - overwrites 
						 //    voc msg, IB bits, and 
						 //    eoc, voc rd/wrt ptrs 

#define  DMC_ADR_TST_2plusERR_ARRAY     0x1802   // 2 + err count array 

#define  DMC_TST_IGNORE_ERR_FRM_NUM 	10       // ignore errors in this
						 // many initial frames 

   // END:  TEMP SHOWTIME TESTING MEM DEFINES 

#define  DMC_ADR_SCRATCH       0x3C00   // adr 15K 
	  // stack = 508 bytes from 15K+512 to 16K-4
#define  DMC_ADR_STK_END       0x3E00   // adr 15K+512 
#define  DMC_ADR_STK_BEGIN     0x3FFC   // 16K - 4 


// globals 

#define  DMC_ADR_PKT_HDR_READ_IN  0x3C00  // size= 32 bytes 

   // fields in above pkt hdr are defined below 
#define  DMC_ADR_DATASIZE_PKT_HDR_READ_IN 0x3C00  // ofst= 0 
#define  DMC_ADR_EXTFLAGS_PKT_HDR_READ_IN 0x3C02  // ofst= 2 
#define  DMC_ADR_CHNUM_PKT_HDR_READ_IN    0x3C04  // ofst= 4 
#define  DMC_ADR_EXTCMD_PKT_HDR_READ_IN   0x3C06  // ofst= 6 

#define  DMC_ADR_PTONE_CHNUM      0x3C20  // save pilot tone for this ch
#define  DMC_ADR_PTONE_RD_PTR     0x3C22  // rd ptr in ptone circ buffer 
#define  DMC_ADR_PTONE_WRT_PTR    0x3C24  // wrt ptr in ptone circ buffer 
#define  DMC_ADR_DmcHdr           0x3C26  // dmc's part of cur pkt hdr 
#define  DMC_ADR_TotalPktsRecd    0x3C28  // total pkts rec'd 
#define  DMC_ADR_DfrmAclCmdAdr    0x3C2A  // wrt ptr in dfrm acl cmd buf 
#define  DMC_ADR_CurChNum         0x3C2C  // ch being processed 
#define  DMC_ADR_LastIdleCtr      0x3C2E  // idle count in wait for prev pktHdr
#define  DMC_ADR_AclStateAdrSaved 0x3C30  // tmp var to save acl state adr

// #define  DMC_ADR_PTONE_BUF_BEGIN  0x3C40  // ptone buf = ? bytes 
// #define  DMC_ADR_PTONE_BUF_END    0x3C80  
#define  DMC_ADR_PTONE_BUF_BEGIN  0x2400  // ptone buf = 1K bytes 
#define  DMC_ADR_PTONE_BUF_END    0x2800  


#endif

/*-------------------------------------------------------------------------
Undefined defines that need to be renamed and cataloged in one of the
existing sections

--------------------------------------------------------------------------*/
#define  FAST_PATH				0x0000
#define  ITLV_PATH				0x0001
#define  VDSL_SUPER_DMC_SIZE	0x000A

#if 0 
		New (Ver 1) MemMap of DMC - Not Yet Implemented 

 Variable   			16 ch   32 ch  StartAdr	 
---------- 			-----	-----  --------
GlobalVars:	512 		 512	512	0x0000 
Unused:		736		 736	736	0x0200 
Per Channel Memory: 
  IBwrtIndex:	1 x 1  =  1 	 16	 32	0x03E0 
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
Common (to all Channels) Memory: 
AclCmdBuf:	1.25K		1.25K  1.25K	0x3200 
PtoneBuf:	1KB 		1KB	1KB	0x3700 
Unused:		256 		256 	256 	0x3B00 
Scratch(tmp):	512		512	512	0x3C00 
Stack:		512		512	512	0x3E00 

------------------------------------------------------------
#endif 

#endif