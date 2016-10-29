include $(ROOTDIR)/.config
DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/ap_code/lib/

FUSIVLIB_OBJ := initlib.o syscalls.o buftrans.o atmupstfpath.o generic_lib.o \
        		netpro/apload.o netpro/appmem.o netpro/apdmem.o \
        		netpro/hostconfig.o netpro/hostbridge.o \
        		netpro/hostapipc.o netpro/hostqos.o netpro/hostroute.o netpro/hostap.o \
        		netpro/ipv6_api.o netpro/hostroute_syms.o \
        		gpiolib/gpioapi.o gpiolib/ipc_syms.o gpiolib/gpioisr.o gpiolib/gpio_adi6843.o \
				../buffmanager/bmmain_syms.o netpro/ipcqos.o

ifeq ($(CONFIG_FUSIV_VX180),y)
FUSIVLIB_OBJ += ../buffmanager/bmmain_vx180.o  mips24ke_cache.o vx180_cache.o 
else
FUSIVLIB_OBJ += ../buffmanager/bmmain.o
endif

obj-m := fusivlib_lkm.o
fusivlib_lkm-objs := $(FUSIVLIB_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/fusivlib_lkm.ko
	cp -f fusivlib_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)
	
clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
	rm -rf  syscalls.o buftrans.o atmupstfpath.o \
            netpro/apload.o netpro/appmem.o netpro/apdmem.o \
            netpro/hostconfig.o netpro/hostbridge.o \
            netpro/hostapipc.o netpro/hostqos.o netpro/hostroute.o netpro/hostap.o \
            netpro/ipv6_api.o netpro/hostroute_syms.o \
            gpiolib/gpioapi.o gpiolib/ipc_syms.o gpiolib/gpioisr.o gpiolib/gpio_adi6843.o \
            ../buffmanager/bmmain.o
