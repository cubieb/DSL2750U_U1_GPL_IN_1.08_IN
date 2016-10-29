include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

ROMFSINST=install

EXTCFLAGS=-I$(TOOLCHAINS_SYSINCLUDE)
EXTLDFLAGS=-L$(TOOLCHAINS_SYSROOT)lib
export EXTCFLAGS EXTLDFLAGS

all:
	$(MAKE) -C source

install: all
	$(ROMFSINST) source/bin/nmbd $(FSROOT)/sbin/nmbd
	$(STRIP) $(FSROOT)/sbin/nmbd
	$(ROMFSINST) source/bin/smbd $(FSROOT)/sbin/smbd
	$(STRIP) $(FSROOT)/sbin/smbd
	$(ROMFSINST) source/bin/smbpasswd $(FSROOT)/sbin/smbpasswd	
	$(STRIP) $(FSROOT)/sbin/smbpasswd
	mkdir -p $(FSROOT)/etc/samba
	mkdir -p $(FSROOT)/var/tmp/smbvar
	$(ROMFSINST) packaging/SGI/smbpasswd $(FSROOT)/var/smbpasswd
	cd $(FSROOT)/etc/samba; ln -snf ../../var/smb.conf smb.conf
clean:
	$(MAKE) -C source clean
