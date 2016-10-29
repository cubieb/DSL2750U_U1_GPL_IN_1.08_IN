#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/delay.h>
#ifdef CONFIG_DEFAULTS_KERNEL_2_6
#include <linux/interrupt.h>
#endif
//#include "Slic_api.h"
#include "snd_define.h"
#include "con_register.h"

#include "rtk_voip.h"
#include "spi.h"
#include "voip_types.h"
#include "voip_control.h"
//#include "Daa_api.h"

#include "snd_proslic_type.h"

#ifdef CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x
#define CHAN_PER_DEVICE 1
#ifdef CONFIG_RTK_VOIP_MULTIPLE_SI32178
#define NUM_OF_DAA	SLIC_CH_NUM
//#define DAA_CHANNEL_OFFSET	SLIC_CH_NUM
#elif defined (CONFIG_RTK_VOIP_SLIC_SI32176_SI32178_CS)
#define NUM_OF_DAA 1		/* It MUST be one!! */
//#define DAA_CHANNEL_OFFSET	SLIC_CH_NUM
#else
#define NUM_OF_DAA 1
//#define DAA_CHANNEL_OFFSET 1
#endif
#elif defined (CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3226)
#define CHAN_PER_DEVICE 2
#endif

#if defined (CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3226) || defined( CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x )
#define NUMBER_OF_CHAN SLIC_CH_NUM
#define NUMBER_OF_PROSLIC (NUMBER_OF_CHAN/CHAN_PER_DEVICE)
#if NUMBER_OF_PROSLIC < 1
#define NUMBER_OF_PROSLIC 1
#endif

#define TOTAL_NUM_OF_CH		( NUM_OF_DAA + NUMBER_OF_CHAN )

#if defined( CONFIG_RTK_VOIP_MULTIPLE_SI32178 ) || \
	defined( CONFIG_RTK_VOIP_SLIC_SI32176_SI32178_CS )
#define NUMBER_OF_HWINTF	NUMBER_OF_PROSLIC
#else
#define NUMBER_OF_HWINTF	1
#endif

//typedef struct chanStatus chanState; //forward declaration
//typedef void (*procState) (chanState *pState, ProslicInt eInput);

/*
** structure to hold state information for pbx demo
*/
//struct chanStatus {
//	proslicChanType *ProObj;
	//timeStamp onHookTime;
	//timeStamp offHookTime;
	//procState currentState;
	//uInt16 digitCount;
	//uInt8 digits[20];
	//uInt8 ringCount;
	//uInt16 connectionWith;
	//uInt16 powerAlarmCount;
	//pulseDialType pulseDialData;
	//BOOLEAN eventEnable;
//} ;

//static ProslicDeviceType *ProSLICDevices[NUMBER_OF_PROSLIC]; //proslic device object
//static proslicChanType_ptr arrayOfProslicChans[NUMBER_OF_CHAN]; //used for initialization only
//static chanState ports[NUMBER_OF_CHAN];  //declare channel state structures
//static proslicChanType *pSlic;	// local access temporal only 

#endif


//#if defined (CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3226) || defined (CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x)
//#if defined (CONFIG_RTK_VOIP_MULTIPLE_SI32178) || defined (CONFIG_RTK_VOIP_SLIC_SI32176_SI32178_CS)
//static ctrl_S spiGciObj[NUMBER_OF_PROSLIC]; //spi interface object
//static controlInterfaceType *ProHWIntf[NUMBER_OF_PROSLIC]; //proslic hardware interface object
//#else
//static ctrl_S spiGciObj; //spi interface object
//systemTimer_S timerObj;    //timer object
//static controlInterfaceType *ProHWIntf; //proslic hardware interface object
//#endif
//#endif

#if defined (CONFIG_RTK_VOIP_SLIC_SI32178) || defined (CONFIG_RTK_VOIP_SLIC_SI32176_SI32178)
//static vdaaChanType* daas[NUM_OF_DAA];
#endif

__attribute__ ((section(".snd_desc_bss")))
static ctrl_S gSpiGciObj[ NUMBER_OF_HWINTF ];
__attribute__ ((section(".snd_desc_bss")))
static ProslicContainer_t gProslicContainer[ TOTAL_NUM_OF_CH ];
__attribute__ ((section(".snd_desc_bss")))
static voip_snd_t snd_proslic[ TOTAL_NUM_OF_CH ];

#ifdef CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x
static int SLIC_init(int pcm_mode, int initonly);
static void si3217x_init(const voip_snd_t p_snd[], ProslicContainer_t container[], int size, int pcm_mode);
static void si3217x_reset(unsigned int law);
#endif

#define ts2count( ts )	( 1 + ( ts ) * 8 )		// Time slot to silab's count 

//static inline void SLIC_read_reg(unsigned char chid, unsigned int num, unsigned char *val);
//static inline void SLIC_write_reg(unsigned char chid, unsigned char num, unsigned char val);
//static inline void SLIC_read_ram(unsigned char chid, unsigned short num, unsigned int *val);
//static inline void SLIC_write_ram(unsigned char chid, unsigned short num, unsigned int val);


//static char slic_choice_flag = 1;

//extern int Hook_Polling_Silicon(hook_struck *hook, unsigned int flash_hook_duration);
//extern unsigned char Hook_Polling_Legerity(hook_struck *hook, unsigned int flash_hook_duration);

static void SLIC_reset_si3217x(voip_snd_t *this, int codec_law)
{
	si3217x_reset(codec_law);
}

#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
#ifndef CONFIG_RTK_VOIP_DRIVERS_VIRTUAL_DAA
#if defined (CONFIG_RTK_VOIP_SLIC_SI32178) || defined (CONFIG_RTK_VOIP_SLIC_SI32176_SI32178)

static int initializeDAA(const voip_snd_t p_snds[], vdaaChanType* port[], proslicChanType* slics[], int pcm_mode){
	
	const unsigned char i = 0;
	
	

	
	
	//for (i=0;i<NUM_OF_DAA;i++){
		Vdaa_setSWDebugMode (port[i],TRUE);

	//}
	if (Vdaa_Init(port,1/*NUM_OF_DAA*/))
	{
		PRINT_R("ERROR: Initialization failed\n");
		return 1;
	}

	//for (i=0;i<NUM_OF_DAA;i++){
	
		//Vdaa_EnableInterrupts(port[i]);
#if 0 //def CONFIG_RTK_VOIP_MULTIPLE_SI32178
		Vdaa_PCMTimeSlotSetup(port[i], 1+2*(i+DAA_CHANNEL_OFFSET)*8, 1+2*(i+DAA_CHANNEL_OFFSET)*8);
#else
		//Vdaa_PCMTimeSlotSetup(port[i], 1+2*SLIC_CH_NUM*8, 1+2*SLIC_CH_NUM*8);
		Vdaa_PCMTimeSlotSetup(port[i], ts2count( p_snds[ i ].TS1 ), ts2count( p_snds[ i ].TS1 ));
#endif
		//printk("fxo-%d: 0x%p, %d, %d\n", i, port[i], 1+(i+DAA_CHANNEL_OFFSET)*8, 1+(i+DAA_CHANNEL_OFFSET)*8);
		
		switch(pcm_mode)
		{
			case 0:// linear
				Vdaa_PCMSetup(port[i], 2, 1/* enable PCM */);
				break;
			case 1:// a-law
				Vdaa_PCMSetup(port[i], 0, 1/* enable PCM */);
				break;
			case 2:// u-law
				Vdaa_PCMSetup(port[i], 1, 1/* enable PCM */);
				break;
			default:// a-law
				Vdaa_PCMSetup(port[i], 0, 1/* enable PCM */);
				break;
		}
	//}
	
	return 0;
}

#endif
#endif
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x

//#define SI32178_PFD

