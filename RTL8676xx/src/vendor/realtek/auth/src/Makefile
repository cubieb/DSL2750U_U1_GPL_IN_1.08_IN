#
# Makefile for WPA for the Linux OS
#
#
SUBDIRS= dlisten
all:

install:
	-mkdir -p $(FSROOT)/usr/bin/
	cp -Lrf auth $(FSROOT)/usr/bin/auth
	for x in $(SUBDIRS); do make -C $$x $@; done

clean:


