#obj-y += 867x_sar.o

#867x_sar-objs := atmdev_init.o ra8670.o ra8670app.o ra867x_pp.o ra_debug.o
obj-y += atmdev_init.o ra8670.o ra8670app.o ra867x_pp.o ra_debug.o
obj-$(CONFIG_SAR_FASTSKB)		+= fastskb.o

all: check_obj first_rule

check_obj:
	file $(obj-y) | grep Intel | awk -F: '{print $$1};' | xargs rm -f --

EXTRA_CFLAGS += -Wall -ggdb -Wall

#include $(TOPDIR)/Rules.make
