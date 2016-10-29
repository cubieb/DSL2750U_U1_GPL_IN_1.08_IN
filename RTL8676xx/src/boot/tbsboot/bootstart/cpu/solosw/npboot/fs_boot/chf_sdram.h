#ifndef CHF_SDRAM_H
#define CHF_SDRAM_H

/* +-----------------------------------------------------------------------+ */ 
/* |                                                                       | */
/* | CHARM SDRAM BLOCK HARDWARE DEFINITIONS                                | */
/* |                                                                       | */
/* +-----------------------------------------------------------------------+ */ 

#define CHARM_SDRAM                (0x600) /* Configuration Block            */

#ifdef CHF_SDRAM_H

/********************************************/
/*** Version 1 SDRAM controller registers ***/
/********************************************/
/*** Registers in the SDRAM Block (offsets from CHM_MAP_PERIPH)         ******/
#define CHARM_SDRAM_CTL            (0x600) /* SDRAM Control                  */
#define CHARM_SDRAM_MODE           (0x604) /* SDRAM Mode                     */
#define CHARM_SDRAM_DRAIN          (0x608) /* SDRAM Drain control            */
#define CHARM_SDRAM_LIMIT0         (0x60C) /* SDRAM Limit0 control           */
#define CHARM_SDRAM_LIMIT1         (0x610) /* SDRAM Limit1 control           */
#define CHARM_SDRAM_LIMIT2         (0x614) /* SDRAM Limit2 control           */
/* Extra Gallus specific register - DELTA is a 4-bit signed integer that     */
/* can be used to "tweek" the DLL delay around the ideal 90 degrees point    */
#define CHARM_SDRAM_DLL            (0x618) /* Gallus DLL controls            */
/*        Reserved                 (0x61C)    SDRAM reserved location        */
#define CHARM_SDRAM_PROF_RD1       (0x620) /* SDRAM Port1 Profile read reg   */
#define CHARM_SDRAM_PROF_WR1       (0x624) /* SDRAM Port1 Profile write reg  */
#define CHARM_SDRAM_PROF_RD2       (0x628) /* SDRAM Port2 Profile read reg   */
#define CHARM_SDRAM_PROF_WR2       (0x62C) /* SDRAM Port2 Profile write reg  */
#define CHARM_SDRAM_PROF_RD3       (0x630) /* SDRAM Port3 Profile read reg   */
#define CHARM_SDRAM_PROF_WR3       (0x634) /* SDRAM Port3 Profile write reg  */
#define CHARM_SDRAM_PROF_RD4       (0x638) /* SDRAM Port4 Profile read reg   */
#define CHARM_SDRAM_PROF_WR4       (0x63C) /* SDRAM Port4 Profile write reg  */

#define CHARM_SDRAM_CTL_VERSION_MASK  (0xF0000000) /* Mask for Version #*/
#define CHARM_SDRAM_CTL_VERSION_0     (0x00000000) /* Version 0 of SDRAM ctrl*/
#define CHARM_SDRAM_CTL_VERSION_1     (0x10000000) /* Version 1 of SDRAM ctrl*/
#define CHARM_SDRAM_CTL_NORFRSH      (0x00000000) /* Disable Refresh       */
#define CHARM_SDRAM_CTL_RFSHPRD      (0x00000F00) /* Refresh period        */
#define CHARM_SDRAM_CTL_RFSHPRD_MASK (0x00003F00) /* Refresh period Mask   */

#define CHARM_SDRAM_CTL_BANKILV      (0x00000080) /* Enable bank interleave*/

/* Extra Gallus specific bit define for CHARM_SDRAM_CTL register (0xFF000600) */
/* Theoretically this mode should NOT work and is only for ATE test vectors   */
#define CHARM_SDRAM_CTL_DLL_BYPASS (0x00000040) /* Bypass Gallus DLLs      */

