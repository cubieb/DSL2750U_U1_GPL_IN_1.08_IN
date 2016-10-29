include $(TOPDIR)/config_bootstart.mk

LIB    = $(obj)libgeneric.a

COBJS  = LzmaDecode.o
#COBJS += string.o dlmalloc.o console.o vsprintf.o ctype.o 
#COBJS += crc32.o adler32.o infblock.o infcodes.o inflate.o \
         inftrees.o infutil.o zutil.o uncompr.o LzmaDecode.o \
         ctype.o display_options.o string.o vsprintf.o

SRCS   := $(COBJS:.o=.c)
OBJS   := $(addprefix $(obj),$(COBJS))


all : $(LIB)


$(LIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)



#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################







