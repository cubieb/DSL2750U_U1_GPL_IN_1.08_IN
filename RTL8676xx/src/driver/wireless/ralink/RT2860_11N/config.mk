
################### Notes: ####################################
# Before include this file, you should define WLAN_CONF_MK_DIR 
# which is the location of this file!!!
#       Otherwise, the paths definitions in this file is not right.
#       Examples:
#            WLAN_CONF_MK_DIR:=$(shell pwd)
#            include $(WLAN_CONF_MK_DIR)/config.mk
#       If in subdirectory, you should use following example:
#            ifeq ($(WLAN_CONF_MK_DIR),) 
#               WLAN_CONF_MK_DIR:=$(CURDIR)/../..
#               include $(WLAN_CONF_MK_DIR)/config.mk
#            endif
###############################################################

###############################################################
# Kernel settings
###############################################################

#TBS platform kernel source code 
export PATH_KERNEL=$(ROOTDIR)/build/kernel/linux-2.6.18.8

export PATH_KERNEL_INCLUDES=${PATH_KERNEL}/include

export EXTRA_CFLAGS += -I$(PATH_KERNEL_INCLUDES) -I$(ROOTDIR)/build/driver/dsl/ikanos/ap_code/inc -I$(ROOTDIR)/build/include
KERNELARCH:=$(ARCH)

ifndef KERNELRELEASE
VERSION := $(strip $(shell head -n 1 ${PATH_KERNEL}/Makefile | cut -f 2 -d'='))
PATCHLEVEL := $(strip $(shell head -n 2 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
SUBVERSION := $(strip $(shell head -n 3 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
EXTRAVERSION ?= $(strip $(shell head -n 4 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
export KERNELRELEASE := $(strip ${VERSION}.${PATCHLEVEL}.${SUBVERSION}${EXTRAVERSION})
endif

###############################################################
# Rootfs settings
###############################################################

export PATH_ROOTFS=$(ROOTDIR)/build/romfs/rootfs
	
export BINDIR:=usr/sbin
export LIBDIR:=lib

EXTRA_MOD_PATH := kernel/drivers/net
export MODULEPATH:=lib/modules/${KERNELRELEASE}/$(EXTRA_MOD_PATH)

export PATH_ROOTFS_MOD:=$(PATH_ROOTFS)/$(MODULEPATH)
export PATH_ROOTFS_BIN:=$(PATH_ROOTFS)/$(BINDIR)
export PATH_ROOTFS_LIB:=$(PATH_ROOTFS)/$(LIBDIR)
export PATH_ROOTFS_INC:=$(PATH_ROOTFS)/usr/include
export PATH_ROOTFS_SCRIPT:=$(PATH_ROOTFS)/etc/ath
export PATH_ROOTFS_RC:=$(PATH_ROOTFS)/etc/init.d
export CONFIG_PATH=$(PATH_ROOTFS)/etc/ath
export DEFAULT_CFG=$(PATH_ROOTFS)/etc/ath/default

export FSROOT:=$(PATH_ROOTFS)
export TFTPPATH=/tftpboot/`whoami`
###############################################################
# Other Settings
###############################################################

#install art module and tools or not?
INSTALL_ART_TOOLS:=0

###############################################################
# For compatible with original ralink driver makefile
###############################################################

export TOOLPREFIX:=$(CROSS_COMPILER)
export PREFIX:=$(PATH_ROOTFS)
export DESTDIR:=$(PATH_ROOTFS)

export KERNELPATH=$(PATH_KERNEL)

###############################################################
# Should not change below settings
###############################################################

#PATH Definitions
export PATH_CROSS_GCC=$(CROSS_COMPILER_PATH)/bin
export PATH_GCC_INCLUDES=$(CROSS_COMPILER_PATH)/include
export PATH_GCC_LIBS=$(CROSS_COMPILER_PATH)/lib
export PATH_GCC_STATIC_LIBS:=$(CROSS_COMPILER_PATH)/mipseb-linux-uclibc/sys-root/usr/lib

export PATH:=$(PATH_CROSS_GCC):$(PATH)

export CC=$(CROSS_COMPILER)gcc
export STRIP=$(CROSS_COMPILER)strip
export CXX = $(CROSS_COMPILER)g++
export LD = $(CROSS_COMPILER)ld
export AR = $(CROSS_COMPILER)ar
export RANLIB = $(CROSS_COMPILER)ranlib

STRIPFLAGS	= --strip-all --remove-section=.note --remove-section=.comment
export STRIPCMD	=$(STRIP) $(STRIPFLAGS)
export KSTRIPCMD	=$(STRIP) --strip-debug 

export KMAKE:=$(MAKE) ARCH=$(KERNELARCH) CROSS_COMPILE=$(CROSS_COMPILER)

export KMAKE:=$(MAKE) ARCH=$(KERNELARCH) CROSS_COMPILE=$(TOOLPREFIX) KBUILD_OUTPUT= DEPMOD=/bin/true

#Wireless Modules definitions
#DO NOT MODIFY THE FOLLOWING

#Libraries Path
export PATH_OPENSSL=$(ROOTDIR)/build/tmpdist/


#madwifi
export TARGET=$(MADWIFITARGET)
export MADWIFITARGET=mipsisa32-be-elf
export BUS=PCI
export AR5416_G_MODE=1

# Set Phy Err Diagnostics (Radar detection) to be enabled for AP builds
export ATH_CAP_PHYERR_DIAG=1

# flag for TX99
export ATH_CAP_TX99=1


###############################################################################
# vx180 config
###############################################################################
# Establish working directory and pathnames.
MADWIFI:=$(WLAN_CONF_MK_DIR)/wlan/madwifi
APPSPATH:=$(WLAN_CONF_MK_DIR)/apps

# The Madwifi Makefiles must have environment varibles
# HAL and ATH_RATE defined for properly compiling
export HAL=$(MADWIFI)/hal/main

ifeq ($(CPU),vx180)
export TARGET=mipsisa32-be-elf
endif

ifeq ($(CPU),vx160)
export TARGET=mips1-be-elf
endif
