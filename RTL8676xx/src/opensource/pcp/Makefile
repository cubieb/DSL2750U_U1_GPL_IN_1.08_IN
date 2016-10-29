# CFLAGS = -Os -fomit-frame-pointer 
# LDFLAGS = -Wl,-allow-shlib-undefined 
ifeq ($(strip $(BUILD_PCP)), static)
CFLAGS += -DBUILD_STATIC
endif

CFLAGS+= -Wall -I$(SRCDIR)/kernel/$(TBS_KERNEL_VERSION)/include/   

OBJS = config.o ext_intf_brcm.o options.o pcp.o pcp_client.o timer.o

all: pcp

.c.o:
	$(CC) $(CFLAGS) -c $<

pcp: $(OBJS) 
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

install:
	install -c pcp $(FSROOT)/usr/bin
	$(STRIP) $(FSROOT)/usr/bin/pcp
clean:
	-rm -f pcp *.o INSTALL README

distclean: clean




## Makefile file for pcp
##
## License:  GNU General Public License, Version 2.
##
#EXE = pcp
#
#
#OBJS=config.o ext_intf_brcm.o options.o pcp.o pcp_client.o timer.o
#
#LIBS= -lcms_msg $(CMS_COMMON_LIBS)
#
#all dynamic: sanity_check $(EXE) generic_exe_install
#
## Remove symlinks so customers do not import them into their source control systems
#clean: generic_clean
#	rm -f $(INSTALL_DIR)/bin/$(EXE)
#	rm -f INSTALL README
#
#
##
## Set our CommEngine directory (by splitting the pwd into two words
## at /userspace and taking the first word only).
## Then include the common defines under CommEngine.
##
#CURR_DIR := $(shell pwd)
#BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
#BUILD_DIR:=$(word 1, $(BUILD_DIR))
#
#include $(BUILD_DIR)/make.common
#
#
#
##
## GPL apps and libs are only allowed to include header files from the
## gpl and public directories
##
## WARNING: Do not modify this section unless you understand the
## license implications of what you are doing.
##
#ALLOWED_INCLUDE_PATHS := -I.\
#                         -I$(BUILD_DIR)/userspace/gpl/include  \
#                         -I$(BUILD_DIR)/userspace/gpl/include/$(OALDIR) \
#                         -I$(BUILD_DIR)/userspace/public/include  \
#                         -I$(BUILD_DIR)/userspace/public/include/$(OALDIR)
#
#
##
## GPL apps and libs are only allowed to link with libraries from the
## gpl and public directories.
##
## WARNING: Do not modify this section unless you understand the
## license implications of what you are doing.
##
#ALLOWED_LIB_DIRS := /lib:/lib/gpl:/lib/public
#
#
#
##
## In CMS, we want a standalone ftpd app, so build dynamic
## makemenuconfig should be changed so that static is not even an option
##
#ifeq ($(strip $(BUILD_PCP)), static)
#	CFLAGS += -DBUILD_STATIC
#endif
#
##
## Implicit rule will make the .c into a .o
## Implicit rule is $(CC) -c $(CPPFLAGS) $(CFLAGS)
## See Section 10.2 of Gnu Make manual
## 
#$(EXE): $(OBJS)
#	$(CC) -o $@ $(OBJS) -Wl,-rpath,$(CMS_LIB_RPATH) $(CMS_LIB_PATH) $(LIBS)
#
#
#
##
## Some legacy targets.  Do we even support static mode anymore?
##
#static: pcp.a
#
#pcp.a: $(OBJS)
#	$(CC) $(CFLAGS) -c -o $(OBJS)
#	$(AR) rcs pcp.a $(OBJS) $(LIBS)
#
#
#
##
## Include the rule for making dependency files.
## The '-' in front of the second include suppresses
## error messages when make cannot find the .d files.
## It will just regenerate them.
## See Section 4.14 of Gnu Make.
##
#
#include $(BUILD_DIR)/make.deprules
#
#-include $(OBJS:.o=.d)
#
#
#
