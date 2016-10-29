#ifndef FRMACL_H 
#define FRMACL_H 

/*========================================================================
**            
**      frmacl.h - include file for Framer Accelerator related defines 
**
**	Copyright (c) 2000,2001 Ikanos Communication
**  This Ikanos Communication Inc. proprietary information of must be 
**  maintained in the strictest of confidence and may not be copied, 
**  disclosed,or used without the express prior written consent of an  
**  officer of Ikanos Communication Inc.
**=======================================================================*/


//----------------------------------------------------------------------
//  defs related to FrmAcl commands
//
/* A Frame consists of a sequence of components (fast byte, fast data bytes,
** fast chk bytes etc). This is built by having a list of framer commands, 
** each of which specifies how to create a component of the frame. 
*/

/* The framer is split into 2 components as below: 
** Framer-Accelerator (indicated by FrmAcl): this is a hardware accelarator
**   unit which performs the framing operations (crc,scramble,rs-encode etc).
** Framer-CPU (indicated by FrmCpu): this controls the operation of the 
**   FrmAcl to ensure the required sequence of bytes for each frame is sent. 
**   The FrmCpu controls the FrmAcl by writing a command (type FrmAclCmd_t) 
**   which directs the FrmAcl to send a certain number of bytes, from a 
**   specified source, and the operations to be done on the data being sent. 
*/ 


#if C_CODE

	// is defined in framer.h as below:
typedef struct {  /* Framer-Accelarator command struct */ 
		  /*  see below for details */ 
    u16 src_adr;
    u16 dst_adr;
    u16 tx_ops; 
    u16 nbytes;
} FrmAclCmd_t; 
#endif 

/* 		The FrmAclCmd_t has the foll. fields: 
** chnum:   channel num for which frame is being created. 
** nbytes:  num of bytes to be sent thru the FrmAcl will in the lsb 8 bits,
** 		and the msb 8 bits are reserved for future use.
** src_adr: address of src of data, encoded as below:  
**   bit 15: 0 means adr is in FrmCpu's data mem 
**                bits 14,13: reserved
**		bits 12 to 0: address 
**
**           1 means adr is in some Fifo as below:
**                bits 14,13: reserved
**              bits 12 to 8: Fifo-Id indicates which Fifo to read from 
**                            0 = X-Fifo Fast path
**                            1 = X-Fifo Interleaved path
**                            2 to 31: reserved 
**              bits 7 to 0:  Channel Id in the fifo (8-bit) 
** tx_ops:  operations to be done by FrmAcl, one per bit as in #defs below. 
*/

/* defs to extract parts of src_adr */ 

#define isFrmCpuMemAdr(a)  (((a) & 0x8000) == 0) 
#define FrmCpuMemAdr(a)    ((a) & 0x3fff) 
#define FrmFifoId(a)       (((a) & 0x1f00) >> 8) 
#define FrmFifoChNum(a)    ((a) & 0x00ff)
#define isXfifoAdr(a)	(((a >> 8) == 0x80) || ((a >> 8) == 0x81))
#define XfifoAdr(a)    (((a & 0x100) << 6) + (256*(a & 0xff)))

/* defs for addresses in Fifo: bit 15 must be 1 */ 
#define  XF_FIFO_ADR  	    0x8000 
#define  XI_FIFO_ADR  	    0x8100 
#define  YF_FIFO_ADR  	    0x8200 
#define  YI_FIFO_ADR  	    0x8300 

#define  isYI_FIFO_ADR(x)       (((x) & YI_FIFO_ADR) == YI_FIFO_ADR) 
#define  isXI_FIFO_ADR(x)       (((x) & XI_FIFO_ADR) == XI_FIFO_ADR) 
 
//----------------------------------------------------------------------
//  defs for FrmAcl operations
//
// FrmAcl operations: start with 'FO_' ; define the various ops & 
//   the bit positions in the 'tx_ops' field sent to the FrmAcl. 
// 
// ToDo: need to re-define macros due to change in positions of the 
// various commands. 
// The tx ops are grouped into 2 groups currently, indicated by the msb 2 bits
// Each op below indicates both its bit as well as the group bits.

#define  FOP_GroupMask 	   0xC000  /* check bits 15,14 */ 
#define  FOP_Group0 	   0x0000  /* bits 15,14 = 00 */ 
#define  FOP_Group1 	   0x4000  /* bits 15,14 = 01 */ 
#define  FOP_Group2 	   0x8000  /* bits 15,14 = 02 */ 
#define  FOP_Group3 	   0xC000  /* bits 15,14 = 03 */ 

  //---- Group0 tx ops ---- 
#define  FOP_PktHdr	   0x0001 /* 0=data, 1= pkt hdr: no crc etc. */ 
#define  FOP_WrtIlvDummyBytes 0x0006 /* bits 2,3 indicate dummy bytes to be */ 
				     /* inserted during write to intrlv mem */
#define  FOP_CrcOn	   0x0010 /* if 1, crc is on */ 
#define  FOP_ScrOn	   0x0020 /* if 1, scrambler is on */ 
#define  FOP_RSencOn	   0x0040 /* if 1, rs enc is on */ 