static void si3217x_alloc_objs( ProslicContainer_t container[], 
		int size, int devices, int chan )
{
	// once call this function, decide:
	//  - one control interface 
	//  - n devices (chip)
	//  - m proslic channel (FXS)
	//  - at most one daa (if size-chan==1 )
	int i, j;
	controlInterfaceType *ProHWIntf;
	ProslicDeviceType *ProSLICDevices;
	const int oneDaaIdx = ( size - chan == 1 ? size - 1 : -1 );
	
	// check some thing 
	if( size <= 0 || size < devices || size < chan || 
		( size != chan && size != ( chan + 1 ) ) ) 
	{
		printk( "Si3217x allocate objects error!!\n" );
		printk( "size=%d, devicdes=%d, chan=%d\n", size, devices, chan );
		return;
	}
	
	// init user
	for( i = 0; i < size; i ++ ) {
		container[ i ].user.hookStatus = INVALID_HOOK_STATUS;
	}
	
	// one control interface 
	ProSLIC_createControlInterface( &ProHWIntf );
	
	for( i = 0; i < size; i ++ )
		container[ i ].ProHWIntf = ProHWIntf;
	
	// n device 
	for( i = 0, j = CHAN_PER_DEVICE; i < chan; i ++, j ++ ) {
		if( j == CHAN_PER_DEVICE ) {
			j = 0;
			ProSLIC_createDevice( &ProSLICDevices );
		} 

		container[ i ].ProSLICDevices = ProSLICDevices;		
	}
	
	if( oneDaaIdx >= 0 )
		container[ oneDaaIdx ].ProSLICDevices = ProSLICDevices;	// DAA
	
	// m proslic channel
	for( i = 0; i < chan; i ++ ) {
		ProSLIC_createChannel( &container[ i ].ProObj );
		ProSLIC_SWInitChan( container[ i ].ProObj, i, SI3217X_TYPE, container[ i ].ProSLICDevices, container[ i ].ProHWIntf );
	}
	
	// at most one daa 
	if( oneDaaIdx >= 0 ) {
		Vdaa_createChannel( &container[ oneDaaIdx ].daas );
		Vdaa_SWInitChan( container[ oneDaaIdx ].daas, oneDaaIdx, SI3217X_TYPE, container[ oneDaaIdx ].ProSLICDevices, container[ oneDaaIdx ].ProHWIntf );
	}
}

static void si3217x_init( const voip_snd_t p_snds[],
		ProslicContainer_t container[],
		int size, int pcm_mode)
{
	// container includes at most one daa
	int i=0;
	//int ret = 0;
	extern int SPI_Init (ctrl_S *hSpi);
	unsigned char reg_val;

	extern unsigned char R_reg_dev(rtl_spi_dev_t *pdev, unsigned char chid, unsigned char regaddr);
	extern void W_reg_dev(rtl_spi_dev_t *pdev, unsigned char chid, unsigned char regaddr, unsigned char data);
	extern void W_ram_dev(rtl_spi_dev_t *pdev, unsigned char chid, unsigned short reg, unsigned int data);
	extern int ctrl_ResetWrapper (ctrl_S *hSpiGci, int status);
	extern int ctrl_WriteRegisterWrapper (ctrl_S *hSpiGci, uInt8 channel, uInt8 regAddr, uInt8 data);
	extern uInt8 ctrl_ReadRegisterWrapper (ctrl_S *hSpiGci, uInt8 channel, uInt8 regAddr);
	extern int ctrl_WriteRAMWrapper (ctrl_S *hSpiGci, uInt8 channel, uInt16 ramAddr, ramData data);	
	extern ramData ctrl_ReadRAMWrapper (ctrl_S *hSpiGci, uInt8 channel, uInt16 ramAddr);
	extern int time_DelayWrapper (void *hTimer, int timeInMs);	
	extern int time_TimeElapsedWrapper (void *hTimer, timeStamp *startTime, int *timeInMs);
	extern int time_GetTimeWrapper (void *hTimer, timeStamp *time);	
	
	ctrl_S * const spiGciObj = container[ 0 ].spiGciObj;
	controlInterfaceType * const ProHWIntf = container[ 0 ].ProHWIntf;
	proslicChanType * const ProObj0 = container[ 0 ].ProObj;
	
	const int daaIdx = ( container[ size - 1 ].daas ? size - 1 : -1 );
	const int prochan_num = ( daaIdx < 0 ? size : size - 1 );
	
	proslicChanType_ptr arrayOfProslicChans[ TOTAL_NUM_OF_CH ];
	proslicChanType *pSlic;
	
	//printk ("\n<<<<<<<<<<< Si3217x Driver Version %s >>>>>>>>>>\n", ProSLIC_Version());

	printk("Start SLIC init....\n");

//#if defined (CONFIG_RTK_VOIP_MULTIPLE_SI32178) || defined(CONFIG_RTK_VOIP_SLIC_SI32176_SI32178_CS)
//	for (i=0; i < NUMBER_OF_CHAN; i++)
//	{
//		spiGciObj[i].portID = i;
//		SPI_Init (&spiGciObj[i]);
//	}
//#else
//	spiGciObj.portID = 0; 	// SPI GPIO set
	SPI_Init (spiGciObj);	// Initialize SPI interface(including SLIC reset)
//#endif

	//printk("3: ports[0].ProObj = 0x%p\n", ports[0].ProObj);
	
	/*** Initialize timer ***/
	//TimerInit(&timerObj);

#if 0	//defined(CONFIG_RTK_VOIP_MULTIPLE_SI32178) || defined(CONFIG_RTK_VOIP_SLIC_SI32176_SI32178_CS)
	for (i=0;i<NUMBER_OF_CHAN;i++)
	{
		ProSLIC_setControlInterfaceCtrlObj (ProHWIntf[i], &spiGciObj[i]);
		//PRINT_Y("ProHWIntf[%d] = 0x%p\n", i, ProHWIntf[i]);
		//PRINT_Y("ProHWIntf[%d]->hCtrl = 0x%p\n", i, ProHWIntf[i]->hCtrl);

		/*** Set control functions ***/
		ProSLIC_setControlInterfaceReset (ProHWIntf[i], ctrl_ResetWrapper);
		ProSLIC_setControlInterfaceWriteRegister (ProHWIntf[i], ctrl_WriteRegisterWrapper);
		ProSLIC_setControlInterfaceReadRegister (ProHWIntf[i], ctrl_ReadRegisterWrapper);
		ProSLIC_setControlInterfaceWriteRAM (ProHWIntf[i], ctrl_WriteRAMWrapper);
		ProSLIC_setControlInterfaceReadRAM (ProHWIntf[i], ctrl_ReadRAMWrapper);
	
		/*** Set timer functions ***/
		//ProSLIC_setControlInterfaceTimerObj (ProHWIntf[i], &timerObj);
		ProSLIC_setControlInterfaceDelay (ProHWIntf[i], time_DelayWrapper);
		ProSLIC_setControlInterfaceTimeElapsed (ProHWIntf[i], time_TimeElapsedWrapper);
		ProSLIC_setControlInterfaceGetTime (ProHWIntf[i], time_GetTimeWrapper);
		ProSLIC_setControlInterfaceSemaphore (ProHWIntf[i], NULL);
		ProSLIC_Reset(ports[i].ProObj);	//Reset the ProSLIC(s) before we begin
	}
#else
	ProSLIC_setControlInterfaceCtrlObj (ProHWIntf, spiGciObj);

	/*** Set control functions ***/
	ProSLIC_setControlInterfaceReset (ProHWIntf, ctrl_ResetWrapper);
	ProSLIC_setControlInterfaceWriteRegister (ProHWIntf, ctrl_WriteRegisterWrapper);
	ProSLIC_setControlInterfaceReadRegister (ProHWIntf, ctrl_ReadRegisterWrapper);
	ProSLIC_setControlInterfaceWriteRAM (ProHWIntf, ctrl_WriteRAMWrapper);
	ProSLIC_setControlInterfaceReadRAM (ProHWIntf, ctrl_ReadRAMWrapper);

	/*** Set timer functions ***/
	//ProSLIC_setControlInterfaceTimerObj (ProHWIntf, &timerObj);
	ProSLIC_setControlInterfaceDelay (ProHWIntf, time_DelayWrapper);
	ProSLIC_setControlInterfaceTimeElapsed (ProHWIntf, time_TimeElapsedWrapper);
	ProSLIC_setControlInterfaceGetTime (ProHWIntf, time_GetTimeWrapper);
	ProSLIC_setControlInterfaceSemaphore (ProHWIntf, NULL);
	ProSLIC_Reset(ProObj0);	//Reset the ProSLIC(s) before we begin
#endif
	
	for (i=0;i<prochan_num;i++)
	{
		//ProSLIC_setSWDebugMode (ports[i].ProObj, TRUE);	// turn on debug message
		proslicChanType * const ProObj = container[ i ].ProObj;

		arrayOfProslicChans[i] = ProObj;		//create array of channel pointers (for init)		
		ProSLIC_setSWDebugMode (ProObj, FALSE);	// turn off debug message
	}

	/*** Always wirte ram 1571 first before any SPI access if PFD is enabled. ***/
#ifdef SI32178_PFD
	
	for (i=0;i<NUMBER_OF_CHAN;i++)
	{
		printk("Enable PFD mode for Si32178 #%d\n", i);
		W_reg_dev(&spiGciObj ->spi_dev, i, 1, 0x1);		// Reset SLIC Channel
		mdelay(300);
		
		pSlic = ports[i].ProObj;
		ProSLIC_SetUserMode(pSlic, 1);	// enable user mode
		W_ram(&spiGciObj ->spi_dev, i, 1571, 0x200000);	// set register 80 to 0x1F
		ProSLIC_SetUserMode(pSlic, 0);	// disable user mode
	}
#endif

	/*** Get SLIC ID ***/
	for (i=0; i<prochan_num; i++)
	{
		printk("SLIC%d ID: ", i);
		reg_val = R_reg_dev(&spiGciObj ->spi_dev, i, 0);
	
		switch ((reg_val&0x38)>>3)
		{
			case 0:
				printk("Si32171 ");
				break;
			case 3:
				printk("Si32175 ");
				break;
			case 4:
				printk("Si32176 ");
				break;
			case 5:
				printk("Si32177 ");
				break;
			case 6:
				printk("Si32178 ");
				break;
			default:
				PRINT_R("Unknow SLIC ");
				break;
		}

		
		if ((reg_val&0x07) == 0)
			printk("Revision A\n");
		else if ((reg_val&0x07) == 1)
			printk("Revision B\n");
		else if ((reg_val&0x07) == 2)
			printk("Revision C\n");
		else
			PRINT_R("Unknow Revision\n");
		
	}

	if (ProSLIC_Init(arrayOfProslicChans,prochan_num))
	{
		PRINT_R ("ERROR: Si3217x SLIC Initialization failed\n");
		goto si3217x_err;
	}


	/*** Initialize the channel state for each channel ***/
	//for (i=0;i<NUMBER_OF_CHAN;i++){
	//	pSlic = ports[i].ProObj;
		//InitStateMachine(&(ports[i])); //initialize the call state machine
		//ProSLIC_InitializeDialPulseDetect(&(ports[i].pulseDialData),&(ports[i].offHookTime),&(ports[i].onHookTime));
	//}

	for (i=0; i<prochan_num; i++)
	{
		// Check master state global  register
		reg_val = R_reg_dev(&spiGciObj ->spi_dev, i, 3);
		
		if (reg_val != 0x1F)
			PRINT_Y("SLIC%d warning! master state = 0x%x (should be 0x1F )\n", i, reg_val);
		else
			printk("SLIC%d master state check pass!\n", i);
	}

	for (i=0;i<prochan_num;i++)
	{
		pSlic = container[i].ProObj;
//#define SLIC_LOOP_TEST
#ifdef SLIC_LOOP_TEST
		if (i == 0)
			ProSLIC_PCMTimeSlotSetup(pSlic, 1, 17);
		else if (i == 1)
			ProSLIC_PCMTimeSlotSetup(pSlic, 17, 1);
		//ProSLIC_PCMSetup(pSlic, 0);//linear
		ProSLIC_PCMSetup(pSlic, 1);//a-law
		//ProSLIC_PCMSetup(pSlic, 2);//u-law
#else
		if (pSlic->debugMode)
			PRINT_MSG("SLIC PCM Setup for CH%d ...", i);
		//ProSLIC_PCMTimeSlotSetup(pSlic, 1+2*i*8, 1+2*i*8);
		ProSLIC_PCMTimeSlotSetup(pSlic, ts2count( p_snds[ i ].TS1 ), ts2count( p_snds[ i ].TS1 ));
		ProSLIC_PCMSetup(pSlic, pcm_mode);
		ProSLIC_PCMStart(pSlic); //enable SLIC pcm
		if (pSlic->debugMode)
			PRINT_MSG("OK\n");
#endif		


		ProSLIC_RingSetup(pSlic, 2);	// 20Hz, 48VRMS
		ProSLIC_DCFeedSetup(pSlic, 0);
		
		if (pSlic->debugMode)
			PRINT_MSG("Set to LF_FWD_ACTIVE\n");
		ProSLIC_SetLinefeedStatus(pSlic, LF_FWD_ACTIVE);

#if 0
		/* Disable Si3217x Automatic Common Mode Control */
		ProSLIC_SetUserMode(pSlic, 1);	// enable user mode
		W_reg_dev(&spiGciObj ->spi_dev, i, 80, R_reg(&spiGciObj ->spi_dev, i, 80)&(~0x20));
		ProSLIC_SetUserMode(pSlic, 0);	// disable user mode
		if (pSlic->debugMode)
			PRINT_MSG("Disable Si3217x Automatic Common Mode Control[%d].\n", i);
#else
		W_ram_dev(&spiGciObj ->spi_dev, i, 750, 0x624F00); // skip Vbat tracking below 6V Vtr variation
#endif


#ifdef SI32178_PFD
		W_reg_dev(&spiGciObj ->spi_dev, i, 126, 0x2);
		W_reg_dev(&spiGciObj ->spi_dev, i, 126, 0x6);
		W_reg_dev(&spiGciObj ->spi_dev, i, 126, 0x10);
#endif
	}

	printk("Init OK!\n");

	if( daaIdx < 0 )
		goto label_skip_init_daa;

#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
#ifndef CONFIG_RTK_VOIP_DRIVERS_VIRTUAL_DAA

	/* Enable DAA channel */	
	// Channel 0
#if 0//def CONFIG_RTK_VOIP_MULTIPLE_SI32178
	for (i=0; i<NUMBER_OF_CHAN; i++)
	{
		W_reg_dev(&spiGciObj ->spi_dev, i, 74, 0x20);		//reset the DAA
		mdelay(500);
		W_reg_dev(&spiGciObj ->spi_dev, i, 74, 0x01);		//enable the DAA
	}
#else
	W_reg_dev(&spiGciObj ->spi_dev, prochan_num - 1, 74, 0x20);	//reset the DAA
	mdelay(500);
	W_reg_dev(&spiGciObj ->spi_dev, prochan_num - 1, 74, 0x01);	//enable the DAA
#endif

	// Si3217x Rev 0.1 Daa driver
	printk("Start DAA init....\n");
	if (initializeDAA( &p_snds[ daaIdx ], &container[ daaIdx ].daas, pSlic, pcm_mode))
	{
		PRINT_R ("ERROR: Si3217x DAA Initialization failed\n");
		goto si3217x_err;
	}

#if 0//def CONFIG_RTK_VOIP_MULTIPLE_SI32178
	for (i=0; i<NUMBER_OF_CHAN; i++)
		DAA_OnHook_Line_Monitor_Enable(i+DAA_CHANNEL_OFFSET);
#else
	//DAA_OnHook_Line_Monitor_Enable(SLIC_CH_NUM);
#endif

label_skip_init_daa:

	for( i = 0; i < size; i ++ ) {
	
		voip_snd_t * const p_snd = &p_snds[ i ];
		
		if( p_snd ->snd_type != SND_TYPE_DAA )
			continue;
		
		p_snd ->daa_ops ->DAA_OnHook_Line_Monitor_Enable( p_snd );
	}
	
	printk("Init OK!\n");

#endif
#endif
	//printk("<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>\n\n");

	return;
si3217x_err:
	PRINT_R("Init error!\n");
	mdelay(3000);
}

