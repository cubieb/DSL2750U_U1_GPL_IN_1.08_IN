#
# Makefile for the Linux VNET layer.
#

obj-$(CONFIG_VNET) += vnet_mod.lib

vnet_mod-objs := vnet.lib vnet_dev.lib vnet_fdb.lib

ifeq ($(CONFIG_PROC_FS),y)
vnet_mod-objs += vnet_proc.lib
endif

