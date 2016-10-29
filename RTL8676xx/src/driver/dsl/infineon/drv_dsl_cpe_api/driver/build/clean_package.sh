#! /bin/sh

ARCH=powerpc
BUILD_ROOT=~/projects/buildroot/build_$ARCH
CROSS_PATH=$BUILD_ROOT/staging_dir/bin/$ARCH-linux-uclibc
MYHOME=`pwd`

# check the parameter
usage()
{
   echo
   echo "Usage:"
   echo "$0 version device-to-leave"
   echo
   echo "Example: $0 2.3.0 danube"
   echo "Remove from drv_dsl_cpe_api-2.3.0.tar.gz the vinax parts"
   echo "and will generate a file drv_dsl_cpe_api_danube-2.3.0.tar.gz"
   echo
   echo "Valid devices are"
   echo "- danube"
   echo "- amazon_se"
   echo "- vinax"
   exit 1
}

# check the return value and abort the script in case of error
check_return()
{
    if [ $? -ne 0 ]; then
      cd $MYHOME
      echo Error: package build failed
      exit 1
    fi
}

# remove specified parts
function remove_part
{
    for x in $@; do
      echo "removing $x" ;
      #find $ORIG_DIST -name '*'$x'*'
      rm -rf `find $ORIG_DIST -name '*'$x'*'`
    done
}


if [ "$2" == "" ]; then
    usage
    exit 1
fi

ORIG_DIST=drv_dsl_cpe_api-$1

if [ -e $ORIG_DIST.tar.gz ]; then
    zcat $ORIG_DIST.tar.gz | tar -xvf - 2>&1 > log.txt
    check_return

    echo removing stuff not used for $2
    # update lists below for files/directories to be deleted!
    case $2 in
       vinax )
          remove_part danube
          remove_part amazon_se
          ;;
       danube )
          remove_part amazon_se
          remove_part vinax
          remove_part sar
          ;;
       amazon_se )
          # Do not remove Danube part because this files includes the implementation!
          #remove_part danube
          remove_part vinax
          remove_part sar
          ;;
       * )
          echo "Unknown chip $2!"
          usage
          exit 1
          ;;
    esac

    chmod -x `find $ORIG_DIST -name '*.c'`
    chmod -x `find $ORIG_DIST -name '*.h'`
    check_return

    echo building new package for $2 device
    tar chof - $ORIG_DIST | GZIP=--best gzip -c > drv_dsl_cpe_api_$2-$1.tar.gz
    check_return

    echo clean temp directory
    rm -rf $ORIG_DIST
    check_return
else
    echo
   echo Error: cannot find distribution source file ./$ORIG_DIST.tar.gz
    usage
   exit 1
fi

cd $MYHOME

exit 0

