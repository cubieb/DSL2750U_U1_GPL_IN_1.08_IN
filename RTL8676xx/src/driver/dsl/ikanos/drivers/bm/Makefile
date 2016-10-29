DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/bm/
EXTRA_CFLAGS += -DBUFFMANAGER

BMDRV_OBJ :=    bmdriver.o \
                ../../ap_code/user_config/bm/bmapif.o \
                ../../ap_code/buffmanager/apbmload.o \
                ../../ap_code/buffmanager/apbm_syms.o 

obj-m := bmdriver_lkm.o
bmdriver_lkm-objs := $(BMDRV_OBJ)

all:  
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules  
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/bmdriver_lkm.ko
	cp -f bmdriver_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
	rm -f ../../ap_code/buffmanager/*.o ../../ap_code/buffmanager/*.mod.o \
                ../../ap_code/buffmanager/*.o.cmd
	rm -f ../../ap_code/user_config/bm/*.o
