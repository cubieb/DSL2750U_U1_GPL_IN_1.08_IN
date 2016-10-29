ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

HOSTARCH := $(shell uname -m | \
	sed -e s/i.86/i386/ \
	    -e s/sun4u/sparc64/ \
	    -e s/arm.*/arm/ \
	    -e s/sa110/arm/ \
	    -e s/powerpc/ppc/ \
	    -e s/macppc/ppc/)

HOSTOS := $(shell uname -s | tr '[:upper:]' '[:lower:]' | \
	    sed -e 's/\(cygwin\).*/cygwin/')

export	HOSTARCH HOSTOS

# Deal with colliding definitions from tcsh etc.
VENDOR=

ifdef BOOT_BUILD_DIR
BUILD_DIR := $(BOOT_BUILD_DIR)/build
else
BUILD_DIR := $(ROOTDIR)/build/boot/build
endif


ifneq ($(BUILD_DIR),)
saved-output := $(BUILD_DIR)

# Attempt to create a output directory.
$(shell [ -d ${BUILD_DIR} ] || mkdir -p ${BUILD_DIR})

# Verify if it was successful.
BUILD_DIR := $(shell cd $(BUILD_DIR) && /bin/pwd)
$(if $(BUILD_DIR),,$(error output directory "$(saved-output)" does not exist))
endif # ifneq ($(BUILD_DIR),)

OBJTREE		:= $(if $(BUILD_DIR),$(BUILD_DIR),$(CURDIR))
SRCTREE		:= $(CURDIR)
TOPDIR		:= $(SRCTREE)
LNDIR		:= $(OBJTREE)
export	TOPDIR SRCTREE OBJTREE

MKCONFIG	:= $(SRCTREE)/mkconfig
export MKCONFIG

ifneq ($(OBJTREE),$(SRCTREE))
REMOTE_BUILD 	:= 1
export REMOTE_BUILD
endif

# $(obj) and (src) are defined in config.mk but here in main Makefile
# we also need them before config.mk is included which is the case for
# some targets like unconfig, clean, clobber, distclean, etc.
ifneq ($(OBJTREE),$(SRCTREE))
obj := $(OBJTREE)/
src := $(SRCTREE)/
else
obj :=
src :=
endif
export obj src

#########################################################################

ifeq ($(OBJTREE)/include/config.mk,$(wildcard $(OBJTREE)/include/config.mk))

# load ARCH, BOARD, and CPU configuration
include $(OBJTREE)/include/config.mk
export	ARCH CPU BOARD VENDOR SOC

LZMA = ${ROOTDIR}/tools/bin/lzma

#########################################################################
ifndef CROSS_COMPILE
CROSS_DIR := /opt/tbs_toolchains

ifeq ($(ARCH),arm)
CROSS_COMPILE =$(CROSS_DIR)/armv5/bin/armv5-linux-uclibc-
endif

ifeq ($(ARCH),mips)
CROSS_COMPILE =$(CROSS_DIR)/mipseb/bin/mipseb-linux-uclibc-

ifeq ($(CPU),vx180)
CROSS_COMPILE =$(CROSS_DIR)/mipseb/bin/mipseb-linux-uclibc-
endif

ifeq ($(CPU),ar2317)
CROSS_COMPILE =$(CROSS_DIR)/mipseb/bin/mipseb-linux-uclibc-
endif

ifeq ($(CPU),amazon_se)
CROSS_COMPILE =$(CROSS_DIR)/mipseb/bin/mipseb-linux-uclibc-
endif

ifeq ($(CPU),rt3052)
CROSS_COMPILE =$(CROSS_DIR)/mipsel/bin/mipsel-linux-uclibc-
endif

ifeq ($(CPU),rtl8196c)
CROSS_COMPILE =$(CROSS_DIR)/realtek/rsdk-1.3.6-4181-EB-2.6.30-0.9.30/bin/mips-linux-
endif

endif
endif

#ifeq ($(CPU),rtl8672)
#CROSS_COMPILE =/opt/tbs_toolchains/realtek/rsdk-1.2.8/linux/uclibc/bin/rsdk-elf-
#endif

#ifeq ($(CPU),rtl8672ve)
#CROSS_COMPILE =/opt/tbs_toolchains/realtek/rsdk-1.2.8/linux/uclibc/bin/rsdk-elf-
#endif

export	CROSS_COMPILE


