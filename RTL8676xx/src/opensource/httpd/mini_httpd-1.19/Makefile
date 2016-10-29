ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
CURDIR = $(shell pwd)

# Makefile for tr069_httpd

# CONFIGURE: If you are using a SystemV-based operating system, such as
# Solaris, you will need to uncomment this definition.
#SYSV_LIBS =    -lnsl -lsocket

# CONFIGURE: Some systems don't need -lcrypt, and indeed they get an
# error if you try to link with it.  If you get an error about libcrypt
# not found, try commenting out this definition.
CRYPT_LIB =    -lcrypt

# CONFIGURE: If you want to compile in support for https, uncomment these
# definitions.  You will need to have already built OpenSSL, available at
# http://www.openssl.org/  Make sure the SSL_TREE definition points to the
# tree with your OpenSSL installation - depending on how you installed it,
# it may be in /usr/local instead of /usr/local/ssl.

#SSL_TREE = ../../../opensource/openssl/openssl-0.9.8f
#SSL_DEFS =	-DUSE_SSL
#SSL_INC =	-I${SSL_TREE}/include
#SSL_LIBS =	-L${SSL_TREE} -lssl -lcrypto -ldl

# If we want to control tr069_httpd status, we can uncomment the following
# definition, it will active to save pid of tr069_httpd when it is running.
# so we can stop it by its pid. also, pid is used to communicate with other
# process.
EOC =	-DCLEAREOC


TARGET = mini_httpd
HTTOOL = htpasswd

#CC =		gcc

CDEFS = ${SSL_DEFS} ${SSL_INC} ${EOC}
EXTRA_CFLAGS = -I${BUILDDIR}/apps/ssap/msg/ -I${BUILDDIR}/apps/include
EXTRA_LDFLAGS = -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz
CFLAGS +=	-Os ${CDEFS} ${EXTRA_CFLAGS}
CFLAGSNOSSL= -Os ${CDEFSNOSSL} -I${BUILDDIR}/apps/tr069fe/cfgmgr_include ${EXTRA_CFLAGS}
#CFLAGS =	-g ${CDEFS}
#LDFLAGS =	-s
#LDFLAGS =	-g
LDLIBS = ${EXTRA_LDFLAGS} ${SSL_LIBS} ${SYSV_LIBS} ${CRYPT_LIB} -ldl
LDLIBSNOSSL = ${EXTRA_LDFLAGS} ${SYSV_LIBS} ${CRYPT_LIB}
CFLAGS += -DTR069
ifdef CONFIG_OPENSOURCE_HTTPD_IPV6
CFLAGS += -DUSE_IPV6
endif

# the following definition is just used to active https request from acs, 
# nornally, if we don't need https response, just comment it.
TBS_MINIHTTPS = y


all:		 $(TARGET) $(htpasswd)

$(TARGET):	mini_httpd.o match.o tdate_parse.o
	if [ "$(TBS_MINIHTTPS)" = "y" ]; then \
	${CC} ${CFLAGS} ${LDFLAGS} mini_httpd.o match.o tdate_parse.o ${LDLIBS} -o $(TARGET); \
	else \
	${CC} ${CFLAGSNOSSL} ${LDFLAGS} mini_httpd.o match.o tdate_parse.o ${LDLIBSNOSSL} -o $(TARGET); \
	fi	

mini_httpd.o:$(ROOTDIR)/.config  mini_httpd.c version.h port.h match.h tdate_parse.h mime_encodings.h mime_types.h mini_httpd.h
	if [ "$(TBS_MINIHTTPS)" = "y" ]; then \
	${CC} ${CFLAGS} -c mini_httpd.c; \
	else \
	${CC} ${CFLAGSNOSSL} -c mini_httpd.c; \
	fi

match.o:$(ROOTDIR)/.config 	match.c match.h
	if [ "$(TBS_MINIHTTPS)" = "y" ]; then \
	${CC} ${CFLAGS} -c match.c; \
	else \
	${CC} ${CFLAGSNOSSL} -c match.c; \
	fi

tdate_parse.o:$(ROOTDIR)/.config 	tdate_parse.c tdate_parse.h
	if [ "$(TBS_MINIHTTPS)" = "y" ]; then \
	${CC} ${CFLAGS} -c tdate_parse.c; \
	else \
	${CC} ${CFLAGSNOSSL} -c tdate_parse.c; \
	fi

mime_encodings.h: mime_encodings.txt
	rm -f mime_encodings.h
	sed < mime_encodings.txt > mime_encodings.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'

mime_types.h: mime_types.txt
	rm -f mime_types.h
	sed < mime_types.txt > mime_types.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'


$(htpasswd):	htpasswd.o
	if [ "$(TBS_MINIHTTPS)" = "y" ]; then \
	${CC} ${CFLAGS} ${LDFLAGS} htpasswd.o ${CRYPT_LIB} -o $(HTTOOL); \
	else \
	${CC} ${CFLAGSNOSSL} ${LDFLAGS} htpasswd.o ${CRYPT_LIB} -o $(HTTOOL); \
	fi

htpasswd.o:$(ROOTDIR)/.config 	htpasswd.c
	if [ "$(TBS_MINIHTTPS)" = "y" ]; then \
	${CC} ${CFLAGS} -c htpasswd.c; \
	else \
	${CC} ${CFLAGSNOSSL} -c htpasswd.c; \
	fi


cert:		mini_httpd.pem
mini_httpd.pem:	mini_httpd.cnf
	openssl req -new -x509 -days 365 -nodes -config mini_httpd.cnf -out mini_httpd.pem -keyout mini_httpd.pem
	openssl x509 -subject -dates -fingerprint -noout -in mini_httpd.pem
	chmod 600 mini_httpd.pem

install:
	cp -f $(TARGET) $(FSROOT)/sbin
	$(STRIP) $(FSROOT)/sbin/$(TARGET)
	#cp -f $(HTTOOL) $(FSROOT)/sbin
	#$(STRIP) $(FSROOT)/sbin/$(HTTOOL)
	cp -f .htdgst $(FSROOT)/usr/www
	cp -f .htpasswd $(FSROOT)/usr/www

clean:
	rm -f $(TARGET) mime_encodings.h mime_types.h $(HTTOOL) mini_httpd.rnd *.o core core.* *.core

