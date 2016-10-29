include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/mini_httpd-1.19

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(BUILDDIR) -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include
ifdef CONFIG_CPU_BIG_ENDIAN
        CFLAGS += -DCONFIG_CPU_BIG_ENDIAN
endif

ifdef CONFIG_APPS_OTHERPORT
	CFLAGS += -DCONFIG_APPS_OTHERPORT
endif

ifdef CONFIG_TELEFONICA_BRAZIL
CFLAGS += -DCONFIG_TELEFONICA_BRAZIL
endif

all:configure build
ifeq ("$(CONFIG_OPENSOURCE_HTTPD_SSL)","y")
SSL_TREE =      $(TMPDISTDIR)/ssl
SSL_DEFS =      -DUSE_SSL
SSL_INC =       -I$(TMPDISTDIR)/include
SSL_LIBS =      -L$(TMPDISTDIR)/lib -lssl -lcrypto
export SSL_TREE SSL_DEFS SSL_INC SSL_LIBS
endif

configure:ssap

ssap:
	 make -C $(ROOTDIR) ssap

build:
	make -C $(SRCDIR) all CC=$(CC) LD=$(LD) 

clean:
	cd $(SRCDIR); \
	make clean

distclean:clean	
#	cd $(SRCDIR); \
#	make distclean
#	rm -f configure
	
install:
	mkdir -p  $(FSROOT)/usr/sbin
	mkdir -p  $(FSROOT)/usr/www
	mkdir -p  $(FSROOT)/etc
ifeq ("$(CONFIG_OPENSOURCE_HTTPD_SSL)","y")
	cp -f ${SRCDIR}/mini_httpd.pem $(FSROOT)/etc/
endif
	cp -f $(SRCDIR)/mini_httpd $(FSROOT)/usr/sbin
	cp -f ${SRCDIR}/.htdgst $(FSROOT)/usr/www
	cp -f ${SRCDIR}/.htpasswd $(FSROOT)/usr/www
