include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all install distclean clean

PUDT_INIT = $(ROOTDIR)/../${CUSTOMER}/product/$(PRODUCT)/init.d
PUDT_MODULES = $(ROOTDIR)/../${CUSTOMER}/product/$(PRODUCT)/modules.d
all:

install:
	$(Q)cp -rf $(ROOTDIR)/src/filesystem/basefilesystem/root/* $(FSROOT)/
	$(Q)find $(FSROOT) -name .svn | xargs rm -rf
	$(Q)rm -f $(FSROOT)/etc/init.d/README
ifeq ("$(CONFIG_APPS_LOGIC_WANSELECT)","y")
	$(Q)cp -f $(ROOTDIR)/../${CUSTOMER}/product/$(PRODUCT)/config_*.xml $(FSROOT)/etc/
	$(Q)cd $(FSROOT)/etc/;ln -s config_dsl.xml config.xml
else
	$(Q)cp -f $(BUILDDIR)/product/config.xml $(FSROOT)/etc/config.xml
endif
#	$(Q)cp -f $(ROOTDIR)/../${CUSTOMER}/product/$(PRODUCT)/config_full.xml $(FSROOT)/etc/config_full.xml
	if [ -f $(ROOTDIR)/../${CUSTOMER}/product/$(PRODUCT)/config_flash.xml ]; then \
		cp $(ROOTDIR)/tools/bin/xml_flash_tool ./ ; \
		./xml_flash_tool $(ROOTDIR)/../${CUSTOMER}/product/$(PRODUCT)/config_flash.xml $(FSROOT)/../tbs_app_cfg ; \
		rm -f ./xml_flash_tool ; \
	else \
		echo "xxxx" > $(FSROOT)/../tbs_app_cfg ; \
	fi;
	if [ "${ARCH}" = "um" ]; then  \
		sed -e "s/ttyS0/tty0/g" root/etc/inittab > $(FSROOT)/etc/inittab; \
	fi;
	if [ "${CPU}" = "rt3052" ]; then  \
		sed -e "s/ttyS0/ttyS1/g" root/etc/inittab > $(FSROOT)/etc/inittab; \
	fi;
	if [ "${CONFIG_OPENSOURCE_SAMBA}" = "y" ]; then \
	  echo "export SAMBA=yes" >> $(FSROOT)/etc/init.d/env.rc; \
	fi;
	if [ "${CONFIG_OPENSOURCE_HTTPD_SSL}" = "y" ]; then \
	  echo "export HTTPS=yes" >> $(FSROOT)/etc/init.d/env.rc; \
	fi;
	if [ "${CONFIG_APPS_LOGIC_USB_MASS}" = "y"]; then \
	  echo "export USB=yes" >> $(FSROOT)/etc/init.d/env.rc; \
	fi; 
	if [ "${CONFIG_TBS_SUPPORT_IPV6}" = "y" ]; then \
	  sh ${ROOTDIR}/../${CUSTOMER}/product/${PRODUCT}/ip6conf.sh; \
	fi;
	-install -c -m 755 $(PUDT_INIT)/* $(FSROOT)/etc/init.d;
	-install -c -m 755 $(PUDT_MODULES)/* $(FSROOT)/etc/modules.d;
	-dos2unix $(FSROOT)/etc/init.d/* 2>/dev/null 1>&2
	./mkdev.sh
	$(Q)echo "install base file system ok!"

distclean:

clean:
