/*
 * IKANOS vx180 GPIO操作公用函数
 *
 * Zhang Yu
 *
 * 2009-2-6
 */


/*====================================================================
 * structures variables used in this file only     
 * copy from IKANOS source code
 *====================================================================
 */
volatile unsigned short *gpio_dir_reg1 =  
                 (volatile unsigned short *)0xb9040000;
volatile unsigned short *gpio_flag_set1 = 
                 (volatile unsigned short *)0xb9040006;
volatile unsigned short *gpio_flag_clear1 = 
                 (volatile unsigned short *)0xb9040004;
volatile unsigned short *gpio_intren_clear_reg1 = 
                 (volatile unsigned short *)0xb9040008;
volatile unsigned short *gpio_intren_set_reg1 = 
                 (volatile unsigned short *)0xb904000A;
volatile unsigned short *gpio_edge_reg1 = 
                 (volatile unsigned short *)0xb9040014;
volatile unsigned short *gpio_polar_reg1 = 
                 (volatile unsigned short *)0xb9040010;




volatile unsigned short *gpio_dir_reg2 =
                 (volatile unsigned short *)0xb90b0000;
volatile unsigned short *gpio_flag_set2 = 
                 (volatile unsigned short *)0xb90b0006;
volatile unsigned short *gpio_flag_clear2 =
                 (volatile unsigned short *)0xb90b0004;
volatile unsigned short *gpio_intren_clear_reg2 = 
                 (volatile unsigned short *)0xb90b0008;
volatile unsigned short *gpio_intren_set_reg2 = 
                 (volatile unsigned short *)0xb90b000A;
volatile unsigned short *gpio_edge_reg2 = 
                 (volatile unsigned short *)0xb90b0014;
volatile unsigned short *gpio_polar_reg2 = 
                 (volatile unsigned short *)0xb90b0010;


void gpioSetDirIn(unsigned short pin)
{

    unsigned short old_value;

    if(pin <16)
    {
        old_value = *gpio_dir_reg1;
        *gpio_dir_reg1 = old_value & ~(0x1 << pin);
    }
    else
    {
        old_value = *gpio_dir_reg2;
        *gpio_dir_reg2 = old_value & ~(0x1 << (pin -16));
    }

    return;
}

void gpioSetDirOut(unsigned short pin)
{

    unsigned short old_value;

    if(pin <16)
    {
        old_value = *gpio_dir_reg1;
        *gpio_dir_reg1 = old_value | (0x1 << pin);
    }
    else
    {
        old_value = *gpio_dir_reg2;
        *gpio_dir_reg2 = old_value | (0x1 << (pin -16));
    }

    return;
}

void gpioSetFlag(unsigned short pin)
{
    if(pin <16)
    {
        *gpio_flag_set1 = (0x1 << pin);
    }
    else
    {
        *gpio_flag_set2 = (0x1 << (pin -16));
    }

    return;
}

void gpioClearFlag(unsigned short pin)
{
    if(pin <16)
    {
        *gpio_flag_clear1 = (0x1 << pin);
    }
    else
    {
        *gpio_flag_clear2 = (0x1 << (pin -16));
    }

    return;
}

void gpioSetIntrEN(unsigned short pin)
{

    if(pin <16)
    {
        *gpio_intren_set_reg1 = (0x1 << pin);
    }
    else
    {
        *gpio_intren_set_reg2 = (0x1 << (pin -16));
    }

    return;
}

void gpioClearIntrEN(unsigned short pin)
{
    if(pin <16)
    {
        *gpio_intren_clear_reg1 = (0x1 << pin);
    }
    else
    {
        *gpio_intren_clear_reg2 = (0x1 << (pin -16));
    }

    return;
}

void gpioSetEdge(unsigned short pin)
{
    unsigned short old_value;

    if(pin <16)
    {
        old_value = *gpio_edge_reg1;
        *gpio_edge_reg1 = old_value | (0x1 << pin);
    }
    else
    {
        old_value = *gpio_edge_reg2;
        *gpio_edge_reg2 = old_value | (0x1 << (pin -16));
    }

    return;
}

void gpioClearEdge(unsigned short pin)
{
    unsigned short old_value;

    if(pin <16)
    {
        old_value = *gpio_edge_reg1;
        *gpio_edge_reg1 = old_value & ~(0x1 << pin);
    }
    else
    {
        old_value = *gpio_edge_reg2;
        *gpio_edge_reg2 = old_value & ~(0x1 << (pin -16));
    }

    return;
}




void gpioSetPolar(unsigned short pin)
{
    unsigned short old_value;

    if(pin <16)
    {
        old_value = *gpio_polar_reg1;

        *gpio_polar_reg1 = old_value | (0x1 << pin);
    }
    else
    {
        old_value = *gpio_polar_reg2;
        *gpio_polar_reg2 = old_value | (0x1 << (pin -16));
    }

    return;
}

void gpioClearPolar(unsigned short pin)
{
    unsigned short old_value;

    if(pin <16)
    {
        old_value = *gpio_polar_reg1;

        *gpio_polar_reg1 = old_value & ~(0x1 << pin);
    }
    else
    {
        old_value = *gpio_polar_reg2;
        *gpio_polar_reg2 = old_value & ~(0x1 << (pin -16));
    }

    return;
}


