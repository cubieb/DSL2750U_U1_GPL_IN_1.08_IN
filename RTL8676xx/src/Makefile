ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config

subdir-y += boot filesystem kernel driver opensource apps vendor

include $(ROOTDIR)/Rules.make
#scb+ 2011-7-13 for new3g
export CONFIG_APPS_LOGIC_NEW3G
