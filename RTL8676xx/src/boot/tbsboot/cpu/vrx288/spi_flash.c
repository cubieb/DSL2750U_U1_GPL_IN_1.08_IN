/*
 * SPI flash interface
 *
 * Copyright (C) 2011 T&W Corporation
 */

#include <common.h>
#include <malloc.h>
#include "../../bootstart/cpu/vrx288/vr9.h"
#include "../../bootstart/cpu/vrx288/spi_flash.h"

#if (defined(SPI_FLASH) && defined(CFG_VRX288_FLASH_SPI_DRIVER))
struct flash_params {
	u8	manufacturer_id;
	u8	memory_id;
	u8	devive_id;
	u32 page_size;
	u32	block_size;
	u8	nr_blocks;
	char	*name;
};

static const struct flash_params flash_table[] = {
	{0xc2, 0x20, 0x14, 256, 65536, 16, "MX25L8005"},
	{0xc2, 0x20, 0x15, 256, 65536, 32, "MX25L1605D"},
	{0xc2, 0x20, 0x16, 256, 65536, 64, "MX25L3205D"},
	{0xc2, 0x20, 0x17, 256, 65536, 128, "MX25L6405D"},
	{0x00, 0x00, 0x00, 0x0, 0, 0x0, "FLASH_UNKOWN"},
};

flash_info_t flash_info[CFG_MAX_FLASH_BANKS];           /* FLASH chips info */
const struct flash_params *pa = NULL;

static void spi_flash_cmd(u8 command, u32 addr, void *write, void*read, size_t data_len)
{
	u32 offset;
	u8 *tx = NULL;
	int i = 0;
	int cmd_cycle = 1;

	if(0 != addr)
		cmd_cycle += 3;
	if ((command == CMD_MX25L_FASTREAD) || (command == CMD_MX25L_DREAD))
		cmd_cycle += 1;
	if((NULL != write) && (data_len > 0))
		{
		cmd_cycle += data_len;
		}
	tx = malloc(cmd_cycle);
	if(NULL == tx)
		{
		printf("SF:Can not allocate memory!\n");
		goto out;
		}
	if(0 != command)
		tx[0] = command;	
	if(0 != addr)
		{
		offset = addr - CONFIG_FLASH_BASE;
		tx[1] = (offset >> 16) & 0xFF;
		tx[2] = (offset >> 8) & 0xFF;
		tx[3] = offset & 0xFF;
		}
	if ((command == CMD_MX25L_FASTREAD) || (command == CMD_MX25L_DREAD))
		{
		tx[4] = 1;
		if((NULL != write) && (data_len > 0))
			memcpy(&tx[5], write, data_len);
		}
	else
		{
		if((NULL != write) && (data_len > 0))
			memcpy(&tx[4], write, data_len);
		}

	*SPI_FGPO = (1<<(CONFIG_ENV_SPI_CS)); //spi_cs_on
	for(i=0; i<cmd_cycle; i++)
		{
		asm("sync");
    	while(((*SPI_STAT) & SPI_STAT_BSY) == SPI_STAT_BSY);
		/* change to TX only mode */
	    *SPI_CON = ((*SPI_CON) & (~SPI_CON_TX_OFF)) | SPI_CON_RX_OFF;
    	while(((*SPI_STAT) & SPI_STAT_BSY) == SPI_STAT_BSY);
		*SPI_TB = (u32)(*(u8*)(tx + i));
    	while(((*SPI_STAT) & SPI_STAT_BSY) == SPI_STAT_BSY);
		asm("sync");
	    }
	
		if((NULL != read) && (data_len > 0))
			for(i=0; i<data_len; i++)
			{
			asm("sync");
   			while(((*SPI_STAT) & SPI_STAT_BSY) == SPI_STAT_BSY);
		    /* change to RX only mode */
    		*SPI_CON = ((*SPI_CON) & (~SPI_CON_RX_OFF)) | SPI_CON_TX_OFF;
			/* flush RX */
			*SPI_RXFCON = (*SPI_RXFCON) | SPI_RXFCON_RXFLU;
		    while(((*SPI_STAT) & SPI_STAT_BSY) == SPI_STAT_BSY);
    		*SPI_RXREQ = 1;
		    /* poll RIR */
		    while((SPI_FSTAT_RXFFL(*SPI_FSTAT)) == 0) { };
			*(u8*)(read + i)= (u8)*SPI_RB;
			asm("sync");
			}
	*SPI_FGPO = (1<<(CONFIG_ENV_SPI_CS+8)); //spi_cs_off
out:
	free(tx);
}


static void get_flash_id(u8 *id)
{
	spi_flash_cmd(CMD_MX25L_RDID, 0, NULL, id, 3);
}

static int wait_for_ready(ulong timeout, int enable_write)
{
	unsigned long timebase;
	u8 status;
	int i;
	int ret = ERR_TIMOUT;

//	debug("SF: Waiting for ready!\n");
	for(i=0; i<timeout; i++)
		{
		timebase = get_timer(0);
		do
			{
			if (0 != enable_write)
				spi_flash_cmd(CMD_MX25L_WREN, 0, NULL, NULL, 0);
			spi_flash_cmd(CMD_MX25L_RDSR, 0, NULL, &status, sizeof(status));
        	if ((status & MX25L_SR_WIP) != MX25L_SR_WIP)
        		{
        		if((0 != enable_write) && ((status & MX25L_SR_WEL) != MX25L_SR_WEL))
					continue;
				else
					{
					ret = ERR_OK;
					goto out;
					}
        		}
			}
		while (get_timer(timebase) <= CFG_HZ);
		}
out:
	if(ret != ERR_OK)
		debug("SF: Status register value= %X\n", status);
	return ret;							
}

static void spi_cmd_read_bytes(u32 addr, void *buf, size_t len)
{
	u8 cmd;
#if defined(CMD_MX25L_FASTREAD)	
	cmd = CMD_MX25L_FASTREAD;
#else
	cmd = CMD_MX25L_READ;
#endif
	spi_flash_cmd(cmd, addr, NULL, buf, len);
}

static int spi_cmd_erase(u8 cmd, u32 addr)
{
	int ret_val = ERR_OK;

	ret_val |= wait_for_ready(pa->nr_blocks * 2, 1);
	spi_flash_cmd(cmd, addr, NULL, NULL, 0);
	ret_val |= wait_for_ready(pa->nr_blocks * 2, 0);
	
	return ret_val;
}

static int spi_cmd_program_pages(u32 addr, void *buf, size_t len)
{
	int ret_val = ERR_OK;

	if((len > 1) && (len <= 256))
		{
		ret_val |= wait_for_ready(pa->nr_blocks, 1);
		spi_flash_cmd(CMD_MX25L_PP, addr, buf, NULL, len);
		ret_val |= wait_for_ready(pa->nr_blocks, 0);
		}
	return ret_val;
}

#if 0
static int program_pages(u32 addr, void *buf, volatile int *count, size_t len)
{
	int cycles = 0;
	int align_bytes = 0;
	int i;
	int ret_val = ERR_OK;
	
	cycles = len / pa->page_size;
	align_bytes = len % pa->page_size;

	if(0 != align_bytes)
		cycles += 1;

	for(i=0; i<cycles; i++)
		{
		if((ret_val & ERR_TIMOUT) == ERR_TIMOUT)
			break;
		if(((*count) % 256) == 0)
			printf("\n\t");
		else if (((*count) % 4) == 1)
			printf("#");

		(*count)++;

		if(i == (cycles - 1))
			{
			if(0 != align_bytes)
				{
//				printf("SF: Write align addr=%08X, datap=%08X, lenth=%d\n",addr + pa->page_size * i, (u32)(buf + pa->page_size * i), align_bytes);
				ret_val |= spi_cmd_program_pages((addr + pa->page_size * i), (buf + pa->page_size * i), align_bytes);
				}
			else
				break;
			}
		else
			{
//			printf("SF: Write page addr=%08X, datap=%08X, lenth=%d\n",addr + pa->page_size * i, (u32)(buf + pa->page_size * i), pa->page_size);
			ret_val |= spi_cmd_program_pages((addr + pa->page_size * i), (buf + pa->page_size * i), pa->page_size);
			}
		}
	return ret_val;
}
#endif

/* Adapter TBS bootloader */

unsigned long flash_init (void)
{
	flash_info_t *info;
	u8 id[3] = {0};
	int i;

	/* enable SSC1 */
    //*DANUBE_PMU_PM_GEN |= DANUBE_PMU_PM_GEN_EN11;
    /* SSC1 Ports */
    /* p0.15 SPI_CS1(EEPROM), P0.13 SPI_CS3, P0.9 SPI_CS5, P0.10 SPI_CS4, P0.11 SPI_CS6 */
    /* Set p0.10 to alternative 01, others to 00 (In/OUT)*/
    *(BSP_GPIO_P0_DIR) = (*BSP_GPIO_P0_DIR)|(0xAE00);
    *(BSP_GPIO_P0_ALTSEL0) = (((*BSP_GPIO_P0_ALTSEL0)&(~0x0400)) & (~(0xAA00)));
    *(BSP_GPIO_P0_ALTSEL1) = (((*BSP_GPIO_P0_ALTSEL1)|(0x0400)) & (~(0xAA00)) );
    *(BSP_GPIO_P0_OD) = (*BSP_GPIO_P0_OD)|0xAE00;
    /* p1.6 SPI_CS2(SFLASH), p1.0 SPI_DIN, p1.1 SPI_DOUT, p1.2 SPI_CLK */
    *(BSP_GPIO_P1_DIR) = ((*BSP_GPIO_P1_DIR)|(0x46))&(~1);
    *(BSP_GPIO_P1_ALTSEL0) = ((*BSP_GPIO_P1_ALTSEL0)|(0x47));
    *(BSP_GPIO_P1_ALTSEL1) = (*BSP_GPIO_P1_ALTSEL1)&(~0x47);
    *(BSP_GPIO_P1_OD) = (*BSP_GPIO_P1_OD)|0x0046;
	
    /* Clock Control Register */ /* DISS OFF and RMC = 1 */ //CLC
	*SPI_CLC = 0x0100;
	/* Disable SSC to get access to the control bits */
	*SPI_WHBSTATE = 0x1;
	asm("SYNC");
	/*set chip select pin*/
	*SPI_GPOCON =1<<(CONFIG_ENV_SPI_CS+8);
    *SPI_BRT = (((F_SSC_CLK>>1) + CONFIG_ENV_SPI_MAX_HZ/2)/(CONFIG_ENV_SPI_MAX_HZ) - 1) & 0xffff;
    /*enable and flush RX/TX FIFO*/
    *SPI_RXFCON = 0xF03;
	*SPI_TXFCON = 0xF03;
	/* set CON, TX off , RX off, ENBV=0, BM=7(8 bit valid) HB=1(MSB first), PO=0,PH=1(SPI Mode 0)*/
	*SPI_CON = 0x00070033;
	asm("SYNC");
	/*Set Master mode and  Enable SSC */
    *SPI_WHBSTATE = 0xA;
    asm("SYNC");
	
	info = &flash_info[0];
	/* Read the ID codes */
	get_flash_id(id);


	for (i=0;i<=(sizeof(flash_table)/sizeof(flash_table[0]));i++)
		{
		if(i ==(sizeof(flash_table)/sizeof(flash_table[0])))
			{
			printf("Flash type not supported ID=0x%02x%02x%02x\n", id[0], id[1], id[2]);
			info->size = 0;
			info->sector_count = 0;
			info->flash_id = FLASH_UNKNOWN;
			goto err;
			}
		else
			{
			pa = &flash_table[i];
			if((pa->manufacturer_id == id[0]) && (pa->memory_id == id[1]) && (pa->devive_id == id[2]))
				{
				info->flash_id = ((id[0])<<16) + ((id[1])<<8) + id[2];
				info->sector_count = pa->nr_blocks; /* align to 64K bytes */
				info->size = pa->block_size * pa->nr_blocks;
				printf("Found flash chip %s with %d sectors\n", pa->name, info->sector_count);
				break;
				}
			}
		}

	for (i=0; i < info->sector_count; i++)
		{
		info->start[i] = pa->block_size * i + CONFIG_FLASH_BASE;
		info->protect[i] = info->start[i];
		}
			
err:
	return info->size;
}


int flash_read_buf(flash_info_t *info, void *addr, unsigned char **buf, int len)
{
	u32 offset;
	u8 *buffer;
	
	if(len > (CONFIG_FLASH_BASE + info->size - (ulong )addr))
	{
		printf("Can NOT read buffers, length is out of flash space.\n");
		return 0;
	}

	buffer = *buf;
	offset = (u32)addr;
	spi_cmd_read_bytes(offset, buffer, len);
	
	return len;	
}

flash_info_t * addr2info (ulong addr)
{
	int i;

    for (i=0, info=&flash_info[0]; i<CFG_MAX_FLASH_BANKS; ++i, ++info)
        {
        if ((info->flash_id != FLASH_UNKNOWN) && (addr >= info->start[0]) &&
        /* WARNING - The '- 1' is needed if the flash is at the end of the address space, 
        since info->start[0] + info->size wraps back to 0. don't change this. */
            (addr <= (info->start[0] + info->size - 1)))
			{
            return (info);
        	}
        }
	
    return (NULL);
}

