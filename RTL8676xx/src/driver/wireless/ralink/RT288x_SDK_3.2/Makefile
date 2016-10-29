ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/build/arch.mk
include config.mk

EXTRA_CFLAGS += -include $(ROOTDIR)/build/autoconf.h
export EXTRA_CFLAGS

dirs-$(CONFIG_RT2860V2_AP)       += rt2860v2_ap
dirs-$(CONFIG_RT2860V2_STA)      += rt2860v2_sta

all:
	$(Q)for d in $(dirs-y) ; \
	do \
		make -C $(OSDIR) M=$(CURDIR)/$$d modules \
			ARCH=${ARCH} V=${V} CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT= DEPMOD=/bin/true ; \
	done

install: create_dir
	$(Q)for d in $(dirs-y) ; \
	do \
		make -C $$d install ; \
	done

clean:
	$(Q)for d in $(dirs-y) ; \
	do \
		make -C $$d clean ; \
	done

distclean:

create_dir:     
	$(Q)test -d $(PATH_ROOTFS_MOD) || mkdir -p $(PATH_ROOTFS_MOD)
	$(Q)test -d $(PATH_ROOTFS_BIN) || mkdir -p $(PATH_ROOTFS_BIN)
	$(Q)test -d $(PATH_ROOTFS_LIB) || mkdir -p $(PATH_ROOTFS_LIB)
	$(Q)@test -d $(PATH_ROOTFS_INC) || mkdir -p $(PATH_ROOTFS_INC)
	$(Q)test -d $(PATH_ROOTFS_SCRIPT) || mkdir -p $(PATH_ROOTFS_SCRIPT)
	$(Q)test -d $(PATH_ROOTFS_RC) || mkdir -p $(PATH_ROOTFS_RC)
