
#romfs:
#	$(ROMFS) -e CONFIG_USER_WIRELESS_TOOLS_AUTH /bin/iwcontrol

all:

install:
	-mkdir -p $(FSROOT)/usr/sbin/
	cp -Lrf iwcontrol $(FSROOT)/usr/sbin/iwcontrol
	chmod 755 $(FSROOT)/usr/sbin/iwcontrol

