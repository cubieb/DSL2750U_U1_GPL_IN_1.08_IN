## Process this file with automake to produce Makefile.in

# Target definition depending on kernel version:
if KERNEL_2_6
noinst_PROGRAMS = drv_dsl_cpe_api
bin_PROGRAMS = drv_dsl_cpe_api.ko
else
bin_PROGRAMS = drv_dsl_cpe_api
endif

noinst_LIBRARIES = lib_dsl_cpe_api.a

#bin_SCRIPTS = inst_drv_dsl_cpe_api.sh


## common driver sources always needed for build
drv_common_sources = \
    common/ifx_adsl_autoboot.c \
    common/ifx_adsl_basic.c \
    common/ifx_adsl_ceoc.c \
    common/ifx_adsl_led.c \
    common/ifx_adsl_mib.c

## common driver sources always needed for build
drv_common_sources_module = \
    common/drv_dsl_linux.c

## common driver sources always included additionally in the distribution
drv_common_extra = \
    include/ifx_adsl_autoboot.h \
    include/ifx_adsl_basic.h \
    include/ifx_adsl_basic_ioctl.h \
    include/ifx_adsl_bsp.h \
    include/ifx_adsl_ceoc.h \
    include/ifx_adsl_cmvs.h \
    include/ifx_adsl_led.h \
    include/ifx_adsl_linux.h \
    include/ifx_adsl_mib.h \
    include/ifx_adsl_mib_cmv.h \
    include/ifx_adsl_mib_ioctl.h \
    include/drv_dsl_cpe_api_danube_kernel_config.h \
    include/drv_dsl_cpe_api_config.h.in


## Files specific for the Danube
## driver sources
drv_danube_specific_sources =

## extra distribution for Danube
drv_danube_specific_extra =

if DRIVER_DANUBE
drv_specific_sources = $(drv_danube_specific_sources)
drv_specific_extra = $(drv_danube_specific_extra)
endif


## all sources for the current driver build
drv_dsl_cpe_api_SOURCES = \
    $(drv_common_sources) \
    $(drv_specific_sources) \
    $(drv_common_sources_module)

lib_dsl_cpe_api_a_SOURCES = \
    $(drv_common_sources) \
    $(drv_specific_sources)


## all extra sources for the current library distribution
EXTRA_DIST = \
    $(drv_common_extra) \
    $(drv_specific_extra)


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

# Extra rule for linux-2.6 kernel object
drv_dsl_cpe_api.ko$(EXEEXT): drv_dsl_cpe_api
		@echo -e "Build Linux-2.6.x Kernel Object"
		@if test ! -d kbuild26; then \
		mkdir kbuild26; \
		fi
		@echo -e "obj-m := $<.o" > kbuild26/Makefile
		@echo -e "$<-objs := ../$<" >> kbuild26/Makefile
		$(MAKE) ARCH=@KERNEL_ARCH@ -C @KERNEL_BUILD_PATH@ M=$(PWD)/kbuild26 modules
		mv kbuild26/$@ .
		rm -rf kbuild26

