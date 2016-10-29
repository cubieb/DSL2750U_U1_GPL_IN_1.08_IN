
#export ROOTDIR ?= /home/lvwz/tbs
#export ROOTDIR ?= /home/ouyangdi/INFINEON_DSL/INFINEON_DSL

export DSL_CONF_MK_DIR:=${ROOTDIR}/build/driver/dsl/infineon
include ${DSL_CONF_MK_DIR}/config.mk
include $(ROOTDIR)/.config

DRV_INSTALL_DIR := ${PATH_ROOTFS_MOD}/kernel/drivers/dsl

DRV_SRC_DIR := drv_dsl_cpe_api
	dsl_subdir-${CONFIG_INFINEON_DSL_DRIVERS} += drv_dsl_cpe_api

ifneq ($(CONFIG_AMAZON_SE_MEI_BSP),y)
endif
all:configure build

configure: 
	@cd $(DRV_SRC_DIR); \
	./configure \
		ROOTDIR=$(ROOTDIR) \
		ARCH=$(ARCH) \
		CROSS_COMPILE=mipseb-linux-uclibc- \
		--host=mips-linux-uclibc \
		--enable-kernel-include=$(PATH_KERNEL_INCLUDES) \
		--enable-driver-include=-I$(PATH_KERNEL_INCLUDES)/asm-mips/amazon_se \
		--enable-kernel-build="$(KERNEL_BUILD)" \
		--build=i386-linux \
		--enable-add-drv-cflags="-DMODVERSIONS -Os -G 0" \
		--enable-amazon-se \
		--with-channels-per-line=1 \
		--enable-linux-26 \
		--enable-dsl-pm \
		--enable-dsl-pm-total \
		--enable-dsl-pm-history \
		--enable-dsl-pm-showtime \
		--enable-dsl-pm-line-counters \
		--enable-dsl-pm-channel-counters \
		--enable-dsl-pm-datapath-counters \
		build_alias=i386-linux \
		host_alias=mips-linux-uclibc 
	@touch configure


build:
	@for dir in ${dsl_subdir-y}; do \
	make -C $${dir}; \
	done;
	
install:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)
ifeq (${CONFIG_INFINEON_DSL_DRIVERS},y)
	cp -f $(DRV_SRC_DIR)/src/drv_dsl_cpe_api.ko $(DRV_INSTALL_DIR)
	echo $(DRV_SRC_DIR) 
endif
	#cp -rf $(subst $(BUILDDIR),$(SRCDIR),$(CURDIR))/firmware $(ROOTDIR)/build/romfs/rootfs
	cp -rf  $(subst $(shell readlink -f $(BUILDDIR)),$(SRCDIR),$(CURDIR))/firmware $(ROOTDIR)/build/romfs/rootfs  
	rm $(ROOTDIR)/build/romfs/rootfs/firmware/.svn -rf
clean:
	@for dir in ${dsl_subdir-y}; do \
	make -C $${dir} clean; \
	done;

distclean:
	@for dir in ${dsl_subdir-y}; do \
	make -C $${dir} distclean; \
	done;
	-rm -rf configure


