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

prefix	= /usr
datadir	= ${prefix}/share
mandir	= ${prefix}/man

INSTALL	= /usr/bin/install -c

all:
clean:

distclean:
	rm -f Makefile

install:
	$(INSTALL) -d $(datadir)/doc/updatedd
	$(INSTALL) updatedd-*.pdf $(datadir)/doc/updatedd
	$(INSTALL) updatedd-pppd-rc $(datadir)/doc/updatedd

uninstall:
	$(RM) -r $(datadir)/doc/updatedd

.PHONY: all clean distclean install uninstall
