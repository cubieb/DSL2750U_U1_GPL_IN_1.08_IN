#ifndef _RTL8676_GPIO_H_
#define _RTL8676_GPIO_H_

/* 
 *GPIO control registers 
*/
#define	GPIO_BASE			0xB8003500
#define PABCD_CNR			(GPIO_BASE + 0x00)
#define	PABCD_PTYPE			(GPIO_BASE + 0x04)
#define	PABCD_DIR			(GPIO_BASE + 0x08)
#define	PABCD_DAT			(GPIO_BASE + 0x0C)
#define	PABCD_ISR			(GPIO_BASE + 0x10)
#define	PAB_IMR				(GPIO_BASE + 0x14)
#define	PCD_IMR				(GPIO_BASE + 0x18)
#define	PEFGH_CNR			(GPIO_BASE + 0x1C)
#define	PEFGH_PTYPE			(GPIO_BASE + 0x20)
#define	PEFGH_DIR			(GPIO_BASE + 0x24)
#define	PEFGH_DAT			(GPIO_BASE + 0x28)
#define	PEFGH_ISR			(GPIO_BASE + 0x2C)
#define	PEF_IMR				(GPIO_BASE + 0x30)
#define	PGH_IMR				(GPIO_BASE + 0x34)

#define BSP_PCIE0_D_MEM     0xB9000000
#define BSP_PCIE1_D_MEM     0xBA000000

//MDIO
#define PCIE_MDIO_DATA_OFFSET (16)
#define PCIE_MDIO_DATA_MASK (0xffff <<PCIE_MDIO_DATA_OFFSET)
#define PCIE_MDIO_REG_OFFSET (8)
#define PCIE_MDIO_RDWR_OFFSET (0)

#ifdef CONFIG_RTL8192CD
//b[31:24]=GPIO_MOD[7:0], b[23:16]=GPIO_IO_SEL[7:0], b[15:8]=GPIO_OUT[]7:0], b[7:0]=GPIO_IN[7:0]
#define RTL8192CD_GPIO_PIN_CTRL         (BSP_PCIE0_D_MEM + 0x44)
        #define RTL8192CD_GPIO_IN_OFFSET                0
        #define RTL8192CD_GPIO_OUT_OFFSET               8
        #define RTL8192CD_GPIO_IO_SEL_OFFSET    16
#endif //CONFIG_RTL8192CD

struct rtl8676_gpio_para {
	unsigned char group;
	unsigned long pins;
	unsigned long ctrl_addr;
	unsigned long ptype_addr;
	unsigned long dir_addr;
	unsigned long data_addr;
};

enum{
	GPIO_A_0 = 0, GPIO_A_1, GPIO_A_2, GPIO_A_3, GPIO_A_4, GPIO_A_5, GPIO_A_6, GPIO_A_7,
	GPIO_B_0 = 8, GPIO_B_1, GPIO_B_2, GPIO_B_3, GPIO_B_4, GPIO_B_5, GPIO_B_6, GPIO_B_7,
	GPIO_C_0 = 16, GPIO_C_1, GPIO_C_2, GPIO_C_3, GPIO_C_4, GPIO_C_5, GPIO_C_6, GPIO_C_7,
	GPIO_D_0 = 24, GPIO_D_1, GPIO_D_2, GPIO_D_3, GPIO_D_4, GPIO_D_5, GPIO_D_6, GPIO_D_7,
	GPIO_E_0 = 32, GPIO_E_1, GPIO_E_2, GPIO_E_3, GPIO_E_4, GPIO_E_5, GPIO_E_6, GPIO_E_7,
	GPIO_F_0 = 40, GPIO_F_1, GPIO_F_2, GPIO_F_3, GPIO_F_4, GPIO_F_5, GPIO_F_6, GPIO_F_7,
	GPIO_G_0 = 48, GPIO_G_1, GPIO_G_2, GPIO_G_3, GPIO_G_4, GPIO_G_5, GPIO_G_6, GPIO_G_7,
	GPIO_H_0 = 56, GPIO_H_1, GPIO_H_2, GPIO_H_3, GPIO_H_4, GPIO_H_5, GPIO_H_6, GPIO_H_7,
#ifdef CONFIG_RTL8192CD	
	RTL8192CD_GPIO_0 = 64, RTL8192CD_GPIO_1, RTL8192CD_GPIO_2, RTL8192CD_GPIO_3, RTL8192CD_GPIO_4, RTL8192CD_GPIO_5, RTL8192CD_GPIO_6, RTL8192CD_GPIO_7, 
	RTL8192CD_GPIO_8 = 72, RTL8192CD_GPIO_9, RTL8192CD_GPIO_A,
#endif //CONFIG_RTL8192CD
#ifdef CONFIG_RTL8271BVN
	GPIO_I_0 = 75, GPIO_I_1, GPIO_I_2, GPIO_I_3, GPIO_I_4, GPIO_I_5, GPIO_I_6, GPIO_I_7,
	GPIO_I_8 = 83, GPIO_I_9, GPIO_I_A, GPIO_I_B, GPIO_I_C, GPIO_I_D, GPIO_I_E, GPIO_I_F,	
#endif	
	GPIO_END
};



/*port function definition*/
#define GPIO_FUNC_INPUT 	0x0001  /*data input*/
#define GPIO_FUNC_OUTPUT 	0x0002	/*data output*/
#define GPIO_FUNC_IRQ_FALL 	0x0003	/*falling edge IRQ*/
#define GPIO_FUNC_IRQ_RISE 	0x0004	/*rising edge IRQ*/
#define GPIO_FUNC_IRQ_LEVEL 	0x0005	/*level trigger IRQ*/

int get_rtl8676_gpio_para(int gpio_num, void * ptr);
void rtl8676_set_gpio_dir(struct rtl8676_gpio_para * para, int gpio_func);

#endif  /* _RTL8676_GPIO_H_ */
