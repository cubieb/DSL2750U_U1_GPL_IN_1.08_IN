#

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(CPU).a

START	= start.o
COBJS	=bcm_serial.o \
	  cpu.o interrupts.o
SOBJS	=  cache.o lowlevel_init.o 

SRCS	:= $(START:.o=.S) $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend $(START) $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
