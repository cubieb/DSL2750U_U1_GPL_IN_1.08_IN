ifeq "$(ROOTDIR)" "" 
	export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
AUTO_BUILDDIR=$(CURDIR)/ffmpeg-0.5
EXTRA_PARAM=--arch=mips --disable-ffmpeg --disable-encoders \
            --disable-network --disable-ffplay  --disable-filters \
            --disable-ffserver  --disable-vhook --enable-small \
            --disable-aandct --disable-fft --disable-golomb --disable-mdct --disable-rdft \
            --disable-devices \
            --disable-decoders --disable-bsfs --disable-outdevs \
            --disable-indevs --disable-parsers --disable-demuxers \
            --disable-muxers --disable-protocols --enable-protocol=file \
            --enable-parser=mpegaudio \
            --enable-demuxer=mpegvideo \
            --enable-demuxer=mpegts  \
            --enable-demuxer=mp3 \


LIBS=
LDFLAGS=-Wl,-rpath,/usr/lib:/lib -L$(TMPDISTDIR)/lib
CFLAGS=-I$(TMPDISTDIR)/include -Os
export LIBS LDFLAGS CFLAGS
MYMAKE=make -C $(AUTO_BUILDDIR)
.PHONY: all clean distclean 
all:configure build

configure:
	cd $(AUTO_BUILDDIR);\
	./configure --prefix=$(TMPDISTDIR) --enable-cross-compile --cross-prefix=$(CROSS_COMPILE) $(EXTRA_PARAM)
	touch configure
# build file depends
build:configure
	$(MYMAKE)
	$(MYMAKE) install
	touch build
clean:
	$(MYMAKE) clean
	rm -f build
distclean:    
	$(MYMAKE) distclean
	rm -f	configure build
        
install:

