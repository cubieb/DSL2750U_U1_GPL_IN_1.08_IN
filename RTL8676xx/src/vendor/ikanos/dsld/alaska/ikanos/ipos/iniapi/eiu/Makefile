#COMPILER_PATH   = /opt/kenati/bin/
#CC              = $(COMPILER_PATH)/lx4189-uclibc-gcc
#COMPILER_PATH	= /opt/montavista/pro/devkit/arm/xscale_be/bin/
#CC		= $(COMPILER_PATH)/xscale_be-gcc
PROJ_DIR 	= .
IKANOS_DIR	= ../../../../ikanos
OBJ_DIR		= $(IKANOS_DIR)/obj

SRC	=       HostPtmfUtils.c

OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))

PROJ_INC	= \
	-I$(IKANOS_DIR)/ipos/include/osshim \
	-I$(IKANOS_DIR)/ipos/include/api \
	-I$(IKANOS_DIR)/ipos/include/dapi \
	-I$(IKANOS_DIR)/include \
        -I$(IKANOS_DIR)/apps \
	-I$(IKANOS_DIR)/apps/rfm \
	-I$(IKANOS_DIR)/apps/elm \
	-I$(IKANOS_DIR)/system \
	-I$(IKANOS_DIR)/co_diags/include \
	-I$(IKANOS_DIR)/vxworklib 

#CFLAGS	= -Wall -O2 -I. $(PROJ_INC)  -DVTU_R -DCPE_1P_BOARD -DUSE_RFM -DBME_CPE_20 -DINI_SUPPORT -DMII_MODE -DCPE_30 -DCPE_40 -DCPE_50 -DIK_PRINTF=printf -DPRODUCTION_CODE -DBUP_DIAGS 
CFLAGS	= -Wall -O2 -I. $(PROJ_INC)  -DVTU_R -DCPE_1P_BOARD -DUSE_RFM -DBME_CPE_20 -DINI_SUPPORT -DMII_MODE -DCPE_30 -DCPE_40 -DCPE_50 -DIK_PRINTF=printf -DPRODUCTION_CODE
all: 	$(OBJ)

$(OBJ_DIR)/%.o: $(PROJ_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean :
	@echo clean objects and binary files
	rm -f $(OBJ_DIR)/*.o 
