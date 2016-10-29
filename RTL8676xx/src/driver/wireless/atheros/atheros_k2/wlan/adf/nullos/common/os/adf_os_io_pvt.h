#ifndef _ADF_CMN_OS_IO_PVT_H
#define _ADF_CMN_OS_IO_PVT_H

/**
 * @brief Read an 8-bit register value
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 *
 * @return An 8-bit register value.
 */
static adf_os_inline a_uint8_t 
__adf_os_reg_read8(adf_os_device_t osdev, a_uint32_t addr)
{
    return 0;
}

/**
 * @brief Read a 16-bit register value
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 *
 * @return A 16-bit register value.
 */
static adf_os_inline a_uint16_t 
__adf_os_reg_read16(adf_os_device_t osdev, a_uint32_t addr)
{
     return 0;
}

/**
 * @brief Read a 32-bit register value
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 *
 * @return A 32-bit register value.
 */
static adf_os_inline a_uint32_t 
__adf_os_reg_read32(adf_os_device_t osdev, a_uint32_t addr)
{
	return 0;
}

/**
 * @brief Read a 64-bit register value
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 *
 * @return A 64-bit register value.
 */
static adf_os_inline a_uint64_t 
__adf_os_reg_read64(adf_os_device_t osdev, a_uint32_t addr)
{
    return 0;
}

/**
 * @brief Write an 8-bit value into register
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 * @param[in] b       the 8-bit value to be written
 */
static adf_os_inline void
__adf_os_reg_write8(adf_os_device_t osdev, a_uint32_t addr, a_uint8_t b)
{
    return;
}

/**
 * @brief Write a 16-bit value into register
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 * @param[in] w       the 16-bit value to be written
 */
static adf_os_inline void
__adf_os_reg_write16(adf_os_device_t osdev, a_uint32_t addr, a_uint16_t w)
{
    return;
}

/**
 * @brief Write a 32-bit value into register
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 * @param[in] l       the 32-bit value to be written
 */
static adf_os_inline void
__adf_os_reg_write32(adf_os_device_t osdev, a_uint32_t addr, a_uint32_t l)
{
    return;
}

/**
 * @brief Write a 64-bit value into register
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 * @param[in] q       the 64-bit value to be written
 */
static adf_os_inline void 
__adf_os_reg_write64(adf_os_device_t osdev, a_uint32_t addr, a_uint64_t q)
{
    return;
}

#define __adf_os_ntohs                      
#define __adf_os_ntohl                      

#define __adf_os_htons                      
#define __adf_os_htonl                      

#define __adf_os_cpu_to_le16                
#define __adf_os_cpu_to_le32                
#define __adf_os_cpu_to_le64                

#define __adf_os_cpu_to_be16                
#define __adf_os_cpu_to_be32                
#define __adf_os_cpu_to_be64                

#define __adf_os_le16_to_cpu                
#define __adf_os_le32_to_cpu                
#define __adf_os_le64_to_cpu                

#define __adf_os_be16_to_cpu                
#define __adf_os_be32_to_cpu                
#define __adf_os_be64_to_cpu                

#endif /*_ADF_CMN_OS_IO_PVT_H*/
