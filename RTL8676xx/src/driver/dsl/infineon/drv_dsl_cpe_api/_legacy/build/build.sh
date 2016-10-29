#! /bin/sh

ARCH="mips"
BUILD_ROOT=

# set home path of DSL CPE API (convert relative path to absolute is necessary!)
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"
MYHOME=`dirname "$ABS_PATH"`/..

# build arguments (and default values if necessary)
DEVICE=
INC_SOAP=0
INC_DEBUG=0
INC_DEBUG_MSGLOG=0
INC_ADSL_MIB=0
INC_ADSL_LED=0
INC_ADSL_CEOC=0
# additional variables that will be set to fixed values according to given
# device name later on
CHANNELS_PER_LINE=
KERNEL_INCLUDE=
KERNEL_BUILD=
DRIVER_INCLUDE=
EN_SOAP=
EN_DEBUG=
EN_DEBUG_MSGLOG=
EN_ADSL_MIB=
EN_ADSL_LED=
EN_ADSL_CEOC=
EN_ADSL_WINHOST_DEBUG=
EN_ADSL_AUTOBOOT=

CONF_KERNEL_BUILD=
CONF_DRIVER_INCLUDE=
CONF_DRV_CFLAGS=


#############################################################
# helper functions
#############################################################
# print usage description
usage()
{
    echo
    echo "Usage:"
    echo "$0"
    echo "   <--device={dev}>"
    echo "   [--arch={arch}]"
    echo "   [--add-drv-cflags={cflags}]"
    echo "   [--inc-soap={inc}]"
    echo "   [--inc-debug={inc}]"
    echo "   [--inc-debug-msglog={inc}]"
    echo "   [--inc-adsl-mib={inc}]"
    echo "   [--inc-adsl-led={inc}]"
    echo "   [--inc-adsl-ceoc={inc}]"
    echo "   [--inc-adsl-winhost-debug={inc}]"
    echo "   [--inc-adsl-autoboot={inc}]"
    echo
    echo "with:"
    echo "  dev  (device selection) - danube"
    echo "  arch (architecture)     - i386 | mips | powerpc"
    echo "  cflags (Add CFLAGS)     - any combination of CFLAGS"
    echo "  inc  (include feature)  - 0 | 1  (0:no, 1:yes)"
    echo "remarks:"
    echo "  <..> - necessary parameter"
    echo "  [..] - optional parameter (default value defined)"
    echo
    echo Example: $0 --device=danube

    exit 1
}

#print arguments
print_args()
{
    echo
    echo "User defined configurations"
    echo "==========================="
    echo "DEVICE=$DEVICE"
    echo "ARCH=$ARCH"
    echo "CONF_DRV_CFLAGS=$CONF_DRV_CFLAGS"
    echo "INC_SOAP=$INC_SOAP"
    echo "INC_DEBUG=$EN_DEBUG"
    echo "INC_DEBUG_MSGLOG=$EN_DEBUG_MSGLOG"
    echo "INC_ADSL_MIB=$EN_ADSL_MIB"
    echo "INC_ADSL_LED=$EN_ADSL_LED"
    echo "INC_ADSL_CEOC=$EN_ADSL_CEOC"
    echo "INC_ADSL_WINHOST_DEBUG=$EN_ADSL_WINHOST_DEBUG"
    echo "INC_ADSL_AUTOBOOT=$EN_ADSL_AUTOBOOT"
    echo
    echo "Internal variables"
    echo "=================="
    echo "MYHOME=$MYHOME"
    echo "KERNEL_INCLUDE=$KERNEL_INCLUDE"
    echo "KERNEL_BUILD=$KERNEL_BUILD"
    echo "CONF_DRIVER_INCLUDE=$CONF_DRIVER_INCLUDE"
    echo "CHANNELS_PER_LINE=$CHANNELS_PER_LINE"
    echo
    echo
}

# check the return value and abort the script in case of error
check_return()
{
    if [ $? -ne 0 ]; then
      cd $MYHOME
      echo package build failed
      exit 1
    fi
}


