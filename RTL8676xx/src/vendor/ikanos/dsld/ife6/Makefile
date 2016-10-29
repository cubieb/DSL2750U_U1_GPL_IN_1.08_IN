TMP_ROOTFS = $(ROOTDIR)/build/romfs/rootfs
IKANOS_DRV_DIR = $(ROOTDIR)/build/driver/dsl/ikanos
PATH_KERNEL=$(ROOTDIR)/build/kernel/linux-2.6.18.8
LIB = vdsld

all: vdsld vdsl

XFLAGS = -DEXPORT_SYMTAB -DVTU_R -DBME_CPE_20 -DCPE_1P_BOARD -DNO_SET_PROD_MODE\
         -DPRODUCTION_CODE -DCPE_30 -DCPE_40 -DCPE_50 -DALASKA \
	 	 -DIK_PRINTF=printf -DFUSIV_VX180 -DIKANOS_CHANGES -DLINUX_2_6 -Os \
#	 -DMENU_BASED
#	 -DIK_PRINTF=printf -DFUSIV_VX180 -Dgets=TKgets
#XFLAGS += -DIDS_VX180

LPFLAGS =
LPFLAGS1 = -lpthread -lm -lssap -L$(ROOTDIR)/build/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz -lcrypt -lm
ZLIB_DEPS = $(ROOTDIR)/build/tmpdist/lib/libz.so

PREINC = -I./include -I./apps -I$(ROOTDIR)/build/tmpdist/include \
	 	 -I./ipos/include -I./vxworklib \
	     -I$(PATH_KERNEL)/include \
	 	 -I${ROOTDIR}/build \
	 	 -I$(IKANOS_DRV_DIR) \
	 	 -I$(ROOTDIR)/build/include
		 
IPOS_BME_OBJS = apicommon.o apientries.o iposapi.o msgbuf.o \
                bmesinit.o dapi.o bmehwapi.o aztec_io.o \
                tstdapi.o bmeEventLog.o bme_mon.o fusiv_bme_api.o \

#elmui.o      oamptask.o     portmaptbl.o   uitask.o
APPS_OBJS = \
appstart.o   notchBnd.o     perToneApps.o  rmmapp.o rmmhook.o \
autoStart.o  led.o          oampdb.o       pmhandler.o    systemState.o \
bmetl.o      linecrc.o      oampfuncs.o    portmap.o \
elmui.o      oamptask.o     uitask.o \
eocapi.o     map1portcpe.o  oneSecTask.o   pprofile.o     uiUtil.o \
eocapp.o     password.o     report.o eoclb.o      \
perfhandler.o  elmapi.o   elmcpepe.o elmpe.o     \
elmcope.o  elmdeferred.o       elmsys.o rfmutil.o\
ipostimer.o  mesgparser.o msg.o

ifneq ($(findstring IDS_VX180,$(XFLAGS)),)
APPS_OBJS += idshost.o idscmds.o
endif

UTIL_OBJS = eventlog.o consoleUtil.o

SYS_OBJS = bsp.o sdram.o flash.o
OS_OBJS = msgQLib.o semLib.o kernelLib.o taskLib.o wdLib.o

vdsld:	$(APPS_OBJS) $(UTIL_OBJS) $(SYS_OBJS) $(IPOS_BME_OBJS) $(OS_OBJS)
	${CC} $(PREINC) $(NCOPTS) $(XFLAGS) $(LPFLAGS1) $(INCS) $(ZLIB_DEPS) -I. -o vdsld $(APPS_OBJS) $(UTIL_OBJS) $(SYS_OBJS) $(IPOS_BME_OBJS) $(OS_OBJS) $(ZLIB_DEPS)
	#cp ipos/bmeapi/dapi/Vx180Code.cbin $(TMP_ROOTFS)/bin/
#	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(LPFLAGS) $(INCS) -I. -o $@ $<


vdsl: vdsl_client.c
	$(CC) $(PREINC) $(XFLAGS) -o vdsl vdsl_client.c


OBJS =  \
	$(IPOS_BME_OBJS) $(SYS_OBJS) $(UTIL_OBJS) $(APPS_OBJS) $(OS_OBJS)

%.o:	ipos/bmeapi/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.o:    ipos/bmeapi/dapi/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.o:	ipos/bmedebug/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.o:	system/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.o:	util/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.o:	apps/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.o:	apps/elm/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.o:	apps/rfm/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.o:	vxworklib/%.c
	$(CC) -c $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.s:	ipos/bmeapi/%.c
	$(CC) -S $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.s:    ipos/bmeapi/dapi/%.c
	$(CC) -S $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.s:	ipos/bmedebug/%.c
	$(CC) -S $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.s:	system/%.c
	$(CC) -S $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.s:	util/%.c
	$(CC) -S $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.s:	apps/%.c
	$(CC) -S $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.s:	apps/elm/%.c
	$(CC) -S $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.s:	apps/rfm/%.c
	$(CC) -S $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

%.s:	vxworklib/%.c
	$(CC) -S $(PREINC) $(NCOPTS) $(XFLAGS) $(INCS) $(LPFLAGS) -I. -o $@ $<

install:
	mkdir -p $(TMP_ROOTFS)/usr/sbin
	cp -f vdsld $(TMP_ROOTFS)/usr/sbin
	mkdir -p $(TMP_ROOTFS)/bin
	cp -f ipos/bmeapi/dapi/Vx180Code.cbin $(TMP_ROOTFS)/bin/
	mkdir -p $(TMP_ROOTFS)/usr/bin
	cp -f vdsl $(TMP_ROOTFS)/usr/bin
	mkdir -p $(TMP_ROOTFS)/etc
	cp -f vdsl.conf $(TMP_ROOTFS)/etc
	$(STRIP) $(TMP_ROOTFS)/usr/sbin/vdsld  $(TMP_ROOTFS)/usr/bin/vdsl	
clean:
	@rm -rf *.o vdsld vdsl
