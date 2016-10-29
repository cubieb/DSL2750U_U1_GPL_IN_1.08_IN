#!/bin/sh

if [ $# -ne 0 ]
then
  echo "================================================="
  echo "ERROR: This script not need parameter. For example:"
  echo "$0"
  echo "================================================="
  exit;
fi

IsKernelHide=0

###############################################
#              compile all code
###############################################
make prepare && \
make tools && \
make V=1

Product=`awk -F= '{if("PRODUCT"==$1) {printf $2}}' ./build/.product`
cp ../customer/product/$Product/app_btn_cfg.c ./src/apps/ssap/button/ -rvf

echo "###############################################"
echo "            make application layer file        "
echo "###############################################"
cp ./src/apps/ssap/lib/tbserror.c .tbserror.c
cp ./src/apps/ssap/cfg cfg -rvf
cp ./src/apps/ssap/lib/tbsutil.c .tbsutil.c

# copy the code to the src directory to generate .obj
echo "#######################################################"
echo "  copy the code to the src directory to generate .obj  "
echo "#######################################################"
cp ../customer/apps/logic ./src/apps/logic -R
cp ../customer/apps/html ./src/apps/html -R
cp ../customer/apps/cli ./src/apps/cli -R
# 3G has shell to execute
chmod -R 777 ./src/apps/logic

if [ -f tools/release_dir ]
then 
    while read line
    do
      echo $line|grep "\/kernel">/dev/null;
      if [ $? -ne 0 ];then
        #Not find /kernel
        #echo $line|grep "customer">/dev/null;
        #if [ $? -eq 0 ];then
        #  build_dir=`echo $line|sed 's/..\/customer\//build\//g'`
        #  for file in `find ${build_dir} -name *.o`;
        #  do
        #    src_lib_file=`echo $file|sed 's/build\//..\/customer\//g'`
        #    cp $file ${src_lib_file}
        #  done
        #  find $line -name '*.c'|xargs rm -rvf
        #else
          make -C $line ROOTDIR=`pwd` BUILDDIR=`pwd`/build
          find $line -name '*.c'|xargs rm -rvf
          find $line -name '*.d'|xargs rm -rvf
          find $line -name '*.flags'|xargs rm -rvf
          for file in `find ${line} -name "*.Plo"`;
          do
            sed 's/\.cpp/\.o/g' ${file} > .tmp
            mv .tmp ${file}
            sed 's/\.c/\.o/g' ${file} > .tmp
            mv .tmp ${file}
          done
        #fi
      else
        build_dir=`echo $line|sed 's/src\//build\//g'`
	      for file in `find ${build_dir} -name *.o*`;
        do
          echo $file|grep "o.cmd">/dev/null;
          if [ $? -ne 0 ];then
            src_lib_file=`echo $file|sed 's/build\//src\//g'|sed 's/\.o$/.lib/g'`
            cp $file ${src_lib_file}

            #src_c_file=`echo $file|sed 's/build\//src\//g'|sed 's/\.o$/.c/g'`
            #rm -rvf ${src_c_file}

            src_makefile=`echo ${src_lib_file}|sed -n 's:/[^/]*$::p'|sed 's/$/\/Makefile/g'`
            sed 's/\.o/.lib/g' ${src_makefile} >.tmp
            mv .tmp ${src_makefile}
          else
            src_cmd_file=`echo $file|sed 's/build\//src\//g'`
            cp ${file} ${src_cmd_file}
          fi
        done
        for file in `find ${build_dir} -name *.ko`;
        do
          src_ko_file=`echo $file|sed 's/build\//src\//g'`
          cp $file $src_ko_file
        done
	      find $line -name '*.c'|xargs rm -rvf
        IsKernelHide=1;
      fi
    done <./tools/release_dir
fi

mv .tbserror.c ./src/apps/ssap/lib/tbserror.c
cp cfg/* ./src/apps/ssap/cfg -R
rm -rvf cfg
mv .tbsutil.c ./src/apps/ssap/lib/tbsutil.c

# move back the directory
echo "##############################################"
echo "          remove customer/apps                "
echo "##############################################"
rm -rvf ../customer/apps/logic
rm -rvf ../customer/apps/html
rm -rvf ../customer/apps/cli

echo "##############################################"
echo "          mv  src/apps --> customer/apps         "
echo "##############################################"
mv ./src/apps/logic ../customer/apps/logic
mv ./src/apps/html ../customer/apps/html
mv ./src/apps/cli ../customer/apps/cli

echo "##############################################"
echo "          remove vendor source                "
echo "##############################################"
grep "CONFIG_VENDOR_ATHERNOS=y" .config>/dev/null;
if [ $? -ne 0 ];then
#   rm -rvf ./src/vendor/atheros
   find ./src/vendor/atheros -name '*.c'|xargs rm -rvf
   sed -e '/atheros/d' ./src/vendor/Config.in > .tmp
   mv .tmp ./src/vendor/Config.in
fi

grep "CONFIG_VENDOR_INFINEON=y" .config>/dev/null;
if [ $? -ne 0 ];then
#   rm -rvf ./src/vendor/infineon
   find ./src/vendor/infineon -name '*.c'|xargs rm -rvf
   sed -e '/infineon/d' ./src/vendor/Config.in > .tmp
   mv .tmp ./src/vendor/Config.in
fi

grep "CONFIG_VENDOR_RALINK=y" .config>/dev/null;
if [ $? -ne 0 ];then
#   rm -rvf ./src/vendor/ralink
   find ./src/vendor/ralink -name '*.c'|xargs rm -rvf
   sed -e '/ralink/d' ./src/vendor/Config.in > .tmp
   mv .tmp ./src/vendor/Config.in
fi

grep "CONFIG_VENDOR_IKANOS=y" .config>/dev/null;
if [ $? -ne 0 ];then
#   rm -rvf ./src/vendor/ikanos
   find ./src/vendor/ikanos -name '*.c'|xargs rm -rvf
   sed -e '/ikanos/d' ./src/vendor/Config.in > .tmp
   mv .tmp ./src/vendor/Config.in
fi

echo "#################################################"
echo "           remove driver source                  "
echo "#################################################"
grep "CONFIG_DRIVERS_WLAN_ATHEROS=y" .config>/dev/null;
if [ $? -ne 0 ];then
#   rm -rvf ./src/driver/wireless/atheros
   find ./src/driver/wireless/atheros -name '*.c'|xargs rm -rvf
   sed -e '/atheros/d' ./src/driver/wireless/Config.in > .tmp
   mv .tmp ./src/driver/wireless/Config.in
fi

grep "CONFIG_DRIVERS_WLAN_RALINK=y" .config>/dev/null;
if [ $? -ne 0 ];then
#   rm -rvf ./src/driver/wireless/ralink
   find ./src/driver/wireless/ralink -name '*.c'|xargs rm -rvf
   sed -e '/ralink/d' ./src/driver/wireless/Config.in > .tmp
   mv .tmp ./src/driver/wireless/Config.in
fi

grep "CONFIG_INFINEON_DSL_DRIVERS=y" .config>/dev/null;
if [ $? -ne 0 ];then
#   rm -rvf ./src/driver/dsl/infineon
   find ./src/driver/dsl/infineon -name '*.c'|xargs rm -rvf
fi

grep "CONFIG_IKANOS_DSL_DRIVERS=y" .config>/dev/null;
if [ $? -ne 0 ];then
   find ./src/driver/dsl/ikanos -name '*.c'|xargs rm -rvf
   find ./src/driver/dsl/ikanos -name '*.h'|xargs rm -rvf
fi

echo "#################################################"
echo "           remove WEB_STYLE source               "
echo "#################################################"
grep "CONFIG_APPS_HTML_WEB_STYLE_DLINK=y" .config>/dev/null;
if [ $? -ne 0 ];then
   rm -rvf ../customer/apps/html/skins/dlink
fi

grep "CONFIG_APPS_HTML_WEB_STYLE_EDIMAX=y" .config>/dev/null;
if [ $? -ne 0 ];then
   rm -rvf ../customer/apps/html/skins/edimax
fi

echo "################################################"
echo "           remove product dir                   "
echo "################################################"
Product=`awk -F= '{if("PRODUCT"==$1) {printf $2}}' ./build/.product`
cd ../customer/product
ls | grep -x -v $Product| xargs rm -rvf
cd ..

echo "================================================="
echo "------------Congratulations, GPL finished !--------------"
echo "================================================="

