#
#/* Copyright (C) 2006 Ikanos Communications
# * See IKANOS_PROP_LICENSE.txt for license information.*/
#

include $(ROOTDIR)/.config

export IKANOS_BASE_DIR=$(ROOTDIR)/build/driver/dsl/ikanos
export AP_BASE_DIR=${IKANOS_BASE_DIR}/ap_code
export DRV_BASE_DIR=${IKANOS_BASE_DIR}/drivers
export PATH_KERNEL=$(ROOTDIR)/build/kernel/linux-2.6.18.8
export PATH_KERNEL_INCLUDES=${PATH_KERNEL}/include

VERSION := $(strip $(shell head -n 1 ${PATH_KERNEL}/Makefile | cut -f 2 -d'='))
PATCHLEVEL := $(strip $(shell head -n 2 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
SUBVERSION := $(strip $(shell head -n 3 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
EXTRAVERSION ?= $(strip $(shell head -n 4 ${PATH_KERNEL}/Makefile | tail -1 | cut -f 2 -d'='))
KERNELRELEASE := $(strip ${VERSION}.${PATCHLEVEL}.${SUBVERSION}${EXTRAVERSION})
export MODULEPATH:=${FSROOT}/lib/modules/${KERNELRELEASE}
 
export KMAKE:=$(MAKE) ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE}
export EXTRA_CFLAGS += -I$(AP_BASE_DIR)/inc  -I$(DRV_BASE_DIR)/inc -I$(AP_BASE_DIR) -I$(DRV_BASE_DIR) -DLINUX_2_6

subdir-y :=
subdir-m :=
subdir-$(CONFIG_FUSIV_LIBRARY) += drivers/opensrc/
subdir-$(CONFIG_FUSIV_LIBRARY) += ap_code/lib/
subdir-$(CONFIG_FUSIV_BMDRIVER) += drivers/bm/
subdir-$(CONFIG_FUSIV_KERNEL_ETHERNET) += drivers/ethdriver/
subdir-$(CONFIG_FUSIV_KERNEL_PERI_AP) += drivers/peri_ap/
subdir-$(CONFIG_FUSIV_KERNEL_ATM) += drivers/atm/
subdir-$(CONFIG_FUSIV_TIMERS) += drivers/timerlib/
subdir-$(CONFIG_FUSIV_KERNEL_AP_2_AP) += drivers/ap2ap/
subdir-$(CONFIG_FUSIV_KERNEL_DSP) += drivers/voice/ipvoice/
subdir-$(CONFIG_FUSIV_KERNEL_UART2) += drivers/uart2/
subdir-$(CONFIG_FUSIV_KERNEL_HOST_IPQOS) += drivers/ipqos/
subdir-$(CONFIG_FUSIV_KERNEL_PROFILER) += profiler/
subdir-$(CONFIG_BT_HCI_FUSIV_UART2)    += drivers/btuart/
subdir-$(CONFIG_FUSIV_KERNEL_PPPOE_RELAY) += drivers/pppoer/
subdir-$(CONFIG_FUSIV_KERNEL_MONIF) += drivers/monif/
subdir-$(CONFIG_FUSIV_KERNEL_IGMP_SNOOP) += drivers/igmpsnoop/
subdir-$(CONFIG_FUSIV_KERNEL_BME) += drivers/bmedriver/
subdir-$(CONFIG_FUSIV_KERNEL_BME_DRIVER_VX180) += drivers/bmedriver/
subdir-$(CONFIG_FUSIV_KERNEL_BME_DRIVER_VX180) += drivers/vdsldriver/
subdir-$(CONFIG_FUSIV_KERNEL_ACL) += drivers/acl_ap/
subdir-$(CONFIG_FUSIV_KERNEL_POLICY_VLAN) += drivers/policyVlan/

DIRS := $(subdir-y) $(subdir-m) 

all:
	@set -e ; for d in $(DIRS) ; do echo ====building in path "$$d"====;$(MAKE) -C $$d ; done

clean:
	@set -e ; for d in $(DIRS) ; do echo ====building in path $$d...====;$(MAKE) -C $$d clean ; done

install:create_dir
	@set -e ; for d in $(DIRS) ; do echo ====installing in path $$d...====;$(MAKE) -C $$d install ; done

create_dir:     
#	test -d $(PATH_ROOTFS_BIN) || mkdir -p $(PATH_ROOTFS_BIN)
#	test -d $(PATH_ROOTFS_LIB) || mkdir -p $(PATH_ROOTFS_LIB)
#	test -d $(PATH_ROOTFS_INC) || mkdir -p $(PATH_ROOTFS_INC)
#	test -d $(PATH_ROOTFS_MOD) || mkdir -p $(PATH_ROOTFS_MOD)
#	test -d $(PATH_ROOTFS_SCRIPT) || mkdir -p $(PATH_ROOTFS_SCRIPT)
#	test -d $(PATH_ROOTFS_RC) || mkdir -p $(PATH_ROOTFS_RC)
#	test -d $(CONFIG_PATH) || mkdir -p $(CONFIG_PATH)
#	test -d $(DEFAULT_CFG) || mkdir -p $(DEFAULT_CFG)
