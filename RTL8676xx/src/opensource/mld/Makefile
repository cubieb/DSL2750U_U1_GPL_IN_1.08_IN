

#
# Simple build file for now
#
#
CFLAGS = -Os -fomit-frame-pointer
LDFLAGS = -Wl,-allow-shlib-undefined
ifeq ($(strip $(BUILD_MLD)), static)
CFLAGS += -DBUILD_STATIC
endif

KERNEL_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)
CFLAGS+= -Wall -I$(KERNEL_DIR)/include/
#-DDEBUG_MLD_PROXY

OBJS = mld_proxy.o mld_util.o mld_krnlif.o mld.o mld_proxy_api.o

all: mld

.c.o:
	$(CC) $(CFLAGS) -c $<

mld: $(OBJS) 
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

install:
	install -c mld $(FSROOT)/usr/bin
	$(STRIP) $(FSROOT)/usr/bin/mld
clean:
	-rm -f mld *.o

distclean: clean