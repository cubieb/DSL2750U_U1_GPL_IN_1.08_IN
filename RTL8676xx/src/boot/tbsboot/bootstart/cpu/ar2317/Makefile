#

include $(TOPDIR)/config_bootstart.mk

LIB	= $(obj)lib$(CPU).a

START	= start.o 
COBJS	= cpu.o interrupts.o ar2317_serial.o #ar2317_spiflash.o 
SOBJS	=  cache.o lowlevel_init.o ar2317_pll.o 

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
