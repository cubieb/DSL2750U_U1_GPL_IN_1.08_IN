#

include $(TOPDIR)/config.mk
$(shell rm ${BUILDDIR}/boot/tbsboot/cpu/vx180/product.c -f)
$(shell ln -s ${ROOTDIR}/product/${PRODUCT}/product.c ${BUILDDIR}/boot/tbsboot/cpu/vx180/product.c)
LIB	= $(obj)lib$(CPU).a



START   = start.o
#SOBJS   = cache.o
COBJS    = interrupts.o cpu.o  ns16550.o seth.o simple_eth.o led.o gpio.o product.o #fusivmr1.o
#COBJS    = interrupts.o cpu.o voxpci.o simple_eth.o seth.o fusivmr1.o


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
