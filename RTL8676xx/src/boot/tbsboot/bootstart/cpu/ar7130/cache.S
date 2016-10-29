

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/cacheops.h>



	.globl	mips_cache_reset	
mips_cache_reset:

	mfc0	v0,$12
	nop
	nop
	nop
	lui	v1,0xffff
	ori	v1,v1,0xff
	and	v0,v0,v1
	mtc0	v0,$12
	nop
	nop
	nop

	mtc0	zero,$28
	nop
	nop
	nop
	mtc0	zero,$29
	nop
	nop
	nop
	lui	a0,0x8000
	lui	v1,0x8000
	ori	a0,a0,0xffff
1:	cache	0x8,0(v1)
	addiu	v1,v1,32
	sltu	v0,a0,v1
	beqz	v0,1b
	nop

	mtc0	zero,$28
	nop
	nop
	nop
	mtc0	zero,$29
	nop
	nop
	nop
	lui	a0,0x8000
	lui	v1,0x8000
	ori	a0,a0,0x7fff
1:	cache	0x9,0(v1)
	addiu	v1,v1,32
	sltu	v0,a0,v1
	beqz	v0,1b
	addiu	a1,s2,32
	lui  v0 ,0x8020
	ori  v0 ,v0,0x8480
	mtc0  v0 ,CP0_CONFIG
	nop
	nop
	nop
	j	ra
	
