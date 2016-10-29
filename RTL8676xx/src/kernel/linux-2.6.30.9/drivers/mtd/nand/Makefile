#
# linux/drivers/nand/Makefile
#

obj-$(CONFIG_MTD_NAND)			+= nand.lib nand_ecc.lib
obj-$(CONFIG_MTD_NAND_IDS)		+= nand_ids.lib

obj-$(CONFIG_MTD_NAND_CAFE)		+= cafe_nand.lib
obj-$(CONFIG_MTD_NAND_SPIA)		+= spia.lib
obj-$(CONFIG_MTD_NAND_AMS_DELTA)	+= ams-delta.lib
obj-$(CONFIG_MTD_NAND_AUTCPU12)		+= autcpu12.lib
obj-$(CONFIG_MTD_NAND_EDB7312)		+= edb7312.lib
obj-$(CONFIG_MTD_NAND_AU1550)		+= au1550nd.lib
obj-$(CONFIG_MTD_NAND_BF5XX)		+= bf5xx_nand.lib
obj-$(CONFIG_MTD_NAND_PPCHAMELEONEVB)	+= ppchameleonevb.lib
obj-$(CONFIG_MTD_NAND_S3C2410)		+= s3c2410.lib
obj-$(CONFIG_MTD_NAND_DAVINCI)		+= davinci_nand.lib
obj-$(CONFIG_MTD_NAND_DISKONCHIP)	+= diskonchip.lib
obj-$(CONFIG_MTD_NAND_H1900)		+= h1910.lib
obj-$(CONFIG_MTD_NAND_RTC_FROM4)	+= rtc_from4.lib
obj-$(CONFIG_MTD_NAND_SHARPSL)		+= sharpsl.lib
obj-$(CONFIG_MTD_NAND_TS7250)		+= ts7250.lib
obj-$(CONFIG_MTD_NAND_NANDSIM)		+= nandsim.lib
obj-$(CONFIG_MTD_NAND_CS553X)		+= cs553x_nand.lib
obj-$(CONFIG_MTD_NAND_NDFC)		+= ndfc.lib
obj-$(CONFIG_MTD_NAND_ATMEL)		+= atmel_nand.lib
obj-$(CONFIG_MTD_NAND_GPIO)		+= gpio.lib
obj-$(CONFIG_MTD_NAND_CM_X270)		+= cmx270_nand.lib
obj-$(CONFIG_MTD_NAND_BASLER_EXCITE)	+= excite_nandflash.lib
obj-$(CONFIG_MTD_NAND_PXA3xx)		+= pxa3xx_nand.lib
obj-$(CONFIG_MTD_NAND_TMIO)		+= tmio_nand.lib
obj-$(CONFIG_MTD_NAND_PLATFORM)		+= plat_nand.lib
obj-$(CONFIG_MTD_ALAUDA)		+= alauda.lib
obj-$(CONFIG_MTD_NAND_PASEMI)		+= pasemi_nand.lib
obj-$(CONFIG_MTD_NAND_ORION)		+= orion_nand.lib
obj-$(CONFIG_MTD_NAND_FSL_ELBC)		+= fsl_elbc_nand.lib
obj-$(CONFIG_MTD_NAND_FSL_UPM)		+= fsl_upm.lib
obj-$(CONFIG_MTD_NAND_SH_FLCTL)		+= sh_flctl.lib
obj-$(CONFIG_MTD_NAND_MXC)		+= mxc_nand.lib
obj-$(CONFIG_MTD_NAND_SOCRATES)		+= socrates_nand.lib
obj-$(CONFIG_MTD_NAND_TXX9NDFMC)	+= txx9ndfmc.lib
obj-$(CONFIG_MTD_NAND_RTK)			+= rtk_nand_sd5.lib

ifeq ($(CONFIG_MTD_NAND_RTK),y)
	nand-objs += nand_base_sd5.lib
else
	nand-objs := nand_base.lib nand_bbt.lib
endif

