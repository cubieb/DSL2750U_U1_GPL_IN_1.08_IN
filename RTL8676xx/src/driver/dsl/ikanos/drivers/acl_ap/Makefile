include $(ROOTDIR)/.config
DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/acl_ap/

OBJS := acl_ap_driver.o aclapi.o aclRFTcheck.o aclBFTcheck.o \
	    ../../ap_code/acl_ap/apaclload.o

ifeq ($(CONFIG_FUSIV_VX180),y)
OBJS += ../../ap_code/acl_ap/aclapmain_vx180.o
else
OBJS += ../../ap_code/acl_ap/aclapmain.o
endif

obj-$(CONFIG_FUSIV_LIBRARY) := aclap_driver_lkm.o
aclap_driver_lkm-objs := $(OBJS)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install: create_dir
	rm -f $(DRV_INSTALL_DIR)/aclap_driver_lkm.ko
	cp -f aclap_driver_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm ../../ap_code/acl_ap/*.o