#########################################################################
#    Define for build solosw image                                       
#########################################################################
ifeq ($(CPU),solosw)
SOLOS_TOOLS =$(CROSS_DIR)/armv5/bin
ELF2AOUT = $(SOLOS_TOOLS)/elf2aout
BIN2AOUT = $(SOLOS_TOOLS)/bin2aout
MKHFBOOT = $(SOLOS_TOOLS)/mkhfboot
export ELF2AOUT BIN2AOUT MKHFBOOT

ISOS_IMG = $(obj)solos.bin
OTHER_OBJ = $(ISOS_IMG)
endif


#########################################################################
#    Define for build infineon amazon-se image                                       
#########################################################################
ifeq ($(CPU),amazon_se)
AMAZON_SE_IMG = $(obj)eeprom_swap.bin
OTHER_OBJ = $(AMAZON_SE_IMG)
endif

#########################################################################
#    Define for build lantiq vrx288 image                      
#########################################################################
ifeq ($(CPU),vrx288)
VRX288_IMG = $(obj)bootloader.sf
OTHER_OBJ = $(VRX288_IMG)
endif

#########################################################################
#    Define for build realtek 8676 image                      
#########################################################################
#ifeq ($(CPU),rtl8676)
#RTL8676_IMG = $(obj)vm.hdr
#OTHER_OBJ = $(RTL8676_IMG)
#endif


# load other configuration
include $(TOPDIR)/config.mk
include $(TOPDIR)/config_bootstart.mk

#########################################################################
# U-Boot stage 1 order is important (i.e. start must be first)
#########################################################################
OBJS_BOOTSTART  = bootstart/cpu/$(CPU)/start.o

OBJS_BOOTSTART := $(addprefix $(obj),$(OBJS_BOOTSTART))

LIBS_BOOTSTART = bootstart/cpu/$(CPU)/lib$(CPU).a
LIBS_BOOTSTART += bootstart/lib_$(ARCH)/lib$(ARCH).a
LIBS_BOOTSTART += bootstart/lib_generic/libgeneric.a
LIBS_BOOTSTART := $(addprefix $(obj),$(LIBS_BOOTSTART))
.PHONY : $(LIBS_BOOTSTART)

__OBJS_BOOTSTART := $(subst $(obj),,$(OBJS_BOOTSTART))
__LIBS_BOOTSTART := $(subst $(obj),,$(LIBS_BOOTSTART))

#########################################################################
# U-Boot stage 2 order is important (i.e. start must be first)
#########################################################################

OBJS  = cpu/$(CPU)/start.o

OBJS := $(addprefix $(obj),$(OBJS))

LIBS  = lib_generic/libgeneric.a
#LIBS += board/$(BOARDDIR)/lib$(BOARD).a
LIBS += cpu/$(CPU)/lib$(CPU).a
ifdef SOC
LIBS += cpu/$(CPU)/$(SOC)/lib$(SOC).a
endif
LIBS += lib_$(ARCH)/lib$(ARCH).a
LIBS += common/libcommon.a
LIBS += net/libnet.a
LIBS += $(BOARDLIBS)

LIBS := $(addprefix $(obj),$(LIBS))
.PHONY : $(LIBS)

# Add GCC lib
PLATFORM_LIBS += -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc

# The "tools" are needed early, so put this first
# Don't include stuff already done in $(LIBS)
#SUBDIRS	= tools \
	  post \
	  post/cpu
.PHONY : $(SUBDIRS)

ifeq ($(CONFIG_NAND_U_BOOT),y)
NAND_SPL = nand_spl
U_BOOT_NAND = $(obj)u-boot-nand.bin
endif

__OBJS := $(subst $(obj),,$(OBJS))
__LIBS := $(subst $(obj),,$(LIBS))

#########################################################################
#########################################################################

ALL = $(obj)u-boot.srec $(obj)u-boot.bin $(obj)System.map  $(U_BOOT_NAND) $(obj)bootloaderstart.bin $(obj)bootloader.bin

all:		$(ALL) $(OTHER_OBJ)
$(obj)bootloader.bin:	$(obj)bootloaderstart.bin $(obj)u-boot.bin $(obj)System.map
	make -C $(SRCTREE)/tools
	${LZMA} e $(obj)u-boot.bin $(obj)u-boot.bin.lzma
	$(TOPDIR)/tools/mkimage -b $(obj)u-boot.bin.lzma -l 0x$(shell grep "T _start" $(obj)System.map | awk '{ printf "%s", $$1 }') -o $(obj)u-boot.bin.lzmaimg
	cat $(obj)bootloaderstart.bin $(obj)u-boot.bin.lzmaimg > $(obj)bootloader.bin
#	cat $(obj)u-boot.bin.lzmaimg >> $(obj)bootloader.bin
	
$(obj)u-boot.hex:	$(obj)u-boot
		$(OBJCOPY) ${OBJCFLAGS} -O ihex $< $@

$(obj)u-boot.srec:	$(obj)u-boot
		$(OBJCOPY) ${OBJCFLAGS} -O srec $< $@

$(obj)u-boot.bin:	$(obj)u-boot
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(obj)u-boot:		depend  $(SUBDIRS) $(OBJS) $(LIBS) $(LDSCRIPT)
	UNDEF_SYM=`$(OBJDUMP) -x $(LIBS) | sed -n -e 's/.*\(__u_boot_cmd_.*\)/-u\1/p'|sort|uniq`; cd $(LNDIR) && $(LD) $(LDFLAGS) $$UNDEF_SYM $(__OBJS) --start-group $(__LIBS) --end-group $(PLATFORM_LIBS) -Map u-boot.map -o u-boot
	@if [ `size $(obj)u-boot | grep u-boot | cut -f 4` -ge 524288 ]; then echo "u-boot bss is too big!" ; exit 1; fi
	$(OBJDUMP)  -D $(obj)u-boot >$(obj)u-boot.s

$(obj)bootloaderstart.bin:	$(obj)bootloaderstart
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@
		
$(obj)bootloaderstart:	depend  $(SUBDIRS) $(OBJS_BOOTSTART) $(LIBS_BOOTSTART) $(LDSCRIPT)
	UNDEF_SYM=`$(OBJDUMP) -x $(LIBS_BOOTSTART) | sed -n -e 's/.*\(__u_boot_cmd_.*\)/-u \1/p'|sort|uniq`; cd $(LNDIR) && $(LD) $(LDFLAGS_BOOTSTART) $$UNDEF_SYM $(__OBJS_BOOTSTART) --start-group $(__LIBS_BOOTSTART) --end-group $(PLATFORM_LIBS) -Map bootloaderstart.map -o bootloaderstart
	$(OBJDUMP)  -D $(obj)bootloaderstart >$(obj)bootloaderstart.s
$(OBJS_BOOTSTART):
		$(MAKE) -C bootstart/cpu/$(CPU) $(if $(REMOTE_BUILD),$@,$(notdir $@))

$(LIBS_BOOTSTART):
		$(MAKE) -C $(dir $(subst $(obj),,$@))

$(obj)bootloaderstartSystem.map:	$(obj)bootloaderstart
		@$(NM) $< | \
		grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | \
		sort > $(obj)bootloaderstartSystem.map
$(OBJS):
		$(MAKE) -C cpu/$(CPU) $(if $(REMOTE_BUILD),$@,$(notdir $@))

$(LIBS):
		$(MAKE) -C $(dir $(subst $(obj),,$@))

$(SUBDIRS):
		$(MAKE) -C $@ all

$(NAND_SPL):	version
		$(MAKE) -C nand_spl/board/$(BOARDDIR) all

$(U_BOOT_NAND):	$(NAND_SPL) $(obj)u-boot.bin
		cat $(obj)nand_spl/u-boot-spl-16k.bin $(obj)u-boot.bin > $(obj)u-boot-nand.bin

depend dep:
		for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir _depend ; done


$(obj)System.map:	$(obj)u-boot
		@$(NM) $< | \
		grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | \
		sort > $(obj)System.map

#########################################################################
else
all $(obj)u-boot.hex $(obj)u-boot.srec $(obj)u-boot.bin \
$(obj)u-boot.img $(obj)u-boot.dis $(obj)u-boot \
$(SUBDIRS) version gdbtools updater env depend \
dep tags ctags etags $(obj)System.map:
	@echo "System not configured - see README" >&2
	@ exit 1
endif

.PHONY : CHANGELOG
CHANGELOG:
	git log --no-merges U-Boot-1_1_5.. | \
	unexpand -a | sed -e 's/\s\s*$$//' > $@
