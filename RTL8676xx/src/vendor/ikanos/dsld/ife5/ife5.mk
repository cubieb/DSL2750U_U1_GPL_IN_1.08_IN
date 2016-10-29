include $(BASE_DIR)/.config
IFE5_APP_BUILD_DIR=$(BASE_DIR)/../fusiv_src/user/dsld/ife5
export CC_COMPILER     = $(TARGET_CC)

export VDSL_FLAGS = -DLINUX_2_6
$(IFE5_APP_BUILD_DIR)/vdsld:
	(cd $(IFE5_APP_BUILD_DIR); make) || exit;
	@cp $(IFE5_APP_BUILD_DIR)/vdsld $(TARGET_DIR)/bin
	@cp $(IFE5_APP_BUILD_DIR)/vdsl $(TARGET_DIR)/bin
	@cp $(IFE5_APP_BUILD_DIR)/vdsl.conf $(TARGET_DIR)/etc


vdsld_unified: $(IFE5_APP_BUILD_DIR)/vdsld

vdsld-clean:

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(CONFIG_FUSIV_USER_VDSLD_UNIFIED)),y)
TARGETS+=vdsld_unified
endif