#############################################################
# scan script arguments
#############################################################
while test $# != 0
do
  case $1 in
  --*=*)
    ac_option=`expr "x$1" : 'x\([^=]*\)='`
    ac_optarg=`expr "x$1" : 'x[^=]*=\(.*\)'`
    ;;
  *) # This is not an option
     ac_option=$1
     ac_need_defaults=false;;
  esac

  case $ac_option in
  --help | --hel | --he | --h | -h )
    usage
    exit 1 ;;
  --arch )
    case $ac_optarg in
      i386 | mips | powerpc )
        ARCH="$ac_optarg";;
      *)
        echo "error: unrecognized architecture: $1"
        exit 1 ;;
    esac ;;
  --add-drv-cflags )
    CONF_DRV_CFLAGS="$ac_optarg";;
  --device | --dev )
    case $ac_optarg in
      danube )
        DEVICE="$ac_optarg";;
      *)
        echo "error: unrecognized device: $1"
        exit 1 ;;
    esac ;;
  --inc-soap )
    INC_SOAP="$ac_optarg";;
  --inc-debug )
    INC_DEBUG="$ac_optarg";;
  --inc-debug-msglog )
    INC_DEBUG_MSGLOG="$ac_optarg";;
  --inc-adsl-mib )
    INC_ADSL_MIB="$ac_optarg";;
  --inc-adsl-led )
    INC_ADSL_LED="$ac_optarg";;
  --inc-adsl-ceoc )
    INC_ADSL_CEOC="$ac_optarg";;
  --inc-adsl-winhost-debug )
    INC_ADSL_WINHOST_DEBUG="$ac_optarg";;
  --inc-adsl-autoboot )
    INC_ADSL_AUTOBOOT="$ac_optarg";;

  # This is an error.
  -*)
    echo "error: unrecognized option: $1"
    exit 1 ;;
  esac
  shift
done

BUILD_ROOT=~/projects/buildroot/build_$ARCH

if [ "$ARCH" == "i386" ]; then
    CROSS_PATH=
    CROSS_CC='gcc'
    CROSS_AR='ar'
    CROSS_LD='ld'
    CROSS_STRIP='strip'
    CROSS_HOST='--host=i386-linux'
    CROSS_TARGET='--target=i386-linux'
fi

if [ "$ARCH" == "mips" ]; then
    KERNEL_INCLUDE=$BUILD_ROOT/linux-2.4.31-easy50712/include
    CROSS_PATH=$BUILD_ROOT/staging_dir/bin/$ARCH-linux-uclibc
    CROSS_CC=$CROSS_PATH-gcc
    CROSS_AR=$CROSS_PATH-ar
    CROSS_LD=$CROSS_PATH-ld
    CROSS_STRIP=$CROSS_PATH-strip
    CROSS_HOST=--host=$ARCH-linux-uclibc
fi

if [ "$ARCH" == "powerpc" ]; then
    KERNEL_INCLUDE=$BUILD_ROOT/linux-2.4.31/include
    CROSS_PATH=$BUILD_ROOT/staging_dir/bin/$ARCH-linux-uclibc
    CROSS_CC=$CROSS_PATH-gcc
    CROSS_AR=$CROSS_PATH-ar
    CROSS_LD=$CROSS_PATH-ld
    CROSS_STRIP=$CROSS_PATH-strip
    CROSS_HOST=--host=$ARCH-linux-uclibc
fi


#############################################################
# sequrity checks and assignment of device specific fixed
# variable settings
#############################################################
# check if device has been specified
if [ "$DEVICE" == "" ]; then
    usage
    exit 1
fi

# set device specific variables here
case $DEVICE in
    danube )
        CHANNELS_PER_LINE=2
        #INCLUDE="-I$BUILD_ROOT/drv_peb55508/src -I$BUILD_ROOT/drv_peb55508/src/common/src"
        ;;
esac

#############################################################
# set configure strings
#############################################################

# SOAP suppport
if [ "$INC_SOAP" = "1" ]
then
    EN_SOAP="--enable-soap-support"
