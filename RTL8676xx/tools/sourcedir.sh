#!/bin/sh
set -x
if [ "-${TBS_KERNEL_VERSION}" = "-" ]; then
  echo "TBS_KERNEL_VERSION Not defined,please use make prepare_kernel";
  exit -1;
fi

for file in `find ${ROOTDIR}/build/kernel/${TBS_KERNEL_VERSION} -name "Kconfig*" | grep -v ".*\.main"`;do 
#  echo $file;
  #add by zhc
  #replace 'source[ "]KCONFIG' to 'source[ "]build\blp\kernel\src\KCONFIG.main'
  #and replase 'mainmenu" to '#mainmenu'
#  sed 's/\(^[\t ]*source[\t "]*\)\([^" \t]*\)/\1build\/kernel\/'${TBS_KERNEL_VERSION}'\/\2\.main/g;s/mainmenu/#mainmenu/g' $file > $file.main;
  sed 's/\(^[\t ]*source[\t "]*\)\([^" \t]*\)/\1kernel\/'${TBS_KERNEL_VERSION}'\/\2\.main/g;s/mainmenu/#mainmenu/g' $file > $file.main;
#  sed 's/\(.*source[ "]*\)\([^" ]*\)/\1build\/blp\/kernel\/os_src\/\2\.main/g' > temp1;
#  sed 's/source \"/source /g' $file >./temp1;
#  sed '/^source/s/\"$//g' ./temp1>./temp2;
#  sed 's/source /source build\/blp\/kernel\/os_src\//g' ./temp2>./temp3;
#  sed '/^source/s/$/.main/g' ./temp3>$file.main;
#  rm -f ./temp*; 
done;