static void si3217x_reset(unsigned int law)
{
	// This function will cause system reset, if watch dog is enable!
	// Because calibration need mdelay(1900).
	unsigned long flags;
	save_flags(flags); cli();
	*(volatile unsigned long *)(0xB800311c) &=  0xFFFFFF;	// Disable watch dog
	*(volatile unsigned long *)(0xB800311c) |=  0xA5000000;
	restore_flags(flags);
	
	SLIC_init( law, 1 /* don't allocate */ );
	//si3217x_init(snd_proslic, gProslicContainer, TOTAL_NUM_OF_CH, law);
	
	save_flags(flags); cli();
	*(volatile unsigned long *)(0xB800311c) &=  0xFFFFFF;	// Enable watch dog
	*(volatile unsigned long *)(0xB800311c) |=  1 << 23;
	restore_flags(flags);

}

#endif // CONFIG_RTK_VOIP_DRIVERS_SLIC_SI32178

#define PHONE_CHECK_CONNECT_COUNT	11	// for Si3217x on RTL8972B
#define PHONE_CHECK_TIME_OUT_COUNT	60
#define VOLT_TR_THRESHOL	0x3000000	
#define VOLT_TIP_THRESHOL	0x3300000

static const uint32 pin_cs[] = {
	PIN_CS1, 
#ifdef PIN_CS2
	PIN_CS2, 
#ifdef PIN_CS4
	PIN_CS3, 
	PIN_CS4, 
#ifdef PIN_CS8
	PIN_CS5, 
	PIN_CS6, 
	PIN_CS7, 
	PIN_CS8, 
#endif
#endif
#endif
};

CT_ASSERT( ( sizeof( pin_cs ) / sizeof( pin_cs[ 0 ] ) ) >= NUMBER_OF_HWINTF );

static int SLIC_init(int pcm_mode, int initonly)
{
	//int ch;
	extern int slic_ch_num;
	int valid_ch = slic_ch_num;
	int i, j, sidx;
	int i_size, i_device, i_channel;
	rtl_spi_dev_t *spi_devs[ NUMBER_OF_HWINTF ];
	
	printk ("\n<<<<<<<<<<< Si3217x Driver Version %s >>>>>>>>>>\n", ProSLIC_Version());
	
	if( !initonly ) {
		// init spi first, because reset pin will affect all SLIC 
		printk( "Preparing spi channel for SLIC...\n" );
		
		for( i = 0; i < NUMBER_OF_HWINTF; i ++ ) {
			spi_devs[ i ] = &gSpiGciObj[ i ].spi_dev;
		}

		init_spi_channels( NUMBER_OF_HWINTF, spi_devs, pin_cs, PIN_RESET1, PIN_CLK, PIN_DO, PIN_DI);
	}
	
#if 1
	for( i = 0, sidx = 0; i < NUMBER_OF_HWINTF; i ++ ) {
		
		printk( "------------------------------\n" );
		printk( "SLIC HW intf %d starting at %d\n", i, sidx );
		
  #if defined( CONFIG_RTK_VOIP_MULTIPLE_SI32178 )
		i_size = CHAN_PER_DEVICE + 1;	// number of FXS/FXO port 
		i_device = 1;					// number of chip 
		i_channel = CHAN_PER_DEVICE;	// number of FXS port 
  #elif defined( CONFIG_RTK_VOIP_SLIC_SI32176_SI32178_CS )
		i_size = ( i == NUMBER_OF_HWINTF - 1 ? 
					CHAN_PER_DEVICE + 1 :
					CHAN_PER_DEVICE );
		i_device = 1;
		i_channel = CHAN_PER_DEVICE;
  #else
		i_size = TOTAL_NUM_OF_CH;
		i_device = NUMBER_OF_PROSLIC;
		i_channel = NUMBER_OF_CHAN;
  #endif
		
		if( initonly )
			goto label_do_init_only;
			
		// create objs
		//init_spi_channel( &gSpiGciObj[ i ].spi_dev, pin_cs[ i ], PIN_RESET1, PIN_CLK, PIN_DO, PIN_DI);
		
		for( j = 0; j < i_size; j ++ ) {
			gProslicContainer[ sidx + j ].spiGciObj = &gSpiGciObj[ i ];

			gProslicContainer[ sidx + j ].line.connect_count = PHONE_CHECK_CONNECT_COUNT;
			gProslicContainer[ sidx + j ].line.timeout_count = PHONE_CHECK_TIME_OUT_COUNT;
			gProslicContainer[ sidx + j ].line.TR_thres = VOLT_TR_THRESHOL;
			gProslicContainer[ sidx + j ].line.TIP_thres = VOLT_TIP_THRESHOL;
		}
		
		si3217x_alloc_objs( &gProslicContainer[ sidx ], 
			i_size, i_device, i_channel );

		// init proslic 
label_do_init_only:
		si3217x_init( &snd_proslic[ sidx ], &gProslicContainer[ sidx ], 
						i_size, pcm_mode );
		
		sidx += i_size;
	}
#else
	// create objs
	//gSpiGciObj[ 0 ].portID = 0;
	init_spi_channel( &gSpiGciObj[ 0 ].spi_dev, PIN_CS1, PIN_RESET1, PIN_CLK, PIN_DO, PIN_DI);
	
	for( i = 0; i < TOTAL_NUM_OF_CH; i ++ )
		gProslicContainer[ i ].spiGciObj = &gSpiGciObj[ 0 ];
	
	si3217x_alloc_objs( gProslicContainer, 
		TOTAL_NUM_OF_CH, NUMBER_OF_PROSLIC, TOTAL_NUM_OF_CH - NUM_OF_DAA );
	si3217x_init( snd_proslic, gProslicContainer, TOTAL_NUM_OF_CH, pcm_mode );
#endif

	printk("<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>\n\n");
	
	return valid_ch > 0 ? 0 : -1;
}

/* mode: 0-type 1 (on-hook), 1-type 2(off-hook)*/
static void CID_for_FSK_HW(unsigned int chid, char mode, unsigned char msg_type, char *str, char *str2, char *cid_name)
{
	//FIXME: chmap
	//fsk_cid_state[chid]=1;
}


static void FXS_Ring_si3217x(voip_snd_t *this, unsigned char ringset )
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	if (ringset)
		ProSLIC_RingStart(pSlic);
	else
		ProSLIC_RingStop(pSlic);
}

static unsigned char FXS_Check_Ring_si3217x(voip_snd_t *this, unsigned char *ring)
{
	unsigned char ringer; //0: ring off, 1: ring on
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;

	//pSlic = ports[chid].ProObj;
	ProSLIC_GetLinefeedStatus(pSlic, &ringer);
	if ((ringer&0x70)==0x40)
		ringer = 1;
	else
		ringer = 0;

	return ringer;
}

static void Set_SLIC_Tx_Gain_si3217x(voip_snd_t *this, unsigned char tx_gain)
{
	printk("Not implemented! Support unity gain only!\n");
}

static void Set_SLIC_Rx_Gain_si3217x(voip_snd_t *this, unsigned char rx_gain)
{
	printk("Not implemented! Support unity gain only!\n");
}

static void SLIC_Set_Ring_Cadence_si3217x(voip_snd_t *this, unsigned short OnMsec, unsigned short OffMsec)
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	
	//pSlic = ports[chid].ProObj;
	ProSLIC_Set_Ring_Cadence_ON(pSlic, OnMsec);
	ProSLIC_Set_Ring_Cadence_OFF(pSlic, OffMsec);

}

static void SLIC_Set_Ring_Freq_Amp_si3217x(voip_snd_t *this, char preset)
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	ProSLIC_RingSetup(pSlic, preset);
}

static void SLIC_Set_Impendance_Country_si3217x(voip_snd_t *this, unsigned short country, unsigned short impd)
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	ProSLIC_Set_Impendance_Silicon(pSlic, country, impd);
	PRINT_MSG("Set SLIC impedance according to the country...\n");
}

static void SLIC_Set_Impendance_si3217x(voip_snd_t *this, unsigned short preset)
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	ProSLIC_Set_Impendance(pSlic, preset);
}

#if 0
static void SLIC_GenProcessTone(unsigned int chid, genTone_struct *gen_tone)
{
}
#endif


static void OnHookLineReversal_si3217x(voip_snd_t *this, unsigned char bReversal) //0: Forward On-Hook Transmission, 1: Reverse On-Hook Transmission
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	if (bReversal)
		ProSLIC_SetLinefeedStatus(pSlic, LF_REV_OHT);
	else
		ProSLIC_SetLinefeedStatus(pSlic, LF_FWD_OHT);
}

static void SLIC_Set_LineVoltageZero_si3217x(voip_snd_t *this)
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	ProSLIC_SetLinefeedStatus(pSlic, LF_OPEN);
	//ProSLIC_SetLinefeedStatus(pSlic, LF_TIP_OPEN);
	//ProSLIC_SetLinefeedStatus(pSlic, LF_RING_OPEN);
}

static uint8 SLIC_CPC_Gen_si3217x(voip_snd_t *this)
{
#if 0	// con_polling.c: SLIC_CPC_Gen_cch() do this 
	extern void HookPollingDisable(int cch);

	if (slic_cpc[chid].cpc_start != 0)
	{
		PRINT_R("SLIC CPC gen not stop, ch=%d\n", chid);
		return;
	}
#endif
	uint8 pre_linefeed;
	extern void W_reg_dev(rtl_spi_dev_t *pdev, unsigned char chid, unsigned char regaddr, unsigned char data);

	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	ProSLIC_GetLinefeedStatus(pSlic, &(pre_linefeed)); // save current linefeed status
	//printk("get linefeed=0x%x\n", (slic_cpc[chid].pre_linefeed)&0xF);
#if 1
	ProSLIC_SetLinefeedStatus(pSlic, LF_OPEN);
	//ProSLIC_SetLinefeedStatus(pSlic, LF_TIP_OPEN);
	//ProSLIC_SetLinefeedStatus(pSlic, LF_RING_OPEN);
#else
	W_reg_dev(pdev, chid, 30, 0);
#endif
	
	return pre_linefeed;

#if 0	// con_polling.c: SLIC_CPC_Gen_cch() do this 
	slic_cpc[chid].cpc_timeout = jiffies + (HZ*time_in_ms_of_cpc_signal/1000);
	slic_cpc[chid].cpc_start = 1;
	slic_cpc[chid].cpc_stop = 0;
	//HookPollFlag[chid] = 0; // disable hook pooling
	HookPollingDisable( chid );
#endif
}

static void SLIC_CPC_Check_si3217x(voip_snd_t *this, uint8 pre_linefeed)	// check in timer
{
#if 0	// con_polling.c: ENTRY_SLIC_CPC_Polling() do this 
	extern void HookPollingEnable(int cch);
	
	if (slic_cpc[chid].cpc_start == 0)
		return;
#endif

	extern void W_reg_dev(rtl_spi_dev_t *pdev, unsigned char chid, unsigned char regaddr, unsigned char data);
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	
	// Stop wink function
#if 0
	if ((slic_cpc[chid].cpc_stop == 0) && (time_after(jiffies, slic_cpc[chid].cpc_timeout)))
#endif
	{
		//pSlic = ports[chid].ProObj;
		//printk("set linefeed=0x%x\n", (slic_cpc[chid].pre_linefeed)&0xF);
		switch((pre_linefeed)&0xF)
		{
#if 1
			case 1:
				ProSLIC_SetLinefeedStatus(pSlic, LF_FWD_ACTIVE);	// restore linefeed status
				break;
			case 5:
				ProSLIC_SetLinefeedStatus(pSlic, LF_REV_ACTIVE);	// restore linefeed status
				break;
			default:
				ProSLIC_SetLinefeedStatus(pSlic, LF_FWD_ACTIVE);	// restore linefeed status
				printk("pre_linefeed = 0x%x, Set default linefeed status to LF_FWD_ACTIVE\n", (pre_linefeed)&0xF);
				break;
#else
			case 1:
				W_reg_dev(pdev, chid, 30, 1);
				break;
			case 5:
				W_reg_dev(pdev, chid, 30, 1);
				break;
			default:
				W_reg_dev(pdev, chid, 30, 1);
				printk("pre_linefeed = 0x%x, Set default linefeed status to LF_FWD_ACTIVE\n", (pre_linefeed)&0xF);
				break;
#endif
		}

#if 0	// con_polling.c: ENTRY_SLIC_CPC_Polling() do this 
		slic_cpc[chid].cpc_timeout2 = jiffies + (HZ*200/1000);
		slic_cpc[chid].cpc_stop = 1;
#endif
	}

	
#if 0	// con_polling.c: ENTRY_SLIC_CPC_Polling() do this 
	if ((slic_cpc[chid].cpc_stop == 1) && (time_after(jiffies, slic_cpc[chid].cpc_timeout2)))
	{
		slic_cpc[chid].cpc_start = 0;
		//HookPollFlag[chid] = 1; // enable hook pooling
		HookPollingEnable( chid );
	}
#endif
}


#ifdef CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x
#define PHONE_CHECK_CONNECT_COUNT	11	// for Si3217x on RTL8972B
#define PHONE_CHECK_TIME_OUT_COUNT	60
#define VOLT_TR_THRESHOL	0x3000000	
#define VOLT_TIP_THRESHOL	0x3300000
#endif



/*  return value:
	0: Phone dis-connect, 
	1: Phone connect, 
	2: Phone off-hook, 
	3: Check time out ( may connect too many phone set => view as connect),
	4: Can not check, Linefeed should be set to active state first.
*/
//static inline unsigned char SLIC_Get_Hook_Status( int chid );

static inline unsigned int FXS_Line_Check_si3217x( voip_snd_t *this )	// Note: this API may cause watch dog timeout. Should it disable WTD?
{
	unsigned long flags;
	//unsigned int v_tip, v_ring, tick=0;
	unsigned int v_tip, v_ring, tick = 0;
	unsigned int connect_flag = 0, time_out_flag = 0;
	unsigned char linefeed, rev_linefeed;
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;

	if ( 1 == this ->fxs_ops ->SLIC_Get_Hook_Status( this, 1 ) )
	{
		//PRINT_MSG("%s: Phone 0ff-hook\n",__FUNCTION__);
		return 2;
	}

	//pSlic = ports[chid].ProObj;
	ProSLIC_GetLinefeedStatus(pSlic, &linefeed);
	linefeed = linefeed >> 4;
	//printk("Get linefeed: %d\n", linefeed);

	if ( (linefeed == LF_FWD_ACTIVE) || (linefeed == LF_REV_ACTIVE) )
	{
		if (linefeed == LF_FWD_ACTIVE)
		{
			rev_linefeed = LF_REV_ACTIVE;
		}
		else if (linefeed == LF_REV_ACTIVE)
		{
			rev_linefeed = LF_FWD_ACTIVE;
		}
#if 0
		SLIC_read_ram(chid, 12, &v_tip);	//~3.8V
		SLIC_read_ram(chid, 13, &v_ring);	//~ 49V
		printk("v_ring=0x%x, v_tip=0x%x, v_tr=0x%x\n", v_ring, v_tip, v_ring - v_tip);
#endif
		save_flags(flags); cli();
		
		ProSLIC_SetLinefeedStatus(pSlic, rev_linefeed);

		while (1)
		{
			tick++;
			
			if (linefeed == LF_FWD_ACTIVE)
			{
				this ->fxs_ops ->SLIC_read_ram(this, 12, &v_tip);
				this ->fxs_ops ->SLIC_read_ram(this, 13, &v_ring);
			}
			else if (linefeed == LF_REV_ACTIVE)
			{
				this ->fxs_ops ->SLIC_read_ram(this, 12, &v_ring);
				this ->fxs_ops ->SLIC_read_ram(this, 13, &v_tip);
			}

			if (((v_tip - v_ring) > VOLT_TR_THRESHOL) && (v_tip > VOLT_TIP_THRESHOL) )
			{
				PRINT_MSG("tick: %d\n", tick);
				
				//printk("v_tip=0x%x\n", v_tip);
				//printk("v_ring=0x%x\n", v_ring);

				if (tick > PHONE_CHECK_CONNECT_COUNT)
				{
					connect_flag = 1;
				}
				break;
			}

			if ( tick > PHONE_CHECK_TIME_OUT_COUNT )
			{
				time_out_flag = 1;
				//connect_flag = 1;
				break;
			}
		}
		
		ProSLIC_SetLinefeedStatus(pSlic, linefeed);
		restore_flags(flags);

	}
	else
	{
		PRINT_MSG("%s: Can't check. Linefeed should be set to active state first!\n", __FUNCTION__);
		return 4;
	}

	if (connect_flag == 1)
	{
		//PRINT_MSG("%s: Phone connected!\n",__FUNCTION__);
		//PRINT_MSG("Connect count: %d\n", PHONE_CHECK_CONNECT_COUNT);
		return 1;
	}
	else if (time_out_flag == 1)
	{
		//PRINT_MSG("%s: Check time out (may connect too many phone set)\n", __FUNCTION__);
		//PRINT_MSG("Connect count: %d\n", PHONE_CHECK_CONNECT_COUNT);
		//PRINT_MSG("Time out count: %d\n", PHONE_CHECK_TIME_OUT_COUNT);
		return 3;
	}
	else
	{
		//PRINT_MSG("%s: Phone dis-connected!\n",__FUNCTION__);
		//PRINT_MSG("Connect count: %d\n", PHONE_CHECK_CONNECT_COUNT);
		return 0;
	}

}


static void SendNTTCAR_si3217x( voip_snd_t *this )
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	ProSLIC_SendNTTCAR(pSlic);

}

static unsigned int SendNTTCAR_check_si3217x(voip_snd_t *this, unsigned long time_out)
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	return ProSLIC_SendNTTCAR_check(this->sch, pSlic, time_out);
	// this->sch can be ignored 
}

static void disableOscillators_si3217x(voip_snd_t *this)
{
}

static char cid_reg64, cid_reg64_prev;

static void SetOnHookTransmissionAndBackupRegister_si3217x(voip_snd_t *this) // use for DTMF caller id
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	ProSLIC_GetLinefeedStatus(pSlic, &cid_reg64);

	if ( (cid_reg64 & 0x07) != 2 )  // force for DTMF CID display
	{
		cid_reg64_prev = cid_reg64; // record it
		PRINT_MSG("Reg64 = 0x%02x\n", cid_reg64);
		this ->fxs_ops ->OnHookLineReversal(this, 0); //Forward On-Hook Transmission
	}

}

static inline void RestoreBackupRegisterWhenSetOnHookTransmission_si3217x(voip_snd_t *this) // use for DTMF caller id
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	ProSLIC_SetLinefeedStatus(pSlic, cid_reg64_prev);
}

static unsigned char Legerity_system_state(unsigned char slic_id, unsigned char state, unsigned char wri_re)
{
	return 0;
}

static void fskInitialization (void)
{}


static void SLIC_Set_PCM_state_si3217x(voip_snd_t *this, int enable)
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	//pSlic = ports[chid].ProObj;
	if (enable)
		ProSLIC_PCMStart(pSlic);
	else
		ProSLIC_PCMStop(pSlic);
}



static unsigned char SLIC_Get_Hook_Status_si3217x(voip_snd_t *this, int directly)
{
	unsigned char status = 0;
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;

	if( !directly && container ->user.hookStatus != INVALID_HOOK_STATUS ) {
		return container ->user.hookStatus;
	}
	
	//pSlic = ports[chid].ProObj;
	ProSLIC_ReadHookStatus(pSlic, &status); /* 1:off-hook  0:on-hook */

	container ->user.hookStatus = status;
	
	return status;
}

static void SLIC_read_reg_si3217x(voip_snd_t *this, unsigned int num, unsigned char *len, unsigned char *val)
{
	extern unsigned char R_reg_dev(rtl_spi_dev_t *pdev,unsigned char chid, unsigned char regaddr);
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pSlic = container ->ProObj;
	ctrl_S * const ctrl = ( ctrl_S * )( 
		( container ->ProObj ? 
			container ->ProObj ->deviceId ->ctrlInterface ->hCtrl:
			container ->daas ->deviceId ->ctrlInterface ->hCtrl ) );
	rtl_spi_dev_t * const pdev = &ctrl ->spi_dev;
	const unsigned char chid = 
		( container ->ProObj ?
			container ->ProObj ->channel :
			container ->daas ->channel );

	if (num == 32261)
	{
		ProSLIC_SetUserMode( pSlic, 1); // enable user mode
		printk("Si3226 channel %d enter user mode.\n", chid);
	}
	else if (num == 32260)
	{
		ProSLIC_SetUserMode( pSlic, 0); // disable user mode
		printk("Si3226 channel %d leave user mode.\n", chid);
	}
	else
		*val = R_reg_dev(pdev, chid, num);	
	
	*len = 1 ;
}


static void SLIC_write_reg_si3217x(voip_snd_t *this, unsigned char num, unsigned char val)
{
	extern void W_reg_dev(rtl_spi_dev_t *pdev, unsigned char chid, unsigned char regaddr, unsigned char data);
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	ctrl_S * const ctrl = ( ctrl_S * )( 
		( container ->ProObj ? 
			container ->ProObj ->deviceId ->ctrlInterface ->hCtrl:
			container ->daas ->deviceId ->ctrlInterface ->hCtrl ) );
	rtl_spi_dev_t * const pdev = &ctrl ->spi_dev;
	const unsigned char chid = 
		( container ->ProObj ?
			container ->ProObj ->channel :
			container ->daas ->channel );
	
	W_reg_dev(pdev, chid, num, val);
}

static void SLIC_read_ram_si3217x(voip_snd_t *this, unsigned short num, unsigned int *val)
{
	extern unsigned int R_ram_dev(rtl_spi_dev_t* pdev, unsigned char chid, unsigned short reg);
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	ctrl_S * const ctrl = ( ctrl_S * )( 
		( container ->ProObj ? 
			container ->ProObj ->deviceId ->ctrlInterface ->hCtrl:
			container ->daas ->deviceId ->ctrlInterface ->hCtrl ) );
	rtl_spi_dev_t * const pdev = &ctrl ->spi_dev;
	const unsigned char chid = 
		( container ->ProObj ?
			container ->ProObj ->channel :
			container ->daas ->channel );
	
	*val = R_ram_dev(pdev, chid, num);
}


static void SLIC_write_ram_si3217x(voip_snd_t *this, unsigned short num, unsigned int val)
{
	extern void W_ram_dev(rtl_spi_dev_t *pdev, unsigned char chid, unsigned short reg, unsigned int data);
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	ctrl_S * const ctrl = ( ctrl_S * )( 
		( container ->ProObj ? 
			container ->ProObj ->deviceId ->ctrlInterface ->hCtrl:
			container ->daas ->deviceId ->ctrlInterface ->hCtrl ) );
	rtl_spi_dev_t * const pdev = &ctrl ->spi_dev;
	const unsigned char chid = 
		( container ->ProObj ?
			container ->ProObj ->channel :
			container ->daas ->channel );
			
	W_ram_dev(pdev, chid, num, val);
}

