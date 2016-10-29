include $(ROOTDIR)/.config
DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/vdsldriver/

VDSLDRIVER_OBJ := vdsldriver.o \
				  ../../ap_code/vdsl_ptm/driver/vdslptm.o \
				  ../../ap_code/vdsl_ptm/driver/vdslptmdriver.o \
				  ../../ap_code/vdsl_ptm/phy/fusiv_decoder.o \
				  ../../ap_code/vdsl_ptm/phy/fusiv_encoder.o \
				  ../../ap_code/vdsl_ptm/phy/fusiv_rx_slb.o \
				  ../../ap_code/vdsl_ptm/phy/fusiv_tx_slb.o \
				  ../../ap_code/vdsl_ptm/ptmap/apptmload.o \
				  ../../ap_code/vdsl_ptm/ptmap/vdslptmmain.o \
				  ../../ap_code/user_config/vdsl_ptm/vdslptmif.o

obj-m := vdsldriver_lkm.o
vdsldriver_lkm-objs := $(VDSLDRIVER_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/vdsldriver_lkm.ko
	cp -f vdsldriver_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions

