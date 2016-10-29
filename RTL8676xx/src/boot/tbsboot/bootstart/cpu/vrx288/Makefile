#

include $(TOPDIR)/config_bootstart.mk

LIB	= $(obj)lib$(CPU).a

START	= start.o 
COBJS	= tune_ddr.o
COBJS += cpu.o ifx_asc.o

SOBJS	= lowlevel_init.o

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
