#
# Makefile for the memory technology device drivers.
#

# Core functionality.
obj-$(CONFIG_MTD)		+= mtd.lib
mtd-y				:= mtdcore.lib mtdsuper.lib mtdbdi.lib
mtd-$(CONFIG_MTD_PARTITIONS)	+= mtdpart.lib

obj-$(CONFIG_MTD_CONCAT)	+= mtdconcat.lib
obj-$(CONFIG_MTD_REDBOOT_PARTS) += redboot.lib
obj-$(CONFIG_MTD_CMDLINE_PARTS) += cmdlinepart.lib
obj-$(CONFIG_MTD_AFS_PARTS)	+= afs.lib
obj-$(CONFIG_MTD_AR7_PARTS)	+= ar7part.lib
obj-$(CONFIG_MTD_OF_PARTS)      += ofpart.lib

# 'Users' - code which presents functionality to userspace.
obj-$(CONFIG_MTD_CHAR)		+= mtdchar.lib
obj-$(CONFIG_MTD_BLKDEVS)	+= mtd_blkdevs.lib
obj-$(CONFIG_MTD_BLOCK)		+= mtdblock.lib
obj-$(CONFIG_MTD_BLOCK_RO)	+= mtdblock_ro.lib
obj-$(CONFIG_FTL)		+= ftl.lib
obj-$(CONFIG_NFTL)		+= nftl.lib
obj-$(CONFIG_INFTL)		+= inftl.lib
obj-$(CONFIG_RFD_FTL)		+= rfd_ftl.lib
obj-$(CONFIG_SSFDC)		+= ssfdc.lib
obj-$(CONFIG_MTD_OOPS)		+= mtdoops.lib

nftl-objs		:= nftlcore.lib nftlmount.lib
inftl-objs		:= inftlcore.lib inftlmount.lib

obj-y		+= chips/ lpddr/ maps/ devices/ nand/ onenand/ tests/
obj-$(CONFIG_MTD_TBS_FLASH_LAYOUT) += mtd_tbs/
obj-$(CONFIG_MTD_UBI)		+= ubi/
