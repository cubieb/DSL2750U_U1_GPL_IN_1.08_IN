ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
ifdef CONFIG_OPENSOURCE_OPENSSL_098
include Makefile.098
endif
ifdef CONFIG_OPENSOURCE_OPENSSL_097 
include Makefile.097
endif
ifdef CONFIG_OPENSOURCE_OPENSSL_097_EAPTLS
include Makefile.097_EAPTLS
endif
