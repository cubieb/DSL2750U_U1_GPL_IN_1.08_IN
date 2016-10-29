#ifndef _ADF_CMN_OS_IO_PVT_H
#define _ADF_CMN_OS_IO_PVT_H

#include <asm/io.h>
#include <asm/byteorder.h>
#include <linux/byteorder/generic.h>

#define __adf_os_reg_read8(_dev, _addr)     \
  readb((volatile void __iomem *)((_dev)->res.vaddr + (_addr)))

#define __adf_os_reg_read16(_dev, _addr)    \
  readw((volatile void __iomem *)((_dev)->res.vaddr + (_addr)))

#define __adf_os_reg_read32(_dev, _addr)    \
  readl((volatile void __iomem *)((_dev)->res.vaddr + (_addr)))
  
#define __adf_os_reg_read64(_dev, _addr)    \
  readq((volatile void __iomem *)((_dev)->res.vaddr + (_addr)))


#define __adf_os_reg_write8(_dev, _addr, _val)      \
  writeb(_val, (volatile void __iomem *)((_dev)->res.vaddr + (_addr)))

#define __adf_os_reg_write16(_dev, _addr, _val)     \
  writew(_val, (volatile void __iomem *)((_dev)->res.vaddr + (_addr)))

#define __adf_os_reg_write32(_dev, _addr, _val)     \
  writel(_val, (volatile void __iomem *)((_dev)->res.vaddr + (_addr)))

#define __adf_os_reg_write64(_dev, _addr, _val)     \
  writeq(_val, (volatile void __iomem *)((_dev)->res.vaddr + (_addr)))

#define __adf_os_ntohs                      ntohs
#define __adf_os_ntohl                      ntohl

#define __adf_os_htons                      htons
#define __adf_os_htonl                      htonl

#define __adf_os_cpu_to_le16                cpu_to_le16
#define __adf_os_cpu_to_le32                cpu_to_le32
#define __adf_os_cpu_to_le64                cpu_to_le64

#define __adf_os_cpu_to_be16                cpu_to_be16
#define __adf_os_cpu_to_be32                cpu_to_be32
#define __adf_os_cpu_to_be64                cpu_to_be64

#define __adf_os_le16_to_cpu                le16_to_cpu
#define __adf_os_le32_to_cpu                le32_to_cpu
#define __adf_os_le64_to_cpu                le64_to_cpu

#define __adf_os_be16_to_cpu                be16_to_cpu
#define __adf_os_be32_to_cpu                be32_to_cpu
#define __adf_os_be64_to_cpu                be64_to_cpu

#endif /*_ADF_CMN_OS_IO_PVT_H*/
