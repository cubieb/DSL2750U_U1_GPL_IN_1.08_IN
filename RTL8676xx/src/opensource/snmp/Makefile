include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean

SNMP_FLAGS=-DSNMP_MYFAKE

ifeq ("$(CONFIG_ZTE_HOT_AP_SNMP_TRAP)","y")
SNMP_FLAGS+=-DZTE_HOT_AP_SNMP_IF
endif

ifeq ("$(CONFIG_TBS_SNMP_APP)","y")
SNMP_FLAGS+=-DTBS_SNMP_APP
endif

ifneq "$(CONFIG_OPENSOURCE_SNMPA_ENTERPRISE_OID)" ""
SNMP_FLAGS+=-DOPENSOURCE_SNMPA_ENTERPRISE_OID=$(CONFIG_OPENSOURCE_SNMPA_ENTERPRISE_OID)
endif

SRCDIR=$(CURDIR)/net-snmp-5.4.1
export PATH:=$(TOOLCHAINS_PATH)/../${TARGET}/bin:$(PATH)
LDFLAGS=-L$(TOOLCHAINS_SYSROOT)./lib -L$(TOOLCHAINS_SYSROOT)./usr/lib -L$(TMPDISTDIR)/lib -L$(BUILDDIR)/apps/ssap/lib -lssap -lexpat -lz -lscew -I${BUILDDIR}
CPPFLAGS=-I$(TOOLCHAINS_SYSINCLUDE) -I$(TMPDISTDIR)/include -I${BUILDDIR} -I${BUILDDIR}/apps/include/  -I${BUILDDIR}/include $(SNMP_FLAGS)
export LDFLAGS CPPFLAGS

BIN_PATH=$(FSROOT)/usr/sbin
LIB_PATH=$(FSROOT)/lib

# To support user choices from menuconfig
ACTIVE_NODES = tbs-private/adslNodes tbs-private/remoteNodes tbs-private/twDslTraps host/hr_storage host/hr_proc mibII/var_route
ifeq (${CONFIG_OPENSOURCE_SNMPA_PUBLIC_NODES}, y)
ACTIVE_NODES += ieee802dot11 ucd-snmp/dlmod if-mib host mibII/system_mib mibII/sysORTable mibII/at mibII/ifTable mibII/ip mibII/snmp_mib mibII/tcp mibII/icmp mibII/udp
endif
ifeq (${CONFIG_OPENSOURCE_SNMPA_PRIVATE_WLAN}, y)
ACTIVE_NODES += tbs-private/remoteNodes
endif	

all:configure build

# Notes:
#	ieee802dot11 supported by an open source project,
#	it's required lib "-lm".
configure:
	cd $(SRCDIR);\
	./configure \
	--prefix=$(TMPDISTDIR) \
	--host=$(TARGET) \
	--enable-as-needed \
	--enable-mini-agent \
	--with-endianness=$(ENDIANNESS) \
	--with-default-snmp-version="2" \
	--with-sys-contact="who@where" \
	--with-sys-location="where" \
	--with-logfile="/var/log/snmp.log" \
	--with-persistent-directory="/var/net-snmp" \
	--with-libs="-lm" \
	--without-perl-modules \
	--without-perl \
	--with-out-transports="IPX AAL5PVC" \
	--with-out-mib-modules="snmpv3 snmpv3mibs" \
	--disable-embedded-perl \
	--disable-applications \
	--disable-manuals \
	--disable-mibs \
	--disable-mib-loading \
	--disable-debugging \
	--disable-des \
	--disable-privacy \
	--disable-md5 \
	--enable-shared=no \
	--enable-static=yes \
	--with-mib-modules="${ACTIVE_NODES}" \
	;
	touch configure;

build:
	make -C $(SRCDIR) all install

clean:
	cd $(SRCDIR); \
	make clean

distclean:
	cd $(SRCDIR); \
	make distclean;
	rm -f configure

install:
	mkdir -p $(BIN_PATH)
	cp -Pf $(TMPDISTDIR)/sbin/snmpd $(BIN_PATH)
	$(STRIP) $(BIN_PATH)/snmpd
	#mkdir -p $(LIB_PATH)
	#cp -Pf $(TMPDISTDIR)/lib/libnetsnmp*.so* $(LIB_PATH)
	#$(STRIP) $(LIB_PATH)/libnetsnmp*.so*
