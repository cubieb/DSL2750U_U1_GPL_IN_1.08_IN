#!/bin/sh
echo "prepare kernel!"
set -e
if [ "-${ROOTDIR}" = "-" ]; then
  echo ROOTDIR not defined
  exit ;
fi
source ${ROOTDIR}/build/arch.mk
OSDIR=${ROOTDIR}/build/kernel/${TBS_KERNEL_VERSION}

if [ "${ARCH}" = "um" ]; then
  cd ${OSDIR}
  make ARCH=${ARCH} defconfig KBUILD_OUTPUT=
fi

sh ${ROOTDIR}/tools/sourcedir.sh
ln -sf ${OSDIR}/arch/${ARCH}/Kconfig.main ${ROOTDIR}/build/kernel/Kconfig.main


