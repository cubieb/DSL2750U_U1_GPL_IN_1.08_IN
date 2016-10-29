#! /bin/sh
#
# Install DSL CPE API Driver for Amazon-SE
#

# check for linux 2.6.x
uname -r | grep -q 2.6.
if [ $? -eq 0 ]; then
    MODEXT=.ko
fi

#drv_major_number=107
drv_dev_base_name=dsl_cpe_api
drv_obj_file_name=drv_dsl_cpe_api$MODEXT


echo "- loading DSL CPE API driver (for Amazon-SE) -"
insmod ./$drv_obj_file_name
# add "drv_major_number=$drv_major_number" for fixed major number

if [ $? -ne 0 ]; then
    echo "- loading driver failed! -"
    exit 1
fi

major_no=`grep $drv_dev_base_name /proc/devices |cut -d' ' -f1`

# exit if major number not found (in case of devfs)
if [ -z $major_no ]; then
    exit 0
fi

echo "- create device nodes for DSL CPE API device driver (for Amazon-SE) -"

prefix=/dev
test ! -d $prefix/ && mkdir $prefix/
test ! -e $prefix/$drv_dev_base_name && mknod $prefix/$drv_dev_base_name c $major_no 0

