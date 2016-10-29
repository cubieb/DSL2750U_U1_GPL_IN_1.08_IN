/*
//////////////////////////////////////////////////////////////////////////
//      Copyright 2000 Velocity Communication, Inc.
//      This Velocity proprietary information must be maintained in the
//      strictest of confidence and may not be copied, disclosed, or used
//      without the express written consent of an officer of Velocity
//      Communication, Inc.
//
//      $Revision: 1.1 $
//
//      Author: ramesh
//
//      Description: PIO read/write access defines
//

// `define PIO_REG    14:8         // address specifier
// `define PIO_NUM    7:2          // channel specifier

////////////////////////////////////////////////////////////////////////
// following defines will be used for PIO register verification
*/

#define PIO_SPACE  0xe      /* address [31:28] specifier for PIO register space */

/* address [27:15] for individual block slects */
#define TNI_BLOCK  0x0         
#define FRM_BLOCK  0x1         
#define DMT_BLOCK  0x2         
#define VIF_BLOCK  0x3         
#define DMC_BLOCK  0x4         
#define RNI_BLOCK  0x5         
#define DMA_BLOCK  0x6         
#define EMI_BLOCK  0x7         
#define XFI_BLOCK  0x8         
#define YFI_BLOCK  0x9         
#define ZFI_BLOCK  0xa         
#define AFI_BLOCK  0xb         
#define BFI_BLOCK  0xc         
#define CFI_BLOCK  0xd         
#define PIO_BLOCK  0xe         
#define RMC_BLOCK  0xf         
#define DMP_BLOCK  0x10         
#define FET_BLOCK  0x11         
#define CKM_BLOCK  0x12         

/* address [7:2] could be used for indexing a channel specific register */
#define CH_0     0x0      
#define CH_1     0x1      
#define CH_2     0x2      
#define CH_3     0x3      
#define CH_4     0x4      
#define CH_5     0x5      
#define CH_6     0x6      
#define CH_7     0x7      
#define CH_8     0x8      
#define CH_9     0x9      
#define CH_10    0xa      
#define CH_11    0xb      
#define CH_12    0xc      
#define CH_13    0xd      
#define CH_14    0xe      
#define CH_15    0xf      
#define CH_16    0x10     
#define CH_17    0x11     
#define CH_18    0x12     
#define CH_19    0x13     
#define CH_20    0x14     
#define CH_21    0x15     
#define CH_22    0x16     
#define CH_23    0x17     
#define CH_24    0x18     
#define CH_25    0x19     
#define CH_26    0x1a     
#define CH_27    0x1b     
#define CH_28    0x1c     
#define CH_29    0x1d     
#define CH_30    0x1e     
#define CH_31    0x1f     

/*
// bfifo pio registers
//
*/
#define CDMT_D     0x0            /* CDMT */
#define CBFT_A_D   0x1            /* CBFT, bank a */
#define CBFT_B_D   0x2            /* CBFT, bank b */


/* Module Specific Register Defines
/////////////////////////////////////////////////////////////////////////
*/

/* DMT Engine Specific Definitions */


/*
// ptone pio registers
//
*/
#define CTX_TBL_SEL   0x0          /*// Pilot Tone TABLE SELECTION*/
#define PTONE_TBL_SEL 0x1          /*// Pilot Tone TABLE SELECTION*/

/*
// Mapper Pilot Tone Context Register Global Address
//
*/
#define PTONE_32T_GL	0xe0014000   
#define PTONE_64T_GL	0xe0014100   
#define PTONE_128T_GL	0xe0014200   
#define PTONE_256T_GL	0xe0014300   
#define PTONE_512T_GL	0xe0014400   
#define PTONE_1024T_GL	0xe0014500   
#define PTONE_2048T_GL	0xe0014600   


/*
// Mapper Context Table 
*/

#define DMPMUT          0x10 


/*
////////////////////////////////////////////////////////////////////
// FIFO Specific Definitons

// cxft pio registers
//
*/

#define F_CXFT_A 0x0            /* Fast CXFT, bank a */
#define F_CXFT_B 0x1            /* Fast CXFT, bank b */
#define I_CXFT_A 0x2            /* Interleaver CXFT, bank a */
#define I_CXFT_B 0x3            /* Interleaver CXFT, bank b */


/*
// cyft pio registers
//
*/
#define CYFT_A	 0x0 		/* CYFT, bank a */
#define CYFT_B	 0x1 		/* CYFT, bank b */
#define CYFT_C	 0x2 		/* CYFT, bank c */
#define CYFT_D	 0x3 		/* CYFT, bank d */
#define TOFT	 0x4 		/* TOFT */

/*
// czft pio registers
//
*/
#define CZFT_A	 0x0 		/* CZFT, bank a */
#define CZFT_B	 0x1 		/* CZFT, bank b */
#define CZFT_C	 0x2 		/* CZFT, bank c */
#define CZFT_D	 0x3 		/* CZFT, bank d */
#define PFXT_A_D   0x4 		/* cyclic prefix table */
#define PFXT_B_D   0x5            /* cyclic prefix table */
#define PFXT_C_D   0x6            /* cyclic prefix table */


/*
// caft pio registers
//
*/
#define CAFT_A_D   0x0            /* CAFT, bank a */
#define CAFT_B_D   0x1            /* CAFT, bank b */
#define CAFT_C_D   0x2            /* CAFT, bank c */
#define CAFT_D_D   0x3            /* CAFT, bank d */


/*
// ccft pio registers
*/
#define CCFT0_A_D 0x0            /* CCFT0 (phy0-31), bank a */
#define CCFT0_B_D 0x1            /* CCFT0 (phy0-31), bank b */
#define CCFT1_A_D 0x2            /* CCFT1 (phy31-63), bank a */
#define CCFT1_B_D 0x3            /* CCFT1 (phy31-63), bank b */


/*
////////////////////////////////////////////////////////////////////
// PIO Interface MOdule Config Register
*/
#define PIO_MCR           0xe0070000     
#define CKM_CCR           0xe0070004     
#define CKM_PCR           0xe0070008     
#define PIO_ISR           0xe007000c     
#define PIO_RSR           0xe0070010     
#define PIO_BID           0xe0070014     
#define DAC_MCR           0xe0070018     
#define CKM_NCR           0xe007001c     


/*
////////////////////////////////////////////////////////////////////
// FMC/RMC/DMC Module Registers
*/

#define FMC_FCR		0xe0008000      /* FMC Control Register */
#define FMC_FPC		0xe0008004      /* FMC Program Counter */
#define FMC_ICR		0xe0008008      /* FMC Interrupt Control Reg */
#define FMC_PIO_DCACHE	0xe0008040      /* FMC D$ Base  */


/*
////////////////////////////////////////////////////////////////////
// TNI Module Registers
*/


/*channel based registers */
#define CH_FAST		0x0    
#define CH_INLV		0x1    
#define PHY_LOW		0x2    
#define PHY_HIGH	0x3    
#define PHY_LOW_ONOFF	0x4    
#define PHY_HIGH_ONOFF	0x5    


/*
// Global Registers
*/
#define TNI_CSR	 	0xe0000600      /* Cell Size */
#define TNI_LOOR 	0xe0000700      /* Level On Off */  


/*
////////////////////////////////////////////////////////////////////
// EMI Module Registers
*/

#define EMI_MCR		0xe0038000    
#define EMI_RPR		0xe0038004    
#define EMI_RCR		0xe0038008    
#define EMI_PCR		0xe003800c    
#define EMI_SCR		0xe0038010    


/*
////////////////////////////////////////////////////////////////////
// Clock Module Control Register
*/

#define CKM_MCR		0xe0090000   



/*
////////////////////////////////////////////////////////////////////
// PIO Registers, Reset Value
// Added by Jason, 12/01/00
*/

