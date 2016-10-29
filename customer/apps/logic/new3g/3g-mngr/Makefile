SYSTEM ?= BRCM
SUPPORT_IP_CONNECT_CHECK = 1
FIND_TTY_PORT_BY_INT_EP = 1


CONFIG=3g-mngr-config.h

PWD = $(shell pwd)

MAKE_ROOT_DIR 	?= $(INSTALL_DIR)
RUN_ROOT_DIR	?=

ifeq ($(strip $(SYSTEM)), TBS)
MNGR_DIR	?=  usr/bin
else
MNGR_DIR	?=	bin
endif

ifeq ($(strip $(SYSTEM)), PC)
MNGR_DIR 		= usr/bin
MAKE_ROOT_DIR 	= 
endif

ifeq ($(strip $(SYSTEM)), PC)
STRIP = strip
endif


export MAKE_ROOT_DIR RUN_ROOT_DIR STRIP



#trans device idVendor and idProduct to kernel
MODULE_DIR  = lib/modules/privat_module
MODULE_FILE = $(RUN_ROOT_DIR)/$(MODULE_DIR)/user_usb_modem.ko
MODULE_INSTALL = $(MAKE_ROOT_DIR)/$(MODULE_DIR)

MODULE_DEMAND_DIR 	  = $(MODULE_DIR)
MODULE_DEMAND_INSTALL = $(MAKE_ROOT_DIR)/$(MODULE_DEMAND_DIR)

PPP_PLUGIN_DIR 	   = lib
PPP_PLUGIN_INSTALL = $(MAKE_ROOT_DIR)/$(PPP_PLUGIN_DIR)


USB_SWITCH_FILE  = device_reference.txt
USB_SWITCH_DIR	 = etc/usb_modeswitch.d
USB_SWITCH_FILE1 = $(RUN_ROOT_DIR)/var/3gppp/device_reference_special.txt
USB_AT_CFG_FILE1 = $(RUN_ROOT_DIR)/var/3gppp/device_reference_special.txt
USB_SWITCH_FILE2 = $(RUN_ROOT_DIR)/$(USB_SWITCH_DIR)/$(USB_SWITCH_FILE)
USB_AT_CFG_FILE2 = $(RUN_ROOT_DIR)/$(USB_SWITCH_DIR)/$(USB_SWITCH_FILE)

ID_CONFIG_FILE = /proc/usb_modem_id


ifeq ($(strip $(SYSTEM)), BRCM)
KCC ?= $(CC)
KLD ?= $(LD)
KERNEL_DIR ?= $(PWD)/../../../../kernel/linux
else
ifeq ($(strip $(SYSTEM)), TBS)
KCC ?= /opt/tbs_toolchains/realtek/rsdk-1.3.6/linux/uclibc/bin/rsdk-elf-gcc
KLD ?= /opt/tbs_toolchains/realtek/rsdk-1.3.6/linux/uclibc/bin/rsdk-elf-ld
KERNEL_DIR ?= $(ROOTDIR)/build/kernel/linux-2.6.18.8
else

#PC
KCC ?= $(CC)
KLD ?= $(LD)
KERNEL_DIR ?= /usr/src/kernels/$(shell  ls /usr/src/kernels/)

endif
endif

export KERNEL_DIR




CFLAGS += -I./ 
CFLAGS += -Werror -Wall				
CFLAGS += -DSUPPORT_3G_WAN_MANAGEMENT
CFLAGS += -DPPP_PLUGIN_DIR=\"$(RUN_ROOT_DIR)/$(PPP_PLUGIN_DIR)\"
CFLAGS += -DMODULE_DEMAND_DIR=\"$(RUN_ROOT_DIR)/$(MODULE_DEMAND_DIR)\"
CFLAGS += -D$(SYSTEM)

ifeq ($(strip $(FIND_TTY_PORT_BY_INT_EP)),1)
CFLAGS+=-DFIND_TTY_PORT_BY_INT_EP
endif

ifeq ($(strip $(SUPPORT_IP_CONNECT_CHECK)),1)
CFLAGS+=-DSUPPORT_IP_CONNECT_CHECK
endif


LDFLAGS +=  -L./ -L./lib  -I./ 


export CFLAGS SYSTEM INSTALL_DIR 


.PHONY: 	usb   module

ifeq ($(strip $(SYSTEM)), BRCM)
all:       3g-lib 3g-mngr  usb   module ppp_plugin install
else
all:       3g-lib 3g-mngr  usb   module ppp_plugin
endif