#define  FOP_IdleOff	   0x0080 /* if 1, idle cell insertion is OFF */ 
#define  FOP_AtmHecScrOff  0x0100 /* if 1, atm hec & scrambling is OFF */ 

#define  FOP_RSlastByte    0x0200 /* if 1, then this has last byte of */ 
				  /*  Rs code word: so after the data,*/
				  /*  output the rs check bytes */ 
#define  FOP_SwapInput     0x0400 /* if 1, then this has last byte of */ 
// #define  FOP_SwapOutput    0x0800 /* if 1, then this has last byte of */ 
#define  FOP_BypassAtmSwap 0x0800 /* if 1, bypass the 'byte swapper' at  */ 
				  /*       ouput of ATM-Scr */ 
/* Note: There is a byte swapper at the output of the ATM Scrambler */ 
/*   which is enabled by default to convert the ATM's MSB bit first output */ 
/*   to the ADSL's input LSB bit first */ 


  //---- Group1 tx ops ---- 
#define  FOP_IlvRead	   0x4001 /* if 1, then the nbytes field is sent */
				  /*  to interleaver to indicate num */
				  /*  bytes to read from interleaver mem */ 
				  /* note: this does not transfer data */ 
#define  FOP_RdIlvDummyBytes 0x4006  /* bits 2,3 indicate dummy bytes to be */ 
				     /* deleted during read from intrlv mem */
#define  FOP_DMTsymReady   0x4008 /* if 1, then after this data, enough */ 
				  /*  data has been sent for a dmt */ 
				  /*  symbol, & Mapper can start */ 

#define  FOP_SetChState    0x4100 /* if 1, then Set framerState for given ch */
				  /*  & for given path to values stored in  */ 
				  /*  FrmCpu's data memory */ 
#define  FOP_SaveChState   0x4200 /* if 1, then Save framerState for given ch */
				  /*  & for given path, from FrmAcl & save */ 
				  /*  them in FrmCpu's memory */ 
#define  FOP_GetXfifoAvail 0x4400 /* if 1, then get # of bytes avail in */
				  /* X-fifo & write it to FrmCpu's mem */ 


/* defs for groups of FrmAcl ops, which contain a group of commands 
** which are usually sent together. The rs enc first & last byte ops 
** are sent separately. 
*/ 
	  // for sending pkt hdr 
#define  FOPS_SEND_PKT_HDR    0x0981 /* bypass atm,idle,'byte swapper', */
				     /*  & set pkt hdr bit */ 

	  // for sending raw data during init modes - no modification of data.
#define  FOPS_SEND_RAW_DATA   0x0980 /* bypass atm,idle,'byte swapper', */
				     
	  // for sending data during showtime 
#define  FOPS_DFLT_FAST_DATA  0x0070 /* do  atm,idle, crc,scr,rse */ 

	  // for sending fast byte during showtime 
#define  FOPS_DFLT_FAST_BYTE  0x09f0 /* do  stm, crc,scr,rse */ 

	  // for sending crc value during showtime 
#define  FOPS_WRT_CRC         0x09e0 /* do  stm, No_crc,scr,rse */ 

  // dflt tx ops for fast data without rs encoder (used when Rf=0) 
#define  FOPS_DFLT_FAST_DATA_NO_RSE  0x0030 /* do  atm,idle, crc,scr,rse */ 

  // if Rf > 0, then enable the rs encoder bits as below 
  // note that fast path always has full rs code-word, and so the 
  // last_byte bit is set to output the check bytes. 
#define  FOPS_FAST_RSE_ENABLE        0x0240 /* enable  rse, last_byte */ 


  // the above txOp for fast data is for atm data. To send data from Framer's
  // mem, such as fast byte, set the bits to Disable ATM. 
#define  FOPS_ATM_DISABLE            0x0980 /* set IdleOff, AtmHecScrOff, */
					    /*  BypassAtmSwap bits */ 

  // for sending fast byte, need to clear rs encoder last_byte bit, in the 
  // tx ops for fast path: done by ANDing the value below. 
#define  FOPS_CLR_RSlastByte         0xFDFF /* all except RSlastByte bit */ 

  // for sending crc value, need to clear crc, and rs encoder last_byte bit. 
#define  FOPS_CLR_CRC_RSlastByte     0xFDEF /* all except CRC, RSlastByte */

  // for sending crc value, need to clear crct. 
#define  FOPS_CLR_CRC                0xFDEF /* all except CRC, RSlastByte */

#define  FOPS_IlvDummyBytes          0x0006 /* to get dummy byte bits */ 

#if  0 
  // the above txOp for fast data (without rse) is XOR'd with the one 
  // below, to get the cmd for sending crc value. 
#define  FOPS_XOR_WRT_CRC_VAL        0x0990 /* set IdleOff, AtmHecScrOff, */
					    /*  BypassAtmSwap bits, */ 
					    /*  and clear Crc bit */ 
#endif 

