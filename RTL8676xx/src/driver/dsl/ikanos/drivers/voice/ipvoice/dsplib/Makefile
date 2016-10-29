#Makfile to compile dsplib drivers.


DSPLIB_OBJ :=\
	$(IPVOICE_DRV_OBJ_DIR)/JitMain.o\
	$(IPVOICE_DRV_OBJ_DIR)/AJBCli.o\
	$(IPVOICE_DRV_OBJ_DIR)/AJBDelayEst.o\
	$(IPVOICE_DRV_OBJ_DIR)/AJBStat.o\
	$(IPVOICE_DRV_OBJ_DIR)/CidGeneration.o\
	$(IPVOICE_DRV_OBJ_DIR)/ToneGen.o\
	$(IPVOICE_DRV_OBJ_DIR)/ToneGenApi.o\
	$(IPVOICE_DRV_OBJ_DIR)/SinGen.o\
	$(IPVOICE_DRV_OBJ_DIR)/HDtmfrxDet.o\
	$(IPVOICE_DRV_OBJ_DIR)/HDtmfrxVar.o\
	$(IPVOICE_DRV_OBJ_DIR)/G711PLCMain.o\
	$(IPVOICE_DRV_OBJ_DIR)/G711PLC_AddHist.o\
	$(IPVOICE_DRV_OBJ_DIR)/G711PLC_Pitch.o\
	$(IPVOICE_DRV_OBJ_DIR)/G711PlcLib.o\
	$(IPVOICE_DRV_OBJ_DIR)/G711Table.o\
	$(IPVOICE_DRV_OBJ_DIR)/G711plc.o\
	$(IPVOICE_DRV_OBJ_DIR)/ToneFourCad.o\
	$(IPVOICE_DRV_OBJ_DIR)/G711PLCBasic_op.o\
        $(IPVOICE_DRV_OBJ_DIR)/CidRxMain.o\
        $(IPVOICE_DRV_OBJ_DIR)/rc8096.o\
	$(IPVOICE_DRV_OBJ_DIR)/clprgDet.o
	


$(IPVOICE_DRV_OBJ_DIR)/%.o	:%.c
	$(IPVOICE_CC) $(VOICE_FLAGS) -c $< -o $@

all: $(DSPLIB_OBJ)

clean:
	\rm -f $(DSPLIB_OBJ)
