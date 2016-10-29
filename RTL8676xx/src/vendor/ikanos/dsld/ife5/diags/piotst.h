
#define  PIO_MkAdr(m,r,c)     ( (PIO_SPACE << 28) | (((m) & _13b) << 15)  | (((r) &  _7b) <<  8)  | (((c) &  _6b) <<  2) )

#define REG_TNI_CH_FAST_0 			PIO_MkAdr(TNI_BLOCK, CH_FAST, 0) 
#define REG_TNI_CH_INLV_0 			PIO_MkAdr(TNI_BLOCK, CH_INLV, 0) 
#define REG_TNI_PHY_LOW_0 			PIO_MkAdr(TNI_BLOCK, PHY_LOW, 0) 
#define REG_TNI_PHY_HIGH_0 			PIO_MkAdr(TNI_BLOCK, PHY_HIGH, 0) 
#define REG_TNI_PHY_LOW_ONOFF_0 	PIO_MkAdr(TNI_BLOCK, PHY_LOW_ONOFF, 0) 
#define REG_TNI_PHY_HIGH_ONOFF_0 	PIO_MkAdr(TNI_BLOCK, PHY_HIGH_ONOFF, 0) 


/* Xfifo Interface address  */

#define REG_XFIFO_F_CXFT_A 			PIO_MkAdr(XFI_BLOCK, F_CXFT_A, 0)
#define REG_XFIFO_F_CXFT_B 			PIO_MkAdr(XFI_BLOCK, F_CXFT_B, 0)
#define REG_XFIFO_I_CXFT_A 			PIO_MkAdr(XFI_BLOCK, I_CXFT_A, 0)
#define REG_XFIFO_I_CXFT_B 			PIO_MkAdr(XFI_BLOCK, F_CXFT_B, 0)


/*
// Framer Micro-contoller address 
// All are global registers, defined in pio_defines.h 

// Yfifo Interface address 
*/

#define REG_YFIFO_CYFT_A 			PIO_MkAdr(YFI_BLOCK, CYFT_A, 0)
#define REG_YFIFO_CYFT_B 			PIO_MkAdr(YFI_BLOCK, CYFT_B, 0)
#define REG_YFIFO_CYFT_C 			PIO_MkAdr(YFI_BLOCK, CYFT_C, 0)
#define REG_YFIFO_CYFT_D 			PIO_MkAdr(YFI_BLOCK, CYFT_D, 0)
#define REG_YFIFO_TOFT 				PIO_MkAdr(YFI_BLOCK, TOFT, 0)


/*
// @@@ GL #define REG_YFIFO_TOFT 				PIO_MkAdr(YFI_BLOCK, TOFT, 0)

// Mapper address registers 
*/

/** @@@ All the global registers, defined in pio_defines.h
*/ 


#define REG_MAP_MCT					PIO_MkAdr(DMT_BLOCK, MAP_MCT, 0)

/* Zfifo Interface address  */

#define REG_ZFIFO_CZFT_A 			PIO_MkAdr(ZFI_BLOCK, CZFT_A, 0)
#define REG_ZFIFO_CZFT_C 			PIO_MkAdr(ZFI_BLOCK, CZFT_C, 0)
#define REG_ZFIFO_CZFT_D 			PIO_MkAdr(ZFI_BLOCK, CZFT_D, 0)
#define REG_ZFIFO_PFXT_A 			PIO_MkAdr(ZFI_BLOCK, PFXT_A_D, 0)
#define REG_ZFIFO_PFXT_B 			PIO_MkAdr(ZFI_BLOCK, PFXT_B_D, 0)
#define REG_ZFIFO_PFXT_C 			PIO_MkAdr(ZFI_BLOCK, PFXT_C_D, 0)

/*
// @@@ Windowing coefficient table 0 thru 63 Global 

// V-Bus interface 
// @@@ All the global registers, defined in pio_defines.h

// Afifo Interface address 
*/

#define REG_AFIFO_CAFT_A 			PIO_MkAdr(AFI_BLOCK, CAFT_A_D, 0)
#define REG_AFIFO_CAFT_C 			PIO_MkAdr(AFI_BLOCK, CAFT_C_D, 0)
#define REG_AFIFO_CAFT_D 			PIO_MkAdr(AFI_BLOCK, CAFT_D_D, 0)


/*
// Receive Micro-controller registers 
// @@@ Global registers 

//  Frequency Domain Equalizer 
// @@@ Globals 



// Demapper registers 
*/

#define REG_DMAP_DMPCTX					PIO_MkAdr(DMP_BLOCK, DMPCTX, 0)
#define REG_DMAP_DMPMUT					PIO_MkAdr(DMP_BLOCK, DMPMUT, 0)
#define REG_DMAP_SNRCTX					PIO_MkAdr(DMP_BLOCK, SNRCTX, 0)



/*
// Bfifo Interface address 

*/

#define REG_BFIFO_CDMT	 				PIO_MkAdr(BFI_BLOCK, CDMT, 0)
#define REG_BFIFO_CBFT_A	 			PIO_MkAdr(BFI_BLOCK, CBFT_A, 0)
#define REG_BFIFO_CBFT_B	 			PIO_MkAdr(BFI_BLOCK, CBFT_B, 0)
#define REG_BFIFO_CBIT_A	 			PIO_MkAdr(BFI_BLOCK, CBIT_A, 0)
#define REG_BFIFO_CBIT_B	 			PIO_MkAdr(BFI_BLOCK, CBIT_B, 0)
#define REG_BFIFO_CBIT_C	 			PIO_MkAdr(BFI_BLOCK, CBIT_C, 0)
#define REG_BFIFO_CBIT_D	 			PIO_MkAdr(BFI_BLOCK, CBIT_D, 0)


/*
// Deframer Micro-controller 
// @@@ All are global

// Cfifo Interface address 
*/

#define REG_CFIFO_CCFT0_A	 			PIO_MkAdr(CFI_BLOCK, CCFT0_A_D, 0)
#define REG_CFIFO_CCFT0_B	 			PIO_MkAdr(CFI_BLOCK, CCFT0_B_D, 0)
#define REG_CFIFO_CCFT1_A	 			PIO_MkAdr(CFI_BLOCK, CCFT1_A_D, 0)
#define REG_CFIFO_CCFT1_B	 			PIO_MkAdr(CFI_BLOCK, CCFT0_B_D, 0)


/*
// RNI registers
*/

#define REG_RNI_PHY_CTRL_A				PIO_MkAdr(RNI_BLOCK, PHY_CTRL_A, 0)
#define REG_RNI_PHY_CTRL_B				PIO_MkAdr(RNI_BLOCK, PHY_CTRL_B, 0)


/*
// PIO interface 
// @@@ Globals are defined in pio_defines.h 
*/


