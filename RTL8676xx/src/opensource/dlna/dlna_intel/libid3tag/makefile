#
## $Copyright Open Broadcom Corporation$
#
## $Id: Makefile,v 1.12.12.2 2003/10/31 21:31:36 mthawani Exp $
#

ifndef DEBUG
#export DEBUG=1
endif

SIZE = $(CROSS_COMPILE)size
RANLIB = $(CROSS_COMPILE)ranlib


ifeq ($(DEBUG),1)
CFLAGS += -g -DDEBUG -UNDEBUG -DBCMDBG
else
CFLAGS += -s -Os -DNDEBUG
endif

CFLAGS += -Wall -Wunused -I./ -I$(TMPDISTDIR)/include/
#CFLAGS += --trace-includes
LDFLAGS += --allow-shlib-undefined -L. -lz

OBJS = version.o ucs4.o latin1.o utf16.o utf8.o parse.o render.o field.o \
	  frametype.o compat.o genre.o frame.o crc.o util.o tag.o file.o 

TARGET = libid3tag.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $(OBJS)
	$(RANLIB) $(TARGET)
	$(SIZE)  $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o *.a
install:

