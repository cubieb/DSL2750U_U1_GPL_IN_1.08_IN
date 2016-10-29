/*****************************************************************************
 * DANUBE BootROM
 * Copyright (c) 2005, Infineon Technologies AG, All rights reserved
 * IFAP DC COM SD
 * SFlash AT45DB321C *
 *****************************************************************************/

#include <config.h>
#include <common.h>
#include <asm/addrspace.h>
#include <asm/mipsregs.h>

#include "ssc.h"
#include "sflash.h"
#include "ssc_extra.h"

#include "amazon_se_reg.h"



#define TEST_IMG	EEPROM_CONTENT	 

#undef DEBUG_SFLASH_OUTPUT
//#define DEBUG_SFLASH_OUTPUT

#if defined(DEBUG_SFLASH_OUTPUT)
#define DEBUG_SFLASH(string)	asc_puts(string)
#define DEBUG_SFLASH_U8(string)	print_u8(string)
#define DEBUG_SFLASH_U32(string)	print_u32(string)
#else
#define DEBUG_SFLASH(string)
#define DEBUG_SFLASH_U8(string)
#define DEBUG_SFLASH_U32(string)
#endif

/* dupm sflash data out */
#undef DUMP_SFLASH_DATA

//#define PAGE_SIZE_BIG		1056
#define PAGE_SIZE_SMALL		256

//#define SECTOR_SIZE		0x10000  /*IFX board  Commented by Manamohan */
//#define SECTOR_SIZE		0x10000

#define MAIN_MEM_PAGE_READ	0xD2

////#define CONT_ARRAY_READ		0x03
////#define FAST_READ		0x0B

#define BUFFER1_WRITE		0x84
#define BUFFER2_WRITE		0x87
#define BUFFER1_TO_PAGE_MEM	0x88
#define BUFFER2_TO_PAGE_MEM	0x89

//#define PAGE_ERASE		0x81
//#define SECTOR_ERASE		0xD8 /*Commented by Manamohan */
////#define SECTOR_ERASE		0x20
//#define BLOCK_ERASE		0x52
////#define CHIP_ERASE		0x60

////#define WREN			0x06

////#define PAGE_PROGRAM		0x02

#define STATUS_OPCODE		0x05 // yclee: RDSR
#define STATUS_RDBY		0x01 // yclee: WIP

#define RDID 0x9f
#define MX_MX25L1605A 0xc22015
#define MX_MX25L3205U 0xc22016
/*--- add for 2M flash support ---*/
#define EON_EN25F16 0x1c3115
#define WINBOND_W25X16 0xef3015
/*--- add for 4M flash support ---*/
#define SPANSION_S25FL032A 0x010215
#define WINBOND_W25X32 0xef3016

static u32 sector_erase=0;
static u32 chip_erase=0;
static u32 cont_array_read=0;
static u32 fast_read=0;
static u32 wren=0;
static u32 page_program=0;
static u32 sector_size=0;


//#define BYTE_ADDR_BIG(addr)		(addr%PAGE_SIZE_BIG)
//#define PAGE_ADDR_BIG(addr)		(addr/PAGE_SIZE_BIG)
//#define BYTE_ADDR_SMALL(addr)		(addr%PAGE_SIZE_SMALL)
//#define PAGE_ADDR_SMALL(addr)		(addr/PAGE_SIZE_SMALL)

spi_dev_t spi_sflash;
typedef void (*convert_addr_t)(u32 address, u8 * addr);
void at45db321c_convert_addr(u32 address, u8* addr);
//int  _program_flash(u8 * wr_buf, u32 data_len,u8 addr_cycles, u32 page_size, convert_addr_t convert,int verify,int offset);

int _program_flash(u8 * wr_buf, u32 data_len,u8 addr_cycles, u32 page_size,
                convert_addr_t convert,int verify,u32 start_addr);
int _program_partial_flash(u8 * wr_buf, u32 data_len,u8 addr_cycles, u32 page_size, convert_addr_t convert,int verify,u32 start_addr);
void _dump_sflash(u8 addr_cycles,u32 page_size, u32 page_no, convert_addr_t convert);
void _sflash_init(void);
void asc_puts (const char *s);
void print_u8(u8 data);
void print_u32(u32 data);

/* 添加flash_info的全局变量，张喻 */
flash_info_t flash_info[CFG_MAX_FLASH_BANKS];           /* FLASH chips info */
flash_info_t * info;


void sflash_sync(void)	
{
	volatile u32 temp;
	int ready=0;
	while(ready==0){
		spi_session(&spi_sflash,0x05,0,3,0,0,(u8*)&temp,4);

#if 0		
		asc_puts("\ntemp = ");
		print_u32(temp);
		asc_puts("\n\n\n\n");
#endif
		
		if((temp&STATUS_RDBY)==0){
			ready=1;
		}
	}
}

void flash_chip_erase(void)
{
	spi_session(&spi_sflash,wren,0,0,0,0,0,0); // write_enable

	spi_session(&spi_sflash,chip_erase,0,0,0,0,0,0);
	sflash_sync();
	return;
}


void flash_sector_erase(u8 * addr)
{
	spi_session(&spi_sflash,wren,0,0,0,0,0,0); // write_enable

	spi_session(&spi_sflash,sector_erase,addr,0,0,0,0,0);
	sflash_sync();
	return;
}

int page_write(u8 *addr, u8 *buf,u32 len)
{
	spi_session(&spi_sflash,wren,0,0,0,0,0,0); // write_enable

	spi_session(&spi_sflash,page_program,addr,0,buf,len,0,0);
	sflash_sync();
	return 0;
}

void flash_read(u8 *addr, u8 *buf, u32 len)
{
	spi_session(&spi_sflash,fast_read,addr,1,0,0,buf,len);

	return;
}

int compare_data(u8* src, u8* dst,u32 size)
{
	int i=0;
	int result=1;
	for(i=0;i<size;i++)
	{
		if(*(src+i)!=*(dst+i))
		{
         printf("\nCompare data falied\n");
			result=0;
			asc_puts("diff: [");
			print_u32((u32)((src+i)));
			asc_puts("]= ");
			print_u8((u32)(*(src+i)));
			asc_puts("--- [");
			print_u32((u32)((dst+i)));
			asc_puts("]= ");
			print_u8((u32)(*(dst+i)));
			asc_puts("\n");
		}
	}
	return result;

}

/*	Brief:	convert address from u32 to u8 array for AT45DB321C
 *	Parameter:
 *		address: the address, maximum 32 bit
 *		addr:	array that holds the results, maximum 4 elemets
 *		addr_cycles: number of real data bytes=, maximum 4
 */
void at45db321c_convert_addr(u32 address, u8* addr)
{
	addr[0] = (u8) ((address>>16) & 0xff);
	addr[1] = (u8) ((address>>8) & 0xff);
	addr[2] = (u8) (address & 0xff);
}

#define MAX_ADDRESS_NUM		5

//static u32 g_sflash_content[] = TEST_IMG;

#if 0 /*Commented by Manamohan */

/*  Brief: read certain number of page
 *  Parameter:
 *    addr_cycles: number of address (eg, 3 bytes)
 *    page_size: eg, 512 bytes
 *    page_no: number of pages to be dump
 */
