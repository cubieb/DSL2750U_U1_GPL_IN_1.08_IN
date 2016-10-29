ifeq "$(ROOTDIR)" ""  
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

E_TARGET=msg4udp
obj-y = msg4udp.o ../udp_lib.o 
EXTRA_LDFLAGS=  -L$(BUILDDIR)/alp/ssap/lib -L$(TMPDISTDIR)/lib  -lz -lssap -lexpat -lscew
EXTRA_CFLAGS=  -I$(ROOTDIR)/src/alp/include  -I../


ifeq (${PRODUCT}, UML)
	EXTRA_CFLAGS +=
endif


include $(ROOTDIR)/Rules.make

installme::
	$(Q)cp $(E_TARGET) $(FSROOT)/usr/sbin/
	$(Q)$(STRIP) $(FSROOT)/usr/sbin/$(E_TARGET)


