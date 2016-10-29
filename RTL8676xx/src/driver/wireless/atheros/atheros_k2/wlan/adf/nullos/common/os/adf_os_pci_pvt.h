#ifndef __ADF_CMN_OS_PCI_PVT_H
#define __ADF_CMN_OS_PCI_PVT_H

#include <adf_os_types.h>
#include <adf_os_util.h>

#define __adf_os_pci_module_init(_fn)
#define __adf_os_pci_module_exit(_fn) 
#define __adf_os_pci_set_drv(_name, _pci_ids, _attach, _detach, _suspend, _resume) 

struct adf_os_pci_drvinfo;
typedef struct adf_os_pci_drvinfo adf_os_pci_drvinfo_t;
/**
 * @brief Read a byte of PCI config space.
 *
 * @param[in]  osdev    platform device instance
 * @param[in]  offset   offset to read
 * @param[out] val      value read
 *
 * @return status of operation
 */ 
static adf_os_inline a_status_t 
__adf_os_pci_config_read8(adf_os_device_t osdev, a_uint32_t offset, a_uint8_t *val)
{
    return A_STATUS_OK;
}

/**
 * @brief Write a byte to PCI config space.
 *
 * @param[in] osdev    platform device instance
 * @param[in] offset   offset to write
 * @param[in] val      value to write
 *
 * @return status of operation
 */ 
static adf_os_inline a_status_t 
__adf_os_pci_config_write8(adf_os_device_t osdev, a_uint32_t offset, a_uint8_t val)
{
    return A_STATUS_OK;
}

/**
 * @brief Read 2 bytes of PCI config space.
 *
 * @param[in]  osdev    platform device instance
 * @param[in]  offset   offset to read
 * @param[out] val      value read
 *
 * @return status of operation
 */ 
static adf_os_inline a_status_t 
__adf_os_pci_config_read16(adf_os_device_t osdev, a_uint32_t offset, a_uint16_t *val)
{
    return A_STATUS_OK;
}

/**
 * @brief Write 2 bytes to PCI config space.
 *
 * @param[in] osdev    platform device instance
 * @param[in] offset   offset to write
 * @param[in] val      value to write
 *
 * @return status of operation
 */ 
static adf_os_inline a_status_t
__adf_os_pci_config_write16(adf_os_device_t osdev, a_uint32_t offset, a_uint16_t val)
{
    return A_STATUS_OK;
}

/**
 * @brief Read 4 bytes of PCI config space.
 *
 * @param[in]  osdev    platform device instance
 * @param[in]  offset   offset to read
 * @param[out] val      value read
 *
 * @return status of operation
 */ 
static adf_os_inline a_status_t 
__adf_os_pci_config_read32(adf_os_device_t osdev, a_uint32_t offset, a_uint32_t *val)
{
    return A_STATUS_OK;
}

/**
 * @brief Write 4 bytes to PCI config space.
 *
 * @param[in] osdev    platform device instance
 * @param[in] offset   offset to write
 * @param[in] val      value to write
 *
 * @return status of operation
 */ 
static adf_os_inline a_status_t
__adf_os_pci_config_write32(adf_os_device_t osdev, a_uint32_t offset, a_uint32_t val)
{
    return A_STATUS_OK;
}

/**
 * @brief Get OS Handle from OS device object.
 *
 *@param[in] osdev OS device object
 * 
 *@return OS handle
 */

static adf_os_inline adf_os_handle_t
__adf_os_pcidev_to_os(adf_os_device_t osdev)
{
        return __adf_os_pcidev_to_os(osdev);
}


/**
 * @brief this register the driver to the shim, but won't get
 *        any handle until create device is called.
 * 
 * @param[in] drv driver info structure
 * 
 * @return status of operation
 */
static adf_os_inline a_status_t
__adf_os_pci_drv_reg(adf_os_pci_drvinfo_t *drv_info)
{
    return A_STATUS_OK;
}


/**
 * @brief deregister the driver from the shim
 *
 * @param[in] name driver name passed in adf_drv_info_t
 *
 * @see adf_os_pci_drv_reg()
 */
static adf_os_inline void
__adf_os_pci_drv_unreg(a_uint8_t *drv_name)
{
    return;
}

#endif/* __ADF_CMN_OS_PCI_PVT_H*/
