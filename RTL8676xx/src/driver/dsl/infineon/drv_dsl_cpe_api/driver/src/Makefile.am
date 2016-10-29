## Process this file with automake to produce Makefile.in

# Target definition depending on kernel version:
if KERNEL_2_6
bin_PROGRAMS = drv_dsl_cpe_api.ko
else
bin_PROGRAMS = drv_dsl_cpe_api
noinst_LIBRARIES = lib_dsl_cpe_api.a
endif


if INCLUDE_DSL_CPE_API_DANUBE
bin_SCRIPTS = ../scripts/inst_drv_dsl_cpe_api_danube.sh
endif
if INCLUDE_DSL_CPE_API_AMAZON_SE
bin_SCRIPTS = ../scripts/inst_drv_dsl_cpe_api_amazon_se.sh
endif
if INCLUDE_DSL_CPE_API_VINAX
bin_SCRIPTS = ../scripts/inst_drv_dsl_cpe_api_vinax.sh
endif

## sources for PM module
drv_pm_sources = \
	pm/drv_dsl_cpe_api_pm.c \
	pm/drv_dsl_cpe_pm_core.c

## extra for PM
drv_pm_extra = \
	include/drv_dsl_cpe_api_pm.h \
	include/drv_dsl_cpe_intern_pm.h \
	include/drv_dsl_cpe_device_pm.h \
	include/drv_dsl_cpe_pm_core.h

## sources for CEOC module
drv_ceoc_sources = \
	ceoc/drv_dsl_cpe_ceoc.c \
	ceoc/drv_dsl_cpe_intern_ceoc.c

## extra for CEOC
drv_ceoc_extra = \
	include/drv_dsl_cpe_api_ceoc_intern.h \
	include/drv_dsl_cpe_api_ceoc.h \
	include/drv_dsl_cpe_intern_ceoc.h \
	include/drv_dsl_cpe_device_ceoc.h

## sources for ADSL MIB module
drv_adsl_mib_sources = \
	mib/drv_dsl_cpe_adslmib.c

## extra for ADSL MIB
drv_adsl_mib_extra= \
	include/drv_dsl_cpe_api_adslmib.h \
	include/drv_dsl_cpe_api_adslmib_ioctl.h

## common driver sources always needed for build
drv_common_sources = \
	common/drv_dsl_cpe_api.c \
	common/drv_dsl_cpe_fifo.c \
	common/drv_dsl_cpe_autoboot.c \
	common/drv_dsl_cpe_debug.c \
	common/drv_dsl_cpe_os_linux.c \
	common/drv_dsl_cpe_timeout.c \
	g997/drv_dsl_cpe_api_g997.c \
	led/drv_dsl_cpe_led.c

## common driver sources always included additionally in the distribution
drv_common_extra = \
	include/drv_dsl_cpe_api.h \
	include/drv_dsl_cpe_fifo.h \
	include/drv_dsl_cpe_api_error.h \
	include/drv_dsl_cpe_api_g997.h \
	include/drv_dsl_cpe_api_ioctl.h \
	include/drv_dsl_cpe_api_sar.h \
	include/drv_dsl_cpe_api_types.h \
	include/drv_dsl_cpe_autoboot.h \
	include/drv_dsl_cpe_debug.h \
	include/drv_dsl_cpe_device.h \
	include/drv_dsl_cpe_timeout.h \
	include/drv_dsl_cpe_intern.h \
	include/drv_dsl_cpe_intern_g997.h \
	include/drv_dsl_cpe_device_g997.h \
	include/drv_dsl_cpe_intern_led.h \
	include/drv_dsl_cpe_intern_mib.h \
	include/drv_dsl_cpe_intern_sar.h \
	include/drv_dsl_cpe_os.h \
	include/drv_dsl_cpe_os_linux.h \
	include/drv_dsl_cpe_api_config.h.in

## Files specific for the Danube
## driver sources
drv_danube_specific_sources = \
	device/drv_dsl_cpe_device_danube.c \
	g997/drv_dsl_cpe_api_g997_danube.c

if INCLUDE_DSL_PM
drv_pm_danube_specific_sources = \
	pm/drv_dsl_cpe_api_pm_danube.c
endif

if INCLUDE_DSL_CEOC
drv_ceoc_danube_specific_sources = \
	ceoc/drv_dsl_cpe_danube_ceoc.c
endif

## extra distribution for Danube
drv_danube_specific_extra = \
	include/drv_dsl_cpe_device_danube.h \
	include/drv_dsl_cpe_danube.h \
	include/drv_dsl_cpe_cmv_danube.h \
	include/drv_dsl_cpe_danube_ctx.h \
	include/drv_dsl_cpe_api_danube_kernel_config.h \
	../scripts/inst_drv_dsl_cpe_api_danube.sh


## Files specific for the Amazon-SE
## driver sources
drv_amazon_se_specific_sources = \
	device/drv_dsl_cpe_device_danube.c \
	g997/drv_dsl_cpe_api_g997_danube.c

