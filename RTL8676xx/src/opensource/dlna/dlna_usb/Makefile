ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 

CURR_DIR	   = $(shell pwd)
EXE_DIR        = $(CURR_DIR)/dms/Build/Linux
FFMPEG_LIB_DIR = $(CURR_DIR)/libs/ffmpeg-0.5/lib
EXE = dlna_usb

all:build

build:
	make -C $(CURR_DIR)/libs/
	ln -sf $(FFMPEG_LIB_DIR)/libavcodec.a $(EXE_DIR)/libavcodec.a
	ln -sf $(FFMPEG_LIB_DIR)/libavformat.a $(EXE_DIR)/libavformat.a
	ln -sf $(FFMPEG_LIB_DIR)/libavutil.a $(EXE_DIR)/libavutil.a
	make -C $(EXE_DIR) -f Makefile_Mips_Framework
	make -C $(EXE_DIR) -f Makefile_Mips_server

clean:
	make -C $(CURR_DIR)/libs/ clean
	rm -f $(EXE_DIR)/libavcodec.a
	rm -f $(EXE_DIR)/libavformat.a
	rm -f $(EXE_DIR)/libavutil.a
	make -C $(EXE_DIR) -f Makefile_Mips_Framework clean
	make -C $(EXE_DIR) -f Makefile_Mips_server clean
	rm -f $(EXE_DIR)/$(EXE)

distclean: clean
	rm -f configure
	
install:
	install -d $(FSROOT)/sbin
	install -m 755 $(EXE_DIR)/$(EXE) $(FSROOT)/sbin
	$(STRIP) $(FSROOT)/sbin/$(EXE)


