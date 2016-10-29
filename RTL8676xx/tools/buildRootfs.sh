#!/bin/sh
#exit if occur error
#FSROOT=${BUILDROOT}/filesystem
#ROMFS=${BUILDDIR}/romfs
#FSROOT=${ROMFS}/rootfs
#rm -rf ${ROMFS}
#mkdir -p ${ROMFS}
#mkdir -p ${FSROOT} 
if [ "-${TOOLCHAINS_SYSROOT}" = "-" ]; then
	echo TOOLCHAINS_SYSROOT Not Defined
	exit -1
fi
echo "Installing base root"
echo "Installing /lib"
mkdir -p ${FSROOT}/lib
#cp -Prf ${TOOLCHAINS_SYSROOT}/lib/*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/libuClibc*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/ld-uClibc*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/libcrypt*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/libc*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/libdl*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/libgcc_s*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/libm*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/libpthread*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/libresolv*.so* ${FSROOT}/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/lib/libutil*.so* ${FSROOT}/lib

chmod -R +w ${FSROOT}/lib/* 
find ${FSROOT}/lib  -type f | xargs ${STRIP}

echo "Installing /usr/lib"
mkdir -p ${FSROOT}/usr/lib
cp -Prf ${TOOLCHAINS_SYSROOT}/usr/lib/*.so* ${FSROOT}/usr/lib
chmod -R +w ${FSROOT}/usr/lib/* 
find ${FSROOT}/usr/lib  -type f | xargs ${STRIP}

exit 0
