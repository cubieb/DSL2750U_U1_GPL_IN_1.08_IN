/*
 * (C) Copyright 2000-2002
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Definitions for Configuring the monitor commands
 */
#ifndef _CMD_CONFIG_H
#define _CMD_CONFIG_H


/*
 * Configurable monitor commands
 */

#define CFG_CMD_LOADS		0x00000002L	/* loads			*/
#define CFG_CMD_LOADB		0x00000004L	/* loadb			*/
#define CFG_CMD_FLASH           0x00000020L   /* flinfo, erase, protect       */
#define CFG_CMD_MEMORY		0x00000040L	/* md, mm, nm, mw, cp, cmp,	*/
#define CFG_CMD_NET             0x00000080L   /* tftpboot, rarpboot    */
#define CFG_CMD_REGINFO		0x00200000ULL	/* Register dump		*/
#define CFG_CMD_ALL	             0xFFFFFFFFL	/* ALL commands			*/

/* Commands that are considered "non-standard" for some reason
 * (memory hogs, requires special hardware, not fully tested, etc.)
 */
#define CFG_CMD_NONSTD (CFG_CMD_ASKENV	| \
			CFG_CMD_BEDBUG	| \
			CFG_CMD_BMP	| \
			CFG_CMD_BSP	| \
			CFG_CMD_CACHE	| \
			CFG_CMD_CDP	| \
			CFG_CMD_DATE	| \
			CFG_CMD_DHCP	| \
			CFG_CMD_DIAG	| \
			CFG_CMD_DISPLAY	| \
			CFG_CMD_DOC	| \
			CFG_CMD_DTT	| \
			CFG_CMD_EEPROM	| \
			CFG_CMD_ELF	| \
			CFG_CMD_EXT2	| \
			CFG_CMD_FDC	| \
			CFG_CMD_FAT	| \
			CFG_CMD_FDOS	| \
			CFG_CMD_HWFLOW	| \
			CFG_CMD_I2C	| \
			CFG_CMD_IDE	| \
			CFG_CMD_IMMAP	| \
			CFG_CMD_IRQ	| \
			CFG_CMD_JFFS2	| \
			CFG_CMD_KGDB	| \
			CFG_CMD_MII	| \
			CFG_CMD_MMC	| \
			CFG_CMD_NAND	| \
			CFG_CMD_PCI	| \
			CFG_CMD_PCMCIA	| \
			CFG_CMD_PORTIO	| \
			CFG_CMD_REISER	| \
			CFG_CMD_SAVES	| \
			CFG_CMD_SCSI	| \
			CFG_CMD_SDRAM	| \
			CFG_CMD_SNTP	| \
			CFG_CMD_SPI	| \
			CFG_CMD_UNIVERSE | \
			CFG_CMD_USB	| \
			CFG_CMD_VFD	)

/* Default configuration
 */
#define CONFIG_CMD_DFL	(CFG_CMD_ALL & ~CFG_CMD_NONSTD)

#ifndef CONFIG_COMMANDS
#define CONFIG_COMMANDS CONFIG_CMD_DFL
#endif

#endif	/* _CMD_CONFIG_H */
