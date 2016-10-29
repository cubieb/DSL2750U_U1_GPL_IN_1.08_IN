include $(ROOTDIR)/.config
include $(BUILDDIR)/arch.mk


target-y = 
target-$(CONFIG_APPS_IPV6_RAMON) += ramon
target-$(CONFIG_APPS_IPV6_IP6MON) += ip6mon
target-$(CONFIG_APPS_IPV6_IP6AAC) += ip6aac
target-$(CONFIG_APPS_IPV6_IFIPV6) += ifip6
all: $(target-y)
include $(ROOTDIR)/Rules.make

ramon: ramon.o
ip6mon: ip6mon.o
ip6aac: ip6aac.o
ifip6: ifip6.o

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/${TBS_KERNEL_VERSION}/include
CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) \
			    -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include
CFLAGS += -Os -Wall -Werror -pipe -I../ssap/msg -I$(BUILDDIR) -I$(BUILDDIR)/apps/include  -I$(TMPDISTDIR)/include
LDFLAGS += -lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz

ifdef CONFIG_TBS_APPS_DEBUG_IP6MON
CFLAGS += -DIP6MON_DEBUG
endif

ifdef CONFIG_TBS_APPS_DEBUG_IP6AAC
CFLAGS += -DIP6AAC_DEBUG
endif

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

install:
	install -c $(target-y) $(FSROOT)/usr/bin/
	-cd $(FSROOT)/usr/bin/; $(STRIP) $(target-y)

clean:
	-rm -f *.o *.d $(target-y)
