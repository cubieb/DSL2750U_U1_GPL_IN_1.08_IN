/**
 * @ingroup adf_os_public
 * @file adf_os_io.h
 * This file abstracts I/O operations.
 */

#ifndef _ADF_CMN_OS_IO_H
#define _ADF_CMN_OS_IO_H

#include <adf_os_io_pvt.h>
#include <adf_os_util.h>


/**
 * @brief Read an 8-bit register value
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 *
 * @return An 8-bit register value.
 */
static adf_os_inline a_uint8_t adf_os_reg_read8(adf_os_device_t osdev, a_uint32_t addr)
{
    return __adf_os_reg_read8(osdev, addr);
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
adf_os_reg_read16(adf_os_device_t osdev, a_uint32_t addr)
{
     return __adf_os_reg_read16(osdev, addr);
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
adf_os_reg_read32(adf_os_device_t osdev, a_uint32_t addr)
{
	return __adf_os_reg_read32(osdev, addr);
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
adf_os_reg_read64(adf_os_device_t osdev, a_uint32_t addr)
{
    return __adf_os_reg_read64(osdev, addr);
}

/**
 * @brief Write an 8-bit value into register
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 * @param[in] b       the 8-bit value to be written
 */
static adf_os_inline void
adf_os_reg_write8(adf_os_device_t osdev, a_uint32_t addr, a_uint8_t b)
{
    __adf_os_reg_write8(osdev, addr, b);
}

/**
 * @brief Write a 16-bit value into register
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 * @param[in] w       the 16-bit value to be written
 */
static adf_os_inline void
adf_os_reg_write16(adf_os_device_t osdev, a_uint32_t addr, a_uint16_t w)
{
    __adf_os_reg_write16(osdev, addr, w);
}

/**
 * @brief Write a 32-bit value into register
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 * @param[in] l       the 32-bit value to be written
 */
static adf_os_inline void
adf_os_reg_write32(adf_os_device_t osdev, a_uint32_t addr, a_uint32_t l)
{
    __adf_os_reg_write32(osdev, addr, l);
}

/**
 * @brief Write a 64-bit value into register
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 * @param[in] q       the 64-bit value to be written
 */
static adf_os_inline void 
adf_os_reg_write64(adf_os_device_t osdev, a_uint32_t addr, a_uint64_t q)
{
    __adf_os_reg_write64(osdev, addr, q);
}

/**
 * @brief Convert a 16-bit value from network byte order to host byte order
 */
#define adf_os_ntohs(x)                         __adf_os_ntohs(x)

/**
 * @brief Convert a 32-bit value from network byte order to host byte order
 */
#define adf_os_ntohl(x)                         __adf_os_ntohl(x)

/**
 * @brief Convert a 16-bit value from host byte order to network byte order
 */
#define adf_os_htons(x)                         __adf_os_htons(x)

/**
 * @brief Convert a 32-bit value from host byte order to network byte order
 */
#define adf_os_htonl(x)                         __adf_os_htonl(x)

/**
 * @brief Convert a 16-bit value from CPU byte order to big-endian byte order
 */
#define adf_os_cpu_to_be16(x)                   __adf_os_cpu_to_be16(x)

/**
 * @brief Convert a 32-bit value from CPU byte order to big-endian byte order
 */
#define adf_os_cpu_to_be32(x)                   __adf_os_cpu_to_be32(x)

/**
 * @brief Convert a 64-bit value from CPU byte order to big-endian byte order
 */
#define adf_os_cpu_to_be64(x)                   __adf_os_cpu_to_be64(x)

/**
 * @brief Convert a 16-bit value from CPU byte order to little-endian byte order
 */
#define adf_os_cpu_to_le16(x)                   __adf_os_cpu_to_le16(x)

/**
 * @brief Convert a 32-bit value from CPU byte order to little-endian byte order
 */
#define adf_os_cpu_to_le32(x)                   __adf_os_cpu_to_le32(x)

/**
 * @brief Convert a 64-bit value from CPU byte order to little-endian byte order
 */
#define adf_os_cpu_to_le64(x)                   __adf_os_cpu_to_le64(x)

/**
 * @brief Convert a 16-bit value from big-endian byte order to CPU byte order
 */
#define adf_os_be16_to_cpu(x)                   __adf_os_be16_to_cpu(x)

/**
 * @brief Convert a 32-bit value from big-endian byte order to CPU byte order
 */
#define adf_os_be32_to_cpu(x)                   __adf_os_be32_to_cpu(x)

/**
 * @brief Convert a 64-bit value from big-endian byte order to CPU byte order
 */
#define adf_os_be64_to_cpu(x)                   __adf_os_be64_to_cpu(x)

/**
 * @brief Convert a 16-bit value from little-endian byte order to CPU byte order
 */
#define adf_os_le16_to_cpu(x)                   __adf_os_le16_to_cpu(x)

/**
 * @brief Convert a 32-bit value from little-endian byte order to CPU byte order
 */
#define adf_os_le32_to_cpu(x)                   __adf_os_le32_to_cpu(x)

/**
 * @brief Convert a 64-bit value from little-endian byte order to CPU byte order
 */
#define adf_os_le64_to_cpu(x)                   __adf_os_le64_to_cpu(x)

#endif
