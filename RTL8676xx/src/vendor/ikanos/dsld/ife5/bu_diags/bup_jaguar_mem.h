#ifndef _PIOMEM_H_
#define _PIOMEM_H_
//#include "bme.h"

#define	ADDRESS_XFIFO_FAST  		    0xC0000000	/* 16 KB	DMA	*/
#ifndef MUSTANG
	#define	ADDRESS_XFIFO_INTERLEAVE 	0xC0800000	/* 16 KB	DMA	*/
#else
	#define	ADDRESS_XFIFO_INTERLEAVE 	ADDRESS_XFIFO_FAST					// mustang -Abhijit
#endif
#define	ADDRESS_YFIFO_INTERLEAVE		0xC1000000	/* 512 KB	DMA	*/
#define	ADDRESS_YFIFO_TONEORDER 		0xC2000000	/* 4 KB		DMA		*/
#define	ADDRESS_FRAMER_ICACHE			0xCE000000	/* 8 KB		DMA		*/
#define	ADDRESS_FRAMER_PIO_ICACHE		0xE00A8000	/* 8 KB		DMA		*/
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
#define	ADDRESS_DEMAPPERCPU_PIO_ICACHE		0xE00B0000	/* 8 KB		DMA		*/
#define ADDRESS_SMC_TONE_M                      0xD2002000
#define ADDRESS_SMC_THR_M                       0xD2002180

#define	ADDRESS_DEMAPPERCPU_DCACHE		0xC8010000	/* 16 KB	PIO and DMA */

#define	ADDRESS_CFIFOBANKA				0xC6000000	/* 16 KB	DMA		*/
#ifndef MUSTANG
	#define	ADDRESS_CFIFOBANKB			0xC6800000	/* 16 KB	DMA		*/
#else
	#define	ADDRESS_CFIFOBANKB			ADDRESS_CFIFOBANKA	/* 16 KB	DMA		*/
#endif

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

#ifdef JAGUAR
//TNI Interface -Jaguar
#define TNI_SRST        0xE007003c
#define TNI_PHYA        0xE0070040
#define TNI_PHYB        0xE0070044

#else

//TNI Interface -Mustang
#define	TNICH_FAST				0xE0000000	/* CH		Port number for each fast channel	*/
#define	TNICH_INLV				0xE0000100	/* CH		Port number for each interleaved channel	*/
#define	TNIPHY_LOW				0xE0000200	/* CH		Channel number for each lower 32 PHYS (port based)*/
#define	TNIPHY_HIGH				0xE0000300	/* CH		Channel number for each high 32 PHYS (port based)*/
#define	TNIPHY_LOW_ONOFF		0xE0000400	/* CH		Port (on/off) control for lower 32 PHYS (port based)*/
#define	TNIPHY_HIGH_ONOFF		0xE0000500	/* CH		Port (on/off) control for higher 32 PHYS (port based)*/
#define	TNICELL_SIZE			0xE0000600	/* GL		ATM cell size configuration*/
#define	TNILEVEL_ONFO			0xE0000700	/* GL		Utopia Level/Mode configuration*/

#endif

#ifdef JAGUAR
//XFIFO Interface -Jaguar
#define	XFIFOF_CXFT_A			0xE0040000	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOF_CXFT_B			0xE0040100	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOI_CXFT_A			0xE0040200	/* CH		Maintains attributes for the interleave buffer on a channel basis*/
#define	XFIFOI_CXFT_B			0xE0040300	/* CH		Maintains attributes for the interleave buffer on a channel basis*/

#define	XFIFOF_CXFT_C			0xE0040400	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOF_CXFT_D			0xE0040500	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOF_CXFT_E			0xE0040600	/* CH		Maintains attributes for the interleave buffer on a channel basis*/

#define	XFIFOI_CXFT_C			0xE0040700	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOI_CXFT_D			0xE0040800	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOI_CXFT_E			0xE0040900	/* CH		Maintains attributes for the interleave buffer on a channel basis*/




#else
//XFIFO Interface - Mustang
#define	XFIFOF_CXFT_A			0xE0040000	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOF_CXFT_B			0xE0040100	/* CH		Maintains attributes for the fast buffer on a channel basis*/
#define	XFIFOI_CXFT_A			0xE0040200	/* CH		Maintains attributes for the interleave buffer on a channel basis*/
#define	XFIFOI_CXFT_B			0xE0040300	/* CH		Maintains attributes for the interleave buffer on a channel basis*/
#endif

//Framer Micro-Controller
#define	FCPU_FMC_FCR			0xE0008000	/* GL		FMC configuration and timer control.*/
#define	FCPU_FMC_FPC			0xE0008004	/* GL		Program Counter in the W-stage.*/
#define	FCPU_FMC_ICR			0xE0008008	/* GL		Interrupt to the Xtensa core*/

// DV MICROCODE defines

#define	FMC_SEM_ADD		FCPU_FMC_FCR + 0x43E	/* GL		*/
#define	FMC_CMD_CNT_ADD	        FCPU_FMC_FCR + 0x43E	/* GL		*/
#define	FMC_CMD_TYP_ADD	        FCPU_FMC_FCR + 0x42C	/* GL		*/



//YFIFO Interface			    	
#define	YFIFO_CYFT_A			0xE0048000	/* CH		YFIFO interleave buffer control attributes on a channel basis*/
#define	YFIFO_CYFT_B			0xE0048100	/* CH		YFIFO interleave buffer control attributes on a channel basis*/
#define	YFIFO_CYFT_C			0xE0048200	/* CH		YFIFO interleave buffer control attributes on a channel basis*/
#define	YFIFO_CYFT_D			0xE0048300	/* CH		YFIFO interleave buffer control attributes on a channel basis*/
#define	YFIFO_CYFT_E			0xE0048700	/* CH		YFIFO interleave buffer control attributes on a channel basis*/
#define	YFIFO_TOFT				0xE0048400	/* GL		Tone Order FIFO Control Attributes*/

#define	YFIFO_CYFF_A			0xE0048500	/* CH		YFIFO fast buffer control attributes on a channel basis*/
#define	YFIFO_CYFF_B			0xE0048600	/* CH		YFIFO fast buffer control attributes on a channel basis*/


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
#define	ZFIFO_CZFT_F   			0xE0050B00	/* CH		Maintains attributes for the FIFO's in ZFIFO on a channel basis*/
#define	PFXT_A					0xE0050400	/* CH		Maintains attributes for the FIFO's in the cyclic prefix buffer on a channel basis*/
#define	PFXT_B					0xE0050500	/* CH		Maintains attributes for the FIFO's in the cyclic prefix buffer on a channel basis*/
#define	PFXT_C					0xE0050600	/* CH		Maintains attributes for the FIFO's in the cyclic prefix buffer on a channel basis*/
#define	WCTABLE					0xE0050700	/* CH		Windowing Table 0..63*/

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



// INTERPOLATOR/DECIMATOR DEFINES/ DDR DEFINES

// addresses for dfu_block
#define INTP_EN                 0xE0050A00
#define DIU_CTRL                0xe00c0000
#define DIU_COEFF_00    0xe00c0100
#define DIU_COEFF_01    0xe00c0104
#define DIU_COEFF_02    0xe00c0108
#define DIU_COEFF_03    0xe00c010c
#define DIU_COEFF_04    0xe00c0110
#define DIU_COEFF_05    0xe00c0114
#define DIU_COEFF_06    0xe00c0118
#define DIU_COEFF_07    0xe00c011c
#define DIU_COEFF_08    0xe00c0120
#define DIU_COEFF_09    0xe00c0124
#define DIU_COEFF_10    0xe00c0128
#define DIU_COEFF_11    0xe00c012c
#define DIU_COEFF_12    0xe00c0130
#define DIU_COEFF_13    0xe00c0134
#define DIU_COEFF_14    0xe00c0138
#define DIU_COEFF_15    0xe00c013c
#define DIU_PWR                 0xe00c0200
#define DIU_THR                 0xe00c0204
#define DIU_PAR_CNT     0xe00c0208

#define OP_GAIN                 0xe00c1800
#define NS_CTRL                 0xe00c1000
#define NS_PWR                  0xe00c1200
#define NS_THR                  0xe00c1204
#define NS_PAR_CNT              0xe00c1208

