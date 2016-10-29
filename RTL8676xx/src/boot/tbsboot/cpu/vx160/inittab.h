/******************************************************************
 * The table in this file is used during the basic initialization *
 * process. It configures a number of Falcon devices. Please      *
 * don't touch this unless you know what you're doing.            *
 ******************************************************************/
#ifndef _INITTAB_H_
#define _INITTAB_H_

struct vrent
{
    unsigned long      	reg;
    unsigned long      	value;
};

struct vrent ad6834pll[] =			/*vx200*/
{
	/* Disable the watchdog timer */
	{ 0xb9000008, 0x00000A01 },
	
	{ 0xb90000b0, 0x0C000C00 },     /* PCI + PCMCIA enable */
       { 0xb90000B4 , 0x0337f017},
	{ 0xb90000B8 , 0x0000000C},
	{ 0xb90000BC , 0x00020002},
	{ 0xb90000b8 , 0x0001000C },     /* Do PLL reset (first pass only) */  
    /* Disable the watchdog timer again */
	{ 0xb9000008, 0x00000A01 },
	{ 0x00000000, 0x00000000 }			/* terminator */
	
};

struct vrent ad6833pll[] =			/* vx150 */
{
	/* Disable the watchdog timer */
	{ 0xb9000008, 0x00000A01 },

	{ 0xb90000b0, 0x0C000C20 },     /* PCI + PCMCIA enable */
       { 0xb90000B4 , 0x0337f017},
	{ 0xb90000B8 , 0x0000000C},
	{ 0xb90000BC , 0x00080008},
	{ 0xb90000b8 , 0x0001000C },     /* Do PLL reset (first pass only) */  
    /* Disable the watchdog timer again */
	{ 0xb9000008, 0x00000A01 },
	{ 0x00000000, 0x00000000 }			/* terminator */
	
};

struct vrent ad6503pll[] =                      /* at 300 */
{
        /* Disable the watchdog timer */
        { 0xb9000008, 0x00000A01 },

        { 0xb90000b0, 0x04000c00 },     /* PCI + PCMCIA enable */
       { 0xb90000B4 , 0x00001006},
        { 0xb90000B8 , 0x000C000C},
        { 0xb90000BC , 0x0000000a},
        { 0xb90000b8 , 0x00010001 },     /* Do PLL reset (first pass only) */
    /* Disable the watchdog timer again */
        { 0xb9000008, 0x00000A01 },
        { 0x00000000, 0x00000000 }                      /* terminator */

};


struct vrent ad6502pll[] =			/* at 200 */
{
	/* Disable the watchdog timer */
	{ 0xb9000008, 0x00000A01 },

	{ 0xb90000b0, 0x04000c00 },     /* PCI + PCMCIA enable */
       { 0xb90000B4 , 0x00001006},
	{ 0xb90000B8 , 0x000C000C},
	{ 0xb90000BC , 0x0000000a},
	{ 0xb90000b8 , 0x00010001 },     /* Do PLL reset (first pass only) */  
    /* Disable the watchdog timer again */
	{ 0xb9000008, 0x00000A01 },
	{ 0x00000000, 0x00000000 }			/* terminator */
	
};

struct vrent ad6501pll[] =			/* at 100 */
{
	/* Disable the watchdog timer */
	{ 0xb9000008, 0x00000A01 },

	{ 0xb90000b0, 0x04000C00 },     /* PCI + PCMCIA enable */
       { 0xb90000B4 , 0x00000109},
	{ 0xb90000B8 , 0x000C000C},
	{ 0xb90000BC , 0x0000000a},
	{ 0xb90000b8 , 0x00010001 },     /* Do PLL reset (first pass only) */  
    /* Disable the watchdog timer again */
	{ 0xb9000008, 0x00000A01 },
	{ 0x00000000, 0x00000000 }			/* terminator */
	
};

struct vrent ad6836pll[] =			/* 160 */
{
	{ 0xb90000b0, 0x00010C00 },     /* PCI + PCMCIA enable */
    { 0xb90000B4 , 0x0000200f},
	{ 0xb90000B8 , 0x00000009},
	{ 0xb90000BC , 0x0000020f},
	{ 0xb90000C8 , 0x000f045f},
	{ 0xb90000b8 , 0x00010009},     /* Do PLL reset (first pass only) */  
	{ 0xb9250010,  0x00000003},
	{ 0x00000000, 0x00000000 }			/* terminator */
	
};

struct vrent reginittab[] =
{
    /* move to processor pll setting */
#if 0

    /* Setup the chip mode select. Controls pin mux */
    /*{ 0xb90000b0, 0x04000440 },*/ /* PCI + PCMCIA enable */
//    { 0xb90000b0, 0x0C000C01 },     /* PCI + PCMCIA enable */
        { 0xb90000b0, 0x00000C00 },     /* PCI + PCMCIA enable */
    /* Program the PLL. During this programming the device will be
     * automatically reset if this is the first time they have been
     * set since power up
     */
    { 0xb90000b4, 0x0337f017 },     /* Proc PLL * 24 /4 for CPU, APU */
    { 0xb90000b8, 0x0000000C },     /* Backup PLL */
    /*{ 0xb90000bc, 0x019A019A },*/     /* DSP PLL */
    { 0xb90000bc, 0x00020002 },     /* DSP PLL */
    { 0xb90000b8, 0x0001000C },     /* Do PLL reset (first pass only) */
#endif
	/* Asynchronous Memory Controller */
	{ 0xb9148004, 0x77aa77a2 },     /* 70 nS flash @ 100 MHz bus clk */
	{ 0xb9148008, 0xffc2ffc2 },
	{ 0xb9148000, 0x000000f6 },


	/* SDRAM Controller Init entries, Don't change here. Auto detect */
    { 0xb9140008, 0x000001AC },
    { 0xb9140004, 0x00000000 },
    { 0xb9140000, 0x00d91148 },

    /* Enable clocks for all peripherals */
    { 0xb9050a00, 0xffffffff },
    { 0xb9050a04, 0xffffffff },
    { 0xb9050a08, 0xffffffff },
    { 0xb9050a0c, 0xffffffff },
    { 0xb9050a10, 0xffffffff },
    { 0xb9050a14, 0xffffffff },
    { 0xb9050a18, 0xffffffff },

    { 0x00000000, 0x00000000 }			/* terminator */
    };

#endif
