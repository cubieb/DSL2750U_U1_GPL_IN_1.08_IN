include $(ROOTDIR)/.config

O_TARGET = ssap_msg.o
obj-y = msg.o hash.o new_msg.o common_func.o common_msg_handler.o cache_list.o

EXTRA_CFLAGS = -fPIC -I. -I../../include -I../include -I../syslog -I../cfg -I../pc -I$(TMPDISTDIR)/include -I$(BUILDDIR) -Wall

ifdef CONFIG_APPS_TM
EXTRA_CFLAGS += -D_MSG_SUPPORT_TM
endif

ifdef CONFIG_TBS_APPS_DEBUG_MSG
EXTRA_CFLAGS += -D_MSG_DEBUG  # -DCOMMON_MSG_DEBUG_SWITCH -D_MSG_DEBUG_SWITCH
endif

include $(ROOTDIR)/Rules.make

