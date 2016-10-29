#include <common.h>
#include "ar7100_flash.h"
#include <malloc.h>


flash_info_t flash_info[CFG_MAX_FLASH_BANKS];           /* FLASH chips info */
flash_info_t * info;

//----------------------------------------------------------------------------
// Functions that put the flash device into non-read mode must reside
// in RAM.
void flash_query(void* data) __attribute__ ((section (".2ram.flash_query")));
int  flash_erase_block(void* block, unsigned int size)
    __attribute__ ((section (".2ram.flash_erase_block")));
int  flash_program_buf(void* addr, void* data, int len)
    __attribute__ ((section (".2ram.flash_program_buf")));


/*
 * statics
 */
static void ar7100_spi_write_enable(void)
      __attribute__((section (".2ram.ar7100_spi_write_enable")));
static void ar7100_spi_poll(void)
      __attribute__((section (".2ram.ar7100_spi_poll")));
static void ar7100_spi_write_page(unsigned int addr, unsigned char *data, int len)
      __attribute__((section (".2ram.ar7100_spi_write_page")));
static void ar7100_spi_sector_erase(unsigned int addr)
      __attribute__((section (".2ram.ar7100_spi_sector_erase")));


void read_id(spi_flash_id * flash_id)
{
    unsigned int  rd = 0x0;

    ar7100_reg_wr_nf(AR7100_SPI_FS, 1);                  
    ar7100_reg_wr_nf(AR7100_SPI_WRITE, AR7100_SPI_CS_DIS);
    ar7100_spi_go();
    ar7100_spi_bit_banger(0x9f);

    ar7100_spi_delay_8();
    rd = ar7100_reg_rd(AR7100_SPI_RD_STATUS);
    flash_id->device_id  = (rd & 0xff) << 16;

    ar7100_spi_delay_8();
    rd = ar7100_reg_rd(AR7100_SPI_RD_STATUS);
    flash_id->device_id |= (rd & 0xff) << 8;

    ar7100_spi_delay_8();
    rd = ar7100_reg_rd(AR7100_SPI_RD_STATUS);
    flash_id->device_id |= (rd & 0xff) << 0;

    ar7100_spi_delay_8();
    rd = ar7100_reg_rd(AR7100_SPI_RD_STATUS);
    flash_id->ext_device_id  = (rd & 0xff) << 8;

    ar7100_spi_delay_8();
    rd = ar7100_reg_rd(AR7100_SPI_RD_STATUS);
    flash_id->ext_device_id |= (rd & 0xff) << 0;

    ar7100_spi_done();
}

//----------------------------------------------------------------------------
// Program Buffer
// 0. Assumption: Caller has already erased the appropriate sectors.
// 1. call page programming for every 256 bytes

int
flash_program_buf(void* addr, void* data, int len)
{
    int total = 0, len_this_lp, bytes_this_page;
    unsigned long dst;
    unsigned char *src;
    unsigned long uladdr = (unsigned long)addr - CONFIG_FLASH_BASE;

    while(total < len) {
        src              = (unsigned char*)data + total;
        dst              = uladdr + total;
        bytes_this_page  = AR7100_SPI_PAGE_SIZE - ((unsigned long)dst % AR7100_SPI_PAGE_SIZE);
        len_this_lp      = ((len - total) > bytes_this_page) ? bytes_this_page
                                                             : (len - total);
	//printf("src=0x%x,dst=0x%x,len_this_lp=0x%x,bytes_this_page=0x%x\n",src,dst,len_this_lp,bytes_this_page);

        ar7100_spi_write_page(dst, src, len_this_lp);
        total += len_this_lp;
    }

    ar7100_spi_done();

    return 0;
}


//----------------------------------------------------------------------------
// read data from flash to Buffer,by Zhangyu
//
// return: the count of bytes read.

