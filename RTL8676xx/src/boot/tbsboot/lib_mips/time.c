/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#if defined(CONFIG_VX160) || defined(CONFIG_VX180)

static u32 softtimer = 0;

void mips_count_set(ulong v)
{
	softtimer = v>>32;
}


inline u32 mips_count_get(void)
{

	return *((unsigned int *)0xb9070008) + (*((unsigned int *)0xb907000c) << 16);
	
}

/*
 * timer without interrupts
 */

int timer_init(void)
{
    *((volatile unsigned int *)0xb9050108) = 0x1f1f1f1f;	
    /* disable Timer 0 */
    *((volatile unsigned short *)0xb9070000) = 0x0200;

    *((volatile unsigned short *)0xb9070004) = 0x0005;

    /* Timer0 Period Low Register */
    *((volatile unsigned short *)0xb9070010) = 0xFFFE;

    /* Timer0 Period High Register */
    *((volatile unsigned short *)0xb9070014) = 0xFFFF;

    /* Timer0 Width High Register */
    *((volatile unsigned short *)0xb907001c) = 0xFFFF;
 
    /* Timer0 Width Low Register */
    *((volatile unsigned short *)0xb9070018) = 0xFFFC;

    /* Enable Timer 0 */
    *((volatile unsigned short *)0xb9070000) = 0x0100;

	return 0;
}

void reset_timer(void)
{
	mips_count_set(0);
}

ulong get_timer(ulong base)
{
	u32	count;
		
	count = mips_count_get();
	if(count  == 0xFFFFFFFD) {
		softtimer ++;
		*((unsigned short *)0xb9070000) = 0x0100;
		count = 0;
		debug("%d %x \n",softtimer, count);
		
	}
	return (softtimer<<16) + (count>>16) -base;
}

void udelay (unsigned long usec)
{
	register u32 tmo;
#if defined(CONFIG_VOX180) ||  defined(CONFIG_VOX170)
	tmo = usec * 200;
#else
	tmo = usec * 100;
#endif
	while (tmo--);
	
}

#elif defined(CONFIG_RTL8196C)

int timer_init(void)
{
	/* disable clock */
	*((unsigned long *)CDBR) = 0x00000000;
	*((unsigned long *)TCCNR) = 0x00000000;
	/* timer 1 interval as 0.5 ms  */
	*((unsigned long *)TC1DATA) = (1000*4*60*60) << 8; /* 1 hour */
	*((unsigned long *)CDBR) = (CLOCKGEN_FREQ/10000/8) << 16;
	*((unsigned long *)TCCNR) = (1<<28)|(1<<29);
	return 0;
}

unsigned long mips_count_get(void)
{
	//tylo, 8672 fpga
	return (((*((unsigned long *)TC1CNT)>>8))/4);
	//return (((1000*4*60*60) - (*((unsigned long *)TC1CNT)>>8))/4);
}

ulong get_timer(ulong base)
{
	return mips_count_get() - base;
}

int timeout(unsigned long start, unsigned long ms)
{
	unsigned long cur_ms = mips_count_get();
	if(cur_ms>=start)
		if((cur_ms-start) > ms)
			return 1;
	if(cur_ms<start)	/* roll over */
		if((cur_ms+(1000*60*60)-start) > ms)
			return 1;
	return 0;
}

void delay_msec(unsigned long ms)
{
	unsigned long start = mips_count_get();
	while(!timeout(start,ms));
	
}

void udelay (unsigned long usec)
{
	delay_msec(usec/1000);
}

#elif defined(CONFIG_VRX288)
static inline void mips_compare_set(u32 v)
{
        asm volatile ("mtc0 %0, $11" : : "r" (v));
}

static inline void mips_count_set(u32 v)
{
        asm volatile ("mtc0 %0, $9" : : "r" (v));
}


static inline u32 mips_count_get(void)
{
        u32 count;

        asm volatile ("mfc0 %0, $9" : "=r" (count) :);
        return count;
}

/*
 * timer without interrupts
 */

int timer_init(void)
{
        mips_compare_set(0);
        mips_count_set(0);

        return 0;
}

void reset_timer(void)
{
        mips_count_set(0);
}

ulong get_timer(ulong base)
{
        return mips_count_get() - base;
}

void udelay (unsigned long usec)
{
        ulong tmo;
        ulong start = get_timer(0);

        tmo = usec * (CFG_HZ / 1000000);
        while ((ulong)((mips_count_get() - start)) < tmo)
                /*NOP*/;
}

#elif defined(CONFIG_RTL8676)
unsigned int CLOCKGEN_FREQ;
int timer_init(void)
{
	u32 temp;
	/* disable clock */
	REGWRITE32(CDBR, 0x0);
	REGWRITE32(TCCNR, 0x0);
	/* timer 1 interval as 0.5 ms  */
	REGWRITE32(TC1DATA, ((1000*4*60*60) << 8)); /* 1 hour */
	temp = (REG32(0xB8000200) & 0x01F00) >> 8;
	CLOCKGEN_FREQ = (temp + 2) * 10000000;
	REGWRITE32(CDBR, ((CLOCKGEN_FREQ/1000/4) << 16));
	REGWRITE32(TCCNR, ((1<<28)|(1<<29)));
	return 0;
}

unsigned long mips_count_get(void)
{
	return ((REG32(TC1CNT) >> 8) / 4);
}

ulong get_timer(ulong base)
{
	return mips_count_get() - base;
}

int timeout(unsigned long start, unsigned long ms)
{
	unsigned long cur_ms = mips_count_get();
	
	if(cur_ms >= start)
		{
		if((cur_ms-start) > ms)
			return 1;
		}
	else
		{/* roll over */
		if((cur_ms+(1000*60*60)-start) > ms)
			return 1;
		}
	return 0;
}

void delay_msec(unsigned long ms)
{
	unsigned long start = mips_count_get();
	while(!timeout(start,ms));
	
}

void udelay (unsigned long usec)
{
	delay_msec(usec/1000);
}


#else /* For general MIPS core */

static inline void mips_compare_set(u32 v)
{
	asm volatile ("mtc0 %0, $11" : : "r" (v));
}

static inline void mips_count_set(u32 v)
{
	asm volatile ("mtc0 %0, $9" : : "r" (v));
}


static inline u32 mips_count_get(void)
{
	u32 count;

	asm volatile ("mfc0 %0, $9" : "=r" (count) :);
	return count;
}

/*
 * timer without interrupts
 */

int timer_init(void)
{
	mips_compare_set(0);
	mips_count_set(0);

	return 0;
}

void reset_timer(void)
{
	mips_count_set(0);
}

ulong get_timer(ulong base)
{
	return mips_count_get() - base;
}

void udelay (unsigned long usec)
{
	ulong tmo;
	ulong start = get_timer(0);

	tmo = usec * (CFG_HZ / 1000000);
	while ((ulong)((mips_count_get() - start)) < tmo)
		/*NOP*/;
}

void set_timer(unsigned long t)
{
	mips_count_set(t);
}

#endif

void mdelay(unsigned long msec)
{
	int i;

	for(i=0;i<msec;i++)
		{
		udelay(1000);
		}
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On MIPS it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return mips_count_get();
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On MIPS it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return CFG_HZ;
}
