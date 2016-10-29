#############################################################
#
# bridgeutils - User Space Program For Controlling Bridging
#
#############################################################
#
SYSUTIL_VERSION=2.4.1
SYSUTIL_SOURCE_URL=http://$(BR2_SOURCEFORGE_MIRROR).dl.sourceforge.net/sourceforge/bridge/
SYSUTIL_SOURCE=sysutil.tar.gz
SYSUTIL_BUILD_DIR=$(BASE_DIR)/../fusiv_src/user/sysutil
SYSUTIL_TARGET_BINARY:=bin/sysutil
LINUX_SRC = $(BASE_DIR)/../fusiv_src
ifeq ("$(strip $(DEFAULT_KERNEL_HEADERS))","2.6.12")
OS_HEADERS_DIR="-I$(BASE_DIR)/../linux-mips-2.6.12/include -I$(LINUX_SRC)/kernel -I$(LINUX_SRC)/kernel/drivers/adsl"
endif
ifeq ("$(strip $(DEFAULT_KERNEL_HEADERS))","2.6.18")
OS_HEADERS_DIR="-I$(BASE_DIR)/../linux-2.6.18/include -I$(LINUX_SRC)/kernel -I$(LINUX_SRC)/kernel/drivers/adsl -I$(LINUX_SRC)/kernel/ap_code"
endif
#ATM_HEADERS_DIR = -I$(BUILD_DIR)/linux-atm-2.4.1/src/include
ATM_HEADERS_DIR =
CC_COMPILER     = $(TARGET_CC)

ifeq ($(strip $(CONFIG_FUSIV_USER_TFTPCLIENT)),y)
TFTP_LIB = $(BASE_DIR)/../fusiv_src/user/tftp-hpa-0.21/tftp/tftp
endif

include $(BASE_DIR)/.config

$(DL_DIR)/$(SYSUTIL_SOURCE):
	 $(WGET) -P $(DL_DIR) $(SYSUTIL_SOURCE_URL)/$(SYSUTIL_SOURCE)

$(SYSUTIL_BUILD_DIR)/.unpacked: $(DL_DIR)/$(SYSUTIL_SOURCE)
	zcat $(DL_DIR)/$(SYSUTIL_SOURCE) | tar -C $(BUILD_DIR) $(TAR_OPTIONS) -
ifdef CONFIG_USER_SYSUTIL_ADSLUTILS
	cp $(SYSUTIL_BUILD_DIR)/L1CODE.BIN $(TARGET_DIR)/bin
	cp $(SYSUTIL_BUILD_DIR)/cmvs.txt $(TARGET_DIR)/bin
	cp $(SYSUTIL_BUILD_DIR)/CMV $(TARGET_DIR)/bin
endif
	touch $(SYSUTIL_BUILD_DIR)/.unpacked

# write the configure script which will modify the Makefile with Proper Kernel
#Headers and Other Variables

#$(SYSUTIL_BUILD_DIR)/.configured: $(SYSUTIL_BUILD_DIR)/.unpacked
$(SYSUTIL_BUILD_DIR)/.configured:
	(cd $(SYSUTIL_BUILD_DIR); rm -rf Makefile .configure; \
	./configure $(OS_HEADERS_DIR) $(CC_COMPILER) ;\
	cd $(SYSUTIL_BUILD_DIR)/srm; rm -rf Makefile; \
	./configure $(CC_COMPILER) $(AR) $(RANLIB);\
	);
	touch  $(SYSUTIL_BUILD_DIR)/.configured

# Add .configured as Dependency Below

$(SYSUTIL_BUILD_DIR)/sysutil: $(SYSUTIL_BUILD_DIR)/.configured $(TFTP_LIB) $(SYSUTIL_BUILD_DIR) $(BASE_DIR)/.config
	$(MAKE) -C $(SYSUTIL_BUILD_DIR)

$(TARGET_DIR)/$(SYSUTIL_TARGET_BINARY): $(SYSUTIL_BUILD_DIR)/sysutil
	cp -af $(SYSUTIL_BUILD_DIR)/sysutil $(TARGET_DIR)/$(SYSUTIL_TARGET_BINARY)
	[ -d $(TARGET_DIR)/etc/config ] || mkdir -p $(TARGET_DIR)/etc/config
ifdef CONFIG_USER_SYSUTIL_ADSLUTILS
	cp $(SYSUTIL_BUILD_DIR)/L1CODE.BIN $(TARGET_DIR)/bin
	cp $(SYSUTIL_BUILD_DIR)/cmvs.txt $(TARGET_DIR)/bin
	cp $(SYSUTIL_BUILD_DIR)/hotplug $(TARGET_DIR)/sbin
endif
	$(STRIP) $(TARGET_DIR)/bin/sysutil

sysutil: $(TARGET_DIR)/$(SYSUTIL_TARGET_BINARY)

sysutil-source: $(DL_DIR)/$(SYSUTIL_SOURCE)

sysutil-clean:
	-$(MAKE) -C $(SYSUTIL_BUILD_DIR) clean

sysutil-dirclean:
	rm -rf $(SYSUTIL_BUILD_DIR)
#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(BR2_PACKAGE_ADI_SYSUTILS)),y)
TARGETS+=sysutil
endif
