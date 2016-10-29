include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
.PHONY: all build clean  
SRCDIR=$(CURDIR)
EXTRA_CFLAGS=-g
#CFLAGS=-Os
CC=$(CROSS_COMPILE)gcc

ifdef CONFIG_APPS_LOGIC_IGMP_PASSTHROUGH
IGMP_PASSTHROUGH=y
endif

ifdef CONFIG_SUPPORT_IGMP_V3
CONFIG_IP_IGMPV3=y
endif

ifdef CONFIG_SUPPORT_MULTIWAN
CONFIG_IGMP_MULTIWAN=y
endif

ifdef CONFIG_APPS_LOGIC_STATIC_IGMP
CFLAGS += -DCONFIG_APPS_LOGIC_STATIC_IGMP
endif

EXEC = igmpproxyv3

OBJS = mroute-api.o lib.o ifvc.o syslog.o timeout.o main.o
CFLAGS += -I$(ROOTDIR)/build/kernel/$(TBS_KERNEL_VERSION)/include
ifeq ($(IGMP_PASSTHROUGH),y)
CFLAGS += -DCONFIG_IGMP_PASSTHROUGH
endif

ifeq ($(CONFIG_IP_IGMPV3),y)
CFLAGS += -D_SUPPORT_IGMPV3_ 
OBJS   += igmpv3.o
DEPH   += igmpproxy.h
endif

ifeq ($(CONFIG_IGMP_MULTIWAN), y)
CFLAGS += -DCONFIG_IGMPPROXY_MULTIWAN
endif

ifdef CONFIG_CPU_BIG_ENDIAN
CFLAGS += -DCONFIG_CPU_BIG_ENDIAN
endif

all: $(EXEC) build

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS$(LDLIBS-$(@)))
	
build:
	mkdir -p  $(FSROOT)/sbin $(TMPDISTDIR)/sbin
	install -m 0755 $(SRCDIR)/igmpproxyv3 $(TMPDISTDIR)/sbin/igmpproxyv3

clean:
	cd $(SRCDIR); \
	rm -f $(EXEC) *.elf *.o *.bak

install:
	cp $(TMPDISTDIR)/sbin/igmpproxyv3 $(FSROOT)/sbin/igmpproxyv3
	$(STRIP) $(FSROOT)/sbin/igmpproxyv3
	
