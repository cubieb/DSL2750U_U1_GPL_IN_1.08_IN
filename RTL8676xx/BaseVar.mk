#
################# set base variable########################################

-include $(ROOTDIR)/build/arch.mk
-include $(ROOTDIR)/build/.product

HOSTCC          = gcc
HOSTCFLAGS      = -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer
CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
  else if [ -x /bin/bash ]; then echo /bin/bash; \
	  else echo sh; fi ; fi)

ifeq ($(V),1)
Q=
else
Q=@
endif
CP=$(shell unalias cp)
AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
RANLIB 		= $(CROSS_COMPILE)ranlib
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
SSTRIP           = $(CROSS_COMPILE)sstrip
LSTRIP           = $(CROSS_COMPILE)lstrip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump
MAKE		= make
#MAKEFILES       = $(ROOTDIR)/.config
GENKSYMS        = /sbin/genksyms
DEPMOD          = /sbin/depmod
KALLSYMS        = /sbin/kallsyms
MODFLAGS        = -DMODULE
CFLAGS_KERNEL   =
TBS_KERNEL_VERSION ?= linux-2.6.18.8

KERNEL_DIR =$(BUILDDIR)/kernel/$(TBS_KERNEL_VERSION)

PERL            = perl
AWK             = awk
RPM             := $(shell if [ -x "/usr/bin/rpmbuild" ]; then echo rpmbuild; \
                        else echo rpm; fi)
TMPPREFIX       =
LDCONFIGS	=

PATH := $(PATH):$(ROOTDIR)/tools:$(TOOLCHAINS_PATH)
BUILDDIR := $(ROOTDIR)$(if $(notdir $(ROOTDIR)),/)build
TMPDISTDIR := $(ROOTDIR)$(if $(notdir $(ROOTDIR)),/)build/tmpdist
SRCDIR	 := $(ROOTDIR)$(if $(notdir $(ROOTDIR)),/)src
ROMFS=${BUILDDIR}/romfs
FSROOT=${ROMFS}/rootfs
CUSTOMER = customer
RTL8676V = RTL8676xx
TBS_VERSION := $(word 2, $(shell LANG=C svn info $(ROOTDIR)| grep Revision))
#ifneq ($(findstring $(SRCDIR),$(CURDIR)),)
#$(error Please build code from $(BUILDDIR) instead of $(SRCDIR), you should build it use the path $(subst $(SRCDIR),$(BUILDDIR),$(CURDIR))))
#endif

#
##########################export variables############################################################

export PATH ROOTDIR BUILDDIR TMPDISTDIR SRCDIR DIRS ROMFS FSROOT TBS_VERSION TBS_KERNEL_VERSION CUSTOMER RTL8676V
export TOOLCHAINS_PATH TOOLCHAINS_SYSROOT
export HOSTCC CROSS_COMPILE AS RANLIB LD CC CPP AR NM STRIP OBJCOPY OBJDUMP
export MAKE GENKSYMS DEPMOD KALLSYMS MODFLAGS CFLAGS_KERNEL PERL AWK RPM TMPPREFIX CONFIG_SHELL
export TARGET ARCH CPU PRODUCT DIGIT_SIG ENDIANNESS CONFIG_TBS_RELEASE Q KERNEL_DIR
export KERNEL_VERSION KERNELVERSION
#######################################################################################################
