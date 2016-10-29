# =============================================================================
#
# Copyright (C) 2001 Analog Devices Inc. All rights reserved
#
# The information and source code contained herein is the exclusive property
# of Analog Devices and may not be disclosed, examined or reproduced in whole
# or in part without the explicit written authorization from Analog Devices.
#
# ===========================================================================*/

# =============================================================================
#
# File name    : $RCSfile: Makefile,v $
# Author       : $Author: vreddy $
# Version      : $Revision: 1.1 $ # Description  : Makefile for the 'C' EMAC driver
#
# ===========================================================================*/

# Get the build configuration information. This includes the target
# to build

LIB =   vdslptm.a

OBJS =  apptmload.o  vdslptmmain.o \
        fusiv_decoder.o fusiv_encoder.o fusiv_rx_slb.o fusiv_tx_slb.o \
	vdslptm.o vdslptmdriver.o

APAS = ../tools/ap_asm/apas

AFLAGS = -DASSEMBLER -MMD -I../inc/netpro -DVDSLAP

PREINC = -I phy -I ptmap -I ptminclude

%.o:    ptmap/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(EXTRA) $(INCS) -I. -o $@ $<

%.o:    phy/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(EXTRA) $(INCS) -I. -o $@ $<

%.o:    driver/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(EXTRA) $(INCS) -I. -o $@ $<

include ../sys/config.mk

.PHONY: ptmap/vdslptmmain.c
ptmap/vdslptmmain.c:
	@( if [ -d ptmap/apcode ] ; then cd ptmap/apcode && $(MAKE) vdslptmmain.c ; fi )

clean:
	$(RM) *.d *.o *.a ptmap/apcode/vdslptmmain.c
