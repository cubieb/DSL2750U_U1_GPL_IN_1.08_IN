include $(ROOTDIR)/BaseVar.mk
BUSYBOX=$(BUILDDIR)/opensource/busybox/busybox-1.6.1
#export KBUILD_OUTPUT=$(BUILDDIR)

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(BUILDDIR) -I$(TOOLCHAINS_SYSROOT)/include -I$(TOOLCHAINS_SYSROOT)/../lib/gcc/mips-linux/4.4.5-1.5.5p2/include -idirafter $(KERNEL_INCLUDE_DIR)

ifdef CONFIG_APPS_LOGIC_NEW3G
CFLAGS += -DCONFIG_APPS_LOGIC_NEW3G
endif

all: busybox 
busybox:
#	mkdir -p $(BUILDDIR)
#	make -C $(BUSYBOX) menuconfig CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH) 
#	make -C $(BUSYBOX) CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH)
	rm -f busybox-1.6.1/.config
	ln -s $(ROOTDIR)/.config busybox-1.6.1/.config 
	make -C $(BUSYBOX) oldconfig CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH) 
	make -C $(BUSYBOX) busybox CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH) 

clean:
	make -C $(BUSYBOX) clean
	rm -rf $(BUSYBOX)/_install/*

distclean:
	make -C $(BUSYBOX) distclean
	
install:
#	make -C $(BUSYBOX) install CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH)
#	echo "Installing Busybox"
#	$(CROSS_COMPILE)strip ${BUILDROOT}/busybox/_install/bin/busybox
	rm -rf $(BUSYBOX)/_install/*
	make -C $(BUSYBOX) install 
	cp -Prf $(BUSYBOX)/_install/* $(FSROOT)
