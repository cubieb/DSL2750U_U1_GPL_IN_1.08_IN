TBS bootloader说明：
1.现在bootloader支持kermmit、tftp和web升级，但是kermmit和web实现有些冲突，默认将web实现关闭，kermmit可用，开关在include/common.h 34行。


2.TBS AR7130目前已经支持的SPI Flash列表如下：

name				device_id		ext_device_id		sector_count		sector_size		size		page_size
spansion S25FL128P		0x12018		0x300			64			256KB			16MB		256B
spansion S25FL128A		0x12018		0x301			256			64KB			16MB		256B
spansion S25FL64A		0x10216		NULL			128			64KB			8MB		256B
mx MX25L128AD			0xc22018		NULL			256			64KB			16MB		256B
mx M25P128			0x202018		NULL			64			256KB			16MB		256B
