include $(ROOTDIR)/.config
DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/kernel/drivers/atm/

EXTRA_CFLAGS += -I$(DRV_BASE_DIR)/adsl
ATMDRIVER_OBJ :=        mini_at200_drv.o atmapdriver.o \
                        ../adsl/idma/adi_adsl_phy.o \
                        ../adsl/idma/adi_adsl_swap.o \
                        ../adsl/idma/idmaboot.o \
                        ../adsl/idma/rwfunc.o \
                        ../adsl/idma/adi_asm.o \
                        ../adsl/mgmt/atmconf.o \
                        ../adsl/mgmt/atmmib.o \
                        ../adsl/mgmt/atmlib.o \
                        ../adsl/mgmt/adslconfig.o \
                        ../adsl/mgmt/adsllib.o \
                        ../adsl/mgmt/cmvrw.o \
                        ../adsl/mgmt/snmp_syms.o \
                        ../adsl/mgmt/adsl2reg.o \
                        ../adsl/mgmt/adslutils.o \
                        ../adsl/mgmt/cmvutils.o \
                        ../../ap_code/atm/atmapload.o \
                        ../../ap_code/atm/utpconfig.o \
                        ../../ap_code/atm/atmhost.o \
                        ../../ap_code/atm/oamresponse.o \
                        ../../ap_code/atm/crc10.o \
                        ../../ap_code/atm/plane.o \
                        ../../ap_code/atm/atmapi.o \
                        ../../ap_code/atm/atm_misc.o \
                        ../../ap_code/user_config/atm/atmapif.o

ifeq ($(CONFIG_FUSIV_VX180),y)
ifeq ($(CONFIG_FUSIV_KERNEL_VX180_AVDSL),m)
ATMDRIVER_OBJ +=        ../../ap_code/atm/atmmain_adsl_180a.o
ATMDRIVER_OBJ +=        ../../ap_code/vdsl_ptm/driver/vdslptmdriver.o
ATMDRIVER_OBJ +=        ../../ap_code/vdsl_ptm/driver/vdslptm.o
ATMDRIVER_OBJ +=        ../../ap_code/vdsl_ptm/phy/fusiv_decoder.o
ATMDRIVER_OBJ +=        ../../ap_code/vdsl_ptm/phy/fusiv_encoder.o
ATMDRIVER_OBJ +=        ../../ap_code/vdsl_ptm/phy/fusiv_rx_slb.o
ATMDRIVER_OBJ +=        ../../ap_code/vdsl_ptm/phy/fusiv_tx_slb.o
ATMDRIVER_OBJ +=        ../../ap_code/user_config/vdsl_ptm/vdslptmif.o
else
ATMDRIVER_OBJ +=        ../../ap_code/atm/atmmain_180a.o
endif
else
ATMDRIVER_OBJ +=        ../../ap_code/atm/atmmain.o
endif

obj-m := atmdriver_lkm.o
atmdriver_lkm-objs := $(ATMDRIVER_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/atmdriver_lkm.ko
	cp -f atmdriver_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)
clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
	rm ../adsl/idma/adi_adsl_phy.o \
                        ../adsl/idma/adi_adsl_swap.o \
                        ../adsl/idma/idmaboot.o \
                        ../adsl/idma/rwfunc.o \
                        ../adsl/idma/adi_asm.o \
                        ../adsl/mgmt/atmconf.o \
                        ../adsl/mgmt/atmmib.o \
                        ../adsl/mgmt/atmlib.o \
                        ../adsl/mgmt/adslconfig.o \
                        ../adsl/mgmt/adsllib.o \
                        ../adsl/mgmt/cmvrw.o \
                        ../adsl/mgmt/snmp_syms.o \
                        ../adsl/mgmt/adsl2reg.o \
                        ../adsl/mgmt/adslutils.o \
                        ../adsl/mgmt/cmvutils.o \
                        ../../ap_code/atm/atmapload.o \
                        ../../ap_code/atm/utpconfig.o \
                        ../../ap_code/atm/atmhost.o \
                        ../../ap_code/atm/oamresponse.o \
                        ../../ap_code/atm/crc10.o \
                        ../../ap_code/atm/plane.o \
                        ../../ap_code/atm/atmmain.o \
                        ../../ap_code/atm/atmapi.o \
                        ../../ap_code/atm/atm_misc.o

