include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean

SRCDIR=$(CURDIR)/samba-3.027

all: build

build:
	make -C $(SRCDIR) all

clean:
	make -C $(SRCDIR) clean

install:
	cp $(ROOTDIR)/src/opensource/samba/smb.conf $(ROOTDIR)/src/filesystem/basefilesystem/root/var/smb.conf
	make -C $(SRCDIR) install