int flash_erase (flash_info_t *info, int s_first, int s_last)
{
	int i = 0;
	u32 addr= 0;
	int ret_val = ERR_OK;

	if ((s_first < 0) || (s_first >= CFG_MAX_FLASH_SECT) || 
		(s_last < 0) || (s_last >= CFG_MAX_FLASH_SECT))
		{
		printf("SF: Sector number [start]=%d [end]=%d out of range, erasing abort!\n", s_first, s_last);
		ret_val |= ERR_INVAL;
		goto err;
		}

	if(s_first != s_last)
		{
		s_first = min(s_first, s_last);
		s_last = max(s_first, s_last);
		}
	
	if((s_last - s_first + 1) == pa->nr_blocks)
		{
		printf("SF: Erasing whole chip at 0x%X, do NOT interrupt...", info->start[s_first]);
		ret_val |= spi_cmd_erase(CMD_MX25L_CE, addr);
		if((ret_val & ERR_TIMOUT) == ERR_TIMOUT)
			{
			printf("SF: Erasing operation timed out, force quit!\n", i);
			goto err;
			}		
		}
	else
		
		{
		for(i=s_first; i<=s_last; i++)
			{
			if (i == s_first) 
				printf("SF: Erasing %d blocks at 0x%X, do NOT interrupt...", 
					(s_last - s_first + 1), info->start[s_first]);
			addr = pa->block_size * (i - 1) + CONFIG_FLASH_BASE;

			ret_val |= spi_cmd_erase(CMD_MX25L_BE, addr);
			if((ret_val & ERR_TIMOUT) == ERR_TIMOUT)
				{
				printf("SF: Erasing operation at block %d timed out, force quit!\n", i);
				if(i == s_first)
					ret_val |= ERR_NOT_ERASED;
				goto err;
				}
			else 
				{
				if(((i-s_first) % 64) == 0)
					printf("\n\t");
				printf(".");
				}
			}
		}
	printf("\n\tDone!\n");

err:
	return ret_val;
}

#if 1
int flash_write (char *src, ulong addr, ulong len)
{
	int cycles = 0;
	int i;
	int ret_val = ERR_OK;
	size_t align_bytes;

	printf("Data write to %08X, length=%lu\n", addr, len);
	if((addr < CONFIG_FLASH_BASE) || (addr - CONFIG_FLASH_BASE) > info->size)
		{
		printf("\nSF: Destination write address out of range, writing abort!\n");
		ret_val |= ERR_INVAL;
		goto err;
		}
	align_bytes = (addr - CONFIG_FLASH_BASE) % pa->page_size;
	if(0 != align_bytes)
		{
		printf("Align first head page!\n");
		align_bytes = pa->page_size - align_bytes;
		align_bytes = min(align_bytes, len);
		ret_val |= spi_cmd_program_pages(addr, src, align_bytes);
		}
	
	if((len - align_bytes) > 0)
		{
		cycles = (len - align_bytes)/ pa->page_size;
		align_bytes = (len - align_bytes) % pa->page_size;

		if(0 != align_bytes)
			cycles += 1;

		for(i=0; i<=cycles; i++)
			{
			if((ret_val & ERR_TIMOUT) == ERR_TIMOUT)
				break;

			if(i == cycles)
				{
				if(0 != align_bytes)
					{
//					printf("SF: Write align addr=%08X, datap=%08X, lenth=%d\n",addr + pa->page_size * i, (u32)(buf + pa->page_size * i), align_bytes);
					ret_val |= spi_cmd_program_pages((addr + pa->page_size * i), (src + pa->page_size * i), align_bytes);
					}
				else
					break;
				}
			else
				{
//				printf("SF: Write page addr=%08X, datap=%08X, lenth=%d\n",addr + pa->page_size * i, (u32)(buf + pa->page_size * i), pa->page_size);
				ret_val |= spi_cmd_program_pages((addr + pa->page_size * i), (src + pa->page_size * i), pa->page_size);
				}
			if((i % 256) == 0)
				printf("\n\t");
			else if ((i % 4) == 0)
				printf("#");
		
			}
		}

	if(cycles != i)
		printf("SF: Finished %d of %d cycles, ret=0x%03X\n", i, cycles, ret_val);

err:
	if((ret_val & ERR_NO_MEM) == ERR_NO_MEM)
		printf("SF:Can not allocate memory!\n");
	
	return ret_val;
}

#else
int flash_write (char *src, ulong addr, ulong len)
{
	flash_info_t *info = &flash_info[0];
    u8 *buf_start = NULL;
	u8 *buf_end = NULL;
	int blocks;
	volatile int count = 0;
	int cycles = 0;
	int start_buf_len = 0;
	int end_buf_len = 0;
	int ret_val = ERR_OK;
	int s_first;

	printf("Data write to %08X, length=%lu\n", addr, len);
	if((addr < CONFIG_FLASH_BASE) || (addr - CONFIG_FLASH_BASE) > info->size)
		{
		printf("\nSF: Destination write address out of range, writing abort!\n");
		ret_val |= ERR_INVAL;
		goto err;
		}
	start_buf_len = (addr - CONFIG_FLASH_BASE) % pa->block_size;
	if(start_buf_len != 0) /*  */
		{
		ret_val |= ERR_ALIGN_START;
		debug("\nSF: Align the head address\n");
		buf_start = (u8 *)malloc(start_buf_len);
		if (NULL == buf_start)
			{
			ret_val |= ERR_NO_MEM;
			goto err;
			}
		else /* Reading start_align data to buffer */
			spi_cmd_read_bytes((addr - start_buf_len), buf_start, start_buf_len);
		}
	
	blocks = (start_buf_len + len) / pa->block_size;
	end_buf_len = (addr + len - CONFIG_FLASH_BASE) % pa->block_size;
	if(end_buf_len != 0) /*  */
		{
		ret_val |= ERR_ALIGN_END;
		debug("\nSF: Align the ending address\n");
		end_buf_len = pa->block_size - end_buf_len;
		blocks += 1;
		buf_end = (u8 *)malloc(end_buf_len);
		if(NULL == buf_end)
			{
			ret_val |= ERR_NO_MEM;
			goto err;
			}
		else /* Reading end_align data to buffer */
			spi_cmd_read_bytes((addr + len), buf_end, end_buf_len);
		}

	s_first = (addr - CONFIG_FLASH_BASE) / pa->block_size;
	debug("SF: Erasing blocks\n");
	ret_val |= flash_erase(info, s_first, s_first + blocks - 1);
	if(((ret_val & ERR_NOT_ERASED) == ERR_NOT_ERASED) ||
	   ((ret_val & ERR_INVAL) == ERR_INVAL))
		goto err;


	if(0 != start_buf_len)
		{
		ret_val |= program_pages(addr - start_buf_len, buf_start, &count, start_buf_len);
		printf("SF: Aligned head addr=[0x%08X] count=[%d] start_buf_len=[%d]\n", addr - start_buf_len, count, start_buf_len);
		cycles += (start_buf_len / pa->page_size);
		if(0 != (start_buf_len % pa->page_size))
			cycles += 1;
		}
	
	ret_val |= program_pages(addr, src, &count, len);
	cycles = len / pa->page_size;		
	if(0 != (len % pa->page_size))
		cycles += 1;
	printf("SF: count=%d\n", count);
	if(0 != end_buf_len)
		{
		ret_val |= program_pages(addr + len, buf_end, &count, end_buf_len);
		printf("SF: Aligned end addr=[0x%08X] count=[%d] end_buf_len=[%d]\n", addr + len, count, end_buf_len);
		cycles += (end_buf_len / pa->page_size);
		if (0 != (end_buf_len % pa->page_size))
			cycles += 1;
		}
	if(cycles != count)
		printf("SF: Finished %d of %d cycles, ret=%03X\n", count, cycles, ret_val);

err:
	if((ret_val & ERR_NO_MEM) == ERR_NO_MEM)
		printf("SF:Can not allocate memory!\n");
	if((NULL != buf_start) && ((ret_val & ERR_ALIGN_START) == ERR_ALIGN_START))
		free((void *)buf_start);
	if((NULL != buf_end) && ((ret_val & ERR_ALIGN_END) == ERR_ALIGN_END))
		free((void *)buf_end);
	return ret_val;
}
#endif

void flash_print_info (flash_info_t *info)
{
	int i;

	printf("Spi flash ID=0x%x Size=%ldMB Sectors=%d\n", info->flash_id, info->size >> 20, info->sector_count);
    puts("Sector Start Addresses:");

    for (i = 0; i < info->sector_count; ++i)
        {
         if ((i % 5) == 0)
         	printf ("\n   ");
         printf (" %08lX%s", info->start[i], info->protect[i] ? " (RO)" : "     ");
        }
    putc ('\n');
}
#endif
