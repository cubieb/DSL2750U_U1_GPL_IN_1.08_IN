#Makfile to compile CONFIGURATION  dependent files.


CONF_OBJ += $(IPVOICE_DRV_OBJ_DIR)/voicelib.o

$(IPVOICE_DRV_OBJ_DIR)/%.o	:%.c
	$(IPVOICE_CC) $(VOICE_FLAGS) -c $< -o $@

all: $(CONF_OBJ)

clean:
	\rm -f $(CONF_OBJ)
