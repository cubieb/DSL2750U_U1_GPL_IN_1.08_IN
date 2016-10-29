#

include $(TOPDIR)/config.mk
$(shell rm ${BUILDDIR}/boot/tbsboot/cpu/${CPU}/product.c -f)
$(shell ln -s ${BUILDDIR}/product/product.c ${BUILDDIR}/boot/tbsboot/cpu/${CPU}/product.c)
LIB	= $(obj)lib$(CPU).a

START	= start.o 
COBJS	= cpu.o product.o spi_flash.o
COBJS  += rtl8676_swiob.o rtl8676_swmain.o rtl8676_swcore.o rtl8676_swsmi.o rtl8676_swnic.o rtl8676_swtable.o rtl8676_swvlan.o rtl8367b_asicdrv.o 
#COBJS += rtl8676_swport.o
#COBJS  += interrupts.o cfi_flash.o
COBJS  += rtl8676_uart.o rtl8676_gpio.o rtl8676_led.o rtl8676_button.o
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
