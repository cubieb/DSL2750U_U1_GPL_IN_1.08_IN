/*
** Copyright ?2007 by Silicon Laboratories
**
**
** SI3226_Intf.c
** SI3226 ProSLIC interface implementation file
**
** Author(s): 
** laj
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This is the implementation file for the main ProSLIC API and is used 
** in the ProSLIC demonstration code. 
**
** Dependancies:
** proslic_Gcispi.c
**
*/

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include "proslic_datatypes.h"
#include "proslic_ctrl.h"
#include "proslic_timer_intf.h"
#include "voip_types.h"
#include "proslic.h"
#include "si3226_intf.h"
#include "si3226.h"
#include "si3226_registers.h"
#include "si3226_constants.c"
#include "rtk_voip.h"
#include "Slic_api.h"

#define PRAM_ADDR (334 + 0x400)
#define PRAM_DATA (335 + 0x400)

#define WriteReg	pProslic->deviceId->ctrlInterface->WriteRegister_fptr
#define ReadReg		pProslic->deviceId->ctrlInterface->ReadRegister_fptr
#define pProHW		pProslic->deviceId->ctrlInterface->hCtrl
#define Reset		pProslic->deviceId->ctrlInterface->Reset_fptr
#define Delay		pProslic->deviceId->ctrlInterface->Delay_fptr
#define pProTimer	pProslic->deviceId->ctrlInterface->hTimer
#define WriteRAM	pProslic->deviceId->ctrlInterface->WriteRAM_fptr
#define ReadRAM		pProslic->deviceId->ctrlInterface->ReadRAM_fptr
#define TimeElapsed pProslic->deviceId->ctrlInterface->timeElapsed_fptr
#define SetSemaphore pProslic->deviceId->ctrlInterface->Semaphore_fptr

#define WriteRegX		deviceId->ctrlInterface->WriteRegister_fptr
#define ReadRegX		deviceId->ctrlInterface->ReadRegister_fptr
#define pProHWX			deviceId->ctrlInterface->hCtrl
#define DelayX			deviceId->ctrlInterface->Delay_fptr
#define pProTimerX		deviceId->ctrlInterface->hTimer
#define WriteRAMX		deviceId->ctrlInterface->WriteRAM_fptr
#define ReadRAMX		deviceId->ctrlInterface->ReadRAM_fptr
#define TimeElapsedX	deviceId->ctrlInterface->timeElapsed_fptr

#define BROADCAST 0xff


/*
** Local functions are defined first
*/

/*
** Function: setUserMode
**
** Description: 
** Puts ProSLIC into user mode or out of user mode
**
** Input Parameters: 
** pProslic: pointer to PROSLIC object
** on: specifies whether user mode should be turned on (TRUE) or off (FALSE)
**
** Return:
** none
*/
static int setUserMode (proslicChanType *pProslic,BOOLEAN on){
	uInt8 data;
	if (SetSemaphore != NULL){
		while (!(SetSemaphore (pProHW,1)));
		if (on == TRUE){
			if (pProslic->deviceId->usermodeStatus<2)
				pProslic->deviceId->usermodeStatus++;
		} else {
			if (pProslic->deviceId->usermodeStatus>0)
				pProslic->deviceId->usermodeStatus--;
			if (pProslic->deviceId->usermodeStatus != 0)
				return -1;
		}
	}
	data = ReadReg(pProHW,pProslic->channel,126);
	if (((data&1) != 0) == on)
		return 0;
	WriteReg(pProHW,pProslic->channel,126,2);
	WriteReg(pProHW,pProslic->channel,126,8);
	WriteReg(pProHW,pProslic->channel,126,0xe);
	WriteReg(pProHW,pProslic->channel,126,0);
	if (SetSemaphore != NULL)
		SetSemaphore(pProHW,0);
	return 0;
}

static int setUserModeBroadcast (proslicChanType *pProslic,BOOLEAN on){
	uInt8 data;
	if (SetSemaphore != NULL){
		while (!(SetSemaphore (pProHW,1)));
		if (on == TRUE){
			if (pProslic->deviceId->usermodeStatus<2)
				pProslic->deviceId->usermodeStatus++;
		} else {
			if (pProslic->deviceId->usermodeStatus>0)
				pProslic->deviceId->usermodeStatus--;
			if (pProslic->deviceId->usermodeStatus != 0)
				return -1;
		}
	}
	data = ReadReg(pProHW,pProslic->channel,126);/*we check first channel. we assume all channels same user mode state*/
	if (((data&1) != 0) == on)
		return 0;
	WriteReg(pProHW,BROADCAST,126,2);
	WriteReg(pProHW,BROADCAST,126,8);
	WriteReg(pProHW,BROADCAST,126,0xe);
	WriteReg(pProHW,BROADCAST,126,0);
	if (SetSemaphore != NULL)
		SetSemaphore(pProHW,0);
	return 0;
}

static int probeDaisyChain (proslicChanType *pProslic){
	int i=0;
	WriteReg(pProHW,BROADCAST,RAM_ADDR_HI,0x40);
	while (ReadReg(pProHW,i++,RAM_ADDR_HI) == 0x40 && (i<=32));
	WriteReg(pProHW,BROADCAST,RAM_ADDR_HI,0x0);
	return i-1;
}

static int cal_iteration (proslicChanType *pProslic,int iter,uInt8 *data2,uInt8 numChan){
	int i;
	uInt8 data,data3;
	int millisecs=0;
	for (i=0;i<numChan;i+=2){ /*we do each channel in each chip*/
		WriteReg(pProHW,i+iter,CALR0,data2[0]);
		WriteReg(pProHW,i+iter,CALR1,data2[1]);
		WriteReg(pProHW,i+iter,CALR2,data2[2]);
		WriteReg(pProHW,i+iter,CALR3,data2[3]);
	}
	do{
		data3=0;
		for (i=0;i<numChan;i+=2){
			data = ReadReg(pProHW,i+iter,CALR3);
			data3 |= data;
		}
		Delay(pProTimer,1);
		if (millisecs++ > 3000){
			if (pProslic->debugMode)
				LOGPRINT ("Calibration timeout channel %d\n",iter);
		}
	}while ((data3&0x80)&&(millisecs<3000));
	return data3;
}

static int calibrate (proslicChanType *pProslic, int broadcast){
	/*
	** This function will perform the ProSLIC calibration sequence 
	*/
	int i,j,k;
	uInt8 data [] = {0x0, 0x0, 0x1, 0x80};/*madc cal is done before other cals*/
	uInt8 data3=0;
	uInt8 numChan=0;
	ramData vbat,vbat_min=0,vbath;
	if (broadcast){
		/*# fix coefficient for zcal*/
		WriteRAM(pProHW,BROADCAST,746,0x8F00000L);
		WriteRAM(pProHW,BROADCAST,927,0x1FFE0000L);
		setUserModeBroadcast(pProslic,TRUE);
		WriteRAM(pProHW,BROADCAST,1537,0x3200000L); /*fix lkg stndby offset for oht*/
		setUserModeBroadcast(pProslic,FALSE);
		numChan = probeDaisyChain(pProslic);
		for (i=0;i<2;i++){
			data3 |= cal_iteration(pProslic,i,data,numChan);
		}
	}
	else {/*to do - add parallel cals on separate devices*/
		/*# fix coefficient for zcal*/
		WriteRAM(pProHW,pProslic->channel,746,0x8F00000L);
		WriteRAM(pProHW,pProslic->channel,927,0x1FFE0000L);
		setUserMode(pProslic,TRUE);
		WriteRAM(pProHW,pProslic->channel,1537,0x3200000L); /*fix lkg stndby offset for oht*/
		setUserMode(pProslic,FALSE);
		numChan = 1;
		i=0;
		WriteReg(pProHW,pProslic->channel,CALR0,data[0]);
		WriteReg(pProHW,pProslic->channel,CALR1,data[1]);
		WriteReg(pProHW,pProslic->channel,CALR2,data[2]);
		WriteReg(pProHW,pProslic->channel,CALR3,data[3]);
		do{
			data3 = ReadReg(pProHW,pProslic->channel,CALR3);
			Delay(pProTimer,1);

			if (i++ > 3000){
				if (pProslic->debugMode)
					LOGPRINT ("Calibration timeout channel %d\n",pProslic->channel);
				pProslic->error = CAL_TIMEOUT;
				pProslic->channelEnable = 0;
			
			}
		} while (i<3000 && (data3&0x80));
	}
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("MADC cal done. channel %d\n",pProslic->channel);
#endif

	/*run the rest of the cals*/
	data[1] = 0xbf; /*no zcal*/
	data[2] = 0xf8;
	data[3] = 0x80;
	if (broadcast){
		
		for (i=0;i<2;i++){
			/*start up convertor*/
			
			setUserModeBroadcast(pProslic,TRUE);
			//WriteReg(pProHW,BROADCAST,80,0x3e);
			WriteRAM(pProHW,BROADCAST,1538, 0x600000L);
			Delay(pProTimer,100);
			WriteRAM(pProHW,BROADCAST,1538, 0);
			setUserModeBroadcast(pProslic,FALSE);
			
			k=0;
			do {
				vbat_min = 0x7FFFFFFFL;
				vbath = ReadRAM(pProHW,i,VBATH_EXPECT);
				for (j=0;j<numChan;j+=2){
					vbat = ReadRAM(pProHW,j+i,MADC_VBAT);
					if (vbat < vbat_min)
						vbat_min = vbat;
				}
				Delay(pProTimer,10);
				if (k > 200){
#ifdef ENABLE_DEBUG
					LOGPRINT ("Si3226 DC-DC Power up timeout\n");
#endif
					return TIMEOUTPOWERUP;
				}
			} while ((vbat_min < (vbath-0x51EB82L)) && (k++ < 200)); /*2 second timeout*/
			data3 |= cal_iteration(pProslic,i,data,numChan);
		}
	}
	else {
		
		/*converter startup sequence*/
		setUserMode(pProslic,TRUE);
		//WriteReg(pProHW,pProslic->channel,80,0x3e);
		WriteRAM(pProHW,pProslic->channel,1538, 0x600000L);
		Delay(pProTimer,100);
		WriteRAM(pProHW,pProslic->channel,1538, 0);
		setUserMode(pProslic,FALSE);
		
		k=0;
		do {
				vbath = ReadRAM(pProHW,pProslic->channel,VBATH_EXPECT);
				vbat = ReadRAM(pProHW,pProslic->channel,MADC_VBAT);			
				Delay(pProTimer,10);
				if (k > 200){
#ifdef ENABLE_DEBUG
					LOGPRINT ("Si3226 DC-DC Power up timeout\n");
#endif
					return TIMEOUTPOWERUP;
				}
		} while ((vbat < (vbath-0x51EB82L)) && (k++ < 200)); /*2 second timeout, 5V margin*/
		i=0;
		WriteReg(pProHW,pProslic->channel,CALR0,data[0]);
		WriteReg(pProHW,pProslic->channel,CALR1,data[1]);
		WriteReg(pProHW,pProslic->channel,CALR2,data[2]);
		WriteReg(pProHW,pProslic->channel,CALR3,data[3]);
		do{
			data3 = ReadReg(pProHW,pProslic->channel,CALR3);
			Delay(pProTimer,100);

			if (i++ > 30){
				if (pProslic->debugMode)
					LOGPRINT ("Calibration timeout channel %d\n",pProslic->channel);
				pProslic->error = CAL_TIMEOUT;
				pProslic->channelEnable = 0;
			
			}
		} while (i<30 && (data3&0x80));
	}
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Calibration done. channel %d\n",pProslic->channel);
#endif
	return data3;
}

static int LoadRegTables (proslicChanType *pProslic, ProslicRAMInit *pRamTable, ProslicRegInit *pRegTable, int broadcast){
	uInt16 i;
	uInt8 channel;
	if (broadcast){
		channel = BROADCAST;
		setUserModeBroadcast(pProslic,TRUE);
	}
	else {
		channel = pProslic->channel;
		setUserMode(pProslic,TRUE);
	}

	i=0; 
	if (pRamTable != 0){
		while (pRamTable[i].address != 0xffff){
			WriteRAM(pProHW,channel,pRamTable[i].address,pRamTable[i].initValue); 
			i++;
		}
	}
	i=0;
	if (pRegTable != 0){
		while (pRegTable[i].address != 0xff){
			WriteReg(pProHW,channel,pRegTable[i].address,pRegTable[i].initValue);
			i++;
		}
	}
	if (broadcast)
		setUserModeBroadcast(pProslic,FALSE);
	else
		setUserMode(pProslic,FALSE);

	return 0;
}

#ifdef DISABLE_LOOPMAN
#else
int enableSi3226Loopman (proslicChanType *pProslic, int broadcast){
	uInt8 channel;
	if (broadcast){
		setUserModeBroadcast(pProslic,TRUE);
		channel = BROADCAST;
	}
	else{
		setUserMode (pProslic,TRUE); /*make sure we are in user mode */
		channel = pProslic->channel;
	}

	WriteRAM(pProHW, channel,1445, 0);
	WriteRAM(pProHW, channel,1451, 0x200000L);
	WriteRAM(pProHW, channel,1445, 0x300000L);
	WriteReg(pProHW, channel,98,0x80); /*enable MADC to prevent power alarm. this could also be done before going active*/
	//PRINT_R("Enable Si3226 Loopman.\n");
	
	if (broadcast){
		setUserModeBroadcast(pProslic,FALSE);
	}
	else {
		setUserMode(pProslic,FALSE); /*turn off user mode*/
	}
	return 0;
}
int disableSi3226Loopman (proslicChanType *pProslic, int broadcast){
	uInt8 channel;
	if (broadcast){
		setUserModeBroadcast(pProslic,TRUE);
		channel = BROADCAST;
	}
	else{
		setUserMode (pProslic,TRUE); /*make sure we are in user mode */
		channel = pProslic->channel;
	}

	//WriteRAM(pProHW, channel,1445, 0x0);
	//WriteReg(pProHW, channel,98,0x00); /*enable MADC to prevent power alarm. this could also be done before going active*/
	if (broadcast){
		setUserModeBroadcast(pProslic,FALSE);
	}
	else {
		setUserMode(pProslic,FALSE); /*turn off user mode*/
	}
	return 0;
}
#endif

static int LoadSi3226Patch (proslicChanType *pProslic, const proslicPatch *pPatch,int broadcast)
{ 
	int32 loop;
	uInt8 jmp_table=82;
	uInt8 channel;
	if (pPatch == NULL)
	{
		PRINT_R("Si3226 Load Patch Fail.\n");
		return 0;
	}
	
	if (broadcast)
	{
		setUserModeBroadcast(pProslic,TRUE);
		channel = BROADCAST;
	}
	else
	{
		setUserMode (pProslic,TRUE); /*make sure we are in user mode to load patch*/
		channel = pProslic->channel;
	}

	WriteReg (pProHW, channel, JMPEN,0); /*disable Patch RAM*/

	for (loop=0;loop<8;loop++)
	{
		/*zero out the jump table*/
		WriteReg (pProHW, channel, jmp_table,0);
		WriteReg (pProHW, channel, jmp_table+1,0);
	
		jmp_table+=2;
	}

	WriteRAM(pProHW, channel,PRAM_ADDR, 0); /*write patch ram address register
	// If the data is all 0, you have hit the end of the programmed values and can stop loading.*/
    	for (loop=0; loop<1024; loop++)
    	{
		if (pPatch->patchData[loop] != 0)
		{
			if ((pProslic->deviceId->chipRev < 3) && broadcast)
				WriteRAM(pProHW, channel,PRAM_ADDR, loop<<19); /*write patch ram address register (only necessary for broadcast rev c and earlier)*/
            		WriteRAM(pProHW, channel,PRAM_DATA,pPatch->patchData[loop]<<9); /*loading patch, note. data is shifted*/
		}
        else
		loop = 1024;
    	}

    	/*zero out RAM_ADDR_HI*/
	WriteReg (pProHW, channel, RAM_ADDR_HI,0);

	jmp_table=82;
	for (loop=0;loop<8;loop++)
	{
		/* Load the jump table with the new values.*/
		if (pPatch->patchEntries[loop] != 0)
		{
			WriteReg (pProHW, channel, jmp_table,(pPatch->patchEntries[loop])&0xff);
			WriteReg (pProHW, channel, jmp_table+1,pPatch->patchEntries[loop]>>8);
		}
		jmp_table+=2;
	}

	WriteRAM(pProHW,channel,448,pPatch->patchSerial); /*write patch identifier*/
	
#ifdef DISABLE_VERIFY_PATCH
	WriteReg (pProHW, channel, JMPEN,1); /*enable the patch (do not enable if you want to verify)*/
#endif
	//PRINT_R("Si3226 Load Patch OK.\n");
	
	if (broadcast)
	{
		setUserModeBroadcast(pProslic,FALSE);
	}
	else
	{
		setUserMode(pProslic,FALSE); /*turn off user mode*/
	}
	return 0;
}

/*
** Function: handleError
**
** Description: 
** Called whenever an error is encountered with the proslic
**
** Input Parameters: 
** pProslic: pointer to PROSLIC object
** fault: error code
**
** Return:
** none
*/
static void handleError (proslicChanType *pProslic, errorCodeType fault){
//#if (PRINTF_IS_OK)
	PRINT_R ("Error encountered\n\n");
//#endif
	/*TODO: add something to recover from power alarm here?*/
}


/*
** Functions below are defined in header file and can be called by external files
*/

/*
**
** PROSLIC INITIALIZATION FUNCTIONS
**
*/

/*
** Function: PROSLIC_Reset
**
** Description: 
** Resets the ProSLIC
*/
int Si3226_Reset (proslicChanType_ptr pProslic){
	/*
	** resets ProSLIC, wait 250ms, release reset, wait 250ms
	*/
	Reset(pProHW,1);
	Delay(pProTimer,250);
	Reset(pProHW,0);
	Delay(pProTimer,250);
	return 0;
}


/*
** Function: PROSLIC_Init
**
** Description: 
** Initializes the ProSLIC
*/



int Si3226_VerifyControlInterface (proslicChanType_ptr pProslic)
{
	int i;
	int numOfChan = probeDaisyChain(pProslic);
	if (numOfChan == 0)
		return SPIFAIL;
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT ("Found %d channels\n",numOfChan);
#endif
	WriteReg(pProHW,BROADCAST,PCMRXLO,0x5a);
	WriteRAM(pProHW,BROADCAST,448,0x12345678L);

	for (i=0;i<numOfChan;i++){
		/*Try to write innocuous register to test SPI is working*/
		
		if (ReadReg(pProHW,i,PCMRXLO) != 0x5A){
			handleError(pProslic,SPIFAIL);
#ifdef ENABLE_DEBUG
			if (pProslic->debugMode)
				LOGPRINT("Proslic %d not communicating. Register access fail.\n",i);
#endif
			return SPIFAIL;
		}	
		if (ReadRAM(pProHW,i,448) != 0x12345678L){
			handleError(pProslic,SPIFAIL);
#ifdef ENABLE_DEBUG
			if (pProslic->debugMode)
				LOGPRINT("Proslic %d not communicating. RAM access fail.\n",i);
#endif
			return SPIFAIL;
		}

	}
	return 0;
}
#include "si3226_patch.c"

extern const proslicPatch RevCPatch;
extern const proslicPatch RevDPatch;
extern Si3226_General_Cfg Si3226_General_Configuration;
int Si3226_Init (proslicChanType_ptr *pProslic, int size){
	/*
	** This function will initialize the chipRev and chipType members in pProslic
	** as well as load the initialization structures.
	*/
	uInt8 data; 
	int k;

	for (k=0;k<size;k++){
		data = pProslic[k]->ReadRegX(pProslic[k]->pProHWX,pProslic[k]->channel,ID);
		pProslic[k]->deviceId->chipRev = data&0x7;
		pProslic[k]->deviceId->chipType= ((data&0x31)>>3) + SI3226;
printk("%s-%d: data=%X (%d,%d)\n", __FUNCTION__, __LINE__, data, 
			pProslic[k]->deviceId->chipRev,
			pProslic[k]->deviceId->chipType);
	}
	for (k=0;k<size;k++){
		pProslic[k]->WriteRegX(pProslic[k]->pProHWX,pProslic[k]->channel,PCMRXLO,0x5a);
		if (pProslic[k]->ReadRegX(pProslic[k]->pProHWX,pProslic[k]->channel,PCMRXLO) != 0x5A){
			pProslic[k]->channelEnable = 0;
			pProslic[k]->error = SPIFAIL;
printk("%s-%d: FAILED!\n", __FUNCTION__, __LINE__);
		}
	}
	
	for (k=0;k<size;k++){
		if (pProslic[k]->channelEnable){
			if (pProslic[k]->deviceId->chipRev == 2 ) {/*rev c*/
				Si3226_LoadPatch(pProslic[k],&RevCPatch);
#ifdef DISABLE_VERIFY_PATCH
#else
				data = Si3226_VerifyPatch(pProslic[k],&RevCPatch);
				if (data){
					pProslic[k]->channelEnable=0;
					pProslic[k]->error = PATCH_ERR;
				} else {
					setUserMode(pProslic[k],TRUE);
					pProslic[k]->WriteRegX (pProslic[k]->pProHWX, pProslic[k]->channel,JMPEN,1);  
					setUserMode(pProslic[k],FALSE);
				}
#endif				

			}
			if (pProslic[k]->deviceId->chipRev == 3 ) {/*rev d*/
				Si3226_LoadPatch(pProslic[k],&RevDPatch);
#ifdef DISABLE_VERIFY_PATCH
#else
				data = Si3226_VerifyPatch(pProslic[k],&RevDPatch);
				if (data){
					pProslic[k]->channelEnable=0;
					pProslic[k]->error = PATCH_ERR;	
				} else {
					setUserMode(pProslic[k],TRUE);
					pProslic[k]->WriteRegX (pProslic[k]->pProHWX, pProslic[k]->channel,JMPEN,1);  
					setUserMode(pProslic[k],FALSE);
				}
#endif

			}
		}

	}
	/*Set up DC-DC converter*/
		for (k=0;k<size;k++){ 
			setUserMode(pProslic[k],TRUE);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_VREF_MIN,Si3226_General_Configuration.dcdc_vref_min);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,VBATR_EXPECT,Si3226_General_Configuration.vbatr_expect);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_FSW_NORM,Si3226_General_Configuration.dcdc_fsw_norm);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_FSW_NORM_LO,Si3226_General_Configuration.dcdc_fsw_norm_lo);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_DIN_LIM,Si3226_General_Configuration.dcdc_din_lim);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_FSW_RING,Si3226_General_Configuration.dcdc_fsw_ring);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_FSW_RING_LO,Si3226_General_Configuration.dcdc_fsw_ring_lo);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_OITHRESH,Si3226_General_Configuration.dcdc_oithresh);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_OVTHRESH,Si3226_General_Configuration.dcdc_ovthresh);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_UVHYST,Si3226_General_Configuration.dcdc_uvhyst);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_UVTHRESH,Si3226_General_Configuration.dcdc_uvthresh);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_VOUT_LIM,Si3226_General_Configuration.dcdc_vout_lim);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_FSW_VHYST,0);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_FSW_VTHLO,0);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_SWDRV_POL,0x100000L);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_SWFET,0x300000L);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_VREF_CTRL,0x600000L);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_RNGTYPE,0x200000L);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,COEF_P_HVIC,Si3226_General_Configuration.coef_p_hvic);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,P_TH_HVIC,Si3226_General_Configuration.p_th_hvic);
		pProslic[k]->WriteRegX(pProslic[k]->pProHWX, pProslic[k]->channel,AUTO,Si3226_General_Configuration.autoRegister);
		pProslic[k]->WriteRegX(pProslic[k]->pProHWX, pProslic[k]->channel,CM_CLAMP,Si3226_General_Configuration.cm_clamp);
		pProslic[k]->WriteRegX(pProslic[k]->pProHWX, pProslic[k]->channel,GPIO_CFG1,0xF0);/*coarse sensors (analog mode)*/
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel, 489, 0);
		pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel, 490, 0);
		if (pProslic[k]->deviceId->chipRev == 2 || pProslic[k]->deviceId->chipRev == 3) { /*rev c/d*/
			pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_FSW_RING_LO,0x100000L);
			pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_FSW_NORM,0x100000L);
			pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,DCDC_OITHRESH,0x1F00000L);
		}
			setUserMode(pProslic[k],FALSE);
		}

	if (pProslic[0]->deviceId->chipRev == 2 || pProslic[k]->deviceId->chipRev == 3) {
		for (k=0;k<size;k++){ 
			
			pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,1022,0x5400000L);
			pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,1023,0x1000L);
			if (pProslic[k]->deviceId->chipRev == 3){
				
				pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,1023,0x500000L);
				pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,1022,0xC00000L);
				pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,1021,0xA00000L);
				pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,1020,0x1F00000L);
				pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,1019,0x300000L);
				pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,1018,0x200000L);
				pProslic[k]->WriteRAMX(pProslic[k]->pProHWX, pProslic[k]->channel,1017,0x100000L);
		
			}
			
		}

	}

    printk("%s-%d\n", __FUNCTION__, __LINE__);
    Si3226_Cal(pProslic,size);
    printk("%s-%d\n", __FUNCTION__, __LINE__);

	
		for (k=0;k<size;k++){ 
		if (pProslic[k]->deviceId->chipRev == 2) {
			pProslic[k]->WriteRAMX (pProslic[k]->pProHWX, pProslic[k]->channel,753,0x600000L);
			pProslic[k]->WriteRAMX (pProslic[k]->pProHWX, pProslic[k]->channel,896,0x260000L);
		}
	}


	

	

	for (k=0;k<size;k++){ /*chan 0 shut off during chan 1 bring up*/
		if (pProslic[k]->channelEnable){
				setUserMode(pProslic[k],TRUE);
				pProslic[k]->WriteRAMX (pProslic[k]->pProHWX, pProslic[k]->channel,1538,0x600000L);
				pProslic[k]->DelayX(pProslic[k]->pProTimerX,100);
				pProslic[k]->WriteRAMX (pProslic[k]->pProHWX, pProslic[k]->channel,1538,0);
				setUserMode(pProslic[k],FALSE);
				pProslic[k]->DelayX(pProslic[k]->pProTimerX,1900);/*todo: make while loop*/
		}
	}
	for (k=0;k<size;k++){ /*chan 0 shut off during chan 1 bring up*/
		if (pProslic[k]->channelEnable){
#ifdef DISABLE_LOOPMAN
#else
				enableSi3226Loopman(pProslic[k],FALSE);
#endif
		}
	}
	return 0;
}


int Si3226_InitBroadcast (proslicChanType_ptr pProslic){
	/*
	** This function will initialize the chipRev and chipType members in pProslic
	** as well as load the initialization structures.
	*/
	uInt8 data; 
	uInt8 i;
	uInt8 numOfChan;
	proslicChanType pProslicCopy;


	data = ReadReg(pProHW,pProslic->channel,ID);
	pProslic->deviceId->chipRev = data&0x7;
	pProslic->deviceId->chipType= ((data&0x38)>>3) + SI3226;
	numOfChan = probeDaisyChain(pProslic);
	if (numOfChan == 0)
		return 0;
	WriteReg(pProHW,BROADCAST,PCMRXLO,0x5a);
	
	for (i=0;i<numOfChan;i++){
		/*Try to write innocuous register to test SPI is working*/
		
		if (ReadReg(pProHW,i,PCMRXLO) != 0x5A){
			handleError(pProslic,SPIFAIL);
			if (pProslic->debugMode)
				PRINT_R("Proslic %d not communicating\n",i);
			return SPIFAIL;
		}	

	}

	pProslicCopy.channelEnable = 1;
	pProslicCopy.channelType = pProslic->channelType;
	pProslicCopy.debugMode = pProslic->debugMode;
	pProslicCopy.deviceId = pProslic->deviceId;
	

	if (pProslic->deviceId->chipRev == 2 || pProslic->deviceId->chipRev == 3) {
		if (pProslic->deviceId->chipRev == 2)
		LoadSi3226Patch(pProslic,&RevCPatch,1);
		else 
			LoadSi3226Patch(pProslic,&RevDPatch,1);
		for (i=0;i<numOfChan;i+=2){
			pProslicCopy.channel = i;
#ifdef DISABLE_VERIFY_PATCH
#else
			if (pProslic->deviceId->chipRev == 2)
			data = Si3226_VerifyPatch(&pProslicCopy,&RevCPatch);
			else
				data = Si3226_VerifyPatch(&pProslicCopy,&RevDPatch);
			if (data)
			{
				PRINT_R("%s-%d: ERR!\n", __FUNCTION__, __LINE__);	
				return PATCH_ERR;
			}
#endif
		}

		setUserModeBroadcast(pProslic,TRUE);
		WriteReg (pProHW, BROADCAST,JMPEN,1); 	//reg 81
		

		WriteRAM(pProHW, BROADCAST,1023,0x1000L);
		WriteRAM(pProHW, BROADCAST,1022,0x5400000L);

		if (pProslic->deviceId->chipRev == 3) { //rev d
			WriteRAM(pProHW, BROADCAST,1023,0x500000L);
			WriteRAM(pProHW, BROADCAST,1022,0xC00000L);
			WriteRAM(pProHW, BROADCAST,1021,0xA00000L);
			WriteRAM(pProHW, BROADCAST,1020,0x1F00000L);
			WriteRAM(pProHW, BROADCAST,1019,0x300000L);
			WriteRAM(pProHW, BROADCAST,1018,0x200000L);
			WriteRAM(pProHW, BROADCAST,1017,0x100000L);
		}
		

		
		setUserModeBroadcast(pProslic,FALSE);

	}
	setUserModeBroadcast(pProslic,TRUE);
	WriteRAM(pProHW, BROADCAST,VBATR_EXPECT,Si3226_General_Configuration.vbatr_expect);		//ram 768
	WriteRAM(pProHW, BROADCAST,DCDC_VREF_MIN,Si3226_General_Configuration.dcdc_vref_min);		//ram 919
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_NORM,Si3226_General_Configuration.dcdc_fsw_norm);		//ram 921
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_NORM_LO,Si3226_General_Configuration.dcdc_fsw_norm_lo);	//ram 922
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_RING,Si3226_General_Configuration.dcdc_fsw_ring);		//ram 923
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_RING_LO,Si3226_General_Configuration.dcdc_fsw_ring_lo);	//ram 924
	WriteRAM(pProHW, BROADCAST,DCDC_DIN_LIM,Si3226_General_Configuration.dcdc_din_lim);		//ram 925
	WriteRAM(pProHW, BROADCAST,DCDC_VOUT_LIM,Si3226_General_Configuration.dcdc_vout_lim);		//ram 926
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_VHYST,0);	//ram 490
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_VTHLO,0);	//ram 489
	WriteRAM(pProHW, BROADCAST,DCDC_OITHRESH,Si3226_General_Configuration.dcdc_oithresh);		//ram 1548
	WriteRAM(pProHW, BROADCAST,DCDC_OVTHRESH,Si3226_General_Configuration.dcdc_ovthresh);		//ram 1547
	WriteRAM(pProHW, BROADCAST,DCDC_UVHYST,Si3226_General_Configuration.dcdc_uvhyst);		//ram 1545
	WriteRAM(pProHW, BROADCAST,DCDC_UVTHRESH,Si3226_General_Configuration.dcdc_uvthresh);		//ram 1546
	WriteRAM(pProHW, BROADCAST,DCDC_SWDRV_POL,0x100000L);		//ram 1553
	WriteRAM(pProHW, BROADCAST,DCDC_SWFET,0x300000L);		//ram 1555
	WriteRAM(pProHW, BROADCAST,DCDC_VREF_CTRL,0x600000L);		//ram 1558
	WriteRAM(pProHW, BROADCAST,DCDC_RNGTYPE,0x200000L);		//ram 1560
	WriteRAM(pProHW, BROADCAST,COEF_P_HVIC,Si3226_General_Configuration.coef_p_hvic);		//ram 759
	WriteRAM(pProHW, BROADCAST,P_TH_HVIC,Si3226_General_Configuration.p_th_hvic);			//ram 756
	WriteReg(pProHW, BROADCAST,AUTO,Si3226_General_Configuration.autoRegister);			//reg 80
	WriteReg(pProHW, BROADCAST,CM_CLAMP,Si3226_General_Configuration.cm_clamp);			//reg 73
	WriteReg(pProHW, BROADCAST,GPIO_CFG1,0xF0);/*coarse sensors (analog mode)*/			//reg 68
	
	if (pProslic->deviceId->chipRev == 2) { /*rev c*/
			WriteRAM(pProHW, BROADCAST,DCDC_FSW_RING_LO,0x100000L);				//ram 924
			WriteRAM(pProHW, BROADCAST,DCDC_FSW_NORM,0x100000L);				//ram 921
			WriteRAM(pProHW, BROADCAST,DCDC_OITHRESH,0x1F00000L);				//ram 1548
	}

	setUserModeBroadcast(pProslic,FALSE);
	
	data = calibrate(pProslic,1);
	if (data != 0)	
		return CAL_TIMEOUT;

	if (pProslic->deviceId->chipRev == 2 ) {
	WriteRAM(pProHW,BROADCAST,753,0x600000L); 
	WriteRAM(pProHW,BROADCAST,896,0x260000L);
	}


	setUserModeBroadcast(pProslic,TRUE);
	 /*chan 0 shut off during chan 1 bring up*/
		WriteRAM (pProHW, BROADCAST,1538,0x600000L);
		Delay(pProTimer,100);
		WriteRAM (pProHW, BROADCAST,1538,0);
		Delay(pProTimer,1900);
	setUserModeBroadcast(pProslic,FALSE);
	
#ifdef DISABLE_LOOPMAN
#else
	enableSi3226Loopman(pProslic,TRUE);
#endif


	return 0;
}

int Si3226_InitBroadcast_GUI (proslicChanType_ptr pProslic){
	/*
	** This function will initialize the chipRev and chipType members in pProslic
	** as well as load the initialization structures.
	*/
	uInt8 data; 
	uInt8 i;
	uInt8 numOfChan;
	proslicChanType pProslicCopy;


	data = ReadReg(pProHW,pProslic->channel,ID);
	pProslic->deviceId->chipRev = data&0x7;
	pProslic->deviceId->chipType= ((data&0x38)>>3) + SI3226;
	numOfChan = probeDaisyChain(pProslic);
	if (pProslic->debugMode)
		PRINT_MSG("probeDaisyChain, numOfChan=%d\n", numOfChan);
	if (numOfChan == 0)
		return 0;
	WriteReg(pProHW,BROADCAST,PCMRXLO,0x5a);
	
	for (i=0;i<numOfChan;i++){
		/*Try to write innocuous register to test SPI is working*/
		
		if (ReadReg(pProHW,i,PCMRXLO) != 0x5A)
		{
			handleError(pProslic,SPIFAIL);
			PRINT_R("Proslic CH%d not communicating\n",i);
			return SPIFAIL;
		}	

	}

	pProslicCopy.channelEnable = 1;
	pProslicCopy.channelType = pProslic->channelType;
	pProslicCopy.debugMode = pProslic->debugMode;
	pProslicCopy.deviceId = pProslic->deviceId;
	
	if (pProslic->deviceId->chipRev == 2 || pProslic->deviceId->chipRev == 3) {
	
		if (pProslic->debugMode)
			PRINT_MSG("Start load patch, and verify ...");
		
		if (pProslic->deviceId->chipRev == 2)
			LoadSi3226Patch(pProslic,&RevCPatch,1);
		else 
			LoadSi3226Patch(pProslic,&RevDPatch,1);
			
		for (i=0;i<numOfChan;i+=2){
			pProslicCopy.channel = i;
#ifdef DISABLE_VERIFY_PATCH
#else
			if (pProslic->deviceId->chipRev == 2)
				data = Si3226_VerifyPatch(&pProslicCopy,&RevCPatch);
			else
				data = Si3226_VerifyPatch(&pProslicCopy,&RevDPatch);
			if (data)
			{
				PRINT_R("Verify Patch ERR!\n");	
				return PATCH_ERR;
			}
#endif
		}
		
		if (pProslic->debugMode)
			PRINT_MSG("OK\n");

		setUserModeBroadcast(pProslic,TRUE);
		WriteReg (pProHW, BROADCAST,JMPEN,1); 	//reg 81
		

		WriteRAM(pProHW, BROADCAST,1023,0x1000L);
		WriteRAM(pProHW, BROADCAST,1022,0x6000000L); // 94V threshold for dcdc_fsw_ringing

		if (pProslic->deviceId->chipRev == 3) { //rev d
			WriteRAM(pProHW, BROADCAST,1023,0x500000L);
			WriteRAM(pProHW, BROADCAST,1022,0xC00000L);
			WriteRAM(pProHW, BROADCAST,1021,0xA00000L);
			WriteRAM(pProHW, BROADCAST,1020,0x1F00000L);
			WriteRAM(pProHW, BROADCAST,1019,0x300000L);
			WriteRAM(pProHW, BROADCAST,1018,0x200000L);
			WriteRAM(pProHW, BROADCAST,1017,0x100000L);
		}
		

		
		setUserModeBroadcast(pProslic,FALSE);

	}

	WriteRAM(pProHW, BROADCAST,VBATR_EXPECT, 0x0655B005L);		//ram 768
	WriteRAM(pProHW, BROADCAST,DCDC_VREF_MIN,0x01800000L);		//ram 919
	WriteRAM(pProHW, BROADCAST,DCDC_VREF_MIN_RNG,0x04C90000L);	//ram 920
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_NORM, 0x00100000L);		//ram 921
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_NORM_LO, 0x00200000L);	//ram 922
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_RING, 0x00200000L);		//ram 923
	WriteRAM(pProHW, BROADCAST,DCDC_FSW_RING_LO, 0x00100000L);	//ram 924
	WriteRAM(pProHW, BROADCAST,DCDC_DIN_LIM, 0x0D980000L);		//ram 925
	WriteRAM(pProHW, BROADCAST,DCDC_VOUT_LIM, 0x00C00000L);		//ram 926
	
	setUserModeBroadcast(pProslic,TRUE);
	
	WriteRAM(pProHW, BROADCAST,DCDC_UVHYST, 0x00500000L);		//ram 1545
	WriteRAM(pProHW, BROADCAST,DCDC_UVTHRESH, 0x00600000L);		//ram 1546
	WriteRAM(pProHW, BROADCAST,DCDC_OVTHRESH, 0x00600000L);		//ram 1547
	WriteRAM(pProHW, BROADCAST,DCDC_OITHRESH, 0x01300000L);		//ram 1548
	
	WriteRAM(pProHW, BROADCAST,DCDC_SWDRV_POL, 0x100000L);		//ram 1553
	WriteRAM(pProHW, BROADCAST,DCDC_SWFET, 0x300000L);		//ram 1555
	WriteRAM(pProHW, BROADCAST,DCDC_VREF_CTRL, 0x00800000L);	//ram 1558
	WriteRAM(pProHW, BROADCAST,DCDC_RNGTYPE, 0x200000L);		//ram 1560

	setUserModeBroadcast(pProslic,FALSE);
	
	if (pProslic->debugMode)
		PRINT_MSG("Start calibrate-1 ...");
	
	data = calibrate(pProslic,1);
	if (data != 0)
	{
		PRINT_R("CAL-1 TIMEOUT\n");
		return CAL_TIMEOUT;
	}
	
	if (pProslic->debugMode)
		PRINT_MSG("OK\n");
		
	/* fix coefficient for zcal */
	WriteRAM(pProHW, BROADCAST,746, 0x8F00000L);
	WriteRAM(pProHW, BROADCAST,927, 0x1FFE0000L);




	setUserModeBroadcast(pProslic,TRUE);
	WriteRAM (pProHW, BROADCAST,1537,0x03200000L);
	setUserModeBroadcast(pProslic,FALSE);
	
	/* run just the MADC cal for now */
	/* it requires that the converter is NOT running */
	WriteReg(pProHW, BROADCAST,26,0);	//CALR0 - cals
	WriteReg(pProHW, BROADCAST,27,0);	//CALR1 - no cals
	WriteReg(pProHW, BROADCAST,28,1);	//CALR2	- MADC cal only
	
	if (pProslic->debugMode)
		PRINT_MSG("Start calibrate-2 ...");
		
	data = calibrate(pProslic,1);		//run the cal
	if (data != 0)
	{
		PRINT_R("CAL-2 TIMEOUT\n");
		return CAL_TIMEOUT;
	}
	
	if (pProslic->debugMode)
		PRINT_MSG("OK\n");
	
	/* converter startup sequence */
	setUserModeBroadcast(pProslic,TRUE);
	WriteRAM(pProHW, BROADCAST,1538,0x00600000L); // start up converter
	Delay(pProTimer,100);
	WriteRAM(pProHW, BROADCAST,1538,0);		// turn on protection
	Delay(pProTimer,900);
	setUserModeBroadcast(pProslic,FALSE);
	
	/* run the rest of the cals */
	WriteReg(pProHW, BROADCAST,27,0xbf);	// CALR1 - DCDAC gain, no zcal, leakage
	WriteReg(pProHW, BROADCAST,28,0xf8);	// CALR2 - leakage, hvic, acdac offset, acadc offset
	
	if (pProslic->debugMode)
		PRINT_MSG("Start calibrate-3 ...");
		
	data = calibrate(pProslic,1);		//run the cal
	if (data != 0)
	{
		PRINT_R("CAL-3 TIMEOUT\n");
		return CAL_TIMEOUT;
	}
	
	if (pProslic->debugMode)
		PRINT_MSG("OK\n");

#if 1	
	// DC_FEED: 
	// Ilim=20mA, Irfeed = 15mA, Slope_vlim=200 Ohms, Slope_rfeed=2 kOhms
	WriteRAM (pProHW, BROADCAST,640,0x567609C);    // V_VLIM
	WriteRAM (pProHW, BROADCAST,641,0x50D2839);    // V_RFEED
	WriteRAM (pProHW, BROADCAST,642,0x3E06C43);    // V_ILIM
	WriteRAM (pProHW, BROADCAST,643,0xA9D628);     // CONST_RFEED
	WriteRAM (pProHW, BROADCAST,644,0x5D0FA6);     // CONST_ILIM
	WriteRAM (pProHW, BROADCAST,634,0x1A10433F);   // SLOPE_VLIM
	WriteRAM (pProHW, BROADCAST,635,0x1FA2311C);   // SLOPE_RFEED
	WriteRAM (pProHW, BROADCAST,636,0x40A0E0);     // SLOPE_ILIM
	WriteRAM (pProHW, BROADCAST,637,0x1CFCA14C);   // SLOPE_RING
	WriteRAM (pProHW, BROADCAST,638,0x1E90517E);   // SLOPE_DELTA1
	WriteRAM (pProHW, BROADCAST,639,0x1EB51625);   // SLOPE_DELTA2
	WriteRAM (pProHW, BROADCAST,645,0x2BA7AF);     // I_VLIM
#endif

	// HVIC power threshold
	WriteRAM (pProHW, BROADCAST,756, 0x48D200);	// P_TH_HVIC = 2W

#if 1	
	if (pProslic->debugMode)
		PRINT_MSG("Impedance synthesis...\n");
		
	// impedance synthesis
	// Filter coefficients for 600ohms
	WriteReg(pProHW,BROADCAST,45, 0x73);
	
	WriteRAM (pProHW, BROADCAST,653,0x00000000);  //ZSYNTH_B0
	WriteRAM (pProHW, BROADCAST,654,0x00000000);  //ZSYNTH_B1
	WriteRAM (pProHW, BROADCAST,655,0x00000000);  //ZSYNTH_B2
	WriteRAM (pProHW, BROADCAST,656,0x00000000);  //ZSYNTH_A1
	WriteRAM (pProHW, BROADCAST,657,0x00000000);  //ZSYNTH_A2
	WriteRAM (pProHW, BROADCAST,563,0x0018A380);  //ECFIR_C2
	WriteRAM (pProHW, BROADCAST,564,0x1FE6AE00);  //ECFIR_C3
	WriteRAM (pProHW, BROADCAST,565,0x01D6BC00);  //ECFIR_C4
	WriteRAM (pProHW, BROADCAST,566,0x0077C880);  //ECFIR_C5
	WriteRAM (pProHW, BROADCAST,567,0x0216E300);  //ECFIR_C6
	WriteRAM (pProHW, BROADCAST,568,0x1EC35580);  //ECFIR_C7
	WriteRAM (pProHW, BROADCAST,569,0x01152D00);  //ECFIR_C8
	WriteRAM (pProHW, BROADCAST,570,0x1F332100);  //ECFIR_C9
	WriteRAM (pProHW, BROADCAST,571,0x00887780);  //ECIIR_B0
	WriteRAM (pProHW, BROADCAST,572,0x1F75CD80);  //ECIIR_B1
	WriteRAM (pProHW, BROADCAST,573,0x02AFFF80);  //ECIIR_A1
	WriteRAM (pProHW, BROADCAST,574,0x052E2100);  //ECIIR_A2
	WriteRAM (pProHW, BROADCAST,546,0x08000000);  //RXACEQ_C0
	WriteRAM (pProHW, BROADCAST,547,0x00000000);  //RXACEQ_C1
	WriteRAM (pProHW, BROADCAST,548,0x00000000);  //RXACEQ_C2
	WriteRAM (pProHW, BROADCAST,549,0x00000000);  //RXACEQ_C3
	WriteRAM (pProHW, BROADCAST,658,0x07AE1480);  //RXACHPF_B0_1
	WriteRAM (pProHW, BROADCAST,659,0x1851EC00);  //RXACHPF_B1_1
	WriteRAM (pProHW, BROADCAST,660,0x075C2900);  //RXACHPF_A1_1
	WriteRAM (pProHW, BROADCAST,540,0x08000000);  //TXACEQ_C0
	WriteRAM (pProHW, BROADCAST,541,0x00000000);  //TXACEQ_C1
	WriteRAM (pProHW, BROADCAST,542,0x00000000);  //TXACEQ_C2
	WriteRAM (pProHW, BROADCAST,543,0x00000000);  //TXACEQ_C3
	WriteRAM (pProHW, BROADCAST,544,0x08730700);  //TXACGAIN
	WriteRAM (pProHW, BROADCAST,545,0x01458600);  //RXACGAIN
	WriteRAM (pProHW, BROADCAST,666,0x08000000);  //RXACHPF_GAIN

	// Adj low freq corner of TX path
	WriteRAM(pProHW, BROADCAST,533, 0x71B0000); 	// TXACHPF_A1_1

	// Adj Tx path gain by 0.16dB
	WriteRAM(pProHW, BROADCAST,544, 0x89b3db3);  	// TXACGAIN
#endif

#if 1
	if (pProslic->debugMode)
		PRINT_MSG("Ringing Setup...\n");
		
	// sets up ringing
	WriteRAM(pProHW, BROADCAST, 637, 0x15E5200E);	// ringing impedance = 100 ohms
	WriteRAM(pProHW, BROADCAST, 753, 0xA00000);	// vov_ring_bat = 10V
	WriteRAM(pProHW, BROADCAST, 896, 0x700000);	// vov_ring_gnd = 7V
	
	WriteRAM(pProHW, BROADCAST, 845, 0x1C6000); 	// ringamp = 50.5V open circuit
	WriteRAM(pProHW, BROADCAST, 844, 0x7EFD9D5);	// ringfr = 20Hz
	WriteRAM(pProHW, BROADCAST, 843, 0x1E00000);	// ringof = 20V
	WriteRAM(pProHW, BROADCAST, 755, 0x50000);	// RTPER
	WriteRAM(pProHW, BROADCAST, 730, 0x51EB8);	// counter_vtr_val
	WriteRAM(pProHW, BROADCAST, 850, 0x6000);	// RTACDB = 75ms
	WriteRAM(pProHW, BROADCAST, 849, 0x6000);	// RTDCDB = 75ms
	WriteRAM(pProHW, BROADCAST, 847, 0x2D82D8);	// RTDCTH = 20mA
	WriteRAM(pProHW, BROADCAST, 848, 0x800000);	// RTACTH = 450mA
	
	//2s on, 4s off
	WriteReg(pProHW,BROADCAST, 39, 0x80);		// RING ACTIVE TIMER LOW
	WriteReg(pProHW,BROADCAST, 40, 0x3E);		// RING ACTIVE TIMER HIGH
	WriteReg(pProHW,BROADCAST, 41, 0x0);	      	// RING INACTIVE TIMER LOW
	WriteReg(pProHW,BROADCAST, 42, 0x7D);        	// RING INACTIVE TIMER HIGH
	WriteReg(pProHW,BROADCAST, 38, 0x58);		// RINGCON Both timers are enabled
#endif

	setUserModeBroadcast(pProslic,TRUE);
	WriteRAM(pProHW, BROADCAST, 1445, 0x0);
	WriteRAM(pProHW, BROADCAST, 1451, 0x00200000);	// hvic_cntl_man 
	WriteRAM(pProHW, BROADCAST, 1445, 0x00300000);	// madc_loop_man 
	WriteReg(pProHW,BROADCAST, 98, 0x80);
	setUserModeBroadcast(pProslic,FALSE);
	
	//vov is less for the 3208
	WriteRAM(pProHW, BROADCAST, 753, 0x00000);	// vov_ring_bat = 6V
	WriteRAM(pProHW, BROADCAST, 896, 0x260000);	// vov_ring_gnd = 2.6V
	
	//enable MADC monitoring for OPEN state
	WriteReg(pProHW,BROADCAST, 98, 0x80);


	return 0;
}

int Si3226_PrintDebugData (proslicChanType *pProslic){
#ifdef ENABLE_DEBUG
		int i;
		for (i=0;i<99;i++)
			LOGPRINT ("Si3226 Register %d = %X\n",i,ReadReg(pProHW,pProslic->channel,i));
		for (i=0;i<1024;i++)
			LOGPRINT ("Si3226 RAM %d = %X\n",i,ReadRAM(pProHW,pProslic->channel,i));
#endif
		return 0;
}

