
include $(ROOTDIR)/build/arch.mk
include $(ROOTDIR)/build/.config

ifneq ($(V), 99)
V=99
endif

ifeq (${ARCH}, um)
CROSS_COMPILE=
else
CROSS_COMPILE:=${TOOLCHAINS_PATH}/${CROSS_COMPILE}
endif

ifeq ($(CPU),C1000)
OTHER_ARGS=modules uImage
endif

ifeq (${CPU}, rtl8672)
CROSS_COMPILE=/opt/tbs_toolchains/realtek/rsdk-1.2.8/linux/uclibc/bin/rsdk-elf-
endif

ifeq (${CPU}, rtl8672ve)
CROSS_COMPILE=/opt/tbs_toolchains/realtek/rsdk-1.3.6/linux/uclibc/bin/rsdk-elf-
endif

ifeq ($(TBS_KERNEL_VERSION), linux-2.6.30-RTL8672)
KERNELVERSION=2.6.30.9
else
KERNELVERSION=$(shell echo $(TBS_KERNEL_VERSION) | cut -d "-" -f2)
endif
export INSTALL_MOD_STRIP=1
OSDIR=${BUILDDIR}/kernel/$(TBS_KERNEL_VERSION)
NSTALL_MOD_PATH=$(ROOTDIR)/build/romfs/rootfs

#hxy+ 2012-2-7 for devired conntrack
export CONFIG_TW_DERIVED_CON


all:
	rm -f ${OSDIR}/.config
	ln -s $(BUILDDIR)/.config ${OSDIR}/.config
	rm -f $(OSDIR)/localversion
	#echo ':'$(TBS_VERSION) > $(OSDIR)/localversion
	make -C ${OSDIR} silentoldconfig ARCH=${ARCH} V=${V} CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT=
	make -C ${OSDIR} ARCH=${ARCH} V=${V} CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT= 
ifdef OTHER_ARGS
	make -C ${OSDIR} ARCH=${ARCH} V=${V} CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT= $(OTHER_ARGS)
endif

install:
	cp -f ${BUILDDIR}/kernel/${TBS_KERNEL_VERSION}/vmlinux ${ROMFS}/vmlinux
	#rm -f ${ROMFS}/vmlinux ${ROMFS}/vmlinux.bin ${ROMFS}/vmlinux.bin.lzma
	${TOOLCHAINS_PATH}/${OBJCOPY} -S -O binary  ${ROMFS}/vmlinux  ${ROMFS}/vmlinux.bin
	#${ROOTDIR}/tools/bin/lzma e ${ROMFS}/vmlinux.bin ${ROMFS}/vmlinux.bin.lzma
	make -C ${OSDIR} modules_install ARCH=${ARCH} INSTALL_MOD_PATH=$(NSTALL_MOD_PATH) V=${V} CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT= DEPMOD=/bin/true

ifeq ($(CONFIG_MODULES), y)
	$(ROOTDIR)/tools/depmod.pl -b $(NSTALL_MOD_PATH)/lib/modules/$(KERNELVERSION) -F $(OSDIR)/System.map 
endif

clean:
	rm -rf ${ROMFS}/vmlinux
	rm -rf $(NSTALL_MOD_PATH)/lib/modules
	make -C ${OSDIR} clean ARCH=${ARCH} V=${V}  CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT=

distclean:
	make -C ${OSDIR} distclean ARCH=${ARCH} V=${V}  CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT=
	