/* defs for intrlv: are same as fast case, since the fast/inrtlv is */
/* now determined by the src/dst address */ 
#define  FOPS_DFLT_ILV_DATA   FOPS_DFLT_FAST_DATA 
#define  FOPS_DFLT_SYNC_BYTE  FOPS_DFLT_FAST_BYTE 

#define  FOPS_INIT_RAW_DATA   0x0980 /* bypass atm, and no other ops */ 
#define  FOPS_GET_XFIFO_DATA  0x0980 /* bypass atm, and no other ops */ 

  // showtime state framer ops
#define  FOPS_DFLT_DATA_RS_CHK 0x0270 /* do  atm,idle, crc,scr,rse, rs_chk */ 
#define  FOPS_DFLT_DATA_RS_NO_CHK 0x0070 /* do  atm,idle, crc,scr,rse */ 
#define  FOPS_DFLT_DATA_NO_RS  0x0030 /* do  atm,idle, crc,scr */ 

#define  FOPS_DFLT_ILV_DATA   FOPS_DFLT_FAST_DATA 
#define  FOPS_DFLT_SYNC_BYTE  FOPS_DFLT_FAST_BYTE 

/* defines to check if FrmAcl op is set */ 
#define  isFOP_PktHdr(x)       (((x) & FOP_PktHdr) == FOP_PktHdr) 
#define  isFOP_RdIlvDummyBytes(x)  (((x) & FOP_RdIlvDummyBytes) == FOP_RdIlvDummyBytes) 
#define  isFOP_CrcOn(x)	       (((x) & FOP_CrcOn) == FOP_CrcOn) 
#define  isFOP_ScrOn(x)	       (((x) & FOP_ScrOn) == FOP_ScrOn) 
#define  isFOP_RSencOn(x)      (((x) & FOP_RSencOn) == FOP_RSencOn) 
#define  isFOP_IdleOff(x)      (((x) & FOP_IdleOff) == FOP_IdleOff) 
#define  isFOP_AtmHecScrOff(x) (((x) & FOP_AtmHecScrOff) == FOP_AtmHecScrOff) 
#define  isFOP_RSlastByte(x)   (((x) & FOP_RSlastByte) == FOP_RSlastByte) 
#define  isFOP_SwapInput(x)    (((x) & FOP_SwapInput) == FOP_SwapInput) 
// #define  isFOP_SwapOutput(x)   (((x) & FOP_BypassAtmSwap) == FOP_BypassAtmSwap)
#define  isFOP_BypassAtmSwap(x) (((x) & FOP_BypassAtmSwap) == FOP_BypassAtmSwap)

#define  isFOP_IlvRead(x)      (((x) & FOP_IlvRead) == FOP_IlvRead) 
#define  isFOP_WrtIlvDummyBytes(x)  (((x) & FOP_WrtIlvDummyBytes) == FOP_WrtIlvDummyBytes) 
#define  isFOP_DMTsymReady(x)  (((x) & FOP_DMTsymReady) == FOP_DMTsymReady) 
#define  isFOP_SetChState(x)   (((x) & FOP_SetChState) == FOP_SetChState) 
#define  isFOP_SaveChState(x)  (((x) & FOP_SaveChState) == FOP_SaveChState) 
#define  isFOP_GetXfifoAvail(x) (((x) & FOP_GetXfifoAvail) ==FOP_GetXfifoAvail) 


/* FrmAcl keeps its state of each path (fast, intrlv) for each channel in the 
**  data memory of the FrmCpu. The offsets below indicates the offsets within 
**  the block of memory used to hold a state. 
*/ 

#if C_CODE

   // framer acl state fields 
typedef struct {  /* Framer-Accelarator command struct */ 
		  /*  see below for details */ 
    u8  atm_scr_state[6];
    u16 hec_state;
    u8  idlebytectr;
    u8  databytectr;
    u8  cellsize;
    u8  crc_state;
    u8  scr_state[3];
    u8  RS_numchkbytes;  // if RS enabled, # of chk bytes - 2, else 0 
    u8  RS_enc_state[16];
} FmcAclState_t; 

#endif 

#define FMC_ST_CELLSIZE_OFST  	 10  // offset of cellSize byte in a 
				     //    save_state block. 

#define FMC_ST_CRC_OFST  	 11  // offset of crc byte 

#define FMC_ST_SCR_OFST  	 12  // offset of first byte (most significant 
			     	     //   byte) of scrambler state 

#define FMC_ST_RSE_NUM_CHK_OFST  15  // offset of 'number of rs enc chk bytes' 
#define FMC_ST_RSE_CHK_OFST      16  // offset of first rs enc chk bytes 

/* reset values for states of framer components */ 
#define FMC_CELLSIZE_RESET_VAL    53     // 0x35  hex 
#define FMC_CRC_RESET_VAL        0x00 
#define FMC_SCR_BYTE0_RESET_VAL  0xff 
#define FMC_SCR_BYTE1_RESET_VAL  0xff 
#define FMC_SCR_BYTE2_RESET_VAL  0xff 


#endif // FRMACL_H 
