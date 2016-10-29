#
# hotplug: linux kernel hotplug entry program for TBS
#          target: libusb.a
#
PROGRAM_NAME:= libusb

EXTRA_LDFLAGS= -lssap -L$(BUILDDIR)/apps/ssap/lib \
		-L$(TMPDISTDIR)/lib 

EXTRA_CFLAGS=  -I$(BUILDDIR)/apps/include -I${BUILDDIR} -I$(BUILDDIR)/apps/ssap/msg -I../lib -I../hotplug
#              -I$(BUILDDIR)/alp/asp/om/include -I../om/omlib

#CUnit need
ifeq (${UT},USB_MASS)
CFLAGS+=-DUT_USB_MASS -g
endif
 
CFLAGS+= -DENABLE_HOTPLUG_DEBUG

HOTPLUGOBJS = usblib_device.o usblib_disk.o 
#usblib_debug.o

OBJS = $(HOTPLUGOBJS)

all: $(PROGRAM_NAME).a

.c.o:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $<

$(PROGRAM_NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $(PROGRAM_NAME) $(OBJS)

$(PROGRAM_NAME).a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

clean:
	-rm -f $(PROGRAM_NAME) *.o  $(PROGRAM_NAME).a

