include $(ROOTDIR)/.config
DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/peri_ap/

EXTRA_CFLAGS += -DPER
PERIAPDRIVER_OBJ := peri_ap_driver.o  \
					../../ap_code/peri_ap/apperiload.o \
					../../ap_code/user_config/peri/periapif.o

ifeq ($(CONFIG_FUSIV_VX180),y)
PERIAPDRIVER_OBJ += ../../ap_code/peri_ap/periapmain_vx180.o
else
PERIAPDRIVER_OBJ += ../../ap_code/peri_ap/periapmain.o
endif

obj-m := periap_driver_lkm.o
periap_driver_lkm-objs := $(PERIAPDRIVER_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/periap_driver_lkm.ko
	cp -f periap_driver_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
	rm -f ../../ap_code/peri_ap/apperiload.o
	rm -f ../../ap_code/peri_ap/periapmain.o
