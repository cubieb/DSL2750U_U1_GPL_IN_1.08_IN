
include $(ROOTDIR)/build/arch.mk

export PATH:=$(TOOLCHAINS_PATH):$(PATH)

############################################################### 
# Kernel settings 
############################################################### 
 
export PATH_KERNEL=$(ROOTDIR)/build/kernel/linux-2.6.18.8
export PATH_KERNEL_INCLUDES=${PATH_KERNEL}/include

export KMAKE:=$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) DEPMOD=/bin/true KBUILD_OUTPUT= V=1

#ifndef KERNELRELEASE
VERSION := $(strip $(shell head -n 1 ${PATH_KERNEL}/Makefile | cut -f 2 -d'='))
PATCHLEVEL := $(strip $(shell head -n 2 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
SUBVERSION := $(strip $(shell head -n 3 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
EXTRAVERSION ?= $(strip $(shell head -n 4 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
export KERNELRELEASE := $(strip ${VERSION}.${PATCHLEVEL}.${SUBVERSION}${EXTRAVERSION})
#endif


###################################################################
# Install Dir Path
###################################################################
export PATH_ROOTFS=$(ROOTDIR)/build/romfs/rootfs

export BINDIR:=usr/sbin
export LIBDIR:=usr/lib

EXTRA_MOD_PATH := 
export MODULEPATH:=lib/modules/2.6.18.8/$(EXTRA_MOD_PATH)

export PATH_ROOTFS_MOD:=$(PATH_ROOTFS)/$(MODULEPATH)
export PATH_ROOTFS_BIN:=$(PATH_ROOTFS)/$(BINDIR)
export PATH_ROOTFS_LIB:=$(PATH_ROOTFS)/$(LIBDIR)
export PATH_ROOTFS_INC:=$(PATH_ROOTFS)/usr/include
export PATH_ROOTFS_ETC:=$(PATH_ROOTFS)/etc/
export PATH_ROOTFS_RC:=$(PATH_ROOTFS)/etc/init.d