static void SLIC_dump_reg_si3217x(voip_snd_t *this)
{
	unsigned char reg_val, reg_len;
	int i;

	printk("Dump SLIC register:\n");
	for (i=0; i <= 126; i++)
	{
		this ->fxs_ops ->SLIC_read_reg(this, i,&reg_len, &reg_val);
		printk("%d: 0x%x\n", i, reg_val);
	}
}

static void SLIC_dump_ram_si3217x(voip_snd_t *this)
{
	unsigned int ram_val;
	int i;

	printk("Dump SLIC ram:\n");
	for (i=0; i <= 1023; i++)
	{
		this ->fxs_ops ->SLIC_read_ram(this, i, &ram_val);
		printk("%d: 0x%x\n", i, ram_val);
	}
}

/* This API is workable only for Si32178, chid is SLIC's chid. */
static void FXS_FXO_DTx_DRx_Loopback_si3217x(voip_snd_t *this, voip_snd_t *daa_snd, unsigned int enable)
{
	ProslicContainer_t * const container = ( ProslicContainer_t * )this ->priv;
	proslicChanType * const pfxs = container ->ProObj;
	ProslicContainer_t * const daa_container = ( ProslicContainer_t * )daa_snd ->priv;
	vdaaChanType * const daas = daa_container ->daas;
	//proslicChanType *pfxs;	
	//pfxs = ports[chid].ProObj;
	const unsigned char chid = this ->sch;
	
	if( container ->ProSLICDevices != daa_container ->ProSLICDevices )
		printk( "Si3217x different devices loopback\n" );
	
	ProSLIC_SO_DTRx_Loopback(pfxs, enable);

	if (enable == 1)
	{
		//ProSLIC_PCMTimeSlotSetup(pfxs, 1+(pfxs->channel*8), 1+(pfxs->channel+DAA_CHANNEL_OFFSET)*8);
		ProSLIC_PCMTimeSlotSetup(pfxs, ts2count( this ->TS1 ), ts2count( daa_snd ->TS1 ));
		
#if 0//def CONFIG_RTK_VOIP_MULTIPLE_SI32178
		Vdaa_PCMTimeSlotSetup(daas[chid], 1+(pfxs->channel+DAA_CHANNEL_OFFSET)*8, 1+(pfxs->channel*8));
#else
		//Vdaa_PCMTimeSlotSetup(daas, 1+(pfxs->channel+DAA_CHANNEL_OFFSET)*8, 1+(pfxs->channel*8));
		Vdaa_PCMTimeSlotSetup(daas, ts2count( daa_snd ->TS1 ), ts2count( this ->TS1 ));
#endif
		//printk("fxo-%d: 0x%p, %d, %d\n", chid, daas[0], 1, 65);
		PRINT_MSG("Set SI32178 FXS/O loopback mode for FXS port%d\n", chid);
	}
	else if (enable == 0)
	{
		ProSLIC_PCMTimeSlotSetup(pfxs, ts2count( this ->TS1 ), ts2count( this ->TS1 ));
#if 0 //def CONFIG_RTK_VOIP_MULTIPLE_SI32178
		Vdaa_PCMTimeSlotSetup(daas[chid], 1+(pfxs->channel+DAA_CHANNEL_OFFSET)*8, 1+(pfxs->channel+DAA_CHANNEL_OFFSET)*8);
#else
		Vdaa_PCMTimeSlotSetup(daas, ts2count( daa_snd ->TS1 ), ts2count( daa_snd ->TS1 ));
#endif
		//printk("fxo-%d: 0x%p, %d, %d\n", chid, daas[chid], 1+(pfxs->channel+DAA_CHANNEL_OFFSET)*8, 1+(pfxs->channel+DAA_CHANNEL_OFFSET)*8);
		//Vdaa_PCMTimeSlotSetup(daas[0], 1+(pfxs->channel+DAA_CHANNEL_OFFSET)*8, 1+(pfxs->channel+DAA_CHANNEL_OFFSET)*8);
		PRINT_MSG("Disable SI32178 FXS/O loopback mode for FXS port%d\n", chid);
	}
}

static void SLIC_OnHookTrans_PCM_start_si3217x(voip_snd_t *this)
{
	this ->fxs_ops ->SLIC_Set_PCM_state(this, SLIC_PCM_ON);
	this ->fxs_ops ->OnHookLineReversal(this, 0);		//Forward On-Hook Transmission
	PRINT_MSG("SLIC_OnHookTrans_PCM_start, ch = %d\n", this ->sch);
}

static void SLIC_show_ID_si3217x( voip_snd_t *this )
{
	unsigned char reg_val, reg_len;
	
	//reg_val = R_reg_dev(&spiGciObj ->spi_dev, i, 0);
	this ->fxs_ops ->SLIC_read_reg( this, 0, &reg_len, &reg_val );
	
	switch ((reg_val&0x38)>>3)
	{
		case 0:
			printk("Si32171 ");
			break;
		case 3:
			printk("Si32175 ");
			break;
		case 4:
			printk("Si32176 ");
			break;
		case 5:
			printk("Si32177 ");
			break;
		case 6:
			printk("Si32178 ");
			break;
		default:
			PRINT_R("Unknow SLIC ");
			break;
	}
	
	
	if ((reg_val&0x07) == 0)
		printk("Revision A\n");
	else if ((reg_val&0x07) == 1)
		printk("Revision B\n");
	else if ((reg_val&0x07) == 2)
		printk("Revision C\n");
	else
		PRINT_R("Unknow Revision\n");
}

static int enable_si3217x( voip_snd_t *this, int enable )
{
	this ->fxs_ops ->SLIC_Set_PCM_state( this, ( enable ? SLIC_PCM_ON : SLIC_PCM_OFF ) );
	
	return 0;
}


// --------------------------------------------------------
// channel mapping architecture 
// --------------------------------------------------------

#include "voip_init.h"

#define IMPLEMENT_SI3217x_OPS_type0( func, rtype )		\
static rtype func##_si3217x( voip_snd_t *this )			\
{														\
	return func( this ->sch );							\
}

#define IMPLEMENT_SI3217x_OPS_type0_void( func )		\
static void func##_si3217x( voip_snd_t *this )			\
{														\
	func( this ->sch );									\
}

#define IMPLEMENT_SI3217x_OPS_type1( func, rtype, v1type )	\
static rtype func##_si3217x( voip_snd_t *this, v1type v1 )	\
{														\
	return func( this ->sch, v1 );						\
}

#define IMPLEMENT_SI3217x_OPS_type1_void( func, v1type )	\
static void func##_si3217x( voip_snd_t *this, v1type v1 )	\
{														\
	func( this ->sch, v1 );								\
}

#define IMPLEMENT_SI3217x_OPS_type2_void( func, v1type, v2type )	\
static void func##_si3217x( voip_snd_t *this, v1type v1, v2type v2 )		\
{														\
	func( this ->sch, v1, v2 );							\
}

//IMPLEMENT_SI3217x_OPS_type1_void( SLIC_reset, int );
//IMPLEMENT_SI3217x_OPS_type1_void( FXS_Ring, unsigned char );
//IMPLEMENT_SI3217x_OPS_type1( FXS_Check_Ring, unsigned char, unsigned char * );
//IMPLEMENT_SI3217x_OPS_type0( FXS_Line_Check, unsigned int );
//IMPLEMENT_SI3217x_OPS_type1_void( SLIC_Set_PCM_state, int );
//IMPLEMENT_SI3217x_OPS_type0( SLIC_Get_Hook_Status, unsigned char );

