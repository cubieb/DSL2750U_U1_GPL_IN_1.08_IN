DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/policyVlan

VLANCLASSIFIER_OBJ := vlan_classifier.o policy_vlan.o

obj-m := vlan_classifier_lkm.o
vlan_classifier_lkm-objs := $(VLANCLASSIFIER_OBJ)
all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/vlan_classifier_lkm.ko
	cp -f vlan_classifier_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
