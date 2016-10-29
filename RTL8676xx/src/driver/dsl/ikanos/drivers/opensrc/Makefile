DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/opensrc/
OPENSRC_OBJ := opensrc.o bme.o voice.o

obj-m := opensrc_lkm.o
opensrc_lkm-objs := $(OPENSRC_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install: create_dir
	rm -f $(DRV_INSTALL_DIR)/opensrc_lkm.ko
	cp -f opensrc_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions

