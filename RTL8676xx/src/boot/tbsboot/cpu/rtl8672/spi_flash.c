#include <common.h>
#include <malloc.h>
#include "spi_flash.h"

#ifdef SPI_FLASH

flash_info_t flash_info[CFG_MAX_FLASH_BANKS];           /* FLASH chips info */
flash_info_t * info;

//tylo, for ic ver. detect
unsigned char ICver=0;

/* SPI Flash Controller */
unsigned int SFCR=0;
unsigned int SFCSR=0;
unsigned int SFDR=0;


static void spi_ready(void)
{
	while (1)
	{
		if ( (*(volatile unsigned int *) SFCSR) & READY(1))
		break;
	}
}


/*
 * This function shall be called when switching from MMIO to PIO mode
 */
static void spi_pio_init(void)
{
	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);

	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);

	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);

	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);
}


static void checkICver(void)
{
	unsigned int sccr;

	sccr=*(volatile unsigned int*)SCCR;
	if((sccr & 0x00100000) == 0)
	{
		ICver = IC8671B_costdown;
	}
	else
	{
		ICver = IC8671B;
	}	

	//set SPI related register
	if(ICver == IC8671B)
	{
		SFCR = 0xB8001200;
		SFCSR= 0xB8001204;
		SFDR = 0xB8001208;
	}
	else if(ICver == IC8671B_costdown)
	{ // 8671B costdown
		SFCR = 0xB8001200;
		SFCSR = 0xB8001208;
		SFDR = 0xB800120C;
	//move to loader.c, ethernet_patch() -shlee		*(volatile unsigned int *) 0xb8003304 = 0x00000008;
	}
	else
	{
		printf("can not detect spi controller!");
	}
}

static void read_id(spi_flash_id * flash_id)
{
	unsigned int temp;
	int cnt =0;

	checkICver();

	*(volatile unsigned int *) SFCR =*(volatile unsigned int *) SFCR |SPI_CLK_DIV(1);//
	spi_pio_init();      


	/* Here set the default setting */
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
	//printf("\n");
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+cnt) | READY(1);

	/* One More Toggle (May not Necessary) */
	// *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
	// *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+cnt) | READY(1);

	/* RDID Command */
	*(volatile unsigned int *) SFDR = 0x9F << 24;
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1+cnt) | READY(1);
	temp = *(volatile unsigned int *) SFDR;

	flash_id->device_id = (temp >> 8) & 0xFFFFFF;
	flash_id->ext_device_id= 0;
	//spi_flash_info[cnt].maker_id = (temp >> 24) & 0xFF;
	//spi_flash_info[cnt].type_id = (temp >> 16) & 0xFF;
	//spi_flash_info[cnt].capacity_id = (temp >> 8) & 0xFF;

	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);

	spi_ready();	  
}

static void spi_erase_sector(int sector)
{
	int chip=0;

	spi_pio_init();

	/* WREN Command */
	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);

	*(volatile unsigned int *) SFDR = 0x06 << 24;
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);

	/* SE Command */
	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1+chip) | READY(1);
	*(volatile unsigned int *) SFDR = (0xD8 << 24) | (sector * 65536);
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);

	/* RDSR Command */
	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);
	*(volatile unsigned int *) SFDR = 0x05 << 24;

	while (1)
	{
		/* RDSR Command */
		if ( ((*(volatile unsigned int *) SFDR) & 0x01000000) == 0x00000000)
		{
			break;
		}
	}

	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
}

static void spi_unprotect_block_other(unsigned int chip)
{
	spi_pio_init();

	/* De-select Chip */
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);

	/* RDSR Command */
	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);
	*(volatile unsigned int *) SFDR = 0x05 << 24;

	while (1)
	{
		unsigned int status;
		status = *(volatile unsigned int *) SFDR;

		/* RDSR Command */
		if ( (status & 0x01000000) == 0x00000000)
		{
			//ql: if block protected, then write status register 0
			if (status & (0x1C<<24))
			{
				*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);

				/* WREN Command */
				spi_ready();
				*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);
				*(volatile unsigned int *) SFDR = 0x06 << 24;
				*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
				/*EWSR Command*/
				spi_ready();
				*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);
				*(volatile unsigned int *) SFDR = 0x50 << 24;
				*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);

				spi_ready();
				//WRSR command
				*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);
				*(volatile unsigned int *) SFDR = (0x01 << 24);
				*(volatile unsigned int *) SFDR = 0;
			}

			break;
		}
	}

	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
}

