#
# drivers/net/rtl8192cd
#
# Makefile for the Linux Wireless network device drivers.
#

# O_TARGET := rtl8192cd.o

#EXTRA_CFLAGS += -Werror

# for rules.make
# WIRELESS_COMPILER=1

#subdir-$(CONFIG_RTL8192SE)	+= rtl8192Pci

#add for MESH
#ifeq ($(CONFIG_MESH_ENABLE),y)
#	EXTRA_CFLAGS += -DCONFIG_RTK_MESH #-DMESH_USE_METRICOP
#	ifeq ($(CONFIG_11S_TEST_MODE),y)
#	EXTRA_CFLAGS += -D_11s_TEST_MODE_
#	endif	
#endif
# -DCONFIG_RTL8196B 

EXTRA_CFLAGS += -DCONFIG_RTL8196B -DCONFIG_RTL8196C -DCONFIG_RTL8196B_GW -DCONFIG_RTL8196C_TESTCHIP_PATCH -D_MP_TELNET_SUPPORT_
EXTRA_CFLAGS += -DCONFIG_COMPAT_NET_DEV_OPS
# -include $(dir $@)rtl8672_port.h 

TARGET = rtl8192cd.o



rtl8192cd-objs := 8192cd_ioctl.o\
 	$(join $(subdir-y), $(patsubst %,/%.o,$(notdir $(subdir-y))))

rtl8192cd-objs +=\
	8192cd_tx.o\
	8192cd_rx.o\
	8192cd_osdep.o\
	8192cd_sme.o\
	8192cd_util.o\
	8192cd_hw.o\
	8192cd_security.o\
	8192cd_tkip.o\
	8192cd_aes.o\
	8192cd_proc.o\
	8192cd_br_ext.o\
	8192cd_eeprom.o\
	8192cd_mp.o\
	8192cd_psk.o\
	1x_kmsm_aes.o\
	1x_kmsm_hmac.o\
	1x_md5c.o\
	1x_rc4.o\
	8192cd_mib.o\
	8192cd_dmem.o\
	romeperf.o\
	rtl8672_port.o
	
#add for MESH
#ifeq ($(CONFIG_MESH_ENABLE),y)
#obj-y +=\
#	mesh_ext/mesh_proc.o\
#	mesh_ext/mesh_route.o\
#	mesh_ext/mesh_rx.o\
#	mesh_ext/mesh_sme.o\
#	mesh_ext/mesh_security.o\
#	mesh_ext/mesh_tx.o\
#	mesh_ext/mesh_util.o\
#	mesh_ext/mesh_11kv.o\
#	mesh_ext/hash_table.o
#endif

#add for WAPI
ifeq ($(CONFIG_RTL_WAPI_SUPPORT),y)
//obj-y += SMS4.o wapi_wai.o wapiCrypto.o	
rtl8192cd-objs += wapi_wai.o wapiCrypto.o	
endif	

# rock: link binary data via bin2c.pl
.SECONDARY: $(SRCS_TXT) $(SRCS_BIN)
SRCS_TXT = $(addprefix data_, $(notdir $(patsubst %.txt,%.c,$(wildcard data/*.txt))))
SRCS_BIN = $(addprefix data_, $(notdir $(patsubst %.bin,%.c,$(wildcard data/*.bin))))
#obj-y += $(SRCS_TXT:.c=.o) $(SRCS_BIN:.c=.o)
data_%.c: data/%.txt
	perl $(dir $@)/bin2c.pl $(notdir $(basename $@)) < $< > $@
data_%.c: data/%.bin
	perl $(dir $@)/bin2c.pl $(notdir $(basename $@)) < $< > $@
# create our target
all: $(SRCS_TXT) $(SRCS_BIN) 

##############################
#krammer add for auto generate the data files 20100208
K_SRCS_TXT = $(addprefix $(obj)/data_, $(notdir $(patsubst %.txt,%.c,$(wildcard $(obj)/data/*.txt))))
K_SRCS_BIN = $(addprefix $(obj)/data_, $(notdir $(patsubst %.bin,%.c,$(wildcard $(obj)/data/*.bin))))
$(obj)/data_%.c: $(obj)/data/%.txt
	    perl $(dir $@)/bin2c.pl $(notdir $(basename $@)) < $< > $@
$(obj)/data_%.c: $(obj)/data/%.bin
	    perl $(dir $@)/bin2c.pl $(notdir $(basename $@)) < $< > $@
$(obj)/8192cd_hw.o: $(K_SRCS_TXT) $(K_SRCS_BIN)
###############################

obj-y += $(SRCS_TXT) $(SRCS_BIN) $(TARGET) 
# include $(TOPDIR)/Rules.make
