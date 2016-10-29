DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/igmpsnoop

IGMP_SNOOP_OBJ := br_igmp_snoop.o br_igmp_snoop_ap.o

obj-m :=  igmpsnoop_lkm.o
igmpsnoop_lkm-objs := $(IGMP_SNOOP_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/igmpsnoop_lkm.ko
	cp -f igmpsnoop_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)


clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions