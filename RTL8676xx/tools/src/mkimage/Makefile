#include ${ROOTDIR}/.config
#ifeq ($(CONFIG_DOUBLE_BACKUP),y)
#CFLAGS += -DCONFIG_DOUBLE_BACKUP
#endif

CC=gcc
PROGS = mkimage 
OBJS += imgbuildermain.o imgbuilder.o cksum.o
CFLAGS += -I${ROOTDIR}/src/include

all : mkimage 
$(OBJS):imgbuilder.h  $(ROOTDIR)/src/include/flash_layout.h 
mkimage:$(OBJS) 
	$(CC) -o ${PROGS} $(OBJS)

clean:
	rm -f $(PROGS) $(OBJS)
