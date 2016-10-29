CC=mips-uclibc-gcc
TARGET=supp
OBJS=main.o cardif.o statemachine.o eap.o MD5.o
CFLAGS= 
.PHONY:all TBS_supp

all:$(TARGET)

TBS_supp: $(OBJS)
	$(CC) -o supp $(OBJS)

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) 
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
clean:
	rm -f $(TARGET) $(OBJS)