#define CHARM_SDRAM_CTL_CAS_MASK   (0x00000030) /* CAS address width mask  */
#define CHARM_SDRAM_CTL_CAS_A0A11  (0x00000030) /* CAS Addr Width A0-A9,A11*/
#define CHARM_SDRAM_CTL_CAS_A0A9   (0x00000020) /* CAS Addr Width A0-A9    */
#define CHARM_SDRAM_CTL_CAS_A0A8   (0x00000010) /* CAS Addr Width A0-A8    */
#define CHARM_SDRAM_CTL_CAS_A0A7   (0x00000000) /* CAS Addr Width A0-A7    */

#define CHARM_SDRAM_CTL_DEVICE_MASK   (0x0000000C) /* Device mask          */
#define CHARM_SDRAM_CTL_DEVICE_SDR    (0x00000008) /* Single Data Rate     */
#define CHARM_SDRAM_CTL_DEVICE_DDR    (0x00000008) /* Double Data Rate     */
#define CHARM_SDRAM_CTL_DEVICE_DIS    (0x00000000) /* Disable SDRAM        */
#define CHARM_SDRAM_CTL_PWRDOWN       (0x00000002) /* Enable power down    */
#define CHARM_SDRAM_CTL_PAD_DIS       (0x00000001) /* Disable DRAM PADS    */



/*** Bit definitions for DDR SDRAM Mode ***/
/* Note that the burst length is set when the device selection field in the
   CHARM_SDRAM_CTL register is set */

#define CHARM_SDRAM_MODE_BURSTLN  (0x00000007) /* Burst length mask         */
#define CHARM_SDRAM_MODE_BURST4   (0x00000002) /* Burst length 4            */
#define CHARM_SDRAM_MODE_BURST8   (0x00000003) /* Burst length 8            */

#define CHARM_SDRAM_MODE_SEQBRST  (0x00000000) /* Sequential bursts         */
#define CHARM_SDRAM_MODE_ILVBRST  (0x00000008) /* Interleaved bursts        */

#define CHARM_SDRAM_MODE_CASLATE  (0x00000070) /* CAS latency mask          */
#define CHARM_SDRAM_MODE_CAS2     (0x00000020) /* CAS latency 2 cycles      */
#define CHARM_SDRAM_MODE_CAS3     (0x00000030) /* CAS latency 3 cycles      */
#define CHARM_SDRAM_MODE_CAS1_5   (0x00000050) /* CAS latency 1.5 cycles    */
#define CHARM_SDRAM_MODE_CAS2_5   (0x00000060) /* CAS latency 2.5 cycles    */

#define CHARM_SDRAM_MODE_OPMODE   (0x00000F80) /* Operating Mode mask       */
#define CHARM_SDRAM_MODE_OPNORM   (0x00000000) /* Op Mode = Normal          */
#define CHARM_SDRAM_MODE_OPRDLL   (0x00000100) /* Op Mode = Normal/reset DLL*/
#define CHARM_SDRAM_MODE_OPTEST   (0x00000080) /* Op Mode = Vendor spec test*/

#define CHARM_SDRAM_MODE_DLLDIS   (0x00010000) /* Disable DDR chip DLL      */
#define CHARM_SDRAM_MODE_WEAKDRV  (0x00020000) /* Weak output drive         */
#define CHARM_SDRAM_MODE_QFCENB   (0x00040000) /* Enable QFC pin            */

#define CHARM_SDRAM_MODE_EOPMODE  (0x0FF80000) /* Extended Op Mode mask     */
#define CHARM_SDRAM_MODE_EOPNORM  (0x00000000) /* E-Op Mode = Normal        */

/*** Bit definitions for SDRAM Drain register *********************************/
#define CHARM_SDRAM_DRAIN_PORT1_MASK (0x000000FF) /* PORT1 Write drain mask */
#define CHARM_SDRAM_DRAIN_PORT2_MASK (0x0000FF00) /* PORT2 Write drain mask */
#define CHARM_SDRAM_DRAIN_PORT3_MASK (0x00FF0000) /* PORT3 Write drain mask */
#define CHARM_SDRAM_DRAIN_PORT4_MASK (0xFF000000) /* PORT4 Write drain mask */

