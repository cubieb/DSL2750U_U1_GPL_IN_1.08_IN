#include <common.h>
#include <malloc.h>
#include <spi_flash.h>

#ifdef SPI_FLASH

flash_info_t flash_info[CFG_MAX_FLASH_BANKS];           /* FLASH chips info */
flash_info_t *info;
extern const struct spi_flash_params flash_table[];

unsigned long SFCR = 0;
unsigned long SFCSR = 0;
unsigned long SFDR = 0;


static inline void spi_ready(void)
{
    while(1)
    {
        if ((*(volatile unsigned int *) SFCSR) & READY(1))
            break;
    }
}

static void flush_prediction(volatile unsigned int x)
{
	if(x>1)
		{
		flush_prediction(x-1);
		}
}

static void spi_pio_init(void)
{
    spi_ready();
	flush_prediction(10);
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);
    spi_ready();
	udelay(1);
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);
    spi_ready();
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);
    spi_ready();
	udelay(1);
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);
    spi_ready();
}

static void rtl8676_spi_cmd_read(u32 offset, u32 *data_out, int len)
{
	unsigned int status;
	unsigned int *buf;
	int read_size = 0;
	
    /* De-Select Chip */
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);
    /* RDSR Command */
    spi_ready();
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1) | READY(1);
    *(volatile unsigned int *) SFDR = 0x05 << 24;
    while(1)
		{
		status = *(volatile unsigned int *) SFDR;
		/* RDSR Command */
		if(0 == (status & 0x01000000))
			{
			break;
			}
		}
	*(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
	/* READ Command */
	spi_ready();
	flush_prediction(10);
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);
	*(volatile unsigned int *) SFDR = (0x03 << 24) | (offset & 0xFFFFFF);
	/* Read Data Out */
	for(buf = data_out; read_size < len; buf++)
		{
		*buf = *(volatile unsigned int *) SFDR;
		read_size += sizeof(unsigned int);
		spi_ready();
		*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);
		}
	*(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);
}



static void rtl8676_spi_cmd_block_erase(int block)
{
    spi_pio_init();

    /* WREN Command */
    spi_ready();
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1) | READY(1);

    *(volatile unsigned int *) SFDR = 0x06 << 24;
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);

    /* SE Command */
    spi_ready();
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);
    *(volatile unsigned int *) SFDR = (0xD8 << 24) | (block << 16);
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);

    /* RDSR Command */
    spi_ready();
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1) | READY(1);
    *(volatile unsigned int *) SFDR = 0x05 << 24;

    while (1)
    {
        /* RDSR Command */
        if(((*(volatile unsigned int *) SFDR) & 0x01000000) == 0x00000000)
            break;
    }

    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
}

static void rtl8676_spi_cmd_page_program(unsigned int addr, unsigned char *data, int len)
{
    unsigned char *cur_addr;
    int cur_size;
    unsigned int temp;
    unsigned int cnt;
    cur_addr = data;
    cur_size = len;

	spi_ready();
	flush_prediction(10);
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1) | READY(1);
    *(volatile unsigned int *) SFDR = 0x06 << 24;/* WREN Command */
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
    /* PP Command */
    spi_ready();
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);
    *(volatile unsigned int *) SFDR = (0x02 << 24) | addr;
    for(cnt = 0; cnt < 64; cnt++)
		{
        spi_ready();
        if (cur_size >= 4)
			{
	        temp = (*(cur_addr)) << 24 | (*(cur_addr + 1)) << 16 | (*(cur_addr + 2)) << 8 | (*(cur_addr + 3));
            *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);
            cur_size -= 4;
			}
        else
			{
			temp = 0;
			switch(cur_size)
				{
				case 3:
					temp |= (*(cur_addr + 2)) << 8;
				case 2:
					temp |= (*(cur_addr + 1)) << 16;
				default:
					temp |= (*(cur_addr)) << 24;
					break;				
				}			
            *(volatile unsigned int *) SFCSR = LENGTH(cur_size-1) | CS(1) | READY(1);
            cur_size = 0;
			}
        *(volatile unsigned int *) SFDR = temp;
        cur_addr += 4;
		if(0 == cur_size)
			{
			break;
			}
		}
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);
    /* RDSR Command */
    spi_ready();
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1) | READY(1);
    *(volatile unsigned int *) SFDR = 0x05 << 24;
    cnt = 0;
    while(1)
		{
        temp = *(volatile unsigned int *) SFDR;

        /* RDSR Command */
        if ((temp & 0x01000000) == 0x00000000)
        {
            break;
        }

        if (cnt > 200000)
        {
            printf("\nBusy Loop for RSDR: %d, Address at 0x%08X\n", temp, addr);
busy:
            goto busy;
        }
        cnt++;
    }

    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(3) | READY(1);

}


unsigned long flash_init(void)
{
    unsigned long id;
    int i;
    volatile u32 value;

    value = REG32(SCCR);
    if((value & 0xFFFF0000) == 0)
    {
        value |= 0xFFFF0000;
        REGWRITE32(SCCR, value);
    }
    if((REG32(SCCR) & 0x00300000) != 0x00300000)
    {
        debug("IC version CD!\n");
        SFCR = 0xB8001200;
        SFCSR = 0xB8001208;
        SFDR = 0xB800120C;
        *(volatile unsigned int *) 0xb8003304 = 0x00000008;
    }
    else
    {
        debug("IC version FC!\n");
        SFCR = 0xB8001200;
        SFCSR= 0xB8001204;
        SFDR = 0xB8001208;
    }

    *(volatile unsigned int *) SFCR =*(volatile unsigned int *) SFCR |SPI_CLK_DIV(1);
    spi_pio_init();

    /* Here set the default setting */
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
    //printf("\n");
    spi_ready();
    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1) | READY(1);

    /* One More Toggle (May not Necessary) */
    // *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
    // *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(1) | READY(1);

    /* RDID Command */
    *(volatile unsigned int *) SFDR = 0x9F << 24;
    spi_ready();
    *(volatile unsigned int *) SFCSR = LENGTH(3) | CS(1) | READY(1);
    value = *(volatile unsigned int *) SFDR;

    *(volatile unsigned int *) SFCSR = LENGTH(0) | CS(3) | READY(1);
    //printf("delay before spi_ready\n\r");
    spi_ready();

    id = (value >> 8) & 0x00FFFFFF;
    for(i =0; ; i++)
    {
        if(flash_table[i].mtc_id == 0)
        {
            flash_info[0].size = 0;
            flash_info[0].sector_count = 0;
            flash_info[0].flash_id = 0;
            break;
        }
        else if(flash_table[i].mtc_id  == id)
        {
            flash_info[0].size           = flash_table[i].MB_size <<20;
            flash_info[0].sector_count   = flash_table[i].n_secs;
            flash_info[0].flash_id       = flash_table[i].mtc_id;
            break;
        }
    }
    printf("Flash: SPI %s size=%dM id=0x%08x\n", flash_table[i].name, flash_table[i].MB_size, id);
    value = REG32(SFCR);
    debug("SPI control register byte order is [0x%08x] %s\n", value, ((value & (1<<27)) == (1<<27))? "HIGH2LOW" : "LOW2HIGH");
    for(i=0; i < flash_info[0].sector_count; i++)
    {
        flash_info[0].start[i] = (flash_info[0].size / flash_info[0].sector_count) * i + CONFIG_FLASH_BASE;
        flash_info[0].protect[i] = flash_info[0].start[i];
    }

    info=&flash_info[0];

    return flash_info[0].size;
}

int flash_read_buf(flash_info_t * info, void* addr, unsigned char **buf, int len)
{
	int i = 0;
	unsigned int data;
	unsigned int offset;
	int data_size;

	if(len  > (CONFIG_FLASH_BASE + info->size - (ulong)addr))
		{
		printf("Can not read flash, length is out of flash space.\n");
		return 0;
		}
	offset = (unsigned int)((unsigned int)(addr) - CONFIG_FLASH_BASE);
	data_size = sizeof(data);
	if((unsigned int)(*buf) % sizeof(unsigned int))
		{/* Target address is not word aligned */
	    while(i < len)
			{
			rtl8676_spi_cmd_read((offset + i), &data, data_size);
	        if((len - i) < data_size)
				{
				data_size = len - i;
				}
			memcpy((unsigned char *)(*buf + i), &data, data_size);
			i += data_size;			
			}
		}
	else
		{
		data = (unsigned int)(len % sizeof(data));
		if(data)
			{/* Data length is not word aligned */
			data_size = len - data;
			rtl8676_spi_cmd_read(offset, (unsigned int *)*buf, data_size);
			i += data_size;
			data_size = data;
			rtl8676_spi_cmd_read(offset + i, &data, sizeof(data));
			memcpy((unsigned char *)(*buf + i), &data, data_size);
			i += data_size;
			}
		else
			{
			rtl8676_spi_cmd_read(offset, (unsigned int *)*buf, len);
			i = len;
			}
		}
	
    return i;
}

