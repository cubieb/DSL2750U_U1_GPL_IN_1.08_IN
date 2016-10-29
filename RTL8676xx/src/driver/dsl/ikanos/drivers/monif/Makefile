DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/monif/

MONIF_OBJ := monif.o

obj-m := monif_lkm.o
monif_lkm-objs := $(MONIF_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/monif_lkm.ko
	cp -f monif_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
