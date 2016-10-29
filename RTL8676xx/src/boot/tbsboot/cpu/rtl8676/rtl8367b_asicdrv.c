/*
 * Copyright (C) 2009 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 1.1 $
 * $Date: 2011/06/14 08:14:53 $
 *
 * Purpose : RTL8367B switch high-level API for RTL8367B
 * Feature :
 *
 */
#include <common.h>
#include "rtl8367b_asicdrv.h"
#include "rtl8676_swsmi.h"
#if defined(RTK_X86_ASICDRV)
#include <I2Clib.h>
#endif

/*for driver verify testing only*/
#ifdef CONFIG_RTL8367B_ASICDRV_TEST
#define CLE_VIRTUAL_REG_SIZE		0x10000
u16 CleVirtualReg[CLE_VIRTUAL_REG_SIZE];
#endif

#if defined(CONFIG_RTL865X_CLE) || defined (RTK_X86_CLE)
u32 cleDebuggingDisplay;
#endif

#ifdef EMBEDDED_SUPPORT
extern void setReg(u16, u16);
extern u16 getReg(u16);
#endif

/* Function Name:
 *      rtl8367b_setAsicRegBit
 * Description:
 *      Set a bit value of a specified register
 * Input:
 *      reg 	- register's address
 *      bit 	- bit location
 *      value 	- value to set. It can be value 0 or 1.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_INPUT  	- Invalid input parameter
 * Note:
 *      Set a bit of a specified register to 1 or 0.
 */
