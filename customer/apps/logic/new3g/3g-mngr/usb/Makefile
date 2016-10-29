USB_LIB=libusb-0.1.12
USB_MODEMSWITCH_DIR=usb-modeswitch-1.1.3

#USB_CC=/opt/toolchains/uclibc-crosstools-gcc-4.2.3-3/usr/bin/mips-linux-gcc
USB_CC=$(CC)
USB_PWD=$(shell pwd)
USB_CFLAG="-I$(USB_PWD)/$(USB_LIB)" 
USB_LDFLAGS="-l usb -L$(USB_PWD)/$(USB_LIB)/.libs" 

ifeq ($(strip $(SYSTEM)), TBS)
#scb+ 2011-10-18 for Nucom, not defined CXX
ifeq ($(strip $(CXX)), g++)
CXX = $(shell echo $(CC)|sed 's/gcc/g++/g')
endif

CXX_INC ?= -I/opt/tbs_toolchains/realtek/toolchain_mips_nofpu/include/c++/3.2.3-1.2.8 \
                   -I/opt/tbs_toolchains/realtek/toolchain_mips_nofpu/include/c++      \
                   -I/opt/tbs_toolchains/realtek/toolchain_mips_nofpu/include/c++/3.2.3-1.2.8/mips-linux

INCLUDES +=  $(CXX_INC)

CFLAGS =

export 	INCLUDES CFLAGS CXX
endif

all: $(USB_LIB)/Makefile
	make -C  $(USB_LIB) 
	make -C  $(USB_MODEMSWITCH_DIR)  CC=$(USB_CC)  CFLAGS+=$(USB_CFLAG) LDFLAGS+=$(USB_LDFLAGS)

	cp $(USB_MODEMSWITCH_DIR)/device_reference.txt $(USB_MODEMSWITCH_DIR)/_device_reference.txt 
	sed -i -e '/^[ \t]*[^;].*/d' -e '/^$$/d'  $(USB_MODEMSWITCH_DIR)/_device_reference.txt 

clean:
	@echo "Clean usb switch and libusb..."
	(make -C  $(USB_MODEMSWITCH_DIR) clean 2> /dev/null || true)
	(make -C  $(USB_LIB) clean 2> /dev/null || true)
	rm -rf $(USB_MODEMSWITCH_DIR)/usb_modeswitch 
	rm -rf $(USB_MODEMSWITCH_DIR)/_device_reference.txt
	rm -rf $(USB_LIB)/Makefile

install:
	@echo "Install usb..."
	mkdir -p $(MAKE_ROOT_DIR)/$(USB_SWITCH_DIR)
	install -m 755  $(USB_MODEMSWITCH_DIR)/_device_reference.txt  $(MAKE_ROOT_DIR)/$(USB_SWITCH_DIR)/$(USB_SWITCH_FILE)
	install -m 755  $(USB_MODEMSWITCH_DIR)/usb_modeswitch   $(MAKE_ROOT_DIR)/usr/bin/
	install -m 755  $(USB_LIB)/.libs/libusb*.so.*  $(MAKE_ROOT_DIR)/lib/
	$(STRIP)   $(MAKE_ROOT_DIR)/usr/bin/usb_modeswitch
uninstall:
	@echo "Uninstall usb..."
	rm -rf $(USB_SWITCH_DIR) 
	rm -rf $(MAKE_ROOT_DIR)/$(USB_SWITCH_DIR)/$(USB_SWITCH_FILE)
	rm -rf $(MAKE_ROOT_DIR)/usr/bin/usb_modeswitch
	rm -rf $(MAKE_ROOT_DIR)/lib/libusb*.so.*




	
ifeq ($(strip $(SYSTEM)), BRCM)
$(USB_LIB)/Makefile:
	(cd $(USB_LIB); export CFLAGS= ; ./configure --host=mips-linux; cd -;)
else

ifeq ($(strip $(SYSTEM)), TBS)
$(USB_LIB)/Makefile:
	@echo "####################################"
	@echo "CXX=$(CXX)"
	@echo "CC=$(CC)"
	@echo "CXX_INC=$(CXX_INC)"
	@echo "PATH=$(PATH)"
	@echo "####################################"
	(cd $(USB_LIB); export CFLAGS= ; ./configure --host=$(ARCH)-linux; cd -;)
else	
$(USB_LIB)/Makefile:
	(cd $(USB_LIB); export CFLAGS= ; ./configure --host=i386-linux; cd -;)
endif

endif

