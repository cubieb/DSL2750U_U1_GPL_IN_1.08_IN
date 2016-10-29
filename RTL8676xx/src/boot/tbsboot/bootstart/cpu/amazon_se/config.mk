#

TEXT_BASE_BOOTSTART =  0xa0010000
#TEXT_BASE_BOOTSTART =  0x80010000
#TEXT_BASE_BOOTSTART = 0xb0000000

v=$(shell \
$(CROSS_COMPILE)as --version|grep "GNU assembler"|awk '{print $$3}'|awk -F . '{print $$2}')
MIPSFLAGS=$(shell \
if [ "$v" -lt "14" ]; then \
	echo "-mcpu=4kc"; \
else \
	echo "-march=4kc -mtune=4kc"; \
fi)

ifneq (,$(findstring 4KCle,$(CROSS_COMPILE)))
ENDIANNESS = -EL
else
ENDIANNESS = -el #test by jy
#ENDIANNESS = -eb
endif

MIPSFLAGS += $(ENDIANNESS) -mabicalls 
#LDFLAGS	+= -EB

PLATFORM_CPPFLAGS += $(MIPSFLAGS) 