#define LB_CAL_TIMEOUT 121 /*12 seconds*/
int Si3226_LBCal (proslicChanType_ptr *pProslic, int size)
{
	int k,i=0;
	uInt8 lf; uInt8 data;
	for (k=0;k<size;k++){
		if (pProslic[k]->channelEnable){
			lf = pProslic[k]->ReadRegX(pProslic[k]->pProHWX, pProslic[k]->channel,LINEFEED); 
			Si3226_SetLinefeedStatus(pProslic[k],LF_OPEN);
#ifdef DISABLE_LOOPMAN
#else
			disableSi3226Loopman(pProslic[k],FALSE);
#endif
			Si3226_SetLinefeedStatus(pProslic[k],LF_FWD_ACTIVE);
			
			pProslic[k]->WriteRegX(pProslic[k]->pProHWX, pProslic[k]->channel,CALR0,0xC); /*enable LB cal*/
			pProslic[k]->WriteRegX(pProslic[k]->pProHWX, pProslic[k]->channel,CALR3,0x80); /*start cal*/
			i=0;
			do {
				data = pProslic[k]->ReadRegX(pProslic[k]->pProHWX,pProslic[k]->channel,CALR3);
				pProslic[k]->DelayX(pProslic[k]->pProTimerX,100);
				if (i++ >= LB_CAL_TIMEOUT){
#ifdef ENABLE_DEBUG
				if (pProslic[k]->debugMode)
					LOGPRINT("Calibration timeout channel %d\n",pProslic[k]->channel);
#endif
				pProslic[k]->error = CAL_TIMEOUT;
				pProslic[k]->WriteRegX(pProslic[k]->pProHWX, pProslic[k]->channel,LINEFEED,LF_OPEN); 
				return CAL_TIMEOUT;
			
			}
			} while (data&0x80 && i<=LB_CAL_TIMEOUT);
#ifdef DISABLE_LOOPMAN
#else
			enableSi3226Loopman(pProslic[k],FALSE);
#endif
			pProslic[k]->WriteRegX(pProslic[k]->pProHWX, pProslic[k]->channel,LINEFEED,lf); 
	
		}
	}
	return 0;
}




int Si3226_LBCalBroadcast (proslicChanType *pProslic)
{
	uInt8 data;int k,j,i;
	int error=0;
	int numOfChan = probeDaisyChain(pProslic);
	WriteReg(pProHW, BROADCAST,LINEFEED,LF_OPEN); 
#ifdef DISABLE_LOOPMAN
#else
			disableSi3226Loopman(pProslic,TRUE);
#endif
	WriteReg(pProHW, BROADCAST,LINEFEED,LF_FWD_ACTIVE); 
	WriteReg(pProHW, BROADCAST,CALR0,0xC); 
	for (j=0;j<2;j++) {
		for (k=0;k<numOfChan;k+=2){
				WriteReg(pProHW, k+j,CALR3,0x80); /*start cal*/
		}
		i=0;
		do {
			data = 0;
			for (k=0;k<numOfChan;k+=2){
					data |= ReadReg(pProHW,k+j,CALR3);
			}	
			Delay(pProTimer,100);
			if (i++ >= LB_CAL_TIMEOUT){
#ifdef ENABLE_DEBUG
				if (pProslic->debugMode)
					LOGPRINT("Si3226 LB Calibration timeout\n");
#endif
				pProslic->error = CAL_TIMEOUT;
				error = CAL_TIMEOUT;

			}
		} while (data&0x80 && i<=LB_CAL_TIMEOUT);

	}
	WriteReg(pProHW, BROADCAST,LINEFEED,LF_OPEN); 
#ifdef DISABLE_LOOPMAN
#else
			enableSi3226Loopman(pProslic,TRUE);
#endif
	WriteReg(pProHW, BROADCAST,LINEFEED,LF_FWD_ACTIVE); 
	return error;
}

int Si3226_GetLBCalResult (proslicChanType *pProslic,int32*result1,int32*result2,int32*result3,int32*result4){
	setUserMode(pProslic,TRUE);
	*result1 = ReadRAM(pProHW,pProslic->channel,1476);
	*result2 = ReadRAM(pProHW,pProslic->channel,1477);
	setUserMode(pProslic,FALSE);
	return 0;
}
int Si3226_LoadPreviousLBCal (proslicChanType *pProslic,int32 result1,int32 result2, int32 result3, int32 result4){
	setUserMode(pProslic,TRUE);
	WriteRAM(pProHW,pProslic->channel,1476,result1);
	WriteRAM(pProHW,pProslic->channel,1477,result2);
	setUserMode(pProslic,FALSE);
	return 0;
}

int Si3226_SetLinefeedStatusBroadcast (proslicChanType_ptr pProslic, uInt8 newLinefeed){
	WriteReg(pProHW,BROADCAST,LINEFEED,newLinefeed);
	return 0;
}

/*
** Function: PROSLIC_Cal
**
** Description: 
** Calibrates the ProSLIC
*/
int Si3226_Cal (proslicChanType_ptr *pProslic, int size){
	/*
	** This function will perform the ProSLIC calibration sequence (for all channels)
	*/
	int i,j=0;
	for (i=0;i<size;i++){
		if (pProslic[i]->channelEnable){
#ifdef DISABLE_LOOPMAN
#else
			//disableSi3226Loopman(pProslic[i],FALSE);
#endif
			j |= calibrate(pProslic[i],0);
#ifdef DISABLE_LOOPMAN
#else
			//enableSi3226Loopman(pProslic[i],FALSE);
#endif
		}
	}
	return j;
}



/*
** Function: PROSLIC_LoadRegTables
**
** Description: 
** Loads registers and ram in the ProSLIC
*/
int Si3226_LoadRegTables (proslicChanType_ptr *pProslic, ProslicRAMInit *pRamTable, ProslicRegInit *pRegTable, int size){
	uInt16 i;
	for (i=0;i<size;i++){
		if (pProslic[i]->channelEnable)
			LoadRegTables(pProslic[i],pRamTable,pRegTable,0);
	}
	return 0;
}


/*
** Function: PROSLIC_LoadPatch
**
** Description: 
** Loads patch to the ProSLIC
*/
int Si3226_LoadPatch (proslicChanType *pProslic, const proslicPatch *pPatch){ 
	LoadSi3226Patch(pProslic,pPatch,0);
	return 0;
}

/*
** Function: PROSLIC_VerifyPatch
**
** Description: 
** Verifiess patch to the ProSLIC
*/
int Si3226_VerifyPatch (proslicChanType *pProslic, const proslicPatch *pPatch)
{ 
	int loop, jmp_table=82;
	uInt8 data; uInt32 ramdata;
	int err = 0;
	if (pPatch == NULL)
	{
		PRINT_R("Si3226 Verify Patch Fail.\n");
		return 0;
	}
	setUserMode (pProslic,TRUE); /*make sure we are in user mode to read patch*/

	WriteReg (pProHW, pProslic->channel, JMPEN,0); /*disable the patch*/

	WriteRAM(pProHW, pProslic->channel,PRAM_ADDR, 0); /*write patch ram address register*/
	
	/* If the data is all 0, you have hit the end of the programmed values and can stop loading.*/
    	for (loop=0; loop<1024; loop++)
    	{
		if (pPatch->patchData[loop] != 0)
		{
			ramdata = ReadRAM(pProHW, pProslic->channel,PRAM_DATA); /*note. data is shifted*/
			if (pPatch->patchData[loop]<<9 != ramdata)
			{
				loop = 1024;			
				err = 1;
			}
		}
        	else
            		loop = 1024;
    	}
	
	/*zero out RAM_ADDR_HI*/
	WriteReg (pProHW, pProslic->channel, RAM_ADDR_HI,0);

    
	jmp_table=82;
	for (loop=0;loop<8;loop++){
		/* check the jump table with the new values.*/
		if (pPatch->patchEntries[loop] != 0){
			data = ReadReg (pProHW, pProslic->channel, jmp_table);
			if (data != ((pPatch->patchEntries[loop])&0xff))
				err = 1;
			data = ReadReg (pProHW, pProslic->channel, jmp_table+1);
			if (data != (pPatch->patchEntries[loop]>>8))
				err = 1;
		}
		jmp_table+=2;
	}
 	
	if (err)
	{
		if (pProslic->debugMode)
			PRINT_R("Si3226 Patch data corrupted: channel %d\n",pProslic->channel);
	}
	else
	{
		WriteReg (pProHW, pProslic->channel, JMPEN,1); /*enable the patch*/
		//PRINT_R("Si3226 Verify Patch Pass.\n");
	}
	setUserMode(pProslic,FALSE); /*turn off user mode*/
	return err;
}


/*
** Function: PROSLIC_EnableInterrupts
**
** Description: 
** Enables interrupts
*/
int Si3226_EnableInterrupts (proslicChanType_ptr pProslic){
	WriteReg (pProHW,pProslic->channel,IRQEN1,Si3226_General_Configuration.irqen1);
	WriteReg (pProHW,pProslic->channel,IRQEN2,Si3226_General_Configuration.irqen2);
	WriteReg (pProHW,pProslic->channel,IRQEN3,Si3226_General_Configuration.irqen3);
	WriteReg (pProHW,pProslic->channel,IRQEN4,Si3226_General_Configuration.irqen4);
	return 0;
}

int Si3226_SetLoopbackMode (proslicChanType_ptr pProslic, ProslicLoopbackModes newMode){
	uInt8 regTemp;
	regTemp = ReadReg (pProHW,pProslic->channel,LOOPBACK);
	switch (newMode){
		case PROSLIC_LOOPBACK_NONE:
			WriteReg (pProHW,pProslic->channel,LOOPBACK,regTemp&~(0x11));
			break;
		case PROSLIC_LOOPBACK_DIG:
			WriteReg (pProHW,pProslic->channel,LOOPBACK,regTemp|(0x1));
			break;
		case PROSLIC_LOOPBACK_ANA:
			WriteReg (pProHW,pProslic->channel,LOOPBACK,regTemp|(0x10));
			break;
	}
	return 0;
}


/*
** Function: PROSLIC_SetMuteStatus
**
** Description: 
** configure RX and TX path mutes
*/
int Si3226_SetMuteStatus (proslicChanType_ptr pProslic, ProslicMuteModes muteEn){
	uInt8 regTemp;
	regTemp = ReadReg (pProHW,pProslic->channel,DIGCON);
	
	WriteReg (pProHW,pProslic->channel,DIGCON,regTemp&~(0x3));
	
	if (muteEn & PROSLIC_MUTE_RX){
		WriteReg (pProHW,pProslic->channel,DIGCON,regTemp|(0x1));
	}
	if (muteEn & PROSLIC_MUTE_TX){
		WriteReg (pProHW,pProslic->channel,DIGCON,regTemp|(0x2));
	}
	return 0;
}

/*
**
** PROSLIC CONFIGURATION FUNCTIONS
**
*/

/*
** Function: PROSLIC_RingSetup
**
** Description: 
** configure ringing
*/
#ifdef DISABLE_RING_SETUP
#else
extern Si3226_Ring_Cfg Si3226_Ring_Presets[];
int Si3226_RingSetup (proslicChanType *pProslic, int preset){
	WriteReg(pProHW,pProslic->channel,RINGTAHI,Si3226_Ring_Presets[preset].tahi);
	WriteRAM(pProHW,pProslic->channel,RINGFR,Si3226_Ring_Presets[preset].freq);
	WriteRAM(pProHW,pProslic->channel,RINGAMP,Si3226_Ring_Presets[preset].amp);
	WriteRAM(pProHW,pProslic->channel,RINGOF,Si3226_Ring_Presets[preset].offset);
	WriteRAM(pProHW,pProslic->channel,RINGPHAS,Si3226_Ring_Presets[preset].phas);
	WriteReg(pProHW,pProslic->channel,RINGTALO,Si3226_Ring_Presets[preset].talo);
	WriteReg(pProHW,pProslic->channel,RINGTIHI,(Si3226_Ring_Presets[preset].tihi));
	WriteReg(pProHW,pProslic->channel,RINGTILO,(Si3226_Ring_Presets[preset].tilo));
	WriteReg(pProHW,pProslic->channel,RINGCON,(Si3226_Ring_Presets[preset].ringcon));
	WriteRAM(pProHW,pProslic->channel,RTPER,Si3226_Ring_Presets[preset].rtper);
	WriteRAM(pProHW,pProslic->channel,RTCOUNT,Si3226_Ring_Presets[preset].rtcount);
	WriteRAM(pProHW,pProslic->channel,ADAP_RING_MIN_I,Si3226_Ring_Presets[preset].adap_ring_min_i);
	WriteRAM(pProHW,pProslic->channel,SLOPE_RING,Si3226_Ring_Presets[preset].slope_ring);
	WriteRAM(pProHW,pProslic->channel,COUNTER_VTR_VAL,Si3226_Ring_Presets[preset].counter_vtr_val);
	WriteRAM(pProHW,pProslic->channel,RTACDB,Si3226_Ring_Presets[preset].rtacdb);
	WriteRAM(pProHW,pProslic->channel,RTACTH,Si3226_Ring_Presets[preset].rtacth);
	WriteRAM(pProHW,pProslic->channel,RTDCDB,Si3226_Ring_Presets[preset].rtdcdb);
	WriteRAM(pProHW,pProslic->channel,RTDCTH,Si3226_Ring_Presets[preset].rtdcth);
	WriteRAM(pProHW,pProslic->channel,VOV_RING_BAT,Si3226_Ring_Presets[preset].vov_ring_bat);
	WriteRAM(pProHW,pProslic->channel,VOV_RING_GND,Si3226_Ring_Presets[preset].vov_ring_gnd);
	/*WriteRAM(pProHW,pProslic->channel,DELTA_VCM, (ReadRAM(pProHW,pProslic->channel,VBATR_EXPECT)>>1) - Si3226_Ring_Presets[preset].vcm_ring);*/
	WriteRAM(pProHW,pProslic->channel,VCM_RING,Si3226_Ring_Presets[preset].vcm_ring);
	
	return 0;
}
#endif

void Si3226_Set_Ring_Cadence_ON(proslicChanType *pProslic, unsigned short msec)
{
	unsigned int l_time, h_time, tmp;

	tmp = msec << 3;
	if (tmp > 255)
	{
		l_time = 255; 
		h_time = (tmp - 255)>>8;
	}
	else
	{
		l_time = tmp;
		h_time = 0;
	}
		
	WriteReg(pProHW,pProslic->channel,RINGTALO, l_time);
	WriteReg(pProHW,pProslic->channel,RINGTAHI, h_time);
}

void Si3226_Set_Ring_Cadence_OFF(proslicChanType *pProslic, unsigned short msec)
{
	unsigned int l_time, h_time, tmp;

	tmp = msec << 3;
	if (tmp > 255)
	{
		l_time = 255; 
		h_time = (tmp - 255)>>8;
	}
	else
	{
		l_time = tmp;
		h_time = 0;
	}
		
	WriteReg(pProHW,pProslic->channel,RINGTILO, l_time);
	WriteReg(pProHW,pProslic->channel,RINGTIHI, h_time);
}

#include <linux/kernel.h>
#include <linux/delay.h>

static unsigned long l_buf[4];
static unsigned char c_buf[9]; // remove 74 (VBATH)
static int b_num[]={RINGOF, RINGFR, RINGAMP, RINGPHAS, RINGCON, RINGTALO, RINGTAHI, RINGTILO, RINGTIHI};
static unsigned long l_data[]={0x0000, 0x7F46000, 0x248000, 0x0000, 0x18, 0xa0, 0x0f, 0xa0, 0x0f};

void Si3226_SendNTTCAR(proslicChanType *pProslic)
{
	int i;
	/************** backup the register ***************/

	for ( i=0;i<4;i++)
		l_buf[i] = ReadRAM(pProHW, pProslic->channel, b_num[i]);

	for ( i=4;i<9;i++)
		c_buf[i] = ReadReg(pProHW, pProslic->channel,b_num[i]);

	/*********** To Create Short Ring *****************/
	//send CAR
	for (i=0;i<4;i++)
		WriteRAM(pProHW,pProslic->channel,b_num[i],l_data[i]);
	for (i=4;i<9;i++)
		WriteReg(pProHW,pProslic->channel,b_num[i],l_data[i]);

	Si3226_RingStart(pProslic);
	
}


