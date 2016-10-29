-include $(BUILDDIR)/.config
v=$(shell $(AS) --version|grep "GNU assembler"|cut -d. -f2)

MIPSFLAGS=$(shell \
if [ "$v" -lt "14" ]; then \
	echo "-mcpu=5281"; \
else \
	echo "-march=5281 -mtune=5281"; \
fi)

ifeq ($(CONFIG_CPU_BIG_ENDIAN),y)
ENDIAN= -EB
BYTEORDER= -D__MIPSEB__
else
ENDIAN= -EL
BYTEORDER= -D__MIPSEL__
endif

MIPSFLAGS += $(ENDIAN) $(BYTEORDER) -G 0 -mabicalls -fpic -msoft-float -Wstrict-prototypes -fno-tree-scev-cprop
PLATFORM_CPPFLAGS += $(MIPSFLAGS)
PLATFORM_LDFLAGS  += -G 0 -static -n -nostdlib

