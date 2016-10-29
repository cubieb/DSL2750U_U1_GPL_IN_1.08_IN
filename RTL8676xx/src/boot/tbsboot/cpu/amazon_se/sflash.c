/*****************************************************************************
 * DANUBE BootROM
 * Copyright (c) 2005, Infineon Technologies AG, All rights reserved
 * IFAP DC COM SD
 *****************************************************************************/

#include <config.h>
#include "lib.h"
#include "ssc.h"
#include "sflash.h"
//#include <asc.h>
#include <asm/addrspace.h>
#include <asm/ifx_mips.h>
#include "bstrap.h"

/*****************************************************************************
 * Local functions
 *****************************************************************************/
//static spi_dev_t spi_sflash;
extern spi_dev_t spi_sflash;

/*****************************************************************************
 * Global functions
 *****************************************************************************/

/************************************************************************
 * Function name:	init
 * Description:	Initialize SSC interface 
 * Input value:	None
 * Return value:	0 on success
 ***********************************************************************/
int sflash_init(void)
{
  asc_puts("SFLASH\n");
	ssc_init((u32)SFLASH_BAUDRATE);
	spi_sflash.read_cmd    = SFLASH_READ;

#if defined(ON_VENUS) || defined(ON_IKOS)
	spi_sflash.cs = 0;      /* SPI_CS1 */
#else
//	spi_sflash.cs = 1;      /* SPI_CS2 */
	spi_sflash.cs = 0;      /* SPI_CS1 actual setting */

#endif
	spi_sflash.addr_cycles = SPI_ADDR_CYC_MAX;
	return SFLASH_OK;
}

/************************************************************************
* Function name:	sflash_config
* Description:	Configure SSC interface
* Input value:	None
* Return value:	0 on success
***********************************************************************/
int sflash_read_block(u32 *addr, int length, u32 offset)
{
	int err;
   //printf("\nEntered sflash_read_block with addr_cycles = %x\n", spi_sflash.addr_cycles);
	err=spi_read(&spi_sflash,addr,length,offset);
	if ( err < 0){
		return BSTRAP_ERR;
	}else{
		return length;
	}
}

int sflash_terminate(void)
{
	SSC1_CS_SET(spi_sflash.cs);
	return SFLASH_OK;
}

