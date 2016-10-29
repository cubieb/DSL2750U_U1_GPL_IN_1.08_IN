#Makfile to compile SLIC drivers.

SLIC_OBJ := $(IPVOICE_DRV_OBJ_DIR)/si3216.o
SLIC_OBJ += $(IPVOICE_DRV_OBJ_DIR)/si3050.o
SLIC_OBJ += $(IPVOICE_DRV_OBJ_DIR)/legerityrw.o
SLIC_OBJ += $(IPVOICE_DRV_OBJ_DIR)/legerity78d11.o
SLIC_OBJ += $(IPVOICE_DRV_OBJ_DIR)/legerity88111.o
SLIC_OBJ += $(IPVOICE_DRV_OBJ_DIR)/legerity88221.o
SLIC_OBJ += $(IPVOICE_DRV_OBJ_DIR)/voicespi.o
SLIC_OBJ += $(IPVOICE_DRV_OBJ_DIR)/slicapi.o

$(IPVOICE_DRV_OBJ_DIR)/%.o  :silabs/%.c
	$(IPVOICE_CC) $(VOICE_FLAGS) -I$(PWD)/silabs/include -c $< -o $@

$(IPVOICE_DRV_OBJ_DIR)/%.o  :legerity/%.c
	$(IPVOICE_CC) $(VOICE_FLAGS) -I./silabs/include -c $< -o $@

$(IPVOICE_DRV_OBJ_DIR)/%.o  :%.c
	$(IPVOICE_CC) $(VOICE_FLAGS) -c $< -o $@


all:	$(SLIC_OBJ)

clean:
	\rm -f $(SLIC_OBJ)
