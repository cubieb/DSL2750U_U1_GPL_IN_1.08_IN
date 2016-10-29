include $(ROOTDIR)/.config

#ifeq "$(CONFIG_OPENSOURCE_LP_FW_HP1020)" "y"
FIRMWARE-$(CONFIG_OPENSOURCE_LP_FW_HP1020) += hp1020.dl
FIRMWARE-$(CONFIG_OPENSOURCE_LP_FW_HPP1008) += hpP1008.dl

all: install

install:
	-test -z "$(FIRMWARE-y)" || \
	mkdir -p $(FSROOT)/etc/hotplug/firmware/; \
	cp -f $(FIRMWARE-y) $(FSROOT)/etc/hotplug/firmware/
