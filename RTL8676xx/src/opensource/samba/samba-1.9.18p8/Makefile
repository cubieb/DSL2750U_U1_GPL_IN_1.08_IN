include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

ROMFSINST=install

EXTCFLAGS=-I$(TOOLCHAINS_SYSINCLUDE)
EXTLDFLAGS=-L$(TOOLCHAINS_SYSROOT)lib
export EXTCFLAGS EXTLDFLAGS

all:
	$(MAKE) -C source

install: all
	$(ROMFSINST) source/nmbd $(FSROOT)/sbin/nmbd
	$(STRIP) $(FSROOT)/sbin/nmbd
	$(ROMFSINST) source/smbd $(FSROOT)/sbin/smbd
	$(STRIP) $(FSROOT)/sbin/smbd
#	$(ROMFSINST) source/bin/smbpasswd $(FSROOT)/sbin/smbpasswd
#	$(STRIP) $(FSROOT)/sbin/smbpasswd
#	$(ROMFSINST) samba.sh $(FSROOT)/bin
#	$(ROMFSINST) samba_add_dir.sh $(FSROOT)/bin
#	cd $(FSROOT)/etc; ln -snf ../var/smb.conf smb.conf
	$(ROMFSINST) -D source/smb.conf $(FSROOT)/etc/samba/smb.conf
#	mkdir -p $(FSROOT)/etc/samba/codepages
#	cp -f source/codepages/* $(FSROOT)/etc/samba/codepages/

clean:
	$(MAKE) -C source clean