static void rtl8672_spi_write_page(unsigned int addr, unsigned char *data, int len)
{
	unsigned char *cur_addr;
	int cur_size;
	int chip;
	int k;
	unsigned int temp;
	unsigned int cnt;

	chip = RTL8672_FLASH_CHIP;
	cur_addr = data;
	cur_size = len;

	spi_unprotect_block_other(chip);

	/* WREN Command */
	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);
	*(volatile unsigned int *) SFDR = 0x06 << 24;
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);

	/* PP Command */
	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1+chip) | READY(1);
	*(volatile unsigned int *) SFDR = (0x02 << 24) | addr;

	for (k = 0; k < 64; k++)
	{
		temp = (*(cur_addr)) << 24 | (*(cur_addr + 1)) << 16 | (*(cur_addr + 2)) << 8 | (*(cur_addr + 3));

		spi_ready();
		if (cur_size >= 4)
		{
			*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1+chip) | READY(1);
			cur_size -= 4;
		}
		else
		{
			*(volatile unsigned int *) SFCSR = LENGTH(cur_size-1) | CS(1+chip) | READY(1);
			cur_size = 0;
		}

		*(volatile unsigned int *) SFDR = temp;

		cur_addr += 4;

		if (cur_size == 0)
			break;
	}

	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);

	/* RDSR Command */
	spi_ready();
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);
	*(volatile unsigned int *) SFDR = 0x05 << 24;

	cnt = 0;
	while (1)
	{
		unsigned int status = *(volatile unsigned int *) SFDR;

		/* RDSR Command */
		if ((status & 0x01000000) == 0x00000000)
		{
			break;
		}

		if (cnt > 200000)
		{
			printf("\nBusy Loop for RSDR: %d, Address at 0x%08X\n", status, addr);
			busy:
			goto busy;
		}
		cnt++;
	}
	
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);

}

#if 0
int spi_program_image(unsigned int chip,unsigned long flash_address ,unsigned char *image_addr, unsigned int image_size)
{
	unsigned int temp;
	unsigned int i, j, k;
	unsigned char *cur_addr;
	unsigned int cur_size;
	unsigned int cnt;
	unsigned int sect_cnt;
	unsigned long uladdr;

	flash_address -= CONFIG_FLASH_BASE;
	cur_addr = image_addr;
	cur_size = image_size;

	if (flash_address & ((1 << 16) -1))
	{
		printf("\r\nflash_address must be 64KB aligned!!");
		return 1;
	}

	printf("spi_program_image :flash_address :%x  image_addr:%x, image_size :%d",
		flash_address, image_addr, image_size);


	sect_cnt = flash_address >> 16; //flash_address must be 64KB aligned
	spi_unprotect_block_other(chip);
	/* Iterate Each Sector */
	for (i = sect_cnt; i < info->sector_count; i++)
	{
		unsigned int spi_data;
		printf(".");

		/* Iterate Each Page,  64 KB = 256(page size)*256(page number) */
		for (j = 0; j < 256; j++)
		{
			if (cur_size == 0)
				break;

			/* WREN Command */
			spi_ready();
			*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);
			*(volatile unsigned int *) SFDR = 0x06 << 24;
			*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);

			/* PP Command */
			spi_ready();
			*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1+chip) | READY(1);
			*(volatile unsigned int *) SFDR = (0x02 << 24) | (i * 65536) | (j * 256);

			for (k = 0; k < 64; k++)
			{
				temp = (*(cur_addr)) << 24 | (*(cur_addr + 1)) << 16 | (*(cur_addr + 2)) << 8 | (*(cur_addr + 3));

				spi_ready();
				if (cur_size >= 4)
				{
					*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1+chip) | READY(1);
					cur_size -= 4;
				}
				else
				{
					*(volatile unsigned int *) SFCSR = LENGTH(cur_size-1) | CS(1+chip) | READY(1);
					cur_size = 0;
				}

				*(volatile unsigned int *) SFDR = temp;

				cur_addr += 4;

				if (cur_size == 0)
					break;
			}

			*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);

			/* RDSR Command */
			spi_ready();
			*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1+chip) | READY(1);
			*(volatile unsigned int *) SFDR = 0x05 << 24;

			cnt = 0;
			while (1)
			{
				unsigned int status = *(volatile unsigned int *) SFDR;

				/* RDSR Command */
				if ((status & 0x01000000) == 0x00000000)
				{
					break;
				}

				if (cnt > 200000)
				{
					printf("\nBusy Loop for RSDR: %d, Address at 0x%08X\n", status, i*65536+j*256);
					busy:
					goto busy;
				}
				cnt++;
			}
			*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);
		}

		if (cur_size == 0)
		break;
	} /* Iterate Each Sector */

	return 0;
}
#endif

//----------------------------------------------------------------------------
// Program Buffer
// 0. Assumption: Caller has already erased the appropriate sectors.
// 1. call page programming for every 256 bytes

static int flash_program_buf(void* addr, void* data, int len)
{
	int total = 0, len_this_lp, bytes_this_page;
	unsigned long dst;
	unsigned char *src;
	unsigned long uladdr = (unsigned long)addr - CONFIG_FLASH_BASE;

	while(total < len) 
	{
		src              = (unsigned char*)data + total;
		dst              = uladdr + total;
		bytes_this_page  = RTL8672_SPI_PAGE_SIZE - ((unsigned long)dst % RTL8672_SPI_PAGE_SIZE);
		len_this_lp      = ((len - total) > bytes_this_page) ? bytes_this_page: (len - total);

		debug("src=0x%x,dst=0x%x,len_this_lp=0x%x,bytes_this_page=0x%x\n",src,dst,len_this_lp,bytes_this_page);

		rtl8672_spi_write_page(dst, src, len_this_lp);
		total += len_this_lp;
	}

	return 0;
}

int flash_read_buf(flash_info_t * info, void* addr, unsigned char ** buf, int len)
{
	unsigned char *buf_tmp ;
	unsigned char *addr_tmp ;

	if( len  > ( CONFIG_FLASH_BASE + info->size  - ( ulong )addr ) )
	{
		printf("The length is out of flash space.\n");
		return 0;
	}

	buf_tmp = *buf;
	addr_tmp = addr;
	memcpy( buf_tmp , addr_tmp , len);

	return len;	
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
		spi_erase_sector(i);
	}while (i++ < s_last);

	printf("done\n");

	return 0;
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

typedef struct rtl8672_flash_type
{
    char *      pname;
    uint32_t    mtc_id;     //mtcid
    uint32_t    MB_size;    //MB
	uint32_t    KB_sec;  //KB
	uint32_t    n_secs;     //m
	uint32_t    B_pgsize;   //B
}rtl8672_flash_type_t;

