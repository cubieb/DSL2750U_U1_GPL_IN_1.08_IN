# $Id: Makefile,v 1.4 2008/06/09 09:43:51 jenny Exp $

.SUFFIXES:
.SUFFIXES: .o .c
.PHONY: clean all depend

LIBS = 
CFLAGS = -g -O2 -Wall -fPIC
CPPFLAGS =  -I. -I.
DEPEND = .depend
LDFLAGS	= -g -s
#CC = mips-uclibc-gcc
CPP = gcc -E
#AR = mips-uclibc-ar

SOURCES = minixml.c, upnphttp.c, upnpreplyparse.c, upnpsoap.c \
	mini_upnp.c		
OBJS = $(SOURCES:.c=.o)

UPNP_DAEMON = mini_upnpd

ifeq ($(STATIC_LIB),1)
########## Build Static Library ##################
all: mini_upnp.a	

UPNP_LIB = mini_upnp.a

mini_upnp.a: mini_upnp.o minixml.o upnphttp.o upnpreplyparse.o upnpsoap.o
	$(AR) rcs $@ mini_upnp.o minixml.o upnphttp.o upnpreplyparse.o upnpsoap.o

else
########## Build Shared Library ##################
all: $(UPNP_DAEMON) $(UPNP_LIB)	

#UPNP_LIB = mini_upnp.so
ifdef CONFIG_USE_RSDK_WRAPPER
UPNP_LIB = libmini_upnp.a
UPNP_LIB_SO = libmini_upnp.so
else #CONFIG_USE_RSDK_WRAPPER
UPNP_LIB = mini_upnp.a
UPNP_LIB_SO = mini_upnp.so
endif #CONFIG_USE_RSDK_WRAPPER


CFLAGS += -DUSE_SHARED_DAEMON

minixml.o: minixml.c
	$(CC) -c -o $@ -fpic $(CFLAGS) $(IFLAGS) $<

upnphttp.o: upnphttp.c
	$(CC) -c -o $@ -fpic $(CFLAGS) $(IFLAGS) $<	

upnpreplyparse.o: upnpreplyparse.c
	$(CC) -c -o $@ -fpic $(CFLAGS) $(IFLAGS) $<		
	
upnpsoap.o: upnpsoap.c
	$(CC) -c -o $@ -fpic $(CFLAGS) $(IFLAGS) $<		
	
#$(UPNP_LIB): minixml.o upnphttp.o upnpreplyparse.o upnpsoap.o
#	$(CC) -s -shared -o $@ minixml.o upnphttp.o upnpreplyparse.o upnpsoap.o
$(UPNP_LIB): mini_upnp.o minixml.o upnphttp.o upnpreplyparse.o upnpsoap.o
	$(AR) $(ARFLAGS) $(UPNP_LIB) $?
	$(LD) -shared --warn-common --warn-once -z combreloc \
		-o $(UPNP_LIB_SO) --whole-archive $(UPNP_LIB)
ifdef CONFIG_USE_RSDK_WRAPPER
	install -m 644 $(UPNP_LIB_SO) $(ROOTDIR)/lib/librtk/$(UPNP_LIB_SO)
	install -m 644 $(UPNP_LIB) $(ROOTDIR)/lib/librtk/$(UPNP_LIB)
else #CONFIG_USE_RSDK_WRAPPER
	install -m 644 $(UPNP_LIB_SO) $(FSROOT)/lib/$(UPNP_LIB_SO)
endif #CONFIG_USE_RSDK_WRAPPER

$(UPNP_DAEMON): mini_upnp.o $(UPNP_LIB)
	$(CC) -o $@ $(APMIB_LIB) $^ $(LDFLAGS) $(LIBS)

endif


clean:
ifdef CONFIG_USE_RSDK_WRAPPER
	rm -rf $(ROOTDIR)/lib/librtk/$(UPNP_LIB_SO)
	rm -rf $(ROOTDIR)/lib/librtk/$(UPNP_LIB)
endif #CONFIG_USE_RSDK_WRAPPER
	rm -f *.o *.so *.a $(UPNP_DAEMON)

install:
#	$(ROMFSINST) /lib/$(UPNP_LIB)
#	cp $(UPNP_LIB_SO) $(FSROOT)/lib/$(UPNP_LIB_SO)
#ifeq ($(CONFIG_WIFI_SIMPLE_CONFIG), y)
#	$(ROMFSINST) /etc/simplecfgservice.xml
#	cp $(UPNP_DAEMON) $(FSROOT)/bin/$(UPNP_DAEMON)
#endif
	cp $(UPNP_DAEMON) $(FSROOT)/usr/sbin/$(UPNP_DAEMON)
	install -m 644 $(UPNP_LIB_SO) $(FSROOT)/lib/$(UPNP_LIB_SO)
# depend stuff
depend: $(SOURCES)
	$(CPP) $(CPPFLAGS) -MM $^ > $(DEPEND)
        
-include $(DEPEND)

# tags
tags:	$(SOURCES)
	ctags -o tags $^ *.h

.c.o:
	${CC} -c -o $@ $(CFLAGS) $(IFLAGS) $<




