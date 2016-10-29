#
# (C) Copyright 2000-2006
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

include $(TOPDIR)/config.mk



FSBOOT = $(obj)solosw-boot.bin

SOBJS	= fs_boot.o

SRCS	:= $(SOBJS:.o=.S)
OBJS	:= $(addprefix $(obj),$(SOBJS))

all:	$(obj).depend  $(FSBOOT)


$(FSBOOT): $(OBJS)
	$(LD) -Bstatic -T fs_boot.lds $(OBJS) -Map $(obj)fs_boot.map -o $(obj)fs_boot.elf
	$(ELF2AOUT) $(obj)fs_boot.elf $(obj)fs_boot.aout
	$(BIN2AOUT) $(obj)orig-boot.bin $(obj)orig-boot.aout
	$(MKHFBOOT) -npboot $(obj)fs_boot.aout -ppboot $(obj)orig-boot.aout  -bps 63 -o $@

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
