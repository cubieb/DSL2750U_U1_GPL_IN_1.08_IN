/*
 * (C) Copyright 2007
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <version.h>
#include <flash.h>
#include <net.h>
#include <flash_layout_private.h>
#include <tbs_common.h>

flash_info_t *info;
extern flash_info_t flash_info[CFG_MAX_FLASH_BANKS];           /* FLASH chips info */

extern int timer_init(void);

extern ulong uboot_end_data;
extern ulong uboot_end;

ulong monitor_flash_len;
const char version_string[] = U_BOOT_VERSION" Build" TBS_VERSION " for " PRODUCT "("__DATE__ "-" __TIME__")";

/* Begin and End of heap memory for malloc(), and current "brk" */
static unsigned long heap_start;
static unsigned long heap_end;
static unsigned long heap_brk;

void *sbrk(ptrdiff_t increment)
{
	unsigned long old = heap_brk;
	unsigned long new_addr = old + increment;

	if((new_addr < heap_start) || (new_addr > heap_end))
	{
		printf("Failed to allocate memory!\n");
		return (NULL);
	}
	heap_brk = new_addr;
	return ((void *)old);
}

static int init_func_ram(unsigned long memsize)
{
	puts("DRAM:  ");
	if(memsize > 0)
		{
		print_size(memsize, "\n");
		gd->bi_memstart	= CONFIG_DRAM_BASE;	/* start of  DRAM memory */
		gd->bi_memsize	= memsize;		/* size  of  DRAM memory in bytes */
		return 0;
		}
	else
		{
		puts("Failed!\n");
		return -1;
		}
}

void hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}

/************************************************************************
 *
 * This is the next part if the initialization sequence: we are now
 * running from RAM and have a "normal" C environment, i. e. global
 * data can be written, BSS has been cleared, the stack size in not
 * that critical any more, etc.
 *
 ************************************************************************
 */
void board_init_r(unsigned long memsize)
{
	cmd_tbl_t *cmdtp;
	unsigned long addr;
	int ret_val = -1;
	
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");
	gd = (struct global_data *)(CONFIG_DRAM_BASE + memsize - BOOT_RUNTIME_SIZE + GLOBAL_DATA_OFFSET);
	memset((void *)gd, 0x00, sizeof(struct global_data));
	timer_init();
	gd->bi_baudrate = (unsigned long)(CONFIG_BAUDRATE);
	if(0 == serial_init())
		{
		gd->have_console = 1;
		}
	printf ("\n\n%s\n\n", version_string);		/* say that we are here */	
	ret_val = init_func_ram(memsize);
	if(ret_val < 0)
		{
		goto error;
		}
	init_led();
	gd->flags |= GD_FLG_RELOC;	/* tell others: relocation done */	
	gd->reloc_off = 0;	
	monitor_flash_len = (ulong)&uboot_end_data - CONFIG_TEXT_BASE;	
 	for (cmdtp = &__u_boot_cmd_start; cmdtp !=  &__u_boot_cmd_end; cmdtp++)
		{
		addr = (ulong) (cmdtp->cmd) + gd->reloc_off;	
		debug ("Command \"%s\": 0x%08lx => 0x%08lx\n", cmdtp->name, (ulong) (cmdtp->cmd), addr);	
		cmdtp->cmd = (int (*)(struct cmd_tbl_s *, int, int, char *[]))addr;
		if(cmdtp->usage)
			{
			addr = (ulong)(cmdtp->usage) + gd->reloc_off;
			cmdtp->usage = (char *)addr;
			}
		#ifdef	CFG_LONGHELP
		if(cmdtp->help)
			{
			addr = (ulong)(cmdtp->help) + gd->reloc_off;
			cmdtp->help = (char *)addr;
			}
		#endif
 		}

	/* initialize heap memory */
	heap_start = (unsigned long)(gd->bi_memstart + gd->bi_memsize - BOOT_RUNTIME_SIZE + HEAP_OFFSET);
	heap_end = heap_start + HEAP_SIZE;
	heap_brk = heap_start;
	memset((void *)heap_start, 0, HEAP_SIZE);	
	alloc_init((unsigned char *)(heap_start + HEAP_SIZE / 2));
	debug("Heap start at %#x, end at %#x, Brk at %#x\n", heap_start, heap_end, heap_brk);
	debug("Global date points to %#x\n", (unsigned int)gd);
	gd->bi_flashstart = CONFIG_FLASH_BASE;	
#if (CFG_MONITOR_BASE == CONFIG_FLASH_BASE)
	gd->bi_flashoffset = monitor_flash_len;	/* reserved area for U-Boot */
#else
	gd->bi_flashoffset = 0;
#endif
	gd->bi_flashsize = flash_init();
	gd->syscfg_addr = (unsigned long)(CONFIG_DRAM_BASE + gd->bi_memsize - BOOT_RUNTIME_SIZE + SYSTEMCFG_OFFSET);
	gd->syscfg_ver = system_config_map(gd->syscfg_addr);
	/* main loop can return to retry autoboot, if so just run it again. */
	for(;;)
		{
		main_loop();
		}
error:
	hang();
}

