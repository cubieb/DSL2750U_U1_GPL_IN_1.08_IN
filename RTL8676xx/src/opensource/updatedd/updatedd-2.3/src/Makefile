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

bin		= updatedd

prefix		= /usr
exec_prefix	= ${prefix}
bindir		= ${exec_prefix}/bin
libdir		= ${exec_prefix}/lib
mandir		= ${prefix}/man

INSTALL		= /usr/bin/install -c
CC		= gcc
CFLAGS		= -I../include -I $(TI_uclibc_include) -Wall -Wwrite-strings -O2  -DLIBPATH=\"$(libdir)/updatedd\"
#CFLAGS		= -I../include -I $(TI_uclibc_include) -Wall -Wwrite-strings -O2  -DLIBPATH=\"/var/updatedd\"

LIBS		= -liresolv -lresolv -ldl 
LIBERROR	= ../libexception_handle/libexception_handle.a

OBJECTS		= updatedd.o

all: $(OBJECTS)
	$(CC) $(LIBS) $(OBJECTS) $(LIBERROR) -o $(bin)
	@echo ${CFLAGS}
updatedd.o: updatedd.h

install:
	$(INSTALL) -m 755 $(bin) $(bindir)

	$(INSTALL) -d $(mandir)/man1
	$(INSTALL) -m 644 updatedd.1 $(mandir)/man1

uninstall:
	$(RM) $(bindir)/$(bin)
	$(RM) $(mandir)/man1/updatedd.1

clean:
	$(RM) *.o
	$(RM) $(bin)

distclean: clean
	$(RM) Makefile

.PHONY: all install uninstall clean distclean
