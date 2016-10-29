ifeq "$(ROOTDIR)" ""
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

SRC_3G = $(ROOTDIR)/../${CUSTOMER}/apps/logic/new3g

PWD = $(shell pwd)

include $(ROOTDIR)/.config

include $(ROOTDIR)/build/arch.mk

include ./env.mk

.PHONY: all build clean distclean  

CFLAGS += -I$(PWD)/3g-mngr
CFLAGS += -I$(PWD)/3g-mngr/lib
CFLAGS += -I$(PWD)/3g-stub
CFLAGS += -DCONFIG_APPS_LOGIC_NEW3G

SYSTEM = TBS
INSTALL_DIR=$(FSROOT)
PPP_SCRIPT_DIR=$(FSROOT)/etc/ppp/

export CFLAGS FSROOT SYSTEM SRC_3G  INSTALL_DIR CPU



#scb+ 2011-10-24
ifeq ("$(CONFIG_SUPPORT_POP_ENTER_PIN_CODE_WEB)","y")
CFLAGS += -DSUPPORT_POP_ENTER_PIN_CODE_WEB
export CONFIG_SUPPORT_POP_ENTER_PIN_CODE_WEB 
endif

#scb+ 2012-06-11
ifneq ($(strip $(CONFIG_DRIVER_Chile)),)
CFLAGS += -DCONFIG_DRIVER_Chile
export CONFIG_DRIVER_Chile
endif

#scb+ 2012-06-11
ifneq ($(strip $(CONFIG_DRIVER_Pakistani)),)
CFLAGS += -DCONFIG_DRIVER_Pakistani
export CONFIG_DRIVER_Pakistani
endif

#scb+ 2012-4-10 for support more 3g dongle
ifeq ("$(CONFIG_SUPPORT_MORE_DONGLE)","y")
CFLAGS += -DSUPPORT_MORE_DONGLE
export CONFIG_SUPPORT_MORE_DONGLE
endif

#scb+ 2012-6-8 when linkdown by the peer, dail again
ifeq ("$(CONFIG_SUPPORT_ALLWAYS_CONNECTED)","y")
CFLAGS += -DSUPPORT_ALLWAYS_CONNECTED
export CONFIG_SUPPORT_ALLWAYS_CONNECTED
endif


#scb+ 2011-10-24
ifeq ("$(CONFIG_APPS_GENERAL_CAPTIVAL_PORTAL)","y")
CFLAGS += -DAPPS_GENERAL_CAPTIVAL_PORTAL
export CONFIG_APPS_GENERAL_CAPTIVAL_PORTAL 
endif

all: download  mksym build stub

download: 3g-mngr 

mksym:
	#$(ROOTDIR)/tools/mksymlink  $(ROOTDIR)/../${CUSTOMER}/apps/logic/new3g    $(ROOTDIR)/build/apps/logic/new3g
	touch mksym
	

3g-mngr:
	@echo "do download 3g-mngr.... form:"
	@echo "$(CONFIG_SVN_PATH_FOR_3G)"	
	(cd $(SRC_3G); svn co "$(CONFIG_SVN_PATH_FOR_3G)"; cd -)
	
build:
	@echo "do build"
	chmod 777 -R $(ROOTDIR)/../${CUSTOMER}/apps/logic/new3g/*
	$(MAKE) INSTALL_DIR=$(FSROOT) SYSTEM=TBS -C 3g-mngr

stub:
	$(MAKE) -C 3g-stub

clean:
	@echo "do clean"
	rm -rf *.o
	$(MAKE) -C 3g-stub clean
	$(MAKE) -C 3g-mngr clean
	#rm -rf 3g-mngr
	#rm -rf mksym
	
distclean:
	@echo "do distclean"
	rm -rf *.o
	$(MAKE) -C 3g-stub clean
	$(MAKE) -C 3g-mngr clean
	rm -rf 3g-mngr
	rm -rf mksym
	
install: install_mngr
	@echo "do install"
	mkdir -p $(FSROOT)/usr/bin

	@echo "cp 3g-stub"
	cp -rf $(ROOTDIR)/build/apps/logic/new3g/3g-stub/3g-stub   $(FSROOT)/usr/bin/3g-stub
	$(STRIP) $(FSROOT)/usr/bin/3g-stub	

	@echo "Install ppp script!"
	if [ ! -d $(PPP_SCRIPT_DIR) ]; then mkdir -p $(PPP_SCRIPT_DIR); fi
	install -m 755 ip-up $(PPP_SCRIPT_DIR)
	install -m 755 ip-down $(PPP_SCRIPT_DIR)
	
c cr clear :
	@echo "do clear"
	rm -rf *.o
	$(MAKE) -C 3g-stub clean
	$(MAKE) -C 3g-mngr clean
	rm -rf mksym

install_mngr:
	$(MAKE) -C 3g-mngr install
#
#scb+ 2011-8-24 add help info
#
#
help:
	@echo "make svn_ci LOG_CI=...		Check in code to the main line"
	@echo "make svn_co LOG_CI=...        Check out code from the main line" 

#
#scb+ 2011-8-24 check in the 3g-mngr code to the trunk
#
#
svn_ci:
	@echo "Check in code to the main line"
	@if [ ! -z  "$(LOG_CI)"  ]; then \
		./svnup.sh "$(LOG_CI)" $(CONFIG_SVN_PATH_FOR_3G); \
	else \
		echo "Please Enter log,such as 'make svn_ci LOG_CI=... ' "; \
	fi

#
#scb+ 2011-8-24 check out the 3g-mngr code from the trunk
#
#
svn_co:
	@echo "Check out code from the main line"
	@if [ ! -z  "$(LOG_CI)"  ]; then \
		./svndown.sh "$(LOG_CI)" $(CONFIG_SVN_PATH_FOR_3G); \
	else \
		echo "Please Enter log,such as 'make svn_co LOG_CI=... ' "; \
	fi
