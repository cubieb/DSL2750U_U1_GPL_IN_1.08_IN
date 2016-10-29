# RAM version
TEXT_BASE =  0x80f80000
#TEXT_BASE =  0x81f80000

v=$(shell \
$(CROSS_COMPILE)as --version|grep "GNU assembler"|awk '{print $$3}'|awk -F . '{print $$2}')
MIPSFLAGS=$(shell \
if [ "$v" -lt "14" ]; then \
	echo "-mcpu=5181"; \
else \
	echo "-march=5181 -mtune=5181"; \
fi)

ENDIANNESS = -eb

MIPSFLAGS += $(ENDIANNESS) -mabicalls
#LDFLAGS	+= -EB

PLATFORM_CPPFLAGS += $(MIPSFLAGS)
