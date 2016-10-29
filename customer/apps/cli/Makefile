ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
obj-y=cli/cli_cmdtree.o cli/cli_cmdreg.o cli/cli_interpret.o cli/cli_io.o cli/cli_lib.o \
          cli/cli_stub.o app/cli_syscmd.o cli/cli_term.o cli_main.o cli/cli_init.o cli/cli_comm.o \
          app/cli_ddns.o app/cli_natp.o app/cli_route.o app/cli_lan.o app/cli_wlan.o app/cli_wan.o app/cli_admin.o app/cli_pmirror.o app/cli_acl.o
obj-$(CONFIG_APPS_LOGIC_TR069) += app/cli_tr069.o
obj-$(CONFIG_APPS_LOGIC_NEW3G) += app/cli_3g.o

E_TARGET=cmd
EXTRA_CFLAGS=-I$(BUILDDIR)/apps/include -I$(BUILDDIR)/apps/ssap/msg -I$(BUILDDIR) -I$(BUILDDIR)/include
ifeq ("$(CONFIG_RTL8673)", "y")
EXTRA_CFLAGS += -DCONFIG_RTL8673
endif
EXTRA_LDFLAGS= -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lssap  -lexpat -lcrypt -lz
include $(ROOTDIR)/Rules.make

installme::
	$(Q)cp cmd $(FSROOT)/usr/bin
	$(Q)$(STRIP) $(FSROOT)/usr/bin/cmd
	
