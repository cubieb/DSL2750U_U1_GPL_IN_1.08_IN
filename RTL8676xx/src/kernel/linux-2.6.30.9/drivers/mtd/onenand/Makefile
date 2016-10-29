#
# Makefile for the OneNAND MTD
#

# Core functionality.
obj-$(CONFIG_MTD_ONENAND)		+= onenand.lib

# Board specific.
obj-$(CONFIG_MTD_ONENAND_GENERIC)	+= generic.lib
obj-$(CONFIG_MTD_ONENAND_OMAP2)		+= omap2.lib

# Simulator
obj-$(CONFIG_MTD_ONENAND_SIM)		+= onenand_sim.lib

onenand-objs = onenand_base.lib onenand_bbt.lib
