ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config

all:first_rule
IntelFrameWork=y
LIBID3TAG=y
export LIBID3TAG

#FFMPEG=y
#export FFMPEG
ifdef LIBID3TAG
subdir-y += libid3tag
endif

ifdef FFMPEG
subdir-y += ffmpeg
endif

ifdef IntelFrameWork
subdir-y += ChinaTelecomDLNA
endif
ifdef USHARE
subdir-y += ffmpeg
subdir-y += libupnp
subdir-y += libdlna
subdir-y += ushare
endif
include $(ROOTDIR)/Rules.make

