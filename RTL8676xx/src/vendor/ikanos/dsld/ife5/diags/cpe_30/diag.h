
/**************************************************************
***************************************************************
**
** Host Application, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
***************************************************************

$Revision: 1.1 $
$Date: 2008/07/18 11:08:44 $
**************************************************************/

#define PASSED	0
#define FAILED  1
#define RAM_LOW_ADRS         0x00100000        /* RAM address for sys image  */
#define RAM_HIGH_ADRS        0x00E00000        /* RAM address for ROM boot   */
#define	COM1_BASE_ADR		 0xffef0000

//#define	PIO_BID					0xE0070014	/* GL		Clock module config options*/
//#define	CKM_CCR					0xE0070004	/* GL		Clock module config options*/

#define MAX_LINE        160	/* max line length for input to 'm' routine */
#define DEC		FALSE	/* getArg parameters */
#define HEX		TRUE
#define OPT		TRUE

#ifdef BME_CPE_20
#define CODE_MEMORY_ADR		0x20000000
#define BME_CHIP_ID		    0x10
#else
#define BME_CHIP_ID		    0x1
#define CODE_MEMORY_ADR		0x40000000
#endif

#define MDIO_POLLING        0xe009808c


#define DMA_MEM_ADR         0xc0000000


#define	ADDRESS_SMCONTROLTONEMEMORY		0xD2002000	/* 8 KB		DMA		*/

#define	ADDRESS_SMCONTROLTHRESMEMORY	0xD2002180	/* 8 KB		DMA		*/

#define	CAFT_B	            0xE0058100	/* CH		A FIFO Context Table B  	AFIFO control attributes on a channel basis*/

#define	MDIO_REG_1          0xe0098080  /* MDIO register 1 */
#define	MDIO_REG_2          0xe0098084  /* MDIO register 2 */
#define	MDIO_REG_3          0xe0098088  /* MDIO register 2 */
#define	MDIO_REG_4          0xe0098090  /* MDIO register 2 */


#define	FPGA_REG_1          0xe00a6000  /* FPGA register 1 */
#define	FPGA_REG_2          0xe00a6004  /* FPGA register 2 */
#define	FPGA_REG_3          0xe00a6008  /* FPGA register 3 */
#define	FPGA_REG_4          0xe00a600c  /* FPGA register 4 */


#define	UART_REG_1          0xe00a0000  /* UART register 1 */
#define	UART_REG_2          0xe00a0004  /* UART register 2 */
#define	UART_REG_3          0xe00a0008  /* UART register 3 */
#define	UART_REG_4          0xe00a000c  /* UART register 4 */


#define writeControlPort_diag(x,y,z)			(*(x+y) = z);iposDelay()
#define readControlPort_diag(x,y,z)			z = (*(x+y));iposDelay()
