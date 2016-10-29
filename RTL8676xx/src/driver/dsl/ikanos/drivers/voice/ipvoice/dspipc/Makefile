#Makfile to compile dspipc drivers.


DSPIPC_OBJ=\
	$(IPVOICE_DRV_OBJ_DIR)/dspinit.o\
	$(IPVOICE_DRV_OBJ_DIR)/voiceopt.o\
	$(IPVOICE_DRV_OBJ_DIR)/dspipc.o

$(IPVOICE_DRV_OBJ_DIR)/%.o	:%.c
	$(IPVOICE_CC) $(VOICE_FLAGS) -I./include -I./$(SLIC_VENDOR)/include -c $< -o $@

all: $(DSPIPC_OBJ)
clean:
	\rm -f $(DSPIPC_OBJ)