#define DDU_CTRL                  0xE00C2000
#define DDU_COEFF_00            0xE00C2100
#define DDU_COEFF_01            0xE00C2104
#define DDU_COEFF_02            0xE00C2108
#define DDU_COEFF_03            0xE00C210C
#define DDU_COEFF_04            0xE00C2110
#define DDU_COEFF_05            0xE00C2114
#define DDU_COEFF_06            0xE00C2118
#define DDU_COEFF_07            0xE00C211C
#define DDU_COEFF_08            0xE00C2120
#define DDU_COEFF_09            0xE00C2124
#define DDU_COEFF_10            0xE00C2128
#define DDU_COEFF_11            0xE00C212C
#define DDU_COEFF_12            0xE00C2130
#define DDU_COEFF_13            0xE00C2134
#define DDU_COEFF_14            0xE00C2138
#define DDU_COEFF_15            0xE00C213C
#define DDU_COEFF_16            0xE00C2140
#define DDU_COEFF_17            0xE00C2144
#define DDU_COEFF_18            0xE00C2148
#define DDU_COEFF_19            0xE00C214C
#define DDU_COEFF_20            0xE00C2150
#define DDU_COEFF_21            0xE00C2154
#define DDU_COEFF_22            0xE00C2158
#define DDU_COEFF_23            0xE00C215C
#define DDU_COEFF_24            0xE00C2160
#define DDU_COEFF_25            0xE00C2164
#define DDU_COEFF_26            0xE00C2168
#define DDU_COEFF_27            0xE00C216C
#define DDU_COEFF_28            0xE00C2170
#define DDU_COEFF_29            0xE00C2174
#define DDU_COEFF_30            0xE00C2178
#define DDU_COEFF_31            0xE00C217C
#define DDU_COEFF_32            0xE00C2180
#define DDU_COEFF_33            0xE00C2184
#define DDU_COEFF_34            0xE00C2188
#define DDU_COEFF_35            0xE00C218C
#define DDU_COEFF_36            0xE00C2190
#define DDU_COEFF_37            0xE00C2194
#define DDU_COEFF_38            0xE00C2198
#define DDU_COEFF_39            0xE00C219C
#define DDU_COEFF_40            0xE00C21A0
#define DDU_COEFF_41            0xE00C21A4
#define DDU_COEFF_42            0xE00C21A8
#define DDU_COEFF_43            0xE00C21AC
#define DDU_COEFF_44            0xE00C21B0
#define DDU_COEFF_45            0xE00C21B4
#define DDU_COEFF_46            0xE00C21B8
#define DDU_COEFF_47            0xE00C21BC
#define DDU_PWR                 0xE00C2200
#define DDU_THR                 0xE00C2204
#define DDU_PAR_CNT             0xE00C2208

#define DRF_CTRL                0xE00C3000
#define DRF_COEFF_C11   0xE00C3100
#define DRF_COEFF_C12   0xE00C3104
#define DRF_PWR                 0xE00C3200
#define DRF_THR                 0xE00C3204
#define DRF_PAR_CNT             0xE00C3208

// AFE_IFE SERIAL INTERFACE DEFINE
#define AFE_RDWR_CMD    0xE00C4000
#define AFE_READ_DATA   0xE00C4004
#define AFE_WRITE_DATA  0xE00C4008
#define AFE_RDWR_STAT   0xE00C400C

#define DDR_TEST_CTRL   0xe00c5000
#define DDR_LBIST_DATA0 0xe00c5004
#define DDR_LBIST_DATA1 0xe00c5008
#define DDR_LBIST_CMP   0xe00c500c
#define DDR_LBIST_STAT  0xe00c5010

// IFE-CPE4.0 Address
#define VERSION_NUMBER 0xA10
#define VERSION_ADDR        0x0B
#define GAINCTRL_ADDR       0x00
#define DTEST_ADDR          0x14
#define AFE_CH0_ADDR        0x05

// EIU Logic BIST defines

#define LBS_FRAME_COUNT         200 * 20 // * 2 // 10           // q can be upto 2^31 - 1; FFFF_FFFF = unlmtd
#define LOGIC_BIST_TX_EN        1              //  # Enabling the tx logic bist
#define LOGIC_BIST_RX_EN        1              //  # Enabling the rx logic bist
#define LBS_FRAME_LENGTH        1              //  # 0-64,1-64,2-1518,3-random
#define LBS_PATTERN_TYPE        0              //  # 0-Inc,1-Dec,2 pattern,3-random
#define LBS_IFG_LENGTH          2              //  # multiply by 'h80
#define LOGIC_BIST_DATAPAT0     0x0000aa55   //    # data pattern 0
#define LOGIC_BIST_DATAPAT1     0x0000aa55   //    # data pattern 1
#define LOGIC_BIST_DATAPAT2     0x0000aa55   //    # data pattern 2
#define LOGIC_BIST_DATAPAT3     0x0000aa55   //    # data pattern 3
#define LOGIC_BIST_DATAPAT4     0x0000aa55   //    # data pattern 4
#define LOGIC_BIST_DATAPAT5     0x0000aa55    //   # data pattern 5