unsigned int Si3226_SendNTTCAR_check(unsigned int chid, proslicChanType *pProslic, unsigned long time_out)
{
	//int protect_cnt = 0;
	int i;
	/*********** Check Phone Hook State ***************/

	if ( !(ReadReg(pProHW,pProslic->channel,LCRRTP) & 2) ) // if phone on-hook
	{

/*		while( !(ReadReg(pProHW,pProslic->channel,LCRRTP) & 2) )  //wait phone off-hook atuomatically
		{
			if (protect_cnt == 30)	// wait 6 sec, if no off-hook-> break to prevent watch dog reset. 
			{
				break;
			}
			mdelay(200);
			protect_cnt ++;
		}	
*/
		if (time_after(jiffies,time_out) ) //time_after(a,b) returns true if the time a is after time b.
		{
			/* don't return 0, return 1, report time out don't wait */

		}
		else
			return 0;
			
	}

	/******* Set Reverse On-Hook Transmission *********/		
	ProSLIC_SetLinefeedStatus(pProslic, LF_REV_OHT);
	
	/************** restore the register ***************/
	for (i=0;i<4;i++)
		WriteRAM(pProHW,pProslic->channel,b_num[i],l_buf[i]);
	for (i=4;i<9;i++)
		WriteReg(pProHW,pProslic->channel,b_num[i],c_buf[i]);


	return 1;

}

/*
** Function: PROSLIC_ToneGenSetup
**
** Description: 
** configure tone generators
*/
#ifdef DISABLE_TONE_SETUP
#else
extern Si3226_Tone_Cfg Si3226_Tone_Presets[];
int Si3226_ToneGenSetup (proslicChanType *pProslic, int preset){
	WriteRAM(pProHW,pProslic->channel,OSC1FREQ,Si3226_Tone_Presets[preset].osc1.freq);
	WriteRAM(pProHW,pProslic->channel,OSC1AMP,Si3226_Tone_Presets[preset].osc1.amp);
	WriteRAM(pProHW,pProslic->channel,OSC1PHAS,Si3226_Tone_Presets[preset].osc1.phas);
	WriteReg(pProHW,pProslic->channel,O1TAHI,(Si3226_Tone_Presets[preset].osc1.tahi));
	WriteReg(pProHW,pProslic->channel,O1TALO,(Si3226_Tone_Presets[preset].osc1.talo));
	WriteReg(pProHW,pProslic->channel,O1TIHI,(Si3226_Tone_Presets[preset].osc1.tihi));
	WriteReg(pProHW,pProslic->channel,O1TILO,(Si3226_Tone_Presets[preset].osc1.tilo));
	WriteRAM(pProHW,pProslic->channel,OSC2FREQ,Si3226_Tone_Presets[preset].osc2.freq);
	WriteRAM(pProHW,pProslic->channel,OSC2AMP,Si3226_Tone_Presets[preset].osc2.amp);
	WriteRAM(pProHW,pProslic->channel,OSC2PHAS,Si3226_Tone_Presets[preset].osc2.phas);
	WriteReg(pProHW,pProslic->channel,O2TAHI,(Si3226_Tone_Presets[preset].osc2.tahi));
	WriteReg(pProHW,pProslic->channel,O2TALO,(Si3226_Tone_Presets[preset].osc2.talo));
	WriteReg(pProHW,pProslic->channel,O2TIHI,(Si3226_Tone_Presets[preset].osc2.tihi));
	WriteReg(pProHW,pProslic->channel,O2TILO,(Si3226_Tone_Presets[preset].osc2.tilo));
	WriteReg(pProHW,pProslic->channel,OMODE,(Si3226_Tone_Presets[preset].omode));
	return 0;
}
#endif
/*
** Function: PROSLIC_FSKSetup
**
** Description: 
** configure fsk
*/
#ifdef DISABLE_FSK_SETUP
#else
extern Si3226_FSK_Cfg Si3226_FSK_Presets[];
int Si3226_FSKSetup (proslicChanType *pProslic, int preset){
	uInt8 data; 


	WriteReg(pProHW,pProslic->channel,O1TAHI,0);
	WriteReg(pProHW,pProslic->channel,O1TIHI,0);
	WriteReg(pProHW,pProslic->channel,O1TILO,0);
	WriteReg(pProHW,pProslic->channel,O1TALO,0x13);

	data = ReadReg(pProHW,pProslic->channel,OMODE);
	if (Si3226_FSK_Presets[preset].eightBit)
		data |= 0x80;
	else 
		data &= ~(0x80);
	WriteReg(pProHW,pProslic->channel,FSKDEPTH,Si3226_FSK_Presets[preset].fskdepth);
	WriteReg(pProHW,pProslic->channel,OMODE,data);
	WriteRAM(pProHW,pProslic->channel,FSK01,Si3226_FSK_Presets[preset].fsk01);
	WriteRAM(pProHW,pProslic->channel,FSK10,Si3226_FSK_Presets[preset].fsk10);
	WriteRAM(pProHW,pProslic->channel,FSKAMP0,Si3226_FSK_Presets[preset].fskamp0);
	WriteRAM(pProHW,pProslic->channel,FSKAMP1,Si3226_FSK_Presets[preset].fskamp1);
	WriteRAM(pProHW,pProslic->channel,FSKFREQ0,Si3226_FSK_Presets[preset].fskfreq0);
	WriteRAM(pProHW,pProslic->channel,FSKFREQ1,Si3226_FSK_Presets[preset].fskfreq1);
	return 0;
}
#endif

int Si3226_CheckCIDBuffer (proslicChanType *pProslic, uInt8 *fsk_buf_avail){
	uInt8 data;
	data = ReadReg(pProHW,pProslic->channel,IRQ1);
	WriteReg(pProHW,pProslic->channel,IRQ1,data); /*clear (for GCI)*/
	*fsk_buf_avail = (data&0x40) ? 1 : 0;
	return 0;
}
/*
** Function: PROSLIC_DTMFDecodeSetup
**
** Description: 
** configure dtmf decode
*/
#ifdef DISABLE_DTMF_SETUP
#else
extern Si3226_DTMFDec_Cfg Si3226_DTMFDec_Presets[];
int Si3226_DTMFDecodeSetup (proslicChanType *pProslic, int preset){
	
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_B0_1,Si3226_DTMFDec_Presets[preset].dtmfdtf_b0_1);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_B1_1,Si3226_DTMFDec_Presets[preset].dtmfdtf_b1_1);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_B2_1,Si3226_DTMFDec_Presets[preset].dtmfdtf_b2_1);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_A1_1,Si3226_DTMFDec_Presets[preset].dtmfdtf_a1_1);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_A2_1,Si3226_DTMFDec_Presets[preset].dtmfdtf_a2_1);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_B0_2,Si3226_DTMFDec_Presets[preset].dtmfdtf_b0_2);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_B1_2,Si3226_DTMFDec_Presets[preset].dtmfdtf_b1_2);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_B2_2,Si3226_DTMFDec_Presets[preset].dtmfdtf_b2_2);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_A1_2,Si3226_DTMFDec_Presets[preset].dtmfdtf_a1_2);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_A2_2,Si3226_DTMFDec_Presets[preset].dtmfdtf_a2_2);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_B0_3,Si3226_DTMFDec_Presets[preset].dtmfdtf_b0_3);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_B1_3,Si3226_DTMFDec_Presets[preset].dtmfdtf_b1_3);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_B2_3,Si3226_DTMFDec_Presets[preset].dtmfdtf_b2_3);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_A1_3,Si3226_DTMFDec_Presets[preset].dtmfdtf_a1_3);
	WriteRAM(pProHW,pProslic->channel,DTMFDTF_A2_3,Si3226_DTMFDec_Presets[preset].dtmfdtf_a2_3);
	return 0;
}
#endif
/*
** Function: PROSLIC_SetProfile
**
** Description: 
** set country profile of the proslic
*/
int Si3226_SetProfile (proslicChanType *pProslic, int preset){
	/*TO DO
	//Will be filled in at a later date*/
	return 0;
}

int Si3226_ImpdanceDump (proslicChanType *pProslic){
	printk("channel-%d: Impedance registers(ZSYNTH_B0-2, A1-2)\n", pProslic->channel);
	int i;
	
	printk("RA: 0x%X\n", ReadReg(pProHW,pProslic->channel,RA));
	for (i=0; i<5; i++)
		printk("%d: 0x%lX\n", (653+i), ReadRAM(pProHW, pProslic->channel,ZSYNTH_B0+i));
	printk("\n");
	return 0;
}

#if 0 // for impedance settings (RTK_VOIP)

int Si3226_ImpdanceSetup (proslicChanType *pProslic, int preset)
{
	uInt8 lf;
	lf = ReadReg(pProHW,pProslic->channel,LINEFEED);
	WriteReg(pProHW,pProslic->channel,LINEFEED,0);
	
        WriteRAM(pProHW,pProslic->channel,TXACEQ_C0,RTK_Si3226_impedence_Presets[preset].txaceq_c0);
        WriteRAM(pProHW,pProslic->channel,TXACEQ_C1,RTK_Si3226_impedence_Presets[preset].txaceq_c1);
        WriteRAM(pProHW,pProslic->channel,TXACEQ_C2,RTK_Si3226_impedence_Presets[preset].txaceq_c2);
        WriteRAM(pProHW,pProslic->channel,TXACEQ_C3,RTK_Si3226_impedence_Presets[preset].txaceq_c3);

        WriteRAM(pProHW,pProslic->channel,RXACEQ_C0,RTK_Si3226_impedence_Presets[preset].rxaceq_c0);
        WriteRAM(pProHW,pProslic->channel,RXACEQ_C1,RTK_Si3226_impedence_Presets[preset].rxaceq_c1);
        WriteRAM(pProHW,pProslic->channel,RXACEQ_C2,RTK_Si3226_impedence_Presets[preset].rxaceq_c2);
        WriteRAM(pProHW,pProslic->channel,RXACEQ_C3,RTK_Si3226_impedence_Presets[preset].rxaceq_c3);

	WriteRAM(pProHW,pProslic->channel,ECFIR_C2,RTK_Si3226_impedence_Presets[preset].ecfir_c2);
	WriteRAM(pProHW,pProslic->channel,ECFIR_C3,RTK_Si3226_impedence_Presets[preset].ecfir_c3);
        WriteRAM(pProHW,pProslic->channel,ECFIR_C4,RTK_Si3226_impedence_Presets[preset].ecfir_c4);
        WriteRAM(pProHW,pProslic->channel,ECFIR_C5,RTK_Si3226_impedence_Presets[preset].ecfir_c5);
        WriteRAM(pProHW,pProslic->channel,ECFIR_C6,RTK_Si3226_impedence_Presets[preset].ecfir_c6);
        WriteRAM(pProHW,pProslic->channel,ECFIR_C7,RTK_Si3226_impedence_Presets[preset].ecfir_c7);
        WriteRAM(pProHW,pProslic->channel,ECFIR_C8,RTK_Si3226_impedence_Presets[preset].ecfir_c8);
        WriteRAM(pProHW,pProslic->channel,ECFIR_C9,RTK_Si3226_impedence_Presets[preset].ecfir_c9);

        WriteRAM(pProHW,pProslic->channel,ECIIR_B0,RTK_Si3226_impedence_Presets[preset].eciir_b0);
        WriteRAM(pProHW,pProslic->channel,ECIIR_B1,RTK_Si3226_impedence_Presets[preset].eciir_b1);

        WriteRAM(pProHW,pProslic->channel,ECIIR_A1,RTK_Si3226_impedence_Presets[preset].eciir_a1);
        WriteRAM(pProHW,pProslic->channel,ECIIR_A2,RTK_Si3226_impedence_Presets[preset].eciir_a2);

	WriteRAM(pProHW,pProslic->channel,ZSYNTH_A1,RTK_Si3226_impedence_Presets[preset].zsynth_a1);
	WriteRAM(pProHW,pProslic->channel,ZSYNTH_A2,RTK_Si3226_impedence_Presets[preset].zsynth_a2);
	WriteRAM(pProHW,pProslic->channel,ZSYNTH_B1,RTK_Si3226_impedence_Presets[preset].zsynth_b1);
	WriteRAM(pProHW,pProslic->channel,ZSYNTH_B0,RTK_Si3226_impedence_Presets[preset].zsynth_b0);
	WriteRAM(pProHW,pProslic->channel,ZSYNTH_B2,RTK_Si3226_impedence_Presets[preset].zsynth_b2);

	WriteReg(pProHW,pProslic->channel,RA,RTK_Si3226_impedence_Presets[preset].ra);

 	WriteRAM(pProHW,pProslic->channel,TXACGAIN,RTK_Si3226_impedence_Presets[preset].txacgain);

        WriteRAM(pProHW,pProslic->channel,RXACGAIN,RTK_Si3226_impedence_Presets[preset].rxacgain);
        WriteRAM(pProHW,pProslic->channel,RXACGAIN_SAVE,RTK_Si3226_impedence_Presets[preset].rxacgain_save);
	
        WriteRAM(pProHW,pProslic->channel,RXACHPF_B0_1,RTK_Si3226_impedence_Presets[preset].rxachpf_b0_1);
        WriteRAM(pProHW,pProslic->channel,RXACHPF_B1_1,RTK_Si3226_impedence_Presets[preset].rxachpf_b1_1);
        WriteRAM(pProHW,pProslic->channel,RXACHPF_A1_1,RTK_Si3226_impedence_Presets[preset].rxachpf_a1_1);

        WriteReg(pProHW,pProslic->channel,LINEFEED,lf);
	return 0;
}
#endif


void Si3226_Set_Impendance_Silicon(proslicChanType *pProslic, unsigned short country, unsigned short impd /*reserve*/)
{

	switch(country)
	{
		case COUNTRY_USA:
		case COUNTRY_HK:
		case COUNTRY_TW:
		#if 1
			/* Do nothing. Use init default setting: ra = 600 ohm */
		#else
			Si3226_ZsynthSetup(pProslic, 0); 	// 600 ohm
		#endif
			break;

		case COUNTRY_JP:
			Si3226_ZsynthSetup(pProslic, 1); 	// 600 ohm + 1uF
			//Si3226_ZsynthSetup(pProslic, 0); 	// 600 ohm
			break;

		
		case COUNTRY_AUSTRALIA:	
			Si3226_ZsynthSetup(pProslic, 5); 	// 220 onm + (600 omh || 100 nF)	
			break;
			
		case COUNTRY_CN:
			//Si3226_ZsynthSetup(pProslic, 2); 	// 200 ohm + (560 ohm || 100 nF)
			Si3226_ZsynthSetup(pProslic, 3);	// 200 ohm + (680 ohm || 100 nF)
			break;
		
		case COUNTRY_GR:
			Si3226_ZsynthSetup(pProslic, 6); 	// 220 ohm + (820 ohm || 115 nF)
			break;
			
		case COUNTRY_UK:
			Si3226_ZsynthSetup(pProslic, 4); 	// CTR21
			break;
			
		case COUNTRY_SE:
			Si3226_ZsynthSetup(pProslic, 8); 	// 200 ohm + (1000 ohm || 100 nF)
			break;
			
		case COUNTRY_FR:
			Si3226_ZsynthSetup(pProslic, 10); 	// 215 ohm + (1000 ohm || 137 nF)
			break;
			
		case COUNTRY_BE:
			Si3226_ZsynthSetup(pProslic, 9); 	// 150 ohm + (830 ohm || 72 nF)
			break;
			
		case COUNTRY_FL:
			Si3226_ZsynthSetup(pProslic, 4); 	// CTR21
			break;
			
		case COUNTRY_IT:
			Si3226_ZsynthSetup(pProslic, 7); 	// 400 ohm + (700 ohm || 200 nF)
			break;
		

			
		default:
			Si3226_ZsynthSetup(pProslic, 0); 
			PRINT_MSG(" Not support impedance of this country. Set to default SLIC impedance 600 ohm.\n");
			break;

	}

	//Si3226_ZsynthCheck(pProslic, 2);
	//Si3226_ImpdanceDump(pProslic);
}

