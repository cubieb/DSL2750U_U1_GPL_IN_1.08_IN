ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 

ifdef CONFIG_OPENSOURCE_EB_2_0_6
SRCDIR=$(CURDIR)/ebtables
export PATH:=$(TOOLCHAINS_PATH)/../${TARGET}/bin:$(PATH)
KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

all:build

build:
	make -C $(SRCDIR) all

clean:
	-make -C $(SRCDIR) clean 

distclean:	
	-make -C $(SRCDIR) clean 
	rm -f configure
	
install:
#	-mkdir -p $(FSROOT)/usr/lib
#	-cp -Pf $(SRCDIR)/extensions/*.so $(FSROOT)/usr/lib
#	-cp -Pf $(SRCDIR)/*.so $(FSROOT)/usr/lib
	cp -Pf $(SRCDIR)/ebtables $(FSROOT)/usr/bin
#	-$(STRIP) $(FSROOT)/usr/lib/*ebt*.so*  $(FSROOT)/usr/bin/ebtables
	-$(STRIP) $(FSROOT)/usr/bin/ebtables
endif


ifdef CONFIG_OPENSOURCE_EB_2_0_9
SRCDIR=$(CURDIR)/ebtables-v2.0.9-2
KERNEL_INCLUDES=$(KERNEL_DIR)/include
export KERNEL_INCLUDES
export CFLAGS = -I./ -I$(KERNEL_INCLUDES)

all:build

build:
	echo ${SRCDIR}
	echo $(KERNEL_INCLUDES)
	make -C $(SRCDIR) all

clean:
	-make -C $(SRCDIR) clean 

distclean:	
	-make -C $(SRCDIR) clean 
	rm -f configure

install:
	install -dv $(FSROOT)/usr/lib
	cp -Pf $(SRCDIR)/extensions/*.so $(FSROOT)/usr/lib
	cp -Pf $(SRCDIR)/*.so $(FSROOT)/usr/lib
	cp -Pf $(SRCDIR)/ebtables $(FSROOT)/usr/bin
#	-$(STRIP) $(FSROOT)/usr/lib/*ebt*.so*  $(FSROOT)/usr/bin/ebtables
	-$(STRIP) $(FSROOT)/usr/bin/ebtables
endif
