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

struct vrent vx180pll[] =                      /* 160 */
{
#if 0
/* Need to modify for VX180 reference */
	{ 0xb90000b0, 0x00010C20 },     /* PCI + PCMCIA enable */
	{ 0xb90000B4 , 0x0000200f},
	{ 0xb90000B8 , 0x00000009},
	{ 0xb90000BC , 0x0000020f},
	{ 0xb90000C8 , 0x000f045f},
	{ 0xb90000b8 , 0x00010009}     /* Do PLL reset (first pass only) */
	{ 0xb9250010,  0x00000003}
#endif
	{ 0xb9268000, 0x00010C20 },     /* Dummy write  */
	{ 0x00000000, 0x00000000 }                      /* terminator */

};

struct vrent reginittab[] =
{

        { 0xb9148004, 0xffc2ffc2 },     /* 90 nS flash @ 166 MHz bus clk */
	{ 0xb9148008, 0xffc2ffc2 },
	{ 0xb9148000, 0x000000f6 },

#if 0
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
#endif

    { 0x00000000, 0x00000000 }			/* terminator */
    };
#endif