/*** Bit definitions for SDRAM Limit0 register ********************************/
#define CHARM_SDRAM_LIMIT0_ACT_TO_ACT_MASK   (0x1F000000) /* Active to active mask     */
#define CHARM_SDRAM_LIMIT0_ACTA_TO_ACTB_MASK (0x00F00000) /* Active A to Active B mask */
#define CHARM_SDRAM_LIMIT0_RD_TO_PRE_MASK    (0x000F0000) /* Read to Precharge mask    */
#define CHARM_SDRAM_LIMIT0_WR_TO_PRE_MASK    (0x0000F000) /* Write to Precharge mask   */
#define CHARM_SDRAM_LIMIT0_WR_CAS_LAT_MASK   (0x00000F00) /* Write CAS latency mask    */
#define CHARM_SDRAM_LIMIT0_MODE_REG_SET_MASK (0x000000F0) /* Mode register set mask    */
#define CHARM_SDRAM_LIMIT0_CAS_TO_CAS_MASK   (0x0000000F) /* CAS to CAS limit mask     */

/*** Bit definitions for SDRAM Limit1 register ********************************/
#define CHARM_SDRAM_LIMIT1_REF_TO_ACT_MASK   (0x1F000000) /* Refresh to active mask    */
#define CHARM_SDRAM_LIMIT1_BANK_CLOSE_MASK   (0x00FF0000) /* Bank close mask           */
#define CHARM_SDRAM_LIMIT1_WR_TO_RD_MASK     (0x0000F000) /* Write to read mask        */
#define CHARM_SDRAM_LIMIT1_PRECHARGE_MASK    (0x00000F00) /* Precharge mask            */
#define CHARM_SDRAM_LIMIT1_ACT_TO_RD_WR_MASK (0x000000F0) /* Active to write/read mask */
#define CHARM_SDRAM_LIMIT1_ACT_TO_PRE_MASK   (0x0000000F) /* Active to precharge mask  */

/*** Bit definitions for SDRAM Limit2 register ********************************/
#define CHARM_SDRAM_LIMIT2_RD_CAS_MASK (0x0000000F) /* Read CAS Latency mask */
#define CHARM_SDRAM_LIMIT2_RD_CAS6     (0x00000006) /* Read CAS Latency 6 */
#define CHARM_SDRAM_LIMIT2_RD_CAS3     (0x00000003) /* Read CAS Latency 3 */

#define CHARM_SDRAM_LIMIT2_RD_TO_WR_MASK (0x000000F0) /* Read to Write mask */

#define CHARM_SDRAM_LIMIT2_WIDTH_MASK  (0x00000300) /* SDRAM Data width */
#define CHARM_SDRAM_LIMIT2_WIDTH_8     (0x00000200) /*  8bit Data width */
#define CHARM_SDRAM_LIMIT2_WIDTH_16    (0x00000100) /* 16bit Data width */
#define CHARM_SDRAM_LIMIT2_WIDTH_32    (0x00000000) /* 32bit Data width */


/*** Bit definitions for SDRAM DLL register ********************************/
#define CHARM_SDRAM_DLL_DELTA0_MASK  (0xF)     /* 4-bit timing delta for DLL0*/
#define CHARM_SDRAM_DLL_DELTA0_SHIFT (0)
#define CHARM_SDRAM_DLL_DELTA1_MASK  (0xF00)   /* 4-bit timing delta for DLL1*/
#define CHARM_SDRAM_DLL_DELTA1_SHIFT (8)

/*** Bit definitions for SDRAM Profile register ********************************/
#define CHARM_SDRAM_PROF_STALLED_MASK  (0x0000FFFF) /* Mask for stalled half word */
#define CHARM_SDRAM_PROF_ACTIVE_MASK   (0xFFFF0000) /* Mask for active half word */


