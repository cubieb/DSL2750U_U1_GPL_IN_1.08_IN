include $(ROOTDIR)/.config

O_TARGET=cfg.o
obj-y = cfg_accessor.o cfg_comm.o cfg_file.o cfg_get.o cfg_mid.o cfg_set.o cfg_r2.o
EXTRA_LDFLAGS =  
EXTRA_CFLAGS = -fPIC -I$(BUILDDIR) -I../msg -I../../include -I../syslog -I../cfg -I$(TMPDISTDIR)/include \
               -I../../../include \
               -D_CFG_TREE_ALL_STANDARD 

ifeq (${PRODUCT}, UML)
	EXTRA_CFLAGS += -D_CFG_UML
endif

ifdef CONFIG_TBS_APPS_DEBUG_CFG
	EXTRA_CFLAGS += -D_CFG_DEBUG
endif

include $(ROOTDIR)/Rules.make

