OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(reset)
SECTIONS
{
	. = 0x50000000;
	. = ALIGN(4);
	.text	:
	{
	  *(.text)
	}
	. = ALIGN(4);
}
