#!/bin/sh -x
for file in `find -name Config.in`
do
cp $file $file.main
sed 's/source /source src\/alp\/cc\/opensource\/busybox\/busybox-1.6.1\//g;s/Config.in/Config.in.main/g' $file.main>temp
mv temp $file.main
done

