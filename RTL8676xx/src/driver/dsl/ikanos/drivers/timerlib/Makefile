DRV_INSTALL_DIR := ${MODULEPATH}/ikanos/drivers/timerlib/

TIMER_OBJ := timers.o

obj-m := timers_lkm.o
timers_lkm-objs := $(TIMER_OBJ)

all: 
	$(KMAKE) -C $(PATH_KERNEL) M=`pwd` modules 
	
install:create_dir
	rm -f $(DRV_INSTALL_DIR)/timers_lkm.ko
	cp -f timers_lkm.ko $(DRV_INSTALL_DIR)

create_dir:
	test -d $(DRV_INSTALL_DIR) || mkdir -p $(DRV_INSTALL_DIR)

clean:
	rm -f *.ko *.o *.mod.o */*.o *~ */*~ core .depend .*.cmd *.mod.c */.*.o.cmd
	rm -rf .tmp_versions

