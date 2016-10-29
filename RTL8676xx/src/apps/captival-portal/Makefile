#scb+ 2011-10-17

SYSTEM ?= PC
LIB_PATH ?= ../logic/new3g/3g-mngr/lib
INSTALL ?= install
STRIP ?= strip

ifeq ($(strip $(SYSTEM)), TBS)
LIB_PATH = ../logic/new3g/3g-mngr/lib
CFLAGS +=  -DHAVE_DEFINE_TYPE
endif

ifeq ($(strip $(SYSTEM)), PC)
FSROOT = 
endif


HEAD = captival-portal.h dns.h httpd.h iptables.h timer.h
CODE = captival-portal.c dns.c httpd.c iptables.c timer.c

HEAD += $(LIB_PATH)/3g-lib-cmd-msg.h
CODE += $(LIB_PATH)/3g-lib-cmd-msg.c

SOURCE = "$(HEAD) $(CODE)"

OBJS = $(shell  echo $(CODE)|sed 's/\.c/.o/g')

TARGET = captival

CFLAGS += -Werror

ifeq ($(strip $(SYSTEM)), TBS)
all:../logic/new3g/mksym $(OBJS)
else
all: $(OBJS)
endif
	echo "OBJS:$(OBJS)"
	$(CC) $(LD_FLAGS) -I$(LIB_PATH)  $(OBJS) -o $(TARGET) 

ifeq ($(strip $(SYSTEM)), TBS)
../logic/new3g/mksym:
	$(MAKE) -C ../logic/new3g mksym
endif

clean:
	rm -f 	$(TRAGET)  $(FSROOT)/usr/bin/$(TARGET) $(OBJS)

install:
	$(INSTALL) -m 755 $(TARGET)  $(FSROOT)/usr/bin/
	$(STRIP) $(FSROOT)/usr/bin/$(TARGET)

.c.o:
	$(CC) -I$(LIB_PATH) $(CFLAGS)   -c -o $*.o $*.c	