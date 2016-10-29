ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
obj-y=main.o tm.o $(BUILDDIR)/apps/ssap/lib/tbserror.o
E_TARGET=tm
EXTRA_CFLAGS=-I. -I$(BUILDDIR)/ -I$(BUILDDIR)/apps/ssap/msg -I$(BUILDDIR)/apps/include -Wall
EXTRA_LDFLAGS=-L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib  -lz -lssap -lexpat -lscew
include $(ROOTDIR)/Rules.make

installme::
	$(Q)cp tm $(FSROOT)/usr/bin/tm
	$(Q)$(STRIP) $(FSROOT)/usr/bin/tm