static int flash_program_buf(void* addr, void* data, int len)
{
    int total = 0, len_this_lp, bytes_this_page;
    unsigned long dst;
    unsigned char *src;
    unsigned long uladdr = (unsigned long)addr - CONFIG_FLASH_BASE;
    int i = 0;

    while(total < len)
		{
        src              = (unsigned char*)data + total;
        dst              = uladdr + total;
        bytes_this_page  = 256 - ((unsigned long)dst % 256);
        len_this_lp      = ((len - total) > bytes_this_page) ? bytes_this_page: (len - total);

        //		debug("src=0x%x,dst=0x%x,len_this_lp=0x%x,bytes_this_page=0x%x\n",src,dst,len_this_lp,bytes_this_page);

        rtl8676_spi_cmd_page_program(dst, src, len_this_lp);
        i++;
        total += len_this_lp;
        if((i % 256) == 1)
			{
            printf("#");
			}
		}
	
    return 0;
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

int flash_write(char *src, ulong addr, ulong cnt)
{

    ulong         end        = addr + cnt - 1;
    flash_info_t *info_first = addr2info (addr);
    flash_info_t *info_last  = addr2info (end );
    flash_info_t *info;
    unsigned char *p;
    unsigned int i,j,tmp,addr_tmp,src_tmp;
    ulong len;
	ulong e_addr;
	ulong b_end;
    unsigned int ret= 0;
	short s_end;

    if(cnt == 0)
		{
        return (ERR_OK);
		}

    if(!info_first || !info_last)
		{
        return (ERR_INVAL);
		}

    for(info = info_first; info <= info_last; ++info)
		{
        b_end = info->start[0] + info->size;      /* bank end addr */
        s_end = info->sector_count - 1;

        for(i=0; i<info->sector_count; ++i)
			{
            e_addr = (i == s_end) ? b_end : info->start[i + 1];
            if((end >= info->start[i]) && (addr < e_addr) &&(info->protect[i] != 0))
				{
                return (ERR_PROTECTED);
				}
			}
		}

    for(info = info_first; info <= info_last && cnt > 0; ++info)
		{
        len = info->start[0] + info->size - addr;
        if(len > cnt)
        	{
            len = cnt;
        	}
        if(((unsigned int)src >= info_first->start[0]) && ((unsigned int)src <= info_first->start[0] + info_first->size))
			{
            addr_tmp=addr;
            src_tmp = (unsigned int)src;
            for(i=0; i<(cnt>>10); i++)
				{
                p=malloc(1024);
                flash_read_buf( info , src , &p , 1024 );
				j = flash_program_buf((unsigned char *)addr, p, 1024);
                if(j != 0)
					{
                    ret = j;
                    goto out;
					}
                src += 1024;
                addr += 1024;
                free(p);
				}

            if((len % 1024) != 0)
				{
                tmp = len % 1024;
                p = malloc(tmp);
                flash_read_buf(info, src,  &p, tmp);
				j = flash_program_buf((unsigned char *)addr, p, tmp);
                if(j != 0)
					{
                    ret = j;
                    goto out;
					}
                free(p);
				}
            addr = addr_tmp;
            src = (char *)src_tmp;
			}
        else
			{
			i = flash_program_buf((unsigned char *)addr, src, cnt);
			if(i != 0)
				{
                ret = i;
                goto out;
				}
			}
        cnt  -= len;
        addr += len;
        src  += len;
		}	
out:
    return ERR_OK;
}

void flash_print_info (flash_info_t * info)
{
    int i;

    printf ("  Spi flash ID: 0x%x  Size: %ld MB in %d Sectors\n", info->flash_id, info->size >> 20, info->sector_count);
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


int flash_erase(flash_info_t *info, int s_first, int s_last)
{
    int i = 0;
	int inc;
	unsigned int count = 0;


    if((s_first < 0) ||	(s_first > info->sector_count - 1) || (s_last < 0) || (s_last > info->sector_count - 1))
		{
        printf("setor number out of flash size!\n");
        goto out;
		}

	if(s_first <= s_last)
		{/* ÉÈÇøÔöÁ¿ */
		inc = 1;
		}
	else
		{
		inc = -1;
		}

	for(i = s_first; i != s_last + inc; i += inc)
		{
		printf(".");
		rtl8676_spi_cmd_block_erase(i);
		count++;
		}
out:
	return count;
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

#endif