else
    EN_SOAP=""
fi

# Debug suppport
if [ "$INC_DEBUG" = "1" ]
then
    EN_DEBUG="--enable-debug-prints=yes"
else
    EN_DEBUG="--enable-debug-prints=no"
fi

# Debug suppport for message log
if [ "$INC_DEBUG_MSGLOG" = "1" ]
then
    EN_DEBUG_MSGLOG="--enable-debug-msg-log"
else
    EN_DEBUG_MSGLOG=""
fi

# ADSL MIB suppport
if [ "$INC_ADSL_MIB" = "1" ]
then
    EN_ADSL_MIB="--enable-adsl-mib"
else
    EN_ADSL_MIB=""
fi

# ADSL LED suppport
if [ "$INC_ADSL_LED" = "1" ]
then
    EN_ADSL_LED="--enable-adsl-led"
else
    EN_ADSL_LED=""
fi

# ADSL CEOC suppport
if [ "$INC_ADSL_CEOC" = "1" ]
then
    EN_ADSL_CEOC="--enable-adsl-ceoc"
else
    EN_ADSL_CEOC=""
fi

# ADSL Winhost support for debug
if [ "$INC_ADSL_WINHOST_DEBUG" = "1" ]
then
    EN_ADSL_WINHOST_DEBUG="--enable-adsl-winhost-debug"
else
    EN_ADSL_WINHOST_DEBUG=""
fi

# ADSL autoboot handling in driver
if [ "$INC_ADSL_AUTOBOOT" = "1" ]
then
    EN_ADSL_AUTOBOOT="--enable-adsl-autoboot"
else
    EN_ADSL_AUTOBOOT=""
fi

# Kernel include
if [ "$KERNEL_INCLUDE" = "" ]
then
    case $ARCH in
        mips | powerpc )
            echo error: no kernel include directory specified!
            exit 1
            ;;
    esac
fi

# Kernel build
if [ "$KERNEL_BUILD" != "" ]
then
    CONF_KERNEL_BUILD="--enable-kernel-build="$KERNEL_BUILD
fi

# Driver include
if [ "$DRIVER_INCLUDE" = "" ]
then
    CONF_DRIVER_INCLUDE="--enable-driver-include="-I$MYHOME/src/include
fi


#############################################################
# preparation of configure/make process
#############################################################

#print_args

# dont configure during autogen
#export NOCONFIGURE=1

#echo generate autogen files
#$MYHOME/autogen.sh --help

#export NOCONFIGURE=

# take care that NO makefile exists in the top level directory of the DSL CPE API
#echo clean the package directory
if [ -f $MYHOME/Makefile ]
then
   make -s -C $MYHOME distclean
fi

echo remove the build directories
rm -Rf $MYHOME/build_$DEVICE

echo create the build directories
mkdir -p $MYHOME/build_$DEVICE


#############################################################
# start configure and make process with specified settings
#############################################################

echo "configure $DEVICE"

cd $MYHOME/build_$DEVICE

#print_args

$MYHOME/configure \
    CC=$CROSS_CC \
    AR=$CROSS_AR \
    LD=$CROSS_LD \
    STRIP=$CROSS_STRIP \
    $CROSS_HOST \
    $CROSS_TARGET \
    --enable-kernel-include=$KERNEL_INCLUDE \
    $CONF_DRIVER_INCLUDE \
    $CONF_KERNEL_BUILD \
    --build=i386-linux \
    --enable-add-drv-cflags="$CONF_DRV_CFLAGS" \
    --enable-$DEVICE \
    --with-channels-per-line=$CHANNELS_PER_LINE \
    $EN_DEBUG \
    $EN_DEBUG_MSGLOG \
    $EN_SOAP \
    $EN_ADSL_MIB \
    $EN_ADSL_LED \
    $EN_ADSL_CEOC \
    $EN_ADSL_WINHOST_DEBUG \
    $EN_ADSL_AUTOBOOT

check_return

echo "build $DEVICE"
make -s
check_return

cd $MYHOME

exit 0
