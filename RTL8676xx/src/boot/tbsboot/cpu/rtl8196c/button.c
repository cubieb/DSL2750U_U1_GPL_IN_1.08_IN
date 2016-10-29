#include <common.h>
#include <types.h>
#include "btn.h"
#include "rtl8196c_gpio.h"


#if defined(CONFIG_BACKUP_IMAGE)


int button_pressed(void)
{
	volatile u32 val;

	
	/* Set WPS_BUTTON Pins as GPIO */
	val = REG_READ(GPIO_PABCD_CNR);
//	printf("GPIO_PABCD_CNR=[0x%08X]\n", val);
	val &= ~GPIO_RESET_BUTTON;
	REG_WRITE(GPIO_PABCD_CNR, val);
//	printf("GPIO_PABCD_CNR=[0x%08X]\n", val);


	/* Configure Shared Pins as GPIO Mode */
	val = REG_READ(PIN_MUX_SEL);
//	printf("PIN_MUX_SEL=[0x%08X]\n", val);
	val |= REG_SHARED_PIN_CFG;
	REG_WRITE(PIN_MUX_SEL, val);
//	printf("PIN_MUX_SEL=[0x%08X]\n", val);


	/* Set WPS_BUTTON Pin as GPIO input */
	val = REG_READ(GPIO_PABCD_DIR);
//	printf("GPIO_PABCD_DIR=[0x%08X]\n", val);
	val &= ~GPIO_RESET_BUTTON;
	REG_WRITE(GPIO_PABCD_DIR, val);
//	printf("GPIO_PABCD_DIR=[0x%08X]\n", val);

	val = REG_READ(GPIO_PABCD_DAT);
//	printf("GPIO_PABCD_DAT=[0x%08X][0x%08X]\n", val, val & GPIO_PABCD_DIR);
	if((val & GPIO_RESET_BUTTON) != GPIO_RESET_BUTTON)
		return BTN_DOWN;
	else
		return BTN_UP;	
	
}


#endif


