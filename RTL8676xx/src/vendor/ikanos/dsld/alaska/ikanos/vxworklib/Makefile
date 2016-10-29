#COMPILER_PATH   = /opt/kenati/bin/
#CC              = $(COMPILER_PATH)/lx4189-uclibc-gcc
#COMPILER_PATH	= /opt/montavista/pro/devkit/arm/xscale_be/bin
#CC		= $(COMPILER_PATH)/xscale_be-gcc
PROJ_DIR 	= ./
IKANOS_DIR	= ../../ikanos
OBJ_DIR		= $(IKANOS_DIR)/obj

OBJS     	= msgQLib.o semLib.o kernelLib.o taskLib.o wdLib.o 

#CFLAGS	= -Wall -O2 -DCPE_30 -DVTU_R -DIKANOS_CHANGES -DCPE_50 -DIK_PRINTF=printf -DALASKA -DBUP_DIAGS
CFLAGS	= -Wall -O2 -DCPE_30 -DVTU_R -DIKANOS_CHANGES -DCPE_50 -DIK_PRINTF=printf -DALASKA

all: 	$(OBJS)

$(OBJS) :
	$(CC) $(CFLAGS) $(VDSL_FLAGS) -c -o $(OBJ_DIR)/$@ $*.c


clean :
	rm -f $(OBJ_DIR)/*.o 
