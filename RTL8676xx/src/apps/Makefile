ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/${TBS_KERNEL_VERSION}/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

#scb+ 2011-6-28
ifeq ("$(CONFIG_APPS_LOGIC_NEW3G)","y")
CFLAGS += -DAPPS_LOGIC_NEW3G -DCONFIG_APPS_LOGIC_NEW3G
endif

#scb+2011-10-19 for captival-portal
subdir-${CONFIG_APPS_GENERAL_CAPTIVAL_PORTAL} += captival-portal
ifeq ("$(CONFIG_APPS_GENERAL_CAPTIVAL_PORTAL)","y")
CFLAGS +=  -DAPPS_GENERAL_CAPTIVAL_PORTAL -DTBS
CFLAGS += -DHAVE_DEFINE_TYPE
SYSTEM=TBS
export  SYSTEM
endif

export CFLAGS

subdir-${CONFIG_APPS_SSAP} += ssap
subdir-${CONFIG_APPS_LOGIC} += logic
subdir-${CONFIG_APPS_CLI} += cli
subdir-${CONFIG_APPS_LIB} += omlib
subdir-${CONFIG_APPS_TR069} += tr069fe
subdir-${CONFIG_APPS_TR064} += tr064fe
subdir-${CONFIG_APPS_WEB} += web
subdir-${CONFIG_APPS_HTML} += html
subdir-${CONFIG_APPS_TM} += tm
subdir-${CONFIG_APPS_SUPP} += supp
subdir-${CONFIG_APPS_IPV6} += ipv6
subdir-${CONFIG_APPS_UPGCGI} += upgcgi

include $(ROOTDIR)/Rules.make

installme::
ifdef CONFIG_TBS_GDB_DEBUG
	$(Q)cp -rf $(ROOTDIR)/src/apps $(FSROOT)/usr/apps
	$(Q)find $(FSROOT)/usr/apps -name .svn | xargs rm -rf
endif