#else

/********************************************/
/*** Version 0 SDRAM controller registers ***/
/********************************************/
/*** Registers in the SDRAM Block (offsets from CHM_MAP_PERIPH)         ******/
#define CHARM_SDRAM_CTL            (0x600) /* SDRAM Control                  */
#define CHARM_SDRAM_MODE           (0x604) /* SDRAM Mode                     */
#define CHARM_SDRAM_PARAM          (0x608) /* SDRAM Paramter Control         */
#define CHARM_SDRAM_PROFILE        (0x60C) /* SDRAM Profile                  */

/*** Bit definitions for SDRAM Control register ******************************/

#define CHARM_SDRAM_CTL_VERSION_MASK  (0xF0000000) /* Mask for Version #*/
#define CHARM_SDRAM_CTL_VERSION_0  (0x00000000) /* Version 0 of SDRAM controller*/
#define CHARM_SDRAM_CTL_VERSION_1  (0x10000000) /* Version 1 of SDRAM controller*/
#define CHARM_SDRAM_CTL_NORFRSH    (0x00200000) /* Disable Refresh          */
#define CHARM_SDRAM_CTL_RFSHPRD    (0x001E0000) /* Refresh period          */
#define CHARM_SDRAM_CTL_P2PR2      (0x0001C000) /* INS/DMA1 priority       */
#define CHARM_SDRAM_CTL_P1PRI      (0x00003800) /* DAT priority            */
#define CHARM_SDRAM_CTL_P0PRI      (0x00000700) /* DMA0 priority           */
#define CHARM_SDRAM_CTL_BANKILV    (0x00000080) /* Enable bank interleave  */
#define CHARM_SDRAM_CTL_ILV        (0x00000040) /* Enable interleave       */

#define CHARM_SDRAM_CTL_CAS_MASK   (0x00000030) /* CAS address width mask  */
#define CHARM_SDRAM_CTL_CAS_A0A11  (0x00000030) /* CAS Addr Width A0-A9,A11*/
#define CHARM_SDRAM_CTL_CAS_A0A9   (0x00000020) /* CAS Addr Width A0-A9    */
#define CHARM_SDRAM_CTL_CAS_A0A8   (0x00000010) /* CAS Addr Width A0-A8    */
#define CHARM_SDRAM_CTL_CAS_A0A7   (0x00000000) /* CAS Addr Width A0-A7    */

#define CHARM_SDRAM_CTL_DEVICE_MASK (0x0000000C) /* Device mask          */
#define CHARM_SDRAM_CTL_DEVICE_FCR  (0x0000000C) /* Fast Cycle device    */
#ifndef GALLUS_WLC
#define CHARM_SDRAM_CTL_DEVICE_DDR  (0x00000008) /* Double Data Rate     */
#define CHARM_SDRAM_CTL_DEVICE_SDR  (0x00000004) /* Single Data Rate     */
#endif
#define CHARM_SDRAM_CTL_DEVICE_DIS  (0x00000000) /* Disable SDRAM        */
#define CHARM_SDRAM_CTL_PWRDOWN     (0x00000002) /* DRAM Power down      */
#define CHARM_SDRAM_CTL_PAD_DIS     (0x00000001) /* Disable DRAM PADS    */

#define CHARM_SDRAM_PARAM_RD_CAS_MASK (0x0000000F) /* SDRAM Read CAS Latency mask */
#define CHARM_SDRAM_PARAM_RD_CAS6     (0x00000006) /* SDRAM Read CAS Latency 6 */
#define CHARM_SDRAM_PARAM_RD_CAS3     (0x00000003) /* SDRAM Read CAS Latency 3 */

#define CHARM_SDRAM_PARAM_WR_CAS_MASK (0x000000F0) /* SDRAM Write CAS Latency mask */
#define CHARM_SDRAM_PARAM_WR_CAS6     (0x00000060) /* SDRAM Write CAS Latency 6 */
#define CHARM_SDRAM_PARAM_WR_CAS3     (0x00000030) /* SDRAM Write CAS Latency 3 */

#define CHARM_SDRAM_PARAM_WIDTH_MASK  (0x00000300) /* SDRAM Data width */
#define CHARM_SDRAM_PARAM_WIDTH_8     (0x00000200) /*  8bit Data width */
#define CHARM_SDRAM_PARAM_WIDTH_16    (0x00000100) /* 16bit Data width */
#define CHARM_SDRAM_PARAM_WIDTH_32    (0x00000000) /* 32bit Data width */

/*** Bit definitions for DDR SDRAM Mode ***/
/* Note that the burst length is set when the device selection field in the
   CHARM_SDRAM_CTL register is set */

#define CHARM_SDRAM_MODE_BURSTLN  (0x00000007) /* Burst length mask         */
#define CHARM_SDRAM_MODE_BURST4   (0x00000002) /* Burst length 4            */
#define CHARM_SDRAM_MODE_BURST8   (0x00000003) /* Burst length 8            */

#define CHARM_SDRAM_MODE_SEQBRST  (0x00000000) /* Sequential bursts         */
#define CHARM_SDRAM_MODE_ILVBRST  (0x00000008) /* Interleaved bursts        */

#define CHARM_SDRAM_MODE_CASLATE  (0x00000070) /* CAS latency mask          */
#define CHARM_SDRAM_MODE_CAS2     (0x00000020) /* CAS latency 2 cycles      */
#define CHARM_SDRAM_MODE_CAS3     (0x00000030) /* CAS latency 3 cycles      */
#define CHARM_SDRAM_MODE_CAS1_5   (0x00000050) /* CAS latency 1.5 cycles    */
#define CHARM_SDRAM_MODE_CAS2_5   (0x00000060) /* CAS latency 2.5 cycles    */

#define CHARM_SDRAM_MODE_OPMODE   (0x00000F80) /* Operating Mode mask       */
#define CHARM_SDRAM_MODE_OPNORM   (0x00000000) /* Op Mode = Normal          */
#define CHARM_SDRAM_MODE_OPRDLL   (0x00000100) /* Op Mode = Normal/reset DLL*/
#define CHARM_SDRAM_MODE_OPTEST   (0x00000080) /* Op Mode = Vendor spec test*/

#define CHARM_SDRAM_MODE_DLLDIS   (0x00010000) /* Disable DDR chip DLL      */
#define CHARM_SDRAM_MODE_WEAKDRV  (0x00020000) /* Weak output drive         */
#define CHARM_SDRAM_MODE_QFCENB   (0x00040000) /* Enable QFC pin            */

#define CHARM_SDRAM_MODE_EOPMODE  (0x0FF80000) /* Extended Op Mode mask     */
#define CHARM_SDRAM_MODE_EOPNORM  (0x00000000) /* E-Op Mode = Normal        */

/*** Bit definitions for SDRAM Profile ***************************************/

#define CHARM_SDRAM_PROFILE_P0STALL  (0x0000007F) /* DMA0 max stall time    */
#define CHARM_SDRAM_PROFILE_P0QJUMP  (0x00000080) /* DMA0 queue was jumped  */
#define CHARM_SDRAM_PROFILE_P1STALL  (0x00007F00) /* DAT max stall time     */
#define CHARM_SDRAM_PROFILE_P1QJUMP  (0x00008000) /* DAT queue was jumped   */
#define CHARM_SDRAM_PROFILE_P2STALL  (0x007F0000) /* INS/DMA1 max stall time*/
#define CHARM_SDRAM_PROFILE_P2QJUMP  (0x00800000) /* INS/DMA1 queue was jumped*/
#define CHARM_SDRAM_PROFILE_ADCLASH  (0x01000000) /* Addr clash between     */

#endif
#endif
