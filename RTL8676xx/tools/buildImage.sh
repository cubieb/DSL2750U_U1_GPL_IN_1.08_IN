#!/bin/sh
OBJCPY=${TOOLCHAINS_PATH}/${OBJCOPY}
BACKUP=$1

if [ "${ENDIANNESS}" = "big" ];
then
ENDIAN=be
fi

if [ "${ENDIANNESS}" = "little" ];
then
ENDIAN=le
fi

MKSQUASHFS=./bin/mksquashfs

DOUBLE_BACKUP=`grep CONFIG_DOUBLE_BACKUP ${BUILDDIR}/.config | awk -F"=" '{print $2}'`
FLASH_ERASESIZE=`grep CONFIG_FLASH_ERASESIZE  ${BUILDDIR}/.config | awk -F"=" '{print $2}'`
MKSQUASHFS_BLOCK=`grep CONFIG_MKSQUASHFS_BLOCK ${BUILDDIR}/.config | awk -F"=" '{print $2}'`
BOARD_ID=`grep CONFIG_BOARD_ID ${BUILDDIR}/.config | awk -F"=" '{print $2}' | awk -F"\"" '{print $2}'`
SYSTEM_VERSION=`grep CONFIG_SYSTEM_VERSION ${BUILDDIR}/.config | awk -F"=" '{print $2}' | awk -F"\"" '{print $2}'`
SDRAM_TRANSFER=`grep CONFIG_SDRAM_TRANSFER ${BUILDDIR}/.config | awk -F"=" '{print $2}'`
FLASH_TRANSFER=`grep CONFIG_FLASH_TRANSFER ${BUILDDIR}/.config | awk -F"=" '{print $2}'`
MAC_OUI=`grep CONFIG_MAC_OUI ${BUILDDIR}/.config | awk -F"=" '{print $2}' | awk -F"\"" '{print $2}'`
TBS_APP_CFG_NAME=`grep TBS_APP_CFG ${ROOTDIR}/src/include/flash_layout.h  | awk -F"\"" {'print $2'}`
DRAM_SIZE=`grep CONFIG_DRAM_SIZE ${BUILDDIR}/.config | awk -F"=" '{print $2}'`
SUPPORT_JFFS2=`grep CONFIG_MTD_JFFS2 ${BUILDDIR}/.config -w | awk -F"=" '{print $2}'`
SINGLE_IMAGE=`grep CONFIG_SINGLE_IMAGE ${BUILDDIR}/.config | awk -F"=" '{print $2}'`
DUAL_IMAGE=`grep CONFIG_DUAL_IMAGE ${BUILDDIR}/.config | awk -F"=" '{print $2}'`

if [ "${DOUBLE_BACKUP}" = "y" ];
then
IMG_TYPE=imgd
else
IMG_TYPE=imgs
fi

if [ "${SDRAM_TRANSFER}" = "y" ];
then
SYSCONFIG_TRANSFER_MODE=sdram
fi

if [ "${FLASH_TRANSFER}" = "y" ];
then
SYSCONFIG_TRANSFER_MODE=flash
fi

# Create random MAC address
MAC="$MAC_OUI:`printf %x $[($RANDOM % 256)]`:`printf %x $[($RANDOM % 256)]`:`printf %x $[($RANDOM % 256)]`"

cd  $ROOTDIR/tools/


if [ "${ARCH}" = "um" ];
then
    ./bin/mksquashfs  ../build/romfs/rootfs  ../build/romfs/rootfs.squashfs.img  -noI -noD -noF -no-fragments -noappend -all-root -${ENDIAN}

    exit 0

else

   ${OBJCPY} -S -O binary  ${BUILDDIR}/kernel/${TBS_KERNEL_VERSION}/vmlinux  ${ROMFS}/vmlinux.bin
   ./bin/lzma e ${ROMFS}/vmlinux.bin ${ROMFS}/vmlinux.bin.lzma

fi
	${MKSQUASHFS} ${ROMFS}/rootfs ${ROMFS}/rootfs.img -comp lzma -noappend -b ${MKSQUASHFS_BLOCK}
#	${MKSQUASHFS} ${ROMFS}/rootfs ${ROMFS}/rootfs.img -comp lzma -${ENDIAN} -noappend -b ${MKSQUASHFS_BLOCK}

