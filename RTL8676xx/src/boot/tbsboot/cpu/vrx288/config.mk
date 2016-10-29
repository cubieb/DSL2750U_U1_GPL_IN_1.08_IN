#
# (C) Copyright 2003
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

#
# Danube board with MIPS 24Kec CPU core

-include $(TOPDIR)/.config
v=$(shell $(AS) --version|grep "GNU assembler"|cut -d. -f2)
ifndef PLATFORM_CPU
PLATFORM_CPU = 4kc
endif

ifeq ($(CONFIG_CPU_BIG_ENDIAN),y)
ENDIAN= -EB
BYTEORDER= -D__MIPSEB__
else
ENDIAN= -EL
BYTEORDER= -D__MIPSEL__
endif

MIPSFLAGS=$(shell if [ "$v" -lt "14" ]; then echo "-mcpu=$(PLATFORM_CPU)"; else echo "-march=$(PLATFORM_CPU) -mtune=$(PLATFORM_CPU)"; fi)
MIPSFLAGS += $(ENDIAN) $(BYTEORDER) -G 0 -mabicalls -fpic -msoft-float -Wstrict-prototypes -fno-tree-scev-cprop
PLATFORM_CPPFLAGS += $(MIPSFLAGS)
PLATFORM_LDFLAGS  += -G 0 -static -n -nostdlib


