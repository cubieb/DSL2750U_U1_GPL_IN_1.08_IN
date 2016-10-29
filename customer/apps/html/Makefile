ifeq "$(ROOTDIR)" ""
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all install distclean clean
HTML=standard

#--- languages ---#
lang-y := #
lang- := #
lang-${CONFIG_APPS_HTML_LANG_SUPPORT_en_us} += en_us
lang-${CONFIG_APPS_HTML_LANG_SUPPORT_zh_cn} += zh_cn
lang-${CONFIG_APPS_HTML_LANG_SUPPORT_es_es} += es_es
#--- style ---#
ifeq "$(CONFIG_APPS_HTML_WEB_STYLE_STANDARD)" "y"
skin := standard
endif
ifeq "$(CONFIG_APPS_HTML_WEB_STYLE_DSL2740EL)" "y"
skin := DSL2740EL
endif
ifeq "$(CONFIG_APPS_HTML_WEB_STYLE_DSL2750U)" "y"
skin := DSL2750U
endif
ifeq "$(CONFIG_APPS_HTML_WEB_STYLE_DSL2730U)" "y"
skin := DSL2730U
endif
ifeq "$(CONFIG_APPS_HTML_WEB_STYLE_DSL2730U_ETISALAT)" "y"
skin := DSL2730U_Etisalat
endif
ifeq "$(CONFIG_APPS_HTML_WEB_STYLE_DSL2750U_ETISALAT)" "y"
skin := DSL2750U_Etisalat
endif
ifeq "$(CONFIG_APPS_HTML_WEB_STYLE_DSL2730E)" "y"
skin := DSL2730E
endif
ifeq "$(CONFIG_APPS_HTML_WEB_STYLE_DSL2750E)" "y"
skin := DSL2750E
endif
ifeq "$(CONFIG_APPS_HTML_WEB_STYLE_DSL2750E_TOT)" "y"
skin := DSL2750E_TOT
endif
#HTML pages .html
page-y := #
page- := #
page-${CONFIG_APPS_LOGIC_LAN} += setup/vlan_lan.html setup/lan_advanced.html \
	setup/lan.html setup/bind_vlan.html setup/multiIPSetting.html setup/dhcp_IpPoll.html
page-${CONFIG_APPS_LOGIC_PPPOE} +=
page-${CONFIG_APPS_LOGIC_DNS} +=
page-${CONFIG_APPS_LOGIC_ETHLAN} +=
page-${CONFIG_APPS_LOGIC_ETHWAN} +=
page-${CONFIG_APPS_LOGIC_PING} +=
page-${CONFIG_APPS_LOGIC_WAN} +=
page-${CONFIG_APPS_LOGIC_WAN_VLAN} +=
page-${CONFIG_APPS_LOGIC_IPCONN} +=
page-${CONFIG_APPS_LOGIC_TR069} +=
page-${CONFIG_APPS_LOGIC_TR111} +=
page-${CONFIG_APPS_LOGIC_IPT} +=
page-${CONFIG_APPS_LOGIC_PORTMAPPING} +=
page-${CONFIG_APPS_LOGIC_ACL} +=
page-${CONFIG_APPS_LOGIC_LOGGER} +=
page-${CONFIG_APPS_LOGIC_DHCPS} +=
page-${CONFIG_APPS_LOGIC_DHCPR} +=
page-${CONFIG_APPS_LOGIC_ALG} +=
page-${CONFIG_APPS_LOGIC_BRIDGE} +=
page-${CONFIG_APPS_LOGIC_IGMP} +=
page-${CONFIG_APPS_LOGIC_TF_IGMP} +=
page-${CONFIG_APPS_LOGIC_SNMPC} +=
page-${CONFIG_APPS_LOGIC_QOS} +=
page-${CONFIG_APPS_LOGIC_STATIC_ROUTING} +=
page-${CONFIG_APPS_LOGIC_DYNAMIC_ROUTING} +=
page-${CONFIG_APPS_LOGIC_AUTH} +=
page-${CONFIG_APPS_LOGIC_FIREWALL} +=
page-${CONFIG_APPS_LOGIC_ANTIATTACK} +=
page-${CONFIG_APPS_LOGIC_URL_FILTER} +=
page-${CONFIG_APPS_LOGIC_MACFILTER} +=
page-${CONFIG_APPS_LOGIC_VLAN} +=
page-${CONFIG_APPS_LOGIC_SNTP} +=
page-${CONFIG_APPS_LOGIC_USB_MASS} +=
page-${CONFIG_APPS_LOGIC_FTPD} +=
page-${CONFIG_APPS_LOGIC_NATPRIO} +=
page-${CONFIG_APPS_LOGIC_LOGGER} +=
page-${CONFIG_APPS_LOGIC_WLAN} +=
page-${CONFIG_APPS_LOGIC__WPS} +=
page-${CONFIG_APPS_LOGIC_UPNP} +=
page-${CONFIG_APPS_LOGIC_IPPMNGR} +=
page-${CONFIG_APPS_LOGIC_DHCPS_FILTER} +=
page-${CONFIG_APPS_LOGIC_TW_SAMBA} += samba.html
page-${CONFIG_APPS_LOGIC_TF_SAMBA} += tf_samba.html

