#include ${ROOTDIR}/.config
#ifeq ($(CONFIG_DOUBLE_BACKUP),y)
#CFLAGS += -DCONFIG_DOUBLE_BACKUP
#endif

CC=gcc
PROGS = mkimage_ex 
OBJS += imgbuilder_ex_main.o imgbuilder_ex.o cksum.o
CFLAGS += -I${ROOTDIR}/src/include -DCONFIG_BACKUP_IMAGE

all : mkimage_ex 
$(OBJS):imgbuilder_ex.h  $(ROOTDIR)/src/include/flash_layout.h 
mkimage_ex:$(OBJS) 
	$(CC) -o ${PROGS} $(OBJS)

clean:
	rm -f $(PROGS) $(OBJS)
