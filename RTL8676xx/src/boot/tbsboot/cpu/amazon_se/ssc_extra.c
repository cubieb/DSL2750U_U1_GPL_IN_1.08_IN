/*****************************************************************************
 * DANUBE BootROM
 * Copyright (c) 2005, Infineon Technologies AG, All rights reserved
 * IFAP DC COM SD
 *****************************************************************************/

#include <asm/lib.h>
#include <asm/addrspace.h>
#include "ssc.h"
#include "amazon_se_reg.h"
//#include <asc.h>
#include "ssc_extra.h"

/* Brief:	SPI WR RD session
 * Parameter:
 	dev: spi device
	address: 0 means null
	Additional dummy cycle: 0 means no
	write buffer
	write cnt
	read buffer
	read cnt
	command
 * Description:
 	CMD [ADDRESS] [DUMMY_CYCLE] [Write Data] [Read Data]
 */
void spi_session(spi_dev_t *dev, u32 cmd, u8 *address, u32 dummy_cycles, u8 * wr_buf, u32 wr_cnt, u8 * rd_buf, u32 rd_cnt)
{
	int i;

	SSC1_CS_CLEAR(dev->cs);
	ssc_tx_only();

	ssc_write(cmd);

  if (address){
    for(i=dev->addr_cycles;i>0;i--){
      ssc_write(*address++);
    }
  }
  while (dummy_cycles--){
    ssc_write(0);
  }

	for(i=0;i<wr_cnt;i++){
		ssc_write(wr_buf[i]);
	}

	ssc_sync();
	if (rd_cnt) {
		ssc_rx_only();
		ssc_read_only(rd_buf,rd_cnt);
	}
	SSC1_CS_SET(dev->cs);

	return;
}
