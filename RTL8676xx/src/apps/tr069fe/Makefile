ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
CURDIR = $(shell pwd)
CROSSTARGET:=$(patsubst %-uclibc,%,$(TARGET))
TARGET = tb_tr069
TARGET2 = verify
SRC2 = verify.c

CONFIG_TBS_SSL = y
export CONFIG_TBS_SSL

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/${TBS_KERNEL_VERSION}/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOO \
LCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

#CFLAGS += -DHAS_COOKIE_CODE -DHAVE_SSL -DUSE_SSL -Wall #-DTR069_FE_DEBUG

EXTRA_CFLAGS = -I${BUILDDIR}/apps/ssap/msg/ -I${BUILDDIR}/apps/include -I${BUILDDIR} -I${BUILDDIR}/include
ifeq ($(CONFIG_TR069_PRINT_PACKAGE_TO_FILE),y)
CFLAGS += -DPRINT_PACKAGE_TO_FILE
endif

ifeq ($(CONFIG_TR069_CN_CHECK),y)
CFLAGS += -DSPECIAL_CHECK_CN
endif

ifeq ($(CONFIG_TR069_SSL),y)
CFLAGS += -DHAS_COOKIE_CODE -Wall -D_TR069_SSL
EXTRA_LDFLAGS = -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lz -lexpat  -lssl -lcrypto  -liconv -ldl
else
CFLAGS += -DHAS_COOKIE_CODE -Wall # -D_TR069_SSL # for openssl-0.9.8 compile warning, heym 20080814
EXTRA_LDFLAGS = -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lz -lexpat # -lssl -lcrypto  -liconv -ldl
endif

W3C_LIB = w3c-libwww-5.4.0
XML_LIB = xml_lib
#CM_LIB = cm_lib
#CMZ_LIB = cmz_lib
#IRESOLV_LIB = iresolv_lib
TMPDIST_LIB = $(ROOTDIR)/build/tmpdist/lib
CFGMGR_INCLUDE = $(CURDIR)/cfgmgr_include
W3C_INCLUDE = $(CURDIR)/$(W3C_LIB)/include/w3c-libwww
TMPDIST_INCLUDE = $(ROOTDIR)/build/tmpdist/include
LIBS = ./lib

CFLAGS += -I$(CFGMGR_INCLUDE) -I$(W3C_INCLUDE) $(EXTRA_CFLAGS) 
LDFLAGS += -L$(LIBS) -L$(TMPDIST_LIB) $(EXTRA_LDFLAGS)

# ifeq ($(CONFIG_TBS_SSL),y)
# LDFLAGS += -lpthread
# else
# LDFLAGS += -lpthread
# endif

obj-y = cfg_interface.o ILibParsers.o SOAPParser.o soapResponseProcessor.o soapMessageConstruct.o TR069Errno.o TR069StateMachine.o TR069Module.o \
        http_client.o mp-qm_interface.o #safe-ctype.o hash.o input_int.o 

#obj-y += cm_lib/msgparse.o cm_lib/msgqueue.o cm_lib/keyvalue.o cm_lib/comm.o cm_lib/message.o
#obj-y +=HTLine.o

CFLAGS += -DHAVE_CONFIG_H -Iw3c -I${TMPDIST_INCLUDE} -I${TMPDIST_INCLUDE}/openssl
ifeq ($(CONFIG_TR069_SSL),y)
all :  $(TARGET) $(TARGET2)
	@echo "#----------------------------------------------------"
	@echo "#"
	@echo "#    finished compiling cm_tr069!!!"
	@echo "#"
	@echo "#----------------------------------------------------"
else
all :  $(TARGET)
	@echo "#----------------------------------------------------"
	@echo "#"
	@echo "#    finished compiling cm_tr069!!!"
	@echo "#"
	@echo "#----------------------------------------------------"
endif
now3c: $(CM_LIB) $(XML_LIB) $(CMZ_LIB) $(TARGET)
	@echo "#----------------------------------------------------"
	@echo "#"
	@echo "#    finished compiling cm_tr069!!!"
	@echo "#"
	@echo "#----------------------------------------------------"


w3c_lib_build:
	tar -xjvf $(W3C_LIB).tar.bz2
	cd $(W3C_LIB); echo $(W3C_LIB); echo $(shell pwd); CC=${CROSS_COMPILE}gcc LD=${CROSS_COMPILE}ld AR=${CROSS_COMPILE}ar CFLAGS="-I${TMPDIST_INCLUDE} -I${TMPDIST_INCLUDE}/openssl" ./configure --host=$(CROSSTARGET) --enable-shared=no --enable-static=yes; cd ..

$(W3C_LIB) :
	make -C $(W3C_LIB)
	cp -f $(W3C_LIB)/Library/src/.libs/*.a $(LIBS)
	cp -f $(W3C_LIB)/Library/src/SSL/.libs/*.a $(LIBS)
	cp -f $(W3C_LIB)/modules/md5/.libs/*.a $(LIBS)
	#cp -f $(W3C_LIB)/Library/src/.libs/*.so* $(LIBS)
	#cp -f $(W3C_LIB)/Library/src/SSL/.libs/*.so* $(LIBS)
	#cp -f $(W3C_LIB)/modules/md5/.libs/*.so* $(LIBS)

$(CM_LIB) :
	make -C $(CM_LIB)
	cp -f $(CM_LIB)/*.so $(LIBS)

$(CMZ_LIB) :
	make -C $(CMZ_LIB)
	cp -f $(CMZ_LIB)/*.so* $(LIBS)
	cp -f $(CMZ_LIB)/*.a $(LIBS)

$(XML_LIB) :
	make -C $(XML_LIB)
	cp -f $(XML_LIB)/*.so $(LIBS)


$(TARGET) : $(obj-y)
	rm -f $@
	$(CC) -o $@ $(CFLAGS) $(filter $(obj-y), $^) $(LDFLAGS)

$(TARGET2) :
	$(CC) -o $(TARGET2) $(SRC2) $(CFLAGS) $(LDFLAGS) -Os

.PHONY : $(XML_LIB) $(CM_LIB) $(CMZ_LIB) $(W3C_LIB) $(IRESOLV_LIB) clean distclean

clean :
	@echo $(obj-y)
	find . \( -name '*.[oas]' -o -name core -o -name '*~' -o -name '.*~' -o -name '.#*' -o -name '*.so' -o -name '*.so.*' \) -type f -print | xargs rm -f
	rm -f $(TARGET)
	rm -f $(TARGET2)
#	$(MAKE) -C $(XML_LIB) clean
#	$(MAKE) -C $(CM_LIB) clean
#	$(MAKE) -C $(CMZ_LIB) clean
	rm -f $(W3C_LIB)/lib/*
	rm -f $(LIBS)/*

distclean : clean

ifeq ($(CONFIG_TR069_SSL),y)
install :
	cp -f $(TARGET) $(FSROOT)/sbin
	$(STRIP) $(FSROOT)/sbin/$(TARGET)
	cp -f $(TARGET2) $(FSROOT)/sbin
	$(STRIP) $(FSROOT)/sbin/$(TARGET2)
else
install :
	cp -f $(TARGET) $(FSROOT)/sbin
	$(STRIP) $(FSROOT)/sbin/$(TARGET)
endif
