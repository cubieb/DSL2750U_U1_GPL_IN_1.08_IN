#!/bin/sh -e
OLDIFS=$IFS
IFS="="
export ROOTDIR=`pwd`
echo making orig logic
cp build/.config origconfig
cd build;../scripts/conf -s ../Config.in >> make.log 2>&1 
cd -  >> make.log 2>&1 
make logic_clean logic logic_install >> make.log 2>&1
origlogic_size=`du -sb build/romfs/rootfs/usr/bin/logic | cut -f 1`
echo '{| border="1" cellpadding="5" cellspacing="0" align="center"' > size.wiki
echo "| logic || $origlogic_size" >> size.wiki
grep CONFIG_APPS_LOGIC_.*=y origconfig |  while read field value; do 
module=`expr "$field" : "CONFIG_APPS_LOGIC_\(.*\)"`
echo "processing remove $module"
sed "s/$field=y/# $field is not set/g" origconfig > build/.config
cd build;../scripts/conf -s ../Config.in  >> make.log 2>&1 
cd -  >> make.log 2>&1 
echo making not $field
make logic_clean logic logic_install >> make.log  2>&1
logic_size=`du -sb build/romfs/rootfs/usr/bin/logic | cut -f 1`
echo "|-" >> size.wiki
echo "| $module || " `expr $origlogic_size - $logic_size` >> size.wiki
done;
echo "|}" >> size.wiki
IFS=$OLDIFS
cat size.wiki