u32 rtl8367b_setAsicRegBit(u32 reg, u32 bit, u32 value)
{

#if defined(RTK_X86_ASICDRV)
	u32 regData;
	u32 retVal;

	if(bit >= RTL8367B_REGBITLENGTH)
		return RT_ERR_INPUT;

	retVal = Access_Read(reg, 2, &regData);
	if(TRUE != retVal)
		return RT_ERR_SMI;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

	if(value)
		regData = regData | (1 << bit);
	else
		regData = regData & (~(1 << bit));

	retVal = Access_Write(reg,2, regData);
	if(TRUE != retVal)
		return RT_ERR_SMI;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("W[0x%4.4x]=0x%4.4x\n", reg, regData);


#elif defined(CONFIG_RTL8367B_ASICDRV_TEST)

	if(bit >= RTL8367B_REGBITLENGTH)
		return RT_ERR_INPUT;

	else if(reg >= CLE_VIRTUAL_REG_SIZE)
		return RT_ERR_OUT_OF_RANGE;

	if(value)
	{
		CleVirtualReg[reg] =  CleVirtualReg[reg] | (1 << bit);
	}
	else
	{
		CleVirtualReg[reg] =  CleVirtualReg[reg] & (~(1 << bit));
	}

	if(0x8367B == cleDebuggingDisplay)
		PRINT("W[0x%4.4x]=0x%4.4x\n", reg, CleVirtualReg[reg]);

#elif defined(EMBEDDED_SUPPORT)
    u16 tmp;

    if(reg > RTL8367B_REGDATAMAX || value > 1)
	    return RT_ERR_INPUT;

	tmp = getReg(reg);
	tmp &= (1 << bitIdx);
	tmp |= (value << bitIdx);
	setReg(reg, tmp);

#else
	u32 regData;
	u32 retVal;

	if(bit >= RTL8367B_REGBITLENGTH)
		return RT_ERR_INPUT;

	retVal = smi_read(reg, &regData);
	if(retVal != RT_ERR_OK)
		return RT_ERR_SMI;

  #ifdef CONFIG_RTL865X_CLE
	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif
	if(value)
		regData = regData | (1 << bit);
	else
		regData = regData & (~(1 << bit));

	retVal = smi_write(reg, regData);
	if(retVal != RT_ERR_OK)
		return RT_ERR_SMI;

  #ifdef CONFIG_RTL865X_CLE
	if(0x8367B == cleDebuggingDisplay)
		PRINT("W[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif

#endif
	return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367b_getAsicRegBit
 * Description:
 *      Get a bit value of a specified register
 * Input:
 *      reg 	- register's address
 *      bit 	- bit location
 *      value 	- value to get.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_INPUT  	- Invalid input parameter
 * Note:
 *      None
 */
u32 rtl8367b_getAsicRegBit(u32 reg, u32 bit, u32 *pValue)
{

#if defined(RTK_X86_ASICDRV)

	u32 regData;
	u32 retVal;

	if(bit >= RTL8367B_REGBITLENGTH)
		return RT_ERR_INPUT;

	retVal = Access_Read(reg, 2, &regData);
	if(TRUE != retVal)
		return RT_ERR_SMI;

	*pValue = (regData & (0x1 << bit)) >> bit;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(CONFIG_RTL8367B_ASICDRV_TEST)

	if(bit >= RTL8367B_REGBITLENGTH)
		return RT_ERR_INPUT;

	if(reg >= CLE_VIRTUAL_REG_SIZE)
		return RT_ERR_OUT_OF_RANGE;

	*pValue = (CleVirtualReg[reg] & (0x1 << bit)) >> bit;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, CleVirtualReg[reg]);

#elif defined(EMBEDDED_SUPPORT)
    u16 tmp;

    if(reg > RTL8367B_REGDATAMAX )
	    return RT_ERR_INPUT;

	tmp = getReg(reg);
	tmp = tmp >> bitIdx;
	tmp &= 1;
	*value = tmp;
#else
	u32 regData;
	u32 retVal;

	retVal = smi_read(reg, &regData);
	if(retVal != RT_ERR_OK)
		return RT_ERR_SMI;

  #ifdef CONFIG_RTL865X_CLE
	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif

	*pValue = (regData & (0x1 << bit)) >> bit;

#endif
	return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367b_setAsicRegBits
 * Description:
 *      Set bits value of a specified register
 * Input:
 *      reg 	- register's address
 *      bits 	- bits mask for setting
 *      value 	- bits value for setting
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_INPUT  	- Invalid input parameter
 * Note:
 *      Set bits of a specified register to value. Both bits and value are be treated as bit-mask
 */
u32 rtl8367b_setAsicRegBits(u32 reg, u32 bits, u32 value)
{

#if defined(RTK_X86_ASICDRV)

	u32 regData;
	u32 retVal;
	u32 bitsShift;
	u32 valueShifted;

	if(bits >= (1 << RTL8367B_REGBITLENGTH) )
		return RT_ERR_INPUT;

	bitsShift = 0;
	while(!(bits & (1 << bitsShift)))
	{
		bitsShift++;
		if(bitsShift >= RTL8367B_REGBITLENGTH)
			return RT_ERR_INPUT;
	}

	valueShifted = value << bitsShift;
	if(valueShifted > RTL8367B_REGDATAMAX)
		return RT_ERR_INPUT;

	retVal = Access_Read(reg, 2, &regData);
	if(TRUE != retVal)
		return RT_ERR_SMI;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

	regData = regData & (~bits);
	regData = regData | (valueShifted & bits);

	retVal = Access_Write(reg,2, regData);
	if(TRUE != retVal)
		return RT_ERR_SMI;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("W[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(CONFIG_RTL8367B_ASICDRV_TEST)
	u32 regData;
	u32 bitsShift;
	u32 valueShifted;

	if(bits >= (1 << RTL8367B_REGBITLENGTH) )
		return RT_ERR_INPUT;

	bitsShift = 0;
	while(!(bits & (1 << bitsShift)))
	{
		bitsShift++;
		if(bitsShift >= RTL8367B_REGBITLENGTH)
			return RT_ERR_INPUT;
	}
	valueShifted = value << bitsShift;

	if(valueShifted > RTL8367B_REGDATAMAX)
		return RT_ERR_INPUT;

	if(reg >= CLE_VIRTUAL_REG_SIZE)
		return RT_ERR_OUT_OF_RANGE;

	regData = CleVirtualReg[reg] & (~bits);
	regData = regData | (valueShifted & bits);

	CleVirtualReg[reg] = regData;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("W[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(EMBEDDED_SUPPORT)
    u32 regData;
    u32 bitsShift;
    u32 valueShifted;

    if(reg > RTL8367B_REGDATAMAX )
	    return RT_ERR_INPUT;

    if(bits >= (1 << RTL8367B_REGBITLENGTH) )
        return RT_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= RTL8367B_REGBITLENGTH)
            return RT_ERR_INPUT;
    }

    valueShifted = value << bitsShift;
    if(valueShifted > RTL8367B_REGDATAMAX)
        return RT_ERR_INPUT;

    regData = getReg(reg);
    regData = regData & (~bits);
    regData = regData | (valueShifted & bits);

	setReg(reg, regData);

#else
	u32 regData;
	u32 retVal;
	u32 bitsShift;
	u32 valueShifted;

	if(bits >= (1 << RTL8367B_REGBITLENGTH) )
		return RT_ERR_INPUT;

	bitsShift = 0;
	while(!(bits & (1 << bitsShift)))
	{
		bitsShift++;
		if(bitsShift >= RTL8367B_REGBITLENGTH)
			return RT_ERR_INPUT;
	}
	valueShifted = value << bitsShift;

	if(valueShifted > RTL8367B_REGDATAMAX)
		return RT_ERR_INPUT;

	retVal = smi_read(reg, &regData);
	if(retVal != RT_ERR_OK)
		return RT_ERR_SMI;
  #ifdef CONFIG_RTL865X_CLE
	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif

	regData = regData & (~bits);
	regData = regData | (valueShifted & bits);

	retVal = smi_write(reg, regData);
	if(retVal != RT_ERR_OK)
		return RT_ERR_SMI;
  #ifdef CONFIG_RTL865X_CLE
	if(0x8367B == cleDebuggingDisplay)
		PRINT("W[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif
#endif
	return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367b_getAsicRegBits
 * Description:
 *      Get bits value of a specified register
 * Input:
 *      reg 	- register's address
 *      bits 	- bits mask for setting
 *      value 	- bits value for setting
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_INPUT  	- Invalid input parameter
 * Note:
 *      None
 */
u32 rtl8367b_getAsicRegBits(u32 reg, u32 bits, u32 *pValue)
{

#if defined(RTK_X86_ASICDRV)

	u32 regData;
	u32 retVal;
	u32 bitsShift;

	if(bits >= (1 << RTL8367B_REGBITLENGTH) )
		return RT_ERR_INPUT;

	bitsShift = 0;
	while(!(bits & (1 << bitsShift)))
	{
		bitsShift++;
		if(bitsShift >= RTL8367B_REGBITLENGTH)
			return RT_ERR_INPUT;
	}

	retVal = Access_Read(reg, 2, &regData);
	if(TRUE != retVal)
		return RT_ERR_SMI;

	*pValue = (regData & bits) >> bitsShift;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(CONFIG_RTL8367B_ASICDRV_TEST)
	u32 bitsShift;

	if(bits >= (1 << RTL8367B_REGBITLENGTH) )
		return RT_ERR_INPUT;

	bitsShift = 0;
	while(!(bits & (1 << bitsShift)))
	{
		bitsShift++;
		if(bitsShift >= RTL8367B_REGBITLENGTH)
			return RT_ERR_INPUT;
	}

	if(reg >= CLE_VIRTUAL_REG_SIZE)
		return RT_ERR_OUT_OF_RANGE;

	 *pValue = (CleVirtualReg[reg] & bits) >> bitsShift;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, CleVirtualReg[reg]);

#elif defined(EMBEDDED_SUPPORT)
    u32 regData;
    u32 bitsShift;

    if(reg > RTL8367B_REGDATAMAX )
	    return RT_ERR_INPUT;

    if(bits >= (1UL << RTL8367B_REGBITLENGTH) )
        return RT_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1UL << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= RTL8367B_REGBITLENGTH)
            return RT_ERR_INPUT;
    }

    regData = getReg(reg);
    *value = (regData & bits) >> bitsShift;

#else
	u32 regData;
	u32 retVal;
	u32 bitsShift;

	if(bits>= (1<<RTL8367B_REGBITLENGTH) )
		return RT_ERR_INPUT;

	bitsShift = 0;
	while(!(bits & (1 << bitsShift)))
	{
		bitsShift++;
		if(bitsShift >= RTL8367B_REGBITLENGTH)
			return RT_ERR_INPUT;
	}

	retVal = smi_read(reg, &regData);
	if(retVal != RT_ERR_OK) return RT_ERR_SMI;

	*pValue = (regData & bits) >> bitsShift;
  #ifdef CONFIG_RTL865X_CLE
	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n",reg, regData);
  #endif

#endif
	return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367b_setAsicReg
 * Description:
 *      Set content of asic register
 * Input:
 *      reg 	- register's address
 *      value 	- Value setting to register
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 * Note:
 *      The value will be set to ASIC mapping address only and it is always return RT_ERR_OK while setting un-mapping address registers
 */
u32 rtl8367b_setAsicReg(u32 reg, u32 value)
{

#if defined(RTK_X86_ASICDRV)/*RTK-CNSD2-NickWu-20061222: for x86 compile*/

	u32 retVal;

	retVal = Access_Write(reg,2,value);
	if(TRUE != retVal) return RT_ERR_SMI;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("W[0x%4.4x]=0x%4.4x\n",reg,value);

#elif defined(CONFIG_RTL8367B_ASICDRV_TEST)

	/*MIBs emulating*/
	if(reg == RTL8367B_REG_MIB_ADDRESS)
	{
		CleVirtualReg[RTL8367B_MIB_COUNTER_BASE_REG] = 0x1;
		CleVirtualReg[RTL8367B_MIB_COUNTER_BASE_REG+1] = 0x2;
		CleVirtualReg[RTL8367B_MIB_COUNTER_BASE_REG+2] = 0x3;
		CleVirtualReg[RTL8367B_MIB_COUNTER_BASE_REG+3] = 0x4;
	}

	if(reg >= CLE_VIRTUAL_REG_SIZE)
		return RT_ERR_OUT_OF_RANGE;

	CleVirtualReg[reg] = value;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("W[0x%4.4x]=0x%4.4x\n",reg,CleVirtualReg[reg]);

#elif defined(EMBEDDED_SUPPORT)
    if(reg > RTL8367B_REGDATAMAX || value > RTL8367B_REGDATAMAX )
	    return RT_ERR_INPUT;

    setReg(reg, value);

#else
	u32 retVal;

	retVal = smi_write(reg, value);
	if(retVal != RT_ERR_OK)
		return RT_ERR_SMI;
  #ifdef CONFIG_RTL865X_CLE
	if(0x8367B == cleDebuggingDisplay)
		PRINT("W[0x%4.4x]=0x%4.4x\n",reg,value);
  #endif

#endif
	return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367b_getAsicReg
 * Description:
 *      Get content of asic register
 * Input:
 *      reg 	- register's address
 *      value 	- Value setting to register
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 * Note:
 *      Value 0x0000 will be returned for ASIC un-mapping address
 */
u32 rtl8367b_getAsicReg(u32 reg, u32 *pValue)
{

#if defined(RTK_X86_ASICDRV)

	u32 regData;
	u32 retVal;

	retVal = Access_Read(reg, 2, &regData);
	if(TRUE != retVal)
		return RT_ERR_SMI;

	*pValue = regData;

	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(CONFIG_RTL8367B_ASICDRV_TEST)
	if(reg >= CLE_VIRTUAL_REG_SIZE)
		return RT_ERR_OUT_OF_RANGE;

	*pValue = CleVirtualReg[reg];

	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, CleVirtualReg[reg]);

#elif defined(EMBEDDED_SUPPORT)
    if(reg > RTL8367B_REGDATAMAX  )
	    return RT_ERR_INPUT;

	*value = getReg(reg);

#else
	u32 regData;
	u32 retVal;

	retVal = smi_read(reg, &regData);
	if(retVal != RT_ERR_OK)
		return RT_ERR_SMI;

	*pValue = regData;
  #ifdef CONFIG_RTL865X_CLE
	if(0x8367B == cleDebuggingDisplay)
		PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif

#endif
	return RT_ERR_OK;
}

