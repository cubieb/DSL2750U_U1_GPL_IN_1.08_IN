# peanuthull client makefile

prefix=$(TI_FILESYSTEM)
SBINDIR=/sbin
USRSBINDIR=${prefix}/sbin
USRBINDIR=${prefix}/bin


CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
INSTALL = install


EXEC = peanuthullc
OBJECTS = ph-client.o ph_encrypt.o md5.o ph_socket.o hmac_md5.o blowfish.o

all: $(EXEC)


.c.o:
	$(CC) -c $(CFLAGS) $<

$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXEC)



install: all


clean:
	-rm -f peanuthullc *.o 