#define LBSCTRL0_reg            0xe0098c20
#define LBSCTRL1_reg            0xe0098c24
#define LBSFCNT0_reg            0xe0098c28
#define LBSFCNT1_reg            0xe0098c2c
#define LBSIFG_reg              0xe0098c34
#define LBSPAT0_reg             0xe0098c38
#define LBSPAT1_reg             0xe0098c3c
#define LBSPAT2_reg             0xe0098c40
#define LBSPAT3_reg             0xe0098c44
#define LBSPAT4_reg             0xe0098c48
#define LBSPAT5_reg             0xe0098c4c
#define LBSSTS_reg              0xe0098c30

//AFIFO Interface
#define	CAFT_A	      0xE0058000        /* CH	A FIFO Context Table A  	AFIFO control attributes on a channel basis*/
#define	CAFT_C 	      0xE0058200	/* CHA FIFO Context Table C  	AFIFO control attributes on a channel basis*/
#define	CAFT_D        0xE0058300	/* CHA FIFO Context Table D  	AFIFO control attributes on a channel basis*/
#define	CAFT_ST	      0xE0058400	/* CH		A FIFO Context Table  Status Register */
#define	CAFT_DFE      0xE0058500	/* CH		DFE Control Reg*/
#define	CAFT_AFE_RW_ST      0xE0058600	/* CH		AFE RW Status register*/



//Receive Micro-Controller RxCPU
#define	RMC_RCR				0xE0078000	/* GL		RMC configuration and timer control.*/
#define	RMC_RPC				0xE0078004	/* GL		Program Counter in the W-stage.*/
#define	RMC_ICR				0xE0078008	/* GL		Interrupt to the Xtensa core*/

// DV MICROCODE defines

#define	RMC_SEM_ADD		RMC_RCR + 0x43E	/* GL		*/
#define	RMC_CMD_CNT_ADD	        RMC_RCR + 0x43E	/* GL		*/
#define	RMC_CMD_TYP_ADD	        RMC_RCR + 0x42C	/* GL		*/
					
// Frequency domain Eq table. 																					  
#define	FETHI					0xE0078008	/* GL		*/

//Demapper:
#define DMPCTX					0xE0080000	// Demapper Context table
#define DMPPLT_01				0xE0080800	// Pilot Tone Table 0 & 1
#define DMPPLT_23				0xE0080804	// Pilot Tone Table 2 & 3
#define DMP_PRD_MASK				0xE0080808	// PRD Randome Mask
#define MDPMUT					0xE0081000	// Loop gain table
#define DMPSIT					0xE0081800	// ShowInit/InitShow Coeff Table
#define SNRCTX					0xE0082000  // SNR Context table
#define SNRTON					0xE0082800	// SNR Tone Table
#define FEQ_REPLACE_COEFF			0xE0083800	// FEQ_REPLACE_COEFF Table
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
#define	CBIT_E					0xE0060700	/* CH		Interleave BFIFO Context Table D    Interleave FIFO control attributes*/


//Deframer Micro-Controller
#define	DMC_DCR					0xE0020000	/* GL		DMC Control Register DMC configuration and timer control.*/
#define	DMC_DPC					0xE0020004	/* GL		DMC Program Counter Program Counter in the W-stage.*/
#define	DMC_ICR					0xE0020008	/* GL		Interrupt to the Xtensa core*/

// DV MICROCODE defines

#define	DMC_SEM_ADD		DMC_DCR + 0x43E	/* GL		*/
#define	DMC_CMD_CNT_ADD	        DMC_DCR + 0x43E	/* GL		*/
#define	DMC_CMD_TYP_ADD	        DMC_DCR + 0x42C	/* GL		*/


