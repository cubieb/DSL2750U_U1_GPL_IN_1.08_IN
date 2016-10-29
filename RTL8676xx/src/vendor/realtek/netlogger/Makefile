
EXEC = netlogger 
OBJS = netlogger.o

all: $(EXEC)


#$(EXEC): $(OBJS) 
	#$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) -L../boa/src/LINUX -lmib


install:
	mkdir -p $(FSROOT)/usr/bin/
	install -m 755 netlogger $(FSROOT)/usr/bin/
	$(STRIP) $(FSROOT)/usr/bin/netlogger

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.o

