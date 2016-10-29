ifeq "$(ROOTDIR)" ""  
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

E_TARGET=hostapd_cfg
obj-y= common.o cliconn.o  client.o
EXTRA_LDFLAGS=  
EXTRA_CFLAGS=  

include $(ROOTDIR)/Rules.make

install:
	$(Q)cp $(E_TARGET) $(FSROOT)/usr/sbin/
	$(Q)$(STRIP) $(FSROOT)/usr/sbin/$(E_TARGET)


