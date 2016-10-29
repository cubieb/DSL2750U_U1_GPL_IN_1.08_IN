/*****************************************************************************
 * CPE Chip Rev1 DDR TUNING
 * Support : Danube, ARX & VRX Platform
 *
 * IFAP DC COM SD
 *****************************************************************************/
#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/addrspace.h>
#include "vr9.h"
#include "spi_flash.h"

#define READMIN 0
#define READMAX 0x3f
#define READMIN_Q 0x15
#define READMAX_Q 0x2b
#define WRITEDQS_Q 0x55
#define WRITEMIN 0x40
#define WRITEMAX 0x7f
#define MC_START     0xbf401070
#define REG01   0xBE22FF10
#define REG02   0xBE22FF14
#define REG03   0xBE22FF18
#define REG04   0xBE22FF1C
#define STATUS  0xBE22FF20

/* VRX Memory controller register for data-eye tuning */
#define MC_RD_SLICE0    0xBF401270
#define MC_RD_SLICE1    0xBF401280
#define MC_WR_SLICE0    0xBF4012B0
#define MC_WR_SLICE1    0xBF4012C0
#define MC_DLL_SLICE0   0xBF4012F0
#define MC_DLL_SLICE1   0xBF401300
#if 0
/* ARX & Danube Memory controller register for data-eye tuning */
#define MC_STAT  0xBF800070     // DLL lock status
#define MC_DC15	 0xBF8010F0     // WRITE DQS
#define MC_DC21	 0xBF801150     // Data Slice 0
#define MC_DC22	 0xBF801160     // Data Slice 1
#define MC_DC24	 0xBF801180     // DQS Out Shift
#endif
// Declare a reference to the global pointer
//extern unsigned int _gp;
#if defined(MM_DEBUG)
void print_u8(u8 data)
{
   char string[3]={0,0,0};
   char ascii[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

   string[0]=ascii[(data&0xf0)>>4];
   string[1]=ascii[data&0xf];

   serial_puts(string);
}

void print_u32(u32 data)
{
   print_u8( (u8) (data>>24)&0xff);
   print_u8( (u8) (data>>16)&0xff);
   print_u8( (u8) (data>>8)&0xff);
   print_u8( (u8) (data>>0)&0xff);
}

void print_state(u8 cur_state)
{
	switch(cur_state)
	{
		case 0:
			serial_puts("\b\b\b[-]");
			break;
		case 1:  
			serial_puts("\b\b\b[\\]");
			break;
		case 2:  
			serial_puts("\b\b\b[|]");
			break;
		default:  serial_puts("\b\b\b[/]");
				 break;
	}
}
#endif
#define TESTADDR    0x80100000
#define TESTADDR_UC 0xA0000000

#if 0
void clear_error(u8* data)
{
    u8 i;

    for(i=0;i<16;i++)
        data[i] =0;
}

u16 check_data_bit_fail(u32 d1, u32 d2, u8* bit)
{
    u8 i;

    for(i=0;i<16;i++)
    {
        if((((u16)d1) & ~(1<<i))!=(((u16)d2) & ~(1<<i)))
            bit[i]=1;
        if((((u16)(d1>>16)) & ~(1<<i))!=(((u16)(d2>>16)) & ~(1<<i)))
            bit[i]=1;
    }
    return(0);
}
#endif





u32 sdramtest(u8 masktype, u8 mode, u32 ddrsize)
{
    u32 j,k,l;
    u32 x;
    u32 mask,pattern;
    u16 wmask;
    u8 bmask;

	u16 w_pattern[4] = {0x5555, 0xFFFF, 0x8000, 0x7FFF};
	u8  b_pattern[4] = {0x55, 0x00, 0xFE, 0x01};
	u32 d_pattern[8] ={0x5555aaaa, 0xFFFF0000, 0x80000001, 0x7FFFFFFE,
					   0x12345678, 0x9ABCDEF0, 0xC3C3C3C3, 0xFFFFFFFF};
	
    //u8  dq_error[16];
    if (masktype == 0) mask =0x00ff00ff;
    else if(masktype==1) mask = 0xff00ff00;
    else mask = 0xffffffff;

    wmask =(u16) mask;
    bmask = (u8) mask;

// Test# 1 32bit short Uncache Access

    for(j=0;j<8;j++)
    {
        for(k=0;k<0x20;k=k+4)
        {
            l=j;
            REG32(TESTADDR_UC+(j<<16)+k)=d_pattern[l];
            REG32(TESTADDR_UC+(ddrsize>>1)+(j<<16)+k)=~d_pattern[l];
        }
    }
    for(j=0;j<8;j++)
    {
        for(k=0;k<0x20;k=k+4)
        {
            l=j;
            x = REG32(TESTADDR_UC+(j<<16)+k);
            if((x&mask)!=(d_pattern[l]&mask))
                return(1);
            x = REG32(TESTADDR_UC+(ddrsize>>1)+(j<<16)+k);
            if((x&mask)!=((~d_pattern[l])&mask))
                return(1);
        }
    }

    if(mode != 1) // Normal mode 1: Quick test
    {
    // Test# 2 16bit Uncache Access
        for(j=0;j<4;j++)
        {
            for(k=0;k<0x40;k=k+2)
            {
                l=j;
                REG16(TESTADDR_UC+(j<<16)+k)=w_pattern[l];
                REG16(TESTADDR_UC+(ddrsize>>1)+(j<<16)+k)=~w_pattern[l];
            }
        }
        for(j=0;j<4;j++)
        {
            for(k=0;k<0x40;k=k+2)
            {
                l=j;
                x = REG16(TESTADDR_UC+(j<<16)+k);
                if((x&wmask)!=(w_pattern[l]&wmask))
                    return(1);
                x = REG16(TESTADDR_UC+(ddrsize>>1)+(j<<16)+k);
                if((x&wmask)!=((~w_pattern[l])&wmask))
                    return(1);
            }
        }
    
    // Test# 3 8bit Uncache Access
        for(j=0;j<4;j++)
        {
            for(k=0;k<0x80;k=k+1)
            {
                l=j;
                REG8(TESTADDR_UC+(j<<16)+k)=b_pattern[l];
                REG8(TESTADDR_UC+(ddrsize>>1)+(j<<16)+k)=~b_pattern[l];
            }
        }
        for(j=0;j<4;j++)
        {
            for(k=0;k<0x80;k=k+1)
            {
                l=j;
                x = REG8(TESTADDR_UC+(j<<16)+k);
                if((x&bmask)!=(b_pattern[l]&bmask))
                    return(1);
                x = REG8(TESTADDR_UC+(ddrsize>>1)+(j<<16)+k);
                if((x&bmask)!=((~b_pattern[l])&bmask))
                    return(1);
            }
        }

    // Test# 4 32bit Cache Access Random Pattern (short)
        for(j=0;j<8;j++)
        {
            for(k=0;k<0x100;k=k+4)
            {
                l=(j+(k>>2))%8;
                REG32(TESTADDR+(j<<16)+k)=d_pattern[l];
                REG32(TESTADDR+(ddrsize>>1)+(j<<16)+k)=~d_pattern[l];
            }
        }
        for(j=0;j<8;j++)
        {
            for(k=0;k<0x100;k=k+4)
            {
                l=(j+(k>>2))%8;
                x = REG32(TESTADDR+(j<<16)+k);
                if((x&mask)!=(d_pattern[l]&mask))
                    return(1);
                x = REG32(TESTADDR+(ddrsize>>1)+(j<<16)+k);
                if((x&mask)!=((~d_pattern[l])&mask))
                    return(1);
            }
        }
    
    // Test# 5 32bit Cache Access Checker Pattern (short)
        pattern=0x55555555;
        for(j=0;j<8;j++)
        {
            for(k=0;k<0x400;k=k+4)
            {
                REG32(TESTADDR+(j<<16)+k)=pattern;
                REG32(TESTADDR+(ddrsize>>1)+(j<<16)+k)=~pattern;
            }
        }
        for(j=0;j<8;j++)
        {
            for(k=0;k<0x400;k=k+4)
            {
                x = REG32(TESTADDR+(j<<16)+k);
                if((x&mask)!=(pattern&mask))
                    return(1);
                x = REG32(TESTADDR+(ddrsize>>1)+(j<<16)+k);
                if((x&mask)!=((~pattern)&mask))
                    return(1);
            }
        }
    
    // Test# 6 32bit Cache Access Moving 0 & 1 Pattern
        for(j=0;j<8;j++)
        {
            pattern=0x10001;
            for(k=0;k<0x400;k=k+4)
            {
                REG32(TESTADDR+(j<<16)+k)=pattern;
                REG32(TESTADDR+(ddrsize>>1)+(j<<16)+k)=~pattern;
                pattern=pattern<<1;
                if(pattern==0) pattern=0x10001;
            }
        }
        for(j=0;j<8;j++)
        {
            pattern=0x10001;
            for(k=0;k<0x400;k=k+4)
            {
                x = REG32(TESTADDR+(j<<16)+k);
                if((x&mask)!=(pattern&mask))
                    return(1);
                x = REG32(TESTADDR+(ddrsize>>1)+(j<<16)+k);
                if((x&mask)!=((~pattern)&mask))
                    return(1);
                pattern=pattern<<1;
                if(pattern==0) pattern=0x10001;
            }
        }
    
    // Test# 7 32bit Cache Access Random Pattern (long)
        for(j=0;j<8;j++)
        {
            for(k=0;k<0x4000;k=k+4)
            {
                l=(j+(k>>2)+2)%8;
                REG32(TESTADDR+(j<<16)+k)=d_pattern[l];
                REG32(TESTADDR+(ddrsize>>1)+(j<<16)+k)=~d_pattern[l];
            }
        }
        for(j=0;j<8;j++)
        {
            for(k=0;k<0x4000;k=k+4)
            {
                l=(j+(k>>2)+2)%8;
                x = REG32(TESTADDR+(j<<16)+k);
                if((x&mask)!=(d_pattern[l]&mask))
                    return(1);
                x = REG32(TESTADDR+(ddrsize>>1)+(j<<16)+k);
                if((x&mask)!=((~d_pattern[l])&mask))
                    return(1);
            }
        }
    }

    return(0);
}


/******************************************************************************
 * Global routines
 ******************************************************************************/

void tune_ddr(void)
{
	u8 mode;
	u8 cur_state=0;
	u32 ddrsize,chipid;

	u32 min_l, max_l, max_u, min_u;
	u32 write_l, read_l, read_u;
    u32 readq_l, readq_u;
	u32 l,u,k,i,j;
	u16 pass=0;
	u8 test_l[128], test_u[128];
    u32 write_u;


	ddrsize=0x20000000;
	for(k=0;k<128;k++)
    {
        test_l[k]=0;
        test_u[k]=0;
    }
	//cache_enable();
	for(k=0;k<0xffff;k++);

	chipid = REG32(0xbf107344);

    REG32(0xbe100b1c) &= ~(1<<3);
    REG32(0xbe100b20) &= ~(1<<3);
#if 0 //defined(MM_DEBUG)
	serial_puts("DDR Access auto data-eye tuning Rev 0.3a\n");
#endif

    ddrsize = ddrsize << 1;
	ddrsize = ddrsize >> ((REG32(0xBF4010B0)&0x7)) ;
    //Row Address; 0=14, 1=13, 2=12, 3=11, 4=10
	ddrsize = ddrsize >> ((REG32(0xBF4010B0)>>24)&0x7) ;
    //Col Address; 0=12, 1=11, 2=10, 3=9, 4=8;
	ddrsize = ddrsize >> ((~(REG32(0xBF401030)))&0x1) ;
    //Bank Address;
	switch ((REG32(0xBF401080)>>24)&0x3)
	{
		   case 1:
		   		k=0;
		   		break;
   		   case 3:
   		   		k=1;
   		   		break;
   		   default:
		   	#if defined(MM_DEBUG)
   		        serial_puts("Chip select selection not define correctly, EXIT\n");
			#endif
   		        goto end;
   		        break;
	}

	ddrsize = ddrsize << k;
	//print_u32(ddrsize); serial_putc('\n');

	 mode =0;
#if defined(MM_DEBUG)	
	serial_puts("DDR size from 0xa0000000 - 0x"); print_u32(0xa0000000+(ddrsize-1));serial_putc('\n');
	//serial_puts("DDR DLL lock element = "); print_u32((REG32(0xbf801150)&0x7f));serial_putc('\n');
#endif	

	
  if(REG32(STATUS)==0x0)
  {
	   if(sdramtest(2,0,ddrsize)==0) pass=1; //serial_puts("Initial DDR setting for SDRAM test pass\n");
  }

	if(pass)
	{
#if defined(MM_DEBUG)	
		serial_puts("DDR check ok... start booting...\n\n");
#endif
		REG32(STATUS)=0x1;
		goto start_uboot;
	}
	else
	{
		if(REG32(STATUS)!=0xff)
#if 0 //defined(MM_DEBUG)			
  		    serial_puts("Unknown command from u-boot\n");
#endif
		pass=0;
#if defined(MM_DEBUG)
		serial_puts("Tuning DDR ...");
#endif
	}
	REG32(STATUS)==0x10;

    cur_state=1;
#if defined(MM_DEBUG)
    serial_puts("[-]");
#endif

/* -------------------------------------------------------------------- */
/* Perform quick test to determine the inital readdqs for write tunning */
#if defined(MM_DEBUG)
	for(k=0;k<128;k++)
    {
        test_l[k]=0;
        test_u[k]=0;
    }
#endif

	min_l = READMAX_Q;
	max_l = READMIN_Q;
	min_u = READMAX_Q;
	max_u = READMIN_Q;

    for(j=0;j<15;j++)
    {
        REG32(MC_START) = 0;
        REG32(MC_WR_SLICE0) = (REG32(MC_WR_SLICE0)&0xff00ff)| ((WRITEDQS_Q+j)<<8);
        REG32(MC_WR_SLICE1) = (REG32(MC_WR_SLICE1)&0xff00ff)| ((WRITEDQS_Q+j)<<8);

        for(k=READMIN_Q;k<READMAX_Q;k++)
        {
            REG32(MC_START) = 0;
            REG32(MC_RD_SLICE0) = (REG32(MC_RD_SLICE0)&0xff00ff)| k<<8;
            REG32(MC_RD_SLICE1) = (REG32(MC_RD_SLICE1)&0xff00ff)| k<<8;
            REG32(MC_START) = 0x1010100;
            while(((REG32(MC_DLL_SLICE0)&0x1)&(REG32(MC_DLL_SLICE1)&0x1))==0);

#if defined(MM_DEBUG)
			if(++cur_state > 3)
				cur_state = 0;
            print_state(cur_state);
#endif
            for(i=0;i<0x200;i++);

            l=0; u=0;
            if(sdramtest(0,1,ddrsize)==0)
            {
                l=1;
                if(min_l > k) min_l = k;
                if(max_l < k) max_l = k;
            }
#if defined(MM_DEBUG)			
            if(++cur_state > 3)
				cur_state = 0;
            print_state(cur_state);
#endif
            if(sdramtest(1,1,ddrsize)==0)
            {
                u=1;
                if(min_u > k) min_u = k;
                if(max_u < k) max_u = k;
            }
            test_l[k]=l;
            test_u[k]=u;
        }
    }
    if( max_l > min_l)
        readq_l = (min_l+max_l)>>1;
    else
    {
#if defined(MM_DEBUG)    
        serial_puts("Error001: Could not determine a suitable read DQS delay for slice 0!\n");
#endif
        goto end;
    }
    if( max_u > min_u)
        readq_u = (min_u+max_u)>>1;
    else
    {
#if defined(MM_DEBUG)    
        serial_puts("Error002: Could not determine a suitable read DQS delay for slice 1!\n");
#endif
        goto end;
    }


/*-----------------End of Quick Test to determine Read DQS-------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
/*------------------------ Start of Write Data Eye Tuning -------------------------------*/

#if defined(MM_DEBUG)
	for(k=0;k<128;k++)
    {
        test_l[k]=0;
        test_u[k]=0;
    }
#endif

    min_l = WRITEMAX;
    max_l = WRITEMIN;
    min_u = WRITEMAX;
    max_u = WRITEMIN;

	for(k=0;k<0x1fff;k++);
    REG32(MC_START) = 0;
    REG32(MC_RD_SLICE0) = (REG32(MC_RD_SLICE0)&0xff00ff)| readq_l<<8;
    REG32(MC_RD_SLICE1) = (REG32(MC_RD_SLICE1)&0xff00ff)| readq_u<<8;

    for(k=WRITEMIN;k<WRITEMAX;k++)
    {
        REG32(MC_START) = 0;
        REG32(MC_WR_SLICE0) = (REG32(MC_WR_SLICE0)&0xff00ff)| k<<8;
        REG32(MC_WR_SLICE1) = (REG32(MC_WR_SLICE1)&0xff00ff)| k<<8;
        REG32(MC_START) = 0x1010100;
        while(((REG32(MC_DLL_SLICE0)&0x1)&(REG32(MC_DLL_SLICE1)&0x1))==0);
#if defined(MM_DEBUG)
        if(++cur_state > 3)
			cur_state = 0;
        print_state(cur_state);
#endif
        for(i=0;i<0x2000;i++);

        l=0; u=0;
        if(sdramtest(0,0,ddrsize)==0)
        {
            l=1;
            if(min_l > k) min_l = k;
            if(max_l < k) max_l = k;
        }
#if defined(MM_DEBUG)		
        if(++cur_state > 3)
			cur_state = 0;
        print_state(cur_state);
#endif
        if(sdramtest(1,0,ddrsize)==0)
        {
            u=1;
            if(min_u > k) min_u = k;
            if(max_u < k) max_u = k;
        }

#if defined(MM_DEBUG)
        test_l[k]=l;
        test_u[k]=u;
#endif
    }
#if 0 //defined(MM_DEBUG)
    serial_puts("\n Show slice 0 write setting\n");
    for(k=WRITEMIN; k< WRITEMAX+1; k++)
    {
        if(test_l[k]==0)
              serial_putc('0');
        else serial_putc('1');
        if(k%16 == 15) serial_putc('\n');
        else serial_putc(' ');
    }

    serial_puts("\n Show slice 1 write setting\n");
    for(k=WRITEMIN; k< WRITEMAX+1; k++)
    {
        if(test_u[k]==0)
              serial_putc('0');
        else serial_putc('1');
        if(k%16 == 15) serial_putc('\n');
        else serial_putc(' ');
    }
#endif
    if( max_l > min_l)
        write_l = (min_l+max_l)>>1;
    else
    {
#if defined(MM_DEBUG)    
        serial_puts("Error003: Could not determine a suitable write DQS delay for slice 0 !\n");
#endif
        goto end;
    }
    if( max_u > min_u)
        write_u = (min_u+max_u)>>1;
    else
    {
#if defined(MM_DEBUG)    
        serial_puts("Error004: Could not determine a suitable write DQS delay for slice 1!\n");
#endif
		goto end;
    }


	REG32(STATUS)==0x11;
/*------------------------ End of Write Data Eye Tuning ---------------------------------*/
/*---------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------*/
/*------------------------ Start of Read Data Eye Tuning --------------------------------*/

#if defined(MM_DEBUG)
	for(k=0;k<128;k++)
    {
        test_l[k]=0;
        test_u[k]=0;
    }
#endif
	min_l = READMAX;
	max_l = READMIN;
	min_u = READMAX;
	max_u = READMIN;

    REG32(MC_START) = 0;
    REG32(MC_WR_SLICE0) = (REG32(MC_WR_SLICE0)&0xff00ff)| write_l<<8;
    REG32(MC_WR_SLICE1) = (REG32(MC_WR_SLICE1)&0xff00ff)| write_u<<8;

    for(k=READMIN;k<READMAX;k++)
    {
        REG32(MC_START) = 0;
        REG32(MC_RD_SLICE0) = (REG32(MC_RD_SLICE0)&0xff00ff)| k<<8;
        REG32(MC_RD_SLICE1) = (REG32(MC_RD_SLICE1)&0xff00ff)| k<<8;
        REG32(MC_START) = 0x1010100;
        while(((REG32(MC_DLL_SLICE0)&0x1)&(REG32(MC_DLL_SLICE1)&0x1))==0);

#if defined(MM_DEBUG)
        if(++cur_state > 3)
			cur_state = 0;
        print_state(cur_state);
#endif
        for(i=0;i<0x2000;i++);

        l=0; u=0;
        if(sdramtest(0,0,ddrsize)==0)
        {
            l=1;
            if(min_l > k) min_l = k;
            if(max_l < k) max_l = k;
        }
#if defined(MM_DEBUG)		
        if(++cur_state > 3)
			cur_state = 0;
        print_state(cur_state);
#endif
        if(sdramtest(1,0,ddrsize)==0)
        {
            u=1;
            if(min_u > k) min_u = k;
               if(max_u < k) max_u = k;
        }
#if defined(MM_DEBUG)
        test_l[k]=l;
        test_u[k]=u;
#endif
    }
#if 0 //defined(MM_DEBUG)
    serial_puts("\n Show slice 0 read setting\n");
    for(k=READMIN; k< READMAX+1; k++)
    {
        if(test_l[k]==0)
              serial_putc('0');
        else serial_putc('1');
        if(k%16 == 15) serial_putc('\n');
        else serial_putc(' ');
    }

    serial_puts("\n Show slice 1 read setting\n");
    for(k=READMIN; k< READMAX+1; k++)
    {
        if(test_u[k]==0)
              serial_putc('0');
        else serial_putc('1');
        if(k%16 == 15) serial_putc('\n');
        else serial_putc(' ');
    }
#endif
    if( max_l > min_l)
        read_l = (min_l+max_l)>>1;
    else
    {
#if defined(MM_DEBUG)    
        serial_puts("Error005: Could not determine a suitable read DQS delay for slice 0 !\n");
#endif
        goto end;
    }
    if( max_u > min_u)
        read_u = (min_u+max_u)>>1;
    else
    {
#if defined(MM_DEBUG)    
        serial_puts("Error006: Could not determine a suitable read DQS delay for slice 1!\n");
#endif
        goto end;
    }
	REG32(STATUS)==0x12;
#if defined(MM_DEBUG)	
        if(++cur_state > 3)
			cur_state = 0;
        print_state(cur_state);
#endif
/*------------------------- End of Read Data Eye Tuning ---------------------------------*/
/*---------------------------------------------------------------------------------------*/


    REG32(MC_START) = 0;
#if defined(MM_DEBUG)
	serial_puts("\nRead DQS Delay Slice0     :"); print_u32(read_l); serial_putc('\n');
	serial_puts("Read DQS Delay Slice1     :"); print_u32(read_u); serial_putc('\n');
	serial_puts("Write DQS Delay Slice0    :"); print_u32(write_l); serial_putc('\n');
	serial_puts("Write DQS Delay Slice1    :"); print_u32(write_u); serial_putc('\n');
#endif	
    REG32(MC_RD_SLICE0) = (REG32(MC_RD_SLICE0)&0xff00ff)| read_l<<8;
    REG32(MC_RD_SLICE1) = (REG32(MC_RD_SLICE1)&0xff00ff)| read_u<<8;
    REG32(MC_START) = 0x1010100;
    while(((REG32(MC_DLL_SLICE0)&0x1)&(REG32(MC_DLL_SLICE1)&0x1))==0);

    for(i=0;i<0x2000;i++);
    goto store_para;
end:
	//TODO: need to decide what happen if tunning fail.....
	REG32(STATUS) = 0x1f;
	while(1);
	//asc_getc();
store_para:
#ifdef CONFIG_VRX288
	REG32(REG01) = REG32(MC_RD_SLICE0);
	REG32(REG02) = REG32(MC_RD_SLICE1);
	REG32(REG03) = REG32(MC_WR_SLICE0);
	REG32(REG04) = REG32(MC_WR_SLICE1);
#endif
	REG32(STATUS) = 0x2;

start_uboot:
#ifdef CONFIG_BOOT_FROM_NOR	
	for(k=0;k<0x80;k++); // jump delay
	void (*jump)(void);
	jump = (void (*)(void))(0xB0000000);
	for(k=0;k<0x80;k++); // jump delay
	(*jump)();

	while(1);
#endif
	
	return;
	/* TODO: Needs to be updated acc. to final CFG definition! */
}