#scb+ 2011-6-28 for new 3g
ifeq ("$(CONFIG_APPS_LOGIC_NEW3G)", "y")
page-${CONFIG_APPS_LOGIC_NEW3G} += wan3gcfg.html
page-${CONFIG_APPS_LOGIC_NEW3G} += wan3gview.html
page-${CONFIG_APPS_LOGIC_NEW3G} += wan3gpin.html
page-${CONFIG_APPS_LOGIC_NEW3G} += wan3gpinresult.html

#scb+ 2011-10-27
ifeq ("$(CONFIG_APPS_GENERAL_CAPTIVAL_PORTAL)", "y")
page-${CONFIG_APPS_GENERAL_CAPTIVAL_PORTAL} += wan3g_connect_status.html
endif

endif


### add corresponding error messages according to the MACRO definition
ifeq ("$(CONFIG_APPS_LOGIC_WLAN_QOS)", "y")
	EXTRA_CFLAGS += -DWLAN_QOS_SUPPORT
endif

all:
distclean:
clean:
install:
	$(Q)rm -rf $(FSROOT)/usr/www/html
	$(Q)mkdir -p  $(FSROOT)/usr/www/html
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/$(HTML)/* $(FSROOT)/usr/www/html
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/*.html $(FSROOT)/usr/www/
	$(Q)cd $(FSROOT)/usr/www/html/test;ln -s  ../../../../var/factory_mode.js factory_mode.js
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/* $(FSROOT)/usr/www/html/
	if [ -e $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/config/config.xml ]; then cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/config/config.xml $(FSROOT)/etc/config.xml; fi
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/webAuth.xml $(FSROOT)/usr/www/
ifeq ("$(CONFIG_APPS_LOGIC_TF_GUI)", "y")
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/gui $(FSROOT)/usr/www/html/gui
endif
ifeq ("$(CONFIG_APPS_OTHERPORT)", "y")
ifeq ("$(CONFIG_TELEFONICA_BRAZIL)", "y")
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/brazilport/index.html $(FSROOT)/usr/www/
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/vivospeedy $(FSROOT)/usr/www/
else
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/standport/index.html $(FSROOT)/usr/www/
endif
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/otherport/restarting.html $(FSROOT)/usr/www/html/page/
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/otherport/upgrading.html $(FSROOT)/usr/www/html/page/
else
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/standport/index.html $(FSROOT)/usr/www/
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/standport/restarting.html $(FSROOT)/usr/www/html/page/
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/standport/upgrading.html $(FSROOT)/usr/www/html/page/
endif
ifeq ("$(CONFIG_TELEFONICA_BRAZIL)", "y")
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/standport/index.html $(FSROOT)/usr/www/
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/padrao $(FSROOT)/usr/www/
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/vivospeedy $(FSROOT)/usr/www/
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/webAuth.xml $(FSROOT)/usr/www/
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/tr69cfg.html $(FSROOT)/usr/www/
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/tr69cfg.js $(FSROOT)/usr/www/
	$(Q)rm -rf $(FSROOT)/usr/www/html/webAuth.xml
	$(Q)rm -rf $(FSROOT)/usr/www/html/tr69cfg.html
	$(Q)rm -rf $(FSROOT)/usr/www/html/tr69cfg.js
	$(Q)rm -rf $(FSROOT)/usr/www/html/page/tr069.html
	$(Q)rm -rf $(FSROOT)/usr/www/html/page/tr069.js
ifeq ("$(CONFIG_APPS_LOGIC_TF_GUI)", "y")
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/gui $(FSROOT)/usr/www/html/gui
endif
endif
ifneq ("$(CONFIG_TELEFONICA_BRAZIL)", "y")
	$(Q)rm -rf $(FSROOT)/usr/www/html/wizard
endif
ifeq ("$(CONFIG_TELEFONICA_COLOMBIA_OBSERVA)", "y")
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/webAuth.xml $(FSROOT)/usr/www/
	$(Q)rm -rf $(FSROOT)/usr/www/html/webAuth.xml
endif
ifeq ("$(CONFIG_TELEFONICA_CHILE)", "y")
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/webAuth.xml $(FSROOT)/usr/www/
	$(Q)rm -rf $(FSROOT)/usr/www/html/webAuth.xml
endif
ifeq ("$(CONFIG_APPS_HTML_WEB_STYLE_DSL2740EL)", "y")
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/restarting.html $(FSROOT)/usr/www/html/page
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/upgrading.html $(FSROOT)/usr/www/html/page
endif
ifeq ("$(CONFIG_APPS_HTML_WEB_STYLE_DSL2730U)", "y")
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/restarting.html $(FSROOT)/usr/www/html/page
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/upgrading.html $(FSROOT)/usr/www/html/page
endif
ifeq ("$(CONFIG_APPS_HTML_WEB_STYLE_DSL2730E)", "y")
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/restarting.html $(FSROOT)/usr/www/html/page
	$(Q)cp -rf $(ROOTDIR)/../${CUSTOMER}/apps/html/skins/$(skin)/upgrading.html $(FSROOT)/usr/www/html/page
endif
	$(Q)-find $(FSROOT)/usr/www/ -name .svn | xargs rm -rf
ifneq ("$(CONFIG_APPS_LOGIC_QrenCode)", "y")
	$(Q)rm -rf $(FSROOT)/usr/www/html/picture/ssid.png
endif
	$(Q)gcc -o html ./html.c $(ROOTDIR)/build/apps/ssap/lib/tbserror.c -Wall -g \
	           -I$(ROOTDIR)/build/apps/include -I$(ROOTDIR)/build/ ${EXTRA_CFLAGS}
	$(Q)./html
	$(Q)rm -f ./html ./html.o $(ROOTDIR)/build/apps/ssap/lib/tbserror.o
	$(Q)cp ./updated_en_us.js -f $(FSROOT)/usr/www/html/languages/en_us/error.js
	$(Q)cp ./updated_zh_cn.js -f $(FSROOT)/usr/www/html/languages/zh_cn/error.js
	$(Q)cp ./updated_es_es.js -f $(FSROOT)/usr/www/html/languages/es_es/error.js
	$(Q)rm -f *.js

	$(Q)echo "****************************************************************"
	$(Q)echo "[Need to delete language directory]: $(lang-)"
	$(Q)echo "[Need to delete HTML pages]: $(page-)"
	$(Q)echo "****************************************************************"
	$(Q)echo "Delete unneeded languages files..."
	$(Q)cd $(FSROOT)/usr/www/html/languages;rm -rf $(lang-)
	$(Q)echo 'Delete unneeded html pages...'
	$(Q)cd $(FSROOT)/usr/www/html/page;rm -rf $(page-:.html=.js) $(page-)
	@for dir in $(lang-y); do  \
		cd $(FSROOT)/usr/www/html/languages/$${dir};\
		rm -rf $(page-:.html=.js);\
	done;
	echo "">>$(FSROOT)/usr/www/html/js/global.js
	echo "var G_Date='`export LANG=C;date +%m/%d/%Y-%H:%M:%S`';">>$(FSROOT)/usr/www/html/js/global.js
	
