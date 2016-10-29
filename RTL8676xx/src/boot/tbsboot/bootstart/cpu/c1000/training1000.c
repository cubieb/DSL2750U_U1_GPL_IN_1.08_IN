/*
 * (C) Copyright Mindspeed Technologies Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include "hardware.h"

#define SZ_1K	0x400
#define SZ_128K  (SZ_1K * 128)
#define ADDR_JUMP_SIZE  SZ_128K
#define WR_RD_TRANS_ITERATION_COUNT 16

void get_dll_clk_window(void);
u32 do_wr_rd_transaction(u32 iteration_count);
u32 do_wr_rd_transaction64_1(u32 ddr_address_offset);
u32 do_wr_rd_transaction64_2(u32 ddr_address_offset);
u32 do_wr_rd_transaction32(u32 ddr_address_offset);

void start_training(void)
{
  /* find a dll wr clk window for a given dqs_index,
   * this function updates the global dll and dqs values, if its finds any bigger window
   * than previous window, and then finally it configures the identified dqs and dll values
   * to DDR controller  */
  get_dll_clk_window();

  return;
}

/* find the a bigger dll clk window for the given dqs value */
void get_dll_clk_window(void)
{   
  u8 dll_clk_win;
  u32 dll_clk_window;
  u8 dll_clk_win_min;
  u8 dll_clk_win_max;
  u32 result;
  int window_start;
  int window_end;
  u64 dqs_value;
  u32 dqs_min_val;
  u32 dqs_max_val;
  u32 dqs_index;

  u32 glb_dll_clk_window = 0;
  u32 glb_dll_clk_window_start = 0;
  u32 glb_dll_clk_window_end = 0;
  u32 glb_dqs_index = 0;

  u32 dll_wr_clk_median;

  /* Find the min and max values for dqs */
  dqs_min_val = DENALI_WR_DQS_DELAY0 - 0x15;
  dqs_max_val = DENALI_WR_DQS_DELAY0 + 0x15;

  /* Looping through ADJ_2 range */
  for(dqs_index = dqs_min_val; dqs_index <= dqs_max_val; dqs_index++)
  {
    window_start = -1;
    window_end = -1;

  /* stop DDR */
  *(volatile u64*)(DENALI_CTL_02_DATA) = (u64)DENALI_CTL_02_VAL_CFG1;

  /* Configure the WRLVL_DELAY_X values */
  dqs_value = (*(volatile u64*)(DENALI_CTL_35_DATA)) & 0xff00000000ffffffULL;
  dqs_value |= (u64)(((u64)dqs_index << 24) | ((u64)dqs_index << 32) | ((u64)dqs_index << 40) | ((u64)dqs_index << 48));
  *(volatile u64*)(DENALI_CTL_35_DATA) = (u64)dqs_value;

  //set sw leveling mode
  *(volatile u64*)(DENALI_CTL_67_DATA) = (u64)DENALI_CTL_67_VAL_CFG1 | (1LL << 8);
  //sw leveling load
  *(volatile u64*)(DENALI_CTL_66_DATA) = (u64)DENALI_CTL_66_VAL_CFG1 | (1LL << 48);
  //reset sw leveling mode
  *(volatile u64*)(DENALI_CTL_67_DATA) = (u64)DENALI_CTL_67_VAL_CFG1;

  // start DDR
  *(volatile u64*)(DENALI_CTL_02_DATA) = (u64)DENALI_CTL_02_VAL_CFG1 | (1LL << 32);
  //wait int_status[2] (DRAM init complete)
  while(( *(volatile u32*)(DENALI_CTL_36_DATA+8) & 0x40000) == 0);

  /* Get the dll clk window max value from DLL master adjust bits */
  dll_clk_win_min = 0;
  dll_clk_win_max = DENALI_DLL_MADJ0;

  for (dll_clk_win = dll_clk_win_min; dll_clk_win <= dll_clk_win_max; dll_clk_win++)
  {

    /* Configure dll write click adj-1 values */
    DENALI_DLL_ADJ1_DS0_L = (u8)dll_clk_win;
    DENALI_DLL_ADJ1_DS1_H = (u8)dll_clk_win;
    DENALI_DLL_ADJ1_DS2_L = (u8)dll_clk_win;
    DENALI_DLL_ADJ1_DS3_H = (u8)dll_clk_win;

    result = do_wr_rd_transaction(WR_RD_TRANS_ITERATION_COUNT);
    if (!result) /* SUCCESS */
    {
      if (window_start == -1) /* its first time success */
      {
	window_start = dll_clk_win;
	window_end = dll_clk_win;
      }
      else
      {
	window_end++;
      }
    }
    else /* result == FAILURE */
    { 
      if (window_start == -1)
	continue;
      else /* window ends here */
      {
	/* find the window width */
        dll_clk_window = (window_end - window_start) + 1;

        /* can we accept this window */
        if (glb_dll_clk_window < dll_clk_window)
        {
          glb_dll_clk_window = dll_clk_window;
	  glb_dll_clk_window_end = window_end;
          glb_dll_clk_window_start = window_start;
          glb_dqs_index = dqs_index;
        }

	/* reset window pointers for measuring the next window */
	window_start = window_end = -1;
      }
    } /* result == FAILURE */
  } /* for loop from dll min to max */ 

  } //End of ADJ_2


  /* check if we find any big window for a given dqs, dll values */
  if (glb_dll_clk_window < 9)
  {
    /* FIXME: No enough window identified,
     * may be this board is bad, we should not proceed further  */
    while(1);
  }
 
  /* Configure the dll wr clk median */
  dll_wr_clk_median = (u32)((glb_dll_clk_window_start + glb_dll_clk_window_end) >> 1);

  // stop DDR
  //*(volatile u64*)(DENALI_CTL_02_DATA) = (u64)DENALI_CTL_02_VAL_CFG1 | ~(1LL << 32);
  *(volatile u64*)(DENALI_CTL_02_DATA) = (u64)DENALI_CTL_02_VAL_CFG1;

  /* Configure the WRLVL_DELAY_X values */
  dqs_value = (*(volatile u64*)(DENALI_CTL_35_DATA)) & 0xff00000000ffffffULL;
  dqs_value |= (u64)(((u64)glb_dqs_index << 24) | ((u64)glb_dqs_index << 32) | ((u64)glb_dqs_index << 40) | ((u64)glb_dqs_index << 48));
  *(volatile u64*)(DENALI_CTL_35_DATA) = (u64)dqs_value;

  //set sw leveling mode
  *(volatile u64*)(DENALI_CTL_67_DATA) = (u64)DENALI_CTL_67_VAL_CFG1 | (1LL << 8);
  //sw leveling load
  *(volatile u64*)(DENALI_CTL_66_DATA) = (u64)DENALI_CTL_66_VAL_CFG1 | (1LL << 48);
  //reset sw leveling mode
  *(volatile u64*)(DENALI_CTL_67_DATA) = (u64)DENALI_CTL_67_VAL_CFG1;

  DENALI_DLL_ADJ1_DS0_L = (u8)(dll_wr_clk_median & 0xFF);
  DENALI_DLL_ADJ1_DS1_H = (u8)(dll_wr_clk_median & 0xFF);
  DENALI_DLL_ADJ1_DS2_L = (u8)(dll_wr_clk_median & 0xFF);
  DENALI_DLL_ADJ1_DS3_H = (u8)(dll_wr_clk_median & 0xFF);

  // start DDR
  *(volatile u64*)(DENALI_CTL_02_DATA) = (u64)DENALI_CTL_02_VAL_CFG1 | (1LL << 32);
  //wait int_status[2] (DRAM init complete)
  while(( *(volatile u32*)(DENALI_CTL_36_DATA+8) & 0x40000) == 0);

  return;
}

u32 do_wr_rd_transaction(u32 iteration_count)
{
  u32 ii;
  u32 result = 0;
  u32 max_limit_addr;
  u32 highmem_val;
  u32 ddr_addr_offset;

  /* find the max ddr address */
  highmem_val =	*(volatile u32*) AHB_HIGHMEM_ADDR_TRANSLATION;
  max_limit_addr =  ((highmem_val << 16) | 0x0000ffff) & 0x7fffffff;

  ddr_addr_offset = 0;

  for(ii = 0; ii < iteration_count; ii++)
  {
    result = 0;

    result |= do_wr_rd_transaction64_1(ddr_addr_offset);
    result |= do_wr_rd_transaction64_2(ddr_addr_offset);
    result |= do_wr_rd_transaction32(ddr_addr_offset);

    if (result)
    {
      break;
    }

    ddr_addr_offset = (ddr_addr_offset + ADDR_JUMP_SIZE) & max_limit_addr;
  }

  return result;
}

u32 do_wr_rd_transaction64_1(u32 ddr_address_offset)
{
  int j;
  u32 d0,d1;
  u64 dword_list_t1[4]  = {0xffffffff00000000ULL, 0xffffffff00000000ULL, 0x1234567876543210ULL, 0x0123456789abcdefULL};
  u64 dword_list_t2[2] = {0x00000000ffffffffULL, 0x00000000ffffffffULL};
  u64 *src1, *src2;
  u64 *dst1, *dst2;

  src1 = &dword_list_t1[0];
  src2 = &dword_list_t2[0];

  /* Load start address */
  dst1 = ((u64 *) (DDR_BASEADDR + ddr_address_offset));
  dst2 = ((u64 *) (DDR_BASEADDR + ddr_address_offset));

  for(j=0; j < 4; j++)
  {
    __asm__("ldmia %0, {%1,%2}" \
          : "=r" (src1), "=r" (d0), "=r" (d1) \
	  : "0" (src1));

    __asm__("stmia %0, {%1,%2}" \
          : "=r" (dst1), "=r" (d0), "=r" (d1) \
	  : "0" (dst1));

    if (*src1 != *dst1)
    {
      return 0x1;
    }
  }

  for(j=0; j < 2; j++)
  {
    __asm__("ldmia %0, {%1,%2}" \
          : "=r" (src2), "=r" (d0), "=r" (d1) \
	  : "0" (src2));

    __asm__("stmia %0, {%1,%2}" \
          : "=r" (dst2), "=r" (d0), "=r" (d1) \
	  : "0" (dst2));

    if (*src2 != *dst2)
    {
      return 0x1;
    }
  }

  return 0;
}


u32 do_wr_rd_transaction64_2(u32 ddr_address_offset)
{
  u32 result = 0x2;
  u64 dword_list[4] = {0xffffffff00000000ULL, 0xffffffff00000000ULL, 0x1234567876543210ULL, 0x0123456789abcdefULL};
  u64 dword_list1[4] = {0x00000000ffffffffULL, 0x00000000ffffffffULL, 0x1234567876543210ULL, 0x0123456789abcdefULL};
  
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset) = dword_list[0];
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 1) = dword_list[1];
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 2) = dword_list[2];
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 3) = dword_list[3]; 

  if (*((volatile u64 *) DDR_BASEADDR + ddr_address_offset) == dword_list[0]) 
  {
    if (*((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 1) == dword_list[1]) 
    {
      if (*((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 2) == dword_list[2]) 
      {
        if (*((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 3) == dword_list[3]) 
	{
	  result = 0;
	}
      }
    }
  }
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset) = 0;
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 1) = 0;
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 2) = 0;
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 3) = 0; 

  if (result == 0x2)
    return result;

  result = 0x2;

  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset) = dword_list1[0];
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 1) = dword_list1[1];
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 2) = dword_list1[2];
  *((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 3) = dword_list1[3]; 

  if (*((volatile u64 *) DDR_BASEADDR + ddr_address_offset) == dword_list1[0]) 
  {
    if (*((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 1) == dword_list1[1]) 
    {
      if (*((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 2) == dword_list1[2]) 
      {
        if (*((volatile u64 *) DDR_BASEADDR + ddr_address_offset + 3) == dword_list1[3]) 
	{
	  result = 0;
	}
      }
    }
  }

  return result;
}

u32 do_wr_rd_transaction32(u32 ddr_address_offset)
{
  u32 word_list[4] = {0xaaaaaaaa,0x55555555,0x55555555, 0xaaaaaaaa};
  ddr_address_offset = 0;

  *((volatile u32 *) DDR_BASEADDR + ddr_address_offset) = word_list[0];
  *((volatile u32 *) DDR_BASEADDR + ddr_address_offset + 1) = word_list[1];
  *((volatile u32 *) DDR_BASEADDR + ddr_address_offset + 2) = word_list[2];
  *((volatile u32 *) DDR_BASEADDR + ddr_address_offset + 3) = word_list[3];

  if (*((volatile u32 *) DDR_BASEADDR + ddr_address_offset) == word_list[0])
  {
    if (*((volatile u32 *) DDR_BASEADDR + ddr_address_offset + 1) == word_list[1]) 
    {
      if (*((volatile u32 *) DDR_BASEADDR + ddr_address_offset + 2) == word_list[2]) 
      {
        if (*((volatile u32 *) DDR_BASEADDR + ddr_address_offset + 3) == word_list[3]) 
	{
          return 0;
	}
      }
    }
  }

  return 0x4;
}

