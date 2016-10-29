include $(ROOTDIR)/.config

SO_TARGET = libssap.so
subdir-y += ../msg/
subdir-y += ../cfg/
obj-y := ../msg/ssap_msg.o  ../cfg/cfg.o ../pc/pc_api.o ../syslog/warnlog.o tbsutil.o common.o tbsflash.o ../led/led_alp.o crypt.o ../ipt_common/ipt_common.o
obj-${CONFIG_TBS_SUPPORT_IPV6} += tbsipv6.o
obj-$(CONFIG_APPS_SSAP_RCDMODULETM) += tbserror.o

EXTRA_LDFLAGS = -lexpat -lz -L$(TMPDISTDIR)/lib $(TMPDISTDIR)/lib/libscew.a
EXTRA_CFLAGS = -fPIC -shared -Wall -I../msg -I../../include  -I../syslog -I../cfg -I../pc -I$(TMPDISTDIR)/include -I${BUILDDIR} -I${BUILDDIR}/include

ifdef CONFIG_TBS_APPS_DEBUG_PC
EXTRA_CFLAGS += -D_PC_DEBUG
endif

ifeq (${PRODUCT}, UML)
	EXTRA_CFLAGS +=
endif

include $(ROOTDIR)/Rules.make
installme::
	cp libssap.so $(FSROOT)/lib/

#include $(ROOTDIR)/Rules.make
