# -- updatedd: Makefile.in --
#
# Copyright (C) 2002, 2003, 2004 Philipp Benner
#
# This file is part of UpdateDD - http://updatedd.philipp-benner.de.
#
# UpdateDD is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# any later version.
#
# UpdateDD is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with UpdateDD; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

prefix		= /usr
exec_prefix	= ${prefix}
sbindir		= ${exec_prefix}/sbin
libdir		= ${exec_prefix}/lib

INSTALL		= /usr/bin/install -c
CC		= gcc
CFLAGS		= -I../../include -I $(TI_uclibc_include) -Wall -Wwrite-strings -O2  -fPIC -g
LIBS		= -liresolv -lresolv -ldl 
LIBERROR	= ../../libexception_handle/libexception_handle.a

UPDATEDDLIBS	= -liresolv -lresolv -ldl 
OBJECTS		= $(patsubst %.c,%.o,$(wildcard *.c)) 
PLUGINS         = $(patsubst %.c,%,$(wildcard *.c))

all: plugins

plugins: $(OBJECTS) $(PLUGINS:%=%)
%: %.o
	$(CC) $(LIBS) -Wl,-soname,$*.so -shared $< \
		$(LIBERROR)  -o $*.so

install:	
	$(INSTALL) -d $(libdir)/updatedd
	$(INSTALL) -m 755 *.so $(libdir)/updatedd

uninstall:
	$(RM) -r $(libdir)/updatedd

clean:
	$(RM) *.o
	$(RM) *.so

distclean: clean
	$(RM) Makefile

#changeip.o:	changeip.h
dyndns.o:	dyndns.c dyndns.h 
gnudip.o:       gnudip.c gnudip.h
#eurodyndns.o:	eurodyndns.h
#hn.o:		hn.h
#noip.o:	noip.h
#ods.o:		ods.h
#ovh.o:		ovh.h
#regfish.o:	regfish.h
#tzo.o:		tzo.h
