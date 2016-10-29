DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/ap2ap/
AP2AP_OBJ :=    ap2aproute.o ap2apbridge.o ap2apmulticast.o ap2apmulticastrt.o

obj-m := ap2ap_lkm.o
ap2ap_lkm-objs := $(AP2AP_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/ap2ap_lkm.ko
	cp -f ap2ap_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