install:
	cp -f $(obj)bootloader.bin ${ROMFS} 
clean:
	find $(OBJTREE) -type f \( -name 'core' -o -name '*.bak' -o -name '*~' \
		-o -name '*.o'  -o -name '*.a'  \) -print | xargs rm -f
	rm -f $(obj)nand_spl/u-boot-spl $(obj)nand_spl/u-boot-spl.map
	rm -f $(obj)u-boot* $(obj)bootloader* ${ROMFS}/bootloader*
#	rm -f $(CROSS_DIR) $(BLP_COMMON_DIR) ${TBS_TOOLS}
	make -C $(SRCTREE)/tools clean
backup:
	F=`basename $(TOPDIR)` ; cd .. ; \
	gtar --force-local -zcvf `date "+$$F-%Y-%m-%d-%T.tar.gz"` $$F

unconfig:
#	make clean
#	@rm -f $(obj)include/config.h $(obj)include/config.mk \
		$(obj)board/*/config.tmp $(obj)board/*/*/config.tmp
	rm -f $(TOPDIR)/tools/config.h
	rm -rf $(BUILD_DIR)

#========================================================================
# MIPS
#========================================================================

#########################################################################
## MIPS32 bcm
#########################################################################
bcm6338_config	\
bcm6348_config	\
bcm6358_config:	unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	@[ -z "$(findstring 6338,$@)" ] || \
		{ echo "#include <chip_bcm63xx.h>" >>$(obj)include/config.h ;\
	echo "#include <configs/bcm6338.h>" >>$(obj)include/config.h ; }
	@[ -z "$(findstring 6348,$@)" ] || \
		{ echo "#include <chip_bcm63xx.h>" >>$(obj)include/config.h ; \
	echo "#include <configs/bcm6348.h>" >>$(obj)include/config.h ; }
	@[ -z "$(findstring 6358,$@)" ] || \
		{ echo "#include <chip_bcm63xx.h>" >>$(obj)include/config.h ; \
	echo "#include <configs/bcm6358.h>" >>$(obj)include/config.h; }
	@$(MKCONFIG) mips bcm63xx  
	
	
#########################################################################
## MIPS32 ar7130
#########################################################################
ar7130_config:	unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	@ >$(TOPDIR)/tools/config.h
#	@[ -z "$(findstring ar7130,$@)" ] || 
	echo "#include <configs/atheros7130.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips ar7130 

#########################################################################
## MIPS32 ar2317
#########################################################################
ar2317_config:  unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
#       @[ -z "$(findstring ar7130,$@)" ] || 
	echo "#include <configs/ar2317.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips ar2317


#########################################################################
## MIPS32 amazon_se
#########################################################################
amazon_se_config:  unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
#       @[ -z "$(findstring ar7130,$@)" ] || 
	echo "#include <configs/amazon_se.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips amazon_se

#########################################################################
## MIPS ikanos vx160
#########################################################################
vx160_config:	unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	echo "#include <configs/vx160.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips vx160

#########################################################################
## MIPS ikanos vx180
#########################################################################
vx180_config:	unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	echo "#include <configs/vx180.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips vx180

#########################################################################
## MIPS32 rt3052
#########################################################################
rt3052_config:	unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	@ >$(TOPDIR)/tools/config.h
#	@[ -z "$(findstring rt3052,$@)" ] || 
	echo "#include <configs/rt3052.h>" >>$(obj)include/config.h ;
	echo "#define __LITTLE_ENDIAN 1234" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips rt3052 

##########################################################################
## MIPS32 RTL8196C
##########################################################################
rtl8196c_config: unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	@ >$(TOPDIR)/tools/config.h
	echo "#include <configs/rtl8196c.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h;
	@$(MKCONFIG) mips rtl8196c
	
	
#========================================================================
# ARM
#========================================================================

#########################################################################
## ARM32 solosw
#########################################################################
solosw_config: unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	echo "#include <configs/solos.h>" >>$(obj)include/config.h ;
	echo "#define __LITTLE_ENDIAN 1234" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) arm solosw

#########################################################################
## ARM1136     Mindspeed C1000
#########################################################################
C1000_config: unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	echo "#include <configs/c1000.h>" >>$(obj)include/config.h ;
	echo "#define __LITTLE_ENDIAN 1234" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) arm c1000



