/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * Copyright (C) 2001  Erik Mouw (J.A.K.Mouw@its.tudelft.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <zlib.h>
#include <asm/byteorder.h>

DECLARE_GLOBAL_DATA_PTR;

#if defined (CONFIG_SETUP_MEMORY_TAGS) || defined (CONFIG_CMDLINE_TAG)

	static void setup_start_tag (bd_t *bd);
        
# ifdef CONFIG_SETUP_MEMORY_TAGS
	static void setup_memory_tags (bd_t *bd);
# endif         
	static void setup_commandline_tag (bd_t *bd, char *commandline);
	static void setup_end_tag (bd_t *bd);

	static struct tag *params;
#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG */


int do_bootm_linux(unsigned int src)
{

	unsigned int entry;
	void (*theKernel)(void);
	int ret = -1;
	
	entry = (unsigned int)(CONFIG_DRAM_BASE + KERNEL_OFFSET);
	printf ("Uncompress linux from %#x to %#x... ", src, entry);
	if(0 == src)
		{
		goto err;
		}
	ret = uncompressLZMA((Bytef *)entry, (Bytef *)src);
	theKernel = (void (*)(void))entry;	   
	if(0 != ret)
		{
		printf("Failed!\n");
		}
	else
		{/* we assume that the kernel is in place */
		printf("OK!\nBooting the kernel ...\n");
//		cleanup_before_linux();
		theKernel();
		}
err:	
	return ret;
}

static void setup_commandline_tag (bd_t *bd, char *commandline, void *sys_data, int syscfg_version)
{
	char *p;
	char partinfo[128];
	struct sys_cfg *syscfg;
	struct sys_config *oldcfg;

	syscfg = (struct sys_cfg *)sys_data;
	oldcfg = (struct sys_config *)sys_data;
	if(syscfg_version == 0)
		{
		if((0 == oldcfg->image_mark) && (0 != oldcfg->first_image_len))
			{
	 		sprintf(partinfo, "%d(boot),%d(kernel),-(rootfs)", 
				oldcfg->layout.zone_offset[ZONE_KERNEL_FIRST] - oldcfg->layout.zone_offset[ZONE_BOOTLOADER], 
				oldcfg->layout.zone_offset[ZONE_ROOTFS_FIRST] - oldcfg->layout.zone_offset[ZONE_KERNEL_FIRST]);
			}
		else if((1 == oldcfg->image_mark) && (0 != oldcfg->second_image_len))
			{
			sprintf(partinfo,  "%d(boot),%d(kernel),-(rootfs)",	
				oldcfg->layout.zone_offset[ZONE_KERNEL_SECOND] - oldcfg->layout.zone_offset[ZONE_BOOTLOADER],
                oldcfg->layout.zone_offset[ZONE_ROOTFS_SECOND] - oldcfg->layout.zone_offset[ZONE_KERNEL_SECOND]);
			}
		}
	else
		{
		
		if((0 == syscfg->boot_img_flag) && (0 != syscfg->first_kernel_len) && (0 != syscfg->first_rootfs_len))
			{
	 		sprintf(partinfo, "%d(boot),%d(kernel),-(rootfs)", syscfg->first_kernel_offset,	syscfg->first_rootfs_offset - syscfg->first_kernel_offset);
			}
		else if((1 == syscfg->boot_img_flag) && (0 != syscfg->second_kernel_len) && (0 != syscfg->second_rootfs_len))

			{
			sprintf(partinfo,  "%d(boot),%d(kernel),-(rootfs)",	syscfg->second_kernel_offset, syscfg->second_rootfs_offset - syscfg->second_kernel_offset);
			}
		}

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + strlen(partinfo)+ 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);
	strcat (params->u.cmdline.cmdline, partinfo);

    	//printf("cmdline=\"%s\"\n",params->u.cmdline.cmdline);

	params = tag_next (params);
}

int prepare_tags(void *sys_data, int syscfg_version)
{
	bd_t *bd = gd->bd;

#ifdef CONFIG_CMDLINE_TAG
	char *commandline = { CONFIG_BOOTARGS };
#endif

	/* pass tags for Linux kernel */

#if defined (CONFIG_SETUP_MEMORY_TAGS) || defined (CONFIG_CMDLINE_TAG) 
    
	bd->bi_boot_params = BOOTARGS_OFFSET;
	
	setup_start_tag (bd);

#ifdef CONFIG_SETUP_MEMORY_TAGS
	setup_memory_tags (bd);
#endif
#ifdef CONFIG_CMDLINE_TAG
	setup_commandline_tag (bd, commandline, sys_data, syscfg_version);
#endif

	setup_end_tag (bd);
#endif

	return 0;
}

#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) 

static void setup_start_tag (bd_t *bd)
{
	params = (struct tag *) bd->bi_boot_params;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}

#ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd)
{
	int i;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		params->hdr.tag = ATAG_MEM;
		params->hdr.size = tag_size (tag_mem32);

		params->u.mem.start = bd->bi_dram[i].start;
		params->u.mem.size = bd->bi_dram[i].size;

		params = tag_next (params);
	}
}
#endif /* CONFIG_SETUP_MEMORY_TAGS */

static void setup_end_tag (bd_t *bd)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG */
