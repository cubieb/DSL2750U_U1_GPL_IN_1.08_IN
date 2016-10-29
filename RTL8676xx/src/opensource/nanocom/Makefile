ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 

export PATH:=$(TOOLCHAINS_PATH)/../${TARGET}/bin:$(PATH)

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

ifdef CONFIG_CPU_BIG_ENDIAN
CFLAGS += -DCONFIG_CPU_BIG_ENDIAN
endif

all: build

build: 
	$(CC) mux.c nanocom.c menu.c -o nanocom
    
install: 
	cp -Pf nanocom $(FSROOT)/usr/bin
	mkdir $(FSROOT)/var/pppd3g
	$(STRIP) $(FSROOT)/usr/bin/nanocom
	
clean:
	rm -rf *.o nanocom
	
distclean:
	rm -rf *.o nanocom
    
