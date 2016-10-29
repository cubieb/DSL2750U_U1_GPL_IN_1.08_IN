include $(TOPDIR)/config_bootstart.mk

LIB	= $(obj)lib$(CPU).a

START	= start.o
SOBJS	= arm1_init.o lowlevel_init.o
COBJS	= interrupts.o cpu.o board.o bsp1000.o serial.o training1000.o

SRCS	:= $(START:.o=.S) $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS) $(SOBJS))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend $(START) $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
