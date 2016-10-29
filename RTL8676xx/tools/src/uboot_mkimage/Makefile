#include ${ROOTDIR}/.config
#ifeq ($(CONFIG_DOUBLE_BACKUP),y)
#CFLAGS += -DCONFIG_DOUBLE_BACKUP
#endif

CC=gcc
PROGS = mkimage 
OBJS += src/mkimage.o src/crc32.o 
CFLAGS += -I${ROOTDIR}/src/include

all : mkimage 
$(OBJS): 
mkimage:$(OBJS) 
	$(CC) -o ${PROGS} $(OBJS)

clean:
	rm -f $(PROGS) $(OBJS)
