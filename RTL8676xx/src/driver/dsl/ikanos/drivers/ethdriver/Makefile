include $(ROOTDIR)/.config
DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/ethdriver/

SYSUTIL_OBJ := sysutil_kernel_code.o
ETHDRIVER_OBJ :=  driver.o userif.o oswrapper.o \
                  ../../ap_code/ethdriver/fusiv/osif.o \
                  ../../ap_code/ethdriver/std_ap/sraap.o \
                  ../../ap_code/ethdriver/std_ap/switchvlan.o \
                  ../../ap_code/ethdriver/std_ap/apethload.o \
                  ../../ap_code/ethdriver/std_ap/eth_syms.o \
                  ../../ap_code/user_config/eth/ethapif.o

ifeq ($(CONFIG_FUSIV_VX180),y)
ETHDRIVER_OBJ += ../../ap_code/ethdriver/std_ap/ethap_vx180.o
else
ETHDRIVER_OBJ += ../../ap_code/ethdriver/std_ap/ethap.o
endif

obj-m:= ethdriver_lkm.o sysKCode_lkm.o
ethdriver_lkm-objs := $(ETHDRIVER_OBJ)
sysKCode_lkm-objs := $(SYSUTIL_OBJ)

all:   
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules   
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/ethdriver_lkm.ko
	rm -f $(DRV_INSTALL_DIR)/sysKCode_lkm.ko
	cp -f ethdriver_lkm.ko $(DRV_INSTALL_DIR)
	cp -f sysKCode_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
	rm -f ../../ap_code/ethdriver/fusiv/osif.o
	rm -f ../../ap_code/ethdriver/phy/ieeephy.o
	rm -f ../../ap_code/ethdriver/std_ap/sraap.o
	rm -f ../../ap_code/ethdriver/std_ap/apethload.o
	rm -f ../../ap_code/ethdriver/std_ap/ethap.o
	rm -f ../../ap_code/ethdriver/std_ap/eth_syms.o
	rm -f ../../ap_code/ethdriver/std_ap/ethap_vx180.o
	rm -f ../../ap_code/user_config/eth/ethapif.o