//IMPLEMENT_SI3217x_OPS_type1_void( Set_SLIC_Tx_Gain, unsigned char );
//IMPLEMENT_SI3217x_OPS_type1_void( Set_SLIC_Rx_Gain, unsigned char );
//IMPLEMENT_SI3217x_OPS_type2_void( SLIC_Set_Ring_Cadence, unsigned short, unsigned short );
//IMPLEMENT_SI3217x_OPS_type1_void( SLIC_Set_Ring_Freq_Amp, char );
//IMPLEMENT_SI3217x_OPS_type2_void( SLIC_Set_Impendance_Country, unsigned short, unsigned short );
//IMPLEMENT_SI3217x_OPS_type1_void( SLIC_Set_Impendance, unsigned short );
//IMPLEMENT_SI3217x_OPS_type1_void( OnHookLineReversal, unsigned char );
//IMPLEMENT_SI3217x_OPS_type0_void( SLIC_Set_LineVoltageZero );

//IMPLEMENT_SI3217x_OPS_type0( SLIC_CPC_Gen, uint8 );
//IMPLEMENT_SI3217x_OPS_type1_void( SLIC_CPC_Check, uint8 );

//IMPLEMENT_SI3217x_OPS_type0_void( SendNTTCAR );
//IMPLEMENT_SI3217x_OPS_type1( SendNTTCAR_check, unsigned int, unsigned long );

//IMPLEMENT_SI3217x_OPS_type0_void( disableOscillators );

//IMPLEMENT_SI3217x_OPS_type0_void( SetOnHookTransmissionAndBackupRegister );
//IMPLEMENT_SI3217x_OPS_type0_void( RestoreBackupRegisterWhenSetOnHookTransmission );

//IMPLEMENT_SI3217x_OPS_type2_void( SLIC_read_reg, unsigned int, unsigned char * );
//IMPLEMENT_SI3217x_OPS_type2_void( SLIC_write_reg, unsigned char, unsigned char );
//IMPLEMENT_SI3217x_OPS_type2_void( SLIC_read_ram, unsigned short, unsigned int * );
//IMPLEMENT_SI3217x_OPS_type2_void( SLIC_write_ram, unsigned short, unsigned int );
//IMPLEMENT_SI3217x_OPS_type0_void( SLIC_dump_reg );
//IMPLEMENT_SI3217x_OPS_type0_void( SLIC_dump_ram );
//IMPLEMENT_SI3217x_OPS_type1_void( FXS_FXO_DTx_DRx_Loopback, unsigned int );
//IMPLEMENT_SI3217x_OPS_type0_void( SLIC_OnHookTrans_PCM_start );

__attribute__ ((section(".snd_desc_data")))
static const snd_ops_fxs_t snd_si3217x_fxs_ops = {
	// common operation 
	.enable = enable_si3217x,
	
	// for each snd_type 
	.SLIC_reset = SLIC_reset_si3217x,
	.FXS_Ring = FXS_Ring_si3217x,
	.FXS_Check_Ring = FXS_Check_Ring_si3217x,
	.FXS_Line_Check = FXS_Line_Check_si3217x,	// Note: this API may cause watch dog timeout. Should it disable WTD?
	.SLIC_Set_PCM_state = SLIC_Set_PCM_state_si3217x,
	.SLIC_Get_Hook_Status = SLIC_Get_Hook_Status_si3217x,
	
	.Set_SLIC_Tx_Gain = Set_SLIC_Tx_Gain_si3217x,
	.Set_SLIC_Rx_Gain = Set_SLIC_Rx_Gain_si3217x,
	.SLIC_Set_Ring_Cadence = SLIC_Set_Ring_Cadence_si3217x,
	.SLIC_Set_Ring_Freq_Amp = SLIC_Set_Ring_Freq_Amp_si3217x,
	.SLIC_Set_Impendance_Country = SLIC_Set_Impendance_Country_si3217x, 
	.SLIC_Set_Impendance = SLIC_Set_Impendance_si3217x,
	.OnHookLineReversal = OnHookLineReversal_si3217x,	//0: Forward On-Hook Transmission, 1: Reverse On-Hook Transmission
	.SLIC_Set_LineVoltageZero = SLIC_Set_LineVoltageZero_si3217x,
	
	.SLIC_CPC_Gen = SLIC_CPC_Gen_si3217x,
	.SLIC_CPC_Check = SLIC_CPC_Check_si3217x,	// check in timer
	
	.SendNTTCAR = SendNTTCAR_si3217x,
	.SendNTTCAR_check = SendNTTCAR_check_si3217x,
	
	.disableOscillators = disableOscillators_si3217x,
	
	.SetOnHookTransmissionAndBackupRegister = SetOnHookTransmissionAndBackupRegister_si3217x,	// use for DTMF caller id
	.RestoreBackupRegisterWhenSetOnHookTransmission = RestoreBackupRegisterWhenSetOnHookTransmission_si3217x,	// use for DTMF caller id
	
	.FXS_FXO_DTx_DRx_Loopback = FXS_FXO_DTx_DRx_Loopback_si3217x,
	.SLIC_OnHookTrans_PCM_start = SLIC_OnHookTrans_PCM_start_si3217x,
	
	// read/write register/ram
	.SLIC_read_reg = SLIC_read_reg_si3217x,
	.SLIC_write_reg = SLIC_write_reg_si3217x,
	.SLIC_read_ram = SLIC_read_ram_si3217x,
	.SLIC_write_ram = SLIC_write_ram_si3217x,
	.SLIC_dump_reg = SLIC_dump_reg_si3217x,
	.SLIC_dump_ram = SLIC_dump_ram_si3217x,
};

static const proslic_args_t proslic_args_3217x = {
	.ring_setup_preset = 2,	// 20Hz, 48VRMS
};

static int __init voip_snd_proslic_init( void )
{
	extern int law;	// FIXME: chmap 
	extern const snd_ops_daa_t snd_si3050_daa_ops;
	int i, sch, daa;
	
	// common port definition 
	for( i = 0, sch = 0; i < TOTAL_NUM_OF_CH; i ++, sch ++ ) {

#if defined( CONFIG_RTK_VOIP_MULTIPLE_SI32178 ) 
		if( sch == CHAN_PER_DEVICE + 1 ) {
			sch = 0;
			daa = 0;
		} else if( sch == CHAN_PER_DEVICE )
			daa = 1;
		else
			daa = 0;
#elif defined( CONFIG_RTK_VOIP_SLIC_SI32176_SI32178_CS )
		if( i == TOTAL_NUM_OF_CH - 1 ) {
			sch = 1;
			daa = 1;
		} else {
			sch = 0;
			daa = 0;
		}
#else
		if( i == TOTAL_NUM_OF_CH - 1 )
			daa = 1;
		else
			daa = 0;
#endif
	
		snd_proslic[ i ].sch = sch;
		snd_proslic[ i ].name = "si3217x";
		snd_proslic[ i ].snd_type = SND_TYPE_FXS;
		snd_proslic[ i ].bus_type_sup = BUS_TYPE_PCM;
		snd_proslic[ i ].TS1 = i * 2;
#ifdef CONFIG_RTK_VOIP_WIDEBAND_APP
		snd_proslic[ i ].TS2 = ( daa || i * 2 >= 16 ? 0 : ( i + 8 ) * 2 );
		snd_proslic[ i ].band_mode_sup = ( daa ? BAND_MODE_8K : BAND_MODE_8K | BAND_MODE_16K );
#else
		snd_proslic[ i ].TS2 = 0;
		snd_proslic[ i ].band_mode_sup = BAND_MODE_8K;
#endif
		snd_proslic[ i ].snd_ops = ( const snd_ops_t * )&snd_si3217x_fxs_ops;
		snd_proslic[ i ].priv = &gProslicContainer[ i ];
		
		// DAA port  
		if( daa ) {
			snd_proslic[ i ].snd_type = SND_TYPE_DAA;
			snd_proslic[ i ].snd_ops = ( const snd_ops_t * )&snd_si3050_daa_ops;
		}
	}
	
	register_voip_snd( &snd_proslic[ 0 ], TOTAL_NUM_OF_CH );
	
	// SLIC init use ops 
	SLIC_init( law, 0 /* allocate */ );
	
	return 0;
}

voip_initcall_snd( voip_snd_proslic_init );

