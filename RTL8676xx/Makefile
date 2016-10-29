###########################################################################
#
# get our software configure 
#
#ifeq (.config,$(wildcard .config))

CONFIGED:= $(shell if [ -a .config ]; then echo yes; else echo no; fi)
ifeq (${CONFIGED}, yes)
include .config
all: tools subdirs rootfs romfs
else
all: help
	@exit 1
endif

# config_error
ROOTDIR  = $(shell pwd)
#export KB_DIR=$(ROOTDIR)/build/blp/kernel
include BaseVar.mk
#include Rules.make

DIRS =  build/
-include quicktarget.mk
###########################################################################
help:
	@echo "*************************************************"
	@echo "The build sequence for this source tree is:"
	@echo "1. 'make tools'"
	@echo "2. 'make prepare'"
	@echo "3. 'make'"
	@echo "4. 'make rootfs'"
	@echo "5. 'make romfs'"
	@echo "**************************************************"
	@echo "**************************************************"
	@echo "make prodefconfig : recover product default config"
	@echo "make refresh_dirs : Refresh the symbol links      "	
	@echo "make mrproper     : Clean all build files         "	
	@echo "make switch     	 : Switch product                "	
	@echo "**************************************************"
	@echo "Quick Build Targets:$(QuickTarget) "
	@echo ""
	@echo "Quick Clean Targets:$(QuickCleanTarget) "
	@echo ""
	@echo "Quick DistClean Targets:$(QuickDistCleanTarget) "
	@echo ""
	@echo "Quick Install Targets:$(QuickInstallTarget) "

.PHONY: subdirs clean menuconfig scripts prepare tools promission_check build_lib
subdirs: build $(DIRS) build_lib
	for dir in $(DIRS) ; do [ ! -d $$dir ] || make -C $$dir || exit 1 ; done
install: $(DIRS)
	for dir in $(DIRS) ; do [ ! -d $$dir ] || make -C $$dir install || exit 1 ; done
clean:
	echo $(DIRS)
	for dir in $(DIRS) ; do [ ! -d $$dir ] || make -C $$dir clean || exit 1 ; done

checkautoproduct:
ifndef AUTOPRODUCT 
	@echo autobuild need define PRODUCT,ex:"make autobuild AUTOPRODUCT=UML"
	@exit -1
endif

ckconfig:
	$(Q)cp -f build/product/defconfig build/.config
	$(Q)cd build;mkdir -p include/config;../scripts/conf -s ../Config.in 2>/dev/null
	$(Q)defconfig=`readlink build/product/defconfig`;\
	SVNST=`svn st $$defconfig`;\
	if [ "-$${SVNST}" != "-" ]; then \
	  echo ;echo;echo;\
	  echo "******Warning!!!        ************";\
	  echo "*The defconfig need to commit";\
	  echo "*";\
	  echo $${SVNST}; \
	  echo "*";\
	  echo "************************************";\
	fi

autoprepare:checkautoproduct scripts
	@sh $(ROOTDIR)/tools/mkbuilddir $(AUTOPRODUCT)
	@sh $(ROOTDIR)/tools/prepareBLP.sh
	cd build;mkdir -p include/config;../scripts/conf -s ../Config.in
	$(Q)make scripts_clean
	$(Q)rm -f .config; echo "ok!!!"
	$(Q)ln -s build/.config .config;
	
autobuild:checkautoproduct autoprepare tools
	make subdirs rootfs romfs

##scripts目标在quicktarget.mk定义
menuconfig: scripts
	$(Q)if [ -f .config -a ! -L .config ]; then mv .config build/; fi;
	$(Q)cd build;mkdir -p include/config; ../scripts/mconf ../Config.in;../scripts/conf -s ../Config.in
	$(Q)make scripts_clean
	$(Q)rm -f .config; echo "ok!!!"
	$(Q)ln -s build/.config .config; 

defconfig:
	svn revert ../customer/product/$(PRODUCT)/defconfig;
	$(Q)cp -f build/product/defconfig build/.config;
	make menuconfig

