#! /bin/sh

ARCH="mips"
ROOT_DIR=

# set home path of DSL CPE API (convert relative path to absolute is necessary!)
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"
MYHOME=`dirname "$ABS_PATH"`/..

# build arguments (and default values if necessary)
DEVICE=
INC_DEBUG=0
INC_DEBUG_PRINTS=1
INC_KERNEL_26=0
INC_ADSL_MIB=0
INC_ADSL_LED=0
INC_DSL_CEOC=0
INC_ADSL_TRACE=0
INC_DSL_STATIC_DELT_DATA=0
INC_DSL_PM=0
INC_DSL_PM_CHANNEL_COUNTERS=0
INC_DSL_PM_DATAPATH_COUNTERS=0
INC_DSL_PM_LINE_COUNTERS=0
INC_DSL_PM_CHANNEL_THRESHOLDS=0
INC_DSL_PM_DATAPATH_THRESHOLDS=0
INC_DSL_PM_LINE_THRESHOLDS=0
INC_DSL_PM_TOTAL_COUNTERS=0
INC_DSL_PM_HISTORY=0
INC_DSL_PM_SHOWTIME_COUNTERS=0
INC_DSL_PM_OPTIONAL_PARAMETERS=0
# additional variables that will be set to fixed values according to given
# device name later on
CHANNELS_PER_LINE=
EN_KERNEL_26=
KERNEL_BUILD=
KERNEL_INCLUDE=
DRIVER_INCLUDE=
DRIVER_CFLAGS=
EN_DEBUG=
EN_DEBUG_PRINTS=
EN_ADSL_MIB=
EN_ADSL_LED=
EN_DSL_CEOC=
EN_ADSL_TRACE=
EN_DSL_STATIC_DELT_DATA=
EN_DSL_PM=
EN_DSL_PM_CHANNEL_COUNTERS=
EN_DSL_PM_DATAPATH_COUNTERS=
EN_DSL_PM_LINE_COUNTERS=
EN_DSL_PM_CHANNEL_THRESHOLDS=
EN_DSL_PM_DATAPATH_THRESHOLDS=
EN_DSL_PM_LINE_THRESHOLDS=
EN_DSL_PM_TOTAL_COUNTERS=
EN_DSL_PM_HISTORY=
EN_DSL_PM_SHOWTIME_COUNTERS=
EN_DSL_PM_OPTIONAL_PARAMETERS=

CONF_KERNEL_BUILD=
CONF_DRIVER_INCLUDE=
CONF_DRIVER_CFLAGS=


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
    echo "   [--kernel-build={kbuild}]"
    echo "   [--drv-include={drvinc}]"
    echo "   [--add-drv-cflags={cflags}]"
    echo "   [--channels={ch}]"
    echo "   [--inc-debug={inc}]"
    echo "   [--inc-debug-prints={inc}]"
    echo "   [--inc-adsl-mib={inc}]"
    echo "   [--inc-adsl-led={inc}]"
    echo "   [--inc-dsl-ceoc={inc}]"
    echo "   [--inc-adsl-trace={inc}]"
    echo "   [--inc-dsl-static-delt={inc}]"
    echo "   [--inc-dsl-pm={inc}]"
    echo "   [--inc-dsl-pm-channel-counters={inc}]"
    echo "   [--inc-dsl-pm-datapath-counters={inc}]"
    echo "   [--inc-dsl-pm-line-counters={inc}]"
    echo "   [--inc-dsl-pm-channel-thresholds={inc}]"
    echo "   [--inc-dsl-pm-datapath-thresholds={inc}]"
    echo "   [--inc-dsl-pm-line-thresholds={inc}]"
    echo "   [--inc-dsl-pm-total-counters={inc}]"
    echo "   [--inc-dsl-pm-history={inc}]"
    echo "   [--inc-dsl-pm-showtime-counters={inc}]"
    echo
    echo "with:"
    echo "  dev  (device selection) - danube | vinax | amazon-se"
    echo "  ch  (bearer channels) - 1 | 2"
    echo "  arch (architecture)     - i386 | mips | mipsel | powerpc"
    echo "  kbuild (kernel build directory)"
    echo "  drvinc (driver include) - e.g. \"-I./src/include\""
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
    echo "CONF_KERNEL_BUILD=$CONF_KERNEL_BUILD"
    echo "CONF_DRIVER_INCLUDE=$CONF_DRIVER_INCLUDE"
    echo "CONF_DRIVER_CFLAGS=$CONF_DRIVER_CFLAGS"
    echo "CHANNELS_PER_LINE=$CHANNELS_PER_LINE"
    echo "INC_DEBUG=$INC_DEBUG"
    echo "INC_DEBUG_PRINTS=$INC_DEBUG_PRINTS"
    echo "INC_ADSL_MIB=$EN_ADSL_MIB"
    echo "INC_ADSL_LED=$EN_ADSL_LED"
    echo "INC_DSL_CEOC=$EN_DSL_CEOC"
    echo "INC_ADSL_TRACE=$EN_ADSL_TRACE"
    echo "INC_DSL_STATIC_DELT_DATA=$EN_DSL_STATIC_DELT_DATA"
    echo "INC_DSL_PM=$EN_DSL_PM"
    echo "INC_DSL_PM_CHANNEL_COUNTERS=$INC_DSL_PM_CHANNEL_COUNTERS"
    echo "INC_DSL_PM_DATAPATH_COUNTERS=$INC_DSL_PM_DATAPATH_COUNTERS"
    echo "INC_DSL_PM_LINE_COUNTERS=$INC_DSL_PM_LINE_COUNTERS"
    echo "INC_DSL_PM_CHANNEL_THRESHOLDS=$INC_DSL_PM_CHANNEL_THRESHOLDS"
    echo "INC_DSL_PM_DATAPATH_THRESHOLDS=$INC_DSL_PM_DATAPATH_THRESHOLDS"
    echo "INC_DSL_PM_LINE_THRESHOLDS=$INC_DSL_PM_LINE_THRESHOLDS"
    echo "INC_DSL_PM_TOTAL_COUNTERS=$INC_DSL_PM_TOTAL_COUNTERS"
    echo "INC_DSL_PM_HISTORY=$INC_DSL_PM_HISTORY"
    echo "INC_DSL_PM_SHOWTIME_COUNTERS=$INC_DSL_PM_SHOWTIME_COUNTERS"
    echo "INC_DSL_PM_OPTIONAL_PARAMETERS=$INC_DSL_PM_OPTIONAL_PARAMETERS"
    echo
    echo "Internal variables"
    echo "=================="
    echo "MYHOME=$MYHOME"
    echo "KERNEL_BUILD=$KERNEL_BUILD"
    echo "KERNEL_INCLUDE=$KERNEL_INCLUDE"
    echo "DRIVER_INCLUDE=$DRIVER_INCLUDE"
    echo "DRIVER_CFLAGS=$DRIVER_CFLAGS"
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
      i386 | mips | mipsel | powerpc )
        ARCH="$ac_optarg";;
      *)
        echo "error: unrecognized architecture: $1"
        exit 1 ;;
    esac ;;
  --add-drv-cflags )
    CONF_DRIVER_CFLAGS="$ac_optarg";;
  --kernel-build )
    CONF_KERNEL_BUILD="$ac_optarg";;
  --enable-linux-26 )
    INC_KERNEL_26="$ac_optarg";;
  --drv-include )
    CONF_DRIVER_INCLUDE="$ac_optarg";;
  --device | --dev )
    case $ac_optarg in
      danube | vinax | amazon-se )
        DEVICE="$ac_optarg";;
      *)
        echo "error: unrecognized device: $1"
        exit 1 ;;
    esac ;;
  --inc-debug )
    INC_DEBUG="$ac_optarg";;
  --inc-debug-prints )
    INC_DEBUG_PRINTS="$ac_optarg";;
  --channels )
    CHANNELS_PER_LINE="$ac_optarg";;
  --inc-adsl-mib )
    INC_ADSL_MIB="$ac_optarg";;
  --inc-adsl-led )
    INC_ADSL_LED="$ac_optarg";;
  --inc-dsl-ceoc )
    INC_DSL_CEOC="$ac_optarg";;
  --inc-adsl-trace )
    INC_ADSL_TRACE="$ac_optarg";;
  --inc-dsl-static-delt )
    INC_DSL_STATIC_DELT_DATA="$ac_optarg";;
  --inc-dsl-pm )
    INC_DSL_PM="$ac_optarg";;
  --inc-dsl-pm-channel-counters )
    INC_DSL_PM_CHANNEL_COUNTERS="$ac_optarg";;
  --inc-dsl-pm-datapath-counters )
    INC_DSL_PM_DATAPATH_COUNTERS="$ac_optarg";;
  --inc-dsl-pm-line-counters )
    INC_DSL_PM_LINE_COUNTERS="$ac_optarg";;
  --inc-dsl-pm-channel-thresholds )
    INC_DSL_PM_CHANNEL_THRESHOLDS="$ac_optarg";;
  --inc-dsl-pm-datapath-thresholds )
    INC_DSL_PM_DATAPATH_THRESHOLDS="$ac_optarg";;
  --inc-dsl-pm-line-thresholds )
    INC_DSL_PM_LINE_THRESHOLDS="$ac_optarg";;
  --inc-dsl-pm-total-counters )
    INC_DSL_PM_TOTAL_COUNTERS="$ac_optarg";;
  --inc-dsl-pm-history )
    INC_DSL_PM_HISTORY="$ac_optarg";;
  --inc-dsl-pm-showtime-counters )
    INC_DSL_PM_SHOWTIME_COUNTERS="$ac_optarg";;
  --inc-dsl-pm-optional-parameters )
    INC_DSL_PM_OPTIONAL_PARAMETERS="$ac_optarg";;

  # This is an error.
  -*)
    echo "error: unrecognized option: $1"
    exit 1 ;;
  esac
  shift
done


if [ "$BUILD_ENV" == "open_wrt" ]; then
	ROOT_DIR=~/projects/open_wrt
else
	ROOT_DIR=~/projects/buildroot/build_$ARCH
fi

if [ "$ARCH" == "i386" ]; then
    CROSS_PATH=
    CROSS_CC='gcc'
    CROSS_AR='ar'
    CROSS_LD='ld'
    CROSS_STRIP='strip'
    CROSS_HOST='--host=i386-linux'
    CROSS_TARGET='--target=i386-linux'
else
    FILE="$ROOT_DIR"/.build_dir_suffix
    BUILD_DIR_SUFFIX=`cat "$FILE"`
    BUILD_DIR=$ROOT_DIR/build_dirs/build_"$ARCH"_"$BUILD_DIR_SUFFIX"

	case $DEVICE in
	    vinax )
	        KERNEL_INCLUDE=$BUILD_DIR/linux/include
		    CROSS_PATH=$ROOT_DIR/staging_dir_"$ARCH"/"$ARCH"-linux/bin
	        ;;
	    danube )
		    KERNEL_INCLUDE=$ROOT_DIR/linux-2.4.31-easy50712/include
		    CROSS_PATH=$ROOT_DIR/staging_dir/bin/$ARCH-linux-uclibc
	        ;;
	esac

	if [ "$BUILD_ENV" == "open_wrt" ]; then
        CROSS_CC=$CROSS_PATH/gcc
        CROSS_AR=$CROSS_PATH/ar
        CROSS_LD=$CROSS_PATH/ld
        CROSS_STRIP=$CROSS_PATH/strip
        CROSS_HOST=--host=$ARCH-linux-uclibc
    else
        CROSS_CC=$CROSS_PATH-gcc
        CROSS_AR=$CROSS_PATH-ar
        CROSS_LD=$CROSS_PATH-ld
        CROSS_STRIP=$CROSS_PATH-strip
    fi

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

# check configuration of used bearer channels and set default
# value if not already set
if [ "$CHANNELS_PER_LINE" = "" ]
then
    CHANNELS_PER_LINE=1
fi

# set device specific variables here
case $DEVICE in
    danube )
        if [ "$CONF_DRIVER_INCLUDE" = "" ]
        then
            DRIVER_INCLUDE="-I$KERNEL_INCLUDE/asm/$DEVICE"
        else
            DRIVER_INCLUDE="$CONF_DRIVER_INCLUDE"
        fi
        #DRIVER_INCLUDE="-I$MYHOME/src/include"
        ;;
    vinax )
        if [ "$CONF_DRIVER_INCLUDE" = "" ]
        then
            DRIVER_INCLUDE="-I$BUILD_DIR/drv_vinax/src -I$BUILD_DIR/drv_vinax/src/common/src"
        else
            DRIVER_INCLUDE="$CONF_DRIVER_INCLUDE"
        fi
        ;;
    amazon-se )
        DRIVER_INCLUDE="-I$MYHOME/src/include"
        ;;
esac

#############################################################
# set configure strings
#############################################################

# Debug suppport
if [ "$INC_DEBUG" = "1" ]
then
    EN_DEBUG="--enable-debug=yes"
else
    EN_DEBUG="--enable-debug=no"
fi

if [ "$INC_KERNEL_26" = "1" ]
then
    EN_KERNEL_26="--enable-linux-26"
else
    EN_KERNEL_26=""
fi

# Debug prints suppport
if [ "$INC_DEBUG_PRINTS" = "1" ]
then
    EN_DEBUG_PRINTS="--enable-debug-prints=yes"
else
    EN_DEBUG_PRINTS="--enable-debug-prints=no"
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

# DSL CEOC suppport
if [ "$INC_DSL_CEOC" = "1" ]
then
    EN_DSL_CEOC="--enable-dsl-ceoc"
else
    EN_DSL_CEOC=""
fi

# ADSL TRACE suppport
if [ "$INC_ADSL_TRACE" = "1" ]
then
    EN_ADSL_TRACE="--enable-adsl-trace"
else
    EN_ADSL_TRACE=""
fi

# DSL static DELT data instead of dynamic suppport
if [ "$INC_DSL_STATIC_DELT_DATA" = "1" ]
then
    EN_DSL_STATIC_DELT_DATA="--enable-dsl-static-delt"
else
    EN_DSL_STATIC_DELT_DATA=""
fi

# DSL PM suppport
if [ "$INC_DSL_PM" = "1" ]
then
    EN_DSL_PM="--enable-dsl-pm"
else
    EN_DSL_PM=""
fi

# DSL PM channel counters suppport
if [ "$INC_DSL_PM_CHANNEL_COUNTERS" = "1" ]
then
    EN_DSL_PM_CHANNEL_COUNTERS="--enable-dsl-pm-channel-counters"
else
    EN_DSL_PM_CHANNEL_COUNTERS=""
fi

# DSL PM datapath counters suppport
if [ "$INC_DSL_PM_DATAPATH_COUNTERS" = "1" ]
then
    EN_DSL_PM_DATAPATH_COUNTERS="--enable-dsl-pm-datapath-counters"
else
    EN_DSL_PM_DATAPATH_COUNTERS=""
fi

# DSL PM line counters suppport
if [ "$INC_DSL_PM_LINE_COUNTERS" = "1" ]
then
    EN_DSL_PM_LINE_COUNTERS="--enable-dsl-pm-line-counters"
else
    EN_DSL_PM_LINE_COUNTERS=""
fi

# DSL PM channel thresholds suppport
if [ "$INC_DSL_PM_CHANNEL_THRESHOLDS" = "1" ]
then
    EN_DSL_PM_CHANNEL_THRESHOLDS="--enable-dsl-pm-channel-thresholds"
else
    EN_DSL_PM_CHANNEL_THRESHOLDS=""
fi

# DSL PM datapath thresholds suppport
if [ "$INC_DSL_PM_DATAPATH_THRESHOLDS" = "1" ]
then
    EN_DSL_PM_DATAPATH_THRESHOLDS="--enable-dsl-pm-datapath-thresholds"
else
    EN_DSL_PM_DATAPATH_THRESHOLDS=""
fi

# DSL PM line thresholds suppport
if [ "$INC_DSL_PM_LINE_THRESHOLDS" = "1" ]
then
    EN_DSL_PM_LINE_THRESHOLDS="--enable-dsl-pm-line-thresholds"
else
    EN_DSL_PM_LINE_THRESHOLDS=""
fi

# DSL PM total counters suppport
if [ "$INC_DSL_PM_TOTAL_COUNTERS" = "1" ]
then
    EN_DSL_PM_TOTAL_COUNTERS="--enable-dsl-pm-total"
else
    EN_DSL_PM_TOTAL_COUNTERS=""
fi

# DSL PM history suppport
if [ "$INC_DSL_PM_HISTORY" = "1" ]
then
    EN_DSL_PM_HISTORY="--enable-dsl-pm-history"
else
    EN_DSL_PM_HISTORY=""
fi

# DSL PM showtime counters suppport
if [ "$INC_DSL_PM_SHOWTIME_COUNTERS" = "1" ]
then
    EN_DSL_PM_SHOWTIME_COUNTERS="--enable-dsl-pm-showtime"
else
    EN_DSL_PM_SHOWTIME_COUNTERS=""
fi

# DSL PM optional parameters suppport
if [ "$INC_DSL_PM_OPTIONAL_PARAMETERS" = "1" ]
then
    EN_DSL_PM_OPTIONAL_PARAMETERS="--enable-dsl-pm-optional-parameters"
else
    EN_DSL_PM_OPTIONAL_PARAMETERS=""
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
if [ "$CONF_KERNEL_BUILD" != "" ]
then
    KERNEL_BUILD=$CONF_KERNEL_BUILD
fi

# Additional CFLAGS
if [ "$CONF_DRIVER_CFLAGS" != "" ]
then
    DRIVER_CFLAGS=$CONF_DRIVER_CFLAGS
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
    --enable-driver-include="$DRIVER_INCLUDE" \
    --enable-kernel-build="$KERNEL_BUILD" \
    --build=i386-linux \
    --enable-add-drv-cflags="$DRIVER_CFLAGS" \
    --enable-$DEVICE \
    --with-channels-per-line=$CHANNELS_PER_LINE \
    $EN_KERNEL_26 \
    $EN_DEBUG \
    $EN_DEBUG_PRINTS \
    $EN_ADSL_MIB \
    $EN_ADSL_LED \
    $EN_ADSL_TRACE \
    $EN_DSL_CEOC \
    $EN_DSL_STATIC_DELT_DATA \
    $EN_DSL_PM \
    $EN_DSL_PM_TOTAL_COUNTERS \
    $EN_DSL_PM_HISTORY \
    $EN_DSL_PM_SHOWTIME_COUNTERS \
    $EN_DSL_PM_LINE_COUNTERS \
    $EN_DSL_PM_CHANNEL_COUNTERS \
    $EN_DSL_PM_DATAPATH_COUNTERS \
    $EN_DSL_PM_CHANNEL_THRESHOLDS \
    $EN_DSL_PM_DATAPATH_THRESHOLDS \
    $EN_DSL_PM_LINE_THRESHOLDS \
    $EN_DSL_PM_OPTIONAL_PARAMETERS

check_return

echo "build $DEVICE"
make -s
check_return

cd $MYHOME

exit 0
