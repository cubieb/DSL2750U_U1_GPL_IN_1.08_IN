#

include $(TOPDIR)/config.mk
$(shell rm ${BUILDDIR}/boot/tbsboot/cpu/amazon_se/product.c -f)
$(shell ln -s ${BUILDDIR}/product/product.c ${BUILDDIR}/boot/tbsboot/cpu/${CPU}/product.c)
LIB	= $(obj)lib$(CPU).a

START	= start.o 
COBJS	= cpu.o interrupts.o amazon_serial.o amazon_cfi_flash.o amazon_sw.o led.o ssc.o ssc_extra.o sflash_extra.o product.o
#amazon_flash.o  #Read flash ID
SOBJS	=  #cache.o lowlevel_init.o 

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
