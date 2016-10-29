
EXEC = usbctl
OBJS = usbctl.o

all: $(EXEC)

CFLAGS += -I$(BUILDDIR)/kernel/$(TBS_KERNEL_VERSION)/include

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

install:
	mkdir -p $(FSROOT)/usr/bin/
	install -m 755 $(EXEC) $(FSROOT)/usr/bin/
	$(STRIP) $(FSROOT)/usr/bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.o

