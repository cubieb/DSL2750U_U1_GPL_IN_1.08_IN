#ifndef _PIOMEM_H_
#define _PIOMEM_H_

#define	ADDRESS_XFIFO_FAST  		    0xC0000000	/* 16 KB	DMA	*/
#define	ADDRESS_XFIFO_INTERLEAVE 		0xC0800000	/* 16 KB	DMA	*/
#define	ADDRESS_YFIFO_INTERLEAVE		0xC1000000	/* 512 KB	DMA	*/
#define	ADDRESS_YFIFO_TONEORDER 		0xC2000000	/* 4 KB		DMA		*/
#define	ADDRESS_FRAMER_ICACHE			0xCE000000	/* 8 KB		DMA		*/
#define	ADDRESS_FRAMER_DCACHE			0xCE010000	/* 16 KB	PIO and DMA */

//#define	ADDRESS_FRAMER_DCACHE_TXCTRL	0xCE011000	/* 1 KB	PIO and DMA */

#define	ADDRESS_FRAMER_DCACHE_EOC_PIO		ADDRESS_FRAMER_DCACHE_PIO + 0x1400	/* 1 KB	PIO and DMA */
#define	ADDRESS_FRAMER_DCACHE_AOC_PIO		ADDRESS_FRAMER_DCACHE_PIO + 0x1800	/* 1 KB	PIO and DMA */
#define	ADDRESS_FRAMER_DCACHE_AFEREG_PIO   	ADDRESS_FRAMER_DCACHE_PIO + 0x1C00	/* 256b	PIO and DMA */
#define	ADDRESS_FRAMER_DCACHE_IB_PIO	   	ADDRESS_FRAMER_DCACHE_PIO + 0x1D00	/* 192b	PIO and DMA */
#define	ADDRESS_FRAMER_DCACHE_EOCOF_PIO	   	ADDRESS_FRAMER_DCACHE_PIO + 0x1DC0	/* 256b	PIO and DMA */

#define	ADDRESS_FRAMER_DCACHE_EOC		ADDRESS_FRAMER_DCACHE + 0x1400	/* 1 KB	PIO and DMA */
#define	ADDRESS_FRAMER_DCACHE_AOC		ADDRESS_FRAMER_DCACHE + 0x1800	/* 1 KB	PIO and DMA */
#define	ADDRESS_FRAMER_DCACHE_AFEREG   	ADDRESS_FRAMER_DCACHE + 0x1C00	/* 256b	PIO and DMA */
#define	ADDRESS_FRAMER_DCACHE_IB	   	ADDRESS_FRAMER_DCACHE + 0x1D00	/* 192b	PIO and DMA */
#define	ADDRESS_FRAMER_DCACHE_EOCOF	   	ADDRESS_FRAMER_DCACHE + 0x1DC0	/* 256b	PIO and DMA */


#define	ADDRESS_MAPPERTONEORDER			0xCA000000	/* 20 KB	DMA */
#define	ADDRESS_GAINTABLE				0xC9000000	/* 24 KB	DMA		*/
#define	ADDRESS_ZFIFO					0xC3000000	/* 128 KB	DMA	*/
#define	ADDRESS_AFIFO 					0xC4000000	/* 128 KB	DMA	*/
#define	ADDRESS_RXCPU_ICACHE			0xCF000000	/* 4 KB		DMA		*/
#define	ADDRESS_RXCPU_DCACHE			0xCF010000	/* 8 KB		PIO and DMA */
#define	ADDRESS_TEQ_TABLE				0xCC000000	/* DMA			*/
#define	ADDRESS_FEQ_TABLE_FINE_S16		0xCD000000	/* 48 KB	PIO and DMA */
#define ADDRESS_FEQ_TABLE_FINE_S8		(0xCD000000+32*1024)
#define	ADDRESS_DEMAPPERTONEORDER	 	0xCB000000	/* 16 KB	DMA		*/
#define	ADDRESS_FEQ_TABLE_COARSE	   	0xCB00C000	/* ?? KB	DMA		*/
#define	ADDRESS_BFIFOINTERLEAVE 		0xC5000000	/* 128 KB	DMA	*/
#define	ADDRESS_BFIFOFAST				0xD1000000	/* 8 KB		DMA		*/
#define	ADDRESS_BFIFODEMAPPER			0xD0000000	/* 8 KB		DMA		*/
#define	ADDRESS_SMCONTROLSNRMEMORY		0xD2000000	/* 8 KB		DMA		*/
#define	ADDRESS_SMCONEMEMORY			0xD2400000	/* 384 B	DMA		*/
#define	ADDRESS_SMCTHRESHOLDMEMORY		0xD2800000	/* 384 B	DMA		*/
#define	ADDRESS_DEMAPPERCPU_ICACHE		0xC8000000	/* 8 KB		DMA		*/
#define	ADDRESS_DEMAPPERCPU_DCACHE		0xC8010000	/* 16 KB	PIO and DMA */
#define	ADDRESS_CFIFOBANKA				0xC6000000	/* 16 KB	DMA		*/
#define	ADDRESS_CFIFOBANKB				0xC6800000	/* 16 KB	DMA		*/
#define	ADDRESS_HOSTINTERFACEFIFO		0xC7000000	/* 256 B	DMA		*/
#define XTENSA_IRAM						0
#define XTENSA_DRAM						0xA0000000


/*1.2	Table of PIO Registers
All PIO registers listed in the table below are 32 bits wide. The Xtensa Core 
accesses the PIO registers using the load/store instructions. 
These registers are also accessible from the Host CPU Interface of the BME. 
PIO Register	Mnemonic	Address	Type	Reset	Description/Comment
*/

//Transmit Network Interface

#define	TNICH_FAST				0xE0000000	/* CH		Port number for each fast channel	*/
#define	TNICH_INLV				0xE0000100	/* CH		Port number for each interleaved channel	*/
#define	TNIPHY_LOW				0xE0000200	/* CH		Channel number for each lower 32 PHYS (port based)*/
#define	TNIPHY_HIGH				0xE0000300	/* CH		Channel number for each high 32 PHYS (port based)*/
#define	TNIPHY_LOW_ONOFF		0xE0000400	/* CH		Port (on/off) control for lower 32 PHYS (port based)*/
#define	TNIPHY_HIGH_ONOFF		0xE0000500	/* CH		Port (on/off) control for higher 32 PHYS (port based)*/
#define	TNICELL_SIZE			0xE0000600	/* GL		ATM cell size configuration*/
#define	TNILEVEL_ONFO			0xE0000700	/* GL		Utopia Level/Mode configuration*/


//XFIFO Interface
#define	XFIFOF_CXFT_A			0xE0040000	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOF_CXFT_B			0xE0040100	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOI_CXFT_A			0xE0040200	/* CH		Maintains attributes for the interleave buffer on a channel basis*/
#define	XFIFOI_CXFT_B			0xE0040300	/* CH		Maintains attributes for the interleave buffer on a channel basis*/


//Framer Micro-Controller
#define	FCPU_FMC_FCR			0xE0008000	/* GL		FMC configuration and timer control.*/
#define	FCPU_FMC_FPC			0xE0008004	/* GL		Program Counter in the W-stage.*/
#define	FCPU_FMC_ICR			0xE0008008	/* GL		Interrupt to the Xtensa core*/



//YFIFO Interface			    	
#define	YFIFO_CYFT_A			0xE0048000	/* CH		YFIFO interleave buffer control attributes on a channel basis*/
#define	YFIFO_CYFT_B			0xE0048100	/* CH		YFIFO interleave buffer control attributes on a channel basis*/
#define	YFIFO_CYFT_C			0xE0048200	/* CH		YFIFO interleave buffer control attributes on a channel basis*/
#define	YFIFO_CYFT_D			0xE0048300	/* CH		YFIFO interleave buffer control attributes on a channel basis*/
#define	YFIFO_TOFT				0xE0048400	/* GL		Tone Order FIFO Control Attributes*/


//Mapper

#define	PTONE_32T				0xE0014000	/* GL		Pilot tone programming for TBD.*/
#define	PTONE_64T				0xE0014100	/* GL		Pilot tone programming for TBD.*/
#define	PTONE_128T				0xE0014200	/* GL		Pilot tone programming for G.Lite.*/
#define	PTONE_256T				0xE0014300	/* GL		Pilot tone programming for ADSL.*/
#define	PTONE_512T				0xE0014400	/* GL		Pilot tone programming for TBD.*/
#define	PTONE_1024T				0xE0014500	/* GL		Pilot tone programming for TBD.*/
#define	PTONE_2048T				0xE0014600	/* GL		Pilot tone programming for VDSL.*/

#define	MAP_MCT					0xE0010000	/* CH		Mapper configuration (Trellis, IFFT Scale Shift, etc.) */


//ZFIFO Interface
#define	ZFIFO_CZFT_A   			0xE0050000	/* CH		Maintains attributes for the FIFO's in ZFIFO on a channel basis*/
#define	ZFIFO_CZFT_B   			0xE0050100	/* CH		Maintains attributes for the FIFO's in ZFIFO on a channel basis*/
#define	ZFIFO_CZFT_C   			0xE0050200	/* CH		Maintains attributes for the FIFO's in ZFIFO on a channel basis*/
#define	ZFIFO_CZFT_D   			0xE0050300	/* CH		Maintains attributes for the FIFO's in ZFIFO on a channel basis*/
#define	PFXT_A					0xE0050400	/* CH		Maintains attributes for the FIFO's in the cyclic prefix buffer on a channel basis*/
#define	PFXT_B					0xE0050500	/* CH		Maintains attributes for the FIFO's in the cyclic prefix buffer on a channel basis*/
#define	PFXT_C					0xE0050600	/* CH		Maintains attributes for the FIFO's in the cyclic prefix buffer on a channel basis*/
#define	WCATBLE					0xE0050700	/* CH		Windowing Table 0..63*/

//V-Bus Interface
#define	VIF_BCR					0xE0018000	/* GL		Bus Control Register V-bus 0 and 1 Enable/Disable Control*/
#define	VIF_ICCR				0xE0018004	/* GL		Interval Counter Control Register	Channel slot (on V-bus) period control */
#define	VIF_RWCR				0xE0018008	/* GL	    AFE Register Write Command Register  AFE register read/write command control*/
#define	VIF_RRCR				0xE001800C	/* GL	    AFE Register Read Data Register      Data read from an AFE register */
#define	VIF_RWDR				0xE0018010	/* GL	    AFE Register Write Data Register     Data to be written to an AFE register*/
#define	VIF_RASR				0xE0018014	/* GL	    AFE Register Access Status Register  AFE register read and VIF command queue status*/

#define	VIF_SCCR				0xE0018018	/* GL	    Slot and Channel Number Control Register             Slot and channel number programming*/
#define	VIF_CLMR				0xE001801C	/* GL	    Close Loop Mode Control Register                     Close/Open loop control for V-bus 0 and V-bus 1*/

#define	VIF_TSCR				0xE0018020	/* GL	    Transmit Size Control in Close Loop Mode Register    Controls the size of the data transmitted (on V-bus 0 and V-bus 1) during close loop mode*/
#define	VIF_RSCR				0xE0018024	/* GL		Receive Size Control in Close Mode Register	 Controls the size of the data received (on V-bus 0 and V-bus 1) during close loop mode*/

#define	VIF_CST					0xE0018400	/* GL		Channel Schedule Table Base Address*/
#define	VIF_CST0L				0xE0018400	/* GL		Channel Schedule Table 0 Low    AFE selection and the channel transmit and receive control (applies to CST0L through CST31L)*/
#define	VIF_CST0H				0xE0018404	/* GL		Channel Schedule Table 0 High   Channel transmit and receive slot control (applies to CST0H through CST31H)*/
#define	VIF_CST1L				0xE0018408	/* GL		Channel Schedule Table 1 Low    */
#define	VIF_CST1H				0xE001840C	/* GL		Channel Schedule Table 1 High   */
#define	VIF_CST2L				0xE0018410	/* GL		Channel Schedule Table 2 Low    */
#define	VIF_CST2H				0xE0018414	/* GL		Channel Schedule Table 2 High   */
#define	VIF_CST3L				0xE0018418	/* GL		Channel Schedule Table 3 Low    */
#define	VIF_CST3H				0xE001841C	/* GL		Channel Schedule Table 3 High   */
#define	VIF_CST4L				0xE0018420	/* GL		Channel Schedule Table 4 Low    */
#define	VIF_CST4H				0xE0018424	/* GL		Channel Schedule Table 4 High   */
#define	VIF_CST5L				0xE0018428	/* GL		Channel Schedule Table 5 Low    */
#define	VIF_CST5H				0xE001842C	/* GL		Channel Schedule Table 5 High   */
#define	VIF_CST6L				0xE0018430	/* GL		Channel Schedule Table 6 Low    */
#define	VIF_CST6H				0xE0018434	/* GL		Channel Schedule Table 6 High   */
#define	VIF_CST7L				0xE0018438	/* GL		Channel Schedule Table 7 Low    */
#define	VIF_CST7H				0xE001843C	/* GL		Channel Schedule Table 7 High   */
#define	VIF_CST8L				0xE0018440	/* GL		Channel Schedule Table 8 Low    */
#define	VIF_CST8H				0xE0018444	/* GL		Channel Schedule Table 8 High   */
#define	VIF_CST9L				0xE0018448	/* GL		Channel Schedule Table 9 Low    */
#define	VIF_CST9H				0xE001844C	/* GL		Channel Schedule Table 9 High   */
#define	VIF_CST10L				0xE0018450	/* GL		Channel Schedule Table 10 Low   */
#define	VIF_CST10H              0xE0018454	/* GL		Channel Schedule Table 10 High  */
#define	VIF_CST11L              0xE0018458	/* GL		Channel Schedule Table1 11 Low  */
#define	VIF_CST11H              0xE001845C	/* GL		Channel Schedule Table 11 High  */
#define	VIF_CST12L              0xE0018460	/* GL		Channel Schedule Table 12 Low   */
#define	VIF_CST12H              0xE0018464	/* GL		Channel Schedule Table 12 High  */
#define	VIF_CST13L              0xE0018468	/* GL		Channel Schedule Table 13 Low   */
#define	VIF_CST13H              0xE001846C	/* GL		Channel Schedule Table 13 High  */
#define	VIF_CST14L              0xE0018470	/* GL		Channel Schedule Table 14 Low   */
#define	VIF_CST14H              0xE0018474	/* GL		Channel Schedule Table 14 High  */
#define	VIF_CST15L              0xE0018478	/* GL		Channel Schedule Table 15 Low   */
#define	VIF_CST15H              0xE001847C	/* GL		Channel Schedule Table 15 High  */
#define	VIF_CST16L              0xE0018480	/* GL		Channel Schedule Table 16  Low  AFE selection and the channel transmit and receive control */
#define	VIF_CST16H              0xE0018484	/* GL		Channel Schedule Table 16 High  Channel transmit and receive slot control*/
#define	VIF_CST17L              0xE0018488	/* GL		Channel Schedule Table 17 Low   */
#define	VIF_CST17H              0xE001848C	/* GL		Channel Schedule Table 17 High  */
#define	VIF_CST18L              0xE0018490	/* GL		Channel Schedule Table 18 Low   */
#define	VIF_CST18H              0xE0018494	/* GL		Channel Schedule Table 18 High  */
#define	VIF_CST19L              0xE0018498	/* GL		Channel Schedule Table 19 Low   */
#define	VIF_CST19H              0xE001849C	/* GL		Channel Schedule Table 19 High  */
#define	VIF_CST20L              0xE00184A0	/* GL		Channel Schedule Table 20 Low   */
#define	VIF_CST20H              0xE00184A4	/* GL		Channel Schedule Table 20 High  */
#define	VIF_CST21L              0xE00184A8	/* GL		Channel Schedule Table 21 Low   */
#define	VIF_CST21H              0xE00184AC	/* GL		Channel Schedule Table 21 High  */
#define	VIF_CST22L              0xE00184B0	/* GL		Channel Schedule Table 22 Low   */
#define	VIF_CST22H              0xE00184B4	/* GL		Channel Schedule Table 22 High  */
#define	VIF_CST23L              0xE00184B8	/* GL		Channel Schedule Table 23 Low   */
#define	VIF_CST23H              0xE00184BC	/* GL		Channel Schedule Table 23 High  */
#define	VIF_CST24L              0xE00184C0	/* GL		Channel Schedule Table 24 Low   */
#define	VIF_CST24H              0xE00184C4	/* GL		Channel Schedule Table 24 High  */
#define	VIF_CST25L              0xE00184C8	/* GL		Channel Schedule Table 25 Low   */
#define	VIF_CST25H              0xE00184CC	/* GL		Channel Schedule Table 25 High  */
#define	VIF_CST26L              0xE00184D0	/* GL		Channel Schedule Table 26 Low   */
#define	VIF_CST26H              0xE00184D4	/* GL		Channel Schedule Table 26 High  */
#define	VIF_CST27L              0xE00184D8	/* GL		Channel Schedule Table 27 Low   */
#define	VIF_CST27H              0xE00184DC	/* GL		Channel Schedule Table 27 High  */
#define	VIF_CST28L              0xE00184E0	/* GL		Channel Schedule Table 28 Low   */
#define	VIF_CST28H              0xE00184E4	/* GL		Channel Schedule Table 28 High  */
#define	VIF_CST29L              0xE00184E8	/* GL		Channel Schedule Table 29 Low   */
#define	VIF_CST29H              0xE00184EC	/* GL		Channel Schedule Table 29 High  */
#define	VIF_CST30L              0xE00184F0	/* GL		Channel Schedule Table 30 Low   */
#define	VIF_CST30H              0xE00184F4	/* GL		Channel Schedule Table 30 High  */
#define	VIF_CST31L              0xE00184F8	/* GL		Channel Schedule Table 31 Low   */
#define	VIF_CST31H              0xE00184FC	/* GL		Channel Schedule Table 31 High  */


//AFIFO Interface
#define	CAFT_A					0xE0058000	/* CH		A FIFO Context Table A  	AFIFO control attributes on a channel basis*/
//#define	CAFT_B					0xE0051100	/* CH		A FIFO Context Table B  	AFIFO control attributes on a channel basis*/
#define	CAFT_C					0xE0058200	/* CH		A FIFO Context Table C  	AFIFO control attributes on a channel basis*/
#define	CAFT_D					0xE0058300	/* CH		A FIFO Context Table D  	AFIFO control attributes on a channel basis*/



//Receive Micro-Controller RxCPU
#define	RMC_RCR				0xE0078000	/* GL		RMC configuration and timer control.*/
#define	RMC_RPC				0xE0078004	/* GL		Program Counter in the W-stage.*/
#define	RMC_ICR				0xE0078008	/* GL		Interrupt to the Xtensa core*/
					
// Frequency domain Eq table. 																					  
#define	FETHI					0xE0078008	/* GL		*/

//Demapper:
#define DMPCTX					0xE0080000	// Demapper Context table
#define DMPPLT					0xE0080800	// Pilot Tone Table
#define MDPMUT					0xE0081000	// Loop gain table
#define DMPSIT					0xE0081800	// ShowInit/InitShow Coeff Table
#define SNRCTX					0xE0082000  // SNR Context table
#define SNRTON					0xE0082800	// SNR Tone Table
#define DMPSBT					0xE0083000	// S bit table
#define FETLOW					0xE0084000	// FEQ Table Least Sig Bytes

					
					
//BFIFO Interface
#define	CDMT					0xE0060000	/* CH		Demapper FIFO Context Table         Demapper FIFO control attributes*/
#define	CBFT_A					0xE0060100	/* CH		Fast BFIFO Context Table A          Fast FIFO control attributes*/
#define	CBFT_B					0xE0060200	/* CH		Fast BFIFO Context Table B          Fast FIFO control attributes*/
#define	CBIT_A					0xE0060300	/* CH		Interleave BFIFO Context Table A    Interleave FIFO control attributes*/
#define	CBIT_B					0xE0060400	/* CH		Interleave BFIFO Context Table B    Interleave FIFO control attributes*/
#define	CBIT_C					0xE0060500	/* CH		Interleave BFIFO Context Table C    Interleave FIFO control attributes*/
#define	CBIT_D					0xE0060600	/* CH		Interleave BFIFO Context Table D    Interleave FIFO control attributes*/


//Deframer Micro-Controller
#define	DMC_DCR					0xE0020000	/* GL		DMC Control Register DMC configuration and timer control.*/
#define	DMC_DPC					0xE0020004	/* GL		DMC Program Counter Program Counter in the W-stage.*/
#define	DMC_ICR					0xE0020008	/* GL		Interrupt to the Xtensa core*/


// CFIFO Interface
#define	CCFT0_A					0xE0068000	/* PHY	  CFFIFO Context table 0 Bank A     CCFT0 table contains control attributes for physical channel 0-31*/
#define	CCFT0_B					0xE0068100	/* PHY	  CFFIFO Context table 0 Bank B     CCFT0 table contains control attributes for physical channel 0-31*/
#define	CCFT1_A					0xE0068200	/* PHY	  CFFIFO Context table 1 Bank A     CCFT1 table contains control attributes for physical channel 32-63*/
#define	CCFT1_B					0xE0068300	/* PHY	  CFFIFO Context table 1 Bank B     CCFT1 table contains control attributes for physical channel 32-63*/
	
// Receive Network Interface
#define	RNI_MCR					0xE0028000	/* GL	   RNI Module Control Register          RNI control attributes*/
#define	PHY_CTRL_A				0xE0028100	/* PHY	   PHY 0 through 31 Control Register    PHY 0 through 31 control attributes*/
#define	PHY_CTRL_B				0xE0028200	/* PHY	   PHY 32 through 63 Control Register   PHY 32 through 63 control attributes*/
	
//DMA Controller

//#define	DMA_CCR					0xE0030000	/* CH		DMA channel control attributes */

#define	DMA_CCR0				0xE0030000	/* GL		DMA channel control attributes */
#define	DMA_CCR1				0xE0030004	/* GL		DMA channel 1 control attributes*/
#define	DMA_CCR2				0xE0030008	/* GL		DMA 2 channel control attributes */
#define	DMA_CCR3				0xE003000C	/* GL		DMA channel 3 control attributes */
#define	DMA_CCR4	            0xE0030010	/* GL		DMA channel 4 control attributes */
#define	DMA_CCR5	            0xE0030014	/* GL		DMA channel 5 control attributes */
#define	DMA_CCR6	            0xE0030018	/* GL		DMA channel 6 control attributes */
#define	DMA_CCR7	            0xE003001C	/* GL		DMA channel 7 control attributes */
#define	DMA_CCR8	            0xE0030020	/* GL		DMA channel 8 control attributes */
#define	DMA_CCR9	            0xE0030024	/* GL		DMA channel 9 control attributes */
#define	DMA_CCR10	            0xE0030028	/* GL		DMA channel 10 control attributes */
#define	DMA_CCR11	            0xE003002C	/* GL		DMA channel 11 control attributes */
#define	DMA_CCR12	            0xE0030030	/* GL		DMA channel 12 control attributes */
#define	DMA_CCR13	            0xE0030034	/* GL		DMA channel 13 control attributes */
#define	DMA_CCR14	            0xE0030038	/* GL		DMA channel 14 control attributes */
#define	DMA_CCR15	            0xE003003C	/* GL		DMA channel 15 control attributes */

//#define	DMA_ECR		            0xE0030040	/* CH		DMA channel 0 event control attributes */
#define	DMA_ECR0	            0xE0030040	/* GL		DMA channel 0 event control attributes */
#define	DMA_ECR1	            0xE0030044	/* GL		DMA channel 1 event control attributes*/
#define	DMA_ECR2	            0xE0030048	/* GL		DMA 2 channel event control attributes */
#define	DMA_ECR3	            0xE003004C	/* GL		DMA channel 3 event control attributes */
#define	DMA_ECR4	            0xE0030050	/* GL		DMA channel 4 event control attributes */
#define	DMA_ECR5	            0xE0030054	/* GL		DMA channel 5 event control attributes */
#define	DMA_ECR6	            0xE0030058	/* GL		DMA channel 6 event control attributes */
#define	DMA_ECR7	            0xE003005C	/* GL		DMA channel 7 event control attributes */
#define	DMA_ECR8	            0xE0030060	/* GL		DMA channel 8 event control attributes */
#define	DMA_ECR9	            0xE0030064	/* GL		DMA channel 9 event control attributes */
#define	DMA_ECR10	            0xE0030068	/* GL		DMA channel 10 event control attributes */
#define	DMA_ECR11	            0xE003006C	/* GL		DMA channel 11 event control attributes */
#define	DMA_ECR12	            0xE0030070	/* GL		DMA channel 12 event control attributes */
#define	DMA_ECR13	            0xE0030074	/* GL		DMA channel 13 event control attributes */
#define	DMA_ECR14	            0xE0030038	/* GL		DMA channel 14 event control attributes */
#define	DMA_ECR15	            0xE003007C	/* GL		DMA channel 15 event control attributes */

//#define	DMA_SAR		            0xE0030080	/* CH		DMA transfer source address for channel 0*/
#define	DMA_SAR0	            0xE0030080	/* GL		DMA transfer source address for channel 0*/
#define	DMA_SAR1	            0xE0030084	/* GL		DMA transfer source address for channel 1*/
#define	DMA_SAR2	            0xE0030088	/* GL		DMA transfer source address for channel 2*/
#define	DMA_SAR3	            0xE003008C	/* GL		DMA transfer source address for channel 3*/
#define	DMA_SAR4	            0xE0030090	/* GL		DMA transfer source address for channel 4*/
#define	DMA_SAR5	            0xE0030094	/* GL		DMA transfer source address for channel 5*/
#define	DMA_SAR6	            0xE0030098	/* GL		DMA transfer source address for channel 6*/
#define	DMA_SAR7	            0xE003009C	/* GL		DMA transfer source address for channel 7*/
#define	DMA_SAR8	            0xE00300A0	/* GL		DMA transfer source address for channel 8*/
#define	DMA_SAR9	            0xE00300A4	/* GL		DMA transfer source address for channel 9*/
#define	DMA_SAR10	            0xE00300A8	/* GL		DMA transfer source address for channel 10*/
#define	DMA_SAR11	            0xE00300AC	/* GL		DMA transfer source address for channel 11*/
#define	DMA_SAR12	            0xE00300B0	/* GL		DMA transfer source address for channel 12*/
#define	DMA_SAR13	            0xE00300B4	/* GL		DMA transfer source address for channel 13*/
#define	DMA_SAR14	            0xE00300B8	/* GL		DMA transfer source address for channel 14*/
#define	DMA_SAR15	            0xE00300BC	/* GL		DMA transfer source address for channel 15*/

