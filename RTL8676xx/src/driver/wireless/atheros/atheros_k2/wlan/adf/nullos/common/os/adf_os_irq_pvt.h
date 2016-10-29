#ifndef __ADF_CMN_OS_IRQ_PVT_H
#define __ADF_CMN_OS_IRQ_PVT_H

/**
 * @brief register the IRQ handler
 * 
 * @param[in] pdev
 * @param[out] osdev (store IRQ)
 * 
 * @return int
 */
static adf_os_inline a_status_t
__adf_os_setup_intr(__adf_os_device_t osdev, adf_os_drv_intr fn)
{
    return A_STATUS_OK;
}

/**
 * @brief release the IRQ
 * 
 * @param irq (irq number)
 * @param sc (cookie)
 */
static adf_os_inline void
__adf_os_free_intr(__adf_os_device_t osdev)
{
     return;
}
#endif
