CC=gcc
PROGS = sstrip
OBJS += sstrip.o
CFLAGS += -I${ROOTDIR}/src/include

all : sstrip
mkimage:$(OBJS)
	$(CC) -o ${PROGS} $(OBJS)

clean:
	rm -f $(PROGS) $(OBJS)

install:
	cp ./sstrip ../../bin/ -rf