// CFIFO Interface
#define	CCFT0_A					0xE0068000	/* PHY	  CFFIFO Context table 0 Bank A     CCFT0 table contains control attributes for physical channel 0-31*/
#define	CCFT1_A					0xE0068200	/* PHY	  CFFIFO Context table 1 Bank A     CCFT1 table contains control attributes for physical channel 32-63*/
#define	CCFT0_B					0xE0068100	/* PHY	  CFFIFO Context table 0 Bank B     CCFT0 table contains control attributes for physical channel 0-31*/
#define	CCFT1_B					0xE0068300	/* PHY	  CFFIFO Context table 1 Bank B     CCFT1 table contains control attributes for physical channel 32-63*/
#define	CCFT0_C					0xE0068400	/* PHY	  CFFIFO Context table 0 Bank B     CCFT0 table contains control attributes for physical channel 0-31*/
#define	CCFT1_C					0xE0068600	/* PHY	  CFFIFO Context table 1 Bank B     CCFT1 table contains control attributes for physical channel 32-63*/
#define	CCFT0_D					0xE0068500	/* PHY	  CFFIFO Context table 0 Bank B     CCFT0 table contains control attributes for physical channel 0-31*/
#define	CCFT1_D					0xE0068700	/* PHY	  CFFIFO Context table 1 Bank B     CCFT1 table contains control attributes for physical channel 32-63*/
#define	CCFT_E					0xE0068800	/* PHY	  CFFIFO Context table 0 Bank B     CCFT0 table contains control attributes for physical channel 0-31*/


#ifdef JAGUAR
// Receive Network Interface -Jaguar
#define RNI_SRST        0xE0070048
#define RNI_PHYA        0xE007004c
#define RNI_PHYB        0xE0070050
#else
// Receive Network Interface -Mustang
#define	RNI_MCR					0xE0028000	/* GL	   RNI Module Control Register          RNI control attributes*/
#define	PHY_CTRL_A				0xE0028100	/* PHY	   PHY 0 through 31 Control Register    PHY 0 through 31 control attributes*/
#define	PHY_CTRL_B				0xE0028200	/* PHY	   PHY 32 through 63 Control Register   PHY 32 through 63 control attributes*/
#endif
// PIO Interface
#define	PIO_MCR					0xE0070000	/* GL		PIO Module Control Register Information for the HIC Interface*/
#define	CKM_CCR					0xE0070004	/* GL		Clock module config options*/
#define	CKM_PCR					0xE0070008	/* GL		Clock module config options*/
#define	PIO_ISR					0xE007000c	/* GL		Clock module config options*/
#define	PIO_RSR					0xE0070010	/* GL		Clock module config options*/
#define	PIO_BID					0xE0070014	/* GL		Clock module config options*/
#define	DAC_MCR					0xE0070018	/* GL		Clock module config options*/


	
//DMA Controller
// Use ch0-ch3 for Jaguar
#define	DMA_CCR					0xE0030000	/* CH		DMA channel control attributes */

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

#define	DMA_ECR		            0xE0030040	/* CH		DMA channel 0 event control attributes */
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

#define	DMA_SAR		            0xE0030080	/* CH		DMA transfer source address for channel 0*/
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

#define	DMA_DAR		            0xE00300C0	/* CH		DMA transfer destination address for channel 0*/
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
#define	EMI_MCR		            0xE0038000	/* GL		EMI Control Register    EMI control attributes*/
#define	EMI_RPR		            0xE0038004	/* GL		SDRAM Refresh Period Register   32'h43	SDRAM refresh period*/
#define	EMI_RCR		            0xE0038008	/* GL		SDRAM Refresh Command Register  	SDRAM refresh command trigger */
#define	EMI_PCR		            0xE003800C	/* GL 		SDRAM Precharge Command RegisterGL		SDRAM precharge command trigger*/
#define	EMI_SCR		            0xE0038010	/* GL		SDRAM Set Mode Command Register 	SDRAM set mode command trigger*/
#define	EMI_BASE_1	            0x20000000	/* GL		Region 1 */
#define	EMI_BASE	            0x40000000	/* GL		Region 2 */

