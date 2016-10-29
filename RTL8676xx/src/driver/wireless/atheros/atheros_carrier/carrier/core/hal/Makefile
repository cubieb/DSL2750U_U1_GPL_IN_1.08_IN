#ifeq ($(WLAN_CR_CONF_MK_DIR),) 
#WLAN_CR_CONF_MK_DIR:=$(CURDIR)/../..
#include $(WLAN_CR_CONF_MK_DIR)/config.mk
#endif

all:
	cd ./main/linux && make  && make  release; 

clean:
	cd ./main/linux && make  clean
	
install:

uninstall:

