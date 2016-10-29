CONFIG_PATH=$(FSROOT)/etc/init.d/
CONFIG_DIR=$(FSROOT)/etc/ath/

all:

clean:
install:
	install -m 755 -d $(CONFIG_DIR)
	cp -f apcfg makeVAP killVAP $(CONFIG_DIR) 
	cp -f insert_modules rc.wlan  $(CONFIG_PATH) 
uninstall:
