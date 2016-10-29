ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/gdb-6.3
GDBPREFIX=export CC=gcc LD=ld CPP=cpp AR=ar AS=as NM=nm OBJCOPY=objcopy OBJDUMP=objdump;
#CC=gcc
#LD=ld
#CPP=cpp
#AR=ar
GDBTARGET:=$(subst uclibc,gnu,$(TARGET))
all:configure build
configure:
	if [ ! -d "$(SRCDIR)" ]; then \
		tar jxf gdb-6.3.tar.bz2; \
	fi
	cd $(SRCDIR); \
	find | xargs touch;\
	$(GDBPREFIX) ./configure --prefix=$(TMPDISTDIR) --target=$(GDBTARGET) --host=$(GDBTARGET);
ifdef CONFIG_OPENSOURCE_GDB_SERVER
	cd $(SRCDIR)/gdb/gdbserver; \
	./configure --prefix=$(TMPDISTDIR) --target=$(TARGET);
endif
	touch configure
build:
	$(GDBPREFIX) make -C $(SRCDIR) all install 
ifdef CONFIG_OPENSOURCE_GDB_SERVER
	make -C $(SRCDIR)/gdb/gdbserver all install 
endif

clean:
	if [ -d "$(SRCDIR)" ]; then \
		cd $(SRCDIR); \
		make clean;  \
	fi
ifdef CONFIG_OPENSOURCE_GDB_SERVER
	make -C $(SRCDIR)/gdb/gdbserver clean 
endif

distclean:
	
	-if [ -d "$(SRCDIR)" ]; then \
		cd $(SRCDIR); \
		make distclean; \
	fi
ifdef CONFIG_OPENSOURCE_GDB_SERVER
	-make -C $(SRCDIR)/gdb/gdbserver distclean 
endif
	rm -f configure

install:
ifdef CONFIG_OPENSOURCE_GDB_SERVER
	cp $(TMPDISTDIR)/bin/gdbserver $(FSROOT)/bin/gdbserver
	$(STRIP) $(FSROOT)/bin/gdbserver
endif
ifeq (${PRODUCT}, UML)
	cp /lib/libm.so.6 $(FSROOT)/lib/
	cp /lib/libc.so.6 $(FSROOT)/lib/
	cp /lib/libdl.so.2 $(FSROOT)/lib/
	cp /lib/ld-linux.so.2 $(FSROOT)/lib/
	cp /usr/lib/libncurses.so.5 $(FSROOT)/lib/ 
endif
	cp $(TMPDISTDIR)/bin/gdb $(FSROOT)/bin/gdb
	$(STRIP) $(FSROOT)/bin/gdb
