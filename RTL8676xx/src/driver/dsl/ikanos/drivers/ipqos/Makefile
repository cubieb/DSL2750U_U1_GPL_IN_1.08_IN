DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/ipqos

EXTRA_CFLAGS += LOCAL_IPQOS_FLAGS="-DCONFIG_FUSIV_KERNEL_IPQOS_TRAFFIC_SCHEDULER -DCONFIG_FUSIV_KERNEL_IPQOS_STATISTICS -DCONFIG_FUSIV_KERNEL_AP_2_AP_MODULE -DCONFIG_FUSIV_KERNEL_IPQOS_APQOS -DCONFIG_FUSIV_KERNEL_IPQOS_TC_MARKING -DCONFIG_FUSIV_KERNEL_IPQOS_TC_PRIORITY_CONVERSION -DCONFIG_FUSIV_KERNEL_IPQOS_TC_MAPPING -DCONFIG_FUSIV_KERNEL_IPQOS_DEBUG_LOG -DATM_UPSTREAM_THRU_HOST" 
IPQOS_OBJ := ipt_fusivTC.o ipqos.o wfqlib.o wfqtimer.o qoslib.o qosconf.o

obj-m :=  ipqos_lkm.o 
ipqos_lkm-objs := $(IPQOS_OBJ)
all: 
	echo "begin compile opensrc"
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/ipqos_lkm.ko
	cp -f ipqos_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions