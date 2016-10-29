#

include $(TOPDIR)/config_bootstart.mk 

LIB	= $(obj)lib$(CPU).a



START   = start.o
SOBJS   = cache.o
COBJS    = fusivmr1.o #ns16550.o interrupts.o cpu.o
#COBJS    = interrupts.o cpu.o voxpci.o simple_eth.o seth.o fusivmr1.o


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
