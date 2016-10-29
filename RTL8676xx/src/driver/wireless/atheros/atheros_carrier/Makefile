ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config


export WLAN_CONF_MK_DIR:=$(shell pwd)
include $(WLAN_CONF_MK_DIR)/config.mk
dirs-$(CONFIG_DRIVERS_WLAN_ATHEROS_CARRIER)+= carrier
#Notes: DO NOT change the path order and build order

export TBS_TOP_BUILD=y
### Note: Should compile app before api

all:mk_debug
	@set -e ; for d in $(dirs-y) ; do echo ====building in path $$d...====;$(MAKE) -C $$d ; done

install:
	@set -e ; for d in $(dirs-y) ; do echo ====building in path $$d...====;$(MAKE) -C $$d install; done

clean:
	@set -e ; for d in $(dirs-y) ; do echo ====building in path $$d...====;$(MAKE) -C $$d clean; done

distclean:

mk_debug:
	@echo '$$(dirs-y) includes $(dirs-y)'


#include $(ROOTDIR)/Rules.make