static rtl8672_flash_type_t g_flash_table[]=
{
    {"AT26DF161"   ,0xEF4600, 2 , 64 , 32 , 256},
    {"AT26DF161A"  ,0xEF4601, 2 , 64 , 32 , 256},
    {"AT26DF161A"  ,0xEF4602, 2 , 64 , 32 , 256},
    {"A25L016"     ,0x373015, 2 , 64 , 32 , 256},
    {"EN25P16"     ,0x1C3015, 2 , 64 , 32 , 256},//ok
    {"EN25F16"     ,0x1C3115, 2 , 64 , 32 , 256},//ok
  //{"F25L016A"    ,0x8C2015, 2 , 64 , 32 , 256},//ESMT
  //{"F25L016A"    ,0x8C2115, 2 , 64 , 32 , 256},
    {"MX25L1605D"  ,0xc22015, 2 , 64 , 32 , 256},//ok
    {"MX25L1635D"  ,0xc22415, 2 , 64 , 32 , 256},//ok
    {"M25P16"      ,0x202015, 2 , 64 , 32 , 256},//numonyx
    {"W25X16"      ,0xEF3015, 2 , 64 , 32 , 256},//ok
    {"W25Q16"      ,0xEF4015, 2 , 64 , 32 , 256},//ok
  //{"QH25FL016"   ,0x898911, 2 , 64 , 32 , 256},//intel
    {"S25FL016A"   ,0x010214, 2 , 64 , 32 , 256},//ok
  //{"SST25VF016"  ,0xBF2541, 2 , 64 , 32 , 256},
    
    {"AT25DF321"   ,0x1F4700, 4 , 64 , 64 , 256},
    {"AT25DF321"   ,0x1F4701, 4 , 64 , 64 , 256},
    {"A25L032"     ,0x373016, 4 , 64 , 64 , 256},
    {"EN25P32"     ,0x1C2016, 4 , 64 , 64 , 256},//ok
    {"EN25F32"     ,0x1C3116, 4 , 64 , 64 , 256},//ok
    {"EN25Q32"     ,0x1C3316, 4 , 64 , 64 , 256},
    {"EN25Q32A"    ,0x1C3016, 4 , 64 , 64 , 256},//ok
    {"M25P32"      ,0x202016, 4 , 64 , 64 , 256},
    {"MX25L3205D"  ,0xc22016, 4 , 64 , 64 , 256},//ok
    {"MX25L3235D"  ,0xc22416, 4 , 64 , 64 , 256},//ok
    {"N25S32"      ,0xD53016, 4 , 64 , 64 , 256},//?
    {"W25X32"      ,0xEF3016, 4 , 64 , 64 , 256},//ok
    {"W25Q32"      ,0xEF4016, 4 , 64 , 64 , 256},//ok
  //{"QH25FL032"   ,0x898912, 4 , 64 , 64 , 256},
    {"S25FL032A"   ,0x010215, 4 , 64 , 64 , 256},//ok
  //{"SST25VF032"  ,0xBF254A, 4 , 64 , 64 , 256},
    
    {"AT25DF641"   ,0x1F4800, 8 , 64 , 128, 256},
    {"A25L064"     ,0x373017, 8 , 64 , 128, 256},
    {"EN25P64"     ,0x1C2017, 8 , 64 , 128, 256},
    {"EN25Q64"     ,0x1C3017, 8 , 64 , 128, 256},//ok
    {"M25P64"      ,0x202017, 8 , 64 , 128, 256},//ok
    {"MX25L6405D"  ,0xc22017, 8 , 64 , 128, 256},//ok
    {"W25X64"      ,0xEF3017, 8 , 64 , 128, 256},
    {"W25Q64"      ,0xEF4017, 8 , 64 , 128, 256},//ok
  //{"QH25FL064"   ,0x898913, 8 , 64 , 128, 256},
    {"S25FL064A"   ,0x010216, 8 , 64 , 128, 256},//ok

    
    {"AT25DF128"   ,0x1F4900, 16, 64 , 256, 256},
    {"A25L128"     ,0x373018, 16, 64 , 256, 256},
    {"EN25P128"    ,0x1C2018, 16, 64 , 256, 256},
    {"EN25Q128"    ,0x1C3018, 16, 64 , 256, 256},//ok
  //{"M25P128"     ,0x202018, 16, 256, 64 , 256},
    {"N25Q128"     ,0x20BA18, 16, 64 , 256, 256},//ok
    {"N25Q128"     ,0x20BB18, 16, 64 , 256, 256},//ok
    {"MX25L12805"  ,0xc22018, 16, 64 , 256, 256},//ok as 12845
    {"W25X128"     ,0XEF3018, 16, 64 , 256, 256},
    {"W25Q128"     ,0XEF4018, 16, 64 , 256, 256},//ok
  //{"S25FL128P"   ,0X012018, 16, 256, 64 , 256},
    {"S25FL128P"   ,0X012018, 16, 64 , 256, 256},//ok
    
    {"Unknown"     ,0       , 0 , 0  , 0  , 0  }
} ;


unsigned long flash_init (void)
{
	spi_flash_id  flash_id;
	int i;
	
	read_id(&flash_id);

	for(i =0; i < (sizeof(g_flash_table)/sizeof(g_flash_table[0])); i++)
	{
        if((g_flash_table[i].mtc_id  == flash_id.device_id)
         ||(g_flash_table[i].mtc_id  == 0))
        {
			break;
        }
	}
	
    flash_info[0].size           = g_flash_table[i].MB_size <<20;
	flash_info[0].sector_count   = g_flash_table[i].n_secs;
	flash_info[0].flash_id       = g_flash_table[i].mtc_id;
	
	printf("flash=%s size=%dM id=0x%0x  sector=%dk\n",g_flash_table[i].pname,  g_flash_table[i].MB_size,
	      flash_id.device_id, g_flash_table[i].KB_sec);
	      
	for(i=0;i < flash_info[0].sector_count;i++)
	{
		flash_info[0].start[i]   = (flash_info[0].size /flash_info[0].sector_count) *  i  + CONFIG_FLASH_BASE;
		flash_info[0].protect[i] = flash_info[0].start[i];
	}

	info=&flash_info[0];
	
	return flash_info[0].size;
}
#endif
