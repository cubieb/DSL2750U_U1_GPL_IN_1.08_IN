SOURCES   = 3g-lib-util.c  3g-lib-msg.c  3g-lib-eject.c 3g-lib.h 3g-lib-cmd-msg.c 
SOURCES += 3g-lib-cmd-msg.h 3g-lib-timer.c 3g-lib-timer.h


ifeq ($(strip $(MEMWATCH_CHECK_FOR_3G)),1)
CFLAGS += -DMEMWATCH -DMW_STDIO
SOURCES += memwatch.c memwatch.h
endif


OBJS = $(shell echo $(SOURCES) | sed -e 's/[^ ]*\.h//g' -e  's/\.c /\.o /g')
TARGET = lib3g-util.a



all:$(TARGET)

$(TARGET): $(OBJS) 
	ar crv $@  $?

.c.o:
	$(CC) $(CFLAGS)   -fPIC -c -o $*.o $*.c

clean:
	rm -f $(TARGET)
	rm -f ./*.o

	
