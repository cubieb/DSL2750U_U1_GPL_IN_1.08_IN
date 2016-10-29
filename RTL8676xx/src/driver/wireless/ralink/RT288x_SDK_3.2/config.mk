
################### Notes: ####################################
# Before include this file, you should define WLAN_CONF_MK_DIR 
# which is the location of this file!!!
###############################################################

###############################################################
# Rootfs settings
###############################################################
export BINDIR:=usr/sbin
export LIBDIR:=lib

###############################################################
# Kernel settings
###############################################################
export OSDIR = ${BUILDDIR}/kernel/linux-2.6.18.8
export PATH_KERNEL = ${OSDIR}

ifndef KERNELRELEASE
VERSION := $(strip $(shell head -n 1 ${PATH_KERNEL}/Makefile | cut -f 2 -d'='))
PATCHLEVEL := $(strip $(shell head -n 2 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
SUBVERSION := $(strip $(shell head -n 3 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
EXTRAVERSION ?= $(strip $(shell head -n 4 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
export KERNELRELEASE := $(strip ${VERSION}.${PATCHLEVEL}.${SUBVERSION}${EXTRAVERSION})
endif

EXTRA_MOD_PATH := net
export MODULEPATH:=lib/modules/${KERNELRELEASE}/$(EXTRA_MOD_PATH)

export PATH_ROOTFS_MOD:=$(FSROOT)/$(MODULEPATH)
export PATH_ROOTFS_BIN:=$(FSROOT)/$(BINDIR)
export PATH_ROOTFS_LIB:=$(FSROOT)/$(LIBDIR)
export PATH_ROOTFS_INC:=$(FSROOT)/usr/include
export PATH_ROOTFS_SCRIPT:=$(FSROOT)/etc/Wireless/ralink
export PATH_ROOTFS_RC:=$(FSROOT)/etc/init.d

