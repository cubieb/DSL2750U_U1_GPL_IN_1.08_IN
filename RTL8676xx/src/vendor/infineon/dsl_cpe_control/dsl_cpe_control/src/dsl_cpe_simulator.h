/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/** \file
   DSL Device Simulator
*/

#include "dsl_cpe_control.h"
#include "dsl_cpe_os.h"

#ifdef DSL_CPE_SIMULATOR

DSL_int_t DSL_CPE_SIM_Open(const DSL_char_t *pName);
DSL_int_t DSL_SIM_Close(const DSL_int_t fd);
DSL_int_t DSL_SIM_Write(const DSL_int_t fd, const DSL_void_t *pData, const DSL_uint32_t nSize);
DSL_int_t DSL_SIM_Read(const DSL_int_t fd, DSL_void_t *pData, const DSL_uint32_t nSize);
DSL_int_t DSL_SIM_Ioctl(const DSL_int_t fd, const DSL_uint32_t cmd, DSL_int_t param);
DSL_int_t DSL_SIM_Select(const DSL_uint32_t max_fd,
                           const DSL_fd_set_t *read_fd_in,
                           DSL_fd_set_t *read_fd_out,
                           const DSL_uint32_t timeout_msec);
DSL_int_t DSL_SIM_FD_ISSET(DSL_uint32_t fd);
DSL_int_t DSL_CPE_SIM_FD_SET(DSL_uint32_t fd, DSL_fd_set_t *read_fd_in);

#endif
