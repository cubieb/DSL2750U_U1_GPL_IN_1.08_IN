include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all 2684ctl install clean distclean

KERNEL_INCLUDE_DIR =${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export EXTRA_CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_  \
			    -I./ -I$(TOOLCHAINS_SYSROOT)/include  \
				-I$(TOOLCHAINS_SYSROOT)/../lib/gcc/mips-linux/4.4.5-1.5.5p2/include  \
				-I$(KERNEL_INCLUDE_DIR)

all: 2684ctl
2684ctl:
	make -C ./2684ctl

install:
	make -C ./2684ctl install

clean:
	make -C ./2684ctl clean

distclean:
