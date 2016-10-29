-include $(TOPDIR)/.config
v=$(shell $(AS) --version|grep "GNU assembler"|cut -d. -f2)

MIPSFLAGS=$(shell \
if [ "$v" -lt "14" ]; then \
	echo "-mcpu=5281"; \
else \
	echo "-march=5281"; \
fi)


MIPSFLAGS += $(ENDIAN) $(BYTEORDER) -mabicalls -fpic -msoft-float -Wstrict-prototypes -fno-tree-scev-cprop
PLATFORM_CPPFLAGS += $(MIPSFLAGS)
