#include <common.h>
#include <types.h>
#include "btn.h"
#include "rtl8676_gpio.h"

extern struct btn_dev rtl8676_btns[];

int get_button_status( btn_name btnn)
{
	volatile u32 val;
	u32 pins;
	int i;
	struct btn_dev *p;
	struct rtl8676_gpio_para para;

	for(i=0; rtl8676_btns[i].name != btn_end; i++)
		{
		if(rtl8676_btns[i].name == btnn)
			{
			break;
			}
		}
	p = &(rtl8676_btns[i]);
	if(get_rtl8676_gpio_para(p->gpio, &para) < 0)
		return BTN_UP;

	/* Set BTN Pins as GPIO input */
	rtl8676_set_gpio_dir(&para, GPIO_FUNC_INPUT);
	val = REG32(para.data_addr);
	if(para.group < 2)
		{
		pins = para.pins;
		}
	else
		{
		pins = ((para.pins) << 8);
		}
	if(p->level == BTN_LEVEL_HIGH)
		return (val & pins) ? BTN_DOWN : BTN_UP;
	else
		return (val & pins) ? BTN_UP : BTN_DOWN;
}


int button_pressed(void)
{
	return get_button_status(btn_reset);
}

