include $(ROOTDIR)/.config
DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/bmedriver/

ifdef CONFIG_FUSIV_VX180
BME_OBJ := bmedriver_vx180.o
else
BME_OBJ := bmedriver.o bmeapi.o
endif

obj-m :=  bmedrv.o
bmedrv-objs := $(BME_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/bmedrv.ko
	cp -f bmedrv.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