clean:     3g-mngr_clean 3g-lib_clean usb_clean  module_clean ppp_plugin_clean
install:   uninstall 3g-mngr_install usb_install module_install ppp_plugin_install
uninstall: 3g-mngr_uninstall usb_uninstall module_uninstall ppp_plugin_uninstall



#for make 3g-mngr
3G_MNGROBJS = obj_mngr.o
3G_MNGROBJS +=	3g-mngr.o 3g-mngr-msg.o  3g-mngr-at.o  3g-mngr-wan.o \
				3g-mngr-modem.o 3g-mngr-pin.o 3g-mngr-switch.o \
				3g-mngr-parameter.o 3g-mngr-dial.o 3g-mngr-at-cfg.o \
				3g-mngr-comm.o 3g-mngr-connect-check.o 3g-mngr-web-interface.o
3G_AUTO = obj_mngr.c  3g-mngr-bit.h	modem_def.h 3g-mngr-at-cfg.c			

3g-mngr: 3g-lib $(3G_AUTO) $(3G_MNGROBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o 3g-mngr  $(3G_MNGROBJS) -l3g-util

3g-mngr_clean:
	rm -rf $(3G_MNGROBJS)
	rm -rf $(3G_AUTO)

__3g-mngr_install:
	install -m 755 3g-mngr  $(MAKE_ROOT_DIR)/$(MNGR_DIR)/
	$(STRIP) $(MAKE_ROOT_DIR)/$(MNGR_DIR)/3g-mngr

__3g-mngr_uninstall:
	rm -rf $(MAKE_ROOT_DIR)/$(MNGR_DIR)/3g-mngr
	
__hotplug_settins:__hotplug_settins_clean
	install -m 755 config3g /etc/init.d/config3g
	ln -s /etc/init.d/config3g /etc/rc0.d/S90config3g
	ln -s /etc/init.d/config3g /etc/rc1.d/S90config3g
	ln -s /etc/init.d/config3g /etc/rc2.d/S90config3g
	ln -s /etc/init.d/config3g /etc/rc3.d/S90config3g
	ln -s /etc/init.d/config3g /etc/rc4.d/S90config3g
	ln -s /etc/init.d/config3g /etc/rc5.d/S90config3g
	ln -s /etc/init.d/config3g /etc/rc6.d/S90config3g
	install -m 755 90-usb-3g.rules /etc/udev/rules.d/
	
__hotplug_settins_clean:	
	rm -rf /etc/init.d/config3g
	rm -rf /etc/init.d/config3g /etc/rc0.d/S90config3g
	rm -rf /etc/init.d/config3g /etc/rc1.d/S90config3g
	rm -rf /etc/init.d/config3g /etc/rc2.d/S90config3g
	rm -rf /etc/init.d/config3g /etc/rc3.d/S90config3g
	rm -rf /etc/init.d/config3g /etc/rc4.d/S90config3g
	rm -rf /etc/init.d/config3g /etc/rc5.d/S90config3g
	rm -rf /etc/init.d/config3g /etc/rc6.d/S90config3g
	rm -rf /etc/udev/rules.d/90-usb-3g.rules

ifeq ($(strip $(SYSTEM)), PC)
3g-mngr_install:__3g-mngr_install __hotplug_settins
3g-mngr_uninstall: __3g-mngr_uninstall __hotplug_settins_clean
else
3g-mngr_install: __3g-mngr_install
3g-mngr_uninstall: __3g-mngr_uninstall
endif

obj_mngr.c 3g-mngr-bit.h: 3g-mngr.h
	rm -f 3g-mngr-bit.h
	./3g-mngr.sh file 3g-mngr.h
	./3g-mngr.sh cmpl mngr obj_mngr.c

obj-mngr.o:obj_mngr.c
	$(CC) $(CFLAGS) -c -o $*.o $*.c

modem_def.h:
	echo "/*the file is auto generate!*/">$(PWD)/modem_def.h
	echo "#define MODULE_FILE \"$(MODULE_FILE)\"">>$(PWD)/modem_def.h
	echo "#define ID_CONFIG_FILE \"$(ID_CONFIG_FILE)\"">>$(PWD)/modem_def.h	
	echo "#define USB_AT_CFG_FILE1  \"$(USB_AT_CFG_FILE1)\"">>$(PWD)/modem_def.h
	echo "#define USB_AT_CFG_FILE2  \"$(USB_AT_CFG_FILE2)\"">>$(PWD)/modem_def.h	
	echo "#define USB_SWITCH_FILE1  \"$(USB_SWITCH_FILE1)\"">>$(PWD)/modem_def.h
	echo "#define USB_SWITCH_FILE2  \"$(USB_SWITCH_FILE2)\"">>$(PWD)/modem_def.h
	
3g-mngr-at-cfg.c:
	echo "/*This file is auto generate!*/" >$(PWD)/3g-mngr-at-cfg.c	
	
	echo "/*Come from enum type at_cmd_t */">>$(PWD)/3g-mngr-at-cfg.c
	echo "char *at_func_map[] = {">>$(PWD)/3g-mngr-at-cfg.c
	sed  -n '/AT_INIT/, /AT_CMD_END/p' 3g-mngr-at.h |sed -n '/AT_.*/p'  >.at_map_tmp
	cat .at_map_tmp|sed -e 's/=.*,/,/' -e 's/,.*//g' -e 's/\(AT[^ \t]*\)/"\1",/g'>>$(PWD)/3g-mngr-at-cfg.c
	echo "};">>$(PWD)/3g-mngr-at-cfg.c
	rm -f .at_map_tmp



#for make usb dir
usb:
	$(MAKE) -C usb
usb_clean:
	$(MAKE) -C usb clean
usb_install:	
	$(MAKE) -C usb install
usb_uninstall:	
	$(MAKE) -C usb uninstall	


#for make module
module:
	@echo  KERNEL_DIR is $(KERNEL_DIR)
	$(MAKE) -C $(KERNEL_DIR)	 M=$(PWD)/module CC=$(KCC) LD=$(KLD) modules
module_clean:	
	rm -rf  $(PWD)/module/*.ko  $(PWD)/module/*.o	$(PWD)/module/*.mod.* 
	rm -rf  $(PWD)/module/*.order $(PWD)/module/*.markers $(PWD)/module/*.symvers 
module_install:
	mkdir -p $(MODULE_INSTALL) $(MODULE_DEMAND_INSTALL)
	cp $(PWD)/module/user_usb_modem.ko  $(MODULE_INSTALL)/user_usb_modem.ko	
	cp $(PWD)/module/demand_hook.ko  $(MODULE_DEMAND_INSTALL)/demand_hook.ko
module_uninstall:	
	rm -rf  $(MODULE_INSTALL)/user_usb_modem.ko	
	rm -rf  $(MODULE_DEMAND_INSTALL)/demand_hook.ko


#scb+ 2012-7-19
PPP_PLUGIN_CFLAGS	= $(CFLAGS)  -fPIC 
PPP_PLUGIN_LDFLAGS	= $(LDFLAGS) -shared


ppp_plugin: ppp_demand_plugin.so
ppp_plugin_clean:	
	rm -rf  ppp_demand_plugin.so
ppp_plugin_install:
	mkdir -p $(PPP_PLUGIN_INSTALL)
	install -m 777 ppp_demand_plugin.so $(PPP_PLUGIN_INSTALL)/
ppp_plugin_uninstall:
	rm -rf $(PPP_PLUGIN_INSTALL)/ppp_demand_plugin.so
	
#ppp_demand_plugin.so:ppp_demand_plugin.o
#	@echo "make ppp plugin"
#	$(CC) -o $@ $(PPP_PLUGIN_LDFLAGS) $(PPP_PLUGIN_CFLAGS) $^



3g-lib:modem_def.h	
	$(MAKE) -C lib

3g-lib_clean:
	$(MAKE) -C lib clean

help:
	@echo ""
	@echo "support the following cmd:"
	@echo "	make"
	@echo "	make SYSTEM=PC"
	@echo "	make install"
	@echo "	make clean"
	@echo "	make uninstall"		
	@echo ""
	@echo "	make 3g-mngr"
	@echo "	make usb"
	@echo "	make 3g-lib "
	@echo "	make module"
	@echo ""
	@echo "	make 3g-mngr_clean"
	@echo "	make 3g-lib_clean"
	@echo "	make usb_clean"
	@echo ""	
	@echo "	make 3g-mngr_install"
	@echo "	make module_install"
	@echo "	make usb_install"
	@echo ""	
	@echo "	make 3g-mngr_uninstall"
	@echo "	make module_uninstall"
	@echo "	make usb_uninstall"	
	
.c.o:
	$(CC) $(CFLAGS) -c -o $*.o $*.c
export 	USB_SWITCH_DIR USB_SWITCH_FILE

