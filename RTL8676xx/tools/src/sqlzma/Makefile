ifeq "$(ROOTDIR)" ""
        export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

include $(ROOTDIR)/.config
Sqlzma = ${CURDIR}

LzmaVer = lzma457

ifeq ($(CONFIG_SQUASHFS_V_3_3), y)
SqVer = squashfs3.3
LzmaVer = lzma457
endif

ifeq ($(CONFIG_SQUASHFS_V_3_4), y)
SqVer = squashfs3.4
LzmaVer = lzma457
endif

ifeq ($(CONFIG_SQUASHFS_V_4_0), y)
SqVer = squashfs4.0
LzmaVer = lzma465
endif

ifeq ($(CONFIG_SQUASHFS_V_4_1), y)
SqVer = squashfs4.1
LzmaVer = lzma465
endif

Lzma = ${Sqlzma}/${LzmaVer}
Squashfs = ${Sqlzma}/${SqVer}
KVer = ${TBS_KERNEL_VERSION}
SqFs = ${Squashfs}/kernel-patches/${KVer}/fs/squashfs
#KDir = /lib/modules/$(shell uname -r)/build
LzmaAlone = ${Sqlzma}/lzma457/CPP/7zip/Compress/LZMA_Alone

ifeq (${LzmaVer}, lzma443)
LzmaC = ${Lzma}/C/7zip/Compress/LZMA_C
endif

ifeq (${LzmaVer}, lzma457)
LzmaC = ${Lzma}/C/Compress/Lzma
endif

SqTools = ${Squashfs}/squashfs-tools

# enable it if you want to add -g option when compiling
UseDebugFlags =
# disable it if you don't want to compile squashfs kernel module here
#BuildSquashfs = 1

export 

all:
ifeq (${LzmaVer}, lzma457)
	${MAKE} -C ${LzmaC} -f sqlzma.mk $@
endif
ifeq (${LzmaVer}, lzma465)
	${MAKE} -C ${Lzma}/C $@
endif
	${MAKE} -C ${SqTools} $@
	${MAKE} -C ${LzmaAlone} -f sqlzma.mk $@
	cp ${SqTools}/mksquashfs .
	
clean:
ifeq (${LzmaVer}, lzma457)
	${MAKE} -C ${LzmaC} -f sqlzma.mk clean
endif

ifeq (${LzmaVer}, lzma465)
	${MAKE} -C ${Lzma}/C clean
endif
	${MAKE} -C ${SqTools} $@
	${MAKE} -C ${LzmaAlone} -f sqlzma.mk clean
	rm -f mksquashfs ${SqTools}/mksquashfs
	${RM} *~

########################################

-include priv.mk

ifdef BuildSquashfs
CONFIG_SQUASHFS = m
CONFIG_SQUASHFS_EMBEDDED =
CONFIG_SQUASHFS_FRAGMENT_CACHE_SIZE = 3
EXTRA_CFLAGS = -I${Sqlzma} -I${SqFs}/../../include -Wall -Werror
EXTRA_CFLAGS += -DCONFIG_SQUASHFS_MODULE -UCONFIG_SQUASHFS
EXTRA_CFLAGS += -UCONFIG_SQUASHFS_EMBEDDED -DCONFIG_SQUASHFS_FRAGMENT_CACHE_SIZE=3
# if you enabled CONFIG_PREEMPT, but want CPU to try concentrating
# the uncompression, then define UnsquashNoPreempt.
# if you don't define UnsquashNoPreempt, the behaviour follows
# the CONFIG_PREEMPT.
EXTRA_CFLAGS += -DUnsquashNoPreempt
export

# the environment variables are not inherited since 2.6.23
MAKE += SQLZMA_EXTRA_CFLAGS="$(shell echo ${EXTRA_CFLAGS} | sed -e 's/\"/\\\\\\"/g')"

all: all_sq

FORCE:
all_sq:
	${MAKE} -C ${KDir} M=${SqFs} modules

clean: clean_sq
clean_sq:
	${MAKE} -C ${KDir} M=${SqFs} clean
endif

########################################

load:
	for i in ${LzmaC}/kmod/unlzma.ko ${LzmaC}/kmod/sqlzma.ko \
		${SqFs}/squashfs.ko; \
	do sudo insmod $$i; done

unload:
	-sudo rmmod squashfs sqlzma unlzma
