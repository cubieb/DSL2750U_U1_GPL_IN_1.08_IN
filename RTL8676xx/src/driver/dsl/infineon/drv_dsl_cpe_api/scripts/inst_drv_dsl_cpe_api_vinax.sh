#! /bin/sh
#
# Install DSL CPE driver
#


# check for linux 2.6.x
uname -r | grep -q 2.6.
if [ $? -eq 0 ]; then
    MODEXT=.ko
fi

drv_dev_base_name=dsl_cpe_api
drv_obj_file_name=drv_dsl_cpe_api$MODEXT


echo "- loading DSL CPE driver -"
insmod -f ./$drv_obj_file_name

if [ $? -ne 0 ]; then
    echo "- loading driver failed! -"
    exit 1
fi

major_no=`grep dsl_cpe_api /proc/devices |cut -d' ' -f1`

# exit if major number not found (in case of devfs)
if [ -z $major_no ]; then
    exit 0
fi

echo - create device nodes for DSL CPE driver -

#prefix=/dev/$drv_dev_base_name
#test ! -d $prefix/ && mkdir $prefix/

#for I in 0; do
#    test ! -e $prefix/$I && mknod $prefix/$I c $major_no `expr $I`
#done

prefix=/dev
test ! -d $prefix
test ! -e $prefix/$drv_dev_base_name && mknod $prefix/$drv_dev_base_name c $major_no 0
