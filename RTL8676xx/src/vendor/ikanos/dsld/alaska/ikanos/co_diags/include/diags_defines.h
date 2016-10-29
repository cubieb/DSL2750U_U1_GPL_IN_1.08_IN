/**************************************************************
$Workfile:   bup_iposapi.h  $
***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communications, Fremont, CA
** All rights reserved. 
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
$Revision: 1.1 $
$Date: 2008/07/18 11:09:33 $
***************************************************************/
#ifdef CO_30_DIAGS

#ifdef CPE_50

#endif

#include <math.h>
#ifdef USE_VXWORKS
#include "config.h"
#else
#define UINT32  unsigned long
#define FALSE   0
#define SYS_CLK_RATE     480
#endif
#include "apiosmacro.h"

// taskdelay in milliseconds rounded to the nearest integer number of ticks
#define BUP_DELAY_MS(a) { double fr_delay = (a*SYS_CLK_RATE)/1000;          \
    UINT32 intdelay = (UINT32) fr_delay; UINT32 tick_Delay;                 \
    if (fr_delay < 1) {tick_Delay = 1;} else                                \
    {tick_Delay = fr_delay - intdelay < 0.5 ? intdelay : intdelay + 1;}     \
    iposTaskDelay(tick_Delay);                                              \
}

#define BUP_DIAGS_SELECT_MSG       (0xff)
#define BUP_DIAGS_GOTO_NORMAL_MODE (0xfe)

#define BUP_DIAGS_HIC_STRESS_TEST  (0x1)
#define BUP_DIAGS_BME_STRESS_TEST  (0x2)
#define BUP_DIAGS_PTM_UTOP_LB_TEST (0x3)

#define BME_CO_20   (0x20)
#define BME_CPE_30  (0x30)
#define BME_CO_30   (0x50)
#define BME_CO_30MM (0x51)
#define BME_CO_40   (0x60)
#define BME_CPE_50  (0x50)

#define CO_DIAGS_MODE (0)
#define CO_NORMAL_MODE (1)


#define NUM_BME_IDS (6) // This number has to match num. of defns below

// CO3 and CPE5 have the same ID
#ifdef CPE_50
#define VALID_BME_IDS \
    {BME_CO_20,  "CO 2.0 BME"},     \
    {BME_CPE_30, "CPE 3.0 BME"},    \
    {BME_CPE_50, "CPE 5.0 BME"},    \
    {BME_CO_30,  "CO 3.0 BME"},     \
    {BME_CO_30MM,"CO 3.0 RevB BME"}, \
    {BME_CO_40,  "CO 4.0 BME"}
#else
#define VALID_BME_IDS \
    {BME_CO_20,  "CO 2.0 BME"},     \
    {BME_CPE_30, "CPE 3.0 BME"},    \
    {BME_CO_30,  "CO 3.0 BME"},     \
    {BME_CO_30MM,"CO 3.0 RevB BME"}, \
    {BME_CPE_50, "CPE 5.0 BME"},    \
    {BME_CO_40,  "CO 4.0 BME"}
#endif
typedef struct
{
    int bmeID;
    UINT8 bmeName[25];

} bmeIdStruct_t;

extern int gDiagDbgLvl;
extern int gBmeMemSz[];

#define MAX_DBG_LVL (4)
#define logfunc IK_PRINTF

#define DBG0(a) {if (gDiagDbgLvl >= 0) {logfunc a;}}
#define DBG1(a) {if (gDiagDbgLvl >= 1) {logfunc a;}}
#define DBG2(a) {if (gDiagDbgLvl >= 2) {logfunc a;}}
#define DBG3(a) {if (gDiagDbgLvl >= 3) {logfunc a;}}
#define DBG4(a) {if (gDiagDbgLvl >= 4) {logfunc a;}}

#if 0
#define DBG_LEVEL (2)

#define DBG0(a)
#define DBG1(a)
#define DBG2(a)
#define DBG3(a)
#define DBG4(a)

#if DBG_LEVEL>=0
#undef DBG0
#define DBG0(a) logfunc a
#if DBG_LEVEL>=1
#undef DBG1
#define DBG1(a) logfunc a
#if DBG_LEVEL>=2
#undef DBG2
#define DBG2(a) logfunc a
#if DBG_LEVEL>=3
#undef DBG3
#define DBG3(a) logfunc a
#if DBG_LEVEL>=4
#undef DBG4
#define DBG4(a) logfunc a
#endif
#endif
#endif
#endif
#endif

#endif

// ENDIANESS DEFINITIONS
#define BIG_ENDIAN      (1)
#define LITTLE_ENDIAN   (0)
#define HOST_ENDIANESS  (BIG_ENDIAN)
// Address locations in BME memory used to test
// the HIC PIO and DMA functions
#define HIC_PIO_TEST_MEM_ADDR (0x40000104)
#define HIC_DMA_TEST_MEM_ADDR (0x40008000)

#define DMA_WAIT_LOOP_TIMEOUT (300)
#define DMA_TEST_START_ADDR (0x40000000)
#define DMA_TEST_DEST_ADDR  (0x40080000)
// has to match address in bme code
#define BME_CODE_EXEC_TEST_LOC  (0x40000100)
#define BME_CODE_EXEC_TEST_VAL  (0x12345678)
#define BME_CODE_EXEC_TIMEOUT   (100)

/**************************************************************
* BME PIO INTERFACE AND MEMORY RELATED CONSTANTS
***************************************************************/
#define BME_SDRAM_MEMBASE				(unsigned long) (0x40000000)
#define BME_SDRAM_MEM_ADDR_SPACE		(unsigned long) (0x10000000)
#define BME_SDRAM_MEMSIZE_BYTES_DEFAULT	(unsigned long) (0x800000)

#define DMA_ECR_SW_REQ                  (0x80000000)
#define DMA_CCR_START                   (0x1)
#define DMA_CCR_DONE                    (0x1<<8)
#define DMA_CCR_DEST_FIFO               (0x1<<9)
#define DMA_CCR_SRC_FIFO                (0x1<<10)
#define DMA_CCR_CH_EN                   (0x1<<12)

#define	ADDRESS_BFIFOINTERLEAVE 		0xC5000000	/* 128 KB	DMA	*/
#define BFI_CBIT_A                      (0xe0060300)
#define BFI_CBIT_B                      (0xe0060400)
#endif //#ifdef CO_30_DIAGS
