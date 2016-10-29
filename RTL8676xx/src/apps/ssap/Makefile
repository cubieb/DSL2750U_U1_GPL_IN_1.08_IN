include $(ROOTDIR)/.config
KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/${TBS_KERNEL_VERSION}/include
export CFLAGS =-fPIC -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include
subdir-y := lib pc flash_test #mon
#subdir-$(CONFIG_APPS_SSAP_BUTTON) += button
subdir-$(CONFIG_APPS_SSAP_UPGRADE) += upg
subdir-$(CONFIG_APPS_SSAP_TFTPUPG) += tftpu
subdir-$(CONFIG_APPS_SSAP_PROTEST) += pti
subdir-$(CONFIG_APPS_SSAP_GETSMAPS) += getsmaps
subdir-$(CONFIG_APPS_LOGIC_TR143) += udpserver
include $(ROOTDIR)/Rules.make