// TC Emulater Registers
#define RX_HDR_CH0				0xE0018000	// Receiver Header Channel 0
#define RX_HDR_CH1				0xE0018004	// Receiver Header Channel 1
#define RX_HDR_CH2				0xE0018008	// Receiver Header Channel 2
#define RX_HDR_CH3				0xE001800C	// Receiver Header Channel 3
#define RX_HDR_CH4				0xE0018010	// Receiver Header Channel 4
#define RX_HDR_CH5				0xE0018014	// Receiver Header Channel 5
#define RX_HDR_CH6				0xE0018018	// Receiver Header Channel 6
#define RX_HDR_CH7				0xE001801c	// Receiver Header Channel 7

#define	YFI_RD_LAT				0xE0018020	// YFIFO read latency (C)

#define	YFI_HDR_LAT				0xE0018024	// YFIFO header read latency (A)
#define	YFI_DREQ_LAT			0xE0018028	// YFIFO first data request latency (B)
#define	BFI_HVAL_LAT			0xE001802c	// BFIFO header ack to header valid latency (D)
#define	BFI_DVAL_LAT			0xE0018030	// BFIFO data ack to data valid latency (E)
#define	BFI_DAT_CTRL			0xE0018034	// BFIFO data control (F)




//#define FRAMER_CPU_CODE_ADDRESS	0x40100000


#define NUM_ADSL_CHANNELS 		32
#define NUM_VDSL_CHANNELS 		1
#define NUM_DSL_CHANNELS 		NUM_VDSL_CHANNELS 


#define SZ_XFIFO_PER_CHANNEL    (0x4000/NUM_DSL_CHANNELS)
												  
#define MEM_OFFSET						0
#define ADDRESS_FRAMER_DCACHE_PIO		0xE0008040
#define ADR_FMC_DCACHE_PIO				0xE0008040
#define	ADDRESS_RXCPU_DCACHE_PIO		0xE0078040
#define	ADDRESS_RXCPU_ICACHE_PIO		0xE00B8000
#define	ADDRESS_DEMAPPERCPU_DCACHE_PIO	0xE0020040
#define	ADR_DMC_DCACHE_PIO				0xE0020040
#define	ADDRESS_DMC_ICACHE_PIO		0xE00B0000
#define	ADDRESS_FEQ_CG_TABLE_PIO		0xE0088000

#ifdef JAGUAR
#define FCPU_BMECTRL_MSGS				(ADDRESS_FRAMER_DCACHE_PIO + 0x0460)
#else
#define FCPU_BMECTRL_MSGS				(ADDRESS_FRAMER_DCACHE_PIO + 0x1000)
#endif

//#define FMC_ADR_TX_STATUS_AND_CONFIGS	(ADDRESS_FRAMER_DCACHE_PIO + 0x2000)
#define RXCPU_BMECTRL_MSGS     			(ADDRESS_RXCPU_DCACHE_PIO + 0x1000)
#define DEFCPU_BMECTRL_MSGS				ADDRESS_DEMAPPERCPU_DCACHE_PIO
#define	TX_FRM_CNT_ADDR					(ADDRESS_FRAMER_DCACHE_PIO + 0x100)

extern int	host_mode_flag;
							    
// channel ranges from 0..31						
// baseAddress is one of the PIO E0xxxxxx addresses
// numegs = number of regs to read. Assumes that data[] has enuf room to hold
void 	writePIO_Regs(int *regAddress, int numRegs, int *data);
void 	readPIO_Regs(int *RegAddress, int numRegs, int *data);
void 	writePIO_SingleRegChannel(int *baseRegAddress, int channel, int  data);
void 	writePIO_short(unsigned short *addr, unsigned short val);
void	initHostIO(int endianType, int baseAddr);
unsigned short 	readPIO_short(unsigned short *addr);
int 	readPIO_SingleRegChannel(int *baseRegAddress, int channel);
//int 	getBMEAddress (int dst_address, int offset, int *isFifoDst, int port, int *address);
int 	writeBMEMemories (char *src_address, char *dst_address, int offset, int len, int port, int write_mode, int dma_channel, int dma_event_flag);
int 	readBMEMemories (char *src_address, char *dst_address, int offset, int len, int port, int read_mode, int dma_channel, int dma_event_flag);
int		hostPIO_read(int address);
int		hostPIO_write(int address, int data);
int		hostWriteBMEMemories(char *dst, char *src, int len, int write_mode, int fifo_flag, int chan_num);
int		hostReadBMEMemories(char *dst, char *src, int len, int read_mode, int fifo_flag, int chan_num);
#endif






