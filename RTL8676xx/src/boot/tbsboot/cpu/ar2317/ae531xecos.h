//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2003 Atheros Communications, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting the copyright
// holders.
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    Atheros Communications, Inc.
// Contributors: Atheros Engineering
// Date:         2003-10-22
// Purpose:      
// Description:  AR531X ethernet hardware driver
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include "ar2316reg.h"
//#define AR531X_ENET0 AR531XPLUS_ENET0
//#define AR531X_RESET AR531XPLUS_RESET
//#define AR531X_ENABLE AR531XPLUS_ENABLE
#define AR531X_ENET0 AR2316_ENET0
#define AR531X_RESET AR2316_RESET
#define AR531X_ENABLE AR2316_ENABLE
#define  diag_printf printf

//#include "ar531xreg.h"

#define CYGARC_KSEG_MASK                               (0xE0000000)
#define CYGARC_KSEG_CACHED                             (0x80000000)
#define CYGARC_KSEG_UNCACHED                           (0xA0000000)
#define CYGARC_KSEG_CACHED_BASE                        (0x80000000)
#define CYGARC_KSEG_UNCACHED_BASE                      (0xA0000000)
#ifndef __ASSEMBLER__
#define CYGARC_CACHED_ADDRESS(x)                       ((((unsigned int)(x)) & ~CYGARC_KSEG_MASK) | CYGARC_KSEG_CACHED)
#define CYGARC_UNCACHED_ADDRESS(x)                     ((((unsigned int)(x)) & ~CYGARC_KSEG_MASK) | CYGARC_KSEG_UNCACHED)
#define CYGARC_PHYSICAL_ADDRESS(x)                     (((unsigned int)(x)) & ~CYGARC_KSEG_MASK)
#else // __ASSEMBLER__
#define CYGARC_CACHED_ADDRESS(x)                       ((((x)) & ~CYGARC_KSEG_MASK) | CYGARC_KSEG_CACHED)
#define CYGARC_UNCACHED_ADDRESS(x)                     ((((x)) & ~CYGARC_KSEG_MASK) | CYGARC_KSEG_UNCACHED)
#define CYGARC_PHYSICAL_ADDRESS(x)                     (((x)) & ~CYGARC_KSEG_MASK)
#define CYGARC_ADDRESS_REG_CACHED(reg)                 \
        and     reg, reg, ~CYGARC_KSEG_MASK;           \
        or      reg, reg, CYGARC_KSEG_CACHED
#define CYGARC_ADDRESS_REG_UNCACHED(reg)               \
        and     reg, reg, ~CYGARC_KSEG_MASK;           \
        or      reg, reg, CYGARC_KSEG_UNCACHED
#endif /* __ASSEMBLER__ */


#ifndef NULL
#define NULL	((void *)0)
#endif

//#define DEBUG 1 /* TBDXXX */

#if !defined(DEBUG)
//#define DEBUG 0
#endif
typedef char *VIRT_ADDR;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned int  UINT8;
typedef enum {FALSE=0, TRUE=1} BOOL; 


#if DEBUG
#define DEBUG_PRINTF diag_printf
#else
#define DEBUG_PRINTF if (0) diag_printf
#endif
#define PRINTF diag_printf

#define virt_to_bus(vaddr) CYGARC_PHYSICAL_ADDRESS(vaddr)
#define PHYS_TO_K1(physaddr) CYGARC_UNCACHED_ADDRESS(physaddr)
#define KSEG1ADDR(vaddr) PHYS_TO_K1(virt_to_bus(vaddr))

#if 0
#define MALLOC malloc
#define FREE free
#else
#define MALLOC _bogus_malloc
#define FREE(x)
#endif
#define intDisable(old) HAL_DISABLE_INTERRUPTS(old)
#define intEnable(old) HAL_RESTORE_INTERRUPTS(old)
#define sysUDelay(us) HAL_DELAY_US(us)
#define sysMsDelay(ms) HAL_DELAY_US(1000*(ms))
#define LOCAL static

typedef unsigned int AR531X_REG;

#undef sysRegRead
#undef sysRegWrite
#define sysRegRead(phys)	\
	(*(volatile AR531X_REG *)PHYS_TO_K1(phys))

#define sysRegWrite(phys, val)	\
	((*(volatile AR531X_REG *)PHYS_TO_K1(phys)) = (val))

#define sysWbFlush() {*(volatile int *)0xa0002000;}

#define A_DATA_CACHE_INVAL(a,l) HAL_DCACHE_INVALIDATE_ALL()

extern char * enet_mac_address_get(int unit);
