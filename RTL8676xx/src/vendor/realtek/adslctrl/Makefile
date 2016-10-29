
all: adslctrl
adslctrl: adslctrl.o

#include $(ROOTDIR)/Rules.make
CFLAGS = -Os -Wall

install:
	mkdir -p $(FSROOT)/usr/bin/
	install -m 755 adslctrl $(FSROOT)/usr/bin/
	$(STRIP) $(FSROOT)/usr/bin/adslctrl

clean:
	-rm -f adslctrl *.elf *.o