$(ISOS_IMG): $(obj)bootloader.bin
	rm -fr $(obj)bootstart/cpu/solosw/npboot
	mkdir $(obj)bootstart/cpu/solosw/npboot
	mkdir $(obj)bootstart/cpu/solosw/npboot/fs_boot
	cp $(obj)bootloader.bin $(obj)bootstart/cpu/solosw/npboot/fs_boot/orig-boot.bin
	$(MAKE)	-C bootstart/cpu/solosw/npboot/fs_boot
	cp -f $(obj)bootstart/cpu/solosw/npboot/fs_boot/solosw-boot.bin $(obj)bootloader.bin


$(AMAZON_SE_IMG): $(obj)bootloader.bin
	perl $(CURDIR)/bootstart/cpu/amazon_se/padfour.pl $(obj)bootloader.bin $(obj)bootloader_spi.bin
	cp -f $(obj)bootloader_spi.bin $(obj)boot_spi.bin
	rm -f $(obj)bootloader.bin
	rm -f $(obj)eeprom.bin
	rm -f $(obj)eeprom_swap.bin
	cd $(obj); $(ROOTDIR)/tools/bin/infineon/mkeeprom 12 < $(CURDIR)/bootstart/cpu/amazon_se/amse_uboot_16m.conf
	cd $(obj); $(ROOTDIR)/tools/bin/infineon/swap-uboot
	cp -f $(obj)eeprom_swap.bin $(obj)bootloader.bin

$(VRX288_IMG): $(obj)bootloaderstart.bin $(obj)u-boot.bin $(obj)bootloader.bin
	touch $(obj)dummy
	cat $(obj)bootloaderstart.bin > $(obj)bootloader.sf
	dd if=/dev/zero of=$(obj)temppad bs=1024 count=1
	cat $(obj)temppad >> $(obj)bootloader.sf
	cat $(obj)u-boot.bin.lzmaimg >> $(obj)bootloader.sf
	$(LZMA) e $(CURDIR)/cpu/vrx288/gigaflow_cpr_bin $(obj)gigaflow_cpr.lzma
	cat $(obj)gigaflow_cpr.lzma >> $(obj)bootloader.sf
	$(CURDIR)/cpu/vrx288/pad2align.sh -n 1024 $(obj)bootloader.sf
	$(CURDIR)/cpu/vrx288/mk_sf.pl $(obj)dummy $(obj)bootloader.sf 0xbe220000 $(obj)bootloader.sfimg
	rm -rf $(obj)bootloader.bin
	cat $(obj)bootloader.sfimg > $(obj)bootloader.bin
	rm -rf $(obj)dummy $(obj)temppad

$(RTL8676_IMG): $(obj)bootloader.bin
	$(CURDIR)/cpu/rtl8676/genhead -i $(obj)bootloader.bin -o $(obj)vm.hdr -k 0xB0010002 -f $(CONFIG_TEXT_BASE_BOOTSTART) -a $(CONFIG_DRAM_BASE) -e $(CONFIG_DRAM_BASE)
#	cat $(obj)bootloader.bin >> $(obj)vm.hdr
#	mv -f $(obj)vm.hdr $(obj)bootloader.bin
	mv -f $(obj)bootloader.bin $(obj)bootloader.bin.org
	cat $(obj)vm.hdr $(obj)bootloader.bin.org > $(obj)bootloader.bin
	
#########################################################################
## MIPS32 RTL8672 
#########################################################################
rtl8672_config: unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	@ >$(TOPDIR)/tools/config.h
	echo "#include <configs/rtl8672.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips rtl8672 

rtl8672ve_config:       unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	@ >$(TOPDIR)/tools/config.h
	echo "#include <configs/rtl8672ve.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips rtl8672ve 

#########################################################################
## MIPS32 RTL8676 
#########################################################################
rtl8676_config:       unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	@ >$(TOPDIR)/tools/config.h
	echo "#include <configs/rtl8676.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips rtl8676 

#########################################################################
## MIPS32 VRX288 
#########################################################################
vrx288_config:       unconfig
	@mkdir -p $(obj)include
	@ >$(obj)include/config.h
	@ >$(TOPDIR)/tools/config.h
	echo "#include <configs/vrx288.h>" >>$(obj)include/config.h ;
	echo "#define __BIG_ENDIAN 4321" >>$(TOPDIR)/tools/config.h ;
	@$(MKCONFIG) mips vrx288 

                         

