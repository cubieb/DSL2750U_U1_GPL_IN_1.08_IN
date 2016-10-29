#include ${ROOTDIR}/.config
#ifeq ($(CONFIG_DOUBLE_BACKUP),y)
#CFLAGS += -DCONFIG_DOUBLE_BACKUP
#endif

CC=gcc
PROGS = formatcsv 
OBJS += formatcsv.o
CFLAGS += -I${ROOTDIR}/src/include

all : formatcsv
mkimage:$(OBJS) 
	$(CC) -o ${PROGS} $(OBJS)

clean:
	rm -f $(PROGS) $(OBJS)

install:
	cp ./formatcsv ../../bin/ -rf
