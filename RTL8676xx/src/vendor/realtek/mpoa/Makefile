
all: mpoactl mpoad
OBJS := mpoad.o text2atm.o ans.o misc.o text2qos.o
CFLAGS := -pipe -Wall -shared -I$(BUILDDIR)/kernel/linux-2.6.18.8/include

mpoactl: mpoactl.o
mpoad: mpoad.o ${OBJS}	

install:
	mkdir -p $(FSROOT)/usr/bin/
	install -m 755 mpoad $(FSROOT)/usr/bin/
	install -m 755 mpoactl $(FSROOT)/usr/bin/
	$(STRIP) $(FSROOT)/usr/bin/mpoad
	$(STRIP) $(FSROOT)/usr/bin/mpoactl
	
clean:
	-rm -f mpoactl mpoad *.elf *.o 
