include $(BASE_DIR)/.config
IFE6_APP_BUILD_DIR=$(BASE_DIR)/../fusiv_src/user/dsld/ife6
export CC_COMPILER     = $(TARGET_CC)

.PHONY: $(IFE6_APP_BUILD_DIR)/vdsld

export VDSL_FLAGS = -DLINUX_2_6
$(IFE6_APP_BUILD_DIR)/vdsld:
	(cd $(IFE6_APP_BUILD_DIR); make) || exit;
	cp $(IFE6_APP_BUILD_DIR)/vdsld $(TARGET_DIR)/bin
	cp $(IFE6_APP_BUILD_DIR)/vdsl $(TARGET_DIR)/bin
	cp $(IFE6_APP_BUILD_DIR)/vdsl.conf $(TARGET_DIR)/etc
	cp $(IFE6_APP_BUILD_DIR)/load_xdsl_modules $(TARGET_DIR)/etc
	chmod a+x $(TARGET_DIR)/etc/load_xdsl_modules
	[ -d $(TARGET_DIR)/etc/config ] || mkdir -p $(TARGET_DIR)/etc/config
	touch $(TARGET_DIR)/etc/config/VDSL

avdsld: $(IFE6_APP_BUILD_DIR)/vdsld

vdsld-clean:

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(CONFIG_FUSIV_USER_AVDSLD)),y)
TARGETS+=avdsld
endif
