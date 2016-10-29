#export CC              = $(CC_COMPILER)

ZLIB_DEPS = ../../../../buildroot/build_mips_nofpu/zlib-1.2.3/libz.so
ZLIB_HEADERS = -I../../../../buildroot/build_mips_nofpu/zlib-1.2.3

export CC              = mips-linux-uclibc-gcc
IKANOS_DIR	= ikanos

CFLAGS		= -Wall -O2 -I. $(PROJ_INC)  -DVTU_R -DCPE_1P_BOARD -DUSE_RFM -DBME_CPE_20 -DINI_SUPPORT -DMII_MODE -DCPE_30 -DCPE_40 -DCPE_50 -DIK_PRINTF=printf -DPRODUCTION_CODE -DALASKA
MFLAGS          = -C

OBJ_DIR		= $(IKANOS_DIR)/obj

APPS_DIR        = ${IKANOS_DIR}/apps
RFM_DIR         = ${APPS_DIR}/rfm
ELM_DIR         = ${APPS_DIR}/elm
BU_DIAGS_DIR    = ${IKANOS_DIR}/bu_diags
INCLUDE_DIR     = ${IKSNOS_DIR}/include
IPOS_DIR        = ${IKANOS_DIR}/ipos
BMEAPI_DIR      = ${IPOS_DIR}/bmeapi
DAPI_DIR        = ${BMEAPI_DIR}/dapi
#HIC2_DIR        = ${DAPI_DIR}/hic2
BMEDEBUG_DIR    = ${IPOS_DIR}/bmedebug
INIAPI_DIR      = ${IPOS_DIR}/iniapi
EIU_DIR         = ${INIAPI_DIR}/eiu
SYSTEM_DIR      = ${IKANOS_DIR}/system
UTIL_DIR        = ${IKANOS_DIR}/util
VXWORKLIB_DIR   = ${IKANOS_DIR}/vxworklib

#OBJ_DIR         = $(IKANOS_DIR)/obj

PHONY: all

all: apps bu_diags ipos system util vxworklib vdsld vdsl

vdsld:
	$(CC) $(ZLIB_HEADERS) $(CFLAGS) -lpthread -lm $(ZLIB_DEPS) -o vdsld $(OBJ_DIR)/*.o $(ZLIB_DEPS)
	cp ikanos/ipos/bmeapi/dapi/cpe50Code.cbin ../../../../buildroot/build_mips_nofpu/root/bin/.
vdsl:
	$(CC) $(CFLAGS) -o vdsl vdsl_client.c

apps:
	mkdir -p $(OBJ_DIR)
	make ${MFLAGS} ${APPS_DIR} 
	make ${MFLAGS} ${RFM_DIR}
	make ${MFLAGS} ${ELM_DIR}

bu_diags:
	make ${MFLAGS} ${BU_DIAGS_DIR}

ipos:
	make ${MFLAGS} ${BMEAPI_DIR}
	make ${MFLAGS} ${DAPI_DIR}
#	make ${MFLAGS} ${HIC2_DIR}
	make ${MFLAGS} ${BMEDEBUG_DIR}
	make ${MFLAGS} ${INIAPI_DIR}
	make ${MFLAGS} ${EIU_DIR}
                                                                     
system:
	make ${MFLAGS} ${SYSTEM_DIR}

util:
	make ${MFLAGS} ${UTIL_DIR}

vxworklib:
	make ${MFLAGS} ${VXWORKLIB_DIR}

romfs:
	$(ROMFSINST) /bin/vdsld
	$(ROMFSINST) /bin/vdsl
clean:
#just to clear the screen
		clear
		@echo
		@echo ---------------------------------------
		@echo "        Clean Objects and Binary     "
		@echo ---------------------------------------
		@rm -rf $(IKANOS_DIR)/obj/*.o clean       
		@rm -f vdsld vdsl vdsl_client.o
