#include <config.h>
#include <version.h>

#include "hardware.h"


.globl lowlevel_init

lowlevel_init:
	/* Save callers address in r11 - r11 must never be modified */
	mov r11, lr

	/*------------------------------------------------------*
	 *mask all IRQs by setting all bits in the INTMR default*
	 *------------------------------------------------------
	mov	r1,	#0xFFFFFFFF
	ldr	r0,	=(CHMCONF_BASE+CHARM_VIC_INTDIS)
	str	r1,	[r0]
*/


	mov	pc,	r11

	/* the literal pools origin */
        .ltorg
