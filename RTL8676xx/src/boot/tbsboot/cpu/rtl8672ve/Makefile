#

include $(TOPDIR)/config.mk
$(shell rm ${BUILDDIR}/boot/tbsboot/cpu/${CPU}/product.c -f)
$(shell ln -s ${BUILDDIR}/product/product.c ${BUILDDIR}/boot/tbsboot/cpu/${CPU}/product.c)
LIB	= $(obj)lib$(CPU).a

START	= start.o 
COBJS	= cpu.o interrupts.o led.o product.o uart.o re8670poll.o iob.o cfi_flash.o spi_flash.o
SOBJS	=  

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