if [ "${BACKUP}" = "backup" ];
then
    echo "make backup image"
    ./bin/mkimage_ex -b ${ROMFS}/bootloader.bin -k1 ${ROMFS}/vmlinux_mini.bin.lzma -k2 ${ROMFS}/vmlinux.bin.lzma  -r1 ${ROMFS}/rootfs_mini.img -r2 ${ROMFS}/rootfs.img -mac $MAC -ip 192.168.1.1 -${ENDIAN} -obin ${ROMFS}/${PRODUCT}_backup.bin -oimg ${ROMFS}/${PRODUCT}_backup.img -id ${BOARD_ID}  -blk ${FLASH_ERASESIZE}  -boot 1 -v ${SYSTEM_VERSION} -p ${DIGIT_SIG} -t $SYSCONFIG_TRANSFER_MODE -app_cfg ${TBS_APP_CFG_NAME} ${ROMFS}/tbs_app_cfg
    ./bin/mkimage_ex -b ${ROMFS}/bootloader.bin -k1 ${ROMFS}/vmlinux.bin.lzma -r1 ${ROMFS}/rootfs.img -k2 ${ROMFS}/vmlinux.bin.lzma -r2 ${ROMFS}/rootfs.img -mac $MAC -ip 192.168.1.1 -${ENDIAN} -obin ${ROMFS}/${PRODUCT}_double.bin -id ${BOARD_ID} -blk ${FLASH_ERASESIZE} -boot 0 -v ${SYSTEM_VERSION} -img_type imgb -p ${DIGIT_SIG} -t $SYSCONFIG_TRANSFER_MODE -app_cfg ${TBS_APP_CFG_NAME} ${ROMFS}/tbs_app_cfg
  
elif [ "${SINGLE_IMAGE}" = "y" ];
then
    echo "make single image"
    IMG_TYPE=imgs
    ./bin/mkimage_ex -b ${ROMFS}/bootloader.bin -k1 ${ROMFS}/vmlinux.bin.lzma  -r1 ${ROMFS}/rootfs.img -mac $MAC -ip 192.168.1.1 -${ENDIAN} -obin ${ROMFS}/${PRODUCT}.bin -oimg ${ROMFS}/${PRODUCT}.img -id ${BOARD_ID}  -blk ${FLASH_ERASESIZE}  -boot 0 -v ${SYSTEM_VERSION} -img_type ${IMG_TYPE} -p ${DIGIT_SIG} -t $SYSCONFIG_TRANSFER_MODE -app_cfg ${TBS_APP_CFG_NAME} ${ROMFS}/tbs_app_cfg
    
elif [ "${DUAL_IMAGE}" = "y" ];
then
    echo "make dual image"
    IMG_TYPE=imgd
    ./bin/mkimage_ex -b ${ROMFS}/bootloader.bin -k1 ${ROMFS}/vmlinux.bin.lzma  -r1 ${ROMFS}/rootfs.img -mac $MAC -ip 192.168.1.1 -${ENDIAN} -obin ${ROMFS}/${PRODUCT}.bin -oimg ${ROMFS}/${PRODUCT}.img -id ${BOARD_ID}  -blk ${FLASH_ERASESIZE}  -boot 0 -v ${SYSTEM_VERSION} -img_type ${IMG_TYPE} -p ${DIGIT_SIG} -t $SYSCONFIG_TRANSFER_MODE -app_cfg ${TBS_APP_CFG_NAME} ${ROMFS}/tbs_app_cfg
    ./bin/mkimage_ex -b ${ROMFS}/bootloader.bin -k1 ${ROMFS}/vmlinux.bin.lzma -r1 ${ROMFS}/rootfs.img -k2 ${ROMFS}/vmlinux.bin.lzma -r2 ${ROMFS}/rootfs.img -mac $MAC -ip 192.168.1.1 -${ENDIAN} -obin ${ROMFS}/${PRODUCT}_double.bin -id ${BOARD_ID} -blk ${FLASH_ERASESIZE} -boot 0 -v ${SYSTEM_VERSION} -img_type imgb -p ${DIGIT_SIG} -t $SYSCONFIG_TRANSFER_MODE -app_cfg ${TBS_APP_CFG_NAME} ${ROMFS}/tbs_app_cfg

fi

cd ${ROMFS}
if [ "${BACKUP}" = "backup" ];
then
	ln -sfv ${PRODUCT}_backup.bin image.bin
	ln -sfv ${PRODUCT}_backup.img image.img
	ln -sfv ${PRODUCT}_double.bin image_double.bin
else
	ln -sfv ${PRODUCT}.bin image.bin
	ln -sfv ${PRODUCT}.img image.img
	ln -sfv ${PRODUCT}_double.bin image_double.bin
fi

echo
echo
echo  "==================================================="
echo  "|                    Image Info                   |"
echo  "---------------------------------------------------"
echo  "| PRODUCT:             ${PRODUCT}"
echo  "| DIGIT SIG:           ${DIGIT_SIG}"
echo  "| Endian:              ${ENDIANNESS}"
echo  "| DRAM Size:           ${DRAM_SIZE} Mbytes"
echo  "| Flash erase size:    ${FLASH_ERASESIZE} Kbytes"
echo  "| Cfg repeat mode:     ${SYSCONFIG_TRANSFER_MODE}"
echo  "| Double Backup:       ${DOUBLE_BACKUP}"
echo  "| IMG_TYPE:            ${IMG_TYPE}"
echo  "| Support Jffs2:       ${SUPPORT_JFFS2}"
echo  "| Squashfs block:      ${MKSQUASHFS_BLOCK}"
echo  "| Board ID:            ${BOARD_ID}"
echo  "| System version:      ${SYSTEM_VERSION}"
echo  "| MAC OUI:             ${MAC_OUI}"
echo  "| Original MAC:        ${MAC}"
echo  "---------------------------------------------------"
