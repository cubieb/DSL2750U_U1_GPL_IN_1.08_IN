include $(TOPDIR)/config_bootstart.mk

LIB	= $(obj)lib$(CPU).a
#CFLAGS += -x assembler-with-cpp 
#CFLAGS += -pedantic -nostartfiles -nostdinc -mno-abicalls -fno-pic -gstabs+
START	= start.o 
#COBJS	= rtl8676_serial.o 
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
