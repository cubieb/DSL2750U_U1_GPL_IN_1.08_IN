TR64CFGLIB = libtr64cfg.o
all : $(TR64CFGLIB)

OBJS += cmmif.o commhandle.o

$(TR64CFGLIB):$(OBJS) 
	$(LD) $(EXTRA_LDFLAGS) -r -o $(TR64CFGLIB) $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) -c -o $@ $<
	
clean :
	rm -f *~ $(OBJS) $(TR64CFGLIB) *.o