int flash_read_buf(flash_info_t * info, void* addr, unsigned char ** buf, int len)
{
	unsigned int  rd = 0x0;
	int i;
	unsigned char *buf_tmp = *buf;

	unsigned int real_addr = (unsigned long)addr - CONFIG_FLASH_BASE;
	
	if( len  > (unsigned long)(info->start[info->sector_count  -1] + info->size / info->sector_count) - (unsigned long)addr)
	{
		printf("The length is out of flash space.\n");
		return 0;
	}
	
	ar7100_reg_wr_nf(AR7100_SPI_FS, 1);              
	ar7100_reg_wr_nf(AR7100_SPI_WRITE, AR7100_SPI_CS_DIS);
	//ar7100_reg_wr_nf(AR7100_SPI_CLOCK, 0x41);		/* switch clock to high spped model */

	ar7100_spi_go();
	ar7100_spi_bit_banger(AR7100_SPI_CMD_FAST_READ);
	
	ar7100_spi_send_addr(real_addr);
	ar7100_spi_delay_8();

	for(i=0;i<len;i++)
	{
		ar7100_spi_delay_8();
		rd = ar7100_reg_rd(AR7100_SPI_RD_STATUS);
		//printf("0x%x\t",rd);
		*buf_tmp= (unsigned char)(rd & 0xff);
		buf_tmp++;
	}
	
	ar7100_spi_done();

	//ar7100_reg_wr_nf(AR7100_SPI_CLOCK, 0x43);		/* switch clock to normal model */
	return i;
	
}


static void
ar7100_spi_write_enable()  
{
    ar7100_reg_wr_nf(AR7100_SPI_FS, 1);                  
    ar7100_reg_wr_nf(AR7100_SPI_WRITE, AR7100_SPI_CS_DIS);     
    ar7100_spi_bit_banger(AR7100_SPI_CMD_WREN);             
    ar7100_spi_go();
}

static void
ar7100_spi_poll()   
{
    int rd;                                                 

    do {
        ar7100_reg_wr_nf(AR7100_SPI_WRITE, AR7100_SPI_CS_DIS);     
        ar7100_spi_bit_banger(AR7100_SPI_CMD_RD_STATUS);        
        ar7100_spi_delay_8();
        rd = (ar7100_reg_rd(AR7100_SPI_RD_STATUS) & 1);               
    }while(rd);
}

static void
ar7100_spi_write_page(unsigned int addr, unsigned char *data, int len)
{
    int i;
    unsigned char ch;

    ar7100_spi_write_enable();
    ar7100_spi_bit_banger(AR7100_SPI_CMD_PAGE_PROG);
    ar7100_spi_send_addr(addr);

    for(i = 0; i < len; i++) {
        ch = *(data + i);
        ar7100_spi_bit_banger(ch);
    }

    ar7100_spi_go();
    ar7100_spi_poll();
}

static void
ar7100_spi_sector_erase(unsigned int addr)
{
    ar7100_spi_write_enable();
    ar7100_spi_bit_banger(AR7100_SPI_CMD_SECTOR_ERASE);
    ar7100_spi_send_addr(addr);
    ar7100_spi_go();
    ar7100_spi_poll();
}



/* adapter for TBS bootloader */

unsigned long flash_init (void)
{
	spi_flash_id  flash_id;
	int i;
		
	read_id(&flash_id);
	
	if(flash_id.device_id == 0x12018)			/*spansion spi flash:S25FL128P */
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
	else if(flash_id.device_id == 0xc22018)			/*Mx spi flash:MX25L128AD */
	{
		flash_info[0].size = 0x10000 * 256;
		flash_info[0].sector_count = 256;
		flash_info[0].flash_id = flash_id.device_id;
		printf("Found flash MX25L12805d:16M size,64K sector!\n");
	}
	else if(flash_id.device_id == 0x202018)			/*Mx spi flash:M25P128 */
	{
		flash_info[0].size = 0x40000 * 64;
		flash_info[0].sector_count = 64;
		flash_info[0].flash_id = flash_id.device_id;
		printf("Found flash M25P128:16M size,256K sector!\n");
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
			unsigned char *p;
			unsigned int i,j,tmp,addr_tmp,src_tmp;
		
			addr_tmp=addr;
			src_tmp = ( unsigned int ) src;
			for(i=0;i<(cnt>>10);i++)
			{
				p=malloc(1024);
				flash_read_buf( info , src , &p , 1024 );
				if ( ( j = flash_program_buf( ( unsigned char *) addr , p , 1024 ) ) != 0) 
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
				flash_read_buf( info , src ,  &p , tmp );
				if ((j = flash_program_buf( ( unsigned char * ) addr , p , tmp )) != 0) 
				{
					return (j);
				}
				free(p);
			}

			addr = addr_tmp;
			src = (char *) src_tmp;
		}
		else
		{
			if ( ( i = flash_program_buf( ( unsigned char * ) addr , src , cnt ) ) != 0) 
			{
				return (i);
			}
		}
		
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
	return;
	
}

int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	int i = s_first;

	do 
	{
		printf(".");
		ar7100_spi_sector_erase( info->start[i] - CONFIG_FLASH_BASE);
	}while (i++ < s_last);

	ar7100_spi_done();

	printf("done\n", i);

	return 0;
}

