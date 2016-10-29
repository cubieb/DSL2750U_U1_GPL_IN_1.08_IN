ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config

BUILD_BOOT_DIR=${BUILDDIR}/boot
export BUILD_BOOT_DIR

BOOTDIR=tbsboot

all:
ifneq (${PRODUCT}, UML)
ifneq ($(shell [[ -f $(BUILD_BOOT_DIR)/build/config ]] && echo 0),0)
	make -C $(BOOTDIR) ${CPU}_config
	mkdir -p $(BUILD_BOOT_DIR)/build
	touch $(BUILD_BOOT_DIR)/build/config
endif
	make -C $(BOOTDIR)
endif

install:
ifneq (${PRODUCT}, UML)
	make -C $(BOOTDIR) install
endif

clean:
ifneq (${PRODUCT}, UML)
	make -C $(BOOTDIR) clean
endif

distclean:
ifneq (${PRODUCT}, UML)
	make -C $(BOOTDIR) unconfig
endif
	rm -f $(BUILD_BOOT_DIR)/build/config

#include $(ROOTDIR)/Rules.make
