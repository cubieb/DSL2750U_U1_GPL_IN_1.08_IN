include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all install clean
CFLAGS := -Wall -Os -I$(TMPDISTDIR)/include -I$(BUILDDIR) $(EXTRA_CFLAGS)

all: 2684ctl 2684d
OBJS := text2atm.o misc.o text2qos.o ans.o

2684ctl: 2684ctl.o
	$(CC) -o $@  $^
2684d: ${OBJS} 2684d.o
	$(CC) -o $@  $^

install:
	cp 2684ctl 2684d $(FSROOT)/sbin/
	$(STRIP) $(FSROOT)/sbin/2684d
	$(STRIP) $(FSROOT)/sbin/2684ctl

clean:
	-rm -f 2684ctl 2684d *.o *.d
