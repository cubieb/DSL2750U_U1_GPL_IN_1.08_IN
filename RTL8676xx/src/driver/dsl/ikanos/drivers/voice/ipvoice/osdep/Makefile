#Makfile to compile OS dependent files.


OS_OBJ := $(IPVOICE_DRV_OBJ_DIR)/oswrapper.o\
	$(IPVOICE_DRV_OBJ_DIR)/dspdrv.o

$(IPVOICE_DRV_OBJ_DIR)/%.o	:%.c
	$(IPVOICE_CC) $(VOICE_FLAGS) -c $< -o $@


all: $(OS_OBJ)

clean:
	\rm -f $(OS_OBJ)
