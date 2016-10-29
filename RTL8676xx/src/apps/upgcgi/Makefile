ifeq "$(ROOTDIR)" ""
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

#include $(ROOTDIR)/.config
obj-y = upg_cgi.o  upg_parse.o
E_TARGET = webupg
EXTRA_CFLAGS = -I$(BUILDDIR)/ -I$(BUILDDIR)/apps/include -Wall 
EXTRA_LDFLAGS = -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz
include $(ROOTDIR)/Rules.make

installme::
	$(Q)cp $(E_TARGET) $(FSROOT)/usr/www/cgi-bin
	$(Q)$(STRIP) $(FSROOT)/usr/www/cgi-bin/$(E_TARGET)

