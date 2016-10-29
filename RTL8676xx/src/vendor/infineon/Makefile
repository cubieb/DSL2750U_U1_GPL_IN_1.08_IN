
#export ROOTDIR ?= /home/lvwz/tbs
#export ROOTDIR ?= /home/ouyangdi/INFINEON_DSL/INFINEON_DSL

export DSL_CONF_MK_DIR:=${ROOTDIR}/build/driver/dsl/infineon
include ${DSL_CONF_MK_DIR}/config.mk
include $(ROOTDIR)/.config

APP_SRC_DIR := dsl_cpe_control

all:configure build

configure: 
	cd $(APP_SRC_DIR); \
	./configure \
		ROOTDIR=$(ROOTDIR) \
		ARCH=$(ARCH) \
		CROSS_COMPILE=mipseb-linux-uclibc- \
		--prefix=$(ROOTDIR)/build/romfs/rootfs/usr \
		--host=mips-linux-uclibc \
		--enable-driver-include=-I$(PATH_KERNEL_INCLUDES)/asm_mips/amazon_se \
		--enable-device-driver-include="-I${DSL_CONF_MK_DIR}/drv_dsl_cpe_api/src/include -I${DSL_CONF_MK_DIR}/drv_dsl_cpe_api/driver/src/include" \
		--with-channels-per-line=1 \
		--enable-amazon-se	\
		--enable-dsl-pm \
		--enable-dsl-pm-total \
		--enable-dsl-pm-history \
		--enable-dsl-pm-showtime \
		--enable-dsl-pm-line-counters \
		--enable-dsl-pm-channel-counters \
		--enable-dsl-pm-datapath-counters \
		--enable-cli-support \
		build_alias=i386-linux \
		host_alias=mips-linux-uclibc 

	@touch configure

build:
	make -C $(APP_SRC_DIR)

install:

	make -C $(APP_SRC_DIR) install
	@echo $(PWD)
	@cp $(ROOTDIR)/src/vendor/infineon/scripts/xdsl_esh.sh $(BUILDDIR)/romfs/rootfs/etc/init.d -f
clean:
	make -C $(APP_SRC_DIR) clean
distclean:
	make -C $(APP_SRC_DIR) distclean
	-rm -rf configure


