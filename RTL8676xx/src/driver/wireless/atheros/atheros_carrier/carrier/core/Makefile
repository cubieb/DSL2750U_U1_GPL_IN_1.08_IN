# Do not change the directory order
dir-y=hal/ ath_hal/ ath/ net80211/

all clean install uninstall:
	 @set -e ; for d in $(dir-y) ; do $(MAKE) -C $$d $@; done