mangle_config:
	$(Q)cp -f build/.config build/product/defconfig;

switch:
	$(Q)./tools/switch.sh	

mrproper:
	$(Q)echo  "Are you sure to remove ALL PRODUCTS files(yes/no)"; 
	$(Q)read item;\
		if [ ! "$${item}" = "yes" ]; then exit -1; fi;
	rm -rf build build.* arch.mk;
	$(Q)rm -f .config
	
distclean:clean
	-for dir in $(DIRS) ; do [ ! -d $$dir ] || make -C $$dir distclean || exit 1 ; done
	
prepare: prepare_dirs menuconfig

prepare_dirs:
	@sh $(ROOTDIR)/tools/mkbuilddir
	@sh $(ROOTDIR)/tools/prepareBLP.sh

prepare_kernel:
	@sh $(ROOTDIR)/tools/prepareBLP.sh

refresh_dirs:
	$(Q)sh ${ROOTDIR}/tools/mksymlink src/ build/

ifeq ($(CONFIG_TBS_ENV_PC),y)		
rootfs:
	make apps_install
	@sh $(ROOTDIR)/build/product/installProfile.sh
else	
rootfs:rootfs_clean install_rootfs install libopt sstrip install_profile
	$(Q)cd $(FSROOT);tar cf etc/var.tar var
	-find $(FSROOT) -name .svn | xargs rm -rf
	-ln -sf ../../../../var/openusb ${FSROOT}/usr/www/html/gui/openusb
endif

install_rootfs:
	@sh $(ROOTDIR)/tools/buildRootfs.sh
	@echo "install_rootfs ok!"

libopt:
	-$(Q)if [ ! -d ${BUILDDIR}/templib ]; then mkdir -p ${BUILDDIR}/templib; fi;
	-$(Q)python $(ROOTDIR)/tools/mklibs.py -D -L $(FSROOT)/lib/ -L $(TOOLCHAINS_SYSROOT)/lib/ --target=$(TOOLCHAINS_PATH)/$(CROSS_COMPILE) -d $(BUILDDIR)/templib `find $(FSROOT) -path $(FSROOT)/lib -prune -o -type f -print | file -f - | grep ELF | cut -d':' -f1`
	-$(Q)cp ${BUILDDIR}/templib/* ${FSROOT}/lib/

sstrip:
	@echo "Stripping userapps and shared libraries more..."
	./tools/bin/sstrip `find $(FSROOT) -path $(FSROOT)/lib/modules -prune -o -type f -print | file -f - | grep ELF | cut -d':' -f1`
	@echo "Install_rootfs ok!"

install_profile:
	$(Q) if [ -f $(ROOTDIR)/build/product/installProfile.sh ]; then sh $(ROOTDIR)/build/product/installProfile.sh;fi;
	@echo "install profile file ok!"

stub_opensource:
	make -C src/stub_opensource
romfs:  
	cp releasenotes.html build/romfs/rootfs/usr/www/
	cp hidden_page.html build/romfs/rootfs/usr/www/
	chmod 777 ${ROOTDIR}/getverinfo.sh
	chmod 777 ${ROOTDIR}/hidenPage.sh
	@./getverinfo.sh 
	rm -rf $(FSROOT)/var/*
	@sh $(ROOTDIR)/tools/buildImage.sh


promission_check:
	@echo -n "Check set-user-ID permisson bit of chmod: "
	@if [ `whoami` == "root" -o -u `which chmod` ]; then echo "OK"; else echo "Failed"; exit 1; fi
	@echo -n "Check set-user-ID permisson bit of mknod: "
	@if [ `whoami` == "root" -o -u `which mknod` ]; then echo "OK"; else echo "Failed"; exit 1; fi

tools: promission_check
	$(Q)make -C tools/src install

tools_clean:
	$(Q)make -C tools/src clean 

rootfs_clean:
	rm ${ROOTDIR}/build/romfs/rootfs/* -fr

build_lib:
#	make openssl
		
info:
	echo ${ROOTDIR}