#define TNI_CSR_RESET	0x35000000   
#define VIF_ICCR_RESET	0x380038     
#define VIF_SCCR_RESET	0x1f1f0707   
#define VIF_TSCR_RESET	0x10201020   
#define VIF_RSCR_RESET	0x404        
#define VIF_CSTXL_RESET 0xffff       
#define VIF_CSTXH_RESET 0x20042004   
#define RNI_MCR_RESET	0x350000     
#define EMI_RPR_RESET	0x43         
#define CKM_MCR_RESET	0x20080000   

/*
////////////////////////////////////////////////////////////////////
// PIO Registers, ZFIFO Window Coefficient Table 0 thru 63
// Added by Jason, 12/07/00
*/

#define WCTABLE_0	0xe0050700   
#define WCTABLE_1	0xe0050704   
#define WCTABLE_2	0xe0050708   
#define WCTABLE_3	0xe005070c   
#define WCTABLE_4	0xe0050710   
#define WCTABLE_5	0xe0050714   
#define WCTABLE_6	0xe0050718   
#define WCTABLE_7	0xe005071c   
#define WCTABLE_8	0xe0050720   
#define WCTABLE_9	0xe0050724   
#define WCTABLE_10	0xe0050728   
#define WCTABLE_11	0xe005072c   
#define WCTABLE_12	0xe0050730   
#define WCTABLE_13	0xe0050734   
#define WCTABLE_14	0xe0050738   
#define WCTABLE_15	0xe005073c   
#define WCTABLE_16	0xe0050740   
#define WCTABLE_17	0xe0050744   
#define WCTABLE_18	0xe0050748   
#define WCTABLE_19	0xe005074c   
#define WCTABLE_20	0xe0050750   
#define WCTABLE_21	0xe0050754   
#define WCTABLE_22	0xe0050758   
#define WCTABLE_23	0xe005075c   
#define WCTABLE_24	0xe0050760   
#define WCTABLE_25	0xe0050764   
#define WCTABLE_26	0xe0050768   
#define WCTABLE_27	0xe005076c   
#define WCTABLE_28	0xe0050770   
#define WCTABLE_29	0xe0050774   
#define WCTABLE_30	0xe0050778   
#define WCTABLE_31	0xe005077c   
#define WCTABLE_32	0xe0050780   
#define WCTABLE_33	0xe0050784   
#define WCTABLE_34	0xe0050788   
#define WCTABLE_35	0xe005078c   
#define WCTABLE_36	0xe0050790   
#define WCTABLE_37	0xe0050794   
#define WCTABLE_38	0xe0050798   
#define WCTABLE_39	0xe005079c   
#define WCTABLE_40	0xe00507a0   
#define WCTABLE_41	0xe00507a4   
#define WCTABLE_42	0xe00507a8   
#define WCTABLE_43	0xe00507ac   
#define WCTABLE_44	0xe00507b0   
#define WCTABLE_45	0xe00507b4   
#define WCTABLE_46	0xe00507b8   
#define WCTABLE_47	0xe00507bc   
#define WCTABLE_48	0xe00507c0   
#define WCTABLE_49	0xe00507c4   
#define WCTABLE_50	0xe00507c8   
#define WCTABLE_51	0xe00507cc   
#define WCTABLE_52	0xe00507d0   
#define WCTABLE_53	0xe00507d4   
#define WCTABLE_54	0xe00507d8   
#define WCTABLE_55	0xe00507dc   
#define WCTABLE_56	0xe00507e0   
#define WCTABLE_57	0xe00507e4   
#define WCTABLE_58	0xe00507e8   
#define WCTABLE_59	0xe00507ec   
#define WCTABLE_60	0xe00507f0   
#define WCTABLE_61	0xe00507f4   
#define WCTABLE_62	0xe00507f8   
#define WCTABLE_63	0xe00507fc   


/* Addresses for C Simulations */

#define  C_SEMAPHORE_ADDR  0x200ffc08  
#define  C_SEMAPHORE_DATA  0x200ffc0c  
#define  C_FAIL_ADDR       0x200ffc0c  

