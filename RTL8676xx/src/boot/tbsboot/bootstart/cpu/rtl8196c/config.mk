# ROM version
#TEXT_BASE_BOOTSTART = 0xbfc00000
TEXT_BASE_BOOTSTART = 0xbd000000
# RAM version
#TEXT_BASE_BOOTSTART =  0xbd380000

v=$(shell \
$(CROSS_COMPILE)as --version|grep "GNU assembler"|awk '{print $$3}'|awk -F . '{print $$2}')
MIPSFLAGS=$(shell \
if [ "$v" -lt "14" ]; then \
	echo "-mcpu=4181"; \
else \
	echo "-march=4181 -mtune=4181"; \
fi)

ENDIANNESS = -eb 
MIPSFLAGS += $(ENDIANNESS) -mabicalls

PLATFORM_CPPFLAGS += $(MIPSFLAGS)
