export WLAN_CONF_MK_DIR:=$(shell pwd)/
include $(WLAN_CONF_MK_DIR)/config.mk

#MODE  STA or AP or APSTA
#RT28xx_MODE = STA
RT28xx_MODE = AP
#RT28xx_MODE = APSTA
#TARTET = LINUX or UCOS 
TARGET = LINUX
#CHIPSET = 2860 or 2870
CHIPSET = 2860
#CHIPSET = 2870
#RT28xx_DIR = home directory of RT28xx source code
RT28xx_DIR = $(shell pwd)
RTMP_SRC_DIR = $(RT28xx_DIR)/RT$(CHIPSET)
#PLATFORM = IKANOS_V160
PLATFORM = IKANOS_V180
RELEASE = DPA
#RELEASE = DPB
LINUX_SRC := $(PATH_KERNEL)

export RT28xx_DIR RT28xx_MODE PATH_KERNEL CROSS_COMPILE PLATFORM RELEASE CHIPSET RTMP_SRC_DIR LINUX_SRC

all: build_tools $(TARGET)


build_tools:
	make -C tools
	$(RT28xx_DIR)/tools/bin2h

UCOS:
	make -C os/ucos/ MODE=$(RT28xx_MODE)
	echo $(RT28xx_MODE)


LINUX:
ifneq (,$(findstring 2.4,$(PATH_KERNEL)))
	cp -f os/linux/Makefile.4 $(RT28xx_DIR)/os/linux/Makefile
	make -C $(RT28xx_DIR)/os/linux/	
else
	cp -f os/linux/Makefile.6 $(RT28xx_DIR)/os/linux/Makefile
	make -C $(KERNELPATH) M=$(RT28xx_DIR)/os/linux/ modules KBUILD_OUTPUT= ARCH=${ARCH} CROSS_COMPILE=$(CROSS_COMPILE) V=1
endif

release:
ifeq ($(TARGET), LINUX)
	make -C $(RT28xx_DIR)/os/linux -f Makefile.release release
ifeq ($(RELEASE), DPA)
	make -C $(RT28xx_DIR)/os/linux -f Makefile.DPA release
	rm -rf build
endif
ifeq ($(RELEASE), DPB)
	make -C $(RT28xx_DIR)/os/linux -f Makefile.DPB release
	rm -rf build
endif
endif

cpdriver:
	mkdir -p ../../fusiv_src/vendors/Ralink/ra11n
	cp -f $(RT28xx_DIR)/os/linux/*.ko  ../../fusiv_src/vendors/Ralink/ra11n
	cp -f $(RT28xx_DIR)/RT2860AP.dat ../../fusiv_src/vendors/Ralink/ra11n

prerelease:
	make -C $(RT28xx_DIR)/os/linux -f Makefile.release prerelease
	cp $(RT28xx_DIR)/os/linux/Makefile.DPB $(RTMP_SRC_DIR)/os/linux/.
	cp $(RT28xx_DIR)/os/linux/Makefile.DPA $(RTMP_SRC_DIR)/os/linux/.

clean:
ifeq ($(TARGET), LINUX)
ifneq (,$(findstring 2.4,$(PATH_KERNEL)))
	cp -f os/linux/Makefile.4 os/linux/Makefile
else
	cp -f os/linux/Makefile.6 os/linux/Makefile
endif
	make -C os/linux clean
	rm -rf os/linux/Makefile
endif	
ifeq ($(TARGET), UCOS)
	make -C os/ucos clean MODE=$(RT28xx_MODE)
endif

uninstall:
ifeq ($(TARGET), LINUX)
ifneq (,$(findstring 2.4,$(PATH_KERNEL)))
	make -C $(RT28xx_DIR)/os/linux -f Makefile.4 uninstall
else
	make -C $(RT28xx_DIR)/os/linux -f Makefile.6 uninstall
endif
endif

install:
ifeq ($(TARGET), LINUX)
ifneq (,$(findstring 2.4,$(PATH_KERNEL)))
	make -C $(RT28xx_DIR)/os/linux -f Makefile.4 install
else
	make -C $(RT28xx_DIR)/os/linux -f Makefile.6 install
endif
endif