//#define	DMA_DAR		            0xE00300C0	/* CH		DMA transfer destination address for channel 0*/
#define	DMA_DAR0	            0xE00300C0	/* GL		DMA transfer destination address for channel 0*/
#define	DMA_DAR1	            0xE00300C4	/* GL		DMA transfer destination address for channel 1*/
#define	DMA_DAR2	            0xE00300C8	/* GL		DMA transfer destination address for channel 2*/
#define	DMA_DAR3	            0xE00300CC	/* GL		DMA transfer destination address for channel 3*/
#define	DMA_DAR4	            0xE00300D0	/* GL		DMA transfer destination address for channel 4*/
#define	DMA_DAR5	            0xE00300D4	/* GL		DMA transfer destination address for channel 5*/
#define	DMA_DAR6	            0xE00300D8	/* GL		DMA transfer destination address for channel 6*/
#define	DMA_DAR7	            0xE00300DC	/* GL		DMA transfer destination address for channel 7*/
#define	DMA_DAR8	            0xE00300E0	/* GL		DMA transfer destination address for channel 8*/
#define	DMA_DAR9	            0xE00300E4	/* GL		DMA transfer destination address for channel 9*/
#define	DMA_DAR10	            0xE00300E8	/* GL		DMA transfer destination address for channel 10*/
#define	DMA_DAR11	            0xE00300EC	/* GL		DMA transfer destination address for channel 11*/
#define	DMA_DAR12	            0xE00300F0	/* GL		DMA transfer destination address for channel 12*/
#define	DMA_DAR13	            0xE00300F4	/* GL		DMA transfer destination address for channel 13*/
#define	DMA_DAR14	            0xE00300F8	/* GL		DMA transfer destination address for channel 14*/
#define	DMA_DAR15	            0xE00300FC	/* GL		DMA transfer destination address for channel 15*/


//External Memory Interface
//#define	EMI_MCR		            0xE0038000	/* GL		EMI Control Register    EMI control attributes*/
//#define	EMI_RPR		            0xE0038004	/* GL		SDRAM Refresh Period Register   32'h43	SDRAM refresh period*/
//#define	EMI_RCR		            0xE0038008	/* GL		SDRAM Refresh Command Register  	SDRAM refresh command trigger */
//#define	EMI_PCR		            0xE003800C	/* GL 		SDRAM Precharge Command RegisterGL		SDRAM precharge command trigger*/
//#define	EMI_SCR		            0xE0038010	/* GL		SDRAM Set Mode Command Register 	SDRAM set mode command trigger*/


//#define FRAMER_CPU_CODE_ADDRESS	0x40100000


#define NUM_ADSL_CHANNELS 		32
#define NUM_VDSL_CHANNELS 		4
#define NUM_DSL_CHANNELS 		NUM_VDSL_CHANNELS 


#define SZ_XFIFO_PER_CHANNEL    (0x4000/NUM_DSL_CHANNELS)
												  
#define MEM_OFFSET						0
#define ADDRESS_FRAMER_DCACHE_PIO		0xE0008040
#define	ADDRESS_RXCPU_DCACHE_PIO		0xE0078040
#define	ADDRESS_DEMAPPERCPU_DCACHE_PIO	0xE0020040
#define	ADDRESS_FEQ_CG_TABLE_PIO		0xE0088000

#define FCPU_BMECTRL_MSGS				(ADDRESS_FRAMER_DCACHE_PIO + 0x1000)
#define FMC_ADR_TX_STATUS_AND_CONFIGS	(ADDRESS_FRAMER_DCACHE_PIO + 0x2000)
#define RXCPU_BMECTRL_MSGS     			(ADDRESS_RXCPU_DCACHE_PIO + 0x1000)
#define DEFCPU_BMECTRL_MSGS				ADDRESS_DEMAPPERCPU_DCACHE_PIO
#define	TX_FRM_CNT_ADDR					(ADDRESS_FRAMER_DCACHE_PIO + 0x100)


#endif






