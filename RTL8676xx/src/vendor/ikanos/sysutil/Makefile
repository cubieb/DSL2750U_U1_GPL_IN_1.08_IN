include $(ROOTDIR)/.config

IKANOS_DRV_DIR = $(ROOTDIR)/build/driver/dsl/ikanos
PATH_KERNEL = $(ROOTDIR)/build/kernel/linux-2.6.18.8

PRI_HEADERS = -I$(ROOTDIR)/build -I${PATH_KERNEL}/include -I${IKANOS_DRV_DIR}/  \
			  -I${IKANOS_DRV_DIR}/drivers/ -I${IKANOS_DRV_DIR}/drivers/adsl -I${IKANOS_DRV_DIR}/ap_code/

ifdef CONFIG_USER_SYSUTIL_COMPDECOMP
ZLIB_DEPS = $(ROOTDIR)/build/tmpdist/lib/libz.so
ZLIB_HEADERS = -I$(ROOTDIR)/build/tmpdist
endif

obj-y = utilmain.o aputil.o linkstate.o ethphyrw.o
obj-${CONFIG_USER_SYSUTIL_ADSLUTILS} += adslatmutil.o adslatmutilhelp.o adslutils.o
obj-${CONFIG_USER_SYSUTIL_XDSL_LINE_MNGR} += xdsl_line_mgr.o
obj-${CONFIG_USER_SYSUTIL_SENDOAMLB} += sendoamlb.o setllid.o showllid.o
obj-${CONFIG_USER_SYSUTIL_SHOWATMCONN} += showatmconn.o bmsession.o
obj-${CONFIG_USER_ADI_PPPOE_RELAY} += ppprelaycli.o ppprelayios.o
obj-${CONFIG_USER_SYSUTIL_IGMPSNOOP} += igmpsnoop.o
obj-${CONFIG_USER_SYSUTIL_SWITCH_CONFIG} += switchvlan.o switchconf.o
obj-${CONFIG_USER_SYSUTIL_APQOS} += qosutil.o
obj-${CONFIG_USER_SYSUTIL_POLICY_VLAN} += vlanclassify.o
obj-${CONFIG_USER_SYSUTIL_GPON} += gponcli.o
obj-${CONFIG_USER_SYSUTIL_SET_ETH_FRAME_READ_CNT} += setethframereadcnt.o
obj-${CONFIG_USER_SYSUTIL_SET_ATM_FRAME_READ_CNT} += setatmframereadcnt.o
obj-${CONFIG_USER_SYSUTIL_SET_VDSL_FRAME_READ_CNT} += setvdslframereadcnt.o
obj-${CONFIG_USER_ACL_FILTER} += acll2Filter.o acll3Filter.o
obj-${CONFIG_USER_SYSUTIL_COMPDECOMP} += compdecomp.o

E_TARGET = sysutil
EXTRA_CFLAGS += $(PRI_HEADERS) $(ZLIB_HEADERS)

include $(ROOTDIR)/Rules.make
CFLAGS = -Os -pipe -Wall

installme::
	mkdir -p $(FSROOT)/usr/bin/
	install -m 755 sysutil $(FSROOT)/usr/bin/
	$(STRIP) $(FSROOT)/usr/bin/sysutil
	cd  $(FSROOT)/usr/bin; \
	ln -sf sysutil apstats; \
	ln -sf sysutil aproutetbl; \
	ln -sf sysutil aread; \
	ln -sf sysutil awrite; \
	ln -sf sysutil igmpsnoop; \
	ln -sf sysutil createlink; \
	ln -sf sysutil mread; \
	ln -sf sysutil mwrite; \
	ln -sf sysutil pppoe-relay; \
	ln -sf sysutil swchqos; \
	ln -sf sysutil swchvlan


	