void _dump_sflash(u8 addr_cycles,u32 page_size, u32 page_no, convert_addr_t convert)
{
	u8 addr[MAX_ADDRESS_NUM];
	u32 address=0;
	u32 rd_buf[PAGE_SIZE_SMALL/4];
	int i,j;
	u32 eff_len,eff_len_in_page;

	spi_sflash.addr_cycles = addr_cycles;
	sflash_sync();
	asc_puts("page no: ");print_u32(page_no); asc_puts("\n");
	(*convert)(address,addr);
	for(i=0;i<page_no;i++){
                eff_len = page_size;
		for(;eff_len>0;){
			eff_len_in_page=(eff_len>PAGE_SIZE_SMALL)?(PAGE_SIZE_SMALL):(eff_len);
			flash_read(addr,rd_buf,eff_len_in_page);
			asc_puts("\n");
			for(j=0;j<eff_len_in_page/4;j++){
				print_u32(rd_buf[j]); asc_puts(" ");
                                if ( j % 8 == 7) asc_puts("\n");
			}
			asc_puts("\n");
			address+=eff_len_in_page;
			(*convert)(address,addr);
			eff_len-=eff_len_in_page;
		}
	}
#endif

int amazon_se_sflash_read(u8* buffer,u32 cnt,u32 offset)
{
//	int err=0;
	u8 addr[MAX_ADDRESS_NUM];
//	u32 rem=0;


	sflash_sync();

	spi_sflash.addr_cycles = 3;
	
	at45db321c_convert_addr(offset,addr);
	
	flash_read(addr,buffer,cnt);
	
	return cnt;
}
        
int amazon_se_sflash_write (u8* buffer,u32 cnt,u32 offset)
{
   int err=0;

   _sflash_init();
   err=_program_flash(buffer,cnt,3,PAGE_SIZE_SMALL,at45db321c_convert_addr,1,offset);
   if(err)
      return err;
   else
      return cnt;

}



/*  Brief:  program a serial flash and check [optional]
 *  Parameter:
 *    addr_cycles: number of address cycles
 *    page_size
 *    convert_addr_t: convert address
 *    verify: read back and compare
 *  Return:
 *    0 no err
 *    1 otherwise
 */
int _program_flash(u8 * wr_buf, u32 data_len,u8 addr_cycles, u32 page_size,
                convert_addr_t convert,int verify,u32 start_addr)
{
	u8 addr[MAX_ADDRESS_NUM];
//	u32 start_addr = 0;
	u32 address;
	u8 rd_buf[PAGE_SIZE_SMALL];
	int i;
	u32 page_no;
//	u32 sector_no;
	u32 eff_len,eff_len_in_page;
	int err=0;
	u32 first_page_len;

	spi_sflash.addr_cycles = addr_cycles;
	sflash_sync();

	

	/* 判断开始地址是否对齐page边界，如果没对齐第一次只能写到边界位置 */
	if(start_addr%page_size){
		first_page_len = data_len<(page_size-start_addr%page_size)?data_len:page_size-start_addr%page_size;
		page_no=(data_len - first_page_len)%page_size==0?(data_len - first_page_len)/page_size+1:(data_len - first_page_len)/page_size+2;
	}
	else
	{
		page_no=(data_len%page_size)==0?(data_len/page_size):(data_len/page_size+1);
	}

#if 0  /* 不进行擦除FLASH的操作 */
	sector_no=(data_len%sector_size)==0?(data_len/sector_size):(data_len/sector_size+1);


	// let's erase enough flash first
#if 1
	address = start_addr;
	for(i=0;i<sector_no;i++){
	        (*convert)(address,addr);
		sflash_sync();
		flash_sector_erase(addr);
		address += sector_size;
        }
#else
        asc_puts("\nDoing flash_chip_erase. This will take a while ...\n");
	flash_chip_erase();
#endif

#endif


	// now is time for programming flash
	address = start_addr;
	for(i=0;i<page_no;i++){
		(*convert)(address,addr);
		eff_len=(data_len>page_size)?page_size:data_len;
		if(i==0){
			eff_len = (page_size - (address % page_size))<eff_len?(page_size - (address % page_size)):eff_len; /* 两者中取小的 */
		}
		
		page_write(addr,wr_buf,eff_len);
		data_len-=eff_len;  /* 此处减去当前已写入的长度 */
		
		for(;eff_len>0 && err==0;){
			eff_len_in_page=(eff_len>PAGE_SIZE_SMALL)?(PAGE_SIZE_SMALL):(eff_len);

            if (verify) {
                flash_read(addr,rd_buf,eff_len_in_page);
                if (!compare_data(wr_buf,(u8 *)rd_buf,eff_len_in_page) ){
                   err=1;
                }
            }
			
			address+=eff_len_in_page;
			eff_len-=eff_len_in_page;
			wr_buf+=eff_len_in_page;
			
			//printf(".");
		} /* for(;eff_len>0 && err==0;) */
		 
		
	}
        return err;
}

void _sflash_init(void)
{
	ssc_init((u32)SFLASH_BAUDRATE);
	spi_sflash.read_cmd    = SFLASH_READ;
	spi_sflash.cs = 0;
	spi_sflash.addr_cycles = SPI_ADDR_CYC_MAX;
}


/* 探测flash，初始化flash_info,返回flash的大小。by 张喻 */
unsigned long flash_init (void)
{
	u8 buf[8];
	u8 len = 4;
	u8 stat_value = 0;
	u32 flash_id =0;
	int i;

	 
	_sflash_init();

	sflash_sync();

	debug("Probing flash ....");
		
	spi_session(&spi_sflash,RDID,0,0,0,0,buf,len);

	flash_id += buf[0];
	flash_id = (flash_id << 8) + buf[1];
	flash_id = (flash_id << 8) + buf[2];
	

	
	switch(flash_id)
	{
		/*--- for 2M flash support ---*/
		case MX_MX25L1605A:
		case EON_EN25F16:
		case WINBOND_W25X16:
			sector_erase=0x20;
			chip_erase=0x60;
			cont_array_read=0x03;
			fast_read=0x0b;
			wren=0x06;
			page_program=0x02;
			sector_size=0x1000;
			
			flash_info[0].size = 0x200000;
			flash_info[0].sector_count = 512;
			flash_info[0].flash_id = flash_id;

			debug("Found flash ID=0x%x chip_size=0x%x  sector_size=0x%x\n", flash_id, flash_info[0].size,sector_size);

			break;
		/*--- for 4M flash support ---*/
		case MX_MX25L3205U:
		case SPANSION_S25FL032A:
		case WINBOND_W25X32:
			sector_erase=0xd8;
			chip_erase=0x60;
			cont_array_read=0x03;
			fast_read=0x0b;
			wren=0x06;
			page_program=0x02;
			sector_size=0x10000;

			flash_info[0].size = 0x400000;
			flash_info[0].sector_count = 64;
			flash_info[0].flash_id = flash_id;

			debug("Found flash ID=0x%x chip_size=0x%x  sector_size=0x%x\n", flash_id, flash_info[0].size,sector_size);

			break;	
		default:
			sector_erase=0xd8;
			chip_erase=0x60;
			cont_array_read=0x03;
			fast_read=0x0b;
			wren=0x06;
			page_program=0x02;
			sector_size=0x10000;

			flash_info[0].size = 0;
			flash_info[0].sector_count = 0;
			flash_info[0].flash_id = FLASH_UNKNOWN;
			printf("Unknow flash id 0x%x!\n",flash_id);

			break;
			//return 1;
	}
	/*software protection disable*/
	spi_session(&spi_sflash,wren,0,0,0,0,0,0); // write_enable
	spi_session(&spi_sflash,0x01,0,0,(u8*)&stat_value,1,0,0);
	sflash_sync();//write status register must delay some times;


	for(i=0;i < flash_info[0].sector_count;i++)
	{
		flash_info[0].start[i] = flash_info[0].size /flash_info[0].sector_count *  i  + CONFIG_FLASH_BASE;
		flash_info[0].protect[i] = flash_info[0].start[i];
	}


	info=&flash_info[0];
	
	return flash_info[0].size;
	

#if 0  
	spi_session(&spi_sflash,0x05,0,3,0,0,(u32*)&temp,1);
	printf("temp = %x\n",temp);
	stat_value = 4;
	spi_session(&spi_sflash,wren,0,0,0,0,0,0); // write_enable
	spi_session(&spi_sflash,0x01,0,0,(u8*)&stat_value,1,0,0);
	sflash_sync();//write status register must delay some times;

	spi_session(&spi_sflash,0x05,0,3,0,0,(u32*)&temp,1);
	printf("temp = %x\n",temp);
	stat_value = 0;
	spi_session(&spi_sflash,wren,0,0,0,0,0,0); // write_enable
	spi_session(&spi_sflash,0x01,0,0,(u8*)&stat_value,1,0,0);
	sflash_sync();//write status register must delay some times;
#endif

}

static inline u32 get_cycles (void)
{
          return read_c0_count();
}

#if 0
#define TEST_PAT_BYTESIZE	40000
u32 g_sflash_test_pattern[TEST_PAT_BYTESIZE/4];
#endif

#define COMBINED_IMAGE_HACK

#if 0

void test_sflash(void)
{
	u8 * wr_buf = (u8 *) g_sflash_content;
	u32 data_len=sizeof(g_sflash_content);


#ifdef COMBINED_IMAGE_HACK
   g_sflash_content[28] = 0x0004000 - 0x20;
#endif
#if 0
	u8 * wr_buf2 = (u8 *) g_sflash_test_pattern;
	u32 data_len2=sizeof(g_sflash_test_pattern);
#endif

  int err=0;

  u32 start_cycle, end_cycle;

#if 0
   int i;

   for (i = 0; i < TEST_PAT_BYTESIZE; i++)
   {
       g_sflash_test_pattern[i] = 0x5678abcd + i;
   } 
#endif

  _sflash_init();

start_cycle = get_cycles();      

#if 0
asc_puts("\n\n");
print_u32(data_len2);
asc_puts("\n\n");

  /* burn in dummy image first */
  err=_program_flash(wr_buf2,data_len2,3,PAGE_SIZE_SMALL,at45db321c_convert_addr,1);
#endif

asc_puts("\n\n");
print_u32(data_len);
asc_puts("\n\n");

  /* burn in actual desired image */
  err=_program_flash(wr_buf,data_len,3,PAGE_SIZE_SMALL,at45db321c_convert_addr,1);

end_cycle = get_cycles();

asc_puts("\ncycle start ....... \n");
print_u32(start_cycle);

asc_puts("\ncycle end ....... \n");
print_u32(end_cycle);

	if (err){
		asc_puts("\nSFLASH READ/WRITE TEST Fails\n");
	}else{
		asc_puts("\nSFLASH READ/WRITE TEST Pass\n");
	}
}
#endif


#if 0
void dump_at45_sflash(u32 page_no)
{
  /* for AT45DB321C */
  _dump_sflash(3,PAGE_SIZE_SMALL,page_no, at45db321c_convert_addr);
}

void test_write_sflash(void)
{
    u8 buf[8];
    u8 len = 4;
    u8 addr[4];
    u8 buf0[PAGE_SIZE_SMALL];
    u8 buf2[PAGE_SIZE_SMALL];
    int i;

    addr[0] = 0;
    addr[1] = 0;
    addr[2] = 0;
    addr[3] = 0;

    for (i = 0; i < PAGE_SIZE_SMALL; i++)
    {
        buf0[i] = 0x12 + i;
    }

    buf2[0] = 0x2a;
    buf2[1] = 0x4b;
    buf2[2] = 0x6c;
    buf2[3] = 0x8d;

    _sflash_init();

    sflash_sync();
    spi_session(&spi_sflash,0x9f,0,3,0,0,buf,len);
    asc_puts("RDID:\n");
    print_u8(buf[0]);
    print_u8(buf[1]);
    print_u8(buf[2]);    
    asc_puts("\n\n");

    spi_sflash.addr_cycles = 3;

    flash_sector_erase(addr);
    sflash_sync();

#if 0
    spi_session(&spi_sflash,0x9f,0,3,0,0,buf,len);
    asc_puts("RDID3:\n");
    print_u8(buf[0]);
    print_u8(buf[1]);
    print_u8(buf[2]);
    asc_puts("\n\n");
#endif

    page_write(addr, buf0, PAGE_SIZE_SMALL);
    sflash_sync();

#if 0
    spi_session(&spi_sflash,0x9f,0,3,0,0,buf,len);
    asc_puts("RDID2:\n");
    print_u8(buf[0]);
    print_u8(buf[1]);
    print_u8(buf[2]);
    asc_puts("\n\n");
#endif

    spi_session(&spi_sflash,cont_array_read,addr,0,0,0,buf2,PAGE_SIZE_SMALL);

    for (i = 0; i < PAGE_SIZE_SMALL; i++)
    {

        print_u8(buf0[i]);
        asc_puts(" -> ");
        print_u8(buf2[i]);
        asc_puts("\n\n");
    }
}

#endif
/*------------------------------------------------------------------------------*/
/* Function Name       : addr_dataflash 					*/
/* Object              : Test if address is valid				*/
/*------------------------------------------------------------------------------*/
int addr_serialflash (unsigned long addr)
{
	int addr_valid = 0;

	if ((((int) addr) & 0xF0000000) == 0) 
   {
			addr_valid = 1;
	}

	return addr_valid;
}

const char ascii[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a',
        'b', 'c', 'd', 'e', 'f' };

void print_u8(u8 data)
{
           char string[3]={0,0,0};

             string[0]=ascii[(data&0xf0)>>4];
               string[1]=ascii[data&0xf];

                  asc_puts(string);
}

void print_u32(u32 data)
{
           print_u8( (u8) (data>>24)&0xff);
              print_u8( (u8) (data>>16)&0xff);
                 print_u8( (u8) (data>>8)&0xff);
                    print_u8( (u8) (data>>0)&0xff);
}



/*
    Decrypt DSP FW
      data - pointer to encrypted firmware (32 bit aligned)
        n    - number of bytes in firmware (multiple of 32)
         */
void decrypt(u32 addr, int n)
{
       u32 * data = (u32*)addr;
       *data ^= 0x86B75CA1;
       while (n > 0)
       {
          *(data + 1) ^= *data;
          data++;
          n -= 4;
       }
}



void asc_puts (const char *s)
{
    while (*s)
    {
       putc (*s++);
    }
}


/***********************************************************
  说明:  添加tbsboot适配接口
  作者:  张喻
************************************************************/


int flash_read_buf(flash_info_t * info, void* addr, unsigned char ** buf, int len)
{
//	unsigned int  rd = 0x0;
	int ret;
	unsigned int real_addr;
	unsigned char *buf_tmp = *buf;

	if((unsigned long)addr < CONFIG_FLASH_BASE)
	{
		printf("the src_addr must not less than  then 0x%x\n",CONFIG_FLASH_BASE);
		return 0;
	}
	
	real_addr = (unsigned long)addr - CONFIG_FLASH_BASE;
	

	if( len  > (unsigned long)(info->start[info->sector_count  -1] + info->size / info->sector_count) - (unsigned long)addr)
	{
	        printf("The length is out of flash space.\n");
	        return 0;
	}


	/* 调用原方案驱动读取数据 */
	ret = amazon_se_sflash_read(buf_tmp, (u32)len,(u32)real_addr);

	return ret;
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
				flash_read_buf( info , src_tmp , &p , 1024 );
				if ( ( j = amazon_se_sflash_write( p , 1024, ( unsigned char *)addr_tmp ) ) != 1024) 
				{
					return (j);
				}
				src_tmp +=1024;
				addr_tmp+=1024;
				free(p);
			}

			if(len%1024!=0)
			{
				tmp=len%1024;
				p=malloc(tmp);
				flash_read_buf( info , src_tmp , &p , tmp );
				
				if ((j = amazon_se_sflash_write( p , tmp, addr_tmp)) != tmp) 
				{
					return (j);
				}
				free(p);
			}

		}
		else
		{
			if ( ( i = amazon_se_sflash_write(( unsigned char * )src, cnt, addr)) != cnt) 
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
	u8 addr[MAX_ADDRESS_NUM];
	int i = s_first;


	spi_sflash.addr_cycles = 3; /* 写地址值的周期 */
	sflash_sync();
   
	do 
	{
		at45db321c_convert_addr(info->start[i] - CONFIG_FLASH_BASE, addr);
		sflash_sync();
		flash_sector_erase(addr);

		printf(".");
		
	}while (i++ < s_last);



	printf("done\n");

	return 0;
}