/*
** Function: PROSLIC_ZsynthSetup
**
** Description: 
** configure impedence synthesis
*/
#ifdef DISABLE_ZSYNTH_SETUP
#else
extern Si3226_Impedance_Cfg Si3226_Impedance_Presets [];
int Si3226_ZsynthSetup (proslicChanType *pProslic, int preset)
{
	uInt8 lf;

	unsigned long flags;
	
	save_flags(flags); cli();

	lf = ReadReg(pProHW,pProslic->channel,LINEFEED);
	WriteReg(pProHW,pProslic->channel,LINEFEED,0);
#if 1	
	WriteRAM(pProHW,pProslic->channel,TXACEQ_C0,Si3226_Impedance_Presets[preset].audioEQ.txaceq_c0);
	WriteRAM(pProHW,pProslic->channel,TXACEQ_C1,Si3226_Impedance_Presets[preset].audioEQ.txaceq_c1);
	WriteRAM(pProHW,pProslic->channel,TXACEQ_C2,Si3226_Impedance_Presets[preset].audioEQ.txaceq_c2);
	WriteRAM(pProHW,pProslic->channel,TXACEQ_C3,Si3226_Impedance_Presets[preset].audioEQ.txaceq_c3);
	
	WriteRAM(pProHW,pProslic->channel,RXACEQ_C0,Si3226_Impedance_Presets[preset].audioEQ.rxaceq_c0);
	WriteRAM(pProHW,pProslic->channel,RXACEQ_C1,Si3226_Impedance_Presets[preset].audioEQ.rxaceq_c1);
	WriteRAM(pProHW,pProslic->channel,RXACEQ_C2,Si3226_Impedance_Presets[preset].audioEQ.rxaceq_c2);
	WriteRAM(pProHW,pProslic->channel,RXACEQ_C3,Si3226_Impedance_Presets[preset].audioEQ.rxaceq_c3);
	
	WriteRAM(pProHW,pProslic->channel,ECFIR_C2,Si3226_Impedance_Presets[preset].hybrid.ecfir_c2);
	WriteRAM(pProHW,pProslic->channel,ECFIR_C3,Si3226_Impedance_Presets[preset].hybrid.ecfir_c3);
	WriteRAM(pProHW,pProslic->channel,ECFIR_C4,Si3226_Impedance_Presets[preset].hybrid.ecfir_c4);
	WriteRAM(pProHW,pProslic->channel,ECFIR_C5,Si3226_Impedance_Presets[preset].hybrid.ecfir_c5);
	WriteRAM(pProHW,pProslic->channel,ECFIR_C6,Si3226_Impedance_Presets[preset].hybrid.ecfir_c6);
	WriteRAM(pProHW,pProslic->channel,ECFIR_C7,Si3226_Impedance_Presets[preset].hybrid.ecfir_c7);
	WriteRAM(pProHW,pProslic->channel,ECFIR_C8,Si3226_Impedance_Presets[preset].hybrid.ecfir_c8);
	WriteRAM(pProHW,pProslic->channel,ECFIR_C9,Si3226_Impedance_Presets[preset].hybrid.ecfir_c9);
	
	WriteRAM(pProHW,pProslic->channel,ECIIR_B0,Si3226_Impedance_Presets[preset].hybrid.ecfir_b0);
	WriteRAM(pProHW,pProslic->channel,ECIIR_B1,Si3226_Impedance_Presets[preset].hybrid.ecfir_b1);
	
	WriteRAM(pProHW,pProslic->channel,ECIIR_A1,Si3226_Impedance_Presets[preset].hybrid.ecfir_a1);
	WriteRAM(pProHW,pProslic->channel,ECIIR_A2,Si3226_Impedance_Presets[preset].hybrid.ecfir_a2);
	
	WriteRAM(pProHW,pProslic->channel,ZSYNTH_A1,Si3226_Impedance_Presets[preset].zsynth.zsynth_a1);
	WriteRAM(pProHW,pProslic->channel,ZSYNTH_A2,Si3226_Impedance_Presets[preset].zsynth.zsynth_a2);
	WriteRAM(pProHW,pProslic->channel,ZSYNTH_B1,Si3226_Impedance_Presets[preset].zsynth.zsynth_b1);
	WriteRAM(pProHW,pProslic->channel,ZSYNTH_B0,Si3226_Impedance_Presets[preset].zsynth.zsynth_b0);
	WriteRAM(pProHW,pProslic->channel,ZSYNTH_B2,Si3226_Impedance_Presets[preset].zsynth.zsynth_b2);
	
	WriteReg(pProHW,pProslic->channel,RA,Si3226_Impedance_Presets[preset].zsynth.ra);
	
	WriteRAM(pProHW,pProslic->channel,TXACGAIN,Si3226_Impedance_Presets[preset].txgain.acgain);

	WriteRAM(pProHW,pProslic->channel,RXACGAIN,Si3226_Impedance_Presets[preset].rxgain.acgain);
	WriteRAM(pProHW,pProslic->channel,RXACGAIN_SAVE,Si3226_Impedance_Presets[preset].rxgain.acgain);

	WriteRAM(pProHW,pProslic->channel,RXACHPF_B0_1,Si3226_Impedance_Presets[preset].rxachpf_b0_1);
	WriteRAM(pProHW,pProslic->channel,RXACHPF_B1_1,Si3226_Impedance_Presets[preset].rxachpf_b1_1);
	WriteRAM(pProHW,pProslic->channel,RXACHPF_A1_1,Si3226_Impedance_Presets[preset].rxachpf_a1_1);
#else
	// test
	printk("set impedance...0505, chid = %d\n", pProslic->channel);
	
	WriteReg(pProHW,pProslic->channel, 45, 0x73);
	
	WriteRAM(pProHW,pProslic->channel, 653, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 654, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 655, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 656, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 657, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 563, 0x0018A380);
	WriteRAM(pProHW,pProslic->channel, 564, 0x1FE6AE00);
	WriteRAM(pProHW,pProslic->channel, 565, 0x01D6BC00);
	WriteRAM(pProHW,pProslic->channel, 566, 0x0077C880);
	WriteRAM(pProHW,pProslic->channel, 567, 0x0216E300);
	WriteRAM(pProHW,pProslic->channel, 568, 0x1EC35580);
	WriteRAM(pProHW,pProslic->channel, 569, 0x01152D00);
	WriteRAM(pProHW,pProslic->channel, 570, 0x1F332100);
	WriteRAM(pProHW,pProslic->channel, 571, 0x00887780);
	WriteRAM(pProHW,pProslic->channel, 572, 0x1F75CD80);
	WriteRAM(pProHW,pProslic->channel, 573, 0x02AFFF80);
	WriteRAM(pProHW,pProslic->channel, 574, 0x052E2100);
	WriteRAM(pProHW,pProslic->channel, 546, 0x08000000);
	WriteRAM(pProHW,pProslic->channel, 547, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 548, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 549, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 658, 0x07AE1480);
	WriteRAM(pProHW,pProslic->channel, 659, 0x1851EC00);
	WriteRAM(pProHW,pProslic->channel, 660, 0x075C2900);
	WriteRAM(pProHW,pProslic->channel, 540, 0x08000000);
	WriteRAM(pProHW,pProslic->channel, 541, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 542, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 543, 0x00000000);
	WriteRAM(pProHW,pProslic->channel, 544, 0x08730700);
	WriteRAM(pProHW,pProslic->channel, 545, 0x01458600);
	WriteRAM(pProHW,pProslic->channel, 666, 0x08000000);

	printk("end\n");	                                   
	                                   
#endif	                                   
	WriteReg(pProHW,pProslic->channel,LINEFEED,lf);

	restore_flags(flags);

	return 0;                          
}

int Si3226_ZsynthCheck (proslicChanType *pProslic, int preset)
{
	printk("check impedance...\n");

	
	if ((ReadRAM(pProHW,pProslic->channel,TXACEQ_C0)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].audioEQ.txaceq_c0)
		printk("error 1\n");
	if ((ReadRAM(pProHW,pProslic->channel,TXACEQ_C1)&0x1FFFFF80) !=Si3226_Impedance_Presets[preset].audioEQ.txaceq_c1)
		printk("error 2\n");
	if ((ReadRAM(pProHW,pProslic->channel,TXACEQ_C2)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].audioEQ.txaceq_c2)
		printk("error 3\n");
	if ((ReadRAM(pProHW,pProslic->channel,TXACEQ_C3)&0x1FFFFF80) !=Si3226_Impedance_Presets[preset].audioEQ.txaceq_c3)
		printk("error 4\n");
		
	if ((ReadRAM(pProHW,pProslic->channel,RXACEQ_C0)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].audioEQ.rxaceq_c0)
		printk("error 5\n");
	if ((ReadRAM(pProHW,pProslic->channel,RXACEQ_C1)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].audioEQ.rxaceq_c1)
		printk("error 6\n");
	if ((ReadRAM(pProHW,pProslic->channel,RXACEQ_C2)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].audioEQ.rxaceq_c2)
		printk("error 7\n");
	if ((ReadRAM(pProHW,pProslic->channel,RXACEQ_C3)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].audioEQ.rxaceq_c3)
		printk("error 8\n");
	
	if ((ReadRAM(pProHW,pProslic->channel,ECFIR_C2)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_c2)
		printk("error 9\n");
	if ((ReadRAM(pProHW,pProslic->channel,ECFIR_C3)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_c3)
		printk("error 10\n");
	if ((ReadRAM(pProHW,pProslic->channel,ECFIR_C4)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_c4)
		printk("error 11\n");
	if ((ReadRAM(pProHW,pProslic->channel,ECFIR_C5)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_c5)
		printk("error 12\n");
	if ((ReadRAM(pProHW,pProslic->channel,ECFIR_C6)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_c6)
		printk("error 13\n");
	if ((ReadRAM(pProHW,pProslic->channel,ECFIR_C7)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_c7)
		printk("error 14\n");
	if ((ReadRAM(pProHW,pProslic->channel,ECFIR_C8)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_c8)
		printk("error 15\n");
	if ((ReadRAM(pProHW,pProslic->channel,ECFIR_C9)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_c9)
		printk("error 16\n");
	
	if ((ReadRAM(pProHW,pProslic->channel,ECIIR_B0)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_b0)
		printk("error 17\n");
	if ((ReadRAM(pProHW,pProslic->channel,ECIIR_B1)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_b1)
		printk("error 18\n");
	
	if ((ReadRAM(pProHW,pProslic->channel,ECIIR_A1)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_a1)
		printk("error 19\n");
	if ((ReadRAM(pProHW,pProslic->channel,ECIIR_A2)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].hybrid.ecfir_a2)
		printk("error 20\n");
	
	if ((ReadRAM(pProHW,pProslic->channel,ZSYNTH_A1)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].zsynth.zsynth_a1)
		printk("error 21\n");
	if ((ReadRAM(pProHW,pProslic->channel,ZSYNTH_A2)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].zsynth.zsynth_a2)
		printk("error 22\n");
	if ((ReadRAM(pProHW,pProslic->channel,ZSYNTH_B1)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].zsynth.zsynth_b1)
		printk("error 23\n");
	if ((ReadRAM(pProHW,pProslic->channel,ZSYNTH_B0)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].zsynth.zsynth_b0)
		printk("error 24\n");
	if ((ReadRAM(pProHW,pProslic->channel,ZSYNTH_B2)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].zsynth.zsynth_b2)
		printk("error 25\n");
	
	if ((ReadRAM(pProHW,pProslic->channel,RA)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].zsynth.ra)
		printk("error 26\n");
	
	if ((ReadRAM(pProHW,pProslic->channel,TXACGAIN)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].txgain.acgain)
		printk("error 27\n");

	if ((ReadRAM(pProHW,pProslic->channel,RXACGAIN)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].rxgain.acgain)
		printk("error 28\n");
	
	if ((ReadRAM(pProHW,pProslic->channel,RXACGAIN_SAVE)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].rxgain.acgain)
		printk("error 29\n");
	if ((ReadRAM(pProHW,pProslic->channel,RXACHPF_B0_1)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].rxachpf_b0_1)
		printk("error 30\n");
	if ((ReadRAM(pProHW,pProslic->channel,RXACHPF_B1_1)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].rxachpf_b1_1)
		printk("error 31\n");
	if ((ReadRAM(pProHW,pProslic->channel,RXACHPF_A1_1)&0x1FFFFF80) != Si3226_Impedance_Presets[preset].rxachpf_a1_1)
		printk("error 32\n");
	

	return 0;
}
#endif

/*
** Function: PROSLIC_GciCISetup
**
** Description: 
** configure CI bits (GCI mode)
*/
#ifdef DISABLE_CI_SETUP
#else
extern Si3226_CI_Cfg Si3226_CI_Presets [];
int Si3226_GciCISetup (proslicChanType *pProslic, int preset){
	WriteReg(pProHW,pProslic->channel,GCI_CI,Si3226_CI_Presets[preset].gci_ci);
	return 0;
}
#endif
/*
** Function: PROSLIC_ModemDetSetup
**
** Description: 
** configure modem detector
*/
int Si3226_ModemDetSetup (proslicChanType *pProslic, int preset){
	/*TO DO
	//Will be filled in at a later date*/
	return 0;
}

/*
** Function: PROSLIC_AudioGainSetup
**
** Description: 
** configure audio gains
*/
#ifdef DISABLE_AUDIOGAIN_SETUP
#else
extern Si3226_audioGain_Cfg Si3226_audioGain_Presets[];
int Si3226_TXAudioGainSetup (proslicChanType *pProslic, int preset){
	WriteRAM(pProHW,pProslic->channel,TXACGAIN,Si3226_audioGain_Presets[preset].acgain);
	return 0;
}

/*
** Function: PROSLIC_AudioGainSetup
**
** Description: 
** configure audio gains
*/
int Si3226_RXAudioGainSetup (proslicChanType *pProslic, int preset){
	WriteRAM(pProHW,pProslic->channel,RXACGAIN,Si3226_audioGain_Presets[preset].acgain);
	WriteRAM(pProHW,pProslic->channel,RXACGAIN_SAVE,Si3226_audioGain_Presets[preset].acgain);
	return 0;
}
#endif




/*
** Function: PROSLIC_DCFeedSetup
**
** Description: 
** configure dc feed
*/
#ifdef DISABLE_DCFEED_SETUP
#else
extern Si3226_DCfeed_Cfg Si3226_DCfeed_Presets[];
int Si3226_DCFeedSetup (proslicChanType *pProslic, int preset){
	uInt8 lf;
	lf = ReadReg(pProHW,pProslic->channel,LINEFEED);
	WriteReg(pProHW,pProslic->channel,LINEFEED,0);
	WriteRAM(pProHW,pProslic->channel,SLOPE_VLIM,Si3226_DCfeed_Presets[preset].slope_vlim);
	WriteRAM(pProHW,pProslic->channel,SLOPE_RFEED,Si3226_DCfeed_Presets[preset].slope_rfeed);
	WriteRAM(pProHW,pProslic->channel,SLOPE_ILIM,Si3226_DCfeed_Presets[preset].slope_ilim);
	WriteRAM(pProHW,pProslic->channel,SLOPE_DELTA1,Si3226_DCfeed_Presets[preset].delta1);
	WriteRAM(pProHW,pProslic->channel,SLOPE_DELTA2,Si3226_DCfeed_Presets[preset].delta2);
	WriteRAM(pProHW,pProslic->channel,V_VLIM,Si3226_DCfeed_Presets[preset].v_vlim);
	WriteRAM(pProHW,pProslic->channel,V_RFEED,Si3226_DCfeed_Presets[preset].v_rfeed);
	WriteRAM(pProHW,pProslic->channel,V_ILIM,Si3226_DCfeed_Presets[preset].v_ilim);
	WriteRAM(pProHW,pProslic->channel,CONST_RFEED,Si3226_DCfeed_Presets[preset].const_rfeed);
	WriteRAM(pProHW,pProslic->channel,CONST_ILIM,Si3226_DCfeed_Presets[preset].const_ilim);
	WriteRAM(pProHW,pProslic->channel,I_VLIM,Si3226_DCfeed_Presets[preset].i_vlim);
	WriteRAM(pProHW,pProslic->channel,LCRONHK,Si3226_DCfeed_Presets[preset].lcronhk);
	WriteRAM(pProHW,pProslic->channel,LCROFFHK,Si3226_DCfeed_Presets[preset].lcroffhk);
	WriteRAM(pProHW,pProslic->channel,LCRDBI,Si3226_DCfeed_Presets[preset].lcrdbi);
	WriteRAM(pProHW,pProslic->channel,LONGHITH,Si3226_DCfeed_Presets[preset].longhith);
	WriteRAM(pProHW,pProslic->channel,LONGLOTH,Si3226_DCfeed_Presets[preset].longloth);
	WriteRAM(pProHW,pProslic->channel,LONGDBI,Si3226_DCfeed_Presets[preset].longdbi);
	WriteRAM(pProHW,pProslic->channel,LCRMASK,Si3226_DCfeed_Presets[preset].lcrmask);
	WriteRAM(pProHW,pProslic->channel,LCRMASK_POLREV,Si3226_DCfeed_Presets[preset].lcrmask_polrev);
	WriteRAM(pProHW,pProslic->channel,LCRMASK_STATE,Si3226_DCfeed_Presets[preset].lcrmask_state);
	WriteRAM(pProHW,pProslic->channel,LCRMASK_LINECAP,Si3226_DCfeed_Presets[preset].lcrmask_linecap);
	WriteRAM(pProHW,pProslic->channel,VCM_OH,Si3226_DCfeed_Presets[preset].vcm_oh);
	WriteRAM(pProHW,pProslic->channel,VOV_BAT,Si3226_DCfeed_Presets[preset].vov_bat);
	WriteRAM(pProHW,pProslic->channel,VOV_GND,Si3226_DCfeed_Presets[preset].vov_gnd);
	WriteReg(pProHW,pProslic->channel,LINEFEED,lf);
	return 0;
}
#endif
/*
** Function: PROSLIC_GPIOSetup
**
** Description: 
** configure gpio
*/
#ifdef DISABLE_GPIO_SETUP
#else
extern Si3226_GPIO_Cfg Si3226_GPIO_Configuration ;
int Si3226_GPIOSetup (proslicChanType *pProslic){
	uInt8 data;
	data = ReadReg(pProHW,pProslic->channel,GPIO);
	data |= Si3226_GPIO_Configuration.outputEn << 4;
	WriteReg(pProHW,pProslic->channel,GPIO,data);
	data = Si3226_GPIO_Configuration.analog << 4;
	data |= Si3226_GPIO_Configuration.direction;
	WriteReg(pProHW,pProslic->channel,GPIO_CFG1,data);
	data = Si3226_GPIO_Configuration.manual << 4;
	data |= Si3226_GPIO_Configuration.polarity;
	WriteReg(pProHW,pProslic->channel,GPIO_CFG2,data);
	data |= Si3226_GPIO_Configuration.openDrain;
	WriteReg(pProHW,pProslic->channel,GPIO_CFG3,data);
	WriteReg(pProHW,pProslic->channel,BATSELMAP,Si3226_GPIO_Configuration.batselmap);
	return 0;
}
#endif

/*
** Function: PROSLIC_PulseMeterSetup
**
** Description: 
** configure pulse metering
*/
int Si3226_PulseMeterSetup (proslicChanType *pProslic, int preset){
	/*not applicable to this part number*/
	return 0;
}

/*
** Function: PROSLIC_PCMSetup
**
** Description: 
** configure pcm
*/
#ifdef DISABLE_PCM_SETUP
#else
extern Si3226_PCM_Cfg Si3226_PCM_Presets [];
int Si3226_PCMSetup (proslicChanType *pProslic, int preset){
	uInt8 regTemp;
	
	
	if (Si3226_PCM_Presets[preset].widebandEn){
		regTemp = ReadReg(pProHW,pProslic->channel,DIGCON);
		WriteReg(pProHW,pProslic->channel,DIGCON,regTemp|0xC);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B0_1,0x27EA83L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B1_1,0x27EA83L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A1_1,0x487977EL);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B0_2,0x8000000L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B1_2,0x7E8704DL);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B2_2,0x8000000L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A1_2,0x368C302L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A2_2,0x18EBB1A4L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B0_3,0x8000000L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B1_3,0x254C75AL);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B2_3,0x7FFFFFFL);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A1_3,0x639A165L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A2_3,0x1B6738A0L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B0_1,0x4FD507L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B1_1,0x4FD507L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A1_1,0x487977EL);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B0_2,0x8000000L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B1_2,0x7E8704DL);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B2_2,0x8000000L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A1_2,0x368C302L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A2_2,0x18EBB1A4L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B0_3,0x8000000L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B1_3,0x254C75AL);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B2_3,0x7FFFFFFL);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A1_3,0x639A165L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A2_3,0x1B6738A0L);
		regTemp = ReadReg(pProHW,pProslic->channel,ENHANCE);
		WriteReg(pProHW,pProslic->channel,ENHANCE,regTemp|1);
	} else {
		regTemp = ReadReg(pProHW,pProslic->channel,DIGCON);
		WriteReg(pProHW,pProslic->channel,DIGCON,regTemp&~(0xC));
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B0_1,0x3538E80L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B1_1,0x3538E80L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A1_1,0x1AA9100L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B0_2,0x216D100L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B1_2,0x2505400L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B2_2,0x216D100L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A1_2,0x2CB8100L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A2_2,0x1D7FA500L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B0_3,0x2CD9B00L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B1_3,0x1276D00L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_B2_3,0x2CD9B00L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A1_3,0x2335300L);
		WriteRAM(pProHW,pProslic->channel,TXACIIR_A2_3,0x19D5F700L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B0_1,0x6A71D00L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B1_1,0x6A71D00L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A1_1,0x1AA9100L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B0_2,0x216D100L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B1_2,0x2505400L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B2_2,0x216D100L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A1_2,0x2CB8100L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A2_2,0x1D7FA500L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B0_3,0x2CD9B00L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B1_3,0x1276D00L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_B2_3,0x2CD9B00L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A1_3,0x2335300L);
		WriteRAM(pProHW,pProslic->channel,RXACIIR_A2_3,0x19D5F700L);
		regTemp = ReadReg(pProHW,pProslic->channel,ENHANCE);
		WriteReg(pProHW,pProslic->channel,ENHANCE,regTemp&~(1));
	}
	regTemp = Si3226_PCM_Presets[preset].pcmFormat;
	regTemp |= Si3226_PCM_Presets[preset].pcm_tri << 5;
	WriteReg(pProHW,pProslic->channel,PCMMODE,regTemp);
	regTemp = ReadReg(pProHW,pProslic->channel,PCMTXHI);
	regTemp &= 3;
	regTemp |= Si3226_PCM_Presets[preset].tx_edge<<4;
	WriteReg(pProHW,pProslic->channel,PCMTXHI,regTemp);



	return 0;
}
#endif
/*
** Function: PROSLIC_PCMSetup
**
** Description: 
** configure pcm
*/
int Si3226_PCMTimeSlotSetup (proslicChanType *pProslic, uInt16 rxcount, uInt16 txcount){
	uInt8 data;
	data = txcount & 0xff;
	WriteReg(pProHW,pProslic->channel,PCMTXLO,data);
	data = txcount >> 8 ;
	WriteReg(pProHW,pProslic->channel,PCMTXHI,data);
	data = rxcount & 0xff;
	WriteReg(pProHW,pProslic->channel,PCMRXLO,data);
	data = rxcount >> 8 ;
	WriteReg(pProHW,pProslic->channel,PCMRXHI,data);

	return 0;
}

/*
**
** PROSLIC CONTROL FUNCTIONS
**
*/



/*
** Function: PROSLIC_GetInterrupts
**
** Description: 
** Reads interrupt registers status (IRQ1-4)
*/
int Si3226_GetInterrupts (proslicChanType *pProslic,proslicIntType *pIntData){
	/*Reading the interrupt registers and will clear any bits which are set (SPI mode only)
	//Multiple interrupts may occur at once so bear that in mind when
	//writing an interrupt handling routine*/
	uInt8 data[4];
	int i,j,k;
	pIntData->number = 0;
	
	data[0] = ReadReg(pProHW,pProslic->channel,IRQ1);
	data[1] = ReadReg(pProHW,pProslic->channel,IRQ2);
	data[2] = ReadReg(pProHW,pProslic->channel,IRQ3);
	data[3] = ReadReg(pProHW,pProslic->channel,IRQ4);
#ifdef GCI_MODE
	WriteReg(pProHW,pProslic->channel,IRQ1,data[0]); /*clear interrupts (gci only)*/
	WriteReg(pProHW,pProslic->channel,IRQ2,data[1]);
	WriteReg(pProHW,pProslic->channel,IRQ3,data[2]);
	WriteReg(pProHW,pProslic->channel,IRQ4,data[3]);
#endif
		for (i=0;i<4;i++){
		for (j=0;j<8;j++){
			if (data[i]&(1<<j)){
				switch (j + (i*8)){
					case OSC1_T1_SI3226:
						k=OSC1_T1;
						break;
					case OSC1_T2_SI3226:
						k=OSC1_T2;
						break;
					case OSC2_T1_SI3226:
						k=OSC2_T1;
						break;
					case OSC2_T2_SI3226:
						k=OSC2_T2;
						break;
					case RING_T1_SI3226:
						k=RING_T1;
						break;
					case RING_T2_SI3226:
						k=RING_T2;
						break;
					case LONG_STAT_SI3226:
						k=LONG_STAT;
						break;
					case VBAT_SI3226:
						k=VBAT;
						break;
					case RING_TRIP_SI3226:
						k=RING_TRIP;
						break;
					case LOOP_STAT_SI3226:
						k=LOOP_STATUS;
						break;
					case PQ1_SI3226:
						k=PQ1;
						break;
					case PQ2_SI3226:
						k=PQ2;
						break;
					case PQ3_SI3226:
						k=PQ3;
						break;
					case PQ4_SI3226:
						k=PQ4;
						break;
					case PQ5_SI3226:
						k=PQ5;
						break;
					case PQ6_SI3226:
						k=PQ6;
						break;
					case DTMF_SI3226:
						k=DTMF;
						break;
					case INDIRECT_SI3226:
						k=INDIRECT;
						break;
					case CM_BAL_SI3226:
						k=CM_BAL;
						break;
					case FSKBUF_AVAIL_SI3226:
						k = FSKBUF_AVAIL;
						break;
					case VOC_TRACK_SI3226:
						k = VOC_TRACK;
						break;
					case TXMDM_SI3226:
						k = TXMDM;
						break;
					case RXMDM_SI3226:
						k = RXMDM;
						break;
					case RING_FAIL_SI3226:
						k = RING_FAIL;
						break;
					case USER_IRQ0_SI3226:
						k = USER_IRQ0;
						break;
					case USER_IRQ1_SI3226:
						k = USER_IRQ1;
						break;
					case USER_IRQ2_SI3226:
						k = USER_IRQ2;
						break;
					case USER_IRQ3_SI3226:
						k = USER_IRQ3;
						break;
					case USER_IRQ4_SI3226:
						k = USER_IRQ4;
						break;
					case USER_IRQ5_SI3226:
						k = USER_IRQ5;
						break;
					case USER_IRQ6_SI3226:
						k = USER_IRQ6;
						break;
					case USER_IRQ7_SI3226:
						k = USER_IRQ7;
						break;
					default:
						k=0xff;
				}
				pIntData->irqs[pIntData->number] = 	k;		
				pIntData->number++;
			
			}
		}	

	}

	return pIntData->number;
}


/*
** Function: PROSLIC_ReadHookStatus
**
** Description: 
** Determine hook status
*/
int Si3226_ReadHookStatus (proslicChanType *pProslic,uInt8 *pHookStat){
	if (ReadReg(pProHW,pProslic->channel,LCRRTP) & 2)
		*pHookStat=OFFHOOK;
	else
		*pHookStat=ONHOOK;
	return 0;
}

/*
** Function: PROSLIC_WriteLinefeed
**
** Description: 
** Sets linefeed state
*/
int Si3226_SetLinefeedStatus (proslicChanType *pProslic,uInt8 newLinefeed){
	WriteReg (pProHW, pProslic->channel, LINEFEED,newLinefeed);
	return 0;
}


int Si3226_GetLinefeedStatus (proslicChanType *pProslic,uInt8 *newLinefeed){
	*newLinefeed = ReadReg (pProHW, pProslic->channel, LINEFEED);
	return 0;
}


/*
** Function: PROSLIC_PolRev
**
** Description: 
** Sets polarity reversal state
*/
int Si3226_PolRev (proslicChanType *pProslic,uInt8 abrupt, uInt8 newPolRevState){
	uInt8 data=0;
	switch (newPolRevState){
		case POLREV_STOP:
			data = 0;
			break;
		case POLREV_START:
			data = 2;
			break;
		case WINK_START:
			data = 6;
			break;
		case WINK_STOP:
			data = 4;
			break;
	}
	if (abrupt)
		data |= 1;
	WriteReg(pProHW,pProslic->channel,POLREV,data);
	
	return 0;
}

/*
** Function: PROSLIC_GPIOControl
**
** Description: 
** Sets gpio of the proslic
*/
int Si3226_GPIOControl (proslicChanType *pProslic,uInt8 *pGpioData, uInt8 read){
	if (read)
		*pGpioData = 0xf & ReadReg(pProHW,pProslic->channel,GPIO);
	else{
		WriteReg(pProHW,pProslic->channel,GPIO,(*pGpioData)|(ReadReg(pProHW,pProslic->channel,GPIO)&0xf0));
	}
	return 0;
}

/*
** Function: PROSLIC_MWI
**
** Description: 
** implements message waiting indicator
*/
int Si3226_MWI (proslicChanType *pProslic,uInt8 lampOn){
	/*message waiting (neon flashing) requires modifications to vbath_expect and slope_vlim.
	The old values are restored to turn off the lamp. We assume all channels set up the same.
	During off-hook event lamp must be disabled manually. */
	static int32 vbath_save = 0;
	static int32 slope_vlim_save = 0;
	uInt8 hkStat; int32 slope_vlim_tmp;
	slope_vlim_tmp = ReadRAM(pProHW,pProslic->channel,SLOPE_VLIM);
	Si3226_ReadHookStatus(pProslic,&hkStat);

	if (lampOn && (hkStat == OFFHOOK) ) {/*cant neon flash during offhook*/
#ifdef ENABLE_DEBUG
		if (pProslic->debugMode)	
			LOGPRINT ("Si3226 MWI cannot operate offhook\n");
#endif
		return 1;
	}

	if (lampOn) {
		if (slope_vlim_tmp != 0x8000000L) { /*check we're not already on*/
			vbath_save = ReadRAM(pProHW,pProslic->channel,VBATH_EXPECT);
			slope_vlim_save = slope_vlim_tmp;
		}
		WriteRAM(pProHW,pProslic->channel,VBATH_EXPECT,0x7AE147AL);/*120V*/
		WriteRAM(pProHW,pProslic->channel,SLOPE_VLIM,0x8000000L);
	} else {
		if (vbath_save != 0) { /*check we saved some valid value first*/
			WriteRAM(pProHW,pProslic->channel,VBATH_EXPECT,vbath_save);
			WriteRAM(pProHW,pProslic->channel,SLOPE_VLIM,slope_vlim_save);
		}
	}

	return 0;
}

/*
** Function: PROSLIC_StartGenericTone
**
** Description: 
** start tone generators
*/
int Si3226_ToneGenStart (proslicChanType *pProslic,uInt8 timerEn){
	uInt8 data;
	data = ReadReg(pProHW,pProslic->channel,OCON);
	data |= 0x11 + (timerEn ? 0x66 : 0);//TODO
	WriteReg(pProHW,pProslic->channel,OCON,data);
	return 0;
}


/*
** Function: PROSLIC_StopTone
**
** Description: 
** Stops tone generators
**
** Input Parameters: 
** pProslic: pointer to Proslic object
**
** Return:
** none
*/
int Si3226_ToneGenStop (proslicChanType *pProslic){
	uInt8 data;
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Si3226 ToneGenStop\n");
#endif
	data = ReadReg(pProHW,pProslic->channel,OCON);
	data &= ~(0x77);
	WriteReg(pProHW,pProslic->channel,OCON,data);
	return 0;
}


/*
** Function: PROSLIC_StartRing
**
** Description: 
** start ring generator
*/
int Si3226_RingStart (proslicChanType *pProslic){
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Si3226 RingStart\n");
#endif
	Si3226_SetLinefeedStatus(pProslic,LF_RINGING);
	return 0;
}


/*
** Function: PROSLIC_StopRing
**
** Description: 
** Stops ring generator
*/
int Si3226_RingStop (proslicChanType *pProslic){
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Si3226 RingStop\n");
#endif
	Si3226_SetLinefeedStatus(pProslic,LF_FWD_ACTIVE);
	return 0;
}

/*
** Function: PROSLIC_EnableCID
**
** Description: 
** enable fsk
*/
int Si3226_EnableCID (proslicChanType *pProslic){
	uInt8 data;
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Si3226 EnableCID\n");
#endif
	WriteReg(pProHW,pProslic->channel,OCON,0);

	data = ReadReg(pProHW,pProslic->channel,OMODE);
	data |= 0xA;
	WriteReg(pProHW,pProslic->channel,OMODE,data);

	WriteReg(pProHW,pProslic->channel,OCON,0x5);
	return 0;
}

/*
** Function: PROSLIC_DisableCID
**
** Description: 
** disable fsk
*/
int Si3226_DisableCID (proslicChanType *pProslic){
	uInt8 data;
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Si3226 DisableCID\n");
#endif
	WriteReg(pProHW,pProslic->channel,OCON,0);
	data = ReadReg(pProHW,pProslic->channel,OMODE);
	data &= ~(0x8);
	WriteReg(pProHW,pProslic->channel,OMODE,data);
	return 0;
}

/*
** Function: PROSLIC_SendCID
**
** Description: 
** send fsk data
*/
int Si3226_SendCID (proslicChanType *pProslic, uInt8 *buffer, uInt8 numBytes){
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Si3226 SendCID\n");
#endif
	while (numBytes-- > 0){
		WriteReg(pProHW,pProslic->channel,FSKDAT,*(buffer++));
	}
	return 0;
}

/*
** Function: PROSLIC_StartPCM
**
** Description: 
** Starts PCM
*/
int Si3226_PCMStart (proslicChanType *pProslic){
	uInt8 data;
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Si3226 PCMStart\n");
#endif
	data = ReadReg(pProHW,pProslic->channel,PCMMODE);
	data |= 0x10;
	WriteReg(pProHW,pProslic->channel,PCMMODE,data);
	return 0;
}


/*
** Function: PROSLIC_StopPCM
**
** Description: 
** Disables PCM
*/
int Si3226_PCMStop (proslicChanType *pProslic){
	uInt8 data;
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Si3226 PCMStop\n");
#endif
	data = ReadReg(pProHW,pProslic->channel,PCMMODE);
	data &= ~(0x10);
	WriteReg(pProHW,pProslic->channel,PCMMODE,data);
	return 0;
}



/*
** Function: PROSLIC_ReadDTMFDigit
**
** Description: 
** Read DTMF digit (would be called after DTMF interrupt to collect digit)
*/
int Si3226_DTMFReadDigit (proslicChanType *pProslic,uInt8 *pDigit){
	*pDigit = ReadReg(pProHW,pProslic->channel,TONDTMF) & 0xf;
#ifdef ENABLE_DEBUG
	if (pProslic->debugMode)
		LOGPRINT("Si3226: DTMFReadDigit %d\n",*pDigit);
#endif
	
	return 0;
}

/*
** Function: PROSLIC_PLLFreeRunStart
**
** Description: 
** initiates pll free run mode
*/
int Si3226_PLLFreeRunStart (proslicChanType *pProslic){
	uInt8 tmp;
	tmp = ReadReg(pProHW,pProslic->channel,ENHANCE);
	WriteReg(pProHW,pProslic->channel,ENHANCE,tmp|0x4);
	return 0;
}

/*
** Function: PROSLIC_PLLFreeRunStop
**
** Description: 
** exit pll free run mode
*/
int Si3226_PLLFreeRunStop (proslicChanType *pProslic){
	uInt8 tmp;
	tmp = ReadReg(pProHW,pProslic->channel,ENHANCE);
	WriteReg(pProHW,pProslic->channel,ENHANCE,tmp&~(0x4));
	return 0;
}

/*
** Function: PROSLIC_PulseMeterStart
**
** Description: 
** start pulse meter tone
*/
int Si3226_PulseMeterStart (proslicChanType *pProslic){
	/*not applicable to this part number*/
	return 0;	
}

/*
** Function: PROSLIC_PulseMeterStop
**
** Description: 
** stop pulse meter tone
*/
int SI3226_PulseMeterStop (proslicChanType *pProslic){
	/*not applicable to this part number*/
	return 0;
}


int SI3226_SetUserMode(proslicChanType *pProslic, int on)
{
	setUserMode (pProslic,on);
#if 0	
	if (on == 1)
		printk("Si3226 channel %d enter user mode.\n", pProslic->channel);
	else if (on == 0)
		printk("Si3226 channel %d leave user mode.\n", pProslic->channel);
#endif
		
	return 0;
}

/*
** Function: PROSLIC_dbgSetDCFeed
**
** Description: 
** provisionary function for setting up
** dcfeed given desired open circuit voltage 
** and loop current.
*/
int Si3226_dbgSetDCFeed (proslicChanType *pProslic, uInt32 v_vlim_val, uInt32 i_ilim_val, int32 preset){
/* Note:  * needs more descriptive return codes in the event of an out of range arguement */
    uInt8 lf;
    uInt16 vslope = 160;
    uInt16 rslope = 720;
    uInt32 vscale1 = 1386; 
    uInt32 vscale2 = 1422;   /* 1386x1422 = 1970892 broken down to minimize trunc err */
    uInt32 iscale1 = 913;
    uInt32 iscale2 = 334;    /* 913x334 = 304942 */
    uInt32 i_rfeed_val, v_rfeed_val, const_rfeed_val, i_vlim_val, const_ilim_val, v_ilim_val;
    int32 signedVal;
    /* Set Linefeed to open state before modifying DC Feed */
	lf = ReadReg(pProHW,pProslic->channel,LINEFEED);
	WriteReg(pProHW,pProslic->channel,LINEFEED,0);

    /* Assumptions must be made to minimize computations.  This limits the
    ** range of available settings, but should be more than adequate for
    ** short loop applications.
    **
    ** Assumtions:
    ** 
    ** SLOPE_VLIM      =>  160ohms
    ** SLOPE_RFEED     =>  720ohms
    ** I_RFEED         =>  3*I_ILIM/4
    ** 
    ** With these assumptions, the DC Feed parameters now become 
    **
    ** Inputs:      V_VLIM, I_ILIM
    ** Constants:   SLOPE_VLIM, SLOPE_ILIM, SLOPE_RFEED, SLOPE_DELTA1, SLOPE_DELTA2
    ** Outputs:     V_RFEED, V_ILIM, I_VLIM, CONST_RFEED, CONST_ILIM
    **
    */

    /* Validate arguements */
    if((i_ilim_val < 15)||(i_ilim_val > 45)) return 1;   /* need error code */
    if((v_vlim_val < 30)||(v_vlim_val > 52)) return 1;   /* need error code */

    /* Calculate voltages in mV and currents in uA */
    v_vlim_val *= 1000;
    i_ilim_val *= 1000;

    /* I_RFEED */
    i_rfeed_val = (3*i_ilim_val)/4;

    /* V_RFEED */
    v_rfeed_val = v_vlim_val - (i_rfeed_val*vslope)/1000;

    /* V_ILIM */ 
    v_ilim_val = v_rfeed_val - (rslope*(i_ilim_val - i_rfeed_val))/1000;

    /* I_VLIM */
    i_vlim_val = (v_vlim_val*1000)/4903;

    /* CONST_RFEED */
    signedVal = v_rfeed_val * (i_ilim_val - i_rfeed_val);
    signedVal /= (v_rfeed_val - v_ilim_val);
    signedVal = i_rfeed_val + signedVal;

// signedVal in uA here
    signedVal *= iscale1;
    signedVal /= 100;
    signedVal *= iscale2;
    signedVal /= 10;

    if(signedVal < 0)
    {
        const_rfeed_val = (signedVal)+ (1L<<29);
    }
    else
    {
        const_rfeed_val = signedVal & 0x1FFFFFFF;
    }

    /* CONST_ILIM */
    const_ilim_val = i_ilim_val;

    /* compute RAM values */
    v_vlim_val *= vscale1;
    v_vlim_val /= 100;
    v_vlim_val *= vscale2;
    v_vlim_val /= 10;

    v_rfeed_val *= vscale1;
    v_rfeed_val /= 100;
    v_rfeed_val *= vscale2;
    v_rfeed_val /= 10;

    v_ilim_val *= vscale1;
    v_ilim_val /= 100;
    v_ilim_val *= vscale2;
    v_ilim_val /= 10;

    const_ilim_val *= iscale1;
    const_ilim_val /= 100;
    const_ilim_val *= iscale2;
    const_ilim_val /= 10;

    i_vlim_val *= iscale1;
    i_vlim_val /= 100;
    i_vlim_val *= iscale2;
    i_vlim_val /= 10;

	Si3226_DCfeed_Presets[preset].slope_vlim = 0x18842BD7L;
	Si3226_DCfeed_Presets[preset].slope_rfeed = 0x1E8886DEL;
	Si3226_DCfeed_Presets[preset].slope_ilim = 0x40A0E0L;
	Si3226_DCfeed_Presets[preset].delta1 = 0x1EABA1BFL;
	Si3226_DCfeed_Presets[preset].delta2 = 0x1EF744EAL;
	Si3226_DCfeed_Presets[preset].v_vlim = v_vlim_val;
	Si3226_DCfeed_Presets[preset].v_rfeed = v_rfeed_val;
	Si3226_DCfeed_Presets[preset].v_ilim = v_ilim_val;
	Si3226_DCfeed_Presets[preset].const_rfeed = const_rfeed_val;
	Si3226_DCfeed_Presets[preset].const_ilim = const_ilim_val;
	Si3226_DCfeed_Presets[preset].i_vlim = i_vlim_val;
	
	Si3226_DCFeedSetup(pProslic,preset);
	WriteReg(pProHW,pProslic->channel,LINEFEED,lf);
	return 0;
}

/*
** Function: PROSLIC_dbgSetDCFeedVopen
**
** Description: 
** provisionary function for setting up
** dcfeed given desired open circuit voltage.
** Entry I_ILIM value will be used.
*/
int Si3226_dbgSetDCFeedVopen (proslicChanType *pProslic, uInt32 v_vlim_val, int32 preset)
{
uInt32 i_ilim_val;
uInt32 iscale1 = 913;
uInt32 iscale2 = 334;    /* 913x334 = 304942 */

    /* Read present CONST_ILIM value */
    i_ilim_val = ReadRAM(pProHW,pProslic->channel,CONST_ILIM);


    i_ilim_val /= iscale2;
    i_ilim_val /= iscale1;

    return Si3226_dbgSetDCFeed(pProslic,v_vlim_val,i_ilim_val,preset);
}

/*
** Function: PROSLIC_dbgSetDCFeedIloop
**
** Description: 
** provisionary function for setting up
** dcfeed given desired loop current.
** Entry V_VLIM value will be used.
*/
int Si3226_dbgSetDCFeedIloop (proslicChanType *pProslic, uInt32 i_ilim_val, int32 preset)
{
uInt32 v_vlim_val;
uInt32 vscale1 = 1386; 
uInt32 vscale2 = 1422;   /* 1386x1422 = 1970892 broken down to minimize trunc err */

    /* Read present V_VLIM value */
    v_vlim_val = ReadRAM(pProHW,pProslic->channel,V_VLIM);

    v_vlim_val /= vscale2;
    v_vlim_val /= vscale1;

    return Si3226_dbgSetDCFeed(pProslic,v_vlim_val,i_ilim_val, preset);
}




typedef struct
{
	uInt8   freq;
	ramData ringfr;      /* trise scale for trap */
    uInt32  ampScale;
} ProSLIC_SineRingFreqLookup;

typedef struct
{
    uInt8    freq;
    ramData  rtacth;
} ProSLIC_SineRingtripLookup;

typedef struct
{
	uInt8   freq;
	uInt16  cfVal[4];
} ProSLIC_TrapRingFreqLookup;

typedef struct
{
    uInt8   freq;
    uInt32  rtacth[4];
} ProSLIC_TrapRingtripLookup;




/*
** Function: PROSLIC_dbgRingingSetup
**
** Description: 
** Provisionary function for setting up
** Ring type, frequency, amplitude and dc offset.
** Main use will be by peek/poke applications.
*/
int Si3226_dbgSetRinging (proslicChanType *pProslic, ProSLIC_dbgRingCfg *ringCfg, int preset){
//uInt32 ramVal;
//uInt8 regVal;
//uInt32 trise;
int errVal,i;
uInt32 vScale = 1608872L;   /* (2^28/170.25)*((100+4903)/4903) */
//uInt32 cfVal;
//uInt32 ipk = 60;   /* peak AC ring current in mA */

const ProSLIC_SineRingFreqLookup sineRingFreqTable[] =
/*  Freq RINGFR, vScale */
   { {20, 0x7EFD9D5L, 24800},
    {25, 0x7E6C925L, 31023},
    {30, 0x7DBB96BL, 37261},
    {35, 0x7CEAD72L, 43518},
    {40, 0x7BFA887L, 49796},
    {45, 0x7AEAE74L, 56100},
    {50, 0x70BC384L, 62432},
    {0,0,0}}; /* terminator */

const ProSLIC_SineRingtripLookup sineRingtripTable[] =
/*  Freq rtacth */
   { {20, 7713844L},
    {25, 6171075L},
    {30, 5142563L},
    {35, 8815822L},
    {40, 7713844L},
    {45, 6856750L},
    {50, 6171075L},
    {0,0L}}; /* terminator */

const ProSLIC_TrapRingFreqLookup trapRingFreqTable[] =
/*  Freq multCF12 multCF13 multCF14 multCF15*/
    {{20, {92,  122, 147,  167}},
    {25, {73,  98,  118,  133}},
    {30, {61,  82,  98,   111}},
    {35, {52,  70,  84,   95}},
    {40, {46,  61,  73,   83}},
    {45, {41,  54,  65,   74}},
    {50, {37,  49,  59,   67}},
    {0,{0L,0L,0L,0L}}}; /* terminator */

const ProSLIC_TrapRingtripLookup trapRingtripTable[] =
/*  Freq rtacthCR12 rtacthCR13 rtacthCR14 rtacthCR15 */
    {{20, {10728486L,  10261705L, 9891329L,  9592529L}},
    {25, {8582789L,  8209364L,  7913063L,  7674023L}},
    {30, {7152324L,  6841137L,  6594219L,  6395019L}},
    {35, {12261127L, 11727663L, 11304376L, 10962890L}},
    {40, {10728486L, 10261705L, 9891329L,  9592529L}},
    {45, {9536432L,  9121516L,  8792293L,  8526692L}},
    {50, {8582789L,  8209364L,  7913063L,  7674023L}},
    {0,{0L,0L,0L,0L}}}; /* terminator */

    errVal = 0;

    switch(ringCfg->ringtype)
    {
        case ProSLIC_RING_SINE:
            i=0;
            do
            {
                if(sineRingFreqTable[i].freq >= ringCfg->freq) 
                {
                    break;
                }
                i++;
            } while (sineRingFreqTable[i].freq);

            /* Set to maximum value if exceeding maximum value from table */
            if(sineRingFreqTable[i].freq == 0)
            {
                i--;
                errVal = 1;
            }

            /* Update RINGFR RINGAMP, RINGOFFSET, and RINGCON */
            Si3226_Ring_Presets[preset].freq = sineRingFreqTable[i].ringfr;
            Si3226_Ring_Presets[preset].amp = ringCfg->amp * sineRingFreqTable[i].ampScale;
            Si3226_Ring_Presets[preset].offset = ringCfg->offset * vScale;
            Si3226_Ring_Presets[preset].phas = 0L;
            /* Don't alter anything in RINGCON other that the TRAP bit */
#ifdef DISABLE_TONE_GEN_TIMERS
            Si3226_Ring_Presets[preset].ringcon &= 0xE2; 
#else
            Si3226_Ring_Presets[preset].ringcon &= 0xFA; 
#endif

            /* Setup ringtrip - fixed at 65mA */
            if(sineRingFreqTable[i].freq >32)
            {
                Si3226_Ring_Presets[preset].rtper = (6553600*2)/sineRingFreqTable[i].freq;
                Si3226_Ring_Presets[preset].rtacdb = (938040*sineRingFreqTable[i].freq)/1600;
            }
            else
            {
                Si3226_Ring_Presets[preset].rtper = 6553600/sineRingFreqTable[i].freq;
                Si3226_Ring_Presets[preset].rtacdb = (938040*sineRingFreqTable[i].freq)/800;
            }
            Si3226_Ring_Presets[preset].rtdcth = 0xFFFFFFFL;
            Si3226_Ring_Presets[preset].rtacth = sineRingtripTable[i].rtacth;
            break;

        case ProSLIC_RING_TRAP_CF12:     
        case ProSLIC_RING_TRAP_CF13: 
        case ProSLIC_RING_TRAP_CF14: 
        case ProSLIC_RING_TRAP_CF15:  
            i=0;
            do
            {
                if(trapRingFreqTable[i].freq >= ringCfg->freq) 
                {
                    break;
                }
                i++;
            } while (trapRingFreqTable[i].freq);

            /* Set to maximum value if exceeding maximum value from table */
            if(trapRingFreqTable[i].freq == 0)
            {
                i--;
                errVal = 1;
            }

            /* Update RINGFR RINGAMP, RINGOFFSET, and RINGCON */
            Si3226_Ring_Presets[preset].amp = ringCfg->amp * vScale;
            Si3226_Ring_Presets[preset].freq = Si3226_Ring_Presets[preset].amp/trapRingFreqTable[i].cfVal[ringCfg->ringtype];
            Si3226_Ring_Presets[preset].offset = ringCfg->offset * vScale;
            Si3226_Ring_Presets[preset].phas = 262144000L/trapRingFreqTable[i].freq;
            /* Don't alter anything in RINGCON other that the TRAP bit */
#ifdef DISABLE_TONE_GEN_TIMERS
            Si3226_Ring_Presets[preset].ringcon |= 0x01; 
            Si3226_Ring_Presets[preset].ringcon &= 0xE7; 
#else
            Si3226_Ring_Presets[preset].ringcon |= 0x19; 
#endif

            /* Setup ringtrip - fixed at 65mA */
            if(trapRingFreqTable[i].freq >32)
            {
                Si3226_Ring_Presets[preset].rtper = (6553600*2)/trapRingFreqTable[i].freq;
                Si3226_Ring_Presets[preset].rtacdb = 1229*trapRingFreqTable[i].freq;
            }
            else
            {
                Si3226_Ring_Presets[preset].rtper = 6553600/trapRingFreqTable[i].freq;
                Si3226_Ring_Presets[preset].rtacdb = 615*trapRingFreqTable[i].freq;
            }
            Si3226_Ring_Presets[preset].rtdcth = 0xFFFFFFFL;
            Si3226_Ring_Presets[preset].rtacth = trapRingtripTable[i].rtacth[ringCfg->ringtype];


            break;
    }

    /* Reload the modified preset */
    Si3226_RingSetup(pProslic, preset);

    return errVal;
}