if INCLUDE_DSL_PM
drv_pm_amazon_se_specific_sources = \
	pm/drv_dsl_cpe_api_pm_danube.c
endif

if INCLUDE_DSL_CEOC
drv_ceoc_amazon_se_specific_sources = \
	ceoc/drv_dsl_cpe_danube_ceoc.c
endif

## extra distribution for Amazon-SE
drv_amazon_se_specific_extra = \
	include/drv_dsl_cpe_device_danube.h \
	include/drv_dsl_cpe_danube.h \
	include/drv_dsl_cpe_cmv_danube.h \
	include/drv_dsl_cpe_danube_ctx.h \
	include/drv_dsl_cpe_api_amazon_se_kernel_config.h \
	../scripts/inst_drv_dsl_cpe_api_amazon_se.sh


## Files specific for the Vinax
## driver sources
drv_vinax_specific_sources = \
	device/drv_dsl_cpe_device_vinax.c \
	g997/drv_dsl_cpe_api_g997_vinax.c \
	sar/drv_dsl_cpe_sar.c

if INCLUDE_DSL_PM
drv_pm_vinax_specific_sources = \
	pm/drv_dsl_cpe_api_pm_vinax.c
endif

if INCLUDE_DSL_CEOC
drv_ceoc_vinax_specific_sources = \
	ceoc/drv_dsl_cpe_vinax_ceoc.c
endif

## extra distribution for Vinax
drv_vinax_specific_extra = \
	include/drv_dsl_cpe_device_vinax.h \
	include/drv_dsl_cpe_vinax_msg.h \
	include/drv_dsl_cpe_vinax_msg_const.h \
	include/drv_dsl_cpe_vinax_msg_tc_counters.h \
	include/drv_dsl_cpe_vinax_msg_utopia_posphy.h \
	include/drv_dsl_cpe_vinax_msg_dmt_scope.h \
	include/drv_dsl_cpe_vinax_msg_xtc.h \
	include/drv_dsl_cpe_vinax_ctx.h \
	include/drv_dsl_cpe_vinax_mdio.h \
	../scripts/inst_drv_dsl_cpe_api_vinax.sh


if INCLUDE_DSL_PM
drv_pm_specific = $(drv_pm_sources)
endif

if INCLUDE_DSL_CEOC
drv_ceoc_specific = $(drv_ceoc_sources)
endif

if INCLUDE_DSL_ADSL_MIB
drv_mib_specific = $(drv_adsl_mib_sources)
endif

## all sources for the current driver build
drv_dsl_cpe_api_SOURCES = \
	$(drv_common_sources) \
	$(drv_pm_specific) \
	$(drv_ceoc_specific) \
	$(drv_mib_specific) \
	$(drv_specific_sources)

if INCLUDE_DSL_CPE_API_DANUBE
drv_dsl_cpe_api_SOURCES += \
	$(drv_danube_specific_sources) \
	$(drv_pm_danube_specific_sources) \
	$(drv_ceoc_danube_specific_sources)
endif

if INCLUDE_DSL_CPE_API_AMAZON_SE
drv_dsl_cpe_api_SOURCES += \
	$(drv_amazon_se_specific_sources) \
	$(drv_pm_amazon_se_specific_sources) \
	$(drv_ceoc_amazon_se_specific_sources)
endif

if INCLUDE_DSL_CPE_API_VINAX
drv_dsl_cpe_api_SOURCES += \
	$(drv_vinax_specific_sources) \
	$(drv_pm_vinax_specific_sources) \
	$(drv_ceoc_vinax_specific_sources)
endif

lib_dsl_cpe_api_a_SOURCES = \
	$(drv_common_sources) \
	$(drv_pm_specific) \
	$(drv_ceoc_specific) \
	$(drv_mib_specific) \
	$(drv_specific_sources)

if INCLUDE_DSL_CPE_API_DANUBE
lib_dsl_cpe_api_a_SOURCES += \
	$(drv_danube_specific_sources) \
	$(drv_pm_danube_specific_sources) \
	$(drv_ceoc_danube_specific_sources)
endif

if INCLUDE_DSL_CPE_API_AMAZON_SE
lib_dsl_cpe_api_a_SOURCES += \
	$(drv_amazon_se_specific_sources) \
	$(drv_pm_amazon_se_specific_sources) \
	$(drv_ceoc_amazon_se_specific_sources)
endif

if INCLUDE_DSL_CPE_API_VINAX
lib_dsl_cpe_api_a_SOURCES += \
	$(drv_vinax_specific_sources) \
	$(drv_pm_vinax_specific_sources) \
	$(drv_ceoc_vinax_specific_sources)
endif
## all extra sources for the current library distribution
EXTRA_DIST = \
	../LICENSE \
	$(drv_danube_specific_sources) \
	$(drv_pm_danube_specific_sources) \
	$(drv_ceoc_danube_specific_sources) \
	$(drv_amazon_se_specific_sources) \
	$(drv_pm_amazon_se_specific_sources) \
	$(drv_ceoc_amazon_se_specific_sources) \
	$(drv_vinax_specific_sources) \
	$(drv_pm_vinax_specific_sources) \
	$(drv_ceoc_vinax_specific_sources) \
	$(drv_common_extra) \
	$(drv_pm_extra) \
	$(drv_ceoc_extra) \
	$(drv_adsl_mib_extra) \
	$(drv_danube_specific_extra) \
	$(drv_amazon_se_specific_extra) \
	$(drv_vinax_specific_extra)


# common driver cflags
drv_dsl_cpe_api_common_cflags = -DLINUX -D__LINUX__ -D__KERNEL__ -DEXPORT_SYMTAB \
    -pipe -Wall -Wformat -Wimplicit -Wunused -Wswitch -Wcomment -Winline \
    -Wuninitialized -Wparentheses -Wsign-compare -Wreturn-type \
    -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common \
    -fomit-frame-pointer

# target cflags, specified by configure options
drv_dsl_cpe_api_target_cflags= $(ADD_DRV_CFLAGS)

# compile cflags
drv_dsl_cpe_api_compile_flags =

if !KERNEL_2_6
# the headerfile of linux kernels 2.6.x contain to much arithmetic
# with void pointers (which is allowed for gcc!)
drv_dsl_cpe_api_common_cflags += -Wpointer-arith
endif

drv_dsl_cpe_api_CFLAGS =  $(drv_dsl_cpe_api_common_cflags) \
                          $(drv_dsl_cpe_api_target_cflags) \
                          $(drv_dsl_cpe_api_compile_flags) \
                          $(drv_dsl_cpe_api_device_flags)

lib_dsl_cpe_api_a_CFLAGS = $(drv_dsl_cpe_api_common_cflags) \
                           $(drv_dsl_cpe_api_target_cflags) \
                           $(drv_dsl_cpe_api_compile_flags) \
                           $(drv_dsl_cpe_api_device_flags)

AM_CPPFLAGS= \
    -I@srcdir@ -I@srcdir@/include \
    $(DSL_DRIVER_INCL_PATH) \
    -I@KERNEL_BUILD_PATH@/include \
    -I@KERNEL_INCL_PATH@


drv_dsl_cpe_api_LDFLAGS = -r -nostdlib

# linux 2.6 kernel object
drv_dsl_cpe_api_ko_SOURCES = ../README

if KERNEL_2_6

drv_dsl_cpe_api_OBJS = "$(subst .c,.o,$(filter %.c,$(drv_dsl_cpe_api_SOURCES)))"

drv_dsl_cpe_api.ko: $(drv_dsl_cpe_api_SOURCES)
	@echo -e "drv_dsl_cpe_api: Making Linux 2.6.x kernel object"
	if test ! -e common/drv_dsl_cpe_api.c ; then \
		echo "copy source files (as links only!)"; \
		for f in $(filter %.c,$(drv_dsl_cpe_api_SOURCES)); do \
			mkdir -p $(PWD)/`dirname $$f`/ ; \
			cp -s $(addprefix @abs_srcdir@/,$$f) $(PWD)/`dirname $$f`/ ; \
		done \
	fi
	@echo -e "# drv_dsl_cpe_api: Generated to build Linux 2.6.x kernel object" > $(PWD)/Kbuild
	@echo -e "obj-m := $(subst .ko,.o,$@)"			>> $(PWD)/Kbuild
	@echo -e "$(subst .ko,,$@)-y := $(drv_dsl_cpe_api_OBJS)"	>> $(PWD)/Kbuild
	@echo -e "EXTRA_CFLAGS := $(CFLAGS) -DHAVE_CONFIG_H $(drv_dsl_cpe_api_CFLAGS) $(DSL_DRIVER_INCL_PATH) -I@abs_srcdir@/include -I$(PWD)/include"	>> $(PWD)/Kbuild
	$(MAKE) ARCH=@KERNEL_ARCH@ -C @KERNEL_BUILD_PATH@ O=@KERNEL_BUILD_PATH@ M=$(PWD) modules

clean-generic:
	@echo "drv_dsl_cpe_api: Cleanup Linux 2.6.x kernel object build"
	@- find . -name ".*.cmd" | xargs rm -f
	@- rm -f Module.symvers Kbuild
	@- rm -rf .tmp_versions drv_dsl_cpe_api.mod.c

endif

lint:
		@flint -vm \
		-i$(shell dirname `$(CC) -print-file-name=include`)/include \
		-i@top_srcdir@ std_lx.lnt env-cw6.lnt $(AM_CPPFLAGS) -I$(PWD)/include\
		$(AM_CFLAGS) -DDSL_INTERN  -DHAVE_CONFIG_H -D__KERNEL__ -D__LINUX__ -DMODULE $(drv_dsl_cpe_api_SOURCES)
