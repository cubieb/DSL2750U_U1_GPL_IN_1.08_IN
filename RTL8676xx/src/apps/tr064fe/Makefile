ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
CURDIR = $(shell pwd)
CROSSTARGET:=$(patsubst %-uclibc,%,$(TARGET))
TARGET = tb_tr064

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/${TBS_KERNEL_VERSION}/include
#KERNEL_INCLUDE__MCAST_DIR=${ROOTDIR}/build/kernel/${TBS_KERNEL_VERSION}/include/linux  -I$(KERNEL_INCLUDE__MCAST_DIR)
export CFLAGS = -Dlinux -D__linux__ -Dunix -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include  -I$(KERNEL_INCLUDE_DIR) -I$(TOO \
LCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

EXTRA_CFLAGS =  -I${BUILDDIR}/apps/ssap/msg/ -I${BUILDDIR}/apps/ssap/msg/ -I${BUILDDIR}/apps/include -I${BUILDDIR} -I${BUILDDIR}/include 
#CFLAGS +=-DTR64_FE_DEBUG -DBCMDBG

CFLAGS += $(EXTRA_CFLAGS) -I$(CURDIR)/inc -I$(CURDIR)/public -I$(CURDIR)/cfgtree
LDFLAGS = -L$(BUILDDIR)/apps/ssap/lib -lssap -L$(TMPDISTDIR)/lib -lz -lexpat

obj-y = main.o protocol/libtr64proto.o data/standard/libtr64std.o cfgtree/libtr64cfg.o

all :  $(TARGET)
	@echo "#----------------------------------------------------"
	@echo "#    finished compiling cm_tr064!!!"
	@echo "#----------------------------------------------------"

$(EXE):  $(OBJS)
	$(CC) -o $@ $(OBJS)
	
$(TARGET) : cfgiflib protocollib cfgiflib datalib $(obj-y)
	rm -f $@
	$(CC) -o $@ $(CFLAGS) $(filter $(obj-y), $^) $(LDFLAGS)

.PHONY : protocollib_clean datalib_clean clean distclean cfgtree_clean

protocollib:
	$(MAKE) -C protocol

cfgiflib:
	$(MAKE) -C cfgtree

datalib:
	$(MAKE) -C data/standard

protocollib_clean:
	$(MAKE) -C protocol clean

datalib_clean:
	$(MAKE) -C data/standard clean

cfgtree_clean:
	$(MAKE) -C cfgtree clean 	
		
clean : protocollib_clean datalib_clean cfgtree_clean
	@echo $(obj-y)
	find . \( -name '*.[oas]' -o -name core -o -name '*~' -o -name '.*~' -o -name '.#*' -o -name '*.so' -o -name '*.so.*' \) -type f -print | xargs rm -f
	rm -f $(TARGET)
	
distclean : clean

install :
	cp -f $(TARGET) $(FSROOT)/sbin
	$(STRIP) $(FSROOT)/sbin/$(TARGET)
