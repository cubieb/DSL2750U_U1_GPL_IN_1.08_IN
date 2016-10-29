
########################################################################
#
# Makefile for Wlan (Host and Target)
#
###########################################################################

export TOPDIR_K2= $(shell pwd)
temp_BOARD_TYPE = $(strip $(subst fus, , $(BOARD_TYPE)))
export TOOLPATH=$(TOOLCHAINS_PATH)



export PATH:=$(TOOLPATH)/bin:$(TOOLPATH)/bin-ccache:`pwd`:${PATH}


export STRIP=$(TOOLPREFIX)strip

export KERNELPATH=$(BUILDDIR)/kernel/linux-2.6.18.8/

# Set the toolchain path
export TOOLCHAIN=gcc
export TOOLPREFIX=mipseb-linux-uclibc-
export TOOLARCH=build_mips

export KERNEL=linux-2.6.18
export KERNELVER=2.6.18
export COMPRESSKERNEL=y
export KERNELARCH=mips
export ATH_TDLS=1

export WIRELESSTOOLS=wireless_tools.28
export WIRELESSTOOLSLIB=libiw.so.28

export BUS=PCI
export MADWIFITARGET=mipsisa32-be-elf

# Set Phy Err Diagnostics (Radar detection) to be enabled for AP builds
export ATH_CAP_PHYERR_DIAG=1

export AR5416_G_MODE=1
# Use Light-WPA security applications default
export BUILD_WPA2=y
# Enable Ether_dongle feature in Light-WPA
export CONFIG_CARRIER_ETHERSTA=y

export APPS_SDK=1
export GSOAP_WITH_LEANER=1


# Enable Video Rate Control
export ATH_VRC=0



export WLAN_11N_DIR=wlan
export WLAN_11N_HAL_DIR=$(TOPDIR_K2)/$(WLAN_11N_DIR)/core/hal
export HAL=$(WLAN_11N_HAL_DIR)
# MADWIFIPATH is for use by e.g. apps/wpa2 code:
export MADWIFIPATH=$(TOPDIR_K2)/wlan/core
export MTD_XPROM ?= 1

export MAKEARCH=$(MAKE) ARCH=$(KERNELARCH) CROSS_COMPILE=$(TOOLPREFIX)

#############################################################
####  K2 Stuff ##############################################
#############################################################

export HOST_11N_INSTALL_PATH=$(MODULEPATH)/11n
export TGT_K2_RAM_BUILD_PATH=$(TOPDIR_K2)/tgt/build/k2_1_0/image/k2_ram
export WLAN_DIR=$(TOPDIR_K2)/$(WLAN_11N_DIR)
export WLAN_BUILD_DIR=$(WLAN_DIR)/build/linux
export WLAN_HOST_DIR=$(WLAN_DIR)/host



ifeq ($(BUILD),no-debug)
export ATH_NO_DEBUG=1
export CONFIG_NO_STDOUT_DEBUG=1
endif



RAMDISK_WLAN_DIR = $(TOPDIR_K2)/bsp/ASEBSP1.1.0.1/source/rootfs.bsp/flashdisk/ramdisk_copy/
WLAN_UTILS_DIR = $(TOPDIR_K2)/wlan/utils

export INSTALL_ROOT=$(TOPDIR_K2)/ubs_build/root_filesystem

####################################################################
### K2 INSTALL PATH
####################################################################

VERSION := $(strip $(shell head -n 1 ${KERNELPATH}/Makefile | cut -f 2 -d'='))
PATCHLEVEL := $(strip $(shell head -n 2 ${KERNELPATH}/Makefile | tail -1 | cut -f 2 -d'='))
SUBVERSION := $(strip $(shell head -n 3 ${KERNELPATH}/Makefile | tail -1 | cut -f 2 -d'='))
EXTRAVERSION ?= $(strip $(shell head -n 4 ${KERNELPATH}/Makefile | tail -1 | cut -f 2 -d'='))
export KERNEL_RELEASE := $(strip ${VERSION}.${PATCHLEVEL}.${SUBVERSION}${EXTRAVERSION})

SHELL_PATH=$(FSROOT)/etc/init.d/
EXTRA_MOD_PATH := kernel/drivers/net/ath_k2
export MODULEPATH:=lib/modules/${KERNEL_RELEASE}/$(EXTRA_MOD_PATH)
export PATH_ROOTFS_MOD:=$(FSROOT)/$(MODULEPATH)
export PATH_ROOTFS_BIN:=${FSROOT}/usr/sbin


####################################################################
# Make rules
#
###################################################################

.PHONY : all clean

#all:	tgt_k2_ram_usb_build host_k2_usb_build athcfg_build
#all:	root_env_prep host_k2_usb_build athcfg_build wpa_supplicant hostapd
all:	host_k2_usb_build athcfg_build 

host_k2_wlan_build:
	echo ${PATH}
	@echo Building host wlan
	cd $(WLAN_BUILD_DIR) && \
	$(MAKEARCH) KDIR=$(KERNELPATH) arch=mipsisa32-be-elf  bld-mod=adf && \
	$(MAKEARCH) KDIR=$(KERNELPATH) arch=mipsisa32-be-elf hif=usb tgt=k2 bld-mod=hst && \
	$(MAKEARCH) KDIR=$(KERNELPATH) arch=mipsisa32-be-elf bld-mod=hst-scansta



host_k2_usb_build: host_k2_wlan_build
	@echo Building host usb
	cd $(WLAN_BUILD_DIR) && \
	$(MAKEARCH) KDIR=$(KERNELPATH) arch=mipsisa32-be-elf tgt=k2 boot=rom bld-mod=hif-usb 

athcfg_build:
	@echo Building athcfg
	cd $(WLAN_DIR)/tools/athcfg && \
	$(MAKEARCH) -f Makefile.linux

tgt_k2_ram_usb_build:
	@echo Building K2 target ram image
	cd $(TGT_K2_RAM_BUILD_PATH) && \
	source builds.ram	 && \
	$(MAKE) && $(MAKE) MAGPIE_IF=usb build
	@echo Building target usb
	cd $(TGT_K2_RAM_BUILD_PATH) && \
	cp -fv fwu.c $(WLAN_HOST_DIR)/htc/hif/usb/fwu_k2_rom_usb.c

#########################################################
# Rules for cleaning
#
###########################################################
host_clean:
	echo $(TOOLPATH)
	@echo Cleaning host
	@echo split driver for HOST
	cd $(WLAN_BUILD_DIR) && \
	$(MAKEARCH) KDIR=$(KERNELPATH) clean 

athcfg_clean:
	@echo Cleaning athcfg
	@echo Split athcfg build for HOST
	cd $(WLAN_DIR)/tools/athcfg && \
	$(MAKEARCH) -f Makefile.linux clean 

tgt_k2_ram_clean:
	@echo Cleaning K2 target RAM image
	cd $(TGT_K2_RAM_BUILD_PATH) && \
	source builds.ram	 && \
	$(MAKEARCH) clean &&  $(MAKEARCH) dep


root_env_prep:
	chmod +w $(INSTALL_ROOT)/lib/*.so
	chmod +w $(INSTALL_ROOT)/bin/*

##########################################################################
# Rules for making WPA and hostapd
#
##########################################################################

ifndef MADWIFIPATH
export MADWIFIPATH=$(TOPDIR_K2)/wlan/madwifi/madwifi
endif

wpa_talk: hostapd wpa_supplicant
	if (test -e ../apps/wpa2_adf/wpatalk ) then \
	chmod -R 755 ../apps/wpa2_adf/wpatalk; \
	cd ../apps/wpa2_adf/wpatalk; $(MAKE) clean; \
	$(MAKE) CC=$(TOOLPREFIX)gcc AR=$(TOOLPREFIX)ar LD=$(TOOLPREFIX)ld \
         PREFIX=$(INSTALL_ROOT)/sbin CONFIG_PATH=$(INSTALL_ROOT)/etc/ath DEFAULT_CFG=$(INSTALL_ROOT)/etc/ath/default install; \
	fi


libwpa_common:
	cd apps/wpa2_adf/common && $(MAKE) install \
		CC=$(TOOLPREFIX)gcc AR=$(TOOLPREFIX)ar LD=$(TOOLPREFIX)ld

wpa_supplicant: libwpa_common
	cd apps/wpa2_adf/wpa_supplicant && $(MAKE) install \
		CC=$(TOOLPREFIX)gcc AR=$(TOOLPREFIX)ar LD=$(TOOLPREFIX)ld
	mkdir -p $(INSTALL_ROOT)/etc/ath
	mkdir -p $(INSTALL_ROOT)/etc/wpa2
	cp -fv apps/wpa2_adf/wpa_supplicant/topology_sta.conf $(INSTALL_ROOT)/etc/ath
	cp -fv apps/wpa2_adf/wpa_supplicant/wpa_s.conf $(INSTALL_ROOT)/etc/wpa2

hostapd: openssl_adf libwpa_common
	cd apps/wpa2_adf/hostapd && $(MAKE) install \
	CC=$(TOOLPREFIX)gcc AR=$(TOOLPREFIX)ar LD=$(TOOLPREFIX)ld
	mkdir -p $(INSTALL_ROOT)/etc/ath
	mkdir -p $(INSTALL_ROOT)/etc/wpa2
	mkdir -p $(INSTALL_ROOT)/sbin
	cp -fv apps/wpa2_adf/hostapd/topology_ap.conf $(INSTALL_ROOT)/etc/ath
	cp -fv apps/wpa2_adf/hostapd/hostapd.eap_user $(INSTALL_ROOT)/etc/wpa2
	cp -fv apps/wpa2_adf/hostapd/80211g.ap_radio $(INSTALL_ROOT)/etc/wpa2
	cp -fv apps/wpa2_adf/hostapd/aes-up.sh $(INSTALL_ROOT)/sbin

openssl_adf:
	if (test -e apps/wpa2_adf/wsc/lib/openssl-0.9.8a ) then \
	chmod -R 755 apps/wpa2_adf/wsc/lib/openssl-0.9.8a; \
	cd apps/wpa2_adf/wsc/lib/openssl-0.9.8a && make;fi
	cd apps/wpa2_adf/wsc/lib/openssl-0.9.8a && cp *.a $(INSTALL_ROOT)/lib/. 

wpa2: openssl
	if ( test -e $(TOPDIR_K2)/wlan/linux ) then \
    ln -fs ../include $(TOPDIR_K2)/wlan/linux/include; fi
	cd apps/wpa2 && $(MAKE) 	\
		CC=$(TOOLPREFIX)gcc AR=$(TOOLPREFIX)ar LD=$(TOOLPREFIX)ld
openssl:
	if (test -e apps/wpa/wsc/lib/openssl-0.9.8a ) then \
	chmod -R 755 apps/wpa/wsc/lib/openssl-0.9.8a; \
	cd apps/wpa/wsc/lib/openssl-0.9.8a && make;fi
	cd apps/wpa/wsc/lib/openssl-0.9.8a && cp *.a $(INSTALL_ROOT)/lib/. 

wpa2_clean: 
	cd apps/wpa2_adf && $(MAKE) clean
   
wlan_ramdisk: 
	echo "Copying to ramdisk folder and Creating the Ramdisk"
	test -d $(RAMDISK_WLAN_DIR) || mkdir $(RAMDISK_WLAN_DIR) && \
	cp -f $(WLAN_BUILD_DIR)/*.ko $(RAMDISK_WLAN_DIR) && \
	cp -f $(WLAN_DIR)/tools/athcfg/athcfg $(RAMDISK_WLAN_DIR) && \
	cp -f $(WLAN_UTILS_DIR)/vap_usb.sh    $(RAMDISK_WLAN_DIR) && \
	cp -f $(WLAN_UTILS_DIR)/wpa-ap.conf    $(RAMDISK_WLAN_DIR) && \
	cp -f $(WLAN_UTILS_DIR)/load_k2_AmazonSE   $(RAMDISK_WLAN_DIR) && \
	cp -f $(WLAN_UTILS_DIR)/sar    $(RAMDISK_WLAN_DIR) && \
	cp -f $(WLAN_UTILS_DIR)/sadc    $(RAMDISK_WLAN_DIR) && \
	cd $(TOPDIR_K2)/bsp/ASEBSP1.1.0.1/tools/build_tools && \
	./build.sh rootfs

create_dir:
	$Qtest -d $(PATH_ROOTFS_BIN) || mkdir -p $(PATH_ROOTFS_BIN)
	$Qtest -d $(PATH_ROOTFS_MOD) || mkdir -p $(PATH_ROOTFS_MOD)

install: create_dir
	echo "Copying to rootfs directory"
	cp -f $(WLAN_BUILD_DIR)/*.ko $(PATH_ROOTFS_MOD) && \
	cp -f $(WLAN_DIR)/tools/athcfg/athcfg $(PATH_ROOTFS_BIN) && \
	cp -f $(WLAN_UTILS_DIR)/vap_usb.sh    $(SHELL_PATH) && \
	cp -f $(WLAN_UTILS_DIR)/wpa-ap.conf    $(PATH_ROOTFS_MOD) && \
	cp -f $(WLAN_UTILS_DIR)/load_k2_AmazonSE   $(SHELL_PATH) && \
	cp -f $(WLAN_DIR)/build/linux/scripts/k2/k2-unload.sh $(SHELL_PATH) && \
	cp -f $(WLAN_UTILS_DIR)/sar    $(PATH_ROOTFS_MOD) && \
	cp -f $(WLAN_UTILS_DIR)/sadc    $(PATH_ROOTFS_MOD) 
	chmod +x $(SHELL_PATH)/load_k2_AmazonSE 
	chmod +x $(SHELL_PATH)/vap_usb.sh 
	chmod +x $(SHELL_PATH)/k2-unload.sh 
	

#clean:	host_clean athcfg_clean tgt_k2_ram_clean
#clean:	host_clean athcfg_clean wpa2_clean
clean:	host_clean athcfg_clean
