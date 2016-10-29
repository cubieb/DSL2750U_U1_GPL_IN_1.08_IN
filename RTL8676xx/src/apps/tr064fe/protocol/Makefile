OBIN = libtr64proto.o
all : $(OBIN)

INC_PATH = -I../inc  -I../public -I../cfgtree
CFLAGS += $(INC_PATH) $(ALLOWED_INCLUDE_PATHS)

OBJS = device.o gena.o http.o service.o soap.o upnp.o ssdp.o uio.o shutils.o digest.o session.o linux_osl.o linux_timer.o

$(OBIN):	$(OBJS) 
	$(LD) $(EXTRA_LDFLAGS) -r -o $(OBIN) $(OBJS) 

clean :
	rm -f *~ $(OBJS) $(OBIN)
	rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_$@) -c -o $@ $<
