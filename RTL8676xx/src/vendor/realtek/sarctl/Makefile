
EXEC = sarctl
OBJS = sarctl.o
CFLAGS := -pipe -I$(BUILDDIR)/kernel/linux-2.6.30.9/include
all: ${EXEC}

${EXEC}: ${OBJS}

install:
	mkdir -p $(FSROOT)/usr/bin/
	install -m 755 ${EXEC} $(FSROOT)/usr/bin/
	$(STRIP) $(FSROOT)/usr/bin/${EXEC}

clean:
	-rm -f ${EXEC} *.elf *.o
