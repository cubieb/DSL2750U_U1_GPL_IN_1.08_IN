include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

INCLUDE=

DEFS=

CFLAGS=-D_GNU_SOURCE -O2 -Wstrict-prototypes -Wall -g -Iinclude-glibc -include include-glibc/glibc-bugs.h
KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
CFLAGS += -nostdinc -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/include -I$(TOOLCHAINS_SYSROOT)/include/gcc

LIBS=-lresolv

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc

SRCS=ping.c ping_common.c
     
OBJS=$(SRCS:.c=.o)
DEPENDS=$(SRCS:.c=.d)
TARGET=tbsping

all:$(TARGET)

$(TARGET): $(OBJS)
	$(LD) -o $@ $^ $(LIBS)
	
$(OBJS):%.o:%.c
	$(CC) $(INCLUDE) $(DEFS) $(CFLAGS)  -c $< -o $@
	
sinclude $(DEPENDS)

%.d: %.c
	$(CC) $(INCLUDE) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

install:
	$(Q)cp tbsping  $(FSROOT)/bin/
	$(Q)$(STRIP) $(FSROOT)/bin/tbsping
	
clean:
	rm -f $(TARGET) $(OBJS) $(DEPENDS) *.da *.bb *.bbg *.c.gcov trace.log tr069p *.o
