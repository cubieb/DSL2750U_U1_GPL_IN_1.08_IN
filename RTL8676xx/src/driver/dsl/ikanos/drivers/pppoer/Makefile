DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/pppoer

PPPRELAY_OBJ := pppoerdrv.o pppoerelay.o ppprelaylib.o ppprelayconf.o \
				ppprelayflashconf.o pppoerelayap.o

obj-m := pppRelay_lkm.o
pppRelay_lkm-objs := $(PPPRELAY_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 

install:create_dir
	rm -f $(DRV_INSTALL_DIR)/pppRelay_lkm.ko
	cp -f pppRelay_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions
