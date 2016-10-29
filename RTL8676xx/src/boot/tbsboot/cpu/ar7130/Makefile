#

include $(TOPDIR)/config.mk
$(shell rm ${BUILDDIR}/boot/tbsboot/cpu/ar7130/product.c -f)
$(shell ln -s ${BUILDDIR}/product/product.c ${BUILDDIR}/boot/tbsboot/cpu/${CPU}/product.c)
LIB	= $(obj)lib$(CPU).a

START	= start.o 
COBJS	= cpu.o interrupts.o atheros_serial.o spi_flash.o ag7100_ecos.o athrs26_phy.o led.o product.o
SOBJS	=  
#cache.o lowlevel_init.o atheros_pll.o 

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
