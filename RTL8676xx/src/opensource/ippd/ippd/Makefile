CFLAGS=-Os -Wall -fomit-frame-pointer
LDFLAGS=-Os -Wl,-allow-shlib-undefined

OBJS=main.o conn.o printers.o http.o ipp.o array.o
TARGET_APP = ippd

all: $(TARGET_APP)

dynamic: ippd install

install: all
	install -c -m 755 $(TARGET_APP) $(FSROOT)/sbin
	$(STRIP) $(FSROOT)/sbin/$(TARGET_APP)
#	cp -f printers.ini $(FSROOT)/etc

main.o: main.c conn.h array.h

ipp.o: ipp.c ipp.h array.h

ippd: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET_APP)

clean:
	-rm -f $(TARGET_APP) $(OBJS)

