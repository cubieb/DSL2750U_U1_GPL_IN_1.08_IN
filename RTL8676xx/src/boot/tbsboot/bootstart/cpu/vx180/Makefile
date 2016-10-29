include $(TOPDIR)/config_bootstart.mk

LIB	= $(obj)lib$(CPU).a

START	= start.o 
COBJS	= cpu.o 
#SOBJS	= cache.o

#SRCS	:= $(START:.o=.S) $(SOBJS:.o=.S) $(COBJS:.o=.c)
#OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
SRCS	:= $(START:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend $(START) $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
