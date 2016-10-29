PRODUCT?=AR7130_GAW2.2Z56-B

ifeq ($(PRODUCT),SOLOSW_GAW1.4Z53-5)
export CONFIG_WLAN_SOLOSW=y
export BOARD_TYPE=solosw
endif

ifeq ($(PRODUCT),AR7130_GAW2.2Z56-B)
export CONFIG_WLAN_AR7130=y
export BOARD_TYPE=ar7130
endif

ifeq ($(PRODUCT),AR7130_AP699E7.2U10-4)
export CONFIG_WLAN_AR7130=y
export BOARD_TYPE=ar7130
endif

ifeq ($(PRODUCT),AR7130_AP699E7G.2Z56-3)
export CONFIG_WLAN_AR7130=y
export BOARD_TYPE=ar7130
endif


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
#flags for local test setting or for top build setting
#may exist in Makefile in up folder
TBS_TOP_BUILD=y

#add by liwei for sta and ap info
SHOW_AP_STA_INFO=y

TBS_DIR=$(WLAN_CONF_MK_DIR)/../../../../../


###############################################################
# Debug Settings
#		ATH_PCI_DEBUG:  Debug ath_pci.ko
#		ATH_HAL_DEBUG:  Debug ath_hal.ko
#		ATH_DFS_DEBUG:  Debug ath_dfs.ko
#		ATH_RATE_DEBUG:  Debug ath_rate_atheros.ko
#		NET80211_DEBUG: Debug wlan*.ko
###############################################################

export ATH_PCI_DEBUG:=0
export ATH_HAL_DEBUG:=0
export ATH_DFS_DEBUG:=0 
export ATH_RATE_DEBUG:=0
export ATH_PKGLOG_DEBUG:=0
export ATH_RC_DEBUG:=0
export NET80211_DEBUG:=0

export DEBUG_COPTS:= -g -ggdb
export RELEASE_COPTS:= -O2


###############################################################
# Cross Compiler Tools Settings
###############################################################

#TBS platform cross compile
CROSS_COMPILER_PATH=$(TBS_DIR)/crossc/mips.eb
export CROSS_COMPILER=mipseb-linux-uclibc-

###############################################################
# Kernel settings
###############################################################

#TBS platform kernel source code 
ifeq ($(TBS_TOP_BUILD),y)
	export PATH_KERNEL=$(TBS_DIR)/build/kernel/linux-2.6.18.8
else
	export PATH_KERNEL=/opt/atheros/linux-2.6.18.8.$(WLAN_DRIVER_MODE)
endif

export PATH_KERNEL_INCLUDES=${PATH_KERNEL}/include
KERNELARCH:=mips
export ARCH:=mips
export CPU:=ar7130
ROOTDIR:=$(TBS_DIR)

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

ifeq ($(TBS_TOP_BUILD),y)
	export PATH_ROOTFS=$(TBS_DIR)/build/romfs/rootfs
else	
	export PATH_ROOTFS=/nfsroot/atheros.$(WLAN_DRIVER_MODE)
endif
	
export BINDIR:=usr/sbin
export LIBDIR:=lib

EXTRA_MOD_PATH := net
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
# For compatible with original atheros driver makefile
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
ifeq ($(MOTHER_BOARD),broadcom)
export PATH_GCC_STATIC_LIBS:=$(CROSS_COMPILER_PATH)/mips-linux/lib
export COPTS += -DBROADCOM_PLATFORM
export CFLAGS += -DBROADCOM_PLATFORM
else
export PATH_GCC_STATIC_LIBS:=$(CROSS_COMPILER_PATH)/mipseb-linux-uclibc/sys-root/usr/lib
endif

#add by liwei for show ap and sta info
export COPTS += -DSHOW_AP_STA_INFO


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

ifeq ($(TBS_TOP_BUILD),y)
	export KMAKE:=$(MAKE) ARCH=$(KERNELARCH) CROSS_COMPILE=$(CROSS_COMPILER) KBUILD_OUTPUT= DEPMOD=/bin/true
else
	export KMAKE:=$(MAKE) ARCH=$(KERNELARCH) CROSS_COMPILE=$(CROSS_COMPILER)
endif

export KMAKE_INST:=$(KMAKE) INSTALL_MOD_PATH=$(PATH_ROOTFS) INSTALL_MOD_DIR=$(EXTRA_MOD_PATH)

#Wireless Modules definitions
#DO NOT MODIFY THE FOLLOWING

#Libraries Path
export PATH_OPENSSL=$(TBS_DIR)/build/tmpdist/


#madwifi
export TARGET=$(MADWIFITARGET)
export MADWIFITARGET=mipsisa32-be-elf
export BUS=PCI


# Set Phy Err Diagnostics (Radar detection) to be enabled for AP builds
export ATH_CAP_PHYERR_DIAG=1
export AR5416_G_MODE=1


ifeq ($(MOTHER_BOARD),broadcom)
#Compatible for e8-wapi Makefile, used in kernel
ifneq ($(strip $(TBS_TOP_BUILD)),y)
#default value
export BRCM_PROFILE_TYPE=CT_E8_B

DEFAULT_E8_C=SG
DEFAULT_E8_B=SWAP1 
ifeq ($(strip $(USB)),)
  ifeq ($(strip $(BRCM_PROFILE_TYPE)),CT_E8_B)
     USB=$(DEFAULT_E8_B)
  else 
     USB=$(DEFAULT_E8_C)
  endif
export USB
endif
endif
endif
