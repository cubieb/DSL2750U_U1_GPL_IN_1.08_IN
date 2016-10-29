/*==========================================================================
#####DESCRIPTIONBEGIN####
## Author(s):    xuanguanglei
## Contributors:
## Date:         2008-6-5
## Purpose:
## Description:  AR2317 SPI flash driver
==========================================================================*/

#ifndef CYGONCE_DEVS_FLASH_AR7100_INL
#define CYGONCE_DEVS_FLASH_AR7100_INL

#include <config.h>
#include <common.h>
#include <flash_layout_private.h>
#include <malloc.h>
#include "ar2316reg.h"
#include "ar2317_spiflash.h"

#define HAL_WRITE_UINT32( _register_, _value_ ) \
        (*((volatile unsigned int *)(_register_)) = (_value_))
#define HAL_READ_UINT32( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned int  *)(_register_)))

typedef struct {
    unsigned int device_id;
    unsigned short int ext_device_id;
}spi_flash_id;


flash_info_t flash_info[CFG_MAX_FLASH_BANKS];           /* FLASH chips info */
flash_info_t * info;

int flash_erase_block(unsigned int  *block);
int flash_read_buf(flash_info_t * info, void* addr, unsigned char * buf, int len);
unsigned int sfi_Command(unsigned int opcode, int write_len, int read_len);
unsigned long flash_init (void);
flash_info_t * addr2info (ulong addr);
int flash_write (char *src, ulong addr, ulong cnt);
void flash_print_info (flash_info_t * info);
int flash_erase (flash_info_t * info, int s_first, int s_last);
int flash_program_buf(volatile unsigned int *addr, unsigned int *buf, int len);

int flash_read_buf(flash_info_t * info, void* addr, unsigned char * buf, int len)
{
	if( len  > (unsigned long)(info->start[info->sector_count  -1] + info->size / info->sector_count) - (unsigned long)addr)
	{
		printf("The length is out of flash space.\n");
		return 0;
	}
	
	memcpy(buf, addr, len);
	return 1;	
}

unsigned int sfi_Command(unsigned int opcode, int write_len, int read_len)
{
	unsigned int reg, mask;

	do 
	{
		HAL_READ_UINT32(AR2316_SPI_CTL, reg);
	} while (reg & SPI_CTL_BUSY);

	HAL_WRITE_UINT32(AR2316_SPI_OPCODE, opcode);
	reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | (write_len<<0) | (read_len<<4) | SPI_CTL_START;

	HAL_WRITE_UINT32(AR2316_SPI_CTL,reg);
	if (read_len > 0) 
	{
		do 
		{
			HAL_READ_UINT32(AR2316_SPI_CTL, reg);
		} 
		while (reg & SPI_CTL_BUSY);
		
		HAL_READ_UINT32(AR2316_SPI_DATA, reg);

		switch (read_len) 
		{
			case 1:
				mask = 0x000000ff;
				break;
			case 2:
				mask = 0x0000ffff;
				break;
			case 3:
				mask = 0x00ffffff;
				break;
			default:
				mask = 0xffffffff;
				break;
		}
		reg &= mask;
	} 
	else 
	{
		reg = 0;
	}
	return reg;
}

/* adapter for TBS bootloader */

unsigned long flash_init (void)
{
	spi_flash_id  flash_id;
	int i;
	sys_config_t *  sys_data= ( sys_config_t * )SYSCFG_OFFSET;

	flash_id.device_id = sfi_Command(0x9f, 1, 3);

	if( flash_id.device_id == 0x1620c2 )		/*MX spi flash:25L3205AML */
	{
		flash_info[0].size = 0x10000 * 64;
		flash_info[0].sector_count = 64;
		flash_info[0].flash_id = flash_id.device_id;
		printf("Found flash 25L3205AML 64K block!\n");
	}
	else if( flash_id.device_id == 0x140201 )		/*spansion spi flash:S25FL016A */
	{
		flash_info[0].size = 0x10000 * 32;
		flash_info[0].sector_count = 32;
		flash_info[0].flash_id = flash_id.device_id;
		printf("Found flash S25FL016A 64K sector!\n");
	}
	else if(flash_id.device_id == 0x12018)			/*spansion spi flash:S25FL128P */
	{
		if(flash_id.ext_device_id == 0x300)		/* 256k byte sector */
		{
			flash_info[0].size = 0x40000 * 64;
			flash_info[0].sector_count = 64;
			flash_info[0].flash_id = flash_id.device_id;
			printf("Found flash 25FL128P 256K sector!\n");
		}
		else if(flash_id.ext_device_id == 0x301)	/* 64k byte sector */
		{
			flash_info[0].size = 0x10000 * 256;
			flash_info[0].sector_count = 256;
			flash_info[0].flash_id = flash_id.device_id;
			printf("Found flash 25FL128P 64K sector!\n");
		}
		else
		{
			flash_info[0].size = 0;
			flash_info[0].sector_count = 0;
			flash_info[0].flash_id = FLASH_UNKNOWN;
			printf("Unknow extend flash ID:0x%x.\n",flash_id.ext_device_id);
		}
	}
	else if(flash_id.device_id == 0x10216)			/*spansion spi flash:S25FL64A */
	{
		flash_info[0].size = 0x10000 * 128;
		flash_info[0].sector_count = 128;
		flash_info[0].flash_id = flash_id.device_id;
		printf("Found flash SPANSION S25FL64A:8M size,64K sector!\n");
	}
	else
	{
		flash_info[0].size = 0;
		flash_info[0].sector_count = 0;
		flash_info[0].flash_id = FLASH_UNKNOWN;
		printf("Unknow flash id 0x%x!\n",flash_id.device_id);
		return 0;
	}

	for(i=0;i < flash_info[0].sector_count;i++)
	{
		flash_info[0].start[i] = flash_info[0].size /flash_info[0].sector_count *  i  + CONFIG_FLASH_BASE;
		flash_info[0].protect[i] = flash_info[0].start[i];
	}

//	i = flash_read_buf(&flash_info[0],0xbf000000,0x80500000,0x100);

//	printf("read 0x%x bytes\n",i);
	info=&flash_info[0];

	if(flash_info[0].size)
	{
		sysdata_get(sys_data,&flash_info[0]);
	}

	return flash_info[0].size;
}

flash_info_t * addr2info (ulong addr)
{
	flash_info_t *info;
	int i;

	for (i=0, info=&flash_info[0]; i<CFG_MAX_FLASH_BANKS; ++i, ++info) 
	{
		if (info->flash_id != FLASH_UNKNOWN && addr >= info->start[0] &&
			/* WARNING - The '- 1' is needed if the flash
			  * is at the end of the address space, since
			  * info->start[0] + info->size wraps back to 0.
			  * Please don't change this unless you understand this.
			  */
			addr <= info->start[0] + info->size - 1) 
			{
				return (info);
			}
	}

	return (NULL);
}

/*-----------------------------------------------------------------------
 * Copy memory to flash.
 * Make sure all target addresses are within Flash bounds,
 * and no protected sectors are hit.
 * Returns:
 * ERR_OK          0 - OK
 * ERR_TIMOUT      1 - write timeout
 * ERR_NOT_ERASED  2 - Flash not erased
 * ERR_PROTECTED   4 - target range includes protected sectors
 * ERR_INVAL       8 - target address not in Flash memory
 * ERR_ALIGN       16 - target address not aligned on boundary
 *                      (only some targets require alignment)
 */
int flash_write (char *src, ulong addr, ulong cnt)
{

	int i;
	ulong         end        = addr + cnt - 1;
	flash_info_t *info_first = addr2info (addr);
	flash_info_t *info_last  = addr2info (end );
	flash_info_t *info;

	if (cnt == 0) 
	{
		return (ERR_OK);
	}

	if (!info_first || !info_last) 
	{
		return (ERR_INVAL);
	}

	for (info = info_first; info <= info_last; ++info) 
	{
		ulong b_end = info->start[0] + info->size;      /* bank end addr */
		short s_end = info->sector_count - 1;
		
		for (i=0; i<info->sector_count; ++i) 
		{
			ulong e_addr = (i == s_end) ? b_end : info->start[i + 1];

			if ((end >= info->start[i]) && (addr < e_addr) &&(info->protect[i] != 0) ) 
			{
				return (ERR_PROTECTED);
			}
		}
	}

	/* finally write data to flash */
	for (info = info_first; info <= info_last && cnt>0; ++info)
	{
		ulong len;
		len = info->start[0] + info->size - addr;

		if (len > cnt)
			len = cnt;
		/*-----------------------------------------------------------------------
		  * FUNCTION:flash_program_buf
		  * Copy memory to flash, returns:
		  * 0 - OK
		  * 1 - write timeout
		  * 2 - Flash not erased
		  */

		if( ( ( unsigned int ) src >= info_first->start[0] ) && ( ( unsigned int ) src <= info_first->start[0] + info_first->size ) )
		{
			unsigned int *p=NULL;
			unsigned int i,j,tmp,addr_tmp,src_tmp;
		
			addr_tmp=addr;
			src_tmp = ( unsigned int ) src;
			for(i=0;i<(cnt>>10);i++)
			{
				p=malloc(1024);
				flash_read_buf( info , src , ( unsigned char *) p , 1024 );
				if ( ( j = flash_program_buf( ( unsigned char *) addr , ( unsigned char *) p , 1024 ) ) != 0) 
				{
					return (j);
				}				
				src+=1024;
				addr+=1024;
				free(p);
			}

			if(len%1024!=0)
			{
				tmp=len%1024;
				p=malloc(tmp);
				flash_read_buf(info,src,p,tmp);
				if ((j = flash_program_buf( addr , p , tmp )) != 0) 
				{
					return (j);
				}
				free(p);
			}

			addr=addr_tmp;
			src=src_tmp;
		}
		else
		{
			if ((i = flash_program_buf(addr,src,cnt)) != 0) 
			{
				return (i);
			}
		}
#if 0
		if ((i = flash_program_buf(addr,src,cnt)) != 0) 
		{
			return (i);
		}
#endif
		
		cnt  -= len;
		addr += len;
		src  += len;
        }
        return (ERR_OK);
}

void flash_print_info (flash_info_t * info)
{
	int i;

	printf ("  Spi flash ID: 0x%x\n", info->flash_id);
	printf ("  Size: %ld MB in %d Sectors\n",
		info->size >> 20, info->sector_count);
	puts ("  Sector Start Addresses:");
	
	for (i = 0; i < info->sector_count; ++i) 
	{
		if ((i % 5) == 0)
				printf ("\n   ");
		printf (" %08lX%s",
		info->start[i], info->protect[i] ? " (RO)" : "     ");

	}
	
	putc ('\n');
}

int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	int i = s_first;

	do 
	{
		printf(".");
		flash_erase_block( info->start[i] );
	}while (i++ < s_last);

	printf("done\n", i);

	return 0;
}

int flash_erase_block(unsigned int  *block)
{
	unsigned int  res;
	unsigned int offset = (unsigned int)block -  CONFIG_FLASH_BASE;

	sfi_Command(STM_OP_WR_ENABLE, 1, 0);
	sfi_Command(STM_OP_SECTOR_ERASE | (offset << 8), 4, 0);
	while (1) 
	{
		res = sfi_Command(STM_OP_RD_STATUS, 1, 1);
		if ((res & STM_STATUS_WIP) == 0) 
		{
			break;
		}
	}
	return 0;
}

int flash_program_buf(volatile unsigned int *addr, unsigned int *buf, int len)
{
	unsigned int  res;
	unsigned int  offset = (unsigned int )addr -  CONFIG_FLASH_BASE;
	unsigned int  data = 0;
	int data_len;
	unsigned char *cb = (unsigned char *)buf;

	while (len > 0) 
	{
		sfi_Command(STM_OP_WR_ENABLE, 1, 0);
		data_len = len & 0x03;
		if (data_len == 0) 
			data_len = 4;
		switch (data_len) 
		{
			case 1:
				data = *cb;
				break;
			case 2:
				data = (cb[1] << 8) | cb[0];
				break;
			case 3:
				data = (cb[2] << 16) | (cb[1] << 8) | cb[0];
				break;
			case 4:
				data = (cb[3] << 24) | (cb[2] << 16) | (cb[1] << 8) | cb[0];
				break;
		}
		HAL_WRITE_UINT32(AR2316_SPI_DATA, data);
		sfi_Command(STM_OP_PAGE_PGRM | (offset << 8), data_len+4, 0);
		while (1) 
		{
			res = sfi_Command(STM_OP_RD_STATUS, 1, 1);
			if ((res & STM_STATUS_WIP) == 0) 
			{
				break;
			}
		}
		offset += data_len;
		cb += data_len;
		len -= data_len;
	}
	return 0;
}

#endif // CYGONCE_DEVS_FLASH_AR7100_INL

