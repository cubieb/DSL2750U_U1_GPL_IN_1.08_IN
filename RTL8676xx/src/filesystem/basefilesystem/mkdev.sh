#!/bin/sh
#cd $ROOTDIR/src/filesystem/basefilesystem/prodev
#./$PRODUCT.sh
if [ -f $ROOTDIR/product/$PRODUCT/mkdev.sh ];then
	sh $ROOTDIR/product/$PRODUCT/mkdev.sh
else
	sh $ROOTDIR/tools/common_mkdev.sh
fi

