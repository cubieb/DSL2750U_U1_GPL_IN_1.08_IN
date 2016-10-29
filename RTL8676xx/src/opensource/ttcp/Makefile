ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all clean distclean
all:
	${CC} -static -o ttcp epi_ttcp.c

clean:
	rm -f *.o ttcp

	
distclean:clean


install:
	#cp -Pf $(TMPDISTDIR)/lib/libtcpdump.so* $(FSROOT)/lib
	#${STRIP} $(TMPDISTDIR)/lib/libtcpdump.so*  